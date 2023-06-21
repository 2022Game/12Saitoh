#include "CEnemy3.h"
#include "CEffect.h"
#include "CApplication.h"
#include "CPlayer.h"
#define _USE_MATH_DEFINES
#include <math.h>

#define HP 3               //耐久値
#define VELOCITY 0.1f      //速度
#define OBJ "res\\SnowGolem.obj" //モデルのファイル
#define MTL "res\\SnowGolem.mtl" //モデルのマテリアルファイル
#define GRAVITY CVector(0.0f, 0.1f, 0.0f)	//重力
#define FOV_ANGLE 45.0f	//視野の角度 (-角度～+角度まで)
#define FOV_LENGTH 30.0f	//視野の距離

float CEnemy3::mDotX = 0.0f;
CModel CEnemy3::sModel;    //モデルデータ作成

//プレイヤーを見つけたかどうか
bool CEnemy3::IsFoundPlayer() const
{
	CVector playerPos = CPlayer::Instance()->Position();
	CVector enemyPos = Position();

	//視野の角度の判定
	//自身からプレイヤーまでのベクトルを取得(要正規化)
	CVector EP = (playerPos - enemyPos).Normalize();
	//自身の正面方向のベクトルを取得(要正規化)
	CVector forward = MatrixRotate().VectorZ().Normalize();
	//自身の横方向のベクトルを取得
	CVector side = MatrixRotate().VectorX().Normalize();
	//正面方向のベクトルとプレイヤーまでのベクトルの
	//内積から角度を求める
	float dotZ = forward.Dot(EP);
	//横方向のベクトルの内積から角度を求める
	float dotX = side.Dot(EP);
	mDotX = dotX;
	//求めた角度が視野角度外で有れば、falseを返す
	if (dotZ <= cosf(FOV_ANGLE * M_PI / 180.0f)) return false;

	//視野距離の判定
	//自身からプレイヤーまでの距離を求める
	float distance = (playerPos - enemyPos).Length();
	//求めた距離が視野距離より遠いならば、falseを返す
	if (distance > FOV_LENGTH)return false;

	//視野角度と視野距離を通ったのでtrueを返す
	return true;
}

//デフォルトコンストラクタ
CEnemy3::CEnemy3()
	: CCharacter3(1)
	, mCollider1(this, &mMatrix, CVector(0.0f, 70.0f, 0.0f), 0.4f, (int)EColliderTag::EENEMY)
	, mCollider2(this, &mMatrix, CVector(0.0f, 45.0f, 0.0f), 0.36f, (int)EColliderTag::EENEMY)
	, mCollider3(this, &mMatrix, CVector(0.0f, 13.0f, 0.0f), 0.45f, (int)EColliderTag::EENEMY)
	, mHp(HP)
	, mBulletTime(0)
	, mFlag(false)
	, mState(EState::EIDLE)
{
	//モデルが無い時は読み込む
	if (sModel.Triangles().size() == 0)
	{
		sModel.Load(OBJ, MTL);
	}
	//モデルデータのポインタ設定
	mpModel = &sModel;
}

//コンストラクタ
//CEnemy3(位置,回転,拡縮)
CEnemy3::CEnemy3(const CVector& position, const CVector& rotation,
	const CVector& scale)
	:CEnemy3() //デフォルトコンストラクタを実行する
{
	//位置,回転,拡縮を設定する
	mPosition = position; //位置の設定
	mRotation = rotation; //回転の設定
	mScale = scale;       //拡縮の設定
	CTransform::Update(); //行列の更新
}

//待機状態の更新処理
void CEnemy3::UpdateIdle()
{
	//プレイヤーを見つけたら、追跡状態げ移行
	if (IsFoundPlayer())
	{
		mState = EState::ECHASE;
	}
}

//追跡状態の更新処理
void CEnemy3::UpdateChase()
{
	//プレイヤーを見失ったら、待機状態へ移行
	if (!IsFoundPlayer())
	{
		mState = EState::EIDLE;
	}
	//プレイヤーを見失っていなければ、移動処理
	else
	{
		CVector forward = mMatrixRotate.VectorZ().Normalize();
		CVector playerPos = CPlayer::Instance()->Position();
		float distance = (playerPos - Position()).Length();
		if (distance >= 5.0f)
		{
			mPosition += forward * VELOCITY;
		}
		else if (distance <= 3.0f)
		{
			mPosition -= forward * VELOCITY;
		}
		//距離が3～5の間であれば、攻撃状態へ移行
		else
		{
			mState = EState::EATTACK;
		}
		//左右方向へ回転
		if (mDotX >= 0.0f)
		{
			mRotation += CVector(0.0f, 1.5f, 0.0f); //左へ回転
		}
		else
		{
			mRotation -= CVector(0.0f, -1.5f, 0.0f); //右へ回転
		}
	}
}

//攻撃状態の更新処理
void CEnemy3::UpdateAttack()
{
	CVector playerPos = CPlayer::Instance()->Position();
	float distance = (playerPos - Position()).Length();

	//攻撃処理
	//雪玉を発射していなければ
	if (mFlag == false)
	{
		//弾を発射
		CBullet* bullet = new CBullet();
		bullet->Set(0.1f, 1.5f);
		bullet->Position(CVector(0.0f, 70.0f, 10.0f) * mMatrix);
		bullet->Rotation(mRotation);
		bullet->Update();
		mFlag = true;
		printf("弾生成\n");
	}
	//雪弾発射済み
	else
	{
		mBulletTime++;
		if (mBulletTime >= 100)
		{
			if (mFlag == true)
			{
				mFlag = false;
				mBulletTime = 0;
				//攻撃が終わったら追跡状態に戻す
				mState = EState::ECHASE;
			}
		}
	}

}

//更新処理
void CEnemy3::Update()
{
	//重力
	mPosition = mPosition - GRAVITY;

	switch (mState)
	{
	case EState::EIDLE:
		UpdateIdle();
		break;
	case EState::ECHASE:
		UpdateChase();
		break;
	case EState::EATTACK:
		UpdateAttack();
		break;
	}


	//HPが0以下の時、撃破
	if (mHp <= 0)
	{
		mHp--;
		//15フレーム毎にエフェクト
		if (mHp % 15 == 0)
		{
			//エフェクト生成
			new CEffect(mPosition, 1.0f, 1.0f, "exp.tga", 4, 4, 2);
		}
		//降下させる
		mPosition = mPosition - CVector(0.0f, 0.03f, 0.0f);
		CTransform::Update();
		return;
	}

	CTransform::Update(); //行列更新
}


void CEnemy3::Collision()
{
	//コライダの優先度変更
	mCollider1.ChangePriority();
	mCollider2.ChangePriority();
	mCollider3.ChangePriority();
	//衝突処理を実行
	CCollisionManager::Instance()->Collision(&mCollider1, COLLISIONRANGE);
	CCollisionManager::Instance()->Collision(&mCollider2, COLLISIONRANGE);
	CCollisionManager::Instance()->Collision(&mCollider3, COLLISIONRANGE);
}

//衝突処理
//Collision(コライダ1,コライダ2)
void CEnemy3::Collision(CCollider* m, CCollider* o)
{
	//相手のコライダタイプの判定
	switch (o->Type())
	{
	case CCollider::ESPHERE: //球コライダの時
		//コライダのmとyが衝突しているか判定
		if (CCollider::Collision(m, o))
		{
			if (o->GetTag() != (int)EColliderTag::EENEMY &&
				o->GetTag() != (int)EColliderTag::EALL)
			{
				mHp--; //ヒットポイントの減算
				//エフェクト生成
				new CEffect(o->Parent()->Position(), 1.0f, 1.0f, "exp.tga", 4, 4, 2);
			}
		}
		break;
	case CCollider::ETRIANGLE: //三角コライダの時
		CVector adjust; //調整値
		//三角コライダと球コライダの衝突判定
		if (CCollider::CollisionTriangleSphere(o, m, &adjust))
		{
			//衝突していない位置まで戻す
			mPosition = mPosition + adjust;
			//撃破で地面に衝突すると無効
			if (mHp <= 0)
			{
				mEnabled = false;
			}
		}
		break;
	}
}
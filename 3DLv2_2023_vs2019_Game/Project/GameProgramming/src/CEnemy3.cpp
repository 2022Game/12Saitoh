#include "CEnemy3.h"
#include "CEffect.h"
#include "CApplication.h"
#include "CPlayer.h"

#define HP 3               //耐久値
#define VELOCITY 0.1f      //速度
#define OBJ "res\\SnowGolem.obj" //モデルのファイル
#define MTL "res\\SnowGolem.mtl" //モデルのマテリアルファイル
#define GRAVITY CVector(0.0f, 0.1f, 0.0f)	//重力


CModel CEnemy3::sModel;    //モデルデータ作成

//デフォルトコンストラクタ
CEnemy3::CEnemy3()
	: CCharacter3(1)
	, mCollider1(this, &mMatrix, CVector(0.0f, 70.0f, 0.0f), 0.4f, (int)EColliderTag::EENEMY)
	, mCollider2(this, &mMatrix, CVector(0.0f, 45.0f, 0.0f), 0.36f, (int)EColliderTag::EENEMY)
	, mCollider3(this, &mMatrix, CVector(0.0f, 13.0f, 0.0f), 0.45f, (int)EColliderTag::EENEMY)
	, mHp(HP)
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
	//目標地点の設定
	mPoint = mPosition + CVector(0.0f, 0.0f, 100.0f) * mMatrixRotate;
}

//更新処理
void CEnemy3::Update()
{
	mPosition = mPosition - GRAVITY;
	//プレイヤーのポインタが0以外の時
	CPlayer* player = CPlayer::Instance();
	if (player != nullptr)
	{
		//目標地点までのベクトルを求める
		CVector vp = mPoint - mPosition;
		//プレイヤーまでのベクトルを求める
		//CVector vp = player->Position() - mPosition;
		//左ベクトルとの内積を求める
		float dx = vp.Dot(mMatrixRotate.VectorX());
		//上ベクトルとの内積を求める
		float dy = vp.Dot(mMatrixRotate.VectorY());
		//前方向ベクトルの内積を求める
		float dz = vp.Dot(mMatrixRotate.VectorZ());

		const float margin = 0.1f;

		//X軸のズレが2.0以下
		if (-2.0f < dx && dx < 2.0f)
		{
			//Y軸のズレ2.0以下
			if (-2.0f < dy && dy < 2.0f)
			{
				//プレイヤーが前方かつ距離が30以内
				if (0.0f < dz && dz < 30.0f)
				{
					//弾を発射します
					//CBullet* bullet = new CBullet();
					//bullet->Set(0.1f, 1.5f);
					//bullet->Position(CVector(0.0f, 0.0f, 10.0f) * mMatrix);
					//bullet->Rotation(mRotation);
					//bullet->Update();
				}
			}
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
		//およそ3秒毎に目標地点を更新
		int r = rand() % 180; //rand()は整数の乱数を返す
		                      //% 180は180で割った余りを求める
		if (r == 0)
		{
			if (player != nullptr)
			{
				mPoint = player->Position();
			}
			else
			{
				mPoint = mPoint * CMatrix().RotateY(45);
			}
		}
		//左右方向へ回転
		if (dx > margin)
		{
			mRotation = mRotation + CVector(0.0f, 1.0f, 0.0f); //左へ回転
		}
		else if (dx < -margin)
		{
			mRotation = mRotation + CVector(0.0f, -1.0f, 0.0f); //右へ回転
		}
		//機体前方へ移動する
		mPosition = mPosition + mMatrixRotate.VectorZ() * VELOCITY;
		CTransform::Update(); //行列更新
	}
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
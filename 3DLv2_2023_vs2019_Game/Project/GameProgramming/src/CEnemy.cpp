#include "CEnemy.h"
#include "CEffect.h"
#include "CApplication.h"
#include "CPlayer.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "Math.h"
#include <time.h>
#include "Primitive.h"
#include "NavManager.h"
#include "Global.h"
#include "CCollisionManager.h"
#include "CEnemyBullet.h"

#define HP 3               //耐久値
#define VELOCITY 0.1f      //速度
#define OBJ "res\\SnowGolem.obj" //モデルのファイル
#define MTL "res\\SnowGolem.mtl" //モデルのマテリアルファイル
#define GRAVITY CVector(0.0f, 0.1f, 0.0f)	//重力
#define MOVE_SPEED 0.125f		//移動速度
#define FOV_ANGLE 80.0f			//視野の角度 (-角度～+角度まで)
#define FOV_ATTACKANGLE 5.0f	//攻撃範囲(角度)
#define FOV_LENGTH 18.0f		//視野の距離
#define FOV_LANGE 10.0f			//認識範囲
#define FOV_ATTACKLENGTH 15.0f	//攻撃範囲(距離)

CModel CEnemy::sModel;	//モデルデータ作成
int CEnemy::sNum = 0;	//初期化
CEnemy* CEnemy::spInstance = nullptr;

//プレイヤーを見つけたかどうか
bool CEnemy::IsFoundPlayer() const
{
	CVector playerPos = CPlayer::Instance()->Position();
	CVector enemyPos = Position();

	//視野の角度の判定
	//自身からプレイヤーまでのベクトルを取得(要正規化)
	CVector EP = (playerPos - enemyPos).Normalize();
	//自身の正面方向のベクトルを取得(要正規化)
	CVector forward = MatrixRotate().VectorZ().Normalize();
	//正面方向のベクトルとプレイヤーまでのベクトルの
	//内積から角度を求める
	float dotZ = forward.Dot(EP);
	//自身からプレイヤーまでの距離を求める
	float distance = (playerPos - enemyPos).Length();
	//一定の距離に入ったらtrueを返す
	if (distance < FOV_LANGE)return true;

	//求めた角度が視野角度外で有れば、falseを返す
	if (dotZ <= cosf(FOV_ANGLE * M_PI / 180.0f)) return false;

	//視野距離の判定
	//求めた距離が視野距離より遠いならば、falseを返す
	if (distance > FOV_LENGTH)return false;

	//視野角度と視野距離を通ったのでtrueを返す
	return true;
}

//攻撃できる範囲内かの判定
bool CEnemy::AttackRange() const
{
	CVector playerPos = CPlayer::Instance()->Position();
	CVector enemyPos = CEnemy::Position();

	//攻撃範囲(角度)の判定
	//自身からプレイヤーまでのベクトルを取得
	CVector EP = (playerPos - enemyPos).Normalize();
	//自身から正面方向のベクトルを取得
	CVector forward = MatrixRotate().VectorZ().Normalize();
	//正面方向のベクトルとプレイヤーまでの
	//ベクトルの内積から角度を求める
	float dotZ = forward.Dot(EP);
	//求めた角度が視野範囲外だったらfalseを返す
	if (dotZ <= cosf(FOV_ATTACKANGLE * M_PI / 180.0f)) return false;

	//攻撃範囲(距離)の判定
	float distance = (playerPos - enemyPos).Length();
	//求めた距離が攻撃範囲より遠いならfalseを返す
	if (distance > FOV_ATTACKLENGTH) return false;

	//攻撃範囲内なのでtrueを返す
	return true;
}

//指定座標まで移動
bool CEnemy::MoveTo(const CVector& target)
{
	//目的地までの距離が移動速度より大きいならば
	float distance = (target - Position()).Length();
	if (distance > MOVE_SPEED)
	{
		//目的地方向へ移動
		CVector moveVec = (target - Position()).Normalize();
		Position(Position() + moveVec * MOVE_SPEED);

		//目的地方向へ向く
		float currentAngle = mRotation.Y();
		float targetAngle = atan2f(moveVec.X(), moveVec.Z());
		targetAngle = Math::RadianToDegree(targetAngle);

		//向きを回転する時に左右で近い方から回転する
		float diff = targetAngle - currentAngle;
		if (diff > 180.0f)
		{
			targetAngle -= 360.0f;
		}
		else if (diff < -180.0f)
		{
			currentAngle -= 360.0f;
		}

		float angle = Math::Lerp(currentAngle, targetAngle, 0.1f);
		mRotation.Y(angle);
	}
	//距離が移動速度より短いのであれば、
	else
	{
		//目的地の座標に配置
		Position(target);
		//移動終了
		return true;
	}

	//移動継続
	return false;
}

//待機状態の更新処理
void CEnemy::UpdateIdle()
{
	//プレイヤーを見つけたら、追跡状態に移行
	if (IsFoundPlayer())
	{
		mState = EState::ECHASE;
	}

	//150フレーム毎に待機中の動作を更新する
	mIdleTime++;
	if (mIdleTime >= 150)
	{
		//ランダム値の取得
		srand((unsigned int)time(NULL));
		//ランダム値を保存する
		//最小値0 ,取得範囲6
		 mRandIdle = 0 + rand() % 6;
	}
	//更新の初期化
	if (mIdleTime == 150)
		mIdleTime = 0;
	switch (mRandIdle)
	{
	case 0:
		//前方向へ移動
		Position(Position() + (CVector(0.0f, 0.0f, 1.0f)
			* MOVE_SPEED) * mMatrixRotate);
		break;
	case 1:
		//右方向へ移動
		Rotation(mRotation - CVector(0.0f, 1.0f, 0.0f));
		Position(Position() + (CVector(0.0f, 0.0f, 1.0f)
			* MOVE_SPEED) * mMatrixRotate);
		break;
	case 2://左方向へ移動
		Rotation(mRotation + CVector(0.0f, 1.0f, 0.0f));
		Position(Position() + (CVector(0.0f, 0.0f, 1.0f)
			* MOVE_SPEED) * mMatrixRotate);
		break;
	case 3://右へ回転
		Rotation(mRotation - CVector(0.0f, 1.0f, 0.0f));
		break;
	case 4://左へ回転
		Rotation(mRotation + CVector(0.0f, 1.0f, 0.0f));
		break;
	case 6://動かない
		break;
	}
}

//追跡状態の更新処理
void CEnemy::UpdateChase()
{
	//プレイヤーのノードを取得
	NavNode* playerNode = CPlayer::Instance()->GetNavNode();
	//自身のノードを取得
	NavNode* enemyNode = mpNode;

	CVector playerNodePos = playerNode->GetPos();
	CVector enemyNodePos = enemyNode->GetPos();
	//プレイヤーまでのベクトル
	CVector vec = playerNodePos - enemyNodePos;
	//プレイヤーまでの距離
	float dist = vec.Length();

	//プレイヤーまでの間に遮蔽物があるか調べる
	float outDist = 9999.0f;
	//レイの当たり判定は目線あたりでとる
	gMap->CollisionRay(enemyNodePos + CVector(0.0f, 0.5f, 0.0f), playerNodePos + CVector(0.0f, 0.5f, 0.0f), &outDist);
	//遮蔽物が存在しない
	if (dist < outDist)
	{
		//自身のノードからプレイヤーのノードまでの
		//経路探索を行う
		mpNextNode = NavManager::Instance()->Navigate(enemyNode, playerNode);
		//プレイヤーを見失った時のノードを更新
		if (mpLostNode == nullptr)
		{
			mpLostNode = new NavNode(playerNodePos);
		}
		else
		{
			mpLostNode->SetPos(playerNodePos);
		}
	}
	//遮蔽物がある
	else
	{
		//現在のプレイヤーノードまでの経路を探索して
		//見失った状態へ移行する
		enemyNode->RemoveConnect(playerNode);
		mpNextNode = NavManager::Instance()->Navigate(enemyNode, mpLostNode);
		mState = EState::ELOST;
		return;
	}
	//次の目的地が存在する
	if (mpNextNode != nullptr)
	{
		//次の目的地まで移動
		MoveTo(mpNextNode->GetPos());
	}
	//次の目的地が存在しない場合は、目的地到着
	else
	{
		//待機状態へ移行
		mState = EState::EIDLE;
	}
	//プレイヤーが範囲内にいれば攻撃する
	if (AttackRange())
	{
		mState = EState::EATTACK;
	}
}

//見失った状態の更新処理
void CEnemy::UpdateLost()
{
	//目的地が存在する
	if (mpNextNode != nullptr)
	{
		//目的地まで移動
		if (MoveTo(mpNextNode->GetPos()))
		{
			if (mpNextNode != mpLostNode)
			{
				mpNextNode = NavManager::Instance()->Navigate(mpNextNode, mpLostNode);
			}
			else
			{
				delete mpLostNode;
				mpLostNode = nullptr;
				mpNextNode = nullptr;
			}
		}
	}
	//目的地まで移動が終われば、
	else
	{
		//待機状態へ移行
		mState = EState::EIDLE;
	}
	//プレイヤーが範囲内にいれば攻撃する
	if (AttackRange())
	{
		mState = EState::EATTACK;
	}
}

//攻撃状態の更新処理
void CEnemy::UpdateAttack()
{
	//攻撃処理
	//雪玉を発射していなければ
	if (mFlag == false)
	{
		//弾を発射
		CEnemyBullet* bullet = new CEnemyBullet(CCollider::EColliderTag::EENEMYBULLET);
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
		if (mBulletTime >= 100 && mFlag)
		{
			mFlag = false;
			mBulletTime = 0;
			//攻撃が終わったら追跡状態に戻す
			mState = EState::EIDLE;
		}
	}

}

//デフォルトコンストラクタ
CEnemy::CEnemy()
	: CCharacter3(1)
	, mState(EState::EIDLE)
	, mCollider1(this, &mMatrix, CVector(0.0f, 70.0f, 0.0f), 1.05, CCollider::EColliderTag::EENEMY)
	, mCollider2(this, &mMatrix, CVector(0.0f, 45.0f, 0.0f), 0.54, CCollider::EColliderTag::EENEMY)
	, mCollider3(this, &mMatrix, CVector(0.0f, 13.0f, 0.0f), 0.675, CCollider::EColliderTag::EENEMY)
	, mpNextNode(nullptr)
	, mpLostNode(nullptr)
	, mHp(HP)
	, mBulletTime(0)
	, mIdleTime(0)
	, mRandIdle(-1)
	, mFlag(false)
{
	//モデルが無い時は読み込む
	if (sModel.Triangles().size() == 0)
	{
		sModel.Load(OBJ, MTL);
	}
	//モデルデータのポインタ設定
	mpModel = &sModel;
	mpNode = new NavNode(Position());
	spInstance = this;
	sNum++;
}

//コンストラクタ
//CEnemy(位置,回転,拡縮)
CEnemy::CEnemy(const CVector& position, const CVector& rotation,
	const CVector& scale)
	:CEnemy() //デフォルトコンストラクタを実行する
{
	//位置,回転,拡縮を設定する
	mPosition = position; //位置の設定
	mRotation = rotation; //回転の設定
	mScale = scale;       //拡縮の設定
	CTransform::Update(); //行列の更新
}

//デストラクタ
CEnemy::~CEnemy()
{
	if (mpNode != nullptr)
	{
		delete mpNode;
		mpNode = nullptr;
	}
}

//インスタンスのポインタの取得
CEnemy* CEnemy::Instance()
{
	return spInstance;
}

//敵の数を初期化
int CEnemy::ResetNum()
{
	return sNum = 0;
}

//敵の数の取得
int CEnemy::Num()const
{
	return sNum;
}

//更新処理
void CEnemy::Update()
{
	//重力
	mPosition = mPosition - GRAVITY;

	switch (mState)
	{
	case EState::EIDLE://待機
		UpdateIdle();
		break;
	case EState::ECHASE://追跡
		UpdateChase();
		break;
	case EState::ELOST://見失った
		UpdateLost();
		break;
	case EState::EATTACK://攻撃
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
	//行列の更新
	CTransform::Update();
	CCharacter3::Update();
}

//描画処理
void CEnemy::Render()
{
	CCharacter3::Render();

	Primitive::DrawSector
	(
		mPosition,
		mRotation,
		-FOV_ANGLE,
		FOV_ANGLE,
		FOV_LENGTH,
		CVector4(1.0f, 1.0f, 0.0f, 1.0f),
		45
	);
	Primitive::DrawSector
	(
		mPosition + CVector (0.0f,0.1f,0.0f),
		mRotation,
		-FOV_ATTACKANGLE,
		FOV_ATTACKANGLE,
		FOV_ATTACKLENGTH,
		CVector4(1.0f, 1.0f, 1.0f, 1.0f),
		45
	);
}


void CEnemy::Collision()
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
void CEnemy::Collision(CCollider* m, CCollider* o)
{
	//相手のコライダタイプの判定
	switch (o->Type())
	{
	case CCollider::ESPHERE: //球コライダの時
		//コライダのmとyが衝突しているか判定
		if (CCollider::Collision(m, o))
		{
			//プレイヤーの弾の時HPを減算する
			if (o->ColliderTag() == CCollider::EColliderTag::EPLAYERBULLET)
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
			//敵の数を1減らす
			if (mHp <= 0)
			{
				mEnabled = false;
				sNum--;
			}
		}
		break;
	}
}

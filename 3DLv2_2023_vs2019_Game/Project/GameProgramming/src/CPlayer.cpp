//プレイヤークラスのインクルード
#include "CPlayer.h"
#include "CApplication.h"
#include "CTaskManager.h"
#include "NavNode.h"
#include "NavManager.h"
#include "CCollisionManager.h"

#define ROTATION_XV CVector(1.0f,0.0f,0.0f) //回転速度
#define ROTATION_YV CVector(0.0f,1.0f,0.0f) //回転速度
#define VELOCITYZ CVector(0.0f, 0.0f, 0.2f) //Z軸移動速度
#define VELOCITYX CVector(0.2f, 0.0f, 0.0f)	//X軸移動速度
#define GRAVITY CVector(0.0f, 0.5f, 0.0f)	//重力

CPlayer* CPlayer::spInstance = nullptr;

CPlayer::CPlayer()
	: mCollider1(this, &mMatrix, CVector(0.0f, 70.0f, 0.0f), 1.05f, CCollider::EColliderTag::EPLAYER)
	, mCollider2(this, &mMatrix, CVector(0.0f, 45.0f, 0.0f), 0.54f, CCollider::EColliderTag::EPLAYER)
	, mCollider3(this, &mMatrix, CVector(0.0f, 13.0f, 0.0f), 0.675f, CCollider::EColliderTag::EPLAYER)
{
	//インスタンスの設定
	spInstance = this;
	//プレイヤーHPの設定
	mHp = 5;
	mpModel = new CModel();
	mpModel->Load("res\\SnowGolem.obj", "res\\SnowGolem.mtl");
	mpNode = new NavNode(Position());
}

//CPlayer(位置,回転,スケール)
CPlayer::CPlayer(const CVector& pos, const CVector& rot, const CVector& scale)
	: mpBullet(nullptr)
{
	CTransform::Update(pos, rot, scale); //行列の更新
}

//デストラクタ
CPlayer::~CPlayer()
{
	if (mpNode != nullptr)
	{
		delete mpNode;
		mpNode = nullptr;
	}
}

//更新処理
void CPlayer::Update()
{
	mPosition = mPosition - GRAVITY;
	//スペースキー入力で弾発射
	if (mInput.PushKey(VK_SPACE))
	{
		mpBullet = new CPlayerBullet(CCollider::EColliderTag::EPLAYERBULLET);
		mpBullet->Set(0.1f, 1.5f);
		mpBullet->Position(CVector(0.0f, 70.0f, 10.0f) * mMatrix);
		mpBullet->Rotation(mRotation);
		mpBullet->Update();
	}
	//Wキー入力で上向き
	if (mInput.Key('W'))
	{
		//Z軸方向の値を回転させ移動させる
		mPosition = mPosition + VELOCITYZ * mMatrixRotate;
	}
	//Sキー入力で上向き
	if (mInput.Key('S'))
	{
		//Z軸方向の値を回転させ移動させる
		mPosition = mPosition - VELOCITYZ * mMatrixRotate;
	}
	if (mInput.Key('A'))
	{
		//X軸方向の値を回転させ移動させる
		mPosition = mPosition + VELOCITYX * mMatrixRotate;
	}
	//Dキー入力で回転
	if (mInput.Key('D'))
	{
		//X軸方向の値を回転させ移動させる
		mPosition = mPosition - VELOCITYX * mMatrixRotate;
	}
	if (mInput.Key(VK_RIGHT))
	{
		//X軸の回転値を加算
		mRotation = mRotation - ROTATION_YV;
	}
	if (mInput.Key(VK_LEFT))
	{
		//X軸の回転値を加算
		mRotation = mRotation + ROTATION_YV;
	}
	if (mInput.Key(VK_UP))
	{
		mRotation = mRotation - ROTATION_XV;
	}
	if (mInput.Key(VK_DOWN))
	{
		mRotation = mRotation + ROTATION_XV;
	}

	//変換行列の更新
	CTransform::Update();
	CCharacter3::Update();
}

void CPlayer::Collision(CCollider* m, CCollider* o)
{
	CVector adjust; //調整値
	//相手のコライダタイプの判定
	switch (o->Type())
	{
	case CCollider::ESPHERE: //球コライダの時
		//敵の弾だった時
		if (o->ColliderTag() == CCollider::EColliderTag::EENEMYBULLET)
		{
			//衝突処理
			if (CCollider::Collision(m, o))
			{
				mHp--;
				printf("Hit\n");
			}
		}
		break;
	case CCollider::ETRIANGLE: //三角コライダの時

		//三角コライダと球コライダの衝突判定
		if (CCollider::CollisionTriangleSphere(o, m, &adjust))
		{
			//衝突していない位置まで戻す
			mPosition = mPosition + adjust;
		}
		break;
	}
}

//衝突処理
void CPlayer::Collision()
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

CPlayer* CPlayer::Instance()
{
	return spInstance;
}

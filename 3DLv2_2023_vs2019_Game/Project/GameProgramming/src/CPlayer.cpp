//プレイヤークラスのインクルード
#include "CPlayer.h"
#include"CApplication.h"
#include"CTaskManager.h"

#define ROTATION_XV CVector(1.0f,0.0f,0.0f) //回転速度
#define ROTATION_YV CVector(0.0f,1.0f,0.0f) //回転速度
#define VELOCITYZ CVector(0.0f, 0.0f, 0.2f) //Z軸移動速度
#define VELOCITYX CVector(0.2f, 0.0f, 0.0f)	//X軸移動速度
#define GRAVITY CVector(0.0f, 0.1f, 0.0f)	//重力

CPlayer* CPlayer::spInstance = nullptr;
int CPlayer::sHp = 0;

CPlayer::CPlayer()
	: mCollider1(this, &mMatrix, CVector(0.0f, 70.0f, 0.0f), 0.7f, CCollider::EColliderTag::EPLAYER)
	, mCollider2(this, &mMatrix, CVector(0.0f, 45.0f, 0.0f), 0.36f, CCollider::EColliderTag::EPLAYER)
	, mCollider3(this, &mMatrix, CVector(0.0f, 13.0f, 0.0f), 0.45f, CCollider::EColliderTag::EPLAYER)

{
	//インスタンスの設定
	spInstance = this;
	//プレイヤーHPの設定
	sHp = 5;
}

//CPlayer(位置,回転,スケール)
CPlayer::CPlayer(const CVector& pos, const CVector& rot, const CVector& scale)
{
	CTransform::Update(pos, rot, scale); //行列の更新
}

//更新処理
void CPlayer::Update()
{
	mPosition = mPosition - GRAVITY;
	//スペースキー入力で弾発射
	if (mInput.PushKey(VK_SPACE))
	{
		CBullet *bullet = new CBullet();
		bullet->Set(0.1f, 1.5f);
		bullet->Position(CVector(0.0f, 70.0f, 10.0f) * mMatrix);
		bullet->Rotation(mRotation);
		bullet->Update();
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
	//カメラが正面に来た時
	if (CApplication::CameraFlag() != 2)
	{
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
	}
	else
	{
		if (mInput.Key('A'))
		{
			//X軸方向の値を回転させ移動させる
			mPosition = mPosition - VELOCITYX * mMatrixRotate;
		}
		//Dキー入力で回転
		if (mInput.Key('D'))
		{
			//X軸方向の値を回転させ移動させる
			mPosition = mPosition + VELOCITYX * mMatrixRotate;
		}
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
}

void CPlayer::Collision(CCollider* m, CCollider* o)
{
	CVector adjust; //調整値
	//相手のコライダタイプの判定
	switch (o->Type())
	{
	case CCollider::ESPHERE: //球コライダの時
		//コライダが雪玉か判定
		if (o->ColliderTag() == CCollider::EColliderTag::EBULLET)
		{
			//衝突処理
			if (CCollider::Collision(m, o))
			{
				sHp--;
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

int CPlayer::HP()
{
	return sHp;
}
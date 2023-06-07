#include "CAlly.h"
#include "CEffect.h"
#include "CApplication.h"
#include "CPlayer.h"

#define HP 3						//耐久値
#define VELOCITY 0.1f				//速度
#define OBJ "res\\SnowGolem.obj"	//モデルのファイル
#define MTL "res\\SnowGolem.mtl"	//モデルのマテリアルファイル

CModel CAlly::sModel;				//モデルデータ作成

//デフォルトコンストラクタ
CAlly::CAlly()
	:CCharacter3(1)
	, mCollider(this, &mMatrix, CVector(0.0f, 70.0f, 0.0f), 0.4f)
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
//CAlly(位置,回転,拡縮)
CAlly::CAlly(const CVector& position, const CVector& rotation,
	const CVector& scale)
	:CAlly() //デフォルトコンストラクタを実行する
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
void CAlly::Update()
{
	//プレイヤーのポインタが0以外の時
	CPlayer* player = CPlayer::Instance();
	if (player != nullptr)
	{
		//目標地点までのベクトルを求める
		CVector vp = mPoint - mPosition;
		//プレイヤーまでのベクトルを求める
		//左ベクトルとの内積を求める
		float dx = vp.Dot(mMatrixRotate.VectorX());
		//上ベクトルとの内積を求める
		float dy = vp.Dot(mMatrixRotate.VectorY());
		//前方向ベクトルの内積を求める
		float dz = vp.Dot(mMatrixRotate.VectorZ());

		const float margin = 0.1f;

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


void CAlly::Collision()
{
	//コライダの優先度変更
	mCollider.ChangePriority();
	//衝突処理を実行
	CCollisionManager::Instance()->Collision(&mCollider, COLLISIONRANGE);
}

//衝突処理
//Collision(コライダ1,コライダ2)
void CAlly::Collision(CCollider* m, CCollider* o)
{
	//相手のコライダタイプの判定
	switch (o->Type())
	{
	case CCollider::ESPHERE: //球コライダの時
		//コライダのmとyが衝突しているか判定
		if (CCollider::Collision(m, o))
		{
			mHp--; //ヒットポイントの減算
			//エフェクト生成
			new CEffect(o->Parent()->Position(), 1.0f, 1.0f, "exp.tga", 4, 4, 2);
			//衝突している時は無効にする
			//mEnabled = false;
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
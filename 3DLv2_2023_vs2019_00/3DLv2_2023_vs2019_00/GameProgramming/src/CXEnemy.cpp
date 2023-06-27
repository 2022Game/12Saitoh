#include "CXEnemy.h"

//コンストラクタ
CXEnemy::CXEnemy()
	: mColSpherHead(this, nullptr, CVector(0.0f, 5.0f, -3.0f), 0.5f)
	, mColSpherBody(this, nullptr, CVector(), 0.5f)
	, mColSpherSword(this, nullptr, CVector(-10.0f, 10.0f, 50.0f), 0.3f)
{

}

void CXEnemy::Init(CModelX* model)
{
	CXCharacter::Init(model);
	//合成行列の設定
	//頭
	mColSpherHead.Matrix(&mpCombinedMatrix[11]);
	//体
	mColSpherBody.Matrix(&mpCombinedMatrix[8]);
	//剣
	mColSpherSword.Matrix(&mpCombinedMatrix[21]);
}

void CXEnemy::Update()
{
	CXCharacter::Update();
}

void CXEnemy::Collision(CCollider* m, CCollider* o)
{
	//自分のコライダタイプ
	switch (m->Type())
	{
	case CCollider::ESPHERE:	//球コライダ
		/*
		条件
		相手のコライダーが球コライダ
		コライダーの親がプレイヤー
		コライダーが剣先
		自分のコライダが体
		*/
		if (o->Type() == CCollider::ESPHERE &&
			o->Parent()->Tag() == ETag::EPLAYER &&
			o->Tag() == CCollider::ETag::ESWORD &&
			m->Tag() == CCollider::ETag::EBODY)
		{
			//衝突処理
			if (CCollider::Collision(m, o))
			{
				//倒れ込むアニメーション
				CXCharacter::ChangeAnimation(11, false, 60);
			}
		}
	}
}
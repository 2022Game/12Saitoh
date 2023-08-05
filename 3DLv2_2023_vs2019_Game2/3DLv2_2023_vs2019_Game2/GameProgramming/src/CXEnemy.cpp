#include "CXEnemy.h"

//コンストラクタ
CXEnemy::CXEnemy()
	: mColSpherHead(this, nullptr, CVector(0.0f, 1.0f, 0.0f), 1.5f)
	, mColSpherBody(this, nullptr, CVector(0.5f, -1.0f, 0.0f), 1.0f)
	, mColSpherSword0(this, nullptr, CVector(0.7f, 3.5f, -0.2f), 0.5f)
	, mColSpherSword1(this, nullptr, CVector(0.5f, 2.5f, -0.2f), 0.5f)
	, mColSpherSword2(this, nullptr, CVector(0.3f, 1.5f, -0.2f), 0.5f)
{

}

void CXEnemy::Init(CModelX* model)
{
	CXCharacter::Init(model);
	//合成行列の設定
	//頭
	mColSpherHead.Matrix(&mpCombinedMatrix[1]);
	//体
	mColSpherBody.Matrix(&mpCombinedMatrix[1]);
	//剣
	mColSpherSword0.Matrix(&mpCombinedMatrix[26]);
	mColSpherSword1.Matrix(&mpCombinedMatrix[26]);
	mColSpherSword2.Matrix(&mpCombinedMatrix[26]);

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
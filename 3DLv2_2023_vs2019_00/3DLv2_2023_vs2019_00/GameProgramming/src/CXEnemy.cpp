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
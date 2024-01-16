#include "CEnemy.h"

void CEnemy::Update_Move()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);

	mMoveSpeed += CVector(0.0f, 0.0f, 1.0f);
}
#include "CEnemy.h"
#include "CPlayer.h"

void CEnemy::Update_Move()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);

	if (IsFoundPlayer())
	{
		CVector playerPos = CPlayer::Instance()->Position();
		CVector enemyPos = Position();
		// ���g����v���C���[�܂ł̃x�N�g�����擾
		CVector EP = (playerPos - enemyPos).Normalized();

		// �v���C���[�̏ꏊ�܂ňړ�����
		mMoveSpeed += EP * 0.5f;
	}
	else
	{
		mState = EState::eIdle;
		ChangeAnimation(EAnimType::eIdle);
	}
}
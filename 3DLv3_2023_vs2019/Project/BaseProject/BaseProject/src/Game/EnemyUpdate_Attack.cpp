#include "CEnemy.h"

void CEnemy::AttackPattern1()
{
	switch (mAttackStep)
	{
	case 0: mAttackStep++; break;
	case 1:
		if (AnimationIndex() != (int)EAnimType::eAttack_Bite)
		{
			ChangeAnimation(EAnimType::eAttack_Bite);
		}
		// ���݂��U�����I�������玟�̍U���ֈڍs����
		if (IsAnimationFinished())
		{
			ChangeAnimation(EAnimType::eAttack_Scratching);
			mAttackStep++;
		}
		break;
	case 2:
		// �U�����I��������ҋ@��Ԃֈڍs����
		if (IsAnimationFinished())
		{
			ChangeAnimation(EAnimType::eIdle);
			mState = EState::eIdle;
			mAttackStep = 0;
		}
		break;
	}
}

void CEnemy::Update_Attack()
{
	// �������g���čU���p�^�[����ύX(�\��)
	AttackPattern1();
}
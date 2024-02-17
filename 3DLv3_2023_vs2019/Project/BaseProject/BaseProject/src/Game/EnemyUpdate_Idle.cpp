#include "CEnemy.h"
#include "CPlayer.h"
#include "Time.h"
#include "Maths.h"

void CEnemy::Update_Idle()
{
	CVector playerPos = CPlayer::Instance()->Position();
	CVector enemyPos = Position();
	float distance = (playerPos - enemyPos).Length();
	// �G���v���C���[���������ꍇ�A�퓬��ԂɈڍs����
	if (IsFoundPlayer() && distance > STOP_LENGE)
	{
		mState = EState::eMove;
		ChangeAnimation(EAnimType::eWalk);
		return;
	}

	if (IsAnimationFinished() && AnimationIndex() != (int)EAnimType::eIdle)
	{
		ChangeAnimation(EAnimType::eIdle);
	}

	// 20�b���ƂɃ����_���l�𐶐�
	mElapsedTime += Time::DeltaTime();
	if (mElapsedTime >= 20.0f)
	{
		int rand = Math::Rand(0.0f, 2.0f);
		printf("�G���� : %d\n", rand);
		// �o�ߎ��Ԃ�������
		mElapsedTime = 0.0f;

		// �������������ɉ����ăA�j���[�V������ύX
		switch (rand)
		{
		case 0:
			ChangeAnimation(EAnimType::eIdle);
		case 1:
			ChangeAnimation(EAnimType::eIdle2);
			break;
		case 2:
			ChangeAnimation(EAnimType::eIdle3);
			break;
		}
	}
}
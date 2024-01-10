#include "CEnemy.h"
#include "CPlayer.h"
#include "Time.h"
#include "Maths.h"

void CEnemy::Update_Idle()
{
	if (IsAnimationFinished() &&AnimationIndex()!= (int)EAnimType::eIdle)
	{
		ChangeAnimation(EAnimType::eIdle);
	}

	mElapsedTime += Time::DeltaTime();

	if (mElapsedTime >= 20.0f)
	{
		float rand = Math::Rand(0.0f, 10.0f);
		printf("%.2f\n", rand);
		mElapsedTime = 0.0f;
	}
}
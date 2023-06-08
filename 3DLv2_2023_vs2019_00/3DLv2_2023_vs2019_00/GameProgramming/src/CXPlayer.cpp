#include "CXPlayer.h"

void CXPlayer::Update()
{
	//移動処理
	//攻撃モーション中は移動や回転ができないようにする
	if (mInput.Key('W') &&
		CXCharacter::AnimationIndex() != 3 &&
		CXCharacter::AnimationIndex() != 4)
	{
		CXCharacter::ChangeAnimation(1, true, 60);
		mPosition = mPosition + CVector(0.0f, 0.0f, 0.1f) * mMatrixRotate;
	}
	//待機モーション
	else if (CXCharacter::AnimationIndex() != 3 && 
		CXCharacter::AnimationIndex() != 4)
	{
		CXCharacter::ChangeAnimation(0, true, 60);
	}
	if (mInput.Key('A') &&
		CXCharacter::AnimationIndex() != 3 &&
		CXCharacter::AnimationIndex() != 4)
	{
		mRotation = mRotation + CVector(0.0f, 2.0f, 0.0f);
	}
	if (mInput.Key('D') &&
		CXCharacter::AnimationIndex() != 3 &&
		CXCharacter::AnimationIndex() != 4)
	{
		mRotation = mRotation - CVector(0.0f, 2.0f, 0.0f);
	}
	//攻撃処理
	if (mInput.Key(VK_SPACE) &&
		CXCharacter::AnimationIndex() != 3 &&
		CXCharacter::AnimationIndex() != 4)
	{
		CXCharacter::ChangeAnimation(3, false, 30);
	}
	if (CXCharacter::AnimationIndex() == 3 && 
		CXCharacter::IsAnimationFinished() == true)
	{
		CXCharacter::ChangeAnimation(4, false, 30);
	}
	//攻撃モーションが終わったら待機モーションにする
	if (CXCharacter::AnimationIndex() == 4 &&
		CXCharacter::IsAnimationFinished() == true)
	{
		CXCharacter::ChangeAnimation(0, true, 60);
	}

	CXCharacter::Update();
}
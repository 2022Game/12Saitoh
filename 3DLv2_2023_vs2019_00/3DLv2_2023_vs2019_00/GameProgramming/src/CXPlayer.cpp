#include "CXPlayer.h"

void CXPlayer::Update()
{
	if (mInput.Key('W'))
	{
		CXCharacter::ChangeAnimation(1, true, 60);
		mPosition = mPosition + CVector(0.0f, 0.0f, 0.1f) * mMatrixRotate;
	}
	else
	{
		CXCharacter::ChangeAnimation(0, true, 60);
	}
	if (mInput.Key('A'))
	{
		mRotation = mRotation + CVector(0.0f, 2.0f, 0.0f);
	}
	if (mInput.Key('D'))
	{
		mRotation = mRotation - CVector(0.0f, 2.0f, 0.0f);
	}
	CXCharacter::Update();
}
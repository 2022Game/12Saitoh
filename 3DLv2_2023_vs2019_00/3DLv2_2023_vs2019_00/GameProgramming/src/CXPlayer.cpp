#include "CXPlayer.h"

void CXPlayer::UpdateCamera()
{
	//カメラ䛾前方
	CVector cameraZ = CActionCamera::Instance()->VectorZ();
	//カメラ䛾左方向
	CVector cameraX = CActionCamera::Instance()->VectorX();
	//キャラクタ䛾前方
	CVector charZ = mMatrixRotate.VectorZ();
	//XZ平面にして正規化
	cameraZ.Y(0.0f); cameraZ = cameraZ.Normalize();
	cameraX.Y(0.0f); cameraX = cameraX.Normalize();
	charZ.Y(0.0f); charZ = charZ.Normalize();
	//移動方向䛾設定
	CVector move;
	if (mInput.Key('A')) {
		move = move + cameraX;
	}
	if (mInput.Key('D')) {
		move = move - cameraX;
	}
	if (mInput.Key('W')) {
		move = move + cameraZ;
	}
	if (mInput.Key('S')) {
		move = move - cameraZ;
	}
}

CXPlayer::CXPlayer()
	: mColSpherHead(this, nullptr, CVector(0.0f, 5.0f, -3.0f), 0.5f)
	, mColSpherBody(this, nullptr, CVector(), 0.5f)
	, mColSpherSword(this, nullptr, CVector(-10.0f, 10.0f, 50.0f), 0.3f, CCollider::ETag::ESWORD)
{
	mTag = ETag::EPLAYER;
}

void CXPlayer::Init(CModelX* model)
{
	CXCharacter::Init(model);
	//合成行列の設定
	//頭
	mColSpherHead.Matrix(&mpCombinedMatrix[12]);
	//体
	mColSpherBody.Matrix(&mpCombinedMatrix[9]);
	//剣
	mColSpherSword.Matrix(&mpCombinedMatrix[22]);
}

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
	UpdateCamera();
	CXCharacter::Update();
}
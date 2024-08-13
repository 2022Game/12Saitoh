#include "CLightingButton.h"
#include "CFont.h"
#include "Easing.h"
#include "Maths.h"
#include "CImage.h"

#define EXPAND_ANIM_TIME 0.25f

CLightingButton::CLightingButton(const CVector2& pos, const CVector2& size,
	ETaskPriority prio, int sortOrder,
	ETaskPauseType pause, bool dontDelete, bool addTaskList)
	: CButton(pos, size, prio, sortOrder, pause, dontDelete, addTaskList)
	, mEnterScale(1.5f)
	, mPressScale(1.25f)
	, mStartScale(1.0f)
	, mEndScale(1.0f)
	, mElapsedTime(0.0f)
	, mIsLighting(false)
{
	mBaseSize = mSize;
	mCenter = mBaseSize * 0.5f;
	mpLighting = new CImage
	(
		IMAGE,
		ETaskPriority::eUI,
		0,
		ETaskPauseType::eDefault,
		false,
		false
	);

	mpLighting->SetSize(mBaseSize*1.2);
	// ‰æ‘œ‚ğ•\¦‚·‚éêŠ‚ğİ’è
	// •¶š‚Ì’†‰›‚É•`‰æ
	CVector2 lighingSize = mpLighting->GetSize();
	mpLighting->SetPos
	(
		pos.X() - lighingSize.X() / 2,
		pos.Y() - lighingSize.Y() / 2
	);
}

CLightingButton::~CLightingButton()
{
	SAFE_DELETE(mpLighting);
}

void CLightingButton::OnPointerEnter(const CVector2& pos)
{
	CButton::OnPointerEnter(pos);
	mIsLighting = true;
}

void CLightingButton::OnPointerExit(const CVector2& pos)
{
	CButton::OnPointerExit(pos);
	mIsLighting = false;
}

void CLightingButton::OnPointerDown(const CVector2& pos)
{
	CButton::OnPointerDown(pos);
}

void CLightingButton::OnPointerUp(const CVector2& pos)
{
	CButton::OnPointerUp(pos);
}

void CLightingButton::OnMove(const CVector2& move)
{
	CButton::OnMove(move);
}

void CLightingButton::ChangeState(EState state)
{
	if (!IsEnable()) return;
	if (state == mState) return;

	mState = state;

	mStartScale = mScale;
	if (mState == EState::eIdle) mEndScale = 1.0f;
	else if (mState == EState::eOn) mEndScale = mEnterScale;
	else if (mState == EState::ePress) mEndScale = mPressScale;
	mElapsedTime = 0.0f;
}

void CLightingButton::Update()
{
	CButton::Update();
	mCenter = mSize * 0.5f;
	mpLighting->Update();
}

void CLightingButton::Render()
{
	CButton::Render();
	if (mIsLighting) mpLighting->Render();
}

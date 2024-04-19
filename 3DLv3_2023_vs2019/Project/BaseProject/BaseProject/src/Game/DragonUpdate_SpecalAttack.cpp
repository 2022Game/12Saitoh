#include "CDragon.h"
#include "Global.h"
#include "CPlayer.h"

// 必殺技(空中ブレス攻撃)処理
void CDragon::UpdateSpecalAttack()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Y(0.0);
	mMoveSpeed.Z(0.0f);

	// 攻撃段階に応じて、処理を実行
	switch (mSpAttackStep)
	{
	case 0:// 咆哮処理
		UpdateSpAttack_Step0();
		break;
	case 1:// 離陸
		UpdateSpAttack_Step1();
		break;
	case 2:
		UpdateSpAttack_Step2();
		break;
	case 3:// 空中移動
		UpdateSpAttack_Step3();
		break;
	case 4:// 空中ブレス攻撃
		UpdateSpAttack_Step4();
		break;
	case 5:// 空中移動
		UpdateSpAttack_Step5();
	case 6:// 着陸処理
		UpdateSpAttack_Step6();
		break;
	}

}

// 咆哮処理
void CDragon::UpdateSpAttack_Step0()
{
	// 咆哮が終了したら、次の処理へ移行する
	if (IsAnimationFinished())
	{
		mSpAttackStep++;
		ChangeAnimation(EDragonAnimType::eTakeOff);
	}
}

// 離陸処理
void CDragon::UpdateSpAttack_Step1()
{
	mMoveSpeed += CVector(0.0f, GRAVITY + 0.7f, 0.0f);
	// 離陸が終了したら、次の処理へ移行する
	if (IsAnimationFinished())
	{
		mSpAttackStep++;
		ChangeAnimation(EDragonAnimType::eFlyForward);
	}
}


void CDragon::UpdateSpAttack_Step2()
{
	// 目的地の設定
	CVector fieldPos = gField->Position();
	CVector playerPos = CPlayer::Instance()->Position();
	CVector playerDir = playerPos - fieldPos;
	playerDir.Y(0.0f);
	CVector targetDir = -playerDir;
	// 目的地
	CVector targetPos = targetDir.Normalized() * FIELD_RADIUS * 0.8;

	// 目的値を保存
	mSaveVec = targetPos;
	mSpAttackStep++;
}

#include "Maths.h"
// 空中移動処理1
void CDragon::UpdateSpAttack_Step3()
{
	// 目的地までのベクトルを取得
	CVector myPos = Position();
	myPos.Y(0.0f);
	//CVector targetVec = (mSaveVec - myPos).Normalized();

	// 0度の角度までのベクトルを取得
	float dist = FIELD_RADIUS * 0.6f;
	CVector pos = CVector::zero;
	pos.X(cosf(Math::DegreeToRadian(mAngle)) * dist);
	pos.Z(sinf(Math::DegreeToRadian(mAngle)) * dist);

	float targetLength = (pos - myPos).Length();
	if (targetLength <= 100.0f)
	{
		mSpAttackStep++;
	}

	CVector posVec = pos.Normalized();
	mMoveSpeed += posVec * 3.0f;

	// 目的地まで移動する
	//mMoveSpeed += targetVec * 1.0 ;
	// 重力で落ちないように調整
	mMoveSpeed += CVector(0.0f, GRAVITY, 0.0f);
}

// 空中ブレス攻撃処理
void CDragon::UpdateSpAttack_Step4()
{
	// 今は仮で空中移動処理を実装
	float dist = FIELD_RADIUS * 0.9f;
	CVector pos = CVector::zero;
	pos.X(cosf(Math::DegreeToRadian(mAngle)) * dist);
	pos.Z(sinf(Math::DegreeToRadian(mAngle)) * dist);

	CVector myPos = Position();
	myPos.Y(0.0f);
	CVector targetVec = (pos - myPos).Normalized();

	mMoveSpeed += targetVec * 4.0f;
	mAngle += 0.6;
	if (mAngle > 360.0f)
	{
		mAngle = 0.0f;
	}
	// 重力で落ちないように調整
	mMoveSpeed += CVector(0.0f, GRAVITY, 0.0f);
}

// 空中移動処理2
void CDragon::UpdateSpAttack_Step5()
{

}

// 着陸処理
void CDragon::UpdateSpAttack_Step6()
{
	// 着陸したら、戦闘状態へ移行
	if (IsAnimationFinished())
	{
		mSpAttackStep = 0;
		mState = EState::eBattle;
		ChangeAnimation(EDragonAnimType::eIdle1);
	}
}

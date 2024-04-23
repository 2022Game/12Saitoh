#include "CDragon.h"
#include "Global.h"
#include "CPlayer.h"
#include "Maths.h"

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
	case 2:// 目的地を設定
		UpdateSpAttack_Step2();
		break;
	case 3:// 空中移動処理1
		UpdateSpAttack_Step3();
		break;
	case 4:// 空中移動処理2
		UpdateSpAttack_Step4();
		break;
	case 5:// 空中ブレス攻撃
		UpdateSpAttack_Step5();
		break;
	case 6:// 空中移動処理3
		UpdateSpAttack_Step6();
		break;
	case 7:// 着陸処理
		UpdateSpAttack_Step7();
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
	// 離陸
	mMoveSpeed += CVector(0.0f, GRAVITY + 0.7f, 0.0f);

	// 離陸が終了したら、次の処理へ移行する
	if (IsAnimationFinished())
	{
		mSpAttackStep++;
		ChangeAnimation(EDragonAnimType::eFlyForward);

		// 次のステップで移動する目的地を設定
		CVector pPos = CPlayer::Instance()->Position();
		CVector fPos = gField->Position();
		CVector FP = pPos - fPos;
		FP.Y(0.0f);
		// 目的地の座標
		CVector targetPos = FP.Normalized() * FIELD_RADIUS * 0.7;
		targetPos.Y(0.0f);

		// 移動する目的地を保存
		mSaveDestination = targetPos;
		mSaveVec = targetPos.Normalized();
	}
}

// 空中移動処理1
// 外周まで移動
void CDragon::UpdateSpAttack_Step2()
{
	CVector myPos = Position();
	myPos.Y(0.0f);
	// 外周までの距離が100以下なら、
	// 次の移動ステップに移行する
	float targetLength = (mSaveDestination - myPos).Length();
	if (targetLength <= 100.0f)
	{
		// 角度の設定
		mAngle = GetAngle();

		// 目的地の更新
		mSaveDestination = GetDestination();
		mSaveVec = CVector::zero;
		mSpAttackStep++;
	}

	// 移動処理
	mMoveSpeed += mSaveVec * 3.0f;

	// 重力で落ちないように調整
	mMoveSpeed += CVector(0.0f, GRAVITY, 0.0f);
}

// 空中移動処理2
// 目的地まで外周を移動
void CDragon::UpdateSpAttack_Step3()
{
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

	// デバッグ表示用
	GetAngle();
}

// 空中アイドル及び高さ調整
void CDragon::UpdateSpAttack_Step4()
{

}

// 空中ブレス攻撃
void CDragon::UpdateSpAttack_Step5()
{

}

// 空中移動処理3
void CDragon::UpdateSpAttack_Step6()
{

}

// 着陸処理
void CDragon::UpdateSpAttack_Step7()
{
	// 着陸したら、戦闘状態へ移行
	if (IsAnimationFinished())
	{
		mSpAttackStep = 0;
		mState = EState::eBattle;
		ChangeAnimation(EDragonAnimType::eIdle1);
	}
}

// 空中ブレス攻撃を行う位置(目的地)の設定
CVector CDragon::GetDestination() const
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
	return targetPos;
}

// 外周の自身のいる位置の中心からみた角度を取得
float CDragon::GetAngle() const
{
	float dist = FIELD_RADIUS;
	// 0度のベクトル
	CVector zeropos = CVector::zero;
	zeropos.X(cosf(Math::DegreeToRadian(0.0f)) * dist);
	zeropos.Z(sinf(Math::DegreeToRadian(0.0f)) * dist);

	// 原点から自身の角度のベクトル
	CVector dPos = Position();
	CVector fPos = gField->Position();
	CVector DP = dPos - fPos;
	DP.Y(0.0f);
	CVector targetPos = DP.Normalized() * FIELD_RADIUS;

	// 角度の計算
	targetPos.Normalized();
	float angle = Math::RadianToDegree(CVector::Angle(zeropos, targetPos));
	// Xがプラス域
	if (targetPos.X() >= 0)
	{
		// Yがプラス域
		if (targetPos.Z() >= 0)
		{
			// 第一象限
			CDebugPrint::Print("第一象限\n");
			CDebugPrint::Print("ドラゴンの現在の角度 : %.1f\n", angle);
		}
		else // Zがマイナス域
		{
			// 第四象限
			CDebugPrint::Print("第四象限\n");
			angle = 180.0f + (180.0f - angle);
			CDebugPrint::Print("ドラゴンの現在の角度 : %.1f\n", angle);
		}
	}
	else // Xがマイナス域
	{
		if (targetPos.Z() >= 0)
		{
			// 第二象限
			CDebugPrint::Print("第二象限\n");
			CDebugPrint::Print("ドラゴンの現在の角度 : %.1f\n", angle);
		}
		// Zがマイナス域
		else
		{
			// 第三象限
			CDebugPrint::Print("第三象限\n");
			angle = 180.0f + (180.0f - angle);
			CDebugPrint::Print("ドラゴンの現在の角度 : %.1f\n", angle);
		}
	}
	return angle;
}
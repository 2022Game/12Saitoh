#include "CDragon.h"
#include "Global.h"
#include "CPlayer.h"
#include "Maths.h"
#include "CSPFlamethrower.h"
#include "CColliderLine.h"

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
	case 6:// 空中アイドル処理
		UpdateSpAttack_Step6();
		break;
	case 7:// 空中移動処理3
		UpdateSpAttack_Step7();
		break;
	case 8:// 着陸処理
		UpdateSpAttack_Step8();
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
		SetAnimationSpeed(0.45f);
		if (!mpFlySE->IsPlaying())
		{
			mpFlySE->PlayLoop(-1, true, 0.2f);
		}
		else
		{
			mpFlySE->SetBaseVolume(0.2f);
		}
		// ドラゴンが飛行中進行方向に向かないバグが発生するため
		// 念のためここでもmBatteleStepを0にしておく
		mBatteleStep = 0;
	}

	// モーションブラーを掛け始めるフレーム以上の時
	// モーションブラーを掛ける
	if (SCREAMBLUR_START <= GetAnimationFrame() &&
		GetAnimationFrame() <= SCREAMBLUR_START + 10.0f)
	{
		// モーションブラーを掛けている最中であれば、
		// 新しくモーションブラーを掛け直さない
		if (mMotionBlurRemainTime <= 0.0f)
		{
			System::SetEnableMotionBlur(true);
			mMotionBlurRemainTime = MOTION_BLUR_TIME;
			// SEを再生
			CSound* screamSE = CResourceManager::Get<CSound>("Scream");
			screamSE->Play(0.4f);
		}
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
		SetAnimationSpeed(0.6f);
		mpFlySE->SetVolume(0.0f);

		// 次のステップで移動する目的地を設定
		CVector pPos = CPlayer::Instance()->Position();
		CVector fPos = gField->Position();
		CVector FP = pPos - fPos;
		FP.Y(0.0f);
		// 目的地の座標
		CVector targetPos = FP.Normalized() * FIELD_RADIUS * 0.7;
		targetPos.Y(0.0f);

		// 目的地までのベクトルを設定
		CVector dPos = Position();
		dPos.Y(0.0f);
		CVector targetVec = (targetPos - dPos).Normalized();

		// 移動する目的地を保存
		mSaveDestination = targetPos;
		mSaveVec = targetVec;
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
	mMoveSpeed += mSaveVec * 3.3f;

	// 重力で落ちないように調整
	mMoveSpeed += CVector(0.0f, GRAVITY, 0.0f);
}

// 空中移動処理2
// 目的地まで外周を移動
void CDragon::UpdateSpAttack_Step3()
{
	float dist = FIELD_RADIUS * 0.8f;
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

	// 目的地に着いたら次のステップに移行
	float targetLength = (mSaveDestination - myPos).Length();
	if (targetLength <= 130.0f)
	{
		mSpAttackStep++;
		ChangeAnimation(EDragonAnimType::eFlyIdle);
		SetAnimationSpeed(0.31f);
		mSaveDestination = CVector::zero;
		mpFlySE->SetVolume(0.2f);
	}
	// デバッグ表示用
	GetAngle();
}

// 空中アイドル及び高さ調整
void CDragon::UpdateSpAttack_Step4()
{
	CVector dPos = Position();
	CVector fPos = gField->Position();
	CVector FD = (fPos - dPos).Normalized();
	FD.Y(0.0f);
	// ステージの中心に向ける
	mMoveSpeed = FD * 0.0000000001;

	mMoveSpeed -= CVector(0.0f, 0.35f, 0.0f);

	// 2秒経過で次のステップへ移行
	mElapsedTime += Time::DeltaTime();
	if (mElapsedTime >= 2.0f)
	{
		if (IsAnimationFinished())
		{
			mElapsedTime = 0.0f;
			mSpAttackStep++;
			ChangeAnimation(EDragonAnimType::eFlyFlame);
			SetAnimationSpeed(0.2f);
			mpFlySE->SetVolume(0.0f);
			mIsFlyBreath = true;
		}
	}
}

// 空中ブレス攻撃
void CDragon::UpdateSpAttack_Step5()
{
	// 重力で落ちないように調整
	mMoveSpeed += CVector(0.0f, GRAVITY, 0.0f);

	// ブレス発射処理
	if (15.0f <= GetAnimationFrame() &&
		!mpSpFlamethrower->IsThrowing())
	{
		mpSpFlamethrower->Start();
		// ブレス攻撃が終わった後にも再生されないように
		// 再生フレームを指定
		if (50.0f > GetAnimationFrame())
		{
			CSound* flybreathSE = CResourceManager::Get<CSound>("FlyBreath");
			flybreathSE->Play(0.35f);
		}
	}

	// ブレス停止処理
	if (77.0f <= GetAnimationFrame() &&
		mpSpFlamethrower->IsThrowing())
	{
		mpSpFlamethrower->Stop();
	}

	// 攻撃が終わったら、次のステップへ移行
	if (IsAnimationFinished())
	{
		mSpAttackStep++;
		ChangeAnimation(EDragonAnimType::eFlyIdle);
		SetAnimationSpeed(0.31f);
		mIsFlyBreath = false;
		if (mpSpFlamethrower->IsThrowing())
		{
			mpSpFlamethrower->Stop();
		}
		mpFlySE->SetVolume(0.2f);
	}
}

// 空中アイドル処理
void CDragon::UpdateSpAttack_Step6()
{
	mElapsedTime += Time::DeltaTime();
	// 4秒経過で次のステップへ移行
	if (mElapsedTime >= 4.0f)
	{
		if (IsAnimationFinished())
		{
			mElapsedTime = 0.0f;
			mSpAttackStep++;
			ChangeAnimation(EDragonAnimType::eFlyForward);
			SetAnimationSpeed(0.6f);
			mpFlySE->SetVolume(0.0f);
		}
	}
	// 重力で落ちないように調整
	mMoveSpeed += CVector(0.0f, GRAVITY, 0.0f);
}

// 空中移動処理3
void CDragon::UpdateSpAttack_Step7()
{
	// ステージ中央へ移動する
	CVector fPos = gField->Position();
	CVector dPos = Position();
	CVector FD = fPos - dPos;
	FD.Y(0.0f);

	// 自身からステージの中心までの距離を取得
	float targetLength = FD.Length();
	mMoveSpeed += FD.Normalized() * 3.0f;

	// 重力で落ちないように調整
	mMoveSpeed += CVector(0.0f, GRAVITY, 0.0f);

	// ステージの中心まで移動したら
	// 次のステップへ移行する
	if (targetLength <= 30.0f)
	{
		mSpAttackStep++;
		ChangeAnimation(EDragonAnimType::eLand);
		SetAnimationSpeed(0.325f);
		mpColliderLine->Position(mpColliderLine->Position() + CVector(0.0f, 20.0f, 0.0f));
		mpFlySE->SetVolume(0.2f);
	}
}

// 着陸処理
void CDragon::UpdateSpAttack_Step8()
{
	// 着陸したら、戦闘状態へ移行
	if (IsAnimationFinished())
	{
		mSpAttackStep = 0;
		mBatteleStep = 0;
		mState = EState::eBattle;
		ChangeAnimation(EDragonAnimType::eIdle1);
		SetAnimationSpeed(0.4f);
		mpColliderLine->Position(CVector::zero);
		mpFlySE->SetVolume(0.0f);

		// 着陸後に怯み値が溜まっている場合、
		// 怯みモーションを行ってしまうため、怯み値を半分にする
		mFearValue /= 2;
	}

	mMoveSpeed -= CVector(0.0f, GRAVITY*3, 0.0f);
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
	targetPos.Y(0.0f);

	// 目的値を保存
	return targetPos;
}

// 外周の自身のいる位置の中心からみた角度を取得
float CDragon::GetAngle() const
{
	// 0度のベクトル
	CVector zeropos = CVector::zero;
	zeropos.X(cosf(0.0f));
	zeropos.Z(sinf(0.0f));

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
			//CDebugPrint::Print("第一象限\n");
			//CDebugPrint::Print("ドラゴンの現在の角度 : %.1f\n", angle);
		}
		else // Zがマイナス域
		{
			// 第四象限
			//CDebugPrint::Print("第四象限\n");
			angle = 180.0f + (180.0f - angle);
			//CDebugPrint::Print("ドラゴンの現在の角度 : %.1f\n", angle);
		}
	}
	else // Xがマイナス域
	{
		if (targetPos.Z() >= 0)
		{
			// 第二象限
			//CDebugPrint::Print("第二象限\n");
			//CDebugPrint::Print("ドラゴンの現在の角度 : %.1f\n", angle);
		}
		// Zがマイナス域
		else
		{
			// 第三象限
			//CDebugPrint::Print("第三象限\n");
			angle = 180.0f + (180.0f - angle);
			//CDebugPrint::Print("ドラゴンの現在の角度 : %.1f\n", angle);
		}
	}
	return angle;
}

// 空中ブレス中かどうか
bool CDragon::IsFlyBreath() const
{
	return mIsFlyBreath;
}
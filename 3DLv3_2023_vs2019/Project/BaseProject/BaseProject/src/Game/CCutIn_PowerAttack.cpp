#include "CCutIn_PowerAttack.h"
#include "Maths.h"

// コンストラクタ
CCutIn_PowerAttack::CCutIn_PowerAttack()
	: mCutInStep(0)
	, mElapsedTime(0.0f)
	, mCenterPos(0.0f, 0.0f, 0.0f)
	, mStartAngleY(0.0f)
	, mStartAngleX(0.0f)
	, mStartSideVec(0.0f, 0.0f, 0.0f)
{
}

// デストラクタ
CCutIn_PowerAttack::~CCutIn_PowerAttack()
{
}

// カットイン開始
bool CCutIn_PowerAttack::Start()
{
	// ベースのカットイン開始処理
	if (!CCutInCamera::Start()) return false;

	// カットインに使用する変数などの初期化
	mCutInStep = 0;
	mElapsedTime = 0.0f;

	return true;
}

// カットイン終了
bool CCutIn_PowerAttack::End()
{
	// ベースのカットイン終了処理
	if (!CCutInCamera::End()) return false;

	return true;
}

// カットインに必要な情報を設定
void CCutIn_PowerAttack::Setup(CObjectBase* obj)
{
	// 設定されたオブジェクトの位置を中心地点に設定
	mCenterPos = obj->Position();
	// 設定されたオブジェクトの現在の向きのY軸の角度を取得
	mStartAngleY = obj->EulerAngles().Y();
	// 設定されたオブジェクトの現在の向きのX軸の角度を取得
	mStartAngleX = obj->EulerAngles().X();
	
	// 設定されたオブジェクトの横方向ベクトルを取得
	mStartSideVec = obj->VectorX();
}

#define CUTIN_TIME 0.4f		// 継続時間
#define START_ANGLE 90.0f	// 開始時のY軸回転角度
#define END_ANGLE 115.0f	// 終了時のY軸回転角度
#define START_ANGLEX 0.0f	// 開始時のX軸回転角度
#define END_ANGLEX -20.0f	// 終了時のX軸回転角度
#define START_Y 10.0f		// 開始時のカメラの高さ
#define END_Y 10.0f			// 終了時のカメラの高さ
#define START_X 0.0f		// 開始時のカメラの横方向
#define END_X -3.0f			// 終了時のカメラの横方向
#define START_DIST 15.0f	// 開始時の画面拡縮
#define END_DIST 14.0f		// 終了時の画面拡縮
#define WAIT_TIME 0.4f	// 待ち時間

// ステップ0 カメラを回転
void CCutIn_PowerAttack::CutInStep0()
{
	if (mElapsedTime < CUTIN_TIME)
	{
		// カットインの進行度(0.0 ～ 1.0)
		float per = mElapsedTime / CUTIN_TIME;

		// カットインの進行度に応じて高さを補完
		CVector offsetPos = CVector::zero;
		float offsetX = Math::Lerp(START_X, END_X, per);
		offsetPos += mStartSideVec * offsetX;
		float offsetY = Math::Lerp(START_Y, END_Y, per);
		offsetPos.Y(offsetY);

		// カットインの進行度に応じて回転角度を補完
		// X軸
		float startAngX = mStartAngleX + START_ANGLEX;
		float endAngX = mStartAngleX + END_ANGLEX;
		float angleX = Math::Lerp(startAngX, endAngX, per);
		float radAngX = Math::DegreeToRadian(angleX);
		// Y軸
		float startAngY = mStartAngleY + START_ANGLE;
		float endAngY = mStartAngleY + END_ANGLE;
		float angleY = Math::Lerp(startAngY, endAngY, per);
		float radAngY = Math::DegreeToRadian(angleY);

		// カットインの進行度に応じて画面拡縮の補完
		float dist = Math::Lerp(START_DIST, END_DIST, per);

		// カメラ位置の計算
		mAt = mCenterPos + offsetPos;
		mEye = mAt + CVector(cosf(radAngX) * cosf(radAngY), sinf(radAngX), cosf(radAngX) * sinf(radAngY)) * dist;

		// 経過時間の更新
		mElapsedTime += Time::DeltaTime();
	}
	else
	{
		// カットイン終了時の位置と向き
		CVector offsetPos = CVector::zero;
		offsetPos += mStartSideVec * END_X;
		offsetPos.Y(END_Y);

		float radAngX = Math::DegreeToRadian(END_ANGLEX);
		float radAngY = Math::DegreeToRadian(mStartAngleY + END_ANGLE);
		float dist = END_DIST;

		mAt = mCenterPos + offsetPos;
		mEye = mAt + CVector(cosf(radAngX) * cosf(radAngY), sinf(radAngX), cosf(radAngX) * sinf(radAngY)) * dist;

		// 経過時間を初期化し、次のステップへ進む
		mCutInStep++;
		mElapsedTime = 0.0f;
	}
}

// ステップ1 カットイン終了時の待ち
void CCutIn_PowerAttack::CutInStep1()
{
	if (mElapsedTime < WAIT_TIME)
	{
		mElapsedTime += Time::DeltaTime();
	}
	else
	{
		mCutInStep = 0;
		mElapsedTime = 0.0f;
		End();
	}
}

// 更新
void CCutIn_PowerAttack::Update()
{
	// カットイン再生中出なければ、処理しない
	if (!mIsPlaying) return;

	switch (mCutInStep)
	{
	case 0:
		CutInStep0();
		break;
	case 1:
		CutInStep1();
		break;
	}

	// カメラの更新
	CCamera::Update();
}
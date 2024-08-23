#include "CCutIn_GameOver.h"
#include "Maths.h"

// コンストラクタ
CCutIn_GameOver::CCutIn_GameOver()
	: mCutInStep(0)
	, mElapsedTime(0.0f)
	, mCenterPos(0.0f, 0.0f, 0.0f)
	, mStartAngleY(0.0f)
	, mStartAngleX(0.0f)
	, mStartSideVec(0.0f, 0.0f, 0.0f)
{
}

// デストラクタ
CCutIn_GameOver::~CCutIn_GameOver()
{
}

// カットイン開始
bool CCutIn_GameOver::Start()
{
	// ベースのカットイン開始処理
	if (!CCutInCamera::Start_NoPause()) return false;

	// カットインに使用する変数などの初期化
	mCutInStep = 0;
	mElapsedTime = 0.0f;

	return true;
}

// カットインに必要な情報を設定
void CCutIn_GameOver::Setup(CObjectBase* obj)
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

#define CUTIN_TIME 10.0f	// 継続時間
#define START_ANGLE 70.0f	// 開始時のY軸回転角度
#define END_ANGLE 120.0f	// 終了時のY軸回転角度
#define START_Y 7.0f		// 開始時のカメラの高さ
#define END_Y 7.0f			// 終了時のカメラの高さ
#define START_DIST 15.0f	// 開始時の画面拡縮
#define END_DIST 15.0f		// 終了時の画面拡縮
#define WAIT_TIME 0.4f		// 待ち時間

// カメラを回転
void CCutIn_GameOver::CutIn()
{
	// カットインの進行度(0.0 ～ 1.0)
	float per = mElapsedTime / CUTIN_TIME;

	// カットインの進行度に応じて高さを補完
	CVector offsetPos = CVector::zero;
	float offsetY = Math::Lerp(START_Y, END_Y, per);
	offsetPos.Y(offsetY);

	// カットインの進行度に応じて回転角度を補完
	// Y軸
	float startAngY = mStartAngleY + START_ANGLE;
	float endAngY = mStartAngleY + END_ANGLE;
	float angleY = Math::Lerp(startAngY, endAngY, per);
	float radAngY = Math::DegreeToRadian(angleY);

	// カットインの進行度に応じて画面拡縮の補完
	float dist = Math::Lerp(START_DIST, END_DIST, per);

	// カメラ位置の計算
	mAt = mCenterPos + offsetPos;
	mEye = mAt + CVector(cosf(radAngY), 0.0f, sinf(radAngY)) * dist;

	// 経過時間の更新
	mElapsedTime += Time::DeltaTime();
}

// 更新
void CCutIn_GameOver::Update()
{
	// カットイン再生中出なければ、処理しない
	if (!mIsPlaying) return;

	CutIn();

	// カメラの更新
	CCamera::Update();
}
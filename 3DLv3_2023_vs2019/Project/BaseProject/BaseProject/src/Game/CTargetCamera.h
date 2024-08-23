#pragma once
#include "CCamera.h"
// ターゲットカメラ
class CTargetCamera : public CCamera
{
public:
	// コンストラクタ
	CTargetCamera();
	// デストラクタ
	~CTargetCamera();

	// ターゲットカメラ開始
	void Start();
	// ターゲットカメラ終了
	void End();

	/// <summary>
	/// ターゲットカメラに必要な情報を取得
	/// </summary>
	/// <param name="Eyeobj">視点に設定するオブジェクト</param>
	/// <param name="Atobj">ターゲットに設定するオブジェクト</param>
	void Update_Set(CObjectBase* Eyeobj, CObjectBase* Atobj);

	// ターゲットカメラ再生中かどうか
	bool IsPlaying() const;

	// 更新処理
	void Update() override;
private:
	// 現在再生中のターゲットカメラ
	static CTargetCamera* spTargetCamera;
	bool mIsPlaying;	// 再生中かどうか

	CVector mEyePos;	// 視点オブジェクトの位置
	CVector mAtPos;		// ターゲットオブジェクトの位置
};
#pragma once
#include <vector>
#include "CTexture.h"
#include "CPlayer.h"
#include "CInput.h"
#include "CFont.h"
#include "CSound.h"
#include "CVector.h"
#include "CModel.h"
#include "CMatrix.h"
#include "CCharacter3.h"
#include "CTransform.h"
#include "CTaskManager.h"
#include "CCollisionManager.h"
#include "CColliderTriangle.h"
#include "CColliderMesh.h"
#include "CUi.h"
#include "CMapManager.h"

class CApplication
{
private:
	//UIクラスのポインタ
	static CUi* spUi;
	//モデルビューの逆行列
	static CMatrix mModelViewInverse;
	static CTexture mTexture;

	//モデルからコライダを生成
	CColliderMesh mColliderMesh;
	CPlayer mPlayer;	//プレイヤー
	CMatrix mMatrix;
	CModel mBackGround; //背景モデル
	//モデルクラスのインスタンス作成
	CModel mModel;
	CSound mSoundBgm;	//BGM
	CSound mSoundOver;	//ゲームオーバー
	CInput mInput;
	CFont mFont;
	CMapManager* mpMapManager;

	enum class EState
	{
		ESTART,	//ゲーム開始
		EPLAY,	//ゲーム中
		ECLEAR,	//ゲームクリア
		EOVER,	//ゲームオーバー
	};
	EState mState;
	static int mCameraFlag; //デバッグ用
	//カメラの設定(後でカメラクラスにまとめる)
	void Camera();
	//各モデルを読み込む
	void CreateModel();
public:
	~CApplication();
	//UIクラスのインスタンスを取得
	static CUi* Ui();
	//モデルビュー行列の取得
	static const CMatrix& ModelViewInverse();
	static CTexture* Texture();
	//デバッグ用
	static int CameraFlag();
	//最初に一度だけ実行するプログラム
	void Start();
	//繰り返し実行するプログラム
	void Update();
};
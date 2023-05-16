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
class CApplication
{
private:
	static CUi* spUi; //UIクラスのポインタ
	//モデルからコライダを生成
	CColliderMesh mColliderMesh;
	//モデルビューの逆行列
	static CMatrix mModelViewInverse;
	//C5モデル
	CModel mModelC5;
	//static CTaskManager mTaskManager;
	CPlayer mPlayer;
	CMatrix mMatrix;
	CModel mBackGround; //背景モデル
	//モデルクラスのインスタンス作成
	CModel mModel;
	CVector mEye;
	CSound mSoundBgm;	//BGM
	CSound mSoundOver;	//ゲームオーバー
	enum class EState
	{
		ESTART,	//ゲーム開始
		EPLAY,	//ゲーム中
		ECLEAR,	//ゲームクリア
		EOVER,	//ゲームオーバー
	};
	EState mState;
	CPlayer* mpPlayer;
	static CTexture mTexture;
	CInput mInput;
	CFont mFont;
	bool CameraFlag = false; //デバッグ用
public:
	~CApplication();
	static CUi* Ui(); //UIクラスのインスタンスを取得
	//モデルビュー行列の取得
	static const CMatrix& ModelViewInverse();
	//static CTaskManager* TaskManager();
	static CTexture* Texture();
	//最初に一度だけ実行するプログラム
	void Start();
	//繰り返し実行するプログラム
	void Update();
};
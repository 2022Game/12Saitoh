#pragma once
#include "CTexture.h"
#include "CCharacter.h"
#include "CInput.h"
#include "CFont.h"
#include <vector>
#include "CSound.h"
#include "CVector.h"
#include "CModel.h"
#include "CTaskManager.h"
#include "CColliderTriangle.h"
#include "CColliderMesh.h"
#include "CUi.h"
#include "CCamera.h"
#include "CModelX.h"
#include "CInput.h"
#include "CXCharacter.h"
#include "CXPlayer.h"
#include "CXEnemy.h"
#include "CPaladin.h"
#include "CActionCamera.h"

class CApplication
{
private:
	static CMatrix mModelViewInverse;	//モデルビューの逆行列
	static CTexture mTexture;
	static CUi* spUi;	//UIクラスのポインタ

	CInput mInput;
	CMatrix mMatrix;
	CModelX mModelX;
	CColliderMesh mColliderMesh; //モデルからコライダを生成
	CModel mModelC5;	         //C5モデル
	CModel mBackGround;          //背景モデル
	CModel mModel;
	CVector mEye;
	CSound mSoundBgm;	        //BGM
	CSound mSoundOver;	        //ゲームオーバー
	CXCharacter mCharacter;		//キャラクタクラスのインスタンス
	CXPlayer mXPlayer;			//プレイヤーのインスタンス
	CXEnemy mXEnemy;			//敵のインスタンス
	CModelX mKnight;			//敵(兵士)のインスタンス
	CActionCamera mActionCamera;//アクションカメラ
	CPaladin* mpPaladin;

	enum class EState
	{
		ESTART,	//ゲーム開始
		EPLAY,	//ゲーム中
		ECLEAR,	//ゲームクリア
		EOVER,	//ゲームオーバー
	};
	EState mState;
	CFont mFont;
public:
	~CApplication();
	static CUi* Ui();	//UIクラスのインスタンスを取得
	//モデルビュー行列の取得
	static const CMatrix& ModelViewInverse();
	static CTexture* Texture();

	//最初に一度だけ実行するプログラム
	void Start();
	//繰り返し実行するプログラム
	void Update();
};
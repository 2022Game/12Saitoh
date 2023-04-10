#pragma once
#include <vector>
#include "CRectangle.h"
#include "CCharacter.h"
#include "CCharacterManager.h"
#include "CTexture.h"
#include "CPlayer.h"
#include "CEnemy.h"
#include "CInput.h"
#include "CFont.h"
#include "CGame.h"
#include "CSound.h"
#include "CBackGround.h"
#include "CHeart.h"
#include "CGage.h"
#include "CFont2.h"
#include "CSound.h"

class CApplication
{
private:
	enum class EState
	{ 
		ESTART,  //ゲーム開始
		EPLAY,	 //ゲーム中
		ESTAGE1, //ステージ1
		ESTAGE2, //ステージ2
		EBOSS,   //ボスステージ
		ECLEAR,	 //ゲームクリア
		EOVER,	 //ゲームオーバー
	};

	CGame* mpGame;
	EState mState;
	CInput mInput;
	CFont mFont;
	CSound mSoundBGM;     //通常BGM
	CSound mSoundBossBGM; //ボスBGM
	CSound mSoundClear;   //ゲームクリア
	CSound mSoundOver;    //ゲームオーバー
	CSound mSoundSE;      //SE

	static int sStageCount; //ステージ移行時のアニメーションカウンタ　
	static bool sDelete;    //テクスチャを消すタイミングを取得

	static CTexture mTexture;  //プレイヤーテクスチャ
	static CTexture mTexture2; //背景テクスチャ
	static CTexture mTexture3; //スタート画面テクスチャ
	static CTexture mTexture4; //ゲームオーバー画面テクスチャ
	static CTexture mTexture5; //ゲームクリア画面テクスチャ
	static CTexture mTexture6; //背景テクスチャ
	static CTexture mTexture7; //背景テクスチャ
	static CTexture mTexture8; //背景テクスチャ
	static CTexture mTexture9; //アイテムテクスチャ
	static CTexture mTexture100; //ハート表示テクスチャ
	static CTexture mTexture101; //スタミナ表示テクスチャ
	static CTexture mTexture102; //スタミナゲージ表示テクスチャ

public:
	static CTexture* Texture();
	static CTexture* Texture2();
	static CTexture* Texture3();
	static CTexture* Texture4();
	static CTexture* Texture5();
	static CTexture* Texture6();
	static CTexture* Texture7();
	static CTexture* Texture8();
	static CTexture* Texture9();
	static CTexture* Texture100();
	static CTexture* Texture101();
	static CTexture* Texture102();

	//sStageCountの値を取得
	static int StageCount();
	//mDeleteの判定を取得
	static bool Delete();

	//コンストラクタ
	CApplication();
	//最初に一度だけ実行するプログラム
	void Start();
	//繰り返し実行するプログラム
	void Update();
};
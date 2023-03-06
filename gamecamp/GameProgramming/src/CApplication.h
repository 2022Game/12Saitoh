#pragma once
#include <vector>
#include "CRectangle.h"
#include "CTexture.h"
#include "CInput.h"
#include "CCharacterManager.h"
#include "CGame.h"
#include "CSound.h"
#include "CCharacter.h"
#include "CTaskManager.h"
#include "CPlayer.h"
#include "CEnemy.h"
#include "CUi.h"
#include "CBack.h"
class CApplication
{
private:
	CGame* mpGame;
	CPlayer* mpPlayer;  //プレイヤークラスのポインタ
	static CUi* spUi;   //UIクラスのポインタ
	CSound mSoundBgm;	//BGM
	CSound mSoundOver;	//ゲームオーバー
	static CCharacterManager mCharacterManager;
	static CTexture mTexture;
	static CTexture mTexture2;
	CInput mInput;
public:
	~CApplication();
	static CUi* Ui(); //UIクラスのインスタンスを取得
	//static CTaskManager* TaskManager();
	static CCharacterManager* CharacterManager();
	static CTexture* Texture();
	static CTexture* Texture2();
	//最初に一度だけ実行するプログラム
	void Start();
	//繰り返し実行するプログラム
	void Update();
};
#pragma once
#include "CCharacter.h"
#include "CInput.h"
#include "CSound.h"

class CPlayer2 : public CCharacter
{
public:
	//プレイヤーの泣く動作を取得
	void Cry();
	//プレイヤーの移動動作を取得
	void Move();
	//プレイヤーのジャンプ動作を取得
	void Jump();
	//プレイヤーのインスタンスを取得
	static CPlayer2* Instance();
	//ゴールの値を取得
	static int Goal();
	//HPを取得
	static int Hp();
	//衝突処理2
	void Collision();
	//衝突処理4
	void Collision(CCharacter* m, CCharacter* o);
	//CPlayer2(X座標,Y座標,幅,高さ,テクスチャのポインタ)
	CPlayer2(float x, float y, float w, float h, CTexture* pt);
	//更新処理
	void Update();
private:
	static int sgoal;
	static CPlayer2* spInstance;
	int interval; //ジャンプのインターバル
	CSound mSoundJump;	//ジャンプSE
	CSound mSoundDown;  //踏みつけSE
	static int sHp;	//HP
	int mInvincible; //無敵カウンタ
	CInput mInput;
};
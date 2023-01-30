#pragma once
#include "CRectangle.h"
#include "CTexture.h"
#include <stdio.h>

#define VELOCITY 4.0f	//移動速度

class CCharacter : public CRectangle
{
public:
	enum class ETag
	{
		EZERO,		//初期値
		EBULLET,	//弾
		EENEMY,		//敵
		EENEMY2,    //敵2
		EPLAYER,	//プレイヤー
		ETURN,		//折り返し
		EBLOCK,		//ブロック
		ETRAP,      //トラップ
		EWALL,      //ゲームオーバー判定用の壁
		EGOAL,      //ゴール
		EFOLLBLOCK, //落下床
		EDOKAN,     //土管
		EBACK,      //背景
	};
	enum class EState	//状態
	{
		EMOVE,	//移動
		ESTOP,	//停止
		EJUMP,	//ジャンプ
		ECRY,	//泣く
		EFOLL,  //落下
	};
	enum class Espstate  //隠しステイタス
	{
		ESPMOVE,    //移動
		EHIGHJUMP,  //ハイジャンプ 
		NULLSTATE,  //空
	};
protected:
	float mVx;	//X軸速度
	float mVy;	//Y軸速度
	bool mEnabled;
	ETag mTag;
	EState mState;
	Espstate mspState;
private:
	CTexture *mpTexture;
	int mLeft, mRight, mBottom, mTop;
public:
	Espstate spState(); //隠しステータスを取得する
	EState State();	//状態を取得する
	bool Enabled();
	ETag Tag();
	//衝突処理２
	virtual void Collision() {};
	//衝突処理４
	//Collision(自分のポインタ, 衝突相手のポインタ)
	virtual void Collision(CCharacter* m, CCharacter* o) {};
	virtual void Update() = 0;
	CCharacter();
	CTexture* Texture();
	void Texture(CTexture *pTexture, int left, int right, int bottom, int top);
	virtual void Render();
	void Move();
};
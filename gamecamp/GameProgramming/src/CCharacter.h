#ifndef CCHARACTER_H
#define CCHARACTER_H
#include"CTask.h"
#include"CRectangle.h"
#include"CTexture.h"
/*
キャラクタークラス
ゲームキャラクタの基本的な機能を定義する
*/
//コライダクラスの宣言
class CCharacter : public CTask, public CRectangle
{
public:
	enum ETag
	{
		EZERO,          //初期値
		EPLAYER,        //プレイヤー
		EENEMY,         //敵
		EBOSS,          //ボス
	};
protected:
	ETag mTag; //タグ
private:
	CTexture* mpTexture;
	int mLeft, mRight, mBottom, mTop;
public:
	//タグの取得
	ETag Tag();
	//コンストラクタ
	CCharacter(int priority);
	//衝突処理
	virtual void Collision(CCharacter* m, CCharacter* o) {}
	//衝突処理2
	virtual void Collision() {};
	//コンストラクタ
	CCharacter();
	//デストラクタ
	~CCharacter();
	//描画処理
	void Render();
	CTexture* Texture();
	void Texture(CTexture* pTexture,
		int left, int right, int bottom, int top);
	bool Enabled();
};
#endif
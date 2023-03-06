#ifndef CPLAYER_H
#define CPLAYER_H
//キャラクタスクラスのインクルード
#include "CCharacter.h"
#include "CInput.h"
/*
プレイヤークラス
キャラクタスを継承
*/
class CPlayer : public CCharacter
{
public:
	//インスタンスのポインタの取得
	static CPlayer* Instance();
	//コンストラクタ
	//CPlayer(X座標,Y座標,幅,高さ,テクスチャのポインタ)
	CPlayer(float x, float y, float w, float h, CTexture* pt);
	//更新処理
	void Update();
private:
	//プレイヤーのインスタンス
	static CPlayer* spInstance;
	CInput mInput;
};
#endif
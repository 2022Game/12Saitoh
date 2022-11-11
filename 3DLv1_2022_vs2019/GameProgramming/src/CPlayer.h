#ifndef CPLAYER_H
#define CPLAYER_H
//キャラクタスクラスのインクルード
#include "CCharacter3.h"
#include "CInput.h"
#include "CBullet.h"

/*
プレイヤークラス
キャラクタスを継承
*/
class CPlayer : public CCharacter3
{
public:
	CPlayer() {}
	//CPlayer(位置,回転,スケール)
	CPlayer(const CVector& pos, const CVector& rot, const CVector& scale);
	//更新処理
	void Update();
private:
	CInput mInput;
};
#endif
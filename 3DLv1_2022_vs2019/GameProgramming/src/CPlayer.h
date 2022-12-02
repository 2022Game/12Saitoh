#ifndef CPLAYER_H
#define CPLAYER_H
//キャラクタスクラスのインクルード
#include "CCharacter3.h"
#include "CInput.h"
#include "CBullet.h"
#include "CColliderLine.h"
/*
プレイヤークラス
キャラクタスを継承
*/
class CPlayer : public CCharacter3
{
public:
	CPlayer();
	//CPlayer(位置,回転,スケール)
	CPlayer(const CVector& pos, const CVector& rot, const CVector& scale);
	//更新処理
	void Update();
private:
	CColliderLine mLine; //親分コライダ
	CColliderLine mLine2; //親分コライダ
	CColliderLine mLine3; //親分コライダ
	CInput mInput;
};
#endif
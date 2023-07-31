#ifndef PLAYER_H
#define PLAYER_H
//キャラクタスクラスのインクルード
#include "CCharacter3.h"
#include "CInput.h"
#include "CPlayerBullet.h"
#include "CColliderLine.h"
/*
プレイヤークラス
キャラクタスを継承
*/
class CPlayer : public CCharacter3
{
private:
	//プレイヤーのインスタンス
	static CPlayer* spInstance;

	CPlayerBullet* mpBullet;
	CCollider mCollider1;	//頭コライダ
	CCollider mCollider2;	//胴体コライダ
	CCollider mCollider3;	//下半身コライダ
	CInput mInput;

public:
	//コンストラクタ
	CPlayer();
	//CPlayer(位置,回転,スケール)
	CPlayer(const CVector& pos, const CVector& rot, const CVector& scale);
	//デストラクタ
	~CPlayer();

	//更新処理
	void Update();

	void Collision();
	//衝突処理
	void Collision(CCollider* o, CCollider* m);

	//インスタンスのポインタの取得
	static CPlayer* Instance();
};
#endif
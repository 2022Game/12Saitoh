#ifndef CENEMY_H
#define CENEMY_H
#include "CXCharacter.h"
#include "CCollider.h"
#include "CModel.h"

/*
エネミークラス
キャラクタクラスを継承
*/
class CEnemy : public CXCharacter
{
public:
	// インスタンスを取得
	static CEnemy* Instance();
	// コンストラクタ
	CEnemy();

	// 更新処理
	void Update();

private:
	static CEnemy* spInstance;
	CModel* mModel;
};

#endif

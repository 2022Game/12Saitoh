#ifndef CCHARACTER3_H
#define CCHARACTER3_H
//変換行列クラスのインクルード
#include "CTransform.h"
//モデルクラスのインクルード
#include "CModel.h"
#include "CTaskManager.h"
#include "CTask.h"
//コライダクラスの宣言
class CCollider;
class NavNode;

/*
キャラクタークラス
ゲームキャラクタの基本的な機能を定義する
*/
class CCharacter3 : public CTransform, public CTask
{
public:
	enum ETag
	{
		EZERO,          //初期値
		EPLAYER,        //プレイヤー
		EENEMY,         //敵
	};
	//タグの取得
	ETag Tag();
	//コンストラクタ
	CCharacter3();
	//コンストラクタ
	CCharacter3(int priority);
	//デストラクタ
	~CCharacter3();

	//モデルの設定
	//Model(モデルクラスのポインタ)
	void Model(CModel* m);
	//衝突処理
	virtual void Collision(CCollider* m, CCollider* o) {}
	//更新処理
	virtual void Update();
	//描画処理
	virtual void Render();

	NavNode* GetNavNode() const;

	//キャラクターが死んでいるかどうか
	bool IsDeath();

	//HPを取得
	int HP() const;
protected:
	int mHp;			//HP

	ETag mTag;			//タグ
	CVector mGravity;	//重力
	CModel* mpModel;	//モデルのポインタ
	NavNode* mpNode;	//自身の座標のノード
};
#endif
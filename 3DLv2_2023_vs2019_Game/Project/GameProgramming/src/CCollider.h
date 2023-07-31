#ifndef CCOLLIDER_H
#define CCOLLIDER_H
//キャラクタクラスのインクルード
#include "CCharacter3.h"

/*
コライダクラス
衝突判定データ
*/
class CCollisionManager;
class CCollider : public CTransform, public CTask
{
	friend CCollisionManager;
public:
	//優先度の変更
	virtual void ChangePriority();
	//優先度の変更
	void ChangePriority(int priorty);
	//CollisionTriangleLine(三角コライダ, 線分コライダ, 調整値)
	//retrun:true（衝突している）false(衝突していない)
	//調整値:衝突しない位置まで戻す値
	static bool CollisionTriangleLine(CCollider* triangle, CCollider* line, CVector* adjust, CVector* cross = nullptr);
	//CollisionTriangleLine(三角形頂点, 線分頂点, 調整値)
	//retrun:true（衝突している）false(衝突していない)
    //調整値:衝突しない位置まで戻す値
	static bool CollisionTriangleLine(CVector(&tv)[3], CVector(&lv)[2], CVector* adjust, CVector* cross = nullptr);

	//CollisionTriangleSphere(三角コライダ,球コライダ,調整値)
	// return:true(衝突している)false(衝突していない)
	//調整値:衝突していない位置まで戻す値
	static bool CollisionTriangleSphere(CCollider* triangle, CCollider* sphere,
		CVector* ajust);
	//コライダタイプ
	enum EType
	{
		ESPHERE,	//球コライダ
		ETRIANGLE,	//三角コライダ
		ELINE,		//線分コライダ
	};
	EType Type();
	//コライダータグ
	enum class EColliderTag
	{
		ENULL,			//初期
		EPLAYER,		//プレイヤー
		EENEMY,			//敵
		EALL,			//味方
		EPLAYERBULLET,	//プレイヤーの弾
		EENEMYBULLET,	//敵の弾
		EFLAG,			//フラッグ
	};
	EColliderTag ColliderTag();
	//デフォルトコンストラクタ
	CCollider();
	//衝突判定
	//Collision(コライダ1,コライダ2)
	//return::true(衝突している)false(衝突していない)
	static bool Collision(CCollider* m, CCollider* o);
	~CCollider();
	//コンストラクタ
	//CCollider(親、親行列、位置、半径)
	CCollider(CCharacter3* parent, CMatrix* matrix,
		const CVector& position, float radius, EColliderTag tag);
	//親ポインタの取得
	CCharacter3* Parent();
	//描画
	void Render();
protected:
	EType mType; //コライダタイプ
	EColliderTag mColliderTag;
	//頂点
	CVector mV[3];
	CCharacter3* mpParent;//親
	CMatrix* mpMatrix;//親行列
	float mRadius;//半径
};
#endif
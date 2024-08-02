#pragma once
#include "CObjectBase.h"
#include "CColliderMesh.h"
#include "CWeapon.h"
#include "CCollider.h"
#include "CNormalSwordEffect.h"
class CSword : public CWeapon
{
public:
	// コンストラクタ
	CSword();
	// デストラクタ
	~CSword();

	// 更新処理
	void Update();
	// 衝突処理
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit);
	// 描画処理
	void Render();

	/// <summary>
	/// 攻撃開始
	/// </summary>
	void AttackStart() override;
	/// <summary>
	/// 攻撃終了
	/// </summary>
	void AttackEnd() override;

	// 行列の取得
	CMatrix Matrix() const override;
private:
	CModel* mpSword;
	CColliderCapsule* mpSwordCollider;
	CNormalSwordEffect* mpSwordEffect;
};
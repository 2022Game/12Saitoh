#pragma once
#include "BaseSystem/CObjectBase.h"

class CWeapon : public CObjectBase
{
public:
	CWeapon();
	virtual ~CWeapon();

	void SetAttachMtx(const CMatrix* mtx);

	/// <summary>
	///  攻撃開始
	/// </summary>
	virtual void AttackStart();
	/// <summary>
	/// 攻撃終了
	/// </summary>
	virtual void AttackEnd();

protected:
	/// <summary>
	/// 攻撃がヒットしたオブジェクトを追加
	/// </summary>
	/// <param name="obj"></param>
	void AddAttackHitObj(CObjectBase* obj);
	/// <summary>
	/// 既に攻撃がヒットしているオブジェクトかどうか
	/// </summary>
	/// <param name="obj"></param>
	/// <returns></returns>
	bool IsAttackHitObj(CObjectBase* obj) const;

	const CMatrix* mpAttachMtx;
	// 攻撃がヒット済みのオブジェクトリスト
	std::list<CObjectBase*> mAttackHitObjects;
};

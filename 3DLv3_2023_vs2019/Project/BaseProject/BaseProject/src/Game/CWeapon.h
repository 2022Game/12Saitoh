#pragma once
#include "BaseSystem/CObjectBase.h"

class CWeapon : public CObjectBase
{
public:
	CWeapon();
	virtual ~CWeapon();

	void SetAttachMtx(const CMatrix* mtx);

	/// <summary>
	///  �U���J�n
	/// </summary>
	virtual void AttackStart();
	/// <summary>
	/// �U���I��
	/// </summary>
	virtual void AttackEnd();

protected:
	/// <summary>
	/// �U�����q�b�g�����I�u�W�F�N�g��ǉ�
	/// </summary>
	/// <param name="obj"></param>
	void AddAttackHitObj(CObjectBase* obj);
	/// <summary>
	/// ���ɍU�����q�b�g���Ă���I�u�W�F�N�g���ǂ���
	/// </summary>
	/// <param name="obj"></param>
	/// <returns></returns>
	bool IsAttackHitObj(CObjectBase* obj) const;

	const CMatrix* mpAttachMtx;
	// �U�����q�b�g�ς݂̃I�u�W�F�N�g���X�g
	std::list<CObjectBase*> mAttackHitObjects;
};

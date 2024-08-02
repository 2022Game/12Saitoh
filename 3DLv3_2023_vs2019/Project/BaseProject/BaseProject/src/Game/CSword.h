#pragma once
#include "CObjectBase.h"
#include "CColliderMesh.h"
#include "CWeapon.h"
#include "CCollider.h"
#include "CNormalSwordEffect.h"
class CSword : public CWeapon
{
public:
	// �R���X�g���N�^
	CSword();
	// �f�X�g���N�^
	~CSword();

	// �X�V����
	void Update();
	// �Փˏ���
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit);
	// �`�揈��
	void Render();

	/// <summary>
	/// �U���J�n
	/// </summary>
	void AttackStart() override;
	/// <summary>
	/// �U���I��
	/// </summary>
	void AttackEnd() override;

	// �s��̎擾
	CMatrix Matrix() const override;
private:
	CModel* mpSword;
	CColliderCapsule* mpSwordCollider;
	CNormalSwordEffect* mpSwordEffect;
};
#pragma once
#include "CObjectBase.h"
#include "CColliderMesh.h"
#include "CWeapon.h"
#include "CCollider.h"
#include "CNormalSwordEffect.h"
#include "CAirAttackEffect.h"
#include "CCounterEffect.h"
#include "CCounterEffect2.h"


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
	// �G�t�F�N�g���쐬
	void CreateEffect(const CHitInfo hit);

	CModel* mpSword;
	CColliderCapsule* mpSwordCollider;
	CNormalSwordEffect* mpSwordEffect;
	CAirAttackEffect* mpAirEffect;
	CCounterEffect* mpCounterEffect1;
	CCounterEffect2* mpCounterEffect2;
};
#ifndef CDRAGON_H
#define CDRAGON_H
#include "DragonData.h"
#include "CXCharacter.h"
#include "CCollider.h"
/*
�h���S���N���X
�L�����N�^�N���X���p��
*/
class CDragon : public CXCharacter
{
public:
	// �C���X�^���X���擾
	static CDragon* Instance();
	// �R���X�g���N�^
	CDragon();
	// �f�X�g���N�^
	~CDragon();

	// �X�V����
	void Update();
	// �Փˏ���
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	// �`�揈��
	void Render();

	// �_���[�W����
	void TakeDamage(int damage)override;
	/// <summary>
	/// �U���J�n
	/// </summary>
	void AttackStart() override;
	/// <summary>
	/// �U���I��
	/// </summary>
	void AttackEnd() override;

private:
	// �ҋ@���
	void UpdateIdle();
	// �퓬���
	void UpdateBattle();
	// �U������
	// UpdateBattle�̒��ōs��
	void UpdateAttack();

	// �A�j���[�V�����̐؂�ւ�
	void ChangeAnimation(EDragonAnimType type);

	// �v���C���[�����������ǂ���
	bool IsFoundPlayer() const;

	// �G�̏��
	enum class EState
	{
		None = -1,

		eIdle,	// �ҋ@
		eBattle,// �퓬
		eDeath,	// ���S
	};
	EState mState;	// �G�̏��

	// �C���X�^���X
	static CDragon* spInstance;

	CColliderLine* mpColliderLine;	// �n�ʂƂ̓����蔻��p
	CColliderSphere* mpBodyCol;		//�����߂��p�R���C�_�[
	CColliderSphere* mpDamageCol;	// �_���[�W�p�R���C�_�[
	CColliderSphere* mpAttackMouthCol;// ���݂��U���p�R���C�_�[

	CVector mMoveSpeed;	// �ړ����x
	bool mIsGrounded;	// �ڒn���Ă��邩�ǂ���
	bool mIsAngry;		// �{���Ԃ��ǂ���

	int mAngryStandardValue;// �{��l�̊�l
	int mAngryValue;	// �{��l

	float mAngryElapsedTime;// �{��o�ߎ��Ԍv����
	float mElapsedTime;		// �o�ߎ��Ԍv����

};
#endif
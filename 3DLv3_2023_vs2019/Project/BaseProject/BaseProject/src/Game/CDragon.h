#ifndef CDRAGON_H
#define CDRAGON_H
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
private:
	// �ҋ@���
	void UpdateIdle();

	// �v���C���[�����������ǂ���
	bool IsFoundPlayer() const;

	// �A�j���[�V�����̎��
	enum class EAnimType
	{
		None = -1,
		
		eIdle1,			// �ҋ@1
		eIdle2,			// �ҋ@2
		eLand,			// ���n
		eRun,			// ����
		eWalk,			// ����
		eScream,		// ���K
		eSleep,			// �Q��
		eTakeOff,		// ����
		eAttackFlame,	// �u���X�U��
		eAttackHand,	// ��ъ|����U��
		eAttackMouth,	// ���݂��U��
		eDie,			// ���S
		eFlyFlame,		// �󒆃u���X�U��
		eFlyForward,	// �󒆑O�i
		eFlyGlide,		// �󒆊���
		eFlyIdle,		// �󒆃A�C�h��
		eGetHit,		// �̂�����

		Num
	};
	// �A�j���[�V�����̐؂�ւ�
	void ChangeAnimation(EAnimType type);
	// �A�j���[�V�����̃f�[�^�e�[�u��
	struct AnimData
	{
		std::string path;	// �A�j���[�V�����f�[�^�p�X
		bool loop;			// ���[�v���邩�ǂ���
		float frameLength;	// �A�j���[�V�����̃t���[����
		float motionValue;	// ���[�V�����l
	};
	// �G�̏��
	enum class EState
	{
		None = -1,

		eIdle,	// �ҋ@
		eMove,	// �ړ�
		eAttack,// �U��
		eDeath,	// ���S
	};
	EState mState;	// �G�̏��

	// �A�j���[�V�����f�[�^�e�[�u��
	static const AnimData ANIM_DATA[];
	static CDragon* spInstance;
};
#endif
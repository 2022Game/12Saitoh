#ifndef CENEMY_H
#define CENEMY_H
#include "CXCharacter.h"
#include "CCollider.h"
#include "CModel.h"

/*
�G�l�~�[�N���X
�L�����N�^�N���X���p��
*/
class CEnemy : public CXCharacter
{
public:
	// �C���X�^���X���擾
	static CEnemy* Instance();
	// �R���X�g���N�^
	CEnemy();

	// �X�V����
	void Update();

	// �Փˏ���
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	// �`�揈��
	void Render();

private:
	// �R���C�_�[�̍X�V����
	void ColliderUpdate();

	// �A�j���[�V�����̎��
	enum class EAnimType
	{
		None = -1,

		eTPose, // T�|�[�Y
		eIdle,	// �A�C�h��

		Num
	};

	// �A�j���[�V�����̐؂�ւ�
	void ChangeAnimation(EAnimType type);
	// �A�j���[�V�����f�[�^�e�[�u��
	struct AnimData
	{
		std::string path;	// �A�j���[�V�����f�[�^�p�X
		bool loop;			// ���[�v���邩�ǂ���
		float frameLength;	// �A�j���[�V�����̃t���[����
		float motionValue;	// ���[�V�����l
	};

	static const AnimData ANIM_DATA[];

	static CEnemy* spInstance;
	CModel* mModel;

	CColliderSphere* mpHeadCol;
};
#endif

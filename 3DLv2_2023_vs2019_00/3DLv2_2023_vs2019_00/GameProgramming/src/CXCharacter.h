#ifndef CXCHARACTER_H
#define CXCHARACTER_H

#include "CModelX.h"
#include "CMaterial.h"
#include "CCharacter3.h"

class CXCharacter : public CCharacter3
{
protected:
	CModelX* mpModel;			//���f���f�[�^

	bool mAnimationLoopFlg;		//true:�A�j���[�V�������J��Ԃ�

	int mAnimationIndex;		//�A�j���[�V�����ԍ�

	float mAnimationFrame;		//�A�j���[�V�����̍Đ��t���[��
	float mAnimationFrameSize;	//�A�j���[�V�����̍Đ��t���[����

public:
	//�R���X�g���N�^
	CXCharacter();
	//����������
	void Init(CModelX* model);
	//�A�j���[�V�����̕ύX
	void ChangeAnimation(int index, bool loop, float framesize);
	//�X�V����
	void Update(CMatrix& m);
	void Update();
	//�`�揈��
	void Render();
	//�A�j���[�V�����̍Đ��I������
	//true:�I�� false:�Đ���
	bool IsAnimationFinished();
	int AnimationIndex();	//�A�j���[�V�����̔ԍ��̎擾
};
#endif
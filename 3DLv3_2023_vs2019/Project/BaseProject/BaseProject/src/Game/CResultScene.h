#pragma once
#include "CTask.h"
#include "CImage.h"

// ���U���g�V�[���̃x�[�X�N���X
class CResultScene : public CTask
{
public:
	// �C���X�^���X���擾
	static CResultScene* Instance();
	// �R���X�g���N�^
	CResultScene();
	// �f�X�g���N�^
	~CResultScene();

	// �X�V����
	void Update() override;
	// �`�揈��
	void Render() override;

	// �J�n����
	void Start();
	// �I������
	void End();

	// ���U���g�V�[���Đ������ǂ����擾
	bool IsPlayResult();
private:
	// �C���X�^���X
	static CResultScene* mpInstance;
	// ���U���g�V�[���Đ������ǂ���
	bool mIsPlay;
	// ���U���g�V�[���̒i�K
	int mResultStep;
	// �o�ߎ��Ԍv����
	float mElapsedTime;

	// ���U���g�p�w�i�C���[�W
	CImage* mpBackImage;
};
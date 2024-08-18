#include "CResultScene.h"
#include "CTaskManager.h"
#include "CInput.h"
#include "CGameOverUI.h"

// �R���X�g���N�^
CResultScene::CResultScene()
	: CTask(ETaskPriority::eMenu, 0, ETaskPauseType::eNone)
	, mIsPlay(false)
	, mResultStep(0)
	, mElapsedTime(0.0f)
{
	// ���U���g��ʂ̔w�i�p�̔��C���[�W��ǂݍ���
	mpBackImage = new CImage
	(
		"UI/white.png",
		ETaskPriority::eUI, 0,
		ETaskPauseType::eResult,
		true, false
	);
	// ���w�i�ɕύX����
	// �ŏ��͔�\���̂܂܂Ȃ̂ŃA���t�@�l��0
	mpBackImage->SetColor(0.0f, 0.0f, 0.0f, 0.0f);	
	// �w�i�C���[�W�͉�ʑS�̂ɕ\��
	mpBackImage->SetPos(0.0f, 0.0f);
	mpBackImage->SetSize(WINDOW_WIDTH, WINDOW_HEIGHT);

	SetEnable(false);
	SetShow(false);
}

// �f�X�g���N�^
CResultScene::~CResultScene()
{
	SAFE_DELETE(mpBackImage);
	// �|�[�Y������
	CTaskManager::Instance()->UnPause(PAUSE_GAME);
}

// �X�V����
void CResultScene::Update()
{
	//�w�i�̕\������\���Ɋ|���鎞�ԁi�b�j
	static const float Time = 0.25f;
	//�w�i�̍ő�A���t�@�l
	static const float maxAlpha = 0.75f;

	// ���U���g�V�[���̏����i�K
	switch (mResultStep)
	{
	case 0: // �w�i��\��
		// �w�i�̕\�����Ԃ��o�߂��Ă��Ȃ�
		if (mElapsedTime < Time)
		{
			// �o�ߎ��Ԃɍ��킹�ăA���t�@�l��ݒ�
			float alpha = mElapsedTime / Time;
			mpBackImage->SetAlpha(maxAlpha * alpha);
			// 1�t���[���̌o�ߎ��Ԃ����Z
			mElapsedTime += Time::DeltaTime();
		}
		// �\�����Ԃ��o��
		else
		{
			// �w�i�����S�ɕ\�����āA���̃X�e�b�v��
			mpBackImage->SetAlpha(maxAlpha);
			mElapsedTime = 0.0f;
			mResultStep++;
		}
		break;
	case 1: // ���U���g��ʒ��͉������Ȃ�
		break;
	case 2: // ���U���g��ʏI������
		if (mElapsedTime < Time)
		{
			float alpha = mElapsedTime / Time;
			// ��\���ɂ��Ă���
			mpBackImage->SetAlpha(Time * (1.0f - alpha));
			mElapsedTime += Time::DeltaTime();
		}
		else
		{
			SetEnable(false);
			SetShow(false);

			// �A���t�@�l��0�Ŕ�\��
			mpBackImage->SetAlpha(0.0f);
			mResultStep = 0;
			mElapsedTime = 0.0f;
			// �Đ����̃t���O�����낷
			mIsPlay = false;
			// �J�[�\���\���𖳌��ɂ���
			CInput::ShowCursor(false);
			// �|�[�Y������
			CTaskManager::Instance()->UnPause(PAUSE_GAME);
			// �Q�[���J�������ĊJ
			CCamera::MainCamera()->SetEnable(true);
		}
		break;
	}
}

// �`�揈��
void CResultScene::Render()
{
	// �w�i�摜��ǂݍ���ł�����`��
	if (mpBackImage != nullptr)
	{
		mpBackImage->Render();
	}
}

// �J�n����
void CResultScene::Start()
{
	SetEnable(true);
	SetShow(true);
	// �Đ����̃t���O�𗧂Ă�
	mIsPlay = true;
	// �|�[�Y��ݒ�
	CTaskManager::Instance()->Pause(PAUSE_GAME);
	// �J�[�\���\����L���ɂ���
	CInput::ShowCursor(true);
	// �Q�[���J��������~
	CCamera::MainCamera()->SetEnable(false);
}

// �I������
void CResultScene::End()
{
	// ���U���g��ʂ̏����i�K���I�������֐i�߂�
	mResultStep++;
}

// ���U���g�V�[���Đ������ǂ����擾
bool CResultScene::IsPlayResult()
{
	return mIsPlay;
}
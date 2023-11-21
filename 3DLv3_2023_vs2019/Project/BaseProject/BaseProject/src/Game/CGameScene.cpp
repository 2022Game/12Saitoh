#include "CGameScene.h"
#include "CSceneManager.h"
#include "CField.h"
#include "CPlayer.h"
#include "CCamera.h"
#include "CSword.h"

//�R���X�g���N�^
CGameScene::CGameScene()
	: CSceneBase(EScene::eGame)
{
}

//�f�X�g���N�^
CGameScene::~CGameScene()
{
}

//�V�[���ǂݍ���
void CGameScene::Load()
{
	// �w�i�F�ݒ�
	System::SetClearColor(0.1921569f, 0.3019608f, 0.4745098f, 1.0f);
	//�����ŃQ�[�����ɕK�v��
	//���\�[�X�̓ǂݍ��݂�N���X�̐������s��

	//�t�B�[���h�𐶐�
	new CField();

	//�v���C���[�𐶐�
	CPlayer* player = new CPlayer();
	player->Scale(10.0f, 10.0f, 10.0f);
	player->Rotate(0.0f, 180.0f, 0.0f);

	//���𐶐�
	CSword* sword = new CSword();
	sword->Rotate(CVector(0.0f, 0.0f, 0.0f));

	//�J�����Z�b�g
	CCamera* mainCamera = new CCamera
	(
		CVector(0.0f, 20.0f, 50.0f),
		player->Position()
	);
	//���C���J�����̒Ǐ]�^�[�Q�b�g���v���C���[�ɐݒ�
	mainCamera->SetFollowTargetTf(player);
}

//�V�[���̍X�V����
void CGameScene::Update()
{
}

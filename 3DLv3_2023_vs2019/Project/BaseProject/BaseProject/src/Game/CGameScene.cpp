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
	//sword->SetAttachMtx(player->GetFrameMtx("Armature_sword_holder"));

	//�J�����Z�b�g
	CCamera* mainCamera = new CCamera
	(
		//CVector(5.0f, -15.0f, 180.0f),
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

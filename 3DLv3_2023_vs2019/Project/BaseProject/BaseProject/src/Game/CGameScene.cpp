#include "CGameScene.h"
#include "CSceneManager.h"
#include "CField.h"
#include "CPlayer.h"
#include "CEnemy.h"
#include "CGameCamera.h"
#include "CInput.h"
#include "CGameMenu.h"

//�R���X�g���N�^
CGameScene::CGameScene()
	: CSceneBase(EScene::eGame)
	, mpGameMenu(nullptr)
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

	CResourceManager::Load<CModel>("Field", "Field\\field.obj");
	CResourceManager::Load<CModel>("FieldCamCol", "Field\\fieldCamCol.obj");
	CResourceManager::Load<CModel>("FieldCube", "Field\\Object\\cube.obj");
	CResourceManager::Load<CModel>("FieldCylinder", "Field\\Object\\cylinder.obj");
	CResourceManager::Load<CModelX>("Player", "Character\\Player\\player.x");
	CResourceManager::Load<CModel>("Sword", "Character\\Sword\\sword.obj");
	CResourceManager::Load<CModelX>("Enemy", "Character\\Enemy\\Dragon.x");
	CResourceManager::Load<CTexture>("Laser", "Effect\\laser.png");

	//�t�B�[���h�𐶐�
	CField * field = new CField();

	//�v���C���[�𐶐�
	CPlayer* player = new CPlayer();
	player->Scale(10.0f, 10.0f, 10.0f);
	player->Rotate(0.0f, 180.0f, 0.0f);

	// �G�𐶐�
	CEnemy* enemy = new CEnemy();
	enemy->Position(-50.0f, 0.0f, -50.0f);
	enemy->Scale(20.0f, 20.0f, 20.0f);

	////�J�����Z�b�g
	//CGameCamera* mainCamera = new CGameCamera
	//(
	//	CVector(0.0f, 30.0f, 40.0f),
	//	player->Position()
	//);
	
	// CGameCamera�̃e�X�g
	CVector atPos = player->Position() + CVector(0.0f, 10.0f, 0.0f);
	CGameCamera* mainCamera = new CGameCamera
	(
		atPos + CVector(0.0f, 0.0f, 40.0f),
		atPos
	);

	//���C���J�����̒Ǐ]�^�[�Q�b�g���v���C���[�ɐݒ�
	mainCamera->SetFollowTargetTf(player);
	// ���C���J�����̏Փ˔���Ƀt�B�[���h�̃R���C�_�[��ǉ�
	mainCamera->AddCollider(field->GetFieldCol());
	// �Q�[�����j���[���쐬
	mpGameMenu = new CGameMenu();

	//CLineEffect* le = new CLineEffect(ETag::eNone);
//le->AddPoint(CVector(0.0f, 10.0f, 10.0f), 1.0f);
//le->AddPoint(CVector(10.0f, 10.0f, 10.0f), 1.0f);
//le->AddPoint(CVector(10.0f, 20.0f, 50.0f), 1.0f);
//le->AddPoint(CVector(50.0f, 10.0f, 50.0f), 1.0f);

}

//�V�[���̍X�V����
void CGameScene::Update()
{
	if (CInput::PushKey('T'))
	{
		CSceneManager::Instance()->LoadScene(EScene::eTitle);
	}

	// �Q�[�����j���[���J���ĂȂ���΁A[�l]�L�[�Ń��j���[���J��
	if (!mpGameMenu->IsOpened())
	{
		if (CInput::PushKey('M'))
		{
			mpGameMenu->Open();
		}
	}
}
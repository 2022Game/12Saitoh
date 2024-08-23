#include "CGameScene.h"
#include "CSceneManager.h"
#include "CGameCamera.h"
#include "CPlayer.h"
#include "CDragon.h"
#include "CField.h"
#include "CInput.h"
#include "CGameMenu.h"
#include "CBGMManager.h"
#include "Global.h"
#include "CGameOverScene.h"
#include "CGameClearScene.h"
#include "CBGMManager.h"
#include "CSky.h"
#include "CGameUI.h"

CField* gField = nullptr;

//�R���X�g���N�^
CGameScene::CGameScene()
	: CSceneBase(EScene::eGame)
	, mElapsedTime(0.0f)
	, mpGameMenu(nullptr)
	, mpGameOver(nullptr)
	, mpGameClear(nullptr)
{
}

//�f�X�g���N�^
CGameScene::~CGameScene()
{
}

//�V�[���ǂݍ���
void CGameScene::Load()
{
	// �Q�[����ʂ̓J�[�\����\��
	CInput::ShowCursor(false);
	// �w�i�F�ݒ�
	System::SetClearColor(0.1921569f, 0.3019608f, 0.4745098f, 1.0f);

	//�����ŃQ�[�����ɕK�v��
	//���\�[�X�̓ǂݍ��݂�N���X�̐������s��
	CResourceManager::Load<CModel>("Field", "Field\\Coliseum\\Coliseum.obj");
	CResourceManager::Load<CModel>("Cylinder", "Field\\Coliseum\\Cylinder.obj");
	CResourceManager::Load<CModel>("Sky", "Field\\sky.obj");
	CResourceManager::Load<CModel>("Cylinder", "Field\\Coliseum\\Cylinder.obj");
	CResourceManager::Load<CModelX>("Player", "Character\\Player\\player.x");
	CResourceManager::Load<CModel>("Sword", "Character\\Sword\\sword.obj");
	CResourceManager::Load<CModelX>("Dragon", "Character\\Dragon\\Dragon.x");	
	CResourceManager::Load<CTexture>("NormalSwordEffect", "Effect\\NormalAttack.png");
	// SE�̓ǂݍ���
	// �v���C���[
	// �ړ�
	CResourceManager::Load<CSound>("RunSE", "Sound\\SE\\Player\\run.wav");
	CResourceManager::Load<CSound>("FastRunSE", "Sound\\SE\\Player\\fastrun.wav");
	// ���[��
	CResourceManager::Load<CSound>("SheathedSE", "Sound\\SE\\Player\\sheathed.wav");
	CResourceManager::Load<CSound>("DrawnSE", "Sound\\SE\\Player\\drawn.wav");
	// �ʏ�E�󒆍U��
	CResourceManager::Load<CSound>("NormalAttackSE1", "Sound\\SE\\Player\\normalattack1.wav");
	CResourceManager::Load<CSound>("NormalAttackSE2", "Sound\\SE\\Player\\normalattack2.wav");
	CResourceManager::Load<CSound>("JumpAttackSE", "Sound\\SE\\Player\\jumpattack.wav");
	// �㓬�Z
	CResourceManager::Load<CSound>("CounterSE1", "Sound\\SE\\Player\\counter1.wav");
	CResourceManager::Load<CSound>("CounterSE2", "Sound\\SE\\Player\\counter3.wav");
	// �����Z
	CResourceManager::Load<CSound>("SpMoveCutinSE", "Sound\\SE\\Player\\cutin.wav");
	CResourceManager::Load<CSound>("SpMoveSE", "Sound\\SE\\Player\\spmove.wav");
	CResourceManager::Load<CSound>("SpMoveENDSE", "Sound\\SE\\Player\\spmove_finish.wav");

	// �h���S��
	CResourceManager::Load<CSound>("Breath", "Sound\\SE\\Dragon\\breath.wav");
	CResourceManager::Load<CSound>("FlyBreath", "Sound\\SE\\Dragon\\flybreath.wav");
	CResourceManager::Load<CSound>("Fly", "Sound\\SE\\Dragon\\fly.wav");
	CResourceManager::Load<CSound>("Scream", "Sound\\SE\\Dragon\\scream.wav");
	CResourceManager::Load<CSound>("Maouth1", "Sound\\SE\\Dragon\\maouth1.wav");
	CResourceManager::Load<CSound>("Maouth2", "Sound\\SE\\Dragon\\maouth2.wav");
	CResourceManager::Load<CSound>("Jump", "Sound\\SE\\Dragon\\jump.wav");

	// �Q�[��BGM��ǂݍ���
	mpGameBGM = CResourceManager::Load<CSound>("GameBGM", "Sound\\BGM\\battle_bgm.wav");
	mpGameBGM2 = CResourceManager::Load<CSound>("GameBGM2", "Sound\\BGM\\battle2_bgm.wav");
	mpGameClearBGM = CResourceManager::Load<CSound>("ClearBGM", "Sound\\BGM\\clear_bgm.wav");
	mpGameOverBGM = CResourceManager::Load<CSound>("OverBGM", "Sound\\BGM\\over_bgm.wav");
	mpNature = CResourceManager::Load<CSound>("Nature", "Sound\\BGM\\nature.wav");

	//�t�B�[���h�𐶐�
	gField = new CField();
	CSky* sky = new CSky();
	sky->Scale(25.0f, 25.0f, 25.0f);
	sky->Rotate(70.0f, 0.0f, 40.0f);
	//�v���C���[�𐶐�
	CPlayer* player = new CPlayer();
	player->Scale(10.0f, 10.0f, 10.0f);
	player->Rotate(0.0f, 180.0f, 0.0f);
	player->Position(0.0f, 50.0f, 200.0f);

	// �h���S��(�G)�𐶐�
	CDragon* dragon = new CDragon();
	dragon->Position(-20.0f, 50.0f, -400.0f);
	dragon->Scale(0.14f, 0.14f, 0.14f);

	//�J�����Z�b�g
	CVector atPos = player->Position() + CVector(0.0f, 10.0f, 0.0f);
	CGameCamera* mainCamera = new CGameCamera
	(
		atPos + CVector(0.0f, 0.0f, 40.0f),
		atPos
	);
	//���C���J�����̒Ǐ]�^�[�Q�b�g���v���C���[�ɐݒ�
	mainCamera->SetFollowTargetTf(player);
	// ���C���J�����̏Փ˔���Ƀt�B�[���h�̃R���C�_�[��ǉ�
	mainCamera->AddCollider(gField->GetFieldCol());

	// �Q�[�����j���[���쐬
	mpGameMenu = new CGameMenu();

	// ���U���g�V�[�����쐬
	mpGameOver = new CGameOverScene();
	mpGameClear = new CGameClearScene();

	// �Q�[���pUI���쐬
	CGameUI* mpUI = new CGameUI();

	//CLineEffect* le = new CLineEffect(ETag::eNone);
//le->AddPoint(CVector(0.0f, 10.0f, 10.0f), 1.0f);
//le->AddPoint(CVector(10.0f, 10.0f, 10.0f), 1.0f);
//le->AddPoint(CVector(10.0f, 20.0f, 50.0f), 1.0f);
//le->AddPoint(CVector(50.0f, 10.0f, 50.0f), 1.0f);

}

//�V�[���̍X�V����
void CGameScene::Update()
{
	if (!mpNature->IsPlaying())
	{
		// �������Đ�
		mpNature->PlayLoop(-1, true, 0.09f, 0.0f);
	}

	CDragon* dragon = CDragon::Instance();
	if (dragon->IsFoundPlayer())
	{
		// BGM�Đ����łȂ���΁ABGM���Đ�
		if (!mpGameBGM->IsPlaying())
		{
			mpGameBGM->PlayLoop(-1, true, 0.15f, false, 0.0f);
			mpNature->SetVolume(0.03);
		}
	}

	// 2��ڂ̋󒆃u���X����܂ł�BGM�̏���
	if (dragon->SpAttackNum() <= 1)
	{
		// �󒆃u���X����BGM�̉��ʂ�}����
		if (dragon->IsFlyBreath())
		{
			mpGameBGM->SetVolume(0.06f);
		}
		else
		{
			mpGameBGM->SetVolume(0.15);
		}
	}
	// 2��ڈȍ~�̏���
	else
	{
		// �󒆃u���X����BGM�̉��ʂ�}����
		if (dragon->IsFlyBreath())
		{
			mpGameBGM->SetVolume(0.05f);
			mpGameBGM2->PlayLoop(-1, true, 0.0f);
		}
		else
		{
			if (mpGameBGM2->IsPlaying())
			{
				mpGameBGM2->SetVolume(0.15f);
				mpGameBGM->Stop();
			}
		}
	}

	CPlayer* player = CPlayer::Instance();
	// �v���C���[�̎��S�t���O����������
	// 5�b��ɃQ�[���I�[�o�[�V�[���ֈȍ~
	if (player->IsDie())
	{
		mElapsedTime += Time::DeltaTime();
		if (mElapsedTime >= 4.0f)
		{
			mpGameOver->Start();
		}
		// �Q�[���I�[�o�[BGM���Đ�����Ă��Ȃ����
		// BGM���Đ�
		if (!mpGameClearBGM->IsPlaying())
		{
			mpGameOverBGM->PlayLoop(-1, true, 0.2f);
			mpGameBGM->Stop();
			mpGameBGM2->Stop();
		}
	}
	// �h���S���̎��S�t���O����������
	// 15�b��ɃQ�[���I�[�o�[�V�[���ֈȍ~
	if (dragon->IsDie())
	{
		mElapsedTime += Time::DeltaTime();
		if (mElapsedTime >= 15.0f)
		{
			mpGameClear->Start();
			mpGameBGM2->Stop();
		}
		// �Q�[���N���ABGM���Đ�����Ă��Ȃ����
		// BGM���Đ�
		if (!mpGameClearBGM->IsPlaying())
		{
			mpGameClearBGM->PlayLoop(-1, true, 0.2f);
			mpGameBGM->Stop();
			mpGameBGM2->Stop();
		}
	}

	// �Q�[�����j���[���J���ĂȂ���΁A[�l]�L�[�Ń��j���[���J��
	//if (!mpGameMenu->IsOpened())
	//{
	//	if (CInput::PushKey('M'))
	//	{
	//		mpGameMenu->Open();
	//	}
	//}
#ifdef _DEBUG
	// ���U���g�V�[��
	if (!mpGameOver->IsPlayResult())
	{
		if (CInput::PushKey('O'))
		{
			mpGameOver->Start();
		}
	}
	else
	{
		if (CInput::PushKey('O'))
		{
			mpGameOver->End();
		}
	}

	if (!mpGameClear->IsPlayResult())
	{
		if (CInput::PushKey('I'))
		{
			mpGameClear->Start();
		}
	}
	else
	{
		if (CInput::PushKey('I'))
		{
			mpGameClear->End();
		}
	}
#endif
}
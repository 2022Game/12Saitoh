#pragma once

// �L�����X�e�[�^�X
struct CharaStatus
{
	int level;	// ���x��
	int hp;		// HP
	int atk;	// �U����
	int def;	// �h���
	float sp;		// �X�^�~�i
	int touki;	// ���C�Q�[�W
};

// �G�̍ő僌�x��
#define ENEMY_LEVEL_MAX 5

// �������p
extern const CharaStatus DEFAULT[];

// �v���C���[�̃X�e�[�^�X�e�[�u��
extern const CharaStatus PLAYER_STATUS[];
// �G�̃X�e�[�^�X�e�[�u��
extern const CharaStatus ENEMY_STATUS[ENEMY_LEVEL_MAX];
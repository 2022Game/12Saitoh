#include "CharaStatus.h"

// �������p�X�e�[�^�X
const CharaStatus DEFAULT[] =
{
	{0,0,0,0,0,0}
};

// �v���C���[�X�e�[�^�X
const CharaStatus PLAYER_STATUS[] =
{
	//���x��	  HP		�U����		�h���	�X�^�~�i	���C�Q�[�W
	{0,			 100,		 100,		100,	  100,		0},
};

// �G�X�e�[�^�X
const CharaStatus ENEMY_STATUS[ENEMY_LEVEL_MAX] = 
{
	//���x��	  HP		�U����		�h���
	{1,			10000,		100,		100},
	{2,			15000,		110,		100},
	{3,			20000,		120,		100},
	{4,			25000,		130,		100},
	{5,			30000,		140,		100},
};
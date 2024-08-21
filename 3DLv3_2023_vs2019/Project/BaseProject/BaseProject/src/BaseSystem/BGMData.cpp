#include "BGMData.h"

// BGM�f�[�^�̃e�[�u��
const BGMData BGM_DATA[] =
{
	//BGM�̎��			���\�[�X�p�X					�x�[�X����		���[�v	���[�v�J�n	���[�v�I��
	{ EBGMType::eTitle,	"Sound\\BGM\\title_bgm.wav",	0.4000f,		true,		0,			0		},
	{ EBGMType::eGame,	"Sound\\BGM\\battle_bgm.wav",	0.1500f,		true,		0,			0		},
	{ EBGMType::eMenu,	"Sound\\BGM\\menu.wav",			0.0625f,		true,		0,			0		},
	{ EBGMType::eClear, "Sound\\BGM\\clear_bgm.wav",	0.4000f,		true,		0,			0		},
};

// BGM�f�[�^��
const int BGM_DATA_COUNT = ARRAY_SIZE(BGM_DATA);
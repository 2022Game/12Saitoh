#include "BGMData.h"

// BGMデータのテーブル
const BGMData BGM_DATA[] =
{
	//BGMの種類			リソースパス					ベース音量		ループ	ループ開始	ループ終了
	{ EBGMType::eTitle,	"Sound\\BGM\\title_bgm.wav",	0.4000f,		true,		0,			0		},
	{ EBGMType::eGame,	"Sound\\BGM\\battle_bgm.wav",	0.1500f,		true,		0,			0		},
	{ EBGMType::eMenu,	"Sound\\BGM\\menu.wav",			0.0625f,		true,		0,			0		},
	{ EBGMType::eClear, "Sound\\BGM\\clear_bgm.wav",	0.4000f,		true,		0,			0		},
};

// BGMデータ数
const int BGM_DATA_COUNT = ARRAY_SIZE(BGM_DATA);
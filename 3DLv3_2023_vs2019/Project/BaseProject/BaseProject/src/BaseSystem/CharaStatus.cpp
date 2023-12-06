#include "CharaStatus.h"

// 初期化用ステータス
const CharaStatus DEFAULT[] =
{
	{0,0,0,0,0,0}
};

// プレイヤーステータス
const CharaStatus PLAYER_STATUS[] =
{
	//レベル	  HP		攻撃力		防御力	スタミナ	闘気ゲージ
	{0,			 100,		 100,		100,	  100,		0},
};

// 敵ステータス
const CharaStatus ENEMY_STATUS[ENEMY_LEVEL_MAX] = 
{
	//レベル	  HP		攻撃力		防御力
	{1,			10000,		100,		100},
	{2,			15000,		110,		100},
	{3,			20000,		120,		100},
	{4,			25000,		130,		100},
	{5,			30000,		140,		100},
};
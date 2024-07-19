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
	{0,			 100,		 100,		100,	  100,		300},
};

// 敵ステータス
const CharaStatus ENEMY_STATUS[] = 
{
	//レベル	  HP		攻撃力		防御力
	{1,			10000,		100,		100},
	{2,			12500,		120,		110},
	{3,			15000,		130,		120},
	{4,			17500,		140,		120},
	{5,			20000,		150,		120},
};
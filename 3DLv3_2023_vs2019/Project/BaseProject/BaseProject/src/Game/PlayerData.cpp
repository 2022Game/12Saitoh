#include "PlayerData.h"

// プレイヤーのアニメーションデータテーブル
std::vector<PlayerData::AnimData> PlayerData::ANIM_DATA = 
{
	{ "",													true,	0.0f	},	// Tポーズ
	{ "Character\\Player\\anim\\idle.x",					true,	601.0f	},	// 待機(納刀)
	{ "Character\\Player\\anim\\idle_combat.x",				true,	601.0f	},	// 待機(抜刀)
	{ "Character\\Player\\anim\\idleair.x",					true,	20.0f	},	// 空中待機(納刀
	{ "Character\\Player\\anim\\idleair_combat.x",			true,	20.0f	},	// 空中待機(抜刀)
	{ "Character\\Player\\anim\\idle_drawn_combat.x",		false,	101.0f	},	// 待機中抜刀動作
	{ "Character\\Player\\anim\\idle_sheathed_combat.x",	false,	81.0f	},	// 待機中納刀動作
	{ "Character\\Player\\anim\\run_start.x",				false,	36.0f	},	// 走り開始(納刀)
	{ "Character\\Player\\anim\\run_loop.x",				true,	40.0f	},	// 走り(納刀)
	{ "Character\\Player\\anim\\run_end.x",					false,	50.0f	},	// 走り終了(納刀)
	{ "Character\\Player\\anim\\run_combat_start.x",		false,	36.0f	},	// 走り開始(抜刀)
	{ "Character\\Player\\anim\\run_combat.x",				true,	40.0f	},	// 走り(抜刀)
	{ "Character\\Player\\anim\\run_combat_end.x",			false,	50.0f	},	// 走り終了(抜刀)
	{ "Character\\Player\\anim\\run_drawn_combat.x",		false,	40.0f	},	// 走り中抜刀動作
	{ "Character\\Player\\anim\\run_sheathed_combat.x",		false,	40.0f	},	// 走り中納刀動作
	{ "Character\\Player\\anim\\fastrun_start.x",			false,	11.0f	},	// ダッシュ開始
	{ "Character\\Player\\anim\\fastrun_loop.x",			true,	28.0f	},	// ダッシュ	
	{ "Character\\Player\\anim\\fastrun_end.x",				false,	52.0f	},	// ダッシュ終了
	{ "Character\\Player\\anim\\roll_start.x",				false,	25.0f	},	// 回避動作開始(納刀)
	{ "Character\\Player\\anim\\roll_end_idle.x",			false,	25.0f	},	// 回避動作からアイドルへ移行(納刀)
	{ "Character\\Player\\anim\\roll_end_run.x",			false,	20.0f	},	// 回避動作から走りへ移行(納刀)
	{ "Character\\Player\\anim\\roll_combat.x",				false,	25.0f	},	// 回避動作開始(抜刀)
	{ "Character\\Player\\anim\\roll_end_combat_idle.x",	false,	20.0f	},	// 回避動作からアイドルへ移行(抜刀)
	{ "Character\\Player\\anim\\roll_end_combat_run.x",		false,	19.0f	},	// 回避動作から走りへ移行(抜刀)
	{ "Character\\Player\\anim\\run_attack.x",				false,	125.0f	},	// 走り後攻撃
	{ "Character\\Player\\anim\\attack_normal_1-1.x",		false,	24.0f	},	// 通常攻撃1-1
	{ "Character\\Player\\anim\\attackwait_normal_1-1.x",	false,	39.0f	},	// 通常攻撃1-1攻撃待ち
	{ "Character\\Player\\anim\\attackend_normal_1-1.x",	false,	30.0f	},	// 通常攻撃1-1終了
	{ "Character\\Player\\anim\\attack_normal_1-2.x",		false,	22.0f	},	// 通常攻撃1-2
	{ "Character\\Player\\anim\\attackwait_normal_1-2.x",	false,	31.0f	},	// 通常攻撃1-2攻撃待ち
	{ "Character\\Player\\anim\\attackend_normal_1-2.x",	false,	30.0f	},	// 通常攻撃1-2終了
	{ "Character\\Player\\anim\\attack_normal_1-3.x",		false,	56.0f	},	// 通常攻撃1-3
	{ "Character\\Player\\anim\\attackwait_normal_1-3.x",	false,	28.0f	},	// 通常攻撃1-3攻撃待ち
	{ "Character\\Player\\anim\\attackend_normal_1-3.x",	false,	30.0f	},	// 通常攻撃1-3終了
	{ "Character\\Player\\anim\\attack_air_1-1.x",			false,	11.0f	},	// 空中攻撃1-1
	{ "Character\\Player\\anim\\attackwait_air_1-1.x",		false,	13.0f	},	// 空中攻撃1-1攻撃待ち
	{ "Character\\Player\\anim\\attackend_air_1-1.x",		false,	19.0f	},	// 空中攻撃1-1終了
	{ "Character\\Player\\anim\\attack_air_1-2.x",			false,	14.0f	},	// 空中攻撃1-2
	{ "Character\\Player\\anim\\attackwait_air_1-2.x",		false,	16.0f	},	// 空中攻撃1-2攻撃待ち
	{ "Character\\Player\\anim\\attackend_air_1-2.x",		false,	19.0f	},	// 空中攻撃1-2終了
	{ "Character\\Player\\anim\\attack_air_1-3.x",			false,	16.0f	},	// 空中攻撃1-3
	{ "Character\\Player\\anim\\attackwait_air_1-3.x",		false,	14.0f	},	// 空中攻撃1-3攻撃待ち
	{ "Character\\Player\\anim\\attackend_air_1-3.x",		false,	22.0f	},	// 空中攻撃1-3終了
	{ "Character\\Player\\anim\\attack_air_1-4.x",			false,	20.0f	},	// 空中攻撃1-4
	{ "Character\\Player\\anim\\attackend_air_1-4.x",		false,	34.0f	},	// 通常攻撃1-4終了
	{ "Character\\Player\\anim\\attack_up.x",				false,	59.0f	},	// ジャンプ攻撃
	{ "Character\\Player\\anim\\landing.x",					false,	35.0f	},	// 着地動作(納刀)
	{ "Character\\Player\\anim\\landing_combat.x",			false,	35.0f	},	// 着地動作(抜刀)
};

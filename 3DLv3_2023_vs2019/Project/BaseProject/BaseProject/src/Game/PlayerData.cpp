#include "PlayerData.h"

// ƒvƒŒƒCƒ„[‚ÌƒAƒjƒ[ƒVƒ‡ƒ“ƒf[ƒ^ƒe[ƒuƒ‹
std::vector<PlayerData::AnimData> PlayerData::ANIM_DATA = 
{
	{ "",													true,	0.0f	},	// Tƒ|[ƒY
	{ "Character\\Player\\anim\\idle.x",					true,	601.0f	},	// ‘Ò‹@(”[“)
	{ "Character\\Player\\anim\\idle_combat.x",				true,	601.0f	},	// ‘Ò‹@(”²“)
	{ "Character\\Player\\anim\\idleair.x",					true,	20.0f	},	// ‹ó’†‘Ò‹@(”[“
	{ "Character\\Player\\anim\\idleair_combat.x",			true,	20.0f	},	// ‹ó’†‘Ò‹@(”²“)
	{ "Character\\Player\\anim\\idle_drawn_combat.x",		false,	101.0f	},	// ‘Ò‹@’†”²““®ì
	{ "Character\\Player\\anim\\idle_sheathed_combat.x",	false,	81.0f	},	// ‘Ò‹@’†”[““®ì
	{ "Character\\Player\\anim\\run_start.x",				false,	36.0f	},	// ‘–‚èŠJn(”[“)
	{ "Character\\Player\\anim\\run_loop.x",				true,	40.0f	},	// ‘–‚è(”[“)
	{ "Character\\Player\\anim\\run_end.x",					false,	50.0f	},	// ‘–‚èI—¹(”[“)
	{ "Character\\Player\\anim\\run_combat_start.x",		false,	36.0f	},	// ‘–‚èŠJn(”²“)
	{ "Character\\Player\\anim\\run_combat.x",				true,	40.0f	},	// ‘–‚è(”²“)
	{ "Character\\Player\\anim\\run_combat_end.x",			false,	50.0f	},	// ‘–‚èI—¹(”²“)
	{ "Character\\Player\\anim\\run_drawn_combat.x",		false,	40.0f	},	// ‘–‚è’†”²““®ì
	{ "Character\\Player\\anim\\run_sheathed_combat.x",		false,	40.0f	},	// ‘–‚è’†”[““®ì
	{ "Character\\Player\\anim\\fastrun_start.x",			false,	11.0f	},	// ƒ_ƒbƒVƒ…ŠJn
	{ "Character\\Player\\anim\\fastrun_loop.x",			true,	28.0f	},	// ƒ_ƒbƒVƒ…	
	{ "Character\\Player\\anim\\fastrun_end.x",				false,	52.0f	},	// ƒ_ƒbƒVƒ…I—¹
	{ "Character\\Player\\anim\\roll_start.x",				false,	25.0f	},	// ‰ñ”ğ“®ìŠJn(”[“)
	{ "Character\\Player\\anim\\roll_end_idle.x",			false,	25.0f	},	// ‰ñ”ğ“®ì‚©‚çƒAƒCƒhƒ‹‚ÖˆÚs(”[“)
	{ "Character\\Player\\anim\\roll_end_run.x",			false,	20.0f	},	// ‰ñ”ğ“®ì‚©‚ç‘–‚è‚ÖˆÚs(”[“)
	{ "Character\\Player\\anim\\roll_combat.x",				false,	25.0f	},	// ‰ñ”ğ“®ìŠJn(”²“)
	{ "Character\\Player\\anim\\roll_end_combat_idle.x",	false,	20.0f	},	// ‰ñ”ğ“®ì‚©‚çƒAƒCƒhƒ‹‚ÖˆÚs(”²“)
	{ "Character\\Player\\anim\\roll_end_combat_run.x",		false,	19.0f	},	// ‰ñ”ğ“®ì‚©‚ç‘–‚è‚ÖˆÚs(”²“)
	{ "Character\\Player\\anim\\run_attack.x",				false,	125.0f	},	// ‘–‚èŒãUŒ‚
	{ "Character\\Player\\anim\\attack_normal_1-1.x",		false,	24.0f	},	// ’ÊíUŒ‚1-1
	{ "Character\\Player\\anim\\attackwait_normal_1-1.x",	false,	39.0f	},	// ’ÊíUŒ‚1-1UŒ‚‘Ò‚¿
	{ "Character\\Player\\anim\\attackend_normal_1-1.x",	false,	30.0f	},	// ’ÊíUŒ‚1-1I—¹
	{ "Character\\Player\\anim\\attack_normal_1-2.x",		false,	22.0f	},	// ’ÊíUŒ‚1-2
	{ "Character\\Player\\anim\\attackwait_normal_1-2.x",	false,	31.0f	},	// ’ÊíUŒ‚1-2UŒ‚‘Ò‚¿
	{ "Character\\Player\\anim\\attackend_normal_1-2.x",	false,	30.0f	},	// ’ÊíUŒ‚1-2I—¹
	{ "Character\\Player\\anim\\attack_normal_1-3.x",		false,	56.0f	},	// ’ÊíUŒ‚1-3
	{ "Character\\Player\\anim\\attackwait_normal_1-3.x",	false,	28.0f	},	// ’ÊíUŒ‚1-3UŒ‚‘Ò‚¿
	{ "Character\\Player\\anim\\attackend_normal_1-3.x",	false,	30.0f	},	// ’ÊíUŒ‚1-3I—¹
	{ "Character\\Player\\anim\\attack_air_1-1.x",			false,	11.0f	},	// ‹ó’†UŒ‚1-1
	{ "Character\\Player\\anim\\attackwait_air_1-1.x",		false,	13.0f	},	// ‹ó’†UŒ‚1-1UŒ‚‘Ò‚¿
	{ "Character\\Player\\anim\\attackend_air_1-1.x",		false,	19.0f	},	// ‹ó’†UŒ‚1-1I—¹
	{ "Character\\Player\\anim\\attack_air_1-2.x",			false,	14.0f	},	// ‹ó’†UŒ‚1-2
	{ "Character\\Player\\anim\\attackwait_air_1-2.x",		false,	16.0f	},	// ‹ó’†UŒ‚1-2UŒ‚‘Ò‚¿
	{ "Character\\Player\\anim\\attackend_air_1-2.x",		false,	19.0f	},	// ‹ó’†UŒ‚1-2I—¹
	{ "Character\\Player\\anim\\attack_air_1-3.x",			false,	16.0f	},	// ‹ó’†UŒ‚1-3
	{ "Character\\Player\\anim\\attackwait_air_1-3.x",		false,	14.0f	},	// ‹ó’†UŒ‚1-3UŒ‚‘Ò‚¿
	{ "Character\\Player\\anim\\attackend_air_1-3.x",		false,	22.0f	},	// ‹ó’†UŒ‚1-3I—¹
	{ "Character\\Player\\anim\\attack_air_1-4.x",			false,	20.0f	},	// ‹ó’†UŒ‚1-4
	{ "Character\\Player\\anim\\attackend_air_1-4.x",		false,	34.0f	},	// ’ÊíUŒ‚1-4I—¹
	{ "Character\\Player\\anim\\attack_up.x",				false,	59.0f	},	// ƒWƒƒƒ“ƒvUŒ‚
	{ "Character\\Player\\anim\\landing.x",					false,	35.0f	},	// ’…’n“®ì(”[“)
	{ "Character\\Player\\anim\\landing_combat.x",			false,	35.0f	},	// ’…’n“®ì(”²“)
};

#include "DragonData.h"

// 敵のアニメーションデータテーブル
std::vector<DragonData::AnimData> DragonData::ANIM_DATA =
{
	{ "Character\\Dragon\\anim\\idle1.x",			true,	41.0f,		0.0f },	// アイドル1
	{ "Character\\Dragon\\anim\\idle2.x",			true,	101.0f,		0.0f },	// アイドル2(使わない予定)
	{ "Character\\Dragon\\anim\\land.x",			false,	121.0f, 	0.0f },	// 着地
	{ "Character\\Dragon\\anim\\run.x",				true,	20.0f,		0.0f },	// 走り
	{ "Character\\Dragon\\anim\\walk.x",			true,	40.0f,		0.0f },	// 歩き
	{ "Character\\Dragon\\anim\\scream.x",			false,	101.0f,		0.0f },	// 咆哮
	{ "Character\\Dragon\\anim\\takeOff.x",			false,	121.0f,		0.0f },	// 離陸
	{ "Character\\Dragon\\anim\\attackflame.x",		false,	81.0f,		0.2f },	// ブレス攻撃
	{ "Character\\Dragon\\anim\\attackHand.x",		false,	91.0f,		0.7f },	// 飛び掛かり攻撃
	{ "Character\\Dragon\\anim\\attackMouth.x",		false,	36.0f,		0.4f },	// 噛みつき攻撃
	{ "Character\\Dragon\\anim\\die.x",				false,	65.0f,		0.0f },	// 死亡
	{ "Character\\Dragon\\anim\\flyFlame.x",		false,	91.0f,		0.3f },	// 空中ブレス攻撃
	{ "Character\\Dragon\\anim\\flyforward.x",		true,	31.0f,		0.0f },	// 空中前進
	{ "Character\\Dragon\\anim\\flyGlide.x",		true,	51.0f,		0.0f },	// 空中滑空(使わない予定)
	{ "Character\\Dragon\\anim\\flyIdle.x",			true,	31.0f,		0.0f },	// 空中アイドル
	{ "Character\\Dragon\\anim\\getHit.x",			false,	41.0f,		0.0f },	// のけ反り
	{ "Character\\Dragon\\anim\\backStep.x",		false,	35.0f,		0.0f },	// バックステップ
};
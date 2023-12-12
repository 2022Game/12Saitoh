#pragma once

// キャラステータス
struct CharaStatus
{
	int level;	// レベル
	int hp;		// HP
	int atk;	// 攻撃力
	int def;	// 防御力
	float sp;		// スタミナ
	int touki;	// 闘気ゲージ
};

// 敵の最大レベル
#define ENEMY_LEVEL_MAX 5

// 初期化用
extern const CharaStatus DEFAULT[];

// プレイヤーのステータステーブル
extern const CharaStatus PLAYER_STATUS[];
// 敵のステータステーブル
extern const CharaStatus ENEMY_STATUS[ENEMY_LEVEL_MAX];
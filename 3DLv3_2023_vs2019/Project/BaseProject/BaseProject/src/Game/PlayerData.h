#pragma once
class PlayerData
{
public:
	// アニメーションのデータ
	struct AnimData
	{
		std::string path;	// アニメーションデータのパス
		bool loop;			// ループするかどうか
		float frameLength;	// アニメーションのフレーム数
	};
	// アニメーションデータテーブルを取得
	static const AnimData& GetAnimData(int index)
	{
		return ANIM_DATA[index];
	}
	// データサイズを取得
	static int GetAnimDataSize()
	{
		return ANIM_DATA.size();
	}
private:
	// アニメーションデータのテーブル
	static std::vector<AnimData> ANIM_DATA;
};

// プレイヤーのモデルデータのパス
#define MODEL_PATH "Character\\Player\\player.x"

#define PLAYER_HEIGHT	1.8f
#define MOVE_SPEED		1.0f	// 走る速度
#define FASTMOVE_SPEED	1.5f	// ダッシュ速度
#define ROLL_SPEED		1.3f	// 回避速度
#define JUMP_SPEED		1.5f
#define GRAVITY			0.0625f
#define JUMP_END_Y		1.0f
// プレイヤー抜刀状態へ切り替えるフレーム(待機状態中)
#define SWITCH_DRAWN_IDLE_FRAME 35
// プレイヤー納刀状態へ切り替えるフレーム(待機状態中)
#define SWITCH_SHEATHED_IDLE_FRAME 31
// プレイヤー抜刀状態へ切り替えるフレーム(走り中)
#define SWITCH_DRAWN_RUN_FRAME 13
// プレイヤー納刀状態へ切り替えるフレーム(走り中)
#define SWITCH_SHEATHED_RUN_FRAME 20

/* 通常攻撃 */
// プレイヤー通常攻撃1-1の移動するフレーム区間(始め)
#define NORMALATTACK1_1_START_FRAME	1
// プレイヤー通常攻撃1-1の移動するフレーム区間(終わり)
#define NORMALATTACK1_1_END_FRAME 15
// プレイヤー通常攻撃1-1の移動速度
#define NORMALATTACK1_1_MOVESPEED 1.0f
// プレイヤー通常攻撃1-2の移動するフレーム区間(始め)
#define NORMALATTACK1_2_START_FRAME 1
// プレイヤー通常攻撃1-2の移動するフレーム区間(終わり)
#define NORMALATTACK1_2_END_FRAME 12
// プレイヤー通常攻撃1-2の移動速度
#define NORMALATTACK1_2_MOVESPEED 1.0f
// プレイヤー通常攻撃1-3の移動するフレーム区間(始め)
#define NORMALATTACK1_3_START_FRAME 7
// プレイヤー通常攻撃1-3の移動するフレーム区間(終わり)
#define NORMALATTACK1_3_END_FRAME 30
// プレイヤー通常攻撃1-3の移動速度
#define NORMALATTACK1_3_MOVESPEED 0.7f

/* ジャンプ攻撃 */
// ジャンプ攻撃の移動するフレーム区間(始め)
#define ATTACK_UP_START_FRAME 7
// ジャンプ攻撃の移動するフレーム区間(終わり)
#define ATTACK_UP_END_FRAME 19
// ジャンプ攻撃の移動速度
#define ATTACK_UP_MOVE_SPEED 1.0f
// ジャンプ攻撃のジャンプを始めるフレーム
#define ATTACK_UP_JUMPSTART_FRAME 26
// ジャンプ攻撃のジャンプ初速度
#define ATTACK_UP_JUMP_SPEED 2.5f

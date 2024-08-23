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
		float motionValue;	// モーション値
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
// アニメーションの種類
enum class EAnimType
{
	None = -1,

	eTPose,					// Tポーズ
	eIdle,					// 待機(納刀)
	eIdle_Combat,			// 待機(抜刀)
	eIdleAir,				// 空中待機(納刀)
	eIdleAir_Combat,		// 空中待機(抜刀)
	eIdle_Drawn_Combat,		// 待機中抜刀動作
	eIdle_Sheathed_Combat,	// 待機中納刀動作
	eRunStart,				// 走り開始(納刀)
	eRun,					// 走り(納刀)
	eRunEnd,				// 走り終了(納刀)
	eRunStart_Combat,		// 走り開始(抜刀)
	eRun_Combat,			// 走り(抜刀)
	eRunEnd_Combat,			// 走り終了(抜刀)
	eRun_Drawn_Combat,		// 走り中抜刀動作
	eRun_Sheathed_Combat,	// 走り中納刀動作
	eFastRunStart,			// ダッシュ開始
	eFastRun,				// ダッシュ
	eFastRunEnd,			// ダッシュ終了
	eRollStart,				// 回避動作開始(納刀)
	eRollEnd_idle,			// 回避後アイドル移行(納刀)
	eRollEnd_run,			// 回避後走り移行(納刀)
	eRollStart_Combat,		// 回避動作開始(抜刀)
	eRollEnd_idle_Combat,	// 回避後アイドル移行(抜刀)
	eRollEnd_run_Combat,	// 回避後走り移行(抜刀)
	eRunAttack,				// 走り後攻撃
	eNormalAttack1_1,		// 通常攻撃1-1
	eNormalWait1_1,			// 通常攻撃1-1攻撃待ち
	eNormalEnd1_1,			// 通常攻撃1-1終了
	eNormalAttack1_2,		// 通常攻撃1-2
	eNormalWait1_2,			// 通常攻撃1-2攻撃待ち
	eNormalEnd1_2,			// 通常攻撃1-2終了
	eNormalAttack1_3,		// 通常攻撃1-3
	eNormalWait1_3,			// 通常攻撃1-3攻撃待ち
	eNormalEnd1_3,			// 通常攻撃1-3終了
	eAirAttack1_1,			// 空中攻撃1-1
	eAirAttackWait1_1,		// 空中攻撃1-1攻撃待ち
	eAirAttackEnd1_1,		// 空中攻撃1-1終了
	eAirAttack1_2,			// 空中攻撃1-2
	eAirAttackWait1_2,		// 空中攻撃1-2攻撃待ち
	eAirAttackEnd1_2,		// 空中攻撃1-2終了
	eAirAttack1_3,			// 空中攻撃1-3
	eAirAttackWait1_3,		// 空中攻撃1-3攻撃待ち
	eAirAttackEnd1_3,		// 空中攻撃1-3終了
	eAirAttack1_4,			// 空中攻撃1-4
	eAirAttackEnd1_4,		// 空中攻撃1-4終了
	eAttack_Up,				// ジャンプ攻撃
	eParryAttack,			// カウンター攻撃
	eParryAttack_End,		// カウンター攻撃終了
	eCounter_Start,			// カウンター構え開始
	eCounter_Loop,			// カウンター構え中
	eCounter_End,			// カウンター構え終了
	ePowerAttack_Start,		// 強闘技構え開始
	ePowerAttack_Loop,		// 強闘技構え中
	ePowerAttack,			// 強闘技攻撃
	eLanding,				// 着地動作(納刀)
	eLandin_Combat,			// 着地動作(抜刀)
	eDie_Start,				// 死亡開始(納刀)
	eDie_Loop,				// 死亡ループ(納刀)
	eDie_Start_Combat,		// 死亡開始(抜刀)
	eDie_Loop_Combat,		// 死亡ループ(抜刀)
	Num
};

#define PLAYER_HEIGHT	1.8f	
#define MOVE_SPEED		1.0f	// 走る速度
#define FASTMOVE_SPEED	1.5f	// ダッシュ速度
#define ROLL_SPEED		1.5f	// 回避速度
#define GRAVITY			0.0625f	// 重力

#define PLAYER_STATAS	0		// プレイヤーデータテーブル
#define PLAYER_MAX_HP	100		// HPの最大値
#define PLAYER_MIN_HP	0		// HPの最小値
#define PLAYER_MAX_SP	100.0f	// スタミナの最大値
#define PLAYER_MIN_SP	0.0f	// スタミナの最小値
#define PLAYER_MAX_TOUKI 300	// 闘気ゲージの最大値
#define PLAYER_MIN_TOUKI 0		// 闘気ゲージの最小値
#define UP_TOUKI 10
#define ATTACKSTEP_END -1		// 攻撃終了の段階
#define ATTACKSTEP_COUNTER 3	// カウンターの段階
#define AVOID_SPCOST 20.0f		// 回避に必要なスタミナ

// プレイヤー抜刀状態へ切り替えるフレーム(待機状態中)
#define SWITCH_DRAWN_IDLE_FRAME 35
// プレイヤー納刀状態へ切り替えるフレーム(待機状態中)
#define SWITCH_SHEATHED_IDLE_FRAME 31
// プレイヤー抜刀状態へ切り替えるフレーム(走り中)
#define SWITCH_DRAWN_RUN_FRAME 15
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
// プレイヤー通常攻撃1-3のコライダー用フレーム
#define NORMALATTACK1_3_COLLIDER 35

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
#define ATTACK_UP_JUMP_SPEED 3.5f

/* 弱闘技*/
// 二度目の攻撃の開始フレーム
#define COUNTERATTACK_START 25.0f

/* 強闘技 */
// 一度目の移動するフレーム区間(始め)
#define POWERATTACK1_START 0
// 一度目の移動するフレーム区間(終わり)
#define POWERATTACK1_END 5
//　一度目の移動方向ベクトル
#define POWERATTACK1_VEC CVector(0.0f, 0.0f, -1.0f)

// 二度目の移動するフレーム区間(始め)
#define POWERATTACK2_START 8
// 二度目の移動するフレーム区間(終わり)
#define POWERATTACK2_END 12
//　二度目の移動方向ベクトル
#define POWERATTACK2_VEC CVector(-1.0f, 0.0f, 1.0f)

// 三度目の移動するフレーム区間(始め)
#define POWERATTACK3_START 16
// 三度目の移動するフレーム区間(終わり)
#define POWERATTACK3_END 20
// 三度目の移動方向ベクトル
#define POWERATTACK3_VEC CVector(1.0f, 0.0f, 0.8f)

// 四度目の移動するフレーム区間(始め)
#define POWERATTACK4_START 24
// 四度目の移動するフレーム区間(終わり)
#define POWERATTACK4_END 30
// 四度目の移動方向ベクトル
#define POWERATTACK4_VEC CVector(-2.0f, 0.0f, 0.0f)

// 五度目の移動するフレーム区間(始め)
#define POWERATTACK5_START 36
// 五度目の移動するフレーム区間(終わり)
#define POWERATTACK5_END 41
// 五度目の移動方向ベクトル
#define POWERATTACK5_VEC CVector(4.0f, 0.0f, -2.0f)

// 六度目の移動するフレーム区間(始め)
#define POWERATTACK6_START 48
// 六度目の移動するフレーム区間(終わり)
#define POWERATTACK6_END 56
// 六度目の移動方向ベクトル
#define POWERATTACK6_VEC CVector(-1.0f, 0.1f, 1.0f)

// 七度目の移動するフレーム区間(始め)
#define POWERATTACK7_START 58
// 七度目の移動するフレーム区間(終わり)
#define POWERATTACK7_END 65
// 七度目の移動方向ベクトル
#define POWERATTACL7_VEC CVector(0.0f, 3.0f, 0.0f)

// 強闘技の攻撃時の移動速度
#define POWERATTACK_MOVESPEED 4.0f
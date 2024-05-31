#pragma once
class DragonData
{
public:
	// アニメーションのデータ
	struct AnimData
	{
		std::string path;	// アニメーションのデータパス
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
	// アニメーションのデータテーブル
	static std::vector<AnimData> ANIM_DATA;
};

// アニメーションの種類
enum class EDragonAnimType
{
	None = -1,

	eIdle1,			// 待機1(0.4)
	eIdle2,			// 待機2
	eLand,			// 着地(0.325)
	eRun,			// 走り(0.5)
	eWalk,			// 歩き(0.7)
	eScream,		// 咆哮(0.5)
	eTakeOff,		// 離陸(0.45)
	eAttackFlame,	// ブレス攻撃(0.33)
	eAttackHand,	// 飛び掛かり攻撃(0.5)
	eAttackMouth,	// 噛みつき攻撃(0.43)
	eDie,			// 死亡(0.25)
	eFlyFlame,		// 空中ブレス攻撃(0.5)
	eFlyForward,	// 空中前進(0.6)
	eFlyGlide,		// 空中滑空
	eFlyIdle,		// 空中アイドル(0.31)
	eGetHit,		// のけ反り(0.49)
	eBackStep,		// バックステップ(0.5)

	Num
};

#define GRAVITY			0.0625f	// 重力
#define ENEMY_HEIGHT	400.0f	// 敵の高さ
#define FOV_LANGE		170.0f	// 視野判定用の半径
#define FOV_ANGLE		60.0f	// 視野判定用の角度
#define FEAR_MAXVALUE	200.0f	// 怯み値の上限

// 噛みつき攻撃関連
// 噛みつき攻撃のコライダーを切り替えるフレーム(始め)
#define ATTACKMOUTH_COL_START 10.0f
// 噛みつき攻撃のコライダーを切り替えるフレーム(終わり)
#define ATTACKMOUTH_COL_END 16.0f

// 飛び掛かり攻撃関連
// 飛び掛かり攻撃の移動開始フレーム
#define ATTACKHAND_START_FRAME 37.0f
// 飛び掛かり攻撃の移動終了フレーム
#define ATTACKHAND_END_FRAME 52.0f
// 飛び掛かり攻撃の移動速度
#define ATTACKHAND_MOVESPEED 5.5f
// バックステップの移動開始フレーム(飛び掛かり攻撃アニメーション中)
#define BACKSTEP_START_FRAME 77.0f
// バックステップの移動終了フレーム(飛び掛かり攻撃アニメーション中)
#define BACKSTEP_END_FRAME 90.0f
// バックステップの移動速度
#define BACKSTEP_MOVESPEED 5.0f
// ジャンプ始めのフレーム
#define JUMP_START_FRAME 37.0f
// バックステップ時のジャンプ始めのフレーム
#define BACKJUMP_START_FRAME 80.0f

// 落下時の調整フレーム１(始め)
#define FALLING_FRAME1_START 42.0f
// 落下時の調整フレーム1(終わり)
#define FALLING_FRAME1_END 50.0f
// 落下時の調整フレーム2(始め)
#define FALLING_FRAME2_START 85.0f
// 落下時の調整フレーム2(終わり)
#define FALLING_FRAME2_END 91.0f

// バックステップのレイの長さ
#define BACKSTEP_RAY 250.0f


// ブレス攻撃関連
// ブレス攻撃開始フレーム
#define FLAME_START_FRAME 19.0f
// ブレス攻撃終了フレーム
#define FLAME_END_FRAME 71.0f

// 空中ブレス攻撃関連
// 空中ブレス開始フレーム
#define FLYFLAME_START_FRAME 15.0f
// 空中ブレス終了フレーム
#define FLYFLAME_END_FRAME 77.0f
// 残りのHP割合(50%)
#define HPPER_50 50
// 残りのHP割合(25%)
#define HPPER_25 25
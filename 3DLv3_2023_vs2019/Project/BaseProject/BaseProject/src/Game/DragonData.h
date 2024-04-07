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

	eIdle1,			// 待機1
	eIdle2,			// 待機2
	eLand,			// 着地
	eRun,			// 走り
	eWalk,			// 歩き
	eScream,		// 咆哮
	eSleep,			// 寝る
	eTakeOff,		// 離陸
	eAttackFlame,	// ブレス攻撃
	eAttackHand,	// 飛び掛かり攻撃
	eAttackMouth,	// 噛みつき攻撃
	eDie,			// 死亡
	eFlyFlame,		// 空中ブレス攻撃
	eFlyForward,	// 空中前進
	eFlyGlide,		// 空中滑空
	eFlyIdle,		// 空中アイドル
	eGetHit,		// のけ反り

	Num
};

#define GRAVITY			0.0625f	// 重力
#define ENEMY_HEIGHT	400.0f	// 敵の高さ
#define FOV_LANGE		170.0f	// 視野判定用の半径
#define FOV_ANGLE		60.0f	// 視野判定用の角度

// 噛みつき攻撃関連
// 噛みつき攻撃のコライダーを切り替えるフレーム(始め)
#define ATTACKMOUTH_COL_START 20.0f
// 噛みつき攻撃のコライダーを切り替えるフレーム(終わり)
#define ATTACKMOUTH_COL_END 50.0f

// 飛び掛かり攻撃関連
// 飛び掛かり攻撃の移動開始フレーム
#define ATTACKHAND_START_FRAME 74.0f
// 飛び掛かり攻撃の移動終了フレーム
#define ATTACKHAND_END_FRAME 106.0f
// 飛び掛かり攻撃の移動速度
#define ATTACKHAND_MOVESPEED 4.0f
// バックステップの移動開始フレーム(飛び掛かり攻撃アニメーション中)
#define BACKSTEP_START_FRAME 148.0f
// バックステップの移動終了フレーム(飛び掛かり攻撃アニメーション中)
#define BACKSTEP_END_FRAME 180.0f
// バックステップの移動速度
#define BACKSTEP_MOVESPEED 3.9f
// ジャンプ始めのフレーム
#define JUMP_START_FRAME 74.0f
// バックステップ時のジャンプ始めのフレーム
#define BACKJUMP_START_FRAME 154.0f
// 落下時の調整フレーム１(始め)
#define FALLING_FRAME1_START 84.0f
// 落下時の調整フレーム1(終わり)
#define FALLING_FRAME1_END 100.0f
// 落下時の調整フレーム2(始め)
#define FALLING_FRAME2_START 170.0f
// 落下時の調整フレーム2(終わり)
#define FALLING_FRAME2_END 182.0f

// ブレス攻撃関連
// ブレス攻撃開始フレーム
#define FLAME_START_FRAME 60.0f
// ブレス攻撃終了フレーム
#define FLAME_END_FRAME 200.0f
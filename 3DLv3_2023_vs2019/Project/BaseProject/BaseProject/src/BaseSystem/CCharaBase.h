#pragma once
#include "CObjectBase.h"

/// <summary>
/// キャラクターのベースクラス
/// </summary>
class CCharaBase : public CObjectBase
{
public:
	CCharaBase(ETag tag, ETaskPriority prio, int sortOrder = 0,
		ETaskPauseType pause = ETaskPauseType::eGame);
	virtual ~CCharaBase();

	// キャラクターステータス
	class CharaStatus
	{
	public:
		int atk;	// 攻撃力
		int def;	// 防御力
		int hp;		// HP
		int sp;		// スタミナ
		int touki;	// 闘気ゲージ
	};
protected:
	CharaStatus mStatas;
};

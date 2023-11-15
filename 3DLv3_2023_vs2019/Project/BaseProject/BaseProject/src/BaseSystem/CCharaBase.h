#pragma once
#include "CObjectBase.h"

/// <summary>
/// キャラクターのベースクラス
/// </summary>
class CCharaBase : public CObjectBase
{
public:
	class CharaStatus
	{
		public:
			int atk;	// 攻撃力
			int def;	// 防御力
			int hp;		// HP
			int sp;		// スタミナ
			int touki;	// 闘気ゲージ
	};
	CCharaBase(ETag tag, ETaskPriority prio);
	virtual ~CCharaBase();
protected:
	CharaStatus mStatas;
};

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

	};
	CCharaBase(ETag tag, ETaskPriority prio);
	virtual ~CCharaBase();
};

#pragma once
#include "CObjectBase.h"

/// <summary>
/// �L�����N�^�[�̃x�[�X�N���X
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

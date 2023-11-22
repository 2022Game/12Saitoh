#pragma once
#include "CObjectBase.h"

/// <summary>
/// �L�����N�^�[�̃x�[�X�N���X
/// </summary>
class CCharaBase : public CObjectBase
{
public:
	CCharaBase(ETag tag, ETaskPriority prio, int sortOrder = 0,
		ETaskPauseType pause = ETaskPauseType::eGame);
	virtual ~CCharaBase();

	// �L�����N�^�[�X�e�[�^�X
	class CharaStatus
	{
	public:
		int atk;	// �U����
		int def;	// �h���
		int hp;		// HP
		int sp;		// �X�^�~�i
		int touki;	// ���C�Q�[�W
	};
protected:
	CharaStatus mStatas;
};

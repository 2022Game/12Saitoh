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
		public:
			int atk;	// �U����
			int def;	// �h���
			int hp;		// HP
			int sp;		// �X�^�~�i
			int touki;	// ���C�Q�[�W
	};
	CCharaBase(ETag tag, ETaskPriority prio);
	virtual ~CCharaBase();
protected:
	CharaStatus mStatas;
};

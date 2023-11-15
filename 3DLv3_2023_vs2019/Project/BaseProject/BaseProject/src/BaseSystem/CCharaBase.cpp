#include "CCharaBase.h"

CCharaBase::CCharaBase(ETag tag, ETaskPriority prio)
	: CObjectBase(tag, prio)
{
	mStatas.atk = 0;
	mStatas.def = 0;
	mStatas.hp = 0;
	mStatas.sp = 0;
	mStatas.touki = 0;
}

CCharaBase::~CCharaBase()
{
}

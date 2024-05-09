#include "DragonData.h"

// �G�̃A�j���[�V�����f�[�^�e�[�u��
std::vector<DragonData::AnimData> DragonData::ANIM_DATA =
{
	{ "Character\\Dragon\\anim\\idle1.x",			true,	102.0f,		0.0f },	// �A�C�h��1
	{ "Character\\Dragon\\anim\\idle2.x",			true,	151.0f,		0.0f },	// �A�C�h��2
	{ "Character\\Dragon\\anim\\land.x",			false,	200.0f*2.0,	0.0f },	// ���n
	{ "Character\\Dragon\\anim\\run.x",				true,	40.0f,		0.0f },	// ����
	{ "Character\\Dragon\\anim\\walk.x",			true,	80.0f,		0.0f },	// ����
	{ "Character\\Dragon\\anim\\scream.x",			false,	202.0f,		0.0f },	// ���K
	{ "Character\\Dragon\\anim\\sleep.x",			true,	162.0f,		0.0f },	// �Q��
	{ "Character\\Dragon\\anim\\takeOff.x",			false,	121.0f*2.2,	0.0f },	// ����
	{ "Character\\Dragon\\anim\\attackflame.x",		false,	241.0f,		0.2f },	// �u���X�U��
	{ "Character\\Dragon\\anim\\attackHand.x",		false,	182.0f,		0.7f },	// ��ъ|����U��
	{ "Character\\Dragon\\anim\\attackMouth.x",		false,	86.0f,		0.4f },	// ���݂��U��
	{ "Character\\Dragon\\anim\\die.x",				false,	260.0f,		0.0f },	// ���S
	{ "Character\\Dragon\\anim\\flyFlame.x",		false,	182.0f*2.3,	0.9f },	// �󒆃u���X�U��
	{ "Character\\Dragon\\anim\\flyforward.x",		true,	80.0f,		0.0f },	// �󒆑O�i
	{ "Character\\Dragon\\anim\\flyGlide.x",		true,	102.0f,		0.0f },	// �󒆊���
	{ "Character\\Dragon\\anim\\flyIdle.x",			true,	102.0f,		0.0f },	// �󒆃A�C�h��
	{ "Character\\Dragon\\anim\\getHit.x",			true,	86.0f,		0.0f },	// �̂�����
	{ "Character\\Dragon\\anim\\backStep.x",		false,	72.0f,		0.0f },	// �o�b�N�X�e�b�v	
};
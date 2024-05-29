#include "DragonData.h"

// �G�̃A�j���[�V�����f�[�^�e�[�u��
std::vector<DragonData::AnimData> DragonData::ANIM_DATA =
{
	{ "Character\\Dragon\\anim\\idle1.x",			true,	41.0f,		0.0f },	// �A�C�h��1
	{ "Character\\Dragon\\anim\\idle2.x",			true,	101.0f,		0.0f },	// �A�C�h��2(�g��Ȃ��\��)
	{ "Character\\Dragon\\anim\\land.x",			false,	121.0f, 	0.0f },	// ���n
	{ "Character\\Dragon\\anim\\run.x",				true,	20.0f,		0.0f },	// ����
	{ "Character\\Dragon\\anim\\walk.x",			true,	40.0f,		0.0f },	// ����
	{ "Character\\Dragon\\anim\\scream.x",			false,	101.0f,		0.0f },	// ���K
	{ "Character\\Dragon\\anim\\takeOff.x",			false,	121.0f,		0.0f },	// ����
	{ "Character\\Dragon\\anim\\attackflame.x",		false,	81.0f,		0.2f },	// �u���X�U��
	{ "Character\\Dragon\\anim\\attackHand.x",		false,	91.0f,		0.7f },	// ��ъ|����U��
	{ "Character\\Dragon\\anim\\attackMouth.x",		false,	36.0f,		0.4f },	// ���݂��U��
	{ "Character\\Dragon\\anim\\die.x",				false,	65.0f,		0.0f },	// ���S
	{ "Character\\Dragon\\anim\\flyFlame.x",		false,	91.0f,		0.3f },	// �󒆃u���X�U��
	{ "Character\\Dragon\\anim\\flyforward.x",		true,	31.0f,		0.0f },	// �󒆑O�i
	{ "Character\\Dragon\\anim\\flyGlide.x",		true,	51.0f,		0.0f },	// �󒆊���(�g��Ȃ��\��)
	{ "Character\\Dragon\\anim\\flyIdle.x",			true,	31.0f,		0.0f },	// �󒆃A�C�h��
	{ "Character\\Dragon\\anim\\getHit.x",			false,	41.0f,		0.0f },	// �̂�����
	{ "Character\\Dragon\\anim\\backStep.x",		false,	35.0f,		0.0f },	// �o�b�N�X�e�b�v
};
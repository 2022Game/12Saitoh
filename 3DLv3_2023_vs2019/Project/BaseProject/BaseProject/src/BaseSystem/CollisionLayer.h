#pragma once
#include <initializer_list>

// �Փ˔��背�C���[
enum class ELayer
{
	eNone = -1,
	eTest,
	eField,
	ePlayer,
	eEnemy,
	eAttackCol, // �U���p�̃R���W����
	eDamageCol, // �_���[�W���󂯂�p�̃R���W����
};
// �Փ˔��背�C���[�̏��������X�g
using Layers = std::initializer_list<ELayer>;

#ifndef MATH_H
#define MATH_H

#define _USE_MATH_DEFINES
#include <math.h>

#include "CVector.h"

namespace Math
{
	//���`��Ԃ̌��ʂ�Ԃ�
	inline float Lerp(float start, float end, float t)
	{
		return start + t * (end - start);
	}

	//�p�x���f�B�O���[���烉�W�A���ɕϊ�
	inline float DegreeToRadian(float d)
	{
		return d * (float)(M_PI / 180.0f);
	}

	//�p�x�����W�A������f�B�O���[�ɕϊ�
	inline float RadianToDegree(float r)
	{
		return r * (float)(180.0f / M_PI);
	}

	//����͈͓����ǂ�����Ԃ�
	//p1      ����̎n�_
	//p2      ���ׂ�Ώۂ̍��W
	//forward ����̐��ʕ����x�N�g��
	//angle   ����p�x�i-angle�`angle�܂ł͈̔́j
	//length  ���싗��
	bool InFieldOfView(const CVector& p1, const CVector& p2,
		const CVector& forward, const float angle, const float length);
}

#endif
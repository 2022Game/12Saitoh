#include "Math.h"

//����͈͓����ǂ�����Ԃ�
bool Math::InFieldOfView(const CVector& p1, const CVector& p2,
	const CVector& forward, const float angle, const float length)
{
	//����̎n�_���璲�ׂ�Ώۂ܂ł̃x�N�g�������߂�
	CVector p12 = p2 - p1;

	//���ʕ����x�N�g���ƒ��ׂ�Ώۂ܂ł̃x�N�g����
	//���ς����A����p�x�������肷��
	float dot = forward.Normalize().Dot(p12.Normalize());
	if (dot < cosf(DegreeToRadian(angle))) return false;

	//���ׂ�Ώۂ܂ł̋��������߂āA���싗���������肷��
	float distance = p12.Length();
	if (distance > length) return false;

	//����p�x�A���싗���A
	//�ǂ�����͈͓��������̂ŁAtrue��Ԃ�
	return true;
}
#ifndef CWALL
#define CWALL
#include "CCharacter3.h"
#include "CTriangle.h"
#include "CCollider.h"

class CWall : public CCharacter3
{
private:
	CCollider mCollider;
	CModel mModelWall;

public:
	CWall();
	//���Ɖ��s���̐ݒ�
	//Set(��,���s)
	void Set(float w, float d);
	//�X�V
	void Update();
	//�`��
	void Render();
	void Collision();
	//�Փˏ���
	//Collision(�R���C�_1,�R���C�_2)
	void Collision(CCollider* m, CCollider* o);
};
#endif;
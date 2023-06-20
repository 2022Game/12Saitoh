#ifndef CWALL_H
#define CWALL_H

#include "CCharacter3.h"
#include "CCollider.h"
#include "CColliderMesh.h"
#include "CColliderTriangle.h"
#include "CColliderLine.h"

class CWall : public CCharacter3 {
private:
	////�R���C�_
	CColliderMesh mColliderMesh;

public:
	//�Փˏ���
	//Collision(�R���C�_�P�A�R���C�_�Q�j
	void Collision(CCollider* m, CCollider* o);
	void Collision();
	//�R���X�g���N�^
	//CWall(���f��,�ʒu,��],�g�k�j
	CWall(CModel* model, const CVector& position,
		const CVector& rotation, const CVector& scale);
	//�X�V����
	void Update();
};
#endif
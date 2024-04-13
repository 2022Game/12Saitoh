#pragma once
#include "CObjectBase.h"
#include "CColliderMesh.h"

#define FIELD_RADIUS 625.0f	// ���Z��̔��a

class CField : public CObjectBase
{
public:
	CField();
	~CField();

	// �t�B�[���h�̏��̃R���C�_�[���擾
	CColliderMesh* GetFieldCol() const;
	void Update();
	void Render();
	// �t�B�[���ƃ��C�̏Փ˔���
	bool CollisionRay(const CVector start, const CVector end, float* outDistance);

private:
	CModel* mpModel;

	CModel* mpColModel;
	CColliderMesh* mpColliderMesh;
};
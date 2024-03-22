#pragma once
#include "CObjectBase.h"
#include "CColliderMesh.h"

class CField : public CObjectBase
{
public:
	CField();
	~CField();

	// �t�B�[���h�̏��̃R���C�_�[���擾
	CColliderMesh* GetFieldCol() const;
	void Update();
	void Render();

private:
	CModel* mpModel;

	CModel* mpColModel;
	CColliderMesh* mpColliderMesh;

};
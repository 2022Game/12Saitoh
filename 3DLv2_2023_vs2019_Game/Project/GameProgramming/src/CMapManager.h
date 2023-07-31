#ifndef CWALLMANAGER_H
#define CWALLMANAGER_H
#include "CTask.h"
#include "CWall.h"
#include "CFlag.h"
#include "CCharacter3.h"
#include "CColliderTriangle.h"

class NavNode;
class CMapManager
{
private:
	std::vector<CWall*> mWall;	//�ǂ̃��X�g
	std::vector<CFlag*> mFlag;	//�t���b�O�̃��X�g
	//�O�p�R���C�_�̃��X�g
	std::vector<CColliderTriangle*> mColliderTriangle;
	CModel* mpWallModel;		//�ǂ̃��f���f�[�^
	CModel* mpRedFlagModel;		//�ԃt���b�O�̃��f���f�[�^
	CModel* mpBlueFlagModel;	//�t���b�O�̃��f���f�[�^
	CColliderTriangle mColldierTriangle;	//�O�p�R���C�_
	CColliderTriangle mColldierTriangle2;	//�O�p�R���C�_

	//�C���X�^���X����
	static CMapManager* spInstance;

	//�ǂ𐶐����ă��X�g�ɒǉ�
	//CreateWall(�ʒu,��],�g�k)
	void CreateWall(CVector& pos, CVector& rot, CVector& scale);
	//�t���b�O�𐶐����ă��X�g�ɒǉ�
	//CreateFlag(�ʒu,��],�g�k)
	void CreateFlag(CVector& pos, CVector& rot, CVector& scale, int priority);
	//�O�p�R���C�_�𐶐����ă��X�g�ɒǉ�
	//CreateColliderTriangle(�e�A�e�s��A���_1�A���_2�A���_3)
	void CreateColliderTriangle(CCharacter3* parent, CMatrix* matrix,
		const CVector& v0, const CVector& v1, const CVector& v2);
public:
	CMapManager();
	~CMapManager();

	//�C���X�^���X���擾
	static CMapManager* Instance();
	//����������
	void Init();
	
	//�t�B�[���h�ɔz�u����Ă���I�u�W�F�N�g�Ƃ̃��C����
	//(���C�̊J�n�ʒu�A���C�̏I���ʒu�A�Փ˒n�_�܂ł̋����ԋp�p�j
	bool CollisionRay(const CVector& start, const CVector& end, float* outDistance);

};
#endif
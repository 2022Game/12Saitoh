#ifndef CWALLMANAGER_H
#define CWALLMANAGER_H
#include "CTask.h"
#include "CWall.h"
#include "CFlag.h"

class CMapManager : public CTask
{
private:
	std::vector<CWall*> mWall;	//�ǂ̃��X�g
	std::vector<CFlag*> mFlag;	//�t���b�O�̃��X�g
	CModel* mpWallModel;		//�ǂ̃��f���f�[�^
	CModel* mpRedFlagModel;		//�ԃt���b�O�̃��f���f�[�^
	CModel* mpBlueFlagModel;	//�t���b�O�̃��f���f�[�^

	//����������
	void Init();
	//�ǂ𐶐����ă��X�g�ɒǉ�
	//CreateWall(�ʒu,��],�g�k)
	void CreateWall(CVector& pos, CVector& rot, CVector& scale);
	//�t���b�O�𐶐����ă��X�g�ɒǉ�
	//CreateFlag(�ʒu,��],�g�k)
	void CreateFlag(CVector& pos, CVector& rot, CVector& scale, int priority);

public:
	CMapManager();
	~CMapManager();

	// �X�V����
	void Update();
};
#endif
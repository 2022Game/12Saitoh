#ifndef CWALLMANAGER_H
#define CWALLMANAGER_H
#include "CTask.h"
#include "CWall.h"

class CWallManager : public CTask
{
private:
	std::vector<CWall*> mWall;	//�ǂ̃��X�g
	CModel* mpWallModel;		//�ǂ̃��f���f�[�^

	//����������
	void Init();
	//�ǂ𐶐����ă��X�g�ɒǉ�
	//CreateWall(�ʒu,��],�g�k)
	void CreateWall(CVector& pos, CVector& rot, CVector& scale);

public:
	CWallManager();
	~CWallManager();

	// �X�V����
	void Update();
};
#endif
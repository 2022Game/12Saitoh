#ifndef CWALLMANAGER_H
#define CWALLMANAGER_H
#include "CTask.h"
#include "CWall.h"

class CWallManager : public CTask
{
private:
	std::vector<CWall*> mWall;	//壁のリスト
	CModel* mpWallModel;		//壁のモデルデータ

	//初期化処理
	void Init();
	//壁を生成してリストに追加
	//CreateWall(位置,回転,拡縮)
	void CreateWall(CVector& pos, CVector& rot, CVector& scale);

public:
	CWallManager();
	~CWallManager();

	// 更新処理
	void Update();
};
#endif
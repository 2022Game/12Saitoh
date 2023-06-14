#ifndef CWALLMANAGER_H
#define CWALLMANAGER_H
#include "CTask.h"
#include "CWall.h"
#include "CFlag.h"

class CMapManager : public CTask
{
private:
	std::vector<CWall*> mWall;	//壁のリスト
	std::vector<CFlag*> mFlag;	//フラッグのリスト
	CModel* mpWallModel;		//壁のモデルデータ
	CModel* mpRedFlagModel;		//赤フラッグのモデルデータ
	CModel* mpBlueFlagModel;	//青フラッグのモデルデータ

	//初期化処理
	void Init();
	//壁を生成してリストに追加
	//CreateWall(位置,回転,拡縮)
	void CreateWall(CVector& pos, CVector& rot, CVector& scale);
	//フラッグを生成してリストに追加
	//CreateFlag(位置,回転,拡縮)
	void CreateFlag(CVector& pos, CVector& rot, CVector& scale, int priority);

public:
	CMapManager();
	~CMapManager();

	// 更新処理
	void Update();
};
#endif
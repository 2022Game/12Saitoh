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
	std::vector<CWall*> mWall;	//壁のリスト
	std::vector<CFlag*> mFlag;	//フラッグのリスト
	//三角コライダのリスト
	std::vector<CColliderTriangle*> mColliderTriangle;
	CModel* mpWallModel;		//壁のモデルデータ
	CModel* mpRedFlagModel;		//赤フラッグのモデルデータ
	CModel* mpBlueFlagModel;	//青フラッグのモデルデータ
	CColliderTriangle mColldierTriangle;	//三角コライダ
	CColliderTriangle mColldierTriangle2;	//三角コライダ

	//インスタンス生成
	static CMapManager* spInstance;

	//壁を生成してリストに追加
	//CreateWall(位置,回転,拡縮)
	void CreateWall(CVector& pos, CVector& rot, CVector& scale);
	//フラッグを生成してリストに追加
	//CreateFlag(位置,回転,拡縮)
	void CreateFlag(CVector& pos, CVector& rot, CVector& scale, int priority);
	//三角コライダを生成してリストに追加
	//CreateColliderTriangle(親、親行列、頂点1、頂点2、頂点3)
	void CreateColliderTriangle(CCharacter3* parent, CMatrix* matrix,
		const CVector& v0, const CVector& v1, const CVector& v2);
public:
	CMapManager();
	~CMapManager();

	//インスタンスを取得
	static CMapManager* Instance();
	//初期化処理
	void Init();
	
	//フィールドに配置されているオブジェクトとのレイ判定
	//(レイの開始位置、レイの終了位置、衝突地点までの距離返却用）
	bool CollisionRay(const CVector& start, const CVector& end, float* outDistance);

};
#endif
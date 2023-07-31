#ifndef NAVMANAGER_H
#define NAVMANAGER

#include <list>
#include "NavNode.h"

//経路探索管理クラス
class NavManager
{
public:
	static NavManager* Instance();

	NavManager();
	~NavManager();

	//経路探索用のノードを追加
	void AddNode(NavNode* node);
	//経路探索用のノードを取り除く
	void RemoveNode(NavNode* node);

	//指定したノードからノードまでの経路を探索
	NavNode* Navigate(NavNode* start, NavNode* goal);

	//指定された座標に一番近いノードを取得
	NavNode* GetNearNavNode(const CVector& pos);

	//接続できるノードを検索
	//（間に遮蔽物があるノードには接続できない）
	int FindConnectNavNodes(NavNode* node, float distance);

	//全ノードを描画
	void Render();

private:
	static NavManager* mpInstance;

	//探索用ノードのリスト
	NavNodeList mNodes;

	//全ノードの状態をリセット
	void Reset();
};

#endif
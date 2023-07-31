#ifndef NAVNODE_H
#define NAVNODE_H

#include <list>
#include "CVector.h"

class NavManager;
class NavNode;
using NavNodeList = std::list<NavNode*>;

//経路探索用のノード
class NavNode
{
public:
	//接続ノードの情報
	class ConnectData
	{
	public:
		NavNode* node;	//接続ノード
		int cost;		//移動に掛かるコスト

		ConnectData(NavNode* node, int cost) : node(node), cost(cost) {}
	};
	using ConnectDataList = std::list<ConnectData>;

	NavNode();
	NavNode(const CVector& pos);
	~NavNode();
	void Reset();

	const CVector& GetPos() const;
	void SetPos(const CVector& pos);

	void AddConnect(NavNode* node);
	void AddConnects(NavNodeList nodes);
	void RemoveConnect(NavNode* node);
	void ClearConnects();

private:
	CVector pos;
	ConnectDataList connects;

	int cost;
	NavNode* toGoal;

	friend NavManager;
};

#endif
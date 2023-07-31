#ifndef NAVNODE_H
#define NAVNODE_H

#include <list>
#include "CVector.h"

class NavManager;
class NavNode;
using NavNodeList = std::list<NavNode*>;

//�o�H�T���p�̃m�[�h
class NavNode
{
public:
	//�ڑ��m�[�h�̏��
	class ConnectData
	{
	public:
		NavNode* node;	//�ڑ��m�[�h
		int cost;		//�ړ��Ɋ|����R�X�g

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
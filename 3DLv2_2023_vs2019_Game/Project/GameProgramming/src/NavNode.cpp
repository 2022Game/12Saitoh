#include "NavNode.h"
#include "NavManager.h"

NavNode::NavNode()
	: NavNode(CVector(0.0f, 0.0f, 0.0f))
{
}

NavNode::NavNode(const CVector& pos)
	: cost(-1)
	, toGoal(nullptr)
{
	SetPos(pos);
	NavManager::Instance()->AddNode(this);
}

NavNode::~NavNode()
{
	NavManager::Instance()->RemoveNode(this);
}

void NavNode::Reset()
{
	cost = -1;
	toGoal = nullptr;
}

const CVector& NavNode::GetPos() const
{
	return pos;
}

void NavNode::SetPos(const CVector& pos)
{
	this->pos = pos;
	NavManager::Instance()->FindConnectNavNodes(this, 25.0f);
}

void NavNode::AddConnect(NavNode* node)
{
	for (NavNode::ConnectData connect : connects)
	{
		if (connect.node == node) return;
	}

	float cost = (node->pos - pos).Length();

	connects.push_back(ConnectData(node, (int)cost));
	node->connects.push_back(ConnectData(this, (int)cost));
}

void NavNode::AddConnects(NavNodeList nodes)
{
	for (NavNode* node : nodes)
	{
		AddConnect(node);
	}
}

void NavNode::RemoveConnect(NavNode* node)
{
	ConnectDataList::iterator it = connects.begin();
	while (it != connects.end())
	{
		if ((*it).node == node)
		{
			it = connects.erase(it);
		}
		else
		{
			it++;
		}
	}
}

void NavNode::ClearConnects()
{
	for (ConnectData connect : connects)
	{
		connect.node->RemoveConnect(this);
	}
	connects.clear();
}

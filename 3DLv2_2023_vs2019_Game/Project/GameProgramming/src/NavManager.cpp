#include "NavManager.h"
#include "Primitive.h"
#include "Math.h"
#include "Global.h"

//ナビゲーションマネージャーのインスタンス
NavManager* NavManager::mpInstance = nullptr;

//インスタンスの取得
NavManager* NavManager::Instance()
{
	//インスタンスがなければ
	if (mpInstance == nullptr)
	{
		//インスタンスを生成する
		mpInstance = new NavManager();
	}
	return mpInstance;
}

//コンストラクタ
NavManager::NavManager()
{
}

//デストラクタ
NavManager::~NavManager()
{
	NavNodeList::iterator it = mNodes.begin();
	while (it != mNodes.end())
	{
		NavNode* del = *it;
		it++;
		delete del;
	}
	mNodes.clear();
}

//全ノードの状態をリセット
void NavManager::Reset()
{
	for (NavNode* node : mNodes)
	{
		node->Reset();
	}
}

//経路探索用のノードを追加
void NavManager::AddNode(NavNode* node)
{
	mNodes.push_back(node);
}

//経路探索用のノードを取り除く
void NavManager::RemoveNode(NavNode* node)
{
	mNodes.remove(node);
	for (NavNode* n : mNodes)
	{
		n->RemoveConnect(node);
	}
	Reset();
}

//指定したノードからノードまでの経路を探索
NavNode* NavManager::Navigate(NavNode* start, NavNode* goal)
{
	if (start == nullptr || goal == nullptr) return nullptr;

	//ノードの情報（移動コストや経路上の次のノードのポインタ）をリセット
	Reset();

	NavNodeList work1;
	NavNodeList work2;

	NavNodeList* currLevel = &work1;
	NavNodeList* nextLevel = &work2;
	NavNodeList* for_swap;

	//目的地から順番にコストを検索するため、
	//最初の検索リストには目的地のノードを追加
	goal->cost = 0;
	currLevel->push_back(goal);

	NavNodeList::iterator it;
	int nodeCost;
	//検索リストの最後までループする
	while (currLevel->size())
	{
		for (it = currLevel->begin(); it != currLevel->end(); it++)
		{
			//検索中のノードの接続リストを全て調べる
			for (NavNode::ConnectData connect : (*it)->connects)
			{
				//目的地から自身のノードまでの移動コストと
				//自身のノードから接続先のノードまでの移動コストを加算して、
				//目的地から接続先のノードまでに掛かるコストを求める
				nodeCost = (*it)->cost + connect.cost;

				//初回の検索もしくは、既に登録されている移動コストより少ないならば、
				if (connect.node->cost == -1 || nodeCost < connect.node->cost)
				{
					//移動コストと移動元のノードを設定
					connect.node->cost = nodeCost;
					connect.node->toGoal = (*it);
				}
				else
				{
					continue;
				}

				//次の検索リストに接続先のノードを追加
				nextLevel->push_back(connect.node);
			}
		}

		//検索リストを入れ替える
		for_swap = currLevel;
		currLevel = nextLevel;
		nextLevel = for_swap;
		nextLevel->clear();
	}

	//目的地のノードまで移動するための経路で
	//次に移動するノードを返す
	return start->toGoal;
}

//指定された座標に一番近いノードを取得
NavNode* NavManager::GetNearNavNode(const CVector& pos)
{
	NavNode* nearNode = nullptr;
	float nearDist = 0.0f;
	for (NavNode* node : mNodes)
	{
		float dist = (pos - node->pos).Length();
		if (dist < nearDist || nearNode == nullptr)
		{
			nearNode = node;
			nearDist = dist;
		}
	}
	return nearNode;
}

//接続できるノードを検索
int NavManager::FindConnectNavNodes(NavNode* node, float distance)
{
	//現在の接続先の情報をクリアしておく
	node->ClearConnects();

	for (NavNode* n : mNodes)
	{
		//自分自身はスルー
		if (n == node) continue;

		//指定された距離の範囲内か判定
		float dist = (n->pos - node->pos).Length();
		if (dist > distance) continue;

		//間に遮蔽物が存在しないか判定
		float outDist = 0.0f;
		if (gMap->CollisionRay(node->pos, n->pos, &outDist))
		{
			if (dist > outDist) continue;
		}

		//両方の条件を満たしたノードを接続リストに追加
		node->AddConnect(n);
	}

	return node->connects.size();
}

//全ノードを描画
void NavManager::Render()
{
	for (NavNode* node : mNodes)
	{
		//接続先のノードまでのラインを描画
		for (NavNode::ConnectData connect : node->connects)
		{
			CVector4 color = CVector4(0.0f, 0.0f, 1.0f, 1.0f);
			Primitive::DrawLine
			(
				node->pos + CVector(0.0f, 0.01f, 0.0f),
				connect.node->pos + CVector(0.0f, 0.01f, 0.0f),
				color,
				3.0f
			);
		}

		//ノードの位置に球を描画
		Primitive::DrawSphere
		(
			node->pos,
			0.3f,
			CVector4(0.0f, 1.0f, 0.0f, 1.0f)
		);
	}
}

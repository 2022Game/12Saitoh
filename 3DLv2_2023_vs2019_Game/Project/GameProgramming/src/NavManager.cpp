#include "NavManager.h"
#include "Primitive.h"
#include "Math.h"
#include "Global.h"

//�i�r�Q�[�V�����}�l�[�W���[�̃C���X�^���X
NavManager* NavManager::mpInstance = nullptr;

//�C���X�^���X�̎擾
NavManager* NavManager::Instance()
{
	//�C���X�^���X���Ȃ����
	if (mpInstance == nullptr)
	{
		//�C���X�^���X�𐶐�����
		mpInstance = new NavManager();
	}
	return mpInstance;
}

//�R���X�g���N�^
NavManager::NavManager()
{
}

//�f�X�g���N�^
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

//�S�m�[�h�̏�Ԃ����Z�b�g
void NavManager::Reset()
{
	for (NavNode* node : mNodes)
	{
		node->Reset();
	}
}

//�o�H�T���p�̃m�[�h��ǉ�
void NavManager::AddNode(NavNode* node)
{
	mNodes.push_back(node);
}

//�o�H�T���p�̃m�[�h����菜��
void NavManager::RemoveNode(NavNode* node)
{
	mNodes.remove(node);
	for (NavNode* n : mNodes)
	{
		n->RemoveConnect(node);
	}
	Reset();
}

//�w�肵���m�[�h����m�[�h�܂ł̌o�H��T��
NavNode* NavManager::Navigate(NavNode* start, NavNode* goal)
{
	if (start == nullptr || goal == nullptr) return nullptr;

	//�m�[�h�̏��i�ړ��R�X�g��o�H��̎��̃m�[�h�̃|�C���^�j�����Z�b�g
	Reset();

	NavNodeList work1;
	NavNodeList work2;

	NavNodeList* currLevel = &work1;
	NavNodeList* nextLevel = &work2;
	NavNodeList* for_swap;

	//�ړI�n���珇�ԂɃR�X�g���������邽�߁A
	//�ŏ��̌������X�g�ɂ͖ړI�n�̃m�[�h��ǉ�
	goal->cost = 0;
	currLevel->push_back(goal);

	NavNodeList::iterator it;
	int nodeCost;
	//�������X�g�̍Ō�܂Ń��[�v����
	while (currLevel->size())
	{
		for (it = currLevel->begin(); it != currLevel->end(); it++)
		{
			//�������̃m�[�h�̐ڑ����X�g��S�Ē��ׂ�
			for (NavNode::ConnectData connect : (*it)->connects)
			{
				//�ړI�n���玩�g�̃m�[�h�܂ł̈ړ��R�X�g��
				//���g�̃m�[�h����ڑ���̃m�[�h�܂ł̈ړ��R�X�g�����Z���āA
				//�ړI�n����ڑ���̃m�[�h�܂łɊ|����R�X�g�����߂�
				nodeCost = (*it)->cost + connect.cost;

				//����̌����������́A���ɓo�^����Ă���ړ��R�X�g��菭�Ȃ��Ȃ�΁A
				if (connect.node->cost == -1 || nodeCost < connect.node->cost)
				{
					//�ړ��R�X�g�ƈړ����̃m�[�h��ݒ�
					connect.node->cost = nodeCost;
					connect.node->toGoal = (*it);
				}
				else
				{
					continue;
				}

				//���̌������X�g�ɐڑ���̃m�[�h��ǉ�
				nextLevel->push_back(connect.node);
			}
		}

		//�������X�g�����ւ���
		for_swap = currLevel;
		currLevel = nextLevel;
		nextLevel = for_swap;
		nextLevel->clear();
	}

	//�ړI�n�̃m�[�h�܂ňړ����邽�߂̌o�H��
	//���Ɉړ�����m�[�h��Ԃ�
	return start->toGoal;
}

//�w�肳�ꂽ���W�Ɉ�ԋ߂��m�[�h���擾
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

//�ڑ��ł���m�[�h������
int NavManager::FindConnectNavNodes(NavNode* node, float distance)
{
	//���݂̐ڑ���̏����N���A���Ă���
	node->ClearConnects();

	for (NavNode* n : mNodes)
	{
		//�������g�̓X���[
		if (n == node) continue;

		//�w�肳�ꂽ�����͈͓̔�������
		float dist = (n->pos - node->pos).Length();
		if (dist > distance) continue;

		//�ԂɎՕ��������݂��Ȃ�������
		float outDist = 0.0f;
		if (gMap->CollisionRay(node->pos, n->pos, &outDist))
		{
			if (dist > outDist) continue;
		}

		//�����̏����𖞂������m�[�h��ڑ����X�g�ɒǉ�
		node->AddConnect(n);
	}

	return node->connects.size();
}

//�S�m�[�h��`��
void NavManager::Render()
{
	for (NavNode* node : mNodes)
	{
		//�ڑ���̃m�[�h�܂ł̃��C����`��
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

		//�m�[�h�̈ʒu�ɋ���`��
		Primitive::DrawSphere
		(
			node->pos,
			0.3f,
			CVector4(0.0f, 1.0f, 0.0f, 1.0f)
		);
	}
}

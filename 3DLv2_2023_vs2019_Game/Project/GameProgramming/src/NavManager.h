#ifndef NAVMANAGER_H
#define NAVMANAGER

#include <list>
#include "NavNode.h"

//�o�H�T���Ǘ��N���X
class NavManager
{
public:
	static NavManager* Instance();

	NavManager();
	~NavManager();

	//�o�H�T���p�̃m�[�h��ǉ�
	void AddNode(NavNode* node);
	//�o�H�T���p�̃m�[�h����菜��
	void RemoveNode(NavNode* node);

	//�w�肵���m�[�h����m�[�h�܂ł̌o�H��T��
	NavNode* Navigate(NavNode* start, NavNode* goal);

	//�w�肳�ꂽ���W�Ɉ�ԋ߂��m�[�h���擾
	NavNode* GetNearNavNode(const CVector& pos);

	//�ڑ��ł���m�[�h������
	//�i�ԂɎՕ���������m�[�h�ɂ͐ڑ��ł��Ȃ��j
	int FindConnectNavNodes(NavNode* node, float distance);

	//�S�m�[�h��`��
	void Render();

private:
	static NavManager* mpInstance;

	//�T���p�m�[�h�̃��X�g
	NavNodeList mNodes;

	//�S�m�[�h�̏�Ԃ����Z�b�g
	void Reset();
};

#endif
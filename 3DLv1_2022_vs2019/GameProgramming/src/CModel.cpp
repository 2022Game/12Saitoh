#include "CModel.h"
//�W�����o�͂̃C���N���[�h
#include <stdio.h>
//���f���t�@�C���̓���
//Load(���f���t�@�C����, �}�e���A���t�@�C����)
void CModel::Load(char* obj, char* mtl)
{
	//�t�@�C���|�C���^�ϐ��̍쐬
	FILE* fp;
	//�t�@�C������f�[�^�����
	//���̓G���A���쐬����
	char buf[256];

	//�t�@�C���̃I�[�v��
	//fopen(�t�@�C����, ���[�h)
	//�I�[�v���ł��Ȃ��Ƃ���NULL��Ԃ�
	fp = fopen(mtl, "r");
	//�t�@�C���I�[�v���G���[�̔���
	//fp��NULL�̎��̓G���[
	if (fp == NULL)
	{
		//�R���\�[���ɃG���[�o�͂��Ė߂�
		printf("%s,file open erro\n",mtl);
		return;
	}
	//�t�@�C������1�s����
	//fqets(���̓G���A,�G�C��T�C�Y,�t�@�C���|�C���^)
	// //�t�@�C���̍Ō�ɂȂ��NULL��Ԃ�
	while (fgets(buf, sizeof(buf), fp) != NULL)
	{
		//���͂����l���R���\�[���ɏo�͂���
		printf("%s", buf);
	}
	fp = fopen(obj, "r");
	if (fp == NULL)
	{
		printf("%s,file open erro\n", obj);
		return;
	}
	while (fgets(buf, sizeof(buf), fp) != NULL)
	{
		printf("%s", buf);
	}
	//�t�@�C���̃N���[�Y
	fclose(fp);
}
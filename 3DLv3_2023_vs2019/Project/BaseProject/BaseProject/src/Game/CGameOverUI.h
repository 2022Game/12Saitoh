#pragma once
#include "CResultUIBase.h"

class CGameOverUI : public CResultUIBase
{
public:
	// �R���X�g���N�^
	CGameOverUI();
	// �f�X�g���N�^
	~CGameOverUI();

	// �X�V����
	void Update() override;
	// �`�揈��
	void Render() override;

	// UI�����J�n
	void Start() override;
	// UI�����I��
	void End() override;
private:
};
#pragma once
#include "CResultUIBase.h"

class CGameClearUI : public CResultUIBase
{
public:
	// �R���X�g���N�^
	CGameClearUI();
	// �f�X�g���N�^
	~CGameClearUI();

	// �X�V����
	void Update() override;
	// �`�揈��
	void Render() override;

	// UI�����J�n
	void Start() override;
	// UI�����I��
	void End() override;
};
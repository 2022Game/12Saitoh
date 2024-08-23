#pragma once
#include "CObjectBase.h"

class CModel;
class CSky : public CObjectBase
{
public:
	CSky();
	~CSky();

	void Update();
	void Render();

private:
	CModel* mpModel;
};
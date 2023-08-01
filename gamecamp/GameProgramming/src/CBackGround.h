#pragma once
#include "CCharacter.h"
#include "CCharacterManager.h"

class CBackGround : public CCharacter
{
private:
	/*
	”wŒi‚ªˆÚ“®’†‚©”»’è
	true:ˆÚ“®’†
	false:’â~’†
	*/
	static bool mMoveStage;
public:
	//ƒRƒ“ƒXƒgƒ‰ƒNƒ^
	CBackGround::CBackGround(float x, float y, float w, float h, float l, float r, float b, float t, CTexture* pt);
	//XVˆ—
	void Update();
	//”wŒi‚Ìó‘Ô‚ğæ“¾
	static bool GetMoveStage();
};
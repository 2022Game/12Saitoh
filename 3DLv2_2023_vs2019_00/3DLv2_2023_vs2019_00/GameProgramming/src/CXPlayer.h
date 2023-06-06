#ifndef CXPLAYER
#define CXPLAYER

#include "CXCharacter.h"
#include "CInput.h"

class CXPlayer : public CXCharacter
{
private:
	CInput mInput;

public:
	void Update();
};
#endif
#pragma once
#include <vector>
#include "CTexture.h"
#include "CVector.h"
#include "CMatrix.h"
#include "CUi.h"
#include "CInput.h"

class CApplication
{
private:
	//UIクラスのポインタ
	static CUi* spUi;
	//モデルビューの逆行列
	static CMatrix mModelViewInverse;
	static CTexture mTexture;

	CInput mInput;

public:
	~CApplication();
	//UIクラスのインスタンスを取得
	static CUi* Ui();
	//モデルビュー行列の取得
	static const CMatrix& ModelViewInverse();
	static CTexture* Texture();
	//最初に一度だけ実行するプログラム
	void Start();
	//繰り返し実行するプログラム
	void Update();
};
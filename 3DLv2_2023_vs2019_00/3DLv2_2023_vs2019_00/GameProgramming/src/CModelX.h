#ifndef CMODELX_H
#define CMODELX_H
#include <vector>      //vectorクラスのインクルード
#include "CMatrix.h"   //マトリクスクラスのインクルード

#define MODEL_FILE "res\\sample.blend.x"
//領域解放をマクロ化
#define SAFE_DELETE_ARRA(a) {if(a) delete[] a; a = nullptr;}

class CModelX;         //CModelクラスの宣言
class CModelXFrame;    //CModelXFrameクラスの宣言
/*
CModel
Xファイル形式の3Dモデルデータをプログラムで認識する
*/
class CModelX {
	friend CModelXFrame;
private:
	std::vector<CModelXFrame*> mFrame;  //フレームの配列
	char* GetToken();  //単語の取り出し
	char* mpPointer;   //読み込み位置
	char mToken[1024]; //取り出した単語の領域

	//cが区切り文字ならtrueを返す
	bool IsDelimiter(char c);
public:
	//コンストラクタ
	CModelX();
	//デストラクタ
	~CModelX();

	//ノードの読み飛ばし
	void SlipNode();
	//ファイル読み込み
	void Load(char* file);
};

//CModelXFrameクラスの定義
class CModelXFrame {
	friend CModelX;
private:
	std::vector<CModelXFrame*> mChild; //子フレームの配列
	CMatrix mTransformMatrix;  //変換行列
	char* mpName;  //フレーム名前
	int mIndex;  //フレーム番号
public:
	//コンストラクタ
	CModelXFrame(CModelX* model);
	//デストラクタ
	~CModelXFrame() ;
};
#endif

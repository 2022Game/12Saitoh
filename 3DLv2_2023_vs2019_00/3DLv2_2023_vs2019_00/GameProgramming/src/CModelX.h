#ifndef CMODELX_H
#define CMODELX_H

#define MODEL_FILE "res\\sample.blend.x"

//領域解放をマクロ化
#define SAFE_DELETE_ARRA(a) {if(a) delete[] a; a = nullptr;}
/*
CModel
Xファイル形式の3Dモデルデータをプログラムで認識する
*/
class CModelX {
private:
	char* GetToken();  //単語の取り出し
	char* mpPointer;   //読み込み位置
	char mToken[1024]; //取り出した単語の領域

	//cが区切り文字ならtrueを返す
	bool IsDelimiter(char c);
public:
	//コンストラクタ
	CModelX();

	//ファイル読み込み
	void Load(char* file);
};
#endif

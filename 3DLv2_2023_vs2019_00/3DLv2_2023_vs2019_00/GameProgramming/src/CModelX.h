#ifndef CMODELX_H
#define CMODELX_H
#include <vector>      //vectorクラスのインクルード
#include "CMatrix.h"   //マトリクスクラスのインクルード
#include "CVector.h"   //Vectorクラスのインクルード

#define MODEL_FILE "res\\sample.blend.x"
//領域解放をマクロ化
#define SAFE_DELETE_ARRA(a) {if(a) delete[] a; a = nullptr;}

class CModelX;         //CModelクラスの宣言
class CModelXFrame;    //CModelXFrameクラスの宣言
class CMesh;           //CMeshクラスの定義を追加する
class CMaterial;       //マテリアルの宣言

/*
CModel
Xファイル形式の3Dモデルデータをプログラムで認識する
*/
class CModelX {
	friend CModelXFrame;
	friend CMesh;
private:
	std::vector<CModelXFrame*> mFrame;  //フレームの配列
	char* mpPointer;   //読み込み位置
	char mToken[1024]; //取り出した単語の領域

	//cが区切り文字ならtrueを返す
	bool IsDelimiter(char c);
public:
	//コンストラクタ
	CModelX();
	//デストラクタ
	~CModelX();

	//単語の取り出し
	char* GetToken();
	char* Token();

	//トークンがなくなったらtre
	bool EOT();

	//ノードの読み飛ばし
	void SlipNode();
	//ファイル読み込み
	void Load(char* file);

	//描画処理
	void Render();
};

//CModelXFrameクラスの定義
class CModelXFrame {
	friend CModelX;
private:
	std::vector<CModelXFrame*> mChild; //子フレームの配列
	CMesh* mpMesh;  //Meshデータ
	CMatrix mTransformMatrix;  //変換行列
	char* mpName;  //フレーム名前
	int mIndex;  //フレーム番号
public:
	//コンストラクタ
	CModelXFrame(CModelX* model);
	//デストラクタ
	~CModelXFrame() ;

	//描画処理
	void Render();
};

//Meshクラスの定義
class CMesh {
	friend CModelX;
	friend CModelXFrame;
private:
	int mFaceNum;          //面数
	int mVertexNum;	   	   //頂点数
	int mNormalNum;        //法線数
	int mMaterialNum;      //マテリアル数
	int mMaterialIndexNum; //マテリアル番号数(面数)
	int* mpMaterialIndex;  //マテリアル番号
	int* mpVertexIndex;    //面を構成する頂点インデックス

	std::vector<CMaterial*> mMaterial; //マテリアルデータ
	CVector* mpVertex;	   //頂点データ
	CVector* mpNormal;     //法線ベクトル

public:
	//コンストラクタ
	CMesh();
	//デストラクタ
	~CMesh();

	//読み込み処理
	void Init(CModelX* model);

	//描画処理
	void Render();
};
#endif

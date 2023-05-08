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
class CMesh;           //CMeshクラスの宣言
class CMaterial;       //マテリアルの宣言
class CSkinWeights;    //スキンウェイトクラスの宣言
class CAnimationSet;   //アニメーションセットクラスの宣言

/*
CModel
Xファイル形式の3Dモデルデータをプログラムで認識する
*/
class CModelX {
	friend CModelXFrame;
	friend CMesh;
	friend CAnimationSet;
private:
	std::vector<CModelXFrame*> mFrame;  //フレームの配列
	std::vector<CAnimationSet*> mAnimationSet;	//アニメーションセット配列
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

	//トークンがなくなったらtrue
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
	friend CSkinWeights;
private:
	int mFaceNum;          //面数
	int mVertexNum;	   	   //頂点数
	int mNormalNum;        //法線数
	int mMaterialNum;      //マテリアル数
	int mMaterialIndexNum; //マテリアル番号数(面数)
	int* mpMaterialIndex;  //マテリアル番号
	int* mpVertexIndex;    //面を構成する頂点インデックス

	std::vector<CSkinWeights*> mSkinWeights;	//スキンウェイト
	std::vector<CMaterial*> mMaterial; //マテリアルデータ
	CVector* mpVertex;	   //頂点データ
	CVector* mpNormal;     //法線ベクトル
	CSkinWeights* mpSkinWeghts;

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

/*
CSkinWeights
スキンウェイトクラス
*/
class CSkinWeights
{
	friend CModelX;
	friend CMesh;
private:
	char* mpFrameName;	//フレーム名
	int mFrameIndex;	//フレーム番号
	int mIndexNum;		//頂点番号数
	int* mpIndex;		//頂点番号配列
	float* mpWeight;	//頂点ウェイト配列
	CMatrix mOffset;	//オフセットマトリックス

public:
	CSkinWeights(CModelX* model);
	~CSkinWeights();
	const int& FrameIndex();
	const CMatrix& Offset();
};

/*
CAnimationSet
アニメーションセット
*/
class CAnimationSet
{
private:
	char* mpName;	//アニメーションセット名

public:
	CAnimationSet(CModelX* model);
	~CAnimationSet();
};
#endif
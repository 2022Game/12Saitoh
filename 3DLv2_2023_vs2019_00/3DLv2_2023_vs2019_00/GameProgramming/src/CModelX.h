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
class CAnimation;	   //アニメーションクラスの宣言
class CAnimationKey;   //アニメーションキークラスの宣言

/*
CModel
Xファイル形式の3Dモデルデータをプログラムで認識する
*/
class CModelX {
	friend CModelXFrame;
	friend CMesh;
	friend CAnimationSet;
	friend CAnimation;
private:
	std::vector<CModelXFrame*> mFrame;  //フレームの配列
	std::vector<CAnimationSet*> mAnimationSet;	//アニメーションセット配列
	char* mpPointer;   //読み込み位置
	char mToken[1024]; //取り出した単語の領域

	//cが区切り文字ならtrueを返す
	bool IsDelimiter(char c);

public:
	std::vector<CModelXFrame*>& Frames();
	std::vector<CAnimationSet*>& CModelX::AnimationSet();
	//フレーム名に該当するフレームのアドレスを返す
	CModelXFrame* FinedFrame(char* name);

	//コンストラクタ
	CModelX();
	//デストラクタ
	~CModelX();

	//単語の取り出し
	char* GetToken();
	char* Token();

	//トークンがなくなったらtrue
	bool EOT();

	//頂点にアニメーションを適用
	void AnimateVertex();
	//スキンウェイトのフレーム番号設定
	void SetSkinWeightFrameIndex();
	//フレームの変換行列をアニメーションデータで更新する
	void AnimateFrame();
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
	friend CAnimation;
	friend CAnimationSet;
private:
	std::vector<CModelXFrame*> mChild; //子フレームの配列
	CMesh* mpMesh;  //Meshデータ
	CMatrix mTransformMatrix;  //変換行列
	CMatrix mCombinedMatrix;   //合成行列
	char* mpName;  //フレーム名前
	int mIndex;  //フレーム番号

public:
	//コンストラクタ
	CModelXFrame(CModelX* model);
	//デストラクタ
	~CModelXFrame();

	int Index();

	const CMatrix& CombinedMatrix();
	//合成行列の作成
	void AnimateCombined(CMatrix* parent);
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
	CVector* mpVertex;			//頂点データ
	CVector* mpNormal;			//法線ベクトル
	CVector* mpAnimateVertex;	//アニメーション用頂点
	CVector* mpAnimateNormal;	//アニメーション用法線
	CSkinWeights* mpSkinWeghts;

public:
	//コンストラクタ
	CMesh();
	//デストラクタ
	~CMesh();

	//頂点にアニメーション適用
	void AnimateVertex(CModelX* model);
	//スキンウェイトにフレーム番号を設定する
	void SetSkinWeightFrameIndex(CModelX* model);
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
	friend CModelX;
private:
	char* mpName;	//アニメーションセット名
	float mTime;	//現在時間
	float mWeight;	//重み
	float mMaxTime;	//最大時間
	std::vector<CAnimation*> mAnimation;

public:
	CAnimationSet(CModelX* model);
	~CAnimationSet();

	float Time();
	float MaxTime();
	std::vector<CAnimation*>& Animation();
	void AnimateMarix(CModelX* model);
	void Time(float time);		//時間の設定
	void Weight(float weight);	//重みの設定
};

/*
CAnimation
アニメーションクラス
*/
class CAnimation
{
	friend CModelX;
	friend CAnimationSet;
private:
	char* mpFrameName;		//フレーム名
	int mFrameIndex;		//フレーム番号
	int mKeyNum;			//キー数(時間数)
	CAnimationKey* mpKey;	//キー配列

public:
	CAnimation(CModelX* model);
	~CAnimation();
};

/*
CAnimationKey
アニメーションキークラス
*/
class CAnimationKey
{
	friend CModelX;
	friend CAnimation;
	friend CAnimationSet;
private:
	float mTime;		//時間
	CMatrix mMatrix;	//行列
};
#endif
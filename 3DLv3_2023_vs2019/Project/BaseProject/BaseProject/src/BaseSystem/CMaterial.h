#ifndef CMATERIAL_H
#define CMATERIAL_H
#include "CTexture.h"
#include "CModelX.h"
#define MATERIAL_NAME_LEN 64 //名前の長さ

class CMyShader;
/*
マテリアルクラス
マテリアルのデータを扱う
*/
class CMaterial {
	friend CMyShader;
private:
	//マテリアル毎の頂点数
	int mVertexNum;
	//拡散光の色RGBA
	float mDiffuse[4];
	float mPower;
	float mSpecular[3];
	float mEmissive[3];

	//テクスチャファイル名
	char* mpTextureFilename;
	//マテリアル名
	char mName[MATERIAL_NAME_LEN + 1];
	//テクスチャ
	CTexture mTexture;

public:
	//テクスチャの取得
	CTexture* Texture();
	//マテリアルを無効にする
	void Disabled();
	//デフォルトコンストラクタ
	CMaterial();
	CMaterial(CModelX* model);
	~CMaterial();
	//マテリアルを有効にする
	void Enabled();
	//マテリアルの名前の取得
	char* Name();
	//マテリアルの名前を設定する
	//Name(マテリアルの名前)
	void Name(char* name);
	//mDiffuse配列の取得
	float* Diffuse();
	//頂点数の設定
	//VertexNum(頂点数)
	void VertexNum(int num);
	//頂点数の取得
	int VertexNum();
};

#endif


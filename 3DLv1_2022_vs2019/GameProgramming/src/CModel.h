#include<vector>
#include "CTriangle.h"
#ifndef CMODEL_H
#define CMEDEL_H
/*
モデルクラス
モデルデータの入力や表示
*/
class CModel
{
private:
	//三角形の可変長配列
	std::vector<CTriangle> mTriangles;
public:
	//描画
	void Render();
	//モデルファイルの入力
	//Load(モデルファイル名, マテリアルファイル名)
	void Load(char* obj, char* mtl);
};
#endif
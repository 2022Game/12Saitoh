#ifndef CACTIONCAMERA_H
#define CACTIONCAMERA_H

#include "CVector.h"
#include "CTransform.h"
#include "CMatrix.h"
#include "CInput.h"

/*
カメラクラス
*/
class CActionCamera :  public CTransform
{
private:
	//インスタンス
	static CActionCamera* spInstance;
	//視点
	CVector mEye;
	//注視点
	CVector mCenter;
	//上方向
	CVector mUp;
	//モデルビュー行列
	CMatrix mModelView;
	//入力クラス
	CInput mInput;

public:
	//カメラのX軸取得
	CVector VectorX();
	//カメラのZ軸取得
	CVector VectorZ();
	//コンストラクタ
	CActionCamera();
	//カメラ設定(距離, X回転, Y回転)
	void Set(float distance, float xaxis, float yaxis);
	//インスタンスの取得
	static CActionCamera* Instance();

	//カメラ更新
	void Update();
	//カメラ適用
	void Render();

};
#endif
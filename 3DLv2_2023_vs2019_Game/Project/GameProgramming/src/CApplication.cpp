#include "CApplication.h"
#include "glut.h"
#include "CVector.h"
#include "CTriangle.h"
#include "CPlayer.h"
#include "CEnemy.h"
#include "CBillBoard.h"
#include "CEnemy3.h"
#include "CAlly.h"
#include <stdlib.h>
#include <time.h>

#define MODEL_OBJ "res\\SnowGolem.obj","res\\SnowGolem.mtl"	//モデルデータの指定
#define MODEL_BACKGROUND "res\\Graund.obj", "res\\Graund.mtl"		//背景モデルデータの指定
#define MODEL_C5 "res\\c5.obj", "res\\c5.mtl"

int CApplication::mCameraFlag = 0;
CUi* CApplication::spUi = nullptr;

CUi* CApplication::Ui()
{
	return spUi; //インスタンスのポインタを返す
}

CMatrix CApplication::mModelViewInverse;

const CMatrix& CApplication::ModelViewInverse()
{
	return mModelViewInverse;
}
//クラスのstatic変数
CTexture CApplication::mTexture;

CTexture* CApplication::Texture()
{
	return &mTexture;
}

int CApplication::CameraFlag()
{
	return mCameraFlag;
}

void CApplication::Start()
{
	//ランダム値の取得方法
	srand((unsigned int)time(NULL));
	for (int i = 0; i < 10; i++)
	{
		//最小値0 ,取得範囲3 
		printf("%d\n", 0 + rand() % 3);
	}

	spUi = new CUi(); //Uiクラスの生成
	//モデルファイルの入力
	mModel.Load(MODEL_OBJ);
	//背景モデルの読み込み;
	mBackGround.Load(MODEL_BACKGROUND);
	mMatrix.Print();
	mPlayer.Model(&mModel);
	mPlayer.Scale(CVector(0.03f, 0.03f, 0.03f));
	mPlayer.Position(CVector(0.0f, 0.0f, -3.0f));
	mPlayer.Rotation(CVector(0.0f, 0.0f, 0.0f));
	mpMapManager = new CMapManager();

	new CEnemy3(CVector(-5.0f, 0.0f, -10.0f), CVector(), CVector(0.03f, 0.03f, 0.03f));
	//new CEnemy3(CVector(5.0f, 0.0f, -10.0f), CVector(), CVector(0.03f, 0.03f, 0.03f));
//	new CAlly(CVector(5.0f, 0.0f, 10.0f), CVector(), CVector(0.03f, 0.03f, 0.03f));
//	new CAlly(CVector(-5.0f, 0.0f, 10.0f), CVector(), CVector(0.03f, 0.03f, 0.03f));

	//ビルボードの作成
	new CBillBoard(CVector(-6.0f, 3.0f, -10.0f), 1.0f, 1.0f);
	//背景モデルから三角コライダを生成
	// 親インスタンスと親行列はな
	mColliderMesh.Set(nullptr, nullptr, &mBackGround);
}

void CApplication::Update()
{
	//タスクマネージャの更新
	CTaskManager::Instance()->Update();
	//タスクマネージャの衝突処理
	CTaskManager::Instance()->Collision();

	//デバッグ用(カメラの切り替え)
	if (mInput.PullKey('C'))
	{
		mCameraFlag++;
		if (mCameraFlag >= 3)
		{
			mCameraFlag = 0;
		}
	}

	//頂点1, 頂点2, 頂点3, 法線データの作成
	CVector v0, v1, v2, n;
	//法線を上向きで設定する
	n.Set(0.0f, 1.0f, 0.0f);
	//頂点1の座標を設定する
	v0.Set(0.0f, 0.0f, 0.5f);
	//頂点2の座標を設定する
	v1.Set(1.0f, 0.0f, 0.0f);
	//頂点3の座標を設定する
	v2.Set(0.0f, 0.0f, -0.5f);
	
	//カメラのパラメータを作成する
	CVector e, c, u; //視点、注視点、上方向
	if (mCameraFlag == 0)
	{
		//視点を求める
		e = mPlayer.Position() + CVector(0.0f, 2.0f, 0.0f) * mPlayer.MatrixRotate();
		//注視点を求める
		c = mPlayer.Position() + CVector(0.0f, 2.0f, 1.0f) * mPlayer.MatrixRotate();
		//上方向を求める
		u = CVector(0.0f, 0.1f, 0.0f) * mPlayer.MatrixRotate();
		//カメラの設定
		gluLookAt(e.X(), e.Y(), e.Z(), c.X(), c.Y(), c.Z(), u.X(), u.Y(), u.Z());
	}
	else if (mCameraFlag == 1)
	{
		//視点を求める
		e = mPlayer.Position() + CVector(-0.9f, 3.0f, -12.0f) * mPlayer.MatrixRotate();
		//注視点を求める
		c = mPlayer.Position();
		//上方向を求める
		u = CVector(0.0f, 1.0f, 0.0f) * mPlayer.MatrixRotate();
		//カメラの設定
		gluLookAt(e.X(), e.Y(), e.Z(), c.X(), c.Y(), c.Z(), u.X(), u.Y(), u.Z());
	}
	else
	{
		//視点を求める
		e = mPlayer.Position() + CVector(0.5f, 3.0f, 12.0f) * mPlayer.MatrixRotate();
		//注視点を求める
		c = mPlayer.Position();
		//上方向を求める
		u = CVector(0.0f, 1.0f, 0.0f) * mPlayer.MatrixRotate();
		//カメラの設定
		gluLookAt(e.X(), e.Y(), e.Z(), c.X(), c.Y(), c.Z(), u.X(), u.Y(), u.Z());

	}

	//モデルビュー行列の取得
	glGetFloatv(GL_MODELVIEW_MATRIX, mModelViewInverse.M());
	//逆行列の取得
	mModelViewInverse = mModelViewInverse.Transpose();
	mModelViewInverse.M(0, 3, 0);
	mModelViewInverse.M(1, 3, 0);
	mModelViewInverse.M(2, 3, 0);
	mBackGround.Render();
	//タスクリストの削除
	CTaskManager::Instance()->Delete();
	//タスクマネージャの描画
	CTaskManager::Instance()->Render();
	spUi->Render(); //Uiの描画
	//コリジョンマネージャ描画
	CCollisionManager::Instance()->Render();
}

CApplication::~CApplication()
{
	delete spUi; //インスタンスUiの削除
}
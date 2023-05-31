#include "CApplication.h"
#include "glut.h"
#include "CVector.h"
#include "CTriangle.h"
#include "CPlayer.h"
#include "CEnemy.h"
#include "CBillBoard.h"
#include "CEnemy3.h"

#define MODEL_OBJ "res\\SnowGolem.obj","res\\SnowGolem.mtl"//モデルデータの指定
#define MODEL_BACKGROUND "res\\sky.obj", "res\\sky.mtl"//背景モデルデータの指定
#define MODEL_C5 "res\\c5.obj", "res\\c5.mtl"

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

void CApplication::Start()
{
	spUi = new CUi(); //Uiクラスの生成
	mEye = CVector(1.0f, 2.0f, 3.0f);
	//モデルファイルの入力
	mModel.Load(MODEL_OBJ);
	//C5モデルの読み込み;
	mBackGround.Load(MODEL_BACKGROUND);
	mMatrix.Print();
	mPlayer.Model(&mModel);
	mPlayer.Scale(CVector(0.1f, 0.1f, 0.1f)*0.2);
	mPlayer.Position(CVector(0.0f, -1.0f, -3.0f));
	mPlayer.Rotation(CVector(0.0f, 0.0f, 0.0f));
	mpWallManager = new CWallManager();

	//new CEnemy(&mModel, CVector(0.0f, 10.0f, -100.0f),
	//	CVector(), CVector(0.1f, 0.1f, 0.1f));
	//new CEnemy(&mModel, CVector(30.0f, 10.0f, -130.0f),
	//	CVector(), CVector(0.1f, 0.1f, 0.1f));
	new CEnemy3(CVector(-5.0f, 1.0f, -10.0f), CVector(), CVector(0.01f, 0.01f, 0.01f));
	new CEnemy3(CVector(5.0f, 1.0f, -10.0f), CVector(), CVector(0.01f, 0.01f, 0.01f));
	//ビルボードの作成
	new CBillBoard(CVector(-6.0f, 3.0f, -10.0f), 1.0f, 1.0f);
	//背景モデルから三角コライダを生成
	// 親インスタンスと親行列はなし
	mColliderMesh.Set(nullptr, nullptr, &mBackGround);
}

void CApplication::Update()
{
	//タスクマネージャの更新
	CTaskManager::Instance()->Update();
	//コリジョンマネージャの衝突処理
	CTaskManager::Instance()->Collision();

	if (mInput.Key('J'))
	{
		mEye = mEye - CVector(0.1f, 0.0f, 0.0f);
	}
	if (mInput.Key('L'))
	{
		mEye = mEye + CVector(0.1f, 0.0f, 0.0f);
	}
	if (mInput.Key('I'))
	{
		mEye = mEye - CVector(0.0f, 0.0f, 0.1f);
	}
	if (mInput.Key('K'))
	{
		mEye = mEye + CVector(0.0f, 0.0f, 0.1f);
	}
	if (mInput.Key('O'))
	{
		mEye = mEye + CVector(0.0f, 0.1f, 0.0f);
	}
	if (mInput.Key('M'))
	{
		mEye = mEye - CVector(0.0f, 0.1f, 0.0f);
	}

	//デバッグ用(カメラの切り替え)
	if (mInput.PullKey('C'))
	{
		if (CameraFlag == false)
		{
			CameraFlag = true;
		}
		else
		{
			CameraFlag = false;
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
	if (CameraFlag == false)
	{
		//視点を求める
		e = mPlayer.Position() + CVector(0.0f, 1.7f, 0.0f) * mPlayer.MatrixRotate();
		//注視点を求める
		c = mPlayer.Position() + CVector(0.0f, 1.7f, 1.0f) * mPlayer.MatrixRotate();
		//上方向を求める
		u = CVector(0.0f, 0.1f, 0.0f) * mPlayer.MatrixRotate();
		//カメラの設定
		gluLookAt(e.X(), e.Y(), e.Z(), c.X(), c.Y(), c.Z(), u.X(), u.Y(), u.Z());
	}
	else
	{
		//視点を求める
		e = mPlayer.Position() + CVector(-0.9f, 2.0f, -12.0f) * mPlayer.MatrixRotate();
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
	spUi->Render(); //Uiの描画
	//タスクリストの削除
	CTaskManager::Instance()->Delete();
	//タスクマネージャの描画
	CTaskManager::Instance()->Render();
	//コリジョンマネージャ描画
	CCollisionManager::Instance()->Render();
}

CApplication::~CApplication()
{
	delete spUi; //インスタンスUiの削除
}
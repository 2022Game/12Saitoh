#include "CApplication.h"
#include "glut.h"
#include "CVector.h"
#include "CTriangle.h"
#include "CPlayer.h"
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
CMatrix CApplication::mModelViewInverse;
CTexture CApplication::mTexture;

//カメラの設定
void CApplication::Camera()
{
	//デバッグ用(カメラの切り替え)
	if (mInput.PullKey('C'))
	{
		mCameraFlag++;
		if (mCameraFlag >= 3)
		{
			mCameraFlag = 0;
		}
	}
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
}

void CApplication::CreateModel()
{
	//モデルファイルの入力
	mModel.Load(MODEL_OBJ);
	//背景モデルの読み込み;
	mBackGround.Load(MODEL_BACKGROUND);
	mMatrix.Print();
	mpMapManager = new CMapManager();
	mPlayer.Scale(CVector(0.03f, 0.03f, 0.03f));
	mPlayer.Position(CVector(0.0f, 0.0f, -3.0f));
	mPlayer.Rotation(CVector(0.0f, 0.0f, 0.0f));

	new CEnemy3(CVector(-5.0f, 0.0f, -10.0f), CVector(), CVector(0.03f, 0.03f, 0.03f));
	//new CEnemy3(CVector(5.0f, 0.0f, -10.0f), CVector(), CVector(0.03f, 0.03f, 0.03f));
	//new CAlly(CVector(5.0f, 0.0f, 10.0f), CVector(), CVector(0.03f, 0.03f, 0.03f));
	//new CAlly(CVector(-5.0f, 0.0f, 10.0f), CVector(), CVector(0.03f, 0.03f, 0.03f));

	//ビルボードの作成
	//new CBillBoard(CVector(-6.0f, 3.0f, -10.0f), 1.0f, 1.0f);
	//背景モデルから三角コライダを生成
	// 親インスタンスと親行列はな
	mColliderMesh.Set(nullptr, nullptr, &mBackGround);
}

CApplication::~CApplication()
{
	delete spUi; //インスタンスUiの削除
}

CUi* CApplication::Ui()
{
	return spUi; //インスタンスのポインタを返す
}

const CMatrix& CApplication::ModelViewInverse()
{
	return mModelViewInverse;
}

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
	mState = EState::ESTART;
}

void CApplication::Update()
{

	switch (mState)
	{
	case EState::ESTART:
		mPlayer.Model(&mModel);
		spUi->Start();
		//タスクマネージャの更新
		CTaskManager::Instance()->Update();
		//タスクマネージャの描画
		CTaskManager::Instance()->Render();
		if (mInput.PushKey(VK_RETURN))
		{
			mState = EState::EPLAY;
			//モデルを生成
			CApplication::CreateModel();
		}
		break;
	case EState::EPLAY:
		CTaskManager::Instance()->Update();
		CApplication::Camera();
		if (CPlayer::HP() == 0)
		{
			mState = EState::EOVER;
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
		//タスクマネージャの衝突処理
		CTaskManager::Instance()->Collision();
		//タスクマネージャの描画
		CTaskManager::Instance()->Render();
		//コリジョンマネージャ描画
		CCollisionManager::Instance()->Render();
		break;
	case EState::ECLEAR:
		break;
	case EState::EOVER:
		spUi->Over();
		CTaskManager::Instance()->Update();
		//タスクマネージャの描画
		CTaskManager::Instance()->Render();
		break;
	}
}
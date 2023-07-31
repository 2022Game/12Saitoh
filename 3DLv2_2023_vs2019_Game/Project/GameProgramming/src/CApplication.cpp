#include "CApplication.h"
#include <stdlib.h>
#include "glut.h"
#include "CVector.h"
#include "NavManager.h"
#include "CSceneManager.h"
#include "CTaskManager.h"
#include "CCollisionManager.h"

CUi* CApplication::spUi = nullptr;


CApplication::~CApplication()
{
	delete spUi; //インスタンスUiの削除
}

CUi* CApplication::Ui()
{
	return spUi; //インスタンスのポインタを返す
}

CMatrix CApplication::mModelViewInverse;
const CMatrix& CApplication::ModelViewInverse()
{
	return mModelViewInverse;
}

CTexture CApplication::mTexture;
CTexture* CApplication::Texture()
{
	return &mTexture;
}

void CApplication::Start()
{
	//ゲームシーンの読み込み
	CSceneManager::Instance()->LoadScene(EScene::eTitle);
}

void CApplication::Update()
{
	//タスクマネージャの更新
	CTaskManager::Instance()->Update();
	//シーンマネージャの更新
	CSceneManager::Instance()->Update();
	//モデルビュー行列の取得
	glGetFloatv(GL_MODELVIEW_MATRIX, mModelViewInverse.M());
	//逆行列の取得
	mModelViewInverse = mModelViewInverse.Transpose();
	mModelViewInverse.M(0, 3, 0);
	mModelViewInverse.M(1, 3, 0);
	mModelViewInverse.M(2, 3, 0);

	//タスクリストの削除
	CTaskManager::Instance()->Delete();
	//タスクマネージャの衝突処理
	CTaskManager::Instance()->Collision();
	//タスクマネージャの描画
	CTaskManager::Instance()->Render();
	//コリジョンマネージャ描画
	CCollisionManager::Instance()->Render();
	//デバッグ用
	//ノードの表示
	static bool sw = false;
	if (mInput.PushKey('V'))
	{
		sw = !sw;
	}
	if (sw)
	{
		NavManager::Instance()->Render();
	}
}
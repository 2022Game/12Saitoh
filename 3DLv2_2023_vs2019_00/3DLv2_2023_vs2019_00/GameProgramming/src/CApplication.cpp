#include "CApplication.h"
//OpenGL
#include "glut.h"
#include "CVector.h"
#include "CTriangle.h"
#include "CMatrix.h"
#include "CTransform.h"
#include "CCollisionManager.h"
#include "CBillBoard.h"
#include "CEnemy3.h"

#define SOUND_BGM "res\\mario.wav" //BGM音声ファイル
#define SOUND_OVER "res\\mdai.wav" //ゲームオーバー音声ファイル
#define MODEL_OBJ "res\\f14.obj", "res\\f14.mtl"//モデルデータの指定
#define MODEL_C5 "res\\c5.obj", "res\\c5.mtl" //敵輸送機モデル
#define MODEL_BACKGROUND  "res\\sky.obj", "res\\sky.mtl"//背景モデルデータの指定
#define MODEL_KNIGHT "res\\knight\\knight_low.X"

CUi* CApplication::spUi = nullptr;
CTexture CApplication::mTexture;
CMatrix CApplication::mModelViewInverse;

CApplication::~CApplication()
{
	delete spUi;	//インスタンスUiの削除
}
CUi* CApplication::Ui()
{
	return spUi;
}

const CMatrix& CApplication::ModelViewInverse()
{
	return mModelViewInverse;
}

CTexture* CApplication::Texture()
{
	return &mTexture;
}

void CApplication::Start()
{
	//3Dモデルファイルの読み込み
	mModelX.Load(MODEL_FILE);
	//敵モデルの読み込み
	mKnight.Load(MODEL_KNIGHT);
	mKnight.SeparateAnimationSet(0, 10, 80, "walk");		//1:移動
	mKnight.SeparateAnimationSet(0, 1530, 1830, "idle1");	//2:待機
	mKnight.SeparateAnimationSet(0, 10, 80, "walk");		//3:ダミー
	mKnight.SeparateAnimationSet(0, 10, 80, "walk");		//4:ダミー
	mKnight.SeparateAnimationSet(0, 10, 80, "walk");		//5:ダミー
	mKnight.SeparateAnimationSet(0, 10, 80, "walk");		//6:ダミー
	mKnight.SeparateAnimationSet(0, 440, 520, "attack1");	//7:攻撃1
	mKnight.SeparateAnimationSet(0, 520, 615, "attack2");	//8:攻撃2
	mKnight.SeparateAnimationSet(0, 10, 80, "walk");		//9:ダミー
	mKnight.SeparateAnimationSet(0, 10, 80, "walk");		//10:ダミー
	mKnight.SeparateAnimationSet(0, 1160, 1260, "death1");	//11:ダウン

	//キャラクターにモデルを設定
	mXPlayer.Init(&mModelX);
	mFont.Load("FontG.png", 1, 4096 / 64);
	//敵の初期設定
	mXEnemy.Init(&mKnight);
	//敵の配置
	mXEnemy.Position(CVector(7.0f, 0.0f, 0.0f));
	mXEnemy.ChangeAnimation(2, true, 200);
	mpPaladin = new CPaladin();
	mpPaladin->Position(CVector(-1.0f, 0.0f, 5.0f));
	//カメラ䛾設定
	mActionCamera.Set(5.0f, -15.0f, 180.0f);
}

void CApplication::Update()
{
	//if (mCharacter.IsAnimationFinished() == true)
	//{
	//	//アニメーションを切り替える
	//	if (mCharacter.AnimationIndex() < mModelX.AnimationSet().size())
	//	{
	//		mCharacter.ChangeAnimation(mCharacter.AnimationIndex() + 1, false, 60);
	//	}
	//}
	//キャラクタークラスの更新
	mXPlayer.Update();
	//敵クラスの更新
	mXEnemy.Update();
	mpPaladin->Update();

	// カメラ設定
	mActionCamera.Position(mXPlayer.Position()+ CVector(0.0f, 2.0f, 0.0f));

	mActionCamera.Update();
	mActionCamera.Render();
	//モデルビュー行列の取得
	glGetFloatv(GL_MODELVIEW_MATRIX, mModelViewInverse.M());
	//逆行列の取得
	mModelViewInverse = mModelViewInverse.Transpose();
	mModelViewInverse.M(0, 3, 0);
	mModelViewInverse.M(1, 3, 0);
	mModelViewInverse.M(2, 3, 0);

	//衝突処理
	CCollisionManager::Instance()->Collision();
	//プレイヤー描画
	mXPlayer.Render();
	//コライダの描画
	CCollisionManager::Instance()->Render();
	//敵描画
	mXEnemy.Render();
	mpPaladin->Render();

	//2D描画開始
	CCamera::Start(0, 800, 0, 600);

	mFont.Draw(20, 20, 10, 12, "3D PROGRAMMING");

	//2D描画終了
	CCamera::End();
}

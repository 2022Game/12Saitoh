#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "CModelX.h"
#include "glut.h"
#include "CMaterial.h"

/*
IsDelimiter(c)
cが\t \r \n スペースなどの空白文字
または、;　" などの文字であれば
区切り文字としてtrueを返す
*/
bool CModelX::IsDelimiter(char c)
{
	//isspace(c)
	//cが空白文字ばら0以外を返す
	if (isspace(c) != 0)
		return true;
	/*
	turchr(文字列,文字)
	文字列に文字が含まれていれば、
	見つかった文字へポインタを返す
	見つからなかったらNULLを返す
	*/
	if (strchr(",;\"", c) != NULL)
		return true;
	//区切り文字ではない
	return false;
}

/*
FinedFrame(フレーム名)
フレーム名に該当するフレームのアドレスを返す
*/
CModelXFrame* CModelX::FinedFrame(char* name)
{
	//イテレータの作成
	std:: vector<CModelXFrame*>::iterator itr;
	//先頭から最後まで繰り返す
	for (itr = mFrame.begin(); itr != mFrame.end(); itr++)
	{
		//名前が一致したか？
		if (strcmp(name, (*itr)->mpName) == 0)
		{
			//一致したらそのアドレスを返す
			return *itr;
		}
	}
	//一致するフレームが無い場合はnullptrを返す
	return nullptr;
}

CModelX::CModelX()
	:mpPointer(nullptr)
{
	//mTokenを初期化
	memset(mToken, 0, sizeof(mToken));
}

CModelX::~CModelX()
{
	if (mFrame.size() > 0)
	{
		delete mFrame[0];
	}
	for (size_t i = 0; i < mAnimationSet.size(); i++)
	{
		delete mAnimationSet[i];
	}
}

/*
GetToken
文字列データから、単語を1つ取得する
*/
char* CModelX::GetToken()
{
	char* p = mpPointer;
	char* q = mToken;
	//タブ(\t)空白()改行(\r)(\n),;"の区切り文字以外になるまで読み飛ばす
	while (*p != '\0' && IsDelimiter(*p))p++;
	if (*p == '{' || *p == '}')
		*q++ = *p++;
	else
	{
		while (*p != '\0' && !IsDelimiter(*p) && *p != '}')
			*q++ = *p++;
	}

	*q = '\0';    //mTokenの最後に\0を代入
	mpPointer = p; //次の読み込むポイントを更新する

	//もしTokenが//の場合は、コメントなので改行まで読み飛ばす
	/*
	strcmp(文字列1,文字列2)
	文字列１と文字列2が等しい場合、0を返します。
	文字列1と文字列2が等しくない場合、0以外を返します。
	*/
	if (!strcmp("//", mToken))
	{
		//改行まで読み飛ばす
		while (*p != '\0' && !strchr("\r\n", *p))p++;
		//読み込み位置の更新
		mpPointer = p;
		//単語を取得する(再帰呼び出し)
		return GetToken();
	}
	return mToken;
}

char* CModelX::Token()
{
	return mToken;
}

bool CModelX::EOT()
{
	if (*mpPointer == '\0')
	{
		return true;
	}
	else
	{
		return false;
	}
}

/*
SkipNode
ノードを読み飛ばす
*/
void CModelX::SlipNode()
{
	//文字が終わったら終了
	while (*mpPointer != '\0')
	{
		GetToken(); //次の単語取得
		//{が見つかったらループ終了
		if (strchr(mToken, '{')) break;
	}
	int count = 1;
	//文字が終わるか、カウントが0になったら終了
	while (*mpPointer != '\0' && count > 0)
	{
		GetToken();  //次の単語取得
		//{を見つけるとカウントアップ
		if (strchr(mToken, '{')) count++;
		//}を見つけるとカウントダウン
		else if (strchr(mToken, '}')) count--;
	}
}

void CModelX::Load(char* file)
{
	//ファイルサイズを取得する
	FILE* fp;

	fp = fopen(file, "rb"); //ファイルをオープンする
	//エラーチェック
	if (fp == NULL)
	{
		printf("fopen error:%s\n", file);
		return;
	}
	printf("%s\n", file);
	//ファイルの最後へ移動
	fseek(fp, 0L, SEEK_END);
	//ファイルサイズの取得
	int size = ftell(fp);
	//ファイルサイズ+1バイト文の領域を確保
	char* buf = mpPointer = new char[size + 1];
	//ファイルから3Dモデルのデータを読み込む
	//ファイルの先頭へ移動
	fseek(fp, 0L, SEEK_SET);
	//確保した領域にファイルサイズ文データを読み込む
	fread(buf, size, 1, fp);
	//最後に\0を設定する(文字列の終端)
	buf[size] = '\0';
	//文字列の最後まで繰り返し
	while (*mpPointer != '\0')
	{
		GetToken(); //単語の取得
		//単語がFrameの場合
		if (strcmp(mToken, "Frame") == 0)
		{
			//フレームを生成する
			new CModelXFrame(this);
		}
		//単語がAnimationSetの場合
		else if (strcmp(mToken, "AnimationSet") == 0)
		{
			new CAnimationSet(this);
		}
	}
	fclose(fp); //ファイルをクローズする

	SAFE_DELETE_ARRA(buf); //確保した領域を解放する
}

/*
Render
すべてのフレームの描画処理を呼び出す
*/
void CModelX::Render()
{
	for (size_t i = 0; i < mFrame.size(); i++)
	{
		mFrame[i]->Render();
	}
}

/*
CModelXFrame
model:CModelXインスタンスへのポインタ
プレームを作成する
読み込み中にFrameが見つかれば、フレームを作成し、
子フレームに追加する
*/
CModelXFrame::CModelXFrame(CModelX* model)
	: mpMesh(nullptr)
	, mpName(nullptr)
	, mIndex(0)
{
	//現在のフレーム配列に要素数を取得し設定する
	mIndex = model->mFrame.size();
	//CModelXフレーム配列に追加する
	model->mFrame.push_back(this);
	//変換行列を単位行列にする
	mTransformMatrix.Identity();
	//次の単語(フレーム名の予定)を取得する
	model->GetToken();  //Frae name
	//フレーム名分エリアを確保する
	mpName = new char[strlen(model->mToken) + 1];
	//フレーム名コピーする
	strcpy(mpName, model->mToken);
	//次の単語({の予定)を取得する
	model->GetToken();//{
	//文字がなくなったら終わり
	while (*model->mpPointer != '\0')
	{
		//次の単語取得
		model->GetToken(); //Frame
	    //}の場合は終了
		if (strchr(model->mToken, '}'))break;
		//新たなフレームの場合は、子フレームに追加
		if (strcmp(model->mToken,"Frame") == 0)
		{
			//フレームを生成し、子フレームに追加
			mChild.push_back(new CModelXFrame(model));
		}
		else if (strcmp(model->mToken, "FrameTransformMatrix") == 0)
		{
			model->GetToken();  //{
			for (int i = 0; i < mTransformMatrix.Size(); i++)
			{
				//atof->文字列のデータをfloat型に変換する
				mTransformMatrix.M()[i] = atof(model->GetToken());
			}
			model->GetToken();
		}
		else if (strcmp(model->mToken, "Mesh") == 0)
		{
		mpMesh = new CMesh;
		mpMesh->Init(model);
		}
		else
		{
		//上記以外の要素は読み飛ばす
		model->SlipNode();
		}
	}
	//デバッグバージョンのみ有効
#ifdef _DEBUG
	//printf("%s\n", mpName);
	//for (int i = 0; i < 4; i++)
	//{
	//	for (int j = 0; j < 4; j++)
	//	{
	//		printf("%f	", mTransformMatrix.M(i, j));
	//	}
	//	printf("\n");
	//}
#endif
}

CModelXFrame::~CModelXFrame()
{
	//子フレームを全て解放する
	std::vector<CModelXFrame*>::iterator itr;
	for (itr = mChild.begin(); itr != mChild.end(); itr++) {
		delete* itr;
	}
	//名前のエリアを解放する
	SAFE_DELETE_ARRA(mpName);

	if (mpMesh != nullptr)
		delete mpMesh;
}

int CModelXFrame::Index()
{
	return mIndex;
}

/*
Render
メッシュが存在すれば描画する
*/
void CModelXFrame::Render()
{
	if (mpMesh != nullptr)
		mpMesh->Render();
}

//コンストラクタ
CMesh::CMesh()
	: mFaceNum(0)
	, mVertexNum(0)
	, mNormalNum(0)
	, mMaterialNum(0)
	, mMaterialIndexNum(0)
	, mpMaterialIndex(nullptr)
	, mpVertexIndex(nullptr)
	, mpVertex(nullptr)
	, mpNormal(nullptr)
{

}

//デストラクタ
CMesh::~CMesh()
{
	SAFE_DELETE_ARRA(mpVertex);
	SAFE_DELETE_ARRA(mpVertexIndex);
	SAFE_DELETE_ARRA(mpNormal);
	SAFE_DELETE_ARRA(mpMaterialIndex);
	//スキンウェイトの削除
	for (size_t i = 0; i < mSkinWeights.size(); i++)
	{
		delete mSkinWeights[i];
	}
}

/*
Init
Meshのデータを読み込む
*/
void CMesh::Init(CModelX* model)
{
	model->GetToken();	//{ or 名前
	if (!strchr(model->mToken, '{'))
	{
		//名前がの場合、次が{
		model->GetToken();	// {
	}
	//頂点数の取得
	mVertexNum = atoi(model->GetToken());
	//頂点数分エリア確保
	mpVertex = new CVector[mVertexNum];
	//頂点数分データを取り込む
	for (int i = 0; i < mVertexNum; i++)
	{
		mpVertex[i].X(atof(model->GetToken()));
		mpVertex[i].Y(atof(model->GetToken()));
		mpVertex[i].Z(atof(model->GetToken()));
	}
	//面数の読み込み
	mFaceNum = atoi(model->GetToken());
	//頂点数は1面に3頂点
	mpVertexIndex = new int[mFaceNum * 3];
	for (int i = 0; i < mFaceNum * 3; i += 3)
	{
		model->GetToken(); //頂点数読み飛ばし
		mpVertexIndex[i] = atoi(model->GetToken());
		mpVertexIndex[i + 1] = atoi(model->GetToken());
		mpVertexIndex[i + 2] = atoi(model->GetToken());
	}

	//単語がある間繰り返し
	while (!model->EOT())
	{
		model->GetToken(); //MeshNormals
		// }かっこの場合は終了
		if (strchr(model->Token(), '}'))
			break;
		if (strcmp(model->Token(), "MeshNormals") == 0)
		{
			model->GetToken(); // {
			//法線データを取得
			mNormalNum = atoi(model->GetToken());
			//法線データを配列に取り込む
			CVector* pNormal = new CVector[mNormalNum];
			for (int i = 0; i < mNormalNum; i++)
			{
				pNormal[i].X(atof(model->GetToken()));
				pNormal[i].Y(atof(model->GetToken()));
				pNormal[i].Z(atof(model->GetToken()));
			}
			//法線数 = 面積 ×　3
			mNormalNum = atoi(model->GetToken()) * 3; //FaceNum
			int ni;
			//頂点毎に法線データを設定する
			mpNormal = new CVector[mNormalNum];
			for (int i = 0; i < mNormalNum; i += 3)
			{
				model->GetToken(); //3
				ni = atoi(model->GetToken());
				mpNormal[i] = pNormal[ni];

				ni = atoi(model->GetToken());
				mpNormal[i + 1] = pNormal[ni];

				ni = atoi(model->GetToken());
				mpNormal[i + 2] = pNormal[ni];

			}
			delete[] pNormal;
			model->GetToken(); // }
		} //End of MeshNormal

		//MeshMaterialListのとき
		else if (strcmp(model->Token(), "MeshMaterialList") == 0)
		{
			model->GetToken();  // {
			//Materialの数
			mMaterialNum = atoi(model->GetToken());
			//FaceNum
			mMaterialIndexNum = atoi(model->GetToken());
			//マテリアルインデックスの作成
			mpMaterialIndex = new int[mMaterialIndexNum];
			for (int i = 0; i < mMaterialIndexNum; i++)
			{
				mpMaterialIndex[i] = atoi(model->GetToken());
			}
			//マテリアルデータの作成
			for (int i = 0; i < mMaterialNum; i++)
			{
				model->GetToken();  //Material
				if (strcmp(model->Token(), "Material") == 0)
				{
					mMaterial.push_back(new CMaterial(model));
				}
			}
			model->GetToken();  // } End of MeshMaterialList
		}// End of MaterialList
		//SkinWeghtsのとき
		else if (strcmp(model->Token(), "SkinWeights") == 0)
		{
			//CSkinWeghtsクラスのインスタンスを作成し、配列に追加
			mSkinWeights.push_back(new CSkinWeights(model));
		}
		else
		{
			//以外のノードは読み飛ばし
			model->SlipNode();
		}
	}

	//デバッグバージョンの追加
#ifdef _DEBUG
	////頂点データの出力
	//printf("VertexNum:%d\n", mVertexNum);
	//for (int i = 0; i < mVertexNum; i++)
	//{
	//printf("%f	%f	%f	\n", mpVertex[i].X(), mpVertex[i].Y(), mpVertex[i].Z());
	//}
	////面のデータの出力
	//printf("FaceNum:%d\n", mFaceNum);
	//for (int i = 0; i < mFaceNum * 3; i += 3)
	//{
	//	printf("%d	%d	%d	\n",
	//		mpVertexIndex[i], mpVertexIndex[i + 1], mpVertexIndex[i + 2]);
	//}
	////法線データの出力
	//printf("NormalNum:%d\n", mNormalNum);
	//for (int i = 0; i < mNormalNum; i++)
	//{
	//	printf("%f	%f	%f	\n",
	//		mpNormal[i].X(), mpNormal[i].Y(), mpNormal[i].Z());
	//}
#endif
}

/*
Render
画面に描画する
*/
void CMesh::Render()
{
	/*頂点データ,法線データの配列を裕子にする*/
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	/*頂点データ,法線データの場所を指定する*/
	glVertexPointer(3, GL_FLOAT, 0, mpVertex);
	glNormalPointer(GL_FLOAT, 0, mpNormal);

	/*頂点インデックスの場所を指定して図形を描画する*/
	for (int i = 0; i < mFaceNum; i++)
	{
		//マテリアルを適用する
		mMaterial[mpMaterialIndex[i]]->Enabled();
		/*頂点のインデックスの場所を指定して図形を描画する*/
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (mpVertexIndex + i * 3));
	}

	/*頂点データ,法線データの配列を無効にする*/
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
}

/*
CSkinWeights
スキンウェイトの読み込み
*/
CSkinWeights::CSkinWeights(CModelX* model)
	: mpFrameName(0)
	, mFrameIndex(0)
	, mIndexNum(0)
	, mpIndex(nullptr)
	, mpWeight(nullptr)
{
	model->GetToken();	// {
	model->GetToken();	// Framename
	//フレーム名エリア確保、設定
	mpFrameName = new char[strlen(model->Token()) + 1];
	strcpy(mpFrameName, model->Token());

	//頂点番号数取得
	mIndexNum = atoi(model->GetToken());
	//頂点番号が0を超える
	if (mIndexNum > 0)
	{
		//頂点番号と頂点ウェイトのエリア確保
		mpIndex = new int[mIndexNum];

		mpWeight = new float[mIndexNum];
		//頂点番号取得
		for (int i = 0; i < mIndexNum; i++)
			mpIndex[i] = atoi(model->GetToken());
		//頂点ウェイト取得
		for (int i = 0; i < mIndexNum; i++)
			mpWeight[i] = atof(model->GetToken());
	}
	//オフセット行列取得
	for (int i = 0; i < 16; i++)
	{
		mOffset.M()[i] = atof(model->GetToken());
	}
	model->GetToken();	// }
#ifdef _DEBUG
	////SkinWeghts　フレーム名の出力
	//printf("SKinWeights %s\n",mpFrameName);
	////頂点番号　頂点ウェイトの出力
	//for (int i = 0; i < mIndexNum; i++)
	//{
	//	printf("%d  %f\n",mpIndex[i],mpWeight[i]);
	//}
	////オフセット行列の出力
	//for (int i = 0; i < 4; i++)
	//{
	//	for (int j = 0; j < 4; j++)
	//	{
	//		printf("%f	", mOffset.M(i,j));
	//	}
	//	printf("\n");
	//}
#endif
}

CSkinWeights::~CSkinWeights()
{
	SAFE_DELETE_ARRA(mpFrameName);
	SAFE_DELETE_ARRA(mpIndex);
	SAFE_DELETE_ARRA(mpWeight);
}

/*
CAnimationSet
*/
CAnimationSet::CAnimationSet(CModelX* model)
	: mpName(nullptr)
{
	model->mAnimationSet.push_back(this);
	model->GetToken();	//Animation Name
	//アニメーションセット名を退避
	mpName = new char[strlen(model->Token()) + 1];
	strcpy(mpName, model->Token());
	model->GetToken();	// {
	while (!model->EOT())
	{
		model->GetToken();	// } or Animation
		if (strchr(model->Token(), '}'))
			break;
		if (strcmp(model->Token(), "Animation") == 0)
		{
			//Animation要素の読み込み
			mAnimation.push_back(new CAnimation(model));
		}
	}
#ifdef _DEBUG
	//printf("AnimationSet:%s\n", mpName);
#endif
}

CAnimationSet::~CAnimationSet()
{
	//アニメーション要素の削除
	for (size_t i = 0; i < mAnimation.size(); i++)
	{
		delete mAnimation[i];
	}
}

/*
CAnimation
アニメーションクラス
*/
CAnimation::CAnimation(CModelX* model)
	: mpFrameName(nullptr)
	, mFrameIndex(0)
{
	model->GetToken(); // { or Animation Name
	if (strchr(model->Token(), '{'))
	{
		model->GetToken(); // {
	}
	else
	{
		model->GetToken(); // {
		model->GetToken(); // {
	}
	model->GetToken(); //FrameName
	mpFrameName = new char[strlen(model->Token()) + 1];
	strcpy(mpFrameName, model->Token());
	mFrameIndex = 
		model->FinedFrame(model->Token())->Index();
	model->GetToken();	// }
	while (!model->EOT())
	{
		model->GetToken();	// { or AnimationKey
		if (strchr(model->Token(), '}'))
			break;
		if (strcmp(model->Token(), "AnimationKey") == 0)
		{
			model->SlipNode();
		}
	}
#ifdef _DEBUG
	printf("Animation::%s\n", mpFrameName);
#endif
}

CAnimation::~CAnimation()
{
	SAFE_DELETE_ARRA(mpFrameName);
}
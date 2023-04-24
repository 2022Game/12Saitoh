#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "CModelX.h"
#include "glut.h"

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
	printf("%s\n", mpName);
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			printf("%f	", mTransformMatrix.M(i, j));
		}
		printf("\n");
	}
#endif
}

CModelXFrame::~CModelXFrame()
{
	if (mpMesh != nullptr)
		delete mpMesh;
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

	model->GetToken(); //MeshNormals
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

	//デバッグバージョンの追加
#ifdef _DEBUG
	//頂点データの出力
	printf("VertexNum:%d\n", mVertexNum);
	for (int i = 0; i < mVertexNum; i++)
	{
	printf("%f	%f	%f	\n", mpVertex[i].X(), mpVertex[i].Y(), mpVertex[i].Z());
	}
	//面のデータの出力
	printf("FaceNum:%d\n", mFaceNum);
	for (int i = 0; i < mFaceNum * 3; i += 3)
	{
		printf("%d	%d	%d	\n",
			mpVertexIndex[i], mpVertexIndex[i + 1], mpVertexIndex[i + 2]);
	}
	//法線データの出力
	printf("NormalNum:%d\n", mNormalNum);
	for (int i = 0; i < mNormalNum; i++)
	{
		printf("%f	%f	%f	\n",
			mpNormal[i].X(), mpNormal[i].Y(), mpNormal[i].Z());
	}
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

	/*頂点のインデックスの場所を指定して図形を描画する*/
	glDrawElements(GL_TRIANGLES, 3 * mFaceNum,GL_UNSIGNED_INT, mpVertexIndex);

	/*頂点データ,法線データの配列を無効にする*/
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
}
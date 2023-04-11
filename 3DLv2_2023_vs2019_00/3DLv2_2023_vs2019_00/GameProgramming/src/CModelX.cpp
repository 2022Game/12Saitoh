#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "CModelX.h"
#include "glut.h"

CModelX::CModelX()
	:mpPointer(nullptr)
{
	//mTokenを初期化
	memset(mToken, 0, sizeof(mToken));
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
			printf("%s" , mToken); //Frame名出力
			GetToken(); //Frame名を取得
			printf("%s\n", mToken); //Frame名出力
		}
		else if (strcmp(mToken, "AnimationSet") == 0)
		{
			printf("%s", mToken); //Animation名出力
			GetToken(); //animation名取得
			printf("%s\n", mToken); //animation名出力)
		}
	}
	fclose(fp); //ファイルをクローズする

	SAFE_DELETE_ARRA(buf); //確保した領域を解放する
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
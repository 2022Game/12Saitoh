#ifndef MATH_H
#define MATH_H

#define _USE_MATH_DEFINES
#include <math.h>

#include "CVector.h"

namespace Math
{
	//線形補間の結果を返す
	inline float Lerp(float start, float end, float t)
	{
		return start + t * (end - start);
	}

	//角度をディグリーからラジアンに変換
	inline float DegreeToRadian(float d)
	{
		return d * (float)(M_PI / 180.0f);
	}

	//角度をラジアンからディグリーに変換
	inline float RadianToDegree(float r)
	{
		return r * (float)(180.0f / M_PI);
	}

	//視野範囲内かどうかを返す
	//p1      視野の始点
	//p2      調べる対象の座標
	//forward 視野の正面方向ベクトル
	//angle   視野角度（-angle～angleまでの範囲）
	//length  視野距離
	bool InFieldOfView(const CVector& p1, const CVector& p2,
		const CVector& forward, const float angle, const float length);
}

#endif
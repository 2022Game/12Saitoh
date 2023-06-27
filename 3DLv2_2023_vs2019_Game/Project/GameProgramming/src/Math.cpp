#include "Math.h"

//視野範囲内かどうかを返す
bool Math::InFieldOfView(const CVector& p1, const CVector& p2,
	const CVector& forward, const float angle, const float length)
{
	//視野の始点から調べる対象までのベクトルを求める
	CVector p12 = p2 - p1;

	//正面方向ベクトルと調べる対象までのベクトルの
	//内積を取り、視野角度内か判定する
	float dot = forward.Normalize().Dot(p12.Normalize());
	if (dot < cosf(DegreeToRadian(angle))) return false;

	//調べる対象までの距離を求めて、視野距離内か判定する
	float distance = p12.Length();
	if (distance > length) return false;

	//視野角度、視野距離、
	//どちらも範囲内だったので、trueを返す
	return true;
}
#ifndef CWALL_H
#define CWALL_H

#include "CCharacter3.h"
#include "CCollider.h"
#include "CColliderMesh.h"
#include "CColliderTriangle.h"
#include "CColliderLine.h"

class CWall : public CCharacter3 {
private:
	////コライダ
	CColliderMesh mColliderMesh;

public:
	//衝突処理
	//Collision(コライダ１、コライダ２）
	void Collision(CCollider* m, CCollider* o);
	void Collision();
	//コンストラクタ
	//CWall(モデル,位置,回転,拡縮）
	CWall(CModel* model, const CVector& position,
		const CVector& rotation, const CVector& scale);
	//更新処理
	void Update();
};
#endif
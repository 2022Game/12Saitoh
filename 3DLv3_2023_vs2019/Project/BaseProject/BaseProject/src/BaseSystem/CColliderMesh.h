#ifndef CCOLLIDERMESH_H
#define CCOLLIDERMESH_H
#include <list>
#include "CModel.h"
#include "CCollider.h"
/*
モデルデータから三角コライダの生成
*/
class CColliderMesh : public CCollider
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="owner">コライダーの持ち主</param>
	/// <param name="layer">衝突判定用レイヤー</param>
	/// <param name="model">モデルデータ</param>
	/// <param name="multiplier">肉質</param>
	/// <param name="isKinematic">trueならば、衝突時に押し戻しの影響を受けない</param>
	/// <param name="weight">コライダーの重量</param>
	CColliderMesh(CObjectBase* owner, ELayer layer, CModel* model,
		int  multiplier = 0, bool isKinematic = false, float weight = 1.0f);
	// デストラクタ
	~CColliderMesh();

	void Set(CModel* model);
	void Get(std::list<STVertex>* tris) const;

	// コライダー描画
	void Render();

private:
	//三角コライダの配列作成
	std::list<STVertex> mVertices;
};
#endif

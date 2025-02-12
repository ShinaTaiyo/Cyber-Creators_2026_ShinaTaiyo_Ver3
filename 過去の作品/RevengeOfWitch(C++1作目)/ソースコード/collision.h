//====================================================
//
//９月１３日：当たり判定用のクラスを作る[collision.h]
//Author:ShinaTaiyo
//
//====================================================

#ifndef _COLLISION_H_  
#define _COLLISION_H_

//======================
//インクルード
//======================
#include "main.h"
//==========================================

//==========================================

//==========================================
//当たり判定クラス
//==========================================
class CCollision
{
public:
	CCollision();                   //コンストラクタ
	~CCollision();                  //デストラクタ

	//正方形の当たり判定
	static bool CollisionSquare(D3DXVECTOR3 MyPos, D3DXVECTOR3 MyVtxMax, D3DXVECTOR3 MyVtxMin,
		D3DXVECTOR3 ComparisonPos, D3DXVECTOR3 ComparisonVtxMax, D3DXVECTOR3 ComparisonVtxMin);

	//円の当たり判定
	static bool CollisionBall(D3DXVECTOR3 MySenterPos, D3DXVECTOR3 MySize,
		D3DXVECTOR3 ComparisonSenterPos, D3DXVECTOR3 ComparisonSize);

	//斜めの当たり判定XY
	static bool RectAngleCollisionXY(D3DXVECTOR3 MyPos, D3DXVECTOR3 MyVtxMax, D3DXVECTOR3 MyVtxMin, D3DXVECTOR3 Rot,
		D3DXVECTOR3 ComparisonPos, D3DXVECTOR3 ComparisonVtxMax, D3DXVECTOR3 ComparisonVtxMin, D3DXVECTOR3 ComparisonRot);

private:
};
#endif

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
#include "objectX.h"
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


	static bool CollisionSquarePoint(const D3DXVECTOR3 & Point,CObjectX* pObjX);//点がAABBと当たったかどうかを判定

	//円の当たり判定
	static bool CollisionBall(D3DXVECTOR3 MySenterPos, D3DXVECTOR3 MySize,
		D3DXVECTOR3 ComparisonSenterPos, D3DXVECTOR3 ComparisonSize);

	//斜めの当たり判定XY
	static bool RectAngleCollisionXY(D3DXVECTOR3 MyPos, D3DXVECTOR3 MyVtxMax, D3DXVECTOR3 MyVtxMin, D3DXVECTOR3 Rot,
		D3DXVECTOR3 ComparisonPos, D3DXVECTOR3 ComparisonVtxMax, D3DXVECTOR3 ComparisonVtxMin);

	//斜めの当たり判定XZ
	static bool RectAngleCollisionXZ(CObjectX* pMyObj, CObjectX* pComObj);

	//点がAABBの中に入っているかどうか
	static bool IsPointInsideAABB(const D3DXVECTOR3 & Point,CObjectX * pComObjX);

	//==============================================
	//正方形の押し出し判定
	//==============================================
	//基本
	static bool ExtrusionCollisionSquare(D3DXVECTOR3 & MyPos,bool& bCollisionX,bool & bCollisionY,bool & bCollisionZ,
		const D3DXVECTOR3 MyMove,const D3DXVECTOR3 MyPosOld,const D3DXVECTOR3 MyVtxMax,const D3DXVECTOR3 MyVtxMin,
		const D3DXVECTOR3 ComPos,const D3DXVECTOR3 ComVtxMax,const D3DXVECTOR3 ComVtxMin,
		const bool bCollisionXOld,const bool bCollisionYOld,const bool bCollisionZOld,bool & bIsLanding);

	//正方形の新しいX押し出し判定
	static void ResolveExtrusionCollisionSquare(CObjectX* pObjX, CObjectX* pComObjX); //正方形の当たり判定を解決する
	static void ExtrusionCollisionSquarePushOutFirstDecide(CObjectX* pObjX, CObjectX* pComObjX);//正方形の押し出し判定のそれぞれの軸の押し出しの優先度を決める

	static bool NewExtrusionCollisionSquareX(CObjectX* pObjX, CObjectX* pComObjX);
	static bool NewExtrusionCollisionSquareY(CObjectX* pObjX, CObjectX* pComObjX);
	static bool NewExtrusionCollisionSquareZ(CObjectX* pObjX, CObjectX* pComObjX);

	//X押し出し
	static bool ExtrusionCollisionSquareX(D3DXVECTOR3& MyPos,const D3DXVECTOR3 MyMove,const D3DXVECTOR3 MyPosOld,const D3DXVECTOR3 MyVtxMax, const D3DXVECTOR3 MyVtxMin, 
		const D3DXVECTOR3 ComPos, const D3DXVECTOR3 ComVtxMax, const D3DXVECTOR3 ComVtxMin,const bool bCollisionX);

	//Y押し出し
	static bool ExtrusionCollisionSquareY(D3DXVECTOR3& MyPos, const D3DXVECTOR3 MyMove, const D3DXVECTOR3 MyPosOld, const D3DXVECTOR3 MyVtxMax, const D3DXVECTOR3 MyVtxMin,
		const D3DXVECTOR3 ComPos, const D3DXVECTOR3 ComVtxMax, const D3DXVECTOR3 ComVtxMin,const bool bCollisionY, bool& bIsLanding);

	//Z押し出し
	static bool ExtrusionCollisionSquareZ(D3DXVECTOR3& MyPos, const D3DXVECTOR3 MyMove, const D3DXVECTOR3 MyPosOld, const D3DXVECTOR3 MyVtxMax, const D3DXVECTOR3 MyVtxMin,
		const D3DXVECTOR3 ComPos, const D3DXVECTOR3 ComVtxMax, const D3DXVECTOR3 ComVtxMin,const bool bCollisionZ);

	//=============================================================================================================================================================================

	//================================================
	//レイ
	//================================================
	
	//レイとaabbの当たり判定と判定箇所を求める
	static bool RayIntersectsAABBCollisionPos(const D3DXVECTOR3& origin, const D3DXVECTOR3& direction, const D3DXVECTOR3& min, const D3DXVECTOR3& max,
		D3DXVECTOR3 & CollisionPos);
	//==============================================================================================================================================================================


private:
};
#endif

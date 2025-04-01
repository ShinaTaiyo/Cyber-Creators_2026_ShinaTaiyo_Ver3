//====================================================================================================================
//
//９月１３日：当たり判定用のクラスを作る[collision.cpp]
//Author:ShinaTaiyo
//
//====================================================================================================================

//================================================================
//インクルード
//================================================================
#include "collision.h"
#include "manager.h"
#include "calculation.h"
#include "particle.h"
#include "debugtext.h"
#include "camera.h"
//====================================================================================================================

//================================================================
//コンストラクタ
//================================================================
CCollision::CCollision()
{

}
//====================================================================================================================

//================================================================
//デストラクタ
//================================================================
CCollision::~CCollision()
{

}
//====================================================================================================================

//================================================================
//正方形の当たり判定を取る
//================================================================
bool CCollision::CollisionSquare(D3DXVECTOR3 MyPos, D3DXVECTOR3 MyVtxMax, D3DXVECTOR3 MyVtxMin,
	D3DXVECTOR3 ComparisonPos, D3DXVECTOR3 ComparisonVtxMax, D3DXVECTOR3 ComparisonVtxMin)
{
	if (MyPos.x + MyVtxMin.x <= ComparisonPos.x + ComparisonVtxMax.x &&
		MyPos.x + MyVtxMax.x >= ComparisonPos.x + ComparisonVtxMin.x &&
		MyPos.y + MyVtxMin.y <= ComparisonPos.y + ComparisonVtxMax.y &&
		MyPos.y + MyVtxMax.y >= ComparisonPos.y + ComparisonVtxMin.y &&
		MyPos.z + MyVtxMin.z <= ComparisonPos.z + ComparisonVtxMax.z &&
		MyPos.z + MyVtxMax.z >= ComparisonPos.z + ComparisonVtxMin.z)
	{//正方形の範囲内に入っていたら
		return true;
	}
	return false;
}

//================================================================
//点と正方形の当たり判定を取る
//================================================================
bool CCollision::CollisionSquarePoint(const D3DXVECTOR3 & Point,CObjectX* pObjX)
{
	if (Point.x <= pObjX->GetPosInfo().GetPos().x + pObjX->GetSizeInfo().GetVtxMax().x &&
		Point.x >= pObjX->GetPosInfo().GetPos().x + pObjX->GetSizeInfo().GetVtxMin().x &&
		Point.y <= pObjX->GetPosInfo().GetPos().y + pObjX->GetSizeInfo().GetVtxMax().y &&
		Point.y >= pObjX->GetPosInfo().GetPos().y + pObjX->GetSizeInfo().GetVtxMin().y &&
		Point.z <= pObjX->GetPosInfo().GetPos().z + pObjX->GetSizeInfo().GetVtxMax().z &&
		Point.z >= pObjX->GetPosInfo().GetPos().z + pObjX->GetSizeInfo().GetVtxMin().z)
	{//点が正方形の範囲内に入っていたら
		return true;
	}
	return false;
}
//====================================================================================================================

//================================================================
//正方形の当たり判定を取る
//================================================================
bool CCollision::CollisionBall(D3DXVECTOR3 MySenterPos, D3DXVECTOR3 MySize,
	D3DXVECTOR3 ComparisonSenterPos, D3DXVECTOR3 ComparisonSize)
{
	float fResultMySize = 0.0f;        //自分のサイズ
	float fResultComprisonSize = 0.0f; //相手のサイズ

	//====================================================================
	//X軸のサイズとY軸のサイズを比べ、大きいほうを半径として定義する
	//====================================================================
	if (MySize.x > MySize.y)
	{//サイズがXの方が大きい
		fResultMySize = MySize.x;
	}
	else
	{//サイズがYの方が大きい
		fResultMySize = MySize.y;
	}

	if (ComparisonSize.x > ComparisonSize.y)
	{//サイズがXの方が大きい
		fResultComprisonSize = ComparisonSize.x;
	}
	else
	{//サイズがYの方が大きい
		fResultComprisonSize = ComparisonSize.y;
	}
	//===============================================================================================================================-

	float fCheckLength = fResultMySize / 2 + fResultComprisonSize / 2;//お互いのモデルの中心点からの距離を足す

	//お互いのモデルの中心点からの距離を求める
	float fLength = CCalculation::CalculationLength(MySenterPos, ComparisonSenterPos);

	if (fLength < fCheckLength)
	{//二つの半径の和より距離が小さければtrueを返す
		return true;
	}

	return false;
}
//====================================================================================================================

//================================================================
//斜めの当たり判定XY
//================================================================
bool CCollision::RectAngleCollisionXY(D3DXVECTOR3 MyPos, D3DXVECTOR3 MyVtxMax, D3DXVECTOR3 MyVtxMin, D3DXVECTOR3 Rot,
	D3DXVECTOR3 ComparisonPos, D3DXVECTOR3 ComparisonVtxMax, D3DXVECTOR3 ComparisonVtxMin)
{
	D3DXVECTOR2 FourVtxPos[4] = {};   //４頂点の位置
	D3DXVECTOR2 FourVtxRotPos[4] = {};//回転した４頂点の位置
	D3DXVECTOR2 FourComparisonVtxPos[4] = {};    //比較用４頂点
	bool bCollision = true;

	//=====================================
	//比較対象の四角形の４頂点
	//=====================================
	FourComparisonVtxPos[0].x = ComparisonPos.x + ComparisonVtxMin.x;
	FourComparisonVtxPos[0].y = ComparisonPos.y + ComparisonVtxMax.y;

	FourComparisonVtxPos[1].x = ComparisonPos.x + ComparisonVtxMax.x;
	FourComparisonVtxPos[1].y = ComparisonPos.y + ComparisonVtxMax.y;

	FourComparisonVtxPos[2].x = ComparisonPos.x + ComparisonVtxMin.x;
	FourComparisonVtxPos[2].y = ComparisonPos.y + ComparisonVtxMin.y;

	FourComparisonVtxPos[3].x = ComparisonPos.x + ComparisonVtxMax.x;
	FourComparisonVtxPos[3].y = ComparisonPos.y + ComparisonVtxMin.y;

	//=============================================================================================================================================================================

	//=====================================
	//攻撃の四角形の４頂点
	//=====================================

	//左上頂点
	FourVtxPos[0] = D3DXVECTOR2(MyPos.x + MyVtxMin.x, MyPos.y + MyVtxMax.y);

	//右上頂点
	FourVtxPos[1] = D3DXVECTOR2(MyPos.x + MyVtxMax.x, MyPos.y + MyVtxMax.y);

	//左下頂点
	FourVtxPos[2] = D3DXVECTOR2(MyPos.x + MyVtxMin.x, MyPos.y + MyVtxMin.y);

	//右下頂点
	FourVtxPos[3] = D3DXVECTOR2(MyPos.x + MyVtxMax.x, MyPos.y + MyVtxMin.y);

	//=============================================================================================================================================================================

	//==================================
	//中心点と４頂点の距離を求める
	//==================================
	float fLength[4] = {};
	for (int nCnVtx = 0; nCnVtx < 4; nCnVtx++)
	{
		fLength[nCnVtx] = sqrtf(powf((FourVtxPos[nCnVtx].x - MyPos.x), 2) + powf((FourVtxPos[nCnVtx].y - MyPos.y), 2));
	}
	//=============================================================================================================================================================================

	//==================================
	//中心点から４頂点の角度を求める
	//==================================
	float fVaim[4] = {};
	for (int nCntVtx = 0; nCntVtx < 4; nCntVtx++)
	{
		fVaim[nCntVtx] = atan2f(FourVtxPos[nCntVtx].x - MyPos.x, FourVtxPos[nCntVtx].y - MyPos.y);
	}
	//=============================================================================================================================================================================

	//==================================
	//回転頂点の位置を求める
	//==================================
	for (int nCntVtx = 0; nCntVtx < 4; nCntVtx++)
	{
		//判定を合わせるため、Rotに-を付ける
		FourVtxRotPos[nCntVtx] = D3DXVECTOR2(sinf(-Rot.z + fVaim[nCntVtx]) * fLength[nCntVtx] + MyPos.x, cosf(-Rot.z + fVaim[nCntVtx]) * fLength[nCntVtx] + MyPos.y);
	}
	//=============================================================================================================================================================================

	//=====================================================
	//二つの四角形のそれぞれの辺の方向を求める
	//=====================================================
	float fNormalizeAim[4] = {};
	fNormalizeAim[0] = sqrtf(powf((FourVtxRotPos[0].x - FourVtxRotPos[1].x), 2) + powf((FourVtxRotPos[0].y - FourVtxRotPos[1].y), 2));
	fNormalizeAim[1] = sqrtf(powf((FourVtxRotPos[1].x - FourVtxRotPos[3].x), 2) + powf((FourVtxRotPos[1].y - FourVtxRotPos[3].y), 2));
	fNormalizeAim[2] = sqrtf(powf((FourComparisonVtxPos[0].x - FourComparisonVtxPos[1].x), 2) + powf((FourComparisonVtxPos[0].y - FourComparisonVtxPos[1].y), 2));
	fNormalizeAim[3] = sqrtf(powf((FourComparisonVtxPos[1].x - FourComparisonVtxPos[3].x), 2) + powf((FourComparisonVtxPos[1].y - FourComparisonVtxPos[3].y), 2));

	//正規化ベクトルを求める
	D3DXVECTOR2 sideDirs[] =
	{
		(FourVtxRotPos[0] - FourVtxRotPos[1]) / fNormalizeAim[0], (FourVtxRotPos[1] - FourVtxRotPos[3]) / fNormalizeAim[1],
		(FourComparisonVtxPos[0] - FourComparisonVtxPos[1]) / fNormalizeAim[2], (FourComparisonVtxPos[1] - FourComparisonVtxPos[3]) / fNormalizeAim[3]
	};
	//==============================================================================================================================================================================


	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		float minA = D3DXVec2Dot(&FourVtxRotPos[0], &sideDirs[nCnt]);
		float maxA = minA;
		float minB = D3DXVec2Dot(&FourComparisonVtxPos[0], &sideDirs[nCnt]);
		float maxB = minB;

		for (int i = 1; i < 4; i++)
		{
			float projA = D3DXVec2Dot(&FourVtxRotPos[i], &sideDirs[nCnt]);

			if (projA < minA) { minA = projA; }
			else if (projA > maxA) { maxA = projA; }

			float projB = D3DXVec2Dot(&FourComparisonVtxPos[i], &sideDirs[nCnt]);

			if (projB < minB) { minB = projB; }
			else if (projB > maxB) { maxB = projB; }
		}

		if (maxB < minA || maxA < minB)
		{
			//return false;
			bCollision = false;
		}
	}

	//==================================================================
	//Zの範囲内にいるかどうか
	//==================================================================
	if (ComparisonPos.z + ComparisonVtxMax.z >= MyPos.z + MyVtxMin.z &&
		ComparisonPos.z + ComparisonVtxMin.z <= MyPos.z + MyVtxMax.z && bCollision == true)
	{
		return true;
	}
	else
	{
	   return false;
	}
	//======================================================================================================
}

//================================================================
//斜めの当たり判定XZ
//================================================================
bool CCollision::RectAngleCollisionXZ(CObjectX* pMyObj, CObjectX* pComObj)
{
	D3DXVECTOR2 FourVtxPos[4] = {};                                    //４頂点の位置
	D3DXVECTOR2 FourVtxRotPos[4] = {};                                 //回転した４頂点の位置
	D3DXVECTOR2 FourComparisonVtxPos[4] = {};                          //比較用４頂点
	bool bCollision = true;                                            //当たったかどうか
	D3DXVECTOR3 MyPos = pMyObj->GetPosInfo().GetPos();                 //自分の位置
	D3DXVECTOR3 MyVtxMin = pMyObj->GetSizeInfo().GetVtxMin();          //自分の最小頂点
	D3DXVECTOR3 MyVtxMax = pMyObj->GetSizeInfo().GetVtxMax();          //自分の最大頂点
	D3DXVECTOR3 MyRot = pMyObj->GetRotInfo().GetRot();                 //自分の向き
	D3DXVECTOR3 ComparisonPos = pComObj->GetPosInfo().GetPos();        //相手の位置
	D3DXVECTOR3 ComparisonVtxMin = pComObj->GetSizeInfo().GetVtxMin(); //相手の最小頂点
	D3DXVECTOR3 ComparisonVtxMax = pComObj->GetSizeInfo().GetVtxMax(); //相手の最大頂点

	//=====================================
	//比較対象の四角形の４頂点
	//=====================================

	//左上[0]
	FourComparisonVtxPos[0].x = ComparisonPos.x + ComparisonVtxMin.x;
	FourComparisonVtxPos[0].y = ComparisonPos.z + ComparisonVtxMax.z;

	//右上[1]
	FourComparisonVtxPos[1].x = ComparisonPos.x + ComparisonVtxMax.x;
	FourComparisonVtxPos[1].y = ComparisonPos.z + ComparisonVtxMax.z;

	//左下[2]
	FourComparisonVtxPos[2].x = ComparisonPos.x + ComparisonVtxMin.x;
	FourComparisonVtxPos[2].y = ComparisonPos.z + ComparisonVtxMin.z;

	//右下[3]
	FourComparisonVtxPos[3].x = ComparisonPos.x + ComparisonVtxMax.x;
	FourComparisonVtxPos[3].y = ComparisonPos.z + ComparisonVtxMin.z;

	//=============================================================================================================================================================================

	//=====================================
	//自分自身の４頂点
	//=====================================

	//左上[0]
	FourVtxPos[0] = D3DXVECTOR2(MyPos.x + MyVtxMin.x, MyPos.z + MyVtxMax.z);

	//右上[1]
	FourVtxPos[1] = D3DXVECTOR2(MyPos.x + MyVtxMax.x, MyPos.z + MyVtxMax.z);

	//左下[2]
	FourVtxPos[2] = D3DXVECTOR2(MyPos.x + MyVtxMin.x, MyPos.z + MyVtxMin.z);

	//右下[3]
	FourVtxPos[3] = D3DXVECTOR2(MyPos.x + MyVtxMax.x, MyPos.z + MyVtxMin.z);

	//=============================================================================================================================================================================

	//==================================
	//自分自身の中心点と４頂点の距離を求める
	//==================================
	float fLength[4] = {};//中心点と４頂点の距離を初期化
	for (int nCnVtx = 0; nCnVtx < 4; nCnVtx++)
	{//※この処理はXZ平面なので、４頂点のYはZと定義する
		fLength[nCnVtx] = sqrtf(powf((FourVtxPos[nCnVtx].x - MyPos.x), 2) + powf((FourVtxPos[nCnVtx].y - MyPos.z), 2));
	}
	//=============================================================================================================================================================================

	//==================================
	//自分自身の中心点から４頂点の角度を求める
	//==================================
	float fVaim[4] = {};
	for (int nCntVtx = 0; nCntVtx < 4; nCntVtx++)
	{//※この処理はXZ平面なので、４頂点のYはZと定義する
		fVaim[nCntVtx] = atan2f(FourVtxPos[nCntVtx].x - MyPos.x, FourVtxPos[nCntVtx].y - MyPos.z);
	}
	//=============================================================================================================================================================================

	//==================================
	//回転頂点の位置を求める
	//==================================
	for (int nCntVtx = 0; nCntVtx < 4; nCntVtx++)
	{//4頂点の中心点からの角度を自分の向きを基準にし、それを踏まえた４頂点の位置を求める
		FourVtxRotPos[nCntVtx] = D3DXVECTOR2(sinf(MyRot.y + fVaim[nCntVtx]) * fLength[nCntVtx] + MyPos.x, cosf(MyRot.y + fVaim[nCntVtx]) * fLength[nCntVtx] + MyPos.z);
	}
	//=============================================================================================================================================================================

	//=====================================================
	//二つの四角形のそれぞれの辺の方向を求める
	//=====================================================
	float fNormalizeAim[4] = {};//自分と相手のXZ平面のポリゴンの横方向と縦方向のベクトルを正規化する
	fNormalizeAim[0] = sqrtf(powf((FourVtxRotPos[0].x - FourVtxRotPos[1].x), 2) + powf((FourVtxRotPos[0].y - FourVtxRotPos[1].y), 2));//自分の横方向
	fNormalizeAim[1] = sqrtf(powf((FourVtxRotPos[1].x - FourVtxRotPos[3].x), 2) + powf((FourVtxRotPos[1].y - FourVtxRotPos[3].y), 2));//自分の縦方向
	fNormalizeAim[2] = sqrtf(powf((FourComparisonVtxPos[0].x - FourComparisonVtxPos[1].x), 2) + powf((FourComparisonVtxPos[0].y - FourComparisonVtxPos[1].y), 2));//相手の横方向
	fNormalizeAim[3] = sqrtf(powf((FourComparisonVtxPos[1].x - FourComparisonVtxPos[3].x), 2) + powf((FourComparisonVtxPos[1].y - FourComparisonVtxPos[3].y), 2));//相手の縦方向

	//正規化ベクトルを求める
	D3DXVECTOR2 sideDirs[] =
	{
		(FourVtxRotPos[0] - FourVtxRotPos[1]) / fNormalizeAim[0], (FourVtxRotPos[1] - FourVtxRotPos[3]) / fNormalizeAim[1],
		(FourComparisonVtxPos[0] - FourComparisonVtxPos[1]) / fNormalizeAim[2], (FourComparisonVtxPos[1] - FourComparisonVtxPos[3]) / fNormalizeAim[3]
	};
	//==============================================================================================================================================================================


	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		float minA = D3DXVec2Dot(&FourVtxRotPos[0], &sideDirs[nCnt]);
		float maxA = minA;
		float minB = D3DXVec2Dot(&FourComparisonVtxPos[0], &sideDirs[nCnt]);
		float maxB = minB;

		for (int i = 1; i < 4; i++)
		{
			float projA = D3DXVec2Dot(&FourVtxRotPos[i], &sideDirs[nCnt]);

			if (projA < minA) { minA = projA; }
			else if (projA > maxA) { maxA = projA; }

			float projB = D3DXVec2Dot(&FourComparisonVtxPos[i], &sideDirs[nCnt]);

			if (projB < minB) { minB = projB; }
			else if (projB > maxB) { maxB = projB; }
		}

		if (maxB < minA || maxA < minB)
		{
			bCollision = false;
		}
	}

	//==================================================================
	//Zの範囲内にいるかどうか
	//==================================================================
	if (ComparisonPos.y + ComparisonVtxMax.y >= MyPos.y + MyVtxMin.y &&
		ComparisonPos.y + ComparisonVtxMin.y <= MyPos.y + MyVtxMax.y && bCollision == true)
	{
		return true;
	}
	else
	{
		return false;
	}
	//======================================================================================================
}
//====================================================================================================================

//================================================================
//点がAABBの中に入っているかどうか
//================================================================
bool CCollision::IsPointInsideAABB(const D3DXVECTOR3& Point, CObjectX* pComObjX)
{
	if (Point.x >= pComObjX->GetPosInfo().GetPos().x + pComObjX->GetSizeInfo().GetVtxMin().x &&
		Point.x <= pComObjX->GetPosInfo().GetPos().x + pComObjX->GetSizeInfo().GetVtxMax().x &&
		Point.y >= pComObjX->GetPosInfo().GetPos().y + pComObjX->GetSizeInfo().GetVtxMin().y &&
		Point.y <= pComObjX->GetPosInfo().GetPos().y + pComObjX->GetSizeInfo().GetVtxMax().y &&
		Point.z >= pComObjX->GetPosInfo().GetPos().z + pComObjX->GetSizeInfo().GetVtxMin().z &&
		Point.z <= pComObjX->GetPosInfo().GetPos().z + pComObjX->GetSizeInfo().GetVtxMax().z)
	{//点がAABBの中に入っていたら
		return true;
	}
	return false;
}
//====================================================================================================================

//==============================================================
//正方形の全ての押し出し判定を解決する関数（ExtrusionCollisionSquarePushOutFirstDecide関数で1f後の位置がどの軸に当たるかを決め、衝突判定を行う軸の順番を決めて、引っ掛かりを阻止する)
//==============================================================
void CCollision::ResolveExtrusionCollisionSquare(CObjectX* pObjX, CObjectX* pComObjX)
{
	if (pObjX->GetCollisionInfo().GetState().GetLandingOld())
	{
		CCollision::NewExtrusionCollisionSquareY(pObjX, pComObjX);
		if (pObjX->GetCollisionInfo().GetSquareInfo().GetPushOutFlag(AXIS::X) == true)
		{
			CCollision::NewExtrusionCollisionSquareX(pObjX, pComObjX);
			CCollision::NewExtrusionCollisionSquareZ(pObjX, pComObjX);
		}
		else
		{
			CCollision::NewExtrusionCollisionSquareZ(pObjX, pComObjX);
			CCollision::NewExtrusionCollisionSquareX(pObjX, pComObjX);
		}
	}
	else if (pObjX->GetCollisionInfo().GetState().GetWallingOld())
	{
		if (pObjX->GetCollisionInfo().GetSquareInfo().GetPushOutFlag(AXIS::X) == true)
		{
			CCollision::NewExtrusionCollisionSquareX(pObjX, pComObjX);
			CCollision::NewExtrusionCollisionSquareZ(pObjX, pComObjX);
		}
		else
		{
			CCollision::NewExtrusionCollisionSquareZ(pObjX, pComObjX);
			CCollision::NewExtrusionCollisionSquareX(pObjX, pComObjX);
		}
		CCollision::NewExtrusionCollisionSquareY(pObjX, pComObjX);
	}
	else
	{
		CCollision::NewExtrusionCollisionSquareX(pObjX, pComObjX);
		CCollision::NewExtrusionCollisionSquareY(pObjX, pComObjX);
		CCollision::NewExtrusionCollisionSquareZ(pObjX, pComObjX);

	}
}
//====================================================================================================================

//==============================================================
//正方形の押し出し判定のそれぞれの軸の優先度を決める
//==============================================================
void CCollision::ExtrusionCollisionSquarePushOutFirstDecide(CObjectX* pObjX, CObjectX* pComObjX)
{
	//*変数

	//自分
	const D3DXVECTOR3& MyPos = pObjX->GetPosInfo().GetPos();              //自分の位置
	const D3DXVECTOR3& MyPosOld = pObjX->GetPosInfo().GetPosOld();        //自分の1f前の位置
	const D3DXVECTOR3& MyMove = pObjX->GetMoveInfo().GetMove();           //自分の移動量
	const D3DXVECTOR3& MyPosFuture = MyPos + MyMove;                      //自分の1f後の位置
	const D3DXVECTOR3& MyVtxMax = pObjX->GetSizeInfo().GetVtxMax();       //自分の最大頂点
	const D3DXVECTOR3& MyVtxMin = pObjX->GetSizeInfo().GetVtxMin();       //自分の最小頂点

	//相手
	const D3DXVECTOR3& ComPos = pComObjX->GetPosInfo().GetPos();          //相手の位置
	const D3DXVECTOR3& ComVtxMax = pComObjX->GetSizeInfo().GetVtxMax();   //相手の最大頂点
	const D3DXVECTOR3& ComVtxMin = pComObjX->GetSizeInfo().GetVtxMin();   //相手の最小頂点
	CObjectX::CollisionInfo& CollisionInfo = pObjX->GetCollisionInfo();   //当たり判定情報を取得する
	CObjectX::CollisionInfo::State& CollisionState = CollisionInfo.GetState();//当たり判定状態を取得する
	//*処理開始
	
	//上
	if (MyPosFuture.x + MyVtxMax.x > ComPos.x + ComVtxMin.x
		&& MyPosFuture.x + MyVtxMin.x < ComPos.x + ComVtxMax.x
		&& MyPosFuture.y + MyVtxMin.y < ComPos.y + ComVtxMax.y
		&& MyPosOld.y + MyVtxMin.y >= ComPos.y + ComVtxMax.y
		&& MyPosFuture.z + MyVtxMax.z > ComPos.z + ComVtxMin.z
		&& MyPosFuture.z + MyVtxMin.z < ComPos.z + ComVtxMax.z)
	{//1f後にオブジェクトに乗るなら
		CollisionInfo.GetSquareInfo().SetPushOutFirstFlag(AXIS::Y, true);
		CollisionInfo.GetSquareInfo().SetPushOutFirstFlag(AXIS::X, false);
		CollisionInfo.GetSquareInfo().SetPushOutFirstFlag(AXIS::Z, false);
	}
	//下
	else if (MyPosFuture.x + MyVtxMax.x > ComPos.x + ComVtxMin.x
		&& MyPosFuture.x + MyVtxMin.x < ComPos.x + ComVtxMax.x
		&& MyPosFuture.y + MyVtxMax.y > ComPos.y + ComVtxMin.y
		&& MyPosOld.y + MyVtxMax.y <= ComPos.y + ComVtxMin.y
		&& MyPosFuture.z + MyVtxMax.z > ComPos.z + ComVtxMin.z
		&& MyPosFuture.z + MyVtxMin.z < ComPos.z + ComVtxMax.z)
	{//1f後にオブジェクトの下に当たるなら
		CollisionInfo.GetSquareInfo().SetPushOutFirstFlag(AXIS::Y, true);
		CollisionInfo.GetSquareInfo().SetPushOutFirstFlag(AXIS::X, false);
		CollisionInfo.GetSquareInfo().SetPushOutFirstFlag(AXIS::Z, false);
	}
	if (MyPosFuture.x + MyVtxMax.x > ComPos.x + ComVtxMin.x
		&& MyPosOld.x + MyVtxMax.x <= ComPos.x + ComVtxMin.x
		&& MyPosFuture.y + MyVtxMax.y > ComPos.y + ComVtxMin.y
		&& MyPosFuture.y + MyVtxMin.y < ComPos.y + ComVtxMax.y
		&& MyPosFuture.z + MyVtxMax.z > ComPos.z + ComVtxMin.z
		&& MyPosFuture.z + MyVtxMin.z < ComPos.z + ComVtxMax.z)
	{//1f後にオブジェクトのX軸の右側に当たるなら
		CollisionInfo.GetSquareInfo().SetPushOutFirstFlag(AXIS::X, true);
		CollisionInfo.GetSquareInfo().SetPushOutFirstFlag(AXIS::Z, false);
	}
	else if (MyPosFuture.x + MyVtxMin.x < ComPos.x + ComVtxMax.x
		&& MyPosOld.x + MyVtxMin.x >= ComPos.x + ComVtxMax.x
		&& MyPosFuture.y + MyVtxMax.y > ComPos.y + ComVtxMin.y
		&& MyPosFuture.y + MyVtxMin.y < ComPos.y + ComVtxMax.y
		&& MyPosFuture.z + MyVtxMax.z > ComPos.z + ComVtxMin.z
		&& MyPosFuture.z + MyVtxMin.z < ComPos.z + ComVtxMax.z)
	{//1f後にオブジェクトのX軸の左側に当たるなら
		CollisionInfo.GetSquareInfo().SetPushOutFirstFlag(AXIS::X, true);
		CollisionInfo.GetSquareInfo().SetPushOutFirstFlag(AXIS::Z, false);
	}
	//前
	if (MyPosFuture.z + MyVtxMax.z > ComPos.z + ComVtxMin.z
		&& MyPosOld.z + MyVtxMax.z <= ComPos.z + ComVtxMin.z
		&& MyPosFuture.y + MyVtxMax.y > ComPos.y + ComVtxMin.y
		&& MyPosFuture.y + MyVtxMin.y < ComPos.y + ComVtxMax.y
		&& MyPosFuture.x + MyVtxMax.x > ComPos.x + ComVtxMin.x
		&& MyPosFuture.x + MyVtxMin.x < ComPos.x + ComVtxMax.x)
	{//1f後にオブジェクトの手前側に当たるなら
		CollisionInfo.GetSquareInfo().SetPushOutFirstFlag(AXIS::Z, true);
		CollisionInfo.GetSquareInfo().SetPushOutFirstFlag(AXIS::X, false);
	}
	//奥
	else if (MyPosFuture.z + MyVtxMin.z < ComPos.z + ComVtxMax.z
		&& MyPosOld.z + MyVtxMin.z >= ComPos.z + ComVtxMax.z
		&& MyPosFuture.y + MyVtxMax.y > ComPos.y + ComVtxMin.y
		&& MyPosFuture.y + MyVtxMin.y < ComPos.y + ComVtxMax.y
		&& MyPosFuture.x + MyVtxMax.x > ComPos.x + ComVtxMin.x
		&& MyPosFuture.x + MyVtxMin.x < ComPos.x + ComVtxMax.x)
	{//1f後にオブジェクトの奥側に当たるなら
		CollisionInfo.GetSquareInfo().SetPushOutFirstFlag(AXIS::Z, true);
		CollisionInfo.GetSquareInfo().SetPushOutFirstFlag(AXIS::X, false);
	}
}
//====================================================================================================================

//================================================================
//新しい正方形の押し出し判定X
//================================================================
bool CCollision::NewExtrusionCollisionSquareX(CObjectX* pObjX, CObjectX* pComObjX)
{
	const D3DXVECTOR3& MyPos = pObjX->GetPosInfo().GetPos();            //自分の位置
	const D3DXVECTOR3& MyMove = pObjX->GetMoveInfo().GetMove();         //自分の移動量
	const D3DXVECTOR3& MyPosOld = pObjX->GetPosInfo().GetPosOld();      //自分の1f前の位置
	const D3DXVECTOR3& MyVtxMax = pObjX->GetSizeInfo().GetVtxMax();     //自分の最大頂点
	const D3DXVECTOR3& MyVtxMin = pObjX->GetSizeInfo().GetVtxMin();     //自分の最小頂点

	const D3DXVECTOR3& ComPos = pComObjX->GetPosInfo().GetPos();        //相手の位置
	const D3DXVECTOR3& ComVtxMax = pComObjX->GetSizeInfo().GetVtxMax(); //相手の最大頂点
	const D3DXVECTOR3& ComVtxMin = pComObjX->GetSizeInfo().GetVtxMin(); //相手の最小頂点

	CObjectX::CollisionInfo& CollisionInfo = pObjX->GetCollisionInfo();        //当たり判定情報を取得する
	CObjectX::CollisionInfo::State& CollisionState = CollisionInfo.GetState(); //当たり判定状態を取得する

	if (MyPos.x + MyVtxMax.x > ComPos.x + ComVtxMin.x
		&& MyPosOld.x + MyVtxMax.x <= ComPos.x + ComVtxMin.x
		&& MyPos.y + MyVtxMax.y > ComPos.y + ComVtxMin.y
		&& MyPos.y + MyVtxMin.y < ComPos.y + ComVtxMax.y
		&& MyPos.z + MyVtxMax.z > ComPos.z + ComVtxMin.z
		&& MyPos.z + MyVtxMin.z < ComPos.z + ComVtxMax.z)
	{//対象の右端がブロックの左端に当たった時の処理
		pObjX->GetPosInfo().SetPos(D3DXVECTOR3(ComPos.x + ComVtxMin.x - MyVtxMax.x - 0.1f,
			pObjX->GetPosInfo().GetPos().y,
			pObjX->GetPosInfo().GetPos().z));
		CollisionState.SetWalling(true);
		return true;
	}
	else if (MyPos.x + MyVtxMin.x < ComPos.x + ComVtxMax.x
		&& MyPosOld.x + MyVtxMin.x >= ComPos.x + ComVtxMax.x
		&& MyPos.y + MyVtxMax.y > ComPos.y + ComVtxMin.y
		&& MyPos.y + MyVtxMin.y < ComPos.y + ComVtxMax.y
		&& MyPos.z + MyVtxMax.z > ComPos.z + ComVtxMin.z
		&& MyPos.z + MyVtxMin.z < ComPos.z + ComVtxMax.z)
	{//対象の左端がブロックの右端に当たった時の処理
		pObjX->GetPosInfo().SetPos(D3DXVECTOR3(ComPos.x + ComVtxMax.x - MyVtxMin.x + 0.1f,
			pObjX->GetPosInfo().GetPos().y,
			pObjX->GetPosInfo().GetPos().z));
		CollisionState.SetWalling(true);
		return true;
	}
	return false;
}
//====================================================================================================================

//================================================================
//新しい正方形の押し出し判定Y
//================================================================
bool CCollision::NewExtrusionCollisionSquareY(CObjectX* pObjX, CObjectX* pComObjX)
{
	const D3DXVECTOR3& MyPos = pObjX->GetPosInfo().GetPos();                   //自分の位置
	const D3DXVECTOR3& MyMove = pObjX->GetMoveInfo().GetMove();                //自分の移動量
	const D3DXVECTOR3& MyPosOld = pObjX->GetPosInfo().GetPosOld();             //自分の1f前の位置
	const D3DXVECTOR3& MyVtxMax = pObjX->GetSizeInfo().GetVtxMax();            //自分の最大頂点
	const D3DXVECTOR3& MyVtxMin = pObjX->GetSizeInfo().GetVtxMin();            //自分の最小頂点
																		       
	const D3DXVECTOR3& ComPos = pComObjX->GetPosInfo().GetPos();               //相手の位置
	const D3DXVECTOR3& ComVtxMax = pComObjX->GetSizeInfo().GetVtxMax();        //相手の最大頂点
	const D3DXVECTOR3& ComVtxMin = pComObjX->GetSizeInfo().GetVtxMin();        //相手の最小頂点
	CObjectX::CollisionInfo& CollisionInfo = pObjX->GetCollisionInfo();        //当たり判定情報を取得する
	CObjectX::CollisionInfo::State& CollisionState = CollisionInfo.GetState(); //当たり判定状態を取得する

	//上
	if (MyPos.x + MyVtxMax.x > ComPos.x + ComVtxMin.x
		&& MyPos.x + MyVtxMin.x < ComPos.x + ComVtxMax.x
		&& MyPos.y + MyVtxMin.y < ComPos.y + ComVtxMax.y
		&& MyPosOld.y + MyVtxMin.y - MyMove.y >= ComPos.y + ComVtxMax.y
		&& MyPos.z + MyVtxMax.z > ComPos.z + ComVtxMin.z
		&& MyPos.z + MyVtxMin.z < ComPos.z + ComVtxMax.z)
	{//対象の下端がモデルの上端に当たった時の処理
		float fPosY = fabsf(MyVtxMin.y);
		//MyPos.y = ComPos.y + ComVtxMax.y + fPosY + 0.1f;
		pObjX->GetPosInfo().SetPos(D3DXVECTOR3(pObjX->GetPosInfo().GetPos().x,
			ComPos.y + ComVtxMax.y + fPosY + 0.1f,
			pObjX->GetPosInfo().GetPos().z));
		CollisionState.SetLanding(true);
		return true;
	}
	//下
	else if (MyPos.x + MyVtxMax.x > ComPos.x + ComVtxMin.x
		&& MyPos.x + MyVtxMin.x < ComPos.x + ComVtxMax.x
		&& MyPos.y + MyVtxMax.y > ComPos.y + ComVtxMin.y
		&& MyPosOld.y + MyVtxMax.y <= ComPos.y + ComVtxMin.y
		&& MyPos.z + MyVtxMax.z > ComPos.z + ComVtxMin.z
		&& MyPos.z + MyVtxMin.z < ComPos.z + ComVtxMax.z)
	{//対象の上端がモデルの下端に当たった時の処理
		//MyPos.y = ComPos.y + ComVtxMin.y - MyVtxMax.y;
		pObjX->GetPosInfo().SetPos(D3DXVECTOR3(pObjX->GetPosInfo().GetPos().x,
			ComPos.y + ComVtxMin.y - MyVtxMax.y,
			pObjX->GetPosInfo().GetPos().z));
		return true;
	}

	return false;
}
//====================================================================================================================

//================================================================
//新しい正方形の押し出し判定Z
//================================================================
bool CCollision::NewExtrusionCollisionSquareZ(CObjectX* pObjX, CObjectX* pComObjX)
{
	const D3DXVECTOR3& MyPos = pObjX->GetPosInfo().GetPos();            //自分の位置
	const D3DXVECTOR3& MyMove = pObjX->GetMoveInfo().GetMove();         //自分の移動量
	const D3DXVECTOR3& MyPosOld = pObjX->GetPosInfo().GetPosOld();      //自分の1f前の位置
	const D3DXVECTOR3& MyVtxMax = pObjX->GetSizeInfo().GetVtxMax();     //自分の最大頂点
	const D3DXVECTOR3& MyVtxMin = pObjX->GetSizeInfo().GetVtxMin();     //自分の最小頂点

	const D3DXVECTOR3& ComPos = pComObjX->GetPosInfo().GetPos();        //相手の位置
	const D3DXVECTOR3& ComVtxMax = pComObjX->GetSizeInfo().GetVtxMax(); //相手の最大頂点
	const D3DXVECTOR3& ComVtxMin = pComObjX->GetSizeInfo().GetVtxMin(); //相手の最小頂点

	CObjectX::CollisionInfo& CollisionInfo = pObjX->GetCollisionInfo();        //当たり判定情報を取得する
	CObjectX::CollisionInfo::State& CollisionState = CollisionInfo.GetState(); //当たり判定状態を取得する

	if (MyPos.z + MyVtxMax.z > ComPos.z + ComVtxMin.z
		&& MyPosOld.z + MyVtxMax.z <= ComPos.z + ComVtxMin.z
		&& MyPos.y + MyVtxMax.y > ComPos.y + ComVtxMin.y
		&& MyPos.y + MyVtxMin.y < ComPos.y + ComVtxMax.y
		&& MyPos.x + MyVtxMax.x > ComPos.x + ComVtxMin.x
		&& MyPos.x + MyVtxMin.x < ComPos.x + ComVtxMax.x)
	{//対象の右端がブロックの左端に当たった時の処理
		pObjX->GetPosInfo().SetPos(D3DXVECTOR3(pObjX->GetPosInfo().GetPos().x,
			pObjX->GetPosInfo().GetPos().y,
			ComPos.z + ComVtxMin.z - MyVtxMax.z - 0.1f));
		CollisionState.SetWalling(true);
		return true;
	}
	else if (MyPos.z + MyVtxMin.z < ComPos.z + ComVtxMax.z
		&& MyPosOld.z + MyVtxMin.z >= ComPos.z + ComVtxMax.z
		&& MyPos.y + MyVtxMax.y > ComPos.y + ComVtxMin.y
		&& MyPos.y + MyVtxMin.y < ComPos.y + ComVtxMax.y
		&& MyPos.x + MyVtxMax.x > ComPos.x + ComVtxMin.x
		&& MyPos.x + MyVtxMin.x < ComPos.x + ComVtxMax.x)
	{//対象の左端がブロックの右端に当たった時の処理
		pObjX->GetPosInfo().SetPos(D3DXVECTOR3(pObjX->GetPosInfo().GetPos().x,
			pObjX->GetPosInfo().GetPos().y,
			ComPos.z + ComVtxMax.z - MyVtxMin.z + 0.1f));
		CollisionState.SetWalling(true);
		return true;
	}
	return false;
}
//====================================================================================================================

//================================================================
//レイとAABBの当たり判定、判定箇所も求める
//================================================================
bool CCollision::RayIntersectsAABBCollisionPos(const D3DXVECTOR3& origin,const D3DXVECTOR3& direction, const D3DXVECTOR3& min, const D3DXVECTOR3& max,
	D3DXVECTOR3& CollisionPos)
{
	D3DXVECTOR3 CopyDirection = direction;

	if (origin.x >= min.x && origin.x <= max.x &&
		origin.y >= min.y && origin.y <= max.y &&
		origin.z >= min.z && origin.z <= max.z)
	{//レイの起点がボックスの内側にある場合
		return false;
	}

	if (CopyDirection.x == 0.0f)
	{
		CopyDirection.x = 0.001f;//０除算を回避
	}
	if (CopyDirection.y == 0.0f)
	{
		CopyDirection.y = 0.001f;//０除算を回避
	}
	if (CopyDirection.z == 0.0f)
	{
		CopyDirection.z = 0.001f;//０除算を回避
	}

	//＊それぞれの最大頂点、最小頂点（軸の平面）との交差範囲を求め、一番近い面との距離を求める

	//X軸の面との交差範囲を求める
	float tmin = (min.x - origin.x) / CopyDirection.x;
	float tmax = (max.x - origin.x) / CopyDirection.x;

	float t = 0.0f;

	//tmaxよりもtminの方が距離が大きければ、最小最大の関係を崩さないために入れ替える
	if (tmin > tmax) std::swap(tmin, tmax);

	//Y軸方向の面との交差範囲を求める
	float tymin = (min.y - origin.y) / CopyDirection.y;
	float tymax = (max.y - origin.y) / CopyDirection.y;

	//tymaxよりもtyminの方が距離が大きければ、最小最大の関係を崩さないために入れ替える
	if (tymin > tymax) std::swap(tymin, tymax);

	//現在の交差範囲がY軸の交差範囲内に収まっていなかったら、レイは当たっていない（AABBは軸が平行であり本質的に交差範囲の最大最小は同じになる）
	if ((tmin > tymax) || (tymin > tmax)) return false;

	//Y軸方向の面がX軸方向の面よりも近かったら、または遠いかったら、交差範囲を更新する
	if (tymin > tmin) tmin = tymin;
	if (tymax < tmax) tmax = tymax;

	//Z軸方向の面との交差範囲を求める
	float tzmin = (min.z - origin.z) / CopyDirection.z;
	float tzmax = (max.z - origin.z) / CopyDirection.z;

	//tzmaxよりもtzminの方が距離が大きければ、最小最大の関係を崩さないために入れ替える
	if (tzmin > tzmax) std::swap(tzmin, tzmax);

	//現在の交差範囲がZ軸の交差範囲内に収まっていなかったら、レイは当たっていない
	if ((tmin > tzmax) || (tzmin > tmax)) return false;

	//Z軸方向の面がXY軸方向の面よりも近かったら、または遠いかったら、交差範囲を更新する
	if (tzmin > tmin) tmin = tzmin;
	if (tzmax < tmax) tmax = tzmax;

	//一番近い面を確定
	t = tmin;

	//レイの支点より後ろに正方形（オブジェクト）がある場合は、意味がないので無視する
	if (t < 0.0f) return false;

	//一番近い面との距離をかけて、レイの衝突位置を求める）
	CollisionPos = D3DXVECTOR3(origin.x + CopyDirection.x * t,
		origin.y + CopyDirection.y * t,
		origin.z + CopyDirection.z * t);
	return true;
}
//====================================================================================================================
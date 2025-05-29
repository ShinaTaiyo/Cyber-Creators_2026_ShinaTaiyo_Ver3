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

	//=====================================================
	//分離軸定理
	// ※分離軸定理とは・・・「２つの物体が衝突していない場合、
	//必ずそれらのいずれかの辺に垂直な軸（分離軸）が存在し、
	//投影した時にその軸上で重ならない」という理論。
	// つまり・・・
	//・全ての分離軸において、投影が重なっていたら→衝突している
	//・１つでも重ならない軸が合ったら→衝突していない（それが"分離軸"）
	//=====================================================
	for (int nCnt = 0; nCnt < 4; nCnt++) // 全4軸（自分と相手の各辺の垂直軸）に対してループ
	{
		// 自分のポリゴンの最初の頂点を現在の軸に正射影（内積）し、minAおよびmaxAを初期化
		float minA = D3DXVec2Dot(&FourVtxRotPos[0], &sideDirs[nCnt]);
		float maxA = minA;

		// 相手のポリゴンの最初の頂点も同様に軸に正射影し、minBおよびmaxBを初期化
		float minB = D3DXVec2Dot(&FourComparisonVtxPos[0], &sideDirs[nCnt]);
		float maxB = minB;

		// 自分の残りの3頂点を現在の軸に投影して、最小値と最大値を更新
		for (int i = 1; i < 4; i++)
		{
			float projA = D3DXVec2Dot(&FourVtxRotPos[i], &sideDirs[nCnt]);
			if (projA < minA)
			{// 投影軸の最小点A更新
				minA = projA; 
			}
			else if (projA > maxA) 
			{// 投影軸の最大点A更新
				maxA = projA;
			}

			// 相手の頂点も同様に投影し、最小・最大値を更新
			float projB = D3DXVec2Dot(&FourComparisonVtxPos[i], &sideDirs[nCnt]);
			if (projB < minB) 
			{// 投影軸の最小点B更新
				minB = projB;
			}
			else if (projB > maxB) 
			{// 投影軸の最大点B更新
				maxB = projB;
			}
		}

		//分離軸があるので当たっていない
		if (maxB < minA || maxA < minB)
		{
			return false;
		}
	}
	//==================================================================
	//Zの範囲内にいるかどうか
	//==================================================================
	if (ComparisonPos.y + ComparisonVtxMax.y >= MyPos.y + MyVtxMin.y &&
		ComparisonPos.y + ComparisonVtxMin.y <= MyPos.y + MyVtxMax.y)
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
//XZ方向のOBBの当たり判定
//================================================================
bool CCollision::RectOBB_XZ(CObjectX* pMyObj, CObjectX* pComObj)
{
	CObjectX::PosInfo& MyPosInfo = pMyObj->GetPosInfo();      //自分自身の位置情報
	CObjectX::RotInfo& MyRotInfo = pMyObj->GetRotInfo();      //自分自身の向き情報
	CObjectX::SizeInfo& MySizeInfo = pMyObj->GetSizeInfo();   //自分自身のサイズ情報

	CObjectX::PosInfo& ComPosInfo = pComObj->GetPosInfo();    //相手の位置情報
	CObjectX::RotInfo& ComRotInfo = pComObj->GetRotInfo();    //相手の向き情報
	CObjectX::SizeInfo& ComSizeInfo = pComObj->GetSizeInfo(); //相手のサイズ情報

	const D3DXVECTOR3& MyPos = MyPosInfo.GetPos();            //自分自身の位置
	const D3DXVECTOR3& MyRot = MyRotInfo.GetRot();            //自分自身の向き
	const D3DXVECTOR3& MyVtxMax = MySizeInfo.GetVtxMax();     //自分自身の最大頂点
	const D3DXVECTOR3& MyVtxMin = MySizeInfo.GetVtxMin();     //自分自身の最小頂点
	const D3DXVECTOR3& MySize = MySizeInfo.GetSize();         //自分自身のサイズ取得
	float fAdjustLength = MyVtxMax.z - MySize.z;              // 中心点を調整する距離   
	const D3DXVECTOR3 MySenterPos = MyPos + D3DXVECTOR3(sinf(MyRot.y) * fAdjustLength, 0.0f,cosf(fAdjustLength) * fAdjustLength);//Y軸を考慮しない自分自身の中心点 
	D3DXVECTOR3 MySenterPosXZ = D3DXVECTOR3(MySenterPos.x,0.0f,MySenterPos.z);     //Y軸を考慮しない自分自身の中心点
	D3DXVECTOR3 MyPosXZ = D3DXVECTOR3(MyPos.x, 0.0f, MyPos.z);                                              // Y軸を考慮しない自分自身の位置
	CParticle::Create(CParticle::TYPE::TYPE00_NORMAL, 100, 10.0f, 10.0f, MySenterPos, D3DXVECTOR3(0.0f, 5.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), true);


	const D3DXVECTOR3& ComPos = ComPosInfo.GetPos();             // 相手の位置
	const D3DXVECTOR3& ComRot = ComRotInfo.GetRot();             // 相手の向き情報
	const D3DXVECTOR3& ComVtxMax = ComSizeInfo.GetVtxMax();      // 相手の最大頂点
	const D3DXVECTOR3& ComVtxMin = ComSizeInfo.GetVtxMin();      // 相手の最小頂点
	const D3DXVECTOR3& ComSize = ComSizeInfo.GetSize();          // 相手のサイズ
	const D3DXVECTOR3 ComSenterPos = ComPosInfo.GetSenterPos(); // 相手の中心点
	D3DXVECTOR3 ComSenterPosXZ = D3DXVECTOR3(ComSenterPos.x, 0.0f, ComSenterPos.z);//Y軸を考慮しない相手の中心
	CParticle::Create(CParticle::TYPE::TYPE00_NORMAL, 100, 10.0f, 10.0f, ComSenterPos, D3DXVECTOR3(0.0f, 5.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), true);//デバッグ用パーティクル

	CDebugText* pDebugText = CManager::GetDebugText();        //デバッグ表示の取得

	D3DXVECTOR3 MyRightPosXZ = MySenterPosXZ + D3DXVECTOR3(sinf(MyRot.y + D3DX_PI * 0.5f) * (MySize.x / 2), 0.0f, cosf(MyRot.y + D3DX_PI * 0.5f) * (MySize.x / 2));//中心点から右端への位置を求める

	//デバッグ用のパーティクル（自分の右ベクトル）
	CParticle::Create(CParticle::TYPE::TYPE00_NORMAL, 100, 10.0f, 10.0f,D3DXVECTOR3(MyRightPosXZ.x,MyPos.y, MyRightPosXZ.z), D3DXVECTOR3(0.0f, 5.0f, 0.0f), D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), true);

	D3DXVECTOR3 ComPosXZ = D3DXVECTOR3(ComPos.x, 0.0f, ComPos.z);               //Y軸を考慮しない相手の位置

	D3DXVECTOR3 DirSenterXZ = ComSenterPosXZ - MySenterPosXZ;//Y軸を考慮しない中心点同士のベクトル
	float fLength = CCalculation::CalculationLength(MySenterPosXZ, ComSenterPosXZ);//Y軸を考慮しない中心点同士の距離


	//上記で求めた位置とのベクトルを求める：EA1
	D3DXVECTOR3 EA1_RightVec = MyRightPosXZ - MySenterPosXZ;       //ポリゴンA右ベクトル(EA1)
	D3DXVECTOR3 NormalizeEA1_RightVec = { 0.0f,0.0f,0.0f };  //ポリゴンA"正規化"右ベクトル
	D3DXVec3Normalize(&NormalizeEA1_RightVec, &EA1_RightVec);//正規化

	//自分の向きを考慮した前側の位置を求める（前方向０にするので、cosをZとする)
	D3DXVECTOR3 MyUpPosXZ = MySenterPosXZ + D3DXVECTOR3(sinf(MyRot.y) * (MySize.z / 2), 0.0f, cosf(MyRot.y) * (MySize.z / 2));
	//デバッグ用のパーティクル（自分の上ベクトル）
	CParticle::Create(CParticle::TYPE::TYPE00_NORMAL, 100, 10.0f, 10.0f, D3DXVECTOR3(MyUpPosXZ.x, MyPos.y, MyUpPosXZ.z), D3DXVECTOR3(0.0f, 5.0f, 0.0f), D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), true);

    //上記で求めた位置とのベクトルを求める：EA2
	D3DXVECTOR3 EA2_UpVec = MyUpPosXZ - MySenterPosXZ;   //ポリゴンA上ベクトル(EA2)
	D3DXVECTOR3 NormalizeEA2_UpVec = { 0.0f,0.0f,0.0f }; //ポリゴンA”正規化”上ベクトル
	D3DXVec3Normalize(&NormalizeEA2_UpVec, &EA2_UpVec);  //正規化

	//相手の向きを考慮した右側の位置を求める（前方向０にするので、cosをZとし、右側なので「+90度」)
	D3DXVECTOR3 ComRightPosXZ = ComSenterPosXZ + D3DXVECTOR3(sinf(ComRot.y + D3DX_PI * 0.5f) * (ComSize.x / 2), 0.0f, cosf(ComRot.y + D3DX_PI * 0.5f) * (ComSize.x / 2));
	//デバッグ用のパーティクル（相手の右ベクトル）
    CParticle::Create(CParticle::TYPE::TYPE00_NORMAL, 100, 10.0f, 10.0f, D3DXVECTOR3(ComRightPosXZ.x, ComPos.y, ComRightPosXZ.z), D3DXVECTOR3(0.0f, 5.0f, 0.0f), D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), true);

	//上記で求めた位置とのベクトルを求める：EB1
	D3DXVECTOR3 EB1_RightVec = ComRightPosXZ - ComSenterPosXZ;       //ポリゴンB右ベクトル(EB1)
	D3DXVECTOR3 NormalizeEB1_RightVec = { 0.0f,0.0f,0.0f };    //ポリゴンB"正規化"右ベクトル
	D3DXVec3Normalize(&NormalizeEB1_RightVec, &EB1_RightVec);  //正規化

	//相手の向きを考慮した前側の位置を求める（前方向０にするので、cosをZとする)
	D3DXVECTOR3 ComUpPosXZ = ComSenterPosXZ + D3DXVECTOR3(sinf(ComRot.y) * (ComSize.z / 2), 0.0f, cosf(ComRot.y) * (ComSize.z / 2));

	//デバッグ用のパーティクル（相手の上ベクトル）
	CParticle::Create(CParticle::TYPE::TYPE00_NORMAL, 100, 10.0f, 10.0f, D3DXVECTOR3(ComUpPosXZ.x, ComPos.y, ComUpPosXZ.z), D3DXVECTOR3(0.0f, 5.0f, 0.0f), D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), true);
	//上記で求めた位置とのベクトルを求める：EB2
	D3DXVECTOR3 EB2_UpVec = ComUpPosXZ - ComSenterPosXZ;    //ポリゴンB上ベクトル(EB2)
	D3DXVECTOR3 NormalizeEB2_UpVec = { 0.0f,0.0f,0.0f };    //ポリゴンB"正規化"上ベクトル
	D3DXVec3Normalize(&NormalizeEB2_UpVec, &EB2_UpVec);     //正規化
	//全て長さです
	float rA[4] = {};    //分離軸に対する半径A
	float rB[4] = {};    //分離軸に対する半径B
	float L[4] = {};     //分離軸に対する距離

	//==========================================
	//分離軸１：ポリゴンA右ベクトル(A.e1)
	//==========================================
	rA[0] = CCalculation::CalculationLength(MySenterPosXZ, MyRightPosXZ);//分離軸上での中心点からの長さがrA半径になる
	rB[0] = fabsf(D3DXVec3Dot(&EB1_RightVec, &NormalizeEA1_RightVec)) + fabsf(D3DXVec3Dot(&EB2_UpVec, &NormalizeEA1_RightVec));//分離軸EA1にポリゴンBの２軸を投影した長さ
	L[0] = fabsf(D3DXVec3Dot(&DirSenterXZ, &NormalizeEA1_RightVec));//中心点間の距離を分離軸EA1に投影

	pDebugText->PrintDebugText("rA[0] = %f\n", rA[0]);
	pDebugText->PrintDebugText("rB[0] = %f\n", rB[0]);
	pDebugText->PrintDebugText("L[0] = %f\n", L[0]);

	if (L[0] > (rA[0] + rB[0]))
	{//投影した中心点間の距離よりも半投影線分の合計が大きければ分離軸が存在しないので、当たっている
		return false;
	}

	//==========================================
	//分離軸２：ポリゴンA上ベクトル(A.e2)
	//==========================================
	rA[1] = CCalculation::CalculationLength(MySenterPosXZ, MyUpPosXZ);
	rB[1] = fabsf(D3DXVec3Dot(&EB1_RightVec, &NormalizeEA2_UpVec)) + fabsf(D3DXVec3Dot(&EB2_UpVec, &NormalizeEA2_UpVec));//分離軸EA2にポリゴンBの２軸を投影した長さ
	L[1] = fabsf(D3DXVec3Dot(&DirSenterXZ, &NormalizeEA2_UpVec));//中心点間の距離を分離軸EA2に投影

	pDebugText->PrintDebugText("rA[1] = %f\n", rA[1]);
	pDebugText->PrintDebugText("rB[1] = %f\n", rB[1]);
	pDebugText->PrintDebugText("L[1] = %f\n", L[1]);

	if (L[1] > (rA[1] + rB[1]))
	{//投影した中心点間の距離よりも半投影線分の合計が大きければ分離軸が存在しないので、当たっている
		return false;
	}

	//==========================================
	//分離軸３：ポリゴンB右ベクトル(B.e1)
	//==========================================
	rA[2] = fabsf(D3DXVec3Dot(&EA1_RightVec, &NormalizeEB1_RightVec)) + fabsf(D3DXVec3Dot(&EA2_UpVec, &NormalizeEB1_RightVec));//分離軸EB1にポリゴンAの２軸を投影した長さ
	rB[2] = CCalculation::CalculationLength(ComSenterPosXZ, ComRightPosXZ);
	L[2] = fabsf(D3DXVec3Dot(&DirSenterXZ, &NormalizeEB1_RightVec));//中心点間の距離を分離軸EB1に投影
	pDebugText->PrintDebugText("rA[2] = %f\n", rA[2]);
	pDebugText->PrintDebugText("rB[2] = %f\n", rB[2]);
	pDebugText->PrintDebugText("L[2] = %f\n", L[2]);
	if (L[2] > (rA[2] + rB[2]))
	{//投影した中心点間の距離よりも半投影線分の合計が大きければ分離軸が存在しないので当たっている
		return false;
	}
	
	//==========================================
	//分離軸４：ポリゴンB上ベクトル(B.e2)
	//==========================================
	rA[3] = fabsf(D3DXVec3Dot(&EA1_RightVec, &NormalizeEB2_UpVec)) + fabsf(D3DXVec3Dot(&EA2_UpVec, &NormalizeEB2_UpVec));//分離軸EB2にポリゴンAの２軸を投影した長さ
	rB[3] = CCalculation::CalculationLength(ComSenterPosXZ, ComUpPosXZ);
	L[3] = fabsf(D3DXVec3Dot(&DirSenterXZ, &NormalizeEB2_UpVec));           //中心点間の距離を分離軸EB2に投影
	pDebugText->PrintDebugText("rA[3] = %f\n", rA[3]);
	pDebugText->PrintDebugText("rB[3] = %f\n", rB[3]);
	pDebugText->PrintDebugText("L[3] = %f\n", L[3]);
	if (L[3] > (rA[3] + rB[3]))
	{//投影下中心点間の距離よりも半投影線分の合計が大きければ分離軸が存在しないので当たっていない
		return false;
	}

	//==========================================
	//Y軸の当たり判定を確認
	//==========================================
	if (MyPos.y + MyVtxMax.y > ComPos.y + MyVtxMin.y &&
		MyPos.y + MyVtxMin.y < ComPos.y + MyVtxMax.y)
	{
		return true;//衝突している（２つのポリゴンの分離軸が存在しないので当たっている)6
	}
	else
	{
		return false;//衝突していない（Y軸が別)
	}
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
//レイとAABBの当たり判定、判定箇所も求める（全ての軸の交差範囲を比べる。重なっている範囲があれば、その範囲で当たっている）
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
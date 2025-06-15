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
	D3DXVECTOR3 MyPos = pMyObj->GetPosInfo().GetPos(); // 自分の位置
	D3DXVECTOR3 MyRot = pMyObj->GetRotInfo().GetRot(); // 自分の向き
	D3DXVECTOR3 MyVtxMin = pMyObj->GetSizeInfo().GetVtxMin(); // 自分の最小頂点
	D3DXVECTOR3 MyVtxMax = pMyObj->GetSizeInfo().GetVtxMax(); // 自分の最大頂点
	MyRot.y *= -1; // オブジェクトは後ろ向きなので前に合わせる

	D3DXVECTOR3 OtherPos = pComObj->GetPosInfo().GetPos(); // 相手の位置
	D3DXVECTOR3 OtherVtxMin = pComObj->GetSizeInfo().GetVtxMin(); // 相手の最小頂点
	D3DXVECTOR3 OtherVtxMax = pComObj->GetSizeInfo().GetVtxMax(); // 相手の最大頂点
	D3DXVECTOR2 FourOtherVtxPos[4] = {}; // 相手の４頂点

	// === 相手の4頂点を求める ===
	
	//左上[0]
	FourOtherVtxPos[0].x = OtherPos.x + OtherVtxMin.x;
	FourOtherVtxPos[0].y = OtherPos.z + OtherVtxMax.z;

	//右上[1]
	FourOtherVtxPos[1].x = OtherPos.x + OtherVtxMax.x;
	FourOtherVtxPos[1].y = OtherPos.z + OtherVtxMax.z;

	//左下[2]
	FourOtherVtxPos[2].x = OtherPos.x + OtherVtxMin.x;
	FourOtherVtxPos[2].y = OtherPos.z + OtherVtxMin.z;

	//右下[3]
	FourOtherVtxPos[3].x = OtherPos.x + OtherVtxMax.x;
	FourOtherVtxPos[3].y = OtherPos.z + OtherVtxMin.z;

    // Y軸先にチェック
	if (OtherPos.y + OtherVtxMax.y < MyPos.y + MyVtxMin.y ||
		OtherPos.y + OtherVtxMin.y > MyPos.y + MyVtxMax.y)
	{
		return false;
	}

	// 自分の回転角
	float sinR = sinf(MyRot.y);
	float cosR = cosf(MyRot.y);

	// 回転前の自分の4頂点
	D3DXVECTOR2 baseVtx[4] =
	{
		{ MyPos.x + MyVtxMin.x, MyPos.z + MyVtxMax.z }, // 左上
		{ MyPos.x + MyVtxMax.x, MyPos.z + MyVtxMax.z }, // 右上
		{ MyPos.x + MyVtxMin.x, MyPos.z + MyVtxMin.z }, // 左下
		{ MyPos.x + MyVtxMax.x, MyPos.z + MyVtxMin.z }, // 右下
	};

	// 回転後の頂点
	D3DXVECTOR2 rotVtx[4];
	for (int i = 0; i < 4; ++i)
	{
		float dx = baseVtx[i].x - MyPos.x; // X軸長さ
		float dz = baseVtx[i].y - MyPos.z; // Y軸長さ
		rotVtx[i].x = dx * cosR - dz * sinR + MyPos.x;
		rotVtx[i].y = dx * sinR + dz * cosR + MyPos.z;
	}

	// 相手の4頂点（XZ）
	D3DXVECTOR2 comVtx[4] =
	{
		{ OtherPos.x + OtherVtxMin.x, OtherPos.z + OtherVtxMax.z },
		{ OtherPos.x + OtherVtxMax.x, OtherPos.z + OtherVtxMax.z },
		{ OtherPos.x + OtherVtxMin.x, OtherPos.z + OtherVtxMin.z },
		{ OtherPos.x + OtherVtxMax.x, OtherPos.z + OtherVtxMin.z },
	};

	// 分離軸ベクトル
	D3DXVECTOR2 axes[4];
	D3DXVECTOR2 Sat[4] =
	{ rotVtx[0] - rotVtx[1] , // 自分の横
	  rotVtx[1] - rotVtx[3],  // 自分の縦
	  comVtx[0] - comVtx[1],  // 相手の横
	  comVtx[1] - comVtx[3]	  // 相手の縦
	};

	// 分離軸を正規化する
	D3DXVec2Normalize(&axes[0], &Sat[0]); // 自分の横
	D3DXVec2Normalize(&axes[1], &Sat[1]); // 自分の縦
	D3DXVec2Normalize(&axes[2], &Sat[2]); // 相手の横
	D3DXVec2Normalize(&axes[3], &Sat[3]); // 相手の縦

	// 分離軸判定
	for (int i = 0; i < 4; ++i)
	{
		// === それぞれの分離軸の最小値と最大値を初期化する ===

		float minA = D3DXVec2Dot(&rotVtx[0], &axes[i]);
		float maxA = minA;
		float minB = D3DXVec2Dot(&comVtx[0], &axes[i]);
		float maxB = minB;

		// === 分離軸が存在するかどうか判定する ===

		for (int j = 1; j < 4; ++j)
		{
			float projA = D3DXVec2Dot(&rotVtx[j], &axes[i]);
			minA = min(minA, projA); 
			maxA = max(maxA, projA);

			float projB = D3DXVec2Dot(&comVtx[j], &axes[i]);
			minB = min(minB, projB);
			maxB = max(maxB, projB);
		}

		// 分離軸が存在している
		if (maxA < minB || maxB < minA) return false;
	}

	return true;
}

//================================================================
//点がAABBの中に入っているかどうか
//================================================================
bool CCollision::IsPointInsideAABB(const D3DXVECTOR3& SelfPos, const D3DXVECTOR3& OtherPos, const D3DXVECTOR3& OtherVtxMax, const D3DXVECTOR3& OtherVtxMin)
{
	// 点がAABBの中に入っているかどうか
	if (SelfPos.x >= OtherPos.x + OtherVtxMin.x &&
		SelfPos.x <= OtherPos.x + OtherVtxMax.x &&
		SelfPos.y >= OtherPos.y + OtherVtxMin.y &&
		SelfPos.y <= OtherPos.y + OtherVtxMax.y &&
		SelfPos.z >= OtherPos.z + OtherVtxMin.z &&
		SelfPos.z <= OtherPos.z + OtherVtxMax.z)
	{
		return true;
	}

	return false;
}

//================================================================
// OBB同士の当たり判定
//================================================================
bool CCollision::OBBToOBB(CObjectX* pSelf, CObjectX* pOther)
{
	// === 列挙型 ===
	
	// 軸
	enum class AXIS
	{
		X = 0,
		Y,
		Z,
		MAX
	};

	// === 処理に使用する情報を宣言、初期化 ===
	CDebugText* pDebugText = CManager::GetDebugText(); // デバッグ表示情報

	CObjectX::PosInfo& SelfPosInfo = pSelf->GetPosInfo();    // 自分自身の位置情報
	CObjectX::RotInfo& SelfRotInfo = pSelf->GetRotInfo();    // 自分自身の向き情報
	CObjectX::SizeInfo& SelfSizeInfo = pSelf->GetSizeInfo(); // 自分自身のサイズ情報
	const D3DXVECTOR3& 
		SelfPos = SelfPosInfo.GetPos(),        // 自分自身の位置
		SelfRot = SelfRotInfo.GetRot(),        // 自分自身の向き
	    SelfVtxMax = SelfSizeInfo.GetVtxMax(), // 自分自身の最大頂点
	    SelfVtxMin = SelfSizeInfo.GetVtxMin(); // 自分自身の最小頂点
	D3DXVECTOR3
		SelfBoxCornars[8] = {}, // 自分自身の正方形の「角」を表す
		SelfWorldCornars[8] = {}; // 自分自身の回転後の「角」を表す

	CObjectX::PosInfo& OtherPosInfo = pOther->GetPosInfo();    // 相手の位置情報
	CObjectX::RotInfo& OtherRotInfo = pOther->GetRotInfo();    // 相手の向き情報
	CObjectX::SizeInfo& OtherSizeInfo = pOther->GetSizeInfo(); // 相手ののサイズ情報
	const D3DXVECTOR3& 
		OtherPos = OtherPosInfo.GetPos(),        // 相手の位置
		OtherRot = OtherRotInfo.GetRot(),        // 相手の向き       
	    OtherVtxMax = OtherSizeInfo.GetVtxMax(), // 相手の最大頂点
	    OtherVtxMin = OtherSizeInfo.GetVtxMin(); // 相手の最小頂点
	D3DXVECTOR3
		OtherBoxCornars[8] = {}, // 相手の正方形の「角」を表す
		OtherWorldCornars[8] = {}; // 相手の回転後の「角」を表す

	// === 正方形８頂点の位置を求める ===

	// 自身
	SelfBoxCornars[0] = D3DXVECTOR3(SelfVtxMin.x, SelfVtxMax.y, SelfVtxMax.z); // 左上前
	SelfBoxCornars[1] = SelfVtxMax; // 右上前
	SelfBoxCornars[2] = D3DXVECTOR3(SelfVtxMin.x, SelfVtxMax.y, SelfVtxMin.z); // 左上後
	SelfBoxCornars[3] = D3DXVECTOR3(SelfVtxMax.x, SelfVtxMax.y, SelfVtxMin.z); // 右上後
	SelfBoxCornars[4] = D3DXVECTOR3(SelfVtxMin.x, SelfVtxMin.y, SelfVtxMax.z); // 左下前
	SelfBoxCornars[5] = D3DXVECTOR3(SelfVtxMax.x, SelfVtxMin.y, SelfVtxMax.z); // 右下前
	SelfBoxCornars[6] = SelfVtxMin; // 左下後
	SelfBoxCornars[7] = D3DXVECTOR3(SelfVtxMax.x, SelfVtxMin.y, SelfVtxMin.z); // 右下後

	// 相手
	OtherBoxCornars[0] = D3DXVECTOR3(OtherVtxMin.x, OtherVtxMax.y, OtherVtxMax.z); // 左上前
	OtherBoxCornars[1] = OtherVtxMax; // 右上前
	OtherBoxCornars[2] = D3DXVECTOR3(OtherVtxMin.x, OtherVtxMax.y, OtherVtxMin.z); // 左上後
	OtherBoxCornars[3] = D3DXVECTOR3(OtherVtxMax.x, OtherVtxMax.y, OtherVtxMin.z); // 右上後
	OtherBoxCornars[4] = D3DXVECTOR3(OtherVtxMin.x, OtherVtxMin.y, OtherVtxMax.z); // 左下前
	OtherBoxCornars[5] = D3DXVECTOR3(OtherVtxMax.x, OtherVtxMin.y, OtherVtxMax.z); // 右下前
	OtherBoxCornars[6] = OtherVtxMin; // 左下後
	OtherBoxCornars[7] = D3DXVECTOR3(OtherVtxMax.x, OtherVtxMin.y, OtherVtxMin.z); // 右下後

	// === 自身の回転後の８頂点の位置を求める ===

	D3DXMATRIX SelfRotMatrix; // 自分自身の回転行列
	D3DXMatrixIdentity(&SelfRotMatrix); // 初期化

	// 自身の回転行列を求める
	D3DXMatrixRotationYawPitchRoll(&SelfRotMatrix,SelfRot.y, SelfRot.x, SelfRot.z);

	// 自身の回転行列を用いて回転後の８頂点を求める
	for (int i = 0; i < 8; ++i) {
		D3DXVec3TransformCoord(&SelfWorldCornars[i], &SelfBoxCornars[i], &SelfRotMatrix);
		SelfWorldCornars[i] += SelfPos;
	}

	// === 相手の回転後の８頂点の位置を求める ===

	D3DXMATRIX OtherRotMatrix; // 相手の回転行列
	D3DXMatrixIdentity(&OtherRotMatrix); // 初期化

    // 相手の回転行列を求める
	D3DXMatrixRotationYawPitchRoll(&OtherRotMatrix, OtherRot.y, OtherRot.x, OtherRot.z);

	// 相手の回転行列を用いて回転後の８頂点を求める
	for (int i = 0; i < 8; ++i) {
		D3DXVec3TransformCoord(&OtherWorldCornars[i], &OtherBoxCornars[i], &OtherRotMatrix);
		OtherWorldCornars[i] += OtherPos;
	}


	// ８頂点が表せているか確認用
	{
		//// 回転後の８頂点を表せているかチェック
		//for (int nCnt = 0; nCnt < 8; nCnt++)
		//{
		//	CParticle::Create(CParticle::TYPE::TYPE00_NORMAL, 2, 30.0f, 30.0f, SelfWorldCornars[nCnt], D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), true);
		//}
	}

	// === ローカル分離軸を求める ===

	// 基準となる軸
	D3DXVECTOR3 BaseAxis[static_cast<int>(AXIS::MAX)] =
	{ {1.0f,0.0f,0.0f},
	 {0.0f,1.0f,0.0f},
	 {0.0f,0.0f,1.0f}
	};

	// 自身の分離軸
	D3DXVECTOR3 SelfSepAxis[static_cast<int>(AXIS::MAX)];

	// それぞれの分離軸を求める
	for (int nCnt = 0; nCnt < static_cast<int>(AXIS::MAX); nCnt++)
	{
		D3DXVec3TransformCoord(&SelfSepAxis[nCnt], &BaseAxis[nCnt], &SelfRotMatrix);
	}

	// 相手の分離軸
	D3DXVECTOR3 OtherSepAxis[static_cast<int>(AXIS::MAX)];

	// それぞれの分離軸を求める
	for (int nCnt = 0; nCnt < static_cast<int>(AXIS::MAX); nCnt++)
	{
		D3DXVec3TransformCoord(&OtherSepAxis[nCnt], &BaseAxis[nCnt], &OtherRotMatrix);
	}

	// === ローカル分離軸に回転後の頂点を投影し、分離しているかどうかを求める ===

	// 自分のローカル軸に互いの回転後頂点を投影
	for (int nCntAxis = 0; nCntAxis < 3; nCntAxis++)
	{
		// お互いのオブジェクトの投影値の最大最小を初期化
		float
			fSelfMin = D3DXVec3Dot(&SelfSepAxis[nCntAxis], &SelfWorldCornars[0]),
			fSelfMax= D3DXVec3Dot(&SelfSepAxis[nCntAxis], &SelfWorldCornars[0]),

			fOtherMin = D3DXVec3Dot(&SelfSepAxis[nCntAxis], &OtherWorldCornars[0]),
			fOtherMax = D3DXVec3Dot(&SelfSepAxis[nCntAxis], &OtherWorldCornars[0]);

		// 相手の回転後頂点を投影
		for (int nCntVtx = 0; nCntVtx < 8; nCntVtx++)
		{
			// 互いの頂点の投影線分を求める
			float fSelfLength = D3DXVec3Dot(&SelfSepAxis[nCntAxis], &SelfWorldCornars[nCntVtx]);
			float fOtherLength = D3DXVec3Dot(&SelfSepAxis[nCntAxis], &OtherWorldCornars[nCntVtx]);

			// 自身の投影値の最低値を更新
			if (fSelfLength < fSelfMin)
			{
				fSelfMin = fSelfLength;
			}

			// 自身の投影値の最大値を更新
			if (fSelfLength > fSelfMax)
			{
				fSelfMax = fSelfLength;
			}

			// 相手の投影値の最低値を更新
			if (fOtherLength < fOtherMin)
			{
				fOtherMin = fOtherLength;
			}

			// 相手の投影値の最大値を更新
			if (fOtherLength > fOtherMax)
			{
				fOtherMax = fOtherLength;
			}
		}

		// 分離軸が存在しているので処理を終了
		if (fSelfMax < fOtherMin || fOtherMax < fSelfMin)
		{
			return false;
		}
	}

	// 相手のローカル軸に互いの回転後頂点を投影
	for (int nCntAxis = 0; nCntAxis < 3; nCntAxis++)
	{
		// お互いのオブジェクトの投影値の最大最小を初期化
		float
			fSelfMin = D3DXVec3Dot(&OtherSepAxis[nCntAxis], &SelfWorldCornars[0]),
			fSelfMax = D3DXVec3Dot(&OtherSepAxis[nCntAxis], &SelfWorldCornars[0]),

			fOtherMin = D3DXVec3Dot(&OtherSepAxis[nCntAxis], &OtherWorldCornars[0]),
			fOtherMax = D3DXVec3Dot(&OtherSepAxis[nCntAxis], &OtherWorldCornars[0]);

		// 相手の回転後頂点を投影
		for (int nCntVtx = 0; nCntVtx < 8; nCntVtx++)
		{
			// 互いの頂点の投影線分を求める
			float fSelfLength = D3DXVec3Dot(&OtherSepAxis[nCntAxis], &SelfWorldCornars[nCntVtx]);
			float fOtherLength = D3DXVec3Dot(&OtherSepAxis[nCntAxis], &OtherWorldCornars[nCntVtx]);

			// 自身の投影値の最低値を更新
			if (fSelfLength < fSelfMin)
			{
				fSelfMin = fSelfLength;
			}

			// 自身の投影値の最大値を更新
			if (fSelfLength > fSelfMax)
			{
				fSelfMax = fSelfLength;
			}

			// 相手の投影値の最低値を更新
			if (fOtherLength < fOtherMin)
			{
				fOtherMin = fOtherLength;
			}

			// 相手の投影値の最大値を更新
			if (fOtherLength > fOtherMax)
			{
				fOtherMax = fOtherLength;
			}
		}

		// 分離軸が存在しているので処理を終了
		if (fSelfMax < fOtherMin || fOtherMax < fSelfMin)
		{
			return false;
		}
	}

	// === それぞれの分離軸と直行する分離軸と互いの回転後頂点を投影 ===

	D3DXVECTOR3 CrossSepAxis[9] = {}; // 互いの分離軸と直行する分離軸
	int nCntArray = 0; // 配列カウント用

	// 互いの分離軸と直行する分離軸を求める
	for (int nCntSelfSepAxis = 0; nCntSelfSepAxis < 3; nCntSelfSepAxis++)
	{
		for (int nCntOtherSepAxis = 0; nCntOtherSepAxis < 3; nCntOtherSepAxis++)
		{
			D3DXVec3Cross(&CrossSepAxis[nCntArray], &SelfSepAxis[nCntSelfSepAxis], &OtherSepAxis[nCntOtherSepAxis]);
			D3DXVec3Normalize(&CrossSepAxis[nCntArray], &CrossSepAxis[nCntArray]); // 正規化
			nCntArray++;
		}
	}

	// 求めた直行する分離軸に互いの回転後頂点を投影し判定する
	for (int nCntAxis = 0; nCntAxis < 9; nCntAxis++)
	{
		// 長さの２条がほぼ0ならスキップ（0.00001f)
		if (D3DXVec3LengthSq(&CrossSepAxis[nCntAxis]) < 1e-5f)
		{
			continue;
		}

		// お互いのオブジェクトの投影値の最大最小を初期化
		float
			fSelfMin = D3DXVec3Dot(&CrossSepAxis[nCntAxis], &SelfWorldCornars[0]),
			fSelfMax = D3DXVec3Dot(&CrossSepAxis[nCntAxis], &SelfWorldCornars[0]),

			fOtherMin = D3DXVec3Dot(&CrossSepAxis[nCntAxis], &OtherWorldCornars[0]),
			fOtherMax = D3DXVec3Dot(&CrossSepAxis[nCntAxis], &OtherWorldCornars[0]);

		// 相手の回転後頂点を投影
		for (int nCntVtx = 0; nCntVtx < 8; nCntVtx++)
		{
			// 互いの頂点の投影線分を求める
			float fSelfLength = D3DXVec3Dot(&CrossSepAxis[nCntAxis], &SelfWorldCornars[nCntVtx]);
			float fOtherLength = D3DXVec3Dot(&CrossSepAxis[nCntAxis], &OtherWorldCornars[nCntVtx]);


			// 自身の投影値の最低値を更新
			if (fSelfLength < fSelfMin)
			{
				fSelfMin = fSelfLength;
			}

			// 自身の投影値の最大値を更新
			if (fSelfLength > fSelfMax)
			{
				fSelfMax = fSelfLength;
			}

			// 相手の投影値の最低値を更新
			if (fOtherLength < fOtherMin)
			{
				fOtherMin = fOtherLength;
			}

			// 相手の投影値の最大値を更新
			if (fOtherLength > fOtherMax)
			{
				fOtherMax = fOtherLength;
			}
		}

		// 分離軸が存在しているので処理を終了
		if (fSelfMax < fOtherMin || fOtherMax < fSelfMin)
		{
			return false;
		}
	}

	// 分離軸が存在しないので当たっている
	return true;
}

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
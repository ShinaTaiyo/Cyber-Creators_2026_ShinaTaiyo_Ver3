//===========================================================================================
//
//６月４日：モデルを描画する[objectX.cpp]
//Author:ShinaTaiyo
//
//===========================================================================================

//===============================
//インクルード
//===============================
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "objectX.h"
#include "manager.h"
#include "block.h"
#include "model.h"
#include "camera.h"
#include "bullet.h"
#include "input.h"
#include "debugtext.h"
#include "damage.h"
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <algorithm>
//===========================================================================================

//================================================
//コンストラクタ
//================================================
CObjectX::CObjectX(int nPriority) : CObject(nPriority),m_bUseDraw(true)
{

}
//================================================================================================================================================

//================================================
//デストラクタ
//================================================
CObjectX::~CObjectX()
{

}
//================================================================================================================================================

//================================================
//初期化処理
//================================================
HRESULT CObjectX::Init()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();//デバイスへのポインタと取得
	//=================================
	//モデル情報の初期化
	//=================================
	m_ObjectXInfo.dwNumMat = {};                                          //マテリアルの数
	m_ObjectXInfo.pBuffMat = {};                                          //頂点バッファへのポイン タ
	m_ObjectXInfo.pMesh = {};                                             //頂点情報へのポインタ
	m_ObjectXInfo.pTexture = nullptr;                                     //テクスチャへのポインタ
	m_ObjectXInfo.Diffuse = nullptr;                                     //色合い
	m_ObjectXInfo.FormarDiffuse = nullptr;                               //元の色合い
	//===================================================================================================================================================

	//==========================
	//変数の初期化
	//==========================
	m_mtxWorld = {};                                   //ワールドマトリックス
	m_bUse = false;                                    //使用しているかどうか
	m_Pos = NULL_VECTOR3;                              //位置
	m_nChengeColorTime = 0;                            //色を変える時間
	m_PosOld = NULL_VECTOR3;                           //1f前の位置
	m_SupportPos = NULL_VECTOR3;                       //設置位置
	m_Rot = NULL_VECTOR3;                              //向き
	m_nIdxShadow = -1;                                 //影のインデックス
	m_nManagerType = 0;                                //マネージャーで呼び出される際のタイプ番号
	m_bColorChenge = false;                            //色合いを変えているかどうか
	m_Move = NULL_VECTOR3;                             //移動量
	m_fInertia = m_fNORMAL_INERTIA;                                 //慣性

	m_VtxMin = NULL_VECTOR3;                           //位置の最小値
	m_OriginVtxMin = NULL_VECTOR3;                     //元々の位置の最小値
	m_VtxMax = NULL_VECTOR3;                           //位置の最大値
	m_OriginVtxMax = NULL_VECTOR3;                     //元々の位置の最大値
	m_nNumVtx = 0;                                     //頂点数
	m_SizeFVF;                                         //頂点フォーマットのサイズ
	m_pVtxBuff;                                        //頂点バッファへのポインタ
	m_Scale = NULL_VECTOR3;                            //拡大率
	m_FormarScale = NULL_VECTOR3;                      //元の拡大率
	m_Size = NULL_VECTOR3;                             //サイズ

	m_bIsLanding = false;                              //地面にいるかどうか
	m_bUseInteria = true;                              //慣性をかけるかどうか
	m_bIsWalling = false;                              //壁にくっついているかどうか
	m_bIsJumping = false;                              //ジャンプしているかどうか

	m_nObjXType = (OBJECTXTYPE)(0);                    //種類

	m_RotType = (ROTTYPE)(0);                          //向きの種類

	m_MoveType = (MOVETYPE)(0);                        //移動タイプ

	m_pLiftLandingObj = nullptr;                       //乗っているリフトのオブジェクトを取得する
	m_bIsLiftLanding = false;                          //リフトの上に乗っているかどうか
	//==================================================================================

	//===========================================
	//オフセット系
	//===========================================
	m_ParentSetPos = NULL_VECTOR3;                     //親の位置
	m_ParentSetRot = NULL_VECTOR3;                     //親の向き
	m_ParentSetScale = NULL_VECTOR3;                   //親の拡大率
	m_bUseParent = false;                              //親を使用するかどうか
	m_ParentMtxWorld = {};                             //親のマトリックスワールド

	m_OffSetPos = NULL_VECTOR3;                        //オフセットの位置
	m_OffSetRot = NULL_VECTOR3;                        //オフセットの向き
	m_OffSetScale = NULL_VECTOR3;                      //オフセットの拡大率
	//==================================================================================

	//===========================================
	//体力関係
	//===========================================
	m_nLife = 1;                                        //体力
	m_nMaxLife = 1;                                     //最大体力
	m_bAutoSubLife = false;                             //自動的に体力を減らし続けるかどうか
	m_bHitStop = false;                                 //ヒットストップ状態かどうか
	m_nHitStopTime = 0;                                 //ヒットストップ時間
	//==================================================================================

	//===========================================
	//影関係
	//===========================================
	m_bUseShadow = true;
	//==================================================================================

	//===========================================
	//重力関係
	//===========================================
	m_bUseGravity = true;         //重力を使用するかどうか
	m_bTimeChengeGravity = false; //重力のONOFFを時間で変えるかどうか
	m_nNotUseGravityTime = 0;     //重力を使わない時間
	//==================================================================================

	//===========================================
	//ブロック当たり判定関係
	//===========================================
	m_bUseBlockCollision = true;//ブロックとの当たり判定を使用するかどうか
	m_bSuccessBlockCollisionX = false;//X方向のブロックとの当たり判定が成功したかどうか
	m_bSuccessBlockCollisionY = false;//Y方向のブロックとの当たり判定が成功したかどうか
	m_bSuccessBlockCollisionZ = false;//Z方向のブロックとの当たり判定が成功したかどうか
	//==================================================================================

	//===========================================
	//位置更新関係
	//===========================================
	m_bUseUpdatePos = true;//位置の更新を使用するかどうか
	m_bUseAdjustOriginPos = true;//原点に到達したときに位置の補正を行うかどうか
	//==================================================================================

	//===========================================
	//召喚関係
	//===========================================
	m_bSummonObjectDeath = false; //このオブジェクトXが召喚したオブジェクトを消すフラグ
	//==================================================================================

	//===========================================
	//中心点関係
	//===========================================
	m_SenterPos = NULL_VECTOR3;//中心点
	//==================================================================================

	//===================================
	//回転関係
	//===================================
	m_bUseAddRot = false;
	m_AddRot = NULL_VECTOR3;
	//==================================================================================

	//===================================
	//色合い関係
	//===================================
	m_bUseRatioLifeAlpha = false;
	//==================================================================================

	m_bUseMultiSpeed = false;                 //乗算加速をするかどうか
	m_MultiSpeed = NULL_VECTOR3;              //乗算加速度

	m_bUseAddSpeed = false;                   //加速を使用するかどうか
	m_AddSpeed = NULL_VECTOR3;                //加速度

	m_bUseGravity = false;                    //重力を使用するかどうか　
	m_fGravityPower = 0.0f;                   //重力の大きさ

	m_bUseAddScaling = false;                 //拡大率の加算を使用するかどうか
	m_AddScale = NULL_VECTOR3;                //拡大率の加算量    

	//===========================================
	//回避関係
	//===========================================
	m_bDodge = false;
	//==================================================================================

	return S_OK;
}
//================================================================================================================================================

//================================================
//終了処理
//================================================
void CObjectX::Uninit()
{
	if (m_ObjectXInfo.Diffuse != nullptr)
	{
		delete[] m_ObjectXInfo.Diffuse;//色合いの動的メモリを開放
		m_ObjectXInfo.Diffuse = nullptr;
	}

	if (m_ObjectXInfo.pTexture != nullptr)
	{
		delete[] m_ObjectXInfo.pTexture;//テクスチャの動的メモリを解放
		m_ObjectXInfo.pTexture = nullptr;
	}

	if (m_ObjectXInfo.FormarDiffuse != nullptr)
	{
		delete[] m_ObjectXInfo.FormarDiffuse;//色合いの動的メモリを開放
		m_ObjectXInfo.FormarDiffuse = nullptr;
	}
}
//================================================================================================================================================

//================================================
//更新処理
//================================================
void CObjectX::Update()
{
	float fRatio = 0.0f;
	if (m_bUseParent == true)
	{//子マトリックスを使用する場合に、ワールド座標を判定する
		D3DXVECTOR3 PosZero = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXVec3TransformCoord(&m_OffSetPos, &PosZero, &m_mtxWorld);//左の引数のローカル座標に真ん中の引数のワールド座標を代入する
	}

	//==============================================
	//中心点を求める
	//==============================================
	CalculateSenterPos();

	//==============================================
	//体力を減らす
	//==============================================
	if (m_bAutoSubLife == true)
	{
		m_nLife--;
	}
	//===========================================================

	//==============================================
	//体力の割合で色合いの透明度を変える
	//==============================================
	if (m_bUseRatioLifeAlpha == true)
	{
		fRatio = float(m_nLife) / float(m_nMaxLife);
		SetColorAlphaOnly(fRatio);
	}
	//===========================================================

	//==================================
	//乗算加速度がONになっていたら
	//==================================
	if (m_bUseMultiSpeed == true)
	{
		m_Move.x *= m_MultiSpeed.x;
		m_Move.y *= m_MultiSpeed.y;
		m_Move.z *= m_MultiSpeed.z;
	}
	//==========================================================

	//==================================
	//加速がONになっていたら
	//==================================
	if (m_bUseAddSpeed == true)
	{
		m_Move += m_AddSpeed;
	}
	//==========================================================

	//==================================
	//拡大率の加算がONになっていたら
	//==================================
	if (m_bUseAddScaling == true)
	{
		m_Scale += m_AddScale;
	}
	//==========================================================

	//==============================================
	//重力関係
	//==============================================
	ChengeGravityProcess();
	//===========================================================


	//==============================================
    //ヒットストップ処理
    //==============================================
	HitStopProcess();
	//===========================================================


	//常に拡大率を参照して最大最小頂点を求める
	m_VtxMax.x = m_OriginVtxMax.x * m_Scale.x;
	m_VtxMax.y = m_OriginVtxMax.y * m_Scale.y;
	m_VtxMax.z = m_OriginVtxMax.z * m_Scale.z;
	m_VtxMin.x = m_OriginVtxMin.x * m_Scale.x;
	m_VtxMin.y = m_OriginVtxMin.y * m_Scale.y;
	m_VtxMin.z = m_OriginVtxMin.z * m_Scale.z;

	m_Size = m_VtxMax - m_VtxMin;

	if (m_bColorChenge == true)
	{
		m_nChengeColorTime--;
	}

	if (m_nChengeColorTime <= 0 && m_bColorChenge == true )
	{
		m_nChengeColorTime = 0;
		SetFormarColor();//元の色合いに戻す
		m_bColorChenge = false;
	}

	if (m_bIsLiftLanding == true && m_pLiftLandingObj != nullptr && m_bUseBlockCollision == true)
	{//動くブロックの上に位置を補正
  		CBlock::LandingCorrection(m_Pos, m_pLiftLandingObj, m_VtxMax, m_VtxMin);
	}

	if (m_bUseUpdatePos == true)
	{//位置更新処理
		UpdatePos();
	}

	if (m_bUseAddRot == true)
	{//向きの加算処理
		m_Rot += m_AddRot;
	}

	if (m_bUseBlockCollision == true)
	{//ブロック当たり判定
		CommonBlockCollision();
	}

	if (GetStageManagerChoose() == true)
	{//オブジェクトXの情報を表示
	    DispInfo();
	}
}
//================================================================================================================================================

//================================================
//描画処理
//================================================
void CObjectX::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice(); //デバイスへのポインタと取得
	D3DXMATRIX mtxRot, mtxTrans, mtxScale;                  //計算用マトリックス
	D3DMATERIAL9 matDef;                                              //現在のマテリアル保存用
	D3DXMATERIAL* pMat;                                               //マテリアルデータへのポインタ

	//現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	if (m_bUseParent == true)
	{
		OffSetUpdate();
	}

	//大きさを反映
	D3DXMatrixScaling(&mtxScale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	if (m_bUseParent == true)
	{
		//親のマトリックスとかけ合わせる
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &m_ParentMtxWorld);
	}

	if (m_ObjectXInfo.Diffuse[0].a > 0.0f && m_Pos.y + m_VtxMax.y >= 0.0f && m_bUseShadow == true && CManager::GetCamera()->GetCameraType() == CCamera::CAMERATYPE_BOSSBATTLE)
	{
		//影の描画
		DrawShadow(m_Pos);
	}
	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	if (GetType() == CObject::TYPE_ATTACK)
	{
		int n = 0;
	}
	//=======================================
	//描画の調整
	//=======================================
	//アルファテストを有効(アルファ値が０より大きい場合に描画する）
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	//法線の長さを１にする。（スケールなどを使った時は、必ず使う。)
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
	//================================================================================================================

	//マテリアルへのポインタを取得
	pMat = (D3DXMATERIAL*)m_ObjectXInfo.pBuffMat->GetBufferPointer();

	if (m_bUseDraw == true)
	{
		//==========================================================================
		//マテリアルの数分、テクスチャを読み込む。
		//==========================================================================
		for (int nCntMat = 0; nCntMat < (int)m_ObjectXInfo.dwNumMat; nCntMat++)
		{
			//色合いの設定
			pMat[nCntMat].MatD3D.Diffuse = m_ObjectXInfo.Diffuse[nCntMat];

			//マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			//テクスチャを設定する
			pDevice->SetTexture(0, m_ObjectXInfo.pTexture[nCntMat]);

			//モデル（パーツ）の描画
			m_ObjectXInfo.pMesh->DrawSubset(nCntMat);
		}
		//================================================================================================================
	}
	
	//=======================================
	//描画の調整を元に戻す
	//=======================================
	//アルファテスト無効に戻す
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	//法線の長さを１にする。（スケールなどを使った時は、必ず使う。)
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
	//================================================================================================================

	//保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);

}
//================================================================================================================================================

//==========================================================================
//重力を使用する
//==========================================================================
void CObjectX::SetUseGravity(float fGravityPower)
{
	m_bUseGravity = true;
	m_fGravityPower = fGravityPower;
}
//================================================================================================================================================

//==========================================================================
//拡大率の加算を設定
//==========================================================================
void CObjectX::SetUseAddScale(D3DXVECTOR3 AddScale, bool bUse)
{
	m_bUseAddScaling = bUse;
	m_AddScale = AddScale;
}
//===================================================================================================================================================

//================================================
//モデル情報を割り当てる
//================================================
void CObjectX::BindObjectXInfo(LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD dwNumMat, LPDIRECT3DTEXTURE9* pTexture, D3DCOLORVALUE* pDiffuse)
{
	m_ObjectXInfo.pMesh = pMesh;
	m_ObjectXInfo.pBuffMat = pBuffMat;
	m_ObjectXInfo.dwNumMat = dwNumMat;
	m_ObjectXInfo.pTexture = DBG_NEW LPDIRECT3DTEXTURE9[int(dwNumMat)];
	m_ObjectXInfo.Diffuse = DBG_NEW D3DCOLORVALUE[int(dwNumMat)];
	m_ObjectXInfo.FormarDiffuse = DBG_NEW D3DCOLORVALUE[int(dwNumMat)];
	for (int nCnt = 0; nCnt < (int)(dwNumMat); nCnt++)
	{
		m_ObjectXInfo.pTexture[nCnt] = pTexture[nCnt];
		m_ObjectXInfo.Diffuse[nCnt] = pDiffuse[nCnt];
		m_ObjectXInfo.FormarDiffuse[nCnt] = pDiffuse[nCnt];
	}
}
//================================================================================================================================================

//================================================
//色合いを設定する
//================================================
void CObjectX::SetColor(COLORTYPE type, int nColChengeTime)
{
	m_bColorChenge = true;
	m_nChengeColorTime = nColChengeTime;
	switch (type)
	{
	case COLORTYPE_RED:
		for (int nCnt = 0; nCnt < (int)(m_ObjectXInfo.dwNumMat); nCnt++)
		{
			m_ObjectXInfo.Diffuse[nCnt].r = 1.0f;
			m_ObjectXInfo.Diffuse[nCnt].g = 0.0f;
			m_ObjectXInfo.Diffuse[nCnt].b = 0.0f;
			m_ObjectXInfo.Diffuse[nCnt].a = 1.0f;
		}
		break;
	case COLORTYPE_GREEN:
		for (int nCnt = 0; nCnt < (int)(m_ObjectXInfo.dwNumMat); nCnt++)
		{
			m_ObjectXInfo.Diffuse[nCnt].r = 0.0f;
			m_ObjectXInfo.Diffuse[nCnt].g = 1.0f;
			m_ObjectXInfo.Diffuse[nCnt].b = 0.0f;
			m_ObjectXInfo.Diffuse[nCnt].a = 1.0f;
		}
		break;
	case COLORTYPE_BRUE:
		for (int nCnt = 0; nCnt < (int)(m_ObjectXInfo.dwNumMat); nCnt++)
		{
			m_ObjectXInfo.Diffuse[nCnt].r = 0.0f;
			m_ObjectXInfo.Diffuse[nCnt].g = 0.0f;
			m_ObjectXInfo.Diffuse[nCnt].b = 1.0f;
			m_ObjectXInfo.Diffuse[nCnt].a = 1.0f;
		}
		break;
	case COLORTYPE_ALPHA:
		for (int nCnt = 0; nCnt < (int)(m_ObjectXInfo.dwNumMat); nCnt++)
		{
			m_ObjectXInfo.Diffuse[nCnt].a = 0.5f;
		}
		break;
	default:
		break;
	}
}
//================================================================================================================================================

//================================================
//色のアルファ値だけを変える
//================================================
void CObjectX::SetColorAlphaOnly(float fAlpha)
{
	for (int nCnt = 0; nCnt < (int)(m_ObjectXInfo.dwNumMat); nCnt++)
	{
		m_ObjectXInfo.Diffuse[nCnt].a = fAlpha;
	}
}
//================================================================================================================================================

//================================================
//オリジナルの色合いを設定する
//================================================
void CObjectX::SetOriginalColor(D3DXCOLOR col, int nColChengeTime)
{
	m_bColorChenge = true;
	m_nChengeColorTime = nColChengeTime;
	for (int nCnt = 0; nCnt < (int)(m_ObjectXInfo.dwNumMat); nCnt++)
	{
		m_ObjectXInfo.Diffuse[nCnt].r = col.r;
		m_ObjectXInfo.Diffuse[nCnt].g = col.g;
		m_ObjectXInfo.Diffuse[nCnt].b = col.b;
		m_ObjectXInfo.Diffuse[nCnt].a = col.a;
	}

}
//================================================================================================================================================

//================================================
//共通のブロックとの当たり判定
//================================================
void CObjectX::CommonBlockCollision()
{
	if (m_bUseBlockCollision == true)
	{
		CBlock::CollisionSquare(this);
	}
}
//================================================================================================================================================

//================================================
//中心点を求める
//================================================
void CObjectX::CalculateSenterPos()
{
	float fVtxMaxPosY = fabsf(m_VtxMax.y);
	float fVtxMinPosY = fabsf(m_VtxMin.y);

	float fAdjustPosY = 0.0f;

	m_SenterPos = m_Pos;
	m_SenterPos.y += (m_VtxMax.y + m_VtxMin.y) / 2;
}
//================================================================================================================================================

//================================================
//元の色合いに戻す
//================================================
void CObjectX::SetFormarColor()
{
	for (int nCnt = 0; nCnt < (int)(m_ObjectXInfo.dwNumMat); nCnt++)
	{
		m_ObjectXInfo.Diffuse[nCnt] = m_ObjectXInfo.FormarDiffuse[nCnt];
	}
}
//================================================================================================================================================

//================================================
//オフセットの情報を設定する
//================================================
void CObjectX::OffSetUpdate()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice(); //デバイスへのポインタと取得
	D3DXMATRIX mtxRot, mtxTrans, mtxScale;                  //計算用マトリックス


	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_ParentMtxWorld);

	//大きさを反映
	D3DXMatrixScaling(&mtxScale, m_ParentSetScale.x, m_ParentSetScale.y, m_ParentSetScale.z);
	D3DXMatrixMultiply(&m_ParentMtxWorld, &m_ParentMtxWorld, &mtxScale);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_ParentSetRot.y, m_ParentSetRot.x, m_ParentSetRot.z);
	D3DXMatrixMultiply(&m_ParentMtxWorld, &m_ParentMtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_ParentSetPos.x, m_ParentSetPos.y, m_ParentSetPos.z);
	D3DXMatrixMultiply(&m_ParentMtxWorld, &m_ParentMtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_ParentMtxWorld);

}
//================================================================================================================================================

//================================================
//重力を変える処理
//================================================
void CObjectX::ChengeGravityProcess()
{
	if (m_bTimeChengeGravity == true)
	{//一定時間重力を使わない場合に使用。Setでこの処理を呼び出さない限り、直接使用状態のONOFFを変えれる。
		if (m_nNotUseGravityTime > 0)
		{
			m_nNotUseGravityTime--;//重力を使わない時間を減らす
		}

		if (m_nNotUseGravityTime > 0)
		{
			m_bUseGravity = false;
		}
		else
		{
			m_bUseGravity = true;
			m_bTimeChengeGravity = false;//時間で重力を変える状態を解除
		}
	}
}
//================================================================================================================================================

//================================================
//ヒットストップの処理
//================================================
void CObjectX::HitStopProcess()
{
	if (m_bHitStop == true)
	{
		if (m_nHitStopTime > 0)
		{
			m_nHitStopTime--;
		}
		else
		{//ヒットストップ解除
			m_bHitStop = false;
			m_nHitStopTime = 0;
		}
	}
}
//================================================================================================================================================

//================================================
//サイズを設定する
//================================================
void CObjectX::SetSize()
{
	BYTE* pVtxBuff;//頂点バッファへのポインタ

	//頂点数の取得
	int nNumVtx = m_ObjectXInfo.pMesh->GetNumVertices();

	//頂点フォーマットのサイズを取得
	DWORD sizeFVF = D3DXGetFVFVertexSize(m_ObjectXInfo.pMesh->GetFVF());

	//頂点バッファのロック
    m_ObjectXInfo.pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		//頂点座標の代入
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

		//====================================================
		//頂点座標を比較してモデルの最小値最大値を取得
		//====================================================
		if (vtx.x > m_OriginVtxMax.x)
		{
			m_OriginVtxMax.x = vtx.x;
		}
		else if (vtx.y > m_OriginVtxMax.y)
		{//今回読み込んだ頂点が、一番大きい頂点より大きい場合
			m_OriginVtxMax.y = vtx.y;
		}
		else if (vtx.z > m_OriginVtxMax.z)
		{//今回読み込んだ頂点が、一番大きい頂点より大きい場合
			m_OriginVtxMax.z = vtx.z;
		}
		else if (vtx.x < m_OriginVtxMin.x)
		{//今回読み込んだ頂点が、一番小さい頂点より小さい場合
			m_OriginVtxMin.x = vtx.x;
		}
		else if (vtx.y < m_OriginVtxMin.y)
		{//今回読み込んだ頂点が、一番小さい頂点より小さい場合
			m_OriginVtxMin.y = vtx.y;
		}
		else if (vtx.z < m_OriginVtxMin.z)
		{//今回読み込んだ頂点が、一番小さい頂点より小さい場合
			m_OriginVtxMin.z = vtx.z;
		}
		//=============================================================================================================

		//頂点フォーマットのサイズ文ポインタを進める
		pVtxBuff += sizeFVF;

	}
	//頂点バッファのアンロック
	m_ObjectXInfo.pMesh->UnlockVertexBuffer();

	//==========================================================
	//上の処理で出した最小最大の頂点を拡大率で計算し直す
	//==========================================================
	m_VtxMax.x = m_OriginVtxMax.x * m_Scale.x;
	m_VtxMax.y = m_OriginVtxMax.y * m_Scale.y;
	m_VtxMax.z = m_OriginVtxMax.z * m_Scale.z;
	m_VtxMin.x = m_OriginVtxMin.x * m_Scale.x;
	m_VtxMin.y = m_OriginVtxMin.y * m_Scale.y;
	m_VtxMin.z = m_OriginVtxMin.z * m_Scale.z;
	//================================================================================================================================================

	//==========================================================
	//最大最小を参照してサイズを設定する
	//==========================================================
	m_Size = m_VtxMax - m_VtxMin;
	//================================================================================================================================================

}
//================================================================================================================================================

//============================================================================
//向きの種類を設定
//============================================================================
void CObjectX::SetRotType(ROTTYPE RecvRotType)
{
	//m_RotType = RecvRotType;

	//if (m_RotType == ROTTYPE_NORMAL)
	//{
	//	m_Rot = NULL_VECTOR3;
	//	m_VtxMax.x = m_OriginVtxMax.x * m_Scale.x;
	//	m_VtxMax.z = m_OriginVtxMax.z * m_Scale.z;

	//	m_VtxMin.x = m_OriginVtxMin.x * m_Scale.x;
	//	m_VtxMin.z = m_OriginVtxMin.z * m_Scale.z;

	//	m_VtxMax.y = m_OriginVtxMax.y * m_Scale.y;
	//	m_VtxMin.y = m_OriginVtxMin.y * m_Scale.y;
	//	m_Size = m_VtxMax - m_VtxMin;
	//}
	//else if (m_RotType == ROTTYPE_Z)
	//{
	//	m_Rot.y = D3DX_PI / 2;
	//	m_Rot.x = 0.0f;
	//	m_Rot.z = 0.0f;

	//	m_VtxMax.x = m_OriginVtxMax.z * m_Scale.z;
	//	m_VtxMax.z = m_OriginVtxMax.x * m_Scale.x;

	//	m_VtxMin.x = m_OriginVtxMin.z * m_Scale.z;
	//	m_VtxMin.z = m_OriginVtxMin.x * m_Scale.x;

	//	m_VtxMax.y = m_OriginVtxMax.y * m_Scale.y;
	//	m_VtxMin.y = m_OriginVtxMin.y * m_Scale.y;

	//	m_Size = m_VtxMax - m_VtxMin;
	//}
}
//================================================================================================================================================

//===================================================================================================================
//位置を更新させる（位置を更新した後にブロックとの当たり判定をしたいので、任意のタイミングで呼べるようにする）
//===================================================================================================================
void CObjectX::UpdatePos()
{
	m_PosOld = m_Pos;   
	m_Pos += m_Move;   //位置の更新
	if (m_bUseInteria == true)
	{
		m_Move.x += (0.0f - m_Move.x) * m_fInertia;
		m_Move.z += (0.0f - m_Move.z) * m_fInertia;
	}
}
//================================================================================================================================================

//===================================================================================================================
//エディタから拡大率を変更する
//===================================================================================================================
void CObjectX::ChengeEditScale()
{
	D3DXVECTOR3& Scale = GetScale();//拡大率

	ChengeEditScaleX();

	ChengeEditScaleY();

	ChengeEditScaleZ();

	CManager::GetDebugText()->PrintDebugText("拡大率(YUIキー) %f %f %f\n", Scale.x,Scale.y,Scale.z);
}
//================================================================================================================================================

//===================================================================================================================
//エディタから拡大率Xを変更する
//===================================================================================================================
void CObjectX::ChengeEditScaleX()
{
	D3DXVECTOR3& Scale = GetScale();                                //拡大率

	if (CManager::GetInputKeyboard()->GetPress(DIK_LCONTROL) == true)
	{//Lコントロールキーを押しながら
		if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
		{//シフトキーを押しながら・・・
			if (CManager::GetInputKeyboard()->GetPress(DIK_Y) == true)
			{
				Scale.x -= 0.1f;
			}
		}
		else if (CManager::GetInputKeyboard()->GetPress(DIK_Y) == true)
		{
			Scale.x += 0.1f;
		}
	}
	else
	{//Lコントロールキーを押していない
		if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
		{//シフトキーを押しながら・・・
			if (CManager::GetInputKeyboard()->GetTrigger(DIK_Y) == true)
			{
				Scale.x -= 0.1f;
			}
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_Y) == true)
		{
			Scale.x += 0.1f;
		}
	}

}
//================================================================================================================================================

//===================================================================================================================
//エディタから拡大率Yを変更する
//===================================================================================================================
void CObjectX::ChengeEditScaleY()
{
	D3DXVECTOR3& Scale = GetScale();                                //拡大率

	if (CManager::GetInputKeyboard()->GetPress(DIK_LCONTROL) == true)
	{//Lコントロールキーを押しながら
		if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
		{//シフトキーを押しながら・・・
			if (CManager::GetInputKeyboard()->GetPress(DIK_U) == true)
			{
				Scale.y -= 0.1f;
			}
		}
		else if (CManager::GetInputKeyboard()->GetPress(DIK_U) == true)
		{
			Scale.y += 0.1f;
		}
	}
	else
	{//Lコントロールキーを押していない
		if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
		{//シフトキーを押しながら・・・
			if (CManager::GetInputKeyboard()->GetTrigger(DIK_U) == true)
			{
				Scale.y -= 0.1f;
			}
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_U) == true)
		{
			Scale.y += 0.1f;
		}
	}
}
//================================================================================================================================================

//===================================================================================================================
//エディタから拡大率Zを変更する
//===================================================================================================================
void CObjectX::ChengeEditScaleZ()
{
	D3DXVECTOR3& Scale = GetScale();                                //拡大率
	if (CManager::GetInputKeyboard()->GetPress(DIK_LCONTROL) == true)
	{//Lコントロールキーを押しながら
		if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
		{//シフトキーを押しながら・・・
			if (CManager::GetInputKeyboard()->GetPress(DIK_I) == true)
			{
				Scale.z -= 0.1f;
			}
		}
		else if (CManager::GetInputKeyboard()->GetPress(DIK_I) == true)
		{
			Scale.z += 0.1f;
		}
	}
	else
	{//Lコントロールキーを押していない
		if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
		{//シフトキーを押しながら・・・
			if (CManager::GetInputKeyboard()->GetTrigger(DIK_I) == true)
			{
				Scale.z -= 0.1f;
			}
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_I) == true)
		{
			Scale.z += 0.1f;
		}
	}

}
//================================================================================================================================================

//===================================================================================================================
//位置を移動させる
//===================================================================================================================
void CObjectX::ChengeEditPos()
{
	D3DXVECTOR3& Pos = GetPos();                                    //モデルの位置の取得
	D3DXVECTOR3& Scale = GetScale();                                //拡大率
	D3DXVECTOR3& SupportPos = GetSupportPos();                      //モデルの支点位置の取得
	D3DXVECTOR3& PosOld = GetPosOld();                              //1f前の位置を設定
	D3DXVECTOR3& Rot = GetRot();                                    //モデルの向きの取得
	D3DXVECTOR3 Size = GetSize();                                   //サイズを取得
	float fMoveX = 0.0f;                                            //X方向の移動量
	float& fInertia = GetInertia();                                 //慣性を取得
	float fMoveZ = 0.0f;                                            //Z方向の移動量
	bool bMove = false;                                             //移動しているかどうか 
	 
	SetColor(CObjectX::COLORTYPE_ALPHA, 10);                         //色を半透明にする

	//位置を支点に固定
	Pos = SupportPos;
	//=====================================================
    //移動処理
    //=====================================================
   	if (CManager::GetInputKeyboard()->GetPress(DIK_UP) == true)
	{
		fMoveZ = 1.0f;
	}
	else if (CManager::GetInputKeyboard()->GetPress(DIK_DOWN) == true)
	{
		fMoveZ = -1.0f;
	}

	if (CManager::GetInputKeyboard()->GetPress(DIK_RIGHT) == true)
	{
		fMoveX = 1.0f;
	}
	else if (CManager::GetInputKeyboard()->GetPress(DIK_LEFT) == true)
	{
		fMoveX = -1.0f;
	}

	//============================
	// 移動ボタンを押していたら
	//============================
	if (fMoveX != 0.0f || fMoveZ != 0.0f)
	{
		bMove = true;//移動状態
	}
	else
	{
		bMove = false;//待機状態
	}
	if (bMove == true)
	{//移動していたら
		Pos.x += sinf(atan2f(fMoveX, fMoveZ)) * 5.0f;
		Pos.y += cosf(atan2f(fMoveX, fMoveZ)) * 5.0f;
	}

	//===========================
	//RTボタンを押していたら
	//===========================
	if (CManager::GetInputKeyboard()->GetPress(DIK_LCONTROL) == true)
	{//Lコントロールキーを押しながら
		if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
		{//シフトキーを押しながら・・・
			if (CManager::GetInputKeyboard()->GetPress(DIK_F) == true)
			{
				Rot.z -= 0.01f;
			}
		}
		else if (CManager::GetInputKeyboard()->GetPress(DIK_G) == true)
		{
			Rot.z += 0.01f;
		}
	}
	else
	{//Lコントロールキーを押していない
		if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
		{//シフトキーを押しながら・・・
			if (CManager::GetInputKeyboard()->GetTrigger(DIK_F) == true)
			{
				Rot.z -= 0.01f;
			}
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_G) == true)
		{
			Rot.z += 0.01f;
		}
	}

	//支点も一緒に移動
	SupportPos = Pos;
	CManager::GetDebugText()->PrintDebugText("支点位置(矢印キー) %f %f %f\n", SupportPos.x, SupportPos.y, SupportPos.z);
	CManager::GetDebugText()->PrintDebugText("向きZ(FGキー) %f\n",Rot.z);
	//================================================================================================================================================

}
//================================================================================================================================================

//============================================================================
//ダメージを与える
//============================================================================
void CObjectX::SetDamage(int nDamage, int nHitStopTime)
{
	if (m_bHitStop == false && nDamage > 0)
	{//ヒットストップ状態じゃなければ
		m_bHitStop = true;              //ヒットストップ状態にする
		m_nHitStopTime = nHitStopTime;  //ヒットストップ時間
		for (int nCnt = 0; nCnt < 3; nCnt++)
		{
			float fRandSpeed = float(rand() % 100) / 10;//速さの乱数値
			float fRandRot = float(rand() % 628) / 100;  //発射向きの乱数値
			D3DXVECTOR3 Pos = CObjectX::GetPos();//位置を取得
			//CParticle::Create(50, 30.0f, 30.0f, Pos, D3DXVECTOR3(sinf(fRandRot) * fRandSpeed, cosf(fRandRot) * fRandSpeed, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		}
		m_nLife -= nDamage;
		SetColor(COLORTYPE_RED, 10);
	}
}
//================================================================================================================================================

//============================================================================
//回復量を割り当てる
//============================================================================
void CObjectX::SetHeal(int nHeal, D3DXCOLOR col, float fWidth, float fHeight)
{
	m_nLife += nHeal;
	CDamage::Create(nHeal, m_Pos, col, fWidth, fHeight);
}
//================================================================================================================================================

//============================================================================
//狙ったマテリアル番号のテクスチャを割り当てる
//============================================================================
void CObjectX::SetLockOnMatBindTexture(int nNumMat, LPDIRECT3DTEXTURE9 pTexture)
{
	m_ObjectXInfo.pTexture[nNumMat] = pTexture;
}
//================================================================================================================================================

//============================================================================
//影を描画する
//============================================================================
void CObjectX::DrawShadow(D3DXVECTOR3 Pos)
{
	D3DXMATRIX mtxShadow,mtxScale,mtxRot,mtxTrans;                  //計算用マトリックス
	D3DXPLANE plane;                       //プレーン
	D3DXVECTOR4 vecLight;                  //ライト逆方向
	D3DXVECTOR3 ShadowPos, ShadowNor;      //平面上の一転と
	D3DXMATERIAL* pMat;                                               //マテリアルデータへのポインタ
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice(); //デバイスへのポインタと取得
	D3DMATERIAL9 matDef;                                              //現在のマテリアル保存用

    //影のマトリックスを初期化
	D3DXMatrixIdentity(&mtxShadow);

	//ライトの逆方向ベクトルを設定
	vecLight = D3DXVECTOR4(-100.0f, 300.0f,300.0f, 0.0f);
	ShadowPos = NULL_VECTOR3;
	ShadowNor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//法線と平面上の一点から平面情報を作成
	D3DXPlaneFromPointNormal(&plane, &ShadowPos, &ShadowNor);

	//ライトの平面情報から影行列を作成
	D3DXMatrixShadow(&mtxShadow, &vecLight, &plane);

	//大きさを反映
	D3DXMatrixScaling(&mtxScale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixMultiply(&mtxShadow, &mtxShadow, &mtxScale);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot,m_Rot.y, m_Rot.x, m_Rot.z);
	//D3DXMatrixMultiply(&mtxShadow, &mtxShadow, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_Pos.x,0.0f, m_Pos.z);
	D3DXMatrixMultiply(&mtxShadow, &mtxShadow, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxShadow);

	//=======================================
    //描画の調整
    //=======================================

    //法線の長さを１にする。（スケールなどを使った時は、必ず使う。)
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
	//================================================================================================================

	//マテリアルへのポインタを取得
	pMat = (D3DXMATERIAL*)m_ObjectXInfo.pBuffMat->GetBufferPointer();

	//==========================================================================
	//マテリアルの数分、テクスチャを読み込む。
	//==========================================================================
	for (int nCntMat = 0; nCntMat < (int)m_ObjectXInfo.dwNumMat; nCntMat++)
	{
		//色合いの設定
		pMat[nCntMat].MatD3D.Diffuse = D3DXCOLOR(0.0f,0.0f,0.0f,1.0f);

		//マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		//テクスチャを設定する
		pDevice->SetTexture(0,NULL);

		//モデル（パーツ）の描画
		m_ObjectXInfo.pMesh->DrawSubset(nCntMat);
	}
	//================================================================================================================

	//=======================================
	//描画の調整を元に戻す
	//=======================================

	//法線の長さを１にする。（スケールなどを使った時は、必ず使う。)
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
	//================================================================================================================

	//保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}
//================================================================================================================================================

//============================================================================
//重力の処理
//============================================================================
void CObjectX::GravityProcess()
{
	if (m_bIsLanding == true)
	{
		m_Move.y = 0.0f;
	}
	if (m_bUseGravity == true)
	{
		m_Move.y += -m_fNORMAL_GRAVITY;
	}

}
//================================================================================================================================================

//============================================================================
//原点に到達したときに位置の補正を行うかどうか
//============================================================================
void CObjectX::AdjusOriginPosProcess()
{
	//if (m_Pos.y + m_VtxMin.y <= 0.0f && m_bUseAdjustOriginPos == true && CManager::GetCamera()->GetCameraType() == CCamera::CAMERATYPE_BOSSBATTLE)
	//{
	//	m_bIsLanding = true;
	//	m_Pos.y = fabsf(GetVtxMin().y);
	//}
}
//================================================================================================================================================

//============================================================================
//情報表示処理
//============================================================================
void CObjectX::DispInfo()
{
	ChengeEditPos();//位置を変える

	ChengeEditScale();//拡大率を変える
}
//================================================================================================================================================
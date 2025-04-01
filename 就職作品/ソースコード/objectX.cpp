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
#include "calculation.h"
#include "manager.h"
#include "camera.h"
#include "input.h"
#include "debugtext.h"
#include <stdio.h>
#include <string.h>
#include <iostream>
#include "collision.h"
#include "particle.h"
#include "damage.h"
#include <algorithm>
//===========================================================================================

//================================================
//静的メンバ宣言
//================================================
bool CObjectX::s_bCOMMON_DRAWSHADOW = true;

//================================================
//コンストラクタ
//================================================
CObjectX::CObjectX(int nPri, bool bUseintPri, CObject::TYPE type, CObject::OBJECTTYPE ObjType) : CObject(nPri, bUseintPri, type, ObjType)
, m_ObjectXInfo(), m_nIndexObjectX(0), m_nManagerType(0), m_nObjXType(OBJECTXTYPE_BLOCK), m_fAxis(0.0f), m_VecAxis(D3DXVECTOR3(0.0f, 1.0f, 0.0f)),
m_nTypeNum(0), m_PosInfo({}), m_DrawInfo({}), m_RotInfo({}), m_SizeInfo({}), m_MoveInfo({}), m_LifeInfo({}), m_CollisionInfo({}),
m_bExtrusionCollisionSquareX(false),m_bExtrusionCollisionSquareY(false),m_bExtrusionCollisionSquareZ(false),m_bIsLanding(false)
{
	SetObjectType(CObject::OBJECTTYPE::OBJECTTYPE_X);//オブジェクトタイプ設定
	D3DXMatrixIdentity(&m_DrawInfo.GetMatrixWorld());        //ワールドマトリックスを初期化
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
	CObject::Init();//オブジェクトの初期化処理

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
		delete[] m_ObjectXInfo.FormarDiffuse;//元の色合いの動的メモリを開放
		m_ObjectXInfo.FormarDiffuse = nullptr;
	}

	if (m_ObjectXInfo.pMesh != nullptr)
	{//メッシュへのポインタを初期化
		m_ObjectXInfo.pMesh = nullptr;
	}

	if (m_ObjectXInfo.pBuffMat != nullptr)
	{//マテリアルへのポインタを初期化
		m_ObjectXInfo.pBuffMat = nullptr;
	}

	CObject::Uninit();//オブジェクトの終了処理
}
//================================================================================================================================================

//================================================
//更新処理
//================================================
void CObjectX::Update()
{
	CalculateSenterPos();                        //モデルの中心点を求める
										         
	m_SizeInfo.AddScaleProcess();                //拡大率加算処理
	m_SizeInfo.MultiScaleProcess();              //拡大率乗算処理
	m_SizeInfo.DecideVtxMaxMinProcess();         //頂点の最大最小を決める処理
										         
	m_DrawInfo.ChengeColorProcess(this);         //色を変える処理
	if (m_RotInfo.GetUseAddRot() == true)
	{//向きの加算処理
		m_RotInfo.Rot += m_RotInfo.AddRot;
	}

	m_LifeInfo.AutoSubLifeProcess();             //体力を自動的に減らす処理
	m_LifeInfo.AutoDeathProcess(this);           //体力が０になった場合に死亡フラグを発動する処理
	m_LifeInfo.HitStopProcess();                 //無敵時間をカウントする処理
	m_LifeInfo.RatioLifeAlphaColorProcess(this); //体力の割合に応じて透明度を変える処理

	m_MoveInfo.MultiSpeedProcess();              //乗算加速処理
	m_MoveInfo.AddSpeedProcess();                //加速処理
	m_MoveInfo.GravityProcess();                 //重力処理

	UpdatePos();                                 //位置の更新を行う

	CObject::Update();                           //オブジェクト更新処理
}
//================================================================================================================================================

//================================================
//描画処理
//================================================
void CObjectX::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice(); //デバイスへのポインタと取得
	D3DXMATRIX mtxRot, mtxTrans, mtxScale;                            //計算用マトリックス
	D3DXQUATERNION quat;                                              //クォータニオン
	D3DXVECTOR3 vecAxis = D3DXVECTOR3(0.0f, 0.0, 1.0f);               //回転軸
	D3DMATERIAL9 matDef;                                              //現在のマテリアル保存用
	D3DXMATERIAL* pMat;                                               //マテリアルデータへのポインタ

	if (m_DrawInfo.bUseShadow == true && s_bCOMMON_DRAWSHADOW == true)
	{//影を描画するなら
		DrawShadow();
	}
	//現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_DrawInfo.mtxWorld);

	//大きさを反映
	D3DXMatrixScaling(&mtxScale, m_SizeInfo.Scale.x, m_SizeInfo.Scale.y, m_SizeInfo.Scale.z);
	D3DXMatrixMultiply(&m_DrawInfo.mtxWorld, &m_DrawInfo.mtxWorld, &mtxScale);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_RotInfo.Rot.y, m_RotInfo.Rot.x, m_RotInfo.Rot.z);
	D3DXMatrixMultiply(&m_DrawInfo.mtxWorld, &m_DrawInfo.mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_PosInfo.Pos.x, m_PosInfo.Pos.y, m_PosInfo.Pos.z);
	D3DXMatrixMultiply(&m_DrawInfo.mtxWorld, &m_DrawInfo.mtxWorld, &mtxTrans);

	//親マトリックスが設定されていた場合
	if (m_DrawInfo.bUseMatrixChild == true && m_DrawInfo.pMtxParent != nullptr)
	{
		D3DXMatrixMultiply(&m_DrawInfo.mtxWorld, &m_DrawInfo.mtxWorld,m_DrawInfo.pMtxParent);
	}

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_DrawInfo.mtxWorld);

	//ワールド座標を求める
	D3DXVECTOR3 PosZero = { 0.0f,0.0f,0.0f };
	D3DXVec3TransformCoord(&m_PosInfo.WorldPos, &PosZero, &m_DrawInfo.mtxWorld);


	//=======================================
	//描画の調整
	//=======================================
	if (m_DrawInfo.Color.a < 1.0f)
	{
    	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);//TRUEだと透明度と背景のブレンドが行われる、FALSEだと完全に不透明な描画になる
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);     //描画するオブジェクトの色の影響度を決める,D3DBLEND_SRCALPHA：(R,G,B) * a(オブジェクトの透明度に応じた色)
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA); //すでに描画されている背景色の影響度を決める,D3DBLEND_INVSRCALPHA:(R,G,B)×(1 - a)<背景の色を残す割合>
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);             //奥行情報を書き込まない
	}
	else
	{//不透明オブジェクトを通常通り描画する
	//アルファテストを有効(アルファ値が０より大きい場合に描画する）
		pDevice->SetRenderState(D3DRS_ALPHAREF, 0);                     //透明度が0以上の時に描画
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);       //透明度が０より大きいピクセルのみ描画
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);              //Zバッファに書き込む
	}

	//法線の長さを１にする。（スケールなどを使った時は、必ず使う。)
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

	if (m_DrawInfo.bUseCulling == true)
	{
		//両面を描画する
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	}
	//================================================================================================================

	if (m_DrawInfo.bUseDraw == true)
	{//描画をするなら
		if (m_ObjectXInfo.pBuffMat != nullptr && m_ObjectXInfo.pMesh != nullptr)
		{//マテリアルとメッシュが存在していたら
			//マテリアルへのポインタを取得
			pMat = (D3DXMATERIAL*)m_ObjectXInfo.pBuffMat->GetBufferPointer();

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
	}

	//=======================================
	//描画の調整を元に戻す
	//=======================================
	//アルファテスト無効に戻す
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	//法線の長さを１にする。（スケールなどを使った時は、必ず使う。)
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);

	//Zバッファに書き込む
    pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	//片面だけ描画する
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	//================================================================================================================

	//保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);

	CObject::Draw();//オブジェクトの描画処理

}
//================================================================================================================================================

//================================================
//死亡フラグ設定処理
//================================================
void CObjectX::SetDeath()
{
	CObject::SetDeath();//オブジェクトの死亡フラグ設定処理
}
//================================================================================================================================================

//================================================
//モデル情報を割り当てる
//================================================
void CObjectX::BindObjectXInfo(LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD dwNumMat, LPDIRECT3DTEXTURE9* pTexture, D3DCOLORVALUE* pDiffuse)
{
	m_ObjectXInfo.pMesh = pMesh;                                       //頂点情報の設定
	m_ObjectXInfo.pBuffMat = pBuffMat;                                 //マテリアルの設定
	m_ObjectXInfo.dwNumMat = dwNumMat;                                 //マテリアルの数を設定
	m_ObjectXInfo.pTexture = DBG_NEW LPDIRECT3DTEXTURE9[int(dwNumMat)];//マテリアルの動的メモリを確保
	m_ObjectXInfo.Diffuse = DBG_NEW D3DCOLORVALUE[int(dwNumMat)];      //マテリアルの動的メモリを確保
	m_ObjectXInfo.FormarDiffuse = DBG_NEW D3DCOLORVALUE[int(dwNumMat)];//元のマテリアルの動的メモリを確保
	for (int nCnt = 0; nCnt < (int)(dwNumMat); nCnt++)
	{//マテリアルの数分それぞれの関連情報を設定
		m_ObjectXInfo.pTexture[nCnt] = pTexture[nCnt];
		m_ObjectXInfo.Diffuse[nCnt] = pDiffuse[nCnt];
		m_ObjectXInfo.FormarDiffuse[nCnt] = pDiffuse[nCnt];
	}
}
//================================================================================================================================================

//================================================
//色合いを設定する
//================================================
void CObjectX::SetColor(D3DXCOLOR col, int nColChengeTime, bool bChoose, bool bSetAlpha, bool bBlinking)
{
	m_DrawInfo.bColorChenge = true;               //色を変えるかどうか
	m_DrawInfo.nChengeColorTime = nColChengeTime; //色を変える時間
	m_DrawInfo.Color = col;                       //色
	m_DrawInfo.bBlinkingColor = bBlinking;        //点滅
	if (bChoose == true)
	{//透明度だけを使用するかを選ぶ
		if (bSetAlpha == false)
		{//透明度以外を設定
			for (int nCnt = 0; nCnt < (int)(m_ObjectXInfo.dwNumMat); nCnt++)
			{
				m_ObjectXInfo.Diffuse[nCnt].r = col.r;
				m_ObjectXInfo.Diffuse[nCnt].g = col.g;
				m_ObjectXInfo.Diffuse[nCnt].b = col.b;
			}
		}
		else
		{
			for (int nCnt = 0; nCnt < (int)(m_ObjectXInfo.dwNumMat); nCnt++)
			{//透明度だけを設定

				m_ObjectXInfo.Diffuse[nCnt].a = col.a;
			}
		}
	}
	else
	{//普通に色を設定する
		for (int nCnt = 0; nCnt < (int)(m_ObjectXInfo.dwNumMat); nCnt++)
		{//色合いを設定する
			m_ObjectXInfo.Diffuse[nCnt] = col;
		}
	}
}
//================================================================================================================================================

//================================================
//ダメージを与える処理
//================================================
void CObjectX::SetDamage(int nDamage, int nHitStopTime)
{
	if (m_LifeInfo.bHitStop == false && nDamage > 0)
	{//ヒットストップ状態じゃなければ
		m_LifeInfo.bHitStop = true;              //ヒットストップ状態にする
		m_LifeInfo.nHitStopTime = nHitStopTime;  //ヒットストップ時間
		CDamage::Create(nDamage, GetPosInfo().GetSenterPos(), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 30.0f + nDamage * 1.0f, 30.0f + nDamage * 1.0f);//ダメージの生成
		m_LifeInfo.nLife -= nDamage;//体力を減らす
	}
}
//================================================================================================================================================

//================================================
//中心点を求める
//================================================
void CObjectX::CalculateSenterPos()
{
	m_PosInfo.SenterPos = m_PosInfo.Pos;//中心点を求める
	m_PosInfo.SenterPos.y += (m_SizeInfo.VtxMax.y + m_SizeInfo.VtxMin.y) / 2;//最大頂点と最小頂点の距離を求め、その半分を足す
}
//================================================================================================================================================

//================================================
//元の色合いに戻す
//================================================
void CObjectX::SetFormarColor()
{
	m_DrawInfo.Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//色合いを1にする
	for (int nCnt = 0; nCnt < (int)(m_ObjectXInfo.dwNumMat); nCnt++)
	{//最初に保存した色合いに現在の色合いを設定
		m_ObjectXInfo.Diffuse[nCnt] = m_ObjectXInfo.FormarDiffuse[nCnt];
	}
}
//================================================================================================================================================

//================================================
//色合い「だけ」を設定する（SetColorにある機能を使わない)
//================================================
void CObjectX::SetOnlyColor(D3DXCOLOR Col)
{
	for (int nCnt = 0; nCnt < (int)(m_ObjectXInfo.dwNumMat); nCnt++)
	{
		m_ObjectXInfo.Diffuse[nCnt] = m_DrawInfo.Color;
	}
}
//================================================================================================================================================

//================================================
//元の色合いに「だけ」設定する（SetColorにある機能を使わない)
//================================================
void CObjectX::SetOnlyFormarColor()
{
	for (int nCnt = 0; nCnt < (int)(m_ObjectXInfo.dwNumMat); nCnt++)
	{
		m_ObjectXInfo.Diffuse[nCnt] = m_ObjectXInfo.FormarDiffuse[nCnt];
	}
}
//================================================================================================================================================

//================================================
//位置の更新
//================================================
void CObjectX::UpdatePos()
{
	if (m_MoveInfo.bUseUpdatePos == true)
	{//位置の更新をするなら
		const D3DXVECTOR3& Pos = GetPosInfo().GetPos();

		//1f前の位置を設定
		GetPosInfo().SetPosOld(Pos);

		//慣性の処理
		if (m_MoveInfo.bUseInteria == true)
		{
			m_MoveInfo.Move.x += (0.0f - m_MoveInfo.Move.x) * m_MoveInfo.fInertia;
			m_MoveInfo.Move.z += (0.0f - m_MoveInfo.Move.z) * m_MoveInfo.fInertia;
		}

		//位置の設定
		GetPosInfo().SetPos(Pos + m_MoveInfo.Move);

		//1f後の位置の設定
		GetPosInfo().SetPosFuture(Pos + m_MoveInfo.Move);
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
		if (vtx.x > m_SizeInfo.OriginVtxMax.x)
		{
			m_SizeInfo.OriginVtxMax.x = vtx.x;
		}
		else if (vtx.y > m_SizeInfo.OriginVtxMax.y)
		{//今回読み込んだ頂点が、一番大きい頂点より大きい場合
			m_SizeInfo.OriginVtxMax.y = vtx.y;
		}
		else if (vtx.z > m_SizeInfo.OriginVtxMax.z)
		{//今回読み込んだ頂点が、一番大きい頂点より大きい場合
			m_SizeInfo.OriginVtxMax.z = vtx.z;
		}
		else if (vtx.x < m_SizeInfo.OriginVtxMin.x)
		{//今回読み込んだ頂点が、一番小さい頂点より小さい場合
			m_SizeInfo.OriginVtxMin.x = vtx.x;
		}
		else if (vtx.y < m_SizeInfo.OriginVtxMin.y)
		{//今回読み込んだ頂点が、一番小さい頂点より小さい場合
			m_SizeInfo.OriginVtxMin.y = vtx.y;
		}
		else if (vtx.z < m_SizeInfo.OriginVtxMin.z)
		{//今回読み込んだ頂点が、一番小さい頂点より小さい場合
			m_SizeInfo.OriginVtxMin.z = vtx.z;
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
	m_SizeInfo.VtxMax.x = m_SizeInfo.OriginVtxMax.x * m_SizeInfo.Scale.x;
	m_SizeInfo.VtxMax.y = m_SizeInfo.OriginVtxMax.y * m_SizeInfo.Scale.y;
	m_SizeInfo.VtxMax.z = m_SizeInfo.OriginVtxMax.z * m_SizeInfo.Scale.z;
	m_SizeInfo.VtxMin.x = m_SizeInfo.OriginVtxMin.x * m_SizeInfo.Scale.x;
	m_SizeInfo.VtxMin.y = m_SizeInfo.OriginVtxMin.y * m_SizeInfo.Scale.y;
	m_SizeInfo.VtxMin.z = m_SizeInfo.OriginVtxMin.z * m_SizeInfo.Scale.z;
	//================================================================================================================================================

	//==========================================================
	//最大最小を参照してサイズを設定する
	//==========================================================
	m_SizeInfo.Size = m_SizeInfo.VtxMax - m_SizeInfo.VtxMin;
	//================================================================================================================================================

}
//================================================================================================================================================

//===================================================================================================================
//エディタから拡大率を変更する
//===================================================================================================================
void CObjectX::ChengeEditScale()
{
	D3DXVECTOR3& Scale = m_SizeInfo.GetScale();//拡大率

	ChengeEditScaleX();//拡大率X

	ChengeEditScaleY();//拡大率Y

	ChengeEditScaleZ();//拡大率Z

	//デバッグ表示
	CManager::GetDebugText()->PrintDebugText("拡大率(RTYキー) %f %f %f\n", Scale.x,Scale.y,Scale.z);
}
//================================================================================================================================================

//===================================================================================================================
//エディタから拡大率Xを変更する
//===================================================================================================================
void CObjectX::ChengeEditScaleX()
{
	D3DXVECTOR3& Scale = m_SizeInfo.GetScale();                                  //拡大率

	if (CManager::GetInputKeyboard()->GetPress(DIK_LCONTROL) == true)
	{//Lコントロールキーを押しながら（値を大きく変動させる)
		if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
		{//シフトキーを押しながら・・・
			if (CManager::GetInputKeyboard()->GetTrigger(DIK_R) == true)
			{
				Scale.x -= 10.0f;
			}
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_R) == true)
		{
			Scale.x += 10.0f;
		}
	}
	else
	{//Lコントロールキーを押していない（値を小さく変動させる)
		if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
		{//シフトキーを押しながら・・・
			if (CManager::GetInputKeyboard()->GetTrigger(DIK_R) == true)
			{
				Scale.x -= 0.1f;
			}
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_R) == true)
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
	D3DXVECTOR3& Scale = m_SizeInfo.GetScale();                                //拡大率

	if (CManager::GetInputKeyboard()->GetPress(DIK_LCONTROL) == true)
	{//Lコントロールキーを押しながら（値を大きく変動させる)
		if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
		{//シフトキーを押しながら・・・
			if (CManager::GetInputKeyboard()->GetTrigger(DIK_T) == true)
			{
				Scale.y -= 10.0f;
			}
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_T) == true)
		{
			Scale.y += 10.0f;
		}
	}
	else
	{//Lコントロールキーを押していない（値を小さく変動させる)
		if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
		{//シフトキーを押しながら・・・
			if (CManager::GetInputKeyboard()->GetTrigger(DIK_T) == true)
			{
				Scale.y -= 0.1f;
			}
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_T) == true)
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
	D3DXVECTOR3& Scale = m_SizeInfo.GetScale();                                //拡大率
	if (CManager::GetInputKeyboard()->GetPress(DIK_LCONTROL) == true)
	{//Lコントロールキーを押しながら（値を大きく変動させる)
		if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
		{//シフトキーを押しながら・・・
			if (CManager::GetInputKeyboard()->GetTrigger(DIK_Y) == true)
			{
				Scale.z -= 10.0f;
			}
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_Y) == true)
		{
			Scale.z += 10.0f;
		}
	}
	else
	{//Lコントロールキーを押していない（値を小さく変動させる)
		if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
		{//シフトキーを押しながら・・・
			if (CManager::GetInputKeyboard()->GetTrigger(DIK_Y) == true)
			{
				Scale.z -= 0.1f;
			}
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_Y) == true)
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
	D3DXVECTOR3 Move = D3DXVECTOR3(0.0f,0.0f,0.0f);
	SetColor(D3DXCOLOR(1.0f,0.0f,0.0f,0.5f),3,true,true,false);           //色を半透明にする
	CInputKeyboard* pInput = CManager::GetInputKeyboard();
	//===========================
	//位置を支点に固定
	//===========================
	m_PosInfo.Pos = m_PosInfo.SupportPos;
	//========================================================================================

	//===========================
	//位置を変更
	//===========================
	if (pInput->GetPress(DIK_LSHIFT) == true)
	{//Y軸移動
		if (pInput->GetPress(DIK_LCONTROL))
		{
			if (pInput->GetTrigger(DIK_W) == true)
			{
				m_PosInfo.Pos.y += 0.1f;
			}
			else if (pInput->GetTrigger(DIK_S) == true)
			{
				m_PosInfo.Pos.y -= 0.1f;
			}
		}
		else
		{
			if (pInput->GetPress(DIK_W) == true)
			{
				m_PosInfo.Pos.y += 5.0f;
			}
			else if (pInput->GetPress(DIK_S) == true)
			{
				m_PosInfo.Pos.y -= 5.0f;
			}
		}
	}
	else
	{//XZ平面移動
		CCalculation::CaluclationMove(false,m_PosInfo.Pos,Move,5.0f, CCalculation::MOVEAIM_XZ, m_RotInfo.Rot.y);
	}

	//支点も一緒に移動
	m_PosInfo.Pos += Move;
	m_PosInfo.SupportPos = m_PosInfo.Pos;

	//デバッグ表示
	CManager::GetDebugText()->PrintDebugText("支点位置(矢印キー) %f %f %f\n", m_PosInfo.SupportPos.x,m_PosInfo.SupportPos.y, m_PosInfo.SupportPos.z);
	CManager::GetDebugText()->PrintDebugText("移動量 ： %f %f %f\n", Move.x, Move.y, Move.z);
	CManager::GetDebugText()->PrintDebugText("向きZ(FGキー) %f\n", m_RotInfo.Rot.z);
	CManager::GetCamera()->SetPosR(m_PosInfo.Pos);//注視点を現在操作しているモデルに固定
	//================================================================================================================================================

}
//================================================================================================================================================

//===================================================================================================================
//位置を移動させる
//===================================================================================================================
void CObjectX::EditLife()
{
	if (CManager::GetInputKeyboard()->GetPress(DIK_LCONTROL) == true)
	{//Lコントロールキーを押しながら（体力を早く増やす)
		if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
		{//シフトキーを押しながら・・・
			if (CManager::GetInputKeyboard()->GetPress(DIK_3) == true)
			{
				m_LifeInfo.nMaxLife++;
			}
		}
		else if (CManager::GetInputKeyboard()->GetPress(DIK_3) == true)
		{
			m_LifeInfo.nMaxLife--;
		}
	}
	else
	{//Lコントロールキーを押していない（体力を少しずつ増やす)
		if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
		{//シフトキーを押しながら・・・
			if (CManager::GetInputKeyboard()->GetTrigger(DIK_3) == true)
			{
				m_LifeInfo.nMaxLife++;
			}
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_3) == true)
		{
			m_LifeInfo.nMaxLife--;
		}
	}

	//デバッグ表示
	CManager::GetDebugText()->PrintDebugText("最大体力(3)：%d\n", m_LifeInfo.nMaxLife);
}
//================================================================================================================================================

//============================================================================
//最大頂点と最小頂点を入れ替えるかどうか
//============================================================================
void CObjectX::ChengeEditSwapVtxXZ()
{
	CInputKeyboard* pInputKeyBoard = CManager::GetInputKeyboard();//キー入力情報を取得

	if (pInputKeyBoard->GetTrigger(DIK_7))
	{//XZの頂点を変更
		m_SizeInfo.bSwapVtxXZ = m_SizeInfo.bSwapVtxXZ ? false : true;
	}

	//デバッグ表示
	CManager::GetDebugText()->PrintDebugText("頂点のXZを入れ替えるかどうか（７）：%d\n", m_SizeInfo.bSwapVtxXZ);
}
//================================================================================================================================================

//============================================================================
//ステージマネージャーが情報を操作する
//============================================================================
void CObjectX::ManagerChooseControlInfo()
{
	ChengeEditPos();                              //位置を変える
						                          
	ChengeEditScale();                            //拡大率を変える
						                          
	ChengeEditSwapVtxXZ();                        //XZの頂点を変える
						                          
	EditLife();                                   //体力を変更する

	CManager::GetCamera()->SetPosR(m_PosInfo.Pos);//カメラの注視点を現在の位置に設定
}
//================================================================================================================================================

//============================================================================
//テキストファイルに情報を保存する
//============================================================================
void CObjectX::SaveInfoTxt(fstream & WritingFile)
{
	WritingFile << "SETOBJECTX" << endl;//読み込み開始用テキスト

	WritingFile << "POS = " << fixed << setprecision(3)<< m_PosInfo.Pos.x << " " <<
		fixed << setprecision(3) << m_PosInfo.Pos.y << " " << 
		fixed << setprecision(3) << m_PosInfo.Pos.z << " " << endl;//位置（小数点３）まで
	WritingFile << "ROT = " << fixed << setprecision(3) << m_RotInfo.Rot.x << " " <<
		fixed << setprecision(3) << m_RotInfo.Rot.y << " " <<
		fixed << setprecision(3) << m_RotInfo.Rot.z << " " << endl;//向き（小数点３）まで
	WritingFile << "SCALE = " << fixed << setprecision(3) << m_SizeInfo.Scale.x << " " <<
		fixed << setprecision(3) << m_SizeInfo.Scale.y << " " <<
		fixed << setprecision(3) << m_SizeInfo.Scale.z << " " << endl;//拡大率（小数点３）まで

	WritingFile << "MOVE = " << (m_MoveInfo.Move.x) << " " << m_MoveInfo.Move.y << " " << m_MoveInfo.Move.z << endl;//移動量

	WritingFile << "LIFE = " << m_LifeInfo.nMaxLife << endl;//体力を設定

	WritingFile << "SWAPVTXXZ = " << m_SizeInfo.bSwapVtxXZ << endl;//頂点のXZを変更するかどうか

	WritingFile << "END_SETOBJECTX" << endl;//読み込み終了用テキスト
}
//================================================================================================================================================

//============================================================================
//テキストファイルから情報を読み込む
//============================================================================
void CObjectX::LoadInfoTxt(fstream& LoadingFile, list<CObject*>& listSaveManager, string& Buff, CObject* pObj)
{
	int nLife = 0;                                   //体力格納用
	int nMaxLife = 0;                                //最大体力格納用
	D3DXVECTOR3 Pos = { 0.0f,0.0f,0.0f };            //位置格納用
	D3DXVECTOR3 Move = { 0.0f,0.0f,0.0f };           //移動量格納用
	D3DXVECTOR3 Rot = { 0.0f,0.0f,0.0f };            //向き格納用
	D3DXVECTOR3 Scale = { 0.0f,0.0f,0.0f };          //拡大率格納用
	bool bSwapVtxXZ = false;                         //XZ座標を入れ替えるかどうか格納用

	CObjectX* pObjX = dynamic_cast<CObjectX*>(pObj); //オブジェクトXんｋダウンキャスト

	if (pObjX != nullptr)
	{//オブジェクトXへのポインタが存在している

		while (Buff != "END_SETOBJECTX")
		{
			LoadingFile >> Buff;           //単語を読み込む
			if (Buff == "#")
			{//行をスキップ
				getline(LoadingFile, Buff);
			}

			else if (Buff == "LIFE")
			{
				LoadingFile >> Buff;       //イコール
				LoadingFile >> nMaxLife;   //最大体力
				nLife = nMaxLife;          //体力（初期値は最大体力と同じ)
			}
			else if (Buff == "MOVE")
			{
				LoadingFile >> Buff;       //イコール
				LoadingFile >> Move.x;     //移動量X
				LoadingFile >> Move.y;     //移動量Y
				LoadingFile >> Move.z;     //移動量Z
			}
			else if (Buff == "POS")
			{
				LoadingFile >> Buff;       //イコール
				LoadingFile >> Pos.x;      //位置X
				LoadingFile >> Pos.y;      //位置Y
				LoadingFile >> Pos.z;      //位置Z
			}
			else if (Buff == "ROT")
			{
				LoadingFile >> Buff;       //イコール
				LoadingFile >> Rot.x;      //位置X
				LoadingFile >> Rot.y;      //位置Y
				LoadingFile >> Rot.z;      //位置Z
			}
			else if (Buff == "SCALE")
			{
				LoadingFile >> Buff;       //イコール
				LoadingFile >> Scale.x;    //拡大率X
				LoadingFile >> Scale.y;    //拡大率Y
				LoadingFile >> Scale.z;    //拡大率Z
			}
			else if (Buff == "SWAPVTXXZ")
			{
				LoadingFile >> Buff;       //イコール
				LoadingFile >> bSwapVtxXZ; //XZの頂点を入れ替えるかどうか
			}
		}

		PosInfo& ObjXPosInfo = GetPosInfo();           //位置情報を取得
		MoveInfo& ObjXMoveInfo = GetMoveInfo();        //移動情報を取得
		RotInfo& ObjXRotInfo = GetRotInfo();           //向き情報を取得
		SizeInfo& ObjXSizeInfo = GetSizeInfo();        //サイズ情報を取得
		LifeInfo& ObjXLifeInfo = GetLifeInfo();        //体力情報を取得

		ObjXPosInfo.SetPos(Pos);                       //位置を設定
		ObjXPosInfo.SetSupportPos(Pos);                //最初の位置を設定

		ObjXMoveInfo.SetMove(Move);                    //移動量を設定

		ObjXRotInfo.SetRot(Rot);                       //向きを設定

		ObjXSizeInfo.SetScale(Scale);                  //拡大率を設定
		ObjXSizeInfo.SetUseSwapVtxXZ(bSwapVtxXZ);      //頂点のXZを入れ替えるかどうかを設定

		ObjXLifeInfo.SetLife(nLife);                   //体力を設定
		ObjXLifeInfo.SetMaxLife(nMaxLife);             //最大体力を設定
	}
	else
	{//例外処理
		assert("オブジェクトXへのポインタが存在していない");
	}
}
//================================================================================================================================================

//============================================================================
//狙ったマテリアル番号のテクスチャを割り当てる
//============================================================================
void CObjectX::SetLockOnMatBindTexture(int nNumMat, LPDIRECT3DTEXTURE9 pTexture)
{
	m_ObjectXInfo.pTexture[nNumMat] = pTexture;//指定した配列番号のテクスチャを変える
}
//================================================================================================================================================

//============================================================================
//影を描画する
//============================================================================
void CObjectX::DrawShadow()
{
	D3DXMATRIX mtxShadow,mtxScale,mtxRot,mtxTrans;                    //計算用マトリックス
	D3DXPLANE plane;                                                  //プレーン
	D3DXVECTOR4 vecLight;                                             //ライト逆方向
	D3DXVECTOR3 ShadowPos, ShadowNor;                                 //影の位置と法線
	D3DXMATERIAL* pMat;                                               //マテリアルデータへのポインタ
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice(); //デバイスへのポインタと取得
	D3DMATERIAL9 matDef;                                              //現在のマテリアル保存用

	D3DXVECTOR3 RayCollisionPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);      //レイが当たった位置格納用
	D3DXVECTOR3 CalcRayCollisionPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  //レイが当たった一番近い位置格納用
	float fResultNearLength = 0.0f;

	//現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

    //影のマトリックスを初期化
	D3DXMatrixIdentity(&mtxShadow);

	//ライトの逆方向ベクトルを設定
	vecLight = D3DXVECTOR4(-100.0f, 300.0f,300.0f, 0.0f);

	ShadowPos = D3DXVECTOR3(0.0f,0.0f,0.0f);   //影の位置
	ShadowNor = D3DXVECTOR3(0.0f, 1.0f, 0.0f); //影の法線（Yが1.0fなので、水平なポリゴン)


	//法線と平面上の一点から平面情報を作成
	D3DXPlaneFromPointNormal(&plane, &ShadowPos, &ShadowNor);

	//ライトの平面情報から影行列を作成
	D3DXMatrixShadow(&mtxShadow, &vecLight, &plane);

	//大きさを反映
	D3DXMatrixScaling(&mtxScale, m_SizeInfo.Scale.x, m_SizeInfo.Scale.y, m_SizeInfo.Scale.z);
	D3DXMatrixMultiply(&mtxShadow, &mtxShadow, &mtxScale);

	//回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_RotInfo.Rot.y, 0.0f, 0.0f);
	D3DXMatrixMultiply(&mtxShadow, &mtxShadow, &mtxRot);

	//==================================================================================
	//下にレイを飛ばし、当たったオブジェクトの中で一番近いオブジェクトの位置を求める
	//==================================================================================
	int nCntColRay = 0;//レイが当たった回数をカウントする
	for (int nCntPri = 0; nCntPri < CObject::m_nMAXPRIORITY; nCntPri++)
	{//オブジェクトリストを検索する
		CObject* pObj = CObject::GetTopObject(nCntPri);//リストの先頭オブジェクトを取得する

		while (pObj != nullptr)
		{
			CObject* pNext = pObj->GetNextObject();//次のオブジェクトを取得する

			if (pObj->GetType() == CObject::TYPE::BLOCK || pObj->GetType() == CObject::TYPE::BGMODEL)
			{//オブジェクトタイプが「ブロック」又は「背景モデル」だったら
				CObjectX* pObjX = static_cast<CObjectX*>(pObj);//オブジェクトXにキャスト
				if (CCollision::RayIntersectsAABBCollisionPos(m_PosInfo.WorldPos + D3DXVECTOR3(0.0f,0.1f,0.0f), D3DXVECTOR3(0.0f, -1.0f, 0.0f),
					pObjX->GetPosInfo().GetPos() + pObjX->m_SizeInfo.GetVtxMin(),pObjX->GetPosInfo().GetPos() + pObjX->m_SizeInfo.GetVtxMax(), CalcRayCollisionPos))
				{//レイが当たっていたら
					float fLength = sqrtf(powf(CalcRayCollisionPos.y - m_PosInfo.WorldPos.y,2));//レイが当たった位置のY軸の距離を取る
					nCntColRay++;                                                               //レイが当たったオブジェクト総数をインクリメント
					if (nCntColRay == 1)
					{//最初の当たったオブジェクトなので、無条件で距離とレイが当たった位置を記録する
						fResultNearLength = fLength;          
						RayCollisionPos = CalcRayCollisionPos;
					}
					else
					{
						if (fResultNearLength > fLength)
						{//レイが当たったオブジェクトの中で一番近いので、距離とレイが当たった位置を更新する
							fResultNearLength = fLength;
							RayCollisionPos = CalcRayCollisionPos;
						}
					}
				}
			}

			pObj = pNext;//リストを次に進める
		}
	}
	//==================================================================================================================================================================

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans,m_PosInfo.WorldPos.x,RayCollisionPos.y, m_PosInfo.WorldPos.z);
	D3DXMatrixMultiply(&mtxShadow, &mtxShadow, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxShadow);

	//=======================================
    //描画の調整
    //=======================================

    //法線の長さを１にする。（スケールなどを使った時は、必ず使う。)
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
	//================================================================================================================

	if (GetDrawInfo().GetUseDraw() == true)
	{
		if (m_ObjectXInfo.pMesh != nullptr && m_ObjectXInfo.pBuffMat != nullptr)
		{
			CObject::TYPE Type = GetType();

			//マテリアルへのポインタを取得
			pMat = (D3DXMATERIAL*)m_ObjectXInfo.pBuffMat->GetBufferPointer();

			//==========================================================================
			//マテリアルの数分、テクスチャを読み込む。
			//==========================================================================
			for (int nCntMat = 0; nCntMat < (int)m_ObjectXInfo.dwNumMat; nCntMat++)
			{
				if (m_ObjectXInfo.Diffuse[nCntMat].a > 0.5f)
				{//a値が0.5f異常なら0.5fに固定
					pMat[nCntMat].MatD3D.Diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);
				}
				else
				{//a値が0.5f以下なら全体色合いのデータから直接a値を参照
					pMat[nCntMat].MatD3D.Diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, m_ObjectXInfo.Diffuse[nCntMat].a);
				}

				//マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//テクスチャを設定する
				pDevice->SetTexture(0, NULL);

				//モデル（パーツ）の描画
				m_ObjectXInfo.pMesh->DrawSubset(nCntMat);
			}
			//================================================================================================================
		}
	}
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
//拡大率を加算するかどうか
//============================================================================
void CObjectX::SizeInfo::SetUseAddScale(D3DXVECTOR3 CopyAddScale, bool bUse)
{
	bUseAddScaling = bUse;       //拡大率の加算をするかどうか
	AddScale = CopyAddScale;     //拡大率の加算を行う
}
//================================================================================================================================================

//============================================================================
//拡大率加算処理
//============================================================================
void CObjectX::SizeInfo::AddScaleProcess()
{
	if (bUseAddScaling == true)
	{//拡大率の加算を行うなら
		Scale += AddScale;
	}
}
//================================================================================================================================================

//============================================================================
//拡大率乗算処理
//============================================================================
void CObjectX::SizeInfo::MultiScaleProcess()
{
	if (bUseMultiScale == true)
	{//拡大率の乗算を行うなら
		Scale.x *= MultiScale.x;
		Scale.y *= MultiScale.y;
		Scale.z *= MultiScale.z;
	}
}
//================================================================================================================================================

//============================================================================
//拡大率を参照して頂点の最大最小を決める処理
//============================================================================
void CObjectX::SizeInfo::DecideVtxMaxMinProcess()
{
	//XZの頂点を入れ替えるかどうかによってx,zの値を変える
	if (bSwapVtxXZ == false)
	{
		VtxMax.x = OriginVtxMax.x * Scale.x;
		VtxMax.y = OriginVtxMax.y * Scale.y;
		VtxMax.z = OriginVtxMax.z * Scale.z;
		VtxMin.x = OriginVtxMin.x * Scale.x;
		VtxMin.y = OriginVtxMin.y * Scale.y;
		VtxMin.z = OriginVtxMin.z * Scale.z;
	}
	else
	{
		VtxMax.x = OriginVtxMax.x * Scale.z;
		VtxMax.y = OriginVtxMax.y * Scale.y;
		VtxMax.z = OriginVtxMax.z * Scale.x;
		VtxMin.x = OriginVtxMin.x * Scale.z;
		VtxMin.y = OriginVtxMin.y * Scale.y;
		VtxMin.z = OriginVtxMin.z * Scale.x;
	}

	//サイズ決定
	Size = VtxMax - VtxMin;
}
//================================================================================================================================================

//============================================================================
//重力の処理
//============================================================================
void CObjectX::MoveInfo::GravityProcess()
{
	if (bUseGravity == true)
	{//重力を使用するなら
		const D3DXVECTOR3& Move = GetMove();
		SetMove(Move + D3DXVECTOR3(0.0f, -fGravityPower, 0.0f));
	}
}

//==================================
//乗算加速処理
//==================================
void CObjectX::MoveInfo::MultiSpeedProcess()
{
	if (bUseMultiSpeed == true)
	{//速度の乗算を行うなら
		Move.x *= MultiSpeed.x;
		Move.y *= MultiSpeed.y;
		Move.z *= MultiSpeed.z;
	}
}

//==================================
//乗算加速処理
//==================================
void CObjectX::MoveInfo::AddSpeedProcess()
{
	if (bUseAddSpeed == true)
	{//速度の加算を行うなら
		Move += AddSpeed;
	}
}
//================================================================================================================================================

//==================================
//ヒットストップ処理
//==================================
void CObjectX::LifeInfo::HitStopProcess()
{
	if (bHitStop == true)
	{
		if (nHitStopTime > 0)
		{
			nHitStopTime--;
		}
		else
		{//ヒットストップ解除
			bHitStop = false;
			nHitStopTime = 0;
		}
	}
}
//================================================================================================================================================

//==============================================
//体力を減らす
//==============================================
void CObjectX::LifeInfo::AutoSubLifeProcess()
{
	if (bAutoSubLife == true)
	{//自動的に体力を減らすかどうか
		nLife--;
	}
}
//================================================================================================================================================

//==============================================
//体力の割合で色合いの透明度を変える
//==============================================
void CObjectX::LifeInfo::RatioLifeAlphaColorProcess(CObjectX* pObjX)
{
	if (bUseRatioLifeAlpha == true)
	{//体力の割合で色合いの透明度を変えるかどうか
		float fRatio;
		fRatio = float(nLife) / float(nMaxLife);
		pObjX->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, fRatio), 3, true, true,false);
		CManager::GetDebugText()->PrintDebugText("透明度割合：%f\n", fRatio);
	}
}
//================================================================================================================================================

//==============================================
//自動的に死亡フラグを発動する処理
//==============================================
void CObjectX::LifeInfo::AutoDeathProcess(CObjectX* pObjX)
{
	if (nLife < 1 && bAutoDeath == true && pObjX->GetUseDeath() == true)
	{//体力がなくなったら自動的に死亡フラグを設定するかどうか
		pObjX->SetDeath();
	}
}
//================================================================================================================================================

//==============================================
//色合いの変更を行うかどうか
//==============================================
void CObjectX::DrawInfo::ChengeColorProcess(CObjectX* pObjX)
{
	if (bColorChenge == true)
	{//色合いの変更
		nChengeColorTime--;

		if (bBlinkingColor == true)
		{
			//2fごとに設定された色と元の色合いを交互に変える
			if (nChengeColorTime % 2 == 0)
			{
				pObjX->SetOnlyColor(Color);
			}
			else
			{
				pObjX->SetOnlyFormarColor();
			}
		}
	}

	if (nChengeColorTime <= 0 && bColorChenge == true)
	{//色合いを変える時間がなくなったら元の色合いに戻す
		nChengeColorTime = 0;//色合いを変えるフレームのリセット
		pObjX->SetFormarColor();//元の色合いに戻す
		bColorChenge = false;//色合いを変えない
	}
}
//================================================================================================================================================
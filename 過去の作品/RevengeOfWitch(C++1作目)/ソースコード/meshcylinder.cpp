//=======================================================================================================================
//
//６月２１日：メッシュシリンダーで背景を出したい。[meshCylinder.cpp]
//Author:ShinaTaiyo
//
//=======================================================================================================================

//=================================================================================================================
//インクルード
//=================================================================================================================
#include "meshcylinder.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
//=================================================================================================================

//==================================================================================================================
//コンストラクタ
//==================================================================================================================
CMeshCylinder::CMeshCylinder()
{
	m_Info.pTexture = nullptr;//テクスチャ
	m_Info.pVtxBuff = nullptr;//頂点バッファへのポインタ
	m_Info.pIdxBuff = nullptr;//インデックスバッファへのポインタ
}
//=========================================================================================================================================

//==================================================================================================================
//デストラクタ
//==================================================================================================================
CMeshCylinder::~CMeshCylinder()
{
}
//=========================================================================================================================================

//==================================================================================================================
//初期化処理
//==================================================================================================================
HRESULT CMeshCylinder::Init()
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

    //デバイスの取得
	pDevice = CManager::GetRenderer()->GetDevice();

	VERTEX_3D* pVtx;

	m_Info.nNumVtxXZ = m_Info.nNumBlockXZ + 1;                                                   //横の頂点数
	m_Info.nDivisionY = m_Info.nNumBlockY - 1;                                                   //Y方向分割数
	m_Info.nNumVtx = m_Info.nNumVtxXZ * (m_Info.nNumBlockY + 1);                                 //メッシュシリンダーの頂点数（横頂点数 * ブロック数　+ 1)
	m_Info.nNumPolygon = ((m_Info.nNumBlockXZ * 2) * m_Info.nNumBlockY + 2 * m_Info.nDivisionY); //ポリゴン数
	m_Info.nNumIdx = m_Info.nNumPolygon + 2;                                                     //インデックス数

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_Info.nNumVtx,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_Info.pVtxBuff,
		NULL);

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * m_Info.nNumIdx,//インデックス数
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_Info.pIdxBuff,
		NULL);

	//=============================
	//頂点の設定
	//=============================

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_Info.pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	float fRatioRot = 0.0f;//角度計算用変数
	float fRatioHeight = 0.0f;//高さ計算用
	float fVecX = 0.0f;//ベクトルX計算用
	float fVecZ = 0.0f;//ベクトルZ計算用
	float fVec = 0.0f; //ベクトル計算用
	int nNumVtx = 0;//現在設定している頂点番号
	int nCntArray = 0;//配列カウント用
	D3DXVECTOR3 MeasureNor = NULL_VECTOR3;//法線計算用
	D3DXVECTOR3 NormalizeNor = NULL_VECTOR3;//正規化した法線を代入する
	for (int nCntVtxY = 0; nCntVtxY < m_Info.nNumBlockY + 1; nCntVtxY++)
	{//頂点の列分設定する（ブロック数＋１）
		for (int nCntVtxXZ = 0; nCntVtxXZ < m_Info.nNumVtxXZ; nCntVtxXZ++)
		{
			nCntArray = m_Info.nNumVtxXZ * nCntVtxY;//前のfor分までに設定した配列をカウントする

			fRatioRot = (2.0f / m_Info.nNumBlockXZ) * (float)(nCntVtxXZ);  //現在の頂点を設定する向きを決める（割合）

			fRatioHeight = 1.0f - (1.0f / m_Info.nNumBlockY) * nCntVtxY;//現在の頂点を設定する高さを決める（割合）

			if (nCntVtxXZ + nCntArray < 0 || nCntVtxXZ + nCntArray >= m_Info.nNumVtx)
			{
				assert("メッシュシリンダーの頂点バッファが配列外アクセス");
			}
			//位置を設定
			pVtx[(nCntVtxXZ)+nCntArray].pos = D3DXVECTOR3(sinf(D3DX_PI - D3DX_PI * -fRatioRot) * m_Info.fRadius,
				(m_Info.fHeight * fRatioHeight) - m_Info.fHeight * 0.5f, cosf(D3DX_PI - D3DX_PI * -fRatioRot) * m_Info.fRadius);

			//原点と頂点のベクトルを計算
			fVecX = pVtx[(nCntVtxXZ)+nCntArray].pos.x - m_Pos.x;
			fVecZ = pVtx[(nCntVtxXZ)+nCntArray].pos.z - m_Pos.z;
			fVec = atan2f(fVecX, fVecZ);

			//法線の向きを求める
			MeasureNor = D3DXVECTOR3(fVecX, 0.0f, fVecZ);//法線を計算	

			//法線のベクトルを正規化する
			D3DXVec3Normalize(&NormalizeNor, &MeasureNor);

			//法線を設定
			pVtx[(nCntVtxXZ)+nCntArray].nor = NormalizeNor;

			//頂点カラーの設定
			pVtx[(nCntVtxXZ)+nCntArray].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//テクスチャ座標の設定
			pVtx[(nCntVtxXZ)+nCntArray].tex = D3DXVECTOR2((1.0f / m_Info.nNumBlockXZ) * (float)(nCntVtxXZ), -fRatioHeight);

			//現在設定している頂点の番号を代入
			nNumVtx = (nCntVtxXZ)+nCntArray;
		}
	}

	//頂点バッファをアンロックする 
	m_Info.pVtxBuff->Unlock();

	//========================================================================================================================================================================================


	//==============================
	//インデックス情報の設定
	//==============================

	//インデックスバッファのロック
	WORD* pIdx;
	m_Info.pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	int nCntIdx = 0;//インデックス計算用
	int nCntIdx2 = 0;//インデックス計算用２
	int nCntIdxX = 0;//X方向のインデックス計算用
	int nIdx = 0; //インデックス代入用
	int nIdx2 = 0;//インデックス代入用２
	int nCntArray2 = 0;//配列計算用２
	for (int nCntVtxY = 0; nCntVtxY < m_Info.nNumBlockY; nCntVtxY++)
	{//ブロック数分、設定を行う

		nCntIdx = (m_Info.nNumVtxXZ * nCntVtxY);       //インデックス計算
		nCntIdx2 = (m_Info.nNumVtxXZ * (nCntVtxY + 1));//インデックス計算

		for (int nCntVtxXZ = 0; nCntVtxXZ < m_Info.nNumVtxXZ; nCntVtxXZ++)
		{
			if (nCntArray2 + 1 < 0 || nCntArray2 + 1 >= m_Info.nNumIdx)
			{
				assert("メッシュシリンダーのインデックスが配列外アクセス");
			}

			pIdx[nCntArray2] = nCntVtxXZ + nCntIdx2;   //9,10,11,12,13,14,15,16,17,18
			pIdx[nCntArray2 + 1] = nCntVtxXZ + nCntIdx;//0, 1, 2, 3, 4, 5, 6, 7, 8, 9

			nIdx = nCntArray2;     //現在設定しているインデックスの番号を代入
			nIdx2 = nCntArray2 + 1;//現在設定しているインデックスの番号を代入2

			nCntArray2 += 2;//設定したインデックス番号分次に進める
		}
	}

	//インデックスバッファのアンロック
	m_Info.pIdxBuff->Unlock();

	////==========================================================================================================================================
	return S_OK;
}
//=========================================================================================================================================

//==================================================================================================================
//終了処理
//==================================================================================================================
void CMeshCylinder::Uninit()
{

	//頂点バッファの破棄
	if (m_Info.pVtxBuff != nullptr)
	{
		m_Info.pVtxBuff->Release();
	    m_Info.pVtxBuff = nullptr;
	}

	//インデックスバッファの開放
	if (m_Info.pIdxBuff != nullptr)
	{
		m_Info.pIdxBuff->Release();
		m_Info.pIdxBuff = nullptr;
	}

}
//=========================================================================================================================================

//==================================================================================================================
//更新処理
//==================================================================================================================
void CMeshCylinder::Update()
{
	m_Rot.y += 0.001f;
	//m_Pos = CGame::GetPlayer()->GetPos();
}
//=========================================================================================================================================

//==================================================================================================================
//描画処理
//==================================================================================================================
void CMeshCylinder::Draw()
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	//デバイスの取得
	pDevice = CManager::GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;//計算用マトリックス

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans,m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);


	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0,m_Info.pVtxBuff, 0, sizeof(VERTEX_3D));//頂点バッファへのポインタと頂点情報の構造体のサイズ

	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(m_Info.pIdxBuff);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, m_Info.pTexture);

	//ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0,m_Info.nNumVtx, 0,m_Info.nNumPolygon);

}
//=========================================================================================================================================

//==================================================================================================================
//テクスチャ割り当て処理
//==================================================================================================================
void CMeshCylinder::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_Info.pTexture = pTexture;
}
//=========================================================================================================================================

//==================================================================================================================
//メッシュシリンダー情報を設定する
//==================================================================================================================
void CMeshCylinder::SetCylinderInfo(D3DXVECTOR3 Pos, D3DXVECTOR3 Rot, int nNumBlockXZ, int nNumBlockY, float fRadius, float fHeight)
{
	m_Pos = Pos;
	m_Rot = Rot;
	m_Info.nNumBlockXZ = nNumBlockXZ;
	m_Info.nNumBlockY = nNumBlockY;
	m_Info.fRadius = fRadius;
	m_Info.fHeight = fHeight;
}
//=========================================================================================================================================
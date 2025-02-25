//=======================================================================================================================
//
//１１月１５日：メッシュシリンダーでワイヤーを表現したいので勉強しなおす[meshCylinder.cpp]
//Author:ShinaTaiyo
//
//=======================================================================================================================

//=================================================================================================================
//インクルード
//=================================================================================================================
#include "meshcylinder.h"
#include "manager.h"
#include "debugtext.h"
#include "renderer.h"
#include "particle.h"
#include "input.h"
#include "game.h"
//=================================================================================================================

//==================================================================================================================
//コンストラクタ
//==================================================================================================================
CMeshCylinder::CMeshCylinder(const float fRadius, const float fHeight, const int nNumDivisionXZ, const int nNumDivisionY,const D3DXVECTOR3 Pos,const D3DXVECTOR3 Rot
,int nPri, bool bUseintPri, CObject::TYPE type, CObject::OBJECTTYPE ObjType) : CObject(nPri,bUseintPri,type,ObjType),
m_fRadius(fRadius),m_fHeight(fHeight),//半径、高さ
m_nNumDivisionXZ(nNumDivisionXZ),m_nNumDivisionY(nNumDivisionY),//XZ方向分割数、Y方向分割数
m_nNumVtx((m_nNumDivisionXZ + 1) * m_nNumDivisionY + 2),//頂点数（上面の中心点　＋　下面の中心点　＋　（ (X方向頂点数 + 1)　＊　Y方向頂点数 ）※X方向の+1はUV対策
m_nNumIdx((m_nNumDivisionXZ + 1) * 2 * (m_nNumDivisionY + 1) + 2 * m_nNumDivisionY),//インデックス数(１面は、XZ方向頂点数　＊　２)　＊　（Y方向頂点数 + 1) + 2 * (縮退計算数)
m_nTextureIndex(0),m_pIdxBuff(nullptr),m_pVtxBuff(nullptr),m_pTexture(nullptr),
m_Pos(Pos),m_Rot(Rot),m_nCheckVtx(0),m_nCheckIdx(0),m_pMtxChild(nullptr),m_bUseDraw(true)
{
	m_nNumPolygon = m_nNumIdx - 2;//例：インデックスバッファが（０、１、２、３、４、５）の場合
	                              //→ポリゴン１（０、１、２）、ポリゴン２（１、２、３）、ポリゴン３（２、３、４）、ポリゴン４（３、４、５）
	                              //つまり、６　ー　２　＝　４
	m_pSenterPos = DBG_NEW D3DXVECTOR3[m_nNumDivisionY];//Y方向の分割数分、中心座標を定義する
}
//=========================================================================================================================================

//2 * (m_nNumDivisionXZ + 1) * (m_nNumDivisionY + 1) + 2 * 2 + 1 * (m_nNumDivisionY - 1) - 2

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

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nNumVtx,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * m_nNumIdx,//インデックス数
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	 VERTEX_3D* pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	int nCntArray = 0;
	float fRatioXZ = 0.0f;
	float fRatioY = 0.0f;
	D3DXVECTOR3 MeasureNor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//法線計算用
	for (int nCntVtxY = 0; nCntVtxY < m_nNumDivisionY; nCntVtxY++)
	{//Y方向のUVはそのまま使う
		fRatioY = (1.0f / (m_nNumDivisionY - 1)) * nCntVtxY;
		for (int nCntVtxXZ = 0; nCntVtxXZ < m_nNumDivisionXZ + 1; nCntVtxXZ++)
		{//X方向のUVは重なる頂点があるので、+ 1
			fRatioXZ = (1.0f / (m_nNumDivisionXZ)) * nCntVtxXZ;//XZ方向の割合を求める
			if (nCntVtxY == 0 && nCntVtxXZ == 0)
			{//最初の周なので上面の中心点を設定する
			    pVtx[nCntArray].pos = m_Pos + D3DXVECTOR3(0.0f, m_fHeight, 0.0f);//位置の設定
				pVtx[nCntArray].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);         //色合い
				MeasureNor = pVtx[nCntArray].pos - m_Pos;                        //法線
				D3DXVec3Normalize(&MeasureNor, &MeasureNor);                     //法線を正規化
				pVtx[nCntArray].nor = MeasureNor;                                //法線を設定
				pVtx[nCntArray].tex = D3DXVECTOR2(0.0f, fRatioY);                //テクスチャを設定
				nCntArray++;
			}

			SetVertexInfo(&pVtx[nCntArray], fRatioXZ, fRatioY, nCntVtxXZ, nCntVtxY);//頂点情報を設定する

			if (nCntArray >= m_nNumVtx || nCntArray < 0)
			{//配列外チェック
				assert("配列外アクセス！");
			}

			//配列カウント
			nCntArray++;

			if (nCntVtxY == m_nNumDivisionY - 1 && nCntVtxXZ == m_nNumDivisionXZ)
			{//最後
				pVtx[nCntArray].pos = m_Pos;//底面の中心に位置を設定
				pVtx[nCntArray].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//色合いを設定
				MeasureNor = pVtx[nCntArray].pos - m_Pos;//法線を計算
				D3DXVec3Normalize(&MeasureNor, &MeasureNor);//正規化する
				pVtx[nCntArray].nor = MeasureNor;//設定
				pVtx[nCntArray].tex = D3DXVECTOR2(0.0f, fRatioY);//テクスチャ座標を設定
			}
		}
	}
	//頂点バッファをアンロックする 
	m_pVtxBuff->Unlock();

	//==============================
	//インデックス情報の設定
	//==============================

	//インデックスバッファのロック
	WORD* pIdx;
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	int nCntIdx = 0;//インデックス計算用
	int nCntArray2 = 0;//配列計算用２

	bool bSide = false;//側面かどうか

	for (int nCntVtxY = 0; nCntVtxY < m_nNumDivisionY + 1; nCntVtxY++)//１は上面、下面の分
	{//ブロック数分、設定を行う
		nCntIdx = (m_nNumDivisionXZ + 1) * nCntVtxY + 1;//1,10,19・・・（上面→側面→側面→底面）
		if (nCntVtxY == 0)
		{//上面の中心
			bSide = false;
		}
		else if (nCntVtxY == m_nNumDivisionY)
		{//底面の中心
			bSide = false;
		}
		else
		{//側面
			bSide = true;
		}

		if (nCntArray2 >= m_nNumIdx || nCntArray2 < 0)
		{
			assert("配列外アクセス！");
		}

		if (bSide == true)
		{//普通に頂点を構成（側面)
			for (int nCntVtxXZ = 0; nCntVtxXZ < m_nNumDivisionXZ + 1; nCntVtxXZ++)
			{
				pIdx[nCntArray2] = nCntVtxXZ + nCntIdx;   //10,11,12,13,14,15,16,17,18
				pIdx[nCntArray2 + 1] = nCntIdx - m_nNumDivisionXZ - 1 + nCntVtxXZ;//1, 2, 3, 4, 5, 6, 7, 8, 9
				nCntArray2 += 2;//設定したインデックス番号分次に進める
			}
		}
		else
		{//底面か上面なのでそれぞれの面の中心点を基準にインデックスを構成
			for (int nCntVtxXZ = 0; nCntVtxXZ < m_nNumDivisionXZ + 1; nCntVtxXZ++)
			{
				if (nCntVtxY == 0)
				{//上面
					pIdx[nCntArray2] = nCntVtxXZ + nCntIdx;   //1,2,3,4,5,6,7,8,9
					pIdx[nCntArray2 + 1] = 0;//0,0,0,0,0,0,0,0,0
				}
				else
				{//下面
					pIdx[nCntArray2] = m_nNumVtx - m_nNumDivisionXZ - 2 + nCntVtxXZ;    //19,20,21,22,23,24,25,26,27
					pIdx[nCntArray2 + 1] = m_nNumVtx - 1;      //頂点数 - 1(配列最大数)(底面の中心点)28
				}
				nCntArray2 += 2;//設定したインデックス番号分次に進める
			}
		}

		if (nCntVtxY != m_nNumDivisionY)
		{//最後の行以外なら縮退ポリゴンを設定
			if (nCntVtxY == 0)
			{//一周目なら中心点から下の段の最初につなげる
				pIdx[nCntArray2] = 0;                       //上面の中心↓//0
				pIdx[nCntArray2 + 1] = m_nNumDivisionXZ + 2;//下の段の最初//10
			}
			else if (nCntVtxY == m_nNumDivisionY - 1)
			{//最後の縮退
				pIdx[nCntArray2] = nCntVtxY * (m_nNumDivisionXZ + 1);//18
				pIdx[nCntArray2 + 1] = nCntVtxY * (m_nNumDivisionXZ + 1) + 1;//19
			}
			else
			{//最初と最後の周以外なら
				pIdx[nCntArray2] = nCntVtxY * (m_nNumDivisionXZ + 1);//9,18
				pIdx[nCntArray2 + 1] = (nCntVtxY + 1) * (m_nNumDivisionXZ + 1) + 1;//19,28
			}
			nCntArray2 += 2;
		}
	}

	//インデックスバッファのアンロック
	m_pIdxBuff->Unlock();

	//////==========================================================================================================================================
	return S_OK;
}
//=========================================================================================================================================

//==================================================================================================================
//終了処理
//==================================================================================================================
void CMeshCylinder::Uninit()
{
	//頂点バッファの破棄
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
	    m_pVtxBuff = nullptr;
	}

	//インデックスバッファの開放
	if (m_pIdxBuff != nullptr)
	{
		m_pIdxBuff->Release();
		m_pIdxBuff = nullptr;
	}

	//テクスチャの開放（別で開放する)
	if (m_pTexture != nullptr)
	{
		m_pTexture = nullptr;
	}

	//それぞれの中心点の開放（動的配列を破棄）
	if (m_pSenterPos != nullptr)
	{
		delete[] m_pSenterPos;
		m_pSenterPos = nullptr;
	}
}
//=========================================================================================================================================

//==================================================================================================================
//更新処理
//==================================================================================================================
void CMeshCylinder::Update()
{
#ifdef _DEBUG
	CheckMeshInfo();
#endif
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
	D3DXMatrixTranslation(&mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	if (m_pMtxChild != nullptr)
	{//子マトリックスが存在するなら掛け合わせる
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, m_pMtxChild);
	}

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//両面を描画する
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));//頂点バッファへのポインタと頂点情報の構造体のサイズ

	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(m_pIdxBuff);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	if (m_bUseDraw == true)
	{
		//ポリゴンの描画
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_nNumVtx, 0, m_nNumPolygon);
	}
	//片面だけ描画する
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}
//=========================================================================================================================================

//==================================================================================================================
//死亡フラグ設定処理
//==================================================================================================================
void CMeshCylinder::SetDeath()
{
	CObject::SetDeath();//死亡フラグ設定処理
}
//=========================================================================================================================================

//==================================================================================================================
//テクスチャ割り当て処理
//==================================================================================================================
void CMeshCylinder::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;//テクスチャアドレスを格納
}
//=========================================================================================================================================

//==================================================================================================================
//頂点情報を設定
//==================================================================================================================
void CMeshCylinder::SetVertexInfo(VERTEX_3D* pVtx, float& fRatioXZ, float& fRatioY, int& nCntVtxXZ, int& nCntVtxY)
{
	//側面に頂点を展開
	pVtx->pos = D3DXVECTOR3(sinf((D3DX_PI * 2) * fRatioXZ) * m_fRadius,m_fHeight - (m_fHeight * fRatioY), cosf((D3DX_PI * 2) * fRatioXZ) * m_fRadius) + m_Pos;//1,2,3,4,5,6,7,8→10,11,12,13,14,15,16,17

	//色合い
	pVtx->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//法線を求める
	D3DXVECTOR3 Nor = pVtx->pos - m_Pos;//ベクトルを求める
	D3DXVec3Normalize(&Nor, &Nor);//法線を正規化
	pVtx->nor = Nor;//代入

	//テクスチャ
	pVtx->tex = D3DXVECTOR2(fRatioXZ, fRatioY);

}
//=========================================================================================================================================

//==================================================================================================================
//ポリゴン数を変える処理
//==================================================================================================================
void CMeshCylinder::ChengeNumPolygon()
{
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_U) == true)
	{
		m_nNumPolygon++;
	}
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_J) == true)
	{
		m_nNumPolygon--;
	}

	//デバッグ表示
	CManager::GetDebugText()->PrintDebugText("ポリゴン数：%d\n", m_nNumPolygon);
	CManager::GetDebugText()->PrintDebugText("頂点数：%d\n", m_nNumVtx);
	CManager::GetDebugText()->PrintDebugText("インデックス数：%d\n", m_nNumIdx);
}
//=========================================================================================================================================

//==================================================================================================================
//メッシュの情報を確認する
//==================================================================================================================
void CMeshCylinder::CheckMeshInfo()
{
	VERTEX_3D* pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//=================================================================
	//頂点情報確認
	//=================================================================
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_O) == true)
	{
		m_nCheckVtx++;
	}
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_L) == true)
	{
		m_nCheckVtx--;
	}

	if (m_nCheckVtx >= m_nNumVtx)
	{//配列サイズを超えるので０に戻す
		m_nCheckVtx = 0;
	}
	else if (m_nCheckVtx < 0)
	{//配列がーになるので最大にする
		m_nCheckVtx = m_nNumVtx - 1;
	}

	if (m_nCheckVtx >= m_nNumVtx || m_nCheckVtx < 0)
	{//配列買いアクセスチェック
		assert("配列外アクセス！");
	}
	//デバッグ表示
	CManager::GetDebugText()->PrintDebugText("確認している頂点番号：%d、頂点の位置：%f %f %f\n",
		m_nCheckVtx, pVtx[m_nCheckVtx].pos.x, pVtx[m_nCheckVtx].pos.y, pVtx[m_nCheckVtx].pos.z);

	//==============================
	//インデックス情報の設定
	//==============================

	//インデックスバッファのロック
	WORD* pIdx;
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);
	//インデックスバッファのアンロック

	if (CManager::GetInputKeyboard()->GetTrigger(DIK_I) == true)
	{
		m_nCheckIdx++;
	}
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_K) == true)
	{
		m_nCheckIdx--;
	}

	if (m_nCheckIdx >= m_nNumIdx)
	{//配列サイズを超えるので０に戻す
		m_nCheckIdx = 0;
	}
	else if (m_nCheckIdx < 0)
	{//配列がーになるので最大にする

		m_nCheckIdx = m_nNumIdx - 1;
	}

	if (m_nCheckIdx >= m_nNumIdx || m_nCheckIdx < 0)
	{//配列外アクセスチェック
		assert("配列外アクセス！");
	}

	int nCheck = pIdx[m_nCheckIdx];

	//デバッグ表示
	CManager::GetDebugText()->PrintDebugText("確認しているインデックス番号：%d、頂点の位置：%f %f %f\n", m_nCheckIdx, pVtx[nCheck].pos.x, pVtx[nCheck].pos.y, pVtx[nCheck].pos.z);

	//インデックスバッファをアンロック
	m_pIdxBuff->Unlock();

	//頂点バッファをアンロックする 
	m_pVtxBuff->Unlock();
	//==================================================================================================================

	//ポリゴン数を変える
	ChengeNumPolygon();
}
//=========================================================================================================================================
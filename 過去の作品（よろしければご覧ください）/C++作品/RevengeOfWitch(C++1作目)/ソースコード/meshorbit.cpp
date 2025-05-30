//===============================================================================================
//
//８月２３日：軌跡を作る[meshorbit.cpp]
//Author:ShinaTaiyo
//
//===============================================================================================

//=============================================================
//インクルード
//=============================================================
#include "meshorbit.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "debugtext.h"
//===============================================================================================

//=============================================================
//静的メンバ宣言
//=============================================================
const char* CMeshOrbit::m_apMESHORBIT_FILENAME[CMeshOrbit::TYPE_MAX] =
{
	"data\\TEXTURE\\Shadow000.jpg",
};

//=============================================================
//コンストラクタ
//=============================================================
CMeshOrbit::CMeshOrbit() : m_mtxWorld(),m_Pos(NULL_VECTOR3),m_Rot(NULL_VECTOR3),m_pTexture(nullptr),m_pVtxBuff(nullptr),
m_nTextureIndex(0),m_pIdxBuff(nullptr),m_OffSetPos(NULL_VECTOR3),m_type(MESHORBITTYPE00_NORMAL),CObject(5)
{

}
//===============================================================================================

//=============================================================
//デストラクタ
//=============================================================
CMeshOrbit::~CMeshOrbit()
{

}
//===============================================================================================

//=============================================================
//初期化処理
//=============================================================
HRESULT CMeshOrbit::Init()
{
	m_Pos = NULL_VECTOR3;   //位置
	m_OffSetPos = NULL_VECTOR3;//オフセット位置
	m_Rot = NULL_VECTOR3;   //向き
	m_pVtxBuff = nullptr;   //頂点バッファ
	m_pTexture = nullptr;   //テクスチャ
	m_pIdxBuff = nullptr;   //インデックスバッファ
	m_mtxWorld = {};        //マトリックスワールド
	m_type = MESHORBITTYPE00_NORMAL;//種類
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nTOTAL_VERTEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * m_nTOTAL_INDEX,//インデックス数
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	VERTEX_3D* pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//インデックスバッファのロック
	WORD* pIdx;
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	float fRatioX = 0.0f;
	float fRatioZ = 0.0f;
	int nCntX = 0;
	int nCntZ = 0;

	for (int nCnt = 0; nCnt < m_nTOTAL_VERTEX; nCnt++)
	{
		fRatioX = (float)(nCntX) / (float)(m_nNUMBLOCK_X);//現在設定している頂点の位置の割合を求める
		fRatioZ = (float)(nCntZ) / (float)(m_nNUMBLOCK_Z);//現在設定している頂点の位置の割合を求める

		pVtx[nCnt].pos = NULL_VECTOR3;//位置を設定

		//法線を設定
		pVtx[nCnt].nor = D3DXVECTOR3(0.0f,0.0f,-1.0f);

		//頂点カラーの設定
		pVtx[nCnt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標の設定
		pVtx[nCnt].tex = D3DXVECTOR2(fRatioX,fRatioZ);

		nCntX++;

		if (nCntX > m_nNUMBLOCK_X)
		{
			nCntX = 0;
			nCntZ++;
		}
		if (nCntZ > m_nNUMBLOCK_Z)
		{
			nCntZ = 0;
		}
	}

	int nCntIdxX = 0;
	int nCntIdxZ = 0;
	int nCntArray = 0;//配列カウント用
	int nIdx = 0;//インデックス代入用
	//=======================================================================
	//インデックスを求める
	//=======================================================================
	for (int nCntBlockZ = 0; nCntBlockZ < m_nNUMBLOCK_Z; nCntBlockZ++)
	{
		for (int nCntBlockX = 0; nCntBlockX < m_nNUMBLOCK_X + 1; nCntBlockX++)
		{
			if (nCntArray + 1 < 0 || nCntArray + 1 >= m_nTOTAL_INDEX)
			{
				assert("軌跡メッシュのインデックスバッファが配列外アクセス");
			}
			pIdx[nCntArray] = m_nNUMVERTEX_X + nCntIdxX;  //インデックスを代入
			pIdx[nCntArray + 1] = nCntIdxX;               //ブロックXの行の上側を設定

			nCntIdxX++;
			nCntArray += 2;
		}

		if (nCntBlockZ != (m_nNUMBLOCK_Z - 1))
		{//最後の行だけは縮退ポリゴンを計算しない
			pIdx[nCntArray] = m_nNUMVERTEX_X * (nCntBlockZ + 1) - 1;
			pIdx[nCntArray + 1] = m_nNUMVERTEX_X * (nCntBlockZ + 2);
			nCntArray += 2;
		}
	}
	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	//インデックスバッファのアンロック
	m_pIdxBuff->Unlock();

	return S_OK;
}
//===============================================================================================

//=============================================================
//終了処理
//=============================================================
void CMeshOrbit::Uninit()
{
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	if (m_pIdxBuff != nullptr)
	{
		m_pIdxBuff->Release();
		m_pIdxBuff = nullptr;
	}
}
//===============================================================================================

//=============================================================
//更新処理
//=============================================================
void CMeshOrbit::Update()
{
	VERTEX_3D* pVtx;

	float fRatioVtx = 0.0f;
	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	int nCntArray = m_nTOTAL_VERTEX - 1;//最後の配列番号

	pVtx[0].pos = m_OffSetPos;  //オフセットの位置を反映
	pVtx[m_nNUMVERTEX_X].pos = m_Pos;        //原点の位置を代入
	int nCol1 = 0;//頂点番号列１
	int nCol2 = 0;//頂点番号列２

	int nCntVtx = 0;//頂点計算用
	for (int nCntZ = 0; nCntZ < m_nNUMBLOCK_Z; nCntZ++)
	{
		nCol1 = m_nNUMBLOCK_X + (nCntZ * m_nNUMVERTEX_X);
		nCol2 = m_nNUMBLOCK_X + (nCntZ * m_nNUMVERTEX_X) + m_nNUMVERTEX_X;
		for (int nCntX = 0; nCntX < m_nNUMBLOCK_X; nCntX++)
		{
			fRatioVtx = float(nCntX) / float(m_nNUMBLOCK_X);
			if (nCol1 < 0 || nCol1 >= m_nTOTAL_VERTEX)
			{
				assert("軌跡メッシュの頂点バッファが配列外アクセス");
			}

			if (nCol2 < 0 || nCol2 >= m_nTOTAL_VERTEX)
			{
				assert("軌跡メッシュの頂点バッファが配列外アクセス");
			}

			//１ブロック分の情報を設定する
			pVtx[nCol1].pos = pVtx[nCol1 - 1].pos;  //オフセット行　〇１：（２　＝　１）、〇２：（１　＝　０）
			pVtx[nCol2].pos = pVtx[nCol2 - 1].pos;  //原点行 〇１：（５　＝　４）、〇２：（４　＝　３）

			//色合い
			pVtx[nCol1].col = D3DXCOLOR(1.0f,1.0f,1.0f,fRatioVtx);  //オフセット行　〇１：（２　＝　１）、〇２：（１　＝　０）
			pVtx[nCol2].col = D3DXCOLOR(1.0f,1.0f,1.0f,fRatioVtx);  //原点行 〇１：（５　＝　４）、〇２：（４　＝　３）

			nCol1--;
			nCol2--;
		}
	}
	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	////インデックスバッファのロック
	//m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);
	//int nCntIdxX = 0;
	//nCntArray = 0;
	////=======================================================================
 //   //インデックスを求める
 //   //=======================================================================
	//for (int nCntBlockZ = 0; nCntBlockZ < m_nNUMBLOCK_Z; nCntBlockZ++)
	//{
	//	for (int nCntBlockX = 0; nCntBlockX < m_nNUMVERTEX_X; nCntBlockX++)
	//	{
	//		pIdx[nCntArray] = m_nNUMVERTEX_X + nCntIdxX;                    //インデックスを代入
	//		pIdx[nCntArray + 1] = nCntIdxX;               //ブロックXの行の上側を設定

	//		nCntIdxX++;
	//		nCntArray += 2;
	//	}

	//	if (nCntBlockZ != (m_nNUMBLOCK_Z - 1))
	//	{//最後の行だけは縮退ポリゴンを計算しない
	//		pIdx[nCntArray] = m_nNUMVERTEX_X * (nCntBlockZ + 1) - 1;
	//		pIdx[nCntArray + 1] = m_nNUMVERTEX_X * (nCntBlockZ + 2);
	//		nCntArray += 2;
	//	}
	//}
	////===============================================================================================


}
//===============================================================================================

//=============================================================
//描画処理
//=============================================================
void CMeshOrbit::Draw()
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

    //デバイスの取得
	pDevice = CManager::GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;//計算用マトリックス

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);


	//向きを反映
	//D3DXMatrixRotationYawPitchRoll(&mtxRot,m_Rot.y,m_Rot.x,m_Rot.z);
	//D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans,0.0f, 0.0f, 0.0f);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);


	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0,m_pVtxBuff, 0, sizeof(VERTEX_3D));//頂点バッファへのポインタと頂点情報の構造体のサイズ

	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(m_pIdxBuff);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	//両面を描画する
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//アルファテストを有効(アルファ値が０より大きい場合に描画する）
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	//ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_nTOTAL_VERTEX, 0, m_nTOTAL_POLYGON);

	//アルファテスト無効に戻す
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	//片面だけ描画する
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}
//===============================================================================================

//=============================================================
//死亡フラグ設定処理
//=============================================================
void CMeshOrbit::SetDeath()
{
	CObject::SetDeath();
}
//===============================================================================================

//=============================================================
//テクスチャを割り当てる
//=============================================================
void CMeshOrbit::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}
//===============================================================================================

//=============================================================
//生成処理
//=============================================================
CMeshOrbit* CMeshOrbit::Create(MESHORBITTYPE type)
{
	CMeshOrbit* pMeshOrbit = DBG_NEW CMeshOrbit;                                           //軌跡メッシュを生成
	CTexture* pTextureClass = CManager::GetTexture();                                      //テクスチャクラスを取得

	pMeshOrbit->Init();                                                                    //初期化処理
	pMeshOrbit->m_type = type;                                                             //種類
	pMeshOrbit->SetTextureIndex(pTextureClass->Regist(m_apMESHORBIT_FILENAME[type]));      //テクスチャを割り当てる
	pMeshOrbit->BindTexture(pTextureClass->GetAddress(pMeshOrbit->GetTextureIndex()));     //テクスチャをセットする　
	pMeshOrbit->SetUseDeath(false);                                                        //死亡フラグを使用しない
	pMeshOrbit->SetType(CObject::TYPE_MESHORBIT);                                          //オブジェクトタイプを設定
	return pMeshOrbit;
}
//===============================================================================================
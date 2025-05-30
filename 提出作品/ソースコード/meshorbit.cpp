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
//===============================================================================================

//=============================================================
//静的メンバ宣言
//=============================================================
const string CMeshOrbit::m_aMESHORBIT_FILENAME[static_cast<int>(CMeshOrbit::MESHORBITTYPE::MAX)] =
{
	"data\\TEXTURE\\Shadow000.jpg",
	"data\\TEXTURE\\UI\\CutIn_DeathEnaga.png",
	"data\\TEXTURE\\UI\\AustorariaConquestUbaiman_000.png",
};
//=============================================================
//コンストラクタ
//=============================================================
CMeshOrbit::CMeshOrbit(int nPri, bool bUseintPri, CObject::TYPE type, CObject::OBJECTTYPE ObjType) :CObject(nPri,bUseintPri,type,ObjType),m_mtxWorld(),m_Pos2(D3DXVECTOR3(0.0f,0.0f,0.0f)),m_Rot(D3DXVECTOR3(0.0f,0.0f,0.0f)),m_pTexture(nullptr),m_pVtxBuff(nullptr),
m_nTextureIndex(0),m_pIdxBuff(nullptr),m_Pos1(D3DXVECTOR3(0.0f,0.0f,0.0f)),m_type(MESHORBITTYPE::NORMAL)
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
		fRatioX = static_cast<float>(nCntX) / static_cast<float>(m_nNUMBLOCK_X);//現在設定している頂点の位置の割合を求める
		fRatioZ = static_cast<float>(nCntZ) / static_cast<float>(m_nNUMBLOCK_Z);//現在設定している頂点の位置の割合を求める

		pVtx[nCnt].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);//位置を設定

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
	int nCntArray = 0;//配列カウント用
	//=======================================================================
	//インデックスを求める
	//=======================================================================
	for (int nCntBlockZ = 0; nCntBlockZ < m_nNUMBLOCK_Z; nCntBlockZ++)
	{
		for (int nCntBlockX = 0; nCntBlockX < m_nNUMBLOCK_X + 1; nCntBlockX++)
		{
			pIdx[nCntArray] = static_cast<WORD>(m_nNUMVERTEX_X + nCntIdxX);  //インデックスを代入         6,7,8,9,10,
			pIdx[nCntArray + 1] = static_cast<WORD>(nCntIdxX);               //ブロックXの行の上側を設定   0,1,2,3,4,5,

			nCntIdxX++;
			nCntArray += 2;
		}

		if (nCntBlockZ != (m_nNUMBLOCK_Z - 1))
		{//最後の行だけは縮退ポリゴンを計算しない
			pIdx[nCntArray] = static_cast<WORD>(m_nNUMVERTEX_X * (nCntBlockZ + 1) - 1);//
			pIdx[nCntArray + 1] = static_cast<WORD>(m_nNUMVERTEX_X * (nCntBlockZ + 2));
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
	{//頂点バッファの破棄
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	if (m_pIdxBuff != nullptr)
	{//インデックスバッファの破棄
		m_pIdxBuff->Release();
		m_pIdxBuff = nullptr;
	}

	if (m_pTexture != nullptr)
	{//テクスチャへのポインタを初期化
		m_pTexture = nullptr;
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
	//int nCntArray = m_nTOTAL_VERTEX - 1;//最後の配列番号

	int nCol1 = 0;//頂点番号列１
	int nCol2 = 0;//頂点番号列２

	//int nCntVtx = 0;//頂点計算用

	float fDivisionPosY = m_Pos2.y - m_Pos1.y;

	fDivisionPosY /= m_nNUMBLOCK_Z;

	for (int nCntZ = 0; nCntZ < m_nNUMBLOCK_Z; nCntZ++)
	{
		//3×3の場合、１週目：0.4、２週目：4,8、3週目：8,12・・・
		pVtx[0 + m_nNUMVERTEX_X * nCntZ].pos = m_Pos2 - D3DXVECTOR3(0.0f,fDivisionPosY * nCntZ,0.0f);                             //端のブロックの上側の更新する頂点を決める
		pVtx[m_nNUMVERTEX_X + m_nNUMVERTEX_X * nCntZ].pos = m_Pos2 - D3DXVECTOR3(0.0f, fDivisionPosY * (nCntZ + 1), 0.0f);        //端のブロックの下側の更新する頂点を決める

		nCol1 = m_nNUMBLOCK_X + (nCntZ * m_nNUMVERTEX_X);//上で更新するブロックの上側の列の一番右端の頂点番号を決める
		nCol2 = m_nNUMBLOCK_X + (nCntZ * m_nNUMVERTEX_X) + m_nNUMVERTEX_X;//上で更新するブロックの下側の列の一番右端の頂点番号を決める
		for (int nCntX = 0; nCntX < m_nNUMBLOCK_X; nCntX++)
		{
			fRatioVtx = float(nCntX) / float(m_nNUMBLOCK_X);

			if (nCntZ == 0)
			{//最初の一週目なので、１ブロック分を設定
				//１ブロック分の情報を設定する
				pVtx[nCol1].pos = pVtx[nCol1 - 1].pos;  //オフセット行　〇１：（２　＝　１）、〇２：（１　＝　０）
				pVtx[nCol2].pos = pVtx[nCol2 - 1].pos;  //原点行 〇１：（５　＝　４）、〇２：（４　＝　３）

				//色合い
				pVtx[nCol1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fRatioVtx);  //オフセット行　〇１：（２　＝　１）、〇２：（１　＝　０）
				pVtx[nCol2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fRatioVtx);  //原点行 〇１：（５　＝　４）、〇２：（４　＝　３）
			    if ( nCol1 >= m_nTOTAL_VERTEX || nCol2 >= m_nTOTAL_VERTEX || 
					nCol1 < 0 || nCol2 < 0)
		        {
					assert(false);
		        }

			}
			else
			{//２週目以降なので、上のブロックにつながる頂点の位置を設定(nCol1)はもう使わない
				pVtx[nCol2].pos = pVtx[nCol2 - 1].pos;  //原点行 〇１：（５　＝　４）、〇２：（４　＝　３）
				pVtx[nCol2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fRatioVtx);  //オフセット行　〇１：（２　＝　１）、〇２：（１　＝　０）
				if (nCol1 >= m_nTOTAL_VERTEX || nCol2 >= m_nTOTAL_VERTEX || 
					nCol1 < 0 || nCol2 < 0)
				{
					assert(false);
				}
			}

			nCol1--;
			nCol2--;
		}
	}
	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
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
	CObject::SetDeath();//死亡フラグ設定処理
}
//===============================================================================================

//=============================================================
//テクスチャを割り当てる
//=============================================================
void CMeshOrbit::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;//テクスチャ
}
//===============================================================================================

//=============================================================
//生成処理
//=============================================================
CMeshOrbit* CMeshOrbit::Create(MESHORBITTYPE type)
{
	CMeshOrbit* pMeshOrbit = DBG_NEW CMeshOrbit;                                                            //軌跡メッシュを生成
	CTexture* pTextureClass = CManager::GetTexture();                                                       //テクスチャクラスを取得
	pMeshOrbit->Init();                                                                                     //初期化処理
	pMeshOrbit->m_type = type;                                                                              //種類
	pMeshOrbit->SetTextureIndex(pTextureClass->Regist(m_aMESHORBIT_FILENAME[static_cast<int>(type)]));      //テクスチャを割り当てる
	pMeshOrbit->BindTexture(pTextureClass->GetAddress(pMeshOrbit->GetTextureIndex()));                      //テクスチャをセットする　
	pMeshOrbit->SetUseDeath(false);                                                                         //死亡フラグを使用しない
	return pMeshOrbit;
}
//===============================================================================================
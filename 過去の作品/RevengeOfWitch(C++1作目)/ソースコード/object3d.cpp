//==================================================================================
//
//６月３日：３Dオブジェクトクラスを作成する[3Dobject.h]
//Author:ShinaTaiyo
//
//===================================================================================

//=================================
//インクルード
//=================================
#include "object3d.h"
#include "manager.h"
#include "renderer.h"
//===================================================================================

//================================================
//コンストラクタ
//================================================
CObject3D::CObject3D(int nPriority) : CObject(nPriority)
{
	m_pTexture = nullptr;
	m_pVtxBuff = nullptr;
}
//================================================================================================================================================

//================================================
//デストラクタ
//================================================
CObject3D::~CObject3D()
{

}
//================================================================================================================================================

//================================================
//初期化処理
//================================================
HRESULT CObject3D::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice;                     //デバイスへのポインタ
	pDevice = CManager::GetRenderer()->GetDevice();//デバイスの取得
	VERTEX_3D* pVtx;                               //頂点情報へのポインタ

    //=================================
	//変数の初期化
	//=================================
	m_Pos = NULL_VECTOR3;                        //位置
	m_Rot = NULL_VECTOR3;                        //向き
	m_mtxWorld = {};                             //マトリックスワールド
	m_fWidth = 0.0f;                             //横幅
	m_fHeight = 0.0f;                            //高さ
	m_nTextureIndex = 0;                         //テクスチャ番号
	//=======================================================================

	//=================================
	//頂点バッファの生成
	//=================================
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,//頂点の数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);
	//=======================================================================


	//=================================
	//モデル情報の初期化
	//=================================

	//頂点バッファのロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定（初期位置）
	pVtx[0].pos = NULL_VECTOR3;
	pVtx[1].pos = NULL_VECTOR3;
	pVtx[2].pos = NULL_VECTOR3;
	pVtx[3].pos = NULL_VECTOR3;

	//norの設定（初期設定）
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。


	//テクスチャ座標の設定（初期設定）
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファのアンロック
	m_pVtxBuff->Unlock();
	//==============================================================================================================

	return S_OK;
}
//================================================================================================================================================

//================================================
//終了処理
//================================================
void CObject3D::Uninit(void)
{
	//テクスチャの破棄
	if (m_pTexture != nullptr)
	{
		m_pTexture = nullptr;
	}

	//頂点バッファの破棄
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

}
//================================================================================================================================================

//================================================
//更新処理
//================================================
void CObject3D::Update(void)
{
	VERTEX_3D* pVtx;                               //頂点情報へのポインタ

	//頂点バッファのロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定（初期位置）
	pVtx[0].pos = D3DXVECTOR3(-m_fWidth, 0.0f,m_fHeight);
	pVtx[1].pos = D3DXVECTOR3(m_fWidth, 0.0f,m_fHeight);
	pVtx[2].pos = D3DXVECTOR3(-m_fWidth, 0.0f, -m_fHeight);
	pVtx[3].pos = D3DXVECTOR3(m_fWidth, 0.0f, -m_fHeight);

	//norの設定（初期設定）
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。


	//テクスチャ座標の設定（初期設定）
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファのアンロック
	m_pVtxBuff->Unlock();
	//==============================================================================================================


}
//================================================================================================================================================

//================================================
//描画処理
//================================================
void CObject3D::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();//デバイスへのポインタ
	pDevice = CManager::GetRenderer()->GetDevice();                  //デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;                                     //計算用マトリックス

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0,m_pTexture);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

}
//================================================================================================================================================

//================================================
//テクスチャ割り当て処理
//================================================
void CObject3D::bindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}
//================================================================================================================================================

//================================================
//生成処理
//================================================
CObject3D* CObject3D::Create(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	CObject3D* pObject3D = DBG_NEW CObject3D;                        //3Dオブジェクトの動的メモリを確保
	bool bSuccess = pObject3D->CObject::GetCreateSuccess();	     //オブジェクトの生成が成功したかどうかを取得
	VERTEX_3D* pVtx;                                             
	if (bSuccess == true)
	{//オブジェクトの生成が成功
		if (pObject3D != nullptr)
		{
			pObject3D->Init();                                   //初期化処理
			pObject3D->m_fWidth = fWidth / 2.0f;                 //横幅
			pObject3D->m_fHeight = fHeight / 2.0f;               //高さ
			pObject3D->m_Pos = pos;                              //位置

			//頂点バッファのロック
			pObject3D->m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			//頂点座標の設定（初期位置）
			pVtx[0].pos = D3DXVECTOR3(- pObject3D->m_fWidth,0.0f,pObject3D->m_fHeight);
			pVtx[1].pos = D3DXVECTOR3(pObject3D->m_fWidth, 0.0f,pObject3D->m_fHeight);
			pVtx[2].pos = D3DXVECTOR3(- pObject3D->m_fWidth, 0.0f,- pObject3D->m_fHeight);
			pVtx[3].pos = D3DXVECTOR3(pObject3D->m_fWidth, 0.0f,- pObject3D->m_fHeight);

			//頂点バッファのアンロック
			pObject3D->m_pVtxBuff->Unlock();

			pObject3D->CObject::SetType(CObject::TYPE_PLAYER);   //オブジェクトの種類を設定
		}
	}
	else
	{//オブジェクトの生成が失敗
		delete pObject3D;
		pObject3D = nullptr;
		return nullptr;
	}
	return pObject3D;
}
//================================================================================================================================================
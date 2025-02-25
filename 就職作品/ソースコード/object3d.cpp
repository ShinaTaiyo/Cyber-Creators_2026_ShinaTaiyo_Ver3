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
CObject3D::CObject3D(int nPri, bool bUseintPri, CObject::TYPE type, CObject::OBJECTTYPE ObjType) : CObject(nPri,bUseintPri,type,ObjType),m_fWidth(0.0f),m_fHeight(0.0f),m_nTextureIndex(0),m_Pos(D3DXVECTOR3(0.0f,0.0f,0.0f)),m_Rot(D3DXVECTOR3(0.0f,0.0f,0.0f)),m_mtxWorld(),
m_pVtxBuff(nullptr),m_pTexture(nullptr)
{

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
	m_Pos = D3DXVECTOR3(0.0f,0.0f,0.0f);                        //位置
	m_Rot = D3DXVECTOR3(0.0f,0.0f,0.0f);                        //向き
	m_mtxWorld = {};                                            //マトリックスワールド
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
	pVtx[0].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	pVtx[3].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);

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

	CObject::Init();//オブジェクト初期化処理

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

	CObject::Uninit();//オブジェクト終了処理
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

	CObject::Update();//オブジェクトの更新処理
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

	CObject::Draw();//オブジェクトの描画処理
}
//================================================================================================================================================

//================================================
//死亡フラグ設定処理
//================================================
void CObject3D::SetDeath()
{
	CObject::SetDeath();//オブジェクトの死亡フラグ設定処理
}
//================================================================================================================================================

//================================================
//テクスチャ割り当て処理
//================================================
void CObject3D::bindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;//テクスチャアドレスを割り当てる
}
//================================================================================================================================================
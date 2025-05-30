//=======================================================================================================================================================================================================================
//
// 天井の処理
// Author : Atsumu Kuboichi
//
//=======================================================================================================================================================================================================================
#include "ceiling.h"
#include "field.h"

//=======================================================================================================================================================================================================================
// グローバル変数
//=======================================================================================================================================================================================================================
Ceiling g_aCeiling;	//構造体の情報

//=======================================================================================================================================================================================================================
// 初期化処理
//=======================================================================================================================================================================================================================
void InitCeiling(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスの取得
	VERTEX_3D* pVtx;						//頂点情報へのポインタ

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Ceiling000.png",
		&g_aCeiling.pTexture);

	//変数
	g_aCeiling.pos = D3DXVECTOR3(0.0f, 1500.0f, 0.0f);	//位置
	g_aCeiling.rot = D3DXVECTOR3(0.0f, 0.0f, D3DX_PI);	//向き
	g_aCeiling.fWidth = FIELD_WIDTH;					//横幅
	g_aCeiling.fHeight = FIELD_HEIGHT;					//縦幅

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_aCeiling.pVtxBuff,
		NULL);

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_aCeiling.pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-g_aCeiling.fWidth, 0.0f, g_aCeiling.fHeight);
	pVtx[1].pos = D3DXVECTOR3(g_aCeiling.fWidth, 0.0f, g_aCeiling.fHeight);
	pVtx[2].pos = D3DXVECTOR3(-g_aCeiling.fWidth, 0.0f, -g_aCeiling.fHeight);
	pVtx[3].pos = D3DXVECTOR3(g_aCeiling.fWidth, 0.0f, -g_aCeiling.fHeight);

	//norの設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	g_aCeiling.pVtxBuff->Unlock();
}

//=======================================================================================================================================================================================================================
// 終了処理
//=======================================================================================================================================================================================================================
void UninitCeiling(void)
{
	//テクスチャの破棄
	if (g_aCeiling.pTexture != NULL)
	{//テクスチャ情報が存在する場合
		g_aCeiling.pTexture->Release();
		g_aCeiling.pTexture = NULL;
	}

	//頂点バッファの破棄
	if (g_aCeiling.pVtxBuff != NULL)
	{//頂点情報が存在する場合
		g_aCeiling.pVtxBuff->Release();
		g_aCeiling.pVtxBuff = NULL;
	}
}

//=======================================================================================================================================================================================================================
// 描画処理
//=======================================================================================================================================================================================================================
void DrawCeiling(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;			//計算用マトリックス

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_aCeiling.mtxworld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aCeiling.rot.y, g_aCeiling.rot.x, g_aCeiling.rot.z);
	D3DXMatrixMultiply(&g_aCeiling.mtxworld, &g_aCeiling.mtxworld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_aCeiling.pos.x, g_aCeiling.pos.y, g_aCeiling.pos.z);
	D3DXMatrixMultiply(&g_aCeiling.mtxworld, &g_aCeiling.mtxworld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_aCeiling.mtxworld);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_aCeiling.pVtxBuff, 0, sizeof(VERTEX_3D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_aCeiling.pTexture);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//========================================================
//
//９月２６日：ハッカソンの基盤作り[Billboard.cpp]
//Author:ShinaTaiyo
//
//=========================================================

#include "billboard.h"
#include "main.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
//グローバル変数
PDIRECT3DVERTEXBUFFER9 g_pVtxBuffBillboard = NULL;//頂点バッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureBillboard = NULL;//テクスチャへのポインタ
D3DXVECTOR3 g_posBillboard;//位置
//D3DXVECTOR3 g_rotBillboard;//向き
D3DXMATRIX g_mtxWorldBillboard;//ワールドマトリックス(float4*4)

#define BILLBOARD_SCALE (100.0f)

//タイトル画面の初期化処理
void InitBillboard(void)
{


	//bFadeFlag = false;

	//g_nCntTime = 0;

	//g_nColorChange = 255;//カラーの初期化（白色）

	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\bullet000.png",
		&g_pTextureBillboard);

	g_posBillboard = D3DXVECTOR3(0.0f, 50.0f, 100.0f);//位置の初期化

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,//頂点の数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBillboard,
		NULL);


	////タイトル画面BGMの設定
	//PlaySound(SOUND_LABEL_BGM000);

	VERTEX_3D* pVtx;
	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBillboard->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定（初期位置）
	pVtx[0].pos = D3DXVECTOR3(-BILLBOARD_SCALE, BILLBOARD_SCALE * 2, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(BILLBOARD_SCALE, BILLBOARD_SCALE * 2, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-BILLBOARD_SCALE, 0.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(BILLBOARD_SCALE, 0.0f, 0.0f);
	//norの設定（初期設定）
	pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

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

	//頂点バッファをアンロックする
	g_pVtxBuffBillboard->Unlock();
}

//==============================
//タイトル画面の終了処理
//==============================
void UninitBillboard(void)
{
	//テクスチャの破棄
	if (g_pTextureBillboard != NULL)
	{
		g_pTextureBillboard->Release();
		g_pTextureBillboard = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffBillboard != NULL)
	{
		g_pVtxBuffBillboard->Release();
		g_pVtxBuffBillboard = NULL;
	}
}
//============================
//タイトル画面の更新処理
//============================
void UpdateBillboard(void)
{

}



//================================
//タイトル画面の描画処理
//================================
void DrawBillboard(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

		//デバイスの取得
	pDevice = GetDevice();

	D3DXMATRIX mtxTrans;//計算用マトリックス

	D3DXMATRIX mtxView;//ビューマトリックス取得用

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldBillboard);

	//ビューマトリックスを取得
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	//ポリゴンをカメラに対して正面に向ける
	D3DXMatrixInverse(&g_mtxWorldBillboard, NULL, &mtxView);

	//逆行列を求める
	g_mtxWorldBillboard._41 = 0.0f;
	g_mtxWorldBillboard._42 = 0.0f;
	g_mtxWorldBillboard._43 = 0.0f;

	////向きを反映
	//D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotBillboard.y, g_rotBillboard.x, g_rotBillboard.z);

	//D3DXMatrixMultiply(&g_mtxWorldBillboard, &g_mtxWorldBillboard, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_posBillboard.x, g_posBillboard.y, g_posBillboard.z);
	D3DXMatrixMultiply(&g_mtxWorldBillboard, &g_mtxWorldBillboard, &mtxTrans);

	//ライトを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldBillboard);

	//ライトを有効に戻す
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBillboard, 0, sizeof(VERTEX_3D));//頂点バッファへのポインタと頂点情報の構造体のサイズ

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureBillboard);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);//0は、描画する最初の頂点インデックスで、2は、描画するプリミティブ
}
//========================================================
//
//９月２６日：ハッカソンの基盤作り[Polygon.cpp]
//Author:ShinaTaiyo
//
//=========================================================

#include "Polygon.h"
#include "main.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
//グローバル変数
PDIRECT3DVERTEXBUFFER9 g_pVtxBuffPolygon = NULL;//頂点バッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTexturePolygon = NULL;//テクスチャへのポインタ
D3DXVECTOR3 g_posPolygon;//位置
D3DXVECTOR3 g_rotPolygon;//向き
D3DXMATRIX g_mtxWorldPolygon;//ワールドマトリックス(float4*4)

#define POLYGON_SCALE (10.0f)

//タイトル画面の初期化処理
void InitPolygon(void)
{


	//bFadeFlag = false;

	//g_nCntTime = 0;

	//g_nColorChange = 255;//カラーの初期化（白色）

	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\gametitle.png",
		&g_pTexturePolygon);

	g_posPolygon = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//位置の初期化
	g_rotPolygon = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//向きの初期化

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,//頂点の数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPolygon,
		NULL);


	////タイトル画面BGMの設定
	//PlaySound(SOUND_LABEL_BGM000);

	VERTEX_3D* pVtx;
	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定（初期位置）
	pVtx[0].pos = D3DXVECTOR3(-POLYGON_SCALE, 0.0f, POLYGON_SCALE);
	pVtx[1].pos = D3DXVECTOR3(POLYGON_SCALE, 0.0f, POLYGON_SCALE);
	pVtx[2].pos = D3DXVECTOR3(-POLYGON_SCALE, 0.0f,-POLYGON_SCALE);
	pVtx[3].pos = D3DXVECTOR3(POLYGON_SCALE, 0.0f,-POLYGON_SCALE);

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

	//頂点バッファをアンロックする
	g_pVtxBuffPolygon->Unlock();
}

//==============================
//タイトル画面の終了処理
//==============================
void UninitPolygon(void)
{
	//テクスチャの破棄
	if (g_pTexturePolygon != NULL)
	{
		g_pTexturePolygon->Release();
		g_pTexturePolygon = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffPolygon != NULL)
	{
		g_pVtxBuffPolygon->Release();
		g_pVtxBuffPolygon = NULL;
	}
}
//============================
//タイトル画面の更新処理
//============================
void UpdatePolygon(void)
{
	
}



//================================
//タイトル画面の描画処理
//================================
void DrawPolygon(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

		//デバイスの取得
	pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;//計算用マトリックス

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldPolygon);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotPolygon.y, g_rotPolygon.x, g_rotPolygon.z);

	D3DXMatrixMultiply(&g_mtxWorldPolygon, &g_mtxWorldPolygon, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_posPolygon.x, g_posPolygon.y, g_posPolygon.z);
	D3DXMatrixMultiply(&g_mtxWorldPolygon, &g_mtxWorldPolygon, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldPolygon);


	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPolygon, 0, sizeof(VERTEX_3D));//頂点バッファへのポインタと頂点情報の構造体のサイズ

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTexturePolygon);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);//0は、描画する最初の頂点インデックスで、2は、描画するプリミティブ
}
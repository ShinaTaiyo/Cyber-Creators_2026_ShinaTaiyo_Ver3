//========================================================
//
//８月３０日：エフェクトもう一回見直す[bg.cpp]
//Author:ShinaTaiyo
//
//=========================================================
#include "main.h"
#include "bg.h"

//マクロ定義
#define NUM_BG (1)//背景の数

//グローバル変数
LPDIRECT3DTEXTURE9 g_apTextureBG[NUM_BG] = {};//テクスチャ3枚分のポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBG = NULL;//頂点バッファへのポインタ
float g_aPosTexV[NUM_BG];//テクスチャ座標の開始位置（V値）

float g_fColorChangeBg;

//============================
//背景の初期化処理
//============================
void InitBG(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntBG;

	g_fColorChangeBg = 0.0f;

	//デバイスの取得
	pDevice = GetDevice();

	D3DXCreateTextureFromFile(pDevice,
		"data\\Game_bg.png",
		&g_apTextureBG[0]);

	//====================================
	//背景を複数枚使いたくなったら使う
	//====================================
	//D3DXCreateTextureFromFile(pDevice,
	//	"data\\new_bg_000.png",
	//	&g_apTextureBG[1]);

	//D3DXCreateTextureFromFile(pDevice,
	//	"data\\new_bg_001.png",
	//	&g_apTextureBG[2]);
	//==============================================================

	//テクスチャ座標の開始位置（V値）の初期化
	for (nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
	{
		g_aPosTexV[nCntBG] = 0.0f;
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_BG,//４は必要な頂点数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBG,
		NULL);

	VERTEX_2D* pVtx;
	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffBG->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
	{
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f,SCREEN_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH,SCREEN_HEIGHT, 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
		pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
		pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
		pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f,g_aPosTexV[nCntBG]);
		pVtx[1].tex = D3DXVECTOR2(1.0f,g_aPosTexV[nCntBG]);
		pVtx[2].tex = D3DXVECTOR2(0.0f,g_aPosTexV[nCntBG]+1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f,g_aPosTexV[nCntBG]+1.0f);

		pVtx += 4;//頂点データのポインタを４つ分進める。
		
	}
	//頂点バッファをアンロックする
	g_pVtxBuffBG->Unlock();
}

//==========================
//背景の終了処理
//==========================
void UninitBG(void)
{
	//テクスチャの破棄
	int nCntBG;
	for (nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
	{
		if (g_apTextureBG[nCntBG] != NULL)
		{
			g_apTextureBG[nCntBG]->Release();
			g_apTextureBG[nCntBG] = NULL;
		}
	}
	//頂点バッファの破棄
	if (g_pVtxBuffBG != NULL)
	{
		g_pVtxBuffBG->Release();
		g_pVtxBuffBG = NULL;
	}
}

//==================================
//背景の更新処理
//==================================
void UpDateBG(void)
{
	int nCntBG;

	VERTEX_2D* pVtx;

	g_fColorChangeBg = 1.0f;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffBG->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
	{
		//テクスチャ座標の設定

		switch (nCntBG)
		{
		case 0:
			/*g_aPosTexV[nCntBG] -= 0.0025f;*/ //背景動かしたいなら使う。



			pVtx[0].col = D3DXCOLOR(g_fColorChangeBg, g_fColorChangeBg, g_fColorChangeBg, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
			pVtx[1].col = D3DXCOLOR(g_fColorChangeBg, g_fColorChangeBg, g_fColorChangeBg, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
			pVtx[2].col = D3DXCOLOR(g_fColorChangeBg, g_fColorChangeBg, g_fColorChangeBg, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
			pVtx[3].col = D3DXCOLOR(g_fColorChangeBg, g_fColorChangeBg, g_fColorChangeBg, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。


			/*pVtx[0].tex = D3DXVECTOR2(0.0f, g_aPosTexV[nCntBG]);
			pVtx[1].tex = D3DXVECTOR2(1.0f, g_aPosTexV[nCntBG]);
			pVtx[2].tex = D3DXVECTOR2(0.0f, g_aPosTexV[nCntBG] + 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, g_aPosTexV[nCntBG] + 1.0f);*/

			break;


			//==================================
			//使いたくなったら使う
			//==================================

		//case 1:
		//	g_aPosTexV[nCntBG] -= 0.004f;



		//	pVtx[0].col = D3DXCOLOR(g_fColorChangeBg, g_fColorChangeBg, g_fColorChangeBg, 0.2f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
		//	pVtx[1].col = D3DXCOLOR(g_fColorChangeBg, g_fColorChangeBg, g_fColorChangeBg, 0.2f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
		//	pVtx[2].col = D3DXCOLOR(g_fColorChangeBg, g_fColorChangeBg, g_fColorChangeBg, 0.2f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
		//	pVtx[3].col = D3DXCOLOR(g_fColorChangeBg, g_fColorChangeBg, g_fColorChangeBg, 0.2f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。


		//	pVtx[0].tex = D3DXVECTOR2(0.0f, g_aPosTexV[nCntBG]);
		//	pVtx[1].tex = D3DXVECTOR2(1.0f, g_aPosTexV[nCntBG]);
		//	pVtx[2].tex = D3DXVECTOR2(0.0f, g_aPosTexV[nCntBG] + 1.0f);
		//	pVtx[3].tex = D3DXVECTOR2(1.0f, g_aPosTexV[nCntBG] + 1.0f);

		//	break;

		//case 2:
		//	g_aPosTexV[nCntBG] -= 0.006f;



		//	pVtx[0].col = D3DXCOLOR(g_fColorChangeBg, g_fColorChangeBg, g_fColorChangeBg, 0.2f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
		//	pVtx[1].col = D3DXCOLOR(g_fColorChangeBg, g_fColorChangeBg, g_fColorChangeBg, 0.2f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
		//	pVtx[2].col = D3DXCOLOR(g_fColorChangeBg, g_fColorChangeBg, g_fColorChangeBg, 0.2f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
		//	pVtx[3].col = D3DXCOLOR(g_fColorChangeBg, g_fColorChangeBg, g_fColorChangeBg, 0.2f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。


		//	pVtx[0].tex = D3DXVECTOR2(0.0f, g_aPosTexV[nCntBG]);
		//	pVtx[1].tex = D3DXVECTOR2(1.0f, g_aPosTexV[nCntBG]);
		//	pVtx[2].tex = D3DXVECTOR2(0.0f, g_aPosTexV[nCntBG] + 1.0f);
		//	pVtx[3].tex = D3DXVECTOR2(1.0f, g_aPosTexV[nCntBG] + 1.0f);

		//	break;
		}

		//============================================================================================================================================

		pVtx += 4;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffBG->Unlock();
}

//==================================
//背景の描画処理
//==================================
void DrawBG(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntBG;
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBG, 0, sizeof(VERTEX_2D));//頂点バッファへのポインタと頂点情報の構造体のサイズ

		//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_apTextureBG[nCntBG]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
			nCntBG * 4,
			2);
	}
}
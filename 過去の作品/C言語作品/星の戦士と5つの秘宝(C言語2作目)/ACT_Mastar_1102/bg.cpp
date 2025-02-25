//========================================================
//
//８月３０日：エフェクトもう一回見直す[bg.cpp]
//Author:ShinaTaiyo
//
//=========================================================
#include "main.h"
#include "bg.h"
#include "Item.h"
#include "character.h"
#include "map.h"
//マクロ定義
#define NUM_BG (BG_MAX)//背景の数

//グローバル変数
LPDIRECT3DTEXTURE9 g_apTextureBG[NUM_BG] = {};//テクスチャ3枚分のポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBG = NULL;//頂点バッファへのポインタ
float g_aPosTexV;//テクスチャ座標の開始位置（V値）

float g_fColorChangeBg;

int g_nCntTime;

int g_nBG = 0;

int g_nUaeMap;

//============================
//背景の初期化処理
//============================
void InitBG(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntBG;

	g_nCntTime = 0;

	 g_nBG = 0;

	 g_nUaeMap = 0;

	g_fColorChangeBg = 1.0f;

	//デバイスの取得
	pDevice = GetDevice();

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\BG_CentralCircle.png",
		&g_apTextureBG[BG_CENTRALCIRCLE]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Grassland_000.jpg",
		&g_apTextureBG[BG_STAGE1]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\SnowFieldBG4.jpg",
		&g_apTextureBG[BG_STAGE2]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\BG_Volcano.jpg",
		&g_apTextureBG[BG_STAGE3]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\BG_OnCloud.jpg",
		&g_apTextureBG[BG_STAGE4]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\BG_FinalBattle.jpg",
		&g_apTextureBG[BG_LASTSTAGE]);

	/*D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Grassland_000.jpg",
		&g_apTextureBG[BG_STAGE1]);*/

	////====================================
	////背景を複数枚使いたくなったら使う
	////====================================
	//D3DXCreateTextureFromFile(pDevice,
	//	"data\\Texture\\inseki.png",
	//	&g_apTextureBG[1]);

	//D3DXCreateTextureFromFile(pDevice,
	//	"data\\Texture\\insekiyuka.png",
	//	&g_apTextureBG[2]);
	//==============================================================

	//テクスチャ座標の開始位置（V値）の初期化
	//for (nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
	//{
		g_aPosTexV = 0.0f;
	//}

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

	/*for (nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
	{*/
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH , 0.0f, 0.0f);
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
		pVtx[0].tex = D3DXVECTOR2(g_aPosTexV,0.0f);
		pVtx[1].tex = D3DXVECTOR2(g_aPosTexV+1.0f,0.0f);
		pVtx[2].tex = D3DXVECTOR2(g_aPosTexV,1.0f);
		pVtx[3].tex = D3DXVECTOR2(g_aPosTexV+1.0f,1.0f);

	//	pVtx += 4;//頂点データのポインタを４つ分進める。
	//	
	//}
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

	CharacterNum* pCharacterNum = GetCharacterNum();


	ItemEffect* pItemEffect = GetItemEffect();

	int *pUseMap = GetUseMap();//使っているマップがどれか判定する

	//====================================
	//背景を決定する処理
	//====================================
	if (*pUseMap == MAPTYPE_8)
	{//「セントラルサークル」の背景
		g_nBG = BG_CENTRALCIRCLE;
	}
	else if (*pUseMap >= MAPTYPE_0 && *pUseMap <= MAPTYPE_5)
	{//「ステージ１」の背景
		g_nBG = BG_STAGE1;
	}
	else if (*pUseMap >= MAPTYPE_6 && *pUseMap <= 7)
	{//「ステージ２」の背景
		g_nBG = BG_STAGE2;
	}
	else if (*pUseMap >= MAPTYPE_9 && *pUseMap <= MAPTYPE_13)
	{//「ステージ２」の背景
		g_nBG = BG_STAGE2;
	}
	else if (*pUseMap >= MAPTYPE_14 && *pUseMap <= MAPTYPE_22)
	{//「ステージ3」の背景
		g_nBG = BG_STAGE3;
	}
	else if (*pUseMap >= MAPTYPE_23 && *pUseMap <= MAPTYPE_31)
	{//「ステージ４」の背景
		g_nBG = BG_STAGE4;

	}
	else if (*pUseMap >= MAPTYPE_32/* && *pUseMap <= MAPTYPE_31*/)
	{//「ラストステージ」の背景
		g_nBG = BG_LASTSTAGE;

	}
	//====================================================================
	

	//else if(*pUseMap )

	//g_nCntTime++;

	if (g_nCntTime == 1)
	{
		if (pCharacterNum->nCharacterNum == 0)
		{
			pItemEffect->fmoveBg += 0.002f;
		}
	}

	VERTEX_2D* pVtx;

	

	g_fColorChangeBg = 1.0f;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffBG->Lock(0, 0, (void**)&pVtx, 0);

	//for (nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
	//{
	//
	//	switch (nCntBG)
	//	{
	//	case 0:
	//		/*g_aPosTexV[nCntBG] -= 0.0025f;*/ //背景動かしたいなら使う。

				//テクスチャ座標の設定
			g_aPosTexV += 0.001f;
			
			pVtx[0].col = D3DXCOLOR(g_fColorChangeBg, g_fColorChangeBg, g_fColorChangeBg, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
			pVtx[1].col = D3DXCOLOR(g_fColorChangeBg, g_fColorChangeBg, g_fColorChangeBg, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
			pVtx[2].col = D3DXCOLOR(g_fColorChangeBg, g_fColorChangeBg, g_fColorChangeBg, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
			pVtx[3].col = D3DXCOLOR(g_fColorChangeBg, g_fColorChangeBg, g_fColorChangeBg, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。


		////テクスチャ座標の設定
		//	pVtx[0].tex = D3DXVECTOR2(g_aPosTexV, 0.0f);
		//	pVtx[1].tex = D3DXVECTOR2(g_aPosTexV + 1.0f, 0.0f);
		//	pVtx[2].tex = D3DXVECTOR2(g_aPosTexV, 1.0f);
		//	pVtx[3].tex = D3DXVECTOR2(g_aPosTexV + 1.0f, 1.0f);

			/*break;*/


			//==================================
			//使いたくなったら使う
			//==================================

		//case 1:
		//	g_aPosTexV[nCntBG] += 0.002f;

		//	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		//	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
		//	pVtx[2].pos = D3DXVECTOR3(0.0f,80.0f, 0.0f);
		//	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, 80.0f, 0.0f);

		//	pVtx[0].col = D3DXCOLOR(g_fColorChangeBg, g_fColorChangeBg, g_fColorChangeBg, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
		//	pVtx[1].col = D3DXCOLOR(g_fColorChangeBg, g_fColorChangeBg, g_fColorChangeBg, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
		//	pVtx[2].col = D3DXCOLOR(g_fColorChangeBg, g_fColorChangeBg, g_fColorChangeBg, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
		//	pVtx[3].col = D3DXCOLOR(g_fColorChangeBg, g_fColorChangeBg, g_fColorChangeBg, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。


		//	pVtx[0].tex = D3DXVECTOR2(g_aPosTexV[nCntBG]+pItemEffect->fmoveBg, 0.0f);
		//	pVtx[1].tex = D3DXVECTOR2(g_aPosTexV[nCntBG]+pItemEffect->fmoveBg + 1.0f, 0.0f);
		//	pVtx[2].tex = D3DXVECTOR2(g_aPosTexV[nCntBG]+pItemEffect->fmoveBg, 1.0f);
		//	pVtx[3].tex = D3DXVECTOR2(g_aPosTexV[nCntBG]+pItemEffect->fmoveBg + 1.0f, 1.0f);

		//	break;

		//case 2:
		//	g_aPosTexV[nCntBG] += 0.002f;

		//	pVtx[0].pos = D3DXVECTOR3(0.0f,SCREEN_HEIGHT-80.0f, 0.0f);
		//	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH,SCREEN_HEIGHT-80.0f, 0.0f);
		//	pVtx[2].pos = D3DXVECTOR3(0.0f,SCREEN_HEIGHT, 0.0f);
		//	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH,SCREEN_HEIGHT, 0.0f);

		//	pVtx[0].col = D3DXCOLOR(g_fColorChangeBg, g_fColorChangeBg, g_fColorChangeBg, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
		//	pVtx[1].col = D3DXCOLOR(g_fColorChangeBg, g_fColorChangeBg, g_fColorChangeBg, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
		//	pVtx[2].col = D3DXCOLOR(g_fColorChangeBg, g_fColorChangeBg, g_fColorChangeBg, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
		//	pVtx[3].col = D3DXCOLOR(g_fColorChangeBg, g_fColorChangeBg, g_fColorChangeBg, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。


		//	pVtx[0].tex = D3DXVECTOR2(g_aPosTexV[nCntBG], 0.0f);
		//	pVtx[1].tex = D3DXVECTOR2(g_aPosTexV[nCntBG] + 1.0f, 0.0f);
		//	pVtx[2].tex = D3DXVECTOR2(g_aPosTexV[nCntBG], 1.0f);
		//	pVtx[3].tex = D3DXVECTOR2(g_aPosTexV[nCntBG] + 1.0f, 1.0f);

		//	break;
	//	}

	//	//============================================================================================================================================

	//	pVtx += 4;
	//}
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

	//for (nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
	//{
		//テクスチャの設定
	if (g_nBG == BG_CENTRALCIRCLE)
	{
		pDevice->SetTexture(0, g_apTextureBG[BG_CENTRALCIRCLE]);
	}
	else if (g_nBG == BG_STAGE1)
	{
		pDevice->SetTexture(0, g_apTextureBG[BG_STAGE1]);
	}
	else if (g_nBG == BG_STAGE2)
	{
		pDevice->SetTexture(0, g_apTextureBG[BG_STAGE2]);
	}
	else if (g_nBG == BG_STAGE3)
	{
		pDevice->SetTexture(0, g_apTextureBG[BG_STAGE3]);
	}
	else if (g_nBG == BG_STAGE4)
	{
		pDevice->SetTexture(0, g_apTextureBG[BG_STAGE4]);
	}
	else if (g_nBG == BG_LASTSTAGE)
	{
		pDevice->SetTexture(0, g_apTextureBG[BG_LASTSTAGE]);
	}
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
			0,
			2);
	//}
}
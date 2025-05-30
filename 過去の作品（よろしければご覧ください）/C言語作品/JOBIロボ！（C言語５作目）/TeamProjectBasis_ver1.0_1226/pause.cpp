//========================================================
//
//１２月２６日：チーム制作の基盤作り[pause.cpp]
//Author:ShinaTaiyo
//
//=========================================================

#include "result.h"
#include "main.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "pause.h"


#define COMMAND_SIZE_X (160.0f)
#define COMMAND_SIZE_Y (40.0f)

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTexturePauseBG = NULL;//ポーズ背景テクスチャへのポインタ
PDIRECT3DVERTEXBUFFER9 g_pVtxBuffPauseBG = NULL;//ポーズ背景の頂点バッファへのポインタ

LPDIRECT3DTEXTURE9 g_pTexturePause[NUM_PAUSETEX] = {};//テクスチャへのポインタ
PDIRECT3DVERTEXBUFFER9 g_pVtxBuffPause = NULL;//頂点バッファへのポインタ
Pause g_aPause[NUM_PAUSETEX];

//リザルト画面の初期化処理
void InitPause(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//================================
	//ポーズ画面背景の初期化処理
	//================================
	

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\PauseBg_000.png",
		&g_pTexturePauseBG);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPauseBG,
		NULL);

	VERTEX_2D* pVtx;
	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPauseBG->Lock(0, 0, (void**)&pVtx, 0);
	
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	//rhwの設定（初期設定）
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定（初期設定）
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//テクスチャ座標の設定（初期設定）
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffPauseBG->Unlock();

	//================================
	//ポーズコマンド初期化処理
	//================================

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\continue_001.png",
		&g_pTexturePause[0]);


	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\retry_001.png",
		&g_pTexturePause[1]);


	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\quit_001.png",
		&g_pTexturePause[2]);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_PAUSETEX,//頂点の数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPause,
		NULL);

	for (int nCntPause = 0; nCntPause < NUM_PAUSETEX; nCntPause++)
	{
		
		g_aPause[nCntPause].pos = D3DXVECTOR3(1060.0f, 350.0f + ((COMMAND_SIZE_Y * 2.0f) + 50.0f) * nCntPause, 0.0f);//真ん中に三段オプションコマンドを用意する。
	}
	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);



	for (int nCntPause = 0; nCntPause < NUM_PAUSETEX; nCntPause++)
	{
		//頂点座標の設定（初期位置）
		g_aPause[nCntPause].nPauseMenu = nCntPause;//4つのポリゴンにテクスチャを割り当てる。
		g_aPause[nCntPause].bChooseMenu = false;
		//コマンドの位置情報の初期化
			pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			//rhwの設定（初期設定）
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			//頂点カラーの設定（初期設定）
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

			//テクスチャ座標の設定（初期設定）
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		

		pVtx += 4;//頂点座標のポインタをインクリメント
	}

	//頂点バッファをアンロックする
	g_pVtxBuffPause->Unlock();

	//==========================================================================================================
}

//==============================
//リザルト画面の終了処理
//==============================
void UninitPause(void)
{
	//ポーズ背景のテクスチャの破棄
	if (g_pTexturePauseBG != NULL)
	{
		g_pTexturePauseBG->Release();
		g_pTexturePauseBG = NULL;
	}

	//ポーズ背景の頂点バッファの破棄
	if (g_pVtxBuffPauseBG != NULL)
	{
		g_pVtxBuffPauseBG->Release();
		g_pVtxBuffPauseBG = NULL;
	}


	//ポーズコマンドのテクスチャの破棄
	for (int nCntPause = 0; nCntPause < NUM_PAUSETEX; nCntPause++)
	{
		if (g_pTexturePause[nCntPause] != NULL)
		{
			g_pTexturePause[nCntPause]->Release();
			g_pTexturePause[nCntPause] = NULL;
		}
	}
	//ポーズコマンドの頂点バッファの破棄
	if (g_pVtxBuffPause != NULL)
	{
		g_pVtxBuffPause->Release();
		g_pVtxBuffPause = NULL;
	}
}

//===================================
//リザルト画面の更新処理
//===================================
void UpdatePause(void)
{
	int nCntPause;
	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntPause = 0; nCntPause < NUM_PAUSETEX; nCntPause++)
	{
		//頂点座標の設定（初期位置）
		pVtx[0].pos = D3DXVECTOR3(g_aPause[nCntPause].pos.x- COMMAND_SIZE_X,g_aPause[nCntPause].pos.y-COMMAND_SIZE_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aPause[nCntPause].pos.x+ COMMAND_SIZE_X,g_aPause[nCntPause].pos.y-COMMAND_SIZE_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aPause[nCntPause].pos.x- COMMAND_SIZE_X,g_aPause[nCntPause].pos.y+COMMAND_SIZE_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aPause[nCntPause].pos.x+ COMMAND_SIZE_X,g_aPause[nCntPause].pos.y+COMMAND_SIZE_Y, 0.0f);

		//頂点カラーの設定（初期設定）
		pVtx[0].col = D3DCOLOR_RGBA(255,255,255,255);
		pVtx[1].col = D3DCOLOR_RGBA(255,255,255,255);
		pVtx[2].col = D3DCOLOR_RGBA(255,255,255,255);
		pVtx[3].col = D3DCOLOR_RGBA(255,255,255,255);

		if (g_aPause[nCntPause].bChooseMenu == true)
		{//	選ばれているコマンドだけ明るくする
			pVtx[0].col = D3DCOLOR_RGBA(0, 255, 255, 255);
			pVtx[1].col = D3DCOLOR_RGBA(0, 255, 255, 255);
			pVtx[2].col = D3DCOLOR_RGBA(0, 255, 255, 255);
			pVtx[3].col = D3DCOLOR_RGBA(0, 255, 255, 255);
		}
		pVtx += 4;//頂点座標のポインタをインクリメント
	}
	//頂点バッファをアンロックする
	g_pVtxBuffPause->Unlock();
}

//================================
//タイトル画面の描画処理
//================================
void DrawPause(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPauseBG, 0, sizeof(VERTEX_2D));//頂点バッファへのポインタと頂点情報の構造体のサイズ

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	pDevice->SetTexture(0, g_pTexturePauseBG);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,0, 2);//0は、描画する最初の頂点インデックスで、2は、描画するプリミティブ数

	//=======================================================================================================================

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPause, 0, sizeof(VERTEX_2D));//頂点バッファへのポインタと頂点情報の構造体のサイズ

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);


	for (int nCntPause = 0; nCntPause < NUM_PAUSETEX; nCntPause++)
	{
		//テクスチャの設定
		if (g_aPause[nCntPause].nPauseMenu == nCntPause)
		{
			pDevice->SetTexture(0, g_pTexturePause[nCntPause]);
		}
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntPause * 4, 2);//0は、描画する最初の頂点インデックスで、2は、描画するプリミティブ数
	}
}

//================================
//リザルト画面分け構造体の取得
//================================

void SetEnablePause(int nType)
{
	int nCntPause;
	for (nCntPause = 0; nCntPause < NUM_PAUSETEX; nCntPause++)
	{//セットされるたびにポーズの明るく表示するフラグをfalseにする。
		g_aPause[nCntPause].bChooseMenu = false;
	}
	//セットされたときに、引き数で呼び出したポーズメニューが明るく表示される。
	g_aPause[nType].bChooseMenu = true;
}
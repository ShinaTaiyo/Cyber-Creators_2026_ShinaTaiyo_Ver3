//========================================================
//
//８月３０日：エフェクトもう一回見直す[pause.cpp]
//Author:ShinaTaiyo
//
//=========================================================

#include "result.h"
#include "main.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "pause.h"


#define COMMAND_SIZE_X (76.0f)
#define COMMAND_SIZE_Y (38.0f)

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTexturePause[MAX_PAUSE_TEX] = {};//テクスチャへのポインタ
//VERTEX_2D g_aVertex[4];//頂点情報を格納（四角形を作りたい）
PDIRECT3DVERTEXBUFFER9 g_pVtxBuffPause = NULL;//頂点バッファへのポインタ

Pause g_aPause[MAX_PAUSE_TEX];



//リザルト画面の初期化処理
void InitPause(void)
{
	int nCntPause;

	

	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\3DGame_Pause.png",
		&g_pTexturePause[0]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\CONTINE_indicate.png",
		&g_pTexturePause[1]);


	D3DXCreateTextureFromFile(pDevice,
		"data\\RETRY_indicate.png",
		&g_pTexturePause[2]);


	D3DXCreateTextureFromFile(pDevice,
		"data\\QUIT_indicate.png",
		&g_pTexturePause[3]);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_PAUSE_TEX,//頂点の数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPause,
		NULL);

	for (nCntPause = 1; nCntPause < MAX_PAUSE_TEX; nCntPause++)
	{
		
		g_aPause[nCntPause].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 + 500.0f,SCREEN_HEIGHT / 2  + 90.0f + 80.0f * nCntPause, 0.0f);//真ん中に三段オプションコマンドを用意する。
	}
	VERTEX_2D* pVtx;
	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);



	for (nCntPause = 0; nCntPause < MAX_PAUSE_TEX; nCntPause++)
	{
		//頂点座標の設定（初期位置）
		g_aPause[nCntPause].nPauseMenu = nCntPause;//4つのポリゴンにテクスチャを割り当てる。
		g_aPause[nCntPause].bChooseMenu = false;
		if (g_aPause->nPauseMenu == PAUSE_MENU_INDICATE)
		{//画面全体に表示するテクスチャ
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
		}
		else
		{//コマンドの位置情報の初期化
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
		}

		pVtx += 4;//頂点座標のポインタをインクリメント
	}

	//頂点バッファをアンロックする
	g_pVtxBuffPause->Unlock();
}

//==============================
//リザルト画面の終了処理
//==============================
void UninitPause(void)
{
	int nCntPause;
	//テクスチャの破棄
	for (nCntPause = 0; nCntPause < MAX_PAUSE_TEX; nCntPause++)
	{
		if (g_pTexturePause[nCntPause] != NULL)
		{
			g_pTexturePause[nCntPause]->Release();
			g_pTexturePause[nCntPause] = NULL;
		}
	}
	//頂点バッファの破棄
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
	for (nCntPause = 0; nCntPause < MAX_PAUSE_TEX; nCntPause++)
	{
		//頂点座標の設定（初期位置）

		if (g_aPause[nCntPause].nPauseMenu >=PAUSE_MENU_CONTINUE&&g_aPause[nCntPause].nPauseMenu<=PAUSE_MENU_QUIT)
		{//ポーズ画面テクスチャ以外のコマンドの位置を決定する。


			pVtx[0].pos = D3DXVECTOR3(g_aPause[nCntPause].pos.x- COMMAND_SIZE_X,g_aPause[nCntPause].pos.y-COMMAND_SIZE_Y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aPause[nCntPause].pos.x+ COMMAND_SIZE_X,g_aPause[nCntPause].pos.y-COMMAND_SIZE_Y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aPause[nCntPause].pos.x- COMMAND_SIZE_X,g_aPause[nCntPause].pos.y+COMMAND_SIZE_Y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aPause[nCntPause].pos.x+ COMMAND_SIZE_X,g_aPause[nCntPause].pos.y+COMMAND_SIZE_Y, 0.0f);

			//頂点カラーの設定（初期設定）
			pVtx[0].col = D3DCOLOR_RGBA(255,255,255,100);
			pVtx[1].col = D3DCOLOR_RGBA(255,255,255,100);
			pVtx[2].col = D3DCOLOR_RGBA(255,255,255,100);
			pVtx[3].col = D3DCOLOR_RGBA(255,255,255,100);
		}
		if (g_aPause[nCntPause].bChooseMenu == true)
		{
			//頂点カラーの設定（選択しているコマンドが明るくなる）
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
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

	int nCntPause;
	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPause, 0, sizeof(VERTEX_2D));//頂点バッファへのポインタと頂点情報の構造体のサイズ

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);


	for (nCntPause = 0; nCntPause < MAX_PAUSE_TEX; nCntPause++)
	{
		//テクスチャの設定
		if (g_aPause[nCntPause].nPauseMenu == PAUSE_MENU_INDICATE)
		{
			pDevice->SetTexture(0, g_pTexturePause[PAUSE_MENU_INDICATE]);
		}
		if (g_aPause[nCntPause].nPauseMenu == PAUSE_MENU_CONTINUE)
		{
			pDevice->SetTexture(0, g_pTexturePause[PAUSE_MENU_CONTINUE]);
		}
		if (g_aPause[nCntPause].nPauseMenu == PAUSE_MENU_RETRY)
		{
			pDevice->SetTexture(0, g_pTexturePause[PAUSE_MENU_RETRY]);
		}
		if (g_aPause[nCntPause].nPauseMenu == PAUSE_MENU_QUIT)
		{
			pDevice->SetTexture(0, g_pTexturePause[PAUSE_MENU_QUIT]);
		}
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntPause * 4, 2);//0は、描画する最初の頂点インデックスで、2は、描画するプリミティブ数
			//&g_pVtxBuffPolygon[0],//頂点情報の先頭アドレス
			//sizeof(VERTEX_2D));//頂点情報構造体のサイズ
	}
}

//================================
//リザルト画面分け構造体の取得
//================================

void SetEnablePause(int nType)
{
	int nCntPause;
	for (nCntPause = 0; nCntPause < MAX_PAUSE_TEX; nCntPause++)
	{//セットされるたびにポーズの明るく表示するフラグをfalseにする。
		g_aPause[nCntPause].bChooseMenu = false;
	}
	//セットされたときに、引き数で呼び出したポーズメニューが明るく表示される。
	g_aPause[nType].bChooseMenu = true;
}
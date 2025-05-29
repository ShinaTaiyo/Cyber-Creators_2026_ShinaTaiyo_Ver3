//========================================================
//
//８月３０日：エフェクトもう一回見直す[Warp.cpp]
//Author:ShinaTaiyo
//
//=========================================================
#include "main.h"
#include <stdlib.h>
#include <time.h>
#include "score.h"
#include "game.h"
#include "result.h"
#include "player.h"
#include "sound.h"
#include "fade.h"
#include "health.h"
#include "warp.h"
#include "map.h"
#include "input.h"
//マクロ定義
#define NUM_WarpTEX (10)//ワープの種類
#define FRAME_WarpSIZE (20.0f) //炎ワープの大きさ



//グローバル変数
LPDIRECT3DTEXTURE9 g_apTextureWarp = NULL;
//D3DXVECTOR3 g_rotPI;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffWarp = NULL;
//Enemy g_Warp[MAX_ENEMY];//敵の情報
//Result g_aResult;

Warp g_aWarp[MAX_WARP];

//int g_nCountTime = 0;//時間を表すグローバル変数

//=====================================================
//==========================
//ワープの初期化処理
//==========================
void InitWarp(void)
{
	//srand((unsigned int)time(NULL));//ゲームを開始した時間毎に乱数の種を設定
	LPDIRECT3DDEVICE9 pDevice;
	int	nCntWarp;
	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\WarpHole_000.png",
		&g_apTextureWarp);

	

	//ワープ情報の初期化
	for (nCntWarp = 0; nCntWarp < MAX_WARP; nCntWarp++)
	{
		g_aWarp[nCntWarp].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//ワープの位置
		g_aWarp[nCntWarp].PlayerPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//ワープさせるプレイヤーの位置
		g_aWarp[nCntWarp].nRoadMapType = 0;//ワープの種類
		g_aWarp[nCntWarp].nLife = 0;//ワープのライフ
		g_aWarp[nCntWarp].bUse = false;//ワープが使われているか銅貨
		g_aWarp[nCntWarp].bDraw = false;//bUseがtrueだったことに加え、更にbDrawがtrueかどうかで描画するかどうかを決める。
		g_aWarp[nCntWarp].bMapChange = false;//マップを変えるときに呼び出すフラグ
		g_aWarp[nCntWarp].fXsize = 0.0f;//X方向のサイズ
		g_aWarp[nCntWarp].fYsize = 0.0f;//Y方向のサイズ
		g_aWarp[nCntWarp].fRadius = 0.0f;//半径でサイズを決める場合のサイズ
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_WARP,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffWarp,
		NULL);

	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffWarp->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntWarp = 0; nCntWarp < MAX_WARP; nCntWarp++)
	{
		//頂点座標の設定(敵の大きさ）
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;//頂点データのポインタを４つ分進める。
	}
	//頂点バッファをアンロックする
	g_pVtxBuffWarp->Unlock();
}

//====================================
//チュートリアルの終了処理
//====================================
void UninitWarp(void)
{
	//テクスチャの破棄
	int nCntWarp;
	if (g_apTextureWarp != NULL)
		{
			g_apTextureWarp->Release();
			g_apTextureWarp = NULL;
		}
	//頂点バッファの破棄
	if (g_pVtxBuffWarp != NULL)
	{
		g_pVtxBuffWarp->Release();
		g_pVtxBuffWarp = NULL;
	}
}


//========================================
//敵の更新処理
//========================================
void UpdateWarp(void)
{
	VERTEX_2D* pVtx;
	int nCntWarp;
	Player* pPlayer = GetPlayer();
	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffWarp->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntWarp = 0; nCntWarp < MAX_WARP; nCntWarp++)
	{
		if (g_aWarp[nCntWarp].bUse == true)
		{
			//ワープの位置の設定
			pVtx[0].pos = D3DXVECTOR3(g_aWarp[nCntWarp].pos.x - g_aWarp[nCntWarp].fRadius, g_aWarp[nCntWarp].pos.y - g_aWarp[nCntWarp].fRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aWarp[nCntWarp].pos.x + g_aWarp[nCntWarp].fRadius, g_aWarp[nCntWarp].pos.y - g_aWarp[nCntWarp].fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aWarp[nCntWarp].pos.x - g_aWarp[nCntWarp].fRadius, g_aWarp[nCntWarp].pos.y + g_aWarp[nCntWarp].fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aWarp[nCntWarp].pos.x + g_aWarp[nCntWarp].fRadius, g_aWarp[nCntWarp].pos.y + g_aWarp[nCntWarp].fRadius, 0.0f);



			if (pPlayer->pos.x >= g_aWarp[nCntWarp].pos.x - g_aWarp[nCntWarp].fRadius
				&& pPlayer->pos.x <= g_aWarp[nCntWarp].pos.x + g_aWarp[nCntWarp].fRadius
				&& pPlayer->pos.y - PLAYER_HEIGHT / 2 >= g_aWarp[nCntWarp].pos.y - g_aWarp[nCntWarp].fRadius
				&& pPlayer->pos.y - PLAYER_HEIGHT / 2 <= g_aWarp[nCntWarp].pos.y + g_aWarp[nCntWarp].fRadius)
			{//プレイヤーの中心で判定。マップを呼び出す
				if (g_aWarp[nCntWarp].bMapChange == true)
				{
					ResetWarp();
					g_aWarp[nCntWarp].bMapChange = false;//マップを呼び出したので、フラグをoffにする
					g_aWarp[nCntWarp].bUse = false;
					pPlayer->pos = g_aWarp[nCntWarp].PlayerPos;//プレイヤーをワープさせる
					pPlayer->posOld = g_aWarp[nCntWarp].PlayerPos;//プレイヤーをワープさせる
					SetMap(g_aWarp[nCntWarp].nRoadMapType);

				}
			}
		}
		pVtx += 4;//頂点データのポインタを４つ分進める。
	}
	//頂点バッファをアンロックする
	g_pVtxBuffWarp->Unlock();


}


//=========================================
//敵の描画処理
//=========================================
void DrawWarp(void)
{

	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	int nCntWarp;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffWarp, 0, sizeof(VERTEX_2D));//頂点バッファへのポインタと頂点情報の構造体のサイズ

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定

	for (nCntWarp = 0; nCntWarp < MAX_WARP; nCntWarp++)
	{

		if (g_aWarp[nCntWarp].bUse == true)
		{
			if (g_aWarp[nCntWarp].bDraw == true)
			{
				pDevice->SetTexture(0, g_apTextureWarp);
				
				//ポリゴンの描画
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntWarp * 4, 2);//0は、描画する最初の頂点インデックスで、2は、描画するプリミティブ数

				//&g_pVtxBuffPolygon[0],//頂点情報の先頭アドレス
				//sizeof(VERTEX_2D));//頂点情報構造体のサイズ
			}
		}
	}


}

void ResetWarp(void)//ワープを一旦全部消す
{

	for (int nCntWarp = 0; nCntWarp < MAX_WARP; nCntWarp++)
	{
		if (g_aWarp[nCntWarp].bUse == true)
		{
			g_aWarp[nCntWarp].bUse = false;//ワープが使われているかどうか
		}

	}
}


void SetWarp(int nRoadMapType, D3DXVECTOR3 pos, D3DXVECTOR3 PlayerPos)
{
	int nCntWarp;

	VERTEX_2D* pVtx;

	Player* pPlayer = GetPlayer();
	/*Enemy* pEnemy = GetEnemy();*/



	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffWarp->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntWarp = 0; nCntWarp < MAX_WARP; nCntWarp++)
	{
		if (g_aWarp[nCntWarp].bUse == false)
		{
			g_aWarp[nCntWarp].pos = pos;
			g_aWarp[nCntWarp].PlayerPos = PlayerPos;//プレイヤーをワープさせる位置
			g_aWarp[nCntWarp].nRoadMapType = nRoadMapType;//ワープの種類
			g_aWarp[nCntWarp].nLife = 0;//ワープのライフ
			g_aWarp[nCntWarp].bUse = true;//ワープが使われているか銅貨
			g_aWarp[nCntWarp].bDraw = true;//bUseがtrueだったことに加え、更にbDrawがtrueかどうかで描画するかどうかを決める。
			g_aWarp[nCntWarp].bMapChange = true;//マップを変えるときに呼び出すフラグ
			g_aWarp[nCntWarp].fRadius = 20.0f;//半径でサイズを決める場合のサイズ
			break;
		}
		pVtx += 4;
	}
	g_pVtxBuffWarp->Unlock();
}

Warp* GetWarp(void)
{
	return &g_aWarp[0];
}



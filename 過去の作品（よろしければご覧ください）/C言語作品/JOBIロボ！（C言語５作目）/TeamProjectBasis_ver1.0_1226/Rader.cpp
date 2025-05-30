//=====================================================================================================================
//
//１２月２６日：チーム制作の基盤づくり[Level.cpp]
//Author:ShinaTaiyo
//
// 
// 注意：fRWidth,fRHeightは、中心点からの半径で大きさ、判定などを取っています。もしこの処理を使うなら、ご注意下さい。
// 
//======================================================================================================================

#include "main.h"
#include "sound.h"
#include "player.h"
#include "Rader.h"
#include "enemy.h"
#include "field.h"
#include "debugproc.h"
//グローバル変数

//=============================================================================================

//=================================
//レーダー表示用
//=================================
#define RADER_WIDTH (100.0f)
#define RADER_HEIGHT (100.0f)

LPDIRECT3DTEXTURE9 g_pTextureRader = NULL;//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRader = NULL;//レベル数値の情報
Rader g_aRader;//レーダー構造体

//=================================
//レーダーディスプレイ表示用
//=================================
#define RADERDISPLAY_WIDTH (4.0f)
#define RADERDISPLAY_HEIGHT (4.0f)

LPDIRECT3DTEXTURE9 g_pTextureRaderDisPlay[NUM_RADERDISPLAY] = {};//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRaderDisplay = NULL;//レベルの情報
RaderDisplay g_aRaderDisPlay[MAX_RADERDISPLAY];//レーダーディスプレイ構造体

#define RADERPOSRATIOX (FIELD_WIDTH / RADER_WIDTH)//レーダーに表示する座標と、ワールド座標を割り、割合を求める
#define RADERPOSRATIOY (FIELD_HEIGHT / RADER_HEIGHT)//レーダーに表示する座標と、ワールド座標を割り、割合を求める


//============================
//総合のレーダー処理
//============================
void InitRaderCom(void)
{
	InitRader();
	InitRaderDisplay();
}

void UninitRaderCom(void)
{
	UninitRader();
	UninitRaderDisplay();
}

void UpdateRaderCom(void)
{
	UpdateRader();
	UpdateRaderDisplay();
}

void DrawRaderCom(void)
{
	DrawRader();
	DrawRaderDisplay();
}

//==================================================================

//============================
//レーダーの初期化処理
//============================
void InitRader(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	pDevice = GetDevice();

	VERTEX_2D* pVtx;

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Rader_000.png",
		&g_pTextureRader);


	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRader,
		NULL);

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffRader->Lock(0, 0, (void**)&pVtx, 0);

	g_aRader.pos = D3DXVECTOR3(SCREEN_WIDTH - RADER_WIDTH,200.0f - RADER_HEIGHT, 0.0f);
	g_aRader.rot = NULL_VECTOR3;
	g_aRader.mtxWorld = {};

		pVtx[0].pos = D3DXVECTOR3(g_aRader.pos.x - RADER_WIDTH, g_aRader.pos.y - RADER_HEIGHT, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aRader.pos.x + RADER_WIDTH, g_aRader.pos.y - RADER_HEIGHT, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aRader.pos.x - RADER_WIDTH, g_aRader.pos.y + RADER_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aRader.pos.x + RADER_WIDTH, g_aRader.pos.y + RADER_HEIGHT, 0.0f);

		//rhwの設定（初期設定）
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定（初期設定）
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標の設定（初期設定）
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffRader->Unlock();

}

//========================================
//レーダーディスプレイの初期化処理
//========================================
void InitRaderDisplay(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	pDevice = GetDevice();

	VERTEX_2D* pVtx;

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Rader_Player_000.png",
		&g_pTextureRaderDisPlay[0]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Rader_Enemy_000.png",
		&g_pTextureRaderDisPlay[1]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\RaderDisplay_Boss_000.png",
		&g_pTextureRaderDisPlay[2]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\RaderDisplay_Outlet_000.png",
		&g_pTextureRaderDisPlay[3]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\RaderDisplay_Editor_000.png",
		&g_pTextureRaderDisPlay[4]);


	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_RADERDISPLAY,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRaderDisplay,
		NULL);

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffRaderDisplay->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntRaderDisplay = 0; nCntRaderDisplay < MAX_RADERDISPLAY; nCntRaderDisplay++)
	{
		g_aRaderDisPlay[nCntRaderDisplay].bUse = false;//使用しているか
		g_aRaderDisPlay[nCntRaderDisplay].nType = 0;//種類
		g_aRaderDisPlay[nCntRaderDisplay].pos = NULL_VECTOR3;//位置
		g_aRaderDisPlay[nCntRaderDisplay].rot = NULL_VECTOR3;//向き
		g_aRaderDisPlay[nCntRaderDisplay].size = 0.0f;//大きさ
		g_aRaderDisPlay[nCntRaderDisplay].fAngle = 0.0f;//アングル
		g_aRaderDisPlay[nCntRaderDisplay].fWidth = 0.0f;
		g_aRaderDisPlay[nCntRaderDisplay].fHeight = 0.0f;
		pVtx[0].pos = NULL_VECTOR3;
		pVtx[1].pos = NULL_VECTOR3;
		pVtx[2].pos = NULL_VECTOR3;
		pVtx[3].pos = NULL_VECTOR3;

		//rhwの設定（初期設定）
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定（初期設定）
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標の設定（初期設定）
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffRaderDisplay->Unlock();

}


//==============================
//レーダーの終了処理
//==============================
void UninitRader(void)
{
	if (g_pTextureRader != NULL)
	{
		g_pTextureRader->Release();
		g_pTextureRader = NULL;
	}
	
	//頂点バッファの破棄
	if (g_pVtxBuffRader != NULL)
	{
		g_pVtxBuffRader->Release();
		g_pVtxBuffRader = NULL;
	}
}


//==============================
//レーダーの終了処理
//==============================
void UninitRaderDisplay(void)
{
	//テクスチャの破棄
	for (int nCntRaderDisplay = 0; nCntRaderDisplay < NUM_RADERDISPLAY; nCntRaderDisplay++)
	{
		if (g_pTextureRaderDisPlay[nCntRaderDisplay] != NULL)
		{
			g_pTextureRaderDisPlay[nCntRaderDisplay]->Release();
			g_pTextureRaderDisPlay[nCntRaderDisplay] = NULL;
		}
	}
	//頂点バッファの破棄
	if (g_pVtxBuffRaderDisplay != NULL)
	{
		g_pVtxBuffRaderDisplay->Release();
		g_pVtxBuffRaderDisplay = NULL;
	}
}

//============================
//レーダーの更新処理
//============================
void UpdateRader(void)
{



}

//============================
//レーダーディスプレイの更新処理
//============================
void UpdateRaderDisplay(void)
{
	
}




//================================
//レーダーの描画処理
//================================
void DrawRader(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffRader, 0, sizeof(VERTEX_2D));//頂点バッファへのポインタと頂点情報の構造体のサイズ

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	pDevice->SetTexture(0,g_pTextureRader);
	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,0, 2);//0は、描画する最初の頂点インデックスで、2は、描画するプリミティブ数

}


//================================
//レーダーディスプレイの描画処理
//================================
void DrawRaderDisplay(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffRaderDisplay, 0, sizeof(VERTEX_2D));//頂点バッファへのポインタと頂点情報の構造体のサイズ

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntRaderDisplay = 0; nCntRaderDisplay < MAX_RADERDISPLAY; nCntRaderDisplay++)
	{
		if (g_aRaderDisPlay[nCntRaderDisplay].bUse == true)
		{
			pDevice->SetTexture(0, g_pTextureRaderDisPlay[g_aRaderDisPlay[nCntRaderDisplay].nType]);
			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntRaderDisplay * 4, 2);//0は、描画する最初の頂点インデックスで、2は、描画するプリミティブ数
		}
	}

}

//================================
//レーダーディスプレイのセット
//================================
int SetRaderDisplay(int nType)
{
	int nCntRaderDisplay = -1;

	for (int nCnt = 0; nCnt < MAX_RADERDISPLAY; nCnt++)
	{
		if (g_aRaderDisPlay[nCnt].bUse == false)
		{
			g_aRaderDisPlay[nCnt].bUse = true;//使用しているかどうか
			g_aRaderDisPlay[nCnt].nType = nType;//種類
			nCntRaderDisplay = nCnt;
			break;
		}
	}

	return nCntRaderDisplay;
}

//====================================
//レーダーディスプレイの位置の更新
//====================================
void SetPositionRaderDisPlay(int nIdxRaderDisPlay, D3DXVECTOR3 pos, float fRotZ)
{
	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffRaderDisplay->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntRaderDisPlay = 0; nCntRaderDisPlay < MAX_RADERDISPLAY; nCntRaderDisPlay++)
	{

		if (g_aRaderDisPlay[nIdxRaderDisPlay].bUse == true && nIdxRaderDisPlay == nCntRaderDisPlay)
		{
		    g_aRaderDisPlay[nIdxRaderDisPlay].rot.z = fRotZ;

			g_aRaderDisPlay[nIdxRaderDisPlay].pos.x = g_aRader.pos.x + (pos.x / RADERPOSRATIOX);//フィールドとレーダーの比率の座標
			g_aRaderDisPlay[nIdxRaderDisPlay].pos.y = g_aRader.pos.y - (pos.z / RADERPOSRATIOY);

			//頂点座標の設定（初期位置）
			pVtx[0].pos = NULL_VECTOR3;
			pVtx[1].pos = NULL_VECTOR3;
			pVtx[2].pos = NULL_VECTOR3;
			pVtx[3].pos = NULL_VECTOR3;


			if (g_aRaderDisPlay[nIdxRaderDisPlay].nType == RADERDISPLAY02_BOSS)
			{//ボスなら
					
				g_aRaderDisPlay[nIdxRaderDisPlay].fWidth = RADERDISPLAY_WIDTH * 5.0f;
				g_aRaderDisPlay[nIdxRaderDisPlay].fHeight = RADERDISPLAY_HEIGHT * 5.0f;
			}
			else if (g_aRaderDisPlay[nIdxRaderDisPlay].nType == RADERDISPLAY03_OUTLET)
			{
				g_aRaderDisPlay[nIdxRaderDisPlay].fWidth = RADERDISPLAY_WIDTH * 4.0f;
				g_aRaderDisPlay[nIdxRaderDisPlay].fHeight = RADERDISPLAY_HEIGHT * 4.0f;

			}
			else if(g_aRaderDisPlay[nIdxRaderDisPlay].nType == RADERDISPLAY00_PLAYER)
			{//ボス以外なら
				
				g_aRaderDisPlay[nIdxRaderDisPlay].fWidth = RADERDISPLAY_WIDTH * 3.5f;
				g_aRaderDisPlay[nIdxRaderDisPlay].fHeight = RADERDISPLAY_HEIGHT * 3.5f;
			}
			else if (g_aRaderDisPlay[nIdxRaderDisPlay].nType == RADERDISPLAY01_ENEMY)
			{
				g_aRaderDisPlay[nIdxRaderDisPlay].fWidth = RADERDISPLAY_WIDTH * 2.5f;
				g_aRaderDisPlay[nIdxRaderDisPlay].fHeight = RADERDISPLAY_HEIGHT * 2.5f;
			}
			else if (g_aRaderDisPlay[nIdxRaderDisPlay].nType == RADERDISPLAY04_EDIT)
			{
				g_aRaderDisPlay[nIdxRaderDisPlay].fWidth = RADERDISPLAY_WIDTH * 3.5f;
				g_aRaderDisPlay[nIdxRaderDisPlay].fHeight = RADERDISPLAY_HEIGHT * 3.5f;
			}

			g_aRaderDisPlay[nIdxRaderDisPlay].fLength = sqrtf(g_aRaderDisPlay[nIdxRaderDisPlay].fWidth * g_aRaderDisPlay[nIdxRaderDisPlay].fWidth +
				g_aRaderDisPlay[nIdxRaderDisPlay].fHeight * g_aRaderDisPlay[nIdxRaderDisPlay].fHeight) / 2.0f;

			g_aRaderDisPlay[nIdxRaderDisPlay].fAngle = atan2f(g_aRaderDisPlay[nIdxRaderDisPlay].fWidth, g_aRaderDisPlay[nIdxRaderDisPlay].fHeight);
			//頂点座標の設定
			pVtx[0].pos.x = g_aRaderDisPlay[nIdxRaderDisPlay].pos.x + sinf(g_aRaderDisPlay[nIdxRaderDisPlay].rot.z - (D3DX_PI - g_aRaderDisPlay[nIdxRaderDisPlay].fAngle)) * g_aRaderDisPlay[nIdxRaderDisPlay].fLength;//上から左に４５度
			pVtx[0].pos.y = g_aRaderDisPlay[nIdxRaderDisPlay].pos.y + cosf(g_aRaderDisPlay[nIdxRaderDisPlay].rot.z - (D3DX_PI - g_aRaderDisPlay[nIdxRaderDisPlay].fAngle)) * g_aRaderDisPlay[nIdxRaderDisPlay].fLength;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_aRaderDisPlay[nIdxRaderDisPlay].pos.x + sinf(g_aRaderDisPlay[nIdxRaderDisPlay].rot.z + (D3DX_PI - g_aRaderDisPlay[nIdxRaderDisPlay].fAngle)) * g_aRaderDisPlay[nIdxRaderDisPlay].fLength;//上から右に４５度
			pVtx[1].pos.y = g_aRaderDisPlay[nIdxRaderDisPlay].pos.y + cosf(g_aRaderDisPlay[nIdxRaderDisPlay].rot.z + (D3DX_PI - g_aRaderDisPlay[nIdxRaderDisPlay].fAngle)) * g_aRaderDisPlay[nIdxRaderDisPlay].fLength;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_aRaderDisPlay[nIdxRaderDisPlay].pos.x + sinf(g_aRaderDisPlay[nIdxRaderDisPlay].rot.z - g_aRaderDisPlay[nIdxRaderDisPlay].fAngle) * g_aRaderDisPlay[nIdxRaderDisPlay].fLength;//下から左に４５度
			pVtx[2].pos.y = g_aRaderDisPlay[nIdxRaderDisPlay].pos.y + cosf(g_aRaderDisPlay[nIdxRaderDisPlay].rot.z - g_aRaderDisPlay[nIdxRaderDisPlay].fAngle) * g_aRaderDisPlay[nIdxRaderDisPlay].fLength;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_aRaderDisPlay[nIdxRaderDisPlay].pos.x + sinf(g_aRaderDisPlay[nIdxRaderDisPlay].rot.z + g_aRaderDisPlay[nIdxRaderDisPlay].fAngle) * g_aRaderDisPlay[nIdxRaderDisPlay].fLength;//下から右に４５度
			pVtx[3].pos.y = g_aRaderDisPlay[nIdxRaderDisPlay].pos.y + cosf(g_aRaderDisPlay[nIdxRaderDisPlay].rot.z + g_aRaderDisPlay[nIdxRaderDisPlay].fAngle) * g_aRaderDisPlay[nIdxRaderDisPlay].fLength;
			pVtx[3].pos.z = 0.0f;
			

			if (pos.y > 280.0f)
			{
				//頂点カラーの設定（初期設定）
				pVtx[0].col = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);
				pVtx[1].col = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);
				pVtx[2].col = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);
				pVtx[3].col = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);
			}
			else
			{
				//頂点カラーの設定（初期設定）
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			}
			break;
		}

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffRaderDisplay->Unlock();
}

//================================
//レーダーディスプレイの破棄
//================================
void KillRaderDisplay(int nIdxRaderDisplay)
{
	g_aRaderDisPlay[nIdxRaderDisplay].bUse = false;
}




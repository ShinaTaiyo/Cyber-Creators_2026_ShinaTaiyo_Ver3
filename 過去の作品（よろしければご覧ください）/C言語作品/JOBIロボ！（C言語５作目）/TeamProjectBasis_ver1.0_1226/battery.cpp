//=====================================================================================================================
//
// １２月２６日：チーム制作の基盤づくり[Battery.cpp]
// Author : ShinaTaiyo
// 
// 注意：fRWidth,fRHeightは、中心点からの半径で大きさ、判定などを取っています。もしこの処理を使うなら、ご注意下さい。
// 
//======================================================================================================================
#include "battery.h"
#include "main.h"
#include "model.h"
#include "sound.h"
#include "player.h"
#include "fade.h"
#include "input.h"
#include "particle.h"
#include "game.h"

#define BATTERY_WIDTH (150.0f)
#define BATTERY_HEIGHT (75.0f)
#define BATTERYNUM_WIDTH (20.0f)
#define BATTERYNUM_HEIGHT (40.0f)

#define MAXBATTERY_WIDTH (100.0f)   //バッテリー最大値の横幅
#define MAXBATTERY_HEIGHT (40.0f)   //バッテリー最大値の高さ

#define MAXBATTERYNUM_WIDTH (15.0f) //バッテリー最大値の数値の横幅
#define MAXBATTERYNUM_HEIGHT (35.0f)//バッテリー最大値の数値の高さ

//グローバル変数
int g_nDelayBattery;

//=================================
//バッテリーフレーム表示用
//=================================
LPDIRECT3DTEXTURE9 g_pTextureBattery[NUM_BATTERY] = {};//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBattery = NULL;//バッテリーの情報
Battery g_aBattery[NUM_BATTERY];//バッテリー表示用

//=================================
//バッテリー数値表示用
//=================================
LPDIRECT3DTEXTURE9 g_pTextureBatteryNum = NULL;//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBatteryNum = NULL;//バッテリー数値の情報
BatteryNum g_aBatteryNum[BATTERYDIGIT];//バッテリー数値の構造体

//=================================
//充電器の位置表示用
//=================================
PDIRECT3DVERTEXBUFFER9 g_pVtxBuffOutletDisp = NULL;//頂点バッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureOutletDisp = NULL;//テクスチャへのポインタ
OutletDisp g_aOutletDisp[MAX_OUTLETDISP];//充電器位置表示の構造体
int g_nDelayOutletDIsp;

//============================
//初期化処理
//============================
void InitBattery(void)
{
	VERTEX_2D* pVtx;
	Player* pPlayer = GetPlayer();//プレイヤーの情報を取得

	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Battery_003.png",
		&g_pTextureBattery[BATTERYTYPE00_NOWBATTERY]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\MaxBattery_000.png",
		&g_pTextureBattery[BATTERYTYPE01_MAXBATTERY]);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_BATTERY,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBattery,
		NULL);

	g_nDelayBattery = 0;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffBattery->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBattery = 0; nCntBattery < NUM_BATTERY; nCntBattery++)
	{
		switch (nCntBattery)
		{
		case BATTERYTYPE00_NOWBATTERY://現在のバッテリーの初期化
			g_aBattery[nCntBattery].pos = D3DXVECTOR3(1150.0f, 580.0f, 0.0f);//位置
			g_aBattery[nCntBattery].nBattery = pPlayer->nBattery;//現在のバッテリー
			g_aBattery[nCntBattery].bCautionBattery = false;//警告
			g_aBattery[nCntBattery].nAnimationCnt = 0;//アニメーションカウント
			g_aBattery[nCntBattery].nAnimationPattern = 0;//アニメーションパターン
			g_aBattery[nCntBattery].bAnimationTurn = 0;//アニメーションを逆にする

			//テクスチャ座標の設定（初期設定）
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.2f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 0.2f);
			pVtx[3].tex = D3DXVECTOR2(0.2f, 0.2f);

			pVtx[0].pos = D3DXVECTOR3(g_aBattery[nCntBattery].pos.x - BATTERY_WIDTH, g_aBattery[nCntBattery].pos.y - BATTERY_HEIGHT, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aBattery[nCntBattery].pos.x + BATTERY_WIDTH, g_aBattery[nCntBattery].pos.y - BATTERY_HEIGHT, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aBattery[nCntBattery].pos.x - BATTERY_WIDTH, g_aBattery[nCntBattery].pos.y + BATTERY_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aBattery[nCntBattery].pos.x + BATTERY_WIDTH, g_aBattery[nCntBattery].pos.y + BATTERY_HEIGHT, 0.0f);

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
			break;

		case BATTERYTYPE01_MAXBATTERY://バッテリー最大値の初期化
			g_aBattery[nCntBattery].pos = D3DXVECTOR3(1155.0f, 675.0f, 0.0f);//位置
			g_aBattery[nCntBattery].nBattery = pPlayer->nBattery;//バッテリーの最大値

			pVtx[0].pos = D3DXVECTOR3(g_aBattery[nCntBattery].pos.x - MAXBATTERY_WIDTH, g_aBattery[nCntBattery].pos.y - MAXBATTERY_HEIGHT, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aBattery[nCntBattery].pos.x + MAXBATTERY_WIDTH, g_aBattery[nCntBattery].pos.y - MAXBATTERY_HEIGHT, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aBattery[nCntBattery].pos.x - MAXBATTERY_WIDTH, g_aBattery[nCntBattery].pos.y + MAXBATTERY_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aBattery[nCntBattery].pos.x + MAXBATTERY_WIDTH, g_aBattery[nCntBattery].pos.y + MAXBATTERY_HEIGHT, 0.0f);

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

			break;

		default:
			break;
		}


		pVtx += 4;

	}

	//頂点バッファをアンロックする
	g_pVtxBuffBattery->Unlock();

	//==========================
	//バッテリー数値の処理
	//==========================

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Number_Battery.png",
		&g_pTextureBatteryNum);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * BATTERYDIGIT * NUM_BATTERY,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBatteryNum,
		NULL);

	int nGetBattery = g_aBattery[0].nBattery;//スコアを保存する

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffBatteryNum->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntBattery = 0; nCntBattery < NUM_BATTERY; nCntBattery++)
	{
		for (int nCntDigit = 0; nCntDigit < BATTERYDIGIT; nCntDigit++)
		{
			switch (nCntBattery)
			{
			case BATTERYTYPE00_NOWBATTERY:
				g_aBattery[nCntBattery].aBatteryNum[nCntDigit].pos = D3DXVECTOR3(1225.0f - 50.0f * nCntDigit, 583.0f, 0.0f);
				if (nCntDigit == 1)
				{
					g_aBattery[nCntBattery].aBatteryNum[nCntDigit].pos = D3DXVECTOR3(1223.0f - 50.0f * nCntDigit, 583.0f, 0.0f);
				}
				if (nCntDigit == 2)
				{
					g_aBattery[nCntBattery].aBatteryNum[nCntDigit].pos = D3DXVECTOR3(1226.0f - 50.0f * nCntDigit, 583.0f, 0.0f);
				}
				if (nCntDigit == 3)
				{
					g_aBattery[nCntBattery].aBatteryNum[nCntDigit].pos = D3DXVECTOR3(1230.0f - 50.0f * nCntDigit, 583.0f, 0.0f);
				}
				//頂点の設定
				pVtx[0].pos = D3DXVECTOR3(g_aBattery[nCntBattery].aBatteryNum[nCntDigit].pos.x - BATTERYNUM_WIDTH, g_aBattery[nCntBattery].aBatteryNum[nCntDigit].pos.y - BATTERYNUM_HEIGHT, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aBattery[nCntBattery].aBatteryNum[nCntDigit].pos.x + BATTERYNUM_WIDTH, g_aBattery[nCntBattery].aBatteryNum[nCntDigit].pos.y - BATTERYNUM_HEIGHT, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aBattery[nCntBattery].aBatteryNum[nCntDigit].pos.x - BATTERYNUM_WIDTH, g_aBattery[nCntBattery].aBatteryNum[nCntDigit].pos.y + BATTERYNUM_HEIGHT, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aBattery[nCntBattery].aBatteryNum[nCntDigit].pos.x + BATTERYNUM_WIDTH, g_aBattery[nCntBattery].aBatteryNum[nCntDigit].pos.y + BATTERYNUM_HEIGHT, 0.0f);

				break;
			case BATTERYTYPE01_MAXBATTERY:
				g_aBattery[nCntBattery].aBatteryNum[nCntDigit].pos = D3DXVECTOR3(1235.0f - 30.0f * nCntDigit, 675.0f, 0.0f);
				if (nCntDigit == 1)
				{
					g_aBattery[nCntBattery].aBatteryNum[nCntDigit].pos = D3DXVECTOR3(1235.0f - 30.0f * nCntDigit, 675.0f, 0.0f);
				}
				if (nCntDigit == 2)
				{
					g_aBattery[nCntBattery].aBatteryNum[nCntDigit].pos = D3DXVECTOR3(1235.0f - 30.0f * nCntDigit, 675.0f, 0.0f);
				}
				if (nCntDigit == 3)
				{
					g_aBattery[nCntBattery].aBatteryNum[nCntDigit].pos = D3DXVECTOR3(1235.0f - 30.0f * nCntDigit, 675.0f, 0.0f);
				}

				//頂点の設定
				pVtx[0].pos = D3DXVECTOR3(g_aBattery[nCntBattery].aBatteryNum[nCntDigit].pos.x - MAXBATTERYNUM_WIDTH, g_aBattery[nCntBattery].aBatteryNum[nCntDigit].pos.y - MAXBATTERYNUM_HEIGHT, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aBattery[nCntBattery].aBatteryNum[nCntDigit].pos.x + MAXBATTERYNUM_WIDTH, g_aBattery[nCntBattery].aBatteryNum[nCntDigit].pos.y - MAXBATTERYNUM_HEIGHT, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aBattery[nCntBattery].aBatteryNum[nCntDigit].pos.x - MAXBATTERYNUM_WIDTH, g_aBattery[nCntBattery].aBatteryNum[nCntDigit].pos.y + MAXBATTERYNUM_HEIGHT, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aBattery[nCntBattery].aBatteryNum[nCntDigit].pos.x + MAXBATTERYNUM_WIDTH, g_aBattery[nCntBattery].aBatteryNum[nCntDigit].pos.y + MAXBATTERYNUM_HEIGHT, 0.0f);

				break;
			default:
				break;
			}
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

			g_aBattery[nCntBattery].aBatteryNum[nCntDigit].fPosTexU = (float)(nGetBattery % 10);
			nGetBattery /= 10;

			pVtx[0].tex = D3DXVECTOR2(0.0f + (0.1f * g_aBattery[nCntBattery].aBatteryNum[nCntDigit].fPosTexU), 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f + (0.1f * g_aBattery[nCntBattery].aBatteryNum[nCntDigit].fPosTexU), 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f + (0.1f * g_aBattery[nCntBattery].aBatteryNum[nCntDigit].fPosTexU), 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f + (0.1f * g_aBattery[nCntBattery].aBatteryNum[nCntDigit].fPosTexU), 1.0f);

			pVtx += 4;//頂点座標のポインタをインクリメント
		}
	}
	
	//頂点バッファをアンロックする
	g_pVtxBuffBatteryNum->Unlock();
}

//==============================
//終了処理
//==============================
void UninitBattery(void)
{
	//============================
	//バッテリーフレームの処理
	//============================

	//テクスチャの破棄

	for (int nCntBattery = 0; nCntBattery < NUM_BATTERY; nCntBattery++)
	{
		if (g_pTextureBattery[nCntBattery] != NULL)
		{
			g_pTextureBattery[nCntBattery]->Release();
			g_pTextureBattery[nCntBattery] = NULL;
		}
	}
	//頂点バッファの破棄
	if (g_pVtxBuffBattery != NULL)
	{
		g_pVtxBuffBattery->Release();
		g_pVtxBuffBattery = NULL;
	}

	//==============================
	//バッテリー数値の処理
	//==============================

	if (g_pTextureBatteryNum != NULL)
	{
		g_pTextureBatteryNum->Release();
		g_pTextureBatteryNum = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffBatteryNum != NULL)
	{
		g_pVtxBuffBatteryNum->Release();
		g_pVtxBuffBatteryNum = NULL;
	}
}
//============================
//更新処理
//============================
void UpdateBattery(void)
{
	Player* pPlayer = GetPlayer();
	VERTEX_2D* pVtx;
	g_nDelayBattery++;

	//========================================================================
	//現在のバッテリーフレームを電池が５００を下回ったら、点滅させる処理
	//========================================================================

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffBattery->Lock(0, 0, (void**)&pVtx, 0);

		g_aBattery[BATTERYTYPE00_NOWBATTERY].nAnimationCnt++;

		if (g_aBattery[BATTERYTYPE00_NOWBATTERY].nBattery <= 500)
		{
			if (g_aBattery[BATTERYTYPE00_NOWBATTERY].nAnimationCnt % 5 == 0)
			{
				//アニメーションをターンさせる
				if (g_aBattery[BATTERYTYPE00_NOWBATTERY].bAnimationTurn == false)
				{
					g_aBattery[BATTERYTYPE00_NOWBATTERY].nAnimationPattern++;

					if (g_aBattery[BATTERYTYPE00_NOWBATTERY].nAnimationPattern >= 5)
					{
						g_aBattery[BATTERYTYPE00_NOWBATTERY].bAnimationTurn = true;
						g_aBattery[BATTERYTYPE00_NOWBATTERY].nAnimationPattern = 3;
					}
				}
				else
				{
					g_aBattery[BATTERYTYPE00_NOWBATTERY].nAnimationPattern--;
					if (g_aBattery[BATTERYTYPE00_NOWBATTERY].nAnimationPattern <= -1)
					{
						g_aBattery[BATTERYTYPE00_NOWBATTERY].bAnimationTurn = false;
						g_aBattery[BATTERYTYPE00_NOWBATTERY].nAnimationPattern = 1;
					}
				}
			}
		}
		else
		{
			g_aBattery[BATTERYTYPE00_NOWBATTERY].nAnimationPattern = 0;
			g_aBattery[BATTERYTYPE00_NOWBATTERY].bAnimationTurn = false;
		}

		//テクスチャ座標の設定（初期設定）
		pVtx[0].tex = D3DXVECTOR2(0.0f + 0.2f * g_aBattery[BATTERYTYPE00_NOWBATTERY].nAnimationPattern, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.2f + 0.2f * g_aBattery[BATTERYTYPE00_NOWBATTERY].nAnimationPattern, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + 0.2f * g_aBattery[BATTERYTYPE00_NOWBATTERY].nAnimationPattern, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.2f + 0.2f * g_aBattery[BATTERYTYPE00_NOWBATTERY].nAnimationPattern, 1.0f);

	
    //頂点バッファをアンロックする
	g_pVtxBuffBattery->Unlock();

	//==============================================
	//現在のバッテリー数値
	//==============================================

	if (GetKeyboardTrigger(DIK_B) == true)
	{
		g_aBattery[BATTERYTYPE00_NOWBATTERY].nBattery += 100;
	}

	if (g_aBattery[BATTERYTYPE00_NOWBATTERY].nBattery >= pPlayer->nBattery)
	{
		//バッテリー量が最大値を超えたら最大値で固定する
		g_aBattery[BATTERYTYPE00_NOWBATTERY].nBattery = pPlayer->nBattery;
	}

	if (g_nDelayBattery % 300 == 0)
	{//５秒ごとにバッテリーを１減らす
		SubBattery(1);
	}

	if (g_aBattery[BATTERYTYPE00_NOWBATTERY].nBattery <= 0)
	{//バッテリーが０以下になったら０に固定
		g_aBattery[BATTERYTYPE00_NOWBATTERY].nBattery = 0;
	}

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffBatteryNum->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBattery = 0; nCntBattery < NUM_BATTERY; nCntBattery++)
	{
		//==================================================
		//反映させる数値の設定
		//==================================================
		int nGetBattery = 0;

		switch (nCntBattery)
		{
		case BATTERYTYPE00_NOWBATTERY:
			nGetBattery = g_aBattery[nCntBattery].nBattery;
			break;
		case BATTERYTYPE01_MAXBATTERY:
			nGetBattery = pPlayer->nBattery;
			break;
		default:
			break;
		}

		for (int nCntDigit = 0; nCntDigit < BATTERYDIGIT; nCntDigit++)
		{
			g_aBattery[nCntBattery].aBatteryNum[nCntDigit].fPosTexU = (float)(nGetBattery % 10);
			nGetBattery /= 10;

			pVtx[0].tex = D3DXVECTOR2(0.0f + (0.1f * g_aBattery[nCntBattery].aBatteryNum[nCntDigit].fPosTexU), 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f + (0.1f * g_aBattery[nCntBattery].aBatteryNum[nCntDigit].fPosTexU), 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f + (0.1f * g_aBattery[nCntBattery].aBatteryNum[nCntDigit].fPosTexU), 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f + (0.1f * g_aBattery[nCntBattery].aBatteryNum[nCntDigit].fPosTexU), 1.0f);

			pVtx += 4;
		}
	}
	//頂点バッファをアンロックする
	g_pVtxBuffBatteryNum->Unlock();
}

//================================
//描画処理
//================================
void DrawBattery(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//===========================
	//バッテリーフレームの処理
	//===========================

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBattery, 0, sizeof(VERTEX_2D));//頂点バッファへのポインタと頂点情報の構造体のサイズ

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntBattery = 0; nCntBattery < NUM_BATTERY; nCntBattery++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureBattery[nCntBattery]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,nCntBattery * 4, 2);//0は、描画する最初の頂点インデックスで、2は、描画するプリミティブ数
	}
	//=============================
	//バッテリー数値の処理
	//=============================

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBatteryNum, 0, sizeof(VERTEX_2D));//頂点バッファへのポインタと頂点情報の構造体のサイズ

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntBatteryNum = 0; nCntBatteryNum < BATTERYDIGIT * NUM_BATTERY; nCntBatteryNum++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureBatteryNum);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBatteryNum * 4, 2);
	}
}

//===========================
// バッテリーの情報を取得
//===========================
Battery* GetBattery(void)//スクラップ構造体の情報をゲットする
{
	return &g_aBattery[0];
}

//===========================
// バッテリーを下げる
//===========================
void SubBattery(int nSubBattery)
{

	if (GetMode() == MODE_GAME)
	{//ゲーム画面の場合
		if (g_aBattery[BATTERYTYPE00_NOWBATTERY].nBattery > 0)
		{//バッテリーが残っている場合
			if (g_aBattery[BATTERYTYPE00_NOWBATTERY].nBattery > 500)
			{//バッテリーが100以上の場合
				g_aBattery[BATTERYTYPE00_NOWBATTERY].bCautionBattery = false;
			}
			else if (g_aBattery[BATTERYTYPE00_NOWBATTERY].nBattery < 500 && !g_aBattery[BATTERYTYPE00_NOWBATTERY].bCautionBattery)
			{
				g_aBattery[BATTERYTYPE00_NOWBATTERY].bCautionBattery = true;
				PlaySound(SOUND_LABEL_SE_BATTERYCAUTION_000);//警告音を出す
			}

			g_aBattery[BATTERYTYPE00_NOWBATTERY].nBattery -= nSubBattery;//バッテリーを1上げる

			if (g_aBattery[BATTERYTYPE00_NOWBATTERY].nBattery <= 0)
			{//バッテリーが0以下の場合
				SetFade(MODE_RESULT);	//リザルト画面へ

				StopSound(SOUND_LABEL_BGM015);
				StopSound(SOUND_LABEL_BGM013);
				
			}
		}
	}
}

void InitOutletDisp(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	g_nDelayOutletDIsp = 0;

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\RaderDisplay_Outlet_000.png",
		&g_pTextureOutletDisp);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_OUTLETDISP,//頂点の数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffOutletDisp,
		NULL);

	VERTEX_3D* pVtx;
	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffOutletDisp->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntOutlet = 0; nCntOutlet < MAX_OUTLETDISP; nCntOutlet++)
	{
		g_aOutletDisp[nCntOutlet].pos = NULL_VECTOR3;//位置
		g_aOutletDisp[nCntOutlet].bUse = false;//使用状態
		g_aOutletDisp[nCntOutlet].rot = NULL_VECTOR3;//向き
		g_aOutletDisp[nCntOutlet].nNumModel = 0;//どのモデルが召喚したか
		g_aOutletDisp[nCntOutlet].mtxWorld = {};//マトリックスワールド

		//頂点座標の設定（初期位置）
		pVtx[0].pos = D3DXVECTOR3(-OUTLETDISP_WIDTH, OUTLETDISP_HEIGHT, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(OUTLETDISP_WIDTH, OUTLETDISP_HEIGHT, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-OUTLETDISP_WIDTH, -OUTLETDISP_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(OUTLETDISP_WIDTH, -OUTLETDISP_HEIGHT, 0.0f);

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

		pVtx += 4;//ポインタを進める
	}

	//頂点バッファをアンロックする
	g_pVtxBuffOutletDisp->Unlock();
}

void UninitOutletDisp(void)
{
	if (g_pTextureOutletDisp != NULL)
	{
		g_pTextureOutletDisp->Release();
		g_pTextureOutletDisp = NULL;
	}

	if (g_pVtxBuffOutletDisp != NULL)
	{
		g_pVtxBuffOutletDisp->Release();
		g_pVtxBuffOutletDisp = NULL;
	}
}

void UpdateOutletDisp(void)
{
	for (int nCntOutlet = 0; nCntOutlet < MAX_OUTLETDISP; nCntOutlet++)
	{
		if (g_aOutletDisp[nCntOutlet].bUse == true)
		{
			Model* pModel = GetModel();

			for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++, pModel++)
			{
				if (pModel->bUse == false && nCntModel == g_aOutletDisp[nCntOutlet].nNumModel)
				{//使用されなくなったモデルが、充電器位置表示を召喚したモデルと一致したら、充電器位置表示も消す
					g_aOutletDisp[nCntOutlet].bUse = false;
					break;
				}
			}
		}
	}
}

void DrawOutletDisp(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ
	D3DXMATRIX mtxTrans;//計算用マトリックス
	D3DXMATRIX mtxView;//ビューマトリックス取得用

    //デバイスの取得
	pDevice = GetDevice();

	for (int nCntOutlet = 0; nCntOutlet < MAX_OUTLETDISP; nCntOutlet++)
	{
		if (g_aOutletDisp[nCntOutlet].bUse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aOutletDisp[nCntOutlet].mtxWorld);

			//ビューマトリックスを取得
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//ポリゴンをカメラに対して正面に向ける
			D3DXMatrixInverse(&g_aOutletDisp[nCntOutlet].mtxWorld, NULL, &mtxView);

			//逆行列を求める
			g_aOutletDisp[nCntOutlet].mtxWorld._41 = 0.0f;
			g_aOutletDisp[nCntOutlet].mtxWorld._42 = 0.0f;
			g_aOutletDisp[nCntOutlet].mtxWorld._43 = 0.0f;

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aOutletDisp[nCntOutlet].pos.x, g_aOutletDisp[nCntOutlet].pos.y, g_aOutletDisp[nCntOutlet].pos.z);
			D3DXMatrixMultiply(&g_aOutletDisp[nCntOutlet].mtxWorld, &g_aOutletDisp[nCntOutlet].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aOutletDisp[nCntOutlet].mtxWorld);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffOutletDisp, 0, sizeof(VERTEX_3D));//頂点バッファへのポインタと頂点情報の構造体のサイズ

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureOutletDisp);

			//===============================================================
			//描画の調整
			//==============================================================
			//ライトを無効にする
			pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

			////Zの比較方法変更。
			//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

			////Zバッファに書き込まない
			//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

			//アルファテストを有効
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
			pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
			pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntOutlet * 4, 2);//0は、描画する最初の頂点インデックスで、2は、描画するプリミティブ

			//============================
			//描画の調整をもとに戻す
			//============================

			//ライトを有効に戻す
			pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

			////Zの比較方法変更。
			//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

			////Zバッファに書き込む
			//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

			//アルファテストを無効に戻す
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		}
	}
}

void SetOutletDisp(D3DXVECTOR3 pos, int nNumModel)
{
	for (int nCntOutlet = 0; nCntOutlet < MAX_OUTLETDISP; nCntOutlet++)
	{
		if (g_aOutletDisp[nCntOutlet].bUse == false)
		{
			g_aOutletDisp[nCntOutlet].pos = pos;//位置
			g_aOutletDisp[nCntOutlet].nNumModel = nNumModel;//どのモデルが召喚したか
			g_aOutletDisp[nCntOutlet].bUse = true;//使用状態
			break;
		}
	}
}



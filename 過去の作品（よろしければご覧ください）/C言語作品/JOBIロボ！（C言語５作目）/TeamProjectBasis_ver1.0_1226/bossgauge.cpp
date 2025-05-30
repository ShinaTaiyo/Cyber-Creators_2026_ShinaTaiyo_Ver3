//========================================================
//
//８月３０日：エフェクトもう一回見直す[BossGaugeFrame.cpp]
//Author:ShinaTaiyo
//
//=========================================================
#include "main.h"
#include "scrap.h"
#include "BossGauge.h"
#include "player.h"
#include "enemy.h"
#include "boss.h"

//マクロ定義
#define BOSSGAUGEFRAME_HEIGHT (70.0f)  //ゲージフレームの長さ
#define REDUCATIONBOSSGAUGE (2.0f) //ゲージのVtxを数値分小さくする。
#define BOSSGAUGEFRAME_WIDTH (600.0f) //ゲージのフレームの長さ

//グローバル変数
#define BOSSGAUGE_HEIGHT (30.0f) //ゲージの高さ
#define BOSSGAUGE_WIDTH (600.0f)//ゲージの長さ 

//====================================
//ゲージフレーム
//====================================
LPDIRECT3DTEXTURE9 g_pTextureBossGaugeFrame[NUM_BOSSGAUGEFRAME] = {};//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBossGaugeFrame = NULL;//ゲージフレームの情報
BossGaugeFrame g_aBossGaugeFrame[MAX_BOSSGAUGEFRAME];

//====================================
//ゲージ
//====================================
LPDIRECT3DTEXTURE9 g_pTextureBossGauge = NULL;//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBossGauge = NULL;
BossGauge g_aBossGauge;//ゲージ情報

//===========================
//ゲージの初期化処理
//===========================
void InitBossGauge(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//===============================
	//ゲージフレームの処理
	//===============================

	//テクスチャ
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\BossGaugeFrame_000.png",
		&g_pTextureBossGaugeFrame[BOSSGAUGEFRAME00_HIDE]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\BossGaugeFrame_001.png",
		&g_pTextureBossGaugeFrame[BOSSGAUGEFRAME01_APPER]);

	for (int nCntBossGaugeFrame = 0; nCntBossGaugeFrame < MAX_BOSSGAUGEFRAME; nCntBossGaugeFrame++)
	{
		g_aBossGaugeFrame[nCntBossGaugeFrame].nType = 0;                          //種類
		g_aBossGaugeFrame[nCntBossGaugeFrame].fBossGaugeLength = 0.0f;                //ゲージの長さ
		g_aBossGaugeFrame[nCntBossGaugeFrame].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//位置
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BOSSGAUGEFRAME,//４は必要な頂点数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBossGaugeFrame,
		NULL);

	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffBossGaugeFrame->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBossGaugeFrame = 0; nCntBossGaugeFrame < MAX_BOSSGAUGEFRAME; nCntBossGaugeFrame++)
	{
		g_aBossGaugeFrame[nCntBossGaugeFrame].pos = D3DXVECTOR3(BOSSGAUGE_POS_X - (BOSSGAUGEFRAME_WIDTH / 2.0f), BOSSGAUGE_POS_Y, 0.0f);//位置を決める
		g_aBossGaugeFrame[nCntBossGaugeFrame].nType = nCntBossGaugeFrame;//種類を決める

		switch (nCntBossGaugeFrame)
		{
		case BOSSGAUGEFRAME00_HIDE:
			g_aBossGaugeFrame[nCntBossGaugeFrame].bUse = true;
			break;
		case BOSSGAUGEFRAME01_APPER:
			g_aBossGaugeFrame[nCntBossGaugeFrame].bUse = false;
			break;
		}
	
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aBossGaugeFrame[nCntBossGaugeFrame].pos.x - REDUCATIONBOSSGAUGE, g_aBossGaugeFrame[nCntBossGaugeFrame].pos.y - REDUCATIONBOSSGAUGE, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aBossGaugeFrame[nCntBossGaugeFrame].pos.x + BOSSGAUGEFRAME_WIDTH + REDUCATIONBOSSGAUGE, g_aBossGaugeFrame[nCntBossGaugeFrame].pos.y - REDUCATIONBOSSGAUGE, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aBossGaugeFrame[nCntBossGaugeFrame].pos.x - REDUCATIONBOSSGAUGE, g_aBossGaugeFrame[nCntBossGaugeFrame].pos.y + BOSSGAUGEFRAME_HEIGHT + REDUCATIONBOSSGAUGE, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aBossGaugeFrame[nCntBossGaugeFrame].pos.x + BOSSGAUGEFRAME_WIDTH + REDUCATIONBOSSGAUGE, g_aBossGaugeFrame[nCntBossGaugeFrame].pos.y + BOSSGAUGEFRAME_HEIGHT + REDUCATIONBOSSGAUGE, 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;//頂点データのポインタを４つ分進める。
	}

	//頂点バッファをアンロックする
	g_pVtxBuffBossGaugeFrame->Unlock();

	//===================================================
	//ゲージ表記処理
	//===================================================
	g_aBossGauge.fBossGauge = 0.0f;                     //ゲージ
	g_aBossGauge.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//位置
	g_aBossGauge.fBossGaugeLength = 0.0f;				//ゲージの増える量
	g_aBossGauge.nBossGaugeNum = 0;						//ゲージの番号
	g_aBossGauge.bUse = false;							//使用しているかどうか

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"",
		&g_pTextureBossGauge);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BOSSGAUGEFRAME,//４は必要な頂点数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBossGauge,
		NULL);

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffBossGauge->Lock(0, 0, (void**)&pVtx, 0);

	g_aBossGauge.pos = D3DXVECTOR3(BOSSGAUGE_POS_X - ((BOSSGAUGE_WIDTH - 5.0f) / 2.0f), BOSSGAUGE_POS_Y + 39.0f, 0.0f);	//位置

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_aBossGauge.pos.x, g_aBossGauge.pos.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aBossGauge.pos.x + (BOSSGAUGE_WIDTH - 5.0f), g_aBossGauge.pos.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aBossGauge.pos.x, g_aBossGauge.pos.y + BOSSGAUGE_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aBossGauge.pos.x + (BOSSGAUGE_WIDTH - 5.0f), g_aBossGauge.pos.y + BOSSGAUGE_HEIGHT, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	
	//頂点バッファをアンロックする
	g_pVtxBuffBossGauge->Unlock();
}

//ゲージの終了処理
void UninitBossGauge(void)
{
	//=====================================
	//ゲージフレームの初期化処理
	//=====================================
	for (int nCntBossGaugeFrame = 0; nCntBossGaugeFrame < NUM_BOSSGAUGEFRAME; nCntBossGaugeFrame++)
	{
		//テクスチャの破棄
		if (g_pTextureBossGaugeFrame[nCntBossGaugeFrame] != NULL)
		{
			g_pTextureBossGaugeFrame[nCntBossGaugeFrame]->Release();
			g_pTextureBossGaugeFrame[nCntBossGaugeFrame] = NULL;
		}
	}
	//頂点バッファの破棄
	if (g_pVtxBuffBossGaugeFrame != NULL)
	{
		g_pVtxBuffBossGaugeFrame->Release();
		g_pVtxBuffBossGaugeFrame = NULL;
	}
	
	//======================================
	//ゲージの終了処理
	//======================================

	//テクスチャの破棄
	if (g_pTextureBossGauge != NULL)
	{
		g_pTextureBossGauge->Release();
		g_pTextureBossGauge = NULL;
	}
	//頂点バッファの破棄
	if (g_pVtxBuffBossGauge != NULL)
	{
		g_pVtxBuffBossGauge->Release();
		g_pVtxBuffBossGauge = NULL;
	}
}

//ゲージの更新処理
void UpdateBossGauge(void)
{
	Boss* pBoss = GetBoss();//ボスの構造体の情報を取得
	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffBossGauge->Lock(0, 0, (void**)&pVtx, 0);

	if (g_aBossGauge.bUse == true)
	{
			g_aBossGauge.fBossGauge = (((BOSSGAUGE_WIDTH - 5.0f)) / (float)(pBoss->nMaxHp)) * (float)(pBoss->nMaxHp - pBoss->nHp);
		
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aBossGauge.pos.x, g_aBossGauge.pos.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aBossGauge.pos.x + (BOSSGAUGE_WIDTH - 5.0f) - g_aBossGauge.fBossGauge, g_aBossGauge.pos.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aBossGauge.pos.x, g_aBossGauge.pos.y + BOSSGAUGE_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aBossGauge.pos.x + (BOSSGAUGE_WIDTH - 5.0f) - g_aBossGauge.fBossGauge, g_aBossGauge.pos.y + BOSSGAUGE_HEIGHT, 0.0f);
		
			float fColor = (1.0f / (float)(pBoss->nMaxHp)) * (float)(pBoss->nHp);
		
			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, fColor, fColor, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, fColor, fColor, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, fColor, fColor, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, fColor, fColor, 1.0f);
		
			if (g_aBossGauge.fBossGauge >= (BOSSGAUGE_WIDTH - 5.0f))
			{
				g_aBossGauge.bUse = false;
				g_aBossGauge.nEnemyNum = -1;
				g_aBossGaugeFrame[BOSSGAUGEFRAME01_APPER].bUse = false;
				g_aBossGaugeFrame[BOSSGAUGEFRAME00_HIDE].bUse = true;
			}
	}
	//頂点バッファをアンロックする
	g_pVtxBuffBossGauge->Unlock();
}

//ゲージの描画処理
void DrawBossGauge(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBossGaugeFrame, 0, sizeof(VERTEX_2D));//頂点バッファへのポインタと頂点情報の構造体のサイズ

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//================================================
	//ゲージフレームの描画処理
	//================================================

	for (int nCntBossGaugeFrame = 0; nCntBossGaugeFrame < MAX_BOSSGAUGEFRAME; nCntBossGaugeFrame++)
	{
		if (g_aBossGauge.bUse == true && g_aBossGaugeFrame[nCntBossGaugeFrame].bUse == true)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureBossGaugeFrame[g_aBossGaugeFrame[nCntBossGaugeFrame].nType]);

			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,//0は、描画する最初の頂点インデックスで、2は、描画するプリミティブ数
				nCntBossGaugeFrame * 4,
				2);
		}
	}

	//================================================
	//ゲージの描画処理
	//================================================

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBossGauge, 0, sizeof(VERTEX_2D));//頂点バッファへのポインタと頂点情報の構造体のサイズ

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (g_aBossGauge.bUse == true)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureBossGauge);

		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,//0は、描画する最初の頂点インデックスで、2は、描画するプリミティブ数
			0,
			2);
	}
}

//=============================================================================
//ボスのゲージセット処理(ボスが出現した)
//=============================================================================
void SetBossGauge(int nEnemyNum)//ボスのゲージを出現させる処理
{
	if (g_aBossGaugeFrame[BOSSGAUGEFRAME01_APPER].bUse == false && g_aBossGauge.bUse == false)
	{
		for (int nCntBossGauge = 0; nCntBossGauge < MAX_BOSSGAUGEFRAME; nCntBossGauge++)
		{
			switch (nCntBossGauge)
			{
				//ボスが出現したので、ボスが隠れている時のゲージを破棄
			case BOSSGAUGEFRAME00_HIDE:
				g_aBossGaugeFrame[nCntBossGauge].bUse = false;
				break;

				//ボス出現用のゲージをセット
			case BOSSGAUGEFRAME01_APPER:
				g_aBossGaugeFrame[nCntBossGauge].bUse = true;
				break;
			}
		}

		g_aBossGauge.bUse = true;
		g_aBossGauge.nEnemyNum = nEnemyNum;
	}
}

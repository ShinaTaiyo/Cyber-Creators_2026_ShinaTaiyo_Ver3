//========================================================
//
//８月３０日：エフェクトもう一回見直す[GaugeFrame.cpp]
//Author:ShinaTaiyo
//
//=========================================================

#include "main.h"
#include "scrap.h"
#include "gauge.h"
#include "player.h"

//マクロ定義
#define GAUGEFRAME_HEIGHT (40.0f)  //ゲージフレームの長さ
#define REDUCATIONGAUGE (2.0f) //ゲージのVtxを数値分小さくする。
#define GAUGEFRAME_WIDTH (200.0f + (REDUCATIONGAUGE * 2)) //ゲージのフレームの長さ
//グローバル変数

//====================================
//ゲージフレーム
//====================================
LPDIRECT3DTEXTURE9 g_pTextureGaugeFrame[NUM_GAUGEFRAME] = {};//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGaugeFrame = NULL;//ゲージフレームの情報
GaugeFrame g_aGaugeFrame[MAX_GAUGEFRAME];
//==============================================================================================

//====================================
//ゲージ
//====================================
LPDIRECT3DTEXTURE9 g_pTextureGauge = NULL;//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGauge = NULL;

Gauge g_aGauge[MAX_GAUGE];                     //ゲージ情報
//==============================================================================================

//===========================
//ゲージの初期化処理
//===========================
void InitGauge(void)
{


	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//===============================
	//ゲージフレームの処理
	//===============================

	//テクスチャ
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\LvUpGaugeFrame_000.png",
		&g_pTextureGaugeFrame[SCRAPGAUGEFRAMETYPE00_TIBOROBO]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\LvUpGaugeFrame_001.png",
		&g_pTextureGaugeFrame[SCRAPGAUGEFRAMETYPE01_SWORD]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\LvUpGaugeFrame_002.png",
		&g_pTextureGaugeFrame[SCRAPGAUGEFRAMETYPE02_BOOMERANG]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\LvUpGaugeFrame_003.png",
		&g_pTextureGaugeFrame[SCRAPGAUGEFRAMETYPE03_SPEAKER]);
	
	for (int nCntGaugeFrame = 0; nCntGaugeFrame < MAX_GAUGEFRAME; nCntGaugeFrame++)
	{
		g_aGaugeFrame[nCntGaugeFrame].nType = 0;                          //種類
		g_aGaugeFrame[nCntGaugeFrame].fGaugeLength = 0.0f;                //ゲージの長さ
		g_aGaugeFrame[nCntGaugeFrame].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//位置
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_GAUGEFRAME,//４は必要な頂点数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffGaugeFrame,
		NULL);

	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffGaugeFrame->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntGaugeFrame = 0; nCntGaugeFrame < MAX_GAUGEFRAME; nCntGaugeFrame++)
	{
		g_aGaugeFrame[nCntGaugeFrame].pos = D3DXVECTOR3(30.0f + 250.0f * nCntGaugeFrame, 660.0f, 0.0f);//位置を決める
		g_aGaugeFrame[nCntGaugeFrame].nType = nCntGaugeFrame;//種類を決める

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aGaugeFrame[nCntGaugeFrame].pos.x, g_aGaugeFrame[nCntGaugeFrame].pos.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aGaugeFrame[nCntGaugeFrame].pos.x + GAUGEFRAME_WIDTH,g_aGaugeFrame[nCntGaugeFrame].pos.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aGaugeFrame[nCntGaugeFrame].pos.x, g_aGaugeFrame[nCntGaugeFrame].pos.y + GAUGEFRAME_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aGaugeFrame[nCntGaugeFrame].pos.x + GAUGEFRAME_WIDTH, g_aGaugeFrame[nCntGaugeFrame].pos.y + GAUGEFRAME_HEIGHT, 0.0f);


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
	g_pVtxBuffGaugeFrame->Unlock();

	//==========================================================================================================================

	//===================================================
	//ゲージ表記処理
	//===================================================

	for (int nCntGauge = 0; nCntGauge < MAX_GAUGEFRAME; nCntGauge++)
	{
		g_aGauge[nCntGauge].fGauge = 0.0f;                      //ゲージ
		g_aGauge[nCntGauge].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//位置
		g_aGauge[nCntGauge].fGaugeLength = 0.0f;      //ゲージの増える量
		g_aGauge[nCntGauge].nGaugeNum = 0;            //ゲージの番号
	}

	//(テクスチャは実際には使わないが、今までの書き方でやるなら、ポリゴンを出すために必要！）
	D3DXCreateTextureFromFile(pDevice,
		"ffffff",
		&g_pTextureGauge);


	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_GAUGEFRAME,//４は必要な頂点数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffGauge,
		NULL);

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffGauge->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntGauge = 0; nCntGauge < MAX_GAUGEFRAME; nCntGauge++)
	{
		g_aGauge[nCntGauge].pos = D3DXVECTOR3(30.0f + 250.0f * nCntGauge, 660.0f, 0.0f);//位置を決める
		g_aGauge[nCntGauge].nGaugeNum = nCntGauge;
		D3DXCOLOR col;

		switch (nCntGauge)
		{
		case 0:
			col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			break;
		case 1:
			col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
			break;
		case 2:
			col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
			break;
		case 3:
			col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			break;
		default:
			break;
		}

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aGauge[nCntGauge].pos.x + REDUCATIONGAUGE, g_aGauge[nCntGauge].pos.y + REDUCATIONGAUGE, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aGauge[nCntGauge].pos.x + GAUGEFRAME_WIDTH - REDUCATIONGAUGE, g_aGauge[nCntGauge].pos.y + REDUCATIONGAUGE, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aGauge[nCntGauge].pos.x + REDUCATIONGAUGE, g_aGauge[nCntGauge].pos.y + GAUGEFRAME_HEIGHT - REDUCATIONGAUGE, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aGauge[nCntGauge].pos.x + GAUGEFRAME_WIDTH - REDUCATIONGAUGE, g_aGauge[nCntGauge].pos.y - REDUCATIONGAUGE, 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = col;
		pVtx[1].col = col;
		pVtx[2].col = col;
		pVtx[3].col = col;

		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx += 4;//頂点データのポインタを４つ分進める。
	}
	//頂点バッファをアンロックする
	g_pVtxBuffGauge->Unlock();
}

//ゲージの終了処理
void UninitGauge(void)
{

	//=====================================
	//ゲージフレームの初期化処理
	//=====================================

	for (int nCntGaugeFrame = 0; nCntGaugeFrame < NUM_GAUGEFRAME; nCntGaugeFrame++)
	{
		//テクスチャの破棄
		if (g_pTextureGaugeFrame[nCntGaugeFrame] != NULL)
		{
			g_pTextureGaugeFrame[nCntGaugeFrame]->Release();
			g_pTextureGaugeFrame[nCntGaugeFrame] = NULL;
		}
	}
	//頂点バッファの破棄
	if (g_pVtxBuffGaugeFrame != NULL)
	{
		g_pVtxBuffGaugeFrame->Release();
		g_pVtxBuffGaugeFrame = NULL;
	}
	//================================================================================================================================

	//======================================
	//ゲージの終了処理
	//======================================
	
	//テクスチャの破棄
	if (g_pTextureGauge != NULL)
	{
		g_pTextureGauge->Release();
		g_pTextureGauge = NULL;
	}
	//頂点バッファの破棄
	if (g_pVtxBuffGauge != NULL)
	{
		g_pVtxBuffGauge->Release();
		g_pVtxBuffGauge = NULL;
	}
	//=================================================================================================================================


}

//ゲージの更新処理
void UpdateGauge(void)
{
	VERTEX_2D* pVtx;
	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffGauge->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntGauge = 0; nCntGauge < MAX_GAUGE; nCntGauge++)
	{
		
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aGauge[nCntGauge].pos.x + REDUCATIONGAUGE, g_aGauge[nCntGauge].pos.y + REDUCATIONGAUGE, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aGauge[nCntGauge].pos.x + g_aGauge[nCntGauge].fGauge - REDUCATIONGAUGE, g_aGauge[nCntGauge].pos.y + REDUCATIONGAUGE, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aGauge[nCntGauge].pos.x + REDUCATIONGAUGE, g_aGauge[nCntGauge].pos.y + GAUGEFRAME_HEIGHT - REDUCATIONGAUGE, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aGauge[nCntGauge].pos.x + g_aGauge[nCntGauge].fGauge - REDUCATIONGAUGE, g_aGauge[nCntGauge].pos.y + GAUGEFRAME_HEIGHT - REDUCATIONGAUGE, 0.0f);

		if (g_aGauge[nCntGauge].fGauge >= (GAUGEFRAME_WIDTH - 4.0f))
		{
			g_aGauge[nCntGauge].fGauge = 0.0f;
		}
		pVtx += 4;

	}

	//頂点バッファをアンロックする
	g_pVtxBuffGauge->Unlock();

}

//ゲージの描画処理
void DrawGauge(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffGaugeFrame, 0, sizeof(VERTEX_2D));//頂点バッファへのポインタと頂点情報の構造体のサイズ

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//================================================
	//ゲージフレームの描画処理
	//================================================

	for (int nCntGaugeFrame = 0; nCntGaugeFrame < MAX_GAUGEFRAME; nCntGaugeFrame++)
	{
	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureGaugeFrame[g_aGaugeFrame[nCntGaugeFrame].nType]);

	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,//0は、描画する最初の頂点インデックスで、2は、描画するプリミティブ数
		nCntGaugeFrame * 4,
		2);
	}

	//==============================================================================================================================================================================

	//================================================
	//ゲージの描画処理
	//================================================

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffGauge, 0, sizeof(VERTEX_2D));//頂点バッファへのポインタと頂点情報の構造体のサイズ

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntGauge = 0; nCntGauge < MAX_GAUGE; nCntGauge++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureGauge);

		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,//0は、描画する最初の頂点インデックスで、2は、描画するプリミティブ数
			nCntGauge * 4,
			2);
	}
	//===================================================================================================================================================================================


}

//===================================
//ゲージ加算処理
//===================================
void AddGauge(int nType, int nScrap, int nNextLvScrap)
{
	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffGauge->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntGauge = 0; nCntGauge < MAX_GAUGE; nCntGauge++)
	{
		if (g_aGauge[nCntGauge].nGaugeNum == nType)//種類が一致したら
		{
			g_aGauge[nCntGauge].fGaugeLength = (GAUGEFRAME_WIDTH - 4.0f) / nNextLvScrap;//ゲージフレームの長さを、次のレベルまでのスクラップ数で割り、1スクラップに付きどれぐらいゲージが増えるかどうかを確かめる。
			g_aGauge[nCntGauge].fGauge = g_aGauge[nCntGauge].fGaugeLength * nScrap;
			break;
		}
		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffGauge->Unlock();
}

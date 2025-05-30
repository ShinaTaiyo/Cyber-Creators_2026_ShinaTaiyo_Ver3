//=======================================================================
//
// タイマーの処理
// Author : ShinaTaiyo
//
//=======================================================================

//=======================================================================
//include
//=======================================================================
#include "timer.h"//タイマー

//=======================================================================
//グローバル変数
//=======================================================================
LPDIRECT3DTEXTURE9 g_pTextureTimer = NULL;      //テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTimer = NULL;	//頂点バッファへのポインタ
D3DXVECTOR3 g_posTimer;							//タイマーの位置
Timer g_aTimer[MAX_TIMER];						//タイマーの情報
int g_MathTimer = 0;							//現在のタイマー
int g_KeepNumTimer = 0;							//情報を保存する用
int g_nTimer[MAX_TIMER] = {};					//タイマーの各桁保存用
float g_fanimTimer = 0.0f;						//タイマーのテクスチャ座標
int g_nDelayTimer = 0;							//フレーム数カウント

//=======================================================================
//タイマーの初期化処理
//=======================================================================
void InitTimer(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntTimer; //数をカウントする

	g_nDelayTimer = 0;

	g_fanimTimer = 0.0f;//タイマーのテクスチャ座標

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\timer000.png", &g_pTextureTimer);

	//スコアの位置の初期化
	g_aTimer[0].pos = D3DXVECTOR3(TIMER_POSX, TIMER_POSY, 0.0f);
	g_aTimer[1].pos = D3DXVECTOR3(TIMER_POSX - 40.0f, TIMER_POSY, 0.0f);
	g_aTimer[2].pos = D3DXVECTOR3(TIMER_POSX - 80.0f, TIMER_POSY, 0.0f);

	//タイマーの情報の初期化
	for (nCntTimer = 0; nCntTimer < MAX_TIMER; nCntTimer++)
	{
		g_aTimer[nCntTimer].bUse = true;                       //使用していない状態
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_TIMER, D3DUSAGE_WRITEONLY,FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffTimer, NULL);

#ifdef _DEBUG
	g_MathTimer = MAX_TIME;	//現在のタイマー
#else
	g_MathTimer = MAX_TIME;	//現在のタイマー
#endif // _DEBUG

	//頂点情報のポインタ
	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffTimer->Lock(0, 0, (void**)&pVtx, 0);

	g_KeepNumTimer = g_MathTimer;

	for (nCntTimer = 0; nCntTimer < MAX_TIMER; nCntTimer++)
	{
		g_nTimer[nCntTimer] = g_KeepNumTimer % 10;
		g_KeepNumTimer /= 10;

		switch (g_nTimer[nCntTimer])
		{
		case 0:
			g_fanimTimer = 0.0f;//タイマーのテクスチャ座標
			break;
		case 1:
			g_fanimTimer = 0.1f;//タイマーのテクスチャ座標
			break;
		case 2:
			g_fanimTimer = 0.2f;//タイマーのテクスチャ座標
			break;
		case 3:
			g_fanimTimer = 0.3f;//タイマーのテクスチャ座標
			break;
		case 4:
			g_fanimTimer = 0.4f;//タイマーのテクスチャ座標
			break;
		case 5:
			g_fanimTimer = 0.5f;//タイマーのテクスチャ座標
			break;
		case 6:
			g_fanimTimer = 0.6f;//タイマーのテクスチャ座標
			break;
		case 7:
			g_fanimTimer = 0.7f;//タイマーのテクスチャ座標
			break;
		case 8:
			g_fanimTimer = 0.8f;//タイマーのテクスチャ座標
			break;
		case 9:
			g_fanimTimer = 0.9f;//タイマーのテクスチャ座標
			break;
		}

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aTimer[nCntTimer].pos.x - 25.0f, g_aTimer[nCntTimer].pos.y - 25.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aTimer[nCntTimer].pos.x + 25.0f, g_aTimer[nCntTimer].pos.y - 25.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aTimer[nCntTimer].pos.x - 25.0f, g_aTimer[nCntTimer].pos.y + 25.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aTimer[nCntTimer].pos.x + 25.0f, g_aTimer[nCntTimer].pos.y + 25.0f, 0.0f);

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

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(g_fanimTimer, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(g_fanimTimer + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(g_fanimTimer, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(g_fanimTimer + 0.1f, 1.0f);

		//頂点データのポインタを4つ分進める
		pVtx += 4;
	}

	//頂点バッファのアンロック
	g_pVtxBuffTimer->Unlock();
}

//=======================================================================
//タイマーの終了処理
//=======================================================================
void UninitTimer(void)
{
	//テクスチャの破棄
	if (g_pTextureTimer != NULL)
	{
		g_pTextureTimer->Release();
		g_pTextureTimer = NULL;
	}
	//頂点バッファの破棄
	if (g_pVtxBuffTimer != NULL)
	{
		g_pVtxBuffTimer->Release();
		g_pVtxBuffTimer = NULL;
	}
}

//=======================================================================
//タイマーの更新処理
//=======================================================================
void UpdateTimer(void)
{
	int nCntTimer;
	VERTEX_2D* pVtx;  //頂点情報のポインタ

	g_nDelayTimer++;

	if (g_nDelayTimer % 60 == 0)
	{
		SubTimer(1);//６０フレームごとにタイマーを１秒減らす
	}

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTimer->Lock(0, 0, (void**)&pVtx, 0);

	g_KeepNumTimer = g_MathTimer;

	for (nCntTimer = 0; nCntTimer < MAX_TIMER; nCntTimer++)
	{
		g_nTimer[nCntTimer] = g_KeepNumTimer % 10;
		g_KeepNumTimer /= 10;

		switch (g_nTimer[nCntTimer])
		{
		case 0:
			g_fanimTimer = 0.0f;//タイマーのテクスチャ座標
			break;
		case 1:
			g_fanimTimer = 0.1f;//タイマーのテクスチャ座標
			break;
		case 2:
			g_fanimTimer = 0.2f;//タイマーのテクスチャ座標
			break;
		case 3:
			g_fanimTimer = 0.3f;//タイマーのテクスチャ座標
			break;
		case 4:
			g_fanimTimer = 0.4f;//タイマーのテクスチャ座標
			break;
		case 5:
			g_fanimTimer = 0.5f;//タイマーのテクスチャ座標
			break;
		case 6:
			g_fanimTimer = 0.6f;//タイマーのテクスチャ座標
			break;
		case 7:
			g_fanimTimer = 0.7f;//タイマーのテクスチャ座標
			break;
		case 8:
			g_fanimTimer = 0.8f;//タイマーのテクスチャ座標
			break;
		case 9:
			g_fanimTimer = 0.9f;//タイマーのテクスチャ座標
			break;
		}

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(g_fanimTimer, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(g_fanimTimer + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(g_fanimTimer, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(g_fanimTimer + 0.1f, 1.0f);

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffTimer->Unlock();
}

//=======================================================================
//タイマーの描画処理
//=======================================================================
void DrawTimer(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntTimer;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTimer, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureTimer);

	for (nCntTimer = 0; nCntTimer < MAX_TIMER; nCntTimer++)
	{
		if (g_aTimer[nCntTimer].bUse == true)
		{//スコアが使用されている
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTimer * 4, 2);
		}
	}
}

//=======================================================================
//タイマーの減算処理
//=======================================================================
void SubTimer(int nValue)
{
	//タイマーを一秒減らす
	g_MathTimer -= nValue;
}

//=======================================================================
//タイマーが0になったかどうかを調べる処理
//=======================================================================
bool CheckTime(void)
{
	if (g_MathTimer <= 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//===================================
//現在の残り時間を取得する
//===================================
int GetTimer(void)//現在の残り時間を取得する
{
	return g_MathTimer;
}

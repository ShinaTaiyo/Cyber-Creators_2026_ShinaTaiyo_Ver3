//=======================================================================================================================================================================================================================
//
// コンボ倍率表示の処理
// Author : Atsumu Kuboichi
//
//=======================================================================================================================================================================================================================
#include "combo.h"
#include <cstdio>
#include "player.h"

//=======================================================================================================================================================================================================================
// グローバル変数
//=======================================================================================================================================================================================================================
LPDIRECT3DTEXTURE9 g_pTextureComboUI = NULL;//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffComboUI = NULL;//頂点バッファへのポインタ
ComboUI g_aComboUI;//構造体の情報

LPDIRECT3DTEXTURE9 g_pTextureComboNUM = NULL;//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffComboNUM = NULL;//頂点バッファへのポインタ
ComboNum g_aComboNum[NUM_COMBO];//構造体の情報

//=======================================================================================================================================================================================================================
// 初期化処理
//=======================================================================================================================================================================================================================
void InitCombo(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスの取得
	VERTEX_2D* pVtx;//頂点情報へのポインタ
	int aPosTexU[NUM_COMBO];//各桁の数字を格納
	int nMulti = 1;//各桁の倍数

	//=======================================================
	// コンボUI
	//=======================================================

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\ComboUI000.png",
		&g_pTextureComboUI);	//コンボ倍率数字

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_COMBO,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffComboUI,
		NULL);

	//各桁の数設定
	for (int nCntCombo = 1; nCntCombo <= NUM_COMBO; nCntCombo++)
	{
		nMulti *= 10;
		aPosTexU[NUM_COMBO - nCntCombo] = (10 % nMulti) / (nMulti / 10);
	}

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffComboUI->Lock(0, 0, (void**)&pVtx, 0);
	
	//情報
	g_aComboUI.pos = D3DXVECTOR3(COMBO_UI_POS.x, COMBO_UI_POS.y, COMBO_UI_POS.z);//位置
	g_aComboUI.fLength = sqrtf(COMBO_UI_WIDTH * COMBO_UI_WIDTH + COMBO_UI_HEIGHT * COMBO_UI_HEIGHT) / 2.0f;//対角線の長さ
	g_aComboUI.fAngle = atan2f(COMBO_UI_WIDTH, COMBO_UI_HEIGHT);//対角線の角度
	g_aComboUI.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//色

	//頂点座標の設定
	pVtx[0].pos.x = g_aComboUI.pos.x + sinf(g_aComboUI.pos.z - (D3DX_PI - g_aComboUI.fAngle)) * g_aComboUI.fLength;
	pVtx[0].pos.y = g_aComboUI.pos.y + cosf(g_aComboUI.pos.z - (D3DX_PI - g_aComboUI.fAngle)) * g_aComboUI.fLength;
	pVtx[0].pos.z = 0.0f;
	pVtx[1].pos.x = g_aComboUI.pos.x + sinf(g_aComboUI.pos.z + (D3DX_PI - g_aComboUI.fAngle)) * g_aComboUI.fLength;
	pVtx[1].pos.y = g_aComboUI.pos.y + cosf(g_aComboUI.pos.z + (D3DX_PI - g_aComboUI.fAngle)) * g_aComboUI.fLength;
	pVtx[1].pos.z = 0.0f;
	pVtx[2].pos.x = g_aComboUI.pos.x + sinf(g_aComboUI.pos.z - g_aComboUI.fAngle) * g_aComboUI.fLength;
	pVtx[2].pos.y = g_aComboUI.pos.y + cosf(g_aComboUI.pos.z - g_aComboUI.fAngle) * g_aComboUI.fLength;
	pVtx[2].pos.z = 0.0f;
	pVtx[3].pos.x = g_aComboUI.pos.x + sinf(g_aComboUI.pos.z + g_aComboUI.fAngle) * g_aComboUI.fLength;
	pVtx[3].pos.y = g_aComboUI.pos.y + cosf(g_aComboUI.pos.z + g_aComboUI.fAngle) * g_aComboUI.fLength;
	pVtx[3].pos.z = 0.0f;

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = g_aComboUI.col;
	pVtx[1].col = g_aComboUI.col;
	pVtx[2].col = g_aComboUI.col;
	pVtx[3].col = g_aComboUI.col;

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffComboUI->Unlock();

	//=======================================================
	// コンボ倍率
	//=======================================================

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\ComboNumber000.png",
		&g_pTextureComboNUM);	//コンボ倍率数字

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_COMBO,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffComboNUM,
		NULL);

	//各桁の数設定
	for (int nCntCombo = 1; nCntCombo <= NUM_COMBO; nCntCombo++)
	{
		nMulti *= 10;
		aPosTexU[NUM_COMBO - nCntCombo] = (10 % nMulti) / (nMulti / 10);
	}

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffComboNUM->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntCombo = 0; nCntCombo < NUM_COMBO; nCntCombo++)
	{
		//情報
		g_aComboNum[nCntCombo].pos = D3DXVECTOR3(COMBO_NUM_POS.x + (COMBO_NUM_SPACE * nCntCombo), COMBO_NUM_POS.y, COMBO_NUM_POS.z);//位置
		g_aComboNum[nCntCombo].fLength = sqrtf(COMBO_NUM_WIDTH * COMBO_NUM_WIDTH + COMBO_NUM_HEIGHT * COMBO_NUM_HEIGHT) / 2.0f;//対角線の長さ
		g_aComboNum[nCntCombo].fAngle = atan2f(COMBO_NUM_WIDTH, COMBO_NUM_HEIGHT);//対角線の角度
		g_aComboNum[nCntCombo].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//色

		//頂点座標の設定
		pVtx[0].pos.x = g_aComboNum[nCntCombo].pos.x + sinf(g_aComboNum[nCntCombo].pos.z - (D3DX_PI - g_aComboNum[nCntCombo].fAngle)) * g_aComboNum[nCntCombo].fLength;
		pVtx[0].pos.y = g_aComboNum[nCntCombo].pos.y + cosf(g_aComboNum[nCntCombo].pos.z - (D3DX_PI - g_aComboNum[nCntCombo].fAngle)) * g_aComboNum[nCntCombo].fLength;
		pVtx[0].pos.z = 0.0f;
		pVtx[1].pos.x = g_aComboNum[nCntCombo].pos.x + sinf(g_aComboNum[nCntCombo].pos.z + (D3DX_PI - g_aComboNum[nCntCombo].fAngle)) * g_aComboNum[nCntCombo].fLength;
		pVtx[1].pos.y = g_aComboNum[nCntCombo].pos.y + cosf(g_aComboNum[nCntCombo].pos.z + (D3DX_PI - g_aComboNum[nCntCombo].fAngle)) * g_aComboNum[nCntCombo].fLength;
		pVtx[1].pos.z = 0.0f;
		pVtx[2].pos.x = g_aComboNum[nCntCombo].pos.x + sinf(g_aComboNum[nCntCombo].pos.z - g_aComboNum[nCntCombo].fAngle) * g_aComboNum[nCntCombo].fLength;
		pVtx[2].pos.y = g_aComboNum[nCntCombo].pos.y + cosf(g_aComboNum[nCntCombo].pos.z - g_aComboNum[nCntCombo].fAngle) * g_aComboNum[nCntCombo].fLength;
		pVtx[2].pos.z = 0.0f;
		pVtx[3].pos.x = g_aComboNum[nCntCombo].pos.x + sinf(g_aComboNum[nCntCombo].pos.z + g_aComboNum[nCntCombo].fAngle) * g_aComboNum[nCntCombo].fLength;
		pVtx[3].pos.y = g_aComboNum[nCntCombo].pos.y + cosf(g_aComboNum[nCntCombo].pos.z + g_aComboNum[nCntCombo].fAngle) * g_aComboNum[nCntCombo].fLength;
		pVtx[3].pos.z = 0.0f;

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = g_aComboNum[nCntCombo].col;
		pVtx[1].col = g_aComboNum[nCntCombo].col;
		pVtx[2].col = g_aComboNum[nCntCombo].col;
		pVtx[3].col = g_aComboNum[nCntCombo].col;

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(aPosTexU[nCntCombo] / 10.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2((aPosTexU[nCntCombo] + 1) / 10.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(aPosTexU[nCntCombo] / 10.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2((aPosTexU[nCntCombo] + 1) / 10.0f, 1.0f);

		//頂点データのポインタを進める
		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffComboNUM->Unlock();
}

//=======================================================================================================================================================================================================================
// 終了処理
//=======================================================================================================================================================================================================================
void UninitCombo(void)
{
	//頂点バッファの破棄
	if (g_pVtxBuffComboUI != NULL)
	{
		g_pVtxBuffComboUI->Release();
		g_pVtxBuffComboUI = NULL;
	}
	if (g_pVtxBuffComboNUM != NULL)
	{
		g_pVtxBuffComboNUM->Release();
		g_pVtxBuffComboNUM = NULL;
	}
	//テクスチャの破棄
	if (g_pTextureComboUI != NULL)
	{
		g_pTextureComboUI->Release();
		g_pTextureComboUI = NULL;
	}
	if (g_pTextureComboNUM != NULL)
	{
		g_pTextureComboNUM->Release();
		g_pTextureComboNUM = NULL;
	}
}

//=======================================================================================================================================================================================================================
// 更新処理
//=======================================================================================================================================================================================================================
void UpdateCombo(void)
{
	VERTEX_2D* pVtx;//頂点情報へのポインタ
	Player* pPlayer = GetPlayer();//プレイヤーの情報を取得
	int aPosTexU[NUM_COMBO];//各桁の数字を格納
	int nMulti = 1;//各桁の倍数

	//各桁の数字を更新
	for (int nCntCombo = 1; nCntCombo <= NUM_COMBO; nCntCombo++)
	{
		nMulti *= 10;
		aPosTexU[NUM_COMBO - nCntCombo] = ((int)(pPlayer->fCombo * 10) % nMulti) / (nMulti / 10);
	}

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffComboNUM->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntCombo = 0; nCntCombo < NUM_COMBO; nCntCombo++)
	{
		//テクスチャ座標の更新
		pVtx[0].tex = D3DXVECTOR2(aPosTexU[nCntCombo] / 10.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2((aPosTexU[nCntCombo] + 1) / 10.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(aPosTexU[nCntCombo] / 10.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2((aPosTexU[nCntCombo] + 1) / 10.0f, 1.0f);

		//色の更新
		g_aComboNum[nCntCombo].col = D3DXCOLOR(1.0f, MAX_COMBO - pPlayer->fCombo, MAX_COMBO - pPlayer->fCombo, 1.0f);

		if (pPlayer->fCombo <= 1.0f)
		{//コンボ倍率が1.0の場合は非表示
			g_aComboNum[nCntCombo].col.a = 0.0f;
		}

		//頂点カラーの更新
		pVtx[0].col = g_aComboNum[nCntCombo].col;
		pVtx[1].col = g_aComboNum[nCntCombo].col;
		pVtx[2].col = g_aComboNum[nCntCombo].col;
		pVtx[3].col = g_aComboNum[nCntCombo].col;

		//頂点データのポインタを進める
		pVtx += 4;
	}

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffComboUI->Lock(0, 0, (void**)&pVtx, 0);

	//頂点カラーの更新
	pVtx[0].col = g_aComboNum[0].col;
	pVtx[1].col = g_aComboNum[0].col;
	pVtx[2].col = g_aComboNum[0].col;
	pVtx[3].col = g_aComboNum[0].col;

	//頂点バッファをアンロックする
	g_pVtxBuffComboUI->Unlock();

	//頂点バッファをアンロックする
	g_pVtxBuffComboNUM->Unlock();
}

//=======================================================================================================================================================================================================================
// コンボUI描画処理
//=======================================================================================================================================================================================================================
void DrawComboUI(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスの取得
	pDevice->SetStreamSource(0, g_pVtxBuffComboUI, 0, sizeof(VERTEX_2D));//頂点バッファをデータストリームに設定
	pDevice->SetFVF(FVF_VERTEX_2D);//頂点フォーマットの設定
	pDevice->SetTexture(0, g_pTextureComboUI);//テクスチャの設定
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);//ポリゴンの描画
}

//=======================================================================================================================================================================================================================
// コンボ倍率描画処理
//=======================================================================================================================================================================================================================
void DrawComboNum(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスの取得
	pDevice->SetStreamSource(0, g_pVtxBuffComboNUM, 0, sizeof(VERTEX_2D));//頂点バッファをデータストリームに設定
	pDevice->SetFVF(FVF_VERTEX_2D);//頂点フォーマットの設定
	pDevice->SetTexture(0, g_pTextureComboNUM);//テクスチャの設定

	//ポリゴンの描画
	for (int nCntCombo = 0; nCntCombo < NUM_COMBO; nCntCombo++)
	{
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntCombo * 4, 2);
	}
}

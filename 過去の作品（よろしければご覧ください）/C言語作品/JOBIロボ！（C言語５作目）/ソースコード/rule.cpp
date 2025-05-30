//=======================================================================================================================================================================================================================
//
// ルール説明画面の処理
// Author : Atsumu Kuboichi
//
//=======================================================================================================================================================================================================================
#include "rule.h"
#include "input.h"
#include "tutorial.h"

//=======================================================================================================================================================================================================================
// グローバル変数
//=======================================================================================================================================================================================================================
LPDIRECT3DTEXTURE9 g_pTextureRule[NUM_PAGE] = {};	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRule = NULL;		//頂点バッファへのポインタ
Rule g_aRule;										//構造体情報

//=======================================================================================================================================================================================================================
// 初期化処理
//=======================================================================================================================================================================================================================
void InitRule(void)
{
	VERTEX_2D* pVtx;						//頂点情報へのポインタ
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスの取得

	//テクスチャの読み込み
	for (int nCntBack = 0; nCntBack < NUM_PAGE; nCntBack++)
	{
		D3DXCreateTextureFromFile(pDevice, RULESCREEN_FILENAME[nCntBack], &g_pTextureRule[nCntBack]);
	}

	//構造体の情報
	g_aRule.nPage = 0;											//ページ数
	g_aRule.fWidthScale = SCREEN_WIDTH / (float)SPLIT_SCALE;	//X軸拡縮率
	g_aRule.fHeightScale = SCREEN_HEIGHT / (float)SPLIT_SCALE;	//Y軸拡縮率
	g_aRule.bScale = true;										//拡縮の切り替え
	g_aRule.bUse = true;										//使用状態

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRule,
		NULL);

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffRule->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntVtx = 0; nCntVtx < 4; nCntVtx++)
	{
		pVtx[nCntVtx].rhw = 1.0f;								//rhwの設定
		pVtx[nCntVtx].col = D3DCOLOR_RGBA(255, 255, 255, 255);	//頂点カラーの設定
	}

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffRule->Unlock();
}

//=======================================================================================================================================================================================================================
// 終了処理
//=======================================================================================================================================================================================================================
void UninitRule(void)
{
	//頂点バッファの破棄
	if (g_pVtxBuffRule != NULL)
	{
		g_pVtxBuffRule->Release();
		g_pVtxBuffRule = NULL;
	}
	//テクスチャの破棄
	for (int nCntRule = 0; nCntRule < NUM_PAGE; nCntRule++)
	{
		if (g_pTextureRule[nCntRule] != NULL)
		{
			g_pTextureRule[nCntRule]->Release();
			g_pTextureRule[nCntRule] = NULL;
		}
	}
}

//=======================================================================================================================================================================================================================
// 更新処理
//=======================================================================================================================================================================================================================
void UpdateRule(void)
{
	VERTEX_2D* pVtx;//頂点情報へのポインタ

	if (GetJoypadTrigger(JOYKEY_A))
	{//Aボタンが押された場合
		if (g_aRule.nPage < NUM_PAGE - 1)
		{//最後のページではない場合
			g_aRule.nPage++;//ページを進める
		}
		else
		{//最後のページの場合
			g_aRule.bScale = false;	//画面を縮小する
		}
	}
	else if (GetJoypadTrigger(JOYKEY_B))
	{//Bボタンが押された場合
		if (g_aRule.nPage != 0)
		{//最初のページではない場合
			g_aRule.nPage--;//ページを戻る
		}
	}

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffRule->Lock(0, 0, (void**)&pVtx, 0);

	if (g_aRule.bScale)
	{//拡大中の場合
		if (pVtx[0].pos != D3DXVECTOR3(0.0f, 0.0f, 0.0f))
		{//頂点が最大まで達していない場合
			//拡大
			pVtx[0].pos.x -= g_aRule.fWidthScale;
			pVtx[0].pos.y -= g_aRule.fHeightScale;
			pVtx[1].pos.x += g_aRule.fWidthScale;
			pVtx[1].pos.y -= g_aRule.fHeightScale;
			pVtx[2].pos.x -= g_aRule.fWidthScale;
			pVtx[2].pos.y += g_aRule.fHeightScale;
			pVtx[3].pos.x += g_aRule.fWidthScale;
			pVtx[3].pos.y += g_aRule.fHeightScale;
		}
	}
	else
	{//縮小中の場合
		if (pVtx[0].pos != D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f))
		{//頂点が最小の位置まで達していない場合
			//縮小
			pVtx[0].pos.x += g_aRule.fWidthScale;
			pVtx[0].pos.y += g_aRule.fHeightScale;
			pVtx[1].pos.x -= g_aRule.fWidthScale;
			pVtx[1].pos.y += g_aRule.fHeightScale;
			pVtx[2].pos.x += g_aRule.fWidthScale;
			pVtx[2].pos.y -= g_aRule.fHeightScale;
			pVtx[3].pos.x -= g_aRule.fWidthScale;
			pVtx[3].pos.y -= g_aRule.fHeightScale;
		}
		else
		{//頂点が最小の位置まで達している場合
			g_aRule.bUse = false;	//ルール説明画面を閉じる
		}
	}

	//頂点バッファをアンロックする
	g_pVtxBuffRule->Unlock();
}

//=======================================================================================================================================================================================================================
// 描画処理
//=======================================================================================================================================================================================================================
void DrawRule(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();							//デバイスの取得
	pDevice->SetStreamSource(0, g_pVtxBuffRule, 0, sizeof(VERTEX_2D));	//頂点バッファをデータストリームに設定
	pDevice->SetFVF(FVF_VERTEX_2D);										//頂点フォーマットの設定
	pDevice->SetTexture(0, g_pTextureRule[g_aRule.nPage]);				//テクスチャの設定
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);					//ポリゴンの描画
}

//=======================================================================================================================================================================================================================
// 構造体の情報の取得
//=======================================================================================================================================================================================================================
Rule* GetRuleScreen(void)
{
	return &g_aRule;
}

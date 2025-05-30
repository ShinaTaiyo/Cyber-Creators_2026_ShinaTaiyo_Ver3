//========================================================
//
// リザルトのスコアの処理
// Author : Atsumu Kuboichi
//
//=========================================================
#include "resultscore.h"
#include "score.h"
#include "player.h"

//=========================================================
// グローバル変数
//=========================================================
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResultScore = NULL;
LPDIRECT3DTEXTURE9 g_pTextureResultScore = NULL;				//テクスチャへのポインタ
ResultScore g_aResultScore[NUM_RESULTSCORE][MAX_RESULTSCORE];	//構造体情報
int g_nNumResutScore;											//桁数

//=========================================================
// 初期化処理
//=========================================================
void InitResultScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスの取得
	VERTEX_2D* pVtx;						//頂点情報へのポインタ
	
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\timer000.png",
		&g_pTextureResultScore);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * (NUM_RESULTSCORE * MAX_RESULTSCORE),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffResultScore,
		NULL);

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffResultScore->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntNumScore = 0; nCntNumScore < NUM_RESULTSCORE; nCntNumScore++)
	{
		for (int nCntResultScore = 0; nCntResultScore < MAX_RESULTSCORE; nCntResultScore++)
		{
			switch (nCntNumScore)
			{
			case 0:	//ゲームスコア
				g_aResultScore[nCntNumScore][nCntResultScore].fWidth = 100.0f;	//横幅
				g_aResultScore[nCntNumScore][nCntResultScore].pos = D3DXVECTOR3(630.0f + (g_aResultScore[nCntNumScore][nCntResultScore].fWidth * nCntResultScore), 230.0f, 0.0f);	//位置
				g_aResultScore[nCntNumScore][nCntResultScore].bUse = true;	//使用する
				break;

			case 1:	//敵撃破数
				g_aResultScore[nCntNumScore][nCntResultScore].fWidth = 90.0f;	//横幅
				g_aResultScore[nCntNumScore][nCntResultScore].pos = D3DXVECTOR3(530.0f + (g_aResultScore[nCntNumScore][nCntResultScore].fWidth * nCntResultScore), 470.0f, 0.0f);	//位置

				if (nCntResultScore < MAX_RESULTSCORE - MAX_RESULTENEMY)
				{
					g_aResultScore[nCntNumScore][nCntResultScore].bUse = false;	//使用しない
				}
				else
				{
					g_aResultScore[nCntNumScore][nCntResultScore].bUse = true;	//使用する
				}
				break;

			case 2:	//スクラップ回収数
				g_aResultScore[nCntNumScore][nCntResultScore].fWidth = 90.0f;	//横幅
				g_aResultScore[nCntNumScore][nCntResultScore].pos = D3DXVECTOR3(590.0f + (g_aResultScore[nCntNumScore][nCntResultScore].fWidth * nCntResultScore), 660.0f, 0.0f);	//位置

				if (nCntResultScore < MAX_RESULTSCORE - MAX_RESULTSCRAP)
				{
					g_aResultScore[nCntNumScore][nCntResultScore].bUse = false;	//使用しない
				}
				else
				{
					g_aResultScore[nCntNumScore][nCntResultScore].bUse = true;	//使用する
				}
				break;

			default:
				break;
			}

			g_aResultScore[nCntNumScore][nCntResultScore].fHeight = g_aResultScore[nCntNumScore][nCntResultScore].fWidth * 2.0f;//高さ

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aResultScore[nCntNumScore][nCntResultScore].pos.x - g_aResultScore[nCntNumScore][nCntResultScore].fWidth / 2.0f,
				g_aResultScore[nCntNumScore][nCntResultScore].pos.y - g_aResultScore[nCntNumScore][nCntResultScore].fWidth / 2.0f,						
				0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aResultScore[nCntNumScore][nCntResultScore].pos.x + g_aResultScore[nCntNumScore][nCntResultScore].fWidth / 2.0f, 
				g_aResultScore[nCntNumScore][nCntResultScore].pos.y - g_aResultScore[nCntNumScore][nCntResultScore].fWidth / 2.0f, 
				0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aResultScore[nCntNumScore][nCntResultScore].pos.x - g_aResultScore[nCntNumScore][nCntResultScore].fWidth / 2.0f, 
				g_aResultScore[nCntNumScore][nCntResultScore].pos.y + g_aResultScore[nCntNumScore][nCntResultScore].fWidth / 2.0f, 
				0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aResultScore[nCntNumScore][nCntResultScore].pos.x + g_aResultScore[nCntNumScore][nCntResultScore].fWidth / 2.0f, 
				g_aResultScore[nCntNumScore][nCntResultScore].pos.y + g_aResultScore[nCntNumScore][nCntResultScore].fWidth / 2.0f, 
				0.0f);

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
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

			pVtx += 4;	//頂点データのポインタを４つ分進める。
		}
	}

	//頂点バッファをアンロックする
	g_pVtxBuffResultScore->Unlock();
}

//=========================================================
// 終了処理
//=========================================================
void UninitResultScore(void)
{
	//テクスチャの破棄
	if (g_pTextureResultScore != NULL)
	{//テクスチャ情報が存在する場合
		g_pTextureResultScore->Release();
		g_pTextureResultScore = NULL;
	}
	//頂点バッファの破棄
	if (g_pVtxBuffResultScore != NULL)
	{//頂点情報が存在する場合
		g_pVtxBuffResultScore->Release();
		g_pVtxBuffResultScore = NULL;
	}
}

//=========================================================
// 更新処理
//=========================================================
void UpdateResultScore(void)
{
	VERTEX_2D* pVtx;								//頂点情報へのポインタ
	int aPosTexU[NUM_RESULTSCORE][MAX_RESULTSCORE];	//各桁の数字を格納
	int nMulti = 1;									//各桁の倍数
	int nScore = 0;									//スコア

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffResultScore->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntNumScore = 0; nCntNumScore < NUM_RESULTSCORE; nCntNumScore++)
	{
		switch (nCntNumScore)
		{//取得するスコア
		case 0:	//ゲームスコア
			nScore = GetScore();//ゲームスコアの取得
			break;

		case 1:	//敵撃破数
			nScore = GetPlayer()->nTotalDefeatEnemy;
			break;

		case 2:	//スクラップ回収数
			nScore = GetPlayer()->nTotalGetScrap;
			break;

		default:
			break;
		}

		//スコアを桁ごとに取得
		for (int nCntResultScore = 1; nCntResultScore <= MAX_RESULTSCORE; nCntResultScore++)
		{
			nMulti *= 10;
			aPosTexU[nCntNumScore][MAX_SCORE - nCntResultScore] = (nScore % nMulti) / (nMulti / 10);
		}

		nMulti = 1;	//各桁の倍数リセット

		for (int nCntResultScore = 0; nCntResultScore < MAX_RESULTSCORE; nCntResultScore++)
		{
			//テクスチャ座標の更新
			pVtx[0].tex = D3DXVECTOR2(aPosTexU[nCntNumScore][nCntResultScore] / 10.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2((aPosTexU[nCntNumScore][nCntResultScore] + 1) / 10.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(aPosTexU[nCntNumScore][nCntResultScore] / 10.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2((aPosTexU[nCntNumScore][nCntResultScore] + 1) / 10.0f, 1.0f);

			//頂点データのポインタを進める
			pVtx += 4;
		}
	}

	//頂点バッファをアンロックする
	g_pVtxBuffResultScore->Unlock();
}

//=========================================================
// 描画処理
//=========================================================
void DrawResultScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスの取得

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffResultScore, 0, sizeof(VERTEX_2D));	//頂点バッファへのポインタと頂点情報の構造体のサイズ

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntNumScore = 0; nCntNumScore < NUM_RESULTSCORE; nCntNumScore++)
	{
		for (int nCntResultScore = 0; nCntResultScore < MAX_RESULTSCORE; nCntResultScore++)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureResultScore);

			if (g_aResultScore[nCntNumScore][nCntResultScore].bUse)
			{//該当する桁が使用されている場合
				//ポリゴンの描画
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntResultScore + (nCntNumScore * MAX_RESULTSCORE)) * 4, 2);
			}
		}
	}
}

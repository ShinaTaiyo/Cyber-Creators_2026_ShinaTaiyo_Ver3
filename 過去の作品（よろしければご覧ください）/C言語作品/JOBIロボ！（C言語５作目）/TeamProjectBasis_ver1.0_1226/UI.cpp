//================================================================================================================================================================================================================================
//
// ゲームUIの処理
// Author : Atsumu Kuboichi
//
//================================================================================================================================================================================================================================
#include "UI.h"
#include "model.h"
#include "player.h"

//================================================================================================================================================================================================================================
// グローバル変数
//================================================================================================================================================================================================================================
LPDIRECT3DTEXTURE9 g_pTextureUI[NUM_UI] = {};	//テクスチャへのポインタ
PDIRECT3DVERTEXBUFFER9 g_pVtxBuffUI = NULL;		//頂点バッファへのポインタ
UI g_aUI[NUM_UI];								//構造体の情報

//================================================================================================================================================================================================================================
// 初期化処理
//================================================================================================================================================================================================================================
void InitUI(void)
{
	VERTEX_2D* pVtx;						//頂点情報へのポインタ
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスの取得

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_UI,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffUI,
		NULL);

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffUI->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntUI = 0; nCntUI < NUM_UI; nCntUI++)
	{
		//テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			UITEXTURE_FILENAME[nCntUI],
			&g_pTextureUI[nCntUI]);

		//位置
		g_aUI[nCntUI].pos = UI_POS[nCntUI];
		g_aUI[nCntUI].bUse = true;

		if (nCntUI == 2)
		{
			g_aUI[nCntUI].bUse = false;
		}

		//位置情報の初期化
		pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - UI_WIDTH[nCntUI] / 2.0f, g_aUI[nCntUI].pos.y - UI_HEIGHT[nCntUI] / 2.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + UI_WIDTH[nCntUI] / 2.0f, g_aUI[nCntUI].pos.y - UI_HEIGHT[nCntUI] / 2.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - UI_WIDTH[nCntUI] / 2.0f, g_aUI[nCntUI].pos.y + UI_HEIGHT[nCntUI] / 2.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + UI_WIDTH[nCntUI] / 2.0f, g_aUI[nCntUI].pos.y + UI_HEIGHT[nCntUI] / 2.0f, 0.0f);

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
	g_pVtxBuffUI->Unlock();
}

//================================================================================================================================================================================================================================
// 終了処理
//================================================================================================================================================================================================================================
void UninitUI(void)
{
	//テクスチャの破棄
	for (int nCntUI = 0; nCntUI < NUM_UI; nCntUI++)
	{
		if (g_pTextureUI[nCntUI] != NULL)
		{
			g_pTextureUI[nCntUI]->Release();
			g_pTextureUI[nCntUI] = NULL;
		}
	}
	//頂点バッファの破棄
	if (g_pVtxBuffUI != NULL)
	{
		g_pVtxBuffUI->Release();
		g_pVtxBuffUI = NULL;
	}
}

//================================================================================================================================================================================================================================
// 更新処理
//================================================================================================================================================================================================================================
void UpdateUI(void)
{
	Player* pPlayer = GetPlayer();	//プレイヤー構造体の情報を取得
	Model* pModel = GetModel();		//モデル構造体の情報を取得
	float fLength = 3000.0f;		//各コンセントとの最小距離

	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++, pModel++)
	{
		if (pModel->bUse && (pModel->nType == 19 || pModel->nType == 41))
		{//コンセントが使用されている場合
			//プレイヤーとの距離を計算
			pPlayer->fAutletLength = sqrtf((pPlayer->pos.x - pModel->pos.x) * (pPlayer->pos.x - pModel->pos.x) + (pPlayer->pos.z - pModel->pos.z) * (pPlayer->pos.z - pModel->pos.z)) / 2.0f;

			if (fLength > pPlayer->fAutletLength)
			{//計算した距離が最小値の場合
				fLength = pPlayer->fAutletLength;//最小距離を更新
			}

			if (fLength <= 50.0f)
			{//コンセントの近くにいる場合
				g_aUI[2].bUse = true;//使用する
			}
			else
			{//コンセントの近くにいない場合
				g_aUI[2].bUse = false;//使用しない
			}
		}
	}
}

//================================================================================================================================================================================================================================
// 描画処理
//================================================================================================================================================================================================================================
void DrawUI(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスの取得

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffUI, 0, sizeof(VERTEX_2D));
	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntUI = 0; nCntUI < NUM_UI; nCntUI++)
	{
		if (g_aUI[nCntUI].bUse)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureUI[nCntUI]);
			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntUI * 4, 2);
		}
	}
}

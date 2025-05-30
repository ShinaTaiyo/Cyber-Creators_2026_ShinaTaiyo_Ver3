//================================================================================================================================================================================================================================
//
// ビルボードの処理
// Author : Atsumu Kuboichi
//
//================================================================================================================================================================================================================================
#include "billboard.h"
#include "debugproc.h"
#include "player.h"

//================================================================================================================================================================================================================================
// グローバル変数
//================================================================================================================================================================================================================================
PDIRECT3DVERTEXBUFFER9 g_pVtxBuffBillboard = NULL;							//頂点バッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureBillboard[(int)USE_MAX][NUM_BILLBOARD] = {};	//テクスチャへのポインタ
Billboard g_aBillboard[MAX_BILLBOARD];										//構造体情報

//================================================================================================================================================================================================================================
// 初期化処理
//================================================================================================================================================================================================================================
void InitBillboard(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスの取得
	VERTEX_3D* pVtx;						//頂点情報へのポインタ

	//テクスチャの読み込み
	for (int nCntTex = 0; nCntTex < (int)USE_MAX; nCntTex++)
	{
		if (nCntTex == 0)
		{//ゲーム用
			for (int nCntTex2 = 0; nCntTex2 < GAME_BILLBOARD; nCntTex2++)
			{
				D3DXCreateTextureFromFile(pDevice,
					GAME_BILLBOARDFILE[nCntTex2],
					&g_pTextureBillboard[nCntTex][nCntTex2]);
			}
		}
		else if (nCntTex == 1)
		{//チュートリアル用
			for (int nCntTex2 = 0; nCntTex2 < TUTORIAL_BILLBOARD; nCntTex2++)
			{
				D3DXCreateTextureFromFile(pDevice,
					TUTORIAL_BILLBOARDFILE[nCntTex2],
					&g_pTextureBillboard[nCntTex][nCntTex2]);
			}
		}
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_BILLBOARD,//頂点の数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBillboard,
		NULL);

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBillboard->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBillboard = 0; nCntBillboard < MAX_BILLBOARD; nCntBillboard++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-g_aBillboard[nCntBillboard].fWidth / 2.0f, g_aBillboard[nCntBillboard].fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aBillboard[nCntBillboard].fWidth / 2.0f, g_aBillboard[nCntBillboard].fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-g_aBillboard[nCntBillboard].fWidth / 2.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aBillboard[nCntBillboard].fWidth / 2.0f, 0.0f, 0.0f);

		for (int nCntVtx = 0; nCntVtx < 4; nCntVtx++)
		{
			//norの設定
			pVtx[nCntVtx].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			//頂点カラーの設定
			pVtx[nCntVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffBillboard->Unlock();
}

//================================================================================================================================================================================================================================
// 終了処理
//================================================================================================================================================================================================================================
void UninitBillboard(void)
{
	//テクスチャの破棄
	for (int nCntTex = 0; nCntTex < (int)USE_MAX; nCntTex++)
	{
		for (int nCntTex2 = 0; nCntTex2 < NUM_BILLBOARD; nCntTex2++)
		{
			if (g_pTextureBillboard[nCntTex][nCntTex2] != NULL)
			{//テクスチャが存在する場合
				g_pTextureBillboard[nCntTex][nCntTex2]->Release();
				g_pTextureBillboard[nCntTex][nCntTex2] = NULL;
			}
		}
	}
	//頂点バッファの破棄
	if (g_pVtxBuffBillboard != NULL)
	{//頂点情報が存在する場合
		g_pVtxBuffBillboard->Release();
		g_pVtxBuffBillboard = NULL;
	}
}

//================================================================================================================================================================================================================================
// 更新処理
//================================================================================================================================================================================================================================
void UpdateBillboard(void)
{
	for (int nCntBillboard = 0; nCntBillboard < MAX_BILLBOARD; nCntBillboard++)
	{
		if ((GetMode() == MODE_TUTORIAL && g_aBillboard[nCntBillboard].nUse == (int)USE_TUTORIAL)
			|| (GetMode() == MODE_GAME && g_aBillboard[nCntBillboard].nUse == (int)USE_GAME))
		{//現在の画面と使用場面が一致する場合
			g_aBillboard[nCntBillboard].bUse = true;//使用する
		}
		else
		{//一致しない場合
			g_aBillboard[nCntBillboard].bUse = false;//使用しない
		}
	}
}

//================================================================================================================================================================================================================================
// 描画処理
//================================================================================================================================================================================================================================
void DrawBillboard(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスの取得
	D3DXMATRIX mtxTrans;					//計算用マトリックス
	D3DXMATRIX mtxView;						//ビューマトリックス取得用

	for (int nCntBillboard = 0; nCntBillboard < MAX_BILLBOARD; nCntBillboard++)
	{
		if (g_aBillboard[nCntBillboard].bUse)
		{//使用している場合
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aBillboard[nCntBillboard].mtxWorld);

			//ビューマトリックスを取得
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//ポリゴンをカメラに対して正面に向ける
			D3DXMatrixInverse(&g_aBillboard[nCntBillboard].mtxWorld, NULL, &mtxView);

			//逆行列を求める
			g_aBillboard[nCntBillboard].mtxWorld._41 = 0.0f;
			g_aBillboard[nCntBillboard].mtxWorld._42 = 0.0f;
			g_aBillboard[nCntBillboard].mtxWorld._43 = 0.0f;

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aBillboard[nCntBillboard].pos.x, g_aBillboard[nCntBillboard].pos.y, g_aBillboard[nCntBillboard].pos.z);
			D3DXMatrixMultiply(&g_aBillboard[nCntBillboard].mtxWorld, &g_aBillboard[nCntBillboard].mtxWorld, &mtxTrans);

			//ライトを無効にする
			pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

			//Zの比較方法変更。
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

			//Zバッファに書き込まない
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aBillboard[nCntBillboard].mtxWorld);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffBillboard, 0, sizeof(VERTEX_3D));//頂点バッファへのポインタと頂点情報の構造体のサイズ

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//使用場面と種類を保存
			int nUse = g_aBillboard[nCntBillboard].nUse;
			int nType = g_aBillboard[nCntBillboard].nType;

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureBillboard[nUse][nType]);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBillboard * 4, 2);

			//ライトを有効に戻す
			pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

			//Zの比較方法変更。
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

			//Zバッファに書き込む
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		}
	}
}

//================================================================================================================================================================================================================================
// 設定処理
//================================================================================================================================================================================================================================
void SetBillboard(D3DXVECTOR3 pos, int nUse, int nType, float fWidth, float fHeight)
{
	VERTEX_3D* pVtx;//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBillboard->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBillboard = 0; nCntBillboard < MAX_BILLBOARD; nCntBillboard++)
	{
		if (!g_aBillboard[nCntBillboard].bUse)
		{//使用されていない場合
			g_aBillboard[nCntBillboard].pos = pos;										//位置
			g_aBillboard[nCntBillboard].nUse = nUse;									//使用場面
			g_aBillboard[nCntBillboard].nType = nType;									//種類
			g_aBillboard[nCntBillboard].fWidth = fWidth;								//横幅
			g_aBillboard[nCntBillboard].fHeight = fHeight;								//高さ
			g_aBillboard[nCntBillboard].fWidthScale = (fWidth / 2.0f) / DIVISION_SCALE;	//横幅拡縮率
			g_aBillboard[nCntBillboard].fHeightScale = fHeight / DIVISION_SCALE;		//高さ拡縮率

			//頂点座標の設定
			if (nUse == 0 && nType == 0)
			{//「Aで回復」の場合
				for (int nCntVtx = 0; nCntVtx < 4; nCntVtx++)
				{
					pVtx[nCntVtx].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				}
			}
			else
			{
				pVtx[0].pos = D3DXVECTOR3(-g_aBillboard[nCntBillboard].fWidth / 2.0f, g_aBillboard[nCntBillboard].fHeight, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aBillboard[nCntBillboard].fWidth / 2.0f, g_aBillboard[nCntBillboard].fHeight, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(-g_aBillboard[nCntBillboard].fWidth / 2.0f, 0.0f, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aBillboard[nCntBillboard].fWidth / 2.0f, 0.0f, 0.0f);
			}

			break;
		}

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffBillboard->Unlock();
}
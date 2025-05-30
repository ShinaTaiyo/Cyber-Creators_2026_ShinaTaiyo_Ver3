//========================================================
//
//１２月２６日：モデル読み込み[Item.cpp]
//Author:ShinaTaiyo
//
//=========================================================
#include "Item.h"
#include "main.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "shadow.h"
#include "player.h"
#include <stdio.h>
#include <string.h>
#include "model.h"
#include "score.h"
#include "bullet.h"
#include "particle.h"
#include "mission.h"

//=============================
//グローバル変数
//=============================
int g_nNumItem = 0;//読み込むモデルの最大数を決める
int g_nDelayItem = 0;
D3DXVECTOR3 g_posItem;//位置
D3DXVECTOR3 g_rotItem;//向き
D3DXMATRIX g_mtxWorldItem;//ワールドマトリックス
Item g_aItem[MAX_ITEM];//モデル情報の構造体
ItemMat g_aItemMat[NUM_ITEM];//モデルのマテリアルや頂点の構造体

//=============================
//キャンディ表示
//=============================
CandyDisp g_aCandyDisp;//構造体
LPDIRECT3DTEXTURE9 g_pTextureCandyDisp = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffCandyDisp = NULL;

//=============================
//キャンディ獲得数表示
//=============================
CandyNum g_aCandyNum[CANDY_DIGIT];//構造体
LPDIRECT3DTEXTURE9 g_pTextureCandyNum = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffCandyNum = NULL;

int g_nldxShadowItem = -1;//対象の影のインデックス（番号）

//==============================
// 初期化処理
//==============================
void InitItem(void)
{
	g_nNumItem = 2;

	g_nDelayItem = 0;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスへのポインタと取得

	g_posItem = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_rotItem = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	for (int nCntMatItem = 0; nCntMatItem < NUM_ITEM; nCntMatItem++)
	{
		g_aItemMat[nCntMatItem].dwNumMatItem = {};
		g_aItemMat[nCntMatItem].pBuffMatItem = {};
		g_aItemMat[nCntMatItem].pMeshItem = {};

		for (int nCntTexture = 0; nCntTexture < MAX_ITEMTEXTURE; nCntTexture++)
		{
			g_aItemMat[nCntMatItem].apTextureItem[nCntTexture] = {};//一つのモデルに関連するテクスチャの初期化
		}
	}
	for (int nCnt = 0; nCnt < MAX_ITEM; nCnt++)
	{
		g_aItem[nCnt].bUse = false;//使用しているかどうか
		g_aItem[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//位置
		g_aItem[nCnt].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//1F前の位置
		g_aItem[nCnt].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//移動量
		g_aItem[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//向き
		g_aItem[nCnt].nIdxShadow = -1;
		g_aItem[nCnt].nType = -1;
		g_aItem[nCnt].vtxMinItem = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//それぞれの座標のモデルの位置の最小値
		g_aItem[nCnt].vtxMaxItem = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//それぞれの座標のモデルの位置の最大値
		g_aItem[nCnt].nNumVtx = 0;//頂点数
		g_aItem[nCnt].sizeFVF;//頂点フォーマットのサイズ
		g_aItem[nCnt].pVtxBuff;//頂点バッファへのポインタ
		g_aItem[nCnt].Size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//敵の大きさ
		g_aItem[nCnt].bIsLanding = false;//地面についているかどうか
		g_aItem[nCnt].nNumLandingModel = 0;//どのモデルの上に乗っているか
	}

	//モデルの読み込み
	D3DXLoadMeshFromX("data\\MODEL\\Item\\Candy_000.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_aItemMat[ITEM00_CANDY].pBuffMatItem,
		NULL,
		&g_aItemMat[ITEM00_CANDY].dwNumMatItem,
		&g_aItemMat[ITEM00_CANDY].pMeshItem);

	D3DXMATERIAL* pMat;//マテリアルへのポインタ

	for (int nCntItemSET = 0; nCntItemSET < NUM_ITEM; nCntItemSET++)
	{
		//マテリアルへのポインタを取得（セットしたモデルと同じタイプ）
		pMat = (D3DXMATERIAL*)g_aItemMat[nCntItemSET].pBuffMatItem->GetBufferPointer();
		int nNumTexture = 0;
		for (int nCntMat = 0; nCntMat < (int)g_aItemMat[nCntItemSET].dwNumMatItem; nCntMat++)
		{
			if (pMat[nCntMat].pTextureFilename != NULL)
			{//テクスチャファイルが存在する
					//テクスチャの読み込み
				D3DXCreateTextureFromFile(pDevice,
					pMat[nCntMat].pTextureFilename,
					&g_aItemMat[nCntItemSET].apTextureItem[nNumTexture]);
				nNumTexture++;
			}
		}
	}
}

//==============================
// 終了処理
//==============================
void UninitItem(void)
{
	for (int nCnt = 0; nCnt < NUM_ITEM; nCnt++)
	{
		//メッシュの破棄
		if (g_aItemMat[nCnt].pMeshItem != NULL)
		{
			g_aItemMat[nCnt].pMeshItem->Release();
			g_aItemMat[nCnt].pMeshItem = NULL;
		}
		//マテリアルの破棄
		if (g_aItemMat[nCnt].pBuffMatItem != NULL)
		{
			g_aItemMat[nCnt].pBuffMatItem->Release();
			g_aItemMat[nCnt].pBuffMatItem = NULL;
		}
	}
}
//============================
// 更新処理
//============================
void UpdateItem(void)
{
	Player* pPlayer = GetPlayer();

	g_nDelayItem++;

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_aItem[nCntItem].bUse == true)
		{
			Mission* pMission = GetMission();

			g_aItem[nCntItem].bIsLanding = bCollisionModel(&g_aItem[nCntItem].pos, &g_aItem[nCntItem].posOld, g_aItem[nCntItem].Size, g_aItem[nCntItem].nNumLandingModel);

			if (g_aItem[nCntItem].bIsLanding == false)
			{
				g_aItem[nCntItem].move.y = -2.0f;//重力処理
			}

			if (g_aItem[nCntItem].pos.y <= 0.0f)
			{
				g_aItem[nCntItem].pos.y = 0.0f;
				g_aItem[nCntItem].move.y = 0.0f;
			}

			switch (g_aItem[nCntItem].nType)
			{
			case ITEM00_CANDY:
				if (g_aItem[nCntItem].pos.x + g_aItem[nCntItem].Size.x / 2 >= pPlayer->pos.x - pPlayer->Size.x / 2 &&
					g_aItem[nCntItem].pos.x - g_aItem[nCntItem].Size.x / 2 <= pPlayer->pos.x + pPlayer->Size.x / 2 &&
					g_aItem[nCntItem].pos.y + g_aItem[nCntItem].Size.y >= pPlayer->pos.y &&
					g_aItem[nCntItem].pos.y <= pPlayer->pos.y + pPlayer->Size.y &&
					g_aItem[nCntItem].pos.z + g_aItem[nCntItem].Size.z / 2 >= pPlayer->pos.z - pPlayer->Size.z / 2 &&
					g_aItem[nCntItem].pos.z - g_aItem[nCntItem].Size.z / 2 <= pPlayer->pos.z + pPlayer->Size.z / 2)
				{
					PlaySound(SOUND_LABEL_SE_GETCANDY_000);
					pMission->aMission0.nGetCandyCount++;
					AddCandyNum(1);
					g_aItem[nCntItem].bUse = false;
				}

				break;
			}

			if (g_aItem[nCntItem].bUse == false)
			{
				/*	PlaySound(SOUND_LABEL_SE_DEFEAT_Item);*/
			}
		}
	}
}

//================================
// 描画処理
//================================
void DrawItem(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();;//デバイスへのポインタと取得
	D3DXMATRIX mtxRot, mtxTrans;//計算用マトリックス
	D3DMATERIAL9 matDef;//現在のマテリアル保存用
	D3DXMATERIAL* pMat;//マテリアルデータへのポインタ

	//現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_aItem[nCntItem].bUse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_mtxWorldItem);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aItem[nCntItem].rot.y, g_aItem[nCntItem].rot.x, g_aItem[nCntItem].rot.z);

			D3DXMatrixMultiply(&g_mtxWorldItem, &g_mtxWorldItem, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aItem[nCntItem].pos.x, g_aItem[nCntItem].pos.y, g_aItem[nCntItem].pos.z);
			D3DXMatrixMultiply(&g_mtxWorldItem, &g_mtxWorldItem, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldItem);
		
		    //マテリアルへのポインタを取得（セットしたモデルと同じタイプ）
		    pMat = (D3DXMATERIAL*)g_aItemMat[g_aItem[nCntItem].nType].pBuffMatItem->GetBufferPointer();
		    
		    int nNumTexture = 0;
		    //セットされたモデルとタイプが同じもののモデルのマテリアルの数分読み込む。
		    for (int nCntMat = 0; nCntMat < (int)g_aItemMat[g_aItem[nCntItem].nType].dwNumMatItem; nCntMat++)
		    {
		    	//マテリアルの設定
		    	pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
		    
		    	if (pMat[nCntMat].pTextureFilename != NULL)
		    	{
		    		//テクスチャの設定
		    		pDevice->SetTexture(0, g_aItemMat[g_aItem[nCntItem].nType].apTextureItem[nNumTexture]);
		    		nNumTexture++;
		    	}
		    	else
		    	{
		    		//テクスチャの設定
		    		pDevice->SetTexture(0, g_aItemMat[g_aItem[nCntItem].nType].apTextureItem[nNumTexture]);
		    		nNumTexture++;
		    	}
		    	//モデル（パーツ）の描画
		    	g_aItemMat[g_aItem[nCntItem].nType].pMeshItem->DrawSubset(nCntMat);
		    }
		    
			//保存していたマテリアルを戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}

//====================================
// キャンディ表示の初期化処理
//====================================
void InitCandyDisp(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ
	pDevice = GetDevice();
	VERTEX_2D* pVtx;
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\CandyDisplay_000.png",
		&g_pTextureCandyDisp);


	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffCandyDisp,
		NULL);

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffCandyDisp->Lock(0, 0, (void**)&pVtx, 0);

	g_aCandyDisp.bUse = false;//使用状態
	g_aCandyDisp.nGetCandy = 0;//キャンディ獲得数
	g_aCandyDisp.pos = D3DXVECTOR3(1130.0f, 490.0f, 0.0f);//位置

	pVtx[0].pos = D3DXVECTOR3(g_aCandyDisp.pos.x - CANDYDISP_WIDTH, g_aCandyDisp.pos.y - CANDYDISP_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aCandyDisp.pos.x + CANDYDISP_WIDTH, g_aCandyDisp.pos.y - CANDYDISP_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aCandyDisp.pos.x - CANDYDISP_WIDTH, g_aCandyDisp.pos.y + CANDYDISP_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aCandyDisp.pos.x + CANDYDISP_WIDTH, g_aCandyDisp.pos.y + CANDYDISP_HEIGHT, 0.0f);

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
	g_pVtxBuffCandyDisp->Unlock();
}

//===================================
// キャンディ表示の終了処理
//===================================
void UninitCandyDisp(void)
{
	//テクスチャの破棄
	if (g_pTextureCandyDisp != NULL)
	{
		g_pTextureCandyDisp->Release();
		g_pTextureCandyDisp = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffCandyDisp != NULL)
	{
		g_pVtxBuffCandyDisp->Release();
		g_pVtxBuffCandyDisp = NULL;
	}
}

//==================================
// キャンディ表示の更新処理
//==================================
void UpdateCandyDisp(void)
{
}

//==================================
// キャンディ表示の描画処理
//==================================
void DrawCandyDisp(void)
{
	LPDIRECT3DDEVICE9 pDevice;

    //デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffCandyDisp, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	pDevice->SetTexture(0,g_pTextureCandyDisp);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,0, 2);
	
}

//=======================================
// キャンディ獲得数表示の初期化処理
//=======================================
void InitCandyNum(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ
	pDevice = GetDevice();
	VERTEX_2D* pVtx;
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Lv_Num.png",
		&g_pTextureCandyNum);


	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * CANDY_DIGIT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffCandyNum,
		NULL);

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffCandyNum->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntDigit = 0; nCntDigit < CANDY_DIGIT; nCntDigit++)
	{
		g_aCandyNum[nCntDigit].bUse = false;//使用状態
		g_aCandyNum[nCntDigit].fPosTexU = 0.0f;//テクスチャ座標
		g_aCandyNum[nCntDigit].pos = D3DXVECTOR3(g_aCandyDisp.pos.x + 105.0f - (CANDYNUM_WIDTH * 2.0f) * nCntDigit,g_aCandyDisp.pos.y,0.0f);

		pVtx[0].pos = D3DXVECTOR3(g_aCandyNum[nCntDigit].pos.x - CANDYNUM_WIDTH, g_aCandyNum[nCntDigit].pos.y - CANDYNUM_HEIGHT, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aCandyNum[nCntDigit].pos.x + CANDYNUM_WIDTH, g_aCandyNum[nCntDigit].pos.y - CANDYNUM_HEIGHT, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aCandyNum[nCntDigit].pos.x - CANDYNUM_WIDTH, g_aCandyNum[nCntDigit].pos.y + CANDYNUM_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aCandyNum[nCntDigit].pos.x + CANDYNUM_WIDTH, g_aCandyNum[nCntDigit].pos.y + CANDYNUM_HEIGHT, 0.0f);

		//rhwの設定（初期設定）
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定（初期設定）
		pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

		//テクスチャ座標の設定（初期設定）
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);
	}
	//頂点バッファをアンロックする
	g_pVtxBuffCandyNum->Unlock();

}

//=======================================
// キャンディ数値の終了処理
//=======================================
void UninitCandyNum(void)
{
	//テクスチャの破棄
	if (g_pTextureCandyNum != NULL)
	{
		g_pTextureCandyNum->Release();
		g_pTextureCandyNum = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffCandyNum != NULL)
	{
		g_pVtxBuffCandyNum->Release();
		g_pVtxBuffCandyNum = NULL;
	}
}

//=======================================
// キャンディ数値の更新処理
//=======================================
void UpdateCandyNum(void)
{

}

//=======================================
// キャンディ数値の描画処理
//=======================================
void DrawCandyNum(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffCandyNum, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	pDevice->SetTexture(0, g_pTextureCandyNum);

	for (int nCntDigit = 0; nCntDigit < CANDY_DIGIT; nCntDigit++)
	{
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,nCntDigit * 4, 2);
	}
}

//===================================
// キャンディ数値の加算処理
//===================================
void AddCandyNum(int nCandy)
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();

	VERTEX_2D* pVtx;

	g_aCandyDisp.nGetCandy += nCandy;
	int nGetCandy = g_aCandyDisp.nGetCandy;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffCandyNum->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < CANDY_DIGIT; nCnt++)
	{
		g_aCandyNum[nCnt].fPosTexU = (float)(nGetCandy % 10);
		nGetCandy = nGetCandy / 10;

		pVtx[0].tex = D3DXVECTOR2(0.0f + (0.1f * g_aCandyNum[nCnt].fPosTexU), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + (0.1f * g_aCandyNum[nCnt].fPosTexU), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + (0.1f * g_aCandyNum[nCnt].fPosTexU), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + (0.1f * g_aCandyNum[nCnt].fPosTexU), 1.0f);

		pVtx += 4;//頂点データのポインタを４つ分進める。
	}

	//頂点バッファをアンロックする
	g_pVtxBuffCandyNum->Unlock();
}

//===========================================================
// アイテムの設定処理
//===========================================================
void SetItem(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType)
{
	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_aItem[nCntItem].bUse == false)
		{
			g_aItem[nCntItem].bUse = true;//使用しているかどうか
			g_aItem[nCntItem].pos = pos;//位置
			g_aItem[nCntItem].posOld = pos;//1F前の位置
			g_aItem[nCntItem].rot = rot;//向き
			g_aItem[nCntItem].nType = nType;//種類
			g_aItem[nCntItem].vtxMaxItem = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aItem[nCntItem].vtxMinItem = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aItem[nCntItem].Size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aItem[nCntItem].nNumLandingModel = 0;//どのモデルの上に乗っているか

			//=====================================
			//モデルの位置の最大最小値を取得する
			//=====================================
		
			//頂点数の取得
			g_aItem[nCntItem].nNumVtx = g_aItemMat[g_aItem[nCntItem].nType].pMeshItem->GetNumVertices();

			//頂点フォーマットのサイズを取得
			g_aItem[nCntItem].sizeFVF = D3DXGetFVFVertexSize(g_aItemMat[g_aItem[nCntItem].nType].pMeshItem->GetFVF());

			//頂点バッファのロック
			g_aItemMat[g_aItem[nCntItem].nType].pMeshItem->LockVertexBuffer(D3DLOCK_READONLY, (void**)&g_aItem[nCntItem].pVtxBuff);

			for (int nCntVtx = 0; nCntVtx < g_aItem[nCntItem].nNumVtx; nCntVtx++)
			{
				//頂点座標の代入
				D3DXVECTOR3 vtx = *(D3DXVECTOR3*)g_aItem[nCntItem].pVtxBuff;

				//====================================================
				//頂点座標を比較してモデルの最小値最大値を取得
				//====================================================
				if (vtx.x > g_aItem[nCntItem].vtxMaxItem.x)
				{
					g_aItem[nCntItem].vtxMaxItem.x = vtx.x;
				}
				else if (vtx.x < g_aItem[nCntItem].vtxMinItem.x)
				{//今回読み込んだ頂点が、一番小さい頂点より小さい場合
					g_aItem[nCntItem].vtxMinItem.x = vtx.x;
				}
				else if (vtx.y > g_aItem[nCntItem].vtxMaxItem.y)
				{//今回読み込んだ頂点が、一番小さい頂点より小さい場合
					g_aItem[nCntItem].vtxMaxItem.y = vtx.y;
				}
				else if (vtx.y < g_aItem[nCntItem].vtxMinItem.y)
				{//今回読み込んだ頂点が、一番小さい頂点より小さい場合
					g_aItem[nCntItem].vtxMinItem.y = vtx.y;
				}
				else if (vtx.z > g_aItem[nCntItem].vtxMaxItem.z)
				{//今回読み込んだ頂点が、一番小さい頂点より小さい場合
					g_aItem[nCntItem].vtxMaxItem.z = vtx.z;
				}
				else if (vtx.z < g_aItem[nCntItem].vtxMinItem.z)
				{//今回読み込んだ頂点が、一番小さい頂点より小さい場合
					g_aItem[nCntItem].vtxMinItem.z = vtx.z;
				}
				
				//頂点フォーマットのサイズ文ポインタを進める
				g_aItem[nCntItem].pVtxBuff += g_aItem[nCntItem].sizeFVF;
			}

			//頂点バッファのアンロック
			g_aItemMat[g_aItem[nCntItem].nType].pMeshItem->UnlockVertexBuffer();

			g_aItem[nCntItem].Size = g_aItem[nCntItem].vtxMaxItem - g_aItem[nCntItem].vtxMinItem;

			break;
		}
	}
}

int GetNumItem(void)
{
	return g_nNumItem;//読み込むモデルの最大数を取得する
}

Item* GetItem(void)//敵構造体の情報を取得する
{
	return &g_aItem[0];
}

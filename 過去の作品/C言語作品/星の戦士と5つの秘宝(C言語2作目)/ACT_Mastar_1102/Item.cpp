//========================================================
//
//８月３０日：エフェクトもう一回見直す[Item.cpp]
//Author:ShinaTaiyo
//
//=========================================================
#include "main.h"
#include <stdlib.h>
#include <time.h>
#include "score.h"
#include "game.h"
#include "result.h"
#include "player.h"
#include "Item.h"
#include "sound.h"
#include "health.h"
#include "Treasure.h"
#include "fade.h"
#include "timer.h"
#include "enemy.h"
//マクロ定義
#include "input.h"
#define NUM_ITEMTEX (6)//アイテムの種類
#define MAX_Item (5000)
#define ITEM_SIZE (50.0f)

#define SCOREBONUS_SIZE (300.0f)
#define APPER_ItemSIZE (20.0f)
#define PRESSSTART_ItemSIZE (20.0f)

#define SHIELD_SIZE (100.0f)


//グローバル変数
LPDIRECT3DTEXTURE9 g_apTextureItem[NUM_ITEMTEX] = {};
//D3DXVECTOR3 g_rotPI;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffItem = NULL;
//Enemy g_Item[MAX_ENEMY];//敵の情報
//Result g_aResult;

Item g_Item[MAX_Item];
ItemEffect g_aItemEffect;

//int g_nCountTime = 0;//時間を表すグローバル変数

//=====================================================
//==========================
//敵の初期化処理
//==========================
void InitItem(void)
{



	//srand((unsigned int)time(NULL));//ゲームを開始した時間毎に乱数の種を設定



	LPDIRECT3DDEVICE9 pDevice;
	int	nCntItem;



	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Heal_Item_000.png",
		&g_apTextureItem[ITEMTYPE_HEAL]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\ReafOrb.png",
		&g_apTextureItem[ITEMTYPE_REAFORB]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\IceOrb.png",
		&g_apTextureItem[ITEMTYPE_ICEORB]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\FireOrb.png",
		&g_apTextureItem[ITEMTYPE_FIREORB]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\ThunderOrb.png",
		&g_apTextureItem[ITEMTYPE_THUNDERORB]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\LastOrb.png",
		&g_apTextureItem[ITEMTYPE_LASTORB]);

	
	//敵情報の初期化
	for (nCntItem = 0; nCntItem < MAX_Item; nCntItem++)
	{
		
		g_Item[nCntItem].bUse = false;
		g_Item[nCntItem].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Item[nCntItem].HealPoint = 0;
	}

	//g_Item->pos = D3DXVECTOR3(0.0f,0.0f, 0.0f);//位置を初期化する

	//チュートリアル情報の初期化
	//for (nCntItem = 0; nCntItem < NUM_ITEMTEX; nCntItem++)
	//{
	//	
	//}


	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4*MAX_Item,// * NUM_ITEMTEX,//４は必要な頂点数。今回使う画像数は2枚だが、一つのポリゴンでやるので、頂点４つで大丈夫。
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffItem,
		NULL);

	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntItem = 0; nCntItem < MAX_Item; nCntItem++)
	{
		//頂点座標の設定(敵の大きさ）
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f,0.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(0.0f,0.0f, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	pVtx += 4;//頂点データのポインタを４つ分進める。
}
//頂点バッファをアンロックする
	g_pVtxBuffItem->Unlock();
}

//====================================
//チュートリアルの終了処理
//====================================
void UninitItem(void)
{
	//テクスチャの破棄
	int nCntItem;
	for (nCntItem = 0; nCntItem < NUM_ITEMTEX; nCntItem++)
	{
		if (g_apTextureItem[nCntItem] != NULL)
		{
			g_apTextureItem[nCntItem]->Release();
			g_apTextureItem[nCntItem] = NULL;
		}
	}
	//頂点バッファの破棄
	if (g_pVtxBuffItem != NULL)
	{
		g_pVtxBuffItem->Release();
		g_pVtxBuffItem = NULL;
	}
}


//========================================
//敵の更新処理
//========================================
void UpdateItem(void)
{
	VERTEX_2D* pVtx;

	int nCntItem;

	Player* pPlayer = GetPlayer();


	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntItem = 0; nCntItem < MAX_Item; nCntItem++)
	{
		if (g_Item[nCntItem].bUse == true)
		{
			//頂点座標の設定(敵の大きさ）
			pVtx[0].pos = D3DXVECTOR3(g_Item[nCntItem].pos.x - ITEM_SIZE, g_Item[nCntItem].pos.y - ITEM_SIZE, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_Item[nCntItem].pos.x + ITEM_SIZE, g_Item[nCntItem].pos.y - ITEM_SIZE, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_Item[nCntItem].pos.x - ITEM_SIZE, g_Item[nCntItem].pos.y + ITEM_SIZE, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_Item[nCntItem].pos.x + ITEM_SIZE, g_Item[nCntItem].pos.y + ITEM_SIZE, 0.0f);
			if (g_Item[nCntItem].type == ITEMTYPE_HEAL)
			{



				if (pPlayer->pos.x >= g_Item[nCntItem].pos.x - ITEM_SIZE - pPlayer->fWidth &&
					pPlayer->pos.x <= g_Item[nCntItem].pos.x + ITEM_SIZE + pPlayer->fWidth &&
					pPlayer->pos.y - pPlayer->fHeight / 2 >= g_Item[nCntItem].pos.y - ITEM_SIZE - pPlayer->fHeight / 2 &&
					pPlayer->pos.y - pPlayer->fHeight / 2 <= g_Item[nCntItem].pos.y + ITEM_SIZE + pPlayer->fHeight / 2)
				{//プレイヤーの中心点がどの位置にあるか
					g_Item[nCntItem].bUse = false;
					SetHealing(g_Item[nCntItem].HealPoint);
					PlaySound(SOUND_LABEL_SE_HEAL_000);
				}

			}
			else if (g_Item[nCntItem].type == ITEMTYPE_REAFORB ||
				g_Item[nCntItem].type == ITEMTYPE_ICEORB ||
				g_Item[nCntItem].type == ITEMTYPE_FIREORB ||
				g_Item[nCntItem].type == ITEMTYPE_THUNDERORB ||
				g_Item[nCntItem].type == ITEMTYPE_LASTORB)
			{
				if (pPlayer->pos.x >= g_Item[nCntItem].pos.x - ITEM_SIZE - pPlayer->fWidth &&
					pPlayer->pos.x <= g_Item[nCntItem].pos.x + ITEM_SIZE + pPlayer->fWidth &&
					pPlayer->pos.y - pPlayer->fHeight / 2 >= g_Item[nCntItem].pos.y - ITEM_SIZE - pPlayer->fHeight / 2 &&
					pPlayer->pos.y - pPlayer->fHeight / 2 <= g_Item[nCntItem].pos.y + ITEM_SIZE + pPlayer->fHeight / 2)
				{//プレイヤーの中心点がどの位置にあるか

					PlaySound(SOUND_LABEL_SE_ORBGET);
					g_Item[nCntItem].bUse = false;

					if (g_Item[nCntItem].type == ITEMTYPE_REAFORB)
					{
						SetTreasure(TREASURETYPE_REAF, -1, D3DXVECTOR3(1145.0f, 50.0f, 0.0f));
					}
					if (g_Item[nCntItem].type == ITEMTYPE_FIREORB)
					{
						
						SetTreasure(TREASURETYPE_FIRE, -1, D3DXVECTOR3(1145.0f, 15.0f, 0.0f));
					}
					if (g_Item[nCntItem].type == ITEMTYPE_ICEORB)
					{
						SetTreasure(TREASURETYPE_ICE, -1, D3DXVECTOR3(1257.0f, 50.0f, 0.0f));
					}
					if (g_Item[nCntItem].type == ITEMTYPE_THUNDERORB)
					{
						SetTreasure(TREASURETYPE_THUNDER, -1, D3DXVECTOR3(1257.0f, 15.0f, 0.0f));
					}
					if (g_Item[nCntItem].type == ITEMTYPE_LASTORB)
					{
						SetTreasure(TREASURETYPE_LAST, -1, D3DXVECTOR3(1200.0f, 35.0f, 0.0f));
						AddScore(1000000);
						int nData = GetScore();
						int nTimer = GetTimer();
						int nDefeatEnemy = DefeatEnemy();
						AddScore((nData * (nTimer / 100)) * (1 + (nDefeatEnemy / 100)) );
						SetGameState(GAMESTATE_END);
					}
				}
			}
		}
		pVtx += 4;//頂点データのポインタを４つ分進める。
	}
		//頂点バッファをアンロックする
		g_pVtxBuffItem->Unlock();

	
}


//=========================================
//敵の描画処理
//=========================================
void DrawItem(void)
{

	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	int nCntItem;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffItem, 0, sizeof(VERTEX_2D));//頂点バッファへのポインタと頂点情報の構造体のサイズ

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定

	for (nCntItem = 0; nCntItem < MAX_Item; nCntItem++)
	{
		if (g_Item[nCntItem].bUse == true)
		{
			
				if (g_Item[nCntItem].type == ITEMTYPE_HEAL)
				{
					pDevice->SetTexture(0, g_apTextureItem[ITEMTYPE_HEAL]);
				}
				if (g_Item[nCntItem].type == ITEMTYPE_REAFORB)
				{
					pDevice->SetTexture(0, g_apTextureItem[ITEMTYPE_REAFORB]);
				}
				if (g_Item[nCntItem].type == ITEMTYPE_ICEORB)
				{
					pDevice->SetTexture(0, g_apTextureItem[ITEMTYPE_ICEORB]);
				}
				if (g_Item[nCntItem].type == ITEMTYPE_FIREORB)
				{
					pDevice->SetTexture(0, g_apTextureItem[ITEMTYPE_FIREORB]);
				}
				if (g_Item[nCntItem].type == ITEMTYPE_THUNDERORB)
				{
					pDevice->SetTexture(0, g_apTextureItem[ITEMTYPE_THUNDERORB]);
				}
				if (g_Item[nCntItem].type == ITEMTYPE_LASTORB)
				{
					pDevice->SetTexture(0, g_apTextureItem[ITEMTYPE_LASTORB]);
				}
				
				//ポリゴンの描画
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntItem * 4, 2);//0は、描画する最初の頂点インデックスで、2は、描画するプリミティブ数

				//&g_pVtxBuffPolygon[0],//頂点情報の先頭アドレス
				//sizeof(VERTEX_2D));//頂点情報構造体のサイズ
			
		}
	}


}

void SetItem(ITEMTYPE type, D3DXVECTOR3 pos, int HealPoint)
{
	int nCntItem;

	VERTEX_2D* pVtx;

	/*Enemy* pEnemy = GetEnemy();*/

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntItem = 0; nCntItem < MAX_Item; nCntItem++)
	{
		if (g_Item[nCntItem].bUse == false)
		{
			g_Item[nCntItem].pos = pos;//SetEnemyの時に、敵の位置が代入された際、位置の引数をさらにSetItemのposに代入することにより、
			g_Item[nCntItem].type = type;//演出の種類をセットする。
			g_Item[nCntItem].bUse = true;
			g_Item[nCntItem].HealPoint = HealPoint;
			break;
		}
		pVtx += 4;
	}
	g_pVtxBuffItem->Unlock();
}

ItemEffect* GetItemEffect(void)
{
	return &g_aItemEffect;
}

Item* GetItem(void)
{
	return &g_Item[0];
}


//========================================================
//
//１０月１９日：宝獲得表示[Treasure.cpp]
//Author:ShinaTaiyo
//
//=========================================================
#include "main.h"
#include "bullet.h"
#include "explosion.h"
#include <stdlib.h>
#include <time.h>
#include "score.h"
#include "game.h"
#include "result.h"
#include "player.h"
#include "Treasure.h"
#include "sound.h"
#include "enemy.h"
#include "fade.h"
//マクロ定義
#include "input.h"
#define NUM_TREASURETEX (6)//宝の種類
#define MAX_TREASURE (5000)

#define APPER_TREASURESIZE (50.0f)
#define PRESSSTART_TREASURESIZE (120.0f)
#define TREASURESIZE (20.0f)
#define ORBSIZE (20.0f)

#define TREASUREBOXSIZEX (80.0f)
#define TREASUREBOXSIZEY (30.0f)
//グローバル変数
LPDIRECT3DTEXTURE9 g_apTextureTreasure[NUM_TREASURETEX] = {};
//D3DXVECTOR3 g_rotPI;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTreasure = NULL;
//Enemy g_Treasure[MAX_ENEMY];//敵の情報
//Result g_aResult;

Treasure g_Treasure[MAX_TREASURE];

//int g_nCountTime = 0;//時間を表すグローバル変数

//=====================================================
//==========================
//敵の初期化処理
//==========================
void InitTreasure(void)
{



	//srand((unsigned int)time(NULL));//ゲームを開始した時間毎に乱数の種を設定



	LPDIRECT3DDEVICE9 pDevice;
	int	nCntTreasure;



	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\GetTreasure.png",
		&g_apTextureTreasure[TREASURETYPE_BOX]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\ReafOrb.png",
		&g_apTextureTreasure[TREASURETYPE_REAF]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\IceOrb.png",
		&g_apTextureTreasure[TREASURETYPE_ICE]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\FireOrb.png",
		&g_apTextureTreasure[TREASURETYPE_FIRE]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\ThunderOrb.png",
		&g_apTextureTreasure[TREASURETYPE_THUNDER]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\LastOrb.png",
		&g_apTextureTreasure[TREASURETYPE_LAST]);


	//敵情報の初期化
	for (nCntTreasure = 0; nCntTreasure < MAX_TREASURE; nCntTreasure++)
	{
		g_Treasure[nCntTreasure].nTreasureType = 0;
		g_Treasure[nCntTreasure].nLife = 0;
		g_Treasure[nCntTreasure].bUse = false;
		g_Treasure[nCntTreasure].bCountStartFlag = false;//演出が始まってからのカウントを開始するフラグを宣言する。
		g_Treasure[nCntTreasure].nTreasureCount = 0;//演出が始まってからのカウントを初期化する。
		g_Treasure[nCntTreasure].fTreasurePatternCountY = 0.0;//アニメーションを遅らせるカウント
		g_Treasure[nCntTreasure].fTreasurePattern = 0.0f;//アニメーションの種類
		g_Treasure[nCntTreasure].nAnimationRepeat = 0;//アニメーションを繰り返す変数
		g_Treasure[nCntTreasure].bDraw = true;//bUseがtrueだったことに加え、更にbDrawがtrueかどうかで描画するかどうかを決める。
		g_Treasure[nCntTreasure].nCntEnemy = 0;
		g_Treasure[nCntTreasure].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Treasure[nCntTreasure].fColor_a = 0.0f;
		g_Treasure[nCntTreasure].bColorChange_a = false;
	}

	//g_Treasure->pos = D3DXVECTOR3(0.0f,0.0f, 0.0f);//位置を初期化する

	//チュートリアル情報の初期化
	//for (nCntTreasure = 0; nCntTreasure < NUM_TREASURETEX; nCntTreasure++)
	//{
	//	
	//}


	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_TREASURE,// * NUM_TREASURETEX,//４は必要な頂点数。今回使う画像数は2枚だが、一つのポリゴンでやるので、頂点４つで大丈夫。
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTreasure,
		NULL);

	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffTreasure->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntTreasure = 0; nCntTreasure < MAX_TREASURE; nCntTreasure++)
	{
		//頂点座標の設定(敵の大きさ）
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

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
	g_pVtxBuffTreasure->Unlock();
}

//====================================
//チュートリアルの終了処理
//====================================
void UninitTreasure(void)
{
	//テクスチャの破棄
	int nCntTreasure;
	for (nCntTreasure = 0; nCntTreasure < NUM_TREASURETEX; nCntTreasure++)
	{
		if (g_apTextureTreasure[nCntTreasure] != NULL)
		{
			g_apTextureTreasure[nCntTreasure]->Release();
			g_apTextureTreasure[nCntTreasure] = NULL;
		}
	}
	//頂点バッファの破棄
	if (g_pVtxBuffTreasure != NULL)
	{
		g_pVtxBuffTreasure->Release();
		g_pVtxBuffTreasure = NULL;
	}
}


//========================================
//敵の更新処理
//========================================
void UpdateTreasure(void)
{
	VERTEX_2D* pVtx;



	int nCntTreasure;


	Enemy* pEnemy = GetEnemy();


	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffTreasure->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntTreasure = 0; nCntTreasure < MAX_TREASURE; nCntTreasure++)
	{
		if (g_Treasure[nCntTreasure].bUse == true)
		{

			if (g_Treasure[nCntTreasure].nLife < 1)
			{
				g_Treasure[nCntTreasure].bUse = false;
			}

			g_Treasure[nCntTreasure].nTreasureCount++;//出現してからの時間をカウントする

		

			if (g_Treasure[nCntTreasure].nTreasureType == TREASURETYPE_BOX)
			{
				pVtx[0].pos = D3DXVECTOR3(g_Treasure[nCntTreasure].pos.x - TREASUREBOXSIZEX, g_Treasure[nCntTreasure].pos.y - TREASUREBOXSIZEY, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_Treasure[nCntTreasure].pos.x + TREASUREBOXSIZEX, g_Treasure[nCntTreasure].pos.y - TREASUREBOXSIZEY, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_Treasure[nCntTreasure].pos.x - TREASUREBOXSIZEX, g_Treasure[nCntTreasure].pos.y + TREASUREBOXSIZEY, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_Treasure[nCntTreasure].pos.x + TREASUREBOXSIZEX, g_Treasure[nCntTreasure].pos.y + TREASUREBOXSIZEY, 0.0f);
			}
			if (g_Treasure[nCntTreasure].nTreasureType == TREASURETYPE_REAF ||
				g_Treasure[nCntTreasure].nTreasureType == TREASURETYPE_ICE ||
				g_Treasure[nCntTreasure].nTreasureType == TREASURETYPE_FIRE ||
				g_Treasure[nCntTreasure].nTreasureType == TREASURETYPE_THUNDER ||
				g_Treasure[nCntTreasure].nTreasureType == TREASURETYPE_LAST)
			{
				pVtx[0].pos = D3DXVECTOR3(g_Treasure[nCntTreasure].pos.x - ORBSIZE, g_Treasure[nCntTreasure].pos.y - ORBSIZE, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_Treasure[nCntTreasure].pos.x + ORBSIZE, g_Treasure[nCntTreasure].pos.y - ORBSIZE, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_Treasure[nCntTreasure].pos.x - ORBSIZE, g_Treasure[nCntTreasure].pos.y + ORBSIZE, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_Treasure[nCntTreasure].pos.x + ORBSIZE, g_Treasure[nCntTreasure].pos.y + ORBSIZE, 0.0f);
			}
		
			
		}
		pVtx += 4;//頂点データのポインタを４つ分進める。
	}
	//頂点バッファをアンロックする
	g_pVtxBuffTreasure->Unlock();


}


//=========================================
//敵の描画処理
//=========================================
void DrawTreasure(void)
{

	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	Enemy* pEnemy = GetEnemy();

	int nCntTreasure;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTreasure, 0, sizeof(VERTEX_2D));//頂点バッファへのポインタと頂点情報の構造体のサイズ

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定

	for (nCntTreasure = 0; nCntTreasure < MAX_TREASURE; nCntTreasure++)
	{
		if (g_Treasure[nCntTreasure].bUse == true)
		{
			if (g_Treasure[nCntTreasure].bDraw == true)
			{
				if (g_Treasure[nCntTreasure].nTreasureType == TREASURETYPE_BOX)
				{
					pDevice->SetTexture(0, g_apTextureTreasure[TREASURETYPE_BOX]);
				}
				if (g_Treasure[nCntTreasure].nTreasureType == TREASURETYPE_REAF)
				{
					pDevice->SetTexture(0, g_apTextureTreasure[TREASURETYPE_REAF]);
				}
				if (g_Treasure[nCntTreasure].nTreasureType == TREASURETYPE_ICE)
				{
					pDevice->SetTexture(0, g_apTextureTreasure[TREASURETYPE_ICE]);
				}
				if (g_Treasure[nCntTreasure].nTreasureType == TREASURETYPE_FIRE)
				{
					pDevice->SetTexture(0, g_apTextureTreasure[TREASURETYPE_FIRE]);
				}
				if (g_Treasure[nCntTreasure].nTreasureType == TREASURETYPE_THUNDER)
				{
					pDevice->SetTexture(0, g_apTextureTreasure[TREASURETYPE_THUNDER]);
				}
				if (g_Treasure[nCntTreasure].nTreasureType == TREASURETYPE_LAST)
				{
					pDevice->SetTexture(0, g_apTextureTreasure[TREASURETYPE_LAST]);
				}
				
				//ポリゴンの描画
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTreasure * 4, 2);//0は、描画する最初の頂点インデックスで、2は、描画するプリミティブ数

				//&g_pVtxBuffPolygon[0],//頂点情報の先頭アドレス
				//sizeof(VERTEX_2D));//頂点情報構造体のサイズ
			}
		}
	}


}

void SetTreasure(TREASURETYPE type, int nCntEnemy, D3DXVECTOR3 pos)
{
	int nCntTreasure;

	VERTEX_2D* pVtx;

	/*Enemy* pEnemy = GetEnemy();*/

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffTreasure->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntTreasure = 0; nCntTreasure < MAX_TREASURE; nCntTreasure++)
	{
		if (g_Treasure[nCntTreasure].bUse == false)
		{
			g_Treasure[nCntTreasure].pos = pos;//SetEnemyの時に、敵の位置が代入された際、位置の引数をさらにSetTreasureのposに代入することにより、
			g_Treasure[nCntTreasure].nLife = 100;
			g_Treasure[nCntTreasure].nTreasureType = type;//演出の種類をセットする。
			g_Treasure[nCntTreasure].bUse = true;
			g_Treasure[nCntTreasure].nTreasureCount = 0;
			g_Treasure[nCntTreasure].bCountStartFlag = true;
			g_Treasure[nCntTreasure].fTreasurePatternCountY = 0.0f;
			g_Treasure[nCntTreasure].fTreasurePattern = 0.0f;
			g_Treasure[nCntTreasure].nAnimationRepeat = 0;
			g_Treasure[nCntTreasure].bDraw = true;
			g_Treasure[nCntTreasure].nCntEnemy = nCntEnemy;
			g_Treasure[nCntTreasure].fColor_a = 1.0f;
			g_Treasure[nCntTreasure].bColorChange_a = false;
			break;
		}
		pVtx += 4;
	}
	g_pVtxBuffTreasure->Unlock();
}

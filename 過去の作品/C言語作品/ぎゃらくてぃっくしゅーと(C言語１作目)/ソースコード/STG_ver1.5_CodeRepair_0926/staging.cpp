//========================================================
//
//８月３０日：エフェクトもう一回見直す[staging.cpp]
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
#include "staging.h"
#include "sound.h"
#include "enemy.h"
#include "fade.h"
//マクロ定義
#include "input.h"
#define NUM_STAGINGTEX (10)//演出の種類
#define MAX_STAGING (5000)

#define SCOREBONUS_SIZE (300.0f)
#define APPER_STAGINGSIZE (50.0f)
#define PRESSSTART_STAGINGSIZE (120.0f)

#define SHIELD_SIZE (100.0f)


//グローバル変数
LPDIRECT3DTEXTURE9 g_apTextureStaging[NUM_STAGINGTEX] = {};
//D3DXVECTOR3 g_rotPI;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffStaging = NULL;
//Enemy g_Staging[MAX_ENEMY];//敵の情報
//Result g_aResult;

Staging g_Staging[MAX_STAGING];

//int g_nCountTime = 0;//時間を表すグローバル変数

//=====================================================
//==========================
//敵の初期化処理
//==========================
void InitStaging(void)
{



	//srand((unsigned int)time(NULL));//ゲームを開始した時間毎に乱数の種を設定



	LPDIRECT3DDEVICE9 pDevice;
	int	nCntStaging;



	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\re_warning!_000.png",
		&g_apTextureStaging[STAGINGTYPE_WARNING]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\appearance_notice.png",
		&g_apTextureStaging[STAGINGTYPE_APPER]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\bullet_warning_000.png",
		&g_apTextureStaging[STAGINGTYPE_BULLETWARNING]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\PressStart_000.png",
		&g_apTextureStaging[STAGINGTYPE_PRESSSTART]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\PressEnter_or_start!_000.png",
		&g_apTextureStaging[STAGINGTYPE_PRESSENTER]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\Shield_000.png",
		&g_apTextureStaging[STAGINGTYPE_SHIELD]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\Normal_ClearBonus.png",
		&g_apTextureStaging[STAGINGTYPE_NORMAL_CLEARBONUS]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\Hard_ClearBonus.png",
		&g_apTextureStaging[STAGINGTYPE_HARD_CLEARBONUS]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\VeryHard_ClearBonus.png",
		&g_apTextureStaging[STAGINGTYPE_VERYHARD_CLEARBONUS]);


	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\ranking_bg_002.jpg",
		&g_apTextureStaging[STAGINGTYPE_RANKING_BG]);

	//敵情報の初期化
	for (nCntStaging = 0; nCntStaging < MAX_STAGING; nCntStaging++)
	{
		g_Staging[nCntStaging].nStagingType = 0;
		g_Staging[nCntStaging].nLife = 0;
		g_Staging[nCntStaging].bUse = false;
		g_Staging[nCntStaging].bCountStartFlag = false;//演出が始まってからのカウントを開始するフラグを宣言する。
		g_Staging[nCntStaging].nStagingCount = 0;//演出が始まってからのカウントを初期化する。
		g_Staging[nCntStaging].fStagingPatternCountY = 0.0;//アニメーションを遅らせるカウント
		g_Staging[nCntStaging].fStagingPattern = 0.0f;//アニメーションの種類
		g_Staging[nCntStaging].nAnimationRepeat = 0;//アニメーションを繰り返す変数
		g_Staging[nCntStaging].bDraw = true;//bUseがtrueだったことに加え、更にbDrawがtrueかどうかで描画するかどうかを決める。
		g_Staging[nCntStaging].nCntEnemy = 0;
		g_Staging[nCntStaging].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Staging[nCntStaging].fColor_a = 0.0f;
		g_Staging[nCntStaging].bColorChange_a=false;
	}

	//g_Staging->pos = D3DXVECTOR3(0.0f,0.0f, 0.0f);//位置を初期化する

	//チュートリアル情報の初期化
	//for (nCntStaging = 0; nCntStaging < NUM_STAGINGTEX; nCntStaging++)
	//{
	//	
	//}


	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4*MAX_STAGING,// * NUM_STAGINGTEX,//４は必要な頂点数。今回使う画像数は2枚だが、一つのポリゴンでやるので、頂点４つで大丈夫。
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffStaging,
		NULL);

	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffStaging->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntStaging = 0; nCntStaging < MAX_STAGING; nCntStaging++)
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
	g_pVtxBuffStaging->Unlock();
}

//====================================
//チュートリアルの終了処理
//====================================
void UninitStaging(void)
{
	//テクスチャの破棄
	int nCntStaging;
	for (nCntStaging = 0; nCntStaging < NUM_STAGINGTEX; nCntStaging++)
	{
		if (g_apTextureStaging[nCntStaging] != NULL)
		{
			g_apTextureStaging[nCntStaging]->Release();
			g_apTextureStaging[nCntStaging] = NULL;
		}
	}
	//頂点バッファの破棄
	if (g_pVtxBuffStaging != NULL)
	{
		g_pVtxBuffStaging->Release();
		g_pVtxBuffStaging = NULL;
	}
}


//========================================
//敵の更新処理
//========================================
void UpdateStaging(void)
{
	VERTEX_2D* pVtx;

	

	int nCntStaging;


	Enemy* pEnemy = GetEnemy();
	

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffStaging->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntStaging = 0; nCntStaging < MAX_STAGING; nCntStaging++)
	{
		if (g_Staging[nCntStaging].bUse == true)
		{
			
			if (g_Staging[nCntStaging].nLife <= 0)
			{
				g_Staging[nCntStaging].bUse = false;
			}
			
				g_Staging[nCntStaging].nStagingCount++;

			if (g_Staging[nCntStaging].nStagingType == STAGINGTYPE_WARNING)
			{
				

				//頂点座標の設定(敵の大きさ）
				pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

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

				if (g_Staging[nCntStaging].nStagingCount % 5 == 0)
				{//アニメーションを右に進める。
					g_Staging[nCntStaging].fStagingPattern += 1.0f;
				}

				if (g_Staging[nCntStaging].fStagingPattern >= 4.0f && g_Staging[nCntStaging].fStagingPatternCountY >= 0.5f)
				{
					g_Staging[nCntStaging].nAnimationRepeat++;
					g_Staging[nCntStaging].fStagingPattern = 0.0f;
					g_Staging[nCntStaging].fStagingPatternCountY = 0.0f;

				}
				else if (g_Staging[nCntStaging].fStagingPattern>=4.0f)
				{//アニメーションを次の段に移す。
					g_Staging[nCntStaging].fStagingPattern = 0.0f;
					g_Staging[nCntStaging].fStagingPatternCountY += 0.5f;
				}

				if (g_Staging[nCntStaging].nAnimationRepeat >= 3)
				{
					g_Staging[nCntStaging].bUse = false;
				}
				pVtx[0].tex = D3DXVECTOR2(0.0f+ (1.0f / 4.0f) * g_Staging[nCntStaging].fStagingPattern,g_Staging[nCntStaging].fStagingPatternCountY);
				pVtx[1].tex = D3DXVECTOR2((1.0f/4.0f)+(1.0f/4.0f)*g_Staging[nCntStaging].fStagingPattern,g_Staging[nCntStaging].fStagingPatternCountY);
				pVtx[2].tex = D3DXVECTOR2(0.0f + (1.0f / 4.0f) * g_Staging[nCntStaging].fStagingPattern,0.5f+g_Staging[nCntStaging].fStagingPatternCountY);
				pVtx[3].tex = D3DXVECTOR2((1.0f / 4.0f) + (1.0f / 4.0f) * g_Staging[nCntStaging].fStagingPattern, 0.5f+g_Staging[nCntStaging].fStagingPatternCountY);
			}
			if (g_Staging[nCntStaging].nStagingType == STAGINGTYPE_APPER)
			{
				
				
						
							pVtx[0].pos = D3DXVECTOR3(g_Staging[nCntStaging].pos.x - APPER_STAGINGSIZE,g_Staging[nCntStaging].pos.y - APPER_STAGINGSIZE, 0.0f);
							pVtx[1].pos = D3DXVECTOR3(g_Staging[nCntStaging].pos.x + APPER_STAGINGSIZE,g_Staging[nCntStaging].pos.y - APPER_STAGINGSIZE, 0.0f);
							pVtx[2].pos = D3DXVECTOR3(g_Staging[nCntStaging].pos.x - APPER_STAGINGSIZE,g_Staging[nCntStaging].pos.y + APPER_STAGINGSIZE, 0.0f);
							pVtx[3].pos = D3DXVECTOR3(g_Staging[nCntStaging].pos.x + APPER_STAGINGSIZE,g_Staging[nCntStaging].pos.y + APPER_STAGINGSIZE, 0.0f);

							pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
							pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
							pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
							pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

							if (g_Staging[nCntStaging].nStagingCount % 5 == 0)
							{
								g_Staging[nCntStaging].bDraw = false;
							}
							else
							{
								g_Staging[nCntStaging].bDraw = true;
							}
							if (g_Staging[nCntStaging].nStagingCount >= 100)
							{
								g_Staging[nCntStaging].bUse = false;
							}
						
					
				
			}
			if (g_Staging[nCntStaging].nStagingType == STAGINGTYPE_BULLETWARNING)
			{



				pVtx[0].pos = D3DXVECTOR3(g_Staging[nCntStaging].pos.x - APPER_STAGINGSIZE, g_Staging[nCntStaging].pos.y - APPER_STAGINGSIZE, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_Staging[nCntStaging].pos.x + APPER_STAGINGSIZE, g_Staging[nCntStaging].pos.y - APPER_STAGINGSIZE, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_Staging[nCntStaging].pos.x - APPER_STAGINGSIZE, g_Staging[nCntStaging].pos.y + APPER_STAGINGSIZE, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_Staging[nCntStaging].pos.x + APPER_STAGINGSIZE, g_Staging[nCntStaging].pos.y + APPER_STAGINGSIZE, 0.0f);

				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

				if (g_Staging[nCntStaging].nStagingCount % 5 == 0)
				{
					g_Staging[nCntStaging].bDraw = false;
				}
				else
				{
					g_Staging[nCntStaging].bDraw = true;
				}
				if (g_Staging[nCntStaging].nStagingCount >= 150)
				{
					PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_004);
					g_Staging[nCntStaging].bUse = false;

					int nCnt;
					for (nCnt = 0; nCnt < 8; nCnt++)
					{
						SetBullet(g_Staging[nCntStaging].pos, D3DXVECTOR3((sinf(-D3DX_PI * (0.25f*nCnt)) * 5.0f), (cosf(-D3DX_PI * (0.25*nCnt)) * 5.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0,-1, -1);

					}
				}



			}
			if (g_Staging[nCntStaging].nStagingType == STAGINGTYPE_PRESSSTART||g_Staging[nCntStaging].nStagingType==STAGINGTYPE_PRESSENTER)

			{



				pVtx[0].pos = D3DXVECTOR3(g_Staging[nCntStaging].pos.x - PRESSSTART_STAGINGSIZE, g_Staging[nCntStaging].pos.y - PRESSSTART_STAGINGSIZE, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_Staging[nCntStaging].pos.x + PRESSSTART_STAGINGSIZE, g_Staging[nCntStaging].pos.y - PRESSSTART_STAGINGSIZE, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_Staging[nCntStaging].pos.x - PRESSSTART_STAGINGSIZE, g_Staging[nCntStaging].pos.y + PRESSSTART_STAGINGSIZE, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_Staging[nCntStaging].pos.x + PRESSSTART_STAGINGSIZE, g_Staging[nCntStaging].pos.y + PRESSSTART_STAGINGSIZE, 0.0f);

				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);



				//===============================================================
				//アルファ値のインクリメント、デクリメントの処理
				//===============================================================
				if (g_Staging[nCntStaging].bColorChange_a == false)
				{
					g_Staging[nCntStaging].fColor_a += 0.02f;
				}
				else
				{
					g_Staging[nCntStaging].fColor_a -= 0.02f;
				}
				//================================================================

				//================================================================
				//アルファ値を変えるフラグの設定処理
				//================================================================
				if (g_Staging[nCntStaging].fColor_a >= 1.0f)
				{
					g_Staging[nCntStaging].bColorChange_a = true;
				}
				if (g_Staging[nCntStaging].fColor_a <= 0.0f)
				{
					g_Staging[nCntStaging].bColorChange_a = false;
				}
				//================================================================
				//頂点カラーの設定
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Staging[nCntStaging].fColor_a);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Staging[nCntStaging].fColor_a);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Staging[nCntStaging].fColor_a);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Staging[nCntStaging].fColor_a);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
			}
			if (g_Staging[nCntStaging].nStagingType == STAGINGTYPE_SHIELD)
			{

				g_Staging[nCntStaging].nLife--;

				int nCntBullet;

				pVtx[0].pos = D3DXVECTOR3(g_Staging[nCntStaging].pos.x - SHIELD_SIZE, g_Staging[nCntStaging].pos.y - SHIELD_SIZE, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_Staging[nCntStaging].pos.x + SHIELD_SIZE, g_Staging[nCntStaging].pos.y - SHIELD_SIZE, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_Staging[nCntStaging].pos.x - SHIELD_SIZE, g_Staging[nCntStaging].pos.y + SHIELD_SIZE, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_Staging[nCntStaging].pos.x + SHIELD_SIZE, g_Staging[nCntStaging].pos.y + SHIELD_SIZE, 0.0f);

				Bullet* pBullet = GetBullet();
				
					for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++, pBullet++)
					{
						if (pBullet->bUse == true)
						{
							if (pBullet->type == BULLETTYPE_BANDY
								|| pBullet->type == BULLETTYPE_BOLBY
								|| pBullet->type == BULLETTYPE_DOWNWIND
								|| pBullet->type == BULLETTYPE_ENEMY
								|| pBullet->type == BULLETTYPE_ENEMY9
								|| pBullet->type == BULLETTYPE_LEFT_WIND
								|| pBullet->type == BULLETTYPE_REAFY
								|| pBullet->type == BULLETTYPE_REFLECTION
								|| pBullet->type == BULLETTYPE_REVERSE_CURVE
								|| pBullet->type == BULLETTYPE_RIGHT_WIND
								|| pBullet->type == BULLETTYPE_ROCKON
								|| pBullet->type == BULLETTYPE_SCATTERING
								|| pBullet->type == BULLETTYPE_UPWIND
								|| pBullet->type == BULLETTYPE_SATELLITE)
							{
								if (pBullet->pos.x >= g_Staging[nCntStaging].pos.x - SHIELD_SIZE && pBullet->pos.x <= g_Staging[nCntStaging].pos.x + SHIELD_SIZE &&
									pBullet->pos.y >= g_Staging[nCntStaging].pos.y - SHIELD_SIZE && pBullet->pos.y <= g_Staging[nCntStaging].pos.y + SHIELD_SIZE)
								{
									pBullet->bUse = false;
									
								}
							}
						}
					}
				
				

				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);



				
			}
			if (g_Staging[nCntStaging].nStagingType == STAGINGTYPE_NORMAL_CLEARBONUS
				|| g_Staging[nCntStaging].nStagingType == STAGINGTYPE_HARD_CLEARBONUS
				|| g_Staging[nCntStaging].nStagingType == STAGINGTYPE_VERYHARD_CLEARBONUS)
			{
				pVtx[0].pos = D3DXVECTOR3(g_Staging[nCntStaging].pos.x - SCOREBONUS_SIZE, g_Staging[nCntStaging].pos.y - SCOREBONUS_SIZE, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_Staging[nCntStaging].pos.x + SCOREBONUS_SIZE, g_Staging[nCntStaging].pos.y - SCOREBONUS_SIZE, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_Staging[nCntStaging].pos.x - SCOREBONUS_SIZE, g_Staging[nCntStaging].pos.y + SCOREBONUS_SIZE, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_Staging[nCntStaging].pos.x + SCOREBONUS_SIZE, g_Staging[nCntStaging].pos.y + SCOREBONUS_SIZE, 0.0f);

				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			}
			if (g_Staging[nCntStaging].nStagingType == STAGINGTYPE_RANKING_BG)
			{
				pVtx[0].pos = D3DXVECTOR3(g_Staging[nCntStaging].pos.x - SCREEN_WIDTH/2, g_Staging[nCntStaging].pos.y - SCREEN_HEIGHT/2, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_Staging[nCntStaging].pos.x + SCREEN_WIDTH/2, g_Staging[nCntStaging].pos.y - SCREEN_HEIGHT/2, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_Staging[nCntStaging].pos.x - SCREEN_WIDTH/2, g_Staging[nCntStaging].pos.y + SCREEN_HEIGHT/2, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_Staging[nCntStaging].pos.x + SCREEN_WIDTH/2, g_Staging[nCntStaging].pos.y + SCREEN_HEIGHT/2, 0.0f);

				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			}
		}
		pVtx += 4;//頂点データのポインタを４つ分進める。
	}
//頂点バッファをアンロックする
	g_pVtxBuffStaging->Unlock();

	
}


//=========================================
//敵の描画処理
//=========================================
void DrawStaging(void)
{

	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	Enemy* pEnemy = GetEnemy();

	int nCntStaging;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffStaging, 0, sizeof(VERTEX_2D));//頂点バッファへのポインタと頂点情報の構造体のサイズ

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定

	for (nCntStaging = 0; nCntStaging < MAX_STAGING; nCntStaging++)
	{
		if (g_Staging[nCntStaging].bUse == true)
		{
			if (g_Staging[nCntStaging].bDraw == true)
			{
				if (g_Staging[nCntStaging].nStagingType == STAGINGTYPE_WARNING)
				{
					pDevice->SetTexture(0, g_apTextureStaging[STAGINGTYPE_WARNING]);
				}
				if (g_Staging[nCntStaging].nStagingType == STAGINGTYPE_APPER)
				{
					pDevice->SetTexture(0, g_apTextureStaging[STAGINGTYPE_APPER]);
				}
				if (g_Staging[nCntStaging].nStagingType ==STAGINGTYPE_BULLETWARNING)
				{
					pDevice->SetTexture(0, g_apTextureStaging[STAGINGTYPE_BULLETWARNING]);
				}
				if (g_Staging[nCntStaging].nStagingType == STAGINGTYPE_PRESSSTART)
				{
					pDevice->SetTexture(0, g_apTextureStaging[STAGINGTYPE_PRESSSTART]);
				}
				if (g_Staging[nCntStaging].nStagingType == STAGINGTYPE_PRESSENTER)
				{
					pDevice->SetTexture(0, g_apTextureStaging[STAGINGTYPE_PRESSENTER]);
				}
				if (g_Staging[nCntStaging].nStagingType == STAGINGTYPE_SHIELD)
				{
					pDevice->SetTexture(0, g_apTextureStaging[STAGINGTYPE_SHIELD]);
				}
				if (g_Staging[nCntStaging].nStagingType == STAGINGTYPE_NORMAL_CLEARBONUS)
				{
					pDevice->SetTexture(0, g_apTextureStaging[STAGINGTYPE_NORMAL_CLEARBONUS]);
				}
				if (g_Staging[nCntStaging].nStagingType == STAGINGTYPE_HARD_CLEARBONUS)
				{
					pDevice->SetTexture(0, g_apTextureStaging[STAGINGTYPE_HARD_CLEARBONUS]);
				}
				if (g_Staging[nCntStaging].nStagingType == STAGINGTYPE_VERYHARD_CLEARBONUS)
				{
					pDevice->SetTexture(0, g_apTextureStaging[STAGINGTYPE_VERYHARD_CLEARBONUS]);
				}
				if (g_Staging[nCntStaging].nStagingType == STAGINGTYPE_RANKING_BG)
				{
					pDevice->SetTexture(0, g_apTextureStaging[STAGINGTYPE_RANKING_BG]);
				}
				//ポリゴンの描画
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntStaging * 4, 2);//0は、描画する最初の頂点インデックスで、2は、描画するプリミティブ数

				//&g_pVtxBuffPolygon[0],//頂点情報の先頭アドレス
				//sizeof(VERTEX_2D));//頂点情報構造体のサイズ
			}
		}
	}


}

void SetStaging(STAGINGTYPE type,int nCntEnemy,D3DXVECTOR3 pos)
{
	int nCntStaging;

	VERTEX_2D* pVtx;

	/*Enemy* pEnemy = GetEnemy();*/

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffStaging->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntStaging = 0; nCntStaging < MAX_STAGING; nCntStaging++)
	{
		if (g_Staging[nCntStaging].bUse == false)
		{
			g_Staging[nCntStaging].pos = pos;//SetEnemyの時に、敵の位置が代入された際、位置の引数をさらにSetStagingのposに代入することにより、
			g_Staging[nCntStaging].nLife = 100;
			g_Staging[nCntStaging].nStagingType = type;//演出の種類をセットする。
			g_Staging[nCntStaging].bUse = true;
			g_Staging[nCntStaging].nStagingCount = 0;
			g_Staging[nCntStaging].bCountStartFlag = true;
			g_Staging[nCntStaging].fStagingPatternCountY = 0.0f;
			g_Staging[nCntStaging].fStagingPattern = 0.0f;
			g_Staging[nCntStaging].nAnimationRepeat = 0;
			g_Staging[nCntStaging].bDraw = true;
			g_Staging[nCntStaging].nCntEnemy = nCntEnemy;
			g_Staging[nCntStaging].fColor_a = 1.0f;
			g_Staging[nCntStaging].bColorChange_a = false;
			break;
		}
		pVtx += 4;
	}
	g_pVtxBuffStaging->Unlock();
}

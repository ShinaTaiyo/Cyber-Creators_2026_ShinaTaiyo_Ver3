//========================================================
//
//８月３０日：エフェクトもう一回見直す[Block.cpp]
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
#include "block.h"
#include "sound.h"
#include "fade.h"
#include "health.h"
//マクロ定義
#include "input.h"
#define NUM_BLOCKTEX (14)//ブロックの種類
#define FRAME_BLOCKSIZE (20.0f) //炎ブロックの大きさ



//グローバル変数
bool g_bIsLanding;

int g_nCntBlockTime;//ブロックが出現してからの時間を測る

float g_fWingMove;//羽ブロックを上下に動かす

LPDIRECT3DTEXTURE9 g_apTextureBlock[NUM_BLOCKTEX] = {};
//D3DXVECTOR3 g_rotPI;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBlock = NULL;
//Enemy g_Block[MAX_ENEMY];//敵の情報
//Result g_aResult;

Block g_aBlock[MAX_BLOCK];

//int g_nCountTime = 0;//時間を表すグローバル変数

//=====================================================
//==========================
//ブロックの初期化処理
//==========================
void InitBlock(void)
{
	//srand((unsigned int)time(NULL));//ゲームを開始した時間毎に乱数の種を設定
	LPDIRECT3DDEVICE9 pDevice;
	int	nCntBlock;
	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Block_000.png",
		&g_apTextureBlock[BLOCKTYPE_NORMAL]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\GrassBlock_001.png",
		&g_apTextureBlock[BLOCKTYPE_GRASS]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\GrayRenga_000.png",
		&g_apTextureBlock[BLOCKTYPE_RENGA]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\FrameBlock_001.png",
		&g_apTextureBlock[BLOCKTYPE_FRAME]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\WaterBlock_000.png",
		&g_apTextureBlock[BLOCKTYPE_WATER]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\SandBlock_000.png",
		&g_apTextureBlock[BLOCKTYPE_SAND]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\WingBlock_000.png",
		&g_apTextureBlock[BLOCKTYPE_WING]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\StoneBrock_001.png",
		&g_apTextureBlock[BLOCKTYPE_STONE]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\LineBlock_000.png",
		&g_apTextureBlock[BLOCKTYPE_LINE]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\IceBlock_000.png",
		&g_apTextureBlock[BLOCKTYPE_ICE]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\ArrowBlock.png",
		&g_apTextureBlock[BLOCKTYPE_ARROW]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\AutoMoveBlock_Right.png",
		&g_apTextureBlock[BLOCKTYPE_AUTOMOVE_RIGHT]);


	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\AutoMoveBlock_Left.png",
		&g_apTextureBlock[BLOCKTYPE_AUTOMOVE_LEFT]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\BlinkingBlock.png",
		&g_apTextureBlock[BLOCKTYPE_BLINKING]);

	g_fWingMove = 0.0f;

	//ブロック情報の初期化
	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		g_aBlock[nCntBlock].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//ブロックの位置
		g_aBlock[nCntBlock].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//ブロックの位置
		g_aBlock[nCntBlock].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//動き
		g_aBlock[nCntBlock].nType= 0;//ブロックの種類
		g_aBlock[nCntBlock].nLife = 0;//ブロックのライフ
		g_aBlock[nCntBlock].bUse = false;//ブロックが使われているか銅貨
		g_aBlock[nCntBlock].bMoveWay = false;//移動方向を決めるフラグ
		g_aBlock[nCntBlock].bDraw = false;//bUseがtrueだったことに加え、更にbDrawがtrueかどうかで描画するかどうかを決める。
		g_aBlock[nCntBlock].fXsize = 0.0f;//X方向のサイズ
		g_aBlock[nCntBlock].fYsize = 0.0f;//Y方向のサイズ
		g_aBlock[nCntBlock].fRadius = 0.0f;//半径でサイズを決める場合のサイズ
		g_aBlock[nCntBlock].nCollisionUse = 0;//何がbCollisionBlockを呼んでいるか
		g_aBlock[nCntBlock].fWingMove = 0.0f;//ブロックをゆらゆら動かす
	}

	g_nCntBlockTime = 0;//ブロックが出現してからの時間を測る

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BLOCK,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBlock,
		NULL);

	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
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
	g_pVtxBuffBlock->Unlock();
}

//====================================
//チュートリアルの終了処理
//====================================
void UninitBlock(void)
{
	//テクスチャの破棄
	int nCntBlock;
	for (nCntBlock = 0; nCntBlock < NUM_BLOCKTEX; nCntBlock++)
	{
		if (g_apTextureBlock[nCntBlock] != NULL)
		{
			g_apTextureBlock[nCntBlock]->Release();
			g_apTextureBlock[nCntBlock] = NULL;
		}
	}
	//頂点バッファの破棄
	if (g_pVtxBuffBlock != NULL)
	{
		g_pVtxBuffBlock->Release();
		g_pVtxBuffBlock = NULL;
	}
}


//========================================
//敵の更新処理
//========================================
void UpdateBlock(void)
{
	VERTEX_2D* pVtx;
	int nCntBlock;
	Player* pPlayer = GetPlayer();
	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	//g_fWingMove += 0.01f;

	g_nCntBlockTime++;//ゲームが始まってからの時間を測る

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if( g_aBlock[nCntBlock].bUse == true)
		{
			g_aBlock[nCntBlock].nLife--;
			g_aBlock[nCntBlock].fWingMove += 0.01f;
				//ブロックの位置の設定
				pVtx[0].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x - g_aBlock[nCntBlock].fRadius, g_aBlock[nCntBlock].pos.y - g_aBlock[nCntBlock].fRadius, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fRadius, g_aBlock[nCntBlock].pos.y - g_aBlock[nCntBlock].fRadius, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x - g_aBlock[nCntBlock].fRadius, g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fRadius, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fRadius, g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fRadius, 0.0f);



				if (g_aBlock[nCntBlock].nType == BLOCKTYPE_WING)
				{
					g_aBlock[nCntBlock].move.y = cosf(D3DX_PI * g_aBlock[nCntBlock].fWingMove ) * 5.0f;
				}

				if (g_aBlock[nCntBlock].nType == BLOCKTYPE_ARROW)
				{
				
					g_aBlock[nCntBlock].move.x = sinf(D3DX_PI * g_aBlock[nCntBlock].fWingMove) * 5.0f;
				}

				if (g_aBlock[nCntBlock].nType == BLOCKTYPE_BLINKING && g_nCntBlockTime % 60 == 0)
				{
					g_aBlock[nCntBlock].bDraw = g_aBlock[nCntBlock].bDraw ? false : true;
				}

				if (g_aBlock[nCntBlock].nType == BLOCKTYPE_WATER)
				{
					if (pPlayer->pos.x >= g_aBlock[nCntBlock].pos.x - g_aBlock[nCntBlock].fRadius - pPlayer->fWidth &&
						pPlayer->pos.x <= g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fRadius + pPlayer->fWidth &&
						pPlayer->pos.y >= g_aBlock[nCntBlock].pos.y - g_aBlock[nCntBlock].fRadius &&
						pPlayer->pos.y <= g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fRadius + pPlayer->fHeight)
					{
						if (GetKeyboardPress(DIK_SPACE) == true || GetJoypadPress(JOYKEY_A) == true)
						{
							pPlayer->move.y = -10.0f;
						}
					}
				}
				
				
				g_aBlock[nCntBlock].pos.x += g_aBlock[nCntBlock].move.x;
				g_aBlock[nCntBlock].pos.y += g_aBlock[nCntBlock].move.y;

				if (g_aBlock[nCntBlock].nLife == 0)
				{
					g_aBlock[nCntBlock].bUse = false;
				}

		}
		pVtx += 4;//頂点データのポインタを４つ分進める。
	}
	//頂点バッファをアンロックする
	g_pVtxBuffBlock->Unlock();


}


//=========================================
//敵の描画処理
//=========================================
void DrawBlock(void)
{

	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	int nCntBlock;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBlock, 0, sizeof(VERTEX_2D));//頂点バッファへのポインタと頂点情報の構造体のサイズ

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
	
		if (g_aBlock[nCntBlock].bUse == true)
		{
			if (g_aBlock[nCntBlock].bDraw == true)
			{
				if (g_aBlock[nCntBlock].nType == BLOCKTYPE_NORMAL)
				{
					pDevice->SetTexture(0, g_apTextureBlock[BLOCKTYPE_NORMAL]);
				}
				else if (g_aBlock[nCntBlock].nType == BLOCKTYPE_GRASS)
				{
					pDevice->SetTexture(0, g_apTextureBlock[BLOCKTYPE_GRASS]);
				}
				else if (g_aBlock[nCntBlock].nType == BLOCKTYPE_RENGA)
				{
					pDevice->SetTexture(0, g_apTextureBlock[BLOCKTYPE_RENGA]);
				}
				else if (g_aBlock[nCntBlock].nType == BLOCKTYPE_FRAME)
				{
					pDevice->SetTexture(0, g_apTextureBlock[BLOCKTYPE_FRAME]);
				}
				else if (g_aBlock[nCntBlock].nType == BLOCKTYPE_WATER)
				{
					pDevice->SetTexture(0, g_apTextureBlock[BLOCKTYPE_WATER]);
				}
				else if (g_aBlock[nCntBlock].nType == BLOCKTYPE_SAND)
				{
					pDevice->SetTexture(0, g_apTextureBlock[BLOCKTYPE_SAND]);
				}
				else if (g_aBlock[nCntBlock].nType == BLOCKTYPE_WING)
				{
					pDevice->SetTexture(0, g_apTextureBlock[BLOCKTYPE_WING]);
				}
				else if (g_aBlock[nCntBlock].nType == BLOCKTYPE_STONE)
				{
					pDevice->SetTexture(0, g_apTextureBlock[BLOCKTYPE_STONE]);
				}
				else if (g_aBlock[nCntBlock].nType == BLOCKTYPE_LINE)
				{
					pDevice->SetTexture(0, g_apTextureBlock[BLOCKTYPE_LINE]);
				}
				else if (g_aBlock[nCntBlock].nType == BLOCKTYPE_ICE)
				{
					pDevice->SetTexture(0, g_apTextureBlock[BLOCKTYPE_ICE]);
				}
				else if (g_aBlock[nCntBlock].nType == BLOCKTYPE_ARROW)
				{
					pDevice->SetTexture(0, g_apTextureBlock[BLOCKTYPE_ARROW]);
				}
				else if (g_aBlock[nCntBlock].nType == BLOCKTYPE_AUTOMOVE_RIGHT)
				{
					pDevice->SetTexture(0, g_apTextureBlock[BLOCKTYPE_AUTOMOVE_RIGHT]);
				}
				else if (g_aBlock[nCntBlock].nType == BLOCKTYPE_AUTOMOVE_LEFT)
				{
					pDevice->SetTexture(0, g_apTextureBlock[BLOCKTYPE_AUTOMOVE_LEFT]);
				}
				else if (g_aBlock[nCntBlock].nType == BLOCKTYPE_BLINKING)
				{
					pDevice->SetTexture(0, g_apTextureBlock[BLOCKTYPE_BLINKING]);
				}
				//ポリゴンの描画
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBlock * 4, 2);//0は、描画する最初の頂点インデックスで、2は、描画するプリミティブ数

				//&g_pVtxBuffPolygon[0],//頂点情報の先頭アドレス
				//sizeof(VERTEX_2D));//頂点情報構造体のサイズ
			}
		}
	}


}

void SetBlock(int ntype, D3DXVECTOR3 pos, int nLife)
{
	int nCntBlock;

	VERTEX_2D* pVtx;

	/*Enemy* pEnemy = GetEnemy();*/

	//頂点バッファをロックし、頂点データへのポインタを取得

	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse == false)
		{
			g_aBlock[nCntBlock].pos = pos;//SetEnemyの時に、敵の位置が代入された際、位置の引数をさらにSetBlockのposに代入することにより、
			g_aBlock[nCntBlock].nLife = nLife;//ライフ（表示時間）
			g_aBlock[nCntBlock].nType = ntype;//ブロックの種類をセットする。
			g_aBlock[nCntBlock].nCollisionUse = 0;//何がbCollisionBlockを呼んでいるか
			g_aBlock[nCntBlock].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//動き
			g_aBlock[nCntBlock].bDraw = true;//描画するかどうか
			g_aBlock[nCntBlock].fWingMove = 0.0f;//ブロックをゆらゆら動かす
			//=====================================================
			//ブロックの大きさ設定処理
			//=====================================================

			//何も変更を加えない場合のブロックのサイズ
			g_aBlock[nCntBlock].fRadius = NORMAL_BLOCKSIZE;



			if (g_aBlock[nCntBlock].nType == BLOCKTYPE_NORMAL)
			{//ブロックのタイプが「ノーマル」
				g_aBlock[nCntBlock].fRadius = NORMAL_BLOCKSIZE;//半径でを決める場合の大きさ
			}
			if (g_aBlock[nCntBlock].nType == BLOCKTYPE_FRAME)
			{
				g_aBlock[nCntBlock].fRadius = FRAME_BLOCKSIZE;//半径でを決める場合の大きさ
			}

			if (g_aBlock[nCntBlock].nType >= BLOCKTYPE_NORMAL && g_aBlock[nCntBlock].nType <= NUM_BLOCKTEX - 1)
			{
				g_aBlock[nCntBlock].bUse = true;//使用しているかどうか
			}
			else
			{//ブロックの種類の中じゃなかったら使用しない
				g_aBlock[nCntBlock].bUse = false;
			}
			break;
		}
		pVtx += 4;
	}
	g_pVtxBuffBlock->Unlock();
}

Block* GetBlock(void)
{
	return &g_aBlock[0];
}
bool bCollisionBlock(D3DXVECTOR3* pPos,
	D3DXVECTOR3* pPosOld,
	D3DXVECTOR3* pMove,
	float fWidth,
	float fHeight,
	float* fGravity,
	COLLISIONTYPE CollisionType,
	bool* bMoveWay,
	COLLISIONUSE C_use,
	bool* bIsWalling)
{
	Player* pPlayer = GetPlayer();
	g_bIsLanding = false;//着地しているかどうか
	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse == true)
		
		{//ブロックを使っているかどうかfWidth
			if (g_aBlock[nCntBlock].nType != BLOCKTYPE_WATER)
			{
				if (g_aBlock[nCntBlock].bDraw == true)
				{//描画されているかどうか
					if (CollisionType == COLLISIONTYPE_RADIUS)
					{


						if (pPos->x - fWidth < 0.0f)
						{
							pPos->x = 0.0f + fWidth;
							if (*bMoveWay == false)
							{
								*bMoveWay = true;
							}
							else if (*bMoveWay == true)
							{
								*bMoveWay = false;
							}
						}
						else if (pPos->x + fWidth > SCREEN_WIDTH)
						{
							pPos->x = SCREEN_WIDTH - fWidth;
							if (*bMoveWay == false)
							{
								*bMoveWay = true;
							}
							else if (*bMoveWay == true)
							{
								*bMoveWay = false;
							}
						}

						if (pPos->x + fWidth >= g_aBlock[nCntBlock].pos.x - g_aBlock[nCntBlock].fRadius//プレイヤーの右端がブロックの左端より内側なら
							&& pPosOld->x + fWidth <= g_aBlock[nCntBlock].pos.x - 5.0f /*- g_aBlock[nCntBlock].fRadius*///1F前のプレイヤーの右端がブロックの右端より
							&& pPos->y - fHeight < g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fRadius//プレイヤーの上端がブロックの下端より上なら
							&& pPos->y > g_aBlock[nCntBlock].pos.y - g_aBlock[nCntBlock].fRadius)//プレイヤーの下端がブロックの上端より下なら
						{//プレイヤーの右端がブロックの中に入ったら
							pPos->x = g_aBlock[nCntBlock].pos.x - g_aBlock[nCntBlock].fRadius - fWidth;
							pMove->x = -0.0f;

							*bIsWalling = true;
							if (*bMoveWay == false)
							{
								*bMoveWay = true;
							}
							else if (*bMoveWay == true)
							{
								*bMoveWay = false;
							}



							if (g_aBlock[nCntBlock].nType == BLOCKTYPE_FRAME && C_use == COLLISIONUSE_PLAYER)
							{
								DamageHealth(1);//１ダメージを与え続ける
							}
						}
						if (pPos->x - fWidth < g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fRadius//プレイヤーの左端がブロックの右端より内側なら
							&& pPosOld->x - fWidth >= g_aBlock[nCntBlock].pos.x + 5.0f/*+ g_aBlock[nCntBlock].fRadius*///1F前のプレイヤーの左端がブロックの右端より外側なら
							&& pPos->y - fHeight < g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fRadius//プレイヤーの上端がブロックの下端より内側なら
							&& pPos->y > g_aBlock[nCntBlock].pos.y - g_aBlock[nCntBlock].fRadius)//プレイヤーの下端がブロックの上端より内側なら
						{//プレイヤーの左端がブロックの中に入ったら
							pPos->x = g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fRadius + fWidth;
							pMove->x = 0.0f;

							*bIsWalling = true;
							if (*bMoveWay == false)
							{
								*bMoveWay = true;
							}
							else if (*bMoveWay == true)
							{
								*bMoveWay = false;
							}

							if (g_aBlock[nCntBlock].nType == BLOCKTYPE_FRAME && C_use == COLLISIONUSE_PLAYER)
							{
								DamageHealth(1);//１ダメージを与え続ける
							}
						}
						if (pPos->x + fWidth > g_aBlock[nCntBlock].pos.x - g_aBlock[nCntBlock].fRadius
							&& pPos->x - fWidth < g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fRadius
							&& pPos->y >= g_aBlock[nCntBlock].pos.y - g_aBlock[nCntBlock].fRadius
							&& pPosOld->y <= g_aBlock[nCntBlock].pos.y - 5.0f/*- g_aBlock[nCntBlock].fRadius*/)
						{//プレイヤーの下端が、ブロックの上端より下なら

							if (g_aBlock[nCntBlock].nType == BLOCKTYPE_ARROW && *bIsWalling == false)
							{
								pPos->x = g_aBlock[nCntBlock].pos.x;//プレイヤーの位置をブロックの上端に移動させる。
							}

							pPos->y = g_aBlock[nCntBlock].pos.y - g_aBlock[nCntBlock].fRadius;//プレイヤーの位置をブロックの上端に移動させる。
							pMove->y = 0.0f;//重力を0にする
							*fGravity = 0.0f;//重力の補正値も0にする　
							g_bIsLanding = true;//地面にいるフラグを立て、ジャンプを可能にする

							if (g_aBlock[nCntBlock].nType == BLOCKTYPE_FRAME && C_use == COLLISIONUSE_PLAYER)
							{
								DamageHealth(1);//１ダメージを与え続ける
							}
							else if (g_aBlock[nCntBlock].nType == BLOCKTYPE_ICE && g_aBlock[nCntBlock].nCollisionUse == COLLISIONUSE_PLAYER)
							{
								pPlayer->nInertia = INERTIA_ICE;//氷ブロックの上にいるので、慣性を強める
							}
							else if (g_aBlock[nCntBlock].nType == BLOCKTYPE_AUTOMOVE_RIGHT && g_aBlock[nCntBlock].nCollisionUse == COLLISIONUSE_PLAYER)
							{
								pPlayer->bAutoMoveR = true;//右方向へ自動的に移動するか
							}
							else if (g_aBlock[nCntBlock].nType == BLOCKTYPE_AUTOMOVE_LEFT && g_aBlock[nCntBlock].nCollisionUse == COLLISIONUSE_PLAYER)
							{
								pPlayer->bAutoMoveL = true;//左方向へ自動的に移動するか
							}


						}
						if (pPos->x + fWidth > g_aBlock[nCntBlock].pos.x - g_aBlock[nCntBlock].fRadius
							&& pPos->x - fWidth < g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fRadius
							&& pPosOld->y - fHeight >= g_aBlock[nCntBlock].pos.y + 5.0f/* + g_aBlock[nCntBlock].fRadius*/
							&& pPos->y - fHeight <= g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fRadius)
						{//PosOldの頭がブロックの下端より上で、プレイヤーのposがブロックの下端より下なら・・・



							pPos->y = g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fRadius + fHeight + 6.0f;//プレイヤーの位置をブロックの下端に移動させる。
							pMove->y = 0.0f;//重力を０にする
							if (g_aBlock[nCntBlock].nType == BLOCKTYPE_FRAME && C_use == COLLISIONUSE_PLAYER)
							{
								DamageHealth(1);//１ダメージを与え続ける
							}
						}
					}
					else if (CollisionType == COLLISIONTYPE_LENGTH)
					{
						if (pPos->x + fWidth / 2 >= g_aBlock[nCntBlock].pos.x - g_aBlock[nCntBlock].fRadius//プレイヤーの右端がブロックの左端より内側なら
							&& pPosOld->x + fWidth / 2 <= g_aBlock[nCntBlock].pos.x - g_aBlock[nCntBlock].fRadius//1F前のプレイヤーの右端がブロックの右端より
							&& pPos->y - fHeight < g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fRadius//プレイヤーの上端がブロックの下端より上なら
							&& pPos->y > g_aBlock[nCntBlock].pos.y - g_aBlock[nCntBlock].fRadius)//プレイヤーの下端がブロックの上端より下なら
						{//プレイヤーの右端がブロックの中に入ったら

							*bIsWalling = true;

							pPos->x = g_aBlock[nCntBlock].pos.x - g_aBlock[nCntBlock].fRadius - fWidth / 2;
							pMove->x = -0.0001f;


						}
						else if (pPos->x - fWidth / 2 <= g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fRadius//プレイヤーの左端がブロックの右端より内側なら
							&& pPosOld->x - fWidth / 2 >= g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fRadius//1F前のプレイヤーの左端がブロックの右端より外側なら
							&& pPos->y - fHeight < g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fRadius//プレイヤーの上端がブロックの下端より内側なら
							&& pPos->y > g_aBlock[nCntBlock].pos.y - g_aBlock[nCntBlock].fRadius)//プレイヤーの下端がブロックの上端より内側なら
						{//プレイヤーの左端がブロックの中に入ったら
							*bIsWalling = true;

							pPos->x = g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fRadius + fWidth / 2;
							pMove->x = 0.0001f;



						}
						else if (pPos->x + fWidth / 2 - 20.0f > g_aBlock[nCntBlock].pos.x - g_aBlock[nCntBlock].fRadius
							&& pPos->x - fWidth / 2 + 20.0f < g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fRadius
							&& pPos->y >= g_aBlock[nCntBlock].pos.y - g_aBlock[nCntBlock].fRadius
							&& pPosOld->y <= g_aBlock[nCntBlock].pos.y - g_aBlock[nCntBlock].fRadius)
						{//プレイヤーの下端が、ブロックの上端より下なら

							if (g_aBlock[nCntBlock].nType == BLOCKTYPE_ARROW && *bIsWalling == false)
							{
								pPos->x = g_aBlock[nCntBlock].pos.x;//プレイヤーの位置をブロックの上端に移動させる。

							}

							pPos->y = g_aBlock[nCntBlock].pos.y - g_aBlock[nCntBlock].fRadius;//プレイヤーの位置をブロックの上端に移動させる。
							pMove->y = 0.0f;//重力を0にする
							*fGravity = 0.0f;//重力の補正値も0にする　
							g_bIsLanding = true;//地面にいるフラグを立て、ジャンプを可能にする

						}
						else if (pPos->x + fWidth / 2 - 20.0f > g_aBlock[nCntBlock].pos.x - g_aBlock[nCntBlock].fRadius
							&& pPos->x - fWidth / 2 + 20.0f < g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fRadius
							&& pPosOld->y - fHeight >= g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fRadius
							&& pPos->y - fHeight <= g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fRadius)
						{//PosOldの頭がブロックの下端より上で、プレイヤーのposがブロックの下端より下なら・・・
							pPos->y = g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fRadius + fHeight;//プレイヤーの位置をブロックの下端に移動させる。
							pMove->y = 0.0f;//重力を０にする

						}
					}

				}
			}
		}
	}

		return g_bIsLanding;
	
}

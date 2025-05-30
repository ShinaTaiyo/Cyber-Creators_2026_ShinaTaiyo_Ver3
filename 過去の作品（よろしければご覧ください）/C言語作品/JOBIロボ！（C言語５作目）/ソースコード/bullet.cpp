//========================================================
//
//９月２６日：ハッカソンの基盤作り[Bullet.cpp]
//Author:ShinaTaiyo
//
//=========================================================

#include "bullet.h"
#include "main.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "shadow.h"
#include "effect.h"
#include "wall.h"
#include "particle.h"
#include "player.h"
#include "enemy.h"
#include "bullet.h"
#include "battery.h"
#include "attackmodel.h"
#include "attackbillboard.h"
#define BULLET_SCALE (10.0f)
//グローバル変数
PDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;//頂点バッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureBullet[NUM_BULLET] = {};//テクスチャへのポインタ
D3DXVECTOR3 g_posBullet;//位置
//D3DXVECTOR3 g_rotBullet;//向き
D3DXMATRIX g_mtxWorldBullet;//ワールドマトリックス(float4*4)

Bullet g_aBullet[MAX_BULLET];//弾の構造体情報

int g_nDelayBullet = 0;

//タイトル画面の初期化処理
void InitBullet(void)
{
	g_nDelayBullet = 0;

	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\lightbrueball_enemy.png",
		&g_pTextureBullet[0]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Bullet001.png",
		&g_pTextureBullet[1]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\greenball_enemy.png",
		&g_pTextureBullet[2]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\purpleball_enemy.png",
		&g_pTextureBullet[3]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Bullet003.png",
		&g_pTextureBullet[4]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\orangeBall_enemy.png",
		&g_pTextureBullet[5]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Bullet002.png",
		&g_pTextureBullet[6]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Bullet004.png",
		&g_pTextureBullet[7]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Bullet001.png",
		&g_pTextureBullet[8]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\ChaosBall.png",
		&g_pTextureBullet[9]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\FireBall.png",
		&g_pTextureBullet[10]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\WaveBall.png",
		&g_pTextureBullet[11]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\ThunderBall.png",
		&g_pTextureBullet[12]);

	g_posBullet = D3DXVECTOR3(0.0f, 50.0f, 100.0f);//位置の初期化

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_BULLET,//頂点の数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBullet,
		NULL);

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		g_aBullet[nCntBullet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//位置
		g_aBullet[nCntBullet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//移動量
		g_aBullet[nCntBullet].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//向き
		g_aBullet[nCntBullet].Size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//大きさ
		g_aBullet[nCntBullet].nLife = 0;//体力
		g_aBullet[nCntBullet].fRWidth = 0.0f;//幅（半径）
		g_aBullet[nCntBullet].fRHeight = 0.0f;//高さ（半径）
		g_aBullet[nCntBullet].nType = 0;//種類
		g_aBullet[nCntBullet].bUse = false;//使用しているかどうか
		g_aBullet[nCntBullet].nNumEnemy = 0;//何番の敵が撃ってきているか
		g_aBullet[nCntBullet].fLength = 0.0f;//距離
		g_aBullet[nCntBullet].bOneFlag = false;
		g_aBullet[nCntBullet].bChangeFlag = false;//値を上下させる時に使う
		g_aBullet[nCntBullet].nMaxLife = 0;//最大体力
		g_aBullet[nCntBullet].nAnimationCnt = 0;//アニメーションカウント
		g_aBullet[nCntBullet].nAnimationPattern = 0;//アニメーションパターン
		g_aBullet[nCntBullet].nIdxShadow = -1;//影のインデックス
	}

	


	////タイトル画面BGMの設定
	//PlaySound(SOUND_LABEL_BGM000);

	VERTEX_3D* pVtx;
	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);


	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		//頂点座標の設定（初期位置）
		pVtx[0].pos = D3DXVECTOR3(-BULLET_SCALE, BULLET_SCALE, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(BULLET_SCALE, BULLET_SCALE, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-BULLET_SCALE, -BULLET_SCALE, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(BULLET_SCALE, -BULLET_SCALE, 0.0f);

		//norの設定（初期設定）
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。

		//テクスチャ座標の設定（初期設定）
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;//ポインタを進める
	}

	//頂点バッファをアンロックする
	g_pVtxBuffBullet->Unlock();
}

//==============================
//タイトル画面の終了処理
//==============================
void UninitBullet(void)
{
	//テクスチャの破棄
	for (int nCntBullet = 0; nCntBullet < NUM_BULLET; nCntBullet++)
	{
		if (g_pTextureBullet[nCntBullet] != NULL)
		{
			g_pTextureBullet[nCntBullet]->Release();
			g_pTextureBullet[nCntBullet] = NULL;
		}
	}
	//頂点バッファの破棄
	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}
}
//============================
//タイトル画面の更新処理
//============================
void UpdateBullet(void)
{
	VERTEX_3D* pVtx;
	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	g_nDelayBullet++;

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{

		if (g_aBullet[nCntBullet].bUse == true)
		{
			/*Wall* pWall = GetWall();*/
			Player* pPlayer = GetPlayer();
			Enemy* pEnemy = GetEnemy();
			switch (g_aBullet[nCntBullet].nType)
			{
			case BULLETTYPE_ENEMY:
				if (g_aBullet[nCntBullet].pos.x + g_aBullet[nCntBullet].fRWidth >= pPlayer->pos.x - pPlayer->Size.x / 2 &&
					g_aBullet[nCntBullet].pos.x - g_aBullet[nCntBullet].fRWidth <= pPlayer->pos.x + pPlayer->Size.x / 2 &&
					g_aBullet[nCntBullet].pos.y + g_aBullet[nCntBullet].fRHeight >= pPlayer->pos.y &&
					g_aBullet[nCntBullet].pos.y - g_aBullet[nCntBullet].fRHeight <= pPlayer->pos.y + pPlayer->Size.y &&
					g_aBullet[nCntBullet].pos.z + g_aBullet[nCntBullet].fRWidth >= pPlayer->pos.z - pPlayer->Size.z / 2 &&
					g_aBullet[nCntBullet].pos.z - g_aBullet[nCntBullet].fRWidth <= pPlayer->pos.z + pPlayer->Size.z / 2)
				{					
					DamageBullet(5);

					g_aBullet[nCntBullet].bUse = false;

				}
				break;

			case BULLETTYPE_SWING:

				g_aBullet[nCntBullet].rot.y += 0.02f;

				for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
				{
					if (pEnemy->bUse == true && g_aBullet[nCntBullet].nNumEnemy == nCntEnemy)
					{
						g_aBullet[nCntBullet].pos = D3DXVECTOR3(pEnemy->pos.x + sinf(-D3DX_PI * g_aBullet[nCntBullet].rot.y) * g_aBullet[nCntBullet].fLength,
							pEnemy->pos.y + pEnemy->Size.y / 2, pEnemy->pos.z + cosf(-D3DX_PI * g_aBullet[nCntBullet].rot.y) * g_aBullet[nCntBullet].fLength);
					}
				}

				if (g_aBullet[nCntBullet].pos.x + g_aBullet[nCntBullet].fRWidth >= pPlayer->pos.x - pPlayer->Size.x / 2 &&
					g_aBullet[nCntBullet].pos.x - g_aBullet[nCntBullet].fRWidth <= pPlayer->pos.x + pPlayer->Size.x / 2 &&
					g_aBullet[nCntBullet].pos.y + g_aBullet[nCntBullet].fRHeight >= pPlayer->pos.y &&
					g_aBullet[nCntBullet].pos.y - g_aBullet[nCntBullet].fRHeight <= pPlayer->pos.y + pPlayer->Size.y &&
					g_aBullet[nCntBullet].pos.z + g_aBullet[nCntBullet].fRWidth >= pPlayer->pos.z - pPlayer->Size.z / 2 &&
					g_aBullet[nCntBullet].pos.z - g_aBullet[nCntBullet].fRWidth <= pPlayer->pos.z + pPlayer->Size.z / 2)
				{
					DamageBullet(1);

					g_aBullet[nCntBullet].bUse = false;
				}
				break;

			case BULLETTYPE_RANDUM:
				if (g_aBullet[nCntBullet].pos.x + g_aBullet[nCntBullet].fRWidth >= pPlayer->pos.x - pPlayer->Size.x / 2 &&
					g_aBullet[nCntBullet].pos.x - g_aBullet[nCntBullet].fRWidth <= pPlayer->pos.x + pPlayer->Size.x / 2 &&
					g_aBullet[nCntBullet].pos.y + g_aBullet[nCntBullet].fRHeight >= pPlayer->pos.y &&
					g_aBullet[nCntBullet].pos.y - g_aBullet[nCntBullet].fRHeight <= pPlayer->pos.y + pPlayer->Size.y &&
					g_aBullet[nCntBullet].pos.z + g_aBullet[nCntBullet].fRWidth >= pPlayer->pos.z - pPlayer->Size.z / 2 &&
					g_aBullet[nCntBullet].pos.z - g_aBullet[nCntBullet].fRWidth <= pPlayer->pos.z + pPlayer->Size.z / 2)
				{
					
					DamageBullet(1);

					g_aBullet[nCntBullet].bUse = false;

				}

				if (g_aBullet[nCntBullet].pos.y <= 0.0f)
				{
					g_aBullet[nCntBullet].bUse = false;
				}
				break;

			case BULLETTYPE_CURVE:

			

				if (g_aBullet[nCntBullet].nNumEnemy == 0)
				{
					g_aBullet[nCntBullet].rot.y += 0.0045f;
				}
				else
				{
					g_aBullet[nCntBullet].rot.y -= 0.0045f;
				}
				g_aBullet[nCntBullet].move = D3DXVECTOR3(sinf(-D3DX_PI * g_aBullet[nCntBullet].rot.y) * g_aBullet[nCntBullet].rot.x, 0.0f, cosf(-D3DX_PI * g_aBullet[nCntBullet].rot.y) * g_aBullet[nCntBullet].rot.x);

				if (g_aBullet[nCntBullet].pos.x + g_aBullet[nCntBullet].fRWidth >= pPlayer->pos.x - pPlayer->Size.x / 2 &&
					g_aBullet[nCntBullet].pos.x - g_aBullet[nCntBullet].fRWidth <= pPlayer->pos.x + pPlayer->Size.x / 2 &&
					g_aBullet[nCntBullet].pos.y + g_aBullet[nCntBullet].fRHeight >= pPlayer->pos.y &&
					g_aBullet[nCntBullet].pos.y - g_aBullet[nCntBullet].fRHeight <= pPlayer->pos.y + pPlayer->Size.y &&
					g_aBullet[nCntBullet].pos.z + g_aBullet[nCntBullet].fRWidth >= pPlayer->pos.z - pPlayer->Size.z / 2 &&
					g_aBullet[nCntBullet].pos.z - g_aBullet[nCntBullet].fRWidth <= pPlayer->pos.z + pPlayer->Size.z / 2)
				{

					DamageBullet(5);

					g_aBullet[nCntBullet].bUse = false;

				}

				break;

			case BULLETTYPE_WALL:
				if (g_nDelayBullet % 60 == 0)
				{
					SetBullet(BULLETTYPE_ENEMY,
						60, 15.0f, 15.0f, g_aBullet[nCntBullet].pos,
						D3DXVECTOR3(sinf(g_aBullet[nCntBullet].rot.y + D3DX_PI / 2) * 3.0f, 0.0f, cosf(g_aBullet[nCntBullet].rot.y + D3DX_PI / 2) * 3.0f),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f),D3DXCOLOR(1.0f,1.0f,1.0f,1.0f), -1, 0.0f);

					SetBullet(BULLETTYPE_ENEMY,
						60, 15.0f, 15.0f, g_aBullet[nCntBullet].pos,
						D3DXVECTOR3(sinf(g_aBullet[nCntBullet].rot.y - D3DX_PI / 2) * 3.0f, 0.0f, cosf(g_aBullet[nCntBullet].rot.y - D3DX_PI / 2) * 3.0f),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f),D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), -1, 0.0f);
				}

				if (g_aBullet[nCntBullet].pos.x + g_aBullet[nCntBullet].fRWidth >= pPlayer->pos.x - pPlayer->Size.x / 2 &&
					g_aBullet[nCntBullet].pos.x - g_aBullet[nCntBullet].fRWidth <= pPlayer->pos.x + pPlayer->Size.x / 2 &&
					g_aBullet[nCntBullet].pos.y + g_aBullet[nCntBullet].fRHeight >= pPlayer->pos.y &&
					g_aBullet[nCntBullet].pos.y - g_aBullet[nCntBullet].fRHeight <= pPlayer->pos.y + pPlayer->Size.y &&
					g_aBullet[nCntBullet].pos.z + g_aBullet[nCntBullet].fRWidth >= pPlayer->pos.z - pPlayer->Size.z / 2 &&
					g_aBullet[nCntBullet].pos.z - g_aBullet[nCntBullet].fRWidth <= pPlayer->pos.z + pPlayer->Size.z / 2)
				{
					DamageBullet(1);

					g_aBullet[nCntBullet].bUse = false;
				}
				break;
			case BULLETTYPE_BOUND:

				g_aBullet[nCntBullet].nAnimationCnt++;

				g_aBullet[nCntBullet].move.y += -0.6f;
				if (g_aBullet[nCntBullet].pos.y - g_aBullet[nCntBullet].fRHeight <= 0.0f)
				{
					g_aBullet[nCntBullet].move.y = 12.0f;
				}
				if (g_aBullet[nCntBullet].pos.x + g_aBullet[nCntBullet].fRWidth >= pPlayer->pos.x - pPlayer->Size.x / 2 &&
					g_aBullet[nCntBullet].pos.x - g_aBullet[nCntBullet].fRWidth <= pPlayer->pos.x + pPlayer->Size.x / 2 &&
					g_aBullet[nCntBullet].pos.y + g_aBullet[nCntBullet].fRHeight >= pPlayer->pos.y &&
					g_aBullet[nCntBullet].pos.y - g_aBullet[nCntBullet].fRHeight <= pPlayer->pos.y + pPlayer->Size.y &&
					g_aBullet[nCntBullet].pos.z + g_aBullet[nCntBullet].fRWidth >= pPlayer->pos.z - pPlayer->Size.z / 2 &&
					g_aBullet[nCntBullet].pos.z - g_aBullet[nCntBullet].fRWidth <= pPlayer->pos.z + pPlayer->Size.z / 2)
				{
					DamageBullet(5);

					g_aBullet[nCntBullet].bUse = false;
				}

				if (g_aBullet[nCntBullet].nAnimationCnt % 5 == 0)
				{
					g_aBullet[nCntBullet].nAnimationPattern++;

					if (g_aBullet[nCntBullet].nAnimationPattern >= 2)
					{
						g_aBullet[nCntBullet].nAnimationPattern = 0;
					}
				}

				//テクスチャ座標の設定（初期設定）
				pVtx[0].tex = D3DXVECTOR2(0.0f + 0.5f * g_aBullet[nCntBullet].nAnimationPattern, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(0.5f + 0.5f * g_aBullet[nCntBullet].nAnimationPattern, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f + 0.5f * g_aBullet[nCntBullet].nAnimationPattern, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(0.5f + 0.5f * g_aBullet[nCntBullet].nAnimationPattern, 1.0f);
				break;
			case BULLETTYPE_RING:

				if (g_aBullet[nCntBullet].bOneFlag == true)
				{
					for (int nCnt = 0; nCnt < 6; nCnt++)
					{
						float fRot = (2.0f / 6) * nCnt;
						SetBullet(BULLETTYPE_CHAINRING, 1000, 10.0f, 10.0f, D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + sinf(-D3DX_PI * fRot) * 50.0f, g_aBullet[nCntBullet].pos.y + sinf(-D3DX_PI * fRot) * 50.0f, g_aBullet[nCntBullet].pos.z + cosf(-D3DX_PI * fRot) * 50.0f),
							D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, fRot, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), nCntBullet, 50.0f);
					}
					g_aBullet[nCntBullet].bOneFlag = false;
				}

				if (g_aBullet[nCntBullet].pos.x + g_aBullet[nCntBullet].fRWidth >= pPlayer->pos.x - pPlayer->Size.x / 2 &&
					g_aBullet[nCntBullet].pos.x - g_aBullet[nCntBullet].fRWidth <= pPlayer->pos.x + pPlayer->Size.x / 2 &&
					g_aBullet[nCntBullet].pos.y + g_aBullet[nCntBullet].fRHeight >= pPlayer->pos.y &&
					g_aBullet[nCntBullet].pos.y - g_aBullet[nCntBullet].fRHeight <= pPlayer->pos.y + pPlayer->Size.y &&
					g_aBullet[nCntBullet].pos.z + g_aBullet[nCntBullet].fRWidth >= pPlayer->pos.z - pPlayer->Size.z / 2 &&
					g_aBullet[nCntBullet].pos.z - g_aBullet[nCntBullet].fRWidth <= pPlayer->pos.z + pPlayer->Size.z / 2)
				{
					DamageBullet(5);

					g_aBullet[nCntBullet].bUse = false;
				}
				break;
			case BULLETTYPE_CHAINRING:


				g_aBullet[nCntBullet].fLength *= 1.02f;



				if (g_aBullet[g_aBullet[nCntBullet].nNumEnemy].nType == BULLETTYPE_RING)
				{
					g_aBullet[nCntBullet].pos = D3DXVECTOR3(g_aBullet[g_aBullet[nCntBullet].nNumEnemy].pos.x + sinf(-D3DX_PI * g_aBullet[nCntBullet].rot.y) * g_aBullet[nCntBullet].fLength,
						0.0f,
						g_aBullet[g_aBullet[nCntBullet].nNumEnemy].pos.z + cosf(-D3DX_PI * g_aBullet[nCntBullet].rot.y) * g_aBullet[nCntBullet].fLength);
				}

				if (g_aBullet[nCntBullet].pos.x + g_aBullet[nCntBullet].fRWidth >= pPlayer->pos.x - pPlayer->Size.x / 2 &&
					g_aBullet[nCntBullet].pos.x - g_aBullet[nCntBullet].fRWidth <= pPlayer->pos.x + pPlayer->Size.x / 2 &&
					g_aBullet[nCntBullet].pos.y + g_aBullet[nCntBullet].fRHeight >= pPlayer->pos.y &&
					g_aBullet[nCntBullet].pos.y - g_aBullet[nCntBullet].fRHeight <= pPlayer->pos.y + pPlayer->Size.y &&
					g_aBullet[nCntBullet].pos.z + g_aBullet[nCntBullet].fRWidth >= pPlayer->pos.z - pPlayer->Size.z / 2 &&
					g_aBullet[nCntBullet].pos.z - g_aBullet[nCntBullet].fRWidth <= pPlayer->pos.z + pPlayer->Size.z / 2)
				{
					DamageBullet(5);


					g_aBullet[nCntBullet].bUse = false;
				}

				if (g_aBullet[g_aBullet[nCntBullet].nNumEnemy].bUse == false)
				{
					g_aBullet[nCntBullet].bUse = false;
				}

				break;
			case BULLETTYPE_CHAOS:

				g_aBullet[nCntBullet].move.y += -0.2f;
				if (g_aBullet[nCntBullet].pos.x + g_aBullet[nCntBullet].fRWidth >= pPlayer->pos.x - pPlayer->Size.x / 2 &&
					g_aBullet[nCntBullet].pos.x - g_aBullet[nCntBullet].fRWidth <= pPlayer->pos.x + pPlayer->Size.x / 2 &&
					g_aBullet[nCntBullet].pos.y + g_aBullet[nCntBullet].fRHeight >= pPlayer->pos.y &&
					g_aBullet[nCntBullet].pos.y - g_aBullet[nCntBullet].fRHeight <= pPlayer->pos.y + pPlayer->Size.y &&
					g_aBullet[nCntBullet].pos.z + g_aBullet[nCntBullet].fRWidth >= pPlayer->pos.z - pPlayer->Size.z / 2 &&
					g_aBullet[nCntBullet].pos.z - g_aBullet[nCntBullet].fRWidth <= pPlayer->pos.z + pPlayer->Size.z / 2)
				{
					DamageBullet(1);


					g_aBullet[nCntBullet].bUse = false;
				}
				if (g_aBullet[nCntBullet].pos.y <= 0.0f)
				{
					g_aBullet[nCntBullet].bUse = false;
				}
				break;
			case BULLETTYPE_FIRE:
			
				if (g_nDelayBullet % 21 == 0)
				{
					PlaySound(SOUND_LABEL_SE_FIRE_000);
					SetAttackbillboard(ATTACKBILLBORAD02_FIRE, 180, 60.0f, 100.0f,D3DXVECTOR3(g_aBullet[nCntBullet].pos.x,g_aBullet[nCntBullet].pos.y + 50.0f,g_aBullet[nCntBullet].pos.z), NULL_VECTOR3, NULL_VECTOR3, 0, 0.0f);
				}

				break;
			case BULLETTYPE_WAVE:

				g_aBullet[nCntBullet].pos = pPlayer->pos;

				if (g_nDelayBullet % 10 == 0)
				{
					/*float fWaveExpansion = (1.0f - ((float)(g_aBullet[nCntBullet].nLife) / (float)(g_aBullet[nCntBullet].nMaxLife))) * 3.0f;
					float fWaveScale = (1.0f - ((float)(g_aBullet[nCntBullet].nLife) / (float)(g_aBullet[nCntBullet].nMaxLife))) * 100.0f;*/
					//SetWave(0,50, fWaveScale, fWaveScale, fWaveExpansion, g_aBullet[nCntBullet].pos, NULL_VECTOR3, g_aBullet[nCntBullet].rot, -1, -1);
				}

				SetEffect(0, 10, 20.0f, 20.0f, 2.0f,
					g_aBullet[nCntBullet].pos,
					D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 0.0f,0);

				if (g_aBullet[nCntBullet].bUse == false)
				{
					for (int nCnt = 0; nCnt < 3; nCnt++)
					{
						float fRandumRot = float(rand() % 200 + 1) / 100;
						float fRandumSpeed = float(rand() % 100 + 30) / 10;
						float fRandumMove = float(rand() % 21 - 10);

						SetParticle(0, 1000, 20.0f, 0.1f, g_aBullet[nCntBullet].pos,
							D3DXVECTOR3(sinf(-D3DX_PI * fRandumRot) * fRandumSpeed, fRandumMove, cosf(-D3DX_PI * fRandumRot) * fRandumSpeed),
							D3DXCOLOR(0.5f, 0.5f, 1.0f, 1.0f), 0.0f, PARTICLETEX_NORMAL,0);
					}
				}
				break;
			case BULLETTYPE_THUNDER:

				g_aBullet[nCntBullet].move.y += -0.6f;
				if (g_aBullet[nCntBullet].pos.y - g_aBullet[nCntBullet].fRHeight <= 0.0f)
				{
					g_aBullet[nCntBullet].move.y = 12.0f;
				}
				if (g_aBullet[nCntBullet].pos.x + g_aBullet[nCntBullet].fRWidth >= pPlayer->pos.x - pPlayer->Size.x / 2 &&
					g_aBullet[nCntBullet].pos.x - g_aBullet[nCntBullet].fRWidth <= pPlayer->pos.x + pPlayer->Size.x / 2 &&
					g_aBullet[nCntBullet].pos.y + g_aBullet[nCntBullet].fRHeight >= pPlayer->pos.y &&
					g_aBullet[nCntBullet].pos.y - g_aBullet[nCntBullet].fRHeight <= pPlayer->pos.y + pPlayer->Size.y &&
					g_aBullet[nCntBullet].pos.z + g_aBullet[nCntBullet].fRWidth >= pPlayer->pos.z - pPlayer->Size.z / 2 &&
					g_aBullet[nCntBullet].pos.z - g_aBullet[nCntBullet].fRWidth <= pPlayer->pos.z + pPlayer->Size.z / 2)
				{
					DamageBullet(1);

					g_aBullet[nCntBullet].bUse = false;
				}
				break;
			}



			g_aBullet[nCntBullet].nLife -= 1;





			g_aBullet[nCntBullet].pos += g_aBullet[nCntBullet].move;
			if (g_aBullet[nCntBullet].nLife <= 0)
			{
				g_aBullet[nCntBullet].bUse = false;//体力が０になったらfalseにする
			}


			SetPositionShadow(g_aBullet[nCntBullet].nIdxShadow, D3DXVECTOR3(g_aBullet[nCntBullet].pos.x, g_aBullet[nCntBullet].pos.y - g_aBullet[nCntBullet].fRHeight, g_aBullet[nCntBullet].pos.z));
			if (g_aBullet[nCntBullet].bUse == false)
			{
				KillShadow(g_aBullet[nCntBullet].nIdxShadow);
			}
		}

		pVtx += 4;//ポインタを進める
	}

	//頂点バッファをアンロックする
	g_pVtxBuffBullet->Unlock();
}



//================================
// 描画処理
//================================
void DrawBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{
				D3DXMATRIX mtxTrans;//計算用マトリックス

				D3DXMATRIX mtxView;//ビューマトリックス取得用

				//ワールドマトリックスの初期化
				D3DXMatrixIdentity(&g_mtxWorldBullet);

				//ビューマトリックスを取得
				pDevice->GetTransform(D3DTS_VIEW, &mtxView);

				//ポリゴンをカメラに対して正面に向ける
				D3DXMatrixInverse(&g_mtxWorldBullet, NULL, &mtxView);

				//逆行列を求める
				g_mtxWorldBullet._41 = 0.0f;
				g_mtxWorldBullet._42 = 0.0f;
				g_mtxWorldBullet._43 = 0.0f;

				//位置を反映
				D3DXMatrixTranslation(&mtxTrans, g_aBullet[nCntBullet].pos.x, g_aBullet[nCntBullet].pos.y, g_aBullet[nCntBullet].pos.z);
				D3DXMatrixMultiply(&g_mtxWorldBullet, &g_mtxWorldBullet, &mtxTrans);

				//===============================================================
				//描画の調整
				//==============================================================
				//ライトを無効にする
				pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

				////Zの比較方法変更。
				//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

				////Zバッファに書き込まない
				//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

				//アルファテストを有効
				pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
				pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
				pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

				//===============================================================

				//ワールドマトリックスの設定
				pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldBullet);


				//頂点バッファをデータストリームに設定
				pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_3D));//頂点バッファへのポインタと頂点情報の構造体のサイズ

				//頂点フォーマットの設定
				pDevice->SetFVF(FVF_VERTEX_3D);

				//テクスチャの設定
				pDevice->SetTexture(0, g_pTextureBullet[g_aBullet[nCntBullet].nType]);
		
				//ポリゴンの描画
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBullet * 4, 2);//0は、描画する最初の頂点インデックスで、2は、描画するプリミティブ

                //============================
				//描画の調整をもとに戻す
				//============================

				//ライトを有効に戻す
				pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

				////Zの比較方法変更。
				//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

				////Zバッファに書き込む
				//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

				//アルファテストを無効に戻す
				pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
			
		}
	}
}

//================================
// 設定処理
//================================
void SetBullet(int nType, int nLife, float fRWidth, float fRHeight, D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot, D3DXCOLOR col, int nNumEnemy, float fLength)
{
	VERTEX_3D* pVtx;
	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == false)
		{
			g_aBullet[nCntBullet].bUse = true;//使用しているかどうか
			g_aBullet[nCntBullet].pos = pos;//位置
			g_aBullet[nCntBullet].move = move;//移動量
			g_aBullet[nCntBullet].rot = rot;//向き
			g_aBullet[nCntBullet].nLife = nLife;//体力
			g_aBullet[nCntBullet].fRWidth = fRWidth;//幅（半径）
			g_aBullet[nCntBullet].fRHeight = fRHeight;//高さ（半径）
			g_aBullet[nCntBullet].nType = nType;//種類
			g_aBullet[nCntBullet].nNumEnemy = nNumEnemy;
			g_aBullet[nCntBullet].fLength = fLength;
			g_aBullet[nCntBullet].bOneFlag = true;//一度だけ発動させる処理用フラグ
			g_aBullet[nCntBullet].bChangeFlag = false;//値を上下させる時に使う
			g_aBullet[nCntBullet].nIdxShadow = SetShadow();
			g_aBullet[nCntBullet].nMaxLife = nLife;//最大体力

			//頂点座標の設定（初期位置）
			pVtx[0].pos = D3DXVECTOR3(-g_aBullet[nCntBullet].fRWidth, g_aBullet[nCntBullet].fRHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].fRWidth, g_aBullet[nCntBullet].fRHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aBullet[nCntBullet].fRWidth, -g_aBullet[nCntBullet].fRHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].fRWidth, -g_aBullet[nCntBullet].fRHeight, 0.0f);

			//テクスチャ座標の設定（初期設定）
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

			

			if (g_aBullet[nCntBullet].nType == BULLETTYPE_FIRE || g_aBullet[nCntBullet].nType == BULLETTYPE_WAVE)
			{
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
			}
			else
			{
				pVtx[0].col = col;
				pVtx[1].col = col;
				pVtx[2].col = col;
				pVtx[3].col = col;
			}

			g_aBullet[nCntBullet].Size = D3DXVECTOR3(g_aBullet[nCntBullet].fRWidth * 2, 0.0f, g_aBullet[nCntBullet].fRWidth * 2);//大きさ（サイズ = 半径 × 2） 


			break;
		}
		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffBullet->Unlock();
}

Bullet* GetBullet(void)//弾構造体の情報をゲットする
{
	return &g_aBullet[0];
}

//=============================================
//弾でダメージを与えられた時に呼び出す
//=============================================
void DamageBullet(int nSubBattery)
{
	Player* pPlayer = GetPlayer();

	if (pPlayer->State != PLAYERSTATE_HEALWALL && pPlayer->State != PLAYERSTATE_HEALGROUND && pPlayer->State != PLAYERSTATE_DODGE)
	{
		for (int nCntParticle = 0; nCntParticle < 20; nCntParticle++)
		{
			float fRot = float(rand() % 200 + 1) / 100;
			float fYmove = float(rand() % 200) / 10;
			float fSpeed = float(rand() % 200) / 10;

			SetParticle(PARTICLETYPE_GRAVITY, 240, 6.0f, 0.5f, pPlayer->pos,
				D3DXVECTOR3(sinf(-D3DX_PI * fRot) * fSpeed, fYmove, cosf(-D3DX_PI * fRot) * fSpeed),
				D3DXCOLOR(1.0f, 0.647f, 0.0f, 1.0f), 0.0f, PARTICLETEX_NORMAL, 0);
		}

		pPlayer->bWeakDamageReaction = true;
		SubBattery(nSubBattery);
	}
}


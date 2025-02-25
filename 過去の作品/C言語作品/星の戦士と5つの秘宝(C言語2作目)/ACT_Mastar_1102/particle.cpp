//========================================================
//
//８月３０日：エフェクトもう一回見直す[Particle.cpp]
//Author:ShinaTaiyo
//
//=========================================================
#include "main.h"
#include "bullet.h"
#include "input.h"
#include "explosion.h"
#include "enemy.h"
#include "player.h"
#include "health.h"
#include <time.h>
#include "particle.h"

//#define MAX_PARTICLE (20000)//エフェクトの最大数
#define NUM_PARTICLE (1)//エフェクトの種類

//パーティクル構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;//位置
	D3DXVECTOR3 move;//移動量
	D3DXCOLOR col;//色
	float fRadius;//半径（大きさ）
	int nLife;//寿命（表示時間）
	bool bUse;//使用しているかどうか
	int nScaleType;//エフェクトの大きさを変えるスピードを決める。
	float fRot;//パーティクルを動かす角度を決める
	float fSpeed;//パーティクルを動かすスピード（移動量）を決める。

}Particle;

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureParticle[NUM_PARTICLE] = {};//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffParticle = NULL;//弾の情報
Particle g_aParticle[MAX_PARTICLE];//エフェクトの情報

float g_fRandumRot;
float  g_fRandumSpeed;

//エフェクトの初期化処理
void InitParticle(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int	nCntParticle;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み（弾のテクスチャを読み込む）
	D3DXCreateTextureFromFile(pDevice,
		"data\\effect000.jpg",
		&g_pTextureParticle[0]);

	//エフェクト情報の初期化
	for (nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		g_aParticle[nCntParticle].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aParticle[nCntParticle].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		g_aParticle[nCntParticle].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aParticle[nCntParticle].nLife = 0;
		g_aParticle[nCntParticle].bUse = false;//使用していない状態にする
		g_aParticle[nCntParticle].fRadius = 0.0f;
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_PARTICLE,//４は必要な頂点数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffParticle,
		NULL);

	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		//頂点座標の設定(エフェクトの大きさ）
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(20.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 20.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(20.0f, 20.0f, 0.0f);

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
	g_pVtxBuffParticle->Unlock();
}

//エフェクトの終了処理
void UninitParticle(void)
{
	//テクスチャの破棄
	int nCnt;
	for (nCnt = 0; nCnt < NUM_PARTICLE; nCnt++)
	{
		if (g_pTextureParticle[nCnt] != NULL)
		{
			g_pTextureParticle[nCnt]->Release();
			g_pTextureParticle[nCnt] = NULL;
		}
	}
	//頂点バッファの破棄
	if (g_pVtxBuffParticle != NULL)
	{
		g_pVtxBuffParticle->Release();
		g_pVtxBuffParticle = NULL;
	}
}

//エフェクトの更新処理
void UpdateParticle(void)
{




	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D* pVtx;

	//デバイスの取得
	pDevice = GetDevice();
	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);

	int nCntParticle;
	for (nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].bUse == true)
		{//エフェクトが使用される
			//頂点カラーの設定
			g_aParticle[nCntParticle].fRadius -= 1.0f;
			g_aParticle[nCntParticle].nLife--;

		

	/*		g_aParticle[nCntParticle].move.y = cosf(g_fRandumRot) * g_fRandumSpeed;
			g_aParticle[nCntParticle].move.x = sinf(g_fRandumRot) * g_fRandumSpeed;*/

			g_aParticle[nCntParticle].pos.y += g_aParticle[nCntParticle].move.y;
			g_aParticle[nCntParticle].pos.x += g_aParticle[nCntParticle].move.x;

			

		pVtx[0].pos = D3DXVECTOR3(g_aParticle[nCntParticle].pos.x - g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].pos.y - g_aParticle[nCntParticle].fRadius, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aParticle[nCntParticle].pos.x + g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].pos.y - g_aParticle[nCntParticle].fRadius, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aParticle[nCntParticle].pos.x - g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].pos.y + g_aParticle[nCntParticle].fRadius, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aParticle[nCntParticle].pos.x + g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].pos.y + g_aParticle[nCntParticle].fRadius, 0.0f);


			pVtx[0].col = g_aParticle[nCntParticle].col;
			pVtx[1].col = g_aParticle[nCntParticle].col;
			pVtx[2].col = g_aParticle[nCntParticle].col;
			pVtx[3].col = g_aParticle[nCntParticle].col;


		
			/*if (g_aParticle[nCntParticle].nScaleType == ParticleSCALE_NORMAL)
			{
				g_aParticle[nCntParticle].fRadius -= 0.5f;
				pVtx[0].pos = D3DXVECTOR3(g_aParticle[nCntParticle].pos.x - g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].pos.y - g_aParticle[nCntParticle].fRadius, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aParticle[nCntParticle].pos.x + g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].pos.y - g_aParticle[nCntParticle].fRadius, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aParticle[nCntParticle].pos.x - g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].pos.y + g_aParticle[nCntParticle].fRadius, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aParticle[nCntParticle].pos.x + g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].pos.y + g_aParticle[nCntParticle].fRadius, 0.0f);
			}
			else if (g_aParticle[nCntParticle].nScaleType == ParticleSCALE_FAST)
			{
				g_aParticle[nCntParticle].fRadius -= 3.0f;
				pVtx[0].pos = D3DXVECTOR3(g_aParticle[nCntParticle].pos.x - g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].pos.y - g_aParticle[nCntParticle].fRadius, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aParticle[nCntParticle].pos.x + g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].pos.y - g_aParticle[nCntParticle].fRadius, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aParticle[nCntParticle].pos.x - g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].pos.y + g_aParticle[nCntParticle].fRadius, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aParticle[nCntParticle].pos.x + g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].pos.y + g_aParticle[nCntParticle].fRadius, 0.0f);
			}*/



			if (g_aParticle[nCntParticle].nLife <= 0)
			{//パーティクルの寿命が尽きた
				g_aParticle[nCntParticle].bUse = false;
			}
			if (g_aParticle[nCntParticle].fRadius <= 0.0f)
			{
				g_aParticle[nCntParticle].bUse = false;
			}


		}
		pVtx += 4;//ポインタを次の値へ
	}
	//頂点バッファをアンロックする
	g_pVtxBuffParticle->Unlock();
}

//弾の描画処理
void DrawParticle(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	int nCntParticle;

	Enemy* pEnemy = GetEnemy();
	Player* pPlayer = GetPlayer();

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffParticle, 0, sizeof(VERTEX_2D));//頂点バッファへのポインタと頂点情報の構造体のサイズ

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//aブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//テクスチャを全てのポリゴンに＜＜割り当てる＞＞
	for (nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].bUse == true)
		{

			pDevice->SetTexture(0, g_pTextureParticle[0]);

			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,//0は、描画する最初の頂点インデックスで、2は、描画するプリミティブ数
				nCntParticle * 4,
				2);
		}

	}
	//aブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//エフェクトの設定処理
void SetParticle(D3DXVECTOR3 pos, D3DXCOLOR col, float fRadius, int nLife)
{
	int nCntParticle;

	VERTEX_2D* pVtx;
	g_pVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].bUse == false)
		{//弾が使用されていない
		/*	if (BULLETTYPE_ENEMY || BULLETTYPE_PLAYER || BULLETTYPE_SKILL1||BULLETTYPE_REAFY)
			{*/

			g_aParticle[nCntParticle].pos = pos;
			/*g_aParticle[nCntParticle].nScaleType = nType;*/
			g_aParticle[nCntParticle].col = col;
			g_aParticle[nCntParticle].fRadius = fRadius;
			g_aParticle[nCntParticle].nLife = nLife;
			g_aParticle[nCntParticle].bUse = true;//使用している状態にする
		
			float fRandumRot;
			float fRandumSpeed;
			fRandumRot = float((rand)() % 629 - 314) / 100.0f;
			fRandumSpeed = float((rand)() % 100) / 5.0f;

			 g_aParticle[nCntParticle].fRot = fRandumRot;
			 g_aParticle[nCntParticle].fSpeed = fRandumSpeed;

			g_aParticle[nCntParticle].move.x = sinf(g_aParticle[nCntParticle].fRot) * fRandumSpeed;
			g_aParticle[nCntParticle].move.y = cosf(g_aParticle[nCntParticle].fRot) * fRandumSpeed;
			
						//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aParticle[nCntParticle].pos.x - g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].pos.y - g_aParticle[nCntParticle].fRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aParticle[nCntParticle].pos.x + g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].pos.y - g_aParticle[nCntParticle].fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aParticle[nCntParticle].pos.x - g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].pos.y + g_aParticle[nCntParticle].fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aParticle[nCntParticle].pos.x + g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].pos.y + g_aParticle[nCntParticle].fRadius, 0.0f);

			pVtx[0].col = g_aParticle[nCntParticle].col;//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
			pVtx[1].col = g_aParticle[nCntParticle].col;//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
			pVtx[2].col = g_aParticle[nCntParticle].col;//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
			pVtx[3].col = g_aParticle[nCntParticle].col;//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
			break;

		}
		pVtx += 4;//ポインタを４つ分進める
	}
	g_pVtxBuffParticle->Unlock();
}
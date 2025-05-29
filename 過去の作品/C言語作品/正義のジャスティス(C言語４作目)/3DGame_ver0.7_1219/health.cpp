//========================================================
//
//１０月１０日：エフェクトもう一回見直す[Health.cpp]
//Author:ShinaTaiyo
//
//=========================================================

#include "main.h"
#include "Health.h"
#include "input.h"
#include "player.h"
#include "health.h"
#include "result.h"
#include "damage.h"
#include "game.h"

#define MAX_HEALTH (4) //体力の最大桁数
#define NUM_HEALTH (1) //体力表示の種類
#define HEALTH_RADIUS (10.0f) //体力表示の数値の半径（大きさ）

#define HEALTH_WIDTH (15.0f) //体力の横幅
#define HEALTH_HEIGHT (30.0f)//体力の高さ

#define HEALTH_UI_RADIUS_X (100.0f)//体力UIの横幅
#define HEALTH_UI_RADIUS_Y (50.0f)//体力UIの高さ

//弾構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;//位置
	bool bUse;//体力表示をしようしているかどうか
}Health;

//グローバル変数

//================================================================================
//体力のUI処理
//================================================================================
LPDIRECT3DTEXTURE9 g_pTextureUIHealth = NULL;//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUIHealth = NULL;
D3DXVECTOR3 g_posUIHealth;//位置
D3DXVECTOR3 g_moveUIHealth;//移動量

//=======================================================================

//体力表示処理============================================================
LPDIRECT3DTEXTURE9 g_pTextureHealth = NULL;//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffHealth = NULL;
Health g_aHealth[MAX_HEALTH];
D3DXVECTOR3 g_posHealth;//位置
D3DXVECTOR3 g_moveHealth;//移動量

int g_nHealth = 0;


int g_aPosHealthTexU[8];//各桁の数字を格納

int g_nSaveHealth = 0;


int g_nMaxHealth;//最大体力保存用
//=========================================================================
//体力の初期化処理
void InitHealth(void)
{
	ResultType* pResultType = GetResultType();
	g_nHealth = 500;//体力の初期値
	LPDIRECT3DDEVICE9 pDevice;
	int	nCntHealth;

	//デバイスの取得
	pDevice = GetDevice();


	//=======================================
	//体力UI処理
	//=======================================


	g_posUIHealth = D3DXVECTOR3(150.0f, 50.0f, 0.0f);//UI表示場所

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\HP_Display_ver1.0.png",
		&g_pTextureUIHealth);
	//テクスチャの読み込み（スコアのテクスチャを読み込む）

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,//４は必要な頂点数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffUIHealth,
		NULL);

	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffUIHealth->Lock(0, 0, (void**)&pVtx, 0);

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_posUIHealth.x - HEALTH_UI_RADIUS_X,g_posUIHealth.y - HEALTH_UI_RADIUS_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_posUIHealth.x + HEALTH_UI_RADIUS_X,g_posUIHealth.y - HEALTH_UI_RADIUS_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_posUIHealth.x - HEALTH_UI_RADIUS_X,g_posUIHealth.y + HEALTH_UI_RADIUS_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_posUIHealth.x + HEALTH_UI_RADIUS_X,g_posUIHealth.y + HEALTH_UI_RADIUS_Y, 0.0f);

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

		//テクスチャの設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);
	//頂点バッファをアンロックする
	g_pVtxBuffUIHealth->Unlock();
	//===============================================================================================================================

	//=================================
	//体力表示処理
	//=================================
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\number_001.png",
		&g_pTextureHealth);
	//テクスチャの読み込み（スコアのテクスチャを読み込む）
	for (nCntHealth = 0; nCntHealth < MAX_HEALTH; nCntHealth++)
	{

		g_aPosHealthTexU[nCntHealth] = 0;
	}
	//スコア情報の初期化
	for (nCntHealth = 0; nCntHealth < MAX_HEALTH; nCntHealth++)
	{
		g_aHealth[nCntHealth].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 - 400.0f - ((nCntHealth + 1) * 30.0f), 20.0f, 0.0f);
		g_aHealth[nCntHealth].bUse = false;//使用していない状態にする
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_HEALTH,//４は必要な頂点数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffHealth,
		NULL);

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffHealth->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntHealth = 0; nCntHealth < MAX_HEALTH; nCntHealth++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aHealth[nCntHealth].pos.x - HEALTH_WIDTH, g_aHealth[nCntHealth].pos.y - HEALTH_HEIGHT, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aHealth[nCntHealth].pos.x + HEALTH_WIDTH, g_aHealth[nCntHealth].pos.y - HEALTH_HEIGHT, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aHealth[nCntHealth].pos.x - HEALTH_WIDTH, g_aHealth[nCntHealth].pos.y + HEALTH_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aHealth[nCntHealth].pos.x + HEALTH_WIDTH, g_aHealth[nCntHealth].pos.y + HEALTH_HEIGHT, 0.0f);

		g_aHealth[nCntHealth].bUse = true;//使用している状態にする

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

		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);
		pVtx += 4;//頂点データのポインタを４つ分進める。
	}
	//頂点バッファをアンロックする
	g_pVtxBuffHealth->Unlock();

	//====================================================================================================================
	
	
}

//体力の終了処理
void UninitHealth(void)
{
	int nCntHealth;

	//===================================
	//体力UIの終了処理
	//===================================
	
	//テクスチャの破棄
	if (g_pTextureUIHealth != NULL)
	{
		g_pTextureUIHealth->Release();
		g_pTextureUIHealth = NULL;
	}
	//頂点バッファの破棄
	if (g_pVtxBuffUIHealth != NULL)
	{
		g_pVtxBuffUIHealth->Release();
		g_pVtxBuffUIHealth = NULL;
	}
	//=======================================================================

	//===================================
	//体力表示の終了処理
	//===================================

	//テクスチャの破棄
	if (g_pTextureHealth != NULL)
	{
		g_pTextureHealth->Release();
		g_pTextureHealth = NULL;
	}
	//頂点バッファの破棄
	if (g_pVtxBuffHealth != NULL)
	{
		g_pVtxBuffHealth->Release();
		g_pVtxBuffHealth = NULL;
	}
	//========================================================================
}

//弾の更新処理
void UpdateHealth(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntHealth;
	VERTEX_2D* pVtx;
	Player* pPlayer = GetPlayer();

	//======================================
	//体力UIの更新処理
	//======================================
	
	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffUIHealth->Lock(0, 0, (void**)&pVtx, 0);

	if (g_nHealth >= 9999)
	{
		g_nHealth = 9999;
	}

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_posUIHealth.x - HEALTH_UI_RADIUS_X, g_posUIHealth.y - HEALTH_UI_RADIUS_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_posUIHealth.x + HEALTH_UI_RADIUS_X, g_posUIHealth.y - HEALTH_UI_RADIUS_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_posUIHealth.x - HEALTH_UI_RADIUS_X, g_posUIHealth.y + HEALTH_UI_RADIUS_Y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_posUIHealth.x + HEALTH_UI_RADIUS_X, g_posUIHealth.y + HEALTH_UI_RADIUS_Y, 0.0f);

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

	//テクスチャの設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	//頂点バッファをアンロックする
	g_pVtxBuffUIHealth->Unlock();
	//====================================================================================================================



	//=================================================
	//体力表示処理
	//=================================================
	int nGetHealth;

	nGetHealth = pPlayer->nHp;//スコアを保存する

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffHealth->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntHealth = 0; nCntHealth < MAX_HEALTH; nCntHealth++)
	{



		////頂点カラーの設定
		//if (pPlayer->pos.x >= g_aHealth[nCntHealth].pos.x - HEALTH_WIDTH && pPlayer->pos.x <= g_aHealth[nCntHealth].pos.x + HEALTH_WIDTH
		//	&& pPlayer->pos.y >= g_aHealth[nCntHealth].pos.y - HEALTH_HEIGHT && pPlayer->pos.y <= g_aHealth[nCntHealth].pos.y + HEALTH_HEIGHT)
		//{
		//	//頂点カラーの設定
		//	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f);
		//	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f);
		//	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f);
		//	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f);
		//}
			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		
		//数値の表示処理
		g_aPosHealthTexU[nCntHealth] = nGetHealth % 10;
		nGetHealth = nGetHealth / 10;

		pVtx[0].tex = D3DXVECTOR2(0.0f + (0.1f * g_aPosHealthTexU[nCntHealth]), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + (0.1f * g_aPosHealthTexU[nCntHealth]), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + (0.1f * g_aPosHealthTexU[nCntHealth]), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + (0.1f * g_aPosHealthTexU[nCntHealth]), 1.0f);
		

		pVtx += 4;//頂点データのポインタを４つ分進める。
	}

	//頂点バッファをアンロックする
	g_pVtxBuffHealth->Unlock();
	//==========================================================================================================================================
}

//弾の描画処理
void DrawHealth(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ


	//===============================================
	//体力UI描画処理
	//===============================================
	
	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffUIHealth, 0, sizeof(VERTEX_2D));//頂点バッファへのポインタと頂点情報の構造体のサイズ

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureUIHealth);

		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,//0は、描画する最初の頂点インデックスで、2は、描画するプリミティブ数
			0,
			2);

	//=============================================
	//体力表示の描画処理
	//=============================================
	int nCntHealth;

	////デバイスの取得
	//pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffHealth, 0, sizeof(VERTEX_2D));//頂点バッファへのポインタと頂点情報の構造体のサイズ

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);



	for (nCntHealth = 0; nCntHealth < MAX_HEALTH; nCntHealth++)
	{
			//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureHealth);

		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,//0は、描画する最初の頂点インデックスで、2は、描画するプリミティブ数
			nCntHealth * 4,
			2);
		
	}
	//==============================================================================================================================================

}

//=====================================
//体力の加算減算処理
//=====================================
void SetHealing(int nHeal)
{
	g_nHealth += nHeal;
	
}

void DamageHealth(int nValue)
{
	SetHealth(nValue);
	Player* pPlayer = GetPlayer();
	float fRandumRot;
	float fRandumSpeed;
	fRandumRot = float((rand)() % 629 - 314) / 100.0f;
	fRandumSpeed = float((rand)() % 30) / 10.0f;
	SetDamage(nValue, 100,pPlayer->pos, 20.0f,D3DXCOLOR(1.0f,0.0f,0.0f,1.0f));
}

//体力の設定処理
void SetHealth(int nDamage)
{

	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D* pVtx;
	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffHealth->Lock(0, 0, (void**)&pVtx, 0);

	g_nHealth -= nDamage;
	Player* pPlayer = GetPlayer();

	int nGetHealth = g_nHealth;
	int nCntHealth;
	bool bGetWaveFlag;

	if (g_nHealth <= 0)
	{
		SetGameState(GAMESTATE_END);
	}




	//頂点バッファをアンロックする
	g_pVtxBuffHealth->Unlock();
}


//===================================
//体力情報の取得
//===================================
int GetHealth(void)
{
	return g_nHealth;
}
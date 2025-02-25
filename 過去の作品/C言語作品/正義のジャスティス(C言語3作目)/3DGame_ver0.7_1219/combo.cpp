//========================================================
//
//１０月１０日：エフェクトもう一回見直す[Combo.cpp]
//Author:ShinaTaiyo
//
//=========================================================

#include "main.h"
#include "Combo.h"
#include "input.h"
#include "player.h"
#include "Combo.h"
#include "result.h"
#include "damage.h"
#include "game.h"

#define MAX_COMBO (4) //体力の最大桁数
#define NUM_COMBO (1) //体力表示の種類
#define COMBO_RADIUS (10.0f) //体力表示の数値の半径（大きさ）

#define COMBO_WIDTH (20.0f) //体力の横幅
#define COMBO_HEIGHT (40.0f)//体力の高さ

#define COMBO_UI_RADIUS_X (60.0f)//体力UIの横幅
#define COMBO_UI_RADIUS_Y (30.0f)//体力UIの高さ

#define COMBOGAGE_WIDTH (500.0f)
#define COMBOGAGE_HEIGHT (20.0f)



//弾構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;//位置
	bool bUse;//体力表示をしようしているかどうか
}Combo;

//グローバル変数

//================================================================================
//コンボ文字表示処理
//================================================================================
LPDIRECT3DTEXTURE9 g_pTextureUICombo = NULL;//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUICombo = NULL;
D3DXVECTOR3 g_posUICombo;//位置
D3DXVECTOR3 g_moveUICombo;//移動量

//=======================================================================

//体力表示処理============================================================
LPDIRECT3DTEXTURE9 g_pTextureCombo = NULL;//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffCombo = NULL;
Combo g_aCombo[MAX_COMBO];
D3DXVECTOR3 g_posCombo;//位置
D3DXVECTOR3 g_moveCombo;//移動量

int g_nCombo = 0;


int g_aPosComboTexU[8];//各桁の数字を格納

int g_nSaveCombo = 0;


int g_nMaxCombo;//最大体力保存用


//========================================
//コンボゲージ表記処理
//========================================
LPDIRECT3DTEXTURE9 g_pTextureComboGage = NULL;//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffComboGage = NULL;
D3DXVECTOR3 g_posComboGage;//位置
D3DXVECTOR3 g_moveComboGage;//移動量

float g_fComboGage;//コンボゲージの表記



//=========================================================================
//体力の初期化処理
void InitCombo(void)
{
	ResultType* pResultType = GetResultType();
	g_nCombo = 0;//体力の初期値
	LPDIRECT3DDEVICE9 pDevice;
	int	nCntCombo;

	//デバイスの取得
	pDevice = GetDevice();


	//=======================================
	//コンボUI処理
	//=======================================


	g_posUICombo = D3DXVECTOR3(SCREEN_WIDTH - 80.0f, 70.0f, 0.0f);//UI表示場所

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Combo.png",
		&g_pTextureUICombo);
	//テクスチャの読み込み（スコアのテクスチャを読み込む）

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,//４は必要な頂点数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffUICombo,
		NULL);

	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffUICombo->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_posUICombo.x - COMBO_UI_RADIUS_X, g_posUICombo.y - COMBO_UI_RADIUS_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_posUICombo.x + COMBO_UI_RADIUS_X, g_posUICombo.y - COMBO_UI_RADIUS_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_posUICombo.x - COMBO_UI_RADIUS_X, g_posUICombo.y + COMBO_UI_RADIUS_Y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_posUICombo.x + COMBO_UI_RADIUS_X, g_posUICombo.y + COMBO_UI_RADIUS_Y, 0.0f);

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
	g_pVtxBuffUICombo->Unlock();
	//===============================================================================================================================

	//=================================
	//体力表示処理
	//=================================
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\ComboNum.png",
		&g_pTextureCombo);
	//テクスチャの読み込み（スコアのテクスチャを読み込む）
	for (nCntCombo = 0; nCntCombo < MAX_COMBO; nCntCombo++)
	{

		g_aPosComboTexU[nCntCombo] = 0;
	}
	//スコア情報の初期化
	for (nCntCombo = 0; nCntCombo < MAX_COMBO; nCntCombo++)
	{
		g_aCombo[nCntCombo].pos = D3DXVECTOR3(SCREEN_WIDTH - 120.0f - ((nCntCombo + 1) * 30.0f), 70.0f, 0.0f);
		g_aCombo[nCntCombo].bUse = false;//使用していない状態にする
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_COMBO,//４は必要な頂点数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffCombo,
		NULL);

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffCombo->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntCombo = 0; nCntCombo < MAX_COMBO; nCntCombo++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aCombo[nCntCombo].pos.x - COMBO_WIDTH, g_aCombo[nCntCombo].pos.y - COMBO_HEIGHT, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aCombo[nCntCombo].pos.x + COMBO_WIDTH, g_aCombo[nCntCombo].pos.y - COMBO_HEIGHT, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aCombo[nCntCombo].pos.x - COMBO_WIDTH, g_aCombo[nCntCombo].pos.y + COMBO_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aCombo[nCntCombo].pos.x + COMBO_WIDTH, g_aCombo[nCntCombo].pos.y + COMBO_HEIGHT, 0.0f);

		g_aCombo[nCntCombo].bUse = true;//使用している状態にする

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
	g_pVtxBuffCombo->Unlock();

	//====================================================================================================================

	//==============================
	//コンボゲージの初期化
	//==============================

	g_fComboGage = 300;//コンボゲージの値の初期化

	g_posComboGage = D3DXVECTOR3(SCREEN_WIDTH,120.0f, 0.0f);//コンボゲージの位置

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Skill_Gage.png",
		&g_pTextureComboGage);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,//４は必要な頂点数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffComboGage,
		NULL);

	g_pVtxBuffComboGage->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	pVtx[3].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);

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
	g_pVtxBuffComboGage->Unlock();





}

//体力の終了処理
void UninitCombo(void)
{
	int nCntCombo;

	//===================================
	//体力UIの終了処理
	//===================================

	//テクスチャの破棄
	if (g_pTextureUICombo != NULL)
	{
		g_pTextureUICombo->Release();
		g_pTextureUICombo = NULL;
	}
	//頂点バッファの破棄
	if (g_pVtxBuffUICombo != NULL)
	{
		g_pVtxBuffUICombo->Release();
		g_pVtxBuffUICombo = NULL;
	}
	//=======================================================================

	//===================================
	//体力表示の終了処理
	//===================================

	//テクスチャの破棄
	if (g_pTextureCombo != NULL)
	{
		g_pTextureCombo->Release();
		g_pTextureCombo = NULL;
	}
	//頂点バッファの破棄
	if (g_pVtxBuffCombo != NULL)
	{
		g_pVtxBuffCombo->Release();
		g_pVtxBuffCombo = NULL;
	}
	//========================================================================

	//===================================
	//コンボゲージ表示の終了処理
	//===================================

	//テクスチャの破棄
	if (g_pTextureComboGage != NULL)
	{
		g_pTextureComboGage->Release();
		g_pTextureComboGage = NULL;
	}
	//頂点バッファの破棄
	if (g_pTextureComboGage != NULL)
	{
		g_pTextureComboGage->Release();
		g_pTextureComboGage = NULL;
	}
	//========================================================================
}

//弾の更新処理
void UpdateCombo(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntCombo;
	VERTEX_2D* pVtx;
	Player* pPlayer = GetPlayer();

	//======================================
	//体力UIの更新処理
	//======================================

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffUICombo->Lock(0, 0, (void**)&pVtx, 0);

	if (g_nCombo >= 9999)
	{
		g_nCombo = 9999;
	}

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_posUICombo.x - COMBO_UI_RADIUS_X, g_posUICombo.y - COMBO_UI_RADIUS_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_posUICombo.x + COMBO_UI_RADIUS_X, g_posUICombo.y - COMBO_UI_RADIUS_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_posUICombo.x - COMBO_UI_RADIUS_X, g_posUICombo.y + COMBO_UI_RADIUS_Y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_posUICombo.x + COMBO_UI_RADIUS_X, g_posUICombo.y + COMBO_UI_RADIUS_Y, 0.0f);

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
	g_pVtxBuffUICombo->Unlock();
	//====================================================================================================================



	//=================================================
	//体力表示処理
	//=================================================
	int nGetCombo;

	nGetCombo = g_nCombo;//コンボ数を保存する

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffCombo->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntCombo = 0; nCntCombo < MAX_COMBO; nCntCombo++)
	{
		//数値の表示処理
		g_aPosComboTexU[nCntCombo] = nGetCombo % 10;
		nGetCombo = nGetCombo / 10;

		pVtx[0].tex = D3DXVECTOR2(0.0f + (0.1f * g_aPosComboTexU[nCntCombo]), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + (0.1f * g_aPosComboTexU[nCntCombo]), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + (0.1f * g_aPosComboTexU[nCntCombo]), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + (0.1f * g_aPosComboTexU[nCntCombo]), 1.0f);

		pVtx += 4;//頂点データのポインタを４つ分進める。
	}

	//頂点バッファをアンロックする
	g_pVtxBuffCombo->Unlock();
	//==========================================================================================================================================

	//=================================================
	//コンボゲージ表示処理
	//=================================================
	

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffComboGage->Lock(0, 0, (void**)&pVtx, 0);

	g_fComboGage -= 1.0f;//コンボゲージを減らし続ける


	if (g_fComboGage < 0.0f)
	{
		g_fComboGage = 0.0f;
		g_nCombo = 0;//コンボ数をリセット
	}

	//頂点の設定（コンボゲージを表す）
	pVtx[0].pos = D3DXVECTOR3(g_posComboGage.x - 300.0f, g_posComboGage.y - COMBOGAGE_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_posComboGage.x - 300.0f + g_fComboGage, g_posComboGage.y - COMBOGAGE_HEIGHT,0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_posComboGage.x - 300.0f, g_posComboGage.y + COMBOGAGE_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_posComboGage.x - 300.0f + g_fComboGage, g_posComboGage.y + COMBOGAGE_HEIGHT, 0.0f);
	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(0.0f,1.0f,0.0f,1.0f);
	pVtx[1].col = D3DXCOLOR(0.0f,1.0f,0.0f,1.0f);
	pVtx[2].col = D3DXCOLOR(0.0f,1.0f,0.0f,1.0f);
	pVtx[3].col = D3DXCOLOR(0.0f,1.0f,0.0f,1.0f);



	//頂点バッファをアンロックする
		g_pVtxBuffComboGage->Unlock();
	//==========================================================================================================================================
}

//弾の描画処理
void DrawCombo(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ


	if (g_nCombo >= 1)
	{
		//===============================================
		//コンボの文字描画処理
		//===============================================

		//デバイスの取得
		pDevice = GetDevice();

		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffUICombo, 0, sizeof(VERTEX_2D));//頂点バッファへのポインタと頂点情報の構造体のサイズ

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureUICombo);

		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,//0は、描画する最初の頂点インデックスで、2は、描画するプリミティブ数
			0,
			2);

		//=============================================
		//コンボ数表示の描画処理
		//=============================================
		int nCntCombo;

		////デバイスの取得
		//pDevice = GetDevice();

		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffCombo, 0, sizeof(VERTEX_2D));//頂点バッファへのポインタと頂点情報の構造体のサイズ

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		int nDigit = 0;

		//コンボ表示の桁数の表記を調整する処理
		if (g_nCombo < 10)
		{
			nDigit = 3;
		}
		else if (g_nCombo < 100)
		{
			nDigit = 2;
		}
		else if (g_nCombo < 1000)
		{
			nDigit = 1;
		}
		//==========================

		for (nCntCombo = 0; nCntCombo < MAX_COMBO - nDigit; nCntCombo++)
		{

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureCombo);

			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,//0は、描画する最初の頂点インデックスで、2は、描画するプリミティブ数
				nCntCombo * 4,
				2);

		}

		//=============================================
		//コンボゲージ表示の描画処理
		//=============================================
			////デバイスの取得
			//pDevice = GetDevice();

			//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffComboGage, 0, sizeof(VERTEX_2D));//頂点バッファへのポインタと頂点情報の構造体のサイズ

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureComboGage);

		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,//0は、描画する最初の頂点インデックスで、2は、描画するプリミティブ数
			0,
			2);

		//==============================================================================================================================================
	}
}
//体力の設定処理
void AddCombo(int nCombo)
{

	g_fComboGage = 300.0f;//コンボしたので、コンボゲージの減少をリセットする。

	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D* pVtx;
	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffCombo->Lock(0, 0, (void**)&pVtx, 0);

	g_nCombo += nCombo;
	Player* pPlayer = GetPlayer();

	int nGetCombo = g_nCombo;
	int nCntCombo;
	bool bGetWaveFlag;

	//頂点バッファをアンロックする
	g_pVtxBuffCombo->Unlock();
}


//===================================
//体力情報の取得
//===================================
int GetCombo(void)
{
	return g_nCombo;
}

//==========================
//コンボ数リセット処理
//==========================
void ResetCombo(void)
{
	g_nCombo = 0;
}
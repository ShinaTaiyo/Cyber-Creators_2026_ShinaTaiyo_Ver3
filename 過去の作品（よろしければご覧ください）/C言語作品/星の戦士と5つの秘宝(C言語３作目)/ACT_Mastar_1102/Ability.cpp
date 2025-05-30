//========================================================
//
//８月３０日：エフェクトもう一回見直す[Ability.cpp]
//Author:ShinaTaiyo
//
//=========================================================

#include "main.h"
#include "Ability.h"
#include "input.h"
#include "player.h"
#include "Ability.h"
#include "result.h"
#include "sound.h"

#define MAX_ABILITY (3) //アビリティの最大桁数
#define NUM_ABILITY (2) //アビリティ表示の種類

#define ABILITY_WIDTH (15.0f) //アビリティの横幅
#define ABILITY_HEIGHT (30.0f)//アビリティの高さ

#define ABILITY_RADIUS (40.0f) //アビリティ表示の数値の半径（大きさ）

#define ABILITY_UI_RADIUS (40.0f)//アビリティUIの半径
#define ABILITY_UI_RADIUS_X (100.0f)//アビリティUIの横幅
#define ABILITY_UI_RADIUS_Y (50.0f)//アビリティUIの高さ



//グローバル変数

//================================================================================
//アビリティのUI処理
//================================================================================
LPDIRECT3DTEXTURE9 g_pTextureUIAbility = NULL;//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUIAbility = NULL;
D3DXVECTOR3 g_posUIAbility;//位置
D3DXVECTOR3 g_moveUIAbility;//移動量

//=======================================================================

//アビリティ表示処理============================================================
LPDIRECT3DTEXTURE9 g_pTextureAbility[NUM_ABILITY] = {};//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffAbility = NULL;
Ability g_aAbility;
D3DXVECTOR3 g_posAbility;//位置
D3DXVECTOR3 g_moveAbility;//移動量

int g_nAbility = 0;
float g_fHESLTHSCALE = 0;

int g_nSaveAbility = 0;


int g_nMaxAbility;//最大アビリティ保存用
//=========================================================================
//アビリティの初期化処理
void InitAbility(void)
{
	ResultType* pResultType = GetResultType();
	g_nAbility = 100;//リザルトモード以外なら、スコアを初期化する
	LPDIRECT3DDEVICE9 pDevice;
	int	nCntAbility;

	//デバイスの取得
	pDevice = GetDevice();


	//=======================================
	//アビリティUI処理
	//=======================================


	g_posUIAbility = D3DXVECTOR3(300.0f, 50.0f, 0.0f);//UI表示場所

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Ability_DisPlay_ver1.0.png",
		&g_pTextureUIAbility);
	//テクスチャの読み込み（スコアのテクスチャを読み込む）

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,//４は必要な頂点数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffUIAbility,
		NULL);

	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffUIAbility->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_posUIAbility.x - ABILITY_UI_RADIUS, g_posUIAbility.y - ABILITY_UI_RADIUS, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_posUIAbility.x + ABILITY_UI_RADIUS, g_posUIAbility.y - ABILITY_UI_RADIUS, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_posUIAbility.x - ABILITY_UI_RADIUS, g_posUIAbility.y + ABILITY_UI_RADIUS, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_posUIAbility.x + ABILITY_UI_RADIUS, g_posUIAbility.y + ABILITY_UI_RADIUS, 0.0f);

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
	g_pVtxBuffUIAbility->Unlock();
	//===============================================================================================================================

	//=================================
	//アビリティ表示処理
	//=================================
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\NormalAbility.png",
		&g_pTextureAbility[ABILITYTYPE_NORMAL]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Ability_Bullet.png",
		&g_pTextureAbility[ABILITYTYPE_BULLET]);

	    //スコア情報の初期化
	    g_aAbility.nAbilityType = 0;
		g_aAbility.pos = D3DXVECTOR3(300.0f,50.0f,0.0f);
		g_aAbility.bUse = false;//使用していない状態にする

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,//４は必要な頂点数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffAbility,
		NULL);

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffAbility->Lock(0, 0, (void**)&pVtx, 0);

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aAbility.pos.x - ABILITY_RADIUS, g_aAbility.pos.y - ABILITY_RADIUS, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aAbility.pos.x + ABILITY_RADIUS, g_aAbility.pos.y - ABILITY_RADIUS, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aAbility.pos.x - ABILITY_RADIUS, g_aAbility.pos.y + ABILITY_RADIUS, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aAbility.pos.x + ABILITY_RADIUS, g_aAbility.pos.y + ABILITY_RADIUS, 0.0f);

		g_aAbility.bUse = true;//使用している状態にする

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
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	
	//頂点バッファをアンロックする
	g_pVtxBuffAbility->Unlock();

	//====================================================================================================================


}

//アビリティの終了処理
void UninitAbility(void)
{
	int nCntAbility;

	//===================================
	//アビリティUIの終了処理
	//===================================

	//テクスチャの破棄
	if (g_pTextureUIAbility != NULL)
	{
		g_pTextureUIAbility->Release();
		g_pTextureUIAbility = NULL;
	}
	//頂点バッファの破棄
	if (g_pVtxBuffUIAbility != NULL)
	{
		g_pVtxBuffUIAbility->Release();
		g_pVtxBuffUIAbility = NULL;
	}
	//=======================================================================

	//===================================
	//アビリティ表示の終了処理
	//===================================

	//テクスチャの破棄
	for (int nCntAbility = 0; nCntAbility < NUM_ABILITY; nCntAbility++)
	{
		if (g_pTextureAbility[nCntAbility] != NULL)
		{
			g_pTextureAbility[nCntAbility]->Release();
			g_pTextureAbility[nCntAbility] = NULL;
		}
	}
	//頂点バッファの破棄
	if (g_pVtxBuffAbility != NULL)
	{
		g_pVtxBuffAbility->Release();
		g_pVtxBuffAbility = NULL;
	}
	//========================================================================
}

//アビリティの更新処理
void UpdateAbility(void)
{
	int nCntAbility;
	VERTEX_2D* pVtx;
	Player* pPlayer = GetPlayer();

	//======================================
	//アビリティUIの更新処理
	//======================================

	

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffUIAbility->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_posUIAbility.x - ABILITY_UI_RADIUS, g_posUIAbility.y - ABILITY_UI_RADIUS, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_posUIAbility.x + ABILITY_UI_RADIUS, g_posUIAbility.y - ABILITY_UI_RADIUS, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_posUIAbility.x - ABILITY_UI_RADIUS, g_posUIAbility.y + ABILITY_UI_RADIUS, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_posUIAbility.x + ABILITY_UI_RADIUS, g_posUIAbility.y + ABILITY_UI_RADIUS, 0.0f);

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
	g_pVtxBuffUIAbility->Unlock();
	//====================================================================================================================



	//=================================================
	//アビリティ表示処理
	//=================================================
	//int nGetAbility;

	if (GetKeyboardTrigger(DIK_K) == true || GetJoyPadTrigger(JOYKEY_R1) == true || GetJoyPadTrigger(JOYKEY_L1) == true)
	{
		PlaySound(SOUND_LABEL_SE_SELECT_000);
		g_aAbility.nAbilityType++;
	}
	/*if (g_aAbility.nAbilityType == 1)
	{
		g_aAbility.nAbilityType = 0;
	}*/
	if (g_aAbility.nAbilityType == 2)
	{
		g_aAbility.nAbilityType = 0;
	}

	//nGetAbility = g_nAbility;//スコアを保存する

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffAbility->Lock(0, 0, (void**)&pVtx, 0);

		//頂点カラーの設定
		if (pPlayer->pos.x >= g_aAbility.pos.x - ABILITY_RADIUS && pPlayer->pos.x <= g_aAbility.pos.x + ABILITY_RADIUS
			&& pPlayer->pos.y >= g_aAbility.pos.y - ABILITY_RADIUS && pPlayer->pos.y <= g_aAbility.pos.y + ABILITY_RADIUS)
		{
			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f);
		}
		else
		{
			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}


		pVtx += 4;//頂点データのポインタを４つ分進める。
	

	//頂点バッファをアンロックする
	g_pVtxBuffAbility->Unlock();
	//==========================================================================================================================================
}

//アビリティの描画処理
void DrawAbility(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ


	//===============================================
	//アビリティUI描画処理
	//===============================================

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffUIAbility, 0, sizeof(VERTEX_2D));//頂点バッファへのポインタと頂点情報の構造体のサイズ

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureUIAbility);

	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,//0は、描画する最初の頂点インデックスで、2は、描画するプリミティブ数
		0,
		2);

	//=============================================
	//アビリティ表示の描画処理
	//=============================================
	

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffAbility, 0, sizeof(VERTEX_2D));//頂点バッファへのポインタと頂点情報の構造体のサイズ

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	if (g_aAbility.nAbilityType == ABILITYTYPE_NORMAL)
	{
		pDevice->SetTexture(0, g_pTextureAbility[ABILITYTYPE_NORMAL]);
	}
	else if (g_aAbility.nAbilityType == ABILITYTYPE_BULLET)
	{
		pDevice->SetTexture(0, g_pTextureAbility[ABILITYTYPE_BULLET]);
	}
		

		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,//0は、描画する最初の頂点インデックスで、2は、描画するプリミティブ数
			0,
			2);

	
	//==============================================================================================================================================

}

//アビリティの設定処理
void SetAbility(ABILITYTYPE type)
{

	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D* pVtx;
	
	g_aAbility.nAbilityType = type;//アビリティを設定する
}

//===================================
//アビリティ情報の取得
//===================================
Ability * GetAbility(void)
{
	return &g_aAbility;
}
//========================================================
//
//８月３０日：エフェクトもう一回見直す[health.cpp]
//Author:ShinaTaiyo
//
//=========================================================
#include "main.h"
#include "health.h"
#include "game.h"
#include "enemy.h"
#include "result.h"
#include "sound.h"
#include "staging.h"
#include "input.h"
#include "player.h"

#define HEALTH_WIDTH (100.0f)
#define HEALTH_HEIGHT (50.0f)

//グローバル変数
LPDIRECT3DTEXTURE9 g_apTextureHealth = NULL;//テクスチャ3枚分のポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffHealth = NULL;//頂点バッファへのポインタ

float g_fAnimationCount=0;
bool g_bResultFlag2;
bool g_bWarning;//残りHPが1以下になると警告を出すフラグ。
Health g_aHealth;
Result_2 g_aResult_2;
//============================
//体力の初期化処理
//============================
void InitHealth(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntBG;

	g_bWarning = false;

	g_aResult_2.bResultFlag2 = false;

	g_bResultFlag2 = false;

	g_aHealth.fHealth = -2;

	g_aHealth.pos = D3DXVECTOR3(1100.0f, 670.0f, 0.0f);

	//デバイスの取得
	pDevice = GetDevice();

	D3DXCreateTextureFromFile(pDevice,
		"data\\re_player_health.png",
		&g_apTextureHealth);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,//４は必要な頂点数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffHealth,
		NULL);

	VERTEX_2D* pVtx;
	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffHealth->Lock(0, 0, (void**)&pVtx, 0);
		pVtx[0].pos = D3DXVECTOR3(g_aHealth.pos.x-HEALTH_WIDTH,g_aHealth.pos.y-HEALTH_HEIGHT, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aHealth.pos.x+HEALTH_WIDTH,g_aHealth.pos.y-HEALTH_HEIGHT, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aHealth.pos.x-HEALTH_WIDTH,g_aHealth.pos.y+HEALTH_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aHealth.pos.x+HEALTH_WIDTH,g_aHealth.pos.y+HEALTH_HEIGHT, 0.0f);

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
		pVtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f,0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f,1.0f);

		pVtx += 4;//頂点データのポインタを４つ分進める。

	//頂点バッファをアンロックする
	g_pVtxBuffHealth->Unlock();
}

//==========================
//体力の終了処理
//==========================
void UninitHealth(void)
{
	//テクスチャの破棄

		if (g_apTextureHealth!= NULL)
		{
			g_apTextureHealth->Release();
			g_apTextureHealth= NULL;
		}
	
	//頂点バッファの破棄
	if (g_pVtxBuffHealth != NULL)
	{
		g_pVtxBuffHealth->Release();
		g_pVtxBuffHealth = NULL;
	}
}

//==================================
//体力の更新処理
//==================================
void UpdateHealth(float fHealthDamage)
{

	ResultType* pResultType = GetResultType();
	Player* pPlayer = GetPlayer();


	int nCntBG;



	VERTEX_2D* pVtx;
	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffHealth->Lock(0, 0, (void**)&pVtx, 0);


	if (GetKeyboardTrigger(DIK_Z) == true)
	{
		PlaySound(SOUND_LABEL_SE_DAMAGE_000);
		g_aHealth.fHealth += 1.0f;
	}

	if (g_aHealth.fHealth == 2.0f&&g_bWarning==true)
	{
		g_bWarning = false;
		PlaySound(SOUND_LABEL_SE_WARNING_000);
		SetStaging(STAGINGTYPE_WARNING, -1, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}
	else if (g_aHealth.fHealth <= 1.0f)
	{
		g_bWarning = true;
	}

	/*for (nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
	{*/
		//テクスチャ座標の設定

		/*g_aPosTexV[nCntBG] -= 0.0025f;*/
	//g_aHealth.fHealth += fHealthDamage;//プレイヤーがダメージを受けたとき、体力を減らす。(ダメージ処理はplayer.cppでやる）

		pVtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f-((1.0f/3.0f)*g_aHealth.fHealth),0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f - ((1.0f / 3.0f) * g_aHealth.fHealth),1.0f);

		/*pVtx += 4;*/
		
	
			if (g_aHealth.fHealth >= 3.0f)//３回以上プレイヤーが被弾した場合
			{
				g_aResult_2.bResultFlag2 = true;

				pResultType->nResultType = RESULTTYPE_CLEAR;//ゲームオーバーようのリザルト画面を呼ぶ。

				StopSound(SOUND_LABEL_BGM001);

				SetGameState(GAMESTATE_END);
			}

			if (pPlayer->pos.x >= g_aHealth.pos.x - HEALTH_WIDTH && pPlayer->pos.x <= g_aHealth.pos.x + HEALTH_WIDTH
				&& pPlayer->pos.y >= g_aHealth.pos.y - HEALTH_HEIGHT && pPlayer->pos.y <= g_aHealth.pos.y + HEALTH_HEIGHT)
			{
				//頂点カラーの設定
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
			}
			else
			{

				//頂点カラーの設定
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
			}
		
	/*}*/
	//頂点バッファをアンロックする
	g_pVtxBuffHealth->Unlock();
}

//==================================
//体力の描画処理
//==================================
void DrawHealth(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntBG;
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffHealth, 0, sizeof(VERTEX_2D));//頂点バッファへのポインタと頂点情報の構造体のサイズ

		//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

		//テクスチャの設定
		pDevice->SetTexture(0, g_apTextureHealth);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
			0,
			2);
	
}

//======================================
//リザルト画面を呼び出すフラグ
//======================================
Result_2* GetResult_2(void)
{
	return &g_aResult_2;
}

Health* GetHealth(void)
{
	return &g_aHealth;
}
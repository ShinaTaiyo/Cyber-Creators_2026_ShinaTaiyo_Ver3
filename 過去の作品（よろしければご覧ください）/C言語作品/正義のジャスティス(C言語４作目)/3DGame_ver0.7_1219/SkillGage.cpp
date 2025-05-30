//========================================================
//
//９月１１日：プレイヤーがスキルゲージの中に入ったときに、薄くする処理[SkillGage.cpp]
//Author:ShinaTaiyo
//
//=========================================================
#include "main.h"
#include "game.h"
#include "enemy.h"
#include "SkillGage.h"
#include "SkillName.h"
#include "input.h"
#include "player.h"

//マクロ定義
#define NUM_SKILLGAGE (2)
#define MAX_SKILLGAGE (2)
#define SKILLGAGE_HEIGHT (30.0f)
#define SKILLGAGE_WIDTH (500.0f)
//グローバル変数
LPDIRECT3DTEXTURE9 g_apTextureSkillGage[NUM_SKILLGAGE] = {};//テクスチャ3枚分のポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSkillGage = NULL;//頂点バッファへのポインタ

SkillGage g_aSkillGage[MAX_SKILLGAGE];

//float g_fSkillGage = 0;
//int g_nSkillGage_Count = 0;
//Result_2 g_aResult_2;
//============================
//体力の初期化処理
//============================
void InitSkillGage(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntSkillGage;

	/*g_nCountBoss1 = 0;*/


	

	/*g_aResult_2.bResultFlag2 = false;*/

	//g_bResultFlag2 = false;

	/* g_fSkillGage = 0;
	 g_nSkillGage_Count = 0;*/

	for (nCntSkillGage = 0; nCntSkillGage < MAX_SKILLGAGE; nCntSkillGage++)
	{
		g_aSkillGage[nCntSkillGage].nSkillGageCount = 0;
		g_aSkillGage[nCntSkillGage].fSkillGage = 0;
		g_aSkillGage[nCntSkillGage].bUse = false;
		g_aSkillGage[nCntSkillGage].bBrinkingFlag= false;
		g_aSkillGage[nCntSkillGage].fBrinkingPoint = 1.0f;
		g_aSkillGage[nCntSkillGage].fBoss1MaxHp = 0.0f;
		g_aSkillGage[nCntSkillGage].nCountBoss1 = 0;
		g_aSkillGage[nCntSkillGage].pos = D3DXVECTOR3(500.0f, 690.0f, 0.0f);
	}



	/*g_aSkillGage[0].bUse = true;
	g_aSkillGage[0].nType = SkillGageTYPE_SKILL;*/
	/*g_fAnimationPattern = 0;*/

	//デバイスの取得
	pDevice = GetDevice();


	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Skill_Gage.png",
		&g_apTextureSkillGage[0]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Skill_Energy_000.png",
		&g_apTextureSkillGage[1]);

	/*D3DXCreateTextureFromFile(pDevice,
		"data\\BossHp_000.png",
		&g_apTextureSkillGage[NUM_SKILLGAGE][0]);*/

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4*MAX_SKILLGAGE,//４は必要な頂点数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffSkillGage,
		NULL);

	VERTEX_2D* pVtx;
	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffSkillGage->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntSkillGage = 0; nCntSkillGage < MAX_SKILLGAGE; nCntSkillGage++)
	{
		pVtx[0].pos = D3DXVECTOR3(0.0f, 670.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 670.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);

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
	g_pVtxBuffSkillGage->Unlock();
}

//==========================
//体力の終了処理
//==========================
void UninitSkillGage(void)
{
	//テクスチャの破棄
	int nCntSkillGage;

	for (nCntSkillGage = 0; nCntSkillGage < NUM_SKILLGAGE; nCntSkillGage++)
	{
		if (g_apTextureSkillGage[nCntSkillGage] != NULL)
		{
			g_apTextureSkillGage[nCntSkillGage]->Release();
			g_apTextureSkillGage[nCntSkillGage] = NULL;
		}
	}
	//頂点バッファの破棄
	if (g_pVtxBuffSkillGage != NULL)
	{
		g_pVtxBuffSkillGage->Release();
		g_pVtxBuffSkillGage = NULL;
	}
}

//==================================
//体力の更新処理
//==================================
void UpdateSkillGage(void)
{
	int nCntSkillGage;
	int nCntEnemy;
	Player* pPlayer = GetPlayer();//プレイヤー構造体の情報をゲットする。
	Skill* pSkill = GetSkill();

	//g_aSkillGage[0].nSkillGageCount++;
	VERTEX_2D* pVtx;
	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffSkillGage->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntSkillGage = 0; nCntSkillGage < MAX_SKILLGAGE; nCntSkillGage++)
	{
		if (nCntSkillGage == 0)
		{//ゲージ表記処理（配列[0]）



			//g_aSkillGage[nCntSkillGage].nSkillGageCount++;


			/*	if (g_aSkillGage[nCntSkillGage].nType == SkillGageTYPE_SKILL)
				{*/
			//if (g_aSkillGage[nCntSkillGage].nSkillGageCount % 2 == 0)
			//{
			//	g_aSkillGage[nCntSkillGage].fSkillGage++;//スキルゲージアップ！
			//}

			if (g_aSkillGage[nCntSkillGage].fSkillGage >= 1000.0f)
			{
				g_aSkillGage[nCntSkillGage].fSkillGage = 1000.0f;
			}

			//	pVtx[0].pos = D3DXVECTOR3(0.0, 670.0f, 0.0f);
			//	pVtx[1].pos = D3DXVECTOR3(0.0f + g_aSkillGage.fSkillGage, 670.0f, 0.0f);
			//	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
			//	pVtx[3].pos = D3DXVECTOR3(0.0f + g_aSkillGage.fSkillGage, SCREEN_HEIGHT, 0.0f);
			////}

			pVtx[0].pos = D3DXVECTOR3(g_aSkillGage[nCntSkillGage].pos.x - 500.0f, g_aSkillGage[nCntSkillGage].pos.y - SKILLGAGE_HEIGHT, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(0.0f + g_aSkillGage[nCntSkillGage].fSkillGage, g_aSkillGage[nCntSkillGage].pos.y - SKILLGAGE_HEIGHT, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aSkillGage[nCntSkillGage].pos.x - 500.0f, g_aSkillGage[nCntSkillGage].pos.y + SKILLGAGE_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(0.0f + g_aSkillGage[nCntSkillGage].fSkillGage, g_aSkillGage[nCntSkillGage].pos.y + SKILLGAGE_HEIGHT, 0.0f);



			if (pPlayer->pos.x >= g_aSkillGage[nCntSkillGage].pos.x - 500.0f && pPlayer->pos.x <= g_aSkillGage[nCntSkillGage].pos.x + 500.0f
				&& pPlayer->pos.y >= g_aSkillGage[nCntSkillGage].pos.y - 20.0f && pPlayer->pos.y <= g_aSkillGage[nCntSkillGage].pos.y + 20.0f)
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
		}
		else
		{//消費ゲージ表記処理（配列[1])

			if (pSkill->nSkillType == SKILLTYPE_FIRETORNADO)
			{//選んでいるスキルが「ファイアトルネード」の時
				g_aSkillGage[nCntSkillGage].fSkillGage = 50.0f;
			}
			else if (pSkill->nSkillType == SKILLTYPE_THUNDERBURST)
			{//選んでいるスキルが「サンダーバースト」の時
				g_aSkillGage[nCntSkillGage].fSkillGage = 100.0f;
			}
			else if (pSkill->nSkillType == SKILLTYPE_METEOBALL)
			{//選んでいるスキルが「メテオボール」の時
				g_aSkillGage[nCntSkillGage].fSkillGage = 200.0f;
			}
			else if (pSkill->nSkillType == SKILLTYPE_KILLNEEDLE)
			{//選んでいるスキルが「キルニードル」の時
				g_aSkillGage[nCntSkillGage].fSkillGage = 150.0f;
			}
			else if (pSkill->nSkillType == SKILLTYPE_CHAINFIRE)
			{//選んでいるスキルが「ヒール」の時
				g_aSkillGage[nCntSkillGage].fSkillGage = 300.0f;
			}
			else if (pSkill->nSkillType == SKILLTYPE_ATOMICSLASH)
			{//選んでいるスキルが「イージスシールド」の時
				g_aSkillGage[nCntSkillGage].fSkillGage = 1000.0f;
			}
			pVtx[0].pos = D3DXVECTOR3(g_aSkillGage[nCntSkillGage].pos.x - 500.0f, g_aSkillGage[nCntSkillGage].pos.y - SKILLGAGE_HEIGHT+17.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(0.0f + g_aSkillGage[nCntSkillGage].fSkillGage, g_aSkillGage[nCntSkillGage].pos.y - SKILLGAGE_HEIGHT+17.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aSkillGage[nCntSkillGage].pos.x - 500.0f, g_aSkillGage[nCntSkillGage].pos.y + SKILLGAGE_HEIGHT-17.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(0.0f + g_aSkillGage[nCntSkillGage].fSkillGage, g_aSkillGage[nCntSkillGage].pos.y + SKILLGAGE_HEIGHT-17.0f, 0.0f);



			if (g_aSkillGage[nCntSkillGage].bBrinkingFlag == false)
			{
				g_aSkillGage[nCntSkillGage].fBrinkingPoint-=0.02f;
			}
			if (g_aSkillGage[nCntSkillGage].bBrinkingFlag == true)
			{
				g_aSkillGage[nCntSkillGage].fBrinkingPoint+=0.02f;
			}

			if (g_aSkillGage[nCntSkillGage].fBrinkingPoint <= 0.0f)
			{
				g_aSkillGage[nCntSkillGage].bBrinkingFlag = true;
			}
			if (g_aSkillGage[nCntSkillGage].fBrinkingPoint >= 1.0f)
			{
				g_aSkillGage[nCntSkillGage].bBrinkingFlag =false;
			}
			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f,1.0f, g_aSkillGage[nCntSkillGage].fBrinkingPoint);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f,1.0f, g_aSkillGage[nCntSkillGage].fBrinkingPoint);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f,1.0f, g_aSkillGage[nCntSkillGage].fBrinkingPoint);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f,1.0f, g_aSkillGage[nCntSkillGage].fBrinkingPoint);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
		}
		pVtx += 4;
	}




	//頂点バッファをアンロックする
	g_pVtxBuffSkillGage->Unlock();
}

//==================================
//体力の描画処理
//==================================
void DrawSkillGage(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntSkillGage;
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffSkillGage, 0, sizeof(VERTEX_2D));//頂点バッファへのポインタと頂点情報の構造体のサイズ

		//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntSkillGage = 0; nCntSkillGage < MAX_SKILLGAGE; nCntSkillGage++)
	{
		pDevice->SetTexture(0, g_apTextureSkillGage[nCntSkillGage]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
			nCntSkillGage*4,
			2);
	}
	//}

}

void AddSkillGage(float fSkillPoint)//スキルゲージを増やす処理
{
	g_aSkillGage[0].fSkillGage += fSkillPoint;
}


SkillGage* GetSkillGage(void)
{
	return &g_aSkillGage[0];
}


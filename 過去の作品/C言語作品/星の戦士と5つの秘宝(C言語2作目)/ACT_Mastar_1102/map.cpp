//========================================================
//
//１０月１４日：マップ切り替え処理[Map.cpp]
//Author:ShinaTaiyo
//
//=========================================================
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "main.h"
#include "score.h"
#include "game.h"
#include "result.h"
#include "player.h"
#include "map.h"
#include "sound.h"
#include "fade.h"
#include "health.h"
#include "block.h"
#include "enemy.h"
#include "warp.h"
#include "input.h"
#include "Item.h"
//マクロ定義
#define NUM_MAPTEX (MAPTYPE_MAX)//ブロックの種類
#define FRAME_MAPSIZE (20.0f) //ブロックの大きさ

#define TEXTFILE_MAP0 "data\\TEXTFILE\\Action_Map0.txt"
#define TEXTFILE_RE_MAP0 "data\\TEXTFILE\\Re_ActionMap0.txt"
#define MAP1 "data\\TEXTFILE\\ActionMap1.txt"
#define MAP2 "data\\TEXTFILE\\Action_Map2.txt"
#define MAP3 "data\\TEXTFILE\\ActionMap3.txt"
#define MAP4 "data\\TEXTFILE\\ActionMap4.txt"
#define MAP5 "data\\TEXTFILE\\ActionMap5.txt"
#define MAP6 "data\\TEXTFILE\\ActionMap6.txt"
#define MAP7 "data\\TEXTFILE\\ActionMap7.txt"
#define MAP8 "data\\TEXTFILE\\ActionMap8.txt"
#define MAP9 "data\\TEXTFILE\\ActionMap9.txt"
#define MAP10 "data\\TEXTFILE\\ActionMap10.txt"
#define MAP11 "data\\TEXTFILE\\ActionMap11.txt"
#define MAP12 "data\\TEXTFILE\\ActionMap12.txt"
#define MAP13 "data\\TEXTFILE\\ActionMap13.txt"
#define MAP14 "data\\TEXTFILE\\ActionMap14.txt"
#define MAP15 "data\\TEXTFILE\\ActionMap15.txt"
#define MAP16 "data\\TEXTFILE\\ActionMap16.txt"
#define MAP17 "data\\TEXTFILE\\ActionMap17.txt"
#define MAP18 "data\\TEXTFILE\\ActionMap18.txt"
#define MAP19 "data\\TEXTFILE\\ActionMap19.txt"
#define MAP20 "data\\TEXTFILE\\ActionMap20.txt"
#define MAP21 "data\\TEXTFILE\\ActionMap21.txt"
#define MAP22 "data\\TEXTFILE\\ActionMap22.txt"
#define MAP23 "data\\TEXTFILE\\ActionMap23.txt"
#define MAP24 "data\\TEXTFILE\\ActionMap24.txt"
#define MAP25 "data\\TEXTFILE\\ActionMap25.txt"
#define MAP26 "data\\TEXTFILE\\ActionMap26.txt"
#define MAP27 "data\\TEXTFILE\\ActionMap27.txt"
#define MAP28 "data\\TEXTFILE\\ActionMap28.txt"
#define MAP29 "data\\TEXTFILE\\ActionMap29.txt"
#define MAP30 "data\\TEXTFILE\\ActionMap30.txt"
#define MAP31 "data\\TEXTFILE\\ActionMap31.txt"
#define MAP32 "data\\TEXTFILE\\ActionMap32.txt"
#define MAP33 "data\\TEXTFILE\\ActionMap33.txt"
#define MAP34 "data\\TEXTFILE\\ActionMap34.txt"
#define MAP35 "data\\TEXTFILE\\ActionMap35.txt"
#define MAP36 "data\\TEXTFILE\\ActionMap36.txt"
#define MAP37 "data\\TEXTFILE\\ActionMap37.txt"
#define MAP38 "data\\TEXTFILE\\ActionMap38.txt"
#define MAP39 "data\\TEXTFILE\\ActionMap39.txt"
#define MAP40 "data\\TEXTFILE\\ActionMap40.txt"
#define MAP41 "data\\TEXTFILE\\ActionMap41.txt"













int g_nUseMap = 0;//現在使用しているマップ
int g_nChallengeStage;//どのステージにチャレンジしたか


#define MAX_MAPHEIGHT (18) //1マップにおいてのブロックの最大の高さ
#define MAX_MAPWIDTH (32) //１マップにおいてのブロックの最大の横幅

Map_h_DefeatBoss g_aDefeatBoss;

//グローバル変数
//bool g_bIsLanding;
LPDIRECT3DTEXTURE9 g_apTextureMap = {};
//D3DXVECTOR3 g_rotPI;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMAP = NULL;
//Enemy g_MAP[MAX_ENEMY];//敵の情報
//Result g_aResult;
int g_nMapBlockData[MAX_MAPHEIGHT][MAX_MAPWIDTH];//マップ情報を読み取る

//int g_nCountTime = 0;//時間を表すグローバル変数

void InitMap(void)
{
//==================================
//各エリアのボスを倒したかどうか
//==================================
	g_aDefeatBoss.bDefeatBoss1 = false;
	g_aDefeatBoss.bDefeatBoss2 = false;
	g_aDefeatBoss.bDefeatBoss3 = false;
	g_aDefeatBoss.bDefeatBoss4 = false;
	//==============================================

	g_nUseMap = 0;
	g_nChallengeStage = 0;

}


void SetMap(int nType)
{
	g_nUseMap = nType;//呼び出したマップ番号を保存

	int nCntMap;
	int nCntWidth;
	int nCntHeight;
	Block* pBlock = GetBlock();
	Enemy* pEnemy = GetEnemy();
	Warp* pWarp = GetWarp();
	Item* pItem = GetItem();
	VERTEX_2D* pVtx;
	FILE* pFile;//ファイルポインタを宣言

	//InitBlock();//ブロックを全て初期化して消す
	//InitEnemy();//敵を全て初期化して消す
	//InitWarp();//ワープを全て初期化して消す

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++, pBlock++)
	{
		if (pBlock->bUse == true)
		{
			pBlock->bUse = false;
		}
	}

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == true)
		{
			pEnemy->bUse = false;
		}
	}

	for (int nCntItem = 0; nCntItem < MAX_ENEMY; nCntItem++, pItem++)
	{
		if (pItem->bUse == true)
		{
			pItem->bUse = false;
		}
	}

	pFile = fopen(TEXTFILE_RE_MAP0, "r");

	fclose(pFile);
    
	switch (nType)
	{
	//========================================
    //セントラルサークル
	//========================================
	case MAPTYPE_8:
		pFile = fopen(MAP8, "r");
		if (g_aDefeatBoss.bDefeatBoss1 == false)
		{
			SetWarp(MAPTYPE_0, D3DXVECTOR3(220.0f, 540.0f, 0.0f), D3DXVECTOR3(120.0f, 620.0f, 0.0f));
		}
		if (g_aDefeatBoss.bDefeatBoss2 == false)
		{
			SetWarp(MAPTYPE_6, D3DXVECTOR3(1060.0f, 540.0f, 0.0f), D3DXVECTOR3(1200.0f, 620.0f, 0.0f));
		}
		if (g_aDefeatBoss.bDefeatBoss3 == false)
		{
			SetWarp(MAPTYPE_14, D3DXVECTOR3(220.0f, 140.0f, 0.0f), D3DXVECTOR3(1220.0f, 620.0f, 0.0f));
		}
		if (g_aDefeatBoss.bDefeatBoss4 == false)
		{
			SetWarp(MAPTYPE_23, D3DXVECTOR3(1060.0f, 140.0f, 0.0f), D3DXVECTOR3(100.0f, 620.0f, 0.0f));
		}

		PlaySound(SOUND_LABEL_BGM011);

		if (g_aDefeatBoss.bDefeatBoss1 == true &&
			g_aDefeatBoss.bDefeatBoss2 == true &&
			g_aDefeatBoss.bDefeatBoss3 == true &&
			g_aDefeatBoss.bDefeatBoss4 == true)
		{
			SetWarp(MAPTYPE_32, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 120.0f, 0.0f), D3DXVECTOR3(140.0f, 620.0f, 0.0f));
		}
		break;
	//================================================================================================
	
	//===============================
	//ステージ１
	//===============================
	case MAPTYPE_0:
		g_nChallengeStage = CHALLENGESTAGE_1;
		StopSound(SOUND_LABEL_BGM011);
		PlaySound(SOUND_LABEL_BGM006);
		pFile = fopen(TEXTFILE_RE_MAP0, "r");
		SetWarp(MAPTYPE_1, D3DXVECTOR3(1200.0f, 80.0f, 0.0f), D3DXVECTOR3(1200.0f, 620.0f, 0.0f));
		break;
	case MAPTYPE_1:
		pFile = fopen(MAP1, "r");
		SetWarp(MAPTYPE_2, D3DXVECTOR3(720.0f, 220.0f, 0.0f), D3DXVECTOR3(1200.0f, 620.0f, 0.0f));
		break;
	case MAPTYPE_2:
		pFile = fopen(MAP2, "r");
		SetWarp(MAPTYPE_3, D3DXVECTOR3(1200.0f, 100.0f, 0.0f), D3DXVECTOR3(1200.0f, 100.0f, 0.0f));
		break;
	case MAPTYPE_3:
		pFile = fopen(MAP3, "r");
		SetWarp(MAPTYPE_4, D3DXVECTOR3(100.0f, 100.0f, 0.0f), D3DXVECTOR3(500.0f, 400.0f, 0.0f));
		break;
	case MAPTYPE_4:
		pFile = fopen(MAP4, "r");
		SetWarp(MAPTYPE_5, D3DXVECTOR3(100.0f, 100.0f, 0.0f), D3DXVECTOR3(1000.0f, 100.0f, 0.0f));
		SetItem(ITEMTYPE_HEAL, D3DXVECTOR3(180.0f, 60.0f, 0.0f), 1000);
		break;
	case MAPTYPE_5:
		pFile = fopen(MAP5, "r");
		break;
		//=============================================================================================

		//======================
		//ステージ２
		//======================
	case MAPTYPE_6:
		g_nChallengeStage = CHALLENGESTAGE_2;
		StopSound(SOUND_LABEL_BGM011);
		PlaySound(SOUND_LABEL_BGM007);
		pFile = fopen(MAP6, "r");
		SetWarp(MAPTYPE_7, D3DXVECTOR3(100.0f, 60.0f, 0.0f), D3DXVECTOR3(1220.0f, 660.0f, 0.0f));
		break;
	case MAPTYPE_7:
		pFile = fopen(MAP7, "r");
		SetWarp(MAPTYPE_9, D3DXVECTOR3(100.0f, 50.0f, 0.0f), D3DXVECTOR3(580.0f, 220.0f, 0.0f));
		break;
	case MAPTYPE_9:
		pFile = fopen(MAP9, "r");
		SetWarp(MAPTYPE_10, D3DXVECTOR3(900.0f,380.0f, 0.0f), D3DXVECTOR3(1220.0f, 60.0f, 0.0f));
		break;
	case MAPTYPE_10:
		pFile = fopen(MAP10, "r");
		SetWarp(MAPTYPE_11, D3DXVECTOR3(540.0f,300.0f, 0.0f), D3DXVECTOR3(1140.0f, 620.0f, 0.0f));
		break;
	case MAPTYPE_11:
		pFile = fopen(MAP11, "r");
		SetWarp(MAPTYPE_12, D3DXVECTOR3(1220.0f,100.0f, 0.0f), D3DXVECTOR3(1220.0f, 600.0f, 0.0f));
		break;
	case MAPTYPE_12:
		pFile = fopen(MAP12, "r");
		SetWarp(MAPTYPE_13, D3DXVECTOR3(660.0f, 100.0f, 0.0f), D3DXVECTOR3(1000.0f, 100.0f, 0.0f));
		SetItem(ITEMTYPE_HEAL, D3DXVECTOR3(610.0f,100.0f, 0.0f), 1000);
		break;
	case MAPTYPE_13:
		pFile = fopen(MAP13, "r");
	/*	SetWarp(MAPTYPE_10, D3DXVECTOR3(100.0f, 50.0f, 0.0f), D3DXVECTOR3(1000.0f, 100.0f, 0.0f));*/
		break;
    //=====================================================================================================

	//======================
	//ステージ３
	//======================
	case MAPTYPE_14:
		g_nChallengeStage = CHALLENGESTAGE_2;
		StopSound(SOUND_LABEL_BGM011);
		PlaySound(SOUND_LABEL_BGM008);
		pFile = fopen(MAP14, "r");
			SetWarp(MAPTYPE_15, D3DXVECTOR3(1180.0f,220.0f, 0.0f), D3DXVECTOR3(100.0f, 540.0f, 0.0f));
		break;
	case MAPTYPE_15:
		pFile = fopen(MAP15, "r");
			SetWarp(MAPTYPE_16, D3DXVECTOR3(60.0f,260.0f, 0.0f), D3DXVECTOR3(60.0f, 620.0f, 0.0f));
		break;
	case MAPTYPE_16:
		pFile = fopen(MAP16, "r");
			SetWarp(MAPTYPE_17, D3DXVECTOR3(1220.0f, 60.0f, 0.0f), D3DXVECTOR3(940.0f, 420.0f, 0.0f));
		break;
	case MAPTYPE_17:
		pFile = fopen(MAP17, "r");
		SetWarp(MAPTYPE_18, D3DXVECTOR3(580.0f, 60.0f, 0.0f), D3DXVECTOR3(660.0f, 60.0f, 0.0f));
		break;
	case MAPTYPE_18:
		pFile = fopen(MAP18, "r");
		SetWarp(MAPTYPE_19, D3DXVECTOR3(380.0f, 60.0f, 0.0f), D3DXVECTOR3(700.0f, 620.0f, 0.0f));
		break;
	case MAPTYPE_19:
		pFile = fopen(MAP19, "r");
		SetWarp(MAPTYPE_20, D3DXVECTOR3(900.0f, 580.0f, 0.0f), D3DXVECTOR3(20.0f, 580.0f, 0.0f));
		break;
	case MAPTYPE_20:
		pFile = fopen(MAP20, "r");
		SetWarp(MAPTYPE_21, D3DXVECTOR3(1220.0f, 60.0f, 0.0f), D3DXVECTOR3(20.0f, 620.0f, 0.0f));
		break;
	case MAPTYPE_21:
		pFile = fopen(MAP21, "r");
		SetWarp(MAPTYPE_22, D3DXVECTOR3(620.0f,60.0f, 0.0f), D3DXVECTOR3(SCREEN_WIDTH / 2 + 300.0f, 620.0f, 0.0f));
		SetItem(ITEMTYPE_HEAL, D3DXVECTOR3(570.0f, 60.0f, 0.0f), 1500);
		break;
	case MAPTYPE_22:
		pFile = fopen(MAP22, "r");
		break;
	//==========================================================================================================================

	//==================================
	//ステージ４
	//==================================
	case MAPTYPE_23:
		g_nChallengeStage = CHALLENGESTAGE_2;
		StopSound(SOUND_LABEL_BGM011);
		PlaySound(SOUND_LABEL_BGM009);
		pFile = fopen(MAP23, "r");
		SetWarp(MAPTYPE_24, D3DXVECTOR3(100.0f,100.0f, 0.0f), D3DXVECTOR3(100.0f,60.0f,0.0f));
		break;
	case MAPTYPE_24:
		pFile = fopen(MAP24, "r");
		SetWarp(MAPTYPE_25, D3DXVECTOR3(220.0f,260.0f,0.0f), D3DXVECTOR3(1220.0f,460.0f,0.0f));
		break;
	case MAPTYPE_25:
		pFile = fopen(MAP25, "r");
		SetWarp(MAPTYPE_26, D3DXVECTOR3(1220.0f,140.0f, 0.0f), D3DXVECTOR3(700.0f,60.0f,0.0f));
		break;
	case MAPTYPE_26:
		pFile = fopen(MAP26, "r");
		SetWarp(MAPTYPE_27, D3DXVECTOR3(20.0f,60.0f,0.0f), D3DXVECTOR3(1220.0f,620.0f,0.0f));
		break;
	case MAPTYPE_27:
		pFile = fopen(MAP27, "r");
		SetWarp(MAPTYPE_28, D3DXVECTOR3(1180.0f,60.0f,0.0f), D3DXVECTOR3(1220.0f, 460.0f, 0.0f));
		break;
	case MAPTYPE_28:
		pFile = fopen(MAP28, "r");
		SetWarp(MAPTYPE_29, D3DXVECTOR3(340.0f, 20.0f, 0.0f), D3DXVECTOR3(660.0f, 620.0f, 0.0f));
		break;
	case MAPTYPE_29:
		pFile = fopen(MAP29, "r");
		SetWarp(MAPTYPE_30, D3DXVECTOR3(1220.0f, 100.0f, 0.0f), D3DXVECTOR3(540.0f, 620.0f, 0.0f));
		break;
	case MAPTYPE_30:
		pFile = fopen(MAP30, "r");
		SetWarp(MAPTYPE_31, D3DXVECTOR3(620.0f, 180.0f, 0.0f), D3DXVECTOR3(900.0f, 660.0f, 0.0f));
		SetItem(ITEMTYPE_HEAL, D3DXVECTOR3(570.0f, 180.0f, 0.0f), 1500);
		break;
	case MAPTYPE_31:
		pFile = fopen(MAP31, "r");
		//SetWarp(MAPTYPE_25, D3DXVECTOR3(220.0f, 260.0f, 0.0f), D3DXVECTOR3(1220.0f, 460.0f, 0.0f));
		break;
	//=====================================================================================================

	//==========================
    //ラストステージ
	//==========================
	case MAPTYPE_32:
		StopSound(SOUND_LABEL_BGM011);
		PlaySound(SOUND_LABEL_BGM010);
		pFile = fopen(MAP32, "r");
		SetWarp(MAPTYPE_33, D3DXVECTOR3(660.0f, 340.0f, 0.0f), D3DXVECTOR3(60.0f, 60.0f, 0.0f));
		break;
	case MAPTYPE_33:
		pFile = fopen(MAP33, "r");
		SetWarp(MAPTYPE_34, D3DXVECTOR3(1260.0f, 60.0f, 0.0f), D3DXVECTOR3(1220.0f, 580.0f, 0.0f));
		break;
	case MAPTYPE_34:
		pFile = fopen(MAP34, "r");
		SetWarp(MAPTYPE_35, D3DXVECTOR3(1220.0f, 60.0f, 0.0f), D3DXVECTOR3(1100.0f, 300.0f, 0.0f));
		break;
	case MAPTYPE_35:
		pFile = fopen(MAP35, "r");
		SetWarp(MAPTYPE_36, D3DXVECTOR3(660.0f, 100.0f, 0.0f), D3DXVECTOR3(1260.0f, 380.0f, 0.0f));
		break;
	case MAPTYPE_36:
		pFile = fopen(MAP36, "r");
		SetWarp(MAPTYPE_37, D3DXVECTOR3(20.0f, 60.0f, 0.0f), D3DXVECTOR3(1220.0f, 340.0f, 0.0f));
		break;
	case MAPTYPE_37:
		pFile = fopen(MAP37, "r");
		SetWarp(MAPTYPE_38, D3DXVECTOR3(660.0f, 380.0f, 0.0f), D3DXVECTOR3(140.0f, 620.0f, 0.0f));
		break;
	case MAPTYPE_38:
		pFile = fopen(MAP38, "r");
		SetWarp(MAPTYPE_39, D3DXVECTOR3(1020.0f, 340.0f, 0.0f), D3DXVECTOR3(60.0f, 340.0f, 0.0f));
		break;
	case MAPTYPE_39:
		pFile = fopen(MAP39, "r");
		SetWarp(MAPTYPE_40, D3DXVECTOR3(1180.0f, 340.0f, 0.0f), D3DXVECTOR3(60.0f, 540.0f, 0.0f));
		break;
	case MAPTYPE_40:
		pFile = fopen(MAP40, "r");
		SetItem(ITEMTYPE_HEAL, D3DXVECTOR3(200.0f, 400.0f, 0.0f), 3000);
		SetWarp(MAPTYPE_41, D3DXVECTOR3(SCREEN_WIDTH / 2, 260.0f, 0.0f), D3DXVECTOR3(60.0f,600.0f, 0.0f));
		break;
	case MAPTYPE_41:
		pFile = fopen(MAP41, "r");
		//SetWarp(MAPTYPE_33, D3DXVECTOR3(660.0f, 340.0f, 0.0f), D3DXVECTOR3(60.0f, 60.0f, 0.0f));
		break;
	}
	

	//============================================
	//マップ読み込み
	//============================================


		if (pFile != NULL)
		{
			Block* pBlock = GetBlock();
			Enemy* pEnemy = GetEnemy();
		/*	Warp* pWarp = GetWarp();*/

			for (nCntHeight = 0; nCntHeight < MAX_MAPHEIGHT; nCntHeight++)
			{
				for (nCntWidth = 0; nCntWidth < MAX_MAPWIDTH; nCntWidth++)
				{
					fscanf(pFile, "%d", &g_nMapBlockData[nCntHeight][nCntWidth]);
					SetBlock(g_nMapBlockData[nCntHeight][nCntWidth], D3DXVECTOR3(20.0f + 40.0f * nCntWidth, 20.0f + 40.0f * nCntHeight, 0.0f),-1);


				}
			}
			for (nCntHeight = 0; nCntHeight < MAX_MAPHEIGHT; nCntHeight++)
			{
				for (nCntWidth = 0; nCntWidth < MAX_MAPWIDTH; nCntWidth++)
				{
					fscanf(pFile, "%d", &g_nMapBlockData[nCntHeight][nCntWidth]);

					if (g_nMapBlockData[nCntHeight][nCntWidth] >= 0)
					{
						SetEnemy(D3DXVECTOR3(20.0f + 40.0f * nCntWidth, 20.0f + 40.0f * nCntHeight, 0.0f), g_nMapBlockData[nCntHeight][nCntWidth], 0.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
					}

				}
			}
			fclose(pFile);
		}
	

	

}

//==================================
//ボス撃破判定の情報を取得
//==================================
Map_h_DefeatBoss * GetDefeatBoss(void)
{
	return &g_aDefeatBoss;
}

int* GetUseMap(void)
{
	return &g_nUseMap;
}

int* GetChallengeStage(void)
{
	return &g_nChallengeStage;
}


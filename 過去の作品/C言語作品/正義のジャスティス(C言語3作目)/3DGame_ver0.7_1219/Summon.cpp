//========================================================
//
//１１月７日：モデル読み込み[Summon.cpp]
//Author:ShinaTaiyo
//
//=========================================================

#include "Summon.h"
#include "main.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "shadow.h"
#include "player.h"
#include <stdio.h>
#include <string.h>
#include "model.h"
#include "score.h"
#include "bullet.h"
#include "combo.h"
#include "timer.h"
#include "enemy.h"
#include "item.h"

//グローバル変数
int g_nDelaySummon = 0;
float g_fRandumRot = 0;
float g_fRandumLength = 0;
float g_fRandumPos = 0;
int g_nType = 0;
//タイトル画面の初期化処理
void InitSummon(void)
{
	g_nDelaySummon = 0;
	g_fRandumRot = 0;
	g_fRandumLength = 0;
	g_fRandumPos = 0;
	g_nType = 0;
}

//============================
//モデルの更新処理
//============================
void UpdateSummon(void)
{
	Player* pPlayer = GetPlayer();

	g_nDelaySummon++;

	g_fRandumRot = float(rand() % 200 + 1) / 100;
	int nGetTimer = GetTimer();//残り時間の取得
	int nLottery = rand()% 1001 + 1;//召喚する敵を抽選する


	

	if (150 <= nGetTimer && nGetTimer <= 180)//開始３０秒
	{
		if (g_nDelaySummon % 20 == 0)
		{
			g_fRandumLength = float(rand() % 1000 + 500);//召喚する位置
			
			if (0 <= nLottery && nLottery <= 900)
			{
				g_nType = ENEMY00_NOOBSLIME;//70%の確率で、弱スライムが登場
			}
			else if (901 <= nLottery && nLottery <= 950)
			{
				g_nType = ENEMY01_ANGRYSLIME;//7%の確率で怒りスライムが登場
			}
			else if (951 <= nLottery && nLottery <= 980)
			{
				g_nType = ENEMY02_SWORDSLIME;//5%の確率でソードスライムが登場
			}
			else if (991 <= nLottery && nLottery <= 1000)
			{
				g_nType = ENEMY03_HAMMERSLIME;//3%の確率でハンマースライムが登場
			}
			else if (nLottery == 1001)
			{
				g_nType = ENEMY11_TREASURESLIME;
			}
			SetEnemy(D3DXVECTOR3(pPlayer->pos.x + sinf(-D3DX_PI * g_fRandumRot) * g_fRandumLength, pPlayer->pos.y, pPlayer->pos.z + cosf(-D3DX_PI * g_fRandumRot) * g_fRandumLength),
				D3DXVECTOR3(0.0f, 0.0f, 0.0f), g_nType);

		}
	}
	else if (120 <= nGetTimer && nGetTimer <= 150)
	{
		if (g_nDelaySummon % 17 == 0)
		{
			g_fRandumLength = float(rand() % 1000 + 500);//召喚する位置

			if (0 <= nLottery && nLottery <= 900)
			{
				g_nType = ENEMY00_NOOBSLIME;//80%の確率で、弱スライムが登場
			}
			else if (901 <= nLottery && nLottery <= 930)
			{
				g_nType = ENEMY01_ANGRYSLIME;//8%の確率で怒りスライムが登場
			}
			else if (931 <= nLottery && nLottery <= 950)
			{
				g_nType = ENEMY02_SWORDSLIME;//5%の確率でソードスライムが登場
			}
			else if (951 <= nLottery && nLottery <= 970)
			{
				g_nType = ENEMY03_HAMMERSLIME;//5%の確率でハンマースライムが登場
			}
			else if (971 <= nLottery && nLottery <= 980)
			{
				g_nType = ENEMY04_ORANGESLIME;//5%の確率でオレンジスライムが登場
			}
			else if (981 <= nLottery && nLottery <= 990)
			{
				g_nType = ENEMY05_GREENSLIME;
			}
			else if (991 <= nLottery && nLottery <= 1000)
			{
				g_nType = ENEMY06_WHITESLIME;
			}
			else if (nLottery == 1001)
			{
				g_nType = ENEMY11_TREASURESLIME;
			}
			SetEnemy(D3DXVECTOR3(pPlayer->pos.x + sinf(-D3DX_PI * g_fRandumRot) * g_fRandumLength, pPlayer->pos.y, pPlayer->pos.z + cosf(-D3DX_PI * g_fRandumRot) * g_fRandumLength),
				D3DXVECTOR3(0.0f, 0.0f, 0.0f), g_nType);

		}
	}
	else if (90 <= nGetTimer && nGetTimer <= 120)
	{
		if (g_nDelaySummon % 15 == 0)
		{
			g_fRandumLength = float(rand() % 1000 + 500);//召喚する位置

			if (0 <= nLottery && nLottery <= 900)
			{
				g_nType = ENEMY00_NOOBSLIME;//80%の確率で、弱スライムが登場
			}
			else if (901 <= nLottery && nLottery <= 910)
			{
				g_nType = ENEMY01_ANGRYSLIME;//8%の確率で怒りスライムが登場
			}
			else if (911 <= nLottery && nLottery <= 925)
			{
				g_nType = ENEMY02_SWORDSLIME;//5%の確率でソードスライムが登場
			}
			else if (926 <= nLottery && nLottery <= 940)
			{
				g_nType = ENEMY03_HAMMERSLIME;//5%の確率でハンマースライムが登場
			}
			else if (941 <= nLottery && nLottery <= 955)
			{
				g_nType = ENEMY04_ORANGESLIME;//5%の確率でオレンジスライムが登場
			}
			else if (956 <= nLottery && nLottery <= 970)
			{
				g_nType = ENEMY05_GREENSLIME;
			}
			else if (971 <= nLottery && nLottery <= 980)
			{
				g_nType = ENEMY06_WHITESLIME;
			}
			else if (981 <= nLottery && nLottery <= 990)
			{
				g_nType = ENEMY07_TURQUOISSLIME;
			}
			else if (991 <= nLottery && nLottery <= 1000)
			{
				g_nType = ENEMY08_ENGELSLIME;
			}
			else if (nLottery == 1001)
			{
				g_nType = ENEMY11_TREASURESLIME;
			}
			SetEnemy(D3DXVECTOR3(pPlayer->pos.x + sinf(-D3DX_PI * g_fRandumRot) * g_fRandumLength, pPlayer->pos.y, pPlayer->pos.z + cosf(-D3DX_PI * g_fRandumRot) * g_fRandumLength),
				D3DXVECTOR3(0.0f, 0.0f, 0.0f), g_nType);

		}
	}
	else if (60 <= nGetTimer && nGetTimer <= 90)
	{
		if (g_nDelaySummon % 12 == 0)
		{
			g_fRandumLength = float(rand() % 1000 + 500);//召喚する位置

			if (0 <= nLottery && nLottery <= 890)
			{
				g_nType = ENEMY00_NOOBSLIME;//80%の確率で、弱スライムが登場
			}
			else if (891 <= nLottery && nLottery <= 900)
			{
				g_nType = ENEMY01_ANGRYSLIME;//8%の確率で怒りスライムが登場
			}
			else if (901 <= nLottery && nLottery <= 915)
			{
				g_nType = ENEMY02_SWORDSLIME;//5%の確率でソードスライムが登場
			}
			else if (916 <= nLottery && nLottery <= 925)
			{
				g_nType = ENEMY03_HAMMERSLIME;//5%の確率でハンマースライムが登場
			}
			else if (925 <= nLottery && nLottery <= 940)
			{
				g_nType = ENEMY04_ORANGESLIME;//5%の確率でオレンジスライムが登場
			}
			else if (946 <= nLottery && nLottery <= 955)
			{
				g_nType = ENEMY05_GREENSLIME;
			}
			else if (956 <= nLottery && nLottery <= 970)
			{
				g_nType = ENEMY06_WHITESLIME;
			}
			else if (971 <= nLottery && nLottery <= 980)
			{
				g_nType = ENEMY07_TURQUOISSLIME;
			}
			else if (981 <= nLottery && nLottery <= 990)
			{
				g_nType = ENEMY08_ENGELSLIME;
			}
			else if (991 <= nLottery && nLottery <= 1000)
			{
				g_nType = ENEMY09_DARKSLIME;
			}
			else if (nLottery == 1001)
			{
				g_nType = ENEMY11_TREASURESLIME;
			}
			SetEnemy(D3DXVECTOR3(pPlayer->pos.x + sinf(-D3DX_PI * g_fRandumRot) * g_fRandumLength, pPlayer->pos.y, pPlayer->pos.z + cosf(-D3DX_PI * g_fRandumRot) * g_fRandumLength),
				D3DXVECTOR3(0.0f, 0.0f, 0.0f), g_nType);

		}
	}
	else if (60 <= nGetTimer && nGetTimer <= 90)
	{
		if (g_nDelaySummon % 9 == 0)
		{
			g_fRandumLength = float(rand() % 1000 + 500);//召喚する位置

			if (0 <= nLottery && nLottery <= 890)
			{
				g_nType = ENEMY00_NOOBSLIME;//80%の確率で、弱スライムが登場
			}
			else if (891 <= nLottery && nLottery <= 900)
			{
				g_nType = ENEMY01_ANGRYSLIME;//8%の確率で怒りスライムが登場
			}
			else if (901 <= nLottery && nLottery <= 915)
			{
				g_nType = ENEMY02_SWORDSLIME;//5%の確率でソードスライムが登場
			}
			else if (916 <= nLottery && nLottery <= 925)
			{
				g_nType = ENEMY03_HAMMERSLIME;//5%の確率でハンマースライムが登場
			}
			else if (925 <= nLottery && nLottery <= 940)
			{
				g_nType = ENEMY04_ORANGESLIME;//5%の確率でオレンジスライムが登場
			}
			else if (946 <= nLottery && nLottery <= 955)
			{
				g_nType = ENEMY05_GREENSLIME;
			}
			else if (956 <= nLottery && nLottery <= 970)
			{
				g_nType = ENEMY06_WHITESLIME;
			}
			else if (971 <= nLottery && nLottery <= 980)
			{
				g_nType = ENEMY07_TURQUOISSLIME;
			}
			else if (981 <= nLottery && nLottery <= 990)
			{
				g_nType = ENEMY08_ENGELSLIME;
			}
			else if (991 <= nLottery && nLottery <= 1000)
			{
				g_nType = ENEMY09_DARKSLIME;
			}
			else if (nLottery == 1001)
			{
				g_nType = ENEMY11_TREASURESLIME;
			}
			SetEnemy(D3DXVECTOR3(pPlayer->pos.x + sinf(-D3DX_PI * g_fRandumRot) * g_fRandumLength, pPlayer->pos.y, pPlayer->pos.z + cosf(-D3DX_PI * g_fRandumRot) * g_fRandumLength),
				D3DXVECTOR3(0.0f, 0.0f, 0.0f), g_nType);

		}
	}
	else if (30 <= nGetTimer && nGetTimer <= 60)
	{
		if (g_nDelaySummon % 5 == 0)
		{
			g_fRandumLength = float(rand() % 1000 + 500);//召喚する位置

			if (0 <= nLottery && nLottery <= 880)
			{
				g_nType = ENEMY00_NOOBSLIME;//80%の確率で、弱スライムが登場
			}
			else if (881 <= nLottery && nLottery <= 890)
			{
				g_nType = ENEMY01_ANGRYSLIME;//8%の確率で怒りスライムが登場
			}
			else if (891 <= nLottery && nLottery <= 900)
			{
				g_nType = ENEMY02_SWORDSLIME;//5%の確率でソードスライムが登場
			}
			else if (901 <= nLottery && nLottery <= 910)
			{
				g_nType = ENEMY03_HAMMERSLIME;//5%の確率でハンマースライムが登場
			}
			else if (911 <= nLottery && nLottery <= 925)
			{
				g_nType = ENEMY04_ORANGESLIME;//5%の確率でオレンジスライムが登場
			}
			else if (926 <= nLottery && nLottery <= 940)
			{
				g_nType = ENEMY05_GREENSLIME;
			}
			else if (941 <= nLottery && nLottery <= 955)
			{
				g_nType = ENEMY06_WHITESLIME;
			}
			else if (956 <= nLottery && nLottery <= 970)
			{
				g_nType = ENEMY07_TURQUOISSLIME;
			}
			else if (971 <= nLottery && nLottery <= 985)
			{
				g_nType = ENEMY08_ENGELSLIME;
			}
			else if (986 <= nLottery && nLottery <= 995)
			{
				g_nType = ENEMY09_DARKSLIME;
			}
			else if (996 <= nLottery && nLottery <= 1000)
			{
				g_nType = ENEMY09_DARKSLIME;
			}
			else if (nLottery == 1001)
			{
				g_nType = ENEMY11_TREASURESLIME;
			}
			SetEnemy(D3DXVECTOR3(pPlayer->pos.x + sinf(-D3DX_PI * g_fRandumRot) * g_fRandumLength, pPlayer->pos.y, pPlayer->pos.z + cosf(-D3DX_PI * g_fRandumRot) * g_fRandumLength),
				D3DXVECTOR3(0.0f, 0.0f, 0.0f), g_nType);

		}
	}
	else if (0 <= nGetTimer && nGetTimer <= 30)
	{
		if (g_nDelaySummon % 4 == 0)
		{
			g_fRandumLength = float(rand() % 1000 + 500);//召喚する位置

			if (0 <= nLottery && nLottery <= 870)
			{
				g_nType = ENEMY00_NOOBSLIME;//80%の確率で、弱スライムが登場
			}
			else if (871 <= nLottery && nLottery <= 875)
			{
				g_nType = ENEMY01_ANGRYSLIME;//8%の確率で怒りスライムが登場
			}
			else if (876 <= nLottery && nLottery <= 880)
			{
				g_nType = ENEMY02_SWORDSLIME;//5%の確率でソードスライムが登場
			}
			else if (881 <= nLottery && nLottery <= 885)
			{
				g_nType = ENEMY03_HAMMERSLIME;//5%の確率でハンマースライムが登場
			}
			else if (911 <= nLottery && nLottery <= 925)
			{
				g_nType = ENEMY04_ORANGESLIME;//5%の確率でオレンジスライムが登場
			}
			else if (886 <= nLottery && nLottery <= 895)
			{
				g_nType = ENEMY05_GREENSLIME;
			}
			else if (896 <= nLottery && nLottery <= 910)
			{
				g_nType = ENEMY06_WHITESLIME;
			}
			else if (911 <= nLottery && nLottery <= 925)
			{
				g_nType = ENEMY07_TURQUOISSLIME;
			}
			else if (926 <= nLottery && nLottery <= 945)
			{
				g_nType = ENEMY08_ENGELSLIME;
			}
			else if (946 <= nLottery && nLottery <= 965)
			{
				g_nType = ENEMY09_DARKSLIME;
			}
			else if (966 <= nLottery && nLottery <= 990)
			{
				g_nType = ENEMY09_DARKSLIME;
			}
			else if (991 <= nLottery && nLottery <= 1000)
			{
				g_nType = ENEMY10_RAINBOWSLIME;
			}
			else if (nLottery == 1001)
			{
				g_nType = ENEMY11_TREASURESLIME;
			}
			SetEnemy(D3DXVECTOR3(pPlayer->pos.x + sinf(-D3DX_PI * g_fRandumRot) * g_fRandumLength, pPlayer->pos.y, pPlayer->pos.z + cosf(-D3DX_PI * g_fRandumRot) * g_fRandumLength),
				D3DXVECTOR3(0.0f, 0.0f, 0.0f), g_nType);


			

		}
	}
}


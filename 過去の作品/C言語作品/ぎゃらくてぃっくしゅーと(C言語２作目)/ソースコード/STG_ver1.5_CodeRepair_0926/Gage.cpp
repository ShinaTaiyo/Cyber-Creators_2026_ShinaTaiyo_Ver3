//========================================================
//
//８月３０日：エフェクトもう一回見直す[Gage.cpp]
//Author:ShinaTaiyo
//
//=========================================================
#include "main.h"
#include "game.h"
#include "enemy.h"
#include "Gage.h"
#include "input.h"
#include "player.h"
#include "sound.h"
//マクロ定義
#define NUM_GAGE (2)
#define MAX_GAGE (10)
#define HPGAGE_LENGTH (1000.0f)
//グローバル変数
LPDIRECT3DTEXTURE9 g_apTextureGage[NUM_GAGE] = {};//テクスチャ3枚分のポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGage = NULL;//頂点バッファへのポインタ

Gage g_aGage[MAX_GAGE];

//float g_fGage = 0;
//int g_nGage_Count = 0;
//Result_2 g_aResult_2;
//============================
//体力の初期化処理
//============================
void InitGage(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntGage;

	/*g_nCountBoss1 = 0;*/

	/*g_aResult_2.bResultFlag2 = false;*/

	//g_bResultFlag2 = false;

	/* g_fGage = 0;
	 g_nGage_Count = 0;*/

	for (nCntGage = 0; nCntGage < MAX_GAGE; nCntGage++)
	{
		g_aGage[nCntGage].nGageCount = 0;
		g_aGage[nCntGage].fGage = 0;
		g_aGage[nCntGage].bUse = false;
		g_aGage[nCntGage].nType = nCntGage;
		g_aGage[nCntGage].fBoss1MaxHp = 0.0f;
		g_aGage[nCntGage].nCountBoss1 = 0;
	}



	/*g_aGage[0].bUse = true;
	g_aGage[0].nType = GAGETYPE_SKILL;*/
	/*g_fAnimationPattern = 0;*/

	//デバイスの取得
	pDevice = GetDevice();


	//D3DXCreateTextureFromFile(pDevice,
	//	"data\\Skill_Gage.png",
	//	&g_apTextureGage[0]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\BossHp_000.png",
		&g_apTextureGage[0]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\BossHp_frame_000.png",
		&g_apTextureGage[1]);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4*MAX_GAGE,//４は必要な頂点数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffGage,
		NULL);

	VERTEX_2D* pVtx;
	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffGage->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntGage = 0; nCntGage < MAX_GAGE; nCntGage++)
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
	g_pVtxBuffGage->Unlock();
}

//==========================
//体力の終了処理
//==========================
void UninitGage(void)
{
	//テクスチャの破棄
	int nCntGage;
	
	for (nCntGage = 0; nCntGage < NUM_GAGE; nCntGage++)
	{
		if (g_apTextureGage[nCntGage] != NULL)
		{
			g_apTextureGage[nCntGage]->Release();
			g_apTextureGage[nCntGage] = NULL;
		}
	}
	//頂点バッファの破棄
	if (g_pVtxBuffGage != NULL)
	{
		g_pVtxBuffGage->Release();
		g_pVtxBuffGage = NULL;
	}
}

//==================================
//体力の更新処理
//==================================
void UpdateGage(void)
{
	int nCntGage;
	int nCntEnemy;
	Player* pPlayer = GetPlayer();//プレイヤー構造体の情報をゲットする。
	Enemy* pEnemy = GetEnemy();//エネミー構造体の情報をゲットする。

	//g_aGage[0].nGageCount++;
	VERTEX_2D* pVtx;
	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffGage->Lock(0, 0, (void**)&pVtx, 0);



	for (nCntGage = 0; nCntGage < MAX_GAGE; nCntGage++)
	{



		if (g_aGage[nCntGage].bUse == true)
		{



			g_aGage[nCntGage].nCountBoss1++;



			/*	g_aGage[nCntGage].bUse = true;*/
			switch (g_aGage[nCntGage].nType)
			{
			case GAGETYPE_BOSSHP_FRAME:

			/*	if (g_aGage[nCntGage].bUse == true)
				{*/
					pVtx[0].pos = D3DXVECTOR3(0.0f, 15.0f, 0.0f);
					pVtx[1].pos = D3DXVECTOR3(HPGAGE_LENGTH, 15.0f, 0.0f);
					pVtx[2].pos = D3DXVECTOR3(0.0f, 65.0f, 0.0f);
					pVtx[3].pos = D3DXVECTOR3(HPGAGE_LENGTH, 65.0f, 0.0f);
				
					break;
			case GAGETYPE_BOSS1HP:
				for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
				{
					if (pEnemy->bUse == false && g_aGage[nCntGage].nSubjectEnemy == nCntEnemy)  //仮の処理
					{
						/*if (g_aGage[nCntEnemy].nSubjectEnemy == nCntEnemy)
						{*/
						g_aGage[nCntGage].nCountBoss1 = 0;



							//ボス１が倒されたら・・・
							StopSound(SOUND_LABEL_BGM003);
							for (nCntGage = 0; nCntGage < MAX_GAGE; nCntGage++)
							{//全てのゲージをfalseにする。
								g_aGage[nCntGage].bUse = false;
							}
					//	}
					}
					//if (pEnemy->bUse == false && g_aGage[nCntGage].nSubjectEnemy == nCntEnemy && pEnemy->nType == ENEMYTYPE_BOSS1_STARWARRIOR)  //仮の処理
					//{//ボス１が倒されたら・・・
					//	StopSound(SOUND_LABEL_BGM003);
					//	for (nCntGage = 0; nCntGage < MAX_GAGE; nCntGage++)
					//	{//全てのゲージをfalseにする。
					//		g_aGage[nCntGage].bUse = false;
					//	}
					//}

				/*	else if (g_aGage[nCntGage].bUse == true)
					{*/
						if (pEnemy->bUse == true && g_aGage[nCntGage].nSubjectEnemy == nCntEnemy)
						{
							if (g_aGage[nCntGage].nCountBoss1 == 1)
							{
								g_aGage[nCntGage].fBoss1MaxHp = pEnemy->fLife;
							}


							pVtx[0].pos = D3DXVECTOR3(0.0f, 20.0f, 0.0f);
							pVtx[1].pos = D3DXVECTOR3(0.0f + (HPGAGE_LENGTH / g_aGage[nCntGage].fBoss1MaxHp) * pEnemy->fLife, 20.0f, 0.0f);
							pVtx[2].pos = D3DXVECTOR3(0.0f, 60.0f, 0.0f);
							pVtx[3].pos = D3DXVECTOR3(0.0f + (HPGAGE_LENGTH / g_aGage[nCntGage].fBoss1MaxHp) * pEnemy->fLife, 60.0f, 0.0f);


							if (pEnemy->fLife <= g_aGage[nCntGage].fBoss1MaxHp * 0.25f)
							{
								pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
								pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
								pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
								pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
							}
							else if (pEnemy->fLife <= g_aGage[nCntGage].fBoss1MaxHp * 0.5f)
							{
								pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
								pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
								pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
								pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
							}

						}
				//	}
				
				}
				break;


			}
		
			

		}
		pVtx += 4;
	}

	
	


	//頂点バッファをアンロックする
	g_pVtxBuffGage->Unlock();
}

//==================================
//体力の描画処理
//==================================
void DrawGage(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntGage;
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffGage, 0, sizeof(VERTEX_2D));//頂点バッファへのポインタと頂点情報の構造体のサイズ

		//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntGage = 0; nCntGage < MAX_GAGE; nCntGage++)
	{
		if (g_aGage[nCntGage].bUse == true)
		{
			//テクスチャの設定
			if (g_aGage[nCntGage].nType == GAGETYPE_BOSS1HP)
			{
				pDevice->SetTexture(0, g_apTextureGage[0]);
			}
			if (g_aGage[nCntGage].nType == GAGETYPE_BOSSHP_FRAME)
			{
				pDevice->SetTexture(0, g_apTextureGage[1]);
			}

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
				nCntGage * 4,
				2);
		}
	}

}


void SetGage(int nCntEnemy, int nType)
{
	int nCntGage;

	VERTEX_2D* pVtx;
	g_pVtxBuffGage->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntGage = 0; nCntGage < MAX_GAGE; nCntGage++)
	{

		if (g_aGage[nCntGage].bUse == false)
		{//弾が使用されていない
		

			g_aGage[nCntGage].bUse = true;//使用している状態にする
			g_aGage[nCntGage].nType = nType;//弾の種類（type)を代入する。
			g_aGage[nCntGage].nSubjectEnemy = nCntEnemy;//どの敵がHPを表示する対象か
			g_aGage[nCntGage].fGage = 0.0f;
			g_aGage[nCntGage].nCountBoss1 = 0;


		

			break;
			//}
		}
		pVtx += 4;//ポインタを４つ分進める
	}
	g_pVtxBuffGage->Unlock();
}


Gage* GetGage(void)
{
	return&g_aGage[0];
}


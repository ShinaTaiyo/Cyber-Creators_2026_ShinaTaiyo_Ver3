//========================================================
//
//ミッションの処理
//Author:ShinaTaiyo
//
//=========================================================
#include "result.h"
#include "main.h"
#include "input.h"
#include "sound.h"
#include "score.h"
#include "fade.h"
#include "Mission.h"
#include "item.h"
#include "player.h"
#include "scrap.h"
#include "battery.h"
#include "timer.h"
#include "boss.h"
#include "missioncheck.h"

//=========================================================
//グローバル変数
//=========================================================
LPDIRECT3DTEXTURE9 g_pTextureMission[MAX_MISSION] = {};//テクスチャへのポインタ
PDIRECT3DVERTEXBUFFER9 g_pVtxBuffMission = NULL;//頂点バッファへのポインタ

Mission g_aMission;
MissionUI g_aMissionUI[MAX_MISSION];

//=========================================================
//初期化処理
//=========================================================
void InitMission(void)
{
	VERTEX_2D* pVtx;
	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	for (int nCntTex = 0; nCntTex < MAX_MISSION; nCntTex++)
	{
		D3DXCreateTextureFromFile(pDevice,
			MISSION_TEXTURE[nCntTex],
			&g_pTextureMission[nCntTex]);
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_MISSION,//頂点の数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMission,
		NULL);

	//============================
	//ミッション０の情報
	//============================
	g_aMission.aMission0.bMissionClear = false;	//ミッションをクリアしたかどうか
	g_aMission.aMission0.bMissionUISet = false;	//ミッションUIが表示済みかどうか
	g_aMission.aMission0.bMissionReward = false;//ミッションの報酬を受け取ったかどうか
	g_aMission.aMission0.nGetCandyCount = 0;	//キャンディ獲得量

	//飴の設定処理
	for (int nCntItem = 0; nCntItem < NUM_CANDY; nCntItem++)
	{
		switch (nCntItem)
		{
		case 0:
			SetItem(D3DXVECTOR3(-1350.0f, 0.0f, 1000.0f), NULL_VECTOR3, ITEM00_CANDY);
			break;
		case 1:
			SetItem(D3DXVECTOR3(850.0f, 0.0f, 1740.0f), NULL_VECTOR3, ITEM00_CANDY);
			break;
		case 2:
			SetItem(D3DXVECTOR3(1200.0f, 0.0f, -1700.0f), NULL_VECTOR3, ITEM00_CANDY);
			break;
		case 3:
			SetItem(D3DXVECTOR3(-650.0f, 0.0f, -1750.0f), NULL_VECTOR3, ITEM00_CANDY);
			break;
		case 4:
			SetItem(D3DXVECTOR3(0.0f, 0.0f, 0.0f), NULL_VECTOR3, ITEM00_CANDY);
			break;
		default:
			break;
		}
	}

	//============================
	//ミッション１の情報
	//============================
	g_aMission.aMission1.bMissionClear = false;	//ミッションをクリアしたかどうか
	g_aMission.aMission1.bMissionUISet = false;	//ミッションUIが表示済みかどうか
	g_aMission.aMission1.bMissionReward = false;//ミッションの報酬を受け取ったかどうか
	g_aMission.aMission1.nDefeatEnemyCount = 0;	//敵撃破総数

	//============================
	//ミッション２の情報
	//============================
	g_aMission.aMission2.bMissionClear = true;	//ミッションをクリアしたかどうか
	g_aMission.aMission2.bMissionUISet = false;	//ミッションUIが表示済みかどうか
	g_aMission.aMission2.nKeepHp = 0;			//プレイヤーの体力を保存する
	g_aMission.aMission2.bTimeUpFlag = false;	//時間になったら一回だけ処理を行う。

	//============================
	//ミッション３の情報
	//============================
	g_aMission.aMission3.bMissionClear = false;		//ミッションをクリアしたかどうか
	g_aMission.aMission3.bMissionUISet = false;		//ミッションUIが表示済みかどうか
	g_aMission.aMission3.bMissionReward = false;	//ミッションの報酬を受け取ったかどうか
	g_aMission.aMission3.nAfterBossApperCntTime = 0;//プレイヤーの体力を保存する

	//============================
	//ミッション４の情報
	//============================
	g_aMission.aMission4.bMissionClear = true;	//ミッションをクリアしたかどうか
	g_aMission.aMission4.bMissionUISet = false;	//ミッションUIが表示済みかどうか
	g_aMission.aMission4.nHealCount = 0;		//回復をした回数をカウントする
	g_aMission.aMission4.nHealTime = 0;			//回復をしている時間をカウントする

	//=============================
	//ミッションクリア表示UIの情報
	//=============================
	
	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffMission->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntMission = 0; nCntMission < MAX_MISSION; nCntMission++)
	{
		g_aMissionUI[nCntMission].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//位置
		g_aMissionUI[nCntMission].nDisplayTime = 0;//表示時間
		g_aMissionUI[nCntMission].nType = -1;//種類
		g_aMissionUI[nCntMission].fWidth = 0.0f;//横幅
		g_aMissionUI[nCntMission].fHeight = 0.0f;//高さ
		g_aMissionUI[nCntMission].bUse = false;//使用状態

		for (int nCntVtx = 0; nCntVtx < 4; nCntVtx++)
		{
			pVtx[nCntVtx].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//頂点情報の設定
			pVtx[nCntVtx].rhw = 1.0f;								//rhwの設定
			pVtx[nCntVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	//頂点カラーの設定
		}

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffMission->Unlock();
}

//=========================================================
//終了処理
//=========================================================
void UninitMission(void)
{
	//テクスチャの破棄
	for (int nCntTex = 0; nCntTex < MAX_MISSION; nCntTex++)
	{
		if (g_pTextureMission[nCntTex] != NULL)
		{
			g_pTextureMission[nCntTex]->Release();
			g_pTextureMission[nCntTex] = NULL;
		}
	}
	//頂点バッファの破棄
	if (g_pVtxBuffMission != NULL)
	{
		g_pVtxBuffMission->Release();
		g_pVtxBuffMission = NULL;
	}
}

//=========================================================
//更新処理
//=========================================================
void UpdateMission(void)
{
	int nCheckType = -1;//設定するチェックマークの種類

	VERTEX_2D* pVtx;
	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffMission->Lock(0, 0, (void**)&pVtx, 0);

	//クリア判定処理
	JudgeClear();

	for (int nCntMission = 0; nCntMission < MAX_MISSION; nCntMission++)
	{
		if (g_aMissionUI[nCntMission].bUse)
		{//使用している場合
			g_aMissionUI[nCntMission].nDisplayTime--;//表示時間を減少

			if (g_aMissionUI[nCntMission].pos.x < g_aMissionUI[nCntMission].fWidth / 2.0f)
			{//移動中の場合
				g_aMissionUI[nCntMission].pos.x += MISSION_MOVE;//画面外から右に移動

				if (g_aMissionUI[nCntMission].pos.x > g_aMissionUI[nCntMission].fWidth / 2.0f)
				{//移動済みの場合
					//指定の位置を超えないように調整
					g_aMissionUI[nCntMission].pos.x = g_aMissionUI[nCntMission].fWidth / 2.0f;

					//チェックマークの種類を指定
					if (g_aMissionUI[nCntMission].nType == 0 || g_aMissionUI[nCntMission].nType == 1 || g_aMissionUI[nCntMission].nType == 3)
					{
						nCheckType = 0;//チェックマーク
					}
					else
					{
						nCheckType = 1;//×マーク
					}

					//チェックマークの設定
					SetMissionCheck(D3DXVECTOR3(g_aMissionUI[nCntMission].pos.x + g_aMissionUI[nCntMission].fWidth * 0.5f - g_aMissionUI[nCntMission].fHeight * 0.5f, g_aMissionUI[nCntMission].pos.y, 0.0f),
						nCheckType, g_aMissionUI[nCntMission].nDisplayTime, g_aMissionUI[nCntMission].fHeight);
				}

				//頂点座標の更新
				pVtx[0].pos = D3DXVECTOR3(g_aMissionUI[nCntMission].pos.x - g_aMissionUI[nCntMission].fWidth / 2.0f, g_aMissionUI[nCntMission].pos.y - g_aMissionUI[nCntMission].fHeight / 2.0f, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aMissionUI[nCntMission].pos.x + g_aMissionUI[nCntMission].fWidth / 2.0f, g_aMissionUI[nCntMission].pos.y - g_aMissionUI[nCntMission].fHeight / 2.0f, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aMissionUI[nCntMission].pos.x - g_aMissionUI[nCntMission].fWidth / 2.0f, g_aMissionUI[nCntMission].pos.y + g_aMissionUI[nCntMission].fHeight / 2.0f, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aMissionUI[nCntMission].pos.x + g_aMissionUI[nCntMission].fWidth / 2.0f, g_aMissionUI[nCntMission].pos.y + g_aMissionUI[nCntMission].fHeight / 2.0f, 0.0f);
			}

			if (g_aMissionUI[nCntMission].nDisplayTime <= 0)
			{//表示時間が0になった場合
				g_aMissionUI[nCntMission].bUse = false;//使用しない
			}
		}

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffMission->Unlock();
}

//=========================================================
//描画処理
//=========================================================
void DrawMission(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffMission, 0, sizeof(VERTEX_2D));//頂点バッファへのポインタと頂点情報の構造体のサイズ

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntMission = 0; nCntMission < MAX_MISSION; nCntMission++)
	{
		if (g_aMissionUI[nCntMission].bUse)
		{//使用している場合
			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureMission[g_aMissionUI[nCntMission].nType]);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntMission * 4, 2);
		}
	}
}

//=========================================================
//ミッションの構造体情報の取得
//=========================================================
Mission* GetMission(void)
{
	return &g_aMission;
}

//=========================================================
//設定処理
//=========================================================
void SetMission(int nType, float fWidth, float fHeight)
{
	VERTEX_2D* pVtx;
	int nCntUse = 0;//表示しているミッション数

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffMission->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntMission = 0; nCntMission < MAX_MISSION; nCntMission++)
	{//表示しているミッション数をカウント
		if (g_aMissionUI[nCntMission].bUse)
		{//使用している場合
			nCntUse++;//表示しているミッション数を加算
		}
	}

	for (int nCntMission = 0; nCntMission < MAX_MISSION; nCntMission++)
	{
		if (!g_aMissionUI[nCntMission].bUse)
		{//使用していない場合
			g_aMissionUI[nCntMission].bUse = true;//使用する
			g_aMissionUI[nCntMission].nDisplayTime = 60 * MISSION_DISPLAY_TIME;//表示時間
			g_aMissionUI[nCntMission].nType = nType;//種類
			g_aMissionUI[nCntMission].fWidth = fWidth;//横幅
			g_aMissionUI[nCntMission].fHeight = fHeight;//高さ
			g_aMissionUI[nCntMission].pos = D3DXVECTOR3(0.0f - fWidth / 2.0f, MISSION_POS_Y + fHeight * nCntUse, 0.0f);//位置

			//頂点座標
			pVtx[0].pos = D3DXVECTOR3(g_aMissionUI[nCntMission].pos.x - fWidth / 2.0f, g_aMissionUI[nCntMission].pos.y - fHeight / 2.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aMissionUI[nCntMission].pos.x + fWidth / 2.0f, g_aMissionUI[nCntMission].pos.y - fHeight / 2.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aMissionUI[nCntMission].pos.x - fWidth / 2.0f, g_aMissionUI[nCntMission].pos.y + fHeight / 2.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aMissionUI[nCntMission].pos.x + fWidth / 2.0f, g_aMissionUI[nCntMission].pos.y + fHeight / 2.0f, 0.0f);

			break;
		}

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffMission->Unlock();
}

//=========================================================
//クリア判定処理
//=========================================================
void JudgeClear(void)
{
	Player* pPlayer = GetPlayer();//プレイヤーの情報の取得
	Battery* pBattery = GetBattery();//体力の取得
	Boss* pBoss = GetBoss();//ボスの情報の取得

	//===========================
	//ミッション0　飴を5つ集める
	//===========================
	if (g_aMission.aMission0.nGetCandyCount >= NUM_CANDY)
	{//全て集めた場合
		if (!g_aMission.aMission0.bMissionClear)
		{//未クリアの場合
			SubBattery(-100);
			
			if (!g_aMission.aMission0.bMissionUISet)
			{//UI表示済みではない場合
				//ミッション表示
				SetMission((int)(MISSION00_COLLECTCANDY), 380.0f, 66.0f);

				//UI表示済み判定にする
				g_aMission.aMission0.bMissionUISet = true;
			}

			PlaySound(SOUND_LABEL_SE_SCRAPCOLLECT_000);

			//スクラップをドロップ
			for (int nCntScrap = 0; nCntScrap < 50; nCntScrap++)
			{
				float fRandPos = float(rand() % 100 + 1);
				float fRandRot = float(rand() % 200) / 100;

				SetScrap(4, 300, 30.0f, 30.0f, D3DXVECTOR3(sinf(D3DX_PI * fRandRot) * fRandPos + pPlayer->pos.x, 0.0f, cosf(D3DX_PI * fRandRot) * fRandPos + pPlayer->pos.z), NULL_VECTOR3, NULL_VECTOR3);
			}

			//クリア済みにする
			g_aMission.aMission0.bMissionClear = true;
		}
	}
	else
	{//全て集めていない場合
		//未クリア
		g_aMission.aMission0.bMissionClear = false;
	}

	//==========================
	//ミッション1　敵を30体倒す
	//==========================
	if (g_aMission.aMission1.nDefeatEnemyCount >= NEED_KILLENEMY)
	{//敵の撃破数が30を超えた場合
		if (!g_aMission.aMission1.bMissionClear)
		{//未クリアの場合
			SubBattery(-100);

			if (!g_aMission.aMission1.bMissionUISet)
			{//UI表示済みではない場合
				//ミッション表示
				SetMission((int)(MISSION01_DEFEATENEMY30), 380.0f, 66.0f);

				//UI表示済み判定にする
				g_aMission.aMission1.bMissionUISet = true;
			}

			//スクラップをドロップ
			for (int nCntScrap = 0; nCntScrap < 50; nCntScrap++)
			{
				float fRandPos = float(rand() % 100 + 1);
				float fRandRot = float(rand() % 200) / 100;

				SetScrap(4, 300, 30.0f, 30.0f, D3DXVECTOR3(sinf(D3DX_PI * fRandRot) * fRandPos + pPlayer->pos.x, 0.0f, cosf(D3DX_PI * fRandRot) * fRandPos + pPlayer->pos.z), NULL_VECTOR3, NULL_VECTOR3);
			}

			g_aMission.aMission1.bMissionClear = true;//クリア済み
		}
	}
	else
	{//敵の撃破数が30未満の場合
		g_aMission.aMission1.bMissionClear = false;//未クリア
	}

	//=====================================
	//ミッション2　体力が50%未満にならない
	//=====================================
	if (g_aMission.aMission2.bMissionClear)
	{//ミッション失敗していない場合
		//プレイヤーの体力の割合を計算
		float fRatioBattery = (float)(pBattery->nBattery) / (float)(pPlayer->nBattery);

		if (fRatioBattery < 0.5f)
		{//体力が50%未満になった場合
			if (!g_aMission.aMission2.bMissionUISet)
			{//UI表示済みではない場合
				//ミッション表示
				SetMission((int)(MISSION02_DONTFALLBELOW70), 380.0f, 66.0f);

				//UI表示済み判定にする
				g_aMission.aMission1.bMissionUISet = true;
			}

			//ミッション失敗
			g_aMission.aMission2.bMissionClear = false;
		}
	}

	//===================================
	//ミッション3　ボスを100秒以内に倒す
	//===================================
	if (pBoss->bBossSummon)
	{//ボスが出現済みの場合
		if (!pBoss->bUse)
		{//ボスが撃破済みの場合場合
			if (g_aMission.aMission3.nAfterBossApperCntTime < 6000)
			{//ボスを倒した時間が100秒以内の場合
				if (!g_aMission.aMission3.bMissionClear)
				{//未達成の場合
					SubBattery(-100);

					if (!g_aMission.aMission3.bMissionUISet)
					{//UI表示済みではない場合
						//ミッション表示
						SetMission((int)(MISSION03_BOSSDEFEAT70SEC), 380.0f, 66.0f);

						//UI表示済み判定にする
						g_aMission.aMission3.bMissionUISet = true;
					}

					//スクラップをドロップ
					for (int nCntScrap = 0; nCntScrap < 50; nCntScrap++)
					{
						float fRandPos = float(rand() % 100 + 1);
						float fRandRot = float(rand() % 200) / 100;

						SetScrap(4, 300, 30.0f, 30.0f, D3DXVECTOR3(sinf(D3DX_PI * fRandRot) * fRandPos + pPlayer->pos.x, 0.0f, cosf(D3DX_PI * fRandRot) * fRandPos + pPlayer->pos.z), NULL_VECTOR3, NULL_VECTOR3);
					}

					g_aMission.aMission3.bMissionClear = true;//達成
				}
			}
		}
		else
		{
			g_aMission.aMission3.nAfterBossApperCntTime++;//ボスを撃破するまでの時間を計測
			g_aMission.aMission3.bMissionClear = false;//未達成
		}
	}

	//=================================
	//ミッション4　回復3回以内でクリア
	//=================================
	if (pPlayer->bChargingFlag == true || pPlayer->bChargingGroundFlag == true)
	{
		g_aMission.aMission4.nHealTime++;

		if (g_aMission.aMission4.nHealTime == 1)
		{
			g_aMission.aMission4.nHealCount++;
		}
	}
	else
	{
		g_aMission.aMission4.nHealTime = 0;
	}

	if (g_aMission.aMission4.nHealCount < 4)
	{//回復回数が3回以下の場合
		g_aMission.aMission4.bMissionClear = true;	//クリア判定
	}
	else
	{//回復回数が4回以上の場合
		if (!g_aMission.aMission4.bMissionUISet)
		{//UI表示済みではない場合
			//ミッション表示
			SetMission((int)(MISSION04_HEALBIND_3), 380.0f, 66.0f);

			//UI表示済み判定にする
			g_aMission.aMission4.bMissionUISet = true;
		}

		g_aMission.aMission4.bMissionClear = false;	//未クリア判定
	}

	if (pBattery->nBattery <= 0)
	{//HPが0になった場合
		g_aMission.aMission4.bMissionClear = false;	//未クリア判定
	}
}

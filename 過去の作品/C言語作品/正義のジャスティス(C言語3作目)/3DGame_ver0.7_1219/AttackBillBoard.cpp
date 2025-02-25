//========================================================
//
//９月２６日：ハッカソンの基盤作り[AttackBillBoard.cpp]
//Author:ShinaTaiyo
//
//=========================================================

#include "AttackBillBoard.h"
#include "main.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "shadow.h"
#include "effect.h"
#include "wall.h"
#include "particle.h"
#include "enemy.h"
#include "damage.h"
#include "SkillGage.h"
#include "player.h"

#define ATTACKBILLBOARD_SCALE (10.0f)
//グローバル変数
PDIRECT3DVERTEXBUFFER9 g_pVtxBuffAttackBillBoard = NULL;//頂点バッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureAttackBillBoard[NUM_ATTACKBILLBOARD] = {};//テクスチャへのポインタ
D3DXVECTOR3 g_posAttackBillBoard;//位置
//D3DXVECTOR3 g_rotAttackBillBoard;//向き
D3DXMATRIX g_mtxWorldAttackBillBoard;//ワールドマトリックス(float4*4)

AttackBillBoard g_aAttackBillBoard[MAX_ATTACKBILLBOARD];//弾の構造体情報

int g_nDelayA_Billboard = 0;//攻撃ビルボードの遅らせるカウント処理

//タイトル画面の初期化処理
void InitAttackBillBoard(void)
{

	g_nDelayA_Billboard = 0;
	//bFadeFlag = false;

	//g_nCntTime = 0;

	//g_nColorChange = 255;//カラーの初期化（白色）

	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\AttackMotion_Thunder.png",
		&g_pTextureAttackBillBoard[0]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Skill_Slash.png",
		&g_pTextureAttackBillBoard[1]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\AtomicSlash.png",
		&g_pTextureAttackBillBoard[2]);


	g_posAttackBillBoard = D3DXVECTOR3(0.0f, 50.0f, 100.0f);//位置の初期化

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_ATTACKBILLBOARD,//頂点の数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffAttackBillBoard,
		NULL);

	for (int nCntAttackBillBoard = 0; nCntAttackBillBoard < MAX_ATTACKBILLBOARD; nCntAttackBillBoard++)
	{
		g_aAttackBillBoard[nCntAttackBillBoard].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//位置
		g_aAttackBillBoard[nCntAttackBillBoard].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//移動量
		g_aAttackBillBoard[nCntAttackBillBoard].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//向き
		g_aAttackBillBoard[nCntAttackBillBoard].nLife = 0;//体力
		g_aAttackBillBoard[nCntAttackBillBoard].fRWidth = 0.0f;//幅（半径）
		g_aAttackBillBoard[nCntAttackBillBoard].fRHeight = 0.0f;//高さ（半径）
		g_aAttackBillBoard[nCntAttackBillBoard].nType = 0;//種類
		g_aAttackBillBoard[nCntAttackBillBoard].bUse = false;//使用しているかどうか
		g_aAttackBillBoard[nCntAttackBillBoard].nIdxShadow = -1;//影のインデックス
		g_aAttackBillBoard[nCntAttackBillBoard].nAnimationCnt = 0;//アニメーションカウント
		g_aAttackBillBoard[nCntAttackBillBoard].nAnimationPattern = 0;//アニメーションパターン
		g_aAttackBillBoard[nCntAttackBillBoard].bOneFlag = false;//一度だけ発動させる処理用
		g_aAttackBillBoard[nCntAttackBillBoard].SavePos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//位置を保存する

	}




	////タイトル画面BGMの設定
	//PlaySound(SOUND_LABEL_BGM000);

	VERTEX_3D* pVtx;
	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffAttackBillBoard->Lock(0, 0, (void**)&pVtx, 0);


	for (int nCntAttackBillBoard = 0; nCntAttackBillBoard < MAX_ATTACKBILLBOARD; nCntAttackBillBoard++)
	{
		//頂点座標の設定（初期位置）
		pVtx[0].pos = D3DXVECTOR3(-ATTACKBILLBOARD_SCALE, ATTACKBILLBOARD_SCALE, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(ATTACKBILLBOARD_SCALE, ATTACKBILLBOARD_SCALE, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-ATTACKBILLBOARD_SCALE, -ATTACKBILLBOARD_SCALE, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(ATTACKBILLBOARD_SCALE, -ATTACKBILLBOARD_SCALE, 0.0f);

		//norの設定（初期設定）
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。

		//テクスチャ座標の設定（初期設定）
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;//ポインタを進める
	}

	//頂点バッファをアンロックする
	g_pVtxBuffAttackBillBoard->Unlock();
}

//==============================
//タイトル画面の終了処理
//==============================
void UninitAttackBillBoard(void)
{
	//テクスチャの破棄
	for (int nCnt = 0; nCnt < NUM_ATTACKBILLBOARD; nCnt++)
	{
		if (g_pTextureAttackBillBoard[nCnt] != NULL)
		{
			g_pTextureAttackBillBoard[nCnt]->Release();
			g_pTextureAttackBillBoard[nCnt] = NULL;
		}
	}
	//頂点バッファの破棄
	if (g_pVtxBuffAttackBillBoard != NULL)
	{
		g_pVtxBuffAttackBillBoard->Release();
		g_pVtxBuffAttackBillBoard = NULL;
	}
}
//============================
//タイトル画面の更新処理
//============================
void UpdateAttackBillBoard(void)
{
	VERTEX_3D* pVtx;
	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffAttackBillBoard->Lock(0, 0, (void**)&pVtx, 0);

	Player* pPlayer = GetPlayer();

	for (int nCnfAttackBillBoard = 0; nCnfAttackBillBoard < MAX_ATTACKBILLBOARD; nCnfAttackBillBoard++)
	{

		if (g_aAttackBillBoard[nCnfAttackBillBoard].bUse == true)
		{
			g_aAttackBillBoard[nCnfAttackBillBoard].nLife -= 1;
			g_aAttackBillBoard[nCnfAttackBillBoard].nAnimationCnt++;

			if (g_aAttackBillBoard[nCnfAttackBillBoard].nLife <= 0)
			{
				g_aAttackBillBoard[nCnfAttackBillBoard].bUse = false;//体力が０になったらfalseにする
			}

			if (g_aAttackBillBoard[nCnfAttackBillBoard].nType == A_BILLBOARDTYPE_THUNDER)
			{
				Enemy* pEnemy = GetEnemy();
				for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
				{
					if (pEnemy->bUse == true)
					{
						//敵と攻撃のXとZの距離をとる
						float fLength = sqrtf((g_aAttackBillBoard[nCnfAttackBillBoard].pos.x - pEnemy->pos.x) * (g_aAttackBillBoard[nCnfAttackBillBoard].pos.x - pEnemy->pos.x) +
							(g_aAttackBillBoard[nCnfAttackBillBoard].pos.z - pEnemy->pos.z) * (g_aAttackBillBoard[nCnfAttackBillBoard].pos.z - pEnemy->pos.z)) / 2;

						if (pEnemy->pos.y + pEnemy->Size.y >= g_aAttackBillBoard[nCnfAttackBillBoard].pos.y - g_aAttackBillBoard[nCnfAttackBillBoard].fRHeight &&
							pEnemy->pos.y - pEnemy->Size.y <= g_aAttackBillBoard[nCnfAttackBillBoard].pos.y + g_aAttackBillBoard[nCnfAttackBillBoard].fRHeight &&
							fLength <= 50.0f)
						{
							/*pEnemy->nHp -= 2;*/
							SetDamageEnemy(2, nCntEnemy);
							SetDamage(2, 100, pEnemy->pos, 20.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
						}
					}
				}

				if (g_aAttackBillBoard[nCnfAttackBillBoard].nAnimationCnt % 5 == 0)
				{
					g_aAttackBillBoard[nCnfAttackBillBoard].nAnimationPattern++;

					if (g_aAttackBillBoard[nCnfAttackBillBoard].nAnimationPattern >= 5)
					{
						g_aAttackBillBoard[nCnfAttackBillBoard].nAnimationPattern = 0;
					}
				}

				//テクスチャ座標の設定（初期設定）
				pVtx[0].tex = D3DXVECTOR2(0.0f + 0.2f * g_aAttackBillBoard[nCnfAttackBillBoard].nAnimationPattern, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(0.2f + 0.2f * g_aAttackBillBoard[nCnfAttackBillBoard].nAnimationPattern, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f + 0.2f * g_aAttackBillBoard[nCnfAttackBillBoard].nAnimationPattern, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(0.2f + 0.2f * g_aAttackBillBoard[nCnfAttackBillBoard].nAnimationPattern, 1.0f);

				if (g_aAttackBillBoard[nCnfAttackBillBoard].bUse == false)
				{
					for (int nCnt = 0; nCnt < 100; nCnt++)
					{
						float fRandumRot = float(rand() % 200 + 1) / 100;
						float fRandumSpeed = float(rand() % 100 + 30) / 10;
						float fRandumMove = float(rand() % 21 - 10);

						SetParticle(0, 1000, 20.0f, 0.1f, g_aAttackBillBoard[nCnfAttackBillBoard].pos,
							D3DXVECTOR3(sinf(-D3DX_PI * fRandumRot) * fRandumSpeed, fRandumMove, cosf(-D3DX_PI * fRandumRot) * fRandumSpeed),
							D3DXCOLOR(0.5f, 0.5f, 1.0f, 1.0f), 0.0f, PARTICLETEX_NORMAL);
					}
				}
			}
			else if (g_aAttackBillBoard[nCnfAttackBillBoard].nType == A_BILLBOARDTYPE_SLASH)
			{
				Enemy* pEnemy = GetEnemy();

				if (g_aAttackBillBoard[nCnfAttackBillBoard].bOneFlag == true)
				{//一回だけダメージを与えたい処理
					for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
					{
						if (pEnemy->bUse == true)
						{
							//敵と攻撃のXとZの距離をとる
							//敵と攻撃のXとZの距離をとる
							float fLength = sqrtf((g_aAttackBillBoard[nCnfAttackBillBoard].pos.x - pEnemy->pos.x) * (g_aAttackBillBoard[nCnfAttackBillBoard].pos.x - pEnemy->pos.x) +
								(g_aAttackBillBoard[nCnfAttackBillBoard].pos.z - pEnemy->pos.z) * (g_aAttackBillBoard[nCnfAttackBillBoard].pos.z - pEnemy->pos.z)) / 2;

							if (pEnemy->pos.y + pEnemy->Size.y >= g_aAttackBillBoard[nCnfAttackBillBoard].pos.y - g_aAttackBillBoard[nCnfAttackBillBoard].fRHeight &&
								pEnemy->pos.y - pEnemy->Size.y <= g_aAttackBillBoard[nCnfAttackBillBoard].pos.y + g_aAttackBillBoard[nCnfAttackBillBoard].fRHeight &&
								fLength < g_aAttackBillBoard[nCnfAttackBillBoard].fRWidth)
							{
								/*pEnemy->nHp -= 10;*/
								SetDamageEnemy(10, nCntEnemy);
								SetDamage(10, 100, pEnemy->pos, 50.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
								g_aAttackBillBoard[nCnfAttackBillBoard].bOneFlag = false;
								AddSkillGage(6.5f);

								for (int nCnt = 0; nCnt < 2; nCnt++)
								{
									float fRandumRot = float(rand() % 200 + 1) / 100;
									float fRandumSpeed = float(rand() % 100 + 30) / 10;
									float fRandumMove = float(rand() % 21 - 10);

									SetParticle(0, 1000, 20.0f, 0.1f, g_aAttackBillBoard[nCnfAttackBillBoard].pos,
										D3DXVECTOR3(sinf(-D3DX_PI * fRandumRot) * fRandumSpeed, fRandumMove, cosf(-D3DX_PI * fRandumRot) * fRandumSpeed),
										D3DXCOLOR(1.0f, 0.5f, 0.5f, 1.0f), 0.0f, PARTICLETEX_NORMAL);
								}
							}
						}
					}
				}

				if (g_aAttackBillBoard[nCnfAttackBillBoard].nAnimationCnt % 2 == 0)
				{
					g_aAttackBillBoard[nCnfAttackBillBoard].nAnimationPattern++;

					if (g_aAttackBillBoard[nCnfAttackBillBoard].nAnimationPattern >= 5)
					{
						g_aAttackBillBoard[nCnfAttackBillBoard].nAnimationPattern = 0;
					}
				}

				//テクスチャ座標の設定（初期設定）
				pVtx[0].tex = D3DXVECTOR2(0.0f + 0.2f * g_aAttackBillBoard[nCnfAttackBillBoard].nAnimationPattern, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(0.2f + 0.2f * g_aAttackBillBoard[nCnfAttackBillBoard].nAnimationPattern, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f + 0.2f * g_aAttackBillBoard[nCnfAttackBillBoard].nAnimationPattern, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(0.2f + 0.2f * g_aAttackBillBoard[nCnfAttackBillBoard].nAnimationPattern, 1.0f);
			}

			else if (g_aAttackBillBoard[nCnfAttackBillBoard].nType == A_BILLBOARDTYPE_ATOMICSLASH)
			{
				Enemy* pEnemy = GetEnemy();
					for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
					{
						if (pEnemy->bUse == true)
						{
							//敵と攻撃のXとZの距離をとる
							//敵と攻撃のXとZの距離をとる
							float fLength = sqrtf((g_aAttackBillBoard[nCnfAttackBillBoard].pos.x - pEnemy->pos.x) * (g_aAttackBillBoard[nCnfAttackBillBoard].pos.x - pEnemy->pos.x) +
								(g_aAttackBillBoard[nCnfAttackBillBoard].pos.z - pEnemy->pos.z) * (g_aAttackBillBoard[nCnfAttackBillBoard].pos.z - pEnemy->pos.z)) / 2;

							if (pEnemy->pos.y + pEnemy->Size.y >= g_aAttackBillBoard[nCnfAttackBillBoard].pos.y - g_aAttackBillBoard[nCnfAttackBillBoard].fRHeight &&
								pEnemy->pos.y - pEnemy->Size.y <= g_aAttackBillBoard[nCnfAttackBillBoard].pos.y + g_aAttackBillBoard[nCnfAttackBillBoard].fRHeight &&
								fLength < g_aAttackBillBoard[nCnfAttackBillBoard].fRWidth)
							{
							/*	pEnemy->nHp -= 25;*/
								SetDamageEnemy(25, nCntEnemy);
								pPlayer->nHp += 1;//ヒットする度HPを回復する
								SetDamage(25, 120, pEnemy->pos, 100.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
								AddSkillGage(1.7f);//スキルゲージをブーストする
								for (int nCnt = 0; nCnt < 10; nCnt++)
								{
									float fRandumRot = float(rand() % 200 + 1) / 100;
									float fRandumSpeed = float(rand() % 100 + 30) / 10;
									float fRandumMove = float(rand() % 21 - 10);

									SetParticle(0, 1000, 15.0f, 0.2f, g_aAttackBillBoard[nCnfAttackBillBoard].pos,
										D3DXVECTOR3(sinf(-D3DX_PI * fRandumRot) * fRandumSpeed, fRandumMove, cosf(-D3DX_PI * fRandumRot) * fRandumSpeed),
										D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 0.0f, PARTICLETEX_NORMAL);


								}
							}
						}
					}
				

				g_aAttackBillBoard[nCnfAttackBillBoard].nAnimationPattern++;

				if (g_aAttackBillBoard[nCnfAttackBillBoard].nAnimationPattern >= 5)
				{
					g_aAttackBillBoard[nCnfAttackBillBoard].nAnimationPattern = 0;
				}

				//テクスチャ座標の設定（初期設定）
				pVtx[0].tex = D3DXVECTOR2(0.0f + 0.2f * g_aAttackBillBoard[nCnfAttackBillBoard].nAnimationPattern, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(0.2f + 0.2f * g_aAttackBillBoard[nCnfAttackBillBoard].nAnimationPattern, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f + 0.2f * g_aAttackBillBoard[nCnfAttackBillBoard].nAnimationPattern, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(0.2f + 0.2f * g_aAttackBillBoard[nCnfAttackBillBoard].nAnimationPattern, 1.0f);
			}


		}

		pVtx += 4;//ポインタを進める
	}

	//頂点バッファをアンロックする
	g_pVtxBuffAttackBillBoard->Unlock();
}



//================================
//タイトル画面の描画処理
//================================
void DrawAttackBillBoard(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

		//デバイスの取得
	pDevice = GetDevice();



	for (int nCntAttackBillBoard = 0; nCntAttackBillBoard < MAX_ATTACKBILLBOARD; nCntAttackBillBoard++)
	{
		if (g_aAttackBillBoard[nCntAttackBillBoard].bUse == true)
		{
			D3DXMATRIX mtxTrans;//計算用マトリックス

			D3DXMATRIX mtxView;//ビューマトリックス取得用

			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_mtxWorldAttackBillBoard);

			//ビューマトリックスを取得
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//ポリゴンをカメラに対して正面に向ける
			D3DXMatrixInverse(&g_mtxWorldAttackBillBoard, NULL, &mtxView);

			//逆行列を求める
			g_mtxWorldAttackBillBoard._41 = 0.0f;
			g_mtxWorldAttackBillBoard._42 = 0.0f;
			g_mtxWorldAttackBillBoard._43 = 0.0f;

			////向きを反映
			//D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotAttackBillBoard.y, g_rotAttackBillBoard.x, g_rotAttackBillBoard.z);

			//D3DXMatrixMultiply(&g_mtxWorldAttackBillBoard, &g_mtxWorldAttackBillBoard, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aAttackBillBoard[nCntAttackBillBoard].pos.x, g_aAttackBillBoard[nCntAttackBillBoard].pos.y, g_aAttackBillBoard[nCntAttackBillBoard].pos.z);
			D3DXMatrixMultiply(&g_mtxWorldAttackBillBoard, &g_mtxWorldAttackBillBoard, &mtxTrans);

			//ライトを無効にする
			pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);



			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldAttackBillBoard);


			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffAttackBillBoard, 0, sizeof(VERTEX_3D));//頂点バッファへのポインタと頂点情報の構造体のサイズ

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			for (int nCnt = 0; nCnt < NUM_ATTACKBILLBOARD; nCnt++)
			{
				if (g_aAttackBillBoard[nCntAttackBillBoard].nType == nCnt)
				{
					//テクスチャの設定
					pDevice->SetTexture(0, g_pTextureAttackBillBoard[nCnt]);
				}
			}
			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntAttackBillBoard * 4, 2);//0は、描画する最初の頂点インデックスで、2は、描画するプリミティブ

			//ライトを有効に戻す
			pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
		}
	}
}

void SetAttackBillBoard(int nType, int nLife, float fRWidth, float fRHeight, D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot)
{

	VERTEX_3D* pVtx;
	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffAttackBillBoard->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntAttackBillBoard = 0; nCntAttackBillBoard < MAX_ATTACKBILLBOARD; nCntAttackBillBoard++)
	{
		if (g_aAttackBillBoard[nCntAttackBillBoard].bUse == false)
		{
			g_aAttackBillBoard[nCntAttackBillBoard].bUse = true;//使用しているかどうか
			g_aAttackBillBoard[nCntAttackBillBoard].pos = pos;//位置
			g_aAttackBillBoard[nCntAttackBillBoard].move = move;//移動量
			g_aAttackBillBoard[nCntAttackBillBoard].rot = rot;//向き
			g_aAttackBillBoard[nCntAttackBillBoard].nLife = nLife;//体力
			g_aAttackBillBoard[nCntAttackBillBoard].fRWidth = fRWidth;//幅（半径）
			g_aAttackBillBoard[nCntAttackBillBoard].fRHeight = fRHeight;//高さ（半径）
			g_aAttackBillBoard[nCntAttackBillBoard].nType = nType;//種類
			g_aAttackBillBoard[nCntAttackBillBoard].nAnimationCnt = 0;//アニメーションカウント
			g_aAttackBillBoard[nCntAttackBillBoard].nAnimationPattern = 0;//アニメーションパターン
			g_aAttackBillBoard[nCntAttackBillBoard].bOneFlag = true;//アニメーションパターン
			g_aAttackBillBoard[nCntAttackBillBoard].SavePos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//位置を保存する

			//頂点座標の設定（初期位置）
			pVtx[0].pos = D3DXVECTOR3(-g_aAttackBillBoard[nCntAttackBillBoard].fRWidth, g_aAttackBillBoard[nCntAttackBillBoard].fRHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aAttackBillBoard[nCntAttackBillBoard].fRWidth, g_aAttackBillBoard[nCntAttackBillBoard].fRHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aAttackBillBoard[nCntAttackBillBoard].fRWidth, -g_aAttackBillBoard[nCntAttackBillBoard].fRHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aAttackBillBoard[nCntAttackBillBoard].fRWidth, -g_aAttackBillBoard[nCntAttackBillBoard].fRHeight, 0.0f);

			break;
		}
		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffAttackBillBoard->Unlock();
}

AttackBillBoard* GetAttackBillBoard(void)//弾構造体の情報をゲットする
{
	return &g_aAttackBillBoard[0];
}


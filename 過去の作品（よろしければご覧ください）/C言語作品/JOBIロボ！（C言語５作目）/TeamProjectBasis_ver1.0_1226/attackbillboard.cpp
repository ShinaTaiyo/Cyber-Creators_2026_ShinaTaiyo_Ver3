//========================================================
//
//９月２６日：ハッカソンの基盤作り[Attackbillboard.cpp]
//Author:ShinaTaiyo
//
//=========================================================

#include "Attackbillboard.h"
#include "main.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "shadow.h"
#include "effect.h"
#include "wall.h"
#include "particle.h"
#include "player.h"
#include "enemy.h"
#include "Attackbillboard.h"
#include "battery.h"
#include "attackmodel.h"
#define ATTACKBILLBOARD_SCALE (10.0f)
//グローバル変数
PDIRECT3DVERTEXBUFFER9 g_pVtxBuffAttackbillboard = NULL;//頂点バッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureAttackbillboard[NUM_ATTACKBILLBOARD] = {};//テクスチャへのポインタ
D3DXVECTOR3 g_posAttackbillboard;//位置
//D3DXVECTOR3 g_rotAttackbillboard;//向き
D3DXMATRIX g_mtxWorldAttackbillboard;//ワールドマトリックス(float4*4)

Attackbillboard g_aAttackbillboard[MAX_ATTACKBILLBOARD];//弾の構造体情報

int g_nDelayAttackbillboard = 0;

//タイトル画面の初期化処理
void InitAttackbillboard(void)
{
	g_nDelayAttackbillboard = 0;

	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Attack\\piercing.png",
		&g_pTextureAttackbillboard[0]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Attack\\Thunder.png",
		&g_pTextureAttackbillboard[1]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Attack\\Fire.png",
		&g_pTextureAttackbillboard[2]);


	g_posAttackbillboard = D3DXVECTOR3(0.0f, 50.0f, 100.0f);//位置の初期化

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_ATTACKBILLBOARD,//頂点の数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffAttackbillboard,
		NULL);

	for (int nCntAttackbillboard = 0; nCntAttackbillboard < MAX_ATTACKBILLBOARD; nCntAttackbillboard++)
	{
		g_aAttackbillboard[nCntAttackbillboard].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//位置
		g_aAttackbillboard[nCntAttackbillboard].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//移動量
		g_aAttackbillboard[nCntAttackbillboard].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//向き
		g_aAttackbillboard[nCntAttackbillboard].Size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//大きさ
		g_aAttackbillboard[nCntAttackbillboard].nLife = 0;//体力
		g_aAttackbillboard[nCntAttackbillboard].fRWidth = 0.0f;//幅（半径）
		g_aAttackbillboard[nCntAttackbillboard].fRHeight = 0.0f;//高さ（半径）
		g_aAttackbillboard[nCntAttackbillboard].nType = 0;//種類
		g_aAttackbillboard[nCntAttackbillboard].bUse = false;//使用しているかどうか
		g_aAttackbillboard[nCntAttackbillboard].nNumEnemy = 0;//何番の敵が撃ってきているか
		g_aAttackbillboard[nCntAttackbillboard].fLength = 0.0f;//距離
		g_aAttackbillboard[nCntAttackbillboard].bOneFlag = false;
		g_aAttackbillboard[nCntAttackbillboard].bChangeFlag = false;//値を上下させる時に使う
		g_aAttackbillboard[nCntAttackbillboard].nMaxLife = 0;//最大体力

		g_aAttackbillboard[nCntAttackbillboard].nAnimationCnt = 0;//アニメーションカウント
		g_aAttackbillboard[nCntAttackbillboard].nAnimationPattern = 0;//アニメーションパターン
		g_aAttackbillboard[nCntAttackbillboard].nAnimationPatternH = 0;//縦のアニメーションパターン
		g_aAttackbillboard[nCntAttackbillboard].nIdxShadow = -1;//影のインデックス
	}

	VERTEX_3D* pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffAttackbillboard->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntAttackbillboard = 0; nCntAttackbillboard < MAX_ATTACKBILLBOARD; nCntAttackbillboard++)
	{
		//頂点座標の設定（初期位置）
		pVtx[0].pos = NULL_VECTOR3;
		pVtx[1].pos = NULL_VECTOR3;
		pVtx[2].pos = NULL_VECTOR3;
		pVtx[3].pos = NULL_VECTOR3;

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
	g_pVtxBuffAttackbillboard->Unlock();
}

//==============================
//タイトル画面の終了処理
//==============================
void UninitAttackbillboard(void)
{
	//テクスチャの破棄
	for (int nCntAttackbillboard = 0; nCntAttackbillboard < NUM_ATTACKBILLBOARD; nCntAttackbillboard++)
	{
		if (g_pTextureAttackbillboard[nCntAttackbillboard] != NULL)
		{
			g_pTextureAttackbillboard[nCntAttackbillboard]->Release();
			g_pTextureAttackbillboard[nCntAttackbillboard] = NULL;
		}
	}
	//頂点バッファの破棄
	if (g_pVtxBuffAttackbillboard != NULL)
	{
		g_pVtxBuffAttackbillboard->Release();
		g_pVtxBuffAttackbillboard = NULL;
	}
}
//============================
//タイトル画面の更新処理
//============================
void UpdateAttackbillboard(void)
{
	VERTEX_3D* pVtx;
	Player* pPlayer = GetPlayer();

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffAttackbillboard->Lock(0, 0, (void**)&pVtx, 0);

	g_nDelayAttackbillboard++;

	for (int nCntAttackbillboard = 0; nCntAttackbillboard < MAX_ATTACKBILLBOARD; nCntAttackbillboard++)
	{
		if (g_aAttackbillboard[nCntAttackbillboard].bUse == true)
		{
			g_aAttackbillboard[nCntAttackbillboard].nAnimationCnt++;

		    //=========================================
		    // 敵とプレイヤーの距離を取る。
		    //=========================================
		    float fLength = sqrtf((g_aAttackbillboard[nCntAttackbillboard].pos.x - pPlayer->pos.x) * (g_aAttackbillboard[nCntAttackbillboard].pos.x - pPlayer->pos.x) +
				(g_aAttackbillboard[nCntAttackbillboard].pos.z - pPlayer->pos.z) * (g_aAttackbillboard[nCntAttackbillboard].pos.z - pPlayer->pos.z)) / 2;

			g_aAttackbillboard[nCntAttackbillboard].fVXaim = pPlayer->pos.x - g_aAttackbillboard[nCntAttackbillboard].pos.x;
			g_aAttackbillboard[nCntAttackbillboard].fVYaim = pPlayer->pos.y - g_aAttackbillboard[nCntAttackbillboard].pos.y;
			g_aAttackbillboard[nCntAttackbillboard].fVZaim = pPlayer->pos.z - g_aAttackbillboard[nCntAttackbillboard].pos.z;

			//=========================================
			//汎用的に使う乱数値
			//=========================================
			float fPurposeRandRot = float(rand() % 200 + 1) / 100.0f;
			float fPurposeRandSpeed = float(rand() % 20 + 1);

			switch (g_aAttackbillboard[nCntAttackbillboard].nType)
			{
			case ATTACKBILLBOARD00_STUB:
				//==========================
				//アニメーション処理
				//==========================
				if (g_aAttackbillboard[nCntAttackbillboard].nAnimationCnt % 5 == 0)
				{
					g_aAttackbillboard[nCntAttackbillboard].nAnimationPattern++;

					if (g_aAttackbillboard[nCntAttackbillboard].nAnimationPattern == 4)
					{
						g_aAttackbillboard[nCntAttackbillboard].nAnimationPattern = 0;
					}
				}

				//テクスチャ座標の設定（初期設定）
				pVtx[0].tex = D3DXVECTOR2(0.0f + 0.25f * g_aAttackbillboard[nCntAttackbillboard].nAnimationPattern,0.0f);
				pVtx[1].tex = D3DXVECTOR2(0.25f + 0.25f * g_aAttackbillboard[nCntAttackbillboard].nAnimationPattern,0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f + 0.25f * g_aAttackbillboard[nCntAttackbillboard].nAnimationPattern,1.0f );
				pVtx[3].tex = D3DXVECTOR2(0.25f + 0.25f * g_aAttackbillboard[nCntAttackbillboard].nAnimationPattern,1.0f);

				if (g_aAttackbillboard[nCntAttackbillboard].pos.y + g_aAttackbillboard[nCntAttackbillboard].fRHeight >= pPlayer->pos.y &&
					g_aAttackbillboard[nCntAttackbillboard].pos.y - g_aAttackbillboard[nCntAttackbillboard].fRHeight <= pPlayer->pos.y + pPlayer->Size.y &&
					fLength < g_aAttackbillboard[nCntAttackbillboard].fRWidth)
				{
					for (int nCntParticle = 0; nCntParticle < 10; nCntParticle++)
					{
						float fRandRot = float(rand() % 200 + 1) / 10;
						float fRandSpeed = float(rand() % 20 + 1);
						float fRandMove = float(rand() % 40 - 20) / 2;

						SetParticle(PARTICLETYPE_NORMAL, 200, 10.0f, 0.4f,pPlayer->pos,
							D3DXVECTOR3(sinf(-D3DX_PI * fRandRot) * fRandSpeed, fRandMove, cosf(-D3DX_PI * fRandRot) * fRandSpeed),
							D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 0.0f, PARTICLETEX_NORMAL,0);
					}

					AttackHitPlayer(30);
				}


				break;

			case ATTACKBILLBORAD02_FIRE:

				if (g_nDelayAttackbillboard % 10 == 0)
				{
					SetParticle(PARTICLETYPE_FIRE, 60, 5.0f, 0.05f, g_aAttackbillboard[nCntAttackbillboard].pos,
						D3DXVECTOR3(sinf(D3DX_PI * fPurposeRandRot) * fPurposeRandSpeed, fPurposeRandSpeed, cosf(D3DX_PI * fPurposeRandRot) * fPurposeRandSpeed),
						D3DXCOLOR(1.0f, 0.647f, 0.0f, 1.0f), 0.0f, PARTICLETEX_NORMAL,0);
				}
				//==========================
				//アニメーション処理
				//==========================
				if (g_aAttackbillboard[nCntAttackbillboard].nAnimationCnt % 4 == 0)
				{
					g_aAttackbillboard[nCntAttackbillboard].nAnimationPattern++;

					if (g_aAttackbillboard[nCntAttackbillboard].nAnimationPattern == 8)
					{
						g_aAttackbillboard[nCntAttackbillboard].nAnimationPattern = 0;
						g_aAttackbillboard[nCntAttackbillboard].nAnimationPatternH++;

						if (g_aAttackbillboard[nCntAttackbillboard].nAnimationPatternH == 2)
						{
							g_aAttackbillboard[nCntAttackbillboard].nAnimationPatternH = 0;
						}
					}
				}

				//テクスチャ座標の設定（初期設定）
				pVtx[0].tex = D3DXVECTOR2(0.0f + 0.125f * g_aAttackbillboard[nCntAttackbillboard].nAnimationPattern, 0.0f + 0.5f * g_aAttackbillboard[nCntAttackbillboard].nAnimationPatternH);
				pVtx[1].tex = D3DXVECTOR2(0.125f + 0.125f * g_aAttackbillboard[nCntAttackbillboard].nAnimationPattern, 0.0f + 0.5f * g_aAttackbillboard[nCntAttackbillboard].nAnimationPatternH);
				pVtx[2].tex = D3DXVECTOR2(0.0f + 0.125f * g_aAttackbillboard[nCntAttackbillboard].nAnimationPattern, 0.5f + 0.5f * g_aAttackbillboard[nCntAttackbillboard].nAnimationPatternH);
				pVtx[3].tex = D3DXVECTOR2(0.125f + 0.125f * g_aAttackbillboard[nCntAttackbillboard].nAnimationPattern, 0.5f + 0.5f * g_aAttackbillboard[nCntAttackbillboard].nAnimationPatternH);

				if (g_aAttackbillboard[nCntAttackbillboard].pos.y + g_aAttackbillboard[nCntAttackbillboard].fRHeight >= pPlayer->pos.y &&
					g_aAttackbillboard[nCntAttackbillboard].pos.y - g_aAttackbillboard[nCntAttackbillboard].fRHeight <= pPlayer->pos.y + pPlayer->Size.y &&
					fLength < g_aAttackbillboard[nCntAttackbillboard].fRWidth / 2.0f)
				{
					for (int nCntParticle = 0; nCntParticle < 10; nCntParticle++)
					{
						float fRandRot = float(rand() % 200 + 1) / 10;
						float fRandSpeed = float(rand() % 20 + 1);
						float fRandMove = float(rand() % 40 - 20) / 2;

						SetParticle(PARTICLETYPE_NORMAL, 60, 6.0f, 0.4f, pPlayer->pos,
							D3DXVECTOR3(sinf(-D3DX_PI * fRandRot) * fRandSpeed, fRandMove, cosf(-D3DX_PI * fRandRot) * fRandSpeed),
							D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 0.0f, PARTICLETEX_NORMAL,0);
					}
					AttackHitPlayer(25);
				}

				break;
			}

			g_aAttackbillboard[nCntAttackbillboard].nLife -= 1;

			g_aAttackbillboard[nCntAttackbillboard].pos += g_aAttackbillboard[nCntAttackbillboard].move;

			if (g_aAttackbillboard[nCntAttackbillboard].nLife <= 0)
			{
				g_aAttackbillboard[nCntAttackbillboard].bUse = false;//体力が０になったらfalseにする
			}

			SetPositionShadow(g_aAttackbillboard[nCntAttackbillboard].nIdxShadow, D3DXVECTOR3(g_aAttackbillboard[nCntAttackbillboard].pos.x, g_aAttackbillboard[nCntAttackbillboard].pos.y - g_aAttackbillboard[nCntAttackbillboard].fRHeight, g_aAttackbillboard[nCntAttackbillboard].pos.z));

			if (g_aAttackbillboard[nCntAttackbillboard].bUse == false)
			{
				KillShadow(g_aAttackbillboard[nCntAttackbillboard].nIdxShadow);
			}
		}

		pVtx += 4;//ポインタを進める
	}

	//頂点バッファをアンロックする
	g_pVtxBuffAttackbillboard->Unlock();
}



//================================
// 描画処理
//================================
void DrawAttackbillboard(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	for (int nCntAttackbillboard = 0; nCntAttackbillboard < MAX_ATTACKBILLBOARD; nCntAttackbillboard++)
	{
		if (g_aAttackbillboard[nCntAttackbillboard].bUse == true)
		{
			if (g_aAttackbillboard[nCntAttackbillboard].pos.y >= 0.0f)
			{
				D3DXMATRIX mtxTrans;//計算用マトリックス

				D3DXMATRIX mtxView;//ビューマトリックス取得用

				//ワールドマトリックスの初期化
				D3DXMatrixIdentity(&g_mtxWorldAttackbillboard);

				//ビューマトリックスを取得
				pDevice->GetTransform(D3DTS_VIEW, &mtxView);

				//ポリゴンをカメラに対して正面に向ける
				D3DXMatrixInverse(&g_mtxWorldAttackbillboard, NULL, &mtxView);

				//逆行列を求める
				g_mtxWorldAttackbillboard._41 = 0.0f;
				g_mtxWorldAttackbillboard._42 = 0.0f;
				g_mtxWorldAttackbillboard._43 = 0.0f;

				//位置を反映
				D3DXMatrixTranslation(&mtxTrans, g_aAttackbillboard[nCntAttackbillboard].pos.x, g_aAttackbillboard[nCntAttackbillboard].pos.y, g_aAttackbillboard[nCntAttackbillboard].pos.z);
				D3DXMatrixMultiply(&g_mtxWorldAttackbillboard, &g_mtxWorldAttackbillboard, &mtxTrans);

				//===============================================================
				//描画の調整
				//==============================================================
				//ライトを無効にする
				pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

				//アルファテストを有効
				pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
				pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
				pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

				//ワールドマトリックスの設定
				pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldAttackbillboard);

				//頂点バッファをデータストリームに設定
				pDevice->SetStreamSource(0, g_pVtxBuffAttackbillboard, 0, sizeof(VERTEX_3D));//頂点バッファへのポインタと頂点情報の構造体のサイズ

				//頂点フォーマットの設定
				pDevice->SetFVF(FVF_VERTEX_3D);

				//テクスチャの設定
				pDevice->SetTexture(0, g_pTextureAttackbillboard[g_aAttackbillboard[nCntAttackbillboard].nType]);

				//ポリゴンの描画
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntAttackbillboard * 4, 2);//0は、描画する最初の頂点インデックスで、2は、描画するプリミティブ

				//============================
				//描画の調整をもとに戻す
				//============================

				//ライトを有効に戻す
				pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

				//アルファテストを無効に戻す
				pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
			}
		}
	}
}

//================================
// 設定処理
//================================
void SetAttackbillboard(int nType, int nLife, float fRWidth, float fRHeight, D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot, int nNumEnemy, float fLength)
{
	VERTEX_3D* pVtx;
	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffAttackbillboard->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntAttackbillboard = 0; nCntAttackbillboard < MAX_ATTACKBILLBOARD; nCntAttackbillboard++)
	{
		if (g_aAttackbillboard[nCntAttackbillboard].bUse == false)
		{
			g_aAttackbillboard[nCntAttackbillboard].bUse = true;//使用しているかどうか
			g_aAttackbillboard[nCntAttackbillboard].pos = pos;//位置
			g_aAttackbillboard[nCntAttackbillboard].move = move;//移動量
			g_aAttackbillboard[nCntAttackbillboard].rot = rot;//向き
			g_aAttackbillboard[nCntAttackbillboard].nLife = nLife;//体力
			g_aAttackbillboard[nCntAttackbillboard].fRWidth = fRWidth;//幅（半径）
			g_aAttackbillboard[nCntAttackbillboard].fRHeight = fRHeight;//高さ（半径）
			g_aAttackbillboard[nCntAttackbillboard].nType = nType;//種類
			g_aAttackbillboard[nCntAttackbillboard].nNumEnemy = nNumEnemy;
			g_aAttackbillboard[nCntAttackbillboard].fLength = fLength;
			g_aAttackbillboard[nCntAttackbillboard].bOneFlag = true;//一度だけ発動させる処理用フラグ
			g_aAttackbillboard[nCntAttackbillboard].bChangeFlag = false;//値を上下させる時に使う
			g_aAttackbillboard[nCntAttackbillboard].nIdxShadow = SetShadow();
			g_aAttackbillboard[nCntAttackbillboard].nMaxLife = nLife;//最大体力

			g_aAttackbillboard[nCntAttackbillboard].nAnimationCnt = 0;//アニメーションカウント
			g_aAttackbillboard[nCntAttackbillboard].nAnimationPattern = 0;//アニメーションパターン
			g_aAttackbillboard[nCntAttackbillboard].nAnimationPatternH = 0;//縦のアニメーションパターン

			//頂点座標の設定（初期位置）
			pVtx[0].pos = D3DXVECTOR3(-g_aAttackbillboard[nCntAttackbillboard].fRWidth, g_aAttackbillboard[nCntAttackbillboard].fRHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aAttackbillboard[nCntAttackbillboard].fRWidth, g_aAttackbillboard[nCntAttackbillboard].fRHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aAttackbillboard[nCntAttackbillboard].fRWidth, -g_aAttackbillboard[nCntAttackbillboard].fRHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aAttackbillboard[nCntAttackbillboard].fRWidth, -g_aAttackbillboard[nCntAttackbillboard].fRHeight, 0.0f);

			g_aAttackbillboard[nCntAttackbillboard].Size = D3DXVECTOR3(g_aAttackbillboard[nCntAttackbillboard].fRWidth * 2, 0.0f, g_aAttackbillboard[nCntAttackbillboard].fRWidth * 2);//大きさ（サイズ = 半径 × 2） 


			break;
		}
		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffAttackbillboard->Unlock();
}

Attackbillboard* GetAttackbillboard(void)//弾構造体の情報をゲットする
{
	return &g_aAttackbillboard[0];
}


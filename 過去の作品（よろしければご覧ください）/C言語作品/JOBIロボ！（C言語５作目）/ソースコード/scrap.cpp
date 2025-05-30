//=====================================================================================================================
//
//１２月２６日：チーム制作の基盤づくり[Scrap.cpp]
//Author:ShinaTaiyo
//
// 
// 注意：fRWidth,fRHeightは、中心点からの半径で大きさ、判定などを取っています。もしこの処理を使うなら、ご注意下さい。
// 
//======================================================================================================================

#include "scrap.h"
#include "main.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "shadow.h"
#include "effect.h"
#include "wall.h"
#include "particle.h"
#include "enemy.h"
#include "player.h"

#define Scrap_SCALE (10.0f)
#define SCRAPDIGIT (4)//スクラップの数表示の桁数
#define SCRAPUIRADIUS (20.0f)//スクラップの表示の数字の大きさ
//グローバル変数

//====================================
//スクラップビルボード表示用
//====================================
PDIRECT3DVERTEXBUFFER9 g_pVtxBuffScrap = NULL;//頂点バッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureScrap[NUM_SCRAP] = {};//テクスチャへのポインタ
D3DXVECTOR3 g_posScrap;//位置
D3DXMATRIX g_mtxWorldScrap;//ワールドマトリックス(float4*4)
Scrap g_aScrap[MAX_SCRAP];//スクラップの構造体情報
int g_nDelayScrap = 0;//スクラップの遅らせるカウント処理
//============================================================================================


//=================================
//スクラップゲージ用
//=================================


//============================
//初期化処理
//============================
void InitScrap(void)
{
	//================================
	//スクラップビルボードの処理
	//================================
	g_nDelayScrap = 0;
	
	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Scrap000_1.png",
		&g_pTextureScrap[SCRAPTYPE00_TIBIROBO]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Scrap_001.png",
		&g_pTextureScrap[SCRAPTYPE01_SWORD]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Scrap_002.png",
		&g_pTextureScrap[SCRAPTYPE02_BOOMERANG]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Scrap_003.png",
		&g_pTextureScrap[SCRAPTYPE03_SPEAKER]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Scrap_004.png",
		&g_pTextureScrap[SCRAPTYPE04_ALL]);

	g_posScrap = D3DXVECTOR3(0.0f, 50.0f, 100.0f);//位置の初期化

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_SCRAP,//頂点の数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffScrap,
		NULL);

	for (int nCntScrap = 0; nCntScrap < MAX_SCRAP; nCntScrap++)
	{
		g_aScrap[nCntScrap].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//位置
		g_aScrap[nCntScrap].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//移動量
		g_aScrap[nCntScrap].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//向き
		g_aScrap[nCntScrap].nLife = 0;//体力
		g_aScrap[nCntScrap].fRWidth = 0.0f;//幅（半径）
		g_aScrap[nCntScrap].fRHeight = 0.0f;//高さ（半径）
		g_aScrap[nCntScrap].nType = 0;//種類
		g_aScrap[nCntScrap].bUse = false;//使用しているかどうか
		g_aScrap[nCntScrap].nIdxShadow = -1;//影のインデックス
		g_aScrap[nCntScrap].nAnimationCnt = 0;//アニメーションカウント
		g_aScrap[nCntScrap].nAnimationPattern = 0;//アニメーションパターン
		g_aScrap[nCntScrap].fLength = 0.0f;//距離を測る。
		g_aScrap[nCntScrap].fXaim = 0.0f;//X方向のベクトル
		g_aScrap[nCntScrap].fYaim = 0.0f;//Y方向のベクトル
		g_aScrap[nCntScrap].fZaim = 0.0f;//Z方向のベクトル
		g_aScrap[nCntScrap].fVXZaim = 0.0f;//XとZの総合ベクトル
		g_aScrap[nCntScrap].fVYZaim = 0.0f;//YとZの総合ベクトル
		g_aScrap[nCntScrap].ScrapMoveType = SCRAPMOVETYPE_DIFFUSION;//スクラップの移動の仕方
		g_aScrap[nCntScrap].DecayMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//減衰用移動量

		//================================
		//３次元自機狙い変数
		//================================
		g_aScrap[nCntScrap].fVXaim3D;
		g_aScrap[nCntScrap].fVYaim3D;
		g_aScrap[nCntScrap].fVZaim3D;
		g_aScrap[nCntScrap].fVLaim3D;

		//===============================================================================
	}

	VERTEX_3D* pVtx;
	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffScrap->Lock(0, 0, (void**)&pVtx, 0);


	for (int nCntScrap = 0; nCntScrap < MAX_SCRAP; nCntScrap++)
	{
		//頂点座標の設定（初期位置）
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

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
	g_pVtxBuffScrap->Unlock();
	//==================================================================================================================================================================================
}

//==============================
//タイトル画面の終了処理
//==============================
void UninitScrap(void)
{
	//テクスチャの破棄
	for (int nCntScrap = 0; nCntScrap < NUM_SCRAP; nCntScrap++)
	{
		if (g_pTextureScrap[nCntScrap] != NULL)
		{
			g_pTextureScrap[nCntScrap]->Release();
			g_pTextureScrap[nCntScrap] = NULL;
		}
	}
	//頂点バッファの破棄
	if (g_pVtxBuffScrap != NULL)
	{
		g_pVtxBuffScrap->Release();
		g_pVtxBuffScrap = NULL;
	}
}
//============================
//タイトル画面の更新処理
//============================
void UpdateScrap(void)
{


	//======================================================================
	//※敵を使えない時の即席処理。同じ位置にスクラップを乱数で召喚
	//======================================================================
	/*if (g_nDelayScrap % 10 == 0)
	{
		int nSummonScrap = rand() % NUM_SCRAP;
		float fRandumRot = float(rand() % 200 + 1) / 100;
		float fRandumSpeed = float(rand() % 100 - 50) / 10;
		float fRandumMove = float(rand() % 21 - 10);
		float fRandumLength = float(rand() % 550 + 200);
		SetScrap(nSummonScrap, GENERALLYSCRAPLIFE, SCRAPRADIUSSIZE, SCRAPRADIUSSIZE,
			D3DXVECTOR3(0.0f,SCRAPRADIUSSIZE,0.0f)
			,NULL_VECTOR3,
			D3DXVECTOR3(sinf(-D3DX_PI * fRandumRot) * fRandumSpeed, 0.0f, cosf(-D3DX_PI * fRandumRot) + fRandumSpeed));
	}*/
	//=============================================================================

	//=======================================
	//スクラップビルボード処理
	//=======================================
	VERTEX_3D* pVtx;
	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffScrap->Lock(0, 0, (void**)&pVtx, 0);

	Player* pPlayer = GetPlayer();

	for (int nCntScrap = 0; nCntScrap < MAX_SCRAP; nCntScrap++)
	{
		if (g_aScrap[nCntScrap].bUse == true)
		{
			g_aScrap[nCntScrap].nLife -= 1;//体力を減らし続ける。
			g_aScrap[nCntScrap].nAnimationCnt++;//アニメーションカウント

			if (g_aScrap[nCntScrap].nLife <= 0)
			{
				g_aScrap[nCntScrap].bUse = false;//体力が０以下になったらfalseにする
			}

			//=================================================
			//スクラップ獲得処理
			//=================================================
			if (g_aScrap[nCntScrap].pos.x + g_aScrap[nCntScrap].fRWidth >= pPlayer->pos.x - pPlayer->Size.x / 2 &&
				g_aScrap[nCntScrap].pos.x - g_aScrap[nCntScrap].fRWidth <= pPlayer->pos.x + pPlayer->Size.x / 2 &&
				g_aScrap[nCntScrap].pos.y + g_aScrap[nCntScrap].fRHeight >= pPlayer->pos.y &&
				g_aScrap[nCntScrap].pos.y - g_aScrap[nCntScrap].fRHeight <= pPlayer->pos.y + pPlayer->Size.y &&
				g_aScrap[nCntScrap].pos.z + g_aScrap[nCntScrap].fRWidth >= pPlayer->pos.z - pPlayer->Size.z / 2 &&
				g_aScrap[nCntScrap].pos.z - g_aScrap[nCntScrap].fRWidth <= pPlayer->pos.z + pPlayer->Size.z / 2)
			{
				switch (g_aScrap[nCntScrap].nType)
				{
				case SCRAPTYPE00_TIBIROBO:
					AddScrapPlayer(SCRAPTYPE00_TIBIROBO,1);//ちびロボ強化スクラップ獲得＋１
					break;
				case SCRAPTYPE01_SWORD:
					AddScrapPlayer(SCRAPTYPE01_SWORD, 1);//剣攻撃強化スクラップ獲得＋１
					break;
				case SCRAPTYPE02_BOOMERANG:
					AddScrapPlayer(SCRAPTYPE02_BOOMERANG, 1);//ブーメラン攻撃強化スクラップ獲得＋１
					break;
				case SCRAPTYPE03_SPEAKER:
					AddScrapPlayer(SCRAPTYPE03_SPEAKER, 1);//スピーカー攻撃強化スクラップ獲得＋１
					break;
				case SCRAPTYPE04_ALL:
					AddScrapPlayer(SCRAPTYPE04_ALL, 1);//全ステータス強化スクラップ獲得＋１
					break;
				}

				g_aScrap[nCntScrap].bUse = false;
			}
			//======================================================================================================================================


			//================================================
			//テクスチャアニメーション処理
			//================================================
			if (g_aScrap[nCntScrap].nAnimationCnt % 5 == 0)
			{
				g_aScrap[nCntScrap].nAnimationPattern++;

				if (g_aScrap[nCntScrap].nAnimationPattern >= 5)
				{
					g_aScrap[nCntScrap].nAnimationPattern = 0;
				}
			}
			pVtx[0].tex = D3DXVECTOR2(0.0f + 0.2f * g_aScrap[nCntScrap].nAnimationPattern, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.2f + 0.2f * g_aScrap[nCntScrap].nAnimationPattern, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f + 0.2f * g_aScrap[nCntScrap].nAnimationPattern, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.2f + 0.2f * g_aScrap[nCntScrap].nAnimationPattern, 1.0f);
			//=========================================================================================================

			//===================================================
		    //移動処理
			//===================================================
		
			switch (g_aScrap[nCntScrap].ScrapMoveType)
			{
			    //=========================================
				//拡散処理（MOVETYPE_DIFFUSION)
				//=========================================
			case SCRAPMOVETYPE_DIFFUSION:

				if (g_aScrap[nCntScrap].move.x >= -0.01f && g_aScrap[nCntScrap].move.x <= 0.01f &&
					g_aScrap[nCntScrap].move.y >= -0.01f && g_aScrap[nCntScrap].move.y <= 0.01f &&
					g_aScrap[nCntScrap].move.z >= -0.01f && g_aScrap[nCntScrap].move.z <= 0.01f)
				{
					g_aScrap[nCntScrap].ScrapMoveType = SCRAPMOVETYPE_NORMAL;//移動量が０に近づいたので、移動タイプを変更
				}

				//移動量の減衰
				g_aScrap[nCntScrap].DecayMove.x = (0.0f - g_aScrap[nCntScrap].move.x) * 0.03f;
				g_aScrap[nCntScrap].DecayMove.y = (0.0f - g_aScrap[nCntScrap].move.y) * 0.03f;
				g_aScrap[nCntScrap].DecayMove.z = (0.0f - g_aScrap[nCntScrap].move.z) * 0.03f;

				
				g_aScrap[nCntScrap].move += g_aScrap[nCntScrap].DecayMove;//少しづつ移動量を減らす

			
				break;
				//===================================================================================================================
				

				//========================================
				//自機狙い処理(MOVETYPE_NORMAL)
				//========================================
			case SCRAPMOVETYPE_NORMAL:
				
				//==========================
	            //３次元自機狙い処理
	            //==========================
				g_aScrap[nCntScrap].fVXaim3D = pPlayer->pos.x - g_aScrap[nCntScrap].pos.x;
				g_aScrap[nCntScrap].fVYaim3D = pPlayer->pos.y - g_aScrap[nCntScrap].pos.y;
				g_aScrap[nCntScrap].fVZaim3D = pPlayer->pos.z - g_aScrap[nCntScrap].pos.z;

				D3DXVECTOR3 BulletMove = NULL_VECTOR3;

				g_aScrap[nCntScrap].fVLaim3D = sqrtf((g_aScrap[nCntScrap].fVXaim3D * g_aScrap[nCntScrap].fVXaim3D) + 
					(g_aScrap[nCntScrap].fVYaim3D * g_aScrap[nCntScrap].fVYaim3D)
					+ (g_aScrap[nCntScrap].fVZaim3D * g_aScrap[nCntScrap].fVZaim3D));

				BulletMove.x = g_aScrap[nCntScrap].fVXaim3D / g_aScrap[nCntScrap].fVLaim3D * 1.0f;
				BulletMove.y = g_aScrap[nCntScrap].fVYaim3D / g_aScrap[nCntScrap].fVLaim3D * 1.0f;
				BulletMove.z = g_aScrap[nCntScrap].fVZaim3D / g_aScrap[nCntScrap].fVLaim3D * 1.0f;

				//===========================================================================================================

				//==================================================================
		        //プレイヤーのスクラップ吸収範囲に入ったら素早く回収する
		        //==================================================================
				float fLength = sqrtf((pPlayer->pos.x - g_aScrap[nCntScrap].pos.x) * (pPlayer->pos.x - g_aScrap[nCntScrap].pos.x) +
					(pPlayer->pos.y - g_aScrap[nCntScrap].pos.y) * (pPlayer->pos.y - g_aScrap[nCntScrap].pos.y) +
					(pPlayer->pos.z - g_aScrap[nCntScrap].pos.z) * (pPlayer->pos.z - g_aScrap[nCntScrap].pos.z)) / 3.0f;

				if (fLength < pPlayer->fSuctionScrapRange)
				{
					BulletMove.x = g_aScrap[nCntScrap].fVXaim3D / g_aScrap[nCntScrap].fVLaim3D * 15.0f;
					BulletMove.y = g_aScrap[nCntScrap].fVYaim3D / g_aScrap[nCntScrap].fVLaim3D * 15.0f;
					BulletMove.z = g_aScrap[nCntScrap].fVZaim3D / g_aScrap[nCntScrap].fVLaim3D * 15.0f;
				}

				g_aScrap[nCntScrap].move = BulletMove;
				//==================================================================================================================================


				break;
				//=========================================================================================================

				
			}


		

			//移動
			g_aScrap[nCntScrap].pos += g_aScrap[nCntScrap].move;

		}
	

		pVtx += 4;//ポインタを進める
	}

	//頂点バッファをアンロックする
	g_pVtxBuffScrap->Unlock();

	//================================================================================================================================================================================
}



//================================
//描画処理
//================================
void DrawScrap(void)
{

	//=========================================================
	//スクラップビルボード処理
    //=========================================================
	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	for (int nCntScrap = 0; nCntScrap < MAX_SCRAP; nCntScrap++)
	{
		if (g_aScrap[nCntScrap].bUse == true)
		{
			D3DXMATRIX mtxTrans;//計算用マトリックス

			D3DXMATRIX mtxView;//ビューマトリックス取得用

			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_mtxWorldScrap);

			//ビューマトリックスを取得
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//ポリゴンをカメラに対して正面に向ける
			D3DXMatrixInverse(&g_mtxWorldScrap, NULL, &mtxView);

			//逆行列を求める
			g_mtxWorldScrap._41 = 0.0f;
			g_mtxWorldScrap._42 = 0.0f;
			g_mtxWorldScrap._43 = 0.0f;

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aScrap[nCntScrap].pos.x, g_aScrap[nCntScrap].pos.y, g_aScrap[nCntScrap].pos.z);
			D3DXMatrixMultiply(&g_mtxWorldScrap, &g_mtxWorldScrap, &mtxTrans);

			//=============================
			//描画の調整
			//=============================
			//ライトを無効にする
			pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

			

			//アルファテストを有効
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
			pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
			pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

			//===============================================================

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldScrap);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffScrap, 0, sizeof(VERTEX_3D));//頂点バッファへのポインタと頂点情報の構造体のサイズ

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureScrap[g_aScrap[nCntScrap].nType]);
				
			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntScrap * 4, 2);//0は、描画する最初の頂点インデックスで、2は、描画するプリミティブ

			//============================
			//描画の調整をもとに戻す
			//============================

			//ライトを有効に戻す
			pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);


			//アルファテストを無効に戻す
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

			//=========================================================
		}
	}
}

//================================
//設定処理
//================================
void SetScrap(int nType, int nLife, float fRWidth, float fRHeight, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 DiffusionMove)
{

	VERTEX_3D* pVtx;
	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffScrap->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntScrap = 0; nCntScrap < MAX_SCRAP; nCntScrap++)
	{
		if (g_aScrap[nCntScrap].bUse == false)
		{
			g_aScrap[nCntScrap].bUse = true;//使用しているかどうか
			g_aScrap[nCntScrap].pos = pos;//位置
			g_aScrap[nCntScrap].move = DiffusionMove;//移動量
			g_aScrap[nCntScrap].ScrapMoveType = SCRAPMOVETYPE_DIFFUSION;//最初は拡散させるように移動させる

			g_aScrap[nCntScrap].rot = rot;//向き
			g_aScrap[nCntScrap].nLife = nLife;//体力
			g_aScrap[nCntScrap].fRWidth = fRWidth;//幅（半径）
			g_aScrap[nCntScrap].fRHeight = fRHeight;//高さ（半径）
			g_aScrap[nCntScrap].nType = nType;//種類
			g_aScrap[nCntScrap].nAnimationCnt = 0;//アニメーションカウント
			g_aScrap[nCntScrap].nAnimationPattern = 0;//アニメーションパターン
			g_aScrap[nCntScrap].fLength = 0.0f;
			g_aScrap[nCntScrap].fLength = 0.0f;//距離を測る。
			g_aScrap[nCntScrap].fXaim = 0.0f;//X方向のベクトル
			g_aScrap[nCntScrap].fYaim = 0.0f;//Y方向のベクトル
			g_aScrap[nCntScrap].fZaim = 0.0f;//Z方向のベクトル
			g_aScrap[nCntScrap].fVXZaim = 0.0f;//XとZの総合ベクトル
			g_aScrap[nCntScrap].fVYZaim = 0.0f;//YとZの総合ベクトル

			//スクラップの頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-g_aScrap[nCntScrap].fRWidth, g_aScrap[nCntScrap].fRHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aScrap[nCntScrap].fRWidth, g_aScrap[nCntScrap].fRHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aScrap[nCntScrap].fRWidth, -g_aScrap[nCntScrap].fRHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aScrap[nCntScrap].fRWidth, -g_aScrap[nCntScrap].fRHeight, 0.0f);

			break;
		}
		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffScrap->Unlock();
}

Scrap* GetScrap(void)//スクラップ構造体の情報をゲットする
{
	return &g_aScrap[0];
}

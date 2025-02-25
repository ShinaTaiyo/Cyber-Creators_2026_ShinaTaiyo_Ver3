//========================================================
//
//８月３０日：エフェクトもう一回見直す[score.cpp]
//Author:ShinaTaiyo
//
//=========================================================

#include "main.h"
#include "damage.h"
#include "input.h"
#include "explosion.h"
#include "enemy.h"
#include "player.h"
#include "health.h"
#include "result.h"

#define MAX_DAMAGE (1024)//ダメージ表記の最大数
#define DAMAGE_DIGIT (3) //一回当たりのダメージの最大桁数
#define DAMAGE_SCALE_X (10)
#define DAMAGE_SCALE_Y (5)
#define DAMAGE_SCALE (100)

#define DAMAGE_RADIUS (10.0f)


//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureDamage= NULL;//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffDamage = NULL;//弾の情報
Damage g_aDamage[MAX_DAMAGE][DAMAGE_DIGIT];
//float g_fXMove;//X座標の移動
//float g_fYMove;//Y座標の移動
D3DXVECTOR3 g_posDamage;//位置
D3DXVECTOR3 g_moveDamage;//移動量

//D3DXVECTOR3 g_rotPl;//向き
float g_fDAMAGE_SCALE=0;





//弾の初期化処理
void InitDamage(void)
{
	ResultType* pResultType = GetResultType();
		

	/*if (pResultType->nResultType == false)
	{*/
	
	//}
	//g_moveDamage = D3DXVECTOR3(0.0f, -10.0f, 0.0f);//移動量を初期化する

	LPDIRECT3DDEVICE9 pDevice;
	int	nCntDamage;
	int nCntDigit;
	//デバイスの取得
	pDevice = GetDevice();

	        D3DXCreateTextureFromFile(pDevice,
			"data\\number_001.png",
			&g_pTextureDamage);
	//テクスチャの読み込み（スコアのテクスチャを読み込む）
	

	/*D3DXCreateTextureFromFile(pDevice,
		"data\\lightbrueball_enemy.png",
		&g_pTextureDamage[0]);*/


	/*g_nScore = 0;*/

	//スコア情報の初期化
	for (nCntDamage = 0; nCntDamage < MAX_DAMAGE; nCntDamage++)
	{
		for (nCntDigit = 0; nCntDigit < DAMAGE_DIGIT; nCntDigit++)
		{
			g_aDamage[nCntDamage][nCntDigit].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
			g_aDamage[nCntDamage][nCntDigit].bUse = false;//使用していない状態にする

			g_aDamage[nCntDamage][nCntDigit].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aDamage[nCntDamage][nCntDigit].nDamagePosTexU = 0;
			g_aDamage[nCntDamage][nCntDigit].nLife = 0;
			g_aDamage[nCntDamage][nCntDigit].fDamageWrite = 0.0f;
		}
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_DAMAGE * DAMAGE_DIGIT,//４は必要な頂点数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffDamage,
		NULL);

	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffDamage->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntDamage = 0; nCntDamage < MAX_DAMAGE * DAMAGE_DIGIT; nCntDamage++)
	{
		//頂点座標の設定
	
		
			pVtx[0].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
			pVtx[1].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
			pVtx[2].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
			pVtx[3].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);

			/*	g_aDamage[nCntDamage].nLife = 1000;*/
		
			/*g_rotPl[nCntDamage] = 200;*/


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
			pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);
		

			pVtx += 4;//頂点データのポインタを４つ分進める。
		
	}
	//頂点バッファをアンロックする
	g_pVtxBuffDamage->Unlock();

	/*for (nCntDamage = 0; nCntDamage < MAX_DAMAGE; nCntDamage++)
	{
		SetScore(10);
	}*/
	/*g_mode = GetMode();*/

	/*if (g_mode == MODE_RESULT)
	{*/
	/*	g_nScore = g_nSaveScore;*/
	//}

	
	
}

//弾の終了処理
void UninitDamage(void)
{
	int nCntDamage;
	//テクスチャの破棄
	//for (nCntDamage = 0; nCntDamage < MAX_DAMAGE; nCntDamage++)
	//{
		if (g_pTextureDamage != NULL)
		{
			g_pTextureDamage->Release();
			g_pTextureDamage = NULL;
		}
	//}
	//頂点バッファの破棄
	if (g_pVtxBuffDamage != NULL)
	{
		g_pVtxBuffDamage->Release();
		g_pVtxBuffDamage = NULL;
	}
}

//弾の更新処理
void UpdateDamage(void)
{
	int nCntDamage;//ダメージ表記の数をカウントする
	int nCntDigit;//ダメージの最大桁数をカウントする

	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffDamage->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntDamage = 0; nCntDamage < MAX_DAMAGE; nCntDamage++)
	{
		//頂点座標の設定

		for (nCntDigit = 0; nCntDigit < DAMAGE_DIGIT; nCntDigit++)
		{
			if (g_aDamage[nCntDamage][nCntDigit].bUse == true)
			{

				g_aDamage[nCntDamage][nCntDigit].nLife-=1;//ダメージ表示時間を減らし続ける
				g_aDamage[nCntDamage][nCntDigit].a_col -= 0.01f;//アルファ値を変え続け、どんどんダメージ表記を透明にしていく処理

					pVtx[0].pos = D3DXVECTOR3(g_aDamage[nCntDamage][nCntDigit].pos.x - g_aDamage[nCntDamage][nCntDigit].fRadius - g_aDamage[nCntDamage][nCntDigit].fRadius * nCntDigit, g_aDamage[nCntDamage][nCntDigit].pos.y - g_aDamage[nCntDamage][nCntDigit].fRadius, 0.0f);
					pVtx[1].pos = D3DXVECTOR3(g_aDamage[nCntDamage][nCntDigit].pos.x + g_aDamage[nCntDamage][nCntDigit].fRadius - g_aDamage[nCntDamage][nCntDigit].fRadius * nCntDigit, g_aDamage[nCntDamage][nCntDigit].pos.y - g_aDamage[nCntDamage][nCntDigit].fRadius, 0.0f);
					pVtx[2].pos = D3DXVECTOR3(g_aDamage[nCntDamage][nCntDigit].pos.x - g_aDamage[nCntDamage][nCntDigit].fRadius - g_aDamage[nCntDamage][nCntDigit].fRadius * nCntDigit, g_aDamage[nCntDamage][nCntDigit].pos.y + g_aDamage[nCntDamage][nCntDigit].fRadius, 0.0f);
					pVtx[3].pos = D3DXVECTOR3(g_aDamage[nCntDamage][nCntDigit].pos.x + g_aDamage[nCntDamage][nCntDigit].fRadius - g_aDamage[nCntDamage][nCntDigit].fRadius * nCntDigit, g_aDamage[nCntDamage][nCntDigit].pos.y + g_aDamage[nCntDamage][nCntDigit].fRadius, 0.0f);



					//rhwの設定
				pVtx[0].rhw = 1.0f;
				pVtx[1].rhw = 1.0f;
				pVtx[2].rhw = 1.0f;
				pVtx[3].rhw = 1.0f;



				//頂点カラーの設定
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aDamage[nCntDamage][nCntDigit].a_col);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aDamage[nCntDamage][nCntDigit].a_col);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aDamage[nCntDamage][nCntDigit].a_col);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aDamage[nCntDamage][nCntDigit].a_col);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。


				//ダメージの動き
				g_aDamage[nCntDamage][nCntDigit].pos.x += g_aDamage[nCntDamage][nCntDigit].move.x;
				g_aDamage[nCntDamage][nCntDigit].pos.y += g_aDamage[nCntDamage][nCntDigit].move.y;

				if (g_aDamage[nCntDamage][nCntDigit].nLife < 1)
				{
					g_aDamage[nCntDamage][nCntDigit].bUse = false;
				}
			}
			pVtx += 4;//頂点データのポインタを４つ分進める。
		}
	}
	//頂点バッファをアンロックする
	g_pVtxBuffDamage->Unlock();
}

//弾の描画処理
void DrawDamage(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	int nCntDamage,nCntDigit;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffDamage, 0, sizeof(VERTEX_2D));//頂点バッファへのポインタと頂点情報の構造体のサイズ

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);



	for (nCntDamage = 0; nCntDamage < MAX_DAMAGE; nCntDamage++)
	{
		for (nCntDigit = 0; nCntDigit < DAMAGE_DIGIT; nCntDigit++)
		{

			if (g_aDamage[nCntDamage][nCntDigit].bUse == true)
			{
				//テクスチャの設定
				pDevice->SetTexture(0, g_pTextureDamage);

				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,//0は、描画する最初の頂点インデックスで、2は、描画するプリミティブ数
					nCntDigit * 4 + ( DAMAGE_DIGIT * 4 ) * nCntDamage,
					2);
			}
		}

	}


}

//スコアの設定処理
void SetDamage(int nDamage,int nLife,D3DXVECTOR3 move,D3DXVECTOR3 pos, float fRadius)
{

	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D* pVtx;

	int nCntDamage;
	int nCntDigit;

	int nGetDamage = nDamage;
	


	int nCnt;
	int nCntDamageDigit=0;//もしダメージ表記が使用されていなかったら、ダメージ表記の桁数分のカウントを開始する。
	//デバイスの取得
	pDevice = GetDevice();
	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffDamage->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntDamage = 0; nCntDamage < MAX_DAMAGE; nCntDamage++)
	{
		
		for (nCntDigit = 0; nCntDigit < DAMAGE_DIGIT; nCntDigit++)
		{
			if (g_aDamage[nCntDamage][nCntDigit].bUse == false)
			{
				
				g_aDamage[nCntDamage][nCntDigit].bUse = true;
				g_aDamage[nCntDamage][nCntDigit].nLife = nLife;
				g_aDamage[nCntDamage][nCntDigit].move = move;
				g_aDamage[nCntDamage][nCntDigit].pos = pos;
				g_aDamage[nCntDamage][nCntDigit].fRadius = fRadius;
				g_aDamage[nCntDamage][nCntDigit].a_col = 0.8f;

				/*	for (nCnt = 0; nCnt < DAMAGE_DIGIT; nCnt++) 
					{*/
	          
				g_aDamage[nCntDamage][nCntDigit].nDamagePosTexU = nGetDamage % 10;
				nGetDamage = nGetDamage / 10;
				//}



			/*	for (nCntDigit= 0;  nCntDigit< DAMAGE_DIGIT; nCntDigit++)
				{*/
				pVtx[0].tex = D3DXVECTOR2(0.0f + (0.1f * g_aDamage[nCntDamage][nCntDigit].nDamagePosTexU), 0.0f);
				pVtx[1].tex = D3DXVECTOR2(0.1f + (0.1f * g_aDamage[nCntDamage][nCntDigit].nDamagePosTexU), 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f + (0.1f * g_aDamage[nCntDamage][nCntDigit].nDamagePosTexU), 1.0f);
				pVtx[3].tex = D3DXVECTOR2(0.1f + (0.1f * g_aDamage[nCntDamage][nCntDigit].nDamagePosTexU), 1.0f);

				nCntDamageDigit++;
			}
			if (nCntDamageDigit >= 3)
			{
				break;//for文の終了
			}
			pVtx += 4;//頂点データのポインタを４つ分進める。
			
		}
		if (nCntDamageDigit >= 3)
		{
			break;//for文の終了
		}
		
	
	}
	//頂点バッファをアンロックする
	g_pVtxBuffDamage->Unlock();

	/*for (nCntDamage = 0; nCntDamage < MAX_DAMAGE; nCntDamage++)
	{
		SetScore(10);
	}*/
}



//===================================
//スコアの取得
//===================================
//int GetDamage(void)
//{
//	return g_nScore;
//}
//========================================================
//
//３月１４日：ボスの攻撃の時に警告表示を出したい[Warning.cpp]
//Author:ShinaTaiyo
//
//=========================================================

#include "Warning.h"
#include "main.h"
#include "boss.h"

//グローバル変数
PDIRECT3DVERTEXBUFFER9 g_pVtxBuffWarning = NULL;//頂点バッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureWarning = NULL;//テクスチャへのポインタ
Warning g_aWarning[MAX_WARNING];//構造体
int g_nDelayWarning = 0;//ディレイ

//=================================
//警告表示の初期化処理
//=================================
void InitWarning(void)
{
	g_nDelayWarning = 0;
	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Warning_000.png",
		&g_pTextureWarning);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_WARNING,//頂点の数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffWarning,
		NULL);

	for (int nCntWarning = 0; nCntWarning < MAX_WARNING; nCntWarning++)
	{
		g_aWarning[nCntWarning].bUse = true;//仕様状態
		g_aWarning[nCntWarning].pos = NULL_VECTOR3;//位置
		g_aWarning[nCntWarning].fRWidth = 0.0f;//横幅
		g_aWarning[nCntWarning].fRHeight = 0.0f;//高さ
		g_aWarning[nCntWarning].mtxWorld = {};//マトリックスワールド
		g_aWarning[nCntWarning].nLife = 0;//体力
		g_aWarning[nCntWarning].Size = NULL_VECTOR3;//サイズ
	
	}

	VERTEX_3D* pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffWarning->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntWarning = 0; nCntWarning < MAX_WARNING; nCntWarning++)
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
	g_pVtxBuffWarning->Unlock();
}

//==============================
//警告表示の終了処理
//==============================
void UninitWarning(void)
{
	//テクスチャの破棄
	if (g_pTextureWarning != NULL)
	{
		g_pTextureWarning->Release();
		g_pTextureWarning = NULL;
	}
	
	//頂点バッファの破棄
	if (g_pVtxBuffWarning != NULL)
	{
		g_pVtxBuffWarning->Release();
		g_pVtxBuffWarning = NULL;
	}
}
//============================
//警告表示の更新処理
//============================
void UpdateWarning(void)
{
	Boss* pBoss = GetBoss();

	g_nDelayWarning++;//ディレイ

	VERTEX_3D* pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffWarning->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntWarning = 0; nCntWarning < MAX_WARNING; nCntWarning++)
	{
		if (g_aWarning[nCntWarning].bUse == true)
		{
			g_aWarning[nCntWarning].nLife--;

			//===========================
			//10fごとに色を変える
			//===========================
			if (g_nDelayWarning % 10 == 0)
			{
				g_aWarning[nCntWarning].bColChange = g_aWarning[nCntWarning].bColChange ? false : true;
			}
			//=========================================================================================================


			if (g_aWarning[nCntWarning].bColChange == false)
			{
				//頂点カラーの設定
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			}
			else
			{
				//頂点カラーの設定
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

			}

			//====================
			//位置の設定
			//====================
			g_aWarning[nCntWarning].pos = D3DXVECTOR3(pBoss->pos.x, pBoss->pos.y + pBoss->Size.y + 120.0f, pBoss->pos.z);

			//==================================================================================================================

			if (g_aWarning[nCntWarning].nLife <= 0)
			{
				g_aWarning[nCntWarning].bUse = false;
			}
		}

		pVtx += 4;
	}
	
	//頂点バッファをアンロックする
	g_pVtxBuffWarning->Unlock();
}



//================================
// 警告表示の描画処理
//================================
void DrawWarning(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	D3DXMATRIX mtxTrans;//計算用マトリックス

	D3DXMATRIX mtxView;//ビューマトリックス取得用

	for (int nCntWarning = 0; nCntWarning < MAX_WARNING; nCntWarning++)
	{
		if (g_aWarning[nCntWarning].bUse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aWarning[nCntWarning].mtxWorld);

			//ビューマトリックスを取得
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//ポリゴンをカメラに対して正面に向ける
			D3DXMatrixInverse(&g_aWarning[nCntWarning].mtxWorld, NULL, &mtxView);

			//逆行列を求める
			g_aWarning[nCntWarning].mtxWorld._41 = 0.0f;
			g_aWarning[nCntWarning].mtxWorld._42 = 0.0f;
			g_aWarning[nCntWarning].mtxWorld._43 = 0.0f;

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aWarning[nCntWarning].pos.x, g_aWarning[nCntWarning].pos.y, g_aWarning[nCntWarning].pos.z);
			D3DXMatrixMultiply(&g_aWarning[nCntWarning].mtxWorld, &g_aWarning[nCntWarning].mtxWorld, &mtxTrans);

			//===============================================================
			//描画の調整
			//==============================================================
			//ライトを無効にする
			pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

			//Zの比較方法変更。
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

			//Zバッファに書き込まない
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

			//アルファテストを有効
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
			pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
			pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);


			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aWarning[nCntWarning].mtxWorld);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0,g_pVtxBuffWarning, 0, sizeof(VERTEX_3D));//頂点バッファへのポインタと頂点情報の構造体のサイズ

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(0,g_pTextureWarning);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntWarning * 4, 2);//0は、描画する最初の頂点インデックスで、2は、描画するプリミティブ

			//============================
			//描画の調整をもとに戻す
			//============================

			//ライトを有効に戻す
			pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

			//Zの比較方法変更。
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

			//Zバッファに書き込む
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

			//アルファテストを無効に戻す
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
			
		}
	}
}

//================================
// 設定処理
//================================
void SetWarning(int nLife, float fRWidth, float fRHeight, D3DXVECTOR3 pos)
{
	VERTEX_3D* pVtx;
	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffWarning->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntWarning = 0; nCntWarning < MAX_WARNING; nCntWarning++)
	{
		if (g_aWarning[nCntWarning].bUse == false)
		{
			g_aWarning[nCntWarning].bUse = true;//使用状態
			g_aWarning[nCntWarning].pos = pos;//位置
			g_aWarning[nCntWarning].fRWidth = fRWidth;//横幅
			g_aWarning[nCntWarning].fRHeight = fRHeight;//高さ
			g_aWarning[nCntWarning].nLife = nLife;//体力

			//頂点座標の設定（初期位置）
			pVtx[0].pos = D3DXVECTOR3(-fRWidth, fRHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(fRWidth, fRHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-fRWidth, -fRHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(fRWidth, -fRHeight, 0.0f);

			break;
		}
		pVtx += 4;
	}
	

	//頂点バッファをアンロックする
	g_pVtxBuffWarning->Unlock();
}

//==============================
//構造体の取得
//==============================
Warning* GetWarning(void)
{
	return &g_aWarning[0];
}


//========================================================
//
// エフェクトの処理
// Author:ShinaTaiyo
//
//=========================================================
#include "effect.h"
#include "main.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "shadow.h"
#include "attackmodel.h"

//==============================
// グローバル変数
//==============================
PDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;//頂点バッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureEffect = NULL;//テクスチャへのポインタ
D3DXVECTOR3 g_posEffect;//位置
Effect g_aEffect[MAX_EFFECT];//弾の構造体情報

//==============================
// 初期化処理
//==============================
void InitEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\shadow000.jpg",
		&g_pTextureEffect);

	g_posEffect = D3DXVECTOR3(0.0f, 50.0f, 100.0f);//位置の初期化

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_EFFECT,//頂点の数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEffect,
		NULL);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		g_aEffect[nCntEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//位置
		g_aEffect[nCntEffect].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//移動量
		g_aEffect[nCntEffect].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//向き
		g_aEffect[nCntEffect].nLife = 0;//体力
		g_aEffect[nCntEffect].fRWidth = 0.0f;//幅（半径）
		g_aEffect[nCntEffect].fRHeight = 0.0f;//高さ（半径）
		g_aEffect[nCntEffect].bUse = false;//使用しているかどうか
		g_aEffect[nCntEffect].nIdxShadow = -1;//影のインデックス
		g_aEffect[nCntEffect].fWidth = 0.0f;//横幅
		g_aEffect[nCntEffect].fHeight = 0.0f;//横幅
		g_aEffect[nCntEffect].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);//色
		g_aEffect[nCntEffect].fRedution = 0.0f;//エフェクトを小さくするスピード
		g_aEffect[nCntEffect].fRotMove = 0.0f;//動く向きの数値を決める
		g_aEffect[nCntEffect].nMaxLife = 0;//体力最大値
		g_aEffect[nCntEffect].nNumAttackModel = 0;//エフェクトを読んだ攻撃モデルの番号
	}

	VERTEX_3D* pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		//頂点座標の設定（初期位置）
		pVtx[0].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fWidth, g_aEffect[nCntEffect].fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fWidth, g_aEffect[nCntEffect].fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fWidth, -g_aEffect[nCntEffect].fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fWidth, -g_aEffect[nCntEffect].fHeight, 0.0f);

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
	g_pVtxBuffEffect->Unlock();
}

//==============================
// 終了処理
//==============================
void UninitEffect(void)
{
	//テクスチャの破棄
	if (g_pTextureEffect != NULL)
	{
		g_pTextureEffect->Release();
		g_pTextureEffect = NULL;
	}
	//頂点バッファの破棄
	if (g_pVtxBuffEffect != NULL)
	{
		g_pVtxBuffEffect->Release();
		g_pVtxBuffEffect = NULL;
	}
}

//============================
// 更新処理
//============================
void UpdateEffect(void)
{
	VERTEX_3D* pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse)
		{
			g_aEffect[nCntEffect].col.a = (float)(g_aEffect[nCntEffect].nLife) / (float)(g_aEffect[nCntEffect].nMaxLife);
			g_aEffect[nCntEffect].nLife--;

			float fdiameter = g_aEffect[nCntEffect].fWidth / g_aEffect[nCntEffect].fHeight;

			g_aEffect[nCntEffect].fWidth -= g_aEffect[nCntEffect].fRedution * fdiameter;
			g_aEffect[nCntEffect].fHeight -= g_aEffect[nCntEffect].fRedution;
			g_aEffect[nCntEffect].pos += g_aEffect[nCntEffect].move;

			//頂点座標の設定（初期位置）
			pVtx[0].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fWidth, g_aEffect[nCntEffect].fHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fWidth, g_aEffect[nCntEffect].fHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fWidth, -g_aEffect[nCntEffect].fHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fWidth, -g_aEffect[nCntEffect].fHeight, 0.0f);

			//norの設定（初期設定）
			pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

			//頂点カラーの設定
			pVtx[0].col = g_aEffect[nCntEffect].col;//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
			pVtx[1].col = g_aEffect[nCntEffect].col;//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
			pVtx[2].col = g_aEffect[nCntEffect].col;//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
			pVtx[3].col = g_aEffect[nCntEffect].col;//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。

			if (g_aEffect[nCntEffect].fWidth <= 0.0f || 
				g_aEffect[nCntEffect].fHeight <= 0.0f || 
				g_aEffect[nCntEffect].col.a <= 0.0f || 
				g_aEffect[nCntEffect].nLife <= 0)
			{
				g_aEffect[nCntEffect].bUse = false;//体力が０になったらfalseにする
			}
		}

		pVtx += 4;//ポインタを進める
	}

	//頂点バッファをアンロックする
	g_pVtxBuffEffect->Unlock();
}

//================================
// 描画処理
//================================
void DrawEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	D3DXMATRIX mtxTrans;//計算用マトリックス
	D3DXMATRIX mtxView;//ビューマトリックス取得用

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aEffect[nCntEffect].mtxWorldEffect);

			//ビューマトリックスを取得
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//ポリゴンをカメラに対して正面に向ける
			D3DXMatrixInverse(&g_aEffect[nCntEffect].mtxWorldEffect, NULL, &mtxView);

			//逆行列を求める
			g_aEffect[nCntEffect].mtxWorldEffect._41 = 0.0f;
			g_aEffect[nCntEffect].mtxWorldEffect._42 = 0.0f;
			g_aEffect[nCntEffect].mtxWorldEffect._43 = 0.0f;

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aEffect[nCntEffect].pos.x, g_aEffect[nCntEffect].pos.y, g_aEffect[nCntEffect].pos.z);
			D3DXMatrixMultiply(&g_aEffect[nCntEffect].mtxWorldEffect, &g_aEffect[nCntEffect].mtxWorldEffect, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aEffect[nCntEffect].mtxWorldEffect);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_3D));//頂点バッファへのポインタと頂点情報の構造体のサイズ

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//ライトを無効にする
			pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

			////Zの比較方法変更。
			//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

			//Zバッファに書き込まない
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

			//アルファテストを有効(アルファ値が０より大きい場合に描画する）
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
			pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
			pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

			//aブレンディングを加算合成に設定
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);


			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureEffect);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEffect * 4, 2);//0は、描画する最初の頂点インデックスで、2は、描画するプリミティブ

			//ライトを有効に戻す
			pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

			////Zの比較方法変更。
			//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

			//Zバッファに書き込む
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

			//アルファテスト無効に戻す
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

			//aブレンディングを元に戻す
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		}
	}
}

//================================
// 設定処理
//================================
void SetEffect(int nType, int nLife, float fWidth, float fHeight, float fRedution, D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRotMove, int nNumModel)
{
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (!g_aEffect[nCntEffect].bUse)
		{
			g_aEffect[nCntEffect].bUse = true;//使用する
			g_aEffect[nCntEffect].pos = pos;//位置
			g_aEffect[nCntEffect].move = move;//移動量
			g_aEffect[nCntEffect].col = col;//色
			g_aEffect[nCntEffect].fRotMove = fRotMove;//動く向きの数値を決める
			g_aEffect[nCntEffect].nType = nType;//種類
			g_aEffect[nCntEffect].nLife = nLife;//体力
			g_aEffect[nCntEffect].nMaxLife = nLife;//体力最大値
			g_aEffect[nCntEffect].nNumAttackModel = nNumModel;//エフェクトを読んだ攻撃モデルの番号
			g_aEffect[nCntEffect].fWidth = fWidth;//横幅
			g_aEffect[nCntEffect].fHeight = fHeight;//高さ
			g_aEffect[nCntEffect].fRedution = fRedution;//エフェクトを小さくするスピード

			break;
		}
	}
}

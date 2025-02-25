//========================================================
//
//９月２６日：ハッカソンの基盤作り[Effect.cpp]
//Author:ShinaTaiyo
//
//=========================================================

#include "effect.h"
#include "main.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "shadow.h"


#define EFFECT_SCALE (10.0f)
//グローバル変数
PDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;//頂点バッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureEffect = NULL;//テクスチャへのポインタ
D3DXVECTOR3 g_posEffect;//位置
//D3DXVECTOR3 g_rotEffect;//向き
D3DXMATRIX g_mtxWorldEffect;//ワールドマトリックス(float4*4)

Effect g_aEffect[MAX_EFFECT];//弾の構造体情報



//タイトル画面の初期化処理
void InitEffect(void)
{


	//bFadeFlag = false;

	//g_nCntTime = 0;

	//g_nColorChange = 255;//カラーの初期化（白色）

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
		g_aEffect[nCntEffect].nType = 0;//種類
		g_aEffect[nCntEffect].bUse = false;//使用しているかどうか
		g_aEffect[nCntEffect].nIdxShadow = -1;//影のインデックス
		g_aEffect[nCntEffect].fScale = 0.0f;//大きさ
		g_aEffect[nCntEffect].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);//色
		g_aEffect[nCntEffect].fRedution = 0.0f;//エフェクトを小さくするスピード
		g_aEffect[nCntEffect].fRotMove = 0.0f;//動く向きの数値を決める
	}




	////タイトル画面BGMの設定
	//PlaySound(SOUND_LABEL_BGM000);

	VERTEX_3D* pVtx;
	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);


	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		//頂点座標の設定（初期位置）
		pVtx[0].pos = D3DXVECTOR3(-EFFECT_SCALE, EFFECT_SCALE, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(EFFECT_SCALE, EFFECT_SCALE, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-EFFECT_SCALE, -EFFECT_SCALE, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(EFFECT_SCALE, -EFFECT_SCALE, 0.0f);

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
//タイトル画面の終了処理
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
//タイトル画面の更新処理
//============================
void UpdateEffect(void)
{
	VERTEX_3D* pVtx;
	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);


	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{

		if (g_aEffect[nCntEffect].bUse == true)
		{

			g_aEffect[nCntEffect].nLife -= 1;

			g_aEffect[nCntEffect].fScale -= g_aEffect[nCntEffect].fRedution;


			//頂点座標の設定（初期位置）
			pVtx[0].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fScale, g_aEffect[nCntEffect].fScale, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fScale, g_aEffect[nCntEffect].fScale, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fScale, -g_aEffect[nCntEffect].fScale, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fScale, -g_aEffect[nCntEffect].fScale, 0.0f);

			//頂点カラーの設定
			pVtx[0].col = g_aEffect[nCntEffect].col;
			pVtx[1].col = g_aEffect[nCntEffect].col;
			pVtx[2].col = g_aEffect[nCntEffect].col;
			pVtx[3].col = g_aEffect[nCntEffect].col;

			//テクスチャ座標の設定（初期設定）
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

			g_aEffect[nCntEffect].pos += g_aEffect[nCntEffect].move;

			if (g_aEffect[nCntEffect].nLife <= 0 || g_aEffect[nCntEffect].fScale <= 0.0f)
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
//タイトル画面の描画処理
//================================
void DrawEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

		//デバイスの取得
	pDevice = GetDevice();



	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == true)
		{
			D3DXMATRIX mtxTrans;//計算用マトリックス

			D3DXMATRIX mtxView;//ビューマトリックス取得用

			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_mtxWorldEffect);

			//ビューマトリックスを取得
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//ポリゴンをカメラに対して正面に向ける
			D3DXMatrixInverse(&g_mtxWorldEffect, NULL, &mtxView);

			//逆行列を求める
			g_mtxWorldEffect._41 = 0.0f;
			g_mtxWorldEffect._42 = 0.0f;
			g_mtxWorldEffect._43 = 0.0f;

			////向きを反映
			//D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotEffect.y, g_rotEffect.x, g_rotEffect.z);

			//D3DXMatrixMultiply(&g_mtxWorldEffect, &g_mtxWorldEffect, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aEffect[nCntEffect].pos.x, g_aEffect[nCntEffect].pos.y, g_aEffect[nCntEffect].pos.z);
			D3DXMatrixMultiply(&g_mtxWorldEffect, &g_mtxWorldEffect, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldEffect);


			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_3D));//頂点バッファへのポインタと頂点情報の構造体のサイズ

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//ライトを無効にする
			pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);


			//aブレンディングを加算合成に設定
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

			//アルファテストを有効(アルファ値が０より大きい場合に描画する）
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
			pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
			pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureEffect);


		

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEffect * 4, 2);//0は、描画する最初の頂点インデックスで、2は、描画するプリミティブ

			//ライトを有効に戻す
			pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

			//aブレンディングを元に戻す
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

			//アルファテスト無効に戻す
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		}
	}
}

void SetEffect(int nType, int nLife, float fScale, float fRedution, D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRotMove)
{
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == false)
		{
			g_aEffect[nCntEffect].bUse = true;//使用しているかどうか
			g_aEffect[nCntEffect].pos = pos;//位置
			g_aEffect[nCntEffect].move = move;//移動量
			g_aEffect[nCntEffect].col = col;//色
			g_aEffect[nCntEffect].fRotMove = fRotMove;//動く向きの数値を決める
			g_aEffect[nCntEffect].nLife = nLife;//体力
			g_aEffect[nCntEffect].nType = nType;//種類
			g_aEffect[nCntEffect].fScale = fScale;//大きさ
			g_aEffect[nCntEffect].fRedution = fRedution;//エフェクトを小さくするスピード
			break;
		}
	}
}

//========================================================
//
//９月２６日：ハッカソンの基盤作り[Emitter.cpp]
//Author:ShinaTaiyo
//
//=========================================================

#include "emitter.h"
#include "main.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "shadow.h"
#include "effect.h"
#include "AttackModel.h"

#define EMITTER_SCALE (10.0f)
//グローバル変数
PDIRECT3DVERTEXBUFFER9 g_pVtxBuffEmitter = NULL;//頂点バッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureEmitter = NULL;//テクスチャへのポインタ
D3DXVECTOR3 g_posEmitter;//位置
//D3DXVECTOR3 g_rotEmitter;//向き
D3DXMATRIX g_mtxWorldEmitter;//ワールドマトリックス(float4*4)

Emitter g_aEmitter[MAX_EMITTER];//弾の構造体情報



//タイトル画面の初期化処理
void InitEmitter(void)
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
		&g_pTextureEmitter);

	g_posEmitter = D3DXVECTOR3(0.0f, 50.0f, 100.0f);//位置の初期化

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_EMITTER,//頂点の数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEmitter,
		NULL);

	for (int nCntEmitter = 0; nCntEmitter < MAX_EMITTER; nCntEmitter++)
	{
		g_aEmitter[nCntEmitter].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//位置
		g_aEmitter[nCntEmitter].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//移動量
		g_aEmitter[nCntEmitter].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//向き
		g_aEmitter[nCntEmitter].nLife = 0;//体力
		g_aEmitter[nCntEmitter].fRWidth = 0.0f;//幅（半径）
		g_aEmitter[nCntEmitter].fRHeight = 0.0f;//高さ（半径）
		g_aEmitter[nCntEmitter].nType = 0;//種類
		g_aEmitter[nCntEmitter].bUse = false;//使用しているかどうか
		g_aEmitter[nCntEmitter].nIdxShadow = -1;//影のインデックス
		g_aEmitter[nCntEmitter].fScale = 0.0f;//大きさ
		g_aEmitter[nCntEmitter].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);//色
		g_aEmitter[nCntEmitter].fRedution = 0.0f;//エフェクトを小さくするスピード
		g_aEmitter[nCntEmitter].EmitterType = EMITTERTYPE_EFFECT;//エミッターで呼び出す処理の種類
		g_aEmitter[nCntEmitter].nSummonLife = 0;//呼び出す処理の体力
		g_aEmitter[nCntEmitter].SummonMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEmitter[nCntEmitter].fRotMove = 0.0f;//動く向きの数値を決める
		g_aEmitter[nCntEmitter].nNumAttack = -1;//エミッターを呼び出している攻撃の番号を格納する
	}




	////タイトル画面BGMの設定
	//PlaySound(SOUND_LABEL_BGM000);

	VERTEX_3D* pVtx;
	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEmitter->Lock(0, 0, (void**)&pVtx, 0);


	for (int nCntEmitter = 0; nCntEmitter < MAX_EMITTER; nCntEmitter++)
	{
		//頂点座標の設定（初期位置）
		pVtx[0].pos = D3DXVECTOR3(-EMITTER_SCALE, EMITTER_SCALE, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(EMITTER_SCALE, EMITTER_SCALE, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-EMITTER_SCALE, -EMITTER_SCALE, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(EMITTER_SCALE, -EMITTER_SCALE, 0.0f);

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
	g_pVtxBuffEmitter->Unlock();
}

//==============================
//タイトル画面の終了処理
//==============================
void UninitEmitter(void)
{
	//テクスチャの破棄
	if (g_pTextureEmitter != NULL)
	{
		g_pTextureEmitter->Release();
		g_pTextureEmitter = NULL;
	}
	//頂点バッファの破棄
	if (g_pVtxBuffEmitter != NULL)
	{
		g_pVtxBuffEmitter->Release();
		g_pVtxBuffEmitter = NULL;
	}
}
//============================
//タイトル画面の更新処理
//============================
void UpdateEmitter(void)
{
	VERTEX_3D* pVtx;
	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEmitter->Lock(0, 0, (void**)&pVtx, 0);


	for (int nCntEmitter = 0; nCntEmitter < MAX_EMITTER; nCntEmitter++)
	{

		if (g_aEmitter[nCntEmitter].bUse == true)
		{

			//体力を減らす
			g_aEmitter[nCntEmitter].nLife -= 1;

			if (g_aEmitter[nCntEmitter].nType == EMITTERTYPE_CURVE)
			{
				g_aEmitter[nCntEmitter].fRotMove += 0.005f;

				g_aEmitter[nCntEmitter].move.x = sinf(D3DX_PI * g_aEmitter[nCntEmitter].fRotMove) * 5.0f;
				g_aEmitter[nCntEmitter].move.z = cosf(D3DX_PI * g_aEmitter[nCntEmitter].fRotMove) * 5.0f;

				//移動処理
				g_aEmitter[nCntEmitter].pos.x += g_aEmitter[nCntEmitter].move.x;
				g_aEmitter[nCntEmitter].pos.y += g_aEmitter[nCntEmitter].move.y;
				g_aEmitter[nCntEmitter].pos.z += g_aEmitter[nCntEmitter].move.z;
			}
			

			

			if (g_aEmitter[nCntEmitter].nType == EMITTERTYPE_TORNADO)
			{
				AttackModel* pAttackModel = GetAttackModel();
				g_aEmitter[nCntEmitter].fRotMove += 0.1f;

				g_aEmitter[nCntEmitter].move.y += 0.001f;
				g_aEmitter[nCntEmitter].fScale += 0.01f;
				for (int nCntAttackModel = 0; nCntAttackModel < MAX_ATTACKMODEL; nCntAttackModel++, pAttackModel++)
				{
					if (pAttackModel->bUse == true && g_aEmitter[nCntEmitter].nNumAttack == nCntAttackModel)
					{
						g_aEmitter[nCntEmitter].pos.x = sinf(-D3DX_PI * g_aEmitter[nCntEmitter].fRotMove) * 50.0f + pAttackModel->pos.x;
						g_aEmitter[nCntEmitter].pos.z = cosf(-D3DX_PI * g_aEmitter[nCntEmitter].fRotMove) * 50.0f + pAttackModel->pos.z;

						if (pAttackModel->nLife <= 1)
						{
							g_aEmitter[nCntEmitter].bUse = false;
						}
					}
				}
			}

			//g_aEmitter[nCntEmitter].fScale -= g_aEmitter[nCntEmitter].fRedution;//大きさを小さくするスピード



			//エフェクトを呼び出す処理
			/*if (g_aEmitter[nCntEmitter].EmitterType == EMITTERTYPE_EFFECT)*/
				SetEffect(0,
					g_aEmitter[nCntEmitter].nSummonLife,
					g_aEmitter[nCntEmitter].fScale,
					g_aEmitter[nCntEmitter].fRedution,
					g_aEmitter[nCntEmitter].pos,
					D3DXVECTOR3(0.0f,0.0f,0.0f),
					g_aEmitter[nCntEmitter].col,
					0.0f);
			
				//移動処理
				g_aEmitter[nCntEmitter].pos.x += g_aEmitter[nCntEmitter].move.x;
				g_aEmitter[nCntEmitter].pos.y += g_aEmitter[nCntEmitter].move.y;
				g_aEmitter[nCntEmitter].pos.z += g_aEmitter[nCntEmitter].move.z;

			//使用状態をfalseにする
			if (g_aEmitter[nCntEmitter].nLife <= 0 || g_aEmitter[nCntEmitter].fScale <= 0.0f)
			{
				g_aEmitter[nCntEmitter].bUse = false;//体力が０になったらfalseにする
			}
		}

		pVtx += 4;//ポインタを進める
	}

	//頂点バッファをアンロックする
	g_pVtxBuffEmitter->Unlock();
}



//================================
//タイトル画面の描画処理
//================================
void DrawEmitter(void)
{

}

void SetEmitterEffect(int nType, int nLife, int nSummonLife, float fScale, float fRedution, D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRotMove, int nNumAttack)
{
	for (int nCntEmitter = 0; nCntEmitter < MAX_EMITTER; nCntEmitter++)
	{
		if (g_aEmitter[nCntEmitter].bUse == false)
		{
			g_aEmitter[nCntEmitter].bUse = true;//使用しているかどうか
			g_aEmitter[nCntEmitter].pos = pos;//位置
			g_aEmitter[nCntEmitter].move = move;//移動量
			g_aEmitter[nCntEmitter].col = col;//色
			g_aEmitter[nCntEmitter].nLife = nLife;//体力
			g_aEmitter[nCntEmitter].nType = nType;//種類
			g_aEmitter[nCntEmitter].fScale = fScale;//大きさ
			g_aEmitter[nCntEmitter].fRedution = fRedution;//エフェクトを小さくするスピード
			g_aEmitter[nCntEmitter].nSummonLife = nSummonLife;//呼び出す処理の体力
			g_aEmitter[nCntEmitter].fRotMove = fRotMove;//動く向きの数値を決める
			g_aEmitter[nCntEmitter].nNumAttack = nNumAttack;//エミッターを呼び出している攻撃の番号を格納する

			break;
		}
	}
}

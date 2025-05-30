//=======================================================================================================================================================================================================================
//
// フィールドの処理
// Author : Atsumu Kuboichi
//
//=======================================================================================================================================================================================================================
#include "field.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "game.h"

//=======================================================================================================================================================================================================================
// グローバル変数
//=======================================================================================================================================================================================================================

//==================
//フィールド
//==================
Field g_aField;	//情報構造体

//==================
//演出フィールド
//==================
PDIRECT3DVERTEXBUFFER9 g_pVtxBuffStagingField = NULL;		        //頂点バッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureStagingField[NUM_STAGINGFIELD] = {};	//テクスチャへのポインタ
StagingField g_aStagingField[MAX_STAGINGFIELD];                 //演出フィールドの構造体
//=======================================================================================================================================================================================================================
// 初期化処理
//=======================================================================================================================================================================================================================
void InitField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスの取得
	VERTEX_3D* pVtx;						//頂点情報へのポインタ

	//テクスチャの読み込み
	for (int nCntTex = 0; nCntTex < NUM_FIELD; nCntTex++)
	{
		D3DXCreateTextureFromFile(pDevice,
			FIELD_FILENAME[nCntTex],
			&g_aField.pTexture[nCntTex]);
	}

	//変数
	g_aField.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//位置
	g_aField.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//向き
	g_aField.fWidth = FIELD_WIDTH;					//横幅
	g_aField.fHeight = FIELD_HEIGHT;				//縦幅

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_aField.pVtxBuff,
		NULL);

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_aField.pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-g_aField.fWidth, 0.0f, g_aField.fHeight);
	pVtx[1].pos = D3DXVECTOR3(g_aField.fWidth, 0.0f, g_aField.fHeight);
	pVtx[2].pos = D3DXVECTOR3(-g_aField.fWidth, 0.0f, -g_aField.fHeight);
	pVtx[3].pos = D3DXVECTOR3(g_aField.fWidth, 0.0f, -g_aField.fHeight);

	//norの設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	g_aField.pVtxBuff->Unlock();
}

//=======================================================================================================================================================================================================================
// 終了処理
//=======================================================================================================================================================================================================================
void UninitField(void)
{
	//テクスチャの破棄
	for (int nCntTex = 0; nCntTex < NUM_FIELD; nCntTex++)
	{
		if (g_aField.pTexture[nCntTex] != NULL)
		{//テクスチャ情報が存在する場合
			g_aField.pTexture[nCntTex]->Release();
			g_aField.pTexture[nCntTex] = NULL;
		}
	}
	//頂点バッファの破棄
	if (g_aField.pVtxBuff != NULL)
	{//頂点情報が存在する場合
		g_aField.pVtxBuff->Release();
		g_aField.pVtxBuff = NULL;
	}
}

//=======================================================================================================================================================================================================================
// 更新処理
//=======================================================================================================================================================================================================================
void UpdateField(void)
{
	VERTEX_3D* pVtx;//頂点情報へのポインタ

	//ゲーム中
	if (!GetEditMode())
	{
		//ゲームステージ
		if (GetMode() != MODE_TUTORIAL)
		{
			g_aField.nType = 1;
			g_aField.fWidth = FIELD_WIDTH;	//横幅
			g_aField.fHeight = FIELD_HEIGHT;//縦幅
		}
		//ちびロボハウス内
		else
		{
			g_aField.nType = 0;
			g_aField.fWidth = TUTORIAL_SCALE;	//横幅
			g_aField.fHeight = TUTORIAL_SCALE;	//縦幅
		}
	}
	//エディタ中
	else
	{
		//ゲームステージ
		if (!GetHouse())
		{
			g_aField.nType = 1;
			g_aField.fWidth = FIELD_WIDTH;	//横幅
			g_aField.fHeight = FIELD_HEIGHT;//縦幅
		}
		//ちびロボハウス内
		else
		{
			g_aField.nType = 0;
			g_aField.fWidth = TUTORIAL_SCALE;	//横幅
			g_aField.fHeight = TUTORIAL_SCALE;	//縦幅
		}
	}

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_aField.pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の更新
	pVtx[0].pos = D3DXVECTOR3(-g_aField.fWidth, 0.0f, g_aField.fHeight);
	pVtx[1].pos = D3DXVECTOR3(g_aField.fWidth, 0.0f, g_aField.fHeight);
	pVtx[2].pos = D3DXVECTOR3(-g_aField.fWidth, 0.0f, -g_aField.fHeight);
	pVtx[3].pos = D3DXVECTOR3(g_aField.fWidth, 0.0f, -g_aField.fHeight);

	//頂点バッファをアンロックする
	g_aField.pVtxBuff->Unlock();
}

//=======================================================================================================================================================================================================================
// 描画処理
//=======================================================================================================================================================================================================================
void DrawField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;			//計算用マトリックス

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_aField.mtxworld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aField.rot.y, g_aField.rot.x, g_aField.rot.z);

	D3DXMatrixMultiply(&g_aField.mtxworld, &g_aField.mtxworld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_aField.pos.x, g_aField.pos.y, g_aField.pos.z);
	D3DXMatrixMultiply(&g_aField.mtxworld, &g_aField.mtxworld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_aField.mtxworld);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_aField.pVtxBuff, 0, sizeof(VERTEX_3D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_aField.pTexture[g_aField.nType]);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//=======================================================================================================================================================================================================================
// 情報の構造体の取得
//=======================================================================================================================================================================================================================
Field* GetField(void)
{
	return &g_aField;
}

void InitStagingField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスの取得
	VERTEX_3D* pVtx;						//頂点情報へのポインタ

	//テクスチャの読み込み
	for (int nCntType = 0; nCntType < NUM_STAGINGFIELD; nCntType++)
	{
		D3DXCreateTextureFromFile(pDevice,
		    STAGINGFIELD_FILENAME[nCntType],
			&g_pTextureStagingField[nCntType]);
	}

	//===========================
	//初期化
	//===========================
	for (int nCntStagingField = 0; nCntStagingField < MAX_STAGINGFIELD; nCntStagingField++)
	{
		g_aStagingField[nCntStagingField].pos = NULL_VECTOR3;//位置
		g_aStagingField[nCntStagingField].mtxworld = {};//マトリックスワールド
		g_aStagingField[nCntStagingField].nType = 0;//種類
		g_aStagingField[nCntStagingField].rot = NULL_VECTOR3;//向き
		g_aStagingField[nCntStagingField].fWidth = 0.0f;//横幅
		g_aStagingField[nCntStagingField].fHeight = 0.0f;//高さ
		g_aStagingField[nCntStagingField].bUse = false;//使用状態
		g_aStagingField[nCntStagingField].nLife = 0;//体力
		g_aStagingField[nCntStagingField].fRot = 0.0f;//向き
	}

	//=========================================================================================

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_STAGINGFIELD,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffStagingField,
		NULL);

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffStagingField->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntField = 0; nCntField < MAX_STAGINGFIELD; nCntField++)
	{
		//頂点座標の設定
		pVtx[0].pos = NULL_VECTOR3;
		pVtx[1].pos = NULL_VECTOR3;
		pVtx[2].pos = NULL_VECTOR3;
		pVtx[3].pos = NULL_VECTOR3;
		//norの設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffStagingField->Unlock();
}

void UninitStagingField(void)
{
	//テクスチャの破棄
	for (int nCntType = 0; nCntType < NUM_STAGINGFIELD; nCntType++)
	{
		if (g_pTextureStagingField[nCntType] != NULL)
		{
			g_pTextureStagingField[nCntType]->Release();
			g_pTextureStagingField[nCntType] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffStagingField != NULL)
	{
		g_pVtxBuffStagingField->Release();
		g_pVtxBuffStagingField = NULL;
	}
}

void UpdateStagingField(void)
{
	for (int nCntField = 0; nCntField < MAX_STAGINGFIELD; nCntField++)
	{
		if (g_aStagingField[nCntField].bUse == true)
		{
			g_aStagingField[nCntField].nLife--;

			//============================
			//魔法陣なら
			//============================
			if (g_aStagingField[nCntField].nType == STAGINGFIELD00_MAGICCIRCLE)
			{
				g_aStagingField[nCntField].rot.y += g_aStagingField[nCntField].fRot;
			}

			//===========================================================================


			//============================
			//体力が０になったら
			//============================
			if (g_aStagingField[nCntField].nLife <= 0)
			{
				g_aStagingField[nCntField].bUse = false;
			}

			//============================================================================
		}
	}
}

void DrawStagingField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;			//計算用マトリックス

	for (int nCntField = 0; nCntField < MAX_STAGINGFIELD; nCntField++)
	{
		if (g_aStagingField[nCntField].bUse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aStagingField[nCntField].mtxworld);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aStagingField[nCntField].rot.y, g_aStagingField[nCntField].rot.x, g_aStagingField[nCntField].rot.z);

			D3DXMatrixMultiply(&g_aStagingField[nCntField].mtxworld, &g_aStagingField[nCntField].mtxworld, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aStagingField[nCntField].pos.x, g_aStagingField[nCntField].pos.y, g_aStagingField[nCntField].pos.z);
			D3DXMatrixMultiply(&g_aStagingField[nCntField].mtxworld, &g_aStagingField[nCntField].mtxworld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aStagingField[nCntField].mtxworld);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffStagingField, 0, sizeof(VERTEX_3D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureStagingField[g_aStagingField[nCntField].nType]);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,nCntField * 4, 2);
		}
	}
}

void SetStagingField(int nType, int nLife, float fWidth, float fHeight, float fRot, D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	VERTEX_3D* pVtx;
	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffStagingField->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntField = 0; nCntField < MAX_STAGINGFIELD; nCntField++)
	{
		if (g_aStagingField[nCntField].bUse == false)
		{
			g_aStagingField[nCntField].pos = pos;//位置
			g_aStagingField[nCntField].mtxworld = {};//マトリックスワールド
			g_aStagingField[nCntField].nType = nType;//種類
			g_aStagingField[nCntField].rot = rot;//向き
			g_aStagingField[nCntField].fWidth = fWidth;//横幅
			g_aStagingField[nCntField].fHeight = fHeight;//高さ
			g_aStagingField[nCntField].bUse = true;//使用状態
			g_aStagingField[nCntField].fRot = fRot;//向きの補正
			g_aStagingField[nCntField].nLife = nLife;//体力

			//頂点座標の更新
			pVtx[0].pos = D3DXVECTOR3(-fWidth, 0.0f, fHeight);
			pVtx[1].pos = D3DXVECTOR3(fWidth, 0.0f, fHeight);
			pVtx[2].pos = D3DXVECTOR3(-fWidth, 0.0f, -fHeight);
			pVtx[3].pos = D3DXVECTOR3(fWidth, 0.0f, -fHeight);

			break;
		}
		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffStagingField->Unlock();
}

StagingField* GetStagingField(void)
{
	return &g_aStagingField[0];
}

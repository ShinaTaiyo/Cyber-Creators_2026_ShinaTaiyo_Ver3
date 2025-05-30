//=======================================================================================================================================================================================================================
//
// チェックマークの処理
// Author : Atsumu Kuboichi
//
//=======================================================================================================================================================================================================================
#include "missioncheck.h"
#include "input.h"
#include "mission.h"

//=======================================================================================================================================================================================================================
// グローバル変数
//=======================================================================================================================================================================================================================
PDIRECT3DVERTEXBUFFER9 g_pVtxBuffMissionCheck = NULL;//頂点バッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureMissionCheck[NUM_CHECK] = {};//テクスチャへのポインタ
MissionCheck g_aMissionCheck[MAX_MISSION];//情報の構造体

//=======================================================================================================================================================================================================================
// 初期化処理
//=======================================================================================================================================================================================================================
void InitMissionCheck(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスの取得

	//テクスチャの読み込み
	for (int nCntTex = 0; nCntTex < NUM_CHECK; nCntTex++)
	{
		D3DXCreateTextureFromFile(pDevice,
			CHECK_TEXTURE[nCntTex],
			&g_pTextureMissionCheck[nCntTex]);
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_MISSION,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMissionCheck,
		NULL);

	//頂点情報へのポインタ
	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffMissionCheck->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntCheck = 0; nCntCheck < MAX_MISSION; nCntCheck++)
	{
		g_aMissionCheck[nCntCheck].pos = D3DXVECTOR3(282.0f, 70.0f + nCntCheck * 38.0f, 0.0f);	//位置
		g_aMissionCheck[nCntCheck].nType = -1;//種類
		g_aMissionCheck[nCntCheck].nDisplayTime = 0;//表示時間
		g_aMissionCheck[nCntCheck].fScale = 0.0f;//横幅
		g_aMissionCheck[nCntCheck].bUse = false;//使用しない

		for (int nCntVtx = 0; nCntVtx < 4; nCntVtx++)
		{
			pVtx[nCntVtx].rhw = 1.0f;//rhwの設定
			pVtx[nCntVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//頂点カラーの設定
		}

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aMissionCheck[nCntCheck].pos.x - g_aMissionCheck[nCntCheck].fScale * 0.5f, g_aMissionCheck[nCntCheck].pos.y - g_aMissionCheck[nCntCheck].fScale * 0.5f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aMissionCheck[nCntCheck].pos.x + g_aMissionCheck[nCntCheck].fScale * 0.5f, g_aMissionCheck[nCntCheck].pos.y - g_aMissionCheck[nCntCheck].fScale * 0.5f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aMissionCheck[nCntCheck].pos.x - g_aMissionCheck[nCntCheck].fScale * 0.5f, g_aMissionCheck[nCntCheck].pos.y + g_aMissionCheck[nCntCheck].fScale * 0.5f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aMissionCheck[nCntCheck].pos.x + g_aMissionCheck[nCntCheck].fScale * 0.5f, g_aMissionCheck[nCntCheck].pos.y + g_aMissionCheck[nCntCheck].fScale * 0.5f, 0.0f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffMissionCheck->Unlock();
}

//=======================================================================================================================================================================================================================
// 終了処理
//=======================================================================================================================================================================================================================
void UninitMissionCheck(void)
{
	//テクスチャの破棄
	for (int nCntTex = 0; nCntTex < NUM_CHECK; nCntTex++)
	{
		if (g_pTextureMissionCheck[nCntTex] != NULL)
		{//テクスチャ情報が存在する場合
			g_pTextureMissionCheck[nCntTex]->Release();
			g_pTextureMissionCheck[nCntTex] = NULL;
		}
	}
	//頂点バッファの破棄
	if (g_pVtxBuffMissionCheck != NULL)
	{//頂点情報が存在する場合
		g_pVtxBuffMissionCheck->Release();
		g_pVtxBuffMissionCheck = NULL;
	}
}

//=======================================================================================================================================================================================================================
// 更新処理
//=======================================================================================================================================================================================================================
void UpdateMissionCheck(void)
{
	for (int nCntCheck = 0; nCntCheck < MAX_MISSION; nCntCheck++)
	{
		if (g_aMissionCheck[nCntCheck].bUse)
		{//使用している場合
			//表示時間を減少
			g_aMissionCheck[nCntCheck].nDisplayTime--;
			
			if (g_aMissionCheck[nCntCheck].nDisplayTime <= 0)
			{//表示時間が0になった場合
				g_aMissionCheck[nCntCheck].bUse = false;//使用しない
			}
		}
	}
}

//=======================================================================================================================================================================================================================
// 描画処理
//=======================================================================================================================================================================================================================
void DrawMissionCheck(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();									//デバイスの取得
	pDevice->SetStreamSource(0, g_pVtxBuffMissionCheck, 0, sizeof(VERTEX_2D));	//頂点バッファをデータストリームに設定
	pDevice->SetFVF(FVF_VERTEX_2D);												//頂点フォーマットの設定

	for (int nCntCheck = 0; nCntCheck < MAX_MISSION; nCntCheck++)
	{
		if (g_aMissionCheck[nCntCheck].bUse)
		{//使用されている場合
			pDevice->SetTexture(0, g_pTextureMissionCheck[g_aMissionCheck[nCntCheck].nType]);//テクスチャの設定
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntCheck * 4, 2);//ポリゴンの描画
		}
	}
}

//=======================================================================================================================================================================================================================
// 設定処理
//=======================================================================================================================================================================================================================
void SetMissionCheck(D3DXVECTOR3 pos, int nType, int nDisplatTime, float fScale)
{
	//頂点情報へのポインタ
	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffMissionCheck->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntCheck = 0; nCntCheck < MAX_MISSION; nCntCheck++)
	{
		if (!g_aMissionCheck[nCntCheck].bUse)
		{//使用していない場合
			g_aMissionCheck[nCntCheck].bUse = true;//使用する
			g_aMissionCheck[nCntCheck].pos = pos;//位置
			g_aMissionCheck[nCntCheck].nType = nType;//種類
			g_aMissionCheck[nCntCheck].nDisplayTime = nDisplatTime;//表示時間
			g_aMissionCheck[nCntCheck].fScale = fScale;//大きさ

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aMissionCheck[nCntCheck].pos.x - g_aMissionCheck[nCntCheck].fScale * 0.5f, g_aMissionCheck[nCntCheck].pos.y - g_aMissionCheck[nCntCheck].fScale * 0.5f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aMissionCheck[nCntCheck].pos.x + g_aMissionCheck[nCntCheck].fScale * 0.5f, g_aMissionCheck[nCntCheck].pos.y - g_aMissionCheck[nCntCheck].fScale * 0.5f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aMissionCheck[nCntCheck].pos.x - g_aMissionCheck[nCntCheck].fScale * 0.5f, g_aMissionCheck[nCntCheck].pos.y + g_aMissionCheck[nCntCheck].fScale * 0.5f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aMissionCheck[nCntCheck].pos.x + g_aMissionCheck[nCntCheck].fScale * 0.5f, g_aMissionCheck[nCntCheck].pos.y + g_aMissionCheck[nCntCheck].fScale * 0.5f, 0.0f);

			break;
		}

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffMissionCheck->Unlock();
}

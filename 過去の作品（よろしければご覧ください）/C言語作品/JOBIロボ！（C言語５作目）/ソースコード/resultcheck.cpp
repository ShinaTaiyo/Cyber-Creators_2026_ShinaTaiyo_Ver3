//=======================================================================================================================================================================================================================
//
// リザルトのミッションチェック処理
// Author : Taiyo Shina
//
//=======================================================================================================================================================================================================================
#include "resultcheck.h"
#include "mission.h"
#include "score.h"

//=======================================================================================================================================================================================================================
// グローバル変数
//=======================================================================================================================================================================================================================
PDIRECT3DVERTEXBUFFER9 g_pVtxBuffResultCheck = NULL;//頂点バッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureResultCheck = NULL;	//テクスチャへのポインタ
ResultCheck g_aResultCheck[MAX_MISSION];		//情報の構造体

//=======================================================================================================================================================================================================================
// 初期化処理
//=======================================================================================================================================================================================================================
void InitResultCheck(void)
{
	VERTEX_2D* pVtx;						//頂点情報へのポインタ
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスの取得
	Mission* pMission = GetMission();		//ミッションの情報を取得

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\ResultCheck000.png",
		&g_pTextureResultCheck);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_MISSION,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffResultCheck,
		NULL);

	//===========================================
	//ミッション0
	//===========================================
	if (pMission->aMission0.bMissionClear)
	{//ミッション達成済みの場合
		g_aResultCheck[0].bUse = true;//使用する
		AddScore(10000);//ボーナススコアを加算
	}
	else
	{//未達成の場合
		g_aResultCheck[0].bUse = false;//使用しない
	}

	//===========================================
	//ミッション1
	//===========================================
	if (pMission->aMission1.bMissionClear)
	{//ミッション達成済みの場合
		g_aResultCheck[1].bUse = true;//使用する
		AddScore(10000);//ボーナススコアを加算
	}
	else
	{//未達成の場合
		g_aResultCheck[1].bUse = false;//使用しない
	}

	//===========================================
	//ミッション2
	//===========================================
	if (pMission->aMission2.bMissionClear)
	{//ミッション達成済みの場合
		g_aResultCheck[2].bUse = true;//使用する
		AddScore(10000);//ボーナススコアを加算
	}
	else
	{//未達成の場合
		g_aResultCheck[2].bUse = false;//使用しない
	}

	//===========================================
	//ミッション3
	//===========================================
	if (pMission->aMission3.bMissionClear)
	{//ミッション達成済みの場合
		g_aResultCheck[3].bUse = true;//使用する
		AddScore(10000);//ボーナススコアを加算
	}
	else
	{//未達成の場合
		g_aResultCheck[3].bUse = false;//使用しない
	}

	//===========================================
	//ミッション4
	//===========================================
	if (pMission->aMission4.bMissionClear)
	{//ミッション達成済みの場合
		g_aResultCheck[4].bUse = true;//使用する
		AddScore(10000);//ボーナススコアを加算
	}
	else
	{//未達成の場合
		g_aResultCheck[4].bUse = false;//使用しない
	}

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffResultCheck->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntCheck = 0; nCntCheck < MAX_MISSION; nCntCheck++)
	{
		g_aResultCheck[nCntCheck].pos = D3DXVECTOR3(520.0f, 445.0f + nCntCheck * 58.0f, 0.0f);	//位置

		for (int nCntVtx = 0; nCntVtx < 4; nCntVtx++)
		{
			//rhwの設定
			pVtx[nCntVtx].rhw = 1.0f;

			//頂点カラーの設定
			pVtx[nCntVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aResultCheck[nCntCheck].pos.x - RESULTCHECK_WIDTH * 0.5f, g_aResultCheck[nCntCheck].pos.y - RESULTCHECK_HEIGHT * 0.5f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aResultCheck[nCntCheck].pos.x + RESULTCHECK_WIDTH * 0.5f, g_aResultCheck[nCntCheck].pos.y - RESULTCHECK_HEIGHT * 0.5f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aResultCheck[nCntCheck].pos.x - RESULTCHECK_WIDTH * 0.5f, g_aResultCheck[nCntCheck].pos.y + RESULTCHECK_HEIGHT * 0.5f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aResultCheck[nCntCheck].pos.x + RESULTCHECK_WIDTH * 0.5f, g_aResultCheck[nCntCheck].pos.y + RESULTCHECK_HEIGHT * 0.5f, 0.0f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffResultCheck->Unlock();
}

//=======================================================================================================================================================================================================================
// 終了処理
//=======================================================================================================================================================================================================================
void UninitResultCheck(void)
{
	//テクスチャの破棄
	if (g_pTextureResultCheck != NULL)
	{//テクスチャ情報が存在する場合
		g_pTextureResultCheck->Release();
		g_pTextureResultCheck = NULL;
	}
	//頂点バッファの破棄
	if (g_pVtxBuffResultCheck != NULL)
	{//頂点情報が存在する場合
		g_pVtxBuffResultCheck->Release();
		g_pVtxBuffResultCheck = NULL;
	}
}

//=======================================================================================================================================================================================================================
// 更新処理
//=======================================================================================================================================================================================================================
void UpdateResultCheck(void)
{

}

//=======================================================================================================================================================================================================================
// 描画処理
//=======================================================================================================================================================================================================================
void DrawResultCheck(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();									//デバイスの取得
	pDevice->SetStreamSource(0, g_pVtxBuffResultCheck, 0, sizeof(VERTEX_2D));	//頂点バッファをデータストリームに設定
	pDevice->SetFVF(FVF_VERTEX_2D);												//頂点フォーマットの設定

	for (int nCntCheck = 0; nCntCheck < MAX_MISSION; nCntCheck++)
	{
		if (g_aResultCheck[nCntCheck].bUse)
		{//使用されている場合
			pDevice->SetTexture(0, g_pTextureResultCheck);					//テクスチャの設定
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntCheck * 4, 2);	//ポリゴンの描画
		}
	}
}

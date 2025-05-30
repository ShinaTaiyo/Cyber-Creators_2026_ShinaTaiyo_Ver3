//=======================================================================================================================================================================================================================
//
// チュートリアルUIの処理
// Author : Atsumu Kuboichi
//
//=======================================================================================================================================================================================================================
#include "operate.h"
#include "check.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "game.h"

//=======================================================================================================================================================================================================================
// グローバル変数
//=======================================================================================================================================================================================================================
PDIRECT3DVERTEXBUFFER9 g_pVtxBuffOperate = NULL;		//頂点バッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureOperate[MAX_OPERATE] = {};	//テクスチャへのポインタ
Operate g_aOperate[MAX_OPERATE];						//情報の構造体

//=======================================================================================================================================================================================================================
// 初期化処理
//=======================================================================================================================================================================================================================
void InitOperate(void)
{
	VERTEX_2D* pVtx;						//頂点情報へのポインタ
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスの取得

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_OPERATE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffOperate,
		NULL);

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffOperate->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntOperate = 0; nCntOperate < MAX_OPERATE; nCntOperate++)
	{
		//テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			OPERATE_FILENAME[nCntOperate],
			&g_pTextureOperate[nCntOperate]);

		
		//変数
		switch (nCntOperate)
		{
		case 0://「操作を確認しよう！」の表示
			g_aOperate[nCntOperate].bUse = true;							//使用する
			g_aOperate[nCntOperate].pos = D3DXVECTOR3(210.0f, 40.0f, 0.0f);//位置
			g_aOperate[nCntOperate].fWidth = 400.0f;						//横幅
			g_aOperate[nCntOperate].fHeight = 80.0f;						//高さ
			break;

		case 1://操作方法一覧
			g_aOperate[nCntOperate].bUse = true;							//使用する
			g_aOperate[nCntOperate].pos = D3DXVECTOR3(300.0f, 560.0f, 0.0f);//位置
			g_aOperate[nCntOperate].fWidth = 500.0f;						//横幅
			g_aOperate[nCntOperate].fHeight = 300.0f;						//高さ
			break;

		case 2://ゲームスタートの表示
			g_aOperate[nCntOperate].bUse = true;							//使用しない
			g_aOperate[nCntOperate].pos = D3DXVECTOR3(940.0f, 640.0f, 0.0f);//位置
			g_aOperate[nCntOperate].fWidth = 600.0f;						//横幅
			g_aOperate[nCntOperate].fHeight = 100.0f;						//高さ
			break;

		default:
			break;
		}

		for (int nCntVtx = 0; nCntVtx < 4; nCntVtx++)
		{
			//rhwの設定
			pVtx[nCntVtx].rhw = 1.0f;

			//頂点カラーの設定
			pVtx[nCntVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aOperate[nCntOperate].pos.x - g_aOperate[nCntOperate].fWidth * 0.5f, g_aOperate[nCntOperate].pos.y - g_aOperate[nCntOperate].fHeight * 0.5f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aOperate[nCntOperate].pos.x + g_aOperate[nCntOperate].fWidth * 0.5f, g_aOperate[nCntOperate].pos.y - g_aOperate[nCntOperate].fHeight * 0.5f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aOperate[nCntOperate].pos.x - g_aOperate[nCntOperate].fWidth * 0.5f, g_aOperate[nCntOperate].pos.y + g_aOperate[nCntOperate].fHeight * 0.5f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aOperate[nCntOperate].pos.x + g_aOperate[nCntOperate].fWidth * 0.5f, g_aOperate[nCntOperate].pos.y + g_aOperate[nCntOperate].fHeight * 0.5f, 0.0f);

		//テクスチャ座標の設定（初期設定）
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffOperate->Unlock();
}

//=======================================================================================================================================================================================================================
// 終了処理
//=======================================================================================================================================================================================================================
void UninitOperate(void)
{
	//テクスチャの破棄
	for (int nCntTex = 0; nCntTex < MAX_OPERATE; nCntTex++)
	{
		if (g_pTextureOperate[nCntTex] != NULL)
		{//テクスチャ情報が存在する場合
			g_pTextureOperate[nCntTex]->Release();
			g_pTextureOperate[nCntTex] = NULL;
		}
	}
	//頂点バッファの破棄
	if (g_pVtxBuffOperate != NULL)
	{//頂点情報が存在する場合
		g_pVtxBuffOperate->Release();
		g_pVtxBuffOperate = NULL;
	}
}

//=======================================================================================================================================================================================================================
// 更新処理
//=======================================================================================================================================================================================================================
void UpdateOperate(void)
{

}

//=======================================================================================================================================================================================================================
// 描画処理
//=======================================================================================================================================================================================================================
void DrawOperate(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();								//デバイスの取得
	pDevice->SetStreamSource(0, g_pVtxBuffOperate, 0, sizeof(VERTEX_2D));	//頂点バッファをデータストリームに設定
	pDevice->SetFVF(FVF_VERTEX_2D);											//頂点フォーマットの設定

	for (int nCntOperate = 0; nCntOperate < MAX_OPERATE; nCntOperate++)
	{
		if (g_aOperate[nCntOperate].bUse == true)
		{//使用されている場合
			pDevice->SetTexture(0, g_pTextureOperate[nCntOperate]);			//テクスチャの設定
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntOperate * 4, 2);//ポリゴンの描画
		}
	}
}

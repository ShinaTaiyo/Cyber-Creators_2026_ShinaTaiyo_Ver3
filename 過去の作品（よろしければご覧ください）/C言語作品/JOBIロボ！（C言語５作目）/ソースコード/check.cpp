//=======================================================================================================================================================================================================================
//
// チェックマークの処理
// Author : Atsumu Kuboichi
//
//=======================================================================================================================================================================================================================
#include "check.h"
#include "input.h"
#include "rule.h"

//=======================================================================================================================================================================================================================
// グローバル変数
//=======================================================================================================================================================================================================================
PDIRECT3DVERTEXBUFFER9 g_pVtxBuffCheck = NULL;	//頂点バッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureCheck = NULL;		//テクスチャへのポインタ
Check g_aCheck[MAX_CHECK];						//情報の構造体
int g_nCheck;									//入力チェック数

//=======================================================================================================================================================================================================================
// 初期化処理
//=======================================================================================================================================================================================================================
void InitCheck(void)
{
	VERTEX_2D* pVtx;						//頂点情報へのポインタ
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスの取得

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Check000.png",
		&g_pTextureCheck);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_CHECK,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffCheck,
		NULL);

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffCheck->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntCheck = 0; nCntCheck < MAX_CHECK; nCntCheck++)
	{
		g_aCheck[nCntCheck].pos = D3DXVECTOR3(277.5f, 24.0f + nCntCheck * 52.2f, 0.0f);	//位置
		g_aCheck[nCntCheck].nNum = nCntCheck;											//番号
		g_aCheck[nCntCheck].fWidth = 37.0f;												//横幅
		g_aCheck[nCntCheck].fHeight = 37.0f;											//高さ
		g_aCheck[nCntCheck].bUse = false;												//使用する
		
		for (int nCntVtx = 0; nCntVtx < 4; nCntVtx++)
		{
			//rhwの設定
			pVtx[nCntVtx].rhw = 1.0f;

			//頂点カラーの設定
			pVtx[nCntVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aCheck[nCntCheck].pos.x - g_aCheck[nCntCheck].fWidth * 0.5f, g_aCheck[nCntCheck].pos.y - g_aCheck[nCntCheck].fHeight * 0.5f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aCheck[nCntCheck].pos.x + g_aCheck[nCntCheck].fWidth * 0.5f, g_aCheck[nCntCheck].pos.y - g_aCheck[nCntCheck].fHeight * 0.5f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aCheck[nCntCheck].pos.x - g_aCheck[nCntCheck].fWidth * 0.5f, g_aCheck[nCntCheck].pos.y + g_aCheck[nCntCheck].fHeight * 0.5f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aCheck[nCntCheck].pos.x + g_aCheck[nCntCheck].fWidth * 0.5f, g_aCheck[nCntCheck].pos.y + g_aCheck[nCntCheck].fHeight * 0.5f, 0.0f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	//変数
	g_nCheck = 0;//チェック数

	//頂点バッファをアンロックする
	g_pVtxBuffCheck->Unlock();
}

//=======================================================================================================================================================================================================================
// 終了処理
//=======================================================================================================================================================================================================================
void UninitCheck(void)
{
	//テクスチャの破棄
	if (g_pTextureCheck != NULL)
	{//テクスチャ情報が存在する場合
		g_pTextureCheck->Release();
		g_pTextureCheck = NULL;
	}
	//頂点バッファの破棄
	if (g_pVtxBuffCheck != NULL)
	{//頂点情報が存在する場合
		g_pVtxBuffCheck->Release();
		g_pVtxBuffCheck = NULL;
	}
}

//=======================================================================================================================================================================================================================
// 更新処理
//=======================================================================================================================================================================================================================
void UpdateCheck(void)
{
	//==============================================================================================================================
	// 入力判定
	//==============================================================================================================================
	if (GetJoypadPress(JOYKEY_UP) || GetJoypadPress(JOYKEY_DOWN) || GetJoypadPress(JOYKEY_RIGHT) || GetJoypadPress(JOYKEY_LEFT))
	{//移動ボタンが入力された場合
		if (!g_aCheck[0].bUse)
		{//チェック済みではない場合
			g_nCheck++;	//チェック数を加算
		}

		g_aCheck[0].bUse = true;//チェック済みにする
	}
	if (GetJoypadTrigger(JOYKEY_LB) || GetJoypadTrigger(JOYKEY_RB))
	{//LB or RBが入力された場合
		if (!g_aCheck[1].bUse)
		{//チェック済みではない場合
			g_nCheck++;	//チェック数を加算
		}

		g_aCheck[1].bUse = true;//チェック済みにする
	}
	if (GetJoypadPress(JOYKEY_X))
	{//Xが入力された場合
		if (!g_aCheck[2].bUse)
		{//チェック済みではない場合
			g_nCheck++;	//チェック数を加算
		}

		g_aCheck[2].bUse = true;//チェック済みにする
	}
	if (GetJoypadPress(JOYKEY_Y))
	{//Yが入力された場合
		if (!g_aCheck[3].bUse)
		{//チェック済みではない場合
			g_nCheck++;	//チェック数を加算
		}

		g_aCheck[3].bUse = true;//チェック済みにする
	}
	if (GetJoypadPress(JOYKEY_B))
	{//Bが入力された場合
		if (!g_aCheck[4].bUse)
		{//チェック済みではない場合
			g_nCheck++;	//チェック数を加算
		}

		g_aCheck[4].bUse = true;//チェック済みにする
	}
	if (GetJoypadPress(JOYKEY_A))
	{//Bが入力された場合
		if (!g_aCheck[5].bUse)
		{//チェック済みではない場合
			g_nCheck++;	//チェック数を加算
		}

		g_aCheck[5].bUse = true;//チェック済みにする
	}
}

//=======================================================================================================================================================================================================================
// 描画処理
//=======================================================================================================================================================================================================================
void DrawCheck(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();							//デバイスの取得
	pDevice->SetStreamSource(0, g_pVtxBuffCheck, 0, sizeof(VERTEX_2D));	//頂点バッファをデータストリームに設定
	pDevice->SetFVF(FVF_VERTEX_2D);										//頂点フォーマットの設定

	for (int nCntCheck = 0; nCntCheck < MAX_CHECK; nCntCheck++)
	{
		if (g_aCheck[nCntCheck].bUse)
		{//使用されている場合
			pDevice->SetTexture(0, g_pTextureCheck);						//テクスチャの設定
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntCheck * 4, 2);	//ポリゴンの描画
		}
	}
}

//=======================================================================================================================================================================================================================
// 情報の構造体を取得
//=======================================================================================================================================================================================================================
int GetCheck(void)
{
	return g_nCheck;
}

//========================================================
//
//９月２６日：ハッカソンの基盤作り[title.cpp]
//Author:ShinaTaiyo
//
//=========================================================

#include "title.h"
#include "main.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
//グローバル変数

#define NUM_DRAWTITLE (3)

LPDIRECT3DTEXTURE9 g_pTextureTitle[NUM_DRAWTITLE] = {};//テクスチャへのポインタ
//VERTEX_2D g_aVertex[4];//頂点情報を格納（四角形を作りたい）
PDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitle = NULL;//頂点バッファへのポインタ

int g_nColorChange;

bool bFadeFlag;//タイトルフェードのフラグ

int g_nCntTime2;

int g_nChooseCommand;//コマンド選択用

//タイトル画面の初期化処理
void InitTitle(void)
{
	g_nChooseCommand = 0;//コマンド選択用

	bFadeFlag = false;


	g_nCntTime2 = 0;

	g_nColorChange = 255;//カラーの初期化（白色）

	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\ACT_Title.png",
		&g_pTextureTitle[0]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\ChooseNormalMode.png",
		&g_pTextureTitle[1]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\ChooseShortMode.png",
		&g_pTextureTitle[2]);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * 2,//頂点の数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTitle,
		NULL);

	//タイトル画面BGMの設定
	PlaySound(SOUND_LABEL_BGM000);

	VERTEX_2D* pVtx;
	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		//頂点座標の設定（初期位置）
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//rhwの設定（初期設定）
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定（初期設定）
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//テクスチャ座標の設定（初期設定）
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffTitle->Unlock();
}

//==============================
//タイトル画面の終了処理
//==============================
void UninitTitle(void)
{
	//テクスチャの破棄
	for (int nCnt = 0; nCnt < NUM_DRAWTITLE; nCnt++)
	{
		if (g_pTextureTitle[nCnt] != NULL)
		{
			g_pTextureTitle[nCnt]->Release();
			g_pTextureTitle[nCnt] = NULL;
		}
	}
	//頂点バッファの破棄
	if (g_pVtxBuffTitle != NULL)
	{
		g_pVtxBuffTitle->Release();
		g_pVtxBuffTitle = NULL;
	}
}
//============================
//タイトル画面の更新処理
//============================
void UpdateTitle(void)
{
	g_nCntTime2++;

	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	VERTEX_2D* pVtx;
	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);


	/*if (g_nCntTime2 == 900)
	{
		SetFade(MODE_RANKING);
	}*/

	if (GetKeyboardTrigger(DIK_W) == true || GetJoyPadTrigger(JOYKEY_UP)==true)
	{
		g_nChooseCommand--;
		PlaySound(SOUND_LABEL_SE_SELECT_000);
	}
	if (GetKeyboardTrigger(DIK_S) == true || GetJoyPadTrigger(JOYKEY_DOWN) == true)
	{
		g_nChooseCommand++;
		PlaySound(SOUND_LABEL_SE_SELECT_000);
	}

	if (g_nChooseCommand <= -1)
	{
		g_nChooseCommand = 1;
	}
	if (g_nChooseCommand >= 2)
	{
		g_nChooseCommand = 0;
	}


	//==========================================
	//タイトルとか動かすなら処理を書く
	//==========================================
	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		switch (nCnt)
		{
		case 0:
			//頂点座標の設定（初期位置）
			pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

			//rhwの設定（初期設定）
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			//頂点カラーの設定（初期設定）
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

			//テクスチャ座標の設定（初期設定）
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			break;
		case 1:
			//頂点座標の設定（初期位置）
			pVtx[0].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 -150.0f,SCREEN_HEIGHT - 300.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 + 150.0f, SCREEN_HEIGHT - 300.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 - 150.0f, SCREEN_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 + 150.0f, SCREEN_HEIGHT, 0.0f);

			//rhwの設定（初期設定）
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			//頂点カラーの設定（初期設定）
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

			//テクスチャ座標の設定（初期設定）
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			break;
		}
		

		pVtx += 4;
	}

    //=====================================================================

	//頂点バッファをアンロックする
	g_pVtxBuffTitle->Unlock();

	//========================================
	//画面遷移処理
	//========================================
	if (GetKeyboardTrigger(DIK_RETURN) == true || GetJoyPadTrigger(JOYKEY_START) == true)
	{//決定キー（ENTERキー）が押された
		//bFadeFlag = true;//フェードさせるフラグをtrueにする。
		SetFade(MODE_TUTORIAL);//チュートリアル画面を呼び出す。
		PlaySound(SOUND_LABEL_SE_DECISION_000);
	}
	//===========================================================================================
}



//================================
//タイトル画面の描画処理
//================================
void DrawTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

		//デバイスの取得
		pDevice = GetDevice();

		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffTitle, 0, sizeof(VERTEX_2D));//頂点バッファへのポインタと頂点情報の構造体のサイズ

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		//テクスチャの設定
		for (int nCnt = 0; nCnt < 2; nCnt++)
		{
			switch (nCnt)
			{
			case 0:
			pDevice->SetTexture(0, g_pTextureTitle[0]);
				break;
			case 1:
				if (g_nChooseCommand == 0)
				{
					pDevice->SetTexture(0, g_pTextureTitle[1]);
				}
				else if (g_nChooseCommand == 1)
				{
					pDevice->SetTexture(0, g_pTextureTitle[2]);
				}
				break;
			}
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,nCnt *4, 2);//0は、描画する最初の頂点インデックスで、2は、描画するプリミティブ
		}
		//ポリゴンの描画
}

int* GetChooseCommand(void)
{
	return &g_nChooseCommand;
}

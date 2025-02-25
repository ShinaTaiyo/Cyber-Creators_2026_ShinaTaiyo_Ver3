//========================================================
//
//８月３０日：エフェクトもう一回見直す[title.cpp]
//Author:ShinaTaiyo
//
//=========================================================

#include "title.h"
#include "main.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "staging.h"
//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureTitle= NULL;//テクスチャへのポインタ
//VERTEX_2D g_aVertex[4];//頂点情報を格納（四角形を作りたい）
PDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitle = NULL;//頂点バッファへのポインタ

int g_nColorChange;

bool bFadeFlag;//タイトルフェードのフラグ

int g_nCntTime;

//タイトル画面の初期化処理
void InitTitle(void)
{


	bFadeFlag = false;

	g_nCntTime = 0;

	g_nColorChange = 255;//カラーの初期化（白色）

	//「PressStart」の表示処理
	


	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\gametitle.png",
		&g_pTextureTitle);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,//頂点の数
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
	pVtx[0].col = D3DCOLOR_RGBA(255,255,255,255);
	pVtx[1].col = D3DCOLOR_RGBA(255,255,255,255);
	pVtx[2].col = D3DCOLOR_RGBA(255,255,255,255);
	pVtx[3].col = D3DCOLOR_RGBA(255,255,255,255);


	//テクスチャ座標の設定（初期設定）
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffTitle->Unlock();
}

//==============================
//タイトル画面の終了処理
//==============================
void UninitTitle(void)
{
	//テクスチャの破棄
	if (g_pTextureTitle != NULL)
	{
		g_pTextureTitle->Release();
		g_pTextureTitle = NULL;
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
	g_nCntTime++;

	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	VERTEX_2D* pVtx;
	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	if (GetKeyboardTrigger(DIK_RETURN) == true||GetJoyPadTrigger(JOYKEY_START))
	{//決定キー（ENTERキー）が押された
	
		PlaySound(SOUND_LABEL_SE_DECISION_000);
		//bFadeFlag = true;//フェードさせるフラグをtrueにする。
		SetFade(MODE_TUTORIAL);//チュートリアル画面をフェードアウト後に呼び出す
		//SetMode(MODE_TUTORIAL);//ゲームモードを呼び出す
	}

	if (g_nCntTime == 2)
	{
		SetStaging(STAGINGTYPE_PRESSSTART, -1, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 140.0f, 0.0f));
	}
	//if (bFadeFlag == true)
	//{
	//	g_nColorChange-=5;//色の値を減らし続けてフェードする。
	//}
	//if (bFadeFlag == false)
	//{
	//	g_nColorChange += 5;
	//	
	//}
	//if (g_nColorChange >= 255)
	//{
	//	g_nColorChange = 255;
	//}
	//モードの設定（ゲーム画面に移行）
	//pVtx[0].col = D3DCOLOR_RGBA(g_nColorChange, g_nColorChange,g_nColorChange,g_nColorChange);
	//pVtx[1].col = D3DCOLOR_RGBA(g_nColorChange, g_nColorChange,g_nColorChange,g_nColorChange);
	//pVtx[2].col = D3DCOLOR_RGBA(g_nColorChange, g_nColorChange,g_nColorChange,g_nColorChange);
	//pVtx[3].col = D3DCOLOR_RGBA(g_nColorChange, g_nColorChange,g_nColorChange,g_nColorChange);

	//頂点バッファをアンロックする
	//g_pVtxBuffTitle->Unlock();

	if (g_nColorChange <= 0)
	{//画面が完全に暗くなったら・・・
		
	}
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
		pDevice->SetTexture(0, g_pTextureTitle);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);//0は、描画する最初の頂点インデックスで、2は、描画するプリミティブ数
			//&g_pVtxBuffPolygon[0],//頂点情報の先頭アドレス
			//sizeof(VERTEX_2D));//頂点情報構造体のサイズ
}
//========================================================
//
//８月３０日：エフェクトもう一回見直す[fade.cpp]
//Author:ShinaTaiyo
//
//=========================================================

#include "title.h"
#include "main.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureFade = NULL;//テクスチャへのポインタ
//VERTEX_2D g_aVertex[4];//頂点情報を格納（四角形を作りたい）
PDIRECT3DVERTEXBUFFER9 g_pVtxBuffFade = NULL;//頂点バッファへのポインタ


int g_colorFade_a;//ポリゴンの色の透明度を変えるグローバル変数


MODE g_ModeNext;//次のモードを表す変数

FADE g_fade;
//タイトル画面の初期化処理
void InitFade()
{
	g_colorFade_a=255;//ポリゴンの色の透明度を変えるグローバル変数

	

	g_fade = FADE_IN;//フェードインから始める。

	


	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\fade_testure_2.png",
		&g_pTextureFade);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,//頂点の数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffFade,
		NULL);

	////タイトル画面BGMの設定
	//PlaySound(SOUND_LABEL_BGM000);

	VERTEX_2D* pVtx;
	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);

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
	pVtx[0].col = D3DCOLOR_RGBA(0,0,0,g_colorFade_a);
	pVtx[1].col = D3DCOLOR_RGBA(0,0,0,g_colorFade_a);
	pVtx[2].col = D3DCOLOR_RGBA(0,0,0,g_colorFade_a);
	pVtx[3].col = D3DCOLOR_RGBA(0,0,0,g_colorFade_a);


	//テクスチャ座標の設定（初期設定）
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffFade->Unlock();
}

//==============================
//タイトル画面の終了処理
//==============================
void UninitFade(void)
{
	//テクスチャの破棄
	if (g_pTextureFade != NULL)
	{
		g_pTextureFade->Release();
		g_pTextureFade = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffFade != NULL)
	{
		g_pVtxBuffFade->Release();
		g_pVtxBuffFade = NULL;
	}
}
//============================
//タイトル画面の更新処理
//============================
void UpdateFade(void)
{

	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	VERTEX_2D* pVtx;
	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);

	//if (GetKeyboardTrigger(DIK_RETURN) == true)
	//{//決定キー（ENTERキー）が押された

	//	PlaySound(SOUND_LABEL_SE_DECISION_000);
	//	bFadeFlag = true;//フェードさせるフラグをtrueにする。
	//	SetMode(MODE_TUTORIAL);//ゲームモードを呼び出す
	//}
	

	if (g_fade != FADE_NONE)
	{
		if (g_fade == FADE_IN)
		{//フェードイン状態
			g_colorFade_a -= 5;//ポリゴンを透明にしていく


			if (g_colorFade_a <= 0)
			{
				g_colorFade_a = 0;
				g_fade = FADE_NONE;//何もしていない状態に戻す。
			}
		}

		else if (g_fade == FADE_OUT)
		{//フェードアウト状態
			g_colorFade_a += 5;//ポリゴンを不透明にしていく。
			if (g_colorFade_a >= 255)
			{
				g_colorFade_a = 255;
				//g_fade = FADE_IN;//フェードイン状態に

				//モード設定（次の画面に移行）

				SetMode(g_ModeNext);
			}

		}
	}

	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);



	//頂点カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(0,0,0,g_colorFade_a);
	pVtx[1].col = D3DCOLOR_RGBA(0,0,0,g_colorFade_a);
	pVtx[2].col = D3DCOLOR_RGBA(0,0,0,g_colorFade_a);
	pVtx[3].col = D3DCOLOR_RGBA(0,0,0,g_colorFade_a);
	//頂点バッファをアンロックする
	g_pVtxBuffFade->Unlock();

}



//================================
//タイトル画面の描画処理
//================================
void DrawFade(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

		//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffFade, 0, sizeof(VERTEX_2D));//頂点バッファへのポインタと頂点情報の構造体のサイズ

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureFade);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);//0は、描画する最初の頂点インデックスで、2は、描画するプリミティブ数
		//&g_pVtxBuffPolygon[0],//頂点情報の先頭アドレス
		//sizeof(VERTEX_2D));//頂点情報構造体のサイズ
}

//===============================================
//フェードの設定処理
//===============================================
void SetFade(MODE ModeNext)
{
	g_fade = FADE_OUT;//この関数がセットされたら、フェードアウトを開始する
	g_ModeNext = ModeNext;

}

FADE GetFade(void)
{
	return g_fade;
}
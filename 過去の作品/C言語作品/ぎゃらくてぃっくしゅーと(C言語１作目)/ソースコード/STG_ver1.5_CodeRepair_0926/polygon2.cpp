//==============================================
//
//７月１８日：テクスチャの回転[polygon2.cpp]
//Author:ShinaTaiyo
//
//==============================================

#include "main.h"
#include "polygon2.h"//作成したpolygon2.hをインクルードする


//グローバル変数
LPDIRECT3DTEXTURE9 g_pTexturePolygon_2 = NULL;//テクスチャへのポインタ
//VERTEX_2D g_aVertex[4];//頂点情報を格納（四角形を作りたい）
PDIRECT3DVERTEXBUFFER9 g_pVtxBuffPolygon_2 = NULL;//頂点バッファへのポインタ

bool g_bTurnPoint_2 = false;//ウインドウの端にキャラが到達した場合、往復させるためのフラグ
bool g_bTurnPoint2_2 = false;
float g_fXmove_2 = 0;//X方向に動かす処理の変数
float g_fYmove_2 = 0;//Y方向に動かす処理の変数
bool g_bColorFlag_2 = true;//色を変える時の数値の増減を管理する変数
//float g_fEnemyMove1[4];

int g_nColorCount1_2 = 0, g_nColorCount2_2 = 0, g_nColorCount3_2 = 0, g_nColorCount4_2 = 0;//常時色を変え続ける変数


//=======================================================
//ポリゴンの初期化処理
//=======================================================
void InitPolygon_2(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	//D3DXCreateTextureFromFile(pDevice,
	//	"data\\bg100.jpg",
	//	&g_pTexturePolygon_2);

	/*D3DXCreateTextureFromFile(pDevice,
		"data\\sword_kirby.png",
		&g_pTexturePolygon_2);*/

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D_2) * 4,//頂点の数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPolygon_2,
		NULL);

	VERTEX_2D_2* pVtx_2;
	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPolygon_2->Lock(0, 0, (void**)&pVtx_2, 0);

	//頂点座標の設定（初期位置）
	pVtx_2[0].pos_2 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx_2[1].pos_2 = D3DXVECTOR3(SCREEN_WIDTH,0.0f, 0.0f);
	pVtx_2[2].pos_2 = D3DXVECTOR3(0.0f,SCREEN_HEIGHT, 0.0f);
	pVtx_2[3].pos_2 = D3DXVECTOR3(SCREEN_WIDTH,SCREEN_HEIGHT, 0.0f);




	//rhwの設定（初期設定）
	pVtx_2[0].rhw_2 = 1.0f;
	pVtx_2[1].rhw_2 = 1.0f;
	pVtx_2[2].rhw_2 = 1.0f;
	pVtx_2[3].rhw_2 = 1.0f;
		
	 
	//頂点カラーの設定（初期設定）
	pVtx_2[0].col_2 = D3DCOLOR_RGBA(0, 0, 0, 0);
	pVtx_2[1].col_2 = D3DCOLOR_RGBA(0, 0, 0, 0);
	pVtx_2[2].col_2 = D3DCOLOR_RGBA(0, 0, 0, 0);
	pVtx_2[3].col_2 = D3DCOLOR_RGBA(0, 0, 0, 0);


	//テクスチャ座標の設定（初期設定）
	pVtx_2[0].tex_2 = D3DXVECTOR2(0.0f, 0.0f);
	pVtx_2[1].tex_2 = D3DXVECTOR2(1.0f, 0.0f);
	pVtx_2[2].tex_2 = D3DXVECTOR2(0.0f, 1.0f);
	pVtx_2[3].tex_2 = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffPolygon_2->Unlock();
}

//========================================
//ポリゴンの終了処理
//========================================
void UninitPolygon_2(void)
{
	//テクスチャの破棄
	if (g_pTexturePolygon_2 != NULL)
	{
		g_pTexturePolygon_2->Release();
		g_pTexturePolygon_2 = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffPolygon_2 != NULL)
	{
		g_pVtxBuffPolygon_2->Release();
		g_pVtxBuffPolygon_2 = NULL;
	}
}
//==========================================================
//ポリゴンの更新処理
//==========================================================
void UpDatePolygon_2(void)//１フレームごとに処理される。
{
	//float fEnemyMove1[4];
	//float *pMove;

	//pMove = fEnemyMove1;
 //   
	//pMove[0] = 0.0f + g_fXmove_2;
	//fEnemyMove1[1] = 1280.0f + g_fXmove_2;
	//fEnemyMove1[2] = 0.0f + g_fYmove_2;
	//fEnemyMove1[3] = 720.0f + g_fYmove_2;


	//

	//if (fEnemyMove1[1] >= SCREEN_WIDTH)//画面右端へ行った場合
	//{
	//	fEnemyMove1[1] = SCREEN_WIDTH;
	//}
	//if (fEnemyMove1[0] == 0)//画面左端へ行った場合
	//{
	//	fEnemyMove1[0] = 0;
	//}

	//if (fEnemyMove1[2] <= 0)//画面上へいった場合
	//{
	//	fEnemyMove1[2] = 0;
	//}
	//if (fEnemyMove1[3] == SCREEN_HEIGHT)//画面下へいった場合
	//{
	//	fEnemyMove1[3] = SCREEN_HEIGHT;
	//}
	//


	//if (g_nColorCount1_2 >= 255)
	//{
	//	g_bColorFlag_2 = true;
	//}
	//if (g_nColorCount1_2 <= 0)
	//{
	//	g_bColorFlag_2 = false;
	//}

	//if (g_bColorFlag_2 == true)
	//{
	//	g_nColorCount1_2 -= 10;
	//	g_nColorCount2_2 -= 10;
	//	g_nColorCount3_2 -= 10;
	//	g_nColorCount4_2 -= 10;
	//}
	//if (g_bColorFlag_2 == false)
	//{
	//	g_nColorCount1_2 += 10;
	//	g_nColorCount2_2 += 10;
	//	g_nColorCount3_2 += 10;
	//	g_nColorCount4_2 += 10;
	//}

	//VERTEX_2D_2* pVtx_2;
	////頂点バッファをロックし、頂点情報へのポインタを取得
	//g_pVtxBuffPolygon_2->Lock(0, 0, (void**)&pVtx_2, 0);

	////頂点座標の設定
	//pVtx_2[0].pos_2 = D3DXVECTOR3(fEnemyMove1[0], fEnemyMove1[2], 0.0f);
	//pVtx_2[1].pos_2 = D3DXVECTOR3(fEnemyMove1[1], fEnemyMove1[2], 0.0f);
	//pVtx_2[2].pos_2 = D3DXVECTOR3(fEnemyMove1[0], fEnemyMove1[3], 0.0f);
	//pVtx_2[3].pos_2 = D3DXVECTOR3(fEnemyMove1[1], fEnemyMove1[3], 0.0f);

	////rhwの設定
	//pVtx_2[0].rhw_2 = 1.0f;
	//pVtx_2[1].rhw_2 = 1.0f;
	//pVtx_2[2].rhw_2 = 1.0f;
	//pVtx_2[3].rhw_2 = 1.0f;
	//
	////頂点カラーの設定
	///*pVtx_2[0].col_2 = D3DCOLOR_RGBA(g_nColorCount1_2, 255, 255, 255);
	//pVtx_2[1].col_2 = D3DCOLOR_RGBA(255, g_nColorCount2_2, 255, 255);
	//pVtx_2[2].col_2 = D3DCOLOR_RGBA(255, 255, g_nColorCount3_2, 255);
	//pVtx_2[3].col_2 = D3DCOLOR_RGBA(g_nColorCount4_2, 255, g_nColorCount4_2, 255);*/

	//pVtx_2[0].col_2 = D3DCOLOR_RGBA(255, 255, 255, 255);
	//pVtx_2[1].col_2 = D3DCOLOR_RGBA(255, 255, 255, 255);
	//pVtx_2[2].col_2 = D3DCOLOR_RGBA(255, 255, 255, 255);
	//pVtx_2[3].col_2 = D3DCOLOR_RGBA(255, 255, 255, 255);


	////テクスチャ座標の設定
	//pVtx_2[0].tex_2 = D3DXVECTOR2(0.0f, 0.0f);
	//pVtx_2[1].tex_2 = D3DXVECTOR2(1.0f, 0.0f);
	//pVtx_2[2].tex_2 = D3DXVECTOR2(0.0f, 1.0f);
	//pVtx_2[3].tex_2 = D3DXVECTOR2(1.0f, 1.0f);

	////頂点バッファをアンロックする
	//g_pVtxBuffPolygon_2->Unlock();
}
//==========================================================
//ポリゴンの描画処理
//==========================================================
void DrawPolygon_2(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPolygon_2, 0, sizeof(VERTEX_2D_2));//頂点バッファへのポインタと頂点情報の構造体のサイズ

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTexturePolygon_2);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);//0は、描画する最初の頂点インデックスで、2は、描画するプリミティブ数
		//&g_pVtxBuffPolygon[0],//頂点情報の先頭アドレス
		//sizeof(VERTEX_2D));//頂点情報構造体のサイズ
}

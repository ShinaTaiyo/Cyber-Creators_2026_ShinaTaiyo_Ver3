//========================================================
//
//８月３０日：エフェクトもう一回見直す[Character.cpp]
//Author:ShinaTaiyo
//
//=========================================================

#include "result.h"
#include "main.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "character.h"


#define CHARACTER_SIZE_X (150.0f)
#define CHARACTER_SIZE_Y (150.0f)

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureCharacter[MAX_CHARACTER_TEX] = {};//テクスチャへのポインタ
//VERTEX_2D g_aVertex[4];//頂点情報を格納（四角形を作りたい）
PDIRECT3DVERTEXBUFFER9 g_pVtxBuffCharacter = NULL;//頂点バッファへのポインタ

LPDIRECT3DTEXTURE9 g_pTextureCharacterBG = NULL;//テクスチャへのポインタ
//VERTEX_2D g_aVertex[4];//頂点情報を格納（四角形を作りたい）
PDIRECT3DVERTEXBUFFER9 g_pVtxBuffCharacterBG = NULL;//頂点バッファへのポインタ


Character g_aCharacter[MAX_CHARACTER_TEX];

CharacterNum g_aCharacterNum;

int g_nChooseNum;

//リザルト画面の初期化処理
void InitChacacter(void)
{
	int nCntCharacter;

	g_nChooseNum = 0;

	g_aCharacterNum.nCharacterNum = 0;


	VERTEX_2D* pVtx;

	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();


	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Select.png",
		&g_pTextureCharacterBG);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,//頂点の数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffCharacterBG,
		NULL);

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffCharacterBG->Lock(0, 0, (void**)&pVtx, 0);




	//コマンドの位置情報の初期化
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

	//頂点バッファをアンロックする
	g_pVtxBuffCharacterBG->Unlock();




	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\PlayerBULE.png",
		&g_pTextureCharacter[1]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\PlayerRED.png",
		&g_pTextureCharacter[0]);


	/*D3DXCreateTextureFromFile(pDevice,
		"data\\RETRY_indicate.png",
		&g_pTextureCharacter[2]);


	D3DXCreateTextureFromFile(pDevice,
		"data\\QUIT_indicate.png",
		&g_pTextureCharacter[3]);*/

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_CHARACTER_TEX,//頂点の数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffCharacter,
		NULL);

	for (nCntCharacter = 0; nCntCharacter < MAX_CHARACTER_TEX; nCntCharacter++)
	{

		g_aCharacter[nCntCharacter].pos = D3DXVECTOR3(SCREEN_WIDTH / 2-200.0f+(450.0f*nCntCharacter),SCREEN_HEIGHT/2, 0.0f);//真ん中に三段オプションコマンドを用意する。
		g_aCharacter[nCntCharacter].nType = nCntCharacter;//4つのポリゴンにテクスチャを割り当てる。
		g_aCharacter[nCntCharacter].bChooseCharacter = false;
	}
	
	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffCharacter->Lock(0, 0, (void**)&pVtx, 0);



	for (nCntCharacter = 0; nCntCharacter < MAX_CHARACTER_TEX; nCntCharacter++)
	{
		//コマンドの位置情報の初期化
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
		

		pVtx += 4;//頂点座標のポインタをインクリメント
	}



	//頂点バッファをアンロックする
	g_pVtxBuffCharacter->Unlock();

	
}

//==============================
//リザルト画面の終了処理
//==============================
void UninitChacacter(void)
{
	int nCntCharacter;
	//テクスチャの破棄
	for (nCntCharacter = 0; nCntCharacter < MAX_CHARACTER_TEX; nCntCharacter++)
	{
		if (g_pTextureCharacter[nCntCharacter] != NULL)
		{
			g_pTextureCharacter[nCntCharacter]->Release();
			g_pTextureCharacter[nCntCharacter] = NULL;
		}
	}
	//頂点バッファの破棄
	if (g_pVtxBuffCharacter != NULL)
	{
		g_pVtxBuffCharacter->Release();
		g_pVtxBuffCharacter = NULL;
	}

	if (g_pTextureCharacterBG != NULL)
	{					   
		g_pTextureCharacterBG->Release();
		g_pTextureCharacterBG = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffCharacterBG != NULL)
	{
		g_pVtxBuffCharacterBG->Release();
		g_pVtxBuffCharacterBG = NULL;
	}
}

//===================================
//リザルト画面の更新処理
//===================================
void UpdateCharacter(void)
{
	int nCntCharacter;
	VERTEX_2D* pVtx;

	if (GetKeyboardTrigger(DIK_RIGHT) == true)
	{
		g_nChooseNum = 1;
	}
	if (GetKeyboardTrigger(DIK_LEFT) == true)
	{
		g_nChooseNum = 0;
	}

	if (g_nChooseNum == 1)
	{
		g_aCharacterNum.nCharacterNum = CHACACTER_TYPE_HEAVY;//重いキャラクターをセットする
		g_aCharacter[CHACACTER_TYPE_HEAVY].bChooseCharacter = true;
		g_aCharacter[CHACACTER_TYPE_LIGHT].bChooseCharacter = false;
	}
	else
	{
		g_aCharacterNum.nCharacterNum = CHACACTER_TYPE_LIGHT;//軽いキャラクターをセットする
		g_aCharacter[CHACACTER_TYPE_HEAVY].bChooseCharacter = false;
		g_aCharacter[CHACACTER_TYPE_LIGHT].bChooseCharacter = true;
	}

	if (GetKeyboardTrigger(DIK_RETURN) == true)
	{
		SetFade(MODE_GAME);//ゲームモードを呼び出す
	}

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffCharacter->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntCharacter = 0; nCntCharacter < MAX_CHARACTER_TEX; nCntCharacter++)
	{
	
		//ポーズ画面テクスチャ以外のコマンドの位置を決定する。


			pVtx[0].pos = D3DXVECTOR3(g_aCharacter[nCntCharacter].pos.x -CHARACTER_SIZE_X, g_aCharacter[nCntCharacter].pos.y - CHARACTER_SIZE_Y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aCharacter[nCntCharacter].pos.x +CHARACTER_SIZE_X, g_aCharacter[nCntCharacter].pos.y - CHARACTER_SIZE_Y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aCharacter[nCntCharacter].pos.x -CHARACTER_SIZE_X, g_aCharacter[nCntCharacter].pos.y + CHARACTER_SIZE_Y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aCharacter[nCntCharacter].pos.x +CHARACTER_SIZE_X, g_aCharacter[nCntCharacter].pos.y + CHARACTER_SIZE_Y, 0.0f);

			//頂点カラーの設定（初期設定）
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 100);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 100);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 100);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 100);
		
		if (g_aCharacter[nCntCharacter].bChooseCharacter == true)
		{
			//頂点カラーの設定（選択しているキャラクターが明るくなる）
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		}
		pVtx += 4;//頂点座標のポインタをインクリメント
	}


	//頂点バッファをアンロックする
	g_pVtxBuffCharacter->Unlock();
}

//================================
//タイトル画面の描画処理
//================================
void DrawCharacter(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	int nCntCharacter;
	//デバイスの取得
	pDevice = GetDevice();


	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffCharacterBG, 0, sizeof(VERTEX_2D));//頂点バッファへのポインタと頂点情報の構造体のサイズ

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	pDevice->SetTexture(0, g_pTextureCharacterBG);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);//0は、描画する最初の頂点インデックスで、2は、描画するプリミティブ数
		//&g_pVtxBuffPolygon[0],//頂点情報の先頭アドレス
		//sizeof(VERTEX_2D));//頂点情報構造体のサイズ



	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffCharacter, 0, sizeof(VERTEX_2D));//頂点バッファへのポインタと頂点情報の構造体のサイズ

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);


	for (nCntCharacter = 0; nCntCharacter < MAX_CHARACTER_TEX; nCntCharacter++)
	{
		//テクスチャの設定
		if (g_aCharacter[nCntCharacter].nType == CHACACTER_TYPE_LIGHT)
		{
			pDevice->SetTexture(0, g_pTextureCharacter[CHACACTER_TYPE_LIGHT]);
		}
		if (g_aCharacter[nCntCharacter].nType == CHACACTER_TYPE_HEAVY)
		{
			pDevice->SetTexture(0, g_pTextureCharacter[CHACACTER_TYPE_HEAVY]);
		}
		
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntCharacter * 4, 2);//0は、描画する最初の頂点インデックスで、2は、描画するプリミティブ数
			//&g_pVtxBuffPolygon[0],//頂点情報の先頭アドレス
			//sizeof(VERTEX_2D));//頂点情報構造体のサイズ
	}



	
	
}

CharacterNum* GetCharacterNum(void)
{
	return &g_aCharacterNum;
}
//========================================================
//
//８月３０日：エフェクトもう一回見直す[pause.cpp]
//Author:ShinaTaiyo
//
//=========================================================

#include "result.h"
#include "main.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "Difficulty.h"
#include "enemy.h"

#define DIFFICULTY_SIZE_X (100.0f)
#define DIFFICULTY_SIZE_Y (50.0f)

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureDifficulty[MAX_DIFFICULTY_TEX+MAX_WAVELENGTH_TEX] = {};//テクスチャへのポインタ
//VERTEX_2D g_aVertex[4];//頂点情報を格納（四角形を作りたい）
PDIRECT3DVERTEXBUFFER9 g_pVtxBuffDifficulty = NULL;//頂点バッファへのポインタ

Difficulty g_aDifficulty[MAX_DIFFICULTY_TEX+MAX_WAVELENGTH_TEX];

int g_nChooseDifficulty;//難易度選択を行う変数
int g_nCntShine;//選んだ難易度を光らせる変数

int g_nCntDifficultyTime;

GameCustom g_aGameCustom;//ゲームの難易度や長さをカスタムする

//リザルト画面の初期化処理
void InitDifficulty(void)
{
	int nCntDifficulty;

	g_nCntShine = 1;
	g_nChooseDifficulty = 1;
	g_aGameCustom.nDecideCount = 0;//選択モードを順番に行う

	g_nCntDifficultyTime = 0;

	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	g_aGameCustom.nDifficulty = 1;
	g_aGameCustom.nWaveLength = 1;
	g_aGameCustom.nDecideCount = 0;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\ChooseDifficulty_bg_001.png",
		&g_pTextureDifficulty[0]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\Difficulty_Easy_000.png",
		&g_pTextureDifficulty[1]);


	D3DXCreateTextureFromFile(pDevice,
		"data\\Difficulty_Normal_000.png",
		&g_pTextureDifficulty[2]);


	D3DXCreateTextureFromFile(pDevice,
		"data\\Difficulty_Hard_000.png",
		&g_pTextureDifficulty[3]);


	D3DXCreateTextureFromFile(pDevice,
		"data\\Difficulty_VeryHard_000.png",
		&g_pTextureDifficulty[4]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\ShortMode_000.png",
		&g_pTextureDifficulty[5]);


	D3DXCreateTextureFromFile(pDevice,
		"data\\NormalMode_000.png",
		&g_pTextureDifficulty[6]);


	D3DXCreateTextureFromFile(pDevice,
		"data\\LongMode_000.png",
		&g_pTextureDifficulty[7]);


	D3DXCreateTextureFromFile(pDevice,
		"data\\VeryLongMode.png",
		&g_pTextureDifficulty[8]);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * (MAX_DIFFICULTY_TEX+MAX_WAVELENGTH_TEX),//頂点の数（難易度のテクスチャ*ウェーブの長さのテクスチャ）
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffDifficulty,
		NULL);

	for (nCntDifficulty = 1; nCntDifficulty < MAX_DIFFICULTY_TEX; nCntDifficulty++)
	{//難易度選択部分の文字の位置調整
		g_aDifficulty[nCntDifficulty].pos = D3DXVECTOR3(SCREEN_WIDTH / 3, 144.0f * nCntDifficulty, 0.0f);
	}
	for (nCntDifficulty = 5; nCntDifficulty < (MAX_DIFFICULTY_TEX + MAX_WAVELENGTH_TEX); nCntDifficulty++)
	{//ウェーブの長さ選択部分の文字の位置調整
		g_aDifficulty[nCntDifficulty].pos= D3DXVECTOR3(SCREEN_WIDTH / 2, 144.0f * (nCntDifficulty-4), 0.0f);
	}
	VERTEX_2D* pVtx;
	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffDifficulty->Lock(0, 0, (void**)&pVtx, 0);



	for (nCntDifficulty = 0; nCntDifficulty < (MAX_DIFFICULTY_TEX+MAX_WAVELENGTH_TEX); nCntDifficulty++)
	{
		//頂点座標の設定（初期位置）
		g_aDifficulty[nCntDifficulty].nNumDifficulty = nCntDifficulty;//読み込むテクスチャに番号をつける。
		g_aDifficulty[nCntDifficulty].bChooseDifficulty = false;
		if (g_aDifficulty->nNumDifficulty == DIFFICULTY_BG)
		{//画面全体に表示するテクスチャ
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
		}
		else
		{//コマンドの位置情報の初期化
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
		}

		pVtx += 4;//頂点座標のポインタをインクリメント
	}



	//頂点バッファをアンロックする
	g_pVtxBuffDifficulty->Unlock();
}

//==============================
//リザルト画面の終了処理
//==============================
void UninitDifficulty(void)
{
	int nCntDifficulty;
	//テクスチャの破棄
	for (nCntDifficulty = 0; nCntDifficulty < (MAX_DIFFICULTY_TEX+MAX_WAVELENGTH_TEX); nCntDifficulty++)
	{
		if (g_pTextureDifficulty[nCntDifficulty] != NULL)
		{
			g_pTextureDifficulty[nCntDifficulty]->Release();
			g_pTextureDifficulty[nCntDifficulty] = NULL;
		}
	}
	//頂点バッファの破棄
	if (g_pVtxBuffDifficulty != NULL)
	{
		g_pVtxBuffDifficulty->Release();
		g_pVtxBuffDifficulty = NULL;
	}
}

//===================================
//リザルト画面の更新処理
//===================================
void UpdateDifficulty(void)
{
	int nCntDifficulty;
	VERTEX_2D* pVtx;

	g_nCntDifficultyTime++;


	if (g_nCntDifficultyTime == 1)
	{
		SetEnemy(D3DXVECTOR3(1000.0f, 150.0f, 0.0f), ENEMYTYPE_DELBY, 100000.0f, D3DXVECTOR3(0.0f,0.0f,0.0f));
		SetEnemy(D3DXVECTOR3(920.0f, 200.0f, 0.0f), ENEMYTYPE_PUNDY, 100000.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}

	//========================================================================================
	//選んでいる難易度を光らせる処理
	//========================================================================================
	if (g_aGameCustom.nDecideCount == 0)
	{
		for (nCntDifficulty = 0; nCntDifficulty < MAX_DIFFICULTY_TEX; nCntDifficulty++)
		{//セットされるたびにポーズの明るく表示するフラグをfalseにする
			g_aDifficulty[nCntDifficulty].bChooseDifficulty = false;
		}
		//０番目は、背景であるため、選択しないように調整する。
		if (GetKeyboardTrigger(DIK_S) == true||GetJoyPadTrigger(JOYKEY_DOWN)==true)
		{
			PlaySound(SOUND_LABEL_SE_SELECT_000);
			g_nCntShine++;//選択場所を下にずれる
			g_aGameCustom.nDifficulty++;//難易度の変動
		}
		if (GetKeyboardTrigger(DIK_W) == true||GetJoyPadTrigger(JOYKEY_UP)==true)
		{
			PlaySound(SOUND_LABEL_SE_SELECT_000);
			g_nCntShine--;//選択場所を上にずれる
			g_aGameCustom.nDifficulty--;//難易度の変動
		}
		if (g_nCntShine >= 5)
		{
			g_nCntShine = 1;//一番下から一番上に選択場所を移動
			g_aGameCustom.nDifficulty = 1;//難易度の変動
		}
		if (g_nCntShine <= 0)
		{
			g_nCntShine = 4;//一番上から一番下に選択場所を移動
			g_aGameCustom.nDifficulty = 4;//難易度の変動
		}
		g_aDifficulty[g_nCntShine].bChooseDifficulty = true;
		if (GetKeyboardTrigger(DIK_RETURN) == true||GetJoyPadTrigger(JOYKEY_START)==true)
		{
			PlaySound(SOUND_LABEL_SE_DECISION_000);
			g_nCntShine = 5;
			g_aGameCustom.nDecideCount++;//難易度選択を終了し、ウェーブの長さ選択に移動する。
			
		}
	}
	else if (g_aGameCustom.nDecideCount == 1)
	{
		for (nCntDifficulty = 5; nCntDifficulty < (MAX_DIFFICULTY_TEX+MAX_WAVELENGTH_TEX); nCntDifficulty++)
		{//セットされるたびにポーズの明るく表示するフラグをfalseにする
			g_aDifficulty[nCntDifficulty].bChooseDifficulty = false;
		}
		//０番目は、背景であるため、選択しないように調整する。
		if (GetKeyboardTrigger(DIK_S) == true || GetJoyPadTrigger(JOYKEY_DOWN) == true)
		{
			PlaySound(SOUND_LABEL_SE_SELECT_000);
			g_nCntShine++;//選択場所を下にずれる
			g_aGameCustom.nWaveLength++;//ウェーブの長さの変動
		}
		if (GetKeyboardTrigger(DIK_W) == true || GetJoyPadTrigger(JOYKEY_UP) == true)
		{
			PlaySound(SOUND_LABEL_SE_SELECT_000);
			g_nCntShine--;//選択場所を上にずれる
			g_aGameCustom.nWaveLength--;//ウェーブの長さの変動
		}
		if (g_nCntShine >= 7)
		{
			g_nCntShine = 5;//一番下から一番上に選択場所を移動
			g_aGameCustom.nWaveLength = 1;//ウェーブの長さの変動
		}
		if (g_nCntShine <= 4)
		{
			g_nCntShine = 6;//一番上から一番下に選択場所を移動
			g_aGameCustom.nWaveLength = 2;//ウェーブの長さの変動
		}
		g_aDifficulty[g_nCntShine].bChooseDifficulty = true;

		if (GetKeyboardTrigger(DIK_RETURN) == true || GetJoyPadTrigger(JOYKEY_START) == true)
		{
			StopSound(SOUND_LABEL_BGM000);
			PlaySound(SOUND_LABEL_SE_DECISION_000);
			g_aGameCustom.nDecideCount++;//ウェーブの長さ選択を終了し、ゲームモードへ
			SetFade(MODE_GAME);
		}
	}
	//===========================================================================================


	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffDifficulty->Lock(0, 0, (void**)&pVtx, 0);

	//============================================================
	//難易度選択の処理
	//============================================================
	for (nCntDifficulty = 0; nCntDifficulty < MAX_DIFFICULTY_TEX; nCntDifficulty++)
	{
		//頂点座標の設定（初期位置）

		if (g_aDifficulty[nCntDifficulty].nNumDifficulty >= DIFFICULTY_EASY && g_aDifficulty[nCntDifficulty].nNumDifficulty <= DIFFICULTY_VERYHARD)
		{//ポーズ画面テクスチャ以外のコマンドの位置を決定する。


			pVtx[0].pos = D3DXVECTOR3(g_aDifficulty[nCntDifficulty].pos.x - DIFFICULTY_SIZE_X, g_aDifficulty[nCntDifficulty].pos.y - DIFFICULTY_SIZE_Y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aDifficulty[nCntDifficulty].pos.x + DIFFICULTY_SIZE_X, g_aDifficulty[nCntDifficulty].pos.y - DIFFICULTY_SIZE_Y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aDifficulty[nCntDifficulty].pos.x - DIFFICULTY_SIZE_X, g_aDifficulty[nCntDifficulty].pos.y + DIFFICULTY_SIZE_Y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aDifficulty[nCntDifficulty].pos.x + DIFFICULTY_SIZE_X, g_aDifficulty[nCntDifficulty].pos.y + DIFFICULTY_SIZE_Y, 0.0f);

			//頂点カラーの設定（初期設定）
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 100);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 100);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 100);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 100);
		}
		if (g_aDifficulty[nCntDifficulty].bChooseDifficulty == true)//選んでいる難易度の文字だけを明るくする。それ以外はフラグをfalseにすることにより、難易度表記の文字を薄くする。
		{
			//頂点カラーの設定（選択しているコマンドが明るくなる）
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		}
		pVtx += 4;//頂点座標のポインタをインクリメント
	}
	//==========================================================================================================================================================================================

	//========================================
	//ウェーブの長さの処理
	//========================================
	for (nCntDifficulty = 5; nCntDifficulty < (MAX_DIFFICULTY_TEX+MAX_WAVELENGTH_TEX); nCntDifficulty++)
	{
		//頂点座標の設定（初期位置）

		//if (g_aDifficulty[nCntDifficulty].nNumDifficulty >= DIFFICULTY_EASY && g_aDifficulty[nCntDifficulty].nNumDifficulty <= DIFFICULTY_VERYHARD)
		//{//ポーズ画面テクスチャ以外のコマンドの位置を決定する。


			pVtx[0].pos = D3DXVECTOR3(g_aDifficulty[nCntDifficulty].pos.x - DIFFICULTY_SIZE_X, g_aDifficulty[nCntDifficulty].pos.y - DIFFICULTY_SIZE_Y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aDifficulty[nCntDifficulty].pos.x + DIFFICULTY_SIZE_X, g_aDifficulty[nCntDifficulty].pos.y - DIFFICULTY_SIZE_Y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aDifficulty[nCntDifficulty].pos.x - DIFFICULTY_SIZE_X, g_aDifficulty[nCntDifficulty].pos.y + DIFFICULTY_SIZE_Y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aDifficulty[nCntDifficulty].pos.x + DIFFICULTY_SIZE_X, g_aDifficulty[nCntDifficulty].pos.y + DIFFICULTY_SIZE_Y, 0.0f);

			//頂点カラーの設定（初期設定）
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 100);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 100);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 100);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 100);
		//}
		if (g_aDifficulty[nCntDifficulty].bChooseDifficulty == true)//選んでいる難易度の文字だけを明るくする。それ以外はフラグをfalseにすることにより、難易度表記の文字を薄くする。
		{
			//頂点カラーの設定（選択しているコマンドが明るくなる）
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		}
		pVtx += 4;//頂点座標のポインタをインクリメント
	}
	//=================================================================================================================================================================================

	//頂点バッファをアンロックする
	g_pVtxBuffDifficulty->Unlock();
}

//================================
//タイトル画面の描画処理
//================================
void DrawDifficulty(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	int nCntDifficulty;
	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffDifficulty, 0, sizeof(VERTEX_2D));//頂点バッファへのポインタと頂点情報の構造体のサイズ

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);


	for (nCntDifficulty = 0; nCntDifficulty < (MAX_DIFFICULTY_TEX+MAX_WAVELENGTH_TEX); nCntDifficulty++)
	{
		//テクスチャの設定
		if (g_aDifficulty[nCntDifficulty].nNumDifficulty == DIFFICULTY_BG)
		{
			pDevice->SetTexture(0, g_pTextureDifficulty[DIFFICULTY_BG]);
		}
		if (g_aDifficulty[nCntDifficulty].nNumDifficulty == DIFFICULTY_EASY)
		{
			pDevice->SetTexture(0, g_pTextureDifficulty[DIFFICULTY_EASY]);
		}
		if (g_aDifficulty[nCntDifficulty].nNumDifficulty == DIFFICULTY_NORMAL)
		{
			pDevice->SetTexture(0, g_pTextureDifficulty[DIFFICULTY_NORMAL]);
		}
		if (g_aDifficulty[nCntDifficulty].nNumDifficulty == DIFFICULTY_HARD)
		{
			pDevice->SetTexture(0, g_pTextureDifficulty[DIFFICULTY_HARD]);
		}
		if (g_aDifficulty[nCntDifficulty].nNumDifficulty == DIFFICULTY_VERYHARD)
		{
			pDevice->SetTexture(0, g_pTextureDifficulty[DIFFICULTY_VERYHARD]);
		}
		if (g_aDifficulty[nCntDifficulty].nNumDifficulty == WAVELENGTH_SHORT)
		{
			pDevice->SetTexture(0, g_pTextureDifficulty[WAVELENGTH_SHORT]);
		}
		if (g_aDifficulty[nCntDifficulty].nNumDifficulty == WAVELENGTH_NORMAL)
		{
			pDevice->SetTexture(0, g_pTextureDifficulty[WAVELENGTH_NORMAL]);
		}
		if (g_aDifficulty[nCntDifficulty].nNumDifficulty == WAVELENGTH_LONG)
		{
			pDevice->SetTexture(0, g_pTextureDifficulty[WAVELENGTH_LONG]);
		}
		if (g_aDifficulty[nCntDifficulty].nNumDifficulty == WAVELENGTH_VERYLONG)
		{
			pDevice->SetTexture(0, g_pTextureDifficulty[WAVELENGTH_VERYLONG]);
		}
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntDifficulty * 4, 2);//0は、描画する最初の頂点インデックスで、2は、描画するプリミティブ数
			//&g_pVtxBuffPolygon[0],//頂点情報の先頭アドレス
			//sizeof(VERTEX_2D));//頂点情報構造体のサイズ
	}
}

//=============================================
//ゲームのモードカスタムの構造体をゲットする。
//=============================================
GameCustom *GetGameCustom(void)
{
	return &g_aGameCustom;
}

	
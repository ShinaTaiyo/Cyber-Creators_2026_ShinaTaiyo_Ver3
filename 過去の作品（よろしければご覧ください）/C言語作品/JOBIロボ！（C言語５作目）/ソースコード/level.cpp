//=====================================================================================================================
//
//１２月２６日：チーム制作の基盤づくり[Level.cpp]
//Author:ShinaTaiyo
//
// 
// 注意：fRWidth,fRHeightは、中心点からの半径で大きさ、判定などを取っています。もしこの処理を使うなら、ご注意下さい。
// 
//======================================================================================================================

#include "level.h"
#include "main.h"
#include "sound.h"
#include "player.h"
#include "battery.h"

#define LEVEL_WIDTH (60.0f)
#define LEVEL_HEIGHT (30.0f)

#define LEVELNUM_WIDTH (20.0f)
#define LEVELNUM_HEIGHT (40.0f)
#define LEVELDIGIT (2)//スクラップの数表示の桁数

#define LEVELUPDISPLAY_WIDTH (25.0f)
#define LEVELUPDISPLAY_HEIGHT (10.0f)

//=================================
//レベルフレーム表示用
//=================================
LPDIRECT3DTEXTURE9 g_pTextureLevel[NUM_LEVEL] = {};//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffLevel = NULL;//レベルの情報
Level g_aLevel[NUM_LEVEL];//レベル表示用

//=================================
//レベル数値表示用
//=================================
LPDIRECT3DTEXTURE9 g_pTextureLevelNum = NULL;//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffLevelNum = NULL;//レベル数値の情報
LevelNum g_aLevelNum[NUM_LEVEL][LEVEL_DIGIT];//レベル数値の構造体

//=================================
//レベルアップ表示用
//==================================
LPDIRECT3DTEXTURE9 g_pTextureLevelUpDisplay = NULL;//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffLevelUpDisplay = NULL;//頂点情報
LevelUpDisplay g_aLevelUpDisplay[MAX_LEVELUPDISPLAY];//レベルアップ表示の構造体
int g_nUseLevelUpDisplay;//レベルアップディスプレイを使用している数をカウントする

LPDIRECT3DTEXTURE9 g_pTextureLevelUpDisplay2D = NULL;//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffLevelUpDisplay2D = NULL;//頂点情報
LevelUpDisplay g_aLevelUpDisplay2D[MAX_LEVELUPDISPLAY];//レベルアップ表示の構造体
int g_nUseLevelUpDisplay2D[NUM_LEVEL];//レベルを表示するステータスの数分、使用されているレベルアップ数を表示する


//============================
// 初期化処理
//============================
void InitLevel(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

 	pDevice = GetDevice();

	VERTEX_2D* pVtx;

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\LvDisplay_000_1.png",
		&g_pTextureLevel[0]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\LvDisplay_001.png",
		&g_pTextureLevel[1]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\LvDisplay_002.png",
		&g_pTextureLevel[2]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\LvDisplay_003.png",
		&g_pTextureLevel[3]);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_LEVEL,//４は必要な頂点数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffLevel,
		NULL);

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffLevel->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntLevel = 0; nCntLevel < NUM_LEVEL; nCntLevel++)
	{
		g_aLevel[nCntLevel].nType = nCntLevel;
		g_aLevel[nCntLevel].pos = D3DXVECTOR3(80.0f + 250.0f * nCntLevel, 620.0f, 0.0f);
		g_aLevel[nCntLevel].nLevel = 0;


		pVtx[0].pos = D3DXVECTOR3(g_aLevel[nCntLevel].pos.x - LEVEL_WIDTH, g_aLevel[nCntLevel].pos.y - LEVEL_HEIGHT, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aLevel[nCntLevel].pos.x + LEVEL_WIDTH, g_aLevel[nCntLevel].pos.y - LEVEL_HEIGHT, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aLevel[nCntLevel].pos.x - LEVEL_WIDTH, g_aLevel[nCntLevel].pos.y + LEVEL_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aLevel[nCntLevel].pos.x + LEVEL_WIDTH, g_aLevel[nCntLevel].pos.y + LEVEL_HEIGHT, 0.0f);

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
    g_pVtxBuffLevel->Unlock();

	//==========================
	//レベル数値の処理
	//==========================

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Lv_Num.png",
		&g_pTextureLevelNum);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_LEVEL * LEVELDIGIT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffLevelNum,
		NULL);

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffLevelNum->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntLevel = 0; nCntLevel < NUM_LEVEL; nCntLevel++)
	{
		for (int nCntDigit = 0; nCntDigit < LEVELDIGIT; nCntDigit++)
		{
			g_aLevelNum[nCntLevel][nCntDigit].pos = D3DXVECTOR3(200.0f + 250.0f * nCntLevel + 40.0f * -nCntDigit, 620.0f, 0.0f);

			pVtx[0].pos = D3DXVECTOR3(g_aLevelNum[nCntLevel][nCntDigit].pos.x - LEVELNUM_WIDTH, g_aLevelNum[nCntLevel][nCntDigit].pos.y - LEVELNUM_HEIGHT, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aLevelNum[nCntLevel][nCntDigit].pos.x + LEVELNUM_WIDTH, g_aLevelNum[nCntLevel][nCntDigit].pos.y - LEVELNUM_HEIGHT, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aLevelNum[nCntLevel][nCntDigit].pos.x - LEVELNUM_WIDTH, g_aLevelNum[nCntLevel][nCntDigit].pos.y + LEVELNUM_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aLevelNum[nCntLevel][nCntDigit].pos.x + LEVELNUM_WIDTH, g_aLevelNum[nCntLevel][nCntDigit].pos.y + LEVELNUM_HEIGHT, 0.0f);
															
			//rhwの設定（初期設定）
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			switch (nCntLevel)
			{
			case 0:
				//頂点カラーの設定（初期設定）
				pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				break;
			case 1:
				//頂点カラーの設定（初期設定）
				pVtx[0].col = D3DCOLOR_RGBA(0, 100, 255, 255);
				pVtx[1].col = D3DCOLOR_RGBA(0, 100, 255, 255);
				pVtx[2].col = D3DCOLOR_RGBA(0, 100, 255, 255);
				pVtx[3].col = D3DCOLOR_RGBA(0, 100, 255, 255);
				break;
			case 2:
				//頂点カラーの設定（初期設定）
				pVtx[0].col = D3DCOLOR_RGBA(255, 255, 0, 255);
				pVtx[1].col = D3DCOLOR_RGBA(255, 255, 0, 255);
				pVtx[2].col = D3DCOLOR_RGBA(255, 255, 0, 255);
				pVtx[3].col = D3DCOLOR_RGBA(255, 255, 0, 255);
				break;
			case 3:
				//頂点カラーの設定（初期設定）
				pVtx[0].col = D3DCOLOR_RGBA(255, 0, 0, 255);
				pVtx[1].col = D3DCOLOR_RGBA(255, 0, 0, 255);
				pVtx[2].col = D3DCOLOR_RGBA(255, 0, 0, 255);
				pVtx[3].col = D3DCOLOR_RGBA(255, 0, 0, 255);
				break;
			}
			//テクスチャ座標の設定（初期設定）
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

			pVtx += 4;//頂点座標のポインタをインクリメント
		}
	}
	//頂点バッファをアンロックする
	g_pVtxBuffLevelNum->Unlock();
}

//==============================
// 終了処理
//==============================
void UninitLevel(void)
{
	//============================
	//レベルフレームの処理
	//============================
	
	//テクスチャの破棄
	for (int nCntLevel = 0; nCntLevel < NUM_LEVEL; nCntLevel++)
	{
		if (g_pTextureLevel[nCntLevel] != NULL)
		{
			g_pTextureLevel[nCntLevel]->Release();
			g_pTextureLevel[nCntLevel] = NULL;
		}
	}
	//頂点バッファの破棄
	if (g_pVtxBuffLevel != NULL)
	{
		g_pVtxBuffLevel->Release();
		g_pVtxBuffLevel = NULL;
	}
  
	//==============================
	//レベル数値の処理
	//==============================

	//テクスチャの破棄
	if (g_pTextureLevelNum != NULL)
	{
		g_pTextureLevelNum->Release();
		g_pTextureLevelNum = NULL;
	}
	//頂点バッファの破棄
	if (g_pVtxBuffLevel != NULL)
	{
		g_pVtxBuffLevel->Release();
		g_pVtxBuffLevel = NULL;
	}
}

//============================
// 更新処理
//============================
void UpdateLevel(void)
{


	
}

//================================
// 描画処理
//================================
void DrawLevel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスの取得

	//===========================
	//レベルフレームの処理
	//===========================

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffLevel, 0, sizeof(VERTEX_2D));//頂点バッファへのポインタと頂点情報の構造体のサイズ

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntLevel = 0; nCntLevel < NUM_LEVEL; nCntLevel++)
	{
		pDevice->SetTexture(0, g_pTextureLevel[g_aLevel[nCntLevel].nType]);
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntLevel * 4, 2);//0は、描画する最初の頂点インデックスで、2は、描画するプリミティブ数
	}

	//=============================
	//レベル数値の処理
	//=============================

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffLevelNum, 0, sizeof(VERTEX_2D));//頂点バッファへのポインタと頂点情報の構造体のサイズ

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntLevelNum = 0; nCntLevelNum < NUM_LEVEL * LEVELDIGIT; nCntLevelNum++)
	{
		pDevice->SetTexture(0, g_pTextureLevelNum);
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntLevelNum * 4, 2);
	}

}

//===================================
//レベルアップ表示の初期化
//===================================
void InitLevelUpDisplay(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	pDevice = GetDevice();

	VERTEX_3D* pVtx;

	g_nUseLevelUpDisplay = 0;//使用しているレベルアップ表示の数をカウントする

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\LvUp000.png",
		&g_pTextureLevelUpDisplay);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_LEVELUPDISPLAY,//４は必要な頂点数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffLevelUpDisplay,
		NULL);

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffLevelUpDisplay->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntLevel = 0; nCntLevel < MAX_LEVELUPDISPLAY; nCntLevel++)
	{
		g_aLevelUpDisplay[nCntLevel].bUse = false;
		g_aLevelUpDisplay[nCntLevel].col = {};
		g_aLevelUpDisplay[nCntLevel].pos = NULL_VECTOR3;
		g_aLevelUpDisplay[nCntLevel].nLife = 0;

		pVtx[0].pos = NULL_VECTOR3;
		pVtx[1].pos = NULL_VECTOR3;
		pVtx[2].pos = NULL_VECTOR3;
		pVtx[3].pos = NULL_VECTOR3;

		//norの設定（初期設定）
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

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
	g_pVtxBuffLevelUpDisplay->Unlock();
}

//===================================
//レベルアップ表示の終了処理
//===================================
void UninitLevelUpDisplay(void)
{

	if (g_pTextureLevelUpDisplay != NULL)
	{
		g_pTextureLevelUpDisplay->Release();
		g_pTextureLevelUpDisplay = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffLevelUpDisplay != NULL)
	{
		g_pVtxBuffLevelUpDisplay->Release();
		g_pVtxBuffLevelUpDisplay = NULL;
	}

}

//===================================
//レベルアップ表示の更新処理
//===================================
void UpdateLevelUpDisplay(void)
{
	VERTEX_3D* pVtx;

	Player* pPlayer = GetPlayer();

	int nCntUseDisplay = 0;//レベルアップ表示を使用している数

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffLevelUpDisplay->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntLevelUpDisplay = 0; nCntLevelUpDisplay < MAX_LEVELUPDISPLAY; nCntLevelUpDisplay++)
	{
		if (g_aLevelUpDisplay[nCntLevelUpDisplay].bUse == true)
		{
			nCntUseDisplay++;

			g_aLevelUpDisplay[nCntLevelUpDisplay].nLife -= 1;

			g_aLevelUpDisplay[nCntLevelUpDisplay].pos = D3DXVECTOR3(pPlayer->pos.x, pPlayer->pos.y + (pPlayer->Size.y * 2.0f) + ((LEVELUPDISPLAY_HEIGHT * 2.0f) * (float)(nCntUseDisplay)), pPlayer->pos.z);
		
			if (g_aLevelUpDisplay[nCntLevelUpDisplay].nLife <= 0)
			{
				g_nUseLevelUpDisplay--;//レベルアップ表示を使用している数をカウントする

				g_aLevelUpDisplay[nCntLevelUpDisplay].bUse = false;
			}
		}
	}

	g_pVtxBuffLevelUpDisplay->Unlock();

}

//===================================
//レベルアップ表示の描画処理
//===================================
void DrawLevelUpDisplay(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	for (int nCntLevelUpDisplay = 0; nCntLevelUpDisplay < MAX_LEVELUPDISPLAY; nCntLevelUpDisplay++)
	{
		if (g_aLevelUpDisplay[nCntLevelUpDisplay].bUse == true)
		{
			D3DXMATRIX mtxTrans;//計算用マトリックス

			D3DXMATRIX mtxView;//ビューマトリックス取得用

			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aLevelUpDisplay[nCntLevelUpDisplay].mtxWorld);

			//ビューマトリックスを取得
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//ポリゴンをカメラに対して正面に向ける
			D3DXMatrixInverse(&g_aLevelUpDisplay[nCntLevelUpDisplay].mtxWorld, NULL, &mtxView);

			//逆行列を求める
			g_aLevelUpDisplay[nCntLevelUpDisplay].mtxWorld._41 = 0.0f;
			g_aLevelUpDisplay[nCntLevelUpDisplay].mtxWorld._42 = 0.0f;
			g_aLevelUpDisplay[nCntLevelUpDisplay].mtxWorld._43 = 0.0f;

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aLevelUpDisplay[nCntLevelUpDisplay].pos.x, g_aLevelUpDisplay[nCntLevelUpDisplay].pos.y, g_aLevelUpDisplay[nCntLevelUpDisplay].pos.z);
			D3DXMatrixMultiply(&g_aLevelUpDisplay[nCntLevelUpDisplay].mtxWorld, &g_aLevelUpDisplay[nCntLevelUpDisplay].mtxWorld, &mtxTrans);

			//===============================================================
			//描画の調整
			//==============================================================
			//ライトを無効にする
			pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

			//Zの比較方法変更。
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

			//Zバッファに書き込まない
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

			//アルファテストを有効
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
			pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
			pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aLevelUpDisplay[nCntLevelUpDisplay].mtxWorld);


			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffLevelUpDisplay, 0, sizeof(VERTEX_3D));//頂点バッファへのポインタと頂点情報の構造体のサイズ

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureLevelUpDisplay);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntLevelUpDisplay * 4, 2);//0は、描画する最初の頂点インデックスで、2は、描画するプリミティブ

			//============================
			//描画の調整をもとに戻す
			//============================

			//ライトを有効に戻す
			pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

			//Zの比較方法変更。
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

			//Zバッファに書き込む
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

			//アルファテストを無効に戻す
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

		}
	}

}

//=======================================================
//レベル構造体の情報を取得
//=======================================================
Level* GetLevel(void)//スクラップ構造体の情報をゲットする
{
	return &g_aLevel[0];
}

//===========================
//レベルを上げる
//===========================
void AddLevel(int nType)//レベルを上げる。
{
	Battery* pBattery = GetBattery();

	PlaySound(SOUND_LABEL_SE_STATUSBUFF_000);

	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffLevelNum->Lock(0, 0, (void**)&pVtx, 0);
	g_aLevel[nType].nLevel += 1;//レベルを1上げる

	switch (nType)
	{
	case 0:
		SetLevelUpDisplay(D3DXVECTOR3(g_aLevel[nType].pos.x + LEVEL_WIDTH, g_aLevel[nType].pos.y - LEVEL_HEIGHT - LEVELUPDISPLAY_HEIGHT, g_aLevel[nType].pos.z),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 60);

		SetLevelUpDisplay2D(D3DXVECTOR3(g_aLevel[nType].pos.x + LEVEL_WIDTH, g_aLevel[nType].pos.y - LEVEL_HEIGHT - LEVELUPDISPLAY_HEIGHT * 4.0f, g_aLevel[nType].pos.z),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 60,nType);
		break;

	case 1:
		SetLevelUpDisplay(D3DXVECTOR3(g_aLevel[nType].pos.x + LEVEL_WIDTH, g_aLevel[nType].pos.y - LEVEL_HEIGHT - LEVELUPDISPLAY_HEIGHT, g_aLevel[nType].pos.z),
			D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), 60);
		SetLevelUpDisplay2D(D3DXVECTOR3(g_aLevel[nType].pos.x + LEVEL_WIDTH, g_aLevel[nType].pos.y - LEVEL_HEIGHT - LEVELUPDISPLAY_HEIGHT * 4.0f, g_aLevel[nType].pos.z),
			D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), 60, nType);
		break;

	case 2:
		SetLevelUpDisplay(D3DXVECTOR3(g_aLevel[nType].pos.x + LEVEL_WIDTH, g_aLevel[nType].pos.y - LEVEL_HEIGHT - LEVELUPDISPLAY_HEIGHT, g_aLevel[nType].pos.z),
			D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), 60);

		SetLevelUpDisplay2D(D3DXVECTOR3(g_aLevel[nType].pos.x + LEVEL_WIDTH, g_aLevel[nType].pos.y - LEVEL_HEIGHT - LEVELUPDISPLAY_HEIGHT * 4.0f, g_aLevel[nType].pos.z),
			D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), 60, nType);
		break;

	case 3:
		SetLevelUpDisplay(D3DXVECTOR3(g_aLevel[nType].pos.x + LEVEL_WIDTH, g_aLevel[nType].pos.y - LEVEL_HEIGHT - LEVELUPDISPLAY_HEIGHT, g_aLevel[nType].pos.z),
			D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 60);

		SetLevelUpDisplay2D(D3DXVECTOR3(g_aLevel[nType].pos.x + LEVEL_WIDTH, g_aLevel[nType].pos.y - LEVEL_HEIGHT - LEVELUPDISPLAY_HEIGHT * 4.0f, g_aLevel[nType].pos.z),
			D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 60, nType);
		break;

	default:
		break;
	}

	int nGetLevel = g_aLevel[nType].nLevel;//スコアを保存する
	int nMatchType = 0;//呼ばれた種類と一致したかどうか
	while (1)
	{
		for (int nCntDigit = 0; nCntDigit < LEVELDIGIT; nCntDigit++)
		{
			if (nMatchType == nType)
			{
				

				g_aLevelNum[nType][nCntDigit].fPosTexU = (float)(nGetLevel % 10);
				nGetLevel /= 10;

				pVtx[0].tex = D3DXVECTOR2(0.0f + (0.1f * g_aLevelNum[nType][nCntDigit].fPosTexU), 0.0f);
				pVtx[1].tex = D3DXVECTOR2(0.1f + (0.1f * g_aLevelNum[nType][nCntDigit].fPosTexU), 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f + (0.1f * g_aLevelNum[nType][nCntDigit].fPosTexU), 1.0f);
				pVtx[3].tex = D3DXVECTOR2(0.1f + (0.1f * g_aLevelNum[nType][nCntDigit].fPosTexU), 1.0f);
			}
			pVtx += 4;//頂点データのポインタを４つ分進める。
		}

		if (nMatchType == nType)
		{
			if (nType == 0)
			{
				pBattery->nBattery += 100;
			}
			break;
		}
		nMatchType++;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffLevelNum->Unlock();
}

//==================================
//レベルアップ表示の設定
//==================================
void SetLevelUpDisplay(D3DXVECTOR3 pos, D3DXCOLOR col, int nLife)
{
	VERTEX_3D* pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffLevelUpDisplay->Lock(0, 0, (void**)&pVtx, 0);

	g_nUseLevelUpDisplay++;//レベルアップ表示を使用している数をカウントする

	for (int nCntLevelUpDisplay = 0; nCntLevelUpDisplay < MAX_LEVELUPDISPLAY; nCntLevelUpDisplay++)
	{
		if (g_aLevelUpDisplay[nCntLevelUpDisplay].bUse == false)
		{
			g_aLevelUpDisplay[nCntLevelUpDisplay].bUse = true;
			g_aLevelUpDisplay[nCntLevelUpDisplay].col = col;
			g_aLevelUpDisplay[nCntLevelUpDisplay].nLife = nLife;
			g_aLevelUpDisplay[nCntLevelUpDisplay].pos = pos;

			//頂点座標の設定（初期位置）
			pVtx[0].pos = D3DXVECTOR3(-LEVELUPDISPLAY_WIDTH, LEVELUPDISPLAY_HEIGHT, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(LEVELUPDISPLAY_WIDTH, LEVELUPDISPLAY_HEIGHT, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-LEVELUPDISPLAY_WIDTH, -LEVELUPDISPLAY_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(LEVELUPDISPLAY_WIDTH, -LEVELUPDISPLAY_HEIGHT, 0.0f);

			//頂点カラーの設定（初期設定）
			pVtx[0].col = col;
			pVtx[1].col = col;
			pVtx[2].col = col;
			pVtx[3].col = col;
			break;
		}
		pVtx += 4;
	}

	g_pVtxBuffLevelUpDisplay->Unlock();
}

//=================================================================================================

//===========================================
//2Dレベルアップ表示の初期化処理
//===========================================
void InitLevelUpDisplay2D(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	pDevice = GetDevice();

	VERTEX_2D* pVtx;

	for (int nCntLevel = 0; nCntLevel < NUM_LEVEL; nCntLevel++)
	{
		g_nUseLevelUpDisplay2D[nCntLevel] = 0;
	}
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\LvUp000.png",
		&g_pTextureLevelUpDisplay2D);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_LEVELUPDISPLAY,//４は必要な頂点数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffLevelUpDisplay2D,
		NULL);

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffLevelUpDisplay2D->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntLevel = 0; nCntLevel < MAX_LEVELUPDISPLAY; nCntLevel++)
	{
		g_aLevelUpDisplay2D[nCntLevel].bUse = false;//使用状態
		g_aLevelUpDisplay2D[nCntLevel].col = {};//色
		g_aLevelUpDisplay2D[nCntLevel].pos = NULL_VECTOR3;//位置
		g_aLevelUpDisplay2D[nCntLevel].nLife = 0;//体力

		pVtx[0].pos = NULL_VECTOR3;
		pVtx[1].pos = NULL_VECTOR3;
		pVtx[2].pos = NULL_VECTOR3;
		pVtx[3].pos = NULL_VECTOR3;

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
	g_pVtxBuffLevelUpDisplay2D->Unlock();
}

void UninitLevelUpDisplay2D(void)
{
	if (g_pTextureLevelUpDisplay2D != NULL)
	{
		g_pTextureLevelUpDisplay2D->Release();
		g_pTextureLevelUpDisplay2D = NULL;
	}

	if (g_pVtxBuffLevelUpDisplay2D != NULL)
	{
		g_pVtxBuffLevelUpDisplay2D->Release();
		g_pVtxBuffLevelUpDisplay2D = NULL;
	}
}

void UpdateLevelUpDisplay2D(void)
{
	for (int nCntLevel = 0; nCntLevel < MAX_LEVELUPDISPLAY; nCntLevel++)
	{
		if (g_aLevelUpDisplay2D[nCntLevel].bUse == true)
		{
			g_aLevelUpDisplay2D[nCntLevel].nLife--;

			if (g_aLevelUpDisplay2D[nCntLevel].nLife <= 0)
			{
				g_aLevelUpDisplay2D[nCntLevel].bUse = false;
			}
		}
	}
}

void DrawLevelUpDisplay2D(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスの取得
	pDevice->SetStreamSource(0, g_pVtxBuffLevelUpDisplay2D, 0, sizeof(VERTEX_2D));//頂点バッファへのポインタと頂点情報の構造体のサイズ
	pDevice->SetFVF(FVF_VERTEX_2D);
	

	for (int nCntLevel = 0; nCntLevel < MAX_LEVELUPDISPLAY; nCntLevel++)
	{
		if (g_aLevelUpDisplay2D[nCntLevel].bUse == true)
		{
			pDevice->SetTexture(0, g_pTextureLevelUpDisplay2D);
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntLevel * 4, 2);//0は、描画する最初の頂点インデックスで、2は、描画するプリミティブ数
		}
	}
}

void SetLevelUpDisplay2D(D3DXVECTOR3 pos, D3DXCOLOR col, int nLife, int nType)
{
	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffLevelUpDisplay2D->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntLevelUpDisplay2D = 0; nCntLevelUpDisplay2D < MAX_LEVELUPDISPLAY; nCntLevelUpDisplay2D++)
	{
		if (g_aLevelUpDisplay[nCntLevelUpDisplay2D].bUse == false)
		{
			g_aLevelUpDisplay2D[nCntLevelUpDisplay2D].bUse = true;
			g_aLevelUpDisplay2D[nCntLevelUpDisplay2D].col = col;
			g_aLevelUpDisplay2D[nCntLevelUpDisplay2D].nLife = nLife;
			g_aLevelUpDisplay2D[nCntLevelUpDisplay2D].pos = pos;
			g_aLevelUpDisplay2D[nCntLevelUpDisplay2D].nType = nType;

			//頂点座標の設定（初期位置）
			pVtx[0].pos = D3DXVECTOR3(g_aLevelUpDisplay2D[nCntLevelUpDisplay2D].pos.x - LEVELUPDISPLAY_WIDTH * 3.0f, g_aLevelUpDisplay2D[nCntLevelUpDisplay2D].pos.y - LEVELUPDISPLAY_HEIGHT * 3.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aLevelUpDisplay2D[nCntLevelUpDisplay2D].pos.x + LEVELUPDISPLAY_WIDTH * 3.0f, g_aLevelUpDisplay2D[nCntLevelUpDisplay2D].pos.y - LEVELUPDISPLAY_HEIGHT * 3.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aLevelUpDisplay2D[nCntLevelUpDisplay2D].pos.x - LEVELUPDISPLAY_WIDTH * 3.0f, g_aLevelUpDisplay2D[nCntLevelUpDisplay2D].pos.y + LEVELUPDISPLAY_HEIGHT * 3.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aLevelUpDisplay2D[nCntLevelUpDisplay2D].pos.x + LEVELUPDISPLAY_WIDTH * 3.0f, g_aLevelUpDisplay2D[nCntLevelUpDisplay2D].pos.y + LEVELUPDISPLAY_HEIGHT * 3.0f, 0.0f);

			//頂点カラーの設定（初期設定）
			pVtx[0].col = col;
			pVtx[1].col = col;
			pVtx[2].col = col;
			pVtx[3].col = col;
			break;
		}
		pVtx += 4;
	}

	g_pVtxBuffLevelUpDisplay->Unlock();
}



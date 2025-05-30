//========================================================
//
//９月２６日：ハッカソンの基盤作り[Wall.cpp]
//Author:ShinaTaiyo
//
//=========================================================
#include <stdio.h>
#include <string.h>

#include "wall.h"
#include "main.h"
#include "input.h"
#include "sound.h"
#include "fade.h"

#define WALLSET "data\\TEXTFILE\\WallSet.txt"//壁セット構造体を呼ぶ

//グローバル変数
PDIRECT3DVERTEXBUFFER9 g_pVtxBuffWall = NULL;//頂点バッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureWall = NULL;//テクスチャへのポインタ
D3DXVECTOR3 g_posWall;//位置
D3DXVECTOR3 g_rotWall;//向き
D3DXMATRIX g_mtxWorldWall;//ワールドマトリックス(float4*4)


Wall g_aWall[MAX_WALL];//壁の情報

float g_fRotWall;

#define WALL_SCALE (200.0f)

//タイトル画面の初期化処理
void InitWall(void)
{
	g_fRotWall = 0.0f;

	//bFadeFlag = false;

	//g_nCntTime = 0;

	//g_nColorChange = 255;//カラーの初期化（白色）

	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\win_001.png",
		&g_pTextureWall);

	//g_posWall = D3DXVECTOR3(0.0f, 0.0f, 200.0f);//位置の初期化
	//g_rotWall = D3DXVECTOR3(0.0f, -D3DX_PI * 0.0f, 0.0f);//向きの初期化

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		g_aWall[nCntWall].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aWall[nCntWall].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aWall[nCntWall].fWidth = 0.0f;
		g_aWall[nCntWall].fHeight = 0.0f;
		g_aWall[nCntWall].bUse = false;
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_WALL,/** MAX_WALL,*///頂点の数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffWall,
		NULL);


	////タイトル画面BGMの設定
	//PlaySound(SOUND_LABEL_BGM000);

	VERTEX_3D* pVtx;
	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffWall->Lock(0, 0, (void**)&pVtx, 0);


	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		//頂点座標の設定（初期位置）
		pVtx[0].pos = D3DXVECTOR3(0.0f,0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//norの設定（初期設定）
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。


		//テクスチャ座標の設定（初期設定）
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffWall->Unlock();


	FILE* pFile;//ファイルポインタを宣言

	char aString[100];

	int nCnt = 0;//カウント

	int nType = 0;//種類
	
	float fPosX = 0.0f;//位置X
	float fPosY = 0.0f;//位置Y
	float fPosZ = 0.0f;//位置Z

	float fRotX = 0.0f;//向きX
	float fRotY = 0.0f;//向きY
	float fRotZ = 0.0f;//向きZ

	float fWidth = 0.0f;//幅
	float fHeight = 0.0f;//高さ

	//pFile = fopen(WALLSET, "r");

	//if (pFile != NULL)//ファイルが開いているかどうか
	//{
	//	while (1)
	//	{

	//		fscanf(pFile,"%s",&aString[0]);
	//		
	//		if (strcmp(&aString[0], "ENDSCRIPT") == 0)
	//		{
	//			break;//行の最後でこの文字列を判定させることにより、ループを終わらせる
	//		}
	//		else if (strcmp(&aString[0], "WALLSET") == 0)
	//		{
	//			while (1)
	//			{
	//				fscanf(pFile, "%s", &aString[0]);
	//				if (strcmp(&aString[0], "TYPE") == 0)
	//				{
	//					fscanf(pFile, "%d", &nType);//ウォールの種類を決める
	//				}
	//				else if (strcmp(&aString[0], "POS") == 0)
	//				{
	//					fscanf(pFile, "%f%f%f", &fPosX, &fPosY, &fPosZ);//位置情報を読み込む
	//				}
	//				else if (strcmp(&aString[0], "ROT") == 0)
	//				{
	//					fscanf(pFile, "%f%f%f", &fRotX, &fRotY, &fRotZ);//向き情報を読み込む
	//				}
	//				else if (strcmp(&aString[0], "WIDTH") == 0)
	//				{
	//					fscanf(pFile, "%f", &fWidth);//幅情報を読み込む
	//				}
	//				else if (strcmp(&aString[0], "HEIGHT") == 0)
	//				{
	//					fscanf(pFile, "%f", &fHeight);//高さ情報を読み込む
	//				}
	//				else if (strcmp(&aString[0], "ENDWALLSET") == 0)
	//				{
	//					SetWall(D3DXVECTOR3(fPosX, fPosY, fPosZ), D3DXVECTOR3(fRotX, fRotY, fRotZ), fWidth, fHeight);
	//					break;
	//				}
	//				nCnt++;
	//			}
	//		}
	//		nCnt++;
	//	}
	//fclose(pFile);
	//}


	SetWall(D3DXVECTOR3(0.0f, 0.0f, 3000.0f),D3DXVECTOR3(0.0f,-D3DX_PI * 0.0f,0.0f),3000.0f,3000.0f);
	SetWall(D3DXVECTOR3(0.0f, 0.0f, -3000.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 1.0f, 0.0f), 3000.0f, 3000.0f);
	SetWall(D3DXVECTOR3(-3000.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.5f, 0.0f), 3000.0f, 3000.0f);
	SetWall(D3DXVECTOR3(3000.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 1.5f, 0.0f), 3000.0f, 3000.0f);
}

//==============================
//タイトル画面の終了処理
//==============================
void UninitWall(void)
{
	//テクスチャの破棄
	if (g_pTextureWall != NULL)
	{
		g_pTextureWall->Release();
		g_pTextureWall = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffWall != NULL)
	{
		g_pVtxBuffWall->Release();
		g_pVtxBuffWall = NULL;
	}
}
//============================
//タイトル画面の更新処理
//============================
void UpdateWall(void)
{

	if (GetKeyboardPress(DIK_R) == true)
	{
		g_fRotWall += 0.01f;
	}
	VERTEX_3D* pVtx;
	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffWall->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		if (g_aWall[nCntWall].bUse == true)
		{
			//頂点座標の設定（初期位置）
			pVtx[0].pos = D3DXVECTOR3(- g_aWall[nCntWall].fWidth,g_aWall[nCntWall].fHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aWall[nCntWall].fWidth,g_aWall[nCntWall].fHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(- g_aWall[nCntWall].fWidth, 0.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aWall[nCntWall].fWidth, 0.0f, 0.0f);
		}
			pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffWall->Unlock();




	g_rotWall = D3DXVECTOR3(0.0f,0.0f + (-D3DX_PI * g_fRotWall), 0.0f);//向きの初期化
}



//================================
//タイトル画面の描画処理
//================================
void DrawWall(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

		//デバイスの取得
	pDevice = GetDevice();

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		if (g_aWall[nCntWall].bUse == true)
		{
	        D3DXMATRIX mtxRot, mtxTrans;//計算用マトリックス
	        
	        //ワールドマトリックスの初期化
	        D3DXMatrixIdentity(&g_mtxWorldWall);
	        
	        //向きを反映
	        D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aWall[nCntWall].rot.y, g_aWall[nCntWall].rot.x, g_aWall[nCntWall].rot.z);
	        
	        D3DXMatrixMultiply(&g_mtxWorldWall, &g_mtxWorldWall, &mtxRot);
	        
	        //位置を反映
	        D3DXMatrixTranslation(&mtxTrans, g_aWall[nCntWall].pos.x, g_aWall[nCntWall].pos.y, g_aWall[nCntWall].pos.z);
	        D3DXMatrixMultiply(&g_mtxWorldWall, &g_mtxWorldWall, &mtxTrans);
	        
	        //ワールドマトリックスの設定
	        pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldWall);
	        
	        
	        //頂点バッファをデータストリームに設定
	        pDevice->SetStreamSource(0, g_pVtxBuffWall, 0, sizeof(VERTEX_3D));//頂点バッファへのポインタと頂点情報の構造体のサイズ
	        
	        //頂点フォーマットの設定
	        pDevice->SetFVF(FVF_VERTEX_3D);

	
			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureWall);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntWall * 4, 2);//0は、描画する最初の頂点インデックスで、2は、描画するプリミティブ
		}
	}
	
}

void SetWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeiht)
{
	
	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		if (g_aWall[nCntWall].bUse == false)
		{
			g_aWall[nCntWall].pos = pos;
			g_aWall[nCntWall].rot = rot;
			g_aWall[nCntWall].fWidth = fWidth;
			g_aWall[nCntWall].fHeight = fHeiht;
			g_aWall[nCntWall].bUse = true;
			break;
		}
	}
}

Wall* GetWall(void)
{
	return &g_aWall[0];
}


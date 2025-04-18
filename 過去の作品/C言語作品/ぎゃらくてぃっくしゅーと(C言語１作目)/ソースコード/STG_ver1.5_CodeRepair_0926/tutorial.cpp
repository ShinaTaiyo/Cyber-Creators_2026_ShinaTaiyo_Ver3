//========================================================
//
//８月３０日：エフェクトもう一回見直す[tutorial.cpp]
//Author:ShinaTaiyo
//
//=========================================================
#include "main.h"
#include "bullet.h"
#include "explosion.h"
#include <stdlib.h>
#include <time.h>
#include "score.h"
#include "game.h"
#include "result.h"
#include "player.h"
#include "tutorial.h"
#include "sound.h"
#include "fade.h"
#include "staging.h"
//マクロ定義
#include "input.h"
#define NUM_TUTORIALTEX (2)//チュートリアル画面の種類

int g_nCountTutorialTime;//チュートリアルが始まってからのカウントをスタートする。

//グローバル変数
LPDIRECT3DTEXTURE9 g_apTextureTutorial[NUM_TUTORIALTEX] = {};
//D3DXVECTOR3 g_rotPI;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTutorial = NULL;
//Enemy g_aEnemy[MAX_ENEMY];//敵の情報
//Result g_aResult;

Tutorial g_Tutorial;
int g_nTutorialNumber;

//int g_nCountTime = 0;//時間を表すグローバル変数

//=====================================================
//==========================
//敵の初期化処理
//==========================
void InitTutorial(void)
{
	g_nCountTutorialTime = 0;

	g_nTutorialNumber = 0;
	//srand((unsigned int)time(NULL));//ゲームを開始した時間毎に乱数の種を設定

	/*SetStaging(STAGINGTYPE_PRESSENTER, -1, D3DXVECTOR3(SCREEN_WIDTH - 150.0f, SCREEN_HEIGHT - 150.0f, 0.0f));*/

	LPDIRECT3DDEVICE9 pDevice;
	int	nCntTutorial;

	g_Tutorial.bTutorialUse = true;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み（敵のテクスチャを読み込む）
	D3DXCreateTextureFromFile(pDevice,
		"data\\re_rule_003.png",
		&g_apTextureTutorial[TUTORIALTYPE_RULE]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\tutorial_operation_005.png",
		&g_apTextureTutorial[TUTORIALTYPE_OPERATION]);


	g_nTutorialNumber = TUTORIALTYPE_RULE;//見る説明の初期化

	//g_aEnemy->pos = D3DXVECTOR3(0.0f,0.0f, 0.0f);//位置を初期化する

	//チュートリアル情報の初期化
	//for (nCntTutorial = 0; nCntTutorial < NUM_TUTORIALTEX; nCntTutorial++)
	//{
	//	
	//}


	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,// * NUM_TUTORIALTEX,//４は必要な頂点数。今回使う画像数は2枚だが、一つのポリゴンでやるので、頂点４つで大丈夫。
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTutorial,
		NULL);

	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffTutorial->Lock(0, 0, (void**)&pVtx, 0);

	//for (nCntTutorial = 0; nCntTutorial < NUM_TUTORIALTEX; nCntTutorial++)
	//{
		//頂点座標の設定(敵の大きさ）
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f,SCREEN_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH,SCREEN_HEIGHT, 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLORは、0.0f〜1.0fの間の数値で色を変える。
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLORは、0.0f〜1.0fの間の数値で色を変える。
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLORは、0.0f〜1.0fの間の数値で色を変える。
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLORは、0.0f〜1.0fの間の数値で色を変える。

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		//pVtx += 4;//頂点データのポインタを４つ分進める。
	//}
	//頂点バッファをアンロックする
	g_pVtxBuffTutorial->Unlock();
}

//====================================
//チュートリアルの終了処理
//====================================
void UninitTutorial(void)
{
	//テクスチャの破棄
	int nCntTutorial;
	for (nCntTutorial = 0; nCntTutorial < NUM_TUTORIALTEX; nCntTutorial++)
	{
		if (g_apTextureTutorial[nCntTutorial] != NULL)
		{
			g_apTextureTutorial[nCntTutorial]->Release();
			g_apTextureTutorial[nCntTutorial] = NULL;
		}
	}
	//頂点バッファの破棄
	if (g_pVtxBuffTutorial != NULL)
	{
		g_pVtxBuffTutorial->Release();
		g_pVtxBuffTutorial = NULL;
	}
}


//========================================
//敵の更新処理
//========================================
void UpdateTutorial(void)
{
	VERTEX_2D* pVtx;

	int nCntTutorial;

	g_nCountTutorialTime++;

	if (GetKeyboardTrigger(DIK_RIGHT) == true||GetJoyPadTrigger(JOYKEY_B)==true)
	{
		PlaySound(SOUND_LABEL_SE_SELECT_000);
		g_nTutorialNumber = TUTORIALTYPE_OPERATION;//操作方法を見る
	}
	if (GetKeyboardTrigger(DIK_LEFT) == true || GetJoyPadTrigger(JOYKEY_Y) == true)
	{
		PlaySound(SOUND_LABEL_SE_SELECT_000);
		g_nTutorialNumber = TUTORIALTYPE_RULE;//ルール説明を見る
	}

	if (GetKeyboardTrigger(DIK_RETURN) == true||GetJoyPadTrigger(JOYKEY_START)==true)
	{
		g_Tutorial.bTutorialUse = false;//チュートリアルが終わるので、falseにする。
		PlaySound(SOUND_LABEL_SE_DECISION_000);
		
		SetFade(MODE_CHOOSE_DIFFICULTY);//難易度選択モードをフェードアウト後に呼び出す
	}
}


//=========================================
//敵の描画処理
//=========================================
void DrawTutorial(void)
{

	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	int nCntTutorial;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTutorial, 0, sizeof(VERTEX_2D));//頂点バッファへのポインタと頂点情報の構造体のサイズ

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定

	//for (nCntTutorial = 0; nCntTutorial < MAX_ENEMY; nCntTutorial++)//ポリゴンの描画
	//{
		//if (g_Tutorial[nCntTutorial].bUse == true)
		//{//敵が使用されている

			if (g_nTutorialNumber == TUTORIALTYPE_RULE)
			{
				pDevice->SetTexture(0, g_apTextureTutorial[TUTORIALTYPE_RULE]);
			}
			if (g_nTutorialNumber == TUTORIALTYPE_OPERATION)
			{
				pDevice->SetTexture(0, g_apTextureTutorial[TUTORIALTYPE_OPERATION]);
			}
			

			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,//0は、描画する最初の頂点インデックスで、2は、描画するプリミティブ数
				0,
				2);
		
	//}


}

Tutorial* GetTutorial(void)
{
	return &g_Tutorial;//チュートリアル構造体の情報をゲットする。
}


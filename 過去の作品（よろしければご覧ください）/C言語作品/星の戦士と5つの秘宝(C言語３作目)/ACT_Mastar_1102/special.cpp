//========================================================
//
//１０月２４日：ボス２の必殺技を作る[Special.cpp]
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
#include "special.h"
#include "sound.h"
#include "enemy.h"
#include "fade.h"
//マクロ定義
#include "input.h"
#define NUM_SpecialTEX (SPECIALTYPE_MAX)//演出の種類
#define MAX_Special (5000)

#define SCOREBONUS_SIZE (300.0f)
#define APPER_SpecialSIZE (50.0f)
#define PRESSSTART_SpecialSIZE (120.0f)

#define SHIELD_SIZE (100.0f)


//グローバル変数
LPDIRECT3DTEXTURE9 g_apTextureSpecial[NUM_SpecialTEX] = {};
//D3DXVECTOR3 g_rotPI;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSpecial = NULL;
//Enemy g_Special[MAX_ENEMY];//敵の情報
//Result g_aResult;

Special g_Special[MAX_Special];

//int g_nCountTime = 0;//時間を表すグローバル変数

//=====================================================
//==========================
//敵の初期化処理
//==========================
void InitSpecial(void)
{



	//srand((unsigned int)time(NULL));//ゲームを開始した時間毎に乱数の種を設定



	LPDIRECT3DDEVICE9 pDevice;
	int	nCntSpecial;

	//デバイスの取得
	pDevice = GetDevice();



	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\SpecialAttack_Boss1.png",
		&g_apTextureSpecial[SPECIALTYPE_BOSS1_SPECIAL_ATTACK]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\SpecialAttack_Boss2.png",
		&g_apTextureSpecial[SPECIALTYPE_BOSS2_SPECIAL_ATTACK]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\SpecialAttack_Boss3.png",
		&g_apTextureSpecial[SPECIALTYPE_BOSS3_SPECIAL_ATTACK]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\SpecialAttack_Boss4.png",
		&g_apTextureSpecial[SPECIALTYPE_BOSS4_SPECIAL_ATTACK]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\SpecialAttack_FinalBoss.png",
		&g_apTextureSpecial[SPECIALTYPE_FINALBOSS_SPECIAL_ATTACK]);








	//敵情報の初期化
	for (nCntSpecial = 0; nCntSpecial < MAX_Special; nCntSpecial++)
	{
		g_Special[nCntSpecial].nSpecialType = 0;
		g_Special[nCntSpecial].nLife = 0;
		g_Special[nCntSpecial].bUse = false;
		g_Special[nCntSpecial].bCountStartFlag = false;//演出が始まってからのカウントを開始するフラグを宣言する。
		g_Special[nCntSpecial].nSpecialCount = 0;//演出が始まってからのカウントを初期化する。
		g_Special[nCntSpecial].fSpecialPatternCountY = 0.0;//アニメーションを遅らせるカウント
		g_Special[nCntSpecial].fSpecialPattern = 0.0f;//アニメーションの種類
		g_Special[nCntSpecial].nAnimationRepeat = 0;//アニメーションを繰り返す変数
		g_Special[nCntSpecial].bDraw = true;//bUseがtrueだったことに加え、更にbDrawがtrueかどうかで描画するかどうかを決める。
		g_Special[nCntSpecial].nCntEnemy = 0;
		g_Special[nCntSpecial].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Special[nCntSpecial].fColor_a = 0.0f;
		g_Special[nCntSpecial].bColorChange_a = false;
		g_Special[nCntSpecial].nAtackSummon = 0;//演出をした場所に攻撃を召喚する
		g_Special[nCntSpecial].rot = 0.0f;//回転
		g_Special[nCntSpecial].fScale = 0.0f;//大きさ
		g_Special[nCntSpecial].fAeffective_Scale = 0.0f;//召喚した攻撃の効果範囲
		g_Special[nCntSpecial].fA_Width = 0.0f;//召喚した攻撃の幅
		g_Special[nCntSpecial].fA_Height = 0.0f;//召喚した攻撃の高さ
		g_Special[nCntSpecial].nA_Life = 0.0f;//召喚した攻撃のライフ
		g_Special[nCntSpecial].fA_rotSpeed = 0.0f;//召喚する攻撃の回転速度
		g_Special[nCntSpecial].nCntTime = 0;//時間を計測する
	}

	//g_Special->pos = D3DXVECTOR3(0.0f,0.0f, 0.0f);//位置を初期化する

	//チュートリアル情報の初期化
	//for (nCntSpecial = 0; nCntSpecial < NUM_SpecialTEX; nCntSpecial++)
	//{
	//	
	//}


	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_Special,// * NUM_SpecialTEX,//４は必要な頂点数。今回使う画像数は2枚だが、一つのポリゴンでやるので、頂点４つで大丈夫。
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffSpecial,
		NULL);

	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffSpecial->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntSpecial = 0; nCntSpecial < MAX_Special; nCntSpecial++)
	{
		//頂点座標の設定(敵の大きさ）
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;//頂点データのポインタを４つ分進める。
	}
	//頂点バッファをアンロックする
	g_pVtxBuffSpecial->Unlock();
}

//====================================
//チュートリアルの終了処理
//====================================
void UninitSpecial(void)
{
	//テクスチャの破棄
	int nCntSpecial;
	for (nCntSpecial = 0; nCntSpecial < NUM_SpecialTEX; nCntSpecial++)
	{
		if (g_apTextureSpecial[nCntSpecial] != NULL)
		{
			g_apTextureSpecial[nCntSpecial]->Release();
			g_apTextureSpecial[nCntSpecial] = NULL;
		}
	}
	//頂点バッファの破棄
	if (g_pVtxBuffSpecial != NULL)
	{
		g_pVtxBuffSpecial->Release();
		g_pVtxBuffSpecial = NULL;
	}
}


//========================================
//敵の更新処理
//========================================
void UpdateSpecial(void)
{
	VERTEX_2D* pVtx;



	int nCntSpecial;


	Enemy* pEnemy = GetEnemy();


	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffSpecial->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntSpecial = 0; nCntSpecial < MAX_Special; nCntSpecial++)
	{
		if (g_Special[nCntSpecial].bUse == true)
		{
			g_Special[nCntSpecial].nLife--;



			g_Special[nCntSpecial].nSpecialCount++;


			if (g_Special[nCntSpecial].nLife <= 0)
			{
				g_Special[nCntSpecial].bUse = false;
			}

			
			if (g_Special[nCntSpecial].nSpecialType == SPECIALTYPE_BOSS1_SPECIAL_ATTACK ||
				g_Special[nCntSpecial].nSpecialType == SPECIALTYPE_BOSS2_SPECIAL_ATTACK ||
				g_Special[nCntSpecial].nSpecialType == SPECIALTYPE_BOSS3_SPECIAL_ATTACK ||
				g_Special[nCntSpecial].nSpecialType == SPECIALTYPE_BOSS4_SPECIAL_ATTACK ||
				g_Special[nCntSpecial].nSpecialType == SPECIALTYPE_FINALBOSS_SPECIAL_ATTACK)
			{
				if (g_Special[nCntSpecial].nSpecialCount == 1)
				{//ポーズ状態にして、演出処理だけ行う
					SetPause(PAUSETYPE_STAGING);
				}
				//頂点座標の設定(敵の大きさ）
				pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

				//rhwの設定
				pVtx[0].rhw = 1.0f;
				pVtx[1].rhw = 1.0f;
				pVtx[2].rhw = 1.0f;
				pVtx[3].rhw = 1.0f;

				//頂点カラーの設定

				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。

				if (g_Special[nCntSpecial].nLife == 0)
				{
					g_Special[nCntSpecial].bUse = false;
					SetPause(PAUSETYPE_UNLOCK);//ポーズ状態解除
				}
			}
		


		}
		pVtx += 4;//頂点データのポインタを４つ分進める。
	}
	//頂点バッファをアンロックする

	g_pVtxBuffSpecial->Unlock();


}


//=========================================
//敵の描画処理
//=========================================
void DrawSpecial(void)
{

	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	Enemy* pEnemy = GetEnemy();

	int nCntSpecial;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffSpecial, 0, sizeof(VERTEX_2D));//頂点バッファへのポインタと頂点情報の構造体のサイズ

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定

	for (nCntSpecial = 0; nCntSpecial < MAX_Special; nCntSpecial++)
	{
		if (g_Special[nCntSpecial].bUse == true)
		{
			if (g_Special[nCntSpecial].bDraw == true)
			{
				
				if (g_Special[nCntSpecial].nSpecialType == SPECIALTYPE_BOSS1_SPECIAL_ATTACK)
				{
					pDevice->SetTexture(0, g_apTextureSpecial[SPECIALTYPE_BOSS1_SPECIAL_ATTACK]);
				}
				if (g_Special[nCntSpecial].nSpecialType == SPECIALTYPE_BOSS2_SPECIAL_ATTACK)
				{
					pDevice->SetTexture(0, g_apTextureSpecial[SPECIALTYPE_BOSS2_SPECIAL_ATTACK]);
				}
				if (g_Special[nCntSpecial].nSpecialType == SPECIALTYPE_BOSS3_SPECIAL_ATTACK)
				{
					pDevice->SetTexture(0, g_apTextureSpecial[SPECIALTYPE_BOSS3_SPECIAL_ATTACK]);
				}
				if (g_Special[nCntSpecial].nSpecialType == SPECIALTYPE_BOSS4_SPECIAL_ATTACK)
				{
					pDevice->SetTexture(0, g_apTextureSpecial[SPECIALTYPE_BOSS4_SPECIAL_ATTACK]);
				}
				if (g_Special[nCntSpecial].nSpecialType == SPECIALTYPE_FINALBOSS_SPECIAL_ATTACK)
				{
					pDevice->SetTexture(0, g_apTextureSpecial[SPECIALTYPE_FINALBOSS_SPECIAL_ATTACK]);
				}
				//ポリゴンの描画
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntSpecial * 4, 2);//0は、描画する最初の頂点インデックスで、2は、描画するプリミティブ数

				//&g_pVtxBuffPolygon[0],//頂点情報の先頭アドレス
				//sizeof(VERTEX_2D));//頂点情報構造体のサイズ
			}
		}
	}


}
void SetSpecial(SpecialTYPE type, D3DXVECTOR3 pos, int nLife)
{
	int nCntSpecial;

	VERTEX_2D* pVtx;

	/*Enemy* pEnemy = GetEnemy();*/

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffSpecial->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntSpecial = 0; nCntSpecial < MAX_Special; nCntSpecial++)
	{
		if (g_Special[nCntSpecial].bUse == false)
		{

			g_Special[nCntSpecial].pos = pos;//SetEnemyの時に、敵の位置が代入された際、位置の引数をさらにSetSpecialのposに代入することにより、
			g_Special[nCntSpecial].nLife = nLife;
			g_Special[nCntSpecial].nSpecialType = type;//演出の種類をセットする。
			g_Special[nCntSpecial].bUse = true;
			g_Special[nCntSpecial].nSpecialCount = 0;
			g_Special[nCntSpecial].bCountStartFlag = true;
			g_Special[nCntSpecial].fSpecialPatternCountY = 0.0f;
			g_Special[nCntSpecial].fSpecialPattern = 0.0f;
			g_Special[nCntSpecial].nAnimationRepeat = 0;
			g_Special[nCntSpecial].bDraw = true;
			g_Special[nCntSpecial].fColor_a = 1.0f;
			g_Special[nCntSpecial].bColorChange_a = false;
			g_Special[nCntSpecial].nCntTime = 0;//時間を計測する

			break;
		}
		pVtx += 4;
	}
	g_pVtxBuffSpecial->Unlock();
}

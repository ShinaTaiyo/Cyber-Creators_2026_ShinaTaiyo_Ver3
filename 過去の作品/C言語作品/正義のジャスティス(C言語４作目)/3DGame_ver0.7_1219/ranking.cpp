//========================================================
//
//８月３０日：エフェクトもう一回見直す[Ranking.cpp]
//Author:ShinaTaiyo
//
//=========================================================
#include "ranking.h"
#include "main.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "ranking.h"
#include <string.h>
#include <stdio.h>
#include "score.h"

#define MAX_RANK (5)//ランクインする順位の数
#define MAX_SCOREDIGIT (8)//ランクインした順位のスコアの最大桁数


#define RANKSCORE_SIZE_X (50.0f)//X方向のスコア表記のサイズ
#define RANKSCORE_SIZE_Y (50.0f)//Y方向のスコア表記のサイズ

#define RANKING_TEXT "data\\TEXTFILE\\Ranking.txt" //読み込むテキストファイル
//#define MAX_SCORERANK (40)
typedef struct
{
	D3DXVECTOR3 pos;//スコアのそれぞれの桁を表示させる位置
	int nScore;//それぞれの順位のスコア

	int nScorePosTexU;//スコアのテクスチャの位置

	int nBlinkingPoint;//スコアを点滅させるための条件に関わるポイント

	bool bBlinkingFlag;//ランクインしたスコアを点滅させるフラグ
}RankScore;
//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureRank =NULL;//テクスチャへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureRankBG = NULL;//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRank = NULL;//頂点バッファへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRankBG= NULL;//頂点バッファへのポインタ

RankScore g_aRankScore[MAX_RANK][MAX_SCOREDIGIT];//ランキングスコア情報。MAX_RANKは、１位～５位、MAX_SCOREDIGITは、桁数。
int g_nRankUpdate = -1;//更新ランクNo
int g_nTimerRanking;//ランキング画面表示タイマー

//int g_nScorePosTexU[MAX_RANK][MAX_SCOREDIGIT];//スコアの数字の座標

int g_nBlinkingCount;

//リザルト画面の初期化処理
void InitRanking(void)
{
	int nCntRank;

	int nCntDigit;

	g_nBlinkingCount = 0;

	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\number_001.png",
		&g_pTextureRank);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Ranking_bg.png",
		&g_pTextureRankBG);
	
	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4*MAX_RANK*MAX_SCOREDIGIT,//頂点の数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRank,
		NULL);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,//頂点の数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRankBG,
		NULL);

	VERTEX_2D* pVtx;
	//============================================================
	//ランキング設定処理
	//============================================================
	for (nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{//構造体の情報の初期化
		for (nCntDigit = 0; nCntDigit < MAX_SCOREDIGIT; nCntDigit++)
		{
			g_aRankScore[nCntRank][nCntDigit].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aRankScore[nCntRank][nCntDigit].pos = D3DXVECTOR3(SCREEN_WIDTH / 2+300.0f-80.0f*nCntDigit, 50.0f + 100.0f * nCntRank, 0.0f);
			g_aRankScore[nCntRank][nCntDigit].nBlinkingPoint = 0;
			g_aRankScore[nCntRank][nCntDigit].bBlinkingFlag = false;//スコアがランクインしたら、そのスコアを点滅させるフラグ
			g_aRankScore[nCntRank][nCntDigit].nScorePosTexU = 0;
				;
		}
	}

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffRank->Lock(0, 0, (void**)&pVtx, 0);


	for (nCntRank = 0; nCntRank < MAX_RANK * MAX_SCOREDIGIT; nCntRank++)
	{

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
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		pVtx += 4;//頂点座標のポインタをインクリメント
	}
	//頂点バッファをアンロックする
	g_pVtxBuffRank->Unlock();
	SetRanking(GetScore());
	//====================================================================================

	//=============================================================
	// ランキング背景の処理
	//=============================================================
	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffRankBG->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f,SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH,SCREEN_HEIGHT, 0.0f);

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
	g_pVtxBuffRankBG->Unlock();
	//==========================================================================

	//SetStaging(STAGINGTYPE_RANKING_BG, -1, D3DXVECTOR3(SCREEN_WIDTH/2,SCREEN_HEIGHT/2, 0.0f));
	
}



//==============================
//リザルト画面の終了処理
//==============================
void UninitRanking(void)
{
	int nCnt;
	//==============================================
	//ランキング設定処理
	// =============================================
	

	//テクスチャの破棄
		if (g_pTextureRank!= NULL)
		{
			g_pTextureRank->Release();
			g_pTextureRank = NULL;
		}
	//頂点バッファの破棄
	if (g_pVtxBuffRank != NULL)
	{
		g_pVtxBuffRank->Release();
		g_pVtxBuffRank = NULL;
	}
	//=============================================================

	//===================================
	//ランキング背景処理
	//===================================

	//テクスチャの破棄
	if (g_pTextureRankBG != NULL)
	{
		g_pTextureRankBG->Release();
		g_pTextureRankBG = NULL;
	}
	//頂点バッファの破棄
	if (g_pVtxBuffRankBG != NULL)
	{
		g_pVtxBuffRankBG->Release();
		g_pVtxBuffRankBG = NULL;
	}
	//============================================================
}

//===================================
//リザルト画面の更新処理
//===================================
void UpdateRanking(void)
{
	int nCntRank;

	int nCntDigit;

	g_nBlinkingCount++;

	VERTEX_2D* pVtx;


	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffRank->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{
		for (nCntDigit = 0; nCntDigit < MAX_SCOREDIGIT; nCntDigit++)
		{
			pVtx[0].pos = D3DXVECTOR3(g_aRankScore[nCntRank][nCntDigit].pos.x - RANKSCORE_SIZE_X, g_aRankScore[nCntRank][nCntDigit].pos.y - RANKSCORE_SIZE_Y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aRankScore[nCntRank][nCntDigit].pos.x + RANKSCORE_SIZE_X, g_aRankScore[nCntRank][nCntDigit].pos.y - RANKSCORE_SIZE_Y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aRankScore[nCntRank][nCntDigit].pos.x - RANKSCORE_SIZE_X, g_aRankScore[nCntRank][nCntDigit].pos.y + RANKSCORE_SIZE_Y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aRankScore[nCntRank][nCntDigit].pos.x + RANKSCORE_SIZE_X, g_aRankScore[nCntRank][nCntDigit].pos.y + RANKSCORE_SIZE_Y, 0.0f);



			if (g_aRankScore[nCntRank][nCntDigit].bBlinkingFlag == true)
			{
				if (g_nBlinkingCount == 1)
				{//スコアランクイン時に一度だけ、拍手音を鳴らす
					//PlaySound(SOUND_LABEL_SE_APPLAUSE_000);
				}

				if (g_nBlinkingCount % 2 == 0)
				{
					//0頂点カラーの設定（初期設定）
					pVtx[0].col = D3DCOLOR_RGBA(255,0,0, 255);
					pVtx[1].col = D3DCOLOR_RGBA(255,0,0, 255);
					pVtx[2].col = D3DCOLOR_RGBA(255,0,0, 255);
					pVtx[3].col = D3DCOLOR_RGBA(255,0,0, 255);
				}
				else
				{

					//頂点カラーの設定（初期設定）
					pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				}
			}

			pVtx += 4;//頂点座標のポインタをインクリメント
		}
	}
	g_pVtxBuffRank->Unlock();

	if (GetKeyboardTrigger(DIK_RETURN) == true||GetJoyPadTrigger(JOYKEY_START)==true)
	{
		PlaySound(SOUND_LABEL_SE_DECISION_000);
		StopSound(SOUND_LABEL_BGM002);
		SetFade(MODE_TITLE);
	}

}






//================================
//タイトル画面の描画処理
//================================
void DrawRanking(void)
{
		LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

    
	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffRankBG, 0, sizeof(VERTEX_2D));//頂点バッファへのポインタと頂点情報の構造体のサイズ

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

		pDevice->SetTexture(0, g_pTextureRankBG);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,0, 2);//0は、描画する最初の頂点インデックスで、2は、描画するプリミティブ数
			//&g_pVtxBuffPolygon[0],//頂点情報の先頭アドレス
			//sizeof(VERTEX_2D));//頂点情報構造体のサイズ
	//===================================
	//ランキング設定処理
	//===================================


	int nCntRank;
	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffRank, 0, sizeof(VERTEX_2D));//頂点バッファへのポインタと頂点情報の構造体のサイズ

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);


	for (nCntRank = 0; nCntRank < MAX_RANK * MAX_SCOREDIGIT; nCntRank++)
	{
		
			pDevice->SetTexture(0, g_pTextureRank);
	
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntRank * 4, 2);//0は、描画する最初の頂点インデックスで、2は、描画するプリミティブ数
			//&g_pVtxBuffPolygon[0],//頂点情報の先頭アドレス
			//sizeof(VERTEX_2D));//頂点情報構造体のサイズ
	}
	////===============================================================================================================================================
	//

}

//====================================
//ランキングのリセット
//====================================
void ResetRanking(void)
{
	int nCntRank;

	int nCntDigit;

	for (nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{//全てのスコアを初期化する。
		for (nCntDigit = 0; nCntDigit < MAX_SCOREDIGIT; nCntDigit++)
		{
			g_aRankScore[nCntRank][nCntDigit].nScore = 0;

		}
	}
}

//========================================
//ランキングのセット
//========================================
void SetRanking(int nScore)
{
	VERTEX_2D* pVtx;

	//GameCustom* pGameCustom = GetGameCustom();

	/*for (nCount = 0; nCount < MAX_RANK; nCount++)
	{
		nData[nCount] = 0;
	}*/

	FILE* pFile;//ファイルポインタを宣言

	int nCount;

	int nData[MAX_RANK];//ランキングのスコア代入する変数
	int nCopyData;//数値を入れ替えるときの元の値の受け皿用変数
	int nCountData;
	int nAdditionData;//追加する値

	

	int nResultScoreData[MAX_RANK];//最終的にランクインしたスコアの情報を代入する変数
	//===============================================================
	//情報処理から持ってきた処理
	//===============================================================
	pFile = fopen(RANKING_TEXT, "r");//エラー防止
	
	if (pFile != NULL)
	{
		for (nCount = 0; nCount < MAX_RANK; nCount++)
		{
			fscanf(pFile, "%d", &nData[nCount]);
			

			nResultScoreData[nCount] = 0;//しょきか
		}
		fclose(pFile);//ファイルを閉じる
	}


	

		nAdditionData = nScore;//今回のスコア

		if (nAdditionData > nData[MAX_RANK - 1])
		{//今回のスコアがランクインしていたスコアの最小値より大きい場合
			nData[MAX_RANK - 1] = nAdditionData;
		}


			for (nCountData = 0; nCountData < MAX_RANK - 1; nCountData++)//比べる数値（要素１）※MAX_RANK-1は、最後の値の一個前の値と最後の値を比べる(４回比べる）
			{
				for (nCount = nCountData + 1; nCount < MAX_RANK; nCount++)//要素２（比べた値の次の値から）※nCountData+1は、要素１の次の値からという意味
				{
					if (nData[nCountData] < nData[nCount])//要素１が要素２より小さいとき
					{
						nCopyData = nData[nCountData];//受け皿作り[nCopyData]
						nData[nCountData] = nData[nCount];//数値の入れ替え（一つ目の数値の順位が、一つ下の順位の、二つ目の順位に変わる。）
						nData[nCount] = nCopyData;//数値の入れ替え（入れ替わった二つ目の順位に、入れ替える前の一つ目の順位の数値を受け皿から代入。）
					}
				}
			}
		
	

			
			pFile = fopen(RANKING_TEXT, "w");

	if (pFile != NULL)
	{//ファイルが開けた場合
		//ファイルに文字列を書き出す
		for (nCount = 0; nCount < MAX_RANK; nCount++)
		{
			fprintf(pFile, "%d\n", nData[nCount]);//並べ終わったので、先に順位を保存する
		}

		//ファイルを閉じる
		fclose(pFile);
	}
	
	//==========================================================================================================================

	for (nCount = 0; nCount < MAX_RANK; nCount++)
	{//	グローバル変数にスコアを保存
		nResultScoreData[nCount] = nData[nCount];//最終的なスコアの代入（ランクインしたスコアがあった場合も含めて）。（表示する順位分）
	}




	//==============================================================================================================
	//ランキングのスコアの数字の位置や、ランクインしたスコアを光らせる処理
	//==============================================================================================================

	int nCnt;
	int	nCntScore;
	int nCntDigit;//桁数
	int nCntRank;//順位（行数）
	/*int nGetScore = nScore;*/

	for (nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)//ランクインしたスコア５個分の読み込み。
	{
		for (nCntDigit = 0; nCntDigit < MAX_SCOREDIGIT; nCntDigit++)
		{
			g_aRankScore[nCntRank][nCntDigit].nScorePosTexU = nData[nCntRank] % 10;//各順位のそれぞれのスコアをそれぞれの順位の８桁分に代入する。(ランクインする順位の数分score.cppでやった処理をする。）
			nData[nCntRank] = nData[nCntRank] / 10;
		}
	}

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffRank->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{
		for (nCntDigit = 0; nCntDigit < MAX_SCOREDIGIT; nCntDigit++)
		{
			pVtx[0].tex = D3DXVECTOR2(0.0f + (0.1f * g_aRankScore[nCntRank][nCntDigit].nScorePosTexU), 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f + (0.1f * g_aRankScore[nCntRank][nCntDigit].nScorePosTexU), 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f + (0.1f * g_aRankScore[nCntRank][nCntDigit].nScorePosTexU), 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f + (0.1f * g_aRankScore[nCntRank][nCntDigit].nScorePosTexU), 1.0f);

			//===========================================================================================================================================================================================
			//光らせる処理
			if (nResultScoreData[nCntRank]==nScore)	
			{//ランクインしたスコアの中に、同じものがなかったスコアの時
				g_aRankScore[nCntRank][nCntDigit].bBlinkingFlag = true;
			}
			//=======================================================================================================================================================================================================
			pVtx += 4;//頂点データのポインタを４つ分進める。
		}
	}
	g_pVtxBuffRank->Unlock();
}

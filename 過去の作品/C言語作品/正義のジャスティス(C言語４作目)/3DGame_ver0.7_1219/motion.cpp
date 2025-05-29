//========================================================
//
//１１月７日：モデル読み込み[Motion.cpp]
//Author:ShinaTaiyo
//
//=========================================================

#include "motion.h"
#include "main.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "shadow.h"
#include "player.h"
#include <stdio.h>
#include <string.h>

//グローバル変数
int g_nNumMotion = 0;//読み込むモデルの最大数を決める

int g_nUseMotion = 0;//何番のモーションを使っているか

int g_nCntFrame = 0;//フレームの長さを測る
#define Motion_SCALE (10.0f)
//#define SAVEMotion "data\\TEXTFILE\\SaveMotion.txt"

#define MAX_TEXTURE (32)


typedef struct
{
	LPD3DXMESH pMeshMotion;//メッシュ（頂点情報）へのポインタ（モデルの種類数分）
	LPD3DXBUFFER pBuffMatMotion;//マテリアルへのポインタ
	DWORD dwNumMatMotion;//マテリアルの数
	LPDIRECT3DTEXTURE9 apTextureMotion[MAX_TEXTURE];//テクスチャへのポインタ
}MotionMat;

D3DXVECTOR3 g_posMotion;//位置
D3DXVECTOR3 g_rotMotion;//向き
D3DXMATRIX g_mtxWorldMotion;//ワールドマトリックス

//Motion g_aMotion[MAX_MOTION];//モデル情報の構造体

MotionMat g_aMotionMat[NUM_MOTION];//モデルのマテリアルや頂点の構造体

int g_nCntKeySet;//キーセットをカウントする

//int g_nldxShadow = -1;//対象の影のインデックス（番号）
//bool g_bIsLanding;//地面にいるかどうか

int g_nNowKey;//現在のキー
int g_nNowMotion;//現在のモーション

//====================
//モーション関連処理
//====================
MotionSet g_aMotionSet[MAX_MOTION];//モーション設定処理

//タイトル画面の初期化処理
void InitMotion(void)
{
	int g_nUseMotion = 0;//何番のモーションを使っているか

	g_nCntKeySet = 0;//キーセットをカウントする

	g_nCntFrame = 0;//フレームの長さを測る

	g_nNowKey = 0;

	g_nNowMotion = 0;
	//=======================
	//初期化処理
	//=======================
	for (int nCntMotion = 0; nCntMotion < MAX_MOTION; nCntMotion++)
	{
		g_aMotionSet[nCntMotion].bLoop = false;//ループさせるかどうかのフラグ
		g_aMotionSet[nCntMotion].nNumKey = 0;//キーの数
		g_aMotionSet[nCntMotion].bUse = false;//モーションを使用するかどうか
		for (int nCntKeySet = 0; nCntKeySet < MAX_KEYSET; nCntKeySet++)
		{
			g_aMotionSet[nCntMotion].aKeySet[nCntKeySet].nFrame = 0;//次のポーズまでの時間を決める

			for (int nCntKey = 0; nCntKey < MAX_KEY; nCntKey++)
			{
				g_aMotionSet[nCntMotion].aKeySet[nCntKeySet].aKey[nCntKey].TargetPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				g_aMotionSet[nCntMotion].aKeySet[nCntKeySet].aKey[nCntKey].Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0);//位置
				g_aMotionSet[nCntMotion].aKeySet[nCntKeySet].aKey[nCntKey].Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0);//向き

			}
		}
	}
	//===========================================================================================================
	char aString[100];//文字列を読み込むための変数
	int nMaxRoadMotion = 0;//読み込むモデルの最大数
	int nType = 0;//読み込んだモデルの種類

	float fPosX = 0.0f;//モデルの位置X
	float fPosY = 0.0f;//モデルの位置Y
	float fPosZ = 0.0f;//モデルの位置Z

	float fRotX = 0.0f;//モデルの位置X
	float fRotY = 0.0f;//モデルの位置Y
	float fRotZ = 0.0f;//モデルの位置Z
	int nCnt = 0;//読み込むモデルを数える用
	int nIdx = 0;//インデックス
	int nCntMotionSet = 0;//セットするモーション数のカウント
	int nCntKeySet = 0;//キーセット数のカウント
	int nCntKey = 0;//キー数のカウント
	int nRoadModel = 0;//読み込むモデルの最大数
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスへのポインタと取得


	FILE* pFile;//ファイルポインタ
	pFile = fopen(MOTIONSET, "r");
	char aStringPass[100] = {};
	char cData;

	Player* pPlayer = GetPlayer();
	//char aStringPass[100] = {};
	if (pFile != NULL)//ファイルが開いているかどうか
	{
		while (1)
		{

			fscanf(pFile, "%s", &aString[0]);

			if (strcmp(&aString[0], "END_SCRIPT") == 0)
			{
				fclose(pFile);
				break;//行の最後でこの文字列を判定させることにより、ループを終わらせる
			}
			else if (aString[0] == '#')
			{
				fgets(&aString[0], 100, pFile);
				continue;
			}
			else if (strcmp(&aString[0], "NUM_MODEL") == 0)//読み込むモデルの種類数を決める
			{
		       fscanf(pFile, "%s", &aString[0]);

				fscanf(pFile, "%d", &g_nNumMotion);
				//do{

				//	fscanf(pFile, "%s", &aString[0]);
				//	if (aString[0] == '#')
				//	{
				//		fgets(&aString[0], 100, pFile);
				//		continue;
				//	}
				//	else if (strcmp(&aString[0], "MODEL_FILENAME") == 0)
				//	{
				//		fscanf(pFile, "%s", &aString[0]);
				//		fscanf(pFile, "%s", &aStringPass[0]);
				//		//モデルの読み込み
				//		D3DXLoadMeshFromX(&aStringPass[0],
				//			D3DXMESH_SYSTEMMEM,
				//			pDevice,
				//			NULL,
				//			&pPlayer->aModel[nRoadModel].pBuffMat,//モデルでは、これも配列にする
				//			NULL,
				//			&pPlayer->aModel[nRoadModel].dwNumMatPlayer,//モデルでは、これも配列にする
				//			&pPlayer->aModel[nRoadModel].pMesh);
				//		nRoadModel++;
				//	}
				//} while (nRoadModel < g_nNumMotion);
			}
			else if (strcmp(&aString[0], "MOTIONSET") == 0)
			{
				while (1)
				{
					int nLoop = 0;
					fscanf(pFile, "%s", &aString[0]);
					if (aString[0] == '#')
					{
						fgets(&aString[0], 100, pFile);
						continue;
					}
					else if (strcmp(&aString[0], "LOOP") == 0)
					{
						fscanf(pFile, "%s", &aString[0]);// =
						fscanf(pFile, "%d", &nLoop);//ループさせるかどうかを数値で宣言する

						if (nLoop == 0)
						{
							g_aMotionSet[nCntMotionSet].bLoop = false;//ループしない
						}
						else if (nLoop == 1)
						{
							g_aMotionSet[nCntMotionSet].bLoop = true;//ループする
						}
					}
					else if (strcmp(&aString[0], "NUM_KEY") == 0)//キー数
					{
						int nData = ~0;
						fscanf(pFile, "%s %d", &aString[0], &nData);// =
						g_aMotionSet[nCntMotionSet].nNumKey = nData;
					}
					else if (strcmp(&aString[0], "KEYSET") == 0)
					{
						while (1)
						{
							fscanf(pFile, "%s", &aString[0]);// =
							if (aString[0] == '#')
							{
								fgets(&aString[0], 100, pFile);
								continue;
							}
							if (strcmp(&aString[0], "FRAME") == 0)
							{
								fscanf(pFile, "%s %d", &aString[0], &g_aMotionSet[nCntMotionSet].aKeySet[nCntKeySet].nFrame);//=,フレームの値
							}
							else if (strcmp(&aString[0], "KEY") == 0)
							{
								while (1)
								{
									fscanf(pFile, "%s", &aString[0]);

									if (aString[0] == '#')
									{
										fgets(&aString[0], 100, pFile);//その行を全文字読み取り、完全に終わらせる
										continue;
									}
									else if (strcmp(&aString[0], "POS") == 0)
									{
										fscanf(pFile, "%s%f%f%f", &aString[0],
											&g_aMotionSet[nCntMotionSet].aKeySet[nCntKeySet].aKey[nCntKey].Pos.x,
											&g_aMotionSet[nCntMotionSet].aKeySet[nCntKeySet].aKey[nCntKey].Pos.y,
											&g_aMotionSet[nCntMotionSet].aKeySet[nCntKeySet].aKey[nCntKey].Pos.z);
									}
									else if (strcmp(&aString[0], "ROT") == 0)
									{
										fscanf(pFile, "%s%f%f%f", &aString[0],
											&g_aMotionSet[nCntMotionSet].aKeySet[nCntKeySet].aKey[nCntKey].Rot.x,
											&g_aMotionSet[nCntMotionSet].aKeySet[nCntKeySet].aKey[nCntKey].Rot.y,
											&g_aMotionSet[nCntMotionSet].aKeySet[nCntKeySet].aKey[nCntKey].Rot.z);
										//次のモーション（nCnt+1)までの角度/Frameの値を求める

										//if (nCntKeySet == g_aMotionSet[nCntMotionSet].nNumKey - 1)
										//{//最後のキーセットなので、最初のキーセットに向けて回転させる。
										//	g_aMotionSet[nCntMotionSet].aKeySet[nCntKeySet].aKey[nCntKey].TargetRot = (g_aMotionSet[nCntMotionSet].aKeySet[0].aKey[nCntKey].Rot - g_aMotionSet[nCntMotionSet].aKeySet[nCntKeySet].aKey[nCntKey].Rot) / g_aMotionSet[nCntMotionSet].aKeySet[nCntKeySet].nFrame;
										//	g_aMotionSet[nCntMotionSet].aKeySet[nCntKeySet].aKey[nCntKey].SaveRot = g_aMotionSet[nCntMotionSet].aKeySet[nCntKeySet].aKey[nCntKey].Rot;
										//}
										//else
										//{
										//	g_aMotionSet[nCntMotionSet].aKeySet[nCntKeySet].aKey[nCntKey].TargetRot = (g_aMotionSet[nCntMotionSet].aKeySet[nCntKeySet + 1].aKey[nCntKey].Rot - g_aMotionSet[nCntMotionSet].aKeySet[nCntKeySet].aKey[nCntKey].Rot) / g_aMotionSet[nCntMotionSet].aKeySet[nCntKeySet].nFrame;
										//	g_aMotionSet[nCntMotionSet].aKeySet[nCntKeySet].aKey[nCntKey].SaveRot = g_aMotionSet[nCntMotionSet].aKeySet[nCntKeySet].aKey[nCntKey].Rot;
										//}
									}
									else if (strcmp(&aString[0], "END_KEY") == 0)
									{
										nCntKey++;
										break;
									}

								}
							}
							else if (strcmp(&aString[0], "END_KEYSET") == 0)
							{
								nCntKey = 0;
								nCntKeySet++;
								break;
							}
							/*nCntKeySet++;*/
						}

					}
					else if (strcmp(&aString[0], "END_MOTIONSET") == 0)
					{
						nCntKeySet = 0;
						nCntMotionSet++;
						break;
					}
				}
			}
		}
	}

	//for (int nCnt = 0; nCnt < nCntMotionSet; nCnt++)//nCntMotionSet
	//{
	//	for (int nCnt2 = 0; nCnt2 < g_aMotionSet[nCnt].nNumKey; nCnt2++)//nCntKeySet
	//	{
	//		for (int nCnt3 = 0; nCnt3 < g_nNumMotion; nCnt3++)//nNumKey
	//		{
	//			if (nCnt2 == g_aMotionSet[nCnt].nNumKey - 1)
	//			{//最後のキーセットなので、最初のキーセットに向けて回転させる。
	//				g_aMotionSet[nCnt].aKeySet[nCnt2].aKey[nCnt3].TargetRot = (g_aMotionSet[nCnt].aKeySet[0].aKey[nCnt3].Rot - g_aMotionSet[nCnt].aKeySet[nCnt2].aKey[nCnt3].Rot) / g_aMotionSet[nCnt].aKeySet[nCnt2].nFrame;
	//				//g_aMotionSet[nCnt].aKeySet[nCnt2].aKey[nCnt3].SaveRot = g_aMotionSet[nCnt].aKeySet[nCnt2].aKey[nCnt3].Rot;
	//			}
	//			else
	//			{
	//				g_aMotionSet[nCnt].aKeySet[nCnt2].aKey[nCnt3].TargetRot = (g_aMotionSet[nCnt].aKeySet[nCnt2 + 1].aKey[nCnt3].Rot - g_aMotionSet[nCnt].aKeySet[nCnt2].aKey[nCnt3].Rot) / g_aMotionSet[nCnt].aKeySet[nCnt2].nFrame;
	//				//g_aMotionSet[nCnt].aKeySet[nCnt2].aKey[nCnt3].SaveRot = g_aMotionSet[nCnt].aKeySet[nCnt2].aKey[nCnt3].Rot;
	//			}
	//		}
	//	}
	//}
}

//============================
//モデルの更新処理
//============================
void UpdateMotion(void)
{
	Player* pPlayer = GetPlayer();


	bool bUse = true;
	int nCnt;
	/*while (bUse == false)
	{
		
		int nData = 1;

	}*/

	//==============================
	//使用するモーションを決める
	//==============================
	for (int nCntMotion = 0; nCntMotion < MAX_MOTION; nCntMotion++)
	{
		g_aMotionSet[nCntMotion].bUse = false;
	}
	/*if (pPlayer->nState == PLAYERSTATE_NUTORAL)
	{
		
	}
	else if (pPlayer->nState == PLAYERSTATE_NUTORAL)
	{
		g_nUseMotion = PLAYERSTATE_MOVE;
	}*/

	g_nUseMotion = pPlayer->nState;

	/*g_nUseMotion = PLAYERSTATE_MOVE;*/

//	g_nUseMotion = PLAYERSTATE_NUTORAL;
	//========================================================================
	

		//if (g_aMotionSet[pPlayer->nState].bUse == true)
		//{//使用しているかどうか

			//for (int nCntKeySet = 0; nCntKeySet < g_aMotionSet[g_nUseMotion].nNumKey; nCntKeySet++)
			//{//モーションごとに設定したnNumKeyの数だけ行う。
	/*int nCntFrame = 0;*/
	           /* int nCntKeySet = 0;*/
				//int nCntPerfectKey = 0;//キーフレームアニメーションが完全に完了した時にカウントする変数
				//bool bMotionFlag = false;//モーションのキーを次に進めるフラグ
			
				/*if (g_aMotionSet[g_nUseMotion].bUse == true)
				{*/


					/*do
					{*/
	g_nCntFrame++;
	if (g_nCntFrame >= g_aMotionSet[g_nUseMotion].aKeySet[g_nCntKeySet].nFrame)
	{
		g_nCntFrame = 0;
		g_nCntKeySet++;
		if (g_nCntKeySet >= g_aMotionSet[g_nUseMotion].nNumKey)
		{
			g_nCntKeySet = 0;
			if (g_aMotionSet[g_nUseMotion].bLoop == false)
			{
				pPlayer->nState = PLAYERSTATE_NUTORAL;
				pPlayer->bMotionFlag = false;
			}
		}
		for (int nCntKey = 0; nCntKey < g_nNumMotion; nCntKey++)
		{
		//　ターゲット　＝　(現在呼び出しているモーションのキー　- (現在のプレイヤーのパーツの向き　- プレイヤーのパーツの初期設定）) / フレーム数 
			g_aMotionSet[g_nUseMotion].aKeySet[g_nCntKeySet].aKey[nCntKey].TargetRot = (g_aMotionSet[g_nUseMotion].aKeySet[g_nCntKeySet].aKey[nCntKey].Rot - (pPlayer->aModel[nCntKey].rot - pPlayer->aModel[nCntKey].SaveRot)) /
				g_aMotionSet[g_nUseMotion].aKeySet[g_nCntKeySet].nFrame;
			//g_aMotionSet[nCnt].aKeySet[nCnt2].aKey[nCnt3].SaveRot = g_aMotionSet[nCnt].aKeySet[nCnt2].aKey[nCnt3].Rot;
	    
		}
	}

						for (int nCntKey = 0; nCntKey < g_nNumMotion; nCntKey++)
						{
						pPlayer->aModel[nCntKey].rot += g_aMotionSet[g_nUseMotion].aKeySet[g_nCntKeySet].aKey[nCntKey].TargetRot;
						}

						
				//	} while (g_nCntFrame < g_aMotionSet[g_nUseMotion].aKeySet[g_nCntKeySet].nFrame);

					/*g_nCntKeySet++;
					if (g_nCntKeySet == g_aMotionSet[g_nUseMotion].nNumKey)
					{
						g_nCntKeySet = 0;
					}*/
				//}

				//do
				//{//
					//for (int nCntKey = 0; nCntKey < g_nNumMotion; nCntKey++)
					//{//モデルの数分のキーを設定する(全てのキーを一気に動かすので、for文で回すと考える
			

					//	///*g_nNumMotion */
					//	//if (g_aMotionSet[nCntMotion].aKeySet[nCntKeySet].aKey[nCntKey].Rot < g_aMotionSet[nCntMotion].aKeySet[nCntKeySet + 1].aKey[nCntKey].TargetRot)
					//	//{
					//

					//	if (g_nCntKeySet == g_aMotionSet[g_nUseMotion].nNumKey - 1)
					//	{//最後のキーセットなので、最初のキーセットのモデルの位置に向けて動きを行う。

					//		pPlayer->aModel[nCntKey].rot += g_aMotionSet[g_nUseMotion].aKeySet[g_nCntKeySet].aKey[nCntKey].TargetRot;

					//		//パーツが次のキーの位置に来た時。
					//		if (pPlayer->aModel[nCntKey].rot >= g_aMotionSet[g_nUseMotion].aKeySet[0].aKey[nCntKey].SaveRot - D3DXVECTOR3(1.0f, 1.0f, 1.0f)
					//			&& pPlayer->aModel[nCntKey].rot <= g_aMotionSet[g_nUseMotion].aKeySet[0].aKey[nCntKey].SaveRot + D3DXVECTOR3(1.0f, 1.0f, 1.0f))
					//		{//キーフレームアニメーションが終わったので、動かしたパーツの位置を初期値に戻す。
					//			/*g_aMotionSet[nCntMotion].aKeySet[nCntKeySet].aKey[nCntKey].Rot = g_aMotionSet[nCntMotion].aKeySet[nCntKeySet].aKey[nCntKey].SaveRot;*/

					//			for (int nCntKey2 = 0; nCntKey2 < g_nNumMotion; nCntKey2++)
					//			{//一つが目的の座標に達したら、全ての今のモーションの向きを元に戻す
					//				g_aMotionSet[g_nUseMotion].aKeySet[g_nCntKeySet].aKey[nCntKey2].Rot = g_aMotionSet[g_nUseMotion].aKeySet[g_nCntKeySet].aKey[nCntKey2].SaveRot;
					//			}
					//		/*	bMotionFlag = true;*/
					//			g_nCntKeySet = 0;//最後のキーセットなので、最初のキーセットへ戻る。
					//			break;
					//		}
					//	}
					//	else
					//	{

				 //           pPlayer->aModel[nCntKey].rot += g_aMotionSet[g_nUseMotion].aKeySet[g_nCntKeySet].aKey[nCntKey].TargetRot;

					//		//パーツが次のキーの位置に来た時。
					//		if (pPlayer->aModel[nCntKey].rot >= g_aMotionSet[g_nUseMotion].aKeySet[g_nCntKeySet + 1].aKey[nCntKey].SaveRot - D3DXVECTOR3(1.0f,1.0f,1.0f)
					//			&& pPlayer->aModel[nCntKey].rot <= g_aMotionSet[g_nUseMotion].aKeySet[g_nCntKeySet + 1].aKey[nCntKey].SaveRot + D3DXVECTOR3(1.0f, 1.0f, 1.0f))
					//		{//キーフレームアニメーションが終わったので、動かしたパーツの位置を初期値に戻す。
					//			
					//			for (int nCntKey2 = 0; nCntKey2 < g_nNumMotion; nCntKey2++)
					//			{//一つが目的の座標に達したら、全ての今のモーションの向きを元に戻す
					//				g_aMotionSet[g_nUseMotion].aKeySet[g_nCntKeySet].aKey[nCntKey2].Rot = g_aMotionSet[g_nUseMotion].aKeySet[g_nCntKeySet].aKey[nCntKey2].SaveRot;
					//			}
					//		/*	bMotionFlag = true;*/
					//			g_nCntKeySet++;//次のキーセットへ進める
					//			break;
					//		/*	break;*/
					//		}
					//	}

					//		
					//}
				//} while (bMotionFlag == true);
		//	}

	//	}
	//}
}

void SetMotion(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, char aPath)
{
	for (int nCntMotion = 0; nCntMotion < MAX_MOTION; nCntMotion++)
	{
		if (g_aMotionSet[nCntMotion].bUse == false)
		{
			g_aMotionSet[nCntMotion].bUse = true;//使用しているかどうか
		
			

			break;
		}
	}
}

//int GetNumMotion(void)
//{
//	return g_nNumMotion;//読み込むモデルの最大数を取得する
//

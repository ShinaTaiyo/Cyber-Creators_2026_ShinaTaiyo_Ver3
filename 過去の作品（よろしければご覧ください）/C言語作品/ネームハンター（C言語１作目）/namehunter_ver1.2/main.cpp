//====================================
//
//自作ゲームその２：「ネームハンター」ver:1.1（完成版！）
//Author:ShinaTaiyo
//
//====================================
#include<stdio.h>
#include<string.h>//文字列を使うときに使用する。
#include<stdlib.h>//ランダム関数を使うときに使用する。
#include<time.h>//タイム関数を使うときに使用する。
#include<Windows.h>
#include<mmsyscom.h>
#pragma comment(lib,"Winmm.lib")


//マクロ定義
#define MAX_PLAYER (2)
#define MAX_NAME (256)
#define PlayerNumber1 (0)
#define PlayerNumber2 (1)
#define MAX_TURN (200)


//列挙型定義
typedef enum
{
	MODE_NOTHIING = 0,//何もしない(0)
	MODE_NORMALATTACK,//通常攻撃(1)
	MODE_SKILLATTACK,//スキル攻撃(2)
	MODE_USEITEM,//アイテムを使う(3)
	MODE_MAX

}MODE;


//構造体定義
typedef struct
{
	char aName[MAX_NAME];//プレイヤーの名前
	char aJob[MAX_NAME];//プレイヤーの役職
	int nCheckJob;//役職の番号を記述する。
	float fLife;//プレイヤーの体力
	float fATK;//プレイヤーの攻撃力
	float fDEF;//プレイヤーの防御力
	float fMP;//プレイヤーのマジックポイント
}Player;



//プロトタイプ宣言
void InputData(Player* pPlayer, float* pLength);//プレイヤー構造体のポインタとfloat型のポインタを宣言する。

void SetStatus(Player* pPlayer, float* pLength);

void SelectJob(Player* pPlayer, int* pSelectJob);//役職を選ぶモードを設定する。

void SetBattle(Player* pPlayer);

void Player1NormalAttack(Player* pPlayer, bool Player2CounterFlag, bool bPlayer1CriticalLineBuff);//プレイヤー１が通常攻撃をするときに呼び出す関数。カウンターのフラグがtrueになっている場合、プレイヤー２が確率でカウンターを発動させる。

void Player2NormalAttack(Player* pPlayer, bool Player1CounterFlag, bool bPlayer2CriticalLineBuff);//プレイヤー２が通常攻撃をするときに呼び出す関数。カウンターのフラグがtrueになっている場合、プレイヤー１が確率でカウンターを発動させる。

void Player1InpactEdge(Player* pPlayer);//プレイヤー１が剣士のスキル（１）、「インパクトエッジ」を発動するときに呼び出す関数。

void Player2InpactEdge(Player* pPlayer);//プレイヤー２が剣士のスキル（１）、「インパクトエッジ」を発動するときに呼び出す関数。

void Player1RecoverHeal(Player* pPlayer, float fPlayer1MaxHp);//プレイヤー１が剣士のスキル（２）、「リカバーヒール」を発動するときに呼び出す関数。

void Player2RecoverHeal(Player* pPlayer, float fPlayer2MaxHp);//プレイヤー２が剣士のスキル（２）、「リカバーヒール」を発動する時に呼び出す関数。

void Player1RainArrow(Player* pPlayer);//プレイヤー１が狩人のスキル（１）、「レインアロー」を発動するときに呼び出す関数。

void Player2RainArrow(Player* pPlayer);//プレイヤー２が狩人のスキル（１）、「レインアロー」を発動するときに呼び出す関数。

bool Player1PowerDownArrow(Player* pPlayer);//プレイヤー１が狩人のスキル（２）、「パワダウンアロー」を発動するときに呼び出す関数。

bool Player2PowerDownArrow(Player* pPlayer);//プレイヤー2が狩人のスキル（２）、「パワダウンアロー」を発動するときに呼び出す関数。

void Player1StarArrow(Player* pPlayer);//プレイヤー１が狩人のスキル（３）、「スターアロー」を発動するときに呼び出す関数。

void Player2StarArrow(Player* pPlayer);//プレイヤー２が狩人のスキル（３）、「スターアロー」を発動するときに呼び出す関数。

void Player1DefenceLine(Player* pPlayer);//プレイヤー１が巫女のスキル（１）、「守りの陣」を発動するときに呼び出す関数。

void Player2DefenceLine(Player* pPlayer);//プレイヤー２が巫女のスキル（１）、「守りの陣」を発動するときに呼び出す関数。

void Player1HolyRay(Player* pPlayer);//プレイヤー１が巫女のスキル（２）、「ホーリーレイ」を発動するときに呼び出す関数。

void Player2HolyRay(Player* pPlayer);//プレイヤー２が巫女のスキル（２）、「ホーリーレイ」を発動するときに呼び出す関数。

//void Player1CriticalLine(Player* pPlayer);//プレイヤー１が巫女のスキル（３）、「会心の陣」を発動するときに呼び出す関数。

void Player1BloodyBite(Player* pPlayer);//プレイヤー１が吸血鬼のスキル（１）、「ブラッティバイト」を発動するときに呼び出す関数。

void Player2BloodyBite(Player* pPlayer);//プレイヤー２が吸血鬼のスキル（１）、「ブラッディバイトを発動するときに呼び出す関数。

void Player1CrazyGracity(Player* pPlayer);//プレイヤー１が吸血鬼のスキル（２）、「クレイジーグラビティ」を発動するときに呼び出す関数。

void Player2CrazyGravity(Player* pPlayer);//プレイヤー２が吸血鬼のスキル（２）、「クレイジーグラビティ」を発動するときに呼び出す関数。

bool Player1PhantomStab(Player* pPlayer);//プレイヤー１が吸血鬼のスキル（３）、「ファントムスタブ」を発動するときに呼び出す関数。

bool Player2PhantomStab(Player* pPlayer);//プレイヤー２が吸血鬼のスキル（３）、「ファントムスタブ」を発動するときに呼び出す関数。

bool Player1PoisonMist(Player* pPlayer);//プレイヤー１が科学者のスキル（１）、「ポイズンミスト」を発動するときに呼び出す関数。

bool Player2PoisonMist(Player* pPlayer);//プレイヤー２が科学者のスキル（１）、「ポイズンミスト」を発動するときに呼び出す関数。

bool Player1SteamBurst(Player* pPlayer);//プレイヤー１が科学者のスキル（２）、「スチームバースト」を発動するときに呼び出す関数。

bool Player2SteamBurst(Player* pPlayer);//プレイヤー２が科学者のスキル（２）、「スチームバースト」を発動するときに呼び出す関数。

bool Player1SquidLaser(Player* pPlayer);//プレイヤー１が科学者のスキル（３）、「イカサマレーザー」を発動するときに呼び出す関数。

bool Player2SquidLaser(Player* pPlayer);//プレイヤー１が科学者のスキル（３）、「イカサマレーザー」を発動するときに呼び出す関数。

bool Player1FlameWave(Player* pPlayer);//プレイヤー１が魔法使いのスキル（１）、「フレイムウェーブ」を発動するときに呼び出す関数。

bool Player2FlameWave(Player* pPlayer);//プレイヤー２が魔法使いのスキル（１）、「フレイムウェーブ」を発動するときに呼び出す関数。

bool Player1IceLance(Player* pPlayer);//プレイヤー1が魔法使いのスキル（３）、「アイスランス」を発動するときに呼び出す関数。

bool Player2IceLance(Player* pPlayer);//プレイヤー２が魔法使いのスキル（３）、「アイスランス」を発動するときに呼び出す関数。

void Player1WaveCanon(Player* pPlayer);//プレイヤー１がアイテム「波動砲」を手に入れたときに呼び出す関数。

void Player2WaveCanon(Player* pPlayer);//プレイヤー２がアイテム「波動砲」を手に入れたときに呼び出す関数。



//グローバル変数宣言
Player g_aPlayer[MAX_PLAYER];



//=====================================
//メイン関数
//=====================================

int main(void)
{
	PlaySound(TEXT("bgm2.wav"), NULL, SND_ASYNC | SND_FILENAME);

	printf("０　　０　　０　　０　　　０　０００\n");
	printf("００　０　０　０　００　００　０\n");
	printf("０　００　０００　０　０　０　０００\n");
	printf("０　　０　０　０　０　０　０　０\n");
	printf("０　　０　０　０　０　０　０　０００\n\n");

	printf("０　０　０　０　０　　０　０００　０００　０００\n");
	printf("０　０　０　０　００　０　　０　　０　　　０　　０\n");
	printf("０００　０　０　０　００　　０　　０００　０００\n");
	printf("０　０　０　０　０　　０　　０　　０　　　０　０\n");
	printf("０　０　０００　０　　０　　０　　０００　０　　０\n\n\n\n\n\n\n\n\n");


	printf("             ～入力した名前で戦うRPG～\n\n");
	printf("                       PressStart！\n");
	rewind(stdin);
	getchar();
	system("cls");
	float fLength[MAX_PLAYER];//文字列の長さを数値として代入するための変数

	int nSelectJob[MAX_PLAYER];//役職をプレイヤーの人数分決定する変数を宣言する。

	//int nCount;

	int nChoose;

	while (1)
	{


		InputData(&g_aPlayer[0], &fLength[0]);//構造体とfloat型の引き数にg_aPlayerのアドレスとfLengthのアドレスを代入する。


		SetStatus(&g_aPlayer[0], &fLength[0]);//構造体とfloatの引数にg_aPlayerのアドレスとfLengthのアドレスをを代入する。

		SelectJob(&g_aPlayer[0], &nSelectJob[0]);//役職を決めて、最終的なステータスとスキルの種類を決める。
		PlaySound(NULL, 0, 0);
		SetBattle(&g_aPlayer[0]);//バトルの処理を行う関数。

		PlaySound(NULL, 0, 0);
		printf("もう一回ネームハンターをやりますか？\n１：もう一回やる、それ以外：ゲームを終了する\n");

		scanf("%d", &nChoose);
		if (nChoose == 1)
		{
			printf("了解しました！\n");
		}
		else
		{
			printf("わかりました！お疲れさまでした！またのプレイをお待ちしております!\n");
			break;
		}
	}
	printf("何かキーを押してください\n");

	rewind(stdin);//標準入力から入力した情報を元に戻す。
	getchar();
	return 0;






	//int nTurnAttack;//攻撃ターンの変数

	//bool bEndBattle;//先頭が終了したかどうかのフラグを設定する。（この型は、true（真)、又はfalse（偽)の値となる。)
}

void InputData(Player* pPlayer, float* pLength)//構造体とfloatのポインタを宣言する。
{
	//float f[MAX_PLAYER];



	int nCount;
	while (1)
	{


		for (nCount = 0; nCount < MAX_PLAYER; nCount++)
		{
			printf("%d人目のプレイヤーの名前を入力してください！\n：", nCount + 1);

			scanf("%s", &pPlayer[nCount].aName[0]);

			printf("\n\n");



			//pPlayer++;
		}



		for (nCount = 0; nCount < MAX_PLAYER; nCount++)
		{
			pLength[nCount] = (float)strlen(&pPlayer[nCount].aName[0]);//文字列の長さを数値として設定する。
			//nLength[1] = (int)strlen(&pPlayer[PlayerNumber2].aName[0]);//文字列の長さを数値として設定する。

			//pPlayer++;//ポインタをインクリメントする。
			//pLength++;
		}
		//pLength[0];

		//for (nCount = 0; nCount < MAX_PLAYER; nCount++,pLength++)
		for (nCount = 0; nCount < MAX_PLAYER; nCount++)
		{
			if (pLength[nCount] > MAX_NAME)//fLengthの値が最大文字数以上ならエラー表示をする。ポインタ先の値を表す。
			{
				printf("入力された名前が長すぎます！\n\n");
			}
			//pLength++;
		}

		if (pLength[0] <= MAX_NAME && pLength[1] <= MAX_NAME)//二人のプレイヤーの入力した名前が最大文字数以下だった場合、ループを終了する。
		{
			break;
		}

		//pPlayer = pPlayer - 2;//プレイヤーのポインタを２個戻し、見る場所をもとにもどす。
	}




	//pPlayer[0];//インクリメントし終わったから、先頭アドレスを代入する。

	//return fLength[MAX_PLAYER];
}
//===================================================
//プレイヤーのパラメータを設定する
//===================================================

void SetStatus(Player* pPlayer, float* pLength)//プレイヤーとfloatのポインタを宣言する。
{
	int CheckCount = 0;

	int nCount;

	float fData[MAX_NAME]; // , nLength[MAX_PLAYER];

	float fData1[MAX_NAME];

	//int LifeParam[MAX_PLAYER];//体力の数値を表す変数（二人分の配列を宣言する）

	//int AtkParam[MAX_PLAYER];//攻撃力を表す変数（二人分の配列を宣言する）

	//float fRandum1;//ランダムで値が決まる変数を宣言する。
	//float fRandum2;//     
	//int ChecknRandum;//ランダムで処理を変えるための変数を宣言する。

	float fBonus[6][10];//条件を満たしたときに発動するボーナスステータスの配列を３種類で１０個分宣言する。


	//================================================
	//お互いのプレイヤーのライフボーナスの値を編集
	//================================================
	for (nCount = 0; nCount < 10; nCount++)
	{
		fBonus[0][nCount] = 1;//１０個分のボーナス変数の中身を一旦１にする。
	}

	for (nCount = 0; nCount < 10; nCount++)
	{
		fBonus[1][nCount] = 1;//１０個分のボーナス変数の中身を一旦１にする。
	}

	for (nCount = 0; nCount < 10; nCount++)
	{
		fBonus[2][nCount] = 1;//１０個分のボーナス変数の中身を一旦１にする。
	}

	for (nCount = 0; nCount < 10; nCount++)
	{
		fBonus[3][nCount] = 1;//１０個分のボーナス変数の中身を一旦１にする。
	}

	for (nCount = 0; nCount < 10; nCount++)
	{
		fBonus[4][nCount] = 1;//１０個分のボーナス変数の中身を一旦１にする。
	}

	for (nCount = 0; nCount < 10; nCount++)
	{
		fBonus[5][nCount] = 1;//１０個分のボーナス変数の中身を一旦１にする。
	}
	//for (nCount = 0; nCount < 10; nCount++)
	//{
	//	fBonus[6][nCount] = 1;//１０個分のボーナス変数の中身を一旦１にする。
	//}

	//for (nCount = 0; nCount < 10; nCount++)
	//{
	//	fBonus[7][nCount] = 1;//１０個分のボーナス変数の中身を一旦１にする。
	//}



	//乱数の種を設定する
	//srand((unsigned int)time(NULL));
	//fRandum1 = rand() % 25 + 1;//乱数値（２５％）を宣言

	//printf("%f\n", fRandum1);


	//fRandum1 = rand() % 25 + 1;//乱数値（２５％）を宣言
	//ChecknRandum = rand() % 2 + 1;//ランダムで処理を変えたいときに使いたい変数

	//float fRandumResult;//最終的ランダム関数の値を確定させる変数。


	//==================================================
	//計算に使う値の設定
	//==================================================


	//入力された文字列をプレイヤーの最大人数分それぞれ読み取る。

	//for (nCount = 0; nCount < MAX_PLAYER; nCount++)
	//{
	//	nLength[nCount] = (float)strlen(&g_aPlayer[nCount].aName[0]);//文字列の長さを数値として設定する。
	//	//nLength[1] = (int)strlen(&pPlayer[PlayerNumber2].aName[0]);//文字列の長さを数値として設定する。

	//	//pPlayer++;//ポインタをインクリメントする。

	//}

	//printf("nLength[0]の値：%f\n", pLength[0]);
	//printf("一人目のプレイヤーのの名前：%s", &g_aPlayer[0].aName[0]);

	//pPlayer;


	//プレイヤー１が読み取った文字列分nDataの配列の値を宣言する。
	for (nCount = 0; nCount < MAX_NAME; nCount++)
	{
		fData[nCount] = 80
			;//文字が少なかった時用に、入力してない文字列分もデータを作る
		if (nCount < pLength[PlayerNumber1])
		{
			fData[nCount] = (float)g_aPlayer[PlayerNumber1].aName[nCount];//指定している文字をアスキーコードの数値として読み込む。
		}
	}
	//プレイヤー２が読み取った文字列分nDataの配列の値を宣言する。
	for (nCount = 0; nCount < MAX_NAME; nCount++)
	{
		fData1[nCount] = 80;//文字が少なかった時用に、入力してない文字列分もデータを作る
		if (nCount < pLength[PlayerNumber2])
		{
			fData1[nCount] = (float)g_aPlayer[PlayerNumber2].aName[nCount];//指定している文字をアスキーコードの数値として読み込む。
		}

		//nData1[nCount] = (int)g_aPlayer[nCount].aName[nCount];//指定している文字をアスキーコードの数値として読み込む。
	}


	//================================
	//プレイヤー１のライフボーナス
	//================================

	if (fData[0] == 107.0 || fData[0] == 115.0)//アスキーコードの文字は"k,s"、一文字目が"k,s"なら、ステータスボーナスを付与する。
	{
		fBonus[0][0] = 1300;//ボーナスステータスの一つ目を宣言する。
	}

	if (fData[1] == 105.0 || fData[1] == 104.0)//アスキーコードの文字は"i,h"、２文字目が"i,h"なら、ステータスボーナスを付与する。
	{
		fBonus[0][1] = 1300;
	}

	if (fData[2] == 114.0 || fData[2] == 105.0)//アスキーコードの文字は"r,i"、3文字目が"r,i"なら、ステータスボーナスを付与する。
	{
		fBonus[0][2] = 1300;//ボーナスステータスの一つ目を宣言する。
	}

	if (fData[3] == 98.0 || fData[3] == 110.0)//アスキーコードの文字は"b,n"、4文字目が"b,n"なら、ステータスボーナスを付与する。
	{
		fBonus[0][3] = 1300;
	}

	if (fData[4] == 121.0 || fData[4] == 97.0)//アスキーコードの文字は"y,a"、5文字目が"y,a"なら、ステータスボーナスを付与する。
	{
		fBonus[0][4] = 1300;
	}

	//======================================
	//プレイヤー２のライフボーナス
	//======================================
	if (fData1[0] == 107.00 || fData1[0] == 115.0)//アスキーコードの文字は"k,s"、一文字目が"k,s"なら、ステータスボーナスを付与する。
	{
		fBonus[3][0] = 1300;//ボーナスステータスの一つ目を宣言する。
	}

	if (fData1[1] == 105.0 || fData1[1] == 104.0)//アスキーコードの文字は"i,h"、２文字目が"i,h"なら、ステータスボーナスを付与する。
	{
		fBonus[3][1] = 1300;
	}

	if (fData1[2] == 114.0 || fData1[2] == 105.0)//アスキーコードの文字は"r,i"、3文字目が"r,i"なら、ステータスボーナスを付与する。
	{
		fBonus[3][2] = 1300;//ボーナスステータスの一つ目を宣言する。
	}

	if (fData1[3] == 98.0 || fData1[3] == 110.0)//アスキーコードの文字は"b,n"、4文字目が"b.n"なら、ステータスボーナスを付与する。
	{
		fBonus[3][3] = 1300;
	}

	if (fData1[4] == 121.0 || fData1[4] == 97.0)//アスキーコードの文字は"y,a"、5文字目が"y,a"なら、ステータスボーナスを付与する。
	{
		fBonus[3][4] = 1300;
	}

	//===================================  1:taiyo 2:fpsgm
	//プレイヤー１のアタックボーナス
	//===================================
	if (fData[0] == 116.0 || fData[0] == 102.0)//アスキーコードの文字は"t,f"、一文字目が"t,f"なら、ステータスボーナスを付与する。
	{
		fBonus[1][0] = 140;//ボーナスステータスの一つ目を宣言する。
	}

	if (fData[1] == 97.0 || fData[1] == 112.0)//アスキーコードの文字は"a,p"、２文字目が"a,p"なら、ステータスボーナスを付与する。
	{
		fBonus[1][1] = 140;
	}

	if (fData[2] == 105.0 || fData[2] == 115.0)//アスキーコードの文字は"i,s"、3文字目が"i,s"なら、ステータスボーナスを付与する。
	{
		fBonus[1][2] = 140;//ボーナスステータスの一つ目を宣言する。
	}

	if (fData[3] == 121.0 || fData[3] == 103.0)//アスキーコードの文字は"y,g"、4文字目が"y,g"なら、ステータスボーナスを付与する。
	{
		fBonus[1][3] = 140;
	}

	if (fData[4] == 111.0 || fData[4] == 109.0)//アスキーコードの文字は"o,m"、5文字目が"o,m"なら、ステータスボーナスを付与する。
	{
		fBonus[1][4] = 140;
	}

	//=======================================
	//プレイヤー２のアタックボーナス
	//=======================================
	if (fData1[0] == 116.0 || fData1[0] == 102.0)//アスキーコードの文字は"t,f"、一文字目が"t,f"なら、ステータスボーナスを付与する。
	{
		fBonus[4][0] = 140;//ボーナスステータスの一つ目を宣言する。
	}

	if (fData1[1] == 97.0 || fData1[1] == 112.0)//アスキーコードの文字は"a,p"、２文字目が"a,p"なら、ステータスボーナスを付与する。
	{
		fBonus[4][1] = 140;
	}

	if (fData1[2] == 105.0 || fData1[2] == 115.0)//アスキーコードの文字は"i,s"、3文字目が"i,s"なら、ステータスボーナスを付与する。
	{
		fBonus[4][2] = 140;//ボーナスステータスの一つ目を宣言する。
	}

	if (fData1[3] == 121.0 || fData1[3] == 103.0)//アスキーコードの文字は"y,g"、4文字目が"y,g"なら、ステータスボーナスを付与する。
	{
		fBonus[4][3] = 140;
	}

	if (fData1[4] == 111.0 || fData1[4] == 109.0)//アスキーコードの文字は"o,m"、5文字目が"o,m"なら、ステータスボーナスを付与する。
	{
		fBonus[4][4] = 140;
	}

	//===============================================  1:haruk 2:aorin
	//プレイヤー１のディフェンスボーナス
	//===============================================
	if (fData[0] == 104.0 || fData[0] == 97.0)//アスキーコードの文字は"h,a"、一文字目が"h,a"なら、ステータスボーナスを付与する。
	{
		fBonus[2][0] = 100;//ボーナスステータスの一つ目を宣言する。
	}

	if (fData[1] == 97.0 || fData[1] == 111.0)//アスキーコードの文字は"a,o"、２文字目が"a,o"なら、ステータスボーナスを付与する。
	{
		fBonus[2][1] = 100;
	}

	if (fData[2] == 114.0)//アスキーコードの文字は"r"、3文字目が"r"なら、ステータスボーナスを付与する。
	{
		fBonus[2][2] = 100;//ボーナスステータスの一つ目を宣言する。
	}

	if (fData[3] == 117.0 || fData[3] == 105.0)//アスキーコードの文字は"u,i"、4文字目が"u,i"なら、ステータスボーナスを付与する。
	{
		fBonus[2][3] = 100;
	}

	if (fData[4] == 117.0 || fData[4] == 110.0)//アスキーコードの文字は"k,n"、5文字目が"k,n"なら、ステータスボーナスを付与する。
	{
		fBonus[2][4] = 100;
	}


	//===============================================  1:haruk 2:aorin
	//プレイヤー２のディフェンスボーナス
	//===============================================
	if (fData1[0] == 104.0 || fData1[0] == 97.0)//アスキーコードの文字は"h,a"、一文字目が"h,a"なら、ステータスボーナスを付与する。
	{
		fBonus[5][0] = 100;//ボーナスステータスの一つ目を宣言する。
	}

	if (fData1[1] == 97.0 || fData1[1] == 111.0)//アスキーコードの文字は"a,o"、２文字目が"a,o"なら、ステータスボーナスを付与する。
	{
		fBonus[5][1] = 100;
	}

	if (fData1[2] == 114.0)//アスキーコードの文字は"r"、3文字目が"r"なら、ステータスボーナスを付与する。
	{
		fBonus[5][2] = 100;//ボーナスステータスの一つ目を宣言する。
	}

	if (fData1[3] == 117.0 || fData1[3] == 105.0)//アスキーコードの文字は"u,i"、4文字目が"u,i"なら、ステータスボーナスを付与する。
	{
		fBonus[5][3] = 100;
	}

	if (fData1[4] == 117.0 || fData1[4] == 110.0)//アスキーコードの文字は"k,n"、5文字目が"k,n"なら、ステータスボーナスを付与する。 
	{
		fBonus[5][4] = 100;
	}

	//=========================================
	//プレイヤー1のステータスボーナス
	//=========================================
	//if (fData[5] == 122.0)//アスキーコードの文字は”ｚ”、６文字目がｚなら、ステータスボーナスを付与する。
	//{
	//	fBonus[6][5] = 120;
	//}
	//if (fData1[5] == 122.0)//アスキーコードの文字は”ｚ”、６文字目がｚなら、ステータスボーナスを付与する。
	//{
	//	fBonus[7][5] = 120;
	//}




	////プレイヤー２が読み取った文字列分nDataの配列の値を宣言する。
	//for (nCount = 0; nCount < MAX_NAME; nCount++)
	//{
	//	fData1[nCount] = 80;//文字が少なかった時用に、入力してない文字列分もデータを作る
	//	if (nCount < nLength[PlayerNumber2])
	//	{
	//		fData1[nCount] = (float)g_aPlayer[PlayerNumber2].aName[nCount];//指定している文字をアスキーコードの数値として読み込む。
	//	}

	//	//nData1[nCount] = (int)g_aPlayer[nCount].aName[nCount];//指定している文字をアスキーコードの数値として読み込む。
	//}


	//========================================
	//パラメータの計算式
	//========================================
	char aBonusName1[20] = "tanakaakira";//名前が田中章だったときにボーナスを付与する。
	char aBonusName2[20] = "uchiyamajunichi";//名前が内山純一だったときにボーナスを付与する。
	char aBonusName3[20] = "azumatakashi";//名前が東高志だったときにボーナスをふよする。
	char aBonusName4[20] = "kirby";//名前がカービィだったときにボーナスを付与する。
	char aBonusName5[20] = "tadokorokoji";
	char aBonusName6[20] = "akira";
	char aBonusName7[20] = "junichi";
	char aBonusName8[20] = "takashi";

	//pLength;


	//for (nCount = 0; nCount < MAX_PLAYER; nCount++)
	//{

	pPlayer->fLife = (fData[0] + fData[1] + fData[2] + fData[3] + fData[4] + fData[5] + fData[6] + pLength[0] * 10) * 24 + fBonus[0][0] + fBonus[0][1] + fBonus[0][2] + fBonus[0][3] + fBonus[0][4];//プレイヤーの体力の数値の計算式

	pPlayer->fATK = (fData[0] + fData[3] + fData[5] + fData[7] + fData[8]) * 2.1f - pLength[0] * 10 + fBonus[1][0] + fBonus[1][1] + fBonus[1][2] + fBonus[1][3] + fBonus[1][4];//プレイヤーの攻撃力の数値の計算式

	pPlayer->fDEF = (fData[1] + fData[2] + fData[4] + fData[6]) * 1.8f - pLength[0] * 10 + fBonus[2][0] + fBonus[2][1] + fBonus[2][2] + fBonus[2][3] + fBonus[2][4];//プレイヤーの攻撃力の数値の計算式

	if (strcmp(&pPlayer->aName[0], &aBonusName1[0]) == 0)
	{
		printf("プレイヤー１の隠しネームの入力を確認！ステータスにボーナスを付与します！\n");

		pPlayer->fLife = pPlayer->fLife + 6000;
		pPlayer->fATK = pPlayer->fATK + 300;
		pPlayer->fDEF = pPlayer->fDEF + 300;
	}
	if (strcmp(&pPlayer->aName[0], &aBonusName2[0]) == 0)
	{
		printf("プレイヤー１の隠しネームの入力を確認！ステータスにボーナスを付与します！\n");

		pPlayer->fLife = pPlayer->fLife + 3000;
		pPlayer->fATK = pPlayer->fATK + 600;
		pPlayer->fDEF = pPlayer->fDEF + 300;
	}
	if (strcmp(&pPlayer->aName[0], &aBonusName3[0]) == 0)
	{
		printf("プレイヤー１の隠しネームの入力を確認！ステータスにボーナスを付与します！\n");

		pPlayer->fLife = pPlayer->fLife + 3000;
		pPlayer->fATK = pPlayer->fATK + 300;
		pPlayer->fDEF = pPlayer->fDEF + 600;
	}
	if (strcmp(&pPlayer->aName[0], &aBonusName4[0]) == 0)
	{
		printf("プレイヤー１の隠しネームの入力を確認！ステータスにボーナスを付与します！\n");

		pPlayer->fLife = pPlayer->fLife + 2000;
		pPlayer->fATK = pPlayer->fATK + 550;
		pPlayer->fDEF = pPlayer->fDEF + 550;
	}
	if (strcmp(&pPlayer->aName[0], &aBonusName5[0]) == 0)
	{
		printf("プレイヤー１の隠しネームの入力を確認！ステータスにボーナスを付与します！\n");

		pPlayer->fLife = pPlayer->fLife + 810;
		pPlayer->fATK = pPlayer->fATK + 810;
		pPlayer->fDEF = pPlayer->fDEF + 810;
	}
	if (strcmp(&pPlayer->aName[0], &aBonusName6[0]) == 0)
	{
		printf("プレイヤー１の隠しネームの入力を確認！ステータスにボーナスを付与します！\n");

		pPlayer->fLife = pPlayer->fLife + 6000;
		pPlayer->fATK = pPlayer->fATK + 300;
		pPlayer->fDEF = pPlayer->fDEF + 300;
	}
	if (strcmp(&pPlayer->aName[0], &aBonusName7[0]) == 0)
	{
		printf("プレイヤー１の隠しネームの入力を確認！ステータスにボーナスを付与します！\n");

		pPlayer->fLife = pPlayer->fLife + 3000;
		pPlayer->fATK = pPlayer->fATK + 600;
		pPlayer->fDEF = pPlayer->fDEF + 300;
	}
	if (strcmp(&pPlayer->aName[0], &aBonusName8[0]) == 0)
	{
		printf("プレイヤー１の隠しネームの入力を確認！ステータスにボーナスを付与します！\n");

		pPlayer->fLife = pPlayer->fLife + 3000;
		pPlayer->fATK = pPlayer->fATK + 300;
		pPlayer->fDEF = pPlayer->fDEF + 600;
	}
	//pLength++;

	pPlayer++;//ポインタをインクリメントする。
 //for (nCount = 0; nCount < MAX_PLAYER; nCount++)
 //{
	pPlayer->fLife = (fData1[0] + fData1[1] + fData1[2] + fData1[3] + fData1[4] + fData1[5] + fData1[6] + pLength[1] * 10) * 24 + fBonus[3][0] + fBonus[3][1] + fBonus[3][2] + fBonus[3][3] + fBonus[3][4];//プレイヤーの体力の数値の計算式

	pPlayer->fATK = (fData1[0] + fData1[3] + fData1[5] + fData1[7] + fData1[8]) * 2.1f - pLength[1] * 10 + fBonus[4][0] + fBonus[4][1] + fBonus[4][2] + fBonus[4][3] + fBonus[4][4];//プレイヤーの体力の数値の計算式

	pPlayer->fDEF = (fData1[1] + fData1[2] + fData1[4] + fData1[6]) * 1.8f - pLength[1] * 10 + fBonus[5][0] + fBonus[5][1] + fBonus[5][2] + fBonus[5][3] + fBonus[5][4];//プレイヤーの防御力の数値の計算式

	if (strcmp(&pPlayer->aName[0], &aBonusName1[0]) == 0)
	{
		printf("プレイヤー２の隠しネームの入力を確認！ステータスにボーナスを付与します！\n");

		pPlayer->fLife = pPlayer->fLife + 6000;
		pPlayer->fATK = pPlayer->fATK + 250;
		pPlayer->fDEF = pPlayer->fDEF + 250;
	}
	if (strcmp(&pPlayer->aName[0], &aBonusName2[0]) == 0)
	{
		printf("プレイヤー２の隠しネームの入力を確認！ステータスにボーナスを付与します！\n");

		pPlayer->fLife = pPlayer->fLife + 3000;
		pPlayer->fATK = pPlayer->fATK + 500;
		pPlayer->fDEF = pPlayer->fDEF + 250;
	}
	if (strcmp(&pPlayer->aName[0], &aBonusName3[0]) == 0)
	{
		printf("プレイヤー２の隠しネームの入力を確認！ステータスにボーナスを付与します！\n");

		pPlayer->fLife = pPlayer->fLife + 3000;
		pPlayer->fATK = pPlayer->fATK + 250;
		pPlayer->fDEF = pPlayer->fDEF + 500;
	}
	if (strcmp(&pPlayer->aName[0], &aBonusName4[0]) == 0)
	{
		printf("プレイヤー２の隠しネームの入力を確認！ステータスにボーナスを付与します！\n");

		pPlayer->fLife = pPlayer->fLife + 2000;
		pPlayer->fATK = pPlayer->fATK + 550;
		pPlayer->fDEF = pPlayer->fDEF + 550;
	}
	if (strcmp(&pPlayer->aName[0], &aBonusName5[0]) == 0)
	{
		printf("プレイヤー２の隠しネームの入力を確認！ステータスにボーナスを付与します！\n");

		pPlayer->fLife = pPlayer->fLife + 810;
		pPlayer->fATK = pPlayer->fATK + 810;
		pPlayer->fDEF = pPlayer->fDEF + 810;
	}
	if (strcmp(&pPlayer->aName[0], &aBonusName6[0]) == 0)
	{
		printf("プレイヤー２の隠しネームの入力を確認！ステータスにボーナスを付与します！\n");

		pPlayer->fLife = pPlayer->fLife + 6000;
		pPlayer->fATK = pPlayer->fATK + 300;
		pPlayer->fDEF = pPlayer->fDEF + 300;
	}
	if (strcmp(&pPlayer->aName[0], &aBonusName7[0]) == 0)
	{
		printf("プレイヤー２の隠しネームの入力を確認！ステータスにボーナスを付与します！\n");

		pPlayer->fLife = pPlayer->fLife + 3000;
		pPlayer->fATK = pPlayer->fATK + 600;
		pPlayer->fDEF = pPlayer->fDEF + 300;
	}
	if (strcmp(&pPlayer->aName[0], &aBonusName8[0]) == 0)
	{
		printf("プレイヤー２の隠しネームの入力を確認！ステータスにボーナスを付与します！\n");

		pPlayer->fLife = pPlayer->fLife + 3000;
		pPlayer->fATK = pPlayer->fATK + 300;
		pPlayer->fDEF = pPlayer->fDEF + 600;
	}
	pPlayer--;//ポインタ先をプレイヤー１へ
//プレイヤーの防御力
	//プレイヤーの攻撃力の数値の計算式
//}

	//	printf("LifeParam[0]の値：%d\n", LifeParam[0]);

//体力、攻撃力を一旦定義しておく。

//aPlayer[0].nLife = 100;
//aPlayer[0].nATK = 15;

//aPlayer[1].nLife = 130;
//aPlayer[1].nATK = 12;



	if (pPlayer->fLife < 0)
	{
		pPlayer->fLife = pPlayer->fLife * -1;//ステータスが０以下だった場合、ステータスの値にー１を掛ける。
	}
	if (pPlayer->fATK < 0)
	{
		pPlayer->fATK = pPlayer->fATK * -1;//ステータスが０以下だった場合、ステータスの値にー１を掛ける。
	}
	if (pPlayer->fDEF < 0)
	{
		pPlayer->fDEF = pPlayer->fDEF * -1;//ステータスが０以下だった場合、ステータスの値にー１を掛ける。
	}

	pPlayer++;//ポインタ先をプレイヤー２へ
	if (pPlayer->fLife < 0)
	{
		pPlayer->fLife = pPlayer->fLife * -1;//ステータスが０以下だった場合、ステータスの値にー１を掛ける。
	}
	if (pPlayer->fATK < 0)
	{
		pPlayer->fATK = pPlayer->fATK * -1;//ステータスが０以下だった場合、ステータスの値にー１を掛ける。
	}
	if (pPlayer->fDEF < 0)
	{
		pPlayer->fDEF = pPlayer->fDEF * -1;//ステータスが０以下だった場合、ステータスの値にー１を掛ける。
	}
	pPlayer--;//ポインタ先をプレイヤー１へ


	//pPlayer[0];//ポインタを先頭アドレスへ


	//先頭アドレスを代入（配列[０]へ）

//パラメータを構造体にあてはめる。(作るから、ポインタを使う。）

	/*for (nCount = 0; nCount < MAX_PLAYER; nCount++, pPlayer++)
	{
		pPlayer->nLife = LifeParam[nCount];

		pPlayer->nATK = AtkParam[nCount];
	}*/

	//pPlayer;//先頭アドレスを代入（配列[0]へ）
	//aPlayer[PlayerNumber2].nLife = LifeParam[PlayerNumber2];

	//aPlayer[PlayerNumber2].nATK = AtkParam[PlayerNumber2];

	/*for (nCount = 0; nCount < MAX_PLAYER; nCount++)
	{*/
	printf("              <<プレイヤー１>>　　      vs　　     <<プレイヤー２>>\n");

	printf("  名前：%16s                     %16s\n", &g_aPlayer[0].aName[0], &g_aPlayer[1].aName[0]);

	printf("  \x1b[32m体力：%16.f                     %16.f\n", g_aPlayer[0].fLife, g_aPlayer[1].fLife);

	printf("\x1b[31m攻撃力：%16.f                     %16.f\n", g_aPlayer[0].fATK, g_aPlayer[1].fATK);

	printf("\x1b[36m防御力：%16.f                     %16.f\n", g_aPlayer[0].fDEF, g_aPlayer[1].fDEF);

	printf("\n\n");

	//	if (nCount == 1)
	//	{
	//		break;
	//	}

	//	printf("VS\n\n");

	//	//pPlayer++;//ポインタをインクリメント
	//}




}

//============================================
//役職の選択処理
//============================================
void SelectJob(Player* pPlayer, int* pSelectJob)
{
	int nChoose;//説明文選択用変数

	int nCount;

	printf("\x1b[0m+-------------------------------------------------------------+\n");
	printf("| ・相手のHPを０にしたら勝利！                                |\n");
	printf("| \x1b[33m・通常攻撃をすると\x1b[36mMPが５ずつ回復\x1b[0mしていくよ！\x1b[0m                |\n");
	printf("| ・スキル発動には\x1b[36mMPが必要だよ！\x1b[0mMPが足りなかった場合は　　　　|\n");
	printf("|\x1b[91m通常攻撃に移行する\x1b[0mので注意しよう！          　　　　　　　　 |\n");
	printf("| \x1b[33m・アイテムの初期個数は\x1b[36m３個\x1b[0mだよ！なお、アイテムは\x1b[93mガチャ\x1b[0mに \x1b[0m　 |\n");
	printf("|なっているよ！\x1b[0m                                               |\n");
	printf("| \x1b[0m・「何もしない」を選択すると、行動を\x1b[91mスキップ\x1b[0mする代わりに    |\n");
	printf("|\x1b[36mMPが１０回復\x1b[0mするよ！                                         |\n");
	printf("| \x1b[33m・\x1b[95m特殊なバフ\x1b[0mはたいていは\x1b[96m通常攻撃\x1b[0mに乗るよ！\x1b[0m                  |\n");
	printf("| ・\x1b[91m相手のスキル\x1b[0mや特定の条件下で\x1b[92mコマンド\x1b[0mが使えなくなると　　　|\n");
	printf("|\x1b[91m自動的に通常攻撃を行う\x1b[0mので注意しよう！		              |\n");
	printf("|\x1b[33m ・\x1b[96m１０ターン\x1b[0mごとに\x1b[92mアイテムは一個補充\x1b[0mされるよ！              \x1b[0m|\n");
	printf("+-------------------------------------------------------------+\n\n");
	printf("何かキーを押してください\n");

	rewind(stdin);
	getchar();
	system("cls");//画面をリセットする。

	//int nSelectJob[MAX_PLAYER];

	printf("\x1b[0m<<説明ページ>>\n\n");





	while (1)
	{

		//system("cls");
		printf("役職の説明は（１～６番）、役職選択に進むなら（０番）を選択してください！\n\n");
		printf("０～６の番号を選んで下さい（\x1b[32m１：剣士、\x1b[34m２：狩人、\x1b[33m３：巫女、\x1b[31m４：吸血鬼、\x1b[36m５：科学者、\x1b[35m６，魔法使い、\x1b[0m０：次へ進む）:");
		scanf("%d", &nChoose);


		if (nChoose == 1)
		{
			printf("+----------+\n");
			printf("|　\x1b[32m剣士\x1b[0m    |\n");
			printf("+----------+\n\n");

			printf("ステータスボーナス\n");
			printf("+----------+\n");
			printf("| LIFE*1.1 |\n");
			printf("|  ATK*1.1 |\n");
			printf("|  DEF*1.1 |\n");
			printf("|  MP :100 |\n");
			printf("+----------+\n\n");


			printf("役職説明\n");
			printf("+--------------------------------------------------------------------------+\n");
			printf("|攻守に優れたバランスタイプで、様々な状況に対応できるオールラウンダーだ！！| \n");
			printf("+--------------------------------------------------------------------------+\n\n");

			printf("スキル概要\n");
			printf("１：インパクトエッジ\n");
			printf("～敵に自分の攻撃力*2.8のダメージを与える！(MP-30)\n\n");

			printf("２：リカバーヒール\n");
			printf("～最大体力の25%%分、体力を回復する!(MP-80)\n\n");

			printf("３：カウンタースタイル\n");
			printf("～3ターンの間、確率で通常攻撃のカウンターを行う(50%%)(MP-40)\n\n");
			rewind(stdin);
			printf("何かキーを押してください\n");
			getchar();

			system("cls");//画面をリセットする。
		}


		else if (nChoose == 2)
		{
			printf("+----------+\n");
			printf("|　\x1b[34m狩人 \x1b[0m   |\n");
			printf("+----------+\n\n");

			printf("ステータスボーナス\n");
			printf("+----------+\n");
			printf("| LIFE*0.8 |\n");
			printf("|  ATK*1.3 |\n");
			printf("|  DEF*1.0 |\n");
			printf("|  MP :100 |\n");
			printf("+----------+\n\n");


			printf("役職説明\n");
			printf("+--------------------------------------------------------------------------+\n");
			printf("| 相手を華麗な弓さばきで追い詰めるスナイパー！力こそパワーという人向け　　 |\n");
			printf("+--------------------------------------------------------------------------+\n\n");

			printf("スキル概要\n");
			printf("１：レインアロー\n");
			printf("～敵に自分の攻撃力*0.7の８連続攻撃を行う！(命中率40％)(MP-25)\n\n");

			printf("２：パワダウンアロー\n");
			printf("攻撃力*2のダメージを与え、確率で２ターン攻撃力ダウン(ATK↓50％)(MP-40)\n\n");

			printf("３：スターアロー\n");
			printf("～敵に自分の攻撃力*5の防御無視の攻撃を行う！(MP-60)\n\n");
			printf("何かキーを押してください\n");
			rewind(stdin);
			getchar();
			system("cls");//画面をリセットする。
		}
		else if (nChoose == 3)
		{
			printf("+----------+\n");
			printf("|　\x1b[33m巫女\x1b[0m 　 |\n");
			printf("+----------+\n\n");

			printf("ステータスボーナス\n");
			printf("+----------+\n");
			printf("| LIFE*1.2 |\n");
			printf("|  ATK*1.0 |\n");
			printf("|  DEF*1.1 |\n");
			printf("|  MP :120 |\n");
			printf("+----------+\n\n");


			printf("役職説明\n");
			printf("+--------------------------------------------------------------------------+\n");
			printf("| 聖なる力で多彩なバフを扱う巫女さんだ！MPが少し多い！　　　　　　　　 　　|\n");
			printf("+--------------------------------------------------------------------------+\n\n");

			printf("スキル概要\n");
			printf("１：守りの陣\n");
			printf("～５ターンの間、DEFが上昇する。（DEF↑40％)(MP-35）\n\n");

			printf("２：ホーリーレイ\n");
			printf("～自分の攻撃力＋防御力＊1.2のダメージを与え、相手のMPを１５減らす。(ＭＰダメージ)(MP-45)\n\n");

			printf("３：会心の祈り\n");
			printf("～通常攻撃が３ターンの間、確率でクリティカルになるようになる。（CRT↑35％)(MP-55)\n\n");
			printf("何かキーを押してください\n");

			rewind(stdin);
			getchar();
			system("cls");//画面をリセットする。
		}
		else if (nChoose == 4)
		{
			printf("+----------+\n");
			printf("|　\x1b[31m吸血鬼\x1b[0m　|\n");
			printf("+----------+\n\n");

			printf("ステータスボーナス\n");
			printf("+----------+\n");
			printf("| LIFE*0.9 |\n");
			printf("|  ATK*1.2 |\n");
			printf("|  DEF*0.9 |\n");
			printf("|  MP :100 |\n");
			printf("+----------+\n\n");


			printf("役職説明\n");
			printf("+--------------------------------------------------------------------------------+\n");
			printf("| 相手のＨＰを吸うのが得意な夜の王者だ！耐久は回復で補おう！　　　　　　　　 　　|\n");
			printf("+--------------------------------------------------------------------------------+\n\n");

			printf("スキル概要\n");
			printf("１：ブラッティバイト\n");
			printf("～自分の攻撃力*2.2のダメージを与え、与えたダメージの７５％ＨＰを回復する！（吸収攻撃)(MP-35）\n\n");

			printf("２：クレイジーグラビティ\n");
			printf("～確率で敵の現在ＨＰを２０％減らす。又は自分の現在ＨＰを１５００回復する。(gravity20(30％))(MP-65)\n\n");

			printf("３：ファントムスタブ\n");
			printf("～自分の攻撃力*3.1のダメージを与え、確率で３ターンの間、スキル不可を付与する。（SKILL×35％)(MP-50)\n\n");
			printf("何かキーを押してください\n");

			rewind(stdin);
			getchar();
			system("cls");//画面をリセットする。
		}
		else if (nChoose == 5)
		{
			printf("+----------+\n");
			printf("|　\x1b[36m科学者\x1b[0m　|\n");
			printf("+----------+\n\n");

			printf("ステータスボーナス\n");
			printf("+----------+\n");
			printf("| LIFE*1.3 |\n");
			printf("|  ATK*0.8 |\n");
			printf("|  DEF*0.7 |\n");
			printf("|  MP :110 |\n");
			printf("+----------+\n\n");


			printf("役職説明\n");
			printf("+--------------------------------------------------------------------------------+\n");
			printf("| トリッキーな戦法を好むマッドサイエンティストだ！体力の倍率がとても高いぞ！　　 |\n");
			printf("+--------------------------------------------------------------------------------+\n\n");

			printf("スキル概要\n");
			printf("１：ポイズンミスト\n");
			printf("～自分の攻撃力*2.7のダメージを与え、確率で３ターンの間、毒状態にする。（毒35％)(MP-40）\n\n");

			printf("２：スチームバースト\n");
			printf("～確率で（自分の攻撃力＋相手の防御力）＊4.5の大ダメージを与える。または自分に火傷状態を付与。(MP-65)\n\n");

			printf("３：イカサマレーザー\n");
			printf("～敵の攻撃力＊2.6のダメージを与え、確率で敵の防御力を３ターンの間、４０％下げる。（敵攻撃力依存攻撃)(MP-45)\n\n");
			printf("何かキーを押してください\n");

			rewind(stdin);
			getchar();
			system("cls");//画面をリセットする。
		}
		else if (nChoose == 6)
		{
			printf("+----------+\n");
			printf("|　\x1b[35m魔法使い\x1b[0m|\n");
			printf("+----------+\n\n");

			printf("ステータスボーナス\n");
			printf("+----------+\n");
			printf("| LIFE*0.8 |\n");
			printf("|  ATK*1.0 |\n");
			printf("|  DEF*1.3 |\n");
			printf("|  MP:100  |\n");
			printf("+----------+\n\n");


			printf("役職説明\n");
			printf("+--------------------------------------------------------------------------------+\n");
			printf("| 魔法で戦う魔法使い！スワップマジックを使いこなせれば一人前だ！　　　　　　　   |\n");
			printf("+--------------------------------------------------------------------------------+\n\n");

			printf("スキル概要\n");
			printf("１：フレイムウェーブn");
			printf("～自分の攻撃力*1.1の４連続攻撃をし、確率で自分の攻撃力を２ターンの間２５％上昇させる。（命中率５０％)(連続攻撃)（攻撃力↑２５％）(MP-35)\n\n");

			printf("２：スワップマジック\n");
			printf("～自分の攻撃力と防御力の数値を入れ替える。(MP-25)\n\n");

			printf("３：アイスランス\n");
			printf("～自分の攻撃力*4.5のダメージを与え、確率で５ターン相手のアイテムコマンドを封印する。（アイテム×50％)(MP-70)\n\n");
			printf("何かキーを押してください\n");

			rewind(stdin);
			getchar();
			system("cls");//画面をリセットする。
		}
		else if (nChoose == 0)
		{
			break;
		}

		else
		{
		
			rewind(stdin);
			getchar();
			system("cls");//画面をリセットする。
		}
		//system("cls");//画面をリセットする
	}

	for (nCount = 0; nCount < MAX_PLAYER; nCount++)
	{



		while (1)
		{
			printf("プレイヤー%dの役職を選んでください\n（\x1b[32m１：剣士、\x1b[34m２：狩人、\x1b[33m３：巫女、\x1b[31m４：吸血鬼、\x1b[36m５：科学者、\x1b[35m６，魔法使い、\x1b[0m\n", nCount + 1);
			scanf("%d", &pSelectJob[nCount]);

			if (pSelectJob[nCount] == 1)
			{
				pPlayer->fLife = g_aPlayer[nCount].fLife * 1.1;//体力の値を1.1倍する。
				pPlayer->fATK = g_aPlayer[nCount].fATK * 1.1;//攻撃力の値を1.1倍する。
				pPlayer->fDEF = g_aPlayer[nCount].fDEF * 1.1;//防御力の値を1.1倍する。
				pPlayer->fMP = 100;//マジックポイントを１００に設定。
				pPlayer->nCheckJob = 1;//役職が剣士の時に、番号１を代入する。
				break;
			}

			else if (pSelectJob[nCount] == 2)
			{
				pPlayer->fLife = g_aPlayer[nCount].fLife * 0.8;//体力の値を0.9倍する。
				pPlayer->fATK = g_aPlayer[nCount].fATK * 1.3;//攻撃力の値を1.2倍する。
				pPlayer->fDEF = g_aPlayer[nCount].fDEF * 1.0;//防御力の値を1.1倍する。
				pPlayer->fMP = 100;//マジックポイントを１００に設定。
				pPlayer->nCheckJob = 2;//役職が狩人の時に、番号２を代入する。
				break;
			}
			else if (pSelectJob[nCount] == 3)
			{
				pPlayer->fLife = g_aPlayer[nCount].fLife * 1.2;//体力の値を1.2倍する。
				pPlayer->fATK = g_aPlayer[nCount].fATK * 1.0;//攻撃力の値を1.2倍する。
				pPlayer->fDEF = g_aPlayer[nCount].fDEF * 1.1;//防御力の値を1.1倍する。
				pPlayer->fMP = 120;//マジックポイントを１００に設定。
				pPlayer->nCheckJob = 3;//役職が巫女の時に、番号３を代入する。
				break;
			}
			else if (pSelectJob[nCount] == 4)//選んだ職業が「吸血鬼」だった場合
			{
				pPlayer->fLife = g_aPlayer[nCount].fLife * 0.9;//体力の値を0.9倍する。
				pPlayer->fATK = g_aPlayer[nCount].fATK * 1.2;//攻撃力の値を1.2倍する。
				pPlayer->fDEF = g_aPlayer[nCount].fDEF * 0.9;//防御力の値を0.9倍する。
				pPlayer->fMP = 100;//マジックポイントを100に設定。
				pPlayer->nCheckJob = 4;//役職が吸血鬼の時に、番号４を代入する。
				break;
			}
			else if (pSelectJob[nCount] == 5)//選んだ職業が「科学者」だった場合
			{
				pPlayer->fLife = g_aPlayer[nCount].fLife * 1.3;//体力の値を1.4倍する。
				pPlayer->fATK = g_aPlayer[nCount].fATK * 0.8;//攻撃力の値を0.8倍する。
				pPlayer->fDEF = g_aPlayer[nCount].fDEF * 0.7;//防御力の値を0.8倍する。
				pPlayer->fMP = 110;//マジックポイントを110に設定。
				pPlayer->nCheckJob = 5;//役職が吸血鬼の時に、番号４を代入する。
				break;
			}
			else if (pSelectJob[nCount] == 6)//選んだ職業が「魔法使い」だった場合
			{
				pPlayer->fLife = g_aPlayer[nCount].fLife * 0.9;//体力の値を0.9倍する。
				pPlayer->fATK = g_aPlayer[nCount].fATK * 1.0;//攻撃力の値を0.9倍する。
				pPlayer->fDEF = g_aPlayer[nCount].fDEF * 1.3;//防御力の値を1.3倍する。
				pPlayer->fMP = 100;//マジックポイントを110に設定。
				pPlayer->nCheckJob = 6;//役職が魔法使いの時に、番号４を代入する。
				break;
			}
			else
			{
				while (getchar() != '\n');//数字以外を入力したときにエラーメッセージを出す。
				printf("入力された値が違います。数字を入力してください\n\n");
				rewind(stdin);
				getchar();
				system("cls");
			}

		}

		pPlayer++;//ポインタをインクリメントして基準点をずらす。


	}
	printf("準備は完了！バトル開始ーーーーーーーーーー！\n\n");


	rewind(stdin);
	getchar();
	system("cls");

	

	

}

//=================================================
//バトルの処理
//=================================================



void SetBattle(Player* pPlayer)
{
	PlaySound(TEXT("bgm3.wav"), NULL, SND_ASYNC | SND_FILENAME);


	float fMaxLife = g_aPlayer[PlayerNumber1].fLife;//

	//pPlayer;//ポインタに先頭アドレスを代入する。（配列[0]へ）

	//int nCount;

	int nTurnAttack;//攻撃ターンの変数

	//float fDamageResult;//ダメージの結果を代入する変数

	//float fRandum1;

	//int CheckRandum;

	bool bEndBattle;//先頭が終了したかどうかのフラグを設定する。（この型は、true（真)、又はfalse（偽)の値となる。)

	//bool bSwordSkill[MAX_PLAYER];

	int nChoose;//コマンドを選ぶときに扱う変数。

	int nSkillChoose;//スキルを選ぶときに扱う変数。

	int nPlayer1ItemRouletteUseCount = 3;//プレイヤー１がアイテムルーレットを使える残りの回数を宣言する。
	int nPlayer2ItemRouletteUseCount = 3;//プレイヤー２がアイテムルーレットを使える残りの回数を宣言する。

	int nShootingStar;//アイテム「シューティングスター」のランダムダメージを宣言する。

	/*printf("<<プレイヤー１>>　　vs　　<<プレイヤー２>>\n");

	printf(" 名前 ：%s                      %s\n", &g_aPlayer[0].aName[0], &g_aPlayer[1].aName[0]);

	printf("  体力：%.f                    %.f\n", g_aPlayer[0].fLife, g_aPlayer[1].fLife);

	printf("攻撃力：%.f                     %.f\n", g_aPlayer[0].fATK, g_aPlayer[1].fATK);

	printf("防御力：%.f                     %.f\n", g_aPlayer[0].fDEF, g_aPlayer[1].fDEF);*/

	//===============================================================
	//スキルの特殊効果フラグ
	//================================================================
	bool Player1CounterFlag;//プレイヤー１のカウンター発動のフラグを宣言する。
	Player1CounterFlag = false;//プレイヤー１のカウンターフラグをfalseにしておく。

	bool Player2CounterFlag;//プレイヤー２のカウンター発動のフラグを宣言する。
	Player2CounterFlag = false;//プレイヤー２のカウンターフラグをfalseにしておく。

	int nPlayer1CounterCount;//プレイヤー１のカウンターフラグがtrueになっているときに、trueになっているターンをカウントする。
	nPlayer1CounterCount = 0;//カウンターカウントに「０」を代入する。

	int nPlayer2CounterCount;//プレイヤー２のカウンターフラグがtrueになっているときに、trueになっているターンをカウントする。
	nPlayer2CounterCount = 0;//カウンターカウントに「０」を代入する。

	bool Player2PowerDownArrowDebuff;//プレイヤー２がプレイヤー１のパワダウンアローのデバフ（攻撃力３０％低下）を受けていることのフラグを宣言する。
	Player2PowerDownArrowDebuff = false;//フラグをfalseにしておく。

	int nPlayer2PowerDownArrowDebuffCount;//プレイヤー２がパワダウンアローのデバフを受けているターン数をカウントする変数。
	nPlayer2PowerDownArrowDebuffCount = 0;


	bool Player1PowerDownArrowDebuff;//プレイヤー１がパワダウンアローのデバフ（攻撃力３０％低下）を受けていることのフラグを宣言する。
	Player1PowerDownArrowDebuff = false;//フラグをfalseにしておく。

	int nPlayer1PowerDownArrowDebuffCount;//プレイヤー１がパワダウンアローのデバフを受けているターン数をカウントする変数。
	nPlayer1PowerDownArrowDebuffCount = 0;//ターン数のカウントを０にしておく。


	bool bPlayer1DefenceLineBuff;//プレイヤー１がスキル「守りの陣」のバフを受けていることを表すフラグを宣言する。
	bPlayer1DefenceLineBuff = false;//フラグをfalseにしておく。

	int nPlayer1DefenceLineBuffCount;//プレイヤー１がスキル「守りの陣」のバフを受けているターン数をカウントする変数。
	nPlayer1DefenceLineBuffCount = 0;//カウントを０にしておく。

	bool bPlayer2DefenceLineBuff;//プレイヤー２がスキル「守りの陣」のバフを受けていることを表すフラグを宣言する。
	bPlayer2DefenceLineBuff = false;//フラグをfalseにしておく。

	int nPlayer2DefenceLineBuffCount;//プレイヤー２がスキル「守りの陣」のバフを受けているターン数をカウントする変数。
	nPlayer2DefenceLineBuffCount = 0;//カウントを０にしておく。


	bool bPlayer1CriticalLineBuff;//プレイヤー１がスキル「会心の陣」のバフを受けていることを表すフラグを宣言する。
	bPlayer1CriticalLineBuff = false;//フラグをfalseにしておく。

	int nPlayer1CriticalLineBuffCount;//プレイヤー１がスキル「会心の陣」のバフを受けているターン数をカウントするフラグを宣言する。
	nPlayer1CriticalLineBuffCount = 0;//カウントを０にしておく。

	bool bPlayer2CriticalLineBuff;//プレイヤー２がスキル「会心の陣」のバフを受けていることを表すフラグを宣言する。
	bPlayer2CriticalLineBuff = false;//フラグをfalseにしておく。

	int nPlayer2CriticalLineBuffCount;//プレイヤー２がスキル「会心の陣」のバフを受けているターン数をカウントするフラグを宣言する。
	nPlayer2CriticalLineBuffCount = 0;//カウントを０にしておく。


	bool bPlayer2PhantomStabDebuff;//プレイヤー２に「ファントムスタブ」のデバフを与えるフラグを宣言する。
	bPlayer2PhantomStabDebuff = false;//フラグをfalseにしておく。

	int nPlayer2PhantomStabDebuffCount;//プレイヤー２がスキル「ファントムスタブ」のデバフを受けているターン数をカウントする変数を宣言する。
	nPlayer2PhantomStabDebuffCount = 0;//カウントを０にしておく。

	bool bPlayer1PhantomStabDebuff;//プレイヤー１に「ファントムスタブ」のデバフを与えるフラグを宣言する。
	bPlayer1PhantomStabDebuff = false;//フラグをfalseにしておく。

	int nPlayer1PhantomStabDebuffCount;//プレイヤー１がスキル「ファントムスタブ」のデバフを受けているターン数をカウントする変数を宣言する。
	nPlayer1PhantomStabDebuffCount = 0;//カウントを０にしておく。


	bool bPlayer2PoisonMistState;//プレイヤー２にスキル「ポイズンミスト」の毒状態を与えるフラグを宣言する。
	bPlayer2PoisonMistState = false;//フラグをfalseにしておく。

	int nPlayer2PoisonMistStateCount;//プレイヤー２がスキル「ポイズンミスト」の毒状態を受けているターン数をカウントする変数を宣言する。
	nPlayer2PoisonMistStateCount = 0;//カウントを０にしておく。

	bool bPlayer1PoisonMistState;//プレイヤー１にスキル「ポイズンミスト」の毒状態を与えるフラグを宣言する。
	bPlayer1PoisonMistState = false;//フラグをfalseにしておく。

	int nPlayer1PoisonMistStateCount;//プレイヤー１がスキル「ポイズンミスト」の毒状態を受けているターン数をカウントする変数を宣言する。
	nPlayer1PoisonMistStateCount = 0;//カウントを０にしておく。


	bool bPlayer1SteamBurstBurn;//プレイヤー1がスキル「スチームバースト」のデメリット効果「火傷」を受けていることを表すフラグを宣言する。
	bPlayer1SteamBurstBurn = false;//フラグをfalseにしておく。

	int nPlayr1SteamBurstBurnCount;//プレイヤー１がスキル「スチームバースト」のデメリット効果「火傷」を受けているターン数をカウントする変数を宣言する。
	nPlayr1SteamBurstBurnCount = 0;//カウントを０にしておく。

	bool bPlayer2SteamBurstBurn;//プレイヤー２がスキル「スチームバースト」のデメリット効果「火傷」を受けていることを表すフラグを宣言する。
	bPlayer2SteamBurstBurn = false;//フラグをfalseにしておく。

	int nPlayr2SteamBurstBurnCount;//プレイヤー２がスキル「スチームバースト」のデメリット効果「火傷」を受けているターン数をカウントする変数を宣言する。
	nPlayr2SteamBurstBurnCount = 0;//カウントを０にしておく。


	bool bPlayer2SquitLaserDebuff;//プレイヤー２がスキル「イカサマレーザー」のデバフを受けているかどうかを表すフラグを宣言する。
	bPlayer2SquitLaserDebuff = false;//フラグをfalseにしておく。

	int nPlayer2SquitLaserDebuffCount;//プレイヤー２がスキル「イカサマレーザー」のデバフを受けているターン数をカウントするフラグを宣言する。
	nPlayer2SquitLaserDebuffCount = 0;//カウントを０にしておく。

	bool bPlayer1SquitLaserDebuff;//プレイヤー１がスキル「イカサマレーザー」のデバフを受けているかどうかを表すフラグを宣言する。
	bPlayer1SquitLaserDebuff = false;//フラグをfalseにしておく。

	int nPlayer1SquitLaserDebuffCount;//プレイヤー１がスキル「イカサマレーザー」のデバフを受けているターン数をカウントするフラグを宣言する。
	nPlayer1SquitLaserDebuffCount = 0;//カウントを０にしておく。


	bool bPlayer1FlameWaveBuff = false;//プレイヤー１がスキル「フレイムウェーブ」のバフを受けているかどうかを表すフラグを宣言する。
	int nPlayer1FlameWaveBuffCount = 0;//ぷれいやー１がスキル「フレイムウェーブ」のバフを受けているターン数をカウントする変数を宣言する。

	bool bPlayer2FlameWaveBuff = false;//プレイヤー2がスキル「フレイムウェーブ」のバフを受けているかどうかを表すフラグを宣言する。
	int nPlayer2FlameWaveBuffCount = 0;//ぷれいやー2がスキル「フレイムウェーブ」のバフを受けているターン数をカウントする変数を宣言する。


	bool bPlayer2IceLanceDebuff = false;//プレイヤー２がスキル「アイスランス」のデバフを受けているかどうかのフラグを宣言し、フラグをfalseにする。
	int nPlayer2IceLanceDebuffCount = 0;//プレイヤー２がスキル「アイスランス」のデバフを受けているターン数をカウントする変数を宣言し、カウントを０にする。

	bool bPlayer1IceLanceDebuff = false;//プレイヤー１がスキル「アイスランス」のデバフを受けているかどうかのフラグを宣言し、フラグをfalseにする。
	int nPlayer1IceLanceDebuffCount = 0;//プレイヤー１がスキル「アイスランス」のデバフを受けているターン数をカウントする変数を宣言し、カウントを０にする。

	//============================================================================================================================================================================
	int nTurn=0;
	int nHalfTurn = 0;


	//最初の攻撃ターンを設定する。
	nTurnAttack = 0;

	//戦闘終了フラグをfalseにする。
	bEndBattle = false;


	float fPlayer1MaxHp;//プレイヤー１の最大体力を代入する変数。
	fPlayer1MaxHp = g_aPlayer[0].fLife;

	float fPlayer2MaxHp;//プレイヤー２の最大体力を代入する変数。
	fPlayer2MaxHp = g_aPlayer[1].fLife;

	float fPlayer1MaxMp;//プレイヤー１の最大MPを代入する変数。
	fPlayer1MaxMp = g_aPlayer[0].fMP;

	float fPlayer2MaxMp;//プレイヤー２の最大MPを代入する変数。
	fPlayer2MaxMp = g_aPlayer[1].fMP;

	float fPlayer1MaxATK;//プレイヤー１の最大攻撃力を代入する変数。
	fPlayer1MaxATK = g_aPlayer[PlayerNumber1].fATK;

	float fPlayer2MaxATK;//プレイヤー２の最大攻撃力を代入する変数。
	fPlayer2MaxATK = g_aPlayer[PlayerNumber2].fATK;

	float fPlayer1MaxDEF;//プレイヤー１の最大防御力を代入する変数。
	fPlayer1MaxDEF = g_aPlayer[PlayerNumber1].fDEF;

	float fPlayer2MaxDEF;//プレイヤー１の最大防御力を代入する変数。
	fPlayer2MaxDEF = g_aPlayer[PlayerNumber2].fDEF;

	int nPlayer1ItemRouletteRandum;//プレイヤー１のアイテムルーレットのランダム関数を宣言する。
	int nPlayer2ItemRouletteRandum;//プレイヤー２のアイテムルーレットのランダム関数を宣言する。


	//================================================
	//アイテム効果フラグ
	//================================================
	bool bPlayer1CurryRiceBuffLV1 = false;//カレーライスのバフを受けている言を表すフラグを宣言し、falseにする。

	int nPlayer1CurryRiceBuffLV1Count = 0;//カレーライスのバフを受けているターン数のカウントを宣言し、カウントを０にする。

	bool bPlayer2CurryRiceBuffLV1 = false;//カレーライスのバフを受けていることを表すフラグを宣言し、falseにする。

	int nPlayer2CurryRiceBuffLV1Count = 0;//カレーライスのバフを受けているターン数のカウントを宣言し、カウントを０にする。


	bool bPlayer1CurryRiceBuffLV2 = false;//プレイヤー１がシーフードカレーのバフを受けている言を表すフラグを宣言し、falseにする。

	int nPlayer1CurryRiceBuffLV2Count = 0;//プレイヤー１がシーフードカレーのバフを受けているターン数のカウントを宣言し、カウントを０にする。

	bool bPlayer2CurryRiceBuffLV2 = false;//プレイヤー２がシーフードカレーのバフを受けていることを表すフラグを宣言し、falseにする。

	int nPlayer2CurryRiceBuffLV2Count = 0;//プレイヤー２がシーフードカレーのバフを受けているターン数のカウントを宣言し、カウントを０にする。


	bool bPlayer1CurryRiceBuffLV3 = false;//プレイヤー１がポークカレーのバフを受けている言を表すフラグを宣言し、falseにする。

	int nPlayer1CurryRiceBuffLV3Count = 0;//プレイヤー１がポークカレーのバフを受けているターン数のカウントを宣言し、カウントを０にする。

	bool bPlayer2CurryRiceBuffLV3 = false;//プレイヤー２がポークカレーのバフを受けていることを表すフラグを宣言し、falseにする。

	int nPlayer2CurryRiceBuffLV3Count = 0;//プレイヤー２がポークカレーのバフを受けているターン数のカウントを宣言し、カウントを０にする。



	bool bPlayer1RiceBallBuffLV1 = false;//プレイヤー１が塩おむすびのバフを受けていることを表すフラグを宣言し、フラグをfalseにする。

	int nPlayer1RiceBallBuffLV1Count = 0;//プレイヤー１が塩おむすびのバフを受けているターン数のカウントを宣言し、カウントを０にする。

	bool bPlayer2RiceBallBuffLV1 = false;//プレイヤー２が塩おむすびのバフを受けていることを表すフラグを宣言し、フラグをfalseにする。

	int nPlayer2RiceBallBuffLV1Count = 0;//プレイヤー２が塩おむすびのバフを受けているターン数のカウントを宣言し、カウントを０にする。


	bool bPlayer1RiceBallBuffLV2 = false;//プレイヤー１が鮭おむすびのバフを受けていることを表すフラグを宣言し、フラグをfalseにする。

	int nPlayer1RiceBallBuffLV2Count = 0;//プレイヤー１が鮭おむすびのバフを受けているターン数のカウントを宣言し、カウントを０にする。

	bool bPlayer2RiceBallBuffLV2 = false;//プレイヤー２が鮭おむすびのバフを受けていることを表すフラグを宣言し、フラグをfalseにする。

	int nPlayer2RiceBallBuffLV2Count = 0;//プレイヤー２が鮭おむすびのバフを受けているターン数のカウントを宣言し、カウントを０にする。


	bool bPlayer1RiceBallBuffLV3 = false;//プレイヤー１がいくらおむすびのバフを受けていることを表すフラグを宣言し、フラグをfalseにする。

	int nPlayer1RiceBallBuffLV3Count = 0;//プレイヤー１がいくらおむすびのバフを受けているターン数のカウントを宣言し、カウントを０にする。

	bool bPlayer2RiceBallBuffLV3 = false;//プレイヤー２がいくらおむすびのバフを受けていることを表すフラグを宣言し、フラグをfalseにする。

	int nPlayer2RiceBallBuffLV3Count = 0;//プレイヤー２がいくらおむすびのバフを受けているターン数のカウントを宣言し、カウントを０にする。


	bool bPlayer2WeakBallDeBuff = false;//プレイヤー２が衰弱玉のデバフを受けていることを表すフラグを宣言し、フラグをfalseにする。
	int nPlayer2WeakBallDeBuffCount = 0;//プレイヤー２が衰弱玉のデバフを受けているターン数のカウントを宣言し、カウントを０にする。

	bool bPlayer1WeakBallDeBuff = false;//プレイヤー１が衰弱玉のデバフを受けていることを表すフラグを宣言し、フラグをfalseにする。
	int nPlayer1WeakBallDeBuffCount = 0;//プレイヤー１が衰弱玉のデバフを受けているターン数のカウントを宣言し、カウントを０にする。



	do
	{
		//===============================================
		//プレイヤー１のターン
		//===============================================
		if (nTurnAttack == 0)
		{

			while (1)//プレイヤー１のターン処理
			{
				printf("                                   %dターン目\n", nTurn/2+1);

				printf("            <<プレイヤー１>>　　       vs　　      <<プレイヤー２>>\n");

				printf("  名前：%16s                      %16s\n", &g_aPlayer[0].aName[0], &g_aPlayer[1].aName[0]);

				printf("  \x1b[32m体力：%16.f                     %16.f\n", g_aPlayer[0].fLife, g_aPlayer[1].fLife);

				printf("\x1b[31m攻撃力：%16.f                     %16.f\n", g_aPlayer[0].fATK, g_aPlayer[1].fATK);

				printf("\x1b[36m防御力：%16.f                     %16.f\n", g_aPlayer[0].fDEF, g_aPlayer[1].fDEF);

				printf("    \x1b[33mMP：%16.f                     %16.f\x1b[0m\n", g_aPlayer[0].fMP, g_aPlayer[1].fMP);

				printf("+<<プレイヤー１のターン>>\n");
				printf("  %sはどうする？                       \n", &g_aPlayer[0].aName[0]);
				printf("+-------------------------------------+\n");
				printf("|  １：攻撃\x1b[0m　　　　　　　　　　　　　 |\n");
				printf("|  ２：スキルアタック\x1b[0m                 |\n");
				printf("|  ３：アイテムを使う(残り%d回)\x1b[0m        |\n", nPlayer1ItemRouletteUseCount);
				printf("|  ４：何もしない\x1b[0m                     |\n");
				printf("+-------------------------------------+\n");
				printf("＊：");
				nChoose = 0;//初期化
				scanf("%d", &nChoose);

				switch (nChoose)
				{
				case 4:
					pPlayer->fMP = pPlayer->fMP + 10;//「行動しない」を選択した場合、プレイヤー１のＭＰを１０回復する
					printf("%sは様子を見ている！", &pPlayer->aName[0]);
					rewind(stdin);
					getchar();
					system("cls");//画面をリセットする。
					break;

				case 1:
					pPlayer->fMP = pPlayer->fMP + 5;//通常攻撃を選択した場合、プレイヤー１のＭＰを５回復する。
					Player1NormalAttack(&g_aPlayer[0], Player2CounterFlag, bPlayer1CriticalLineBuff);
					rewind(stdin);
					getchar();
					system("cls");//画面をリセットする
					break;

				case 2://スキルコマンド
					if (bPlayer1PhantomStabDebuff == false)//ファントムスタブの「スキル不可」のデバフが発動していない場合に処理を実行する。
					{

						if (g_aPlayer[0].nCheckJob == 1)//プレイヤー１の選択した職業が「剣士」なら処理を実行する。================================================================
						{
							printf("+---------------------------------------------------------------------------+\n");
							printf("<<使いたいスキルを選んでください>>\n");
							printf("残りMP:%.f\n", g_aPlayer[PlayerNumber1].fMP);
							printf("１：インパクトエッジ「攻撃力*3ののダメージを与える。」(MP-30)\n");
							printf("２：リカバーヒール「最大ＨＰの２５％分のＨＰを回復する」(MP-80)\n");
							printf("３：カウンタースタイル「３ターンの間、確率で通常攻撃をカウンターをする。」(MP-40)\n＊：");
							nSkillChoose = 0;//初期化
							while (1)
							{
								scanf("%d", &nSkillChoose);//スキル選択をする。



								switch (nSkillChoose)
								{
								case 1:
									if (g_aPlayer[PlayerNumber1].fMP >= 30)//プレイヤー１のMPが３０以上なら処理を実行する。
									{
										Player1InpactEdge(&g_aPlayer[0]);//プレイヤー１のスキル「インパクトエッジ」を実行する。
									}
									else
									{
										pPlayer->fMP = pPlayer->fMP + 5;//通常攻撃を選択した場合、プレイヤー１のＭＰを５回復する。
										printf("ＭＰが足りない(´・ω・｀)やむをえず通常攻撃！\n");
										getchar();
										Player1NormalAttack(&g_aPlayer[0], Player2CounterFlag, bPlayer1CriticalLineBuff);//スキルが発動できなかった場合に通常攻撃を発動。

									}
									break;

								case 2:
									if (g_aPlayer[PlayerNumber1].fMP >= 80)//プレイヤー１のMPが80以上なら処理を実行する。
									{
										Player1RecoverHeal(&g_aPlayer[0], fPlayer1MaxHp);//プレイヤー１のスキル「リカバーヒール」を実行する。
									}
									else
									{
										pPlayer->fMP = pPlayer->fMP + 5;//通常攻撃を選択した場合、プレイヤー１のＭＰを５回復する。
										printf("ＭＰが足りない(´・ω・｀)やむをえず通常攻撃！\n");
										getchar();
										Player1NormalAttack(&g_aPlayer[0], Player2CounterFlag, bPlayer1CriticalLineBuff);//スキルが発動できなかった場合に通常攻撃を発動。
									}
									break;

								case 3:
									if (g_aPlayer[PlayerNumber1].fMP >= 40)
									{
										pPlayer->fMP = pPlayer->fMP - 40;
										printf("%sはカウンターの姿勢をとった！\n", &g_aPlayer[PlayerNumber1].aName[0]);//プレイヤー１の技：「カウンタースタイル」
										Player1CounterFlag = true;//プレイヤー１のカウンターフラグをtrueにする（通常攻撃を受けた場合、確率でカウンター（５０％）
									}
									else
									{
										pPlayer->fMP = pPlayer->fMP + 5;//通常攻撃を選択した場合、プレイヤー１のＭＰを５回復する。
										printf("ＭＰが足りない(´・ω・｀)やむをえず通常攻撃！\n");
										getchar();
										Player1NormalAttack(&g_aPlayer[0], Player2CounterFlag, bPlayer1CriticalLineBuff);//スキルが発動できなかった場合に通常攻撃を発動。
									}

									break;

								default:
									while (getchar() != '\n');//数字以外を入力したときにエラーメッセージを出す。
									printf("入力された値が違うよ！\n＊：");
									break;
								}

								if (nSkillChoose == 1 || nSkillChoose == 2 || nSkillChoose == 3)
								{
									break;
								}
							}

						}
						else if (g_aPlayer[0].nCheckJob == 2)//プレイヤー１の選択した職業が「狩人」なら処理を実行する。======================================================================
						{
							printf("+---------------------------------------------------------------------------+\n");
							printf("<<使いたいスキルを選んでください>>\n");
							printf("残りMP:%.f\n", g_aPlayer[PlayerNumber1].fMP);
							printf("１：レインアロー「攻撃力＊０．５の８連続j攻撃！」（命中率４０％）(MP-25)\n");
							printf("２：パワダウンアロー「攻撃力*2のダメージを与え、確率で２ターン攻撃力ダウン」(ATK↓50%)(デバフ確率４０％）(MP-40)\n");
							printf("３：スターアロー「敵に自分の攻撃力*5の防御無視の攻撃を行う！」(MP-60)\n＊：");
							nSkillChoose = 0;//初期化
							while (1)
							{
								scanf("%d", &nSkillChoose);//スキル選択をする。



								switch (nSkillChoose)
								{
								case 1:
									if (g_aPlayer[PlayerNumber1].fMP >= 25)//プレイヤー１のMPが25以上なら処理を実行する。
									{
										Player1RainArrow(&g_aPlayer[0]);//プレイヤー１のスキル「インパクトエッジ」を実行する。
									}
									else
									{
										pPlayer->fMP = pPlayer->fMP + 5;//通常攻撃を選択した場合、プレイヤー１のＭＰを５回復する。
										printf("ＭＰが足りない(´・ω・｀)やむをえず通常攻撃！\n");
										getchar();
										Player1NormalAttack(&g_aPlayer[0], Player2CounterFlag, bPlayer1CriticalLineBuff);//スキルが発動できなかった場合に通常攻撃を発動。

									}
									break;

								case 2:
									if (g_aPlayer[PlayerNumber1].fMP >= 40)//プレイヤー１のMPが４０以上なら処理を実行する。
									{
										Player2PowerDownArrowDebuff = Player1PowerDownArrow(&g_aPlayer[0]);//プレイヤー１のスキル「パワダウンアロー」を実行する。
									}
									else
									{
										pPlayer->fMP = pPlayer->fMP + 5;//通常攻撃を選択した場合、プレイヤー１のＭＰを５回復する。
										printf("ＭＰが足りない(´・ω・｀)やむをえず通常攻撃！\n");
										getchar();
										Player1NormalAttack(&g_aPlayer[0], Player2CounterFlag, bPlayer1CriticalLineBuff);//スキルが発動できなかった場合に通常攻撃を発動。
									}
									break;

								case 3:
									if (g_aPlayer[PlayerNumber1].fMP >= 60)//プレイヤー１のMPが６０以上なら処理を実行する。
									{
										Player1StarArrow(&g_aPlayer[0]);//プレイヤー１のスキル「スターアロー」を実行する。
									}
									else
									{
										pPlayer->fMP = pPlayer->fMP + 5;//通常攻撃を選択した場合、プレイヤー１のＭＰを５回復する。
										printf("ＭＰが足りない(´・ω・｀)やむをえず通常攻撃！\n");
										getchar();
										Player1NormalAttack(&g_aPlayer[0], Player2CounterFlag, bPlayer1CriticalLineBuff);//スキルが発動できなかった場合に通常攻撃を発動。
									}

									break;

								default:
									while (getchar() != '\n');//数字以外を入力したときにエラーメッセージを出す。
									printf("入力された値が違うよ！\n＊：");
									break;
								}

								if (nSkillChoose == 1 || nSkillChoose == 2 || nSkillChoose == 3)
								{
									break;
								}
							}

						}
						else if (g_aPlayer[PlayerNumber1].nCheckJob == 3)//プレイヤー１の選択した職業が「巫女」なら処理を実行する。======================================================================
						{
							printf("+---------------------------------------------------------------------------+\n");
							printf("<<使いたいスキルを選んでください>>\n");
							printf("残りMP:%.f\n", g_aPlayer[PlayerNumber1].fMP);
							printf("１：守りの陣「５ターンの間、防御力を上昇させる。」（DEF↑40％）(MP-35)\n");
							printf("２：ホーリーレイ「攻撃力+防御力＊1.2のダメージを与え、相手のＭＰを１５減らす。」(ＭＰダメージ)(MP-45)\n");
							printf("３：会心の陣「３ターンの間、通常攻撃が確率でクリティカルになる。」（CRT↑35％）(MP-55)\n＊：");
							nSkillChoose = 0;//初期化
							while (1)
							{
								scanf("%d", &nSkillChoose);//スキル選択をする。



								switch (nSkillChoose)
								{
								case 1:
									if (g_aPlayer[PlayerNumber1].fMP >= 35 && bPlayer1DefenceLineBuff == false)//プレイヤー１のMPが35以上かつ、守りの陣が発動していない場合に処理を実行する。
									{
										Player1DefenceLine(&g_aPlayer[0]);//プレイヤー１のスキル「守りの陣」を実行する。
										bPlayer1DefenceLineBuff = true;//スキルのバフが発動したのでフラグをtrueにする。
									}
									else if (bPlayer1DefenceLineBuff == true)
									{
										printf("守りの陣は発動中だ！　やむを得ず通常攻撃！\n");
										getchar();
										Player1NormalAttack(&g_aPlayer[0], Player2CounterFlag, bPlayer1CriticalLineBuff);//スキルが発動できなかった場合に通常攻撃を発動。
									}

									else
									{
										pPlayer->fMP = pPlayer->fMP + 5;//通常攻撃を選択した場合、プレイヤー１のＭＰを５回復する。
										printf("ＭＰが足りない(´・ω・｀)やむをえず通常攻撃！\n");
										getchar();
										Player1NormalAttack(&g_aPlayer[0], Player2CounterFlag, bPlayer1CriticalLineBuff);//スキルが発動できなかった場合に通常攻撃を発動。

									}
									break;

								case 2:
									if (g_aPlayer[PlayerNumber1].fMP >= 45)//プレイヤー１のMPが４５以上なら処理を実行する。
									{
										Player1HolyRay(&g_aPlayer[0]);//プレイヤー１のスキル「ホーリーレイ」を実行する。
									}
									else
									{
										pPlayer->fMP = pPlayer->fMP + 5;//通常攻撃を選択した場合、プレイヤー１のＭＰを５回復する。
										printf("ＭＰが足りない(´・ω・｀)やむをえず通常攻撃！\n");
										getchar();
										Player1NormalAttack(&g_aPlayer[0], Player2CounterFlag, bPlayer1CriticalLineBuff);//スキルが発動できなかった場合に通常攻撃を発動。
									}
									break;

								case 3:
									if (g_aPlayer[PlayerNumber1].fMP >= 55)//プレイヤー１のMPが５５以上なら処理を実行する。
									{
										pPlayer->fMP = pPlayer->fMP - 55;//ＭＰを５５減らす。
										printf("%sの会心の陣！\n", &pPlayer[PlayerNumber1].aName[0]);//プレイヤー１のスキル「会心の陣」を発動する。 
										bPlayer1CriticalLineBuff = true;//バフを受けていることを表すフラグをtrueにする。
										printf("３ターンの間、%sの通常攻撃のクリティカル率が上昇する！\n", &g_aPlayer[PlayerNumber1].aName[0]);
									}
									else if (bPlayer1CriticalLineBuff == true)
									{
										printf("会心の陣は発動中だ！　やむを得ず通常攻撃！\n");
										getchar();
										Player1NormalAttack(&g_aPlayer[0], Player2CounterFlag, bPlayer1CriticalLineBuff);//スキルが発動できなかった場合に通常攻撃を発動。
									}
									else
									{
										pPlayer->fMP = pPlayer->fMP + 5;//通常攻撃を選択した場合、プレイヤー１のＭＰを５回復する。
										printf("ＭＰが足りない(´・ω・｀)やむをえず通常攻撃！\n");
										getchar();
										Player1NormalAttack(&g_aPlayer[0], Player2CounterFlag, bPlayer1CriticalLineBuff);//スキルが発動できなかった場合に通常攻撃を発動。
									}

									break;

								default:
									while (getchar() != '\n');//数字以外を入力したときにエラーメッセージを出す。
									printf("入力された値が違うよ！\n＊：");
									break;
								}

								if (nSkillChoose == 1 || nSkillChoose == 2 || nSkillChoose == 3)
								{
									break;
								}
							}

						}
						else if (g_aPlayer[PlayerNumber1].nCheckJob == 4)//プレイヤー１の選択した職業が「吸血鬼」なら処理を実行する。================================================================
						{
							printf("+---------------------------------------------------------------------------+\n");
							printf("<<使いたいスキルを選んでください>>\n");
							printf("残りMP:%.f\n", g_aPlayer[PlayerNumber1].fMP);
							printf("１：ブラッティバイト「攻撃力*2.2のダメージを与え、与えたダメージの７５％ＨＰを回復する！」(吸収攻撃）(MP-35)\n");
							printf("２：クレイジーグラビティ「確率で敵の現在ＨＰを２０％減らす。又は自分のＨＰを１５００回復する。」(gravity20％)(30%)(MP-65)\n");
							printf("３：ファントムスタブ「攻撃力＊3.1のダメージを与え、確率で３ターンの間、スキル不可を付与する。」(SKILL×35％)(MP-50)\n＊：");
							nSkillChoose = 0;//初期化
							while (1)
							{
								scanf("%d", &nSkillChoose);//スキル選択をする。



								switch (nSkillChoose)
								{
								case 1:
									if (g_aPlayer[PlayerNumber1].fMP >= 35)//プレイヤー１のMPが３５以上なら処理を実行する。
									{
										Player1BloodyBite(&g_aPlayer[0]);//プレイヤー１のスキル「ブラッディバイト」を実行する。
									}
									else
									{
										pPlayer->fMP = pPlayer->fMP + 5;//通常攻撃を選択した場合、プレイヤー１のＭＰを５回復する。
										printf("ＭＰが足りない(´・ω・｀)やむをえず通常攻撃！\n");
										getchar();
										Player1NormalAttack(&g_aPlayer[0], Player2CounterFlag, bPlayer1CriticalLineBuff);//スキルが発動できなかった場合に通常攻撃を発動。

									}
									break;

								case 2:
									if (g_aPlayer[PlayerNumber1].fMP >= 65)//プレイヤー１のMPが65以上なら処理を実行する。
									{
										Player1CrazyGracity(&g_aPlayer[0]);//プレイヤー１のスキル「クレイジーグラビティ」を実行する。
									}
									else
									{
										pPlayer->fMP = pPlayer->fMP + 5;//通常攻撃を選択した場合、プレイヤー１のＭＰを５回復する。
										printf("ＭＰが足りない(´・ω・｀)やむをえず通常攻撃！\n");
										getchar();
										Player1NormalAttack(&g_aPlayer[0], Player2CounterFlag, bPlayer1CriticalLineBuff);//スキルが発動できなかった場合に通常攻撃を発動。
									}
									break;

								case 3:
									if (g_aPlayer[PlayerNumber1].fMP >= 50)//プレイヤー１のＭＰが５０以上なら処理を実行する。
									{
										bPlayer2PhantomStabDebuff = Player1PhantomStab(&g_aPlayer[0]);//プレイヤー１のスキル「ファントムスタブ」を発動する。
									}
									else
									{
										pPlayer->fMP = pPlayer->fMP + 5;//通常攻撃を選択した場合、プレイヤー１のＭＰを５回復する。
										printf("ＭＰが足りない(´・ω・｀)やむをえず通常攻撃！\n");
										getchar();
										Player1NormalAttack(&g_aPlayer[0], Player2CounterFlag, bPlayer1CriticalLineBuff);//スキルが発動できなかった場合に通常攻撃を発動。
									}

									break;

								default:
									while (getchar() != '\n');//数字以外を入力したときにエラーメッセージを出す。
									printf("入力された値が違うよ！\n＊：");
									break;
								}

								if (nSkillChoose == 1 || nSkillChoose == 2 || nSkillChoose == 3)
								{
									break;
								}
							}

						}
						else if (g_aPlayer[PlayerNumber1].nCheckJob == 5)//プレイヤー１の選択した職業が「科学者」なら処理を実行する。================================================================
						{
							printf("+---------------------------------------------------------------------------+\n");
							printf("<<使いたいスキルを選んでください>>\n");
							printf("残りMP:%.f\n", g_aPlayer[PlayerNumber1].fMP);
							printf("１：ポイズンミスト「攻撃力*2.7のダメージを与え、確率で３ターンの間、毒状態にする。」(毒35％）(MP-40)\n");
							printf("２：スチームバースト「確率で（自分の攻撃力＋相手の防御力）＊4.5の大ダメージを与える。又は自分に火傷状態を付与する。(MP-60)\n");
							printf("３：イカサマレーザー「敵の攻撃力*2.6のダメージを与え、確率で敵の防御力を３ターンの間、４０％下げる。(DEF↓40％)(MP-45)\n＊：");
							nSkillChoose = 0;//初期化
							while (1)
							{
								scanf("%d", &nSkillChoose);//スキル選択をする。



								switch (nSkillChoose)
								{
								case 1:
									if (g_aPlayer[PlayerNumber1].fMP >= 40)//プレイヤー１のMPが４０以上なら処理を実行する。
									{
										bPlayer2PoisonMistState = Player1PoisonMist(&g_aPlayer[0]);//プレイヤー１のスキル「ポイズンミスト」を実行する。
									}
									else
									{
										pPlayer->fMP = pPlayer->fMP + 5;//通常攻撃を選択した場合、プレイヤー１のＭＰを５回復する。
										printf("ＭＰが足りない(´・ω・｀)やむをえず通常攻撃！\n");
										getchar();
										Player1NormalAttack(&g_aPlayer[0], Player2CounterFlag, bPlayer1CriticalLineBuff);//スキルが発動できなかった場合に通常攻撃を発動。

									}
									break;

								case 2:
									if (g_aPlayer[PlayerNumber1].fMP >= 60)//プレイヤー１のMPが60以上なら処理を実行する。
									{
										bPlayer1SteamBurstBurn = Player1SteamBurst(&g_aPlayer[0]);//プレイヤー１のスキル「スチームバースト」を実行する。
									}
									else
									{
										pPlayer->fMP = pPlayer->fMP + 5;//通常攻撃を選択した場合、プレイヤー１のＭＰを５回復する。
										printf("ＭＰが足りない(´・ω・｀)やむをえず通常攻撃！\n");
										getchar();
										Player1NormalAttack(&g_aPlayer[0], Player2CounterFlag, bPlayer1CriticalLineBuff);//スキルが発動できなかった場合に通常攻撃を発動。
									}
									break;

								case 3:
									if (g_aPlayer[PlayerNumber1].fMP >= 45)//プレイヤー１のＭＰが45以上なら処理を実行する。
									{
										bPlayer2SquitLaserDebuff = Player1SquidLaser(&g_aPlayer[0]);//プレイヤー１のスキル「イカサマレーザー」を発動する。
									}
									else
									{
										pPlayer->fMP = pPlayer->fMP + 5;//通常攻撃を選択した場合、プレイヤー１のＭＰを５回復する。
										printf("ＭＰが足りない(´・ω・｀)やむをえず通常攻撃！\n");
										getchar();
										Player1NormalAttack(&g_aPlayer[0], Player2CounterFlag, bPlayer1CriticalLineBuff);//スキルが発動できなかった場合に通常攻撃を発動。
									}

									break;

								default:
									while (getchar() != '\n');//数字以外を入力したときにエラーメッセージを出す。
									printf("入力された値が違うよ！\n＊：");
									break;
								}

								if (nSkillChoose == 1 || nSkillChoose == 2 || nSkillChoose == 3)
								{
									break;
								}
							}

						}
						else if (g_aPlayer[PlayerNumber1].nCheckJob == 6)//プレイヤー１の選択した職業が「魔法使い」なら処理を実行する。================================================================
						{
							printf("+---------------------------------------------------------------------------+\n");
							printf("<<使いたいスキルを選んでください>>\n");
							printf("残りMP:%.f\n", g_aPlayer[PlayerNumber1].fMP);
							printf("１：フレイムウェーブ「攻撃力*1.1の４連続攻撃をし、確率で２ターンの間、攻撃力を２５％上昇させる。」(MP-35)\n");
							printf("２：スワップマジック「自分の攻撃力と防御力の数値を入れ替える。」(MP-25)\n");
							printf("３：アイスランス「自分の攻撃力*4.5のダメージを与え、確率で５ターンの間、相手のアイテムコマンドを封印する。」(MP-70)\n＊：");
							nSkillChoose = 0;//初期化
							while (1)
							{
								scanf("%d", &nSkillChoose);//スキル選択をする。



								switch (nSkillChoose)
								{
								case 1:
									if (g_aPlayer[PlayerNumber1].fMP >= 35)//プレイヤー１のMPが３５以上なら処理を実行する。
									{
										bPlayer1FlameWaveBuff = Player1FlameWave(&g_aPlayer[0]);//プレイヤー１のスキル「フレイムウェーブ」を実行する。
									}
									else
									{
										pPlayer->fMP = pPlayer->fMP + 5;//通常攻撃を選択した場合、プレイヤー１のＭＰを５回復する。
										printf("ＭＰが足りない(´・ω・｀)やむをえず通常攻撃！\n");
										getchar();
										Player1NormalAttack(&g_aPlayer[0], Player2CounterFlag, bPlayer1CriticalLineBuff);//スキルが発動できなかった場合に通常攻撃を発動。

									}
									break;

								case 2:
									if (g_aPlayer[PlayerNumber1].fMP >= 25)//プレイヤー１のMPが25以上なら処理を実行する。
									{
										printf("%sのスワップマジック！", &pPlayer->aName[0]);
										pPlayer->fMP = pPlayer->fMP - 25;//プレイヤー１のMPを２５減らす。
										//攻撃力と防御力の値を入れ替える。
										pPlayer->fATK = fPlayer1MaxDEF;
										pPlayer->fDEF = fPlayer1MaxATK;

										fPlayer1MaxATK = pPlayer->fATK;
										fPlayer1MaxDEF = pPlayer->fDEF;

										printf("%sの攻撃力と防御力の数値が入れ替わった！\n", &pPlayer->aName[0]);
									}
									else
									{
										pPlayer->fMP = pPlayer->fMP + 5;//通常攻撃を選択した場合、プレイヤー１のＭＰを５回復する。
										printf("ＭＰが足りない(´・ω・｀)やむをえず通常攻撃！\n");
										getchar();
										Player1NormalAttack(&g_aPlayer[0], Player2CounterFlag, bPlayer1CriticalLineBuff);//スキルが発動できなかった場合に通常攻撃を発動。
									}
									break;

								case 3:
									if (g_aPlayer[PlayerNumber1].fMP >= 70)//プレイヤー１のＭＰが70以上なら処理を実行する。
									{
										bPlayer2IceLanceDebuff = Player1IceLance(&g_aPlayer[0]);//プレイヤー１のスキル「アイスランス」を発動する。
									}
									else
									{
										pPlayer->fMP = pPlayer->fMP + 5;//通常攻撃を選択した場合、プレイヤー１のＭＰを５回復する。
										printf("ＭＰが足りない(´・ω・｀)やむをえず通常攻撃！\n");
										getchar();
										Player1NormalAttack(&g_aPlayer[0], Player2CounterFlag, bPlayer1CriticalLineBuff);//スキルが発動できなかった場合に通常攻撃を発動。
									}

									break;

								default:
									while (getchar() != '\n');//数字以外を入力したときにエラーメッセージを出す。
									printf("入力された値が違うよ！\n＊：");
									break;
								}

								if (nSkillChoose == 1 || nSkillChoose == 2 || nSkillChoose == 3)
								{
									break;
								}
							}

						}
					}
					else if (bPlayer1PhantomStabDebuff == true)
					{
						pPlayer->fMP = pPlayer->fMP + 5;//通常攻撃を選択した場合、プレイヤー１のＭＰを５回復する。
						printf("スキルは現在使用できない！(´・ω・｀)やむをえず通常攻撃！\n");
						getchar();
						Player1NormalAttack(&g_aPlayer[0], Player2CounterFlag, bPlayer1CriticalLineBuff);//スキルが発動できなかった場合に通常攻撃を発動。
					}
					rewind(stdin);
					getchar();
					system("cls");//画面をリセットする
					break;

				case 3://アイテムガチャのコマンド
					if (bPlayer1IceLanceDebuff == false)//プレイヤー１がスキル「アイスランス」の特殊効果「アイテム封印」を受けていなかったら処理を実行する。
					{
						if (nPlayer1ItemRouletteUseCount != 0)//アイテム使用回数のカウントが０じゃなかったら
						{
							nPlayer1ItemRouletteUseCount--;//プレイヤー１のアイテム使用回数を１減らす。
							printf("アイテムガチャスタート！\n");
							srand((unsigned int)time(NULL));//乱数の種を現在時刻を基準にランダム生成。
							nPlayer1ItemRouletteRandum = rand() % 16 + 1;//１５項目の中から一つを抽選する。
							getchar();//キー入力待ち

							if (nPlayer1ItemRouletteRandum == 1)
							{
								pPlayer->fLife = pPlayer->fLife + 1200;
								printf("マッシュルームをゲットした！体力を１２００回復する！\n");
							}
							if (nPlayer1ItemRouletteRandum == 2)
							{
								pPlayer->fLife = pPlayer->fLife + 2500;
								printf("スーパーマッシュルームをゲットした！体力を２５００回復する！\n");
							}
							if (nPlayer1ItemRouletteRandum == 3)
							{
								pPlayer->fLife = pPlayer->fLife + 4000;
								printf("ウルトラマッシュルームをゲットした！（レアアイテム☆）体力を４０００回復する！\n");
							}
							if (nPlayer1ItemRouletteRandum == 4)
							{
								pPlayer->fMP = pPlayer->fMP + 30;
								printf("マジックサイダーＳをゲットした！ＭＰを３０回復する！\n");
							}
							if (nPlayer1ItemRouletteRandum == 5)
							{
								pPlayer->fMP = pPlayer->fMP + 60;
								printf("マジックサイダーＭをゲットした！ＭＰを６０回復する！\n");
							}
							if (nPlayer1ItemRouletteRandum == 6)
							{
								pPlayer->fMP = pPlayer->fMP + 100;
								printf("マジックサイダーＬをゲットした！（レアアイテム☆）ＭＰを１００回復する！\n");
							}
							if (nPlayer1ItemRouletteRandum == 7)
							{
								pPlayer->fATK = pPlayer->fATK + (fPlayer1MaxATK * 0.1);
								printf("カレーライスをゲットした！４ターンの間、攻撃力が１０％上昇する！\n");
								bPlayer1CurryRiceBuffLV1 = true;
							}
							if (nPlayer1ItemRouletteRandum == 8)
							{
								pPlayer->fATK = pPlayer->fATK + (fPlayer1MaxATK * 0.3);
								printf("シーフードカレーをゲットした！４ターンの間、攻撃力が３０％上昇する！\n");
								bPlayer1CurryRiceBuffLV2 = true;
							}
							if (nPlayer1ItemRouletteRandum == 9)
							{
								pPlayer->fATK = pPlayer->fATK + (fPlayer1MaxATK * 0.5);
								printf("ポークカレーをゲットした！（レアアイテム☆）４ターンの間、攻撃力が５０％上昇する！\n");
								bPlayer1CurryRiceBuffLV3 = true;
							}
							if (nPlayer1ItemRouletteRandum == 10)
							{
								pPlayer->fDEF = pPlayer->fDEF + (fPlayer1MaxDEF * 0.1);
								printf("塩おむすびをゲットした！４ターンの間、防御力が１０％上昇する！\n");
								bPlayer1RiceBallBuffLV1 = true;
							}
							if (nPlayer1ItemRouletteRandum == 11)
							{
								pPlayer->fDEF = pPlayer->fDEF + (fPlayer1MaxDEF * 0.3);
								printf("鮭おむすびをゲットした！４ターンの間、防御力が３０％上昇する！\n");
								bPlayer1RiceBallBuffLV2 = true;
							}
							if (nPlayer1ItemRouletteRandum == 12)
							{
								pPlayer->fDEF = pPlayer->fDEF + (fPlayer1MaxDEF * 0.5);
								printf("いくらおむすびをゲットした！（レアアイテム☆）４ターンの間、防御力が５０％上昇する！\n");
								bPlayer1RiceBallBuffLV3 = true;
							}
							if (nPlayer1ItemRouletteRandum == 13)
							{
								srand((unsigned int)time(NULL));//乱数の種を現在時刻を基準にランダム生成。
								nShootingStar = rand() % 2500 + 500;//５００～３０００の固定ダメージを与える。
								pPlayer++;//ポインタ先をプレイヤー２へ
								pPlayer->fLife = pPlayer->fLife - nShootingStar;
								printf("相手に流星群が降り注ぐ！%sに%dのダメージ！\n", &pPlayer->aName[0], nShootingStar);
								pPlayer--;//ポインタ先をプレイヤー１へ
							}
							if (nPlayer1ItemRouletteRandum == 14)
							{
								Player1WaveCanon(&g_aPlayer[0]);

							}
							if (nPlayer1ItemRouletteRandum == 15)
							{
								pPlayer++;//ポインタ先をプレイヤー２へ
								printf("衰弱玉を手に入れた！３ターンの間、相手の攻撃力と防御力が４０％減少する！\n");
								pPlayer->fATK = pPlayer->fATK - (fPlayer2MaxATK * 0.4);
								pPlayer->fDEF = pPlayer->fDEF - (fPlayer2MaxDEF * 0.4);
								pPlayer--;//ポインタ先をプレイヤー１へ
								bPlayer2WeakBallDeBuff = true;//デバフが発動したので、フラグをtrueにする。
							}
							if (nPlayer1ItemRouletteRandum == 16)
							{
								printf("下剋上バッジを手に入れた！もし攻撃力が１０００以下、又は防御力が７００以下の場合、永久的に攻撃力と防御力の数値を＋３００する。\nそれ以外なら＋１００する。\n");
								if (pPlayer->fATK <= 1000 || pPlayer->fDEF <= 700)
								{
									pPlayer->fATK = pPlayer->fATK + 300;
									fPlayer1MaxATK = pPlayer->fATK;
									pPlayer->fDEF = pPlayer->fDEF + 300;
									fPlayer1MaxDEF = pPlayer->fDEF;
								}
								else
								{
									pPlayer->fATK = pPlayer->fATK + 100;
									fPlayer1MaxATK = pPlayer->fATK;
									pPlayer->fDEF = pPlayer->fDEF + 100;
									fPlayer1MaxDEF = pPlayer->fDEF;
								}

							}
						}
						else
						{
							printf("アイテムはもう使用できない！やむを得ず通常攻撃！\n");
							pPlayer->fMP = pPlayer->fMP + 5;//通常攻撃を選択した場合、プレイヤー１のＭＰを５回復する。
							getchar();
							Player1NormalAttack(&g_aPlayer[0], Player2CounterFlag, bPlayer1CriticalLineBuff);//スキルが発動できなかった場合に通常攻撃を発動。
						}
					}
					else
					{
						printf("アイテムコマンドは封印されている！やむを得ず通常攻撃！\n");
						pPlayer->fMP = pPlayer->fMP + 5;//通常攻撃を選択した場合、プレイヤー１のＭＰを５回復する。
						getchar();
						Player1NormalAttack(&g_aPlayer[0], Player2CounterFlag, bPlayer1CriticalLineBuff);//スキルが発動できなかった場合に通常攻撃を発動。
					}

					rewind(stdin);
					getchar();
					system("cls");//画面をリセットする
					break;

				default:
					printf("入力された値が違うよ！\n\n");
					rewind(stdin);
					getchar();
					system("cls");//画面をリセットする
					break;
				}

				if (nChoose == 1 || nChoose == 2 || nChoose == 3 || nChoose == 4)
				{
					nTurnAttack = 1;
					break;
				}
			}



		}
		//==========================================================
		//プレイヤー２のターン
		//==========================================================
		else
		{

			while (1)//プレイヤー２のターン処理
			{
				printf("                                   %dターン目\n", nTurn/2 + 1);

				printf("            <<プレイヤー１>>　　       vs　　      <<プレイヤー２>>\n");

				printf("  名前：%16s                      %16s\n", &g_aPlayer[0].aName[0], &g_aPlayer[1].aName[0]);

				printf("  \x1b[32m体力：%16.f                     %16.f\n", g_aPlayer[0].fLife, g_aPlayer[1].fLife);

				printf("\x1b[31m攻撃力：%16.f                     %16.f\n", g_aPlayer[0].fATK, g_aPlayer[1].fATK);

				printf("\x1b[36m防御力：%16.f                     %16.f\n", g_aPlayer[0].fDEF, g_aPlayer[1].fDEF);

				printf("    \x1b[33mMP：%16.f                     %16.f\x1b[0m\n", g_aPlayer[0].fMP, g_aPlayer[1].fMP);

				printf("+<<プレイヤー２のターン>>\n");
				printf("  %sはどうする？                       \n", &g_aPlayer[1].aName[0]);
				printf("+-------------------------------------+\n");
				printf("|  １：攻撃\x1b[0m　　　　　　　　　　　　　 |\n");
				printf("|  ２：スキルアタック\x1b[0m                 |\n");
				printf("|  ３：アイテムを使う(残り%d回)\x1b[0m        |\n", nPlayer2ItemRouletteUseCount);
				printf("|  ４：何もしない\x1b[0m                     |\n");
				printf("+-------------------------------------+\n");
				printf("＊：");
				nChoose = 0;//初期化
				scanf("%d", &nChoose);

				switch (nChoose)
				{
				case 4://何もしない
					pPlayer++;//ポインタをプレイヤー２へ
					pPlayer->fMP = pPlayer->fMP + 10;
					printf("%sは様子を見ている！", &pPlayer->aName[0]);
					pPlayer--;//ポインタをプレイヤー１へ

					rewind(stdin);
					getchar();
					system("cls");//画面をリセットする。
					break;

				case 1://通常攻撃
					pPlayer++;//ポインタをプレイヤー２へ
					pPlayer->fMP = pPlayer->fMP + 5;//通常攻撃発動時にプレイヤー２のＭＰを５回復する。

					Player2NormalAttack(&g_aPlayer[0], Player1CounterFlag, bPlayer2CriticalLineBuff);
					pPlayer--;//ポインタをプレイヤー１へ
					rewind(stdin);
					getchar();
					system("cls");//画面をリセットする。
					break;

				case 2://スキル攻撃
					if (bPlayer2PhantomStabDebuff == false)//状態異常「スキル不可」が発動していなかった場合、処理を実行する。
					{
						if (g_aPlayer[1].nCheckJob == 1)//プレイヤー２の選択した職業が「剣士」なら処理を実行する。==============================================
						{
							printf("+---------------------------------------------------------------------------+\n");
							printf("<<使いたいスキルを選んでください>>\n");
							printf("残りMP:%.f\n", g_aPlayer[PlayerNumber2].fMP);
							printf("１：インパクトエッジ「攻撃力*3ののダメージを与える。」(MP-30)\n");
							printf("２：リカバーヒール「最大ＨＰの２５％分のＨＰを回復する」(MP-80)\n");
							printf("３：カウンタースタイル「３ターンの間、確率で通常攻撃をカウンターをする。」(MP-40)\n＊：");
							nSkillChoose = 0;//初期化
							while (1)//「剣士」の技選択
							{
								scanf("%d", &nSkillChoose);//スキル選択をする。



								switch (nSkillChoose)
								{
								case 1:
									if (g_aPlayer[PlayerNumber2].fMP >= 30)
									{
										Player2InpactEdge(&g_aPlayer[0]);//プレイヤー１のスキル「インパクトエッジ」を実行する。
									}
									else
									{
										pPlayer++;//ポインタをぷれいやー２へ　
										pPlayer->fMP = pPlayer->fMP + 5;//通常攻撃発動時にプレイヤー２のＭＰを５回復する。
										printf("MPが足りない(´・ω・`)やむを得ず通常攻撃！\n");
										getchar();
										Player2NormalAttack(&g_aPlayer[0], Player1CounterFlag, bPlayer2CriticalLineBuff);//スキルが発動できなかった場合に通常攻撃を発動。
										pPlayer--;//ポインタをプレイヤー１へ
									}
									break;

								case 2:
									if (g_aPlayer[PlayerNumber2].fMP >= 65)//プレイヤー２のMPが65以上なら処理を実行す	る。
									{
										Player2RecoverHeal(&g_aPlayer[0], fPlayer2MaxHp);//プレイヤー2のスキル「クレイジーグラビティ」を実行する。
									}
									else
									{
										pPlayer++;//ポインタをプレイヤー２へ
										pPlayer->fMP = pPlayer->fMP + 5;//通常攻撃発動時にプレイヤー２のＭＰを５回復する。
										printf("ＭＰが足りない(´・ω・｀)やむをえず通常攻撃！\n");
										getchar();
										Player2NormalAttack(&g_aPlayer[0], Player1CounterFlag, bPlayer2CriticalLineBuff);//スキルが発動できなかった場合に通常攻撃を発動。
										pPlayer--;//ポインタをプレイヤー１へ
									}
									break;

								case 3:
									if (g_aPlayer[PlayerNumber2].fMP >= 40)
									{
										pPlayer++;//ポインタをプレイヤー２へ
										pPlayer->fMP = pPlayer->fMP - 40;
										printf("%sはカウンターの姿勢をとった！\n", &g_aPlayer[PlayerNumber2].aName[0]);//プレイヤー２の技：「カウンタースタイル」
										Player2CounterFlag = true;
										pPlayer--;
									}
									else
									{
										pPlayer++;//ポインタをプレイヤー２へ
										pPlayer->fMP = pPlayer->fMP + 5;//通常攻撃発動時にプレイヤー２のＭＰを５回復する。
										printf("ＭＰが足りない(´・ω・｀)やむをえず通常攻撃！\n");
										getchar();
										Player2NormalAttack(&g_aPlayer[0], Player1CounterFlag, bPlayer2CriticalLineBuff);//スキルが発動できなかった場合に通常攻撃を発動。
										pPlayer--;//ポインタをプレイヤー１へ
									}
									break;

								default:
									while (getchar() != '\n');//数字以外を入力したときにエラーメッセージを出す。
									printf("入力された値が違うよ！\n＊：");
									break;
								}

								if (nSkillChoose == 1 || nSkillChoose == 2 || nSkillChoose == 3)
								{
									break;
								}
							}

						}
						else if (g_aPlayer[PlayerNumber2].nCheckJob == 2)//プレイヤー2の選択した職業が「狩人」なら処理を実行する。======================================================================
						{
							printf("+---------------------------------------------------------------------------+\n");
							printf("<<使いたいスキルを選んでください>>\n");
							printf("残りMP:%.f\n", g_aPlayer[PlayerNumber2].fMP);
							printf("１：レインアロー「攻撃力＊０．５の８連続j攻撃！」（命中率４０％）(MP-25)\n");
							printf("２：パワダウンアロー「攻撃力*2のダメージを与え、確率で２ターン攻撃力ダウン」(ATK↓50%)(デバフ確率４０％）(MP-40)\n");
							printf("３：スターアロー「敵に自分の攻撃力*5の防御無視の攻撃を行う！」(MP-60)\n＊：");
							nSkillChoose = 0;//初期化
							while (1)
							{
								scanf("%d", &nSkillChoose);//スキル選択をする。



								switch (nSkillChoose)
								{
								case 1:
									if (g_aPlayer[PlayerNumber2].fMP >= 25)//プレイヤー2のMPが25以上なら処理を実行する。
									{
										Player2RainArrow(&g_aPlayer[0]);//プレイヤー2のスキル「レインアロー」を実行する。
									}
									else
									{
										pPlayer++;//ポインタをプレイヤー２へ
										pPlayer->fMP = pPlayer->fMP + 5;//通常攻撃を選択した場合、プレイヤー2のＭＰを５回復する。
										printf("ＭＰが足りない(´・ω・｀)やむをえず通常攻撃！\n");
										getchar();
										Player2NormalAttack(&g_aPlayer[0], Player1CounterFlag, bPlayer2CriticalLineBuff);//スキルが発動できなかった場合に通常攻撃を発動。
										pPlayer--;//ポインタ先をプレイヤー１へ

									}
									break;

								case 2:
									if (g_aPlayer[PlayerNumber2].fMP >= 40)//プレイヤー2のMPが４０以上なら処理を実行する。
									{
										Player1PowerDownArrowDebuff = Player2PowerDownArrow(&g_aPlayer[0]);//プレイヤー2のスキル「パワダウンアロー」を実行する。
									}
									else
									{
										pPlayer++;//ポインタ先をプレイヤー２へ
										pPlayer->fMP = pPlayer->fMP + 5;//通常攻撃を選択した場合、プレイヤー2のＭＰを５回復する。
										printf("ＭＰが足りない(´・ω・｀)やむをえず通常攻撃！\n");
										getchar();
										Player2NormalAttack(&g_aPlayer[0], Player1CounterFlag, bPlayer2CriticalLineBuff);//スキルが発動できなかった場合に通常攻撃を発動。
										pPlayer--;//ポインタ先をプレイヤー１へ
									}
									break;

								case 3:
									if (g_aPlayer[PlayerNumber2].fMP >= 60)//プレイヤー２のMPが６０以上なら処理を実行する。
									{
										Player2StarArrow(&g_aPlayer[0]);//プレイヤー２のスキル「スターアロー」を実行する。
									}
									else
									{
										pPlayer++;//ポインタ先をプレイヤー２へ
										pPlayer->fMP = pPlayer->fMP + 5;//通常攻撃を選択した場合、プレイヤー１のＭＰを５回復する。
										printf("ＭＰが足りない(´・ω・｀)やむをえず通常攻撃！\n");
										getchar();
										Player2NormalAttack(&g_aPlayer[0], Player1CounterFlag, bPlayer2CriticalLineBuff);//スキルが発動できなかった場合に通常攻撃を発動。
										pPlayer--;//ポインタ先をプレイヤー１へ
									}

									break;

								default:
									while (getchar() != '\n');//数字以外を入力したときにエラーメッセージを出す。
									printf("入力された値が違うよ！\n＊：");
									break;
								}

								if (nSkillChoose == 1 || nSkillChoose == 2 || nSkillChoose == 3)
								{
									break;
								}
							}

						}
						else if (g_aPlayer[PlayerNumber2].nCheckJob == 3)//プレイヤー２の選択した職業が「巫女」なら処理を実行する。======================================================================
						{
							printf("+---------------------------------------------------------------------------+\n");
							printf("<<使いたいスキルを選んでください>>\n");
							printf("残りMP:%.f\n", g_aPlayer[PlayerNumber2].fMP);
							printf("１：守りの陣「５ターンの間、防御力を上昇させる。」（DEF↑40％）(MP-35)\n");
							printf("２：ホーリーレイ「攻撃力+防御力＊1.2のダメージを与え、相手のＭＰを１５減らす。」(ＭＰダメージ)(MP-45)\n");
							printf("３：会心の陣「３ターンの間、通常攻撃が確率でクリティカルになる。」（CRT↑35％）(MP-55)\n＊：");
							nSkillChoose = 0;//初期化
							while (1)
							{
								scanf("%d", &nSkillChoose);//スキル選択をする。



								switch (nSkillChoose)
								{
								case 1:
									if (g_aPlayer[PlayerNumber2].fMP >= 35 && bPlayer2DefenceLineBuff == false)//プレイヤー２のMPが35以上かつ、守りの陣が発動していない場合に処理を実行する。
									{
										Player2DefenceLine(&g_aPlayer[0]);//プレイヤー２のスキル「守りの陣」を実行する。
										bPlayer2DefenceLineBuff = true;//スキルのバフが発動したのでフラグをtrueにする。
									}
									else if (bPlayer2DefenceLineBuff == true)
									{
										printf("守りの陣は発動中だ！　やむを得ず通常攻撃！\n");
										getchar();
										Player2NormalAttack(&g_aPlayer[0], Player1CounterFlag, bPlayer2CriticalLineBuff);//スキルが発動できなかった場合に通常攻撃を発動。
									}
									else
									{
										pPlayer++;//ポインタ先をプレイヤー２へ
										pPlayer->fMP = pPlayer->fMP + 5;//通常攻撃を選択した場合、プレイヤー１のＭＰを５回復する。
										printf("ＭＰが足りない(´・ω・｀)やむをえず通常攻撃！\n");
										getchar();
										Player2NormalAttack(&g_aPlayer[0], Player1CounterFlag, bPlayer2CriticalLineBuff);//スキルが発動できなかった場合に通常攻撃を発動。
										pPlayer--;//ポインタ先をプレイヤー１へ

									}
									break;

								case 2:
									if (g_aPlayer[PlayerNumber2].fMP >= 45)//プレイヤー２のMPが45以上なら処理を実行する。
									{
										Player2HolyRay(&g_aPlayer[0]);//プレイヤー２のスキル「ホーリーレイ」を実行する。
									}
									else
									{
										pPlayer++;//ポインタ先をプレイヤー２へ
										pPlayer->fMP = pPlayer->fMP + 5;//通常攻撃を選択した場合、プレイヤー１のＭＰを５回復する。
										printf("ＭＰが足りない(´・ω・｀)やむをえず通常攻撃！\n");
										getchar();
										Player2NormalAttack(&g_aPlayer[0], Player1CounterFlag, bPlayer2CriticalLineBuff);//スキルが発動できなかった場合に通常攻撃を発動。
										pPlayer--;//ポインタ先をプレイヤー１へ
									}
									break;

								case 3:
									if (g_aPlayer[PlayerNumber2].fMP >= 55)//プレイヤー２のMPが５５以上なら処理を実行する。
									{
										pPlayer++;//ポインタ先をプレイヤー２へ
										pPlayer->fMP = pPlayer->fMP - 55;//ＭＰを５５減らす。
										printf("%sの会心の陣！\n", &g_aPlayer[PlayerNumber2].aName[0]);//プレイヤー２のスキル「会心の陣」を発動する。 
										bPlayer2CriticalLineBuff = true;//バフを受けていることを表すフラグをtrueにする。
										printf("３ターンの間、%sの通常攻撃のクリティカル率が上昇する！\n", &g_aPlayer[PlayerNumber2].aName[0]);
										pPlayer--;//ポインタ先をプレイヤー１へ
									}
									else if (bPlayer1CriticalLineBuff == true)
									{
										printf("会心の陣は発動中だ！　やむを得ず通常攻撃！\n");
										getchar();
										Player2NormalAttack(&g_aPlayer[0], Player1CounterFlag, bPlayer2CriticalLineBuff);//スキルが発動できなかった場合に通常攻撃を発動。
									}
									else
									{
										pPlayer++;//ポインタ先をプレイヤー２へ
										pPlayer->fMP = pPlayer->fMP + 5;//通常攻撃を選択した場合、プレイヤー１のＭＰを５回復する。
										printf("ＭＰが足りない(´・ω・｀)やむをえず通常攻撃！\n");
										getchar();
										Player2NormalAttack(&g_aPlayer[0], Player1CounterFlag, bPlayer2CriticalLineBuff);//スキルが発動できなかった場合に通常攻撃を発動。
										pPlayer--;//ポインタ先をプレイヤー１へ
									}
									break;

								default:
									while (getchar() != '\n');//数字以外を入力したときにエラーメッセージを出す。
									printf("入力された値が違うよ！\n＊：");
									break;
								}

								if (nSkillChoose == 1 || nSkillChoose == 2 || nSkillChoose == 3)
								{
									break;
								}
							}

						}
						else if (g_aPlayer[PlayerNumber2].nCheckJob == 4)//プレイヤー２の選択した職業が「吸血鬼」なら処理を実行する。================================================================
						{
							printf("+---------------------------------------------------------------------------+\n");
							printf("<<使いたいスキルを選んでください>>\n");
							printf("残りMP:%.f\n", g_aPlayer[PlayerNumber2].fMP);
							printf("１：ブラッティバイト「攻撃力*2.2のダメージを与え、与えたダメージの７５％ＨＰを回復する！」(吸収攻撃）(MP-35)\n");
							printf("２：クレイジーグラビティ「確率で敵の現在ＨＰを２０％減らす。又は自分のＨＰを１５００回復する。」(MP-65)\n");
							printf("３：ファントムスタブ「攻撃力＊3.1のダメージを与え、確率で３ターンの間、スキル不可を付与する。」(SKILL×35％)(MP-50)\n＊：");
							nSkillChoose = 0;//初期化
							while (1)
							{
								scanf("%d", &nSkillChoose);//スキル選択をする。



								switch (nSkillChoose)
								{
								case 1:
									if (g_aPlayer[PlayerNumber2].fMP >= 35)//プレイヤー２のMPが３５以上なら処理を実行する。
									{
										Player2BloodyBite(&g_aPlayer[0]);//プレイヤー２のスキル「ブラッディバイト」を実行する。
									}
									else
									{
										pPlayer++;//ポインタ先をプレイヤー２へ
										pPlayer->fMP = pPlayer->fMP + 5;//通常攻撃を選択した場合、プレイヤー２のＭＰを５回復する。
										printf("ＭＰが足りない(´・ω・｀)やむをえず通常攻撃！\n");
										getchar();
										Player2NormalAttack(&g_aPlayer[0], Player1CounterFlag, bPlayer2CriticalLineBuff);//スキルが発動できなかった場合に通常攻撃を発動。
										pPlayer--;//ポインタ先をプレイヤー１へ
									}
									break;

								case 2:
									if (g_aPlayer[PlayerNumber2].fMP >= 65)//プレイヤー２のMPが65以上なら処理を実行する。
									{
										Player2CrazyGravity(&g_aPlayer[0]);//プレイヤー２のスキル「クレイジーグラビティ」を実行する。
									}
									else
									{
										pPlayer++;//ポインタ先をプレイヤー２へ
										pPlayer->fMP = pPlayer->fMP + 5;//通常攻撃を選択した場合、プレイヤー２のＭＰを５回復する。
										printf("ＭＰが足りない(´・ω・｀)やむをえず通常攻撃！\n");
										getchar();
										Player2NormalAttack(&g_aPlayer[0], Player1CounterFlag, bPlayer2CriticalLineBuff);//スキルが発動できなかった場合に通常攻撃を発動。
										pPlayer--;//ポインタ先をプレイヤー１へ
									}
									break;

								case 3:
									if (g_aPlayer[PlayerNumber2].fMP >= 50)//プレイヤー２のＭＰが５０以上なら処理を実行する。
									{
										bPlayer1PhantomStabDebuff = Player2PhantomStab(&g_aPlayer[0]);//プレイヤー２のスキル「ファントムスタブ」を発動する。
									}
									else
									{
										pPlayer++;//ポインタ先をプレイヤー２へ
										pPlayer->fMP = pPlayer->fMP + 5;//通常攻撃を選択した場合、プレイヤー２のＭＰを５回復する。
										printf("ＭＰが足りない(´・ω・｀)やむをえず通常攻撃！\n");
										getchar();
										Player2NormalAttack(&g_aPlayer[0], Player1CounterFlag, bPlayer2CriticalLineBuff);//スキルが発動できなかった場合に通常攻撃を発動。
										pPlayer--;//ポインタ先をプレイヤー１へ
									}

									break;

								default:
									while (getchar() != '\n');//数字以外を入力したときにエラーメッセージを出す。
									printf("入力された値が違うよ！\n＊：");
									break;
								}

								if (nSkillChoose == 1 || nSkillChoose == 2 || nSkillChoose == 3)
								{
									break;
								}
							}

						}
						else if (g_aPlayer[PlayerNumber2].nCheckJob == 5)//プレイヤー２の選択した職業が「科学者」なら処理を実行する。================================================================
						{
							printf("+---------------------------------------------------------------------------+\n");
							printf("<<使いたいスキルを選んでください>>\n");
							printf("残りMP:%.f\n", g_aPlayer[PlayerNumber2].fMP);
							printf("１：ポイズンミスト「攻撃力*2.7のダメージを与え、確率で３ターンの間、毒状態にする。」(毒35％）(MP-40)\n");
							printf("２：スチームバースト「確率で（自分の攻撃力＋相手の防御力）＊4.5の大ダメージを与える。又は自分に火傷状態を付与する。(MP-60)\n");
							printf("３：イカサマレーザー「敵の攻撃力*2.6のダメージを与え、確率で敵の防御力を３ターンの間、４０％下げる。(DEF↓40％)(MP-45)\n＊：");
							nSkillChoose = 0;//初期化
							while (1)
							{
								scanf("%d", &nSkillChoose);//スキル選択をする。



								switch (nSkillChoose)
								{
								case 1:
									if (g_aPlayer[PlayerNumber2].fMP >= 40)//プレイヤー２のMPが４０以上なら処理を実行する。
									{
										bPlayer1PoisonMistState = Player2PoisonMist(&g_aPlayer[0]);//プレイヤー２のスキル「ポイズンミスト」を実行する。
									}
									else
									{
										pPlayer++;//ポインタ先をプレイヤー２へ
										pPlayer->fMP = pPlayer->fMP + 5;//通常攻撃を選択した場合、プレイヤー２のＭＰを５回復する。
										printf("ＭＰが足りない(´・ω・｀)やむをえず通常攻撃！\n");
										getchar();
										Player2NormalAttack(&g_aPlayer[0], Player1CounterFlag, bPlayer2CriticalLineBuff);//スキルが発動できなかった場合に通常攻撃を発動。
										pPlayer--;//ポインタ先をプレイヤー１へ

									}
									break;

								case 2:
									if (g_aPlayer[PlayerNumber2].fMP >= 60)//プレイヤー２のMPが60以上なら処理を実行する。
									{
										bPlayer2SteamBurstBurn = Player2SteamBurst(&g_aPlayer[0]);//プレイヤー２のスキル「スチームバースト」を実行する。
									}
									else
									{
										pPlayer++;//ポインタ先をプレイヤー２へ
										pPlayer->fMP = pPlayer->fMP + 5;//通常攻撃を選択した場合、プレイヤー２のＭＰを５回復する。
										printf("ＭＰが足りない(´・ω・｀)やむをえず通常攻撃！\n");
										getchar();
										Player2NormalAttack(&g_aPlayer[0], Player1CounterFlag, bPlayer2CriticalLineBuff);//スキルが発動できなかった場合に通常攻撃を発動。
										pPlayer--;//ポインタ先をプレイヤー１へ
									}
									break;

								case 3:
									if (g_aPlayer[PlayerNumber2].fMP >= 45)//プレイヤー２のＭＰが４５以上なら処理を実行する。
									{
										bPlayer1SquitLaserDebuff = Player2SquidLaser(&g_aPlayer[0]);//プレイヤー２のスキル「イカサマレーザー」を発動する。
									}
									else
									{
										pPlayer++;//ポインタ先をプレイヤー２へ
										pPlayer->fMP = pPlayer->fMP + 5;//通常攻撃を選択した場合、プレイヤー２のＭＰを５回復する。
										pPlayer--;//ポインタ先をプレイヤー１へ
										printf("ＭＰが足りない(´・ω・｀)やむをえず通常攻撃！\n");
										getchar();
										Player2NormalAttack(&g_aPlayer[0], Player1CounterFlag, bPlayer2CriticalLineBuff);//スキルが発動できなかった場合に通常攻撃を発動。
									}

									break;

								default:
									while (getchar() != '\n');//数字以外を入力したときにエラーメッセージを出す。
									printf("入力された値が違うよ！\n＊：");
									break;
								}

								if (nSkillChoose == 1 || nSkillChoose == 2 || nSkillChoose == 3)
								{
									break;
								}
							}

						}
						else if (g_aPlayer[PlayerNumber2].nCheckJob == 6)//プレイヤー２の選択した職業が「魔法使い」なら処理を実行する。================================================================
						{
							printf("+---------------------------------------------------------------------------+\n");
							printf("<<使いたいスキルを選んでください>>\n");
							printf("残りMP:%.f\n", g_aPlayer[PlayerNumber2].fMP);
							printf("１：フレイムウェーブ「攻撃力*1.1の４連続攻撃をし、確率で２ターンの間、攻撃力を２５％上昇させる。」(MP-35)\n");
							printf("２：スワップマジック「自分の攻撃力と防御力の数値を入れ替える。」(MP-25)\n");
							printf("３：アイスランス「自分の攻撃力*4.5のダメージを与え、確率で５ターンの間、相手のアイテムコマンドを封印する。」(MP-70)\n＊：");
							nSkillChoose = 0;//初期化
							while (1)
							{
								scanf("%d", &nSkillChoose);//スキル選択をする。



								switch (nSkillChoose)
								{
								case 1:
									if (g_aPlayer[PlayerNumber2].fMP >= 35)//プレイヤー２のMPが３５以上なら処理を実行する。
									{
										bPlayer2FlameWaveBuff = Player2FlameWave(&g_aPlayer[0]);//プレイヤー２のスキル「フレイムウェーブ」を実行する。
									}
									else
									{
										pPlayer++;//ポインタ先をプレイヤー２へ
										pPlayer->fMP = pPlayer->fMP + 5;//通常攻撃を選択した場合、プレイヤー２のＭＰを５回復する。
										printf("ＭＰが足りない(´・ω・｀)やむをえず通常攻撃！\n");
										getchar();
										Player2NormalAttack(&g_aPlayer[0], Player1CounterFlag, bPlayer2CriticalLineBuff);//スキルが発動できなかった場合に通常攻撃を発動。
										pPlayer--;//ポインタ先をプレイヤー１へ

									}
									break;

								case 2:
									if (g_aPlayer[PlayerNumber2].fMP >= 25)//プレイヤー２のMPが25以上なら処理を実行する。
									{
										//ポインタ先をプレイヤー２へ
										pPlayer++;
										printf("%sのスワップマジック！", &pPlayer->aName[0]);
										pPlayer->fMP = pPlayer->fMP - 25;//プレイヤー１のMPを２５減らす。
										//攻撃力と防御力の値を入れ替える。
										pPlayer->fATK = fPlayer2MaxDEF;
										pPlayer->fDEF = fPlayer2MaxATK;

										fPlayer2MaxATK = pPlayer->fATK;
										fPlayer2MaxDEF = pPlayer->fDEF;

										printf("%sの攻撃力と防御力の数値が入れ替わった！\n", &pPlayer->aName[0]);
										pPlayer--;//ポインタ先をプレイヤー１へ
									}
									else
									{
										pPlayer++;//ポインタ先をプレイヤー２へ
										pPlayer->fMP = pPlayer->fMP + 5;//通常攻撃を選択した場合、プレイヤー２のＭＰを５回復する。
										printf("ＭＰが足りない(´・ω・｀)やむをえず通常攻撃！\n");
										getchar();
										Player2NormalAttack(&g_aPlayer[0], Player1CounterFlag, bPlayer2CriticalLineBuff);//スキルが発動できなかった場合に通常攻撃を発動。
										pPlayer--;//ポインタ先をプレイヤー１へ
									}
									break;

								case 3:
									if (g_aPlayer[PlayerNumber2].fMP >= 70)//プレイヤー２のＭＰが70以上なら処理を実行する。
									{
										bPlayer1IceLanceDebuff = Player2IceLance(&g_aPlayer[0]);//プレイヤー２のスキル「アイスランス」を発動する。
									}
									else
									{
										pPlayer++;//ポインタ先をプレイヤー２へ
										pPlayer->fMP = pPlayer->fMP + 5;//通常攻撃を選択した場合、プレイヤー１のＭＰを５回復する。
										printf("ＭＰが足りない(´・ω・｀)やむをえず通常攻撃！\n");
										getchar();
										Player2NormalAttack(&g_aPlayer[0], Player1CounterFlag, bPlayer2CriticalLineBuff);//スキルが発動できなかった場合に通常攻撃を発動。
										pPlayer--;//ポインタ先をプレイヤー１へ
									}

									break;

								default:
									while (getchar() != '\n');//数字以外を入力したときにエラーメッセージを出す。
									printf("入力された値が違うよ！\n＊：");
									break;
								}

								if (nSkillChoose == 1 || nSkillChoose == 2 || nSkillChoose == 3)
								{
									break;
								}
							}

						}
					}
					else if (bPlayer2PhantomStabDebuff == true)//ファントムスタブの「スキル不可」が発動していた場合
					{
						pPlayer++;//ポインタ先をプレイヤー２へ
						pPlayer->fMP = pPlayer->fMP + 5;//通常攻撃を選択した場合、プレイヤー２のＭＰを５回復する。
						printf("スキルは現在使用できない！(´・ω・｀)やむをえず通常攻撃！\n");
						getchar();
						Player2NormalAttack(&g_aPlayer[0], Player1CounterFlag, bPlayer2CriticalLineBuff);//スキルが発動できなかった場合に通常攻撃を発動。
						pPlayer--;//ポインタ先をプレイヤー１へ
					}

					rewind(stdin);
					getchar();
					system("cls");//画面をリセットする。
					break;

				case 3://アイテムを使う

					if (bPlayer2IceLanceDebuff == false)///プレイヤー２がスキル「アイスランス」のアイテム封印のデバフを受けていない場合に処理をする。
					{

						if (nPlayer2ItemRouletteUseCount != 0)//アイテム使用回数のカウントが０じゃなかったら
						{
							nPlayer2ItemRouletteUseCount--;//アイテム使用回数を１減らす。
							printf("アイテムガチャスタート！\n");
							srand((unsigned int)time(NULL));//乱数の種を現在時刻を基準にランダム生成。
							nPlayer2ItemRouletteRandum = rand() % 16 + 1;//１５項目の中から一つを抽選する。
							getchar();//キー入力待ち

							if (nPlayer2ItemRouletteRandum == 1)
							{
								pPlayer++;//ポインタ先をプレイヤー２へ
								pPlayer->fLife = pPlayer->fLife + 1200;
								printf("マッシュルームをゲットした！体力を１２００回復する！\n");
								pPlayer--;//ポインタ先をプレイヤー１へ
							}
							if (nPlayer2ItemRouletteRandum == 2)
							{
								pPlayer++;//ポインタ先をプレイヤー２へ
								pPlayer->fLife = pPlayer->fLife + 2500;
								printf("スーパーマッシュルームをゲットした！体力を２５００回復する！\n");
								pPlayer--;//ポインタ先をプレイヤー１へ
							}
							if (nPlayer2ItemRouletteRandum == 3)
							{
								pPlayer++;//ポインタ先をプレイヤー２へ
								pPlayer->fLife = pPlayer->fLife + 4000;
								printf("ウルトラマッシュルームをゲットした！（レアアイテム☆）体力を４０００回復する！\n");
								pPlayer--;//ポインタ先をプレイヤー１へ
							}
							if (nPlayer2ItemRouletteRandum == 4)
							{
								pPlayer++;//ポインタ先をプレイヤー２へ
								pPlayer->fMP = pPlayer->fMP + 30;
								printf("マジックサイダーＳをゲットした！ＭＰを３０回復する！\n");
								pPlayer--;//ポインタ先をプレイヤー１へ
							}
							if (nPlayer2ItemRouletteRandum == 5)
							{
								pPlayer++;//ポインタ先をプレイヤー２へ
								pPlayer->fMP = pPlayer->fMP + 60;
								printf("マジックサイダーＭをゲットした！ＭＰを６０回復する！\n");
								pPlayer--;//ポインタ先をプレイヤー１へ
							}
							if (nPlayer2ItemRouletteRandum == 6)
							{
								pPlayer++;//ポインタ先をプレイヤー２へ
								pPlayer->fMP = pPlayer->fMP + 100;
								printf("マジックサイダーＬをゲットした！（レアアイテム☆）ＭＰを１００回復する！\n");
								pPlayer--;//ポインタ先をプレイヤー１へ
							}
							if (nPlayer2ItemRouletteRandum == 7)
							{
								pPlayer++;//ポインタ先をプレイヤー２へ
								pPlayer->fATK = pPlayer->fATK + (fPlayer2MaxATK * 0.1);
								printf("カレーライスをゲットした！４ターンの間、攻撃力が１０％上昇する！\n");
								bPlayer2CurryRiceBuffLV1 = true;
								pPlayer--;//ポインタ先をプレイヤー１へ
							}
							if (nPlayer2ItemRouletteRandum == 8)
							{
								pPlayer++;//ポインタ先をプレイヤー２へ
								pPlayer->fATK = pPlayer->fATK + (fPlayer2MaxATK * 0.3);
								printf("シーフードカレーをゲットした！４ターンの間、攻撃力が３０％上昇する！\n");
								bPlayer2CurryRiceBuffLV2 = true;
								pPlayer--;//ポインタ先をプレイヤー１へ
							}
							if (nPlayer2ItemRouletteRandum == 9)
							{
								pPlayer++;//ポインタ先をプレイヤー２へ
								pPlayer->fATK = pPlayer->fATK + (fPlayer2MaxATK * 0.5);
								printf("ポークカレーをゲットした！（レアアイテム☆）４ターンの間、攻撃力が５０％上昇する！\n");
								bPlayer2CurryRiceBuffLV3 = true;
								pPlayer--;//ポインタ先をプレイヤー１へ
							}
							if (nPlayer2ItemRouletteRandum == 10)
							{
								pPlayer++;//ポインタ先をプレイヤー２へ
								pPlayer->fDEF = pPlayer->fDEF + (fPlayer2MaxDEF * 0.1);
								printf("塩おむすびをゲットした！４ターンの間、防御力が１０％上昇する！\n");
								bPlayer2RiceBallBuffLV1 = true;
								pPlayer--;//ポインタ先をプレイヤー１へ
							}
							if (nPlayer2ItemRouletteRandum == 11)
							{
								pPlayer++;//ポインタ先をプレイヤー２へ
								pPlayer->fDEF = pPlayer->fDEF + (fPlayer2MaxDEF * 0.3);
								printf("鮭おむすびをゲットした！４ターンの間、防御力が３０％上昇する！\n");
								bPlayer2RiceBallBuffLV2 = true;
								pPlayer--;//ポインタ先をプレイヤー１へ
							}
							if (nPlayer2ItemRouletteRandum == 12)
							{
								pPlayer++;//ポインタ先をプレイヤー２へ
								pPlayer->fDEF = pPlayer->fDEF + (fPlayer2MaxDEF * 0.5);
								printf("いくらおむすびをゲットした！（レアアイテム☆）４ターンの間、防御力が５０％上昇する！\n");
								bPlayer2RiceBallBuffLV3 = true;
								pPlayer--;//ポインタ先をプレイヤー１へ
							}
							if (nPlayer2ItemRouletteRandum == 13)
							{
								srand((unsigned int)time(NULL));//乱数の種を現在時刻を基準にランダム生成。
								nShootingStar = rand() % 2500 + 500;//５００～３０００の固定ダメージを与える。
								pPlayer->fLife = pPlayer->fLife - nShootingStar;
								printf("相手に流星群が降り注ぐ！%sに%dのダメージ！\n", &pPlayer->aName[0], nShootingStar);

							}
							if (nPlayer2ItemRouletteRandum == 14)
							{
								Player2WaveCanon(&g_aPlayer[0]);//プレイヤー２が波動砲を発動！
							}
							if (nPlayer2ItemRouletteRandum == 15)
							{
								printf("衰弱玉を手に入れた！（レアアイテム☆）３ターンの間、相手の攻撃力と防御力が４０％減少する！\n");
								pPlayer->fATK = pPlayer->fATK - (fPlayer1MaxATK * 0.4);
								pPlayer->fDEF = pPlayer->fDEF - (fPlayer1MaxDEF * 0.4);
								bPlayer1WeakBallDeBuff = true;//デバフが発動したので、フラグをtrueにする。
							}
							if (nPlayer2ItemRouletteRandum == 16)
							{
								printf("下剋上バッジを手に入れた！もし攻撃力が１０００以下、又は防御力が７００以下の場合、永久的に攻撃力と防御力の数値を＋３００する。\nそれ以外なら＋１００する。\n");
								pPlayer++;//ポインタ先をプレイヤー２へ
								if (pPlayer->fATK <= 1000 || pPlayer->fDEF <=700)
								{
									pPlayer->fATK = pPlayer->fATK + 300;
									fPlayer2MaxATK = pPlayer->fATK;
									pPlayer->fDEF = pPlayer->fDEF + 300;
									fPlayer2MaxDEF = pPlayer->fDEF;
								}
								else
								{
									pPlayer->fATK = pPlayer->fATK + 100;
									fPlayer2MaxATK = pPlayer->fATK;
									pPlayer->fDEF = pPlayer->fDEF + 100;
									fPlayer2MaxDEF = pPlayer->fDEF;
								}
								pPlayer--;//ポインタ先をプレイヤー１へ
							}
						}
						else
						{
							printf("アイテムはもう使用できない！やむを得ず通常攻撃！");
							pPlayer++;//ポインタ先をプレイヤー２へ
							pPlayer->fMP = pPlayer->fMP + 5;//通常攻撃を選択した場合、プレイヤー２のＭＰを５回復する。
							pPlayer--;//ポインタ先をプレイヤー１へ
							getchar();
							Player2NormalAttack(&g_aPlayer[0], Player1CounterFlag, bPlayer2CriticalLineBuff);//スキルが発動できなかった場合に通常攻撃を発動。
						}
					}
					else
					{
						printf("アイテムコマンドが封印されている！やむを得ず通常攻撃！\n");
						pPlayer++;//ポインタ先をプレイヤー２へ
						pPlayer->fMP = pPlayer->fMP + 5;//通常攻撃を選択した場合、プレイヤー２のＭＰを５回復する。
						pPlayer--;//ポインタ先をプレイヤー１へ
						getchar();
						Player2NormalAttack(&g_aPlayer[0], Player1CounterFlag, bPlayer2CriticalLineBuff);//スキルが発動できなかった場合に通常攻撃を発動。
					}

					rewind(stdin);
					getchar();
					system("cls");//画面をリセットする。
					break;

				default:
					printf("入力された値が違うよ！\n\n");

					rewind(stdin);
					getchar();
					system("cls");//画面をリセットする。
					break;
				}

				if (nChoose == 1 || nChoose == 2 || nChoose == 3 || nChoose == 4)
				{
					nTurnAttack = 0;
					break;
				}

			}


		}


		//========================================================
		//最大値や最低値を下回った場合の処理
		//========================================================
		if (g_aPlayer[PlayerNumber1].fLife >= fPlayer1MaxHp)
		{
			g_aPlayer[PlayerNumber1].fLife = fPlayer1MaxHp;//最大HPを回復などで上回った場合、体力を最大体力に固定。
		}

		if (g_aPlayer[PlayerNumber2].fLife >= fPlayer2MaxHp)
		{
			g_aPlayer[PlayerNumber2].fLife = fPlayer2MaxHp;//最大HPを回復などで上回った場合、体力を最大体力に固定。
		}

		if (g_aPlayer[PlayerNumber1].fMP >= fPlayer1MaxMp)//最大MPを上回った場合に、それ以上増えないように固定。
		{
			g_aPlayer[PlayerNumber1].fMP = fPlayer1MaxMp;
		}

		if (g_aPlayer[PlayerNumber2].fMP >= fPlayer2MaxMp)//最大MPを上回った場合、それ以上増えないように固定。
		{
			g_aPlayer[PlayerNumber2].fMP = fPlayer2MaxMp;
		}


		if (g_aPlayer[PlayerNumber1].fMP <= 0)//プレイヤー１のＭＰが０を下回った場合、それ以上下がらないように固定。
		{
			g_aPlayer[PlayerNumber1].fMP = 0;
		}

		if (g_aPlayer[PlayerNumber2].fMP <= 0)//プレイヤー１のＭＰが０を下回った場合、それ以上下がらないように固定。
		{
			g_aPlayer[PlayerNumber2].fMP = 0;
		}
		//================================================================================================================

		//===============================================
		//カウンター状態になってからの処理
		//===============================================

		if (Player1CounterFlag == true)
		{
			nPlayer1CounterCount++;//カウンター状態になってから経過したターンをカウントする。

			if (nPlayer1CounterCount == 6)//プレイヤー１とプレイヤー２が行動するので、１ターンでカウントは２個進む。カウンターは３ターンの間発動するので、カウンターカウントは「６」で大丈夫。
			{
				Player1CounterFlag = false;//カウンター状態になってから３ターンたったら、
				nPlayer1CounterCount = 0;//カウンター状態になってからのカウントを０にする。
			}
		}

		if (Player2CounterFlag == true)
		{
			nPlayer2CounterCount++;//カウンター状態になってから経過したターンをカウントする。

			if (nPlayer2CounterCount == 7)//プレイヤー１とプレイヤー２が行動するので、１ターンでカウントは２個進む。カウンターは３ターンの間発動するので、カウンターカウントは「６」で大丈夫。
			{
				Player2CounterFlag = false;//カウンター状態になってから３ターンたったら、
				nPlayer2CounterCount = 0;//カウンター状態になってからのカウントを０にする。
			}
		}
		//===================================================================================================================


		//=======================================================
		//パワダウンアローのデバフが発動しているときの処理
		//=======================================================

		if (Player2PowerDownArrowDebuff == true)
		{
			nPlayer2PowerDownArrowDebuffCount++;//プレイヤー２がパワダウンアローのデバフを受けているターン数のカウントを＋１する。

			if (nPlayer2PowerDownArrowDebuffCount == 5)
			{
				pPlayer++;//ポインタ先をプレイヤー２へ
				pPlayer->fATK = pPlayer->fATK + (fPlayer2MaxATK * 0.5);//プレイヤー２の攻撃力を＋０．３倍する。
				pPlayer--;//ポインタ先をプレイヤー１へ
				nPlayer2PowerDownArrowDebuffCount = 0;//カウントを０に戻す。
				Player2PowerDownArrowDebuff = false;//デバフが終わったので、発動フラグをfalseにしておく。
			}
		}

		if (Player1PowerDownArrowDebuff == true)
		{
			nPlayer1PowerDownArrowDebuffCount++;//プレイヤー1がパワダウンアローのデバフを受けているターン数のカウントを＋１する。

			if (nPlayer1PowerDownArrowDebuffCount == 5)
			{

				pPlayer->fATK = pPlayer->fATK + (fPlayer1MaxATK * 0.5);//プレイヤー１の攻撃力を＋０．３倍する。
				nPlayer1PowerDownArrowDebuffCount = 0;
				Player1PowerDownArrowDebuff = false;//デバフが終わったので、発動フラグをfalseにしておく。
			}
		}
		//===========================================================================================================================


		//=============================================
		//守りの陣のバフが発動しているときの処理
		//=============================================

		if (bPlayer1DefenceLineBuff == true)
		{
			nPlayer1DefenceLineBuffCount++;//プレイヤー１が守りの陣のバフを受けているターン数のカウントを＋１する。

			if (nPlayer1DefenceLineBuffCount == 12)//バフを受けてから３ターンたった時
			{
				pPlayer->fDEF = pPlayer->fDEF - (fPlayer1MaxDEF * 0.4);//プレイヤー１の防御力をー０．４倍する。
				bPlayer1DefenceLineBuff = false;
				nPlayer1DefenceLineBuffCount = 0;//バフが終わったので、バフを受けているターン数のカウントも０にする。
			}
		}

		if (bPlayer2DefenceLineBuff == true)
		{
			nPlayer2DefenceLineBuffCount++;//プレイヤー２が守りの陣のバフを受けているターン数のカウントを＋１する。

			if (nPlayer2DefenceLineBuffCount == 12)//バフを受けてから３ターンたった時
			{
				pPlayer++;//ポインタ先をプレイヤー２へ
				pPlayer->fDEF = pPlayer->fDEF - (fPlayer2MaxDEF * 0.4);//プレイヤー２の防御力をー０．４倍する。
				bPlayer2DefenceLineBuff = false;
				nPlayer2DefenceLineBuffCount = 0;//バフが終わったので、バフを受けているターン数のカウントも０にする。
				pPlayer--;//ポインタ先をプレイヤー１へ
			}
		}
		//================================================================================================================================

		//==========================================================
		//会心の陣のバフが発動しているときの処理
		//==========================================================

		if (bPlayer1CriticalLineBuff == true)
		{
			nPlayer1CriticalLineBuffCount++;//プレイヤー１が会心の陣のバフを受けているターン数のカウントを＋１する。

			if (nPlayer1CriticalLineBuffCount == 8)//バフを受けた次のターンから３ターン経ったとき
			{
				bPlayer1CriticalLineBuff = false;//バフのターンが終わったので、フラグをfalseにする。
				nPlayer1CriticalLineBuffCount = 0;//バフが終わったので、バフを受けているターン数のカウントを０にする。
			}
		}

		if (bPlayer2CriticalLineBuff == true)
		{
			nPlayer2CriticalLineBuffCount++;//プレイヤー２が会心の陣のバフを受けているターン数のカウントを＋１する。

			if (nPlayer2CriticalLineBuffCount == 8)
			{
				bPlayer2CriticalLineBuff = false;//バフのターンが終わったので、フラグをfalseにする。
				nPlayer2CriticalLineBuffCount = 0;//バフが消えたので、バフを受けているターン数のカウントも０にする。
			}
		}
		//======================================================================================================================================================

		//=================================================================================
		//スキル：「ファントムスタブ」のスキル不可のデバフを受けているときの処理
		//=================================================================================

		if (bPlayer2PhantomStabDebuff == true)
		{
			nPlayer2PhantomStabDebuffCount++;//プレイヤー２がファントムスタブのデバフを受けているターン数のカウントを＋１する。

			if (nPlayer2PhantomStabDebuffCount == 7)
			{
				bPlayer2PhantomStabDebuff = false;//デバフのターンが終わったので、フラグをfalseにする。
				nPlayer2PhantomStabDebuffCount = 0;//デバフが消えたので、デバフを受けているターン数のカウントも０にする。
			}
		}

		if (bPlayer1PhantomStabDebuff == true)
		{
			nPlayer1PhantomStabDebuffCount++;//プレイヤー１がファントムスタブのデバフを受けているターン数のカウントを＋１する。

			if (nPlayer1PhantomStabDebuffCount == 7)
			{
				bPlayer1PhantomStabDebuff = false;//デバフのターンが終わったので、フラグをfalseにする。
				nPlayer1PhantomStabDebuffCount = 0;//デバフが消えたので、デバフを受けているターン数のカウントも０にする。
			}
		}

		if (g_aPlayer[PlayerNumber1].fLife <= 0 || g_aPlayer[PlayerNumber2].fLife <= 0)//どちらかのプレイヤーの体力が０になったらbEndBattleをtrueにする。
		{
			bEndBattle = true;//値を真に変える。
		}

		//==================================================================================================================================================================

		//====================================================================
		//スキル「ポイズンミスト」の毒状態を受けているときの処理
		//====================================================================

		if (bPlayer2PoisonMistState == true)
		{
			nPlayer2PoisonMistStateCount++;//プレイヤー２がスキル「ポイズンミスト」の毒状態を受けているターン数をカウントする変数。

			pPlayer++;//ポインタ先をプレイヤー２へ
			pPlayer->fLife = pPlayer->fLife - (pPlayer->fLife * 0.03);//毒状態の時、毎ターンＨＰが現在ＨＰの3％ずつ減っていく。

			if (nPlayer2PoisonMistStateCount == 1)//毒状態になった直後に処理を実行する。
			{
				pPlayer->fATK = pPlayer->fATK - (fPlayer2MaxATK * 0.2);//毒状態の時、攻撃力が２０％減少する。
			}

			if (nPlayer2PoisonMistStateCount == 7)//プレイヤー２が毒状態になってから３ターン立った時
			{
				bPlayer2PoisonMistState = false;//ターンがなくなったので、毒状態のフラグをfalseにする。
				nPlayer2PoisonMistStateCount = 0;//毒状態になってからのターン数のカウントも０にする。

				pPlayer->fATK = pPlayer->fATK + (fPlayer2MaxATK * 0.2);//プレイヤー２の攻撃力を２０％上昇させる。
			}
			pPlayer--;//ポインタ先をプレイヤー１へ

		}

		if (bPlayer1PoisonMistState == true)
		{
			nPlayer1PoisonMistStateCount++;//プレイヤー１がスキル「ポイズンミスト」の毒状態を受けているターン数をカウントする変数。

			pPlayer->fLife = pPlayer->fLife - (pPlayer->fLife * 0.03);//毒状態の時、毎ターンＨＰが現在ＨＰの3％ずつ減っていく。

			if (nPlayer1PoisonMistStateCount == 1)//毒状態になった直後に処理を実行する。
			{
				pPlayer->fATK = pPlayer->fATK - (fPlayer1MaxATK * 0.2);//毒状態の時、攻撃力が２０％減少する。
			}

			if (nPlayer1PoisonMistStateCount == 7)//プレイヤー１が毒状態になってから３ターン立った時
			{
				bPlayer1PoisonMistState = false;//ターンがなくなったので、毒状態のフラグをfalseにする。
				nPlayer1PoisonMistStateCount = 0;//毒状態になってからのターン数のカウントも０にする。

				pPlayer->fATK = pPlayer->fATK + (fPlayer1MaxATK * 0.2);//プレイヤー１の攻撃力を２０％上昇させる。
			}
		}
		//==============================================================================================================================================

		//==============================================================================
		//スキル「スチームバーストのデメリット効果「火傷」が発動しているときの処理
		//==============================================================================

		if (bPlayer1SteamBurstBurn == true)//プレイヤー１がデメリット効果を受けているとき
		{
			nPlayr1SteamBurstBurnCount++;//火傷を受けているターン数をカウントする変数。

			pPlayer->fLife = pPlayer->fLife - (pPlayer->fLife * 0.03);//火傷状態の時、毎ターンＨＰが現在ＨＰの3％ずつ減っていく。

			if (nPlayr1SteamBurstBurnCount == 1)//火傷が発動した瞬間
			{
				pPlayer->fDEF = pPlayer->fDEF - (fPlayer1MaxDEF * 0.2);//火傷状態の時、防御力が２０％減少する。	
			}

			if (nPlayr1SteamBurstBurnCount == 7)//やけどを受けてから３ターンが立った時
			{
				bPlayer1SteamBurstBurn = false;//火傷状態が終わったので、火傷状態のフラグをfalseにする。
				nPlayr1SteamBurstBurnCount = 0;//ターン数のカウントも０にする。

				pPlayer->fDEF = pPlayer->fDEF + (fPlayer1MaxDEF * 0.2);//プレイヤー１の防御力を＋２０％
			}
		}

		if (bPlayer2SteamBurstBurn == true)//プレイヤー２がデメリット効果を受けているとき
		{
			nPlayr2SteamBurstBurnCount++;//火傷を受けているターン数をカウントする変数。

			pPlayer++;//ポインタ先をプレイヤー２へ
			pPlayer->fLife = pPlayer->fLife - (pPlayer->fLife * 0.03);//火傷状態の時、毎ターンＨＰが現在ＨＰの3％ずつ減っていく。

			if (nPlayr2SteamBurstBurnCount == 1)//火傷が発動した瞬間
			{
				pPlayer->fDEF = pPlayer->fDEF - (fPlayer2MaxDEF * 0.2);//火傷状態の時、防御力が２０％減少する。	
			}

			if (nPlayr2SteamBurstBurnCount == 7)//やけどを受けてから３ターンが立った時
			{
				bPlayer2SteamBurstBurn = false;//火傷状態が終わったので、火傷状態のフラグをfalseにする。
				nPlayr2SteamBurstBurnCount = 0;//ターン数のカウントも０にする。

				pPlayer->fDEF = pPlayer->fDEF + (fPlayer2MaxDEF * 0.2);//プレイヤー２の防御力を＋２０％
			}
			pPlayer--;//ポインタ先をプレイヤー１へ
		}
		//==========================================================================================================================================================

		//=============================================================================
		//スキル「イカサマレーザー」のデバフが発動しているときの処理
		//=============================================================================

		if (bPlayer2SquitLaserDebuff == true)//プレイヤー２がデバフを受けているとき
		{
			nPlayer2SquitLaserDebuffCount++;//デバフを受けているターン数をカウントする変数。

			if (nPlayer2SquitLaserDebuffCount == 1)
			{
				pPlayer++;//ポインタ先をプレイヤー２へ
				pPlayer->fDEF = pPlayer->fDEF - (fPlayer2MaxDEF * 0.4);
				pPlayer--;//ポインタ先をプレイヤー１へ
			}

			if (nPlayer2SquitLaserDebuffCount == 7)
			{
				bPlayer2SquitLaserDebuff = false;//デバフのターンが終わったので、フラグをfalseにする。
				nPlayer2SquitLaserDebuffCount = 0;//ターン数のカウントも０にする。
				pPlayer++;//ポインタ先をプレイヤー２へ
				pPlayer->fDEF = pPlayer->fDEF + (fPlayer2MaxDEF * 0.4);//防御力を＋４０％
				pPlayer--;//ポインタ先をプレイヤー１へ
			}
		}

		if (bPlayer1SquitLaserDebuff == true)//プレイヤー１がデバフを受けているとき
		{
			nPlayer1SquitLaserDebuffCount++;//デバフを受けているターン数をカウントする変数。

			if (nPlayer1SquitLaserDebuffCount == 1)
			{

				pPlayer->fDEF = pPlayer->fDEF - (fPlayer1MaxDEF * 0.4);

			}

			if (nPlayer1SquitLaserDebuffCount == 7)
			{
				bPlayer1SquitLaserDebuff = false;//デバフのターンが終わったので、フラグをfalseにする。
				nPlayer1SquitLaserDebuffCount = 0;//ターン数のカウントも０にする。
				pPlayer->fDEF = pPlayer->fDEF + (fPlayer1MaxDEF * 0.4);//防御力を＋４０％
			}
		}
		//=========================================================================================================================================

		//================================================
		//カレーライスのバフを受けているときの処理
		//================================================

		if (bPlayer1CurryRiceBuffLV1 == true)//カレーライスのバフを受けていることを表すフラグがtrueになったとき
		{
			nPlayer1CurryRiceBuffLV1Count++;//カレーライスのバフを受けてからのターン数をカウントする。

			if (nPlayer1CurryRiceBuffLV1Count == 10)
			{
				bPlayer1CurryRiceBuffLV1 = false;//バフのターンが終わったのでフラグをfalseにする。
				nPlayer1CurryRiceBuffLV1Count = 0;//カレーライスのバフを受けているターン数のカウントを０にする。
				pPlayer->fATK = pPlayer->fATK - (fPlayer1MaxATK * 0.1);//プレイヤー１の攻撃力を元に戻す。
			}

		}
		if (bPlayer2CurryRiceBuffLV1 == true)//カレーライスのバフを受けていることを表すフラグがtrueになったとき
		{
			nPlayer2CurryRiceBuffLV1Count++;//カレーライスのバフを受けてからのターン数をカウントする。

			if (nPlayer2CurryRiceBuffLV1Count == 10)
			{
				bPlayer2CurryRiceBuffLV1 = false;//バフのターンが終わったのでフラグをfalseにする。
				nPlayer2CurryRiceBuffLV1Count = 0;//カレーライスのバフを受けているターン数のカウントを０にする。
				pPlayer++;//ポインタ先をプレイヤー２へ
				pPlayer->fATK = pPlayer->fATK - (fPlayer2MaxATK * 0.1);//プレイヤー１の攻撃力を元に戻す。
				pPlayer--;//ポインタ先をプレイヤー１へ
			}

		}
		//================================================================================================================================================

		//=====================================================
		//シーフードカレーのバフを受けている時の処理
		//=====================================================

		if (bPlayer1CurryRiceBuffLV2 == true)//シーフードカレーのバフを受けていることを表すフラグがtrueになったとき
		{
			nPlayer1CurryRiceBuffLV2Count++;//シーフードカレーのバフを受けてからのターン数をカウントする。

			if (nPlayer1CurryRiceBuffLV2Count == 10)
			{
				bPlayer1CurryRiceBuffLV2 = false;//バフのターンが終わったのでフラグをfalseにする。
				nPlayer1CurryRiceBuffLV2Count = 0;//シーフードカレーのバフを受けているターン数のカウントを０にする。
				pPlayer->fATK = pPlayer->fATK - (fPlayer1MaxATK * 0.3);//プレイヤー１の攻撃力を元に戻す。
			}

		}
		if (bPlayer2CurryRiceBuffLV2 == true)//シーフードカレーのバフを受けていることを表すフラグがtrueになったとき
		{
			nPlayer2CurryRiceBuffLV2Count++;//シーフードカレーのバフを受けてからのターン数をカウントする。

			if (nPlayer2CurryRiceBuffLV2Count == 10)
			{
				bPlayer2CurryRiceBuffLV2 = false;//バフのターンが終わったのでフラグをfalseにする。
				nPlayer2CurryRiceBuffLV2Count = 0;//シーフードカレーのバフを受けているターン数のカウントを０にする。
				pPlayer++;//ポインタ先をプレイヤー２へ
				pPlayer->fATK = pPlayer->fATK - (fPlayer2MaxATK * 0.3);//プレイヤー２の攻撃力を元に戻す。
				pPlayer--;//ポインタ先をプレイヤー１へ
			}

		}
		//================================================================================================================================================

		//=====================================================
		//ポークカレーのバフを受けている時の処理
		//=====================================================

		if (bPlayer1CurryRiceBuffLV3 == true)//ポークカレーのバフを受けていることを表すフラグがtrueになったとき
		{
			nPlayer1CurryRiceBuffLV3Count++;//ポークカレーのバフを受けてからのターン数をカウントする。

			if (nPlayer1CurryRiceBuffLV3Count == 10)
			{
				bPlayer1CurryRiceBuffLV3 = false;//バフのターンが終わったのでフラグをfalseにする。
				nPlayer1CurryRiceBuffLV3Count = 0;//ポークカレーのバフを受けているターン数のカウントを０にする。
				pPlayer->fATK = pPlayer->fATK - (fPlayer1MaxATK * 0.5);//プレイヤー１の攻撃力を元に戻す。
			}

		}
		if (bPlayer2CurryRiceBuffLV3 == true)//ポークカレーのバフを受けていることを表すフラグがtrueになったとき
		{
			nPlayer2CurryRiceBuffLV3Count++;//ポークカレーのバフを受けてからのターン数をカウントする。

			if (nPlayer2CurryRiceBuffLV3Count == 10)
			{
				bPlayer2CurryRiceBuffLV3 = false;//バフのターンが終わったのでフラグをfalseにする。
				nPlayer2CurryRiceBuffLV3Count = 0;//ポークカレーのバフを受けているターン数のカウントを０にする。
				pPlayer++;//ポインタ先をプレイヤー２へ
				pPlayer->fATK = pPlayer->fATK - (fPlayer2MaxATK * 0.5);//プレイヤー２の攻撃力を元に戻す。
				pPlayer--;//ポインタ先をプレイヤー１へ
			}

		}
		//========================================================================================================================================================

		//====================================================
		//塩おむすびのバフを受けている時の処理
		//====================================================

		if (bPlayer1RiceBallBuffLV1 == true)//プレイヤー１が塩おむすびのバフを受けていることを表すフラグがtrueになったとき
		{
			nPlayer1RiceBallBuffLV1Count++;//塩おむすびのバフを受けてからのターン数をカウントする。

			if (nPlayer1RiceBallBuffLV1Count == 10)
			{
				bPlayer1RiceBallBuffLV1 = false;//バフのターンが終わったのでフラグをfalseにする。
				nPlayer1RiceBallBuffLV1Count = 0;//塩おむすびのバフを受けているターン数のカウントも０にする。
				pPlayer->fDEF = pPlayer->fDEF - (fPlayer1MaxDEF * 0.1);//プレイヤー１の防御力を元に戻す。
			}
		}


		if (bPlayer2RiceBallBuffLV1 == true)//プレイヤー２が塩おむすびのバフを受けていることを表すフラグがtrueになったとき
		{
			nPlayer2RiceBallBuffLV1Count++;//塩おむすびのバフを受けてからのターン数をカウントする。

			if (nPlayer2RiceBallBuffLV1Count == 10)
			{
				bPlayer2RiceBallBuffLV1 = false;//バフのターンが終わったのでフラグをfalseにする。
				nPlayer2RiceBallBuffLV1Count = 0;//塩おむすびのバフを受けているターン数のカウントも０にする。
				pPlayer++;//ポインタ先をプレイヤー２へ
				pPlayer->fDEF = pPlayer->fDEF - (fPlayer2MaxDEF * 0.1);//プレイヤー２の防御力を元に戻す。
				pPlayer--;//ポインタ先をプレイヤー１へ
			}
		}
		//========================================================================================================================================================

		//====================================================
		//鮭おむすびのバフを受けている時の処理
		//====================================================

		if (bPlayer1RiceBallBuffLV2 == true)//プレイヤー１が鮭おむすびのバフを受けていることを表すフラグがtrueになったとき
		{
			nPlayer1RiceBallBuffLV2Count++;//鮭おむすびのバフを受けてからのターン数をカウントする。

			if (nPlayer1RiceBallBuffLV2Count == 10)
			{
				bPlayer1RiceBallBuffLV2 = false;//バフのターンが終わったのでフラグをfalseにする。
				nPlayer1RiceBallBuffLV2Count = 0;//鮭おむすびのバフを受けているターン数のカウントも０にする。
				pPlayer->fDEF = pPlayer->fDEF - (fPlayer1MaxDEF * 0.3);//プレイヤー１の防御力を元に戻す。
			}
		}

		if (bPlayer2RiceBallBuffLV2 == true)//プレイヤー２が鮭おむすびのバフを受けていることを表すフラグがtrueになったとき
		{
			nPlayer2RiceBallBuffLV2Count++;//鮭おむすびのバフを受けてからのターン数をカウントする。

			if (nPlayer2RiceBallBuffLV2Count == 10)
			{
				bPlayer2RiceBallBuffLV2 = false;//バフのターンが終わったのでフラグをfalseにする。
				nPlayer2RiceBallBuffLV2Count = 0;//鮭おむすびのバフを受けているターン数のカウントも０にする。
				pPlayer++;//ポインタ先をプレイヤー２へ
				pPlayer->fDEF = pPlayer->fDEF - (fPlayer2MaxDEF * 0.3);//プレイヤー２の防御力を元に戻す。
				pPlayer--;//ポインタ先をプレイヤー１へ
			}
		}

		if (g_aPlayer[PlayerNumber1].fLife <= 0 || g_aPlayer[PlayerNumber2].fLife <= 0)//どちらかのプレイヤーの体力が０になったらbEndBattleをtrueにする。
		{
			bEndBattle = true;//値を真に変える。
		}
		//========================================================================================================================================================

		//====================================================
		//いくらおむすびのバフを受けている時の処理
		//====================================================

		if (bPlayer1RiceBallBuffLV3 == true)//プレイヤー１がいくらおむすびのバフを受けていることを表すフラグがtrueになったとき
		{
			nPlayer1RiceBallBuffLV3Count++;//いくらおむすびのバフを受けてからのターン数をカウントする。

			if (nPlayer1RiceBallBuffLV3Count == 10)
			{
				bPlayer1RiceBallBuffLV3 = false;//バフのターンが終わったのでフラグをfalseにする。
				nPlayer1RiceBallBuffLV3Count = 0;//いくらおむすびのバフを受けているターン数のカウントも０にする。
				pPlayer->fDEF = pPlayer->fDEF - (fPlayer1MaxDEF * 0.5);//プレイヤー１の防御力を元に戻す。
			}
		}

		if (bPlayer2RiceBallBuffLV3 == true)//プレイヤー２がいくらおむすびのバフを受けていることを表すフラグがtrueになったとき
		{
			nPlayer2RiceBallBuffLV3Count++;//いくらおむすびのバフを受けてからのターン数をカウントする。

			if (nPlayer2RiceBallBuffLV3Count == 10)
			{
				bPlayer2RiceBallBuffLV3 = false;//バフのターンが終わったのでフラグをfalseにする。
				nPlayer2RiceBallBuffLV3Count = 0;//いくらおむすびのバフを受けているターン数のカウントも０にする。
				pPlayer++;//ポインタ先をプレイヤー２へ
				pPlayer->fDEF = pPlayer->fDEF - (fPlayer2MaxDEF * 0.5);//プレイヤー２の防御力を元に戻す。
				pPlayer--;//ポインタ先をプレイヤー１へ
			}
		}
		//==========================================================================================================================================

		//=====================================================================
		//スキル「フレイムウェーブ」のバフが発動している時の処理
		//=====================================================================

		if (bPlayer1FlameWaveBuff == true)//プレイヤー１がスキル「フレイムウェーブ」のバフを受けていることを表すフラグがtrueになったとき
		{
			nPlayer1FlameWaveBuffCount++;//フレイムウェーブのバフを受けてからのターン数をカウントする。

			if (nPlayer1FlameWaveBuffCount == 1)
			{
				pPlayer->fATK = pPlayer->fATK + (fPlayer1MaxATK * 0.25);//攻撃力を＋２５％
			}

			if (nPlayer1FlameWaveBuffCount == 6)//バフを受けてから２ターン立った時
			{
				bPlayer1FlameWaveBuff = false;//バフのターンが終わったのでフラグをfalseにする。
				nPlayer1FlameWaveBuffCount = 0;//バフを受けているターン数のカウントも０にする。
				pPlayer->fATK = pPlayer->fATK - (fPlayer1MaxATK * 0.25);//攻撃力をー２５％
			}
		}

		if (bPlayer2FlameWaveBuff == true)//プレイヤー２がスキル「フレイムウェーブ」のバフを受けていることを表すフラグがtrueになったとき
		{
			nPlayer2FlameWaveBuffCount++;//フレイムウェーブのバフを受けてからのターン数をカウントする。

			if (nPlayer2FlameWaveBuffCount == 1)
			{
				pPlayer++;//ポインタ先をプレイヤー２へ
				pPlayer->fATK = pPlayer->fATK + (fPlayer1MaxATK * 0.25);//攻撃力を＋２５％
				pPlayer--;//ポインタ先をプレイヤー１へ
			}

			if (nPlayer2FlameWaveBuffCount == 6)//バフを受けてから２ターン立った時
			{
				bPlayer2FlameWaveBuff = false;//バフのターンが終わったのでフラグをfalseにする。
				nPlayer2FlameWaveBuffCount = 0;//バフを受けているターン数のカウントも０にする。
				pPlayer++;//ポインタ先をプレイヤー２へ
				pPlayer->fATK = pPlayer->fATK - (fPlayer1MaxATK * 0.25);//攻撃力をー２５％
				pPlayer--;//ポインタ先をプレイヤー１へ
			}
		}
		//========================================================================================================================================================

		//===============================================================================
		//スキル「アイスランス」の特殊効果「アイテム封印」が発動しているときの処理
		//===============================================================================

		if (bPlayer2IceLanceDebuff == true)
		{
			nPlayer2IceLanceDebuffCount++;//プレイヤー２がスキル「アイスランス」のアイテム封印を受けているターン数をカウントする。

			if (nPlayer2IceLanceDebuffCount == 11)//特殊効果を受けてから３ターンが経った時
			{
				bPlayer2IceLanceDebuff = false;//特殊効果を受けるターンが終わったのでフラグをfalseにする。
			}
		}

		if (bPlayer1IceLanceDebuff == true)
		{
			nPlayer1IceLanceDebuffCount++;//プレイヤー１がスキル「アイスランス」のアイテム封印を受けているターン数をカウントする。

			if (nPlayer1IceLanceDebuffCount == 11)//特殊効果を受けてから３ターンが経った時
			{
				bPlayer1IceLanceDebuff = false;//特殊効果を受けるターンが終わったのでフラグをfalseにする。
			}
		}
		//==============================================================================================================================================

		//====================================================
		//アイテム：衰弱玉のデバフを受けているときの処理
		//====================================================

		if (bPlayer2WeakBallDeBuff == true)
		{
			nPlayer2WeakBallDeBuffCount++;//プレイヤー２がアイテム「衰弱玉」のデバフを受けているときの処理

			if (nPlayer2WeakBallDeBuffCount == 7)//デバフを受けてから３ターンが経った時
			{
				bPlayer2WeakBallDeBuff = false;//デバフを受けるターンが終わったのでフラグをfalseにする。
				pPlayer++;//ポインタ先をプレイヤー２へ
				pPlayer->fATK = pPlayer->fATK + (fPlayer2MaxATK * 0.4);//攻撃力を元に戻す。
				pPlayer->fDEF = pPlayer->fDEF + (fPlayer2MaxDEF * 0.4);//防御力を元に戻す。
				pPlayer--;//ポインタ先をプレイヤー１へ
			}
		}
		if (bPlayer1WeakBallDeBuff == true)
		{
			nPlayer1WeakBallDeBuffCount++;//プレイヤー１がアイテム「衰弱玉」のデバフを受けているときの処理

			if (nPlayer1WeakBallDeBuffCount == 7)//デバフを受けてから３ターンが経った時
			{
				bPlayer1WeakBallDeBuff = false;//デバフを受けるターンが終わったのでフラグをfalseにする。
				pPlayer->fATK = pPlayer->fATK + (fPlayer1MaxATK * 0.4);//攻撃力を元に戻す。
				pPlayer->fDEF = pPlayer->fDEF + (fPlayer1MaxDEF * 0.4);//防御力を元に戻す。
			}
		}
		//==================================================================================================================================

		//==========================================
		//ターンの処理
		//==========================================
		
		/*nHalfTurn++;
		if (nHalfTurn % 2 == 0)
		{
			nTurn++;
		}*/
		nTurn++;
		if (nTurn != 0)
		{
			if (nTurn % 20 == 0)//２０で割って余りが０のとき
			{
				nPlayer1ItemRouletteUseCount++;
				nPlayer2ItemRouletteUseCount++;
				printf("アイテムが補充された！\n");
				rewind(stdin);
				getchar();
				system("cls");
			}
		}

		if (g_aPlayer[PlayerNumber1].fLife <= 0 || g_aPlayer[PlayerNumber2].fLife <= 0)//どちらかのプレイヤーの体力が０になったらbEndBattleをtrueにする。
		{
			bEndBattle = true;//値を真に変える。
		}

	} while (bEndBattle == false);


	if (g_aPlayer[PlayerNumber1].fLife >= 1)//プレイヤー１の体力が１以上なら処理を実行する。
	{
		printf("%sの勝利！\n\n", &g_aPlayer[PlayerNumber1].aName[0]);
	}

	else
	{

		printf("%sの勝利!\n\n", &g_aPlayer[PlayerNumber2].aName[0]);
	}


}




//===============================================================================
//プレイヤー１の通常攻撃
//===============================================================================
void  Player1NormalAttack(Player* pPlayer, bool Player2CounterFlag, bool bPlayer1CriticalLineBuff)//プレイヤー１が通常攻撃をするときに呼び出す関数。プレイヤー２がカウンター状態になったときに４０％の確率でカウンターを発動させる。
{
	srand((unsigned int)time(NULL));//乱数の種を現在時刻を基準にランダム生成。
	//int nCount;
	//int nTurnAttack;//攻撃ターンの変数
	float fDamageResult;//ダメージの結果を代入する変数
	float fRandum1;
	int CheckRandum;
	int nCounterOn;//カウンターを確率で発動させる変数。]
	int CriticalLineBuffRandum;//スキル「会心の陣」が発動中に反応するランダム関数。


	fRandum1 = rand() % 25 + 1, CheckRandum = rand() % 20 + 1, CriticalLineBuffRandum = rand() % 21 + 1;//最終的なダメージを２５％の範囲で上下させる。ダメージか上がるか下がるかの判断は、CheckRandum変数で判断する。

	nCounterOn = rand() % 2 + 1;//２分の一の確率でカウンターを発動させる。
	   //if (nTurnAttack == 0)	   //{
	printf("%sの攻撃！\n\n", &g_aPlayer[PlayerNumber1].aName[0]);

	getchar();


	if (bPlayer1CriticalLineBuff == false)//会心の陣のバフが発動していないときに扱う処理。
	{
		//ダメージ計算
		if (CheckRandum >= 1 && CheckRandum <= 9)//乱数倍率が＋の方
		{
			//pPlayer++;//ポインタ先をプレイヤー２へ
			g_aPlayer[1].fLife = g_aPlayer[PlayerNumber2].fLife - (g_aPlayer[PlayerNumber1].fATK - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 + (fRandum1 / 100));//プレイヤー１の攻撃力-プレイヤー2の防御力/2＊（１．０１～１．２５）

			fDamageResult = (g_aPlayer[PlayerNumber1].fATK - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 + (fRandum1 / 100));////プレイヤー１の攻撃力-プレイヤー2の防御力/2＊（１．０１～１．２５）


			//pPlayer->fLife=g_aPlayer[1].fLife - fDamageResult;//プレイヤー2の体力を削る。

			printf("%sに\x1b[31m%.f\x1b[0mのダメージ！\n", &g_aPlayer[PlayerNumber2].aName[0], fDamageResult);

			if (Player2CounterFlag == true && nCounterOn == 1)//プレイヤー２のカウンターフラグがtrueであるかつ、５０％を引き当てた場合、プレイヤー２がカウンターを発動。
			{
				printf("%sがカウンターを発動！\n", &g_aPlayer[PlayerNumber2].aName[0]);
				g_aPlayer[PlayerNumber1].fLife = g_aPlayer[PlayerNumber1].fLife - ((g_aPlayer[PlayerNumber2].fATK - (g_aPlayer[PlayerNumber1].fDEF / 2)) * 2.2);//
				fDamageResult = ((g_aPlayer[PlayerNumber2].fATK - (g_aPlayer[PlayerNumber1].fDEF / 2)) * 2.2);
				printf("%sに\x1b[31m%.f\x1b[0mのダメージ！\n", &g_aPlayer[PlayerNumber1].aName[0], fDamageResult);//プレイヤー１にカウンターのダメージを与える。
			}


		}

		else if (CheckRandum >= 10 && CheckRandum <= 18)//乱数倍率が-の方
		{

			g_aPlayer[1].fLife = g_aPlayer[PlayerNumber2].fLife - (g_aPlayer[PlayerNumber1].fATK - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 - (fRandum1 / 100));//プレイヤー１の攻撃力-プレイヤー２の防御力/2＊（１．０１～１．２５）
			fDamageResult = (g_aPlayer[PlayerNumber1].fATK - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 - (fRandum1 / 100));//プレイヤー１の攻撃力-プレイヤー２の防御力/2＊（0.75～0.99）
			//pPlayer++;//ポインタ先をプレイヤー２へ

			//pPlayer->fLife - fDamageResult;//プレイヤー２の体力を削る。

			printf("%sに\x1b[31m%.f\x1b[0mのダメージ！\n", &g_aPlayer[PlayerNumber2].aName[0], fDamageResult);//ダメージ結果の表示

			if (Player2CounterFlag == true && nCounterOn == 1)//プレイヤー２のカウンターフラグがtrueであるかつ、５０％を引き当てた場合、プレイヤー２がカウンターを発動。
			{
				printf("%sがカウンターを発動！\n", &g_aPlayer[PlayerNumber2].aName[0]);
				g_aPlayer[PlayerNumber1].fLife = g_aPlayer[PlayerNumber1].fLife - ((g_aPlayer[PlayerNumber2].fATK - (g_aPlayer[PlayerNumber1].fDEF / 2)) * 2.2);
				fDamageResult = ((g_aPlayer[PlayerNumber2].fATK - (g_aPlayer[PlayerNumber1].fDEF / 2)) * 2.2);
				printf("%sに\x1b[31m%.f\x1b[0mのダメージ！\n", &g_aPlayer[PlayerNumber1].aName[0], fDamageResult);
			}

		}
		else if (CheckRandum == 19)//クリティカル
		{
			g_aPlayer[1].fLife = g_aPlayer[PlayerNumber2].fLife - (g_aPlayer[PlayerNumber1].fATK) * 2 * (1 + (fRandum1 / 100));//プレイヤー1の攻撃力*2＊（１．０１～１．２５）

			fDamageResult = (g_aPlayer[PlayerNumber1].fATK) * 2 * (1 + (fRandum1 / 100));//プレイヤー１の攻撃力*2＊（１．０１～１．２５）

			printf("\x1b[33mクリティカルだ！\x1b[0m\n");

			printf("%sに%.fのダメージ！\n", &g_aPlayer[PlayerNumber2].aName[0], fDamageResult);

			if (Player2CounterFlag == true && nCounterOn == 1)//プレイヤー２のカウンターフラグがtrueであるかつ、５０％を引き当てた場合、プレイヤー２がカウンターを発動。
			{
				printf("%sがカウンターを発動！\n", &g_aPlayer[PlayerNumber2].aName[0]);
				g_aPlayer[PlayerNumber1].fLife = g_aPlayer[PlayerNumber1].fLife - ((g_aPlayer[PlayerNumber2].fATK - (g_aPlayer[PlayerNumber1].fDEF / 2)) * 2.2);//
				fDamageResult = ((g_aPlayer[PlayerNumber2].fATK - (g_aPlayer[PlayerNumber1].fDEF / 2)) * 2.2);
				printf("%sに\x1b[31m%.f\x1b[0mのダメージ！\n", &g_aPlayer[PlayerNumber1].aName[0], fDamageResult);//プレイヤー１にカウンターのダメージを与える。
			}
		}

		else if (CheckRandum == 20)//回避
		{
			printf("%sは攻撃を回避した！\n", &g_aPlayer[PlayerNumber2].aName[0]);
		}
	}
	else if (bPlayer1CriticalLineBuff == true)//「会心の陣」のバフが発動しているときに扱う処理
	{
		if (CriticalLineBuffRandum >= 1 && CriticalLineBuffRandum <= 6)//乱数倍率が＋の方
		{
			//pPlayer++;//ポインタ先をプレイヤー２へ
			g_aPlayer[1].fLife = g_aPlayer[PlayerNumber2].fLife - (g_aPlayer[PlayerNumber1].fATK - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 + (fRandum1 / 100));//プレイヤー１の攻撃力-プレイヤー2の防御力/2＊（１．０１～１．２５）

			fDamageResult = (g_aPlayer[PlayerNumber1].fATK - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 + (fRandum1 / 100));////プレイヤー１の攻撃力-プレイヤー2の防御力/2＊（１．０１～１．２５）


			//pPlayer->fLife=g_aPlayer[1].fLife - fDamageResult;//プレイヤー2の体力を削る。

			printf("%sに\x1b[31m%.f\x1b[0mのダメージ！\n", &g_aPlayer[PlayerNumber2].aName[0], fDamageResult);

			if (Player2CounterFlag == true && nCounterOn == 1)//プレイヤー２のカウンターフラグがtrueであるかつ、５０％を引き当てた場合、プレイヤー２がカウンターを発動。
			{
				printf("%sがカウンターを発動！\n", &g_aPlayer[PlayerNumber2].aName[0]);
				g_aPlayer[PlayerNumber1].fLife = g_aPlayer[PlayerNumber1].fLife - ((g_aPlayer[PlayerNumber2].fATK - (g_aPlayer[PlayerNumber1].fDEF / 2)) * 2.2);//
				fDamageResult = ((g_aPlayer[PlayerNumber2].fATK - (g_aPlayer[PlayerNumber1].fDEF / 2)) * 2.2);
				printf("%sに\x1b[31m%.f\x1b[0mのダメージ！\n", &g_aPlayer[PlayerNumber1].aName[0], fDamageResult);//プレイヤー１にカウンターのダメージを与える。
			}


		}

		else if (CriticalLineBuffRandum >= 7 && CriticalLineBuffRandum <= 12)//乱数倍率が-の方
		{

			g_aPlayer[1].fLife = g_aPlayer[PlayerNumber2].fLife - (g_aPlayer[PlayerNumber1].fATK - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 - (fRandum1 / 100));//プレイヤー１の攻撃力-プレイヤー２の防御力/2＊（１．０１～１．２５）
			fDamageResult = (g_aPlayer[PlayerNumber1].fATK - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 - (fRandum1 / 100));//プレイヤー１の攻撃力-プレイヤー２の防御力/2＊（0.75～0.99）
			//pPlayer++;//ポインタ先をプレイヤー２へ

			//pPlayer->fLife - fDamageResult;//プレイヤー２の体力を削る。

			printf("%sに\x1b[31m%.f\x1b[0mのダメージ！\n", &g_aPlayer[PlayerNumber2].aName[0], fDamageResult);//ダメージ結果の表示

			if (Player2CounterFlag == true && nCounterOn == 1)//プレイヤー２のカウンターフラグがtrueであるかつ、５０％を引き当てた場合、プレイヤー２がカウンターを発動。
			{
				printf("%sがカウンターを発動！\n", &g_aPlayer[PlayerNumber2].aName[0]);
				g_aPlayer[PlayerNumber1].fLife = g_aPlayer[PlayerNumber1].fLife - ((g_aPlayer[PlayerNumber2].fATK - (g_aPlayer[PlayerNumber1].fDEF / 2)) * 2.2);
				fDamageResult = ((g_aPlayer[PlayerNumber2].fATK - (g_aPlayer[PlayerNumber1].fDEF / 2)) * 2.2);
				printf("%sに\x1b[31m%.f\x1b[0mのダメージ！\n", &g_aPlayer[PlayerNumber1].aName[0], fDamageResult);
			}

		}
		else if (CriticalLineBuffRandum >= 13 && CriticalLineBuffRandum <= 20)//クリティカル
		{
			g_aPlayer[1].fLife = g_aPlayer[PlayerNumber2].fLife - (g_aPlayer[PlayerNumber1].fATK) * 2 * (1 + (fRandum1 / 100));//プレイヤー1の攻撃力*2＊（１．０１～１．２５）

			fDamageResult = (g_aPlayer[PlayerNumber1].fATK) * 2 * (1 + (fRandum1 / 100));//プレイヤー１の攻撃力*2＊（１．０１～１．２５）

			printf("\x1b[33mクリティカルだ！\x1b[0m\n");

			printf("%sに\x1b[31m%.f\x1b[0mのダメージ！\n", &g_aPlayer[PlayerNumber2].aName[0], fDamageResult);

			if (Player2CounterFlag == true && nCounterOn == 1)//プレイヤー２のカウンターフラグがtrueであるかつ、５０％を引き当てた場合、プレイヤー２がカウンターを発動。
			{
				printf("%sがカウンターを発動！\n", &g_aPlayer[PlayerNumber2].aName[0]);
				g_aPlayer[PlayerNumber1].fLife = g_aPlayer[PlayerNumber1].fLife - ((g_aPlayer[PlayerNumber2].fATK - (g_aPlayer[PlayerNumber1].fDEF / 2)) * 2.2);//
				fDamageResult = ((g_aPlayer[PlayerNumber2].fATK - (g_aPlayer[PlayerNumber1].fDEF / 2)) * 2.2);
				printf("%sに\x1b[31m%.f\x1b[0mのダメージ！\n", &g_aPlayer[PlayerNumber1].aName[0], fDamageResult);//プレイヤー１にカウンターのダメージを与える。
			}
		}

		else if (CriticalLineBuffRandum == 21)//回避
		{
			printf("%sは攻撃を\x1b[36m回避\x1b[0mした！\n", &g_aPlayer[PlayerNumber2].aName[0]);
		}
	}

}


//nTurnAttack = 1;//次のターンに回すために変数の値を+1する。

	//else

//===============================================================
//プレイヤー２の通常攻撃
//===============================================================

void Player2NormalAttack(Player* pPlayer, bool Player1CounterFlag, bool bPlayer2CriticalLineBuff)//プレイヤー２が通常攻撃をするときに呼び出す関数。
{
	srand((unsigned int)time(NULL));//乱数の種を現在時刻を基準にランダム生成。

	//int nCount;

	//int nTurnAttack;//攻撃ターンの変数

	float fDamageResult;//ダメージの結果を代入する変数

	float fRandum1;

	int CheckRandum;

	int nCounterOn;

	int nCriticalLineBuffRandum;//「会心の陣のバフが発動しているときに反応する変数。


	fRandum1 = rand() % 25 + 1, CheckRandum = rand() % 20 + 1, nCriticalLineBuffRandum = rand() % 21 + 1;//乱数値（２５％）を宣言)。乱数地（１か２）を宣言

	nCounterOn = rand() % 2 + 1;//２分の一の確率でカウンターを発動させる。

	printf("%sの攻撃！\n\n", &g_aPlayer[PlayerNumber2].aName[0]);//プレイヤー２の攻撃

	getchar();


	//ダメージ計算
	if (bPlayer2CriticalLineBuff == false)//会心の陣のバフが発動していないときに処理をする変数。
	{
		if (CheckRandum >= 1 && CheckRandum <= 9)//乱数倍率が＋の方
		{
			g_aPlayer[0].fLife = g_aPlayer[PlayerNumber1].fLife - (g_aPlayer[PlayerNumber2].fATK - (g_aPlayer[PlayerNumber1].fDEF / 2)) * (1 + (fRandum1 / 100));//プレイヤー２の攻撃力-プレイヤー１の防御力/2＊（１．０１～１．２５）

			fDamageResult = (g_aPlayer[PlayerNumber2].fATK - (g_aPlayer[PlayerNumber1].fDEF / 2)) * (1 + (fRandum1 / 100));//プレイヤー２の攻撃力-プレイヤー１の防御力/2＊（１．０１～１．２５）

			//pPlayer;//ポインタ先をプレイヤー１へ


			//pPlayer->fLife - fDamageResult;//プレイヤー１の体力を削る。

			printf("%sに\x1b[31m%.f\x1b[0mのダメージ！\n", &g_aPlayer[PlayerNumber1].aName[0], fDamageResult);

			if (Player1CounterFlag == true && nCounterOn == 1)//プレイヤー１のカウンターフラグがtrueであるかつ、５０％を引き当てた場合、プレイヤー１がカウンターを発動。
			{
				printf("%sがカウンターを発動！\n", &g_aPlayer[PlayerNumber1].aName[0]);
				g_aPlayer[PlayerNumber2].fLife = g_aPlayer[PlayerNumber2].fLife - ((g_aPlayer[PlayerNumber1].fATK - (g_aPlayer[PlayerNumber2].fDEF / 2)) * 2.2);
				fDamageResult = ((g_aPlayer[PlayerNumber1].fATK - (g_aPlayer[PlayerNumber2].fDEF / 2)) * 2.2);
				printf("%sに\x1b[31m%.f\x1b[0mのダメージ！\n", &g_aPlayer[PlayerNumber2].aName[0], fDamageResult);//プレイヤー２にカウンターのダメージを与える。
			}

		}

		else if (CheckRandum >= 10 && CheckRandum <= 18)//乱数倍率が-の方
		{
			g_aPlayer[0].fLife = g_aPlayer[0].fLife - (g_aPlayer[PlayerNumber2].fATK - (g_aPlayer[PlayerNumber1].fDEF / 2)) * (1 - (fRandum1 / 100));//プレイヤー２の攻撃力-プレイヤー１の防御力/2＊（０．７５～０．９９）

			fDamageResult = (g_aPlayer[PlayerNumber2].fATK - (g_aPlayer[PlayerNumber1].fDEF / 2)) * (1 - (fRandum1 / 100));//プレイヤー２の攻撃力-プレイヤー１の防御力/2＊（０．７５～０．９９）
			//pPlayer;//ポインタ先をプレイヤーへ

			//pPlayer->fLife - fDamageResult;//プレイヤー１の体力を削る。


			printf("%sに\x1b[31m%.f\x1b[0mのダメージ！\n", &g_aPlayer[PlayerNumber1].aName[0], fDamageResult);//ダメージ結果の表示

			if (Player1CounterFlag == true && nCounterOn == 1)//プレイヤー１のカウンターフラグがtrueであるかつ、５０％を引き当てた場合、プレイヤー１がカウンターを発動。
			{
				printf("%sがカウンターを発動！\n", &g_aPlayer[PlayerNumber1].aName[0]);
				g_aPlayer[PlayerNumber2].fLife = g_aPlayer[PlayerNumber2].fLife - ((g_aPlayer[PlayerNumber1].fATK - (g_aPlayer[PlayerNumber2].fDEF / 2)) * 2.2);
				fDamageResult = ((g_aPlayer[PlayerNumber1].fATK - (g_aPlayer[PlayerNumber2].fDEF / 2)) * 2.2);
				printf("%sに\x1b[31m%.f\x1b[0mのダメージ！\n", &g_aPlayer[PlayerNumber2].aName[0], fDamageResult);//プレイヤー２にカウンターのダメージを与える。
			}
		}
		else if (CheckRandum == 19)//クリティカル
		{
			g_aPlayer[PlayerNumber1].fLife = g_aPlayer[PlayerNumber1].fLife - (g_aPlayer[PlayerNumber2].fATK) * 2 * (1 + (fRandum1 / 100));//プレイヤー1の攻撃力*2＊（１．０１～１．２５）

			fDamageResult = (g_aPlayer[PlayerNumber2].fATK) * 2 * (1 + (fRandum1 / 100));//プレイヤー１の攻撃力*2＊（１．０１～１．２５）

			printf("\x1b[33mクリティカル\x1b[0mだ！\n");

			printf("%sに\x1b[31m%.f\x1b[0mのダメージ！\n", &g_aPlayer[PlayerNumber1].aName[0], fDamageResult);

			if (Player1CounterFlag == true && nCounterOn == 1)//プレイヤー２のカウンターフラグがtrueであるかつ、５０％を引き当てた場合、プレイヤー２がカウンターを発動。
			{
				printf("%sがカウンターを発動！\n", &g_aPlayer[PlayerNumber1].aName[0]);
				g_aPlayer[PlayerNumber2].fLife = g_aPlayer[PlayerNumber2].fLife - ((g_aPlayer[PlayerNumber1].fATK - (g_aPlayer[PlayerNumber2].fDEF / 2)) * 2.2);//
				fDamageResult = ((g_aPlayer[PlayerNumber1].fATK - (g_aPlayer[PlayerNumber2].fDEF / 2)) * 2.2);
				printf("%sに\x1b[31m%.f\x1b[0mのダメージ！\n", &g_aPlayer[PlayerNumber2].aName[0], fDamageResult);//プレイヤー２にカウンターのダメージを与える。
			}
		}
		else if (CheckRandum == 20)//回避
		{
			printf("%sは攻撃を\x1b[36m回避\x1b[0mした！\n", &g_aPlayer[PlayerNumber1].aName[0]);
		}
	}
	else if (bPlayer2CriticalLineBuff == true)//会心の陣のバフが発動しているときに処理をする変数。
	{
		if (nCriticalLineBuffRandum >= 1 && nCriticalLineBuffRandum <= 6)//乱数倍率が＋の方
		{
			g_aPlayer[0].fLife = g_aPlayer[PlayerNumber1].fLife - (g_aPlayer[PlayerNumber2].fATK - (g_aPlayer[PlayerNumber1].fDEF / 2)) * (1 + (fRandum1 / 100));//プレイヤー２の攻撃力-プレイヤー１の防御力/2＊（１．０１～１．２５）

			fDamageResult = (g_aPlayer[PlayerNumber2].fATK - (g_aPlayer[PlayerNumber1].fDEF / 2)) * (1 + (fRandum1 / 100));//プレイヤー２の攻撃力-プレイヤー１の防御力/2＊（１．０１～１．２５）

			//pPlayer;//ポインタ先をプレイヤー１へ


			//pPlayer->fLife - fDamageResult;//プレイヤー１の体力を削る。

			printf("%sに\x1b[31m%.f\x1b[0mのダメージ！\n", &g_aPlayer[PlayerNumber1].aName[0], fDamageResult);

			if (Player1CounterFlag == true && nCounterOn == 1)//プレイヤー１のカウンターフラグがtrueであるかつ、５０％を引き当てた場合、プレイヤー１がカウンターを発動。
			{
				printf("%sがカウンターを発動！\n", &g_aPlayer[PlayerNumber1].aName[0]);
				g_aPlayer[PlayerNumber2].fLife = g_aPlayer[PlayerNumber2].fLife - ((g_aPlayer[PlayerNumber1].fATK - (g_aPlayer[PlayerNumber2].fDEF / 2)) * 2.2);
				fDamageResult = ((g_aPlayer[PlayerNumber1].fATK - (g_aPlayer[PlayerNumber2].fDEF / 2)) * 2.2);
				printf("%sに\x1b[31m%.f\x1b[0mのダメージ！\n", &g_aPlayer[PlayerNumber2].aName[0], fDamageResult);//プレイヤー２にカウンターのダメージを与える。
			}

		}

		else if (nCriticalLineBuffRandum >= 7 && nCriticalLineBuffRandum <= 12)//乱数倍率が-の方
		{
			g_aPlayer[0].fLife = g_aPlayer[0].fLife - (g_aPlayer[PlayerNumber2].fATK - (g_aPlayer[PlayerNumber1].fDEF / 2)) * (1 - (fRandum1 / 100));//プレイヤー２の攻撃力-プレイヤー１の防御力/2＊（０．７５～０．９９）

			fDamageResult = (g_aPlayer[PlayerNumber2].fATK - (g_aPlayer[PlayerNumber1].fDEF / 2)) * (1 - (fRandum1 / 100));//プレイヤー２の攻撃力-プレイヤー１の防御力/2＊（０．７５～０．９９）
			//pPlayer;//ポインタ先をプレイヤーへ

			//pPlayer->fLife - fDamageResult;//プレイヤー１の体力を削る。


			printf("%sに\x1b[31m%.f\x1b[0mのダメージ！\n", &g_aPlayer[PlayerNumber1].aName[0], fDamageResult);//ダメージ結果の表示

			if (Player1CounterFlag == true && nCounterOn == 1)//プレイヤー１のカウンターフラグがtrueであるかつ、５０％を引き当てた場合、プレイヤー１がカウンターを発動。
			{
				printf("%sがカウンターを発動！\n", &g_aPlayer[PlayerNumber1].aName[0]);
				g_aPlayer[PlayerNumber2].fLife = g_aPlayer[PlayerNumber2].fLife - ((g_aPlayer[PlayerNumber1].fATK - (g_aPlayer[PlayerNumber2].fDEF / 2)) * 2.2);
				fDamageResult = ((g_aPlayer[PlayerNumber1].fATK - (g_aPlayer[PlayerNumber2].fDEF / 2)) * 2.2);
				printf("%sに\x1b[31m%.f\x1b[0mのダメージ！\n", &g_aPlayer[PlayerNumber2].aName[0], fDamageResult);//プレイヤー２にカウンターのダメージを与える。
			}
		}
		else if (nCriticalLineBuffRandum >= 13 && nCriticalLineBuffRandum <= 20)//クリティカル
		{
			g_aPlayer[PlayerNumber1].fLife = g_aPlayer[PlayerNumber1].fLife - (g_aPlayer[PlayerNumber2].fATK) * 2 * (1 + (fRandum1 / 100));//プレイヤー1の攻撃力*2＊（１．０１～１．２５）

			fDamageResult = (g_aPlayer[PlayerNumber2].fATK) * 2 * (1 + (fRandum1 / 100));//プレイヤー１の攻撃力*2＊（１．０１～１．２５）

			printf("\x1b[33mクリティカル\x1b[0mだ！\n");

			printf("%sに\x1b[31m%.f\x1b[0mのダメージ！\n", &g_aPlayer[PlayerNumber1].aName[0], fDamageResult);

			if (Player1CounterFlag == true && nCounterOn == 1)//プレイヤー２のカウンターフラグがtrueであるかつ、５０％を引き当てた場合、プレイヤー２がカウンターを発動。
			{
				printf("%sがカウンターを発動！\n", &g_aPlayer[PlayerNumber1].aName[0]);
				g_aPlayer[PlayerNumber2].fLife = g_aPlayer[PlayerNumber2].fLife - ((g_aPlayer[PlayerNumber1].fATK - (g_aPlayer[PlayerNumber2].fDEF / 2)) * 2.2);//
				fDamageResult = ((g_aPlayer[PlayerNumber1].fATK - (g_aPlayer[PlayerNumber2].fDEF / 2)) * 2.2);
				printf("%sに\x1b[31m%.f\x1b[0mのダメージ！\n", &g_aPlayer[PlayerNumber2].aName[0], fDamageResult);//プレイヤー２にカウンターのダメージを与える。
			}
		}
		else if (nCriticalLineBuffRandum == 21)//回避
		{
			printf("%sは攻撃を\x1b[36m回避\x1b[0mした！\n", &g_aPlayer[PlayerNumber1].aName[0]);
		}
	}
	//nTurnAttack = 0;
}

//if (g_aPlayer[PlayerNumber1].fLife <= 0 || g_aPlayer[PlayerNumber2].fLife <= 0)//どちらかのプレイヤーの体力が０になったらbEndBattleをtrueにする。
//{
//	bEndBattle = true;//値を真に変える。
//}



//pPlayer;//先頭アドレスを代入(配列[0]へ）
//} while (bEndBattle == false);


//if (g_aPlayer[PlayerNumber1].fLife >= 1)//プレイヤー１の体力が１以上なら処理を実行する。
//{
//	printf("%sの勝利！\n\n", &g_aPlayer[PlayerNumber1].aName[0]);
//}

//else
//{

//	printf("%sの勝利!\n\n", &g_aPlayer[PlayerNumber2].aName[0]);
//}


//=====================================================
//プレイヤー１のスキル：インパクトエッジ
//=====================================================
void Player1InpactEdge(Player* pPlayer)//プレイヤー１のスキル：インパクトエッジ
{
	srand((unsigned int)time(NULL));//乱数の種を現在時刻を基準にランダム生成。

	float fRandum1;
	int CheckRandum;
	float fDamageResult;

	pPlayer->fMP = pPlayer->fMP - 30;//スキル発動時にプレイヤー１のMPを30減らす。

	printf("%sのインパクトエッジ！\n", &g_aPlayer[0].aName[0]);

	fRandum1 = rand() % 25 + 1, CheckRandum = rand() % 2 + 1;//最終的なダメージを２５％の範囲で上下させる。ダメージか上がるか下がるかの判断は、CheckRandum変数で判断する。


	if (CheckRandum == 1)
	{
		g_aPlayer[1].fLife = g_aPlayer[PlayerNumber2].fLife - (g_aPlayer[PlayerNumber1].fATK * 3 - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 + (fRandum1 / 100));//(プレイヤー１の攻撃力*2.5-プレイヤー2の防御力/2)＊（１．０１～１．２５）

		fDamageResult = (g_aPlayer[PlayerNumber1].fATK * 3 - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 + (fRandum1 / 100));//(プレイヤー１の攻撃力*2.5-プレイヤー2の防御力/2)＊（１．０１～１．２５）


	}
	else if (CheckRandum == 2)
	{
		g_aPlayer[1].fLife = g_aPlayer[PlayerNumber2].fLife - (g_aPlayer[PlayerNumber1].fATK * 3 - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 - (fRandum1 / 100));//(プレイヤー１の攻撃力*2.5-プレイヤー2の防御力/2)＊（０．７５～０．９９）
		fDamageResult = (g_aPlayer[PlayerNumber1].fATK * 3 - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 - (fRandum1 / 100));//(プレイヤー１の攻撃力*2.5-プレイヤー2の防御力/2)＊（０．７５～０．９９）
	}

	printf("%sに\x1b[31m%.f\x1b[0mのダメージ！\n", &g_aPlayer[PlayerNumber2].aName[0], fDamageResult);//ダメージ表記



}

//=====================================================================
//プレイヤー２のスキル：インパクトエッジ
//=====================================================================
void Player2InpactEdge(Player* pPlayer)//プレイヤー２が剣士のスキル（１）、「インパクトエッジ」が発動するときに呼び出す関数。
{
	srand((unsigned int)time(NULL));//乱数の種を現在時刻を基準にランダム生成。

	float fRandum1;
	int CheckRandum;
	float fDamageResult;

	pPlayer++;//インクリメントして基準点を１ずらす。
	pPlayer->fMP = pPlayer->fMP - 30;//スキル発動時にＭＰを３０減らす。

	printf("%sのインパクトエッジ！\n", &g_aPlayer[1].aName[0]);

	fRandum1 = rand() % 25 + 1, CheckRandum = rand() % 2 + 1;//最終的なダメージを２５％の範囲で上下させる。ダメージか上がるか下がるかの判断は、CheckRandum変数で判断する。


	if (CheckRandum == 1)
	{
		g_aPlayer[0].fLife = g_aPlayer[PlayerNumber1].fLife - (g_aPlayer[PlayerNumber2].fATK * 3 - (g_aPlayer[PlayerNumber1].fDEF / 2)) * (1 + (fRandum1 / 100));//(プレイヤー１の攻撃力*2.5-プレイヤー2の防御力/2)＊（１．０１～１．２５）

		fDamageResult = (g_aPlayer[PlayerNumber2].fATK * 3 - (g_aPlayer[PlayerNumber1].fDEF / 2)) * (1 + (fRandum1 / 100));//(プレイヤー１の攻撃力*2.5-プレイヤー2の防御力/2)＊（１．０１～１．２５）


	}
	else if (CheckRandum == 2)
	{
		g_aPlayer[0].fLife = g_aPlayer[PlayerNumber1].fLife - (g_aPlayer[PlayerNumber2].fATK * 3 - (g_aPlayer[PlayerNumber1].fDEF / 2)) * (1 - (fRandum1 / 100));//(プレイヤー１の攻撃力*2.5-プレイヤー2の防御力/2)＊（０．７５～０．９９）
		fDamageResult = (g_aPlayer[PlayerNumber2].fATK * 3 - (g_aPlayer[PlayerNumber1].fDEF / 2)) * (1 - (fRandum1 / 100));//(プレイヤー１の攻撃力*2.5-プレイヤー2の防御力/2)＊（０．７５～０．９９）
	}

	printf("%sに\x1b[31m%.f\x1b[0mのダメージ！\n", &g_aPlayer[PlayerNumber1].aName[0], fDamageResult);//ダメージ表記
}



//======================================================
//プレイヤー１のスキル：リカバーヒール
//======================================================
void Player1RecoverHeal(Player* pPlayer, float fPlayer1MaxHp)//プレイヤー１が剣士のスキル（２）、「リカバーヒール」を発動するときに呼び出す関数。
{
	float fRandum1;
	int CheckRandum;
	//float fDamageResult;

	pPlayer->fMP = pPlayer->fMP - 80;//スキル発動時にプレイヤー１のＭＰを５０減らす。

	printf("%sのリカバーヒール！\n", &g_aPlayer[0].aName[0]);

	fRandum1 = rand() % 25 + 1, CheckRandum = rand() % 2 + 1;//最終的なダメージを２５％の範囲で上下させる。ダメージか上がるか下がるかの判断は、CheckRandum変数で判断する。

	pPlayer->fLife = pPlayer->fLife + (fPlayer1MaxHp * 0.25);//プレイヤー１の体力をさいだいＨＰの２５分回復する。


	printf("%sの最大体力の\x1b[36m２５％分\x1b[0m体力を回復する！", &pPlayer->aName[0]);
}


//===========================================================
//プレイヤー２のスキル：リカバーヒール
//===========================================================
void Player2RecoverHeal(Player* pPlayer, float fPlayer2MaxHp)//プレイヤー２が剣士のスキル（２）、「リカバーヒール」を発動する時に呼び出す関数。
{
	float fRandum1;
	int CheckRandum;
	//float fDamageResult;

	pPlayer++;//インクリメントして基準点をづらす。

	pPlayer->fMP = pPlayer->fMP - 80;//スキル発動時にプレイヤー１のＭＰを５０減らす。

	printf("%sのリカバーヒール！\n", &g_aPlayer[0].aName[0]);

	fRandum1 = rand() % 25 + 1, CheckRandum = rand() % 2 + 1;//最終的なダメージを２５％の範囲で上下させる。ダメージか上がるか下がるかの判断は、CheckRandum変数で判断する。

	pPlayer->fLife = pPlayer->fLife + (fPlayer2MaxHp * 0.25);//プレイヤー１の体力をさいだいＨＰの２５分回復する。


	printf("%sの最大体力の\x1b[36m２５％分\x1b[0m体力を回復する！", &pPlayer->aName[0]);
}

//===========================================================
//プレイヤー１のスキル：レインアロー
//===========================================================
void Player1RainArrow(Player* pPlayer)//プレイヤー１が狩人のスキル（１）、「レインアロー」を発動するときに呼び出す関数。
{
	srand((unsigned int)time(NULL));//乱数の種を現在時刻を基準にランダム生成。

	float fRandum1;
	int nAttackSucces;
	nAttackSucces = 0;
	int CheckRandum;
	float fDamageResult;
	int nCount;

	pPlayer->fMP = pPlayer->fMP - 25;//プレイヤー１のＭＰを２５減らす。

	printf("%sのレインアロー！\n", &pPlayer->aName[0]);

	/*fRandum1 = rand() % 25 + 1*/ CheckRandum = rand() % 2 + 1;//最終的なダメージを２５％の範囲で上下させる。ダメージか上がるか下がるかの判断は、CheckRandum変数で判断する。


	if (CheckRandum == 1)
	{
		for (nCount = 0; nCount < 8; nCount++, nAttackSucces = rand() % 10 + 1, fRandum1 = rand() % 25 + 1)//８連続攻撃を行う。（命中率４０％）
		{
			if (nAttackSucces >= 1 && nAttackSucces <= 4)//攻撃成功確率のランダム関数が１～４だったら攻撃を成功させる。
			{
				g_aPlayer[PlayerNumber2].fLife = g_aPlayer[PlayerNumber2].fLife - ((g_aPlayer[PlayerNumber1].fATK * 0.75) - (g_aPlayer[PlayerNumber2].fDEF / 4)) * (1 + (fRandum1 / 100));//プレイヤー１の攻撃力＊３-プレイヤー２の防御力/4＊（１．０１～１．２５）

				fDamageResult = ((g_aPlayer[PlayerNumber1].fATK * 0.75) - (g_aPlayer[PlayerNumber2].fDEF / 4)) * (1 + (fRandum1 / 100));//プレイヤー１の攻撃力＊３-プレイヤー２の防御力/4＊（１．０１～１．２５）


				printf("%sに\x1b[31m%.f\x1b[0mのダメージ！\n", &g_aPlayer[PlayerNumber2].aName[0], fDamageResult);//ダメージ表記
				getchar();
			}

			else
			{
				printf("\x1b[34m攻撃が外れた！\x1b[0m\n");//６０％の確率で攻撃を外させる。
				getchar();
			}
		}

	}
	else if (CheckRandum == 2)
	{
		for (nCount = 0; nCount < 8; nCount++, nAttackSucces = rand() % 10 + 1, fRandum1 = rand() % 25 + 1)//８連続攻撃を行う。（命中率４０％）
		{
			if (nAttackSucces >= 1 && nAttackSucces <= 4)//攻撃成功確率のランダム関数が１～４だったら攻撃を成功させる。
			{
				g_aPlayer[PlayerNumber2].fLife = g_aPlayer[PlayerNumber2].fLife - ((g_aPlayer[PlayerNumber1].fATK * 0.75) - (g_aPlayer[PlayerNumber2].fDEF / 4)) * (1 - (fRandum1 / 100));//プレイヤー１の攻撃力＊３-プレイヤー２の防御力/4＊（１．０１～１．２５）

				fDamageResult = ((g_aPlayer[PlayerNumber1].fATK * 0.75) - (g_aPlayer[PlayerNumber2].fDEF / 4)) * (1 - (fRandum1 / 100));//プレイヤー１の攻撃力＊３-プレイヤー２の防御力/4＊（１．０１～１．２５）


				printf("%sに\x1b[31m%.f\x1b[0mのダメージ！\n", &g_aPlayer[PlayerNumber2].aName[0], fDamageResult);//ダメージ表記
				getchar();
			}

			else
			{
				printf("\x1b[34m攻撃が外れた！\x1b[0m\n");//６０％の確率で攻撃を外させる。
				getchar();
			}
		}

	}





}

//========================================================
//プレイヤー２のスキル：レインアロー
//========================================================
void Player2RainArrow(Player* pPlayer)//プレイヤー２が狩人のスキル（１）、「レインアロー」を発動するときに呼び出す関数。
{
	srand((unsigned int)time(NULL));//乱数の種を現在時刻を基準にランダム生成。

	float fRandum1;
	int nAttackSucces;
	nAttackSucces = 0;
	int CheckRandum;
	float fDamageResult;
	int nCount;

	pPlayer++;//ポインタ先をプレイヤー２へ
	pPlayer->fMP = pPlayer->fMP - 25;//プレイヤー１のＭＰを２５減らす。

	printf("%sのレインアロー！\n", &pPlayer->aName[0]);

	/*fRandum1 = rand() % 25 + 1*/ CheckRandum = rand() % 2 + 1;//最終的なダメージを２５％の範囲で上下させる。ダメージか上がるか下がるかの判断は、CheckRandum変数で判断する。


	if (CheckRandum == 1)
	{
		for (nCount = 0; nCount < 8; nCount++, nAttackSucces = rand() % 10 + 1, fRandum1 = rand() % 25 + 1)//８連続攻撃を行う。（命中率４０％）
		{
			if (nAttackSucces >= 1 && nAttackSucces <= 4)//攻撃成功確率のランダム関数が１～４だったら攻撃を成功させる。
			{
				g_aPlayer[PlayerNumber1].fLife = g_aPlayer[PlayerNumber1].fLife - ((g_aPlayer[PlayerNumber2].fATK * 0.75) - (g_aPlayer[PlayerNumber1].fDEF / 4)) * (1 + (fRandum1 / 100));//プレイヤー２の攻撃力＊３-プレイヤー２の防御力/4＊（１．０１～１．２５）

				fDamageResult = ((g_aPlayer[PlayerNumber2].fATK * 0.75) - (g_aPlayer[PlayerNumber1].fDEF / 4)) * (1 + (fRandum1 / 100));//プレイヤー１の攻撃力＊３-プレイヤー２の防御力/4＊（１．０１～１．２５）


				printf("%sに\x1b[31m%.f\x1b[0mのダメージ！\n", &g_aPlayer[PlayerNumber1].aName[0], fDamageResult);//ダメージ表記
				getchar();
			}

			else
			{
				printf("\x1b[34m攻撃が外れた！\x1b[0m\n");//６０％の確率で攻撃を外させる。
				getchar();
			}
		}

	}
	else if (CheckRandum == 2)
	{
		for (nCount = 0; nCount < 8; nCount++, nAttackSucces = rand() % 10 + 1, fRandum1 = rand() % 25 + 1)//８連続攻撃を行う。（命中率４０％）
		{
			if (nAttackSucces >= 1 && nAttackSucces <= 4)//攻撃成功確率のランダム関数が１～４だったら攻撃を成功させる。
			{
				g_aPlayer[PlayerNumber1].fLife = g_aPlayer[PlayerNumber1].fLife - ((g_aPlayer[PlayerNumber2].fATK * 0.75) - (g_aPlayer[PlayerNumber1].fDEF / 4)) * (1 - (fRandum1 / 100));//プレイヤー2の攻撃力＊３-プレイヤー２の防御力/4＊（１．０１～１．２５）

				fDamageResult = ((g_aPlayer[PlayerNumber2].fATK * 0.75) - (g_aPlayer[PlayerNumber1].fDEF / 4)) * (1 - (fRandum1 / 100));//プレイヤー１の攻撃力＊３-プレイヤー２の防御力/4＊（１．０１～１．２５）


				printf("%sに\x1b[31m%.f\x1b[0mのダメージ！\n", &g_aPlayer[PlayerNumber1].aName[0], fDamageResult);//ダメージ表記
				getchar();
			}

			else
			{
				printf("\x1b[34m攻撃が外れた！\x1b[0m\n");//６０％の確率で攻撃を外させる。
				getchar();
			}
		}

	}

}
//=====================================================================
//プレイヤー１のスキル：パワダウンアロー
//=====================================================================

bool Player1PowerDownArrow(Player* pPlayer)//プレイヤー１が狩人のスキル（２）、「パワダウンアロー」を発動するときに呼び出す関数。
{
	srand((unsigned int)time(NULL));//乱数の種を現在時刻を基準にランダム生成。

	//int nCount;

	//int nTurnAttack;//攻撃ターンの変数

	float fDamageResult;//ダメージの結果を代入する変数

	float fRandum1;

	int CheckRandum;

	int PowerDownSuccess;//攻撃力低下が成功するかを判定する変数。

	bool Player2PowerDownArrowDebuff;//プレイヤー１がパワダウンアローのデバフ（攻撃力３０％低下）のフラグを宣言する。

	Player2PowerDownArrowDebuff = false;

	pPlayer->fMP = pPlayer->fMP - 40;//スキル発動時にプレイヤー１のMPを40減らす。

	printf("%sのパワダウンアロー！\n", &g_aPlayer[PlayerNumber1].aName[0]);

	fRandum1 = rand() % 25 + 1, CheckRandum = rand() % 2 + 1;//最終的なダメージを２５％の範囲で上下させる。ダメージか上がるか下がるかの判断は、CheckRandum変数で判断する。


	if (CheckRandum == 1)
	{
		g_aPlayer[1].fLife = g_aPlayer[PlayerNumber2].fLife - (g_aPlayer[PlayerNumber1].fATK * 2 - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 + (fRandum1 / 100));//(プレイヤー１の攻撃力*2-プレイヤー2の防御力/2)＊（１．０１～１．２５）

		fDamageResult = (g_aPlayer[PlayerNumber1].fATK * 2 - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 + (fRandum1 / 100));//(プレイヤー１の攻撃力*2-プレイヤー2の防御力/2)＊（１．０１～１．２５）


	}
	else if (CheckRandum == 2)
	{
		g_aPlayer[1].fLife = g_aPlayer[PlayerNumber2].fLife - (g_aPlayer[PlayerNumber1].fATK * 2 - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 - (fRandum1 / 100));//(プレイヤー１の攻撃力*2.5-プレイヤー2の防御力/2)＊（０．７５～０．９９）
		fDamageResult = (g_aPlayer[PlayerNumber1].fATK * 2 - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 - (fRandum1 / 100));//(プレイヤー１の攻撃力*2.5-プレイヤー2の防御力/2)＊（０．７５～０．９９）
	}

	printf("%sに\x1b[31m%.f\x1b[0mのダメージ！\n", &g_aPlayer[PlayerNumber2].aName[0], fDamageResult);//ダメージ表記

	PowerDownSuccess = rand() % 5 + 1;//攻撃力低下の成功を判定する。

	if (PowerDownSuccess >= 1 && PowerDownSuccess <= 2)
	{
		pPlayer++;//ポインタ先をプレイヤー２へ
		pPlayer->fATK = pPlayer->fATK - (pPlayer->fATK * 0.5);//攻撃力低下のデバフが成功した場合に、プレイヤー２の攻撃力を３０％低下させる。
		pPlayer--;//ポインタ先をプレイヤー１へ

		printf("%sの\x1b[36m攻撃力が低下した！\x1b[0m（５０％）（２Ｔ）\n", &pPlayer->aName);


		Player2PowerDownArrowDebuff = true;//デバフが発動していることを表すフラグをtrueにする。
	}

	return Player2PowerDownArrowDebuff;//プレイヤー２へパワダウンアローのデバフが発動しているかどうかを返す。
}

//=================================================================
//プレイヤー２のスキル：パワダウンアロー
//=================================================================
bool Player2PowerDownArrow(Player* pPlayer)//プレイヤー2が狩人のスキル（２）、「パワダウンアロー」を発動するときに呼び出す関数。
{
	srand((unsigned int)time(NULL));//乱数の種を現在時刻を基準にランダム生成。

	int nCount;

	int nTurnAttack;//攻撃ターンの変数

	float fDamageResult;//ダメージの結果を代入する変数

	float fRandom1;

	int CheckRandum;

	int PowerDownSuccess;//攻撃力低下が成功するかを判定する変数。

	bool Player1PowerDownArrowDebuff;//プレイヤー２がプレイヤー１へパワダウンアローのデバフ（攻撃力５０％低下）のフラグを宣言する。

	Player1PowerDownArrowDebuff = false;

	pPlayer++;//ポインタ先をプレイヤー２へ
	pPlayer->fMP = pPlayer->fMP - 40;//スキル発動時にプレイヤー１のMPを40減らす。
	pPlayer--;//ポインタ先をプレイヤー１へ

	printf("%sのパワダウンアロー！\n", &g_aPlayer[PlayerNumber2].aName[0]);

	fRandom1 = rand() % 25 + 1;
	CheckRandum = rand() % 2 + 1;//最終的なダメージを２５％の範囲で上下させる。ダメージか上がるか下がるかの判断は、CheckRandum変数で判断する。

	if (CheckRandum == 1)
	{
		g_aPlayer[PlayerNumber1].fLife = g_aPlayer[PlayerNumber1].fLife - (g_aPlayer[PlayerNumber2].fATK * 2 - (g_aPlayer[PlayerNumber1].fDEF / 2)) * (1 + (fRandom1 / 100));//(プレイヤー2の攻撃力*2-プレイヤー1の防御力/2)＊（１．０１～１．２５）

		fDamageResult = (g_aPlayer[PlayerNumber2].fATK * 2 - (g_aPlayer[PlayerNumber1].fDEF / 2)) * (1 + (fRandom1 / 100));//(プレイヤー2の攻撃力*2-プレイヤー1の防御力/2)＊（１．０１～１．２５）


	}
	else if (CheckRandum == 2)
	{
		g_aPlayer[PlayerNumber1].fLife = g_aPlayer[PlayerNumber1].fLife - (g_aPlayer[PlayerNumber2].fATK * 2 - (g_aPlayer[PlayerNumber1].fDEF / 2)) * (1 - (fRandom1 / 100));//(プレイヤー2の攻撃力*2-プレイヤー1の防御力/2)＊（０．７５～０．９９）
		fDamageResult = (g_aPlayer[PlayerNumber2].fATK * 2 - (g_aPlayer[PlayerNumber1].fDEF / 2)) * (1 - (fRandom1 / 100));//(プレイヤー2の攻撃力*2-プレイヤー1の防御力/2)＊（０．７５～０．９９）
	}

	printf("%sに\x1b[31m%.f\x1b[0mのダメージ！\n", &g_aPlayer[PlayerNumber1].aName[0], fDamageResult);//ダメージ表記

	PowerDownSuccess = rand() % 5 + 1;//攻撃力低下の成功を判定する。

	if (PowerDownSuccess >= 1 && PowerDownSuccess <= 2)
	{
		pPlayer->fATK = pPlayer->fATK - (pPlayer->fATK * 0.5);//攻撃力低下のデバフが成功した場合に、プレイヤー1の攻撃力を５０％低下させる。

		printf("%sの\x1b[36m攻撃力が低下した！\x1b[0m（５０％）（２Ｔ）\n", &pPlayer->aName);


		Player1PowerDownArrowDebuff = true;//デバフが発動していることを表すフラグをtrueにする。
	}

	return Player1PowerDownArrowDebuff;//プレイヤー1へパワダウンアローのデバフが発動しているかどうかを返す。
}

//======================================================
//プレイヤー１のスキル：スターアロー
//======================================================
void Player1StarArrow(Player* pPlayer)//プレイヤー１が狩人のスキル（３）、「スターアロー」を発動するときに呼び出す関数。
{
	srand((unsigned int)time(NULL));//乱数の種を現在時刻を基準にランダム生成。

	float fRandum1;
	int CheckRandum;
	float fDamageResult;

	pPlayer->fMP = pPlayer->fMP - 60;//スキル発動時にプレイヤー１のMPを60減らす。

	printf("%sのスターアロー！\n", &g_aPlayer[0].aName[0]);

	fRandum1 = rand() % 25 + 1, CheckRandum = rand() % 2 + 1;//最終的なダメージを２５％の範囲で上下させる。ダメージか上がるか下がるかの判断は、CheckRandum変数で判断する。


	if (CheckRandum == 1)
	{
		g_aPlayer[PlayerNumber2].fLife = g_aPlayer[PlayerNumber2].fLife - (g_aPlayer[PlayerNumber1].fATK * 5) * (1 + (fRandum1 / 100));//(プレイヤー１の攻撃力*4.5＊（１．０１～１．２５）

		fDamageResult = (g_aPlayer[PlayerNumber1].fATK * 5) * (1 + (fRandum1 / 100));//(プレイヤー１の攻撃力*5＊（１．０１～１．２５）


	}
	else if (CheckRandum == 2)
	{
		g_aPlayer[PlayerNumber2].fLife = g_aPlayer[PlayerNumber2].fLife - (g_aPlayer[PlayerNumber1].fATK * 5) * (1 - (fRandum1 / 100));//(プレイヤー１の攻撃力*4.5＊（０．７５～０．９９）

		fDamageResult = (g_aPlayer[PlayerNumber1].fATK * 5) * (1 - (fRandum1 / 100));//(プレイヤー１の攻撃力*4.5＊（１．０１～１．２５）
	}

	printf("%sに\x1b[31m%.f\x1b[0mのダメージ！\n", &g_aPlayer[PlayerNumber2].aName[0], fDamageResult);//ダメージ表記
}

//============================================================================
//プレイヤー２のスキル「スターアロー」
//============================================================================
void Player2StarArrow(Player* pPlayer)//プレイヤー２が狩人のスキル（３）、「スターアロー」を発動するときに呼び出す関数。
{
	srand((unsigned int)time(NULL));//乱数の種を現在時刻を基準にランダム生成。

	float fRandum1;
	int CheckRandum;
	float fDamageResult;

	pPlayer++;//ポインタ先をプレイヤー２へ
	pPlayer->fMP = pPlayer->fMP - 60;//スキル発動時にプレイヤー１のMPを60減らす。
	pPlayer--;//ポインタ先をプレイヤー１へ

	printf("%sのスターアロー！\n", &g_aPlayer[PlayerNumber2].aName[0]);

	fRandum1 = rand() % 25 + 1, CheckRandum = rand() % 2 + 1;//最終的なダメージを２５％の範囲で上下させる。ダメージか上がるか下がるかの判断は、CheckRandum変数で判断する。


	if (CheckRandum == 1)
	{
		g_aPlayer[PlayerNumber1].fLife = g_aPlayer[PlayerNumber1].fLife - (g_aPlayer[PlayerNumber2].fATK * 5) * (1 + (fRandum1 / 100));//(プレイヤー2の攻撃力*5＊（１．０１～１．２５）

		fDamageResult = (g_aPlayer[PlayerNumber2].fATK * 5) * (1 + (fRandum1 / 100));//(プレイヤー2の攻撃力*5＊（１．０１～１．２５）


	}
	else if (CheckRandum == 2)
	{
		g_aPlayer[PlayerNumber1].fLife = g_aPlayer[PlayerNumber1].fLife - (g_aPlayer[PlayerNumber2].fATK * 5) * (1 - (fRandum1 / 100));//(プレイヤー2の攻撃力*5＊（０．７５～０．９９）

		fDamageResult = (g_aPlayer[PlayerNumber2].fATK * 5) * (1 - (fRandum1 / 100));//(プレイヤー2の攻撃力*5＊（０．７５～０．９９）
	}

	printf("%sに\x1b[31m%.f\x1b[0mのダメージ！\n", &g_aPlayer[PlayerNumber1].aName[0], fDamageResult);//ダメージ表記
}

//============================================================================
//プレイヤー１のスキル「守りの陣」
//============================================================================
void Player1DefenceLine(Player* pPlayer)//プレイヤー１が巫女のスキル（１）、「守りの陣」を発動するときに呼び出す関数。
{
	float fRandum1;
	int CheckRandum;
	float fDamageResult;

	pPlayer->fMP = pPlayer->fMP - 35;//スキル発動時にプレイヤー１のMPを60減らす。


	printf("%sの守りの陣！\n", &g_aPlayer[PlayerNumber1].aName[0]);

	pPlayer->fDEF = pPlayer->fDEF + (pPlayer->fDEF * 0.4);//防御力を＋４０％

	printf("%sの\x1b[33m防御力が４０％上昇した！\x1b[0m\n", &pPlayer->aName[0]);//バフを受けたことの表記
}

//=================================================================================
//プレイヤー２のスキル「守りの陣」
//=================================================================================
void Player2DefenceLine(Player* pPlayer)//プレイヤー２が巫女のスキル（１）、「守りの陣」を発動するときに呼び出す関数。
{
	float fRandum1;
	int CheckRandum;
	float fDamageResult;

	pPlayer++;//ポインタ先をプレイヤー２へ
	pPlayer->fMP = pPlayer->fMP - 35;//スキル発動時にプレイヤー２のMPを60減らす。


	printf("%sの守りの陣！\n", &pPlayer->aName[0]);

	pPlayer->fDEF = pPlayer->fDEF + (pPlayer->fDEF * 0.4);//防御力を＋４０％

	printf("%sの\x1b[33m防御力が４０％上昇した！\x1b[0m\n", &pPlayer->aName[0]);//バフを受けたことの表記
	pPlayer--;//ポインタ先をプレイヤー１へ
}

//==============================================================================
//プレイヤー１のスキル「ホーリーレイ」
//==============================================================================
void Player1HolyRay(Player* pPlayer)//プレイヤー１が巫女のスキル（２）、「ホーリーレイ」を発動するときに呼び出す関数。
{
	srand((unsigned int)time(NULL));//乱数の種を現在時刻を基準にランダム生成。

	float fRandum1;
	int CheckRandum;
	float fDamageResult;

	pPlayer->fMP = pPlayer->fMP - 45;//スキル発動時にプレイヤー１のMPを５０減らす。

	printf("%sのホーリーレイ！\n", &g_aPlayer[PlayerNumber1].aName[0]);

	fRandum1 = rand() % 25 + 1, CheckRandum = rand() % 2 + 1;//最終的なダメージを２５％の範囲で上下させる。ダメージか上がるか下がるかの判断は、CheckRandum変数で判断する。


	if (CheckRandum == 1)
	{
		g_aPlayer[PlayerNumber2].fLife = g_aPlayer[PlayerNumber2].fLife - ((g_aPlayer[PlayerNumber1].fATK + g_aPlayer[PlayerNumber1].fDEF) * 1.2 - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 + (fRandum1 / 100));//((プレイヤー１の攻撃力＋プレイヤー１の防御力-プレイヤー2の防御力/2))＊（１．０１～１．２５）

		fDamageResult = ((g_aPlayer[PlayerNumber1].fATK + g_aPlayer[PlayerNumber1].fDEF) * 1.2 - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 + (fRandum1 / 100));//((プレイヤー１の攻撃力＋プレイヤー１の防御力)*1.2-プレイヤー2の防御力/2))＊（１．０１～１．２５）


	}
	else if (CheckRandum == 2)
	{
		g_aPlayer[PlayerNumber2].fLife = g_aPlayer[PlayerNumber2].fLife - ((g_aPlayer[PlayerNumber1].fATK + g_aPlayer[PlayerNumber1].fDEF) * 1.2 - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 - (fRandum1 / 100));//((プレイヤー１の攻撃力＋プレイヤー１の防御力-プレイヤー2の防御力/2))＊（０．７５～０．９９）
		fDamageResult = ((g_aPlayer[PlayerNumber1].fATK + g_aPlayer[PlayerNumber1].fDEF) * 1.2 - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 - (fRandum1 / 100));//((プレイヤー１の攻撃力＋プレイヤー１の防御力-プレイヤー2の防御力/2))＊（１．０１～１．２５）
	}

	printf("%sに\x1b[31m%.f\x1b[0mのダメージ！\n", &g_aPlayer[PlayerNumber2].aName[0], fDamageResult);//ダメージ表記


	pPlayer++;//ポインタ先をプレイヤー２へ
	pPlayer->fMP = pPlayer->fMP - 15;//プレイヤー２のＭＰを１５減らす。
	printf("%sの\x1b[32mMPが１５減った！\x1b[0m\n", &g_aPlayer[PlayerNumber2].aName[0]);
}

//=======================================================================================
//プレイヤー２のスキル「ホーリーレイ」
//=======================================================================================
void Player2HolyRay(Player* pPlayer)//プレイヤー２が巫女のスキル（２）、「ホーリーレイ」を発動するときに呼び出す関数。
{
	srand((unsigned int)time(NULL));//乱数の種を現在時刻を基準にランダム生成。

	float fRandum1;
	int CheckRandum;
	float fDamageResult;

	pPlayer++;//ポインタ先をプレイヤー２へ
	pPlayer->fMP = pPlayer->fMP - 45;//スキル発動時にプレイヤー２のMPを５０減らす。
	pPlayer--;//ポインタ先をプレイヤー１へ

	printf("%sのホーリーレイ！\n", &g_aPlayer[PlayerNumber2].aName[0]);

	fRandum1 = rand() % 25 + 1, CheckRandum = rand() % 2 + 1;//最終的なダメージを２５％の範囲で上下させる。ダメージか上がるか下がるかの判断は、CheckRandum変数で判断する。


	if (CheckRandum == 1)
	{
		g_aPlayer[PlayerNumber1].fLife = g_aPlayer[PlayerNumber1].fLife - ((g_aPlayer[PlayerNumber2].fATK + g_aPlayer[PlayerNumber2].fDEF) * 1.2 - (g_aPlayer[PlayerNumber1].fDEF / 2)) * (1 + (fRandum1 / 100));//((プレイヤー２の攻撃力＋プレイヤー２の防御力-プレイヤー2の防御力/2))＊（１．０１～１．２５）

		fDamageResult = ((g_aPlayer[PlayerNumber2].fATK + g_aPlayer[PlayerNumber2].fDEF) * 1.2 - (g_aPlayer[PlayerNumber1].fDEF / 2)) * (1 + (fRandum1 / 100));//((プレイヤー２の攻撃力＋プレイヤー２の防御力)*1.2-プレイヤー2の防御力/2))＊（１．０１～１．２５）


	}
	else if (CheckRandum == 2)
	{
		g_aPlayer[PlayerNumber1].fLife = g_aPlayer[PlayerNumber1].fLife - ((g_aPlayer[PlayerNumber2].fATK + g_aPlayer[PlayerNumber2].fDEF) * 1.2 - (g_aPlayer[PlayerNumber1].fDEF / 2)) * (1 - (fRandum1 / 100));//((プレイヤー１の攻撃力＋プレイヤー１の防御力-プレイヤー2の防御力/2))＊（０．７５～０．９９）
		fDamageResult = ((g_aPlayer[PlayerNumber2].fATK + g_aPlayer[PlayerNumber2].fDEF) * 1.2 - (g_aPlayer[PlayerNumber1].fDEF / 2)) * (1 - (fRandum1 / 100));//((プレイヤー１の攻撃力＋プレイヤー１の防御力-プレイヤー2の防御力/2))＊（１．０１～１．２５）
	}

	printf("%sに\x1b[31m%.f\x1b[0mのダメージ！\n", &g_aPlayer[PlayerNumber1].aName[0], fDamageResult);//ダメージ表記



	pPlayer->fMP = pPlayer->fMP - 15;//プレイヤー１のＭＰを１５減らす。
	printf("%sの\x1b[32mMPが１５減った！\x1b[0m\n", &g_aPlayer[PlayerNumber1].aName[0]);
}

//===========================================================================
//プレイヤー１のスキル：ブラッディバイト
//===========================================================================
void Player1BloodyBite(Player* pPlayer)//プレイヤー１が吸血鬼のスキル（１）、「ブラッティバイト」を発動するときに呼び出す関数。
{
	srand((unsigned int)time(NULL));//乱数の種を現在時刻を基準にランダム生成。

	float fRandum1;
	int CheckRandum;
	float fDamageResult;

	pPlayer->fMP = pPlayer->fMP - 35;//スキル発動時にプレイヤー１のMPを35減らす。

	printf("%sのブラッディバイト！\n", &g_aPlayer[PlayerNumber1].aName[0]);

	fRandum1 = rand() % 25 + 1, CheckRandum = rand() % 2 + 1;//最終的なダメージを２５％の範囲で上下させる。ダメージか上がるか下がるかの判断は、CheckRandum変数で判断する。


	if (CheckRandum == 1)
	{
		g_aPlayer[PlayerNumber2].fLife = g_aPlayer[PlayerNumber2].fLife - (g_aPlayer[PlayerNumber1].fATK * 2.2 - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 + (fRandum1 / 100));//(プレイヤー１の攻撃力*2.2-プレイヤー2の防御力/2)＊（１．０１～１．２５）

		fDamageResult = (g_aPlayer[PlayerNumber1].fATK * 2.2 - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 + (fRandum1 / 100));//(プレイヤー１の攻撃力*2.2-プレイヤー2の防御力/2)＊（１．０１～１．２５）


	}
	else if (CheckRandum == 2)
	{
		g_aPlayer[PlayerNumber2].fLife = g_aPlayer[PlayerNumber2].fLife - (g_aPlayer[PlayerNumber1].fATK * 2.2 - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 - (fRandum1 / 100));//(プレイヤー１の攻撃力*2.2-プレイヤー2の防御力/2)＊（０．７５～０．９９）
		fDamageResult = (g_aPlayer[PlayerNumber1].fATK * 2.2 - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 - (fRandum1 / 100));//(プレイヤー１の攻撃力*2.2-プレイヤー2の防御力/2)＊（０．７５～０．９９）
	}
	printf("%sに\x1b[31m%.f\x1b[0mのダメージ！\n", &g_aPlayer[PlayerNumber2].aName[0], fDamageResult);//ダメージ表記

	printf("与えたダメージの\x1b[32m７５％\x1b[0mＨＰを回復する！\n");
	pPlayer->fLife = pPlayer->fLife + (fDamageResult * 0.75);//プレイヤー１が与えたダメージの０．７５倍ＨＰを回復する。
}

//====================================================================================
//プレイヤー２のスキル：ブラッディバイト
//====================================================================================
void Player2BloodyBite(Player* pPlayer)//プレイヤー２が吸血鬼のスキル（１）、「ブラッディバイトを発動するときに呼び出す関数。
{
	srand((unsigned int)time(NULL));//乱数の種を現在時刻を基準にランダム生成。

	float fRandum1;
	int CheckRandum;
	float fDamageResult;

	pPlayer++;//ポインタ先をプレイヤー２へ
	pPlayer->fMP = pPlayer->fMP - 35;//スキル発動時にプレイヤー２のMPを35減らす。
	pPlayer--;//ポインタ先をプレイヤー１へ

	printf("%sのブラッディバイト！\n", &g_aPlayer[PlayerNumber2].aName[0]);

	fRandum1 = rand() % 25 + 1, CheckRandum = rand() % 2 + 1;//最終的なダメージを２５％の範囲で上下させる。ダメージか上がるか下がるかの判断は、CheckRandum変数で判断する。


	if (CheckRandum == 1)
	{
		g_aPlayer[PlayerNumber1].fLife = g_aPlayer[PlayerNumber1].fLife - (g_aPlayer[PlayerNumber2].fATK * 2.2 - (g_aPlayer[PlayerNumber1].fDEF / 2)) * (1 + (fRandum1 / 100));//(プレイヤー２の攻撃力*2.2-プレイヤー１の防御力/2)＊（１．０１～１．２５）

		fDamageResult = (g_aPlayer[PlayerNumber2].fATK * 2.2 - (g_aPlayer[PlayerNumber1].fDEF / 2)) * (1 + (fRandum1 / 100));//(プレイヤー２の攻撃力*2.2-プレイヤー１の防御力/2)＊（１．０１～１．２５）


	}
	else if (CheckRandum == 2)
	{
		g_aPlayer[PlayerNumber1].fLife = g_aPlayer[PlayerNumber1].fLife - (g_aPlayer[PlayerNumber2].fATK * 2.2 - (g_aPlayer[PlayerNumber1].fDEF / 2)) * (1 - (fRandum1 / 100));//(プレイヤー２の攻撃力*2.2-プレイヤー１の防御力/2)＊（０．７５～０．９９）
		fDamageResult = (g_aPlayer[PlayerNumber2].fATK * 2.2 - (g_aPlayer[PlayerNumber1].fDEF / 2)) * (1 - (fRandum1 / 100));//(プレイヤー２の攻撃力*2.2-プレイヤー１の防御力/2)＊（０．７５～０．９９）
	}
	printf("%sに\x1b[31m%.f\x1b[0mのダメージ！\n", &g_aPlayer[PlayerNumber1].aName[0], fDamageResult);//ダメージ表記

	printf("与えたダメージの\x1b[32m７５％\x1b[0mＨＰを回復する！\n");
	pPlayer++;//ポインタ先をプレイヤー２へ
	pPlayer->fLife = pPlayer->fLife + (fDamageResult * 0.75);//プレイヤー２が与えたダメージの０．７５倍ＨＰを回復する。
	pPlayer--;//ポインタ先をプレイヤー１へ
}

//===================================================================
//プレイヤー１のスキル：クレイジーグラビティ
//===================================================================
void Player1CrazyGracity(Player* pPlayer)//プレイヤー１が吸血鬼のスキル（２）、「クレイジーグラビティ」を発動するときに呼び出す関数。
{
	float fRandum1;
	//int CheckRandum;
	//float fDamageResult;

	pPlayer->fMP = pPlayer->fMP - 65;//スキル発動時にプレイヤー1のMPを70減らす。

	fRandum1 = rand() % 10 + 1;//２種類の効果が確率でどちらかが発動する。

	printf("%sのクレイジーグラビティ！\n", &g_aPlayer[PlayerNumber1].aName[0]);

	if (fRandum1 >= 1 && fRandum1 <= 3)//効果が３０％の確率で発動する。
	{
		pPlayer++;//ポインタ先をプレイヤー２へ
		pPlayer->fLife = pPlayer->fLife - (pPlayer->fLife * 0.2);//プレイヤー２の現在ＨＰを２０％減らす、
		printf("%sの体力が\x1b[35m現在ＨＰ２０％分減少した！\x1b[0m\n", &pPlayer->aName);
		pPlayer--;//ポインタ先をプレイヤー１へ
	}
	else
	{
		pPlayer->fLife = pPlayer->fLife + 1500;//プレイヤー１の体力を１５００回復する。
		printf("%sの体力が\x1b[32m１５００\x1b[0m回復した！\n", &pPlayer->aName);
	}

}

//===============================================================================
//プレイヤー２のスキル：クレイジーグラビティ
//===============================================================================
void Player2CrazyGravity(Player* pPlayer)//プレイヤー２が吸血鬼のスキル（２）、「クレイジーグラビティ」を発動するときに呼び出す関数。
{
	float fRandum1;
	//int CheckRandum;
	//float fDamageResult;

	pPlayer++;//ポインタ先をプレイヤー２へ
	pPlayer->fMP = pPlayer->fMP - 65;//スキル発動時にプレイヤー1のMPを70減らす。
	pPlayer--;//ポインタ先をプレイヤー１へ

	fRandum1 = rand() % 10 + 1;//２種類の効果を確率でどちらかを発動させるために呼び出す変数。

	printf("%sのクレイジーグラビティ！\n", &g_aPlayer[PlayerNumber2].aName[0]);

	if (fRandum1 >= 1 && fRandum1 <= 3)//効果が３０％の確率で発動する。
	{
		pPlayer->fLife = pPlayer->fLife - (pPlayer->fLife * 0.2);//プレイヤー１の現在ＨＰを２０％減らす、
		printf("%sの体力が\x1b[35m現在ＨＰ２０％分減少した！\x1b[0m\n", &pPlayer->aName);
	}
	else
	{
		pPlayer++;//ポインタ先をプレイヤー２へ
		pPlayer->fLife = pPlayer->fLife + 1500;//プレイヤー１の体力を１５００回復する。
		printf("%sの体力が\x1b[32m１５００\x1b[0m回復した！\n", &pPlayer->aName);
		pPlayer--;//ポインタ先をプレイヤー１へ
	}

}

//======================================================================================
//プレイヤー１のスキル：ファントムスタブ
//======================================================================================
bool Player1PhantomStab(Player* pPlayer)//プレイヤー１が吸血鬼のスキル（３）、「ファントムスタブ」を発動するときに呼び出す関数。
{
	srand((unsigned int)time(NULL));//乱数の種を現在時刻を基準にランダム生成。

	float fRandum1;
	int CheckRandum;
	float fDamageResult;

	int SkillDebuffRandum;//スキル不可を与える確率を扱う関数を宣言する。
	bool bPlayre2PhantomStabDebuff = false;//プレイヤー２にスキル不可を与えるフラグをfalseにする。

	pPlayer->fMP = pPlayer->fMP - 50;//スキル発動時にプレイヤー１のMPを50減らす。

	printf("%sのファントムスタブ！\n", &g_aPlayer[PlayerNumber1].aName[0]);

	fRandum1 = rand() % 25 + 1;
	CheckRandum = rand() % 2 + 1;
	SkillDebuffRandum = rand() % 20 + 1;//最終的なダメージを２５％の範囲で上下させる。ダメージか上がるか下がるかの判断は、CheckRandum変数で判断する。

	if (CheckRandum == 1)
	{
		g_aPlayer[PlayerNumber2].fLife = g_aPlayer[PlayerNumber2].fLife - (g_aPlayer[PlayerNumber1].fATK * 3.1 - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 + (fRandum1 / 100));//(プレイヤー１の攻撃力*3.1-プレイヤー2の防御力/2)＊（１．０１～１．２５）

		fDamageResult = (g_aPlayer[PlayerNumber1].fATK * 3.1 - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 + (fRandum1 / 100));//(プレイヤー１の攻撃力*3.1-プレイヤー2の防御力/2)＊（１．０１～１．２５）


	}
	else if (CheckRandum == 2)
	{
		g_aPlayer[PlayerNumber2].fLife = g_aPlayer[PlayerNumber2].fLife - (g_aPlayer[PlayerNumber1].fATK * 3.1 - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 - (fRandum1 / 100));//(プレイヤー１の攻撃力*3.1-プレイヤー2の防御力/2)＊（０．７５～０．９９）
		fDamageResult = (g_aPlayer[PlayerNumber1].fATK * 3.1 - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 - (fRandum1 / 100));//(プレイヤー１の攻撃力*3.1-プレイヤー2の防御力/2)＊（０．７５～０．９９）
	}
	printf("%sに\x1b[31m%.f\x1b[0mのダメージ！\n", &g_aPlayer[PlayerNumber2].aName[0], fDamageResult);//ダメージ表記

	if (SkillDebuffRandum >= 1 && SkillDebuffRandum <= 7)//３５％の確率でスキル不可（３Ｔ）を発動する。
	{
		bPlayre2PhantomStabDebuff = true;//プレイヤー２にスキル不可を与えるフラグをtrueにする。
		printf("%sは３ターンの間、\x1b[35mスキルが使用できなくなった！\x1b[0m\n", &g_aPlayer[PlayerNumber2].aName[0]);
	}
	return bPlayre2PhantomStabDebuff;//プレイヤー２がスキル「ファントムスタブ」のスキル不可のデバフを受けているかどうかのフラグを返す。
}

//==========================================================
//プレイヤー２のスキル：ファントムスタブ
//==========================================================
bool Player2PhantomStab(Player* pPlayer)//プレイヤー２が吸血鬼のスキル（３）、「ファントムスタブ」を発動するときに呼び出す関数。
{
	srand((unsigned int)time(NULL));//乱数の種を現在時刻を基準にランダム生成。

	float fRandum1;
	int CheckRandum;
	float fDamageResult;

	int SkillDebuffRandum;//スキル不可を与える確率を扱う関数を宣言する。
	bool bPlayre1PhantomStabDebuff = false;//プレイヤー１にスキル不可を与えるフラグをfalseにする。

	pPlayer++;//ポインタ先をプレイヤー２へ
	pPlayer->fMP = pPlayer->fMP - 50;//スキル発動時にプレイヤー２のMPを50減らす。
	pPlayer--;//ポインタ先をプレイヤー１へ

	printf("%sのファントムスタブ！\n", &g_aPlayer[PlayerNumber2].aName[0]);

	fRandum1 = rand() % 25 + 1, CheckRandum = rand() % 2 + 1, SkillDebuffRandum = rand() % 20 + 1;//最終的なダメージを２５％の範囲で上下させる。ダメージか上がるか下がるかの判断は、CheckRandum変数で判断する。


	if (CheckRandum == 1)
	{
		g_aPlayer[PlayerNumber1].fLife = g_aPlayer[PlayerNumber1].fLife - (g_aPlayer[PlayerNumber2].fATK * 3.1 - (g_aPlayer[PlayerNumber1].fDEF / 2)) * (1 + (fRandum1 / 100));//(プレイヤー２の攻撃力*3.1-プレイヤー１の防御力/2)＊（１．０１～１．２５）

		fDamageResult = (g_aPlayer[PlayerNumber2].fATK * 3.1 - (g_aPlayer[PlayerNumber1].fDEF / 2)) * (1 + (fRandum1 / 100));//(プレイヤー２の攻撃力*3.1-プレイヤー１の防御力/2)＊（１．０１～１．２５）


	}
	else if (CheckRandum == 2)
	{
		g_aPlayer[PlayerNumber1].fLife = g_aPlayer[PlayerNumber1].fLife - (g_aPlayer[PlayerNumber2].fATK * 3.1 - (g_aPlayer[PlayerNumber1].fDEF / 2)) * (1 - (fRandum1 / 100));//(プレイヤー２の攻撃力*3.1-プレイヤー１の防御力/2)＊（０．７５～０．９９）
		fDamageResult = (g_aPlayer[PlayerNumber2].fATK * 3.1 - (g_aPlayer[PlayerNumber1].fDEF / 2)) * (1 - (fRandum1 / 100));//(プレイヤー２の攻撃力*3.1-プレイヤー１の防御力/2)＊（０．７５～０．９９）
	}
	printf("%sに\x1b[31m%.f\x1b[0mのダメージ！\n", &g_aPlayer[PlayerNumber1].aName[0], fDamageResult);//ダメージ表記

	if (SkillDebuffRandum >= 1 && SkillDebuffRandum <= 7)//３５％の確率でスキル不可（３Ｔ）を発動する。
	{
		bPlayre1PhantomStabDebuff = true;//プレイヤー１にスキル不可を与えるフラグをtrueにする。
		printf("%sは３ターンの間、\x1b[35mスキルが使用できなくなった！\x1b[0m\n", &g_aPlayer[PlayerNumber1].aName[0]);
	}
	return bPlayre1PhantomStabDebuff;//プレイヤー１がスキル「ファントムスタブ」のスキル不可のデバフを受けているかどうかのフラグを返す。
}

//============================================================
//プレイヤー１のスキル：ポイズンミスト
//============================================================
bool Player1PoisonMist(Player* pPlayer)//プレイヤー１が科学者のスキル（１）、「ポイズンミスト」を発動するときに呼び出す関数。
{
	srand((unsigned int)time(NULL));//乱数の種を現在時刻を基準にランダム生成。

	float fRandum1;
	int CheckRandum;
	float fDamageResult;
	bool bPlayer2PoisonMistState;//プレイヤー２へ毒状態を与えるフラグを宣言する。
	bPlayer2PoisonMistState = false;//フラグをfalseにする。
	int nPoisonStateRandum;//毒状態にする確率を操作する変数。

	pPlayer->fMP = pPlayer->fMP - 40;//スキル発動時にプレイヤー１のMPを40減らす。

	printf("%sのポイズンミスト！\n", &g_aPlayer[PlayerNumber1].aName[0]);

	fRandum1 = rand() % 25 + 1, CheckRandum = rand() % 2 + 1, nPoisonStateRandum = rand() % 20 + 1;//最終的なダメージを２５％の範囲で上下させる。ダメージか上がるか下がるかの判断は、CheckRandum変数で判断する。


	if (CheckRandum == 1)
	{
		g_aPlayer[PlayerNumber2].fLife = g_aPlayer[PlayerNumber2].fLife - (g_aPlayer[PlayerNumber1].fATK * 2.7 - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 + (fRandum1 / 100));//(プレイヤー１の攻撃力*2.7-プレイヤー2の防御力/2)＊（１．０１～１．２５）

		fDamageResult = (g_aPlayer[PlayerNumber1].fATK * 2.7 - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 + (fRandum1 / 100));//(プレイヤー１の攻撃力*2.7-プレイヤー2の防御力/2)＊（１．０１～１．２５）


	}
	else if (CheckRandum == 2)
	{
		g_aPlayer[PlayerNumber2].fLife = g_aPlayer[PlayerNumber2].fLife - (g_aPlayer[PlayerNumber1].fATK * 2.7 - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 - (fRandum1 / 100));//(プレイヤー１の攻撃力*2.7-プレイヤー2の防御力/2)＊（０．７５～０．９９）
		fDamageResult = (g_aPlayer[PlayerNumber1].fATK * 2.7 - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 - (fRandum1 / 100));//(プレイヤー１の攻撃力*2.7-プレイヤー2の防御力/2)＊（０．７５～０．９９）
	}
	printf("%sに\x1b[31m%.f\x1b[0mのダメージ！\n", &g_aPlayer[PlayerNumber2].aName[0], fDamageResult);//ダメージ表記

	if (nPoisonStateRandum >= 1 && nPoisonStateRandum <= 7)
	{
		bPlayer2PoisonMistState = true;//プレイヤー２を毒状態にするフラグをtrueにする。
		printf("%sは３ターンの間、\x1b[35n毒状態になった！\x1b[0m（お互いのプレイヤーが行動する度、現在HPの３％ダメージ＆攻撃力２０％DOWN）", &g_aPlayer[PlayerNumber2].aName[0]);
	}
	return bPlayer2PoisonMistState;//プレイヤー２がスキル「ポイズンミスト」の毒状態を受けているかどうかのフラグを返す。
}

//=================================================
//プレイヤー２のスキル：ポイズンミスト
//=================================================
bool Player2PoisonMist(Player* pPlayer)//プレイヤー２が科学者のスキル（１）、「ポイズンミスト」を発動するときに呼び出す関数。
{
	srand((unsigned int)time(NULL));//乱数の種を現在時刻を基準にランダム生成。

	float fRandum1;
	int CheckRandum;
	float fDamageResult;
	bool bPlayer1PoisonMistState;//プレイヤー１へ毒状態を与えるフラグを宣言する。
	bPlayer1PoisonMistState = false;//フラグをfalseにする。
	int nPoisonStateRandum;//毒状態にする確率を操作する変数。

	pPlayer++;//ポインタ先をプレイヤー２へ
	pPlayer->fMP = pPlayer->fMP - 40;//スキル発動時にプレイヤー２のMPを40減らす。
	pPlayer--;//ポインタ先をプレイヤー１へ

	printf("%sのポイズンミスト！\n", &g_aPlayer[PlayerNumber1].aName[0]);

	fRandum1 = rand() % 25 + 1, CheckRandum = rand() % 2 + 1, nPoisonStateRandum = rand() % 20 + 1;//最終的なダメージを２５％の範囲で上下させる。ダメージか上がるか下がるかの判断は、CheckRandum変数で判断する。


	if (CheckRandum == 1)
	{
		g_aPlayer[PlayerNumber1].fLife = g_aPlayer[PlayerNumber1].fLife - (g_aPlayer[PlayerNumber2].fATK * 2.7 - (g_aPlayer[PlayerNumber1].fDEF / 2)) * (1 + (fRandum1 / 100));//(プレイヤー２の攻撃力*2.7-プレイヤー１の防御力/2)＊（１．０１～１．２５）

		fDamageResult = (g_aPlayer[PlayerNumber2].fATK * 2.7 - (g_aPlayer[PlayerNumber1].fDEF / 2)) * (1 + (fRandum1 / 100));//(プレイヤー２の攻撃力*2.7-プレイヤー１の防御力/2)＊（１．０１～１．２５）


	}
	else if (CheckRandum == 2)
	{
		g_aPlayer[PlayerNumber1].fLife = g_aPlayer[PlayerNumber1].fLife - (g_aPlayer[PlayerNumber2].fATK * 2.7 - (g_aPlayer[PlayerNumber1].fDEF / 2)) * (1 - (fRandum1 / 100));//(プレイヤー２の攻撃力*2.7-プレイヤー１の防御力/2)＊（０．７５～０．９９）
		fDamageResult = (g_aPlayer[PlayerNumber2].fATK * 2.7 - (g_aPlayer[PlayerNumber1].fDEF / 2)) * (1 - (fRandum1 / 100));//(プレイヤー２の攻撃力*2.7-プレイヤー１の防御力/2)＊（０．７５～０．９９）
	}
	printf("%sに\x1b[31m%.f\x1b[0mのダメージ！\n", &g_aPlayer[PlayerNumber1].aName[0], fDamageResult);//ダメージ表記

	if (nPoisonStateRandum >= 1 && nPoisonStateRandum <= 7)
	{
		bPlayer1PoisonMistState = true;//プレイヤー１を毒状態にするフラグをtrueにする。
		printf("%sは３ターンの間、\x1b[35m毒状態になった！\x1b[0m（お互いのプレイヤーが行動する度、現在HPの３％のダメージ＆攻撃力２０％DOWN）\n", &g_aPlayer[PlayerNumber1].aName[0]);
	}
	return bPlayer1PoisonMistState;//プレイヤー１がスキル「ポイズンミスト」の毒状態を受けているかどうかのフラグを返す。
}

//================================================
//プレイヤー１のスキル：スチームバースト
//================================================
bool Player1SteamBurst(Player* pPlayer)//プレイヤー１が科学者のスキル（２）、「スチームバースト」を発動するときに呼び出す関数。
{
	srand((unsigned int)time(NULL));//乱数の種を現在時刻を基準にランダム生成。

	float fRandum1;
	int CheckRandum;
	float fDamageResult;

	bool bPlayer1SteamBurstBurn = false;//プレイヤー１がスキル「スチームバースト」のデメリット効果「火傷」を受けていることを表すフラグを宣言する。
	pPlayer->fMP = pPlayer->fMP - 60;//スキル発動時にプレイヤー１のMPを60減らす。

	int nSteamBurstBurnRandum;//スキル「スチームバースト」のデメリット効果を起こす確率を操作する変数を宣言する。

	printf("%sのスチームバースト！\n", &g_aPlayer[PlayerNumber1].aName[0]);

	fRandum1 = rand() % 25 + 1, CheckRandum = rand() % 2 + 1, nSteamBurstBurnRandum = rand() % 2 + 1;//最終的なダメージを２５％の範囲で上下させる。ダメージか上がるか下がるかの判断は、CheckRandum変数で判断する。


	if (nSteamBurstBurnRandum == 1)
	{
		if (CheckRandum == 1)
		{
			g_aPlayer[PlayerNumber2].fLife = g_aPlayer[PlayerNumber2].fLife - ((g_aPlayer[PlayerNumber1].fATK + g_aPlayer[PlayerNumber2].fDEF) * 4.5 - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 + (fRandum1 / 100));//(プレイヤー１の攻撃力＋プレイヤー２の防御力)*4-プレイヤー2の防御力/2)＊（１．０１～１．２５）

			fDamageResult = ((g_aPlayer[PlayerNumber1].fATK + g_aPlayer[PlayerNumber2].fDEF) * 4.5 - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 + (fRandum1 / 100));//(プレイヤー１の攻撃力＋プレイヤー２の防御力)*4-プレイヤー2の防御力/2)＊（１．０１～１．２５）


		}
		else if (CheckRandum == 2)
		{
			g_aPlayer[PlayerNumber2].fLife = g_aPlayer[PlayerNumber2].fLife - ((g_aPlayer[PlayerNumber1].fATK + g_aPlayer[PlayerNumber2].fDEF) * 4.5 - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 - (fRandum1 / 100));//(プレイヤー１の攻撃力＋プレイヤー２の防御力)*4-プレイヤー2の防御力/2)＊（０．７５～０．９９）

			fDamageResult = ((g_aPlayer[PlayerNumber1].fATK + g_aPlayer[PlayerNumber2].fDEF) * 4.5 - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 - (fRandum1 / 100));//(プレイヤー１の攻撃力＋プレイヤー２の防御力)*4-プレイヤー2の防御力/2)＊（０．７５～０．９９）

		}
		printf("%sに\x1b[31m%.f\x1b[0mのダメージ！\n", &g_aPlayer[PlayerNumber2].aName[0], fDamageResult);//ダメージ表記
	}

	else if (nSteamBurstBurnRandum == 2)
	{
		bPlayer1SteamBurstBurn = true;//プレイヤー１のスキル「スチームバースト」が失敗＆デメリット効果「火傷３ターン」を付与させるので、フラグをtrueにする。
		printf("%sのスチームバーストが暴発した！攻撃が失敗＆３ターンの間\x1b[31m火傷状態\x1b[0mになった！（お互いのプレイヤーが行動する度、現在HPの３％ダメージ＆防御力２０％DOWN）", &g_aPlayer[PlayerNumber1].aName[0]);
	}

	return bPlayer1SteamBurstBurn;
}

//==============================================================
//プレイヤー２のスキル：スチームバースト
//==============================================================
bool Player2SteamBurst(Player* pPlayer)//プレイヤー２が科学者のスキル（２）、「スチームバースト」を発動するときに呼び出す関数。
{
	srand((unsigned int)time(NULL));//乱数の種を現在時刻を基準にランダム生成。

	float fRandum1;
	int CheckRandum;
	float fDamageResult;

	bool bPlayer2SteamBurstBurn = false;//プレイヤー２がスキル「スチームバースト」のデメリット効果「火傷」を受けていることを表すフラグを宣言する。

	pPlayer++;//ポインタ先をプレイヤー２へ
	pPlayer->fMP = pPlayer->fMP - 60;//スキル発動時にプレイヤー２のMPを60減らす。
	pPlayer--;//ポインタ先をプレイヤー１へ

	int nSteamBurstBurnRandum;//スキル「スチームバースト」のデメリット効果を起こす確率を操作する変数を宣言する。

	printf("%sのスチームバースト！\n", &g_aPlayer[PlayerNumber1].aName[0]);

	fRandum1 = rand() % 25 + 1, CheckRandum = rand() % 2 + 1, nSteamBurstBurnRandum = rand() % 2 + 1;//最終的なダメージを２５％の範囲で上下させる。ダメージか上がるか下がるかの判断は、CheckRandum変数で判断する。


	if (nSteamBurstBurnRandum == 1)
	{
		if (CheckRandum == 1)
		{
			g_aPlayer[PlayerNumber1].fLife = g_aPlayer[PlayerNumber1].fLife - ((g_aPlayer[PlayerNumber2].fATK + g_aPlayer[PlayerNumber1].fDEF) * 4.5 - (g_aPlayer[PlayerNumber1].fDEF / 2)) * (1 + (fRandum1 / 100));//(プレイヤー２の攻撃力＋プレイヤー１の防御力)*4-プレイヤー１の防御力/2)＊（１．０１～１．２５）

			fDamageResult = ((g_aPlayer[PlayerNumber2].fATK + g_aPlayer[PlayerNumber1].fDEF) * 4.5 - (g_aPlayer[PlayerNumber1].fDEF / 2)) * (1 + (fRandum1 / 100));//(プレイヤー２の攻撃力＋プレイヤー１の防御力)*4-プレイヤー１の防御力/2)＊（１．０１～１．２５）


		}
		else if (CheckRandum == 2)
		{
			g_aPlayer[PlayerNumber1].fLife = g_aPlayer[PlayerNumber1].fLife - ((g_aPlayer[PlayerNumber2].fATK + g_aPlayer[PlayerNumber1].fDEF) * 4.5 - (g_aPlayer[PlayerNumber1].fDEF / 2)) * (1 - (fRandum1 / 100));//(プレイヤー２の攻撃力＋プレイヤー１の防御力)*4-プレイヤー１の防御力/2)＊（０．７５～０．９９）

			fDamageResult = ((g_aPlayer[PlayerNumber2].fATK + g_aPlayer[PlayerNumber1].fDEF) * 4.5 - (g_aPlayer[PlayerNumber1].fDEF / 2)) * (1 - (fRandum1 / 100));//(プレイヤー２の攻撃力＋プレイヤー１の防御力)*4-プレイヤー１の防御力/2)＊（０．７５～０．９９）

		}
		printf("%sに\x1b[31m%.f\x1b[0mのダメージ！\n", &g_aPlayer[PlayerNumber2].aName[0], fDamageResult);//ダメージ表記
	}

	else if (nSteamBurstBurnRandum == 2)
	{
		bPlayer2SteamBurstBurn = true;//プレイヤー２のスキル「スチームバースト」が失敗＆デメリット効果「火傷３ターン」を付与させるので、フラグをtrueにする。
		printf("%sのスチームバーストが暴発した！攻撃が失敗＆３ターンの間\x1b[31m火傷状態\x1b[0mになった！", &g_aPlayer[PlayerNumber2].aName[0]);
	}

	return bPlayer2SteamBurstBurn;
}

//=======================================================
//プレイヤー１のスキル：イカサマレーザー
//=======================================================
bool Player1SquidLaser(Player* pPlayer)//プレイヤー１が科学者のスキル（３）、「イカサマレーザー」を発動するときに呼び出す関数。
{
	srand((unsigned int)time(NULL));//乱数の種を現在時刻を基準にランダム生成。

	float fRandum1;
	int CheckRandum;
	float fDamageResult;
	int nSquitLaserDebuffRandum;//デバフが発動する確率を操作する変数を宣言する。
	bool bPlayer2SquitLaserDebuff = false;//イカサマレーザーのデバフが発動しているかどうかを判定するフラグを宣言する。


	pPlayer->fMP = pPlayer->fMP - 45;//スキル発動時にプレイヤー１のMPを45減らす。

	printf("%sのイカサマレーザー！\n", &g_aPlayer[PlayerNumber1].aName[0]);

	fRandum1 = rand() % 25 + 1, CheckRandum = rand() % 2 + 1, nSquitLaserDebuffRandum = rand() % 10 + 1;//最終的なダメージを２５％の範囲で上下させる。ダメージか上がるか下がるかの判断は、CheckRandum変数で判断する。


	if (CheckRandum == 1)
	{
		g_aPlayer[PlayerNumber2].fLife = g_aPlayer[PlayerNumber2].fLife - (g_aPlayer[PlayerNumber2].fATK * 2.6 - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 + (fRandum1 / 100));//(プレイヤー２の攻撃力*2.6-プレイヤー2の防御力/2)＊（１．０１～１．２５）

		fDamageResult = (g_aPlayer[PlayerNumber2].fATK * 2.6 - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 + (fRandum1 / 100));//(プレイヤー２の攻撃力*2.6-プレイヤー2の防御力/2)＊（１．０１～１．２５）


	}
	else if (CheckRandum == 2)
	{
		g_aPlayer[PlayerNumber2].fLife = g_aPlayer[PlayerNumber2].fLife - (g_aPlayer[PlayerNumber2].fATK * 2.6 - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 - (fRandum1 / 100));//(プレイヤー２の攻撃力*2.6-プレイヤー2の防御力/2)＊（０．７５～０．９９））

		fDamageResult = (g_aPlayer[PlayerNumber2].fATK * 2.6 - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 - (fRandum1 / 100));//(プレイヤー２の攻撃力*2.6-プレイヤー2の防御力/2)＊（１．０１～１．２５）

	}

	printf("%sに\x1b[31m%.f\x1b[0mのダメージ！\n", &g_aPlayer[PlayerNumber2].aName[0], fDamageResult);//ダメージ表記

	if (nSquitLaserDebuffRandum >= 1 && nSquitLaserDebuffRandum <= 10)
	{
		bPlayer2SquitLaserDebuff = true;//プレイヤー２にイカサマレーザーのデバフ「防御力ダウン」を与えるフラグをtrueにする。
		printf("%sの\x1b[34m防御力が４０％減少した！\x1b[0m\n", &g_aPlayer[PlayerNumber2].aName[0]);
	}

	return bPlayer2SquitLaserDebuff;
}

//=======================================================
//プレイヤー２のスキル：イカサマレーザー
//=======================================================
bool Player2SquidLaser(Player* pPlayer)//プレイヤー１が科学者のスキル（３）、「イカサマレーザー」を発動するときに呼び出す関数。
{
	srand((unsigned int)time(NULL));//乱数の種を現在時刻を基準にランダム生成。

	float fRandum1;
	int CheckRandum;
	float fDamageResult;
	int nSquitLaserDebuffRandum;//デバフが発動する確率を操作する変数を宣言する。
	bool bPlayer1SquitLaserDebuff = false;//イカサマレーザーのデバフが発動しているかどうかを判定するフラグを宣言する。

	pPlayer++;//1ポインタ先をプレイヤー２へ
	pPlayer->fMP = pPlayer->fMP - 45;//スキル発動時にプレイヤー１のMPを45減らす。
	pPlayer--;//ポインタ先をプレイヤー１へ

	printf("%sのイカサマレーザー！\n", &g_aPlayer[PlayerNumber2].aName[0]);

	fRandum1 = rand() % 25 + 1, CheckRandum = rand() % 2 + 1, nSquitLaserDebuffRandum = rand() % 10 + 1;//最終的なダメージを２５％の範囲で上下させる。ダメージか上がるか下がるかの判断は、CheckRandum変数で判断する。


	if (CheckRandum == 1)
	{
		g_aPlayer[PlayerNumber1].fLife = g_aPlayer[PlayerNumber1].fLife - (g_aPlayer[PlayerNumber1].fATK * 2.6 - (g_aPlayer[PlayerNumber1].fDEF / 2)) * (1 + (fRandum1 / 100));//(プレイヤー２の攻撃力*2.6-プレイヤー2の防御力/2)＊（１．０１～１．２５）

		fDamageResult = (g_aPlayer[PlayerNumber1].fATK * 2.6 - (g_aPlayer[PlayerNumber1].fDEF / 2)) * (1 + (fRandum1 / 100));//(プレイヤー１の攻撃力*2.6-プレイヤー１の防御力/2)＊（１．０１～１．２５）


	}
	else if (CheckRandum == 2)
	{
		g_aPlayer[PlayerNumber1].fLife = g_aPlayer[PlayerNumber1].fLife - (g_aPlayer[PlayerNumber1].fATK * 2.6 - (g_aPlayer[PlayerNumber1].fDEF / 2)) * (1 - (fRandum1 / 100));//(プレイヤー１の攻撃力*2.6-プレイヤー１の防御力/2)＊（０．７５～０．９９））

		fDamageResult = (g_aPlayer[PlayerNumber1].fATK * 2.6 - (g_aPlayer[PlayerNumber1].fDEF / 2)) * (1 - (fRandum1 / 100));//(プレイヤー１の攻撃力*2.6-プレイヤー１の防御力/2)＊（０．７５～０．９９）

	}

	printf("%sに\x1b[31m%.f\x1b[0mのダメージ！\n", &g_aPlayer[PlayerNumber1].aName[0], fDamageResult);//ダメージ表記

	if (nSquitLaserDebuffRandum >= 1 && nSquitLaserDebuffRandum <= 10)
	{
		bPlayer1SquitLaserDebuff = true;//プレイヤー１にイカサマレーザーのデバフ「防御力ダウン」を与えるフラグをtrueにする。
		printf("%sの\x1b[34m防御力が４０％減少した！\x1b[0m\n", &g_aPlayer[PlayerNumber1].aName[0]);
	}

	return bPlayer1SquitLaserDebuff;
}

//======================================================
//プレイヤー１のスキル：フレイムウェーブ
//======================================================
bool Player1FlameWave(Player* pPlayer)//プレイヤー１が魔法使いのスキル（１）、「フレイムウェーブ」を発動するときに呼び出す関数。
{
	srand((unsigned int)time(NULL));//乱数の種を現在時刻を基準にランダム生成。

	float fRandum1;
	int nAttackSucces;
	nAttackSucces = 0;
	int CheckRandum;
	float fDamageResult;
	int nCount;
	bool bPlayer1FlameWaveBuff = false;//プレイヤー１がスキル「フレイムウェーブ」のバフを受けているかどうかのフラグを宣言する。
	int FlameWaveBuffRandum;//スキルのバフが発動する確率を操作する変数。
	FlameWaveBuffRandum = rand() % 3 + 1;

	pPlayer->fMP = pPlayer->fMP - 35;//プレイヤー１のＭＰを３５減らす。

	printf("%sのフレイムウェーブ！\n", &pPlayer->aName[0]);

	/*fRandum1 = rand() % 25 + 1*/ CheckRandum = rand() % 2 + 1;//最終的なダメージを２５％の範囲で上下させる。ダメージか上がるか下がるかの判断は、CheckRandum変数で判断する。


	if (CheckRandum == 1)
	{
		for (nCount = 0; nCount < 4; nCount++, nAttackSucces = rand() % 2 + 1, fRandum1 = rand() % 25 + 1)//４連続攻撃を行う。（命中率５０％）
		{
			if (nAttackSucces == 1)//攻撃成功確率のランダム関数が１だったら攻撃を成功させる。
			{
				g_aPlayer[PlayerNumber2].fLife = g_aPlayer[PlayerNumber2].fLife - ((g_aPlayer[PlayerNumber1].fATK * 1.1) - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 + (fRandum1 / 100));//プレイヤー１の攻撃力＊１．１-プレイヤー２の防御力/2＊（１．０１～１．２５）

				fDamageResult = ((g_aPlayer[PlayerNumber1].fATK * 1.1) - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 + (fRandum1 / 100));//プレイヤー１の攻撃力＊1.1-プレイヤー２の防御力/2＊（１．０１～１．２５）


				printf("%sに\x1b[31m%.f\x1b[0mのダメージ！\n", &g_aPlayer[PlayerNumber2].aName[0], fDamageResult);//ダメージ表記
				getchar();
			}

			else
			{
				printf("\x1b[34m攻撃が外れた！\x1b[0m\n");//６０％の確率で攻撃を外させる。
				getchar();
			}
		}

	}
	else if (CheckRandum == 2)
	{
		for (nCount = 0; nCount < 4; nCount++, nAttackSucces = rand() % 2 + 1, fRandum1 = rand() % 25 + 1)//４連続攻撃を行う。（命中率５０％）
		{
			if (nAttackSucces == 1)//攻撃成功確率のランダム関数が１だったら攻撃を成功させる。
			{
				g_aPlayer[PlayerNumber2].fLife = g_aPlayer[PlayerNumber2].fLife - ((g_aPlayer[PlayerNumber1].fATK * 1.1) - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 - (fRandum1 / 100));//プレイヤー１の攻撃力＊1.1-プレイヤー２の防御力/2＊（０．７５～０．９９）

				fDamageResult = ((g_aPlayer[PlayerNumber1].fATK * 1.1) - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 - (fRandum1 / 100));//プレイヤー１の攻撃力＊1.1-プレイヤー２の防御力/4＊（０．７５～０．９９）


				printf("%sに\x1b[31m%.f\x1b[0mのダメージ！\n", &g_aPlayer[PlayerNumber2].aName[0], fDamageResult);//ダメージ表記
				getchar();
			}

			else
			{
				printf("\x1b[34m攻撃が外れた！\x1b[0m\n");//５０％の確率で攻撃を外させる。
				getchar();
			}
		}

	}

	if (FlameWaveBuffRandum == 1)
	{
		bPlayer1FlameWaveBuff = true;//バフを発動させるのでフラグをtrueにする。
		printf("%sの攻撃力が２ターンの間、\x1b[31m２５％\x1b[0m上昇！", &pPlayer->aName[0]);
	}
	return bPlayer1FlameWaveBuff;
}

//========================================================================================
//プレイヤー２のスキル：フレイムウェーブ
//========================================================================================
bool Player2FlameWave(Player* pPlayer)//プレイヤー２が魔法使いのスキル（１）、「フレイムウェーブ」を発動するときに呼び出す関数。
{
	srand((unsigned int)time(NULL));//乱数の種を現在時刻を基準にランダム生成。

	float fRandum1;
	int nAttackSucces;
	nAttackSucces = 0;
	int CheckRandum;
	float fDamageResult;
	int nCount;
	bool bPlayer2FlameWaveBuff = false;//プレイヤー２がスキル「フレイムウェーブ」のバフを受けているかどうかのフラグを宣言する。
	int FlameWaveBuffRandum;//スキルのバフが発動する確率を操作する変数。
	FlameWaveBuffRandum = rand() % 3 + 1;

	pPlayer++;//ポインタ先プレイヤー２へ
	pPlayer->fMP = pPlayer->fMP - 35;//プレイヤー２のＭＰを３５減らす。
	printf("%sのフレイムウェーブ！\n", &pPlayer->aName[0]);

	/*fRandum1 = rand() % 25 + 1*/ CheckRandum = rand() % 2 + 1;//最終的なダメージを２５％の範囲で上下させる。ダメージか上がるか下がるかの判断は、CheckRandum変数で判断する。


	if (CheckRandum == 1)
	{
		for (nCount = 0; nCount < 4; nCount++, nAttackSucces = rand() % 2 + 1, fRandum1 = rand() % 25 + 1)//４連続攻撃を行う。（命中率５０％）
		{
			if (nAttackSucces == 1)//攻撃成功確率のランダム関数が１だったら攻撃を成功させる。
			{
				g_aPlayer[PlayerNumber1].fLife = g_aPlayer[PlayerNumber1].fLife - ((g_aPlayer[PlayerNumber2].fATK * 1.1) - (g_aPlayer[PlayerNumber1].fDEF / 2)) * (1 + (fRandum1 / 100));//プレイヤー２の攻撃力＊１．１-プレイヤー１の防御力/2＊（１．０１～１．２５）

				fDamageResult = ((g_aPlayer[PlayerNumber2].fATK * 1.1) - (g_aPlayer[PlayerNumber1].fDEF / 2)) * (1 + (fRandum1 / 100));//プレイヤー２の攻撃力＊1.1-プレイヤー１の防御力/2＊（１．０１～１．２５）


				printf("%sに\x1b[31m%.f\x1b[0mのダメージ！\n", &g_aPlayer[PlayerNumber1].aName[0], fDamageResult);//ダメージ表記
				getchar();
			}

			else
			{
				printf("\x1b[34m攻撃が外れた！\x1b[0m\n");//５０％の確率で攻撃を外させる。
				getchar();
			}
		}

	}
	else if (CheckRandum == 2)
	{
		for (nCount = 0; nCount < 4; nCount++, nAttackSucces = rand() % 2 + 1, fRandum1 = rand() % 25 + 1)//４連続攻撃を行う。（命中率５０％）
		{
			if (nAttackSucces == 1)//攻撃成功確率のランダム関数が１だったら攻撃を成功させる。
			{
				g_aPlayer[PlayerNumber1].fLife = g_aPlayer[PlayerNumber1].fLife - ((g_aPlayer[PlayerNumber2].fATK * 1.1) - (g_aPlayer[PlayerNumber1].fDEF / 2)) * (1 - (fRandum1 / 100));//プレイヤー２の攻撃力＊1.1-プレイヤー１の防御力/2＊（０．７５～０．９９）

				fDamageResult = ((g_aPlayer[PlayerNumber2].fATK * 1.1) - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 - (fRandum1 / 100));//プレイヤー１の攻撃力＊1.1-プレイヤー２の防御力/4＊（０．７５～０．９９）


				printf("%sに\x1b[31m%.f\x1b[0mのダメージ！\n", &g_aPlayer[PlayerNumber1].aName[0], fDamageResult);//ダメージ表記
				getchar();
			}

			else
			{
				printf("\x1b[34m攻撃が外れた！\x1b[0m\n");//６０％の確率で攻撃を外させる。
				getchar();
			}
		}

	}

	if (FlameWaveBuffRandum == 1)
	{
		bPlayer2FlameWaveBuff = true;//バフを発動させるのでフラグをtrueにする。
		printf("%sの攻撃力が２ターンの間、\x1b[31m２５％\x1b[0m上昇！", &pPlayer->aName[0]);
	}
	return bPlayer2FlameWaveBuff;
}

//===============================================================================
//プレイヤー１のスキル：アイスランス
//===============================================================================
bool Player1IceLance(Player* pPlayer)//プレイヤー1が魔法使いのスキル（３）、「アイスランス」を発動するときに呼び出す関数。
{
	srand((unsigned int)time(NULL));//乱数の種を現在時刻を基準にランダム生成。

	float fRandum1;
	int CheckRandum;
	float fDamageResult;

	int ItemDebuffRandum;//スキル不可を与える確率を扱う関数を宣言する。
	bool bPlayer2IceLanceDebuff = false;//プレイヤー２にアイテムコマンド封印を与えるフラグをfalseにする。

	pPlayer->fMP = pPlayer->fMP - 70;//スキル発動時にプレイヤー１のMPを70減らす。

	printf("%sのアイスランス！\n", &g_aPlayer[PlayerNumber1].aName[0]);

	fRandum1 = rand() % 25 + 1, CheckRandum = rand() % 2 + 1, ItemDebuffRandum = rand() % 20 + 1;//最終的なダメージを２５％の範囲で上下させる。ダメージか上がるか下がるかの判断は、CheckRandum変数で判断する。


	if (CheckRandum == 1)
	{
		g_aPlayer[PlayerNumber2].fLife = g_aPlayer[PlayerNumber2].fLife - (g_aPlayer[PlayerNumber1].fATK * 4.5 - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 + (fRandum1 / 100));//(プレイヤー１の攻撃力*4.5-プレイヤー2の防御力/2)＊（１．０１～１．２５）

		fDamageResult = (g_aPlayer[PlayerNumber1].fATK * 4.5 - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 + (fRandum1 / 100));//(プレイヤー１の攻撃力*4.5-プレイヤー2の防御力/2)＊（１．０１～１．２５）


	}
	else if (CheckRandum == 2)
	{
		g_aPlayer[PlayerNumber2].fLife = g_aPlayer[PlayerNumber2].fLife - (g_aPlayer[PlayerNumber1].fATK * 4.5 - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 - (fRandum1 / 100));//(プレイヤー１の攻撃力*4.5-プレイヤー2の防御力/2)＊（０．７５～０．９９）
		fDamageResult = (g_aPlayer[PlayerNumber1].fATK * 4.5 - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 - (fRandum1 / 100));//(プレイヤー１の攻撃力*4.5-プレイヤー2の防御力/2)＊（０．７５～０．９９）
	}
	printf("%sに\x1b[31m%.f\x1b[0mのダメージ！\n", &g_aPlayer[PlayerNumber2].aName[0], fDamageResult);//ダメージ表記

	if (ItemDebuffRandum >= 1 && ItemDebuffRandum <= 20)//４５％の確率でアイテム封印（３Ｔ）を発動する。
	{
		bPlayer2IceLanceDebuff = true;//プレイヤー２にアイテム封印を与えるフラグをtrueにする。
		printf("%sは５ターンの間、\x1b[35mアイテムコマンド\x1b[0mが使用できなくなった！\n", &g_aPlayer[PlayerNumber2].aName[0]);
	}
	return bPlayer2IceLanceDebuff;//プレイヤー２がスキル「アイスランス」のアイテム封印のデバフを受けているかどうかのフラグを返す。
}

//=================================================
//プレイヤー２のスキル：アイスランス
//=================================================
bool Player2IceLance(Player* pPlayer)//プレイヤー２が魔法使いのスキル（３）、「アイスランス」を発動するときに呼び出す関数。
{
	srand((unsigned int)time(NULL));//乱数の種を現在時刻を基準にランダム生成。

	float fRandum1;
	int CheckRandum;
	float fDamageResult;

	int ItemDebuffRandum;//スキル不可を与える確率を扱う関数を宣言する。
	bool bPlayer1IceLanceDebuff = false;//プレイヤー１にアイテムコマンド封印を与えるフラグをfalseにする。

	pPlayer++;//ポインタ先をプレイヤー２へ
	pPlayer->fMP = pPlayer->fMP - 70;//スキル発動時にプレイヤー２のMPを70減らす。
	pPlayer--;//ポインタ先をプレイヤー１へ

	printf("%sのアイスランス！\n", &g_aPlayer[PlayerNumber2].aName[0]);

	fRandum1 = rand() % 25 + 1, CheckRandum = rand() % 2 + 1, ItemDebuffRandum = rand() % 20 + 1;//最終的なダメージを２５％の範囲で上下させる。ダメージか上がるか下がるかの判断は、CheckRandum変数で判断する。


	if (CheckRandum == 1)
	{
		g_aPlayer[PlayerNumber1].fLife = g_aPlayer[PlayerNumber1].fLife - (g_aPlayer[PlayerNumber2].fATK * 4.5 - (g_aPlayer[PlayerNumber1].fDEF / 2)) * (1 + (fRandum1 / 100));//(プレイヤー２の攻撃力*4.5-プレイヤー2の防御力/2)＊（１．０１～１．２５）

		fDamageResult = (g_aPlayer[PlayerNumber2].fATK * 4.5 - (g_aPlayer[PlayerNumber1].fDEF / 2)) * (1 + (fRandum1 / 100));//(プレイヤー２の攻撃力*4.5-プレイヤー2の防御力/2)＊（１．０１～１．２５）


	}
	else if (CheckRandum == 2)
	{
		g_aPlayer[PlayerNumber1].fLife = g_aPlayer[PlayerNumber1].fLife - (g_aPlayer[PlayerNumber2].fATK * 4.5 - (g_aPlayer[PlayerNumber1].fDEF / 2)) * (1 - (fRandum1 / 100));//(プレイヤー２の攻撃力*4.5-プレイヤー１の防御力/2)＊（０．７５～０．９９）
		fDamageResult = (g_aPlayer[PlayerNumber2].fATK * 4.5 - (g_aPlayer[PlayerNumber1].fDEF / 2)) * (1 - (fRandum1 / 100));//(プレイヤー２の攻撃力*4.5-プレイヤー１の防御力/2)＊（０．７５～０．９９）
	}
	printf("%sに\x1b[31m%.f\x1b[0mのダメージ！\n", &g_aPlayer[PlayerNumber2].aName[0], fDamageResult);//ダメージ表記

	if (ItemDebuffRandum >= 1 && ItemDebuffRandum <= 20)//４５％の確率でアイテム封印（３Ｔ）を発動する。
	{
		bPlayer1IceLanceDebuff = true;//プレイヤー１にアイテム封印を与えるフラグをtrueにする。
		printf("%sは５ターンの間、\x1b[35mアイテムコマンド\x1b[0mが使用できなくなった！\n", &g_aPlayer[PlayerNumber1].aName[0]);
	}
	return bPlayer1IceLanceDebuff;//プレイヤー１がスキル「アイスランス」のアイテム封印のデバフを受けているかどうかのフラグを返す。
}

//========================================================
//プレイヤー１がアイテム「波動砲」を使用した時の処理
//========================================================
void Player1WaveCanon(Player* pPlayer)//プレイヤー１がアイテム「波動砲」を手に入れたときに呼び出す関数。
{
	printf("%sは波動砲を手に入れた！\x1b[33m攻撃力＊１．５倍の１０連続攻撃を行う！\x1b[0m\n", &pPlayer->aName[0]);
	float fRandum1;
	int nAttackSucces;
	nAttackSucces = 0;
	int CheckRandum;
	float fDamageResult;
	int nCount;




	/*fRandum1 = rand() % 25 + 1*/ CheckRandum = rand() % 1 + 1;//最終的なダメージを２５％の範囲で上下させる。ダメージか上がるか下がるかの判断は、CheckRandum変数で判断する。


	if (CheckRandum == 1)
	{
		for (nCount = 0; nCount < 10; nCount++, nAttackSucces = rand() % 3 + 1, fRandum1 = rand() % 25 + 1)//１０連続攻撃を行う。（命中率約３３％）
		{
			if (nAttackSucces == 1)//攻撃成功確率のランダム関数が１だったら攻撃を成功させる。
			{
				g_aPlayer[PlayerNumber2].fLife = g_aPlayer[PlayerNumber2].fLife - ((g_aPlayer[PlayerNumber1].fATK * 1.5) - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 + (fRandum1 / 100));//プレイヤー１の攻撃力＊1.5-プレイヤー２の防御力/2＊（１．０１～１．２５）

				fDamageResult = ((g_aPlayer[PlayerNumber1].fATK * 1.5) - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 + (fRandum1 / 100));//プレイヤー１の攻撃力＊1.5-プレイヤー２の防御力/2＊（１．０１～１．２５）


				printf("%sに\x1b[31m%.fのダメージ！\x1b[0m\n", &g_aPlayer[PlayerNumber2].aName[0], fDamageResult);//ダメージ表記
				getchar();
			}

			else
			{
				printf("\x1b[34m攻撃が外れた！\x1b[0m\n");//約６６％の確率で攻撃を外させる。
				getchar();
			}
		}

	}

}

void Player2WaveCanon(Player* pPlayer)//プレイヤー２がアイテム「波動砲」を手に入れたときに呼び出す関数。
{
	pPlayer++;//ポインタ先をプレイヤー２へ
	printf("%sは波動砲を手に入れた！\x1b[33m攻撃力＊１．５倍の１０連続攻撃を行う！\x1b[0m\n", &pPlayer->aName[0]);
	pPlayer--;//ポインタ先をプレイヤー１へ
	float fRandum1;
	int nAttackSucces;
	nAttackSucces = 0;
	int CheckRandum;
	float fDamageResult;
	int nCount;




	/*fRandum1 = rand() % 25 + 1*/ CheckRandum = rand() % 1 + 1;//最終的なダメージを２５％の範囲で上下させる。ダメージか上がるか下がるかの判断は、CheckRandum変数で判断する。


	if (CheckRandum == 1)
	{
		for (nCount = 0; nCount < 10; nCount++, nAttackSucces = rand() % 3 + 1, fRandum1 = rand() % 25 + 1)//１０連続攻撃を行う。（命中率約３３％）
		{
			if (nAttackSucces == 1)//攻撃成功確率のランダム関数が１だったら攻撃を成功させる。
			{
				g_aPlayer[PlayerNumber1].fLife = g_aPlayer[PlayerNumber1].fLife - ((g_aPlayer[PlayerNumber2].fATK * 1.5) - (g_aPlayer[PlayerNumber1].fDEF / 2)) * (1 + (fRandum1 / 100));//プレイヤー２の攻撃力＊1.5-プレイヤー１の防御力/2＊（１．０１～１．２５）

				fDamageResult = ((g_aPlayer[PlayerNumber2].fATK * 1.5) - (g_aPlayer[PlayerNumber1].fDEF / 2)) * (1 + (fRandum1 / 100));//プレイヤー２の攻撃力＊1.5-プレイヤー１の防御力/2＊（１．０１～１．２５）


				printf("%sに\x1b[31m%.f\x1b[0mのダメージ！\n", &g_aPlayer[PlayerNumber1].aName[0], fDamageResult);//ダメージ表記
				getchar();
			}

			else
			{
				printf("\x1b[34m攻撃が外れた！\x1b[0m\n");//約６６％の確率で攻撃を外させる。
				getchar();
			}
		}

	}

}
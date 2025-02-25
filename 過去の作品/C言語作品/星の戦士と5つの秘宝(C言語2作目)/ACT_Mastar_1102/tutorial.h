//========================================================
//
//８月３０日：エフェクトもう一回見直す[tutorial.cpp]
//Author:ShinaTaiyo
//
//=========================================================
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H

#include "main.h"
//プロトタイプ宣言
void InitTutorial(void);
void UninitTutorial(void);
void UpdateTutorial(void);
void DrawTutorial(void);

//構造体定義
typedef struct
{
	int nTutorialType;//スキルのタイプを決める変数
	float fTutorialPattern;//使うスキルを決めさせる変数
	bool bTutorialUse;//チュートリアル画面が使われているかどうかを判定するフラグ
}Tutorial;



//列挙型定義
typedef enum
{
	TUTORIALTYPE_RULE= 0,//ルールが書かれた画面を呼び出す。
	TUTORIALTYPE_OPERATION,//操作方法が書かれた画面を呼び出す。
	TUTORIALTYPE_MAX
}TutorialType;

Tutorial* GetTutorial(void);
#endif


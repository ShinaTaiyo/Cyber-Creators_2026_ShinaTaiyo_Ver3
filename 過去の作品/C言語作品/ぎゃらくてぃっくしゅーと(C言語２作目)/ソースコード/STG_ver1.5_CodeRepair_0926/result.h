//========================================================
//
//８月３０日：エフェクトもう一回見直す[result.h]
//Author:ShinaTaiyo
//
//=========================================================
#ifndef _RESULT_H_
#define _RESULT_H_

#include "main.h"

//マクロ定義
#define MAX_RESULT_TEX (2)

//列挙型定義
typedef enum
{
	RESULTTYPE_CLEAR=0,//ゲームクリア時のリザルト
	RESULTTYPE_GAMEOVER,//ゲームオーバー時のリザルト
	RESULTTYPE_MAX
}RESULTTYPE;

//構造体定義
typedef struct
{
	int nResultType;//呼び出すリザルトの種類を決める変数
	bool bResultMode;//今がリザルトモードかどうかを判定する
}ResultType;


//プロトタイプ宣言
void InitResult(void);
void UninitResult(void);
void UpdateResult (void);
void DrawResult(void);

ResultType* GetResultType(void);

#endif

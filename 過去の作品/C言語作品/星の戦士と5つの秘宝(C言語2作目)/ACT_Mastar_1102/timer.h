#ifndef _TIMER_H_
#define _TIMER_H_

//include
#include "main.h"

//マクロ定義
#define MAX_TIMER   (4)	//タイマーの最大桁数
#define MAX_TIME	(3000)	//ゲームの最大時間

//タイマーの構造体の定義
typedef struct
{
	D3DXVECTOR3 pos; //タイマーの位置
	bool bUse;        //タイマーを使用しているかどうか
}Timer;

//プロトタイプ宣言
void InitTimer(void);	//タイマーの初期化処理
void UninitTimer(void);	//タイマーの終了処理
void UpdateTimer(void);	//タイマーの更新処理
void DrawTimer(void);	//タイマーの描画処理
void SetTimerPos(D3DXVECTOR3 pos); //タイマーの設定処理
void SubTimer(int nValue);	//タイマーの減算処理
bool CheckTime(void);	//タイマーが0になったかどうか

int GetTimer(void);//現在の残り時間を取得する

#endif


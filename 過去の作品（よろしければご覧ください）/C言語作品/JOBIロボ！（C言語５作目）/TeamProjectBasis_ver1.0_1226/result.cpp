//=========================================================
//
// リザルト画面の処理
// Author : Atsumu Kuboichi
//
//=========================================================
#include "result.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "resultcheck.h"
#include "resultscore.h"
#include "score.h"

//=========================================================
// グローバル変数
//=========================================================

//=========================================================
// 初期化処理
//=========================================================
void InitResult(void)
{
	InitBG();			//背景
	InitResultCheck();	//ミッションクリアチェック
	InitResultScore();	//リザルト表示スコア

	PlaySound(SOUND_LABEL_BGM014);
}

//=========================================================
// 終了処理
//=========================================================
void UninitResult(void)
{
	UninitResultCheck();//ミッションクリアチェック
	UninitResultScore();//リザルト表示スコア
	UninitBG();	        //背景
}

//=========================================================
// 更新処理
//=========================================================
void UpdateResult(void)
{
	UpdateBG();			//背景
	UpdateResultCheck();//ミッションクリアチェック
	UpdateResultScore();//リザルト表示スコア

	if (GetKeyboardTrigger(DIK_RETURN) || GetJoypadTrigger(JOYKEY_A))
	{//Enterキー or Startボタンが押された場合
		PlaySound(SOUND_LABEL_SE_DECIDE_000);

		SetFade(MODE_RANKING);	//ランキング画面へ移行
	}
}

//=========================================================
// 描画処理
//=========================================================
void DrawResult(void)
{
	DrawBG();			//背景
	DrawResultCheck();	//ミッションクリアチェック
	DrawResultScore();	//リザルト表示スコア
}

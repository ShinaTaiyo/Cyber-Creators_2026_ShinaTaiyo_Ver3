//====================================================
//
//９月４日：スコアを実装 [score.h]
//Author:ShinaTaiyo
//
//====================================================

#ifndef _SCORE_H_  
#define _SCORE_H_

//======================
//インクルード
//======================
#include "main.h"
#include "object2d.h"
//==========================================

//======================
//前方宣言
//======================
class CNumber;

//==========================================

//==========================================
//スコアクラス
//==========================================
class CScore : public CObject
{
public:
	CScore();                   //コンストラクタ
	~CScore() override;         //デストラクタ
	HRESULT Init() override;     //初期化処理
	void Uninit() override;      //終了処理
	void Update() override;      //更新処理
	void Draw() override;        //描画処理
	void SetDeath() override;    //死亡フラグを設定
	static CScore* Create(int nInitialScore);//生成処理
	void AddScore(int nScore);
private:
	static const int m_nMAX_SCOREDIGIT = 5;//スコアの最大桁数
	CNumber* m_pNumber[m_nMAX_SCOREDIGIT];//数字へのポインタ
	int m_nScore;//スコア
};

#endif

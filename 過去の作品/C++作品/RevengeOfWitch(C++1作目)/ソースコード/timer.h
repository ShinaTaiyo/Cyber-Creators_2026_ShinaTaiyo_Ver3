//====================================================
//
//９月７日：タイマーを実装 [timer.h]
//Author:ShinaTaiyo
//
//====================================================

#ifndef _TIMER_H_  
#define _TIMER_H_

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
//タイマークラス
//==========================================
class CTimer : public CObject
{
public:
	CTimer();                                //コンストラクタ
	~CTimer() override;                      //デストラクタ
	HRESULT Init() override;                 //初期化処理
	void Uninit() override;                  //終了処理
	void Update() override;                  //更新処理
	void Draw() override;                    //描画処理
	void SetDeath() override;                //死亡フラグを設定
	static CTimer* Create(int nInitialTimer,D3DXVECTOR3 Pos,float fNumberWidth,float fNumberHeight);//生成処理
	void AddTimer(int nTimer);               //時間を増やす
	void SubTimer(int nTimer);               //時間を減らす
private:
	static const int m_nMAX_TIMERDIGIT = 3;  //スコアの最大桁数
	static const int m_nMAX_TIME = 180;      //制限時間
	CNumber* m_pNumber[m_nMAX_TIMERDIGIT];   //数字へのポインタ
	int m_nTime;
};

#endif

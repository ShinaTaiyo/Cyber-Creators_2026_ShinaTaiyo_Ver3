//====================================================
//
//６月１６日：スコアを実装 [number.h]
//Author:ShinaTaiyo
//
//====================================================

#ifndef _NUMBER_H_  
#define _NUMBER_H_

//======================
//インクルード
//======================
#include "main.h"
#include "object2d.h"
//==========================================

//==========================================

//==========================================
//ナンバークラス
//==========================================
class CNumber : public CObject2D
{
public:
	CNumber();                   //コンストラクタ
	~CNumber() override;         //デストラクタ
	HRESULT Init() override;     //初期化処理
	void Uninit() override;      //終了処理
	void ExtraUninit() override; //別枠の終了処理
	void Update() override;      //更新処理
	void Draw() override;        //描画処理
	void SetDeath() override;    //死亡フラグを設定
	void SetNumber(int nNum) {m_nNumber = nNum; }//数字を設定
	static CNumber* Create(D3DXVECTOR3 pos,float fWidth,float fHeight);     //数字を生
private:
	int m_nNumber;              //数字がどれか
};

#endif

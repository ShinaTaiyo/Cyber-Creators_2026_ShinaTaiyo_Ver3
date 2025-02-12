//====================================================
//
//８月２９日：計算用のクラスを作る[calculation.h]
//Author:ShinaTaiyo
//
//====================================================

#ifndef _CALCULATION_H_  
#define _CALCULATION_H_

//======================
//インクルード
//======================
#include "main.h"
//==========================================

//==========================================

//==========================================
//計算クラス
//==========================================
class CCalculation
{
public:
	CCalculation();                   //コンストラクタ
	~CCalculation();                  //デストラクタ
	static float CalculationLength(D3DXVECTOR3 Pos, D3DXVECTOR3 PurposePos);
	static float CalculationXYaim(D3DXVECTOR3 Pos, D3DXVECTOR3 PurposePos);
	static float CalculationParabola(float fLength, float fGravity, float fSpeed,D3DXVECTOR3 Pos,D3DXVECTOR3 PurposePos);
	static float CalculationRandVecXY();
	static void CaluclationMove(D3DXVECTOR3& Pos,float fSpeed);//移動に使う処理の角度を求める
	static int CalculationDigit(int nNum);//桁数を計算する
	static D3DXVECTOR3 Calculation3DVec(D3DXVECTOR3 MyPos, D3DXVECTOR3 AimPos, float fSpeed);  //目的への移動量を求める
private:
};
#endif

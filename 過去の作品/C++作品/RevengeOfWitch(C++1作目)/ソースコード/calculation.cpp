//===========================================================================================================
//
//８月２９日：計算用のクラスを作る[calculation.cpp]
//Author:ShinaTaiyo
//
//===========================================================================================================

//=========================================================
//インクルード
//=========================================================
#include "calculation.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
//===========================================================================================================

//=========================================================
//コンストラクタ
//=========================================================
CCalculation::CCalculation()
{

}
//===========================================================================================================

//=========================================================
//デストラクタ
//=========================================================
CCalculation::~CCalculation()
{

}
//===========================================================================================================

//=========================================================
//距離を計算する
//=========================================================
float CCalculation::CalculationLength(D3DXVECTOR3 Pos, D3DXVECTOR3 PurposePos)
{
	float fLength = 0.0f;
	fLength = sqrtf(powf(PurposePos.x - Pos.x, 2) + powf(PurposePos.y - Pos.y, 2) + powf(PurposePos.z - Pos.z, 2));
	return fLength;
}
//==========================================================================================================

//=========================================================
//XYベクトルを計算する
//=========================================================
float CCalculation::CalculationXYaim(D3DXVECTOR3 Pos, D3DXVECTOR3 PurposePos)
{
	float fVXaim = PurposePos.x - Pos.x;
	float fVYaim = PurposePos.y - Pos.y;
	float fVLaim = atan2f(fVXaim, fVYaim);
	return fVLaim;
}
//==========================================================================================================

//=========================================================
//放物線の角度を計算する
//=========================================================
float CCalculation::CalculationParabola(float fLength, float fGravity, float fSpeed, D3DXVECTOR3 Pos, D3DXVECTOR3 PurposePos)
{
	float fRot = 0.0f;//角度

	float fXG = fLength * fGravity;//距離×重力
	float fV = fSpeed;             //速度
	float fV2 = powf(fV, 2);       //速度２条
	if (fXG == 0.0f)
	{//0除算回避
		fXG = 0.001f;
	}
	if (fV2 == 0.0f)
	{//0除算回避
		fV2 = 0.001f;
	}

	fRot = asinf(fXG / fV2);
	fRot /= 2;

	if (PurposePos.x < Pos.x)
	{//角度が絶対値なので、目的の位置が自分の位置よりマイナス方向にある場合、負の値にする
		fRot *= -1;
	}

	return fRot;
}
//===========================================================================================================

//=========================================================
//XY方向の乱数ベクトルを用意する
//=========================================================
float CCalculation::CalculationRandVecXY()
{
	float fRandRot = float(rand() % 628 + 1) / 100;
	return fRandRot;
}
//===========================================================================================================

//=========================================================
//移動方向への角度を計算する
//=========================================================
void CCalculation::CaluclationMove(D3DXVECTOR3& Pos, float fSpeed)
{
	float fMoveX = 0.0f;                                            //X方向の移動量
	float fMoveZ = 0.0f;                                            //Z方向の移動量
	float fRot = 0.0f;                                              //移動方向
	bool bMove = false;                                             //移動しているかどうか 

	if (CManager::GetInputKeyboard()->GetPress(DIK_W) == true)
	{
		fMoveZ = 1.0f;
	}
	else if (CManager::GetInputKeyboard()->GetPress(DIK_S) == true)
	{
		fMoveZ = -1.0f;
	}
	if (CManager::GetInputKeyboard()->GetPress(DIK_D) == true)
	{
		fMoveX = 1.0f;
	}
	else if (CManager::GetInputKeyboard()->GetPress(DIK_A) == true)
	{
		fMoveX = -1.0f;
	}

	if (fMoveX != 0.0f || fMoveZ != 0.0f)
	{
		bMove = true;//移動状態
	}
	else
	{
		bMove = false;//待機状態
	}


	if (bMove == true)
	{//移動状態なら
		Pos.x += sinf(atan2f(fMoveX, fMoveZ)) * fSpeed;
		Pos.y += cosf(atan2f(fMoveX, fMoveZ)) * fSpeed;
	}
}
//===========================================================================================================

//=========================================================
//桁数を計算する
//=========================================================
int CCalculation::CalculationDigit(int nNum)
{
	int nDigit = 0;
	while (nNum != 0) {
		nNum /= 10;
		nDigit++;
	}
	return nDigit;
}
//=========================================================
//目的の位置への移動量を計算する
//=========================================================
D3DXVECTOR3 CCalculation::Calculation3DVec(D3DXVECTOR3 MyPos, D3DXVECTOR3 AimPos, float fSpeed)
{
	D3DXVECTOR3 VecAim = D3DXVECTOR3(0.0f, 0.0f, 0.0f);       //それぞれの方向のベクトル
	D3DXVECTOR3 ResultMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   //結果の移動量
	float fVLaim = 0.0f;                     //総合ベクトル

	//==========================
	//３次元ベクトルを取る
	//==========================

	//方向ベクトルの計算
	VecAim.x = AimPos.x - MyPos.x;
	VecAim.y = AimPos.y - MyPos.y;
	VecAim.z = AimPos.z - MyPos.z;

	//ベクトルの大きさを求める
	fVLaim = sqrtf(powf(VecAim.x, 2) + powf(VecAim.y, 2) + powf(VecAim.z, 2));

	//方向ベクトルを正規化し、それぞれの軸に対する移動量を求める
	ResultMove.x = VecAim.x / fVLaim * fSpeed;
	ResultMove.y = VecAim.y / fVLaim * fSpeed;
	ResultMove.z = VecAim.z / fVLaim * fSpeed;

	return ResultMove;
}
//===========================================================================================================

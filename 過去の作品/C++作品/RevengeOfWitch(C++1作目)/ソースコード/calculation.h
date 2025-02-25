//====================================================
//
//�W���Q�X���F�v�Z�p�̃N���X�����[calculation.h]
//Author:ShinaTaiyo
//
//====================================================

#ifndef _CALCULATION_H_  
#define _CALCULATION_H_

//======================
//�C���N���[�h
//======================
#include "main.h"
//==========================================

//==========================================

//==========================================
//�v�Z�N���X
//==========================================
class CCalculation
{
public:
	CCalculation();                   //�R���X�g���N�^
	~CCalculation();                  //�f�X�g���N�^
	static float CalculationLength(D3DXVECTOR3 Pos, D3DXVECTOR3 PurposePos);
	static float CalculationXYaim(D3DXVECTOR3 Pos, D3DXVECTOR3 PurposePos);
	static float CalculationParabola(float fLength, float fGravity, float fSpeed,D3DXVECTOR3 Pos,D3DXVECTOR3 PurposePos);
	static float CalculationRandVecXY();
	static void CaluclationMove(D3DXVECTOR3& Pos,float fSpeed);//�ړ��Ɏg�������̊p�x�����߂�
	static int CalculationDigit(int nNum);//�������v�Z����
	static D3DXVECTOR3 Calculation3DVec(D3DXVECTOR3 MyPos, D3DXVECTOR3 AimPos, float fSpeed);  //�ړI�ւ̈ړ��ʂ����߂�
private:
};
#endif

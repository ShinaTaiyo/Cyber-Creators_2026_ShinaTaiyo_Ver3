//===========================================================================================================
//
//�W���Q�X���F�v�Z�p�̃N���X�����[calculation.cpp]
//Author:ShinaTaiyo
//
//===========================================================================================================

//=========================================================
//�C���N���[�h
//=========================================================
#include "calculation.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
//===========================================================================================================

//=========================================================
//�R���X�g���N�^
//=========================================================
CCalculation::CCalculation()
{

}
//===========================================================================================================

//=========================================================
//�f�X�g���N�^
//=========================================================
CCalculation::~CCalculation()
{

}
//===========================================================================================================

//=========================================================
//�������v�Z����
//=========================================================
float CCalculation::CalculationLength(D3DXVECTOR3 Pos, D3DXVECTOR3 PurposePos)
{
	float fLength = 0.0f;
	fLength = sqrtf(powf(PurposePos.x - Pos.x, 2) + powf(PurposePos.y - Pos.y, 2) + powf(PurposePos.z - Pos.z, 2));
	return fLength;
}
//==========================================================================================================

//=========================================================
//XY�x�N�g�����v�Z����
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
//�������̊p�x���v�Z����
//=========================================================
float CCalculation::CalculationParabola(float fLength, float fGravity, float fSpeed, D3DXVECTOR3 Pos, D3DXVECTOR3 PurposePos)
{
	float fRot = 0.0f;//�p�x

	float fXG = fLength * fGravity;//�����~�d��
	float fV = fSpeed;             //���x
	float fV2 = powf(fV, 2);       //���x�Q��
	if (fXG == 0.0f)
	{//0���Z���
		fXG = 0.001f;
	}
	if (fV2 == 0.0f)
	{//0���Z���
		fV2 = 0.001f;
	}

	fRot = asinf(fXG / fV2);
	fRot /= 2;

	if (PurposePos.x < Pos.x)
	{//�p�x����Βl�Ȃ̂ŁA�ړI�̈ʒu�������̈ʒu���}�C�i�X�����ɂ���ꍇ�A���̒l�ɂ���
		fRot *= -1;
	}

	return fRot;
}
//===========================================================================================================

//=========================================================
//XY�����̗����x�N�g����p�ӂ���
//=========================================================
float CCalculation::CalculationRandVecXY()
{
	float fRandRot = float(rand() % 628 + 1) / 100;
	return fRandRot;
}
//===========================================================================================================

//=========================================================
//�ړ������ւ̊p�x���v�Z����
//=========================================================
void CCalculation::CaluclationMove(D3DXVECTOR3& Pos, float fSpeed)
{
	float fMoveX = 0.0f;                                            //X�����̈ړ���
	float fMoveZ = 0.0f;                                            //Z�����̈ړ���
	float fRot = 0.0f;                                              //�ړ�����
	bool bMove = false;                                             //�ړ����Ă��邩�ǂ��� 

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
		bMove = true;//�ړ����
	}
	else
	{
		bMove = false;//�ҋ@���
	}


	if (bMove == true)
	{//�ړ���ԂȂ�
		Pos.x += sinf(atan2f(fMoveX, fMoveZ)) * fSpeed;
		Pos.y += cosf(atan2f(fMoveX, fMoveZ)) * fSpeed;
	}
}
//===========================================================================================================

//=========================================================
//�������v�Z����
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
//�ړI�̈ʒu�ւ̈ړ��ʂ��v�Z����
//=========================================================
D3DXVECTOR3 CCalculation::Calculation3DVec(D3DXVECTOR3 MyPos, D3DXVECTOR3 AimPos, float fSpeed)
{
	D3DXVECTOR3 VecAim = D3DXVECTOR3(0.0f, 0.0f, 0.0f);       //���ꂼ��̕����̃x�N�g��
	D3DXVECTOR3 ResultMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   //���ʂ̈ړ���
	float fVLaim = 0.0f;                     //�����x�N�g��

	//==========================
	//�R�����x�N�g�������
	//==========================

	//�����x�N�g���̌v�Z
	VecAim.x = AimPos.x - MyPos.x;
	VecAim.y = AimPos.y - MyPos.y;
	VecAim.z = AimPos.z - MyPos.z;

	//�x�N�g���̑傫�������߂�
	fVLaim = sqrtf(powf(VecAim.x, 2) + powf(VecAim.y, 2) + powf(VecAim.z, 2));

	//�����x�N�g���𐳋K�����A���ꂼ��̎��ɑ΂���ړ��ʂ����߂�
	ResultMove.x = VecAim.x / fVLaim * fSpeed;
	ResultMove.y = VecAim.y / fVLaim * fSpeed;
	ResultMove.z = VecAim.z / fVLaim * fSpeed;

	return ResultMove;
}
//===========================================================================================================

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
#include "camera.h"
#include "renderer.h"
#include "particle.h"
#include "attack.h"
#include "input.h"
#include "debugtext.h"
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
	float fLength = 0.0f;//�����i�[�p
	fLength = sqrtf(powf(PurposePos.x - Pos.x, 2) + powf(PurposePos.y - Pos.y, 2) + powf(PurposePos.z - Pos.z, 2));//�ړI�̈ʒu�Ƃ̋��������߂�
	return fLength;
}
//==========================================================================================================

//=========================================================
//XY�x�N�g�����v�Z����
//=========================================================
float CCalculation::CalculationXYaim(D3DXVECTOR3 Pos, D3DXVECTOR3 PurposePos)
{
	float fVXaim = PurposePos.x - Pos.x;//X�����̃x�N�g�������߂�
	float fVYaim = PurposePos.y - Pos.y;//Y�����̃x�N�g�������߂�
	float fVLaim = atan2f(fVXaim, fVYaim);//��̃x�N�g���̊p�x�����߂�
	return fVLaim;
}
//==========================================================================================================

//=========================================================
//�������̊p�x���v�Z����
//=========================================================
float CCalculation::CalculationParabola(float fLength, float fGravity, float fSpeed, D3DXVECTOR3 Pos, D3DXVECTOR3 PurposePos)
{
	float fRot = 0.0f;             //�p�x

	float fXG = fLength * fGravity;//�����~�d��
	float fV = fSpeed;             //���x
	float fV2 = powf(fV, 2);       //���x�Q��

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
	float fRandRot = float(rand() % 628 + 1) / 100;//D3DX_PI * 2�i��6.28)�������_���ŋ��߂�
	return fRandRot;
}
//===========================================================================================================

//=========================================================
//2D�����̌����̕␳���s��
//=========================================================
float CCalculation::CalculationCollectionRot2D(float fMyRot, float fRotAim, float fDecayRot, bool bCameraOffSet)
{
	float fRotDiff = 0.0f;                                 //�����̍���
	float fCameraRot = CManager::GetCamera()->GetRot().y;  //�J������Yaw���擾
	fRotDiff = fRotAim - fMyRot;

	//�����̍����̒����i�W���o�����b�N��������邽��)
	if (fRotDiff > D3DX_PI)
	{
		fRotDiff -= D3DX_PI * 2;
	}
	else if (fRotDiff < -D3DX_PI)
	{
		fRotDiff += D3DX_PI * 2;
	}

	//���X�ɖړI�̌����֍��킹�Ă���
	fMyRot += fRotDiff * fDecayRot;

	if (bCameraOffSet == true)
	{//�J��������Ɍ��������߂邩�ǂ���
		//�����̒����i�J��������ɒl��3.14�`-3.14�̒��ɌŒ肵�����̂ŁE�E�E�j
		if (fMyRot >= D3DX_PI + fCameraRot)
		{//3.14��-3.14�ɂ���
			fMyRot -= D3DX_PI * 2;
		}
		else if (fMyRot <= -D3DX_PI + fCameraRot)
		{//-3.14��3.14�ɂ���
			fMyRot += D3DX_PI * 2;
		}
	}
	else
	{//���ʂɌ��������߂�
		//�����̒���
		if (fMyRot >= D3DX_PI)
		{//3.14��-3.14�ɂ���
			fMyRot -= D3DX_PI * 2;
		}
		else if (fMyRot <= -D3DX_PI)
		{//-3.14��3.14�ɂ���
			fMyRot += D3DX_PI * 2;
		}
	}
	return fMyRot;
}
//===========================================================================================================

//=========================================================
//�ړ������ւ̊p�x���v�Z����
//=========================================================
bool CCalculation::CaluclationMove(bool bUseStick, D3DXVECTOR3& Pos, D3DXVECTOR3& Move, float fSpeed, MOVEAIM MoveAim, float& fRot)
{
	float fCameraRot = CManager::GetCamera()->GetRot().y;           //�J�����̌������擾
	float fMoveX = 0.0f;                                            //X�����̈ړ���
	float fMoveZ = 0.0f;                                            //Z�����̈ړ���
	bool bMove = false;                                             //�ړ����Ă��邩�ǂ��� 
	bool bUseController = true;                                     //�R���g���[���[���g�p���邩�ǂ���
	bMove = CManager::GetInputJoypad()->GetLStickPress(8, 0.0f);    //�R���g���[���[�̓���
	D3DXVECTOR3 PosFuture = { 0.0f,0.0f,0.0f };                     //1f��̈ʒu
	if (bMove == false)
	{//���̎��_�ŃR���g���[���[�̓��͂�����Ă��Ȃ��ꍇ�A�L�[���͂̎�t���J�n
		bUseController = false;//�R���g���[���[�͎g�p���Ȃ�
		if (CManager::GetInputKeyboard()->GetPress(DIK_W) == true || CManager::GetInputJoypad()->GetPress(CInputJoypad::JOYKEY::UP) == true)
		{//�O�ւ̃x�N�g��
			fMoveZ = 1.0f;
		}
		else if (CManager::GetInputKeyboard()->GetPress(DIK_S) == true || CManager::GetInputJoypad()->GetPress(CInputJoypad::JOYKEY::DOWN) == true)
		{//���ւ̃x�N�g��
			fMoveZ = -1.0f;
		}
		if (CManager::GetInputKeyboard()->GetPress(DIK_D) == true || CManager::GetInputJoypad()->GetPress(CInputJoypad::JOYKEY::RIGHT) == true)
		{//�E�ւ̃x�N�g��
			fMoveX = 1.0f;
		}
		else if (CManager::GetInputKeyboard()->GetPress(DIK_A) == true || CManager::GetInputJoypad()->GetPress(CInputJoypad::JOYKEY::LEFT) == true)
		{//���ւ̃x�N�g��
			fMoveX = -1.0f;
		}

		if (fMoveX != 0.0f || fMoveZ != 0.0f)
		{//��L�̏����Ń{�^������͂��Ă����瓮���Ă���Ƃ݂Ȃ�
			bMove = true;//�ړ����

		}
		else
		{
			bMove = false;//�ҋ@���
		}
	}
	if (bMove == true)
	{//�ړ���ԂȂ�
		float fMoveRot = 0.0f;//�ړ�������������߂�
		//�J��������Ɍ��������߂�
		if (bUseController == true)
		{
			fMoveRot = CManager::GetInputJoypad()->GetLStickAimRot();                     //���X�e�B�b�N�̌������擾����
		}
		else
		{//�䂩��p�x�����߂�iZ���̐��������O�Ȃ̂ŁAZ������u�E�����v��X�����̊p�x�u�������v�����߂�j
			fMoveRot = atan2f(fMoveX, fMoveZ);
		}
		switch (MoveAim)
		{//�ǂ̖ʂ����Ɉړ����邩�����߂�
		case MOVEAIM_XY://XY��������Ɉړ�����i�ړ��������J������Ō��߂�)
			Move.x = sinf(fMoveRot + fCameraRot) * fSpeed;
			Move.y = cosf(fMoveRot + fCameraRot) * fSpeed;
			break;
		case MOVEAIM_XZ://XZ��������Ɉړ�����
			Move.x = sinf(fMoveRot + fCameraRot) * fSpeed;
			Move.z = cosf(fMoveRot + fCameraRot) * fSpeed;
			break;
		case MOVEAIM_ZY://ZY��������Ɉړ�����
			Move.z = sinf(fMoveRot + fCameraRot) * fSpeed;
			Move.y = cosf(fMoveRot + fCameraRot) * fSpeed;
			break;
		default:
			break;
		}

		//1f��̈ʒu���v�Z
		PosFuture = Pos + Move;

		//1f��̈ʒu�ƌ��݂̈ʒu�̃x�N�g����������ׂ��p�x�����߂�
		fRot = atan2f(PosFuture.x - Pos.x, PosFuture.z - Pos.z);
	}
	return bMove;

}
//===========================================================================================================


//=========================================================
//�ړI�̈ʒu�ւ̈ړ��ʂ��v�Z����
//=========================================================
D3DXVECTOR3 CCalculation::Calculation3DVec(D3DXVECTOR3 MyPos, D3DXVECTOR3 AimPos, float fSpeed)
{
	D3DXVECTOR3 VecAim = D3DXVECTOR3(0.0f,0.0f,0.0f);       //���ꂼ��̕����̃x�N�g��
	D3DXVECTOR3 ResultMove = D3DXVECTOR3(0.0f,0.0f,0.0f);   //���ʂ̈ړ���
	float fVLaim = 0.0f;                                    //�����x�N�g��

	//==========================
	//�R�����x�N�g�������
	//==========================

	//���ꂼ��̎��̃x�N�g�����v�Z
	VecAim.x = AimPos.x - MyPos.x;
	VecAim.y = AimPos.y - MyPos.y;
	VecAim.z = AimPos.z - MyPos.z;

	//�x�N�g���̑傫�������߂�
	fVLaim = sqrtf(powf(VecAim.x,2) + powf(VecAim.y,2) + powf(VecAim.z,2));

	//�����x�N�g���𐳋K�����A���ꂼ��̎��ɑ΂���ړ��ʂ����߂�
	ResultMove.x = VecAim.x / fVLaim * fSpeed;
	ResultMove.y = VecAim.y / fVLaim * fSpeed;
	ResultMove.z = VecAim.z / fVLaim * fSpeed;

	return ResultMove;
}
//===========================================================================================================

//=========================================================
//�ړI�n�ւ̃x�N�g����Yaw��Pitch�ɕϊ�����
//=========================================================
D3DXVECTOR2 CCalculation::VectorToYawPitch(const D3DXVECTOR3& MyPos, const D3DXVECTOR3& AimPos)
{
	D3DXVECTOR2 YawPitch = { 0.0f,0.0f };//Yaw��Pitch���v�Z����

	YawPitch.y = atan2f(AimPos.x - MyPos.x, AimPos.z - MyPos.z);//XZ�����̔��ˊp�x�������̔䂩��Yaw�����߂�
	YawPitch.x = atan2f(AimPos.y - MyPos.y, sqrtf(powf(AimPos.x - MyPos.x, 2) + powf(AimPos.z - MyPos.z, 2)));//XZ�����̋���������Pitch�����߂�

	return YawPitch;
}
//===========================================================================================================

//=========================================================
//�����_���ȂR������Ԃ̑��x�����߂�
//=========================================================
D3DXVECTOR3 CCalculation::Rand3DVec(int nMathSpeed, int nDivisionSpeed)
{
	D3DXVECTOR3 Speed = { 0.0f,0.0f,0.0f };
	bool bMinus = false;

	//X�����̃����_���Ȍ��������߂�
	bMinus = rand() % 2;//�E�����������߂�
	Speed.x = static_cast<float>(rand() % nMathSpeed) / nDivisionSpeed;
	if (bMinus == true)
	{
		Speed.x *= -1;
	}

	//Y�����̃����_���Ȍ��������߂�
	bMinus = rand() % 2;//�ォ�������߂�
	Speed.y = static_cast<float>(rand() % nMathSpeed) / nDivisionSpeed;
	if (bMinus == true)
	{
		Speed.y *= -1;
	}

	//Z�����̃����_���Ȍ��������߂�
	bMinus = rand() % 2;//�O����납�����߂�
	Speed.z = static_cast<float>(rand() % nMathSpeed) / nDivisionSpeed;
	if (bMinus == true)
	{
		Speed.z *= -1;
	}
	return Speed;
}
//===========================================================================================================

//=========================================================
//���F���v�Z����
//=========================================================
D3DXCOLOR CCalculation::CalRaibowColor()
{
    //RGB�̒l�������_���Ɍ��ߑ������F��\������
	D3DXCOLOR RaibowColor = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	RaibowColor.r = float(rand() % 100 + 1) / 100;//R�l�����߂�
	RaibowColor.g = float(rand() % 100 + 1) / 100;//G�l�����߂�
	RaibowColor.b = float(rand() % 100 + 1) / 100;//B�l�����߂�
	return RaibowColor;
}
//===========================================================================================================

//=========================================================
// �X�N���[�����W�����[���h���W�ɕϊ�
//=========================================================
D3DXVECTOR3* CCalculation::CalcScreenToWorld(D3DXVECTOR3* pout, float Sx, float Sy, float fZ, int Screen_w, int Screen_h, D3DXMATRIX* View, D3DXMATRIX* Prj)
{
	// �e�s��̋t�s����Z�o�i�r���[�A�v���W�F�N�V�����}�g���b�N�X�̋t�s���������̂́A�J�����̈ʒu��2D��UI���o�Ă���ƒ�`�ł��邩��)
	//�t�s��Ƃ́A�l�ɒl-1�������A�|�����킳���O�ɖ߂�����
	D3DXMATRIX InvView, InvPrj, VP, InvViewport;
	D3DXMatrixInverse(&InvView, NULL, View);//�r���[�}�g���b�N�X�Ƃ̋t����������ă��[���h���W�����߂�
	D3DXMatrixInverse(&InvPrj, NULL, Prj);  //�v���W�F�N�V�����}�g���b�N�X�Ƃ̋t�s��i�������i���s���e�A����p�Ȃǁj��ς��Ă���̂ŁA�t�s����|�����킹�A���Ƃɖ߂��K�v������j
	D3DXMatrixIdentity(&VP);

	//�X�P�[�����O�̒l��ς��Ă���B�X�N���[�����W�̒��S����ʒ����ɂ���
	VP._11 = Screen_w / 2.0f; VP._22 = -Screen_h / 2.0f;//�X�N���[�����W�n�ł́A�ʏ����������ɂȂ�̂ŁA���W�ϊ�����ۂ�-�ɂ��Ă���
	VP._41 = Screen_w / 2.0f; VP._42 = Screen_h / 2.0f;//�X�N���[�����W�n�ł́A�ʏ����������ɂȂ�̂ŁA���W�ϊ�����ۂ�-�ɂ��Ă���
	D3DXMatrixInverse(&InvViewport, NULL, &VP);

	//����
	D3DXVECTOR3 MyPos = D3DXVECTOR3(Sx,Sy,fZ);

	// �t�ϊ�
	D3DXMATRIX tmp = InvViewport * InvPrj * InvView;//���[���h���W�����߂�
	D3DXVec3TransformCoord(pout, &MyPos, &tmp);     //�ʒu�����߂�
	return pout;
}
//===========================================================================================================

//=========================================================
// ���[���h���W���X�N���[�����W�ɕϊ�����
//=========================================================
D3DXVECTOR3 CCalculation::CalcWorldToScreenNoViewport(D3DXVECTOR3 worldPos, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, float screenWidth, float screenHeight)
{
	//���[���h���W���N���b�v���W�ɕϊ�
	D3DXVECTOR4 ClipSpacePos;
	D3DXMATRIX mtxTrans;

	mtxTrans = viewMatrix * projectionMatrix;
	D3DXVec3Transform(&ClipSpacePos, &worldPos, &mtxTrans);

	//�������Z�i�N���b�v���W����NDC��Ԃցj
	if (ClipSpacePos.w != 0.0f)
	{
		ClipSpacePos.x /= ClipSpacePos.w;
		ClipSpacePos.y /= ClipSpacePos.w;
		ClipSpacePos.z /= ClipSpacePos.w;
	}

	//�X�N���[�����W�֕ϊ�
	D3DXVECTOR3 ScreenPos;
	ScreenPos.x = (ClipSpacePos.x * 0.5f + 0.5f) * screenWidth;
	ScreenPos.y = (1.0f - (ClipSpacePos.y * 0.5f + 0.5f)) * screenHeight;
	ScreenPos.z = ClipSpacePos.z;//�[�x�l�i�O�`�P�j�͈̔�

	return ScreenPos;
}
//===========================================================================================================

//=========================================================
//�������v�Z����
//=========================================================
int CCalculation::CalculationDigit(int nNum)
{
	int nDigit = 0;
	if (nNum == 0)
	{//���l���Ȃ��̂�1��
		return 1;
	}

	while (nNum != 0) {//int��10�Ŋ����Ēl���O�ɂȂ�܂ŌJ��Ԃ��A�������v�Z����
		nNum /= 10;
		nDigit++;
	}
	return nDigit;
}

//=========================================================
//�w�肵���������擾����(number��345�Aposition��2��������)��345 / 100 = 3.45 = 3 % 10 = 3)
//=========================================================
int CCalculation::getDigit(int number, int position)
{
	int divisor = static_cast<int>(std::pow(10, position));
	return (number / divisor) % 10;
}
//===========================================================================================================

//=========================================================
//�x�N�g�����v�Z����
//=========================================================
D3DXVECTOR3 CCalculation::CalcVec(D3DXVECTOR3 MyPos, D3DXVECTOR3 AimPos, bool bNormalize)
{
	D3DXVECTOR3 Vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//�x�N�g�������߂�
	Vec = AimPos - MyPos;//�ړI�̈ʒu�Ƃ̃x�N�g�������߂�

	if (bNormalize == true)
	{//���K�����邩�ǂ���
		D3DXVec3Normalize(&Vec, &Vec);
	}
	return Vec;
}
//===========================================================================================================

//=====================================================================
//���W�A��������x�N�g���ɕϊ�����
//=====================================================================
D3DXVECTOR3 CCalculation::RadToVec(const D3DXVECTOR3& Rot)
{
	D3DXVECTOR3 RotToVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//����O�Ƃ��邽�߁AY����cos����ɂ���BXZ���ʂƔ�����킹�邽�߁AXZ�̈ʒu�����߂�Ƃ��́Asin��������
	RotToVec.x = sinf(Rot.x) * sinf(Rot.y);
	RotToVec.y = cosf(Rot.x);
	RotToVec.z = sinf(Rot.x) * cosf(Rot.y);

	D3DXVec3Normalize(&RotToVec, &RotToVec);
	return RotToVec;
}
//===========================================================================================================

//=====================================================================
//�����x�N�g���ɑ΂��ē_�����E�ǂ���ɂ��邩�𔻒肷��
//=====================================================================
float CCalculation::DetermineSide3D(const D3DXVECTOR3& origin, const D3DXVECTOR3& direction, const D3DXVECTOR3& up, const D3DXVECTOR3& point)
{
	// ���΃x�N�g�����v�Z
	D3DXVECTOR3 Relative = { point.x - origin.x, point.y - origin.y, point.z - origin.z };

	// �E�����x�N�g�����v�Z
	D3DXVECTOR3 Right = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVec3Cross(&Right,&direction,&up);

	// �E�����x�N�g���Ƒ��΃x�N�g���̓��ς��v�Z
	float fDot = D3DXVec3Dot(&Right,&Relative);

	return fDot;
}
//===========================================================================================================

//=====================================================================
//�ړI�̍����֓��B���邽�߂̏����x�����߂�iv0 = ��2gh)
//=====================================================================
float CCalculation::GetInitialVelocityHeight(float fHeight, float fGravity)
{
	float fAim = sqrtf(2 * fGravity * fHeight);
	return fAim;
}
//===========================================================================================================

//=====================================================================
//�I�u�W�F�N�g�̌������z�[�~���O������
//=====================================================================
D3DXVECTOR3 CCalculation::HormingVecRotXZ(float& fRotMove, const D3DXVECTOR3& MyPos, const D3DXVECTOR3& AimPos, float CorrectionRot, float fSpeed)
{
	float fRotDest = 0.0f;//�ړI�̈ʒu�ւ̊p�x
	float fRotDiff = 0.0f;//���݂̊p�x�ƖړI�̊p�x�̍���
	float fVXaim = 0.0f;  //X�x�N�g��
	float fVZaim = 0.0f;  //Y�x�N�g��
	D3DXVECTOR3 Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�ړ���
	//===================================
	//XZ�x�N�g�����o��
	//===================================
	fVXaim = AimPos.x - MyPos.x;
	fVZaim = AimPos.z - MyPos.z;

	fRotDest = atan2f(fVXaim, fVZaim);

	//==========================================================================================================================================================

	//���݂̈ړ������̊p�x�ƁA�ړI�̈ړ������̊p�x�̍��������߂�
	fRotDiff = fRotDest - fRotMove;

	//===============================
	//�p�x�����̏C��
	//===============================
	//�����̍����̒����u�W���o�����b�N������v(3.14�𒴂�����߂������ɕ␳�j
	if (fRotDiff > D3DX_PI)
	{
		fRotDiff -= D3DX_PI * 2;
	}
	else if (fRotDiff < -D3DX_PI)
	{
		fRotDiff += D3DX_PI * 2;
	}

	//==============================================================================================
	fRotMove += fRotDiff * CorrectionRot;//�ړ������i�p�x�␳�j

	//�����̒����u�W���o�����b�N������v�i�J��������ɒl��3.14�`-3.14�̒��ɌŒ肵�����̂ŁE�E�E�j
	if (fRotMove >= D3DX_PI)
	{//3.14��-3.14�ɂ���
		fRotMove -= D3DX_PI * 2;
	}
	else if (fRotMove <= -D3DX_PI)
	{//-3.14��3.14�ɂ���
		fRotMove += D3DX_PI * 2;
	}

	//�ړ��ʂ��X�V����
	Move.x = sinf(fRotMove) * fSpeed;
	Move.z = cosf(fRotMove) * fSpeed;

	return Move;
}
//===========================================================================================================


//=====================================================================
//�����̕␳���s��
//=====================================================================
float CCalculation::CorrectionRot(float fRot)
{
	//�W���o�����b�N���������
	if (fRot > D3DX_PI)
	{
		fRot -= D3DX_PI * 2;
	}
	if (fRot < -D3DX_PI)
	{
		fRot += D3DX_PI * 2;
	}

	return fRot;
}
//===========================================================================================================
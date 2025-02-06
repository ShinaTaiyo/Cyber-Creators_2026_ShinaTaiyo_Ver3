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
//2D�����̌����̕␳���s��
//=========================================================
float CCalculation::CalculationCollectionRot2D(float fMyRot, float fRotAim, float fDecayRot, bool bCameraOffSet)
{
	float fRotDiff = 0.0f;//�����̍���
	float fCameraRot = CManager::GetCamera()->GetRot().y;
	fRotDiff = fRotAim - fMyRot;

	//�����̍����̒���
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
	{
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
	{
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
bool CCalculation::CaluclationMove(bool bUseStick, D3DXVECTOR3& Move, float fSpeed, MOVEAIM MoveAim, float& fRot)
{
	float fCameraRot = CManager::GetCamera()->GetRot().y;
	float fMoveX = 0.0f;                                            //X�����̈ړ���
	float fMoveZ = 0.0f;                                            //Z�����̈ړ���
	bool bMove = false;                                             //�ړ����Ă��邩�ǂ��� 
	bool bUseController = true;                                    //�R���g���[���[���g�p���邩�ǂ���
	bMove = CManager::GetInputJoypad()->GetLStickPress(8, 0.0f);//�R���g���[���[�̓���

	if (bMove == false)
	{//���̎��_�ŃR���g���[���[�̓��͂�����Ă��Ȃ��ꍇ�A�L�[���͂̎�t���J�n
		bUseController = false;//�R���g���[���[�͎g�p���Ȃ�
		if (CManager::GetInputKeyboard()->GetPress(DIK_W) == true || CManager::GetInputJoypad()->GetPress(CInputJoypad::JOYKEY::UP) == true)
		{
			fMoveZ = 1.0f;
		}
		else if (CManager::GetInputKeyboard()->GetPress(DIK_S) == true || CManager::GetInputJoypad()->GetPress(CInputJoypad::JOYKEY::DOWN) == true)
		{
			fMoveZ = -1.0f;
		}
		if (CManager::GetInputKeyboard()->GetPress(DIK_D) == true || CManager::GetInputJoypad()->GetPress(CInputJoypad::JOYKEY::RIGHT) == true)
		{
			fMoveX = 1.0f;
		}
		else if (CManager::GetInputKeyboard()->GetPress(DIK_A) == true || CManager::GetInputJoypad()->GetPress(CInputJoypad::JOYKEY::LEFT) == true)
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
	}
	if (bMove == true)
	{//�ړ���ԂȂ�
		//�J��������Ɍ��������߂�
		if (bUseController == true)
		{
			fRot = CManager::GetInputJoypad()->GetLStickAimRot();
			CManager::GetDebugText()->PrintDebugText("�X�e�B�b�N�̌����F%f\n",fRot);
			CManager::GetDebugText()->PrintDebugText("�J�����̌����F%f\n",fCameraRot);
			fRot += fCameraRot;
			CManager::GetDebugText()->PrintDebugText("�ړI�̌����F%f\n",fRot);
		}
		else
		{//�䂩��p�x�����߂�iZ���̐��������O�Ȃ̂ŁAZ������u�E�����v��X�����̊p�x�u�������v�����߂�j
			fRot = atan2f(fMoveX,fMoveZ) + fCameraRot;
			CManager::GetDebugText()->PrintDebugText("�ړI�̌����F%f\n", fRot);
		}
		switch (MoveAim)
		{
		case MOVEAIM_XY:
			Move.x = sinf(fRot) * fSpeed;
			Move.y = cosf(fRot) * fSpeed;
			break;
		case MOVEAIM_XZ:
			Move.x = sinf(fRot) * fSpeed;
			Move.z = cosf(fRot) * fSpeed;
			break;
		case MOVEAIM_ZY:
			Move.z = sinf(fRot) * fSpeed;
			Move.y = cosf(fRot) * fSpeed;
			break;
		default:
			break;
		}
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
	float fVLaim = 0.0f;                     //�����x�N�g��

	//==========================
	//�R�����x�N�g�������
	//==========================

	//�����x�N�g���̌v�Z
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

	bMinus = rand() % 2;
	Speed.x = static_cast<float>(rand() % nMathSpeed) / nDivisionSpeed;
	if (bMinus == true)
	{
		Speed.x *= -1;
	}
	bMinus = rand() % 2;
	Speed.y = static_cast<float>(rand() % nMathSpeed) / nDivisionSpeed;
	if (bMinus == true)
	{
		Speed.y *= -1;
	}
	bMinus = rand() % 2;
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
	D3DXCOLOR RaibowColor = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	RaibowColor.r = float(rand() % 100 + 1) / 100;
	RaibowColor.g = float(rand() % 100 + 1) / 100;
	RaibowColor.b = float(rand() % 100 + 1) / 100;
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
	D3DXMatrixInverse(&InvView, NULL, View);//�r���[�}�g���b�N�X�Ƃ̋t����i�J�����̎��_�A�����_�Ȃǂ𓥂܂��āA�ϊ�����Ă���̂Ŗ߂��j
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
// XZ���ʂƃX�N���[�����W�̌�_�Z�o�֐�
//=========================================================
D3DXVECTOR3* CCalculation::CalcScreenToXZ(D3DXVECTOR3* pout,float Sx,float Sy, int Screen_w, int Screen_h, D3DXMATRIX* View, D3DXMATRIX* Prj)
{
	D3DXVECTOR3 nearpos;
	D3DXVECTOR3 farpos;
	D3DXVECTOR3 ray;
	bool bCross = false;

	D3DXVECTOR3 Pos1 = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	CalcScreenToWorld(&nearpos, Sx, Sy, 0.0f, Screen_w, Screen_h, View, Prj);//�i�Ŗ��j�����J�����̈ʒu
	CalcScreenToWorld(&farpos, Sx, Sy, 1.0f, Screen_w, Screen_h, View, Prj); //�i�Ŗ��j�����`��͈͂̈�ԉ��̈ʒu
	ray = farpos - nearpos;

	D3DXVec3Normalize(&ray, &ray);

	nearpos *= -1;
	// ���Ƃ̌������N���Ă���ꍇ�͌�_��
	// �N���Ă��Ȃ��ꍇ�͉����̕ǂƂ̌�_���o��
	//if (ray.y <= 0) {
	//	// ����_
	//	float Lray = D3DXVec3Dot(&ray, &Pos1);
	//	float LP0 = D3DXVec3Dot(&nearpos, &Pos1);
	//	*pout = nearpos + (LP0 / Lray) * ray;
	//	bCross = true;
	//}
	//else {
	* pout = farpos;
	bCross = false;
	//}

	return pout;
}
//===========================================================================================================

//=========================================================
// ���C�Ƌ��̏Փ˔���
//=========================================================
bool CCalculation::CalcRaySphere(D3DXVECTOR3 LayPos, D3DXVECTOR3 LayVec, D3DXVECTOR3 SphereSenterPos, float r, D3DXVECTOR3& CollisionStartPos, D3DXVECTOR3& CollisoinEndPos)
{
	SphereSenterPos.x = SphereSenterPos.x - LayPos.x;
	SphereSenterPos.y = SphereSenterPos.y - LayPos.y;
	SphereSenterPos.z = SphereSenterPos.z - LayPos.z;

	float A = LayVec.x * LayVec.x + LayVec.y * LayVec.y + LayVec.z * LayVec.z;
	float B = LayVec.x * SphereSenterPos.x + LayVec.y * SphereSenterPos.y + LayVec.z * SphereSenterPos.z;
	float C = SphereSenterPos.x * SphereSenterPos.x + SphereSenterPos.y * SphereSenterPos.y + SphereSenterPos.z * SphereSenterPos.z - r * r;
	
	if (A == 0.0f)
		return false; // ���C�̒�����0

	float s = B * B - A * C;
	if (s < 0.0f)
		return false; // �Փ˂��Ă��Ȃ�

	s = sqrtf(s);
	float a1 = (B - s) / A;
	float a2 = (B + s) / A;

	if (a1 < 0.0f || a2 < 0.0f)
		return false; // ���C�̔��΂ŏՓ�

	//�ՓˊJ�n�ʒu�����߂�
	CollisionStartPos.x = LayPos.x + a1 * LayVec.x;
	CollisionStartPos.y = LayPos.y + a1 * LayVec.y;
	CollisionStartPos.z = LayPos.z + a1 * LayVec.z;

	//�ՓˏI���ʒu�����߂�
	CollisoinEndPos.x = LayPos.x + a2 * LayVec.x;
	CollisoinEndPos.y = LayPos.y + a2 * LayVec.y;
	CollisoinEndPos.z = LayPos.z + a2 * LayVec.z;

	return true;
}
//===========================================================================================================

//=========================================================
//�ړI�̈ʒu�Ƒ_���Ă���ʒu�Ƃ̃��C����v���Ă��邩�ǂ����𔻒�
//=========================================================
bool CCalculation::CalcMatchRay(D3DXVECTOR3 AimPos, float fSx, float fSy, int nScreen_w, int nScreen_h, D3DXMATRIX* View, D3DXMATRIX* Prj)
{
	D3DXVECTOR3 nearpos1;
	D3DXVECTOR3 farpos1;
	D3DXVECTOR3 ray1;

	D3DXVECTOR3 Pos1 = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	CalcScreenToWorld(&nearpos1,fSx,fSy, 0.0f, nScreen_w, nScreen_h, View, Prj);//�i�Ŗ��j�����J�����̈ʒu
	CalcScreenToWorld(&farpos1, fSx,fSy, 1.0f, nScreen_w, nScreen_h, View, Prj); //�i�Ŗ��j�����`��͈͂̈�ԉ��̈ʒu
	ray1 = farpos1 - nearpos1;

	D3DXVec3Normalize(&ray1, &ray1);

	D3DXVECTOR3 farpos2;
	D3DXVECTOR3 ray2;

	ray2 = AimPos - nearpos1;//�ړI�̈ʒu�ƃJ�����̈ʒu�̃��C�����߂�

	D3DXVec3Normalize(&ray2, &ray2);

	if (ray1.x >= ray2.x - 0.05f && ray1.x <= ray2.x + 0.05f &&
		ray1.y >= ray2.y - 0.05f && ray1.y <= ray2.y + 0.05f &&
		ray1.z >= ray2.z - 0.05f && ray1.z <= ray2.z + 0.05f)
	{
		return true;
	}
	return false;
}
//===========================================================================================================

//=========================================================
//�_�ƕӂ̋��������߂�
//=========================================================
float CCalculation::pointLineDistance(float cx, float cy, float x1, float y1, float x2, float y2)
{
	float dx = x2 - x1;
	float dy = y2 - y1;

	// �~�̒��S����ӂ̒[�_�܂ł̃x�N�g�����v�Z
	float t = ((cx - x1) * dx + (cy - y1) * dy) / (dx * dx + dy * dy);

	// t�͈̔͂�[0, 1]�ɐ������A�~�̒��S�ɍł��߂��_��������
	t = (std::max)(0.0f, (std::min)(1.0f, t));

	// �ŒZ�������v�Z
	float closestX = x1 + t * dx;
	float closestY = y1 + t * dy;
	float distanceX = cx - closestX;
	float distanceY = cy - closestY;

	return std::sqrt(distanceX * distanceX + distanceY * distanceY);
}
//===========================================================================================================

//=========================================================
//�����`�Ɖ~�̓����蔻��i2D)
//=========================================================
bool CCalculation::checkCollisionCircleRectangle(float cx, float cy, float r, float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4)
{
	// �l�p�`�̊e�ӂɑ΂��ĉ~�Ƃ̋������v�Z
	if (pointLineDistance(cx, cy, x1, y1, x2, y2) <= r) return true;
	if (pointLineDistance(cx, cy, x2, y2, x3, y3) <= r) return true;
	if (pointLineDistance(cx, cy, x3, y3, x4, y4) <= r) return true;
	if (pointLineDistance(cx, cy, x4, y4, x1, y1) <= r) return true;

	// �ǂ̕ӂƂ��������Ă��Ȃ��ꍇ��false
	return false;
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
//�w�肵���������擾����
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
	Vec = AimPos - MyPos;

	if (bNormalize == true)
	{//���K�����邩�ǂ���
		D3DXVec3Normalize(&Vec, &Vec);
	}
	return Vec;
}
//===========================================================================================================

//=========================================================
//�p���v�Z����
//=========================================================
float CCalculation::CalcElevationAngle(const D3DXVECTOR3& MyPos, const D3DXVECTOR3& AimPos)
{
	D3DXVECTOR3 Vec = CalcVec(MyPos, AimPos, false);
	float fElevAngle = atan2f(Vec.y, sqrtf(powf(Vec.x, 2) + powf(Vec.z, 2)));
	return fElevAngle;
}
//===========================================================================================================

//=========================================================
//��̃x�N�g�����Ȃ��p
//=========================================================
float CCalculation::GetAngleBetweenVectors(const D3DXVECTOR3& v1, const D3DXVECTOR3& v2)
{
	// �x�N�g���̐��K��
	D3DXVECTOR3 v1_normalized = v1;
	D3DXVECTOR3 v2_normalized = v2;
	D3DXVec3Normalize(&v1_normalized, &v1);
	D3DXVec3Normalize(&v2_normalized, &v2);

	// ���ς��v�Z
	float dotProduct = D3DXVec3Dot(&v1_normalized, &v2_normalized);

	// ���ς���p�x���v�Z (acos�Ń��W�A���ɕϊ�)(v1 *v2 = |v1| * |v2| cos��)�Ȃ̂ŁA�t�O�p�֐������߂�Ηǂ�
	float angle = acosf(dotProduct);
	return angle; // ���ʂ̓��W�A��
}
//===========================================================================================================


//=====================================================================
//�v���C���[���猩�Đ��`�͈͂ɃI�u�W�F�N�g�������Ă��邩�ǂ����𔻒�
//=====================================================================
bool CCalculation::IsObjectInFieldOfView(const D3DXVECTOR3& PlayerPos, const D3DXVECTOR3& PlayerViewDir, const D3DXVECTOR3& ObjectPos, float fovAngle, float maxDistance)
{

	// �v���C���[����I�u�W�F�N�g�ւ̕����x�N�g�����v�Z
	D3DXVECTOR3 objectDir = ObjectPos - PlayerPos;

	// �v���C���[�̎����ƃI�u�W�F�N�g�̕����x�N�g���Ƃ̊p�x���v�Z
	float angle = GetAngleBetweenVectors(PlayerViewDir, objectDir);

	// �I�u�W�F�N�g������p�x���ɓ����Ă��邩�ǂ����𔻒�
	if (angle <= fovAngle) {
		// ����͈͓��ɂ��邩�m�F�i����������j
		float distance = D3DXVec3Length(&objectDir);
		if (distance <= maxDistance) {
			return true; // ������������͈͓�
		}
	}

	return false; // ����p�x�O�܂��͋�������������
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
	// �����Ŕ���
	//if (dot > 0) {
	//	return "�E��";
	//}
	//else if (dot < 0) {
	//	return "����";
	//}
	//else {
	//	return "���ʏ�";
	//}
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
	//�����̍����̒���(3.14�𒴂�����߂������ɕ␳�j
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

	//�����̒����i�J��������ɒl��3.14�`-3.14�̒��ɌŒ肵�����̂ŁE�E�E�j
	if (fRotMove >= D3DX_PI)
	{//3.14��-3.14�ɂ���
		fRotMove -= D3DX_PI * 2;
	}
	else if (fRotMove <= -D3DX_PI)
	{//-3.14��3.14�ɂ���
		fRotMove += D3DX_PI * 2;
	}

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

//=========================================================
//�ړI�̈ʒu�ւ̌��������߂�
//=========================================================
void CCalculation::CalcRotToTarget(const D3DXVECTOR3& MyPos, const D3DXVECTOR3& AimPos, float& OutYaw, float& OutPitch)
{
	D3DXVECTOR3 VecAim = D3DXVECTOR3(0.0f, 0.0f, 0.0f); // �����x�N�g��
	float fVLaim = 0.0f; // �x�N�g���̑傫��

	// �����x�N�g�������߂�
	VecAim.x = AimPos.x - MyPos.x;
	VecAim.y = AimPos.y - MyPos.y;
	VecAim.z = AimPos.z - MyPos.z;

	// �x�N�g���̑傫�������߂�
	fVLaim = sqrtf(VecAim.x * VecAim.x + VecAim.y * VecAim.y + VecAim.z * VecAim.z);

	if (fVLaim != 0.0f) { // �[�����Z��h��
		VecAim.x /= fVLaim; // ���K��
		VecAim.y /= fVLaim;
		VecAim.z /= fVLaim;
	}

	// Yaw (������])
	OutYaw = atan2f(VecAim.x, VecAim.z);

	// Pitch (������])
	OutPitch = asinf(-VecAim.y);
}
//===========================================================================================================

//=========================================================
//Yaw��Pitch��p���ĖړI�̊p�x�����߂�
//=========================================================
D3DXVECTOR3 CCalculation::CalcDirectionFromYawPitch(const float Yaw, const float Pitch)
{
	D3DXVECTOR3 direction;

	// Yaw �� Pitch ���g�p���ĕ����x�N�g�����v�Z
	direction.x = cosf(Pitch) * sinf(Yaw);  // X����
	direction.y = sinf(Pitch);              // Y����
	direction.z = cosf(Pitch) * cosf(Yaw);  // Z����

	return direction;
}
//===========================================================================================================

//=========================================================
//�ړI�̈ʒu�ւ̊p�x���܂Ƃ߂ċ��߂�
//=========================================================
D3DXVECTOR3 CCalculation::CalcSummarizeRotToTarget(const D3DXVECTOR3& MyPos, const D3DXVECTOR3& AimPos)
{
	float fYaw = 0.0f;
	float fPitch = 0.0f;
	D3DXVECTOR3 Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	CalcRotToTarget(MyPos, AimPos, fYaw, fPitch);
	Rot = CalcDirectionFromYawPitch(fYaw, fPitch);
	return Rot;
}
//===========================================================================================================
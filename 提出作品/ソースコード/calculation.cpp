//===========================================================================================================
//
//�W���Q�X���F�v�Z�p�̃N���X�����[calculation.cpp]
//Author:ShinaTaiyo
//
//===========================================================================================================

//*******************************************************************************************************************
// �C���N���[�h
//*******************************************************************************************************************
#include "calculation.h"
#include "manager.h"
#include "camera.h"
#include "renderer.h"
#include "particle.h"
#include "attack.h"
#include "input.h"
#include "debugtext.h"

////=========================================================
////�������v�Z����
////=========================================================
//float CCalculation::CalculationLength(D3DXVECTOR3 Pos, D3DXVECTOR3 PurposePos)
//{
//	float fLength = 0.0f;//�����i�[�p
//	fLength = sqrtf(powf(PurposePos.x - Pos.x, 2) + powf(PurposePos.y - Pos.y, 2) + powf(PurposePos.z - Pos.z, 2));//�ړI�̈ʒu�Ƃ̋��������߂�
//	return fLength;
//}
////==========================================================================================================

////=========================================================
////XY�x�N�g�����v�Z����
////=========================================================
//float CCalculation::CalculationXYaim(D3DXVECTOR3 Pos, D3DXVECTOR3 PurposePos)
//{
//	float fVXaim = PurposePos.x - Pos.x;//X�����̃x�N�g�������߂�
//	float fVYaim = PurposePos.y - Pos.y;//Y�����̃x�N�g�������߂�
//	float fVLaim = atan2f(fVXaim, fVYaim);//��̃x�N�g���̊p�x�����߂�
//	return fVLaim;
//}
////==========================================================================================================

////=========================================================
////XY�����̗����x�N�g����p�ӂ���
////=========================================================
//float CCalculation::CalculationRandVecXY()
//{
//	float fRandRot = float(rand() % 628 + 1) / 100;//D3DX_PI * 2�i��6.28)�������_���ŋ��߂�
//	return fRandRot;
//}
////===========================================================================================================

////=========================================================
////2D�����̌����̕␳���s��
////=========================================================
//float CCalculation::CalculationCollectionRot2D(float fMyRot, float fRotAim, float fDecayRot, bool bCameraOffSet)
//{
//	float fRotDiff = 0.0f;                                 //�����̍���
//	float fCameraRot = CManager::GetCamera()->GetRot().y;  //�J������Yaw���擾
//	fRotDiff = fRotAim - fMyRot;
//
//	//�����̍����̒����i�W���o�����b�N��������邽��)
//	if (fRotDiff > D3DX_PI)
//	{
//		fRotDiff -= D3DX_PI * 2;
//	}
//	else if (fRotDiff < -D3DX_PI)
//	{
//		fRotDiff += D3DX_PI * 2;
//	}
//
//	//���X�ɖړI�̌����֍��킹�Ă���
//	fMyRot += fRotDiff * fDecayRot;
//
//	if (bCameraOffSet == true)
//	{//�J��������Ɍ��������߂邩�ǂ���
//		//�����̒����i�J��������ɒl��3.14�`-3.14�̒��ɌŒ肵�����̂ŁE�E�E�j
//		if (fMyRot >= D3DX_PI + fCameraRot)
//		{//3.14��-3.14�ɂ���
//			fMyRot -= D3DX_PI * 2;
//		}
//		else if (fMyRot <= -D3DX_PI + fCameraRot)
//		{//-3.14��3.14�ɂ���
//			fMyRot += D3DX_PI * 2;
//		}
//	}
//	else
//	{//���ʂɌ��������߂�
//		//�����̒���
//		if (fMyRot >= D3DX_PI)
//		{//3.14��-3.14�ɂ���
//			fMyRot -= D3DX_PI * 2;
//		}
//		else if (fMyRot <= -D3DX_PI)
//		{//-3.14��3.14�ɂ���
//			fMyRot += D3DX_PI * 2;
//		}
//	}
//	return fMyRot;
//}
////===========================================================================================================

////=========================================================
////�ړ������ւ̊p�x���v�Z����
////=========================================================
//bool CCalculation::CaluclationMove(bool bUseStick, D3DXVECTOR3& Pos, D3DXVECTOR3& Move, float fSpeed, MOVEAIM MoveAim, float fAddMoveRot, float& fRot)
//{
//	// === �ϐ� ===
//	float fMoveX = 0.0f; // X�����̈ړ���
//	float fMoveZ = 0.0f; // Z�����̈ړ���
//	bool bMove = false;  // �ړ����Ă��邩�ǂ��� 
//	bool bUseController = true; // �R���g���[���[���g�p���邩�ǂ���
//	bMove = CManager::GetInputJoypad()->GetLStickPress(8, 0.0f); // �R���g���[���[�̓���
//	D3DXVECTOR3 PosFuture = { 0.0f,0.0f,0.0f }; // 1f��̈ʒu
//
//	// === ���� ===
//
//	// ���̎��_�ŃR���g���[���[�̓��͂�����Ă��Ȃ��ꍇ�A�L�[���͂̎�t���J�n
//	if (bMove == false)
//	{
//		bUseController = false;//�R���g���[���[�͎g�p���Ȃ�
//		if (CManager::GetInputKeyboard()->GetPress(DIK_W) == true || CManager::GetInputJoypad()->GetPress(CInputJoypad::JOYKEY::UP) == true)
//		{//�O�ւ̃x�N�g��
//			fMoveZ = 1.0f;
//		}
//		else if (CManager::GetInputKeyboard()->GetPress(DIK_S) == true || CManager::GetInputJoypad()->GetPress(CInputJoypad::JOYKEY::DOWN) == true)
//		{//���ւ̃x�N�g��
//			fMoveZ = -1.0f;
//		}
//		if (CManager::GetInputKeyboard()->GetPress(DIK_D) == true || CManager::GetInputJoypad()->GetPress(CInputJoypad::JOYKEY::RIGHT) == true)
//		{//�E�ւ̃x�N�g��
//			fMoveX = 1.0f;
//		}
//		else if (CManager::GetInputKeyboard()->GetPress(DIK_A) == true || CManager::GetInputJoypad()->GetPress(CInputJoypad::JOYKEY::LEFT) == true)
//		{//���ւ̃x�N�g��
//			fMoveX = -1.0f;
//		}
//
//		if (fMoveX != 0.0f || fMoveZ != 0.0f)
//		{//��L�̏����Ń{�^������͂��Ă����瓮���Ă���Ƃ݂Ȃ�
//			bMove = true;//�ړ����
//
//		}
//		else
//		{
//			bMove = false;//�ҋ@���
//		}
//	}
//	// �ړ���ԂȂ�
//	if (bMove == true)
//	{
//		float fMoveRot = 0.0f;//�ړ�������������߂�
//
//		// �R���g���[���[�̍��X�e�B�b�N����p�x�����߂�
//		if (bUseController == true)
//		{
//			fMoveRot = CManager::GetInputJoypad()->GetLStickAimRot();
//		}
//		// �䂩��p�x�����߂�iZ���̐��������O�Ȃ̂ŁAZ������u�E�����v��X�����̊p�x�u�������v�����߂�j
//		else
//		{
//			fMoveRot = atan2f(fMoveX, fMoveZ);
//		}
//
//		fMoveRot += fAddMoveRot; // ���������Z���␳
//
//		// �ǂ̖ʂ����Ɉړ����邩�����߂�
//		switch (MoveAim)
//		{
//		case MOVEAIM_XY: // XY��������Ɉړ�����
//			Move.x = sinf(fMoveRot) * fSpeed;
//			Move.y = cosf(fMoveRot) * fSpeed;
//			break;
//		case MOVEAIM_XZ: // XZ��������Ɉړ�����
//			Move.x = sinf(fMoveRot) * fSpeed;
//			Move.z = cosf(fMoveRot) * fSpeed;
//			break;
//		case MOVEAIM_ZY: // ZY��������Ɉړ�����
//			Move.z = sinf(fMoveRot) * fSpeed;
//			Move.y = cosf(fMoveRot) * fSpeed;
//			break;
//		default:
//			break;
//		}
//
//		// 1f��̈ʒu���v�Z
//		PosFuture = Pos + Move;
//
//		// 1f��̈ʒu�ƌ��݂̈ʒu�̃x�N�g����������ׂ��p�x�����߂�
//		fRot = atan2f(PosFuture.x - Pos.x, PosFuture.z - Pos.z);
//	}
//	return bMove;
//
//}
////===========================================================================================================


////=========================================================
////�ړI�̈ʒu�ւ̈ړ��ʂ��v�Z����
////=========================================================
//D3DXVECTOR3 CCalculation::Calculation3DVec(D3DXVECTOR3 MyPos, D3DXVECTOR3 AimPos, float fSpeed)
//{
//	// === �����Ɏg�p�������錾�A������ ===
//
//	D3DXVECTOR3 VecAim = D3DXVECTOR3(0.0f,0.0f,0.0f);       // �ړI�n�ւ̃x�N�g��
//	D3DXVECTOR3 ResultMove = D3DXVECTOR3(0.0f,0.0f,0.0f);   // �ړ��ʌ���
//	float fVLaim = 0.0f; // �x�N�g������
//
//	// === �v�Z�J�n ===
//
//	// �x�N�g���v�Z
//	VecAim = AimPos - MyPos;
//
//	//�x�N�g���̑傫�������߂�
//	fVLaim = sqrtf(powf(VecAim.x,2) + powf(VecAim.y,2) + powf(VecAim.z,2));
//
//	//�����x�N�g���𐳋K�����A���ꂼ��̎��ɑ΂���ړ��ʂ����߂�
//	ResultMove.x = VecAim.x / fVLaim * fSpeed;
//	ResultMove.y = VecAim.y / fVLaim * fSpeed;
//	ResultMove.z = VecAim.z / fVLaim * fSpeed;
//
//	return ResultMove;
//}
////===========================================================================================================

////=========================================================
////�ړI�n�ւ̃x�N�g����Yaw��Pitch�ɕϊ�����
////=========================================================
//D3DXVECTOR2 CCalculation::VectorToYawPitch(const D3DXVECTOR3& MyPos, const D3DXVECTOR3& AimPos)
//{
//	D3DXVECTOR2 YawPitch = { 0.0f,0.0f };//Yaw��Pitch���v�Z����
//
//	YawPitch.y = atan2f(AimPos.x - MyPos.x, AimPos.z - MyPos.z);//XZ�����̔��ˊp�x�������̔䂩��Yaw�����߂�
//	YawPitch.x = atan2f(AimPos.y - MyPos.y, sqrtf(powf(AimPos.x - MyPos.x, 2) + powf(AimPos.z - MyPos.z, 2)));//XZ�����̋���������Pitch�����߂�
//
//	return YawPitch;
//}
//
////=========================================================
////���F���v�Z����
////=========================================================
//D3DXCOLOR CCalculation::CalRaibowColor()
//{
//    //RGB�̒l�������_���Ɍ��ߑ������F��\������
//	D3DXCOLOR RaibowColor = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
//	RaibowColor.r = float(rand() % 100 + 1) / 100;//R�l�����߂�
//	RaibowColor.g = float(rand() % 100 + 1) / 100;//G�l�����߂�
//	RaibowColor.b = float(rand() % 100 + 1) / 100;//B�l�����߂�
//	return RaibowColor;
//}
////===========================================================================================================

////=========================================================
//// �X�N���[�����W�����[���h���W�ɕϊ�
////=========================================================
//D3DXVECTOR3* CCalculation::CalcScreenToWorld(D3DXVECTOR3* pout, float Sx, float Sy, float fZ, int Screen_w, int Screen_h, D3DXMATRIX* View, D3DXMATRIX* Prj)
//{
//	// �e�s��̋t�s����Z�o�i�r���[�A�v���W�F�N�V�����}�g���b�N�X�̋t�s���������̂́A�J�����̈ʒu��2D��UI���o�Ă���ƒ�`�ł��邩��)
//	// �t�s��Ƃ́A�l�ɒl-1�������A�|�����킳���O�ɖ߂�����
//	D3DXMATRIX InvView, InvPrj, VP, InvViewport;
//	D3DXMatrixInverse(&InvView, NULL, View);//�r���[�}�g���b�N�X�Ƃ̋t����������ă��[���h���W�����߂�i�r���[�}�g���b�N�X�̌�����ʒu�ɉ����ĕϊ�����Ă���̂Łj
//	D3DXMatrixInverse(&InvPrj, NULL, Prj);  //�v���W�F�N�V�����}�g���b�N�X�Ƃ̋t�s��i�������i���s���e�A����p�Ȃǁj��ς��Ă���̂ŁA�t�s����|�����킹�A���Ƃɖ߂��K�v������j
//	D3DXMatrixIdentity(&VP);
//
//	// �X�P�[�����O�̒l��ς��Ă���B�X�N���[�����W�̒��S����ʒ����ɂ���iNDC���W���g�p���ĕϊ�����̂ŁA�X�N���[�����W�͂O�`�P�Q�W�O�A�Ȃ��A
//	//NDC���W�́[�P�`�P������A�ϊ�����ۂɍ��킹��K�v������(�P�Q�W�O�@���@�U�S�O�j�A�i�O�@���@�[�U�S�O)
//	VP._11 = Screen_w / 2.0f; VP._22 = -Screen_h / 2.0f;//�X�P�[�����O�����iX���̂܂܁AY�͏�𐳂Ɂj
//	VP._41 = Screen_w / 2.0f; VP._42 = Screen_h / 2.0f; //���s�ړ������i���S�����_�ɍ��킹��)
//	D3DXMatrixInverse(&InvViewport, NULL, &VP);//NDC���W�ɍ��킹��
//
//	//����
//	D3DXVECTOR3 MyPos = D3DXVECTOR3(Sx,Sy,fZ);//�X�N���[�����W�Ɛ[�x�l
//
//	// �t�ϊ�
//	D3DXMATRIX tmp = InvViewport * InvPrj * InvView;// ���[���h���W�����߂�iInvViewport�u�X�N���[�����W��NDC���W�v�AInvPrj�uNDC���W���r���[��ԁv�AInvView�u�r���[��ԁ����[���h���W�v)
//	                                                //���̍s��ɐ[�x�l�ƃX�N���[�����W�����킹�邱�ƂŁA�[�x�l�ɍ��킹�����W�ϊ����\�ɂȂ�)
//	D3DXVec3TransformCoord(pout, &MyPos, &tmp);     //�ʒu�����߂�
//	return pout;
//}
////===========================================================================================================

////=========================================================
//// ���[���h���W���X�N���[�����W�ɕϊ�����
////=========================================================
//D3DXVECTOR3 CCalculation::CalcWorldToScreenNoViewport(D3DXVECTOR3 worldPos, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, float screenWidth, float screenHeight)
//{
//	//���[���h���W���N���b�v���W�ɕϊ�
//	D3DXVECTOR4 ClipSpacePos;
//	D3DXMATRIX mtxTrans;
//
//	mtxTrans = viewMatrix * projectionMatrix; // �r���[�|�[�g�A�v���W�F�N�V�����ϊ�
//	D3DXVec3Transform(&ClipSpacePos, &worldPos, &mtxTrans); // ��L�̕ϊ��������̃��[���h���W�̈ʒu�����߂�
//
//	//�������Z�i�N���b�v���W����NDC��Ԃցj�i�{����GPU������ɂ�邪�A�f�[�^�Ƃ��Ă͏o�Ȃ��̂Ŏ����ŋ��߂�)
//	if (ClipSpacePos.w != 0.0f)
//	{//X�AY�AZ�́A�J��������̑��ΓI�Ȉʒu��\���Ă���i�ϊ����ꂽ�ʒu)�B
//	 //W�́A�J��������̋����i�[�x���j��\���Ă���
//		ClipSpacePos.x /= ClipSpacePos.w;
//		ClipSpacePos.y /= ClipSpacePos.w;
//		ClipSpacePos.z /= ClipSpacePos.w;
//	}
//
//	//�X�N���[�����W�֕ϊ�
//	D3DXVECTOR3 ScreenPos;
//	ScreenPos.x = (ClipSpacePos.x * 0.5f + 0.5f) * screenWidth;           //�^�񒆂��O�ɂ��邽�߁{�O�D�T���A�[�P�Ȃ獶�[�A�{�P�Ȃ�E�[�ƂȂ�
//	ScreenPos.y = (1.0f - (ClipSpacePos.y * 0.5f + 0.5f)) * screenHeight; //�^�񒆂��O�ɂ��邽�߁{�O�D�T���A�������ƂȂ�A�オ�O�ƂȂ�̂ŁA�P�̎��͂O�ɂȂ�A�[�P�̎��͂P�ƂȂ�
//	ScreenPos.z = ClipSpacePos.z;                                         //�[�x�l�i�O�`�P�j�͈̔�
//
//	return ScreenPos;
//}
////===========================================================================================================

////=========================================================
////�������v�Z����
////=========================================================
//int CCalculation::CalculationDigit(int nNum)
//{
//	int nDigit = 0;
//	if (nNum == 0)
//	{//���l���Ȃ��̂�1��
//		return 1;
//	}
//
//	while (nNum != 0) {//int��10�Ŋ����Ēl���O�ɂȂ�܂ŌJ��Ԃ��A�������v�Z����
//		nNum /= 10;
//		nDigit++;
//	}
//	return nDigit;
//}

////=========================================================
////�w�肵���������擾����(number��345�Aposition��2��������)��345 / 100 = 3.45 = 3 % 10 = 3)
////=========================================================
//int CCalculation::getDigit(int number, int position)
//{
//	int divisor = static_cast<int>(std::pow(10, position));
//	return (number / divisor) % 10;
//}
////===========================================================================================================

////=========================================================
////�x�N�g�����v�Z����
////=========================================================
//D3DXVECTOR3 CCalculation::CalcVec(D3DXVECTOR3 MyPos, D3DXVECTOR3 AimPos, bool bNormalize)
//{
//	D3DXVECTOR3 Vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//
//	//�x�N�g�������߂�
//	Vec = AimPos - MyPos;//�ړI�̈ʒu�Ƃ̃x�N�g�������߂�
//
//	if (bNormalize == true)
//	{//���K�����邩�ǂ���
//		D3DXVec3Normalize(&Vec, &Vec);
//	}
//	return Vec;
//}
////===========================================================================================================

////=====================================================================
////���W�A��������x�N�g���ɕϊ�����
////=====================================================================
//D3DXVECTOR3 CCalculation::RadToVec(const D3DXVECTOR3& Rot)
//{
//	D3DXVECTOR3 RotToVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//	RotToVec.x = cosf(Rot.x) * sinf(Rot.y);
//	RotToVec.y = sinf(Rot.x);
//	RotToVec.z = cosf(Rot.x) * cosf(Rot.y);
//
//	D3DXVec3Normalize(&RotToVec, &RotToVec);
//	return RotToVec;
//}
////===========================================================================================================
//
////=====================================================================
////�����x�N�g���ɑ΂��ē_�����E�ǂ���ɂ��邩�𔻒肷��
////=====================================================================
//float CCalculation::DetermineSide3D(const D3DXVECTOR3& origin, const D3DXVECTOR3& direction, const D3DXVECTOR3& up, const D3DXVECTOR3& point)
//{
//	// ���΃x�N�g�����v�Z
//	D3DXVECTOR3 Relative = { point.x - origin.x, point.y - origin.y, point.z - origin.z };
//
//	// �E�����x�N�g�����v�Z
//	D3DXVECTOR3 Right = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//	D3DXVec3Cross(&Right,&direction,&up);
//
//	// �E�����x�N�g���Ƒ��΃x�N�g���̓��ς��v�Z
//	float fDot = D3DXVec3Dot(&Right,&Relative);
//
//	return fDot;
//}
////===========================================================================================================

////=====================================================================
////�I�u�W�F�N�g�̌������z�[�~���O������
////=====================================================================
//D3DXVECTOR3 CCalculation::HormingVecRotXZ(float& fRotMove, const D3DXVECTOR3& MyPos, const D3DXVECTOR3& AimPos, float CorrectionRot, float fSpeed)
//{
//	float fRotDest = 0.0f;//�ړI�̈ʒu�ւ̊p�x
//	float fRotDiff = 0.0f;//���݂̊p�x�ƖړI�̊p�x�̍���
//	float fVXaim = 0.0f;  //X�x�N�g��
//	float fVZaim = 0.0f;  //Y�x�N�g��
//	D3DXVECTOR3 Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�ړ���
//	//===================================
//	//XZ�x�N�g�����o��
//	//===================================
//	fVXaim = AimPos.x - MyPos.x;
//	fVZaim = AimPos.z - MyPos.z;
//
//	fRotDest = atan2f(fVXaim, fVZaim);
//
//	//==========================================================================================================================================================
//
//	//���݂̈ړ������̊p�x�ƁA�ړI�̈ړ������̊p�x�̍��������߂�
//	fRotDiff = fRotDest - fRotMove;
//
//	//===============================
//	//�p�x�����̏C��
//	//===============================
//	//�����̍����̒����u�W���o�����b�N������v(3.14�𒴂�����߂������ɕ␳�j
//	if (fRotDiff > D3DX_PI)
//	{
//		fRotDiff -= D3DX_PI * 2;
//	}
//	else if (fRotDiff < -D3DX_PI)
//	{
//		fRotDiff += D3DX_PI * 2;
//	}
//
//	//==============================================================================================
//	fRotMove += fRotDiff * CorrectionRot;//�ړ������i�p�x�␳�j
//
//	//�����̒����u�W���o�����b�N������v�i�J��������ɒl��3.14�`-3.14�̒��ɌŒ肵�����̂ŁE�E�E�j
//	if (fRotMove >= D3DX_PI)
//	{//3.14��-3.14�ɂ���
//		fRotMove -= D3DX_PI * 2;
//	}
//	else if (fRotMove <= -D3DX_PI)
//	{//-3.14��3.14�ɂ���
//		fRotMove += D3DX_PI * 2;
//	}
//
//	//�ړ��ʂ��X�V����
//	Move.x = sinf(fRotMove) * fSpeed;
//	Move.z = cosf(fRotMove) * fSpeed;
//
//	return Move;
//}
////===========================================================================================================


////=====================================================================
////�����̕␳���s��
////=====================================================================
//float CCalculation::CorrectionRot(float fRot)
//{
//	// �����̒l���~�����ɌŒ�
//	if (fRot > D3DX_PI)
//	{
//		fRot -= D3DX_PI * 2;
//	}
//	if (fRot < -D3DX_PI)
//	{
//		fRot += D3DX_PI * 2;
//	}
//
//	return fRot;
//}
////===========================================================================================================

//*******************************************************************************************************************
// �x�N�g�����O���
//*******************************************************************************************************************

//===================================================================================================================
// ����������x�N�g���֕ς���
//===================================================================================================================
D3DXVECTOR3 Calculation::Vec::RadTo(const D3DXVECTOR3& Rot)
{
	// === �����Ɏg�p�������錾�A������ ===

	D3DXVECTOR3 RotToVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f); // ���W�A�����x�N�g���ɂ���ϐ� 

	// === �v�Z�J�n ===

	RotToVec.x = cosf(Rot.x) * sinf(Rot.y);
	RotToVec.y = sinf(Rot.x);
	RotToVec.z = cosf(Rot.x) * cosf(Rot.y);

	// ���K������
	D3DXVec3Normalize(&RotToVec, &RotToVec);

	return RotToVec;
}

//===================================================================================================================
// �x�N�g���ɑ΂����E�ǂ���ɂ��邩�𔻒肷��
//===================================================================================================================
float Calculation::Vec::DetermineSide(const D3DXVECTOR3& origin, const D3DXVECTOR3& direction, const D3DXVECTOR3& up, const D3DXVECTOR3& point)
{
	// ���΃x�N�g�����v�Z
	D3DXVECTOR3 Relative = { point.x - origin.x, point.y - origin.y, point.z - origin.z };

	// �E�����x�N�g�����v�Z
	D3DXVECTOR3 Right = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVec3Cross(&Right, &direction, &up);

	// �E�����x�N�g���Ƒ��΃x�N�g���̓��ς��v�Z
	float fDot = D3DXVec3Dot(&Right, &Relative);

	return fDot;
}

//*******************************************************************************************************************
// �ړ��ʖ��O���
//*******************************************************************************************************************

//===================================================================================================================
// �O�����ړ��ʗ����l�����߂�
//===================================================================================================================
D3DXVECTOR3 Calculation::Move::Rand3D(int nMaxSpeed, int nDivisionMaxSpeed)
{
	// === �����Ɏg�p�������錾�A������ ===

	D3DXVECTOR3 Speed = { 0.0f,0.0f,0.0f }; // ���x
	bool bMinus = false; // �l���}�C�i�X�ɂ��邩�ǂ���

	// === X�����̃����_���ȑ��x�����߂� ===

	// X���̑��x�̗����l
	Speed.x = static_cast<float>(rand() % nMaxSpeed) / nDivisionMaxSpeed;

	bMinus = rand() % 2; // �E�����������߂�
	
	// �l�Ƀ}�C�i�X���|����
	if (bMinus)
	{
		Speed.x *= -1;
	}

	// === Y�����̃����_���ȑ��x�����߂�

	// Y���̑��x�̗����l
	Speed.y = static_cast<float>(rand() % nMaxSpeed) / nDivisionMaxSpeed;

	bMinus = rand() % 2; // �ォ�������߂�

	// �l�Ƀ}�C�i�X���|����
	if (bMinus)
	{
		Speed.y *= -1;
	}

	// === Z�����̃����_���Ȍ��������߂� ===

	bMinus = rand() % 2; // �O����납�����߂�

	// Z���̑��x�̗����l
	Speed.z = static_cast<float>(rand() % nMaxSpeed) / nDivisionMaxSpeed;

	// �l�Ƀ}�C�i�X���|����
	if (bMinus)
	{
		Speed.z *= -1;
	}

	return Speed;
}

//===================================================================================================================
// �ړI�n�ւ̈ړ��ʌv�Z
//===================================================================================================================
D3DXVECTOR3 Calculation::Move::DirrectionToTarget(D3DXVECTOR3 SelfPos, D3DXVECTOR3 AimPos, float fSpeed)
{
	// === �����Ɏg�p�������錾�A������ ===

	D3DXVECTOR3 
		VecAim = D3DXVECTOR3(0.0f, 0.0f, 0.0f),       // �ړI�n�ւ̃x�N�g��
	    ResultMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   // �ړ��ʌ���

	float fVLaim = 0.0f; // �x�N�g������

	// === �v�Z�J�n ===

	// �x�N�g���v�Z
	VecAim = AimPos - SelfPos;

	//�x�N�g���̑傫�������߂�
	fVLaim = sqrtf(powf(VecAim.x, 2) + powf(VecAim.y, 2) + powf(VecAim.z, 2));

	//�����x�N�g���𐳋K�����A���ꂼ��̎��ɑ΂���ړ��ʂ����߂�
	ResultMove.x = VecAim.x / fVLaim * fSpeed;
	ResultMove.y = VecAim.y / fVLaim * fSpeed;
	ResultMove.z = VecAim.z / fVLaim * fSpeed;

	return ResultMove;
}

//===================================================================================================================
// ���͂ɂ��ړ�����
//===================================================================================================================
bool Calculation::Move::MovementInput(bool bUseStick, D3DXVECTOR3& Pos, D3DXVECTOR3& Move, float fSpeed,
	MOVEAIM MoveAim, float fAddAimRot, float& fRot)
{
	// === �����Ɏg�p�������錾�A������ ===

	float
		fMoveX = 0.0f, // X�����̈ړ���
	    fMoveZ = 0.0f; // Z�����̈ړ���

	bool 
		bMove = false,         // �ړ����Ă��邩�ǂ��� 
        bUseController = true; // �R���g���[���[���g�p���邩�ǂ���


	D3DXVECTOR3 PosFuture = { 0.0f,0.0f,0.0f }; // 1f��̈ʒu

	// === ���͂ɂ��ړ��������J�n ===

	bMove = CManager::GetInputJoypad()->GetLStickPress(8, 0.0f); // �R���g���[���[�̓���

	// ���̎��_�ŃR���g���[���[�̓��͂�����Ă��Ȃ��ꍇ�A�L�[���͂̎�t���J�n
	if (bMove == false)
	{
		bUseController = false;//�R���g���[���[�͎g�p���Ȃ�

		// �O�����ɓ��͂����Ȃ�O�����ւ̃x�N�g��
		if (CManager::GetInputKeyboard()->GetPress(DIK_W) == true || CManager::GetInputJoypad()->GetPress(CInputJoypad::JOYKEY::UP) == true)
		{
			fMoveZ = 1.0f;
		}
		//�������ɓ��͂����Ȃ�������ւ̃x�N�g��
		else if (CManager::GetInputKeyboard()->GetPress(DIK_S) == true || CManager::GetInputJoypad()->GetPress(CInputJoypad::JOYKEY::DOWN) == true)
		{
			fMoveZ = -1.0f;
		}

		// �E�����ɓ��͂����Ȃ�E�����ւ̃x�N�g��
		if (CManager::GetInputKeyboard()->GetPress(DIK_D) == true || CManager::GetInputJoypad()->GetPress(CInputJoypad::JOYKEY::RIGHT) == true)
		{
			fMoveX = 1.0f;
		}
		// �������ɓ��͂����Ȃ獶�����ւ̃x�N�g��
		else if (CManager::GetInputKeyboard()->GetPress(DIK_A) == true || CManager::GetInputJoypad()->GetPress(CInputJoypad::JOYKEY::LEFT) == true)
		{
			fMoveX = -1.0f;
		}

		// ��L�̏����Ń{�^������͂��Ă����瓮���Ă���Ƃ݂Ȃ�
		if (fMoveX != 0.0f || fMoveZ != 0.0f)
		{
			bMove = true;

		}
		// ���͂��󂯕t���Ă��Ȃ��̂œ����Ă��Ȃ�
		else
		{
			bMove = false;
		}
	}

	// �ړ���ԂȂ�
	if (bMove == true)
	{
		float fMoveRot = 0.0f; // �ړ�������������߂�

		// �R���g���[���[�̍��X�e�B�b�N����p�x�����߂�
		if (bUseController == true)
		{
			fMoveRot = CManager::GetInputJoypad()->GetLStickAimRot();
		}
		// �䂩��p�x�����߂�iZ���̐��������O�Ȃ̂ŁAZ������u�E�����v��X�����̊p�x�u�������v�����߂�j
		else
		{
			fMoveRot = atan2f(fMoveX, fMoveZ);
		}

		fMoveRot += fAddAimRot; // ���������Z���␳

		// �ǂ̖ʂ����Ɉړ����邩�����߂�
		switch (MoveAim)
		{
		case Calculation::Move::MOVEAIM::XY: // XY��������Ɉړ�����
			Move.x = sinf(fMoveRot) * fSpeed;
			Move.y = cosf(fMoveRot) * fSpeed;
			break;
		case Calculation::Move::MOVEAIM::XZ: // XZ��������Ɉړ�����
			Move.x = sinf(fMoveRot) * fSpeed;
			Move.z = cosf(fMoveRot) * fSpeed;
			break;
		case Calculation::Move::MOVEAIM::ZY: // ZY��������Ɉړ�����
			Move.z = sinf(fMoveRot) * fSpeed;
			Move.y = cosf(fMoveRot) * fSpeed;
			break;
		default:
			break;
		}

		// 1f��̈ʒu���v�Z
		PosFuture = Pos + Move;

		// 1f��̈ʒu�ƌ��݂̈ʒu�̃x�N�g����������ׂ��p�x�����߂�
		fRot = atan2f(PosFuture.x - Pos.x, PosFuture.z - Pos.z);
	}

	return bMove;
}

//===================================================================================================================
// XZ�����ŖړI�n�փz�[�~���O�����鏈��
//===================================================================================================================
D3DXVECTOR3 Calculation::Move::HormingToGoalPosXZ(float& fNowRot, const D3DXVECTOR3& SelfPos, const D3DXVECTOR3& GoalPos, float CorrectionRot, float fSpeed)
{
	// === �����Ɏg�p�������錾�A������ ===

	float 
		fRotDest = 0.0f, // �ړI�̈ʒu�ւ̊p�x
	    fRotDiff = 0.0f, // �p�x����
	    fVecX = 0.0f,  // X�x�N�g��
	    fVecZ = 0.0f;  // Z�x�N�g��

	D3DXVECTOR3 Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f); // �ړ���

	// === �ړI�n�ւ̃z�[�~���O�����J�n ===

	fVecX = GoalPos.x - SelfPos.x;
	fVecZ = GoalPos.z - SelfPos.z;
	fRotDest = atan2f(fVecX, fVecZ);

	// ���݂̈ړ������̊p�x�ƁA�ړI�̈ړ������̊p�x���������߂�
	fRotDiff = fRotDest - fNowRot;

	// �����̍������~����ɕ��
	fRotDiff = Calculation::Rot::WrapAngle(fRotDiff);

	fNowRot += fRotDiff * CorrectionRot; // ��������`��Ԃ���

	// ���݂̌������~����ɕ��
	fNowRot = Calculation::Rot::WrapAngle(fNowRot);

	// �ړ��ʂ��X�V����
	Move.x = sinf(fNowRot) * fSpeed;
	Move.z = cosf(fNowRot) * fSpeed;

	return Move;
}

//===================================================================================================================
// �ړI�̍����֓��B���邽�߂̏����x�����߂�iv0 = ��2gh)
//===================================================================================================================
float Calculation::Move::GetInitialVelocityHeight(float fHeight, float fGravity)
{
	float fAim = sqrtf(2 * fGravity * fHeight);
	return fAim;
}

//*******************************************************************************************************************
// �������O���
//*******************************************************************************************************************

//===================================================================================================================
// �x�N�g����Yaw��Pitch�ɕϊ����鏈��
//===================================================================================================================
D3DXVECTOR2 Calculation::Rot::VectorToYawPitch(const D3DXVECTOR3& SelfPos, const D3DXVECTOR3& GoalPos)
{
	// === �����Ɏg�p�������錾�A������ ===

	D3DXVECTOR2 YawPitch = { 0.0f,0.0f }; // Yaw��Pitch���v�Z����

   // === �ϊ������J�n ===

	// Yaw
	YawPitch.y = atan2f(GoalPos.x - SelfPos.x, GoalPos.z - SelfPos.z); 

	// Pitch
	YawPitch.x = atan2f(GoalPos.y - SelfPos.y, sqrtf(powf(GoalPos.x - SelfPos.x, 2) + powf(GoalPos.z - SelfPos.z, 2))); 

	return YawPitch;
}

//===================================================================================================================
// ���݂̌�����ړI�̌����ɐ��`��Ԃ��鏈��
//===================================================================================================================
float Calculation::Rot::Lerp(float fSelfRot, float fRotAim, float fDecayRot)
{
	float fRotDiff = 0.0f;                                 //�����̍���
	float fCameraRot = CManager::GetCamera()->GetRot().y;  //�J������Yaw���擾
	fRotDiff = fRotAim - fSelfRot;

	// �����̍������~����ɕ�ݍ���
	fRotDiff = Calculation::Rot::WrapAngle(fRotDiff);

	//���X�ɖړI�̌����֍��킹�Ă���
	fSelfRot += fRotDiff * fDecayRot;

	// �������~����ɕ�ݍ���
	fSelfRot = Calculation::Rot::WrapAngle(fSelfRot);

	return fSelfRot;
}

//===================================================================================================================
// �������~����ɕ�ݍ��ޏ���
//===================================================================================================================
float Calculation::Rot::WrapAngle(float fRot)
{
	// ������D3DX_PI�𒴂��Ă�����D3DX_PI��艺�ɂȂ�܂Ō��炷
	while (fRot > D3DX_PI)
	{
		fRot -= D3DX_PI * 2;
	}

	// ������-D3DX_PI��������Ă�����-D3DX_PI����ɂȂ�܂ő��₷
	while (fRot < -D3DX_PI)
	{
		fRot += D3DX_PI * 2;
	}
		
	return fRot;
}

//===================================================================================================================
// �ړI�n�܂ł̕������p�x�����߂�
//===================================================================================================================
float Calculation::Rot::ParabolaToGoalPos(float fLength, float fGravity, float fSpeed, D3DXVECTOR3 SelfPos, D3DXVECTOR3 GoalPos)
{
	// === �����Ɏg�p����ϐ���錾�A������ ===

	float fRot = 0.0f; // �p�x
	float fXG = fLength * fGravity; // �����~�d��
	float fV = fSpeed; // ���x
	float fV2 = powf(fV, 2); // ���x�Q��

	fRot = asinf(fXG / fV2);

	// 0���Z���
	if (fRot > 0.00001f)
	{
		fRot /= 2;
	}

	// �p�x����Βl�Ȃ̂ŁA�ړI�̈ʒu�������̈ʒu���}�C�i�X�����ɂ���ꍇ�A���̒l�ɂ���
	if (GoalPos.x < SelfPos.x)
	{
		fRot *= -1;
	}

	return fRot;
}

//*******************************************************************************************************************
// �������O���
//*******************************************************************************************************************

//===================================================================================================================
// �ړI�n�܂ł̋��������߂鏈��
//===================================================================================================================
float Calculation::Length::ToGoalPos(D3DXVECTOR3 SelfPos, D3DXVECTOR3 GoalPos)
{
	float fLength = 0.0f;//�����i�[�p
	fLength = sqrtf(powf(GoalPos.x - SelfPos.x, 2) + powf(GoalPos.y - SelfPos.y, 2) + powf(GoalPos.z - SelfPos.z, 2));//�ړI�̈ʒu�Ƃ̋��������߂�
	return fLength;
}

//*******************************************************************************************************************
// �ϊ����O���
//*******************************************************************************************************************

//===================================================================================================================
// �X�N���[�����W�����[���h���W�ɕϊ�
//===================================================================================================================
D3DXVECTOR3* Calculation::Conversion::ScreenToWorld(D3DXVECTOR3* pout, float Sx, float Sy, float fZ, int Screen_w, int Screen_h, D3DXMATRIX* View, D3DXMATRIX* Prj)
{
	// �e�s��̋t�s����Z�o�i�r���[�A�v���W�F�N�V�����}�g���b�N�X�̋t�s���������̂́A�J�����̈ʒu��2D��UI���o�Ă���ƒ�`�ł��邩��)
	// �t�s��Ƃ́A�l�ɒl-1�������A�|�����킳���O�ɖ߂�����
	D3DXMATRIX InvView, InvPrj, VP, InvViewport; // �v�Z�p�}�g���b�N�X

	//�r���[�}�g���b�N�X�Ƃ̋t����������ă��[���h���W�����߂�i�r���[�}�g���b�N�X�̌�����ʒu�ɉ����ĕϊ�����Ă���̂Łj
	D3DXMatrixInverse(&InvView, NULL, View);

	// �v���W�F�N�V�����}�g���b�N�X�Ƃ̋t�s��i�������i���s���e�A����p�Ȃǁj��ς��Ă���̂ŁA�t�s����|�����킹�A���Ƃɖ߂��K�v������j
	D3DXMatrixInverse(&InvPrj, NULL, Prj);  
	D3DXMatrixIdentity(&VP);

	// �X�P�[�����O�̒l��ς��Ă���B�X�N���[�����W�̒��S����ʒ����ɂ���iNDC���W���g�p���ĕϊ�����̂ŁA�X�N���[�����W�͂O�`�P�Q�W�O�A�Ȃ��A
	//NDC���W�́[�P�`�P������A�ϊ�����ۂɍ��킹��K�v������(�P�Q�W�O�@���@�U�S�O�j�A�i�O�@���@�[�U�S�O)
	VP._11 = Screen_w / 2.0f; VP._22 = -Screen_h / 2.0f; // �X�P�[�����O�����iX���̂܂܁AY�͏�𐳂Ɂj
	VP._41 = Screen_w / 2.0f; VP._42 = Screen_h / 2.0f;  // ���s�ړ������i���S�����_�ɍ��킹��)
	D3DXMatrixInverse(&InvViewport, NULL, &VP); // NDC���W�ɍ��킹��

	//����
	D3DXVECTOR3 MyPos = D3DXVECTOR3(Sx, Sy, fZ);//�X�N���[�����W�Ɛ[�x�l

	// �t�ϊ�
	D3DXMATRIX tmp = InvViewport * InvPrj * InvView;// ���[���h���W�����߂�iInvViewport�u�X�N���[�����W��NDC���W�v�AInvPrj�uNDC���W���r���[��ԁv�AInvView�u�r���[��ԁ����[���h���W�v)
													//���̍s��ɐ[�x�l�ƃX�N���[�����W�����킹�邱�ƂŁA�[�x�l�ɍ��킹�����W�ϊ����\�ɂȂ�)

	D3DXVec3TransformCoord(pout, &MyPos, &tmp); // ���W�ϊ���̈ʒu�����߂�
	return pout;
}

//===================================================================================================================
// ���[���h���W���X�N���[�����W�ɕϊ�
//===================================================================================================================
D3DXVECTOR3 Calculation::Conversion::WorldToScreen(D3DXVECTOR3 worldPos, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, float screenWidth, float screenHeight)
{
	// === �����Ɏg�p�������錾�A������ ===
	
	//���[���h���W���N���b�v���W�ɕϊ�
	D3DXVECTOR4 ClipSpacePos;
	D3DXMATRIX mtxTrans;

	// === ���[���h���W���X�N���[�����W�ɕϊ����鏈�����J�n ===

	mtxTrans = viewMatrix * projectionMatrix; // �r���[�A�v���W�F�N�V�����ϊ�
	D3DXVec3Transform(&ClipSpacePos, &worldPos, &mtxTrans); // ��L�̕ϊ��������̃��[���h���W�̈ʒu�����߂�

	//�������Z�i�N���b�v���W����NDC��Ԃցj�i�{����GPU������ɂ�邪�A�f�[�^�Ƃ��Ă͏o�Ȃ��̂Ŏ����ŋ��߂�)
	if (ClipSpacePos.w != 0.0f)
	{//X�AY�AZ�́A�J��������̑��ΓI�Ȉʒu��\���Ă���i�ϊ����ꂽ�ʒu)�B
	 //W�́A�J��������̋����i�[�x���j��\���Ă���
		ClipSpacePos.x /= ClipSpacePos.w;
		ClipSpacePos.y /= ClipSpacePos.w;
		ClipSpacePos.z /= ClipSpacePos.w;
	}

	//�X�N���[�����W�֕ϊ�
	D3DXVECTOR3 ScreenPos;
	ScreenPos.x = (ClipSpacePos.x * 0.5f + 0.5f) * screenWidth; // �^�񒆂��O�ɂ��邽�߁{�O�D�T���A�[�P�Ȃ獶�[�A�{�P�Ȃ�E�[�ƂȂ�
	ScreenPos.y = (1.0f - (ClipSpacePos.y * 0.5f + 0.5f)) * screenHeight; // �^�񒆂��O�ɂ��邽�߁{�O�D�T���A�������ƂȂ�A�オ�O�ƂȂ�̂ŁA�P�̎��͂O�ɂȂ�A�[�P�̎��͂P�ƂȂ�
	ScreenPos.z = ClipSpacePos.z; // �[�x�l�i�O�`�P�j�͈̔�

	return ScreenPos;
}

//*******************************************************************************************************************
// �l���O���
//*******************************************************************************************************************

//===================================================================================================================
// �����v�Z����
//===================================================================================================================
int Calculation::Value::CalcDigit(int nValue)
{
	int nDigit = 0; // ����

	// ���l���Ȃ��̂łP���ł�
	if (nValue == 0)
	{
		return 1;
	}

	// �P�O�Ŋ����Ēl���O�ɂȂ�܂ŌJ��Ԃ������𑝂₵�܂�
	while (nValue != 0)
	{
		nValue /= 10;
		nDigit++;
	}

	return nDigit;
}

//===================================================================================================================
// �w�肵�������擾
//===================================================================================================================
int Calculation::Value::GetDigit(int number, int position)
{
	int divisor = static_cast<int>(std::pow(10, position));
	return (number / divisor) % 10;
}

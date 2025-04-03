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
    //==============================
	//�ړ������񋓌^
	//==============================
	typedef enum
	{
		MOVEAIM_XY = 0,//XY�����ɓ���
		MOVEAIM_XZ,    //XZ�����ɓ���
		MOVEAIM_ZY,    //ZY�����ɓ���
		MOVEAIM_MAX
	}MOVEAIM;
	//=============================================================================================================

	//==============================
	//�v���g�^�C�v�錾
	//==============================
	CCalculation();                                                         //�R���X�g���N�^
	~CCalculation();                                                        //�f�X�g���N�^
	static float CalculationLength(D3DXVECTOR3 Pos, D3DXVECTOR3 PurposePos);//2�_�̋������v�Z����
	static float CalculationXYaim(D3DXVECTOR3 Pos, D3DXVECTOR3 PurposePos); //�Q�����̖ړI�����ւ̊p�x�����߂�
	static float CalculationParabola(float fLength, float fGravity, float fSpeed,D3DXVECTOR3 Pos,D3DXVECTOR3 PurposePos);//�����A�d�́A���x�����ƂɂQ�_�̕������ړ����v�Z����
	static float CalculationRandVecXY();                                    //�����_���ȂQ�����x�N�g����Ԃ�
	static float CalculationCollectionRot2D(float fMyRot, float fRotAim, float fDecayRot,bool bCameraOffSet); //�Q���������̌����̕␳���s��
	static bool CaluclationMove(bool bUseStick,D3DXVECTOR3 & Pos,D3DXVECTOR3& Move, float fSpeed, MOVEAIM MoveAim,float & fRot);//�ړ��Ɏg�������̊p�x�����߂�
	static D3DXVECTOR3 Calculation3DVec(D3DXVECTOR3 MyPos, D3DXVECTOR3 AimPos, float fSpeed);                 //�ړI�ւ̈ړ��ʂ����߂�
	static D3DXVECTOR2 VectorToYawPitch(const D3DXVECTOR3& MyPos, const D3DXVECTOR3& AimPos);                 //�ړI�n�ւ�Yaw��Pitch�֕ϊ�����
	static D3DXVECTOR3 Rand3DVec(int nMathSpeed,int nDivisionSpeed);                                          //�����_����3������Ԃ̈ړ��ʂ��v�Z����
	static D3DXCOLOR CalRaibowColor();

	// �X�N���[�����W�����[���h���W�ɕϊ�
	static D3DXVECTOR3* CalcScreenToWorld(D3DXVECTOR3* pout,
		float Sx,  // �X�N���[��X���W
		float Sy,  // �X�N���[��Y���W
		float fZ,  // �ˉe��Ԃł�Z�l�i0�`1�j
		int Screen_w,
		int Screen_h,
		D3DXMATRIX* View,
		D3DXMATRIX* Prj
	);

	//���[���h���W���X�N���[�����W�ɕϊ�
	static D3DXVECTOR3 CalcWorldToScreenNoViewport(D3DXVECTOR3 worldPos, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, float screenWidth, float screenHeight);

	//�������v�Z����
	static int CalculationDigit(int nNum);

	//�w�肵���������擾����
	static int getDigit(int number, int position);

	//�x�N�g�����v�Z����
	static D3DXVECTOR3 CalcVec(D3DXVECTOR3 MyPos, D3DXVECTOR3 AimPos,bool bNormalize);

	//���W�A��������x�N�g���ɕς���
	static D3DXVECTOR3 RadToVec(const D3DXVECTOR3& Rot);//���W�A��������x�N�g���ɕς���

	//�x�N�g���ɑ΂��ē_�����E�ǂ���ɂ��邩�����߂�
	static float DetermineSide3D(const D3DXVECTOR3& origin, const D3DXVECTOR3& direction, const D3DXVECTOR3& up, const D3DXVECTOR3& point);

	//�ړI�̍����ֈړ����邽�߂̏����x�����߂�
	static float GetInitialVelocityHeight(float fHeight,float fGravity);

	//�ړI�̈ʒu�ւ̊p�x���z�[�~���O���ɋ��߂�
	static D3DXVECTOR3 HormingVecRotXZ(float& fRotMove, const D3DXVECTOR3& MyPos, const D3DXVECTOR3& AimPos, float CorrectionRot,float fSpeed);

	//�p�x���W�A���̕␳���s��
	static float CorrectionRot(float fRot);
	//=============================================================================================================
};
#endif

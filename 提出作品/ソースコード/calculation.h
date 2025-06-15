//===================================================================================================================
//
// �Q�O�Q�T�N�U���P�R���F�v�Z�p�N���X���N���X�ł���K�v�Ȃ��̂�namespace�ɕύX[calculation.h]
// Author:ShinaTaiyo
//
//===================================================================================================================

//*******************************************************************************************************************
// �Q�d�C���N���[�h�h�~
//*******************************************************************************************************************
#ifndef _CALCULATION_H_  
#define _CALCULATION_H_

//*******************************************************************************************************************
// �C���N���[�h
//*******************************************************************************************************************
#include "main.h"

//*******************************************************************************************************************
// �v�Z�p���O���
//*******************************************************************************************************************
namespace Calculation
{

	// === ���O��� ===

	// �x�N�g�����O���
	namespace Vec
	{
		// ���W�A��������x�N�g���ɕς���
		D3DXVECTOR3 RadTo(const D3DXVECTOR3& Rot);

		// �x�N�g���ɑ΂��ē_�����E�ǂ���ɂ��邩�����߂�
		float DetermineSide(
			const D3DXVECTOR3& origin,    // �x�N�g�����_ 
			const D3DXVECTOR3& direction, // �x�N�g��
			const D3DXVECTOR3& up,        // ������x�N�g��
			const D3DXVECTOR3& point);    // ���E�ǂ���ɂ��邩�̓_

	}

	// �ړ��ʖ��O��� 
	namespace Move
	{
		// === �񋓌^ ===

        // �ړ�����
		enum class MOVEAIM
		{
			XY = 0, // XY����
			XZ,     // XZ����
			ZY,     // ZY����
			MAX     // �ő�
		};

		// === �֐� ===

		// �O�����x�N�g�������l�����߂�
		D3DXVECTOR3 Rand3D(
			int nMaxSpeed,        // �ő呬�x 
			int nDivisionMaxSpeed // �ő呬�x������l
		);

		// �ړI�n�ւ̈ړ��ʂ����߂�
		D3DXVECTOR3 DirrectionToTarget(
			D3DXVECTOR3 SelfPos, // ���g�̈ʒu 
			D3DXVECTOR3 AimPos,  // �ړI�n
			float fSpeed         // ���x
		);

		// ���͂ɂ��ړ�����
		bool MovementInput(
			bool bUseStick,    // �X�e�B�b�N���g�p���邩�ǂ���
			D3DXVECTOR3& Pos,  // �ʒu
			D3DXVECTOR3& Move, // �ړ���
			float fSpeed,      // ���x
			MOVEAIM MoveAim,   // �ړ�����
			float fAddAimRot,  // �ړI�̌����ɉ��Z����ʁi�v���C���[���t���������Ă���Ƃ��ȂǂɎg����j
			float& fRot        // ���݂̊p�x���Q�Ƃ��ĕύX����p
		);   

		// XZ�����ŖړI�n�փz�[�~���O������
		D3DXVECTOR3 HormingToGoalPosXZ(
			float& fNowRot, // ���݂̌���
			const D3DXVECTOR3& SelfPos,  // ���g�̈ʒu 
			const D3DXVECTOR3& GoalPos,  // �ړI�n
			float CorrectionRot, // �����␳�l
			float fSpeed // ���x
		);

		// �ړI�̍����ֈړ����邽�߂̏����x�����߂�
		float GetInitialVelocityHeight(
			float fHeight,  // ����
			float fGravity  // �d��
		);
	}

	// �������O���
	namespace Rot
	{
		// �ړI�n�ւ̃x�N�g����Yaw��Pitch�֕ϊ�����
		D3DXVECTOR2 VectorToYawPitch(
			const D3DXVECTOR3& SelfPos,  // ���g�̈ʒu 
			const D3DXVECTOR3& GoalPos   // �ړI�n
		);

		// ���݂̌�����ړI�̌����ɐ��`��Ԃ��鏈��
		float Lerp(
			float fSelfRot,   // ���g�̌���
			float fRotAim,    // �ړI�̌���
			float fDecayRot   // �␳�l
		);

		// �������~����(-D3DX_PI�`D3DX_PI)�ɕ��
		float WrapAngle(float fRot);

		// �ړI�n�܂ł̕������p�x�����߂�
		float ParabolaToGoalPos(
			float fLength,  // ����
			float fGravity, // �d��
			float fSpeed,   // ���x
			D3DXVECTOR3 SelfPos, // ���g�̈ʒu
			D3DXVECTOR3 GoalPos  // �ړI�n
		); // �����A�d�́A���x�����ƂɂQ�_�̕������ړ����v�Z����

	}

	// �������O���
	namespace Length
	{
		// �ړI�n�܂ł̋��������߂鏈��
		float ToGoalPos(
			D3DXVECTOR3 SelfPos, // ���g�̈ʒu
			D3DXVECTOR3 GoalPos  // �ړI�n
		);
	}

	// �ϊ�
	namespace Conversion
	{
		// �X�N���[�����W�����[���h���W�ɕϊ�
		D3DXVECTOR3* ScreenToWorld(D3DXVECTOR3* pout,
			float Sx,  // �X�N���[��X���W
			float Sy,  // �X�N���[��Y���W
			float fZ,  // �ˉe��Ԃł�Z�l�i0�`1�j
			int Screen_w, // �X�N���[������
			int Screen_h, // �X�N���[������
			D3DXMATRIX* View, // �r���[�}�g���b�N�X
			D3DXMATRIX* Prj   // �v���W�F�N�V�����}�g���b�N�X
		);

		// ���[���h���W���X�N���[�����W�ɕϊ�
		D3DXVECTOR3 WorldToScreen(
			D3DXVECTOR3 worldPos,        // ���[���h���W
			D3DXMATRIX viewMatrix,       // �r���[�}�g���b�N�X
			D3DXMATRIX projectionMatrix, // �v���W�F�N�V�����}�g���b�N�X
			float screenWidth,   // �X�N���[������
			float screenHeight); // �X�N���[������
	}

	// �l
	namespace Value
	{
		// �������v�Z����
		int CalcDigit(
			int nValue // ���l
		);

		// �w�肵���������擾����
		int GetDigit(int number, int position);
	}
};
#endif

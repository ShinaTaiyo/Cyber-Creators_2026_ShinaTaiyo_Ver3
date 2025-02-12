//==========================================================================
//
//�U���R���F3D�I�u�W�F�N�g��`�悷��[camera.h]
//Author:ShinaTaiyo
//
//==========================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

//==========================================================================
//�C���N���[�h
//==========================================================================
#include "main.h"
//==========================================================================

//==========================================================================
//�J�����N���X
//==========================================================================
class CCamera
{
public:

	//===================================================
	//�J�������[�h�񋓌^
	//===================================================
	typedef enum
	{
		CAMERATYPE_BIRD = 0,  //�o�[�h�r���[
		CAMERATYPE_BESIDE,    //�r�T�C�h�r���[
		CAMERATYPE_SIDE,      //�T�C�h�r���[
		CAMERATYPE_BOSSBATTLE,//�{�X�o�g��
		CAMERATYPE_BOSSDEFEAT,//�{�X��|�����Ƃ��̉��o�p�J����
		CAMERATYPE_TURNINGXZ, //XZ���񃂁[�h
		CAMERATYPE_NONE,      //�J�����^�C�v�F�Ȃ�
		CAMERATYPE_MAX
	}CAMERATYPE;
	//=================================================================================================

	CCamera();            //�R���X�g���N�^
	~CCamera();           //�f�X�g���N�^
	HRESULT Init();       //����������
	void Uninit();        //�I������
	void Update();        //�X�V����
	void SetCamera();     //�J�����̐ݒ�

	void SetPosR(D3DXVECTOR3 PosR) { m_PosR = PosR; }  //�����_��ݒ肷��
	D3DXVECTOR3& GetRot() { return m_Rot; }             //�������擾����
	void SetShake(int nShakeFrame, float fShakePower); //�J�����̗h���ݒ肷��
	CAMERATYPE GetCameraType() { return m_CameraType; }//�J�����^�C�v���擾����
	void SetCameraType(CAMERATYPE type) { m_CameraType = type; m_ModeTime = 0; }//�J�����^�C�v��ݒ肷��
	D3DXVECTOR3& GetPosV() { return m_PosV; }      //���_���擾
	D3DXVECTOR3& GetPosR() { return m_PosR; }      //�����_���擾
	void SetTurningInfo(float fLength, float fRotSpeed, D3DXVECTOR3 PosR,float fPosVY,float fTurningSpeedY,float fAddLength)
	{
		m_fLength = fLength;            //�����_�Ƃ̋���
		m_fTurningRotSpeed = fRotSpeed; //��]���x
		m_PosR = PosR;                  //�����_
		m_PosV.y = fPosVY;              //�x�_��Y�ʒu
		m_fTurningSpeedY = fTurningSpeedY;//���񒆂�Y�����̈ړ���
		m_fAddLength = fAddLength;      //���Z����
	}
	void SetLength(float fLength) { m_fLength = fLength; }
	float& GetLength() { return m_fLength; }

	void SetTurningRotSpeed(float fTurningRotSpeed) { m_fTurningRotSpeed = fTurningRotSpeed; }
	float& GetTurningRotSpeed() { return m_fTurningRotSpeed; }

	void SetTurningSpeedY(float fTurningSpeedY) { m_fTurningSpeedY = fTurningSpeedY; }
	float& GetTurningSpeedY() { return m_fTurningSpeedY; }

	void SetAddLength(float fAddLength) { m_fAddLength = fAddLength; }
	float& GetAddLength() { return m_fAddLength; }
private:
	static const float m_BESIDECAMERALENGTH;      //�r�T�C�h���[�h�̃J�����̋���
	D3DXVECTOR3 m_PosV;                           //���_
	D3DXVECTOR3 m_PosR;                           //�����_
	D3DXVECTOR3 m_VecU;                           //������x�N�g��
	D3DXVECTOR3 m_Rot;                            //����
	D3DXMATRIX m_mtxProjection;                   //�v���W�F�N�V�����s��
	D3DXMATRIX m_mtxView;                         //�r���[�}�g���b�N�X
	CAMERATYPE m_CameraType;                      //�J�������[�h�̎��
	D3DXVECTOR3 m_DifferenceLength;               //����
	D3DXVECTOR3 m_ZoomSpeed;                      //�Y�[�����鑬��
	
	D3DXVECTOR3 m_SupportPos;                     //�Q�l�ʒu
	float m_fLength;                              //����
	float m_fAddLength;                           //�ǉ�����
	float m_fTurningRotSpeed;                     //���񑬓x
	float m_fTurningSpeedY;                       //���񒆂�Y�����̈ړ���

	int m_nShakeFrame;                            //�J������h�炷�t���[����
	int m_ModeTime;                               //�J�������[�h�̎���
	float m_fShakePower;                          //�J������h�炷��

	void BossDefeatCameraProcess();               //�{�X��|�����Ƃ��̃J�����̏������s��
	void TurningCameraProcess();                  //����̃J�����̏������s��
	void NormalCameraSetPosR();                   //���ʂ̃J�����̒����_��ݒ肵������
};
#endif

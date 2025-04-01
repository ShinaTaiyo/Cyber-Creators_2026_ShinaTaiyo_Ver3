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
//�O���錾
//==========================================================================
class CCameraState;
class CCameraLengthState;
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
		CAMERATYPE_NONE = 0,  //�Ȃ�
		CAMERATYPE_BIRD,      //�o�[�h
		CAMERATYPE_BESIDE,    //�r�T�C�h
		CAMERATYPE_SIDE,      //�T�C�h
		CAMERATYPE_MAX
	}CAMERATYPE;
	//=================================================================================================

	//===================================================
	//�J������ԗ񋓌^
	//===================================================
	enum class CAMERASTATE
	{
		NORMAL = 0,
		TURNAROUND,
		MAX
	};

	CCamera();                                                                            //�R���X�g���N�^
	~CCamera();                                                                           //�f�X�g���N�^
	HRESULT Init();                                                                       //����������
	void Uninit();                                                                        //�I������
	void Update();                                                                        //�X�V����
	void SetCamera();                                                                     //�J�����̐ݒ�
													                                      
	void SetPosR(D3DXVECTOR3 PosR) { m_PosR = PosR; }                                     //�����_��ݒ肷��
	void SetRot(const D3DXVECTOR3 Rot) { m_Rot = Rot; }                                   //������ݒ�
	const D3DXVECTOR3& GetRot() const{ return m_Rot; }                                    //�������擾����
	void SetShake(int nShakeFrame, float fShakePower);                                    //�J�����̗h���ݒ肷��
	CAMERATYPE GetCameraType() { return m_CameraType; }                                   //�J�����^�C�v���擾����
	void SetCameraType(CAMERATYPE type) { m_CameraType = type; m_ModeTime = 0; }          //�J�����^�C�v��ݒ肷��
	void SetPosV(D3DXVECTOR3 PosV) { m_PosV = PosV; }                                     //���_��ݒ�
	D3DXVECTOR3& GetPosV() { return m_PosV; }                                             //���_���擾
	void SetAddPosV(D3DXVECTOR3 AddPosV) { m_AddPosV = AddPosV; }                         //���_�����Z����
	const D3DXVECTOR3& GetAddPosV() const { return m_AddPosV; }                           //���_�̉��Z�ʂ��擾����
	D3DXVECTOR3& GetPosR() { return m_PosR; }                                             //�����_���擾
	const D3DXVECTOR3& GetAddPosR() const { return m_AddPosR; }                           //�����_�̉��Z�ʂ��擾����
	void SetAddPosR(D3DXVECTOR3 AddPosR) { m_AddPosR = AddPosR; }                         //�����_�̉��Z�ʂ�ݒ肷��
	void SetLength(float fLength) { m_fLength = fLength; }                                //�����_�Ǝ��_�̋�����ݒ肷��
	float& GetLength() { return m_fLength; }                                              //�����_�Ǝ��_�̋������擾����
	void SetAddLength(float fAddLength) { m_fAddLength = fAddLength; }                    //�����̉��Z�ʂ�ݒ肷��
	float& GetAddLength() { return m_fAddLength; }                                        //�����̉��Z�ʂ��擾����
																				          
	void ChengeState(CCameraState * pCameraState);                                        //�J�����̏�Ԃ�ς���
	void ChengeLengthState(CCameraLengthState* pCameraLengthState);                       //�J�����̋����̏�Ԃ�ς���
																				          
	const float& GetPosRToPosVLength() const { return m_fLength; }                        //���~�_�Ǝ��_�̋������擾����
																				          
	//======================================									          
	//�ÓI�����o�h														             
	//======================================									          
	static const float & GetInitialLength() { return s_fINITIAL_LENGTH; }                 //�ŏ��̋������擾����
	static const bool& GetUseCameraControllMouse() { return s_bCAMERACONTROLLMOUSE; }     //�f�o�b�O�ɃJ�������}�E�X�ő��삷�邩�ǂ������擾����
	static const int& GetSensitivityLevel() { return s_nSENSITIVITYLEVEL; }               //���x���x�����擾����
	static const int& GetMaxSensitivityLevel() { return s_nMAX_SENSITIVITYLEVEL; }        //�ő労�x���x�����擾����
	static void SetSensitivityLevel(int nLevel) {s_nSENSITIVITYLEVEL = nLevel;}           //���x���x����ݒ肷��                   
	//==========================================================================================================================================

	//======================================
	//�}�g���b�N�X�n
	//======================================
	D3DXMATRIX* GetMtxView() { return &m_mtxView; }                                       //�r���[�}�g���b�N�X���擾
	D3DXMATRIX* GetMtxProjection() { return &m_mtxProjection; }                           //�v���W�F�N�V�����}�g���b�N�X���擾
	//==========================================================================================================================================
private:
	//======================================
    //�ÓI�����o�錾
    //======================================
	static const float m_BESIDECAMERALENGTH;                                              //�r�T�C�h���[�h�̃J�����̋���
	static const float s_fINITIAL_LENGTH;                                                 //�ŏ��̋���
	static constexpr int s_nMAX_SENSITIVITYLEVEL = 20;                                    //�ő労�x���x��
	static int s_nSENSITIVITYLEVEL;                                                       //�J�����̊��x���x��
	static bool s_bCAMERACONTROLLMOUSE;                                                   //�J�������}�E�X�ő��삷�邩�ǂ����iDebug�ł͎g��Ȃ���������Ȃ����ARelease�ł͕K���g��)
	static constexpr bool s_bDEBUGCAMERACONTROLLMOUSE = false;                            //�f�o�b�O���ɂ��J�������}�E�X�ő��삷�邩�ǂ����i�J�������g�p����ƃJ�[�\�����Œ肳��Ă��܂��̂ŁA�g�p���邩�����߂�)
	//==========================================================================================================================================

	//======================================
	//�ϐ��錾
	//======================================
	D3DXVECTOR3 m_PosV;                           //���_!
	D3DXVECTOR3 m_PosR;                           //�����_!
	D3DXVECTOR3 m_AddPosR;                        //���Z�����_
	D3DXVECTOR3 m_AddPosV;                        //���Z���_
	D3DXVECTOR3 m_VecU;                           //������x�N�g��!
	D3DXVECTOR3 m_Rot;                            //����!
	D3DXMATRIX m_mtxProjection;                   //�v���W�F�N�V�����s��!!
	D3DXMATRIX m_mtxView;                         //�r���[�}�g���b�N�X!
	CAMERATYPE m_CameraType;                      //�J�������[�h�̎��!
	D3DXVECTOR3 m_DifferenceLength;               //����!
	D3DXVECTOR3 m_ZoomSpeed;                      //�Y�[�����鑬��!
	D3DXVECTOR3 m_SupportPos;                     //�Q�l�ʒu!

	CAMERASTATE m_State;                          //��Ԗ�
	CCameraState* m_pCameraState;                 //�J�����̏�ԃN���X
	CCameraLengthState* m_pCameraLengthState;     //�J�����̋����̏�ԃN���X

	float m_fLength;                              //����!
	float m_fAddLength;                           //�ǉ�����!
	float m_fTurningRotSpeed;                     //���񑬓x!
	float m_fTurningSpeedY;                       //���񒆂�Y�����̈ړ���!

	int m_nShakeFrame;                            //�J������h�炷�t���[����!
	int m_ModeTime;                               //�J�������[�h�̎���!
	float m_fShakePower;                          //�J������h�炷��!
	//==========================================================================================================================================

	//======================================
	//�v���g�^�C�v�錾
	//======================================
	void NormalCameraMove();                      //���ʂ̃J�����̒����_��ݒ肵������
	void MakeTransparent();                       //�������߂��I�u�W�F�N�g�𓧖��ɂ��鏈��
	//==========================================================================================================================================
};

//�J�����̏�ԃX�[�p�[�N���X
class CCameraState
{
public:
	CCameraState() {};                          //�R���X�g���N�^
	virtual ~CCameraState() {};                 //�f�X�g���N�^
	virtual void Process(CCamera* pCamera) {};  //����
};

//���ʂ̏��
class CCameraState_Normal : public CCameraState
{
public:
	CCameraState_Normal();         //�R���X�g���N�^
	~CCameraState_Normal();        //�f�X�g���N�^
	void Process(CCamera* pCamera);//����
private:
	static const float s_fNORMAL_AROUNDROTSPEED;  //�J�����̉�]���x
	static constexpr float s_fMAX_STICKSENSITIVITY = 0.08f;//�X�e�B�b�N�̍ő労�x
	static constexpr float s_fMAX_MOUSESENSITIVITY = 0.01f;//�}�E�X�ő労�x
};

//�_������������������
class CCameraState_TurnAround : public CCameraState
{
public:
	CCameraState_TurnAround(D3DXVECTOR3 AimRot,float fAdjustTurnSpeed);//�R���X�g���N�^
	~CCameraState_TurnAround() override;                               //�f�X�g���N�^
	void Process(CCamera * pCamera) override;                          //����
private:

	const D3DXVECTOR3 m_AimRot;     //�ړI�̌���
	const float m_fAdjustTurnSpeed; //������ς��鑬�x�̕␳�x
};

//������ς�����
class CCameraLengthState
{
public:
	CCameraLengthState();                      //�R���X�g���N�^
	virtual ~CCameraLengthState();             //�f�X�g���N�^
	virtual void Process(CCamera * pCamera);   //����
};

//���X�ɋ�����ړI�̋����𑝂₵�Ė߂�
class CCameraLengthState_Gradually : public CCameraLengthState
{
public:
	CCameraLengthState_Gradually(float fLength,float fAdjustAddSpeed,int nChengeFrame); //�R���X�g���N�^
	~CCameraLengthState_Gradually();                                                    //�f�X�g���N�^
	void Process(CCamera* pCamera) override;                                            //����
private:
	//*�ϐ�
	float m_fAimLength;                         //����
	float m_fAdjustAddSpeed;                    //�����𑝂₷���x
	int m_nChengeLengthFrame;                   //�J�����̋����𑝂₵�Ă����Ԃɂ���t���[����
	bool m_bNowAddLength;                       //�J�����̋����𑝂₵�Ă��邩�ǂ���
	bool m_bStartReturnLength;                  //�J�����̋�����߂��������J�n���邩�ǂ���
	int m_nChengeFrameCnt;                      //�J�����̋����𑝂₵���܂܂ɂ���t���[�������J�E���g����

	//*�֐�
	void AddLengthProcess(CCamera * pCamera);   //�����𑝂₷����
	void MaintainLengthFrameCntProcess();       //�������ێ�����t���[�������J�E���g����
	void ReturnLengthProcess(CCamera* pCamera); //���������ɖ߂�����
};
#endif

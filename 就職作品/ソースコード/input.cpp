//=========================================================
//
//�T���Q�Q���FDirectX�N���X���A�L�[���̓N���X���A����
//Author:ShinaTaiyo
//
//=========================================================

//=========================
//�C���N���[�h
//=========================
#include "input.h"
#include "main.h"
#include "debugtext.h"
#include "manager.h"
//=========================================================

//=========================
//�ÓI�����o������
//=========================
LPDIRECTINPUT8 CInput::m_pInput = nullptr;

//=========================
//�R���X�g���N�^
//=========================
CInput::CInput() : m_pDevice()
{

}
//=========================================================

//=========================
//�f�X�g���N�^
//=========================
CInput::~CInput()
{
	
}

//=========================
//����������
//=========================
HRESULT CInput::Init(HINSTANCE hInstance, HWND hWnd)
{
	
	//====================================
	//DirectInput�I�u�W�F�N�g�̐���
	//====================================
	if (m_pInput == nullptr)
	{
		if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION,
			IID_IDirectInput8, (void**)&m_pInput, NULL)))
		{
			return E_FAIL;
		}
	}
	//=================================================================
	return S_OK;
}
//===============================================================

//=========================
//�I������
//=========================
void CInput::Uninit()
{
	//=====================================
	//DirectInput�I�u�W�F�N�g�̔j��
	//=====================================
	if (m_pInput != nullptr)
	{
		m_pInput->Release();
		m_pInput = nullptr;
	}
	//======================================================
}

//=====================================
//�R���X�g���N�^�i�L�[�{�[�h�N���X�j
//=====================================
CInputKeyboard::CInputKeyboard() : m_aKeyState(),m_aKeyStateTrigger(),m_nRepeatCnt(0)
{
	memset(&m_aKeyState[0], NULL, sizeof(m_aKeyState));
	memset(&m_aKeyStateTrigger[0], NULL, sizeof(m_aKeyStateTrigger));
}
//===============================================================

//=====================================
//�f�X�g���N�^�i�L�[�{�[�h�N���X�j
//=====================================
CInputKeyboard::~CInputKeyboard()
{

}
//===============================================================

//=====================================
//�����������i�L�[�{�[�h�N���X�j
//=====================================
HRESULT CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	m_nRepeatCnt = 0;//���s�[�g���͂̃J�E���g������
	if (CInput::Init(hInstance, hWnd))
	{
		return E_FAIL;
	}

	//===================================
	//���̓f�o�C�X�̐���
	//===================================
	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard,
		&m_pDevice, NULL)))
	{
		return E_FAIL;
	}
	//========================================================

	//===================================
//�f�[�^�t�H�[�}�b�g��ݒ�
//===================================
if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
{
	return E_FAIL;
}
//========================================================

//===================================
//�������[�h��ݒ�
//===================================
if (FAILED(m_pDevice->SetCooperativeLevel(hWnd,
	(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
{
	return E_FAIL;
}
//========================================================

//===================================
//�L�[�{�[�h�ւ̃A�N�Z�X�����l��
//===================================
m_pDevice->Acquire();
//========================================================

return S_OK;
}
//===============================================================

//=====================================
//�I�������i�L�[�{�[�h�N���X�j
//=====================================
void CInputKeyboard::Uninit()
{
	//==========================================
	//���̓f�o�C�X�i�L�[�{�[�h�j�̔j��
	//==========================================
	if (m_pDevice != NULL)
	{
		m_pDevice->Unacquire();//�L�[�{�[�h�ւ̃A�N�Z�X�����
		m_pDevice->Release();
		m_pDevice = NULL;
	}
	//========================================================================

	CInput::Uninit();
}
//===============================================================

//=====================================
//�X�V�����i�L�[�{�[�h�N���X�j
//=====================================
void CInputKeyboard::Update()
{
	BYTE aKeyState[MAX_KEY];//�L�[�{�[�h�̓��͏��
	int nCntKey;

	//���̓f�o�C�X����f�[�^���擾
	if (SUCCEEDED(m_pDevice->GetDeviceState
	(sizeof(aKeyState), &aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < MAX_KEY; nCntKey++)
		{
			m_aKeyStateTrigger[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];//�L�[�{�[�h�̃g���K�[����ۑ�
			m_aKeyState[nCntKey] = aKeyState[nCntKey];//�L�[�{�[�h�̃v���X����ۑ�
		}
	}
	else
	{
		m_pDevice->Acquire();//�L�[�{�[�h�ւ̃A�N�Z�X�����l��
	}
	//=====================================================================================================================================================

}
//===============================================================

//=====================================
//�v���X���i�L�[�{�[�h�N���X�j
//=====================================
bool CInputKeyboard::GetPress(int nKey)
{
	return ((m_aKeyState[nKey] & 0x80) != 0) ? true : false;
}
//===============================================================

//=====================================
//�g���K�[���i�L�[�{�[�h�N���X�j
//=====================================
bool CInputKeyboard::GetTrigger(int nKey)
{
	return ((m_aKeyStateTrigger[nKey] & 0x80) != 0) ? true : false;
}
//===============================================================

//=====================================
//���s�[�g���i�L�[�{�[�h�N���X�j
//=====================================
bool CInputKeyboard::GetRepeat(int nKey)
{
	if (GetPress(nKey) == true)
	{
		if (GetTrigger(nKey) == true)
		{
			m_nRepeatCnt = 0;
			return true;
		}

		m_nRepeatCnt++;

		if (m_nRepeatCnt >= m_nMAX_REPEATCNT)
		{
			m_nRepeatCnt = 0;
			return true;
		}
	}
	return false;
}
//===============================================================

//*************************************
//�W���C�p�b�h�N���X
//*************************************

//=====================================
//�ÓI�����o�錾
//=====================================
const BYTE CInputJoypad::s_bRightTrigger_DeadZone = 10;//�E�g���K�[�{�^���̔����Ȓl�̓��͂𖳎�����

//=====================================
//�R���X�g���N�^�i�W���C�p�b�h�N���X�j
//=====================================
CInputJoypad::CInputJoypad() : m_joykeyStatePress(),m_joykeyStateTrigger(),m_fLSitckAimRot(0.0f), m_fRStickAimRot(0.0f), m_nRTTrigger_RepeatCnt(0),
m_nLTTrigger_RepeatCnt(0)
{
	//=======================
	//�������̃N���A
	//=======================
	memset(&m_joykeyStatePress, 0, sizeof(XINPUT_STATE));
	memset(&m_joykeyStateTrigger, 0, sizeof(XINPUT_STATE));

	//===============================================================

}
//================================================================

//=====================================
//�f�X�g���N�^�i�W���C�p�b�h�N���X�j
//=====================================
CInputJoypad::~CInputJoypad()
{

}
//================================================================

//=====================================
//�����������i�W���C�p�b�h�N���X�j
//=====================================
HRESULT CInputJoypad::Init(HINSTANCE hInstance, HWND hWnd)
{
	//XInput�̃X�e�[�g��ݒ�i�L���ɂ���j
	XInputEnable(true);

	return S_OK;
}
//================================================================

//=====================================
//�I�������i�W���C�p�b�h�N���X�j
//=====================================
void CInputJoypad::Uninit()
{
	//XInput�̃X�e�[�g��ݒ�i�����ɂ���j
	XInputEnable(false);
}
//================================================================

//=====================================
//�X�V�����i�W���C�p�b�h�N���X�j
//=====================================
void CInputJoypad::Update()
{
	XINPUT_STATE joykeyState;//�W���C�p�b�h�̓��͏��
	//GamePad.bRightTrigger

	//�W���C�p�b�h�̏�Ԃ��擾
	if (XInputGetState(0, &joykeyState) == ERROR_SUCCESS)
	{
		//WORD�^��unsigined short(�����Ȃ�)�Ȃ̂ŁA��{�I�ɑ�������l���i0�`65535)�Ȃ̂ŁA�I�[�o�[�t���[��͈͊O�̂��������������邱�Ƃ͂Ȃ��B
		//�G���[�͋N���Ȃ��̂ŗ�O�����͕K�v�Ȃ�
		WORD Button = joykeyState.Gamepad.wButtons;
		WORD OldButton = m_joykeyStatePress.Gamepad.wButtons;
		m_joykeyStateTrigger.Gamepad.wButtons = ~OldButton & Button;//1f�O�̓��͏�Ԃ𔽓]�B���݉�����Ă���{�^����&���Z�i�ǂ�����P�Ȃ�P�j����̂ŁA1f�O�����Ă��Ȃ��ꍇ�A�A�N�e�B�u�ɂȂ�A�g���K�[��������������

		BYTE RTTrigger = joykeyState.Gamepad.bRightTrigger;
		BYTE OldRTrigger = m_joykeyStatePress.Gamepad.bRightTrigger;	
		m_joykeyStateTrigger.Gamepad.bRightTrigger = ~OldRTrigger & RTTrigger;//1f�O�̓��͏�Ԃ𔽓]�B���݉�����Ă���{�^����&���Z�i�ǂ�����P�Ȃ�P�j����̂ŁA1f�O�����Ă��Ȃ��ꍇ�A�A�N�e�B�u�ɂȂ�A�g���K�[��������������

		BYTE LTTrigger = joykeyState.Gamepad.bLeftTrigger;
		BYTE OldLTrigger = m_joykeyStatePress.Gamepad.bLeftTrigger;
		m_joykeyStateTrigger.Gamepad.bLeftTrigger = ~OldLTrigger & LTTrigger;//1f�O�̓��͏�Ԃ𔽓]�B���݉�����Ă���{�^����&���Z�i�ǂ�����P�Ȃ�P�j����̂ŁA1f�O�����Ă��Ȃ��ꍇ�A�A�N�e�B�u�ɂȂ�A�g���K�[��������������
		m_joykeyStatePress = joykeyState;//�W���C�p�b�h�̃v���X��Ԃ�ۑ�
	}
	else
	{//��Ԏ擾���s���̏���
	   ZeroMemory(&m_joykeyStatePress,sizeof(XINPUT_STATE));//�l��������
	   ZeroMemory(&m_joykeyStateTrigger,sizeof(XINPUT_STATE));//�l��������
	}
}
//================================================================

//=====================================
//�v���X���̎擾�i�W���C�p�b�h�N���X�j
//=====================================
bool CInputJoypad::GetPress(JOYKEY key)
{
	return (m_joykeyStatePress.Gamepad.wButtons & (0x01 << static_cast<int>(key))) != 0;
}
//================================================================

//=====================================
//�g���K�[���̎擾�i�W���C�p�b�h�N���X�j
//=====================================
bool CInputJoypad::GetTrigger(JOYKEY key)
{
	return (m_joykeyStateTrigger.Gamepad.wButtons & (0x01 << static_cast<int>(key))) != 0;
}
//================================================================

//=====================================
//L�g���K�[�̃v���X���̎擾�i�W���C�p�b�h�N���X�j
//=====================================
bool CInputJoypad::GetRT_Press()
{
	return m_joykeyStatePress.Gamepad.bRightTrigger > s_bRightTrigger_DeadZone;//���R�ɔ����Ȓl�����͂���邱�Ƃ�����̂ŁA�f�b�h�]�[���𒴂����l�����͂���Ă�����trueni
}
//========================================================================================================================================================

//==================================================================================
//L�X�e�B�b�N�̃v���X���̎擾�i�W���C�p�b�h�N���X�j
//==================================================================================
bool CInputJoypad::GetRT_Repeat(const int nRepeatLoop)
{
	if (GetRT_Press() == true)
	{
		if (GetRT_Trigger() == true)
		{

			m_nRTTrigger_RepeatCnt = 0;
			return true;
		}

		m_nRTTrigger_RepeatCnt++;

		if (m_nRTTrigger_RepeatCnt >= nRepeatLoop)
		{
			m_nRTTrigger_RepeatCnt = 0;
			return true;
		}
	}
	return false;
}

//=====================================
//�g���K�[���̎擾�i�W���C�p�b�h�N���X�j
//=====================================
bool CInputJoypad::GetLT_Trigger()
{
	return  m_joykeyStateTrigger.Gamepad.bLeftTrigger != 0;//0x0004�iJOYKEY_LEFT)�Ȃ�0x01<<2 = 00000111 = 0x0004
}
//========================================================================================================================================================

//=====================================
//�v���X���̎擾�i�W���C�p�b�h�N���X�j
//=====================================
bool CInputJoypad::GetLT_Press()
{
	return m_joykeyStatePress.Gamepad.bLeftTrigger != 0;//0x0004�iJOYKEY_LEFT)�Ȃ�0x01<<2 = 00000111 = 0x0004;
}
//========================================================================================================================================================

//=====================================
//���s�[�g���̎擾�i�W���C�p�b�h�N���X�j
//=====================================
bool CInputJoypad::GetLT_Repeat(const int nRepeatLoop)
{
	if (GetLT_Press() == true)
	{
		if (GetLT_Trigger() == true)
		{
			m_nLTTrigger_RepeatCnt = 0;
			return true;
		}

		m_nLTTrigger_RepeatCnt++;

		if (m_nLTTrigger_RepeatCnt >= nRepeatLoop)
		{
			m_nLTTrigger_RepeatCnt = 0;
			return true;
		}
	}
	return false;
}
//========================================================================================================================================================

//==================================================================================
//L�X�e�B�b�N�̃v���X���̎擾�i�W���C�p�b�h�N���X�j
//==================================================================================
bool CInputJoypad::GetLStickPress(const int nDivisionRot,float fDirectionRot)
{
	float LX = m_joykeyStatePress.Gamepad.sThumbLX;
	float LY = m_joykeyStatePress.Gamepad.sThumbLY;


	bool bActive = false;//�X�e�B�b�N�������Ă��邩�ǂ���
	bool bSuccessDivision = false;//nDivisionRot�Ŋ������l�̎擾�ɐ����������ǂ���

	//determine how far the controller is pushed
	float magnitude = sqrt(LX * LX + LY * LY);

	//��ŋ��߂��p�x�𐳋K������
	float normalizedLX = LX / magnitude;
	float normalizedLY = LY / magnitude;

	//����̃Q�[���́AZ���̐��������O�Ȃ̂ŁAZ��(�E�����j�����X���i�������j�̊p�x�����߂�
	float fAimRot = atan2f(normalizedLX, normalizedLY);
	//======================================
	//��܂��ȕ��������߂�
	//======================================
	float fLapRot = D3DX_PI * 2;//������̒l
	float fDivRot = fLapRot / nDivisionRot;       //�������������̒l

	float fRangeRotA = 0.0f;
	float fRangeRotB = 0.0f;
	for (int nCnt = 0; nCnt < nDivisionRot; nCnt++)
	{//-3.14 �`�@3.14�̊ԂɌŒ肷��A�����������W�A���͈̔͂����߁A���͈̔͂̊Ԃɍ������炻�̒l��Ԃ�
		fRangeRotA = fDivRot * nCnt - D3DX_PI - fDivRot * 0.5f + fDirectionRot;
		fRangeRotB = fDivRot * (nCnt + 1) - D3DX_PI - fDivRot * 0.5f + fDirectionRot;

		if (fAimRot >= fRangeRotA && fAimRot <= fRangeRotB)
		{
			fAimRot = fDivRot * nCnt - D3DX_PI + fDirectionRot;//�Ō�̒l�͕␳�l
			bSuccessDivision = true;
			break;
		}
	}

	if (bSuccessDivision == false)
	{//���͔͈́A�p�x�̕␳
		fAimRot = fDivRot * nDivisionRot - D3DX_PI + fDirectionRot;
	}

	//============================================================================================================

	float normalizedMagnitude = 0.0f;

	//check if the controller is outside a circular dead zone
	if (magnitude > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{//�X�e�B�b�N�������Ă���

		//clip the magnitude at its expected maximum value
		if (magnitude > 32767) magnitude = 32767;

		//adjust magnitude relative to the end of the dead zone
		magnitude -= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;

		//optionally normalize the magnitude with respect to its expected range
		//giving a magnitude value of 0.0 to 1.0
		normalizedMagnitude = magnitude / (32767 - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);

		m_fLSitckAimRot = fAimRot;//���߂��ړI�̊p�x����
		bActive = true;
	}
	else //if the controller is in the deadzone zero out the magnitude
	{//�X�e�B�b�N�������Ă��Ȃ�
		magnitude = 0.0;
		normalizedMagnitude = 0.0;
		bActive = false;
	}
	return bActive;
}
//===============================================================

//=====================================================================================
//R�X�e�B�b�N�̃v���X�����擾�i�W���C�p�b�h�N���X�j
//=====================================================================================
bool CInputJoypad::GetRStickPress(const int nDivisionRot)
{
	float RX = m_joykeyStatePress.Gamepad.sThumbRX;
	float RY = m_joykeyStatePress.Gamepad.sThumbRY;

	bool bActive = false;//�X�e�B�b�N�������Ă��邩�ǂ���
	bool bSuccessDivision = false;//nDivisionRot�Ŋ������l�̎擾�ɐ����������ǂ���

	//determine how far the controller is pushed
	float magnitude = sqrt(RX * RX + RY * RY);

	//��ŋ��߂��p�x�𐳋K������
	float normalizedRX = RX / magnitude;
	float normalizedRY = RY / magnitude;

	//���K�������p�x�ŖړI�̊p�x�����߂�
	float fAimRot = atan2f(normalizedRX, normalizedRY);

	//======================================
	//��܂��ȕ��������߂�
	//======================================
	float fLapRot = D3DX_PI * 2;//������̒l
	float fDivRot = fLapRot / nDivisionRot;       //�������������̒l

	float fRangeRotA = 0.0f;
	float fRangeRotB = 0.0f;
	for (int nCnt = 0; nCnt < nDivisionRot; nCnt++)
	{//�X�e�B�b�N���͕����̒���(�������ĕ������̒l�����͕��������炷)
		fRangeRotA = fDivRot * nCnt - D3DX_PI - fDivRot * 0.5f;
		fRangeRotB = fDivRot * (nCnt + 1) - D3DX_PI - fDivRot * 0.5f;

		if (fAimRot >= fRangeRotA && fAimRot <= fRangeRotB)
		{//���͕��������炵���Ƃ��ɋ��E��������Ĉꕔ�̕������@�\���Ȃ��̂ŁA���̑΍�
			fAimRot = fDivRot * nCnt - D3DX_PI;
			bSuccessDivision = true;
			break;
		}
	}

	if (bSuccessDivision == false)
	{
		fAimRot = fDivRot * nDivisionRot - D3DX_PI;
	}

	//============================================================================================================

	float normalizedMagnitude = 0.0f;

	//check if the controller is outside a circular dead zone
	if (magnitude > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
	{//�X�e�B�b�N�������Ă���

		//clip the magnitude at its expected maximum value
		if (magnitude > 32767) magnitude = 32767;

		//adjust magnitude relative to the end of the dead zone
		magnitude -= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;

		//optionalRY normalize the magnitude with respect to its expected range
		//giving a magnitude value of 0.0 to 1.0
		normalizedMagnitude = magnitude / (32767 - XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);

		m_fRStickAimRot = fAimRot;//���߂��ړI�̊p�x����
		bActive = true;
	}
	else //if the controller is in the deadzone zero out the magnitude
	{//�X�e�B�b�N�������Ă��Ȃ�
		magnitude = 0.0;
		normalizedMagnitude = 0.0;
		bActive = false;
	}

	return bActive;
}
//===============================================================

//=====================================
//L�g���K�[�̃g���K�[���̎擾�i�W���C�p�b�h�N���X�j
//=====================================
bool CInputJoypad::GetRT_Trigger()
{
	return m_joykeyStateTrigger.Gamepad.bRightTrigger > s_bRightTrigger_DeadZone;//���R�ɔ����Ȓl�����͂���邱�Ƃ�����̂ŁA�f�b�h�]�[���𒴂����l�����͂���Ă�����true�ɂ���
}
//================================================================

//****************************************************************************
//�}�E�X�N���X
//****************************************************************************

//======================================
//�R���X�g���N�^
//======================================
CInputMouse::CInputMouse() : m_CursorPosOld(D3DXVECTOR2(0.0f,0.0f)), m_bLeftClickTriggerFlag(false),m_nLeftClickRepeatCnt(0),m_bRightClickTriggerFlag(false),
m_bCursorSenterWarp(false),m_bPrevRightClick(false),m_bPrevLeftClick(false), m_nRightClickRepeatCnt(0),m_bPrevMiddleClick(false),m_nMiddleClickRepeatCnt(0),
m_bMiddleClickTriggerFlag(false)
{

}
//================================================================

//======================================
//�f�X�g���N�^
//======================================
CInputMouse::~CInputMouse()
{

}
//================================================================

//======================================
//����������
//======================================
HRESULT CInputMouse::Init(HINSTANCE hInstance, HWND hWnd)
{
	return S_OK;
}
//================================================================

//======================================
//�I������
//======================================
void CInputMouse::Uninit()
{

}
//================================================================

//======================================
//�X�V����
//======================================
void CInputMouse::Update()
{
	GetStateRightClick();//�E�N���b�N��Ԃ��擾
	GetStateLeftClick(); //���N���b�N��Ԃ��擾
	GetStateMiddleClick();//�~�h���N���b�N��Ԃ��擾
}
//================================================================

//======================================
//�J�[�\�����������p�x���擾
//======================================
bool CInputMouse::GetMouseMoveAngle(float& fYaw, float& fPitch, float fAdjust)
{
	// ��ʂ̒��S���W
	POINT center = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };

	POINT curPos;
	memset(&curPos, 0, sizeof(POINT));//�J�[�\���̈ʒu��������
	GetCursorPos(&curPos); // ���݂̃}�E�X�ʒu���擾

	if (curPos.x == center.x && curPos.y == center.y)
	{//1f�O�Ɠ����ʒu�̏ꍇ�A�}�E�X�𓮂����Ă��Ȃ��Ɖ��肵�Afalse�Ƃ���
		return false;
	}

	// �}�E�X�̈ړ��ʂ��v�Z
	float deltaX = (curPos.y - center.y) * fAdjust;
	float deltaY = (curPos.x - center.x) * fAdjust;

	// �J�����̊p�x�ɔ��f�i�K�X�ύX�j
	fYaw += deltaY;
	fPitch += deltaX;

	// �}�E�X�𒆉��Ƀ��Z�b�g
	SetCursorPos(static_cast<int>(center.x), static_cast<int>(center.y));
	return true;
}
//================================================================

//======================================
//�N���b�N�������ǂ������擾(Press)
//======================================
bool CInputMouse::GetMouseLeftClickPress()
{
	return m_bPrevLeftClick;//���̂܂܂̏�Ԃ�Ԃ�
}
//================================================================

//======================================
//�N���b�N�������ǂ������擾(Trigger)
//======================================
bool CInputMouse::GetMouseLeftClickTrigger()
{
	return m_bLeftClickTriggerFlag;
}
//================================================================

//======================================
//�N���b�N�������ǂ������擾(Repeat)
//======================================
bool CInputMouse::GetMouseLeftClickRepeat(int nRepeat)
{
	if (m_bPrevLeftClick)
	{//���N���b�N���Ă�����
		if (m_nLeftClickRepeatCnt % nRepeat == 0)
		{//���s�[�g�J�E���g�Ə�]�Z���Ĉ�v������
			return true;
		}
	}
	return false;
}
//================================================================

//======================================
//�E�N���b�N�̃v���X�����擾
//======================================
bool CInputMouse::GetMouseRightClickPress()
{
	return m_bPrevLeftClick;
}
//================================================================

//======================================
//�E�N���b�N�̃g���K�[�����擾
//======================================
bool CInputMouse::GetMouseRightClickTrigger()
{
	return m_bRightClickTriggerFlag;
}
//================================================================

//======================================
//�E�N���b�N�̃��s�[�g�����擾
//======================================
bool CInputMouse::GetMouseRightClickRepeat(int nRepeat)
{
	if (m_bPrevRightClick)
	{//�E�N���b�N���Ă�����
		if (m_nRightClickRepeatCnt % nRepeat == 0)
		{//���s�[�g�J�E���g�Ə�]�Z���Ĉ�v������
			return true;
		}
	}
	return false;
}

//======================================
//�~�h���N���b�N�̃v���X�����擾
//======================================
bool CInputMouse::GetMouseMiddleClickPress()
{
	return m_bPrevMiddleClick;
}
//================================================================

//======================================
//�~�h���N���b�N�̃g���K�[�����擾
//======================================
bool CInputMouse::GetMouseMiddleClickTrigger()
{
	return m_bMiddleClickTriggerFlag;
}
//================================================================

//======================================
//�~�h���N���b�N�̃��s�[�g�����擾
//======================================
bool CInputMouse::GetMouseMiddleClickRepeat(int nRepeat)
{
	if (m_bPrevMiddleClick)
	{//�E�N���b�N���Ă�����
		if (m_nMiddleClickRepeatCnt % nRepeat == 0)
		{//���s�[�g�J�E���g�Ə�]�Z���Ĉ�v������
			return true;
		}
	}
	return false;
}
//================================================================

//======================================
//�E�N���b�N�̏�Ԃ��擾����
//======================================
void CInputMouse::GetStateRightClick()
{
    //�L�[��������Ă�����true(0x8000)�A&�Z�́A���݂��̃r�b�g���m���S�ē�����������true��Ԃ�
	bool isRightClick = (GetAsyncKeyState(VK_RBUTTON) & 0x8000);

	// �E�N���b�N���u�����ꂽ�u�ԁv�����o�i�u&&�v�̈Ӗ��F�ǂ����true��������true�A����ȊO��false��Ԃ��j
	m_bRightClickTriggerFlag = (isRightClick && !m_bPrevRightClick);

	//��Ԃ��X�V
	m_bPrevRightClick = isRightClick;

	if (m_bPrevRightClick)
	{//���s�[�g�J�E���g���C���N�������g
		m_nRightClickRepeatCnt++;
	}
}
//================================================================

//======================================
//�~�h���N���b�N�̏�Ԃ��擾����
//======================================
void CInputMouse::GetStateMiddleClick()
{
	//�L�[��������Ă�����true(0x8000)�A&�Z�́A���݂��̃r�b�g���m���S�ē�����������true��Ԃ�
	bool isMiddleClick = (GetAsyncKeyState(VK_MBUTTON) & 0x8000);

	// �~�h���N���b�N���u�����ꂽ�u�ԁv�����o�i�u&&�v�̈Ӗ��F�ǂ����true��������true�A����ȊO��false��Ԃ��j
	m_bMiddleClickTriggerFlag = (isMiddleClick && !m_bPrevMiddleClick);

	//��Ԃ��X�V
	m_bPrevMiddleClick = isMiddleClick;

	if (m_bPrevMiddleClick)
	{//���s�[�g�J�E���g���C���N�������g
		m_nMiddleClickRepeatCnt++;
	}

}
//================================================================

//======================================
//���N���b�N�̏�Ԃ��擾����
//======================================
void CInputMouse::GetStateLeftClick()
{
	//�L�[��������Ă�����true(0x8000)�A&�Z�́A���݂��̃r�b�g���m���S�ē�����������true��Ԃ�
	bool isLeftClick = (GetAsyncKeyState(VK_LBUTTON) & 0x8000);

	// ���N���b�N���u�����ꂽ�u�ԁv�����o�i�u&&�v�̈Ӗ��F�ǂ����true��������true�A����ȊO��false��Ԃ��j
	m_bLeftClickTriggerFlag = (isLeftClick && !m_bPrevLeftClick);

	//��Ԃ��X�V
	m_bPrevLeftClick = isLeftClick;

	if (m_bPrevLeftClick)
	{//���N���b�N���Ă���ԁA���s�[�g�J�E���g���C���N�������g
		m_nLeftClickRepeatCnt++;
	}
}
//================================================================
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
//=========================================================

//=========================
//�ÓI�����o������
//=========================
LPDIRECTINPUT8 CInput::m_pInput = nullptr;

//=========================
//�R���X�g���N�^
//=========================
CInput::CInput()
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
CInputKeyboard::CInputKeyboard()
{
	//memset(&m_aKeyState[0], NULL, sizeof(m_aKeyState));
	//memset(&m_aKeyStateTrigger[0], NULL, sizeof(m_aKeyStateTrigger));
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

//=====================================
//�R���X�g���N�^�i�W���C�p�b�h�N���X�j
//=====================================
CInputJoypad::CInputJoypad()
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
		WORD Button = joykeyState.Gamepad.wButtons;
		WORD OldButton = m_joykeyStatePress.Gamepad.wButtons;
		m_joykeyStateTrigger.Gamepad.wButtons = ~OldButton & Button;//�g���K�[�����i1f�O�Ɠ�����������j�i��΂ɂP�ɂȂ�j

		BYTE RTTrigger = joykeyState.Gamepad.bRightTrigger;
		BYTE OldRTrigger = m_joykeyStatePress.Gamepad.bRightTrigger;
		m_joykeyStateTrigger.Gamepad.bRightTrigger = ~OldRTrigger & RTTrigger;//�g���K�[�����i1f�O�Ɠ�����������j�i��΂ɂP�ɂȂ�j

		m_joykeyStatePress = joykeyState;//�W���C�p�b�h�̃v���X����ۑ�
	}
}
//================================================================

//=====================================
//�v���X���̎擾�i�W���C�p�b�h�N���X�j
//=====================================
bool CInputJoypad::GetPress(JOYKEY key)
{
	return (m_joykeyStatePress.Gamepad.wButtons & (0x01 << key)) != 0;
}
//================================================================

//=====================================
//�g���K�[���̎擾�i�W���C�p�b�h�N���X�j
//=====================================
bool CInputJoypad::GetTrigger(JOYKEY key)
{
	return (m_joykeyStateTrigger.Gamepad.wButtons & (0x01 << key)) != 0;
}
//================================================================

//=====================================
//L�g���K�[�̃v���X���̎擾�i�W���C�p�b�h�N���X�j
//=====================================
bool CInputJoypad::GetRT_Press()
{
	return m_joykeyStatePress.Gamepad.bRightTrigger != 0;//0x0004�iJOYKEY_LEFT)�Ȃ�0x01<<2 = 00000111 = 0x0004;
}
//===============================================================

//=====================================
//L�g���K�[�̃g���K�[���̎擾�i�W���C�p�b�h�N���X�j
//=====================================
bool CInputJoypad::GetRT_Trigger()
{
	return  m_joykeyStateTrigger.Gamepad.bRightTrigger != 0;//0x0004�iJOYKEY_LEFT)�Ȃ�0x01<<2 = 00000111 = 0x0004
}
//================================================================

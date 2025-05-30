//=======================================================================================================================================================================================================================
//
//�L�[���͂Ɋւ��鏈��
//Author:�v�ێs�ĕ�
//
//=======================================================================================================================================================================================================================
#include "input.h"

//=======================================================================================================================================================================================================================
//�O���[�o���ϐ�
//=======================================================================================================================================================================================================================
LPDIRECTINPUT8 g_pInput = NULL;								//DirectInput�I�u�W�F�N�g�ւ̃|�C���^
LPDIRECTINPUTDEVICE8 g_pDevKeyboard = NULL;					//���̓f�o�C�X(�L�[�{�[�h)�ւ̃|�C���^
LPDIRECTINPUTDEVICE8 g_pDevMouse = NULL;					//���̓f�o�C�X(�}�E�X)�ւ̃|�C���^
BYTE g_aKeyState[NUM_KEY_MAX];								//�L�[�{�[�h�̃v���X���
BYTE g_aKeyStateTrigger[NUM_KEY_MAX];						//�L�[�{�[�h�̃g���K�[���
BYTE g_aKeyStateRepeat[NUM_KEY_MAX];						//�L�[�{�[�h�̃��s�[�g���
XINPUT_STATE g_joyKeyState;									//�W���C�p�b�h�̃v���X���
XINPUT_STATE g_joyKeyStateTrigger;							//�W���C�p�b�h�̃g���K�[���
XINPUT_STATE g_joyKeyStateRepeat;							//�W���C�p�b�h�̃��s�[�g���
XINPUT_STATE g_ajoyKeyStateRelease;							//�R���g���[���[�̃����[�X���
XINPUT_STATE g_aJoypadCurrentTime;							//�R���g���[���[�̌��݂̎���
XINPUT_STATE g_aJoypadExecLastTime;							//�R���g���[���[�̍Ō�ɐ^��Ԃ�������
XINPUT_STATE g_JoypadInput;									//�R���g���[���[�̓��͏��
D3DXVECTOR3 g_joyStickPos;									//�X�e�B�b�N�̌X��
bool g_bAngle[STICKTYPE_MAX][STICKANGLE_MAX];				//�X�e�B�b�N�̓��͏��
DWORD g_aStickCurrentTime[STICKTYPE_MAX][STICKANGLE_MAX];	//�X�e�B�b�N�̌��݂̎���
DWORD g_aStickExecLastTime[STICKTYPE_MAX][STICKANGLE_MAX];	//�X�e�B�b�N�̌��݂̎���
STICKINPUT g_stick;											//�X�e�B�b�N�̓��͏��
XINPUT_STATE g_XInput;										//���͏��
XINPUT_VIBRATION g_JoypadMotor;
int g_nCntKey;
DIMOUSESTATE g_MouseState;
DIMOUSESTATE g_MouseStateTrigger;

//=======================================================================================================================================================================================================================
//�L�[�{�[�h�̏���������
//=======================================================================================================================================================================================================================
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	//DirectInput�I�u�W�F�N�g�̐���
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_pInput, NULL)))
	{
		return E_FAIL;
	}
	//���̓f�o�C�X(�L�[�{�[�h)�̐���
	if (FAILED(g_pInput->CreateDevice(GUID_SysKeyboard, &g_pDevKeyboard, NULL)))
	{
		return E_FAIL;
	}
	//�f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(g_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}
	//�������[�h��ݒ�
	if (FAILED(g_pDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}
	//�L�[�{�[�h�ւ̃A�N�Z�X�����l��
	g_pDevKeyboard->Acquire();
	return S_OK;
}

//=======================================================================================================================================================================================================================
//�L�[�{�[�h�̏I������
//=======================================================================================================================================================================================================================
void UninitKeyboard(void)
{
	//���̓f�o�C�X(�L�[�{�[�h)�̔j��
	if (g_pDevKeyboard != NULL)
	{
		g_pDevKeyboard->Unacquire(); //�L�[�{�[�h�ւ̃A�N�Z�X�������
		g_pDevKeyboard->Release();
		g_pDevKeyboard = NULL;
	}

	//DirectInput�I�u�W�F�N�g�̔j��
	if (g_pInput != NULL)
	{
		g_pInput->Release();
		g_pInput = NULL;
	}
}

//=======================================================================================================================================================================================================================
//�L�[�{�[�h�̍X�V����
//=======================================================================================================================================================================================================================
void UpdateKeyboard(void)
{
	BYTE aKeyState[NUM_KEY_MAX]; //�L�[�{�[�h�̓��͏��
	int nCntKey;

	//���̓f�o�C�X����f�[�^���擾
	if (SUCCEEDED(g_pDevKeyboard->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			g_aKeyStateTrigger[nCntKey] = (g_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey]; //�L�[�{�[�h�̃g���K�[����ۑ�
			g_aKeyState[nCntKey] = aKeyState[nCntKey]; //�L�[�{�[�h�̃v���X����ۑ�
		}
	}
	else
	{
		g_pDevKeyboard->Acquire(); //�L�[�{�[�h�ւ̃A�N�Z�X�����l��
	}
}

//=======================================================================================================================================================================================================================
//�L�[�{�[�h�̃v���X�����擾
//=======================================================================================================================================================================================================================
bool GetKeyboardPress(int nKey)
{
	return ((g_aKeyState[nKey] & 0x80) != 0) ? true : false;
}

//=======================================================================================================================================================================================================================
//�L�[�{�[�h�̃g���K�[�����擾
//=======================================================================================================================================================================================================================
bool GetKeyboardTrigger(int nKey)
{
	return ((g_aKeyStateTrigger[nKey] & 0x80) != 0) ? true : false;
}

//=======================================================================================================================================================================================================================
//�L�[�{�[�h�̃��s�[�g�����擾
//=======================================================================================================================================================================================================================
bool GetKeyboardRepeat(int nKey)
{
	//�L�[�������Ă����
	if (GetKeyboardPress(nKey) == true)
	{
		if (GetKeyboardTrigger(nKey) == true)
		{
			g_nCntKey = 0;
			return true;
		}

		g_nCntKey++; //�����Ă���Ԃ��J�E���g

		if (g_nCntKey >= SPEED_KEY) //�����������
		{
			g_nCntKey = 0;
			return true;
		}
	}

	return false;
}

//=======================================================================================================================================================================================================================
// �}�E�X�̏���������
//=======================================================================================================================================================================================================================
HRESULT InitMouse(HINSTANCE hInstance, HWND hWnd)
{
	//DirectInput�I�u�W�F�N�g�̐���
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_pInput, NULL)))
	{
		return E_FAIL;
	}
	//���̓f�o�C�X(�L�[�{�[�h)�̐���
	if (FAILED(g_pInput->CreateDevice(GUID_SysMouse, &g_pDevMouse, NULL)))
	{
		return E_FAIL;
	}
	//�f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(g_pDevMouse->SetDataFormat(&c_dfDIMouse)))
	{
		return E_FAIL;
	}
	//�������[�h��ݒ�
	if (FAILED(g_pDevMouse->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// �f�o�C�X�̐ݒ�
	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = DIPROPAXISMODE_REL;	// ���Βl���[�h�Őݒ�i��Βl��DIPROPAXISMODE_ABS�j

	if (FAILED(g_pDevMouse->SetProperty(DIPROP_AXISMODE, &diprop.diph)))
	{
		// �f�o�C�X�̐ݒ�Ɏ��s
		return E_FAIL;
	}

	//�L�[�{�[�h�ւ̃A�N�Z�X�����l��
	g_pDevMouse->Acquire();
	return S_OK;
}

//=======================================================================================================================================================================================================================
// �}�E�X�̏I������
//=======================================================================================================================================================================================================================
void UninitMouse(void)
{
	//���̓f�o�C�X(�L�[�{�[�h)�̔j��
	if (g_pDevMouse != NULL)
	{
		g_pDevMouse->Unacquire(); //�L�[�{�[�h�ւ̃A�N�Z�X�������
		g_pDevMouse->Release();
		g_pDevMouse = NULL;
	}
	//DirectInput�I�u�W�F�N�g�̔j��
	if (g_pInput != NULL)
	{
		g_pInput->Release();
		g_pInput = NULL;
	}
}

//=======================================================================================================================================================================================================================
// �}�E�X�̍X�V����
//=======================================================================================================================================================================================================================
void UpdateMouse(void)
{
	DIMOUSESTATE MouseState;

	//���̓f�o�C�X����f�[�^���擾
	if (SUCCEEDED(g_pDevMouse->GetDeviceState(sizeof(MouseState), &MouseState)))
	{
		g_MouseState = MouseState; //�}�E�X�̃v���X����ۑ�
	}
	else
	{
		g_pDevMouse->Acquire(); //�}�E�X�ւ̃A�N�Z�X�����l��
	}
}

//=======================================================================================================================================================================================================================
// �}�E�X�̃v���X�����擾
//=======================================================================================================================================================================================================================
bool GetMousePress(int nKey)
{
	return ((g_MouseState.rgbButtons[nKey] & 0x80) != 0) ? true : false;
}

//=======================================================================================================================================================================================================================
// �}�E�X�̃g���K�[�����擾
//=======================================================================================================================================================================================================================
bool GetMouseTrigger(int nKey)
{
	return ((g_MouseStateTrigger.rgbButtons[nKey] & 0x80) != 0) ? true : false;
}

//=======================================================================================================================================================================================================================
// �}�E�X�̃��s�[�g�����擾
//=======================================================================================================================================================================================================================
bool GetMouseRepeat(int nKey)
{
	//�L�[�������Ă����
	if (GetMousePress(nKey) == true)
	{
		if (GetMouseTrigger(nKey) == true)
		{
			g_nCntKey = 0;
			return true;
		}

		g_nCntKey++; //�����Ă���Ԃ��J�E���g

		if (g_nCntKey >= SPEED_KEY) //�����������
		{
			g_nCntKey = 0;
			return true;
		}
	}

	return false;
}

//=======================================================================================================================================================================================================================
// �}�E�X�̏����擾
//=======================================================================================================================================================================================================================
DIMOUSESTATE GetMouse(void)
{
	return g_MouseState;
}

//=======================================================================================================================================================================================================================
//�W���C�p�b�h�̏���������
//=======================================================================================================================================================================================================================
HRESULT InitJoypad(void)
{
	//�������̃N���A
	memset(&g_joyKeyState, 0, sizeof(XINPUT_STATE));

	//�������̃N���A
	memset(&g_JoypadMotor, 0, sizeof(XINPUT_VIBRATION));

	//XInput�̃X�e�[�g��ݒ�(�L���ɂ���)
	XInputEnable(true);

	return S_OK;
}

//=======================================================================================================================================================================================================================
//�W���C�p�b�h�̏I������
//=======================================================================================================================================================================================================================
void UninitJoypad(void)
{
	//XInput�̃X�e�[�g��ݒ�(�����ɂ���)
	XInputEnable(false);
}

//=======================================================================================================================================================================================================================
// �X�e�B�b�N�̓��͂�����p�b�h�t���O�ɕϊ�
//=======================================================================================================================================================================================================================
WORD GetJoypadStick(SHORT sThumbX, SHORT sThumbY, SHORT sDeadZone)
{
	WORD wButtons = 0;

	if (sThumbY >= sDeadZone)
	{
		wButtons |= XINPUT_GAMEPAD_DPAD_UP;
	}
	else if (sThumbY <= -sDeadZone)
	{
		wButtons |= XINPUT_GAMEPAD_DPAD_DOWN;
	}

	if (sThumbX <= -sDeadZone)
	{
		wButtons |= XINPUT_GAMEPAD_DPAD_LEFT;
	}
	else if (sThumbX >= sDeadZone)
	{
		wButtons |= XINPUT_GAMEPAD_DPAD_RIGHT;
	}

	return wButtons;
}

//=======================================================================================================================================================================================================================
//�W���C�p�b�h�̍X�V����
//=======================================================================================================================================================================================================================
void UpdateJoypad(void)
{
	XINPUT_STATE joykeyState; //�W���C�p�b�h�̓��͏��

	//�W���C�p�b�h�̏�Ԃ��擾
	if (XInputGetState(0, &joykeyState) == ERROR_SUCCESS)
	{
		WORD Button = joykeyState.Gamepad.wButtons;
		WORD OldButton = g_joyKeyState.Gamepad.wButtons;
		g_joyKeyStateTrigger.Gamepad.wButtons = Button & ~OldButton;    // �g���K�[����
		g_ajoyKeyStateRelease.Gamepad.wButtons = ~Button & OldButton;    // �����[�X����
		joykeyState.Gamepad.wButtons |= GetJoypadStick(g_joyKeyState.Gamepad.sThumbLX, g_joyKeyState.Gamepad.sThumbLY, JOYPAD_DEADZONE);

		// ���݂̎��Ԃ��擾����
		g_aJoypadCurrentTime.Gamepad.wButtons = (WORD)timeGetTime();

		if (joykeyState.Gamepad.wButtons && ((g_aJoypadCurrentTime.Gamepad.wButtons - g_aJoypadExecLastTime.Gamepad.wButtons) > SPEED_KEY))
		{
			// �Ō�ɐ^��Ԃ������Ԃ�ۑ�
			g_aJoypadExecLastTime.Gamepad.wButtons = g_aJoypadCurrentTime.Gamepad.wButtons;

			// �L�[�{�[�h�̃��s�[�g����ۑ�
			g_joyKeyStateRepeat = joykeyState;
		}
		else
		{
			// �L�[�{�[�h�̃��s�[�g����ۑ�
			g_joyKeyStateRepeat.Gamepad.wButtons = 0;
		}

		g_joyKeyState = joykeyState;                // �v���X����
	}
}

//=======================================================================================================================================================================================================================
//�W���C�p�b�h�̃v���X�����擾
//=======================================================================================================================================================================================================================
bool GetJoypadPress(JOYKEY key)
{
	return (g_joyKeyState.Gamepad.wButtons & (0x01 << key)) ? true : false;
}

//=======================================================================================================================================================================================================================
//�W���C�p�b�h�̃g���K�[�����擾
//=======================================================================================================================================================================================================================
bool GetJoypadTrigger(JOYKEY key)
{
	return (g_joyKeyStateTrigger.Gamepad.wButtons & (0x01 << key)) ? true : false;
}

//=======================================================================================================================================================================================================================
//�W���C�p�b�h�̃��s�[�g�����擾
//=======================================================================================================================================================================================================================
bool GetJoypadRepeat(JOYKEY key)
{
	//�L�[�������Ă����
	if (GetJoypadPress(key) == true)
	{
		if (GetJoypadTrigger(key) == true)
		{
			g_nCntKey = 0;
			return true;
		}

		g_nCntKey++; //�����Ă���Ԃ��J�E���g

		if (g_nCntKey >= SPEED_KEY) //�����������
		{
			g_nCntKey = 0;
			return true;
		}
	}

	return false;
}

//=======================================================================================================================================================================================================================
//�X�e�B�b�N�̍X�V����
//=======================================================================================================================================================================================================================
void UpdateStick(void)
{
	//�X�e�B�b�N��X���AY��
	float fX = 0.0f;
	float fY = 0.0f;

	//�L�[�{�[�h�̏�Ԃ��擾
	if (XInputGetState(0, &g_XInput) == ERROR_SUCCESS)
	{
		for (int nCntStick = 0; nCntStick < STICKTYPE_MAX; nCntStick++)
		{
			switch (nCntStick)
			{
			case STICKTYPE_LEFT:
				fX = (float)(GetXInputState()->Gamepad.sThumbLX);
				fY = (float)(GetXInputState()->Gamepad.sThumbLY);
				break;
			case STICKTYPE_RIGHT:
				fX = (float)(GetXInputState()->Gamepad.sThumbRX);
				fY = (float)(GetXInputState()->Gamepad.sThumbRY);
				break;
			}

			//�p�x���擾
			g_stick.afAngle[nCntStick] = FindAngle(D3DXVECTOR3(fX, fY, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f)) * -1;

			//�X�e�B�b�N�̓|������擾
			g_stick.afTplDiameter[nCntStick] = fabsf(fX);

			if (g_stick.afTplDiameter[nCntStick] < fabsf(fY))
			{
				g_stick.afTplDiameter[nCntStick] = fabsf(fY);
			}

			g_stick.afTplDiameter[nCntStick] /= 32768.0f; //�|���Ă����Ԃ̏����l

			//�������̓t���O��������
			for (int nCntAngle = 0; nCntAngle < STICKANGLE_MAX; nCntAngle++)
			{
				g_bAngle[nCntStick][nCntAngle] = false;
			}

			if (g_stick.afTplDiameter[nCntStick] > 0.1f)
			{
				//�p�x����
				if (g_stick.afAngle[nCntStick] < -D3DX_PI)
				{
					g_stick.afAngle[nCntStick] = D3DX_PI - (D3DX_PI + g_stick.afAngle[nCntStick]) * -1.0f;
				}

				//�p�x���l�����ŏ�Ɉʒu���鎞�A��t���O��^�ɂ���
				if ((g_stick.afAngle[nCntStick] < D3DX_PI * -0.625f) || (g_stick.afAngle[nCntStick] > D3DX_PI * 0.625f))
				{
					g_bAngle[nCntStick][STICKANGLE_UP] = true;
				}

				//�p�x���l�����ŉ��Ɉʒu���鎞�A���t���O��^�ɂ���
				if ((g_stick.afAngle[nCntStick] > D3DX_PI * -0.375f) && (g_stick.afAngle[nCntStick] < D3DX_PI * 0.375f))
				{
					g_bAngle[nCntStick][STICKANGLE_DOWN] = true;
				}

				//�p�x���l�����ŉE�Ɉʒu���鎞�A�E�t���O��^�ɂ���
				if ((g_stick.afAngle[nCntStick] > D3DX_PI * 0.125f) && (g_stick.afAngle[nCntStick] < D3DX_PI * 0.875f))
				{
					g_bAngle[nCntStick][STICKANGLE_RIGHT] = true;
				}

				//�p�x���l�����ō��Ɉʒu���鎞�A���t���O��^�ɂ���
				if ((g_stick.afAngle[nCntStick] > D3DX_PI * -0.875f) && (g_stick.afAngle[nCntStick] < D3DX_PI * -0.125f))
				{
					g_bAngle[nCntStick][STICKANGLE_LEFT] = true;
				}
			}

			//�p�x�ɉ��������͏���
			for (int nCntAngle = 0; nCntAngle < STICKANGLE_MAX; nCntAngle++)
			{
				//�X�e�B�b�N�̃g���K�[����ۑ�
				g_stick.abAngleTrigger[nCntStick][nCntAngle] = (g_stick.abAnglePress[nCntStick][nCntAngle] ^ g_bAngle[nCntStick][nCntAngle]) & g_bAngle[nCntStick][nCntAngle];

				//�X�e�B�b�N�̃����[�X����ۑ�
				g_stick.abAngleRelease[nCntStick][nCntAngle] = (g_stick.abAnglePress[nCntStick][nCntAngle] ^ g_bAngle[nCntStick][nCntAngle]) & g_bAngle[nCntStick][nCntAngle];

				//���݂̎��Ԃ��擾
				g_aStickCurrentTime[nCntStick][nCntAngle] = timeGetTime();

				if (g_bAngle[nCntStick][nCntAngle] && ((g_aStickCurrentTime[nCntStick][nCntAngle] - g_aStickExecLastTime[nCntStick][nCntAngle]) > SPEED_KEY))
				{
					//�Ō�ɐ^��Ԃ������Ԃ�ۑ�
					g_aStickExecLastTime[nCntStick][nCntAngle] = g_aStickCurrentTime[nCntStick][nCntAngle];

					//�X�e�B�b�N�̃��s�[�g����ۑ�
					g_stick.abAngleRepeat[nCntStick][nCntAngle] = g_bAngle[nCntStick][nCntAngle];
				}
				else
				{
					//�X�e�B�b�N�̃��s�[�g����ۑ�
					g_stick.abAngleRepeat[nCntStick][nCntAngle] = 0;
				}

				//�X�e�B�b�N�̃v���X����ۑ�
				g_stick.abAnglePress[nCntStick][nCntAngle] = g_bAngle[nCntStick][nCntAngle];
			}
		}
	}
}

//=======================================================================================================================================================================================================================
//2�_�̊p�x�����߂�
//=======================================================================================================================================================================================================================
float FindAngle(D3DXVECTOR3 pos, D3DXVECTOR3 TargetPos)
{
	float fAngle; //�p�x

	fAngle = atan2f(TargetPos.y - pos.y, TargetPos.x - pos.x);

	fAngle -= (D3DX_PI * 0.5f);
	fAngle *= -1.0f;

	return fAngle;
}

//=======================================================================================================================================================================================================================
// �R���g���[���[�̓��͏��̎擾
//=======================================================================================================================================================================================================================
XINPUT_STATE* GetXInputState(void)
{
	return &g_XInput;
}

//=======================================================================================================================================================================================================================
// �X�e�B�b�N�̓��͏����擾
//=======================================================================================================================================================================================================================
STICKINPUT GetStick(void)
{
	return g_stick;
}

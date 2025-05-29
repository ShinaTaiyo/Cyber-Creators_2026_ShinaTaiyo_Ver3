//========================================================
//
//９月１５日：ジョイパッドのトリガーを実装する[input.cpp]
//Author:ShinaTaiyo
//
//=========================================================


#include "input.h"

//マクロ定義
#define NUM_KEY_MAX (256)//キーの最大数

//グローバル変数
LPDIRECTINPUT8 g_pInput = NULL;//DirectInputオブジェクトへのポインタ
LPDIRECTINPUTDEVICE8 g_pDevKeyboard = NULL;//入力デバイス（キーボード）のポインタ

BYTE g_aKeyState[NUM_KEY_MAX];//キーボードのプレス情報
BYTE g_aKeyStateTrigger[NUM_KEY_MAX];//キーボードのトリガー情報（一発ずつ弾を撃つようにする）

XINPUT_STATE g_joykeyState;//ジョイパッドのプレス情報
XINPUT_STATE g_joykeyStateTrigger;//ジョイパッドのトリガー情報
//キーボードの初期化処理
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	//DirectInputオブジェクトの生成
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION,
		IID_IDirectInput8, (void**)&g_pInput,
		NULL)))
	{
		return E_FAIL;
	}

	//入力デバイス（キーボード）の生成
	if (FAILED(g_pInput->CreateDevice(GUID_SysKeyboard,
		&g_pDevKeyboard, NULL)))
	{
		return E_FAIL;
	}

	//データフォーマットを設定
	if (FAILED(g_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	//協調モードを設定
	if (FAILED(g_pDevKeyboard->SetCooperativeLevel(hWnd,
		(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	//キーボードへのアクセス権を獲得
	g_pDevKeyboard->Acquire();
	return S_OK;
}

//=================================
//ジョイパッドの初期化処理
//=================================
HRESULT InitJoypad(void)
{
	//メモリのクリア
	memset(&g_joykeyState, 0, sizeof(XINPUT_STATE));

	//XInputのステートを設定（有効にする）
	XInputEnable(true);

	return S_OK;
}

//=================================
//キーボードの終了処理
//=================================
void UninitKeyboard(void)
{
	//入力デバイス（キーボード）の破棄

	if (g_pDevKeyboard != NULL)
	{
		g_pDevKeyboard->Unacquire();//キーボードへのアクセスを放棄
		g_pDevKeyboard->Release();
		g_pDevKeyboard=NULL;
	}

	//DirectInputオブジェクトの破棄
	if (g_pInput != NULL)
	{
		g_pInput->Release();
		g_pInput = NULL;
	}
}

//===============================
//ジョイパッドの終了処理
//===============================
void UninitJoypad(void)
{
	//XInputのステートを設定（無効にする）
	XInputEnable(false);
}

//===================================
//キーボードの更新処理
//===================================
void UpdateKeyboard(void)
{
	BYTE aKeyState[NUM_KEY_MAX];//キーボードの入力情報
	int nCntKey;

	//入力デバイスからデータを取得
	if (SUCCEEDED(g_pDevKeyboard->GetDeviceState
	(sizeof(aKeyState), &aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{

			g_aKeyStateTrigger[nCntKey] = (g_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];//キーボードのトリガー情報を保存
			g_aKeyState[nCntKey] = aKeyState[nCntKey];//キーボードのプレス情報を保存
		}
		
	}
	else
	{
		g_pDevKeyboard->Acquire();//キーボードへのアクセス権を獲得
	}
}

//================================
//ジョイパッドの更新処理
//================================
void UpdateJoypad(void)
{
	XINPUT_STATE joykeyState;//ジョイパッドの入力情報

	//ジョイパッドの状態を取得
	if (XInputGetState(0, &joykeyState) == ERROR_SUCCESS)
	{
		WORD Button = joykeyState.Gamepad.wButtons;
		WORD OldButton = g_joykeyState.Gamepad.wButtons;
		g_joykeyStateTrigger.Gamepad.wButtons = ~OldButton & Button;//トリガー処理
		g_joykeyState = joykeyState;//ジョイパッドのプレス情報を保存
	}
}

//=======================================
//キーボードのプレス情報を取得
//=======================================
bool GetKeyboardPress(int nKey)
{
	return (g_aKeyState[nKey] & 0x80) != 0;
	//return ((g_aKeyState[nKey] & 0x80 != 0) ? true : false);
}

//===========================================
//ジョイパッドのプレス情報を取得
//===========================================
bool GetJoypadPress(JOYKEY key)
{
	return (g_joykeyState.Gamepad.wButtons & (0x01 << key)) != 0;
}

//======================================
//キーボードのトリガー情報を取得
//======================================
bool GetKeyboardTrigger(int nKey)
{
	return (g_aKeyStateTrigger[nKey] & 0x80) != 0;
	//return ((g_aKeyStateTrigger[nKey] & 0x80 != 0) ? true : false);
}
//======================================
//ジョイパッドのトリガー情報を取得
//======================================
bool GetJoyPadTrigger(JOYKEY key)
{
	return (g_joykeyStateTrigger.Gamepad.wButtons & (0x01 << key)) != 0;
}
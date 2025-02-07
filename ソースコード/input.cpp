//=========================================================
//
//５月２２日：DirectXクラス化、キー入力クラス化、実装
//Author:ShinaTaiyo
//
//=========================================================

//=========================
//インクルード
//=========================
#include "input.h"
#include "main.h"
#include "debugtext.h"
#include "manager.h"
//=========================================================

//=========================
//静的メンバ初期化
//=========================
LPDIRECTINPUT8 CInput::m_pInput = nullptr;

//=========================
//コンストラクタ
//=========================
CInput::CInput() : m_pDevice()
{

}
//=========================================================

//=========================
//デストラクタ
//=========================
CInput::~CInput()
{
	
}

//=========================
//初期化処理
//=========================
HRESULT CInput::Init(HINSTANCE hInstance, HWND hWnd)
{
	
	//====================================
	//DirectInputオブジェクトの生成
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
//終了処理
//=========================
void CInput::Uninit()
{
	//=====================================
	//DirectInputオブジェクトの破棄
	//=====================================
	if (m_pInput != nullptr)
	{
		m_pInput->Release();
		m_pInput = nullptr;
	}
	//======================================================
}

//=====================================
//コンストラクタ（キーボードクラス）
//=====================================
CInputKeyboard::CInputKeyboard() : m_aKeyState(),m_aKeyStateTrigger(),m_nRepeatCnt(0)
{
	memset(&m_aKeyState[0], NULL, sizeof(m_aKeyState));
	memset(&m_aKeyStateTrigger[0], NULL, sizeof(m_aKeyStateTrigger));
}
//===============================================================

//=====================================
//デストラクタ（キーボードクラス）
//=====================================
CInputKeyboard::~CInputKeyboard()
{

}
//===============================================================

//=====================================
//初期化処理（キーボードクラス）
//=====================================
HRESULT CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	m_nRepeatCnt = 0;//リピート入力のカウントをする
	if (CInput::Init(hInstance, hWnd))
	{
		return E_FAIL;
	}

	//===================================
	//入力デバイスの生成
	//===================================
	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard,
		&m_pDevice, NULL)))
	{
		return E_FAIL;
	}
	//========================================================

	//===================================
//データフォーマットを設定
//===================================
if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
{
	return E_FAIL;
}
//========================================================

//===================================
//協調モードを設定
//===================================
if (FAILED(m_pDevice->SetCooperativeLevel(hWnd,
	(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
{
	return E_FAIL;
}
//========================================================

//===================================
//キーボードへのアクセス権を獲得
//===================================
m_pDevice->Acquire();
//========================================================

return S_OK;
}
//===============================================================

//=====================================
//終了処理（キーボードクラス）
//=====================================
void CInputKeyboard::Uninit()
{
	//==========================================
	//入力デバイス（キーボード）の破棄
	//==========================================
	if (m_pDevice != NULL)
	{
		m_pDevice->Unacquire();//キーボードへのアクセスを放棄
		m_pDevice->Release();
		m_pDevice = NULL;
	}
	//========================================================================

	CInput::Uninit();
}
//===============================================================

//=====================================
//更新処理（キーボードクラス）
//=====================================
void CInputKeyboard::Update()
{
	BYTE aKeyState[MAX_KEY];//キーボードの入力情報
	int nCntKey;

	//入力デバイスからデータを取得
	if (SUCCEEDED(m_pDevice->GetDeviceState
	(sizeof(aKeyState), &aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < MAX_KEY; nCntKey++)
		{
			m_aKeyStateTrigger[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];//キーボードのトリガー情報を保存
			m_aKeyState[nCntKey] = aKeyState[nCntKey];//キーボードのプレス情報を保存
		}
	}
	else
	{
		m_pDevice->Acquire();//キーボードへのアクセス権を獲得
	}
	//=====================================================================================================================================================

}
//===============================================================

//=====================================
//プレス情報（キーボードクラス）
//=====================================
bool CInputKeyboard::GetPress(int nKey)
{
	return ((m_aKeyState[nKey] & 0x80) != 0) ? true : false;
}
//===============================================================

//=====================================
//トリガー情報（キーボードクラス）
//=====================================
bool CInputKeyboard::GetTrigger(int nKey)
{
	return ((m_aKeyStateTrigger[nKey] & 0x80) != 0) ? true : false;
}
//===============================================================

//=====================================
//リピート情報（キーボードクラス）
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
//ジョイパッドクラス
//*************************************

//=====================================
//静的メンバ宣言
//=====================================
const BYTE CInputJoypad::s_bRightTrigger_DeadZone = 10;//右トリガーボタンの微小な値の入力を無視する

//=====================================
//コンストラクタ（ジョイパッドクラス）
//=====================================
CInputJoypad::CInputJoypad() : m_joykeyStatePress(),m_joykeyStateTrigger(),m_fLSitckAimRot(0.0f), m_fRStickAimRot(0.0f), m_nRTTrigger_RepeatCnt(0),
m_nLTTrigger_RepeatCnt(0)
{
	//=======================
	//メモリのクリア
	//=======================
	memset(&m_joykeyStatePress, 0, sizeof(XINPUT_STATE));
	memset(&m_joykeyStateTrigger, 0, sizeof(XINPUT_STATE));

	//===============================================================

}
//================================================================

//=====================================
//デストラクタ（ジョイパッドクラス）
//=====================================
CInputJoypad::~CInputJoypad()
{

}
//================================================================

//=====================================
//初期化処理（ジョイパッドクラス）
//=====================================
HRESULT CInputJoypad::Init(HINSTANCE hInstance, HWND hWnd)
{
	//XInputのステートを設定（有効にする）
	XInputEnable(true);

	return S_OK;
}
//================================================================

//=====================================
//終了処理（ジョイパッドクラス）
//=====================================
void CInputJoypad::Uninit()
{
	//XInputのステートを設定（無効にする）
	XInputEnable(false);
}
//================================================================

//=====================================
//更新処理（ジョイパッドクラス）
//=====================================
void CInputJoypad::Update()
{
	XINPUT_STATE joykeyState;//ジョイパッドの入力情報
	//GamePad.bRightTrigger

	//ジョイパッドの状態を取得
	if (XInputGetState(0, &joykeyState) == ERROR_SUCCESS)
	{
		//WORD型はunsigined short(符号なし)なので、基本的に代入される値が（0～65535)なので、オーバーフローや範囲外のあたいが代入されることはない。
		//エラーは起きないので例外処理は必要ない
		WORD Button = joykeyState.Gamepad.wButtons;
		WORD OldButton = m_joykeyStatePress.Gamepad.wButtons;
		m_joykeyStateTrigger.Gamepad.wButtons = ~OldButton & Button;//1f前の入力状態を反転。現在押されているボタンと&演算（どちらも１なら１）するので、1f前おしていない場合、アクティブになり、トリガー処理が実現する

		BYTE RTTrigger = joykeyState.Gamepad.bRightTrigger;
		BYTE OldRTrigger = m_joykeyStatePress.Gamepad.bRightTrigger;	
		m_joykeyStateTrigger.Gamepad.bRightTrigger = ~OldRTrigger & RTTrigger;//1f前の入力状態を反転。現在押されているボタンと&演算（どちらも１なら１）するので、1f前おしていない場合、アクティブになり、トリガー処理が実現する

		BYTE LTTrigger = joykeyState.Gamepad.bLeftTrigger;
		BYTE OldLTrigger = m_joykeyStatePress.Gamepad.bLeftTrigger;
		m_joykeyStateTrigger.Gamepad.bLeftTrigger = ~OldLTrigger & LTTrigger;//1f前の入力状態を反転。現在押されているボタンと&演算（どちらも１なら１）するので、1f前おしていない場合、アクティブになり、トリガー処理が実現する
		m_joykeyStatePress = joykeyState;//ジョイパッドのプレス状態を保存
	}
	else
	{//状態取得失敗時の処理
	   ZeroMemory(&m_joykeyStatePress,sizeof(XINPUT_STATE));//値を初期化
	   ZeroMemory(&m_joykeyStateTrigger,sizeof(XINPUT_STATE));//値を初期化
	}
}
//================================================================

//=====================================
//プレス情報の取得（ジョイパッドクラス）
//=====================================
bool CInputJoypad::GetPress(JOYKEY key)
{
	return (m_joykeyStatePress.Gamepad.wButtons & (0x01 << static_cast<int>(key))) != 0;
}
//================================================================

//=====================================
//トリガー情報の取得（ジョイパッドクラス）
//=====================================
bool CInputJoypad::GetTrigger(JOYKEY key)
{
	return (m_joykeyStateTrigger.Gamepad.wButtons & (0x01 << static_cast<int>(key))) != 0;
}
//================================================================

//=====================================
//Lトリガーのプレス情報の取得（ジョイパッドクラス）
//=====================================
bool CInputJoypad::GetRT_Press()
{
	return m_joykeyStatePress.Gamepad.bRightTrigger > s_bRightTrigger_DeadZone;//自然に微小な値が入力されることがあるので、デッドゾーンを超えた値が入力されていたらtrueni
}
//========================================================================================================================================================

//==================================================================================
//Lスティックのプレス情報の取得（ジョイパッドクラス）
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
//トリガー情報の取得（ジョイパッドクラス）
//=====================================
bool CInputJoypad::GetLT_Trigger()
{
	return  m_joykeyStateTrigger.Gamepad.bLeftTrigger != 0;//0x0004（JOYKEY_LEFT)なら0x01<<2 = 00000111 = 0x0004
}
//========================================================================================================================================================

//=====================================
//プレス情報の取得（ジョイパッドクラス）
//=====================================
bool CInputJoypad::GetLT_Press()
{
	return m_joykeyStatePress.Gamepad.bLeftTrigger != 0;//0x0004（JOYKEY_LEFT)なら0x01<<2 = 00000111 = 0x0004;
}
//========================================================================================================================================================

//=====================================
//リピート情報の取得（ジョイパッドクラス）
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
//Lスティックのプレス情報の取得（ジョイパッドクラス）
//==================================================================================
bool CInputJoypad::GetLStickPress(const int nDivisionRot,float fDirectionRot)
{
	float LX = m_joykeyStatePress.Gamepad.sThumbLX;
	float LY = m_joykeyStatePress.Gamepad.sThumbLY;


	bool bActive = false;//スティックを押しているかどうか
	bool bSuccessDivision = false;//nDivisionRotで割った値の取得に成功したかどうか

	//determine how far the controller is pushed
	float magnitude = sqrt(LX * LX + LY * LY);

	//上で求めた角度を正規化する
	float normalizedLX = LX / magnitude;
	float normalizedLY = LY / magnitude;

	//今回のゲームは、Z軸の正方向が前なので、Z軸(右引数）を基準にX軸（左引数）の角度を求める
	float fAimRot = atan2f(normalizedLX, normalizedLY);
	//======================================
	//大まかな方向を決める
	//======================================
	float fLapRot = D3DX_PI * 2;//一周分の値
	float fDivRot = fLapRot / nDivisionRot;       //分割した向きの値

	float fRangeRotA = 0.0f;
	float fRangeRotB = 0.0f;
	for (int nCnt = 0; nCnt < nDivisionRot; nCnt++)
	{//-3.14 ～　3.14の間に固定する、分割したラジアンの範囲を決め、その範囲の間に合ったらその値を返す
		fRangeRotA = fDivRot * nCnt - D3DX_PI - fDivRot * 0.5f + fDirectionRot;
		fRangeRotB = fDivRot * (nCnt + 1) - D3DX_PI - fDivRot * 0.5f + fDirectionRot;

		if (fAimRot >= fRangeRotA && fAimRot <= fRangeRotB)
		{
			fAimRot = fDivRot * nCnt - D3DX_PI + fDirectionRot;//最後の値は補正値
			bSuccessDivision = true;
			break;
		}
	}

	if (bSuccessDivision == false)
	{//入力範囲、角度の補正
		fAimRot = fDivRot * nDivisionRot - D3DX_PI + fDirectionRot;
	}

	//============================================================================================================

	float normalizedMagnitude = 0.0f;

	//check if the controller is outside a circular dead zone
	if (magnitude > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{//スティックを押している

		//clip the magnitude at its expected maximum value
		if (magnitude > 32767) magnitude = 32767;

		//adjust magnitude relative to the end of the dead zone
		magnitude -= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;

		//optionally normalize the magnitude with respect to its expected range
		//giving a magnitude value of 0.0 to 1.0
		normalizedMagnitude = magnitude / (32767 - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);

		m_fLSitckAimRot = fAimRot;//求めた目的の角度を代入
		bActive = true;
	}
	else //if the controller is in the deadzone zero out the magnitude
	{//スティックを押していない
		magnitude = 0.0;
		normalizedMagnitude = 0.0;
		bActive = false;
	}
	return bActive;
}
//===============================================================

//=====================================================================================
//Rスティックのプレス情報を取得（ジョイパッドクラス）
//=====================================================================================
bool CInputJoypad::GetRStickPress(const int nDivisionRot)
{
	float RX = m_joykeyStatePress.Gamepad.sThumbRX;
	float RY = m_joykeyStatePress.Gamepad.sThumbRY;

	bool bActive = false;//スティックを押しているかどうか
	bool bSuccessDivision = false;//nDivisionRotで割った値の取得に成功したかどうか

	//determine how far the controller is pushed
	float magnitude = sqrt(RX * RX + RY * RY);

	//上で求めた角度を正規化する
	float normalizedRX = RX / magnitude;
	float normalizedRY = RY / magnitude;

	//正規化した角度で目的の角度を求める
	float fAimRot = atan2f(normalizedRX, normalizedRY);

	//======================================
	//大まかな方向を決める
	//======================================
	float fLapRot = D3DX_PI * 2;//一周分の値
	float fDivRot = fLapRot / nDivisionRot;       //分割した向きの値

	float fRangeRotA = 0.0f;
	float fRangeRotB = 0.0f;
	for (int nCnt = 0; nCnt < nDivisionRot; nCnt++)
	{
		fRangeRotA = fDivRot * nCnt - D3DX_PI - fDivRot * 0.5f;
		fRangeRotB = fDivRot * (nCnt + 1) - D3DX_PI - fDivRot * 0.5f;

		if (fAimRot >= fRangeRotA && fAimRot <= fRangeRotB)
		{
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
	{//スティックを押している

		//clip the magnitude at its expected maximum value
		if (magnitude > 32767) magnitude = 32767;

		//adjust magnitude relative to the end of the dead zone
		magnitude -= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;

		//optionalRY normalize the magnitude with respect to its expected range
		//giving a magnitude value of 0.0 to 1.0
		normalizedMagnitude = magnitude / (32767 - XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);

		m_fRStickAimRot = fAimRot;//求めた目的の角度を代入
		bActive = true;
	}
	else //if the controller is in the deadzone zero out the magnitude
	{//スティックを押していない
		magnitude = 0.0;
		normalizedMagnitude = 0.0;
		bActive = false;
	}

	return bActive;
}
//===============================================================

//=====================================
//Lトリガーのトリガー情報の取得（ジョイパッドクラス）
//=====================================
bool CInputJoypad::GetRT_Trigger()
{
	return m_joykeyStateTrigger.Gamepad.bRightTrigger > s_bRightTrigger_DeadZone;//自然に微小な値が入力されることがあるので、デッドゾーンを超えた値が入力されていたらtrueにする
}
//================================================================

//****************************************************************************
//マウスクラス
//****************************************************************************

//======================================
//コンストラクタ
//======================================
CInputMouse::CInputMouse() : m_CursorPosOld(D3DXVECTOR2(0.0f,0.0f)), m_bLeftClickTriggerFlag(false),m_nLeftClickRepeatCnt(0),m_bRightClickTriggerFlag(false),
m_bCursorSenterWarp(false)
{

}
//================================================================

//======================================
//デストラクタ
//======================================
CInputMouse::~CInputMouse()
{

}
//================================================================

//======================================
//初期化処理
//======================================
HRESULT CInputMouse::Init(HINSTANCE hInstance, HWND hWnd)
{
	return S_OK;
}
//================================================================

//======================================
//終了処理
//======================================
void CInputMouse::Uninit()
{

}
//================================================================

//======================================
//更新処理
//======================================
void CInputMouse::Update()
{

}
//================================================================

//======================================
//カーソルの位置を取得
//======================================
D3DXVECTOR2 CInputMouse::GetMousePos()
{
	POINT MousePoint;
	memset(&MousePoint, 0, sizeof(POINT));
	GetCursorPos(&MousePoint);//スクリーン座標を取得
	HWND hwnd = FindWindow(nullptr, "AcrobaticGuns");//exeのウインドウを取得
	ScreenToClient(hwnd, &MousePoint);//現在のカーソルの位置をウインドウの位置に変換
	D3DXVECTOR2 CursorPos;
	CursorPos.x = static_cast<float>(MousePoint.x);//float型にキャストした位置を代入
	CursorPos.y = static_cast<float>(MousePoint.y);//float型にキャストした位置を代入

	m_bCursorSenterWarp = false;//端にいった場合に中心に移動する処理の前で毎回初期化
	if (CScene::GetMode() != CScene::MODE_TITLE)
	{
		if (CManager::GetInputKeyboard()->GetPress(DIK_LCONTROL) == false)
		{//タイトル画面でウインドウの位置を調整できるようにするため
			if (CursorPos.x > SCREEN_WIDTH || CursorPos.x < 0.0f)
			{//カーソルを中心に移動
				CursorPos.x = SCREEN_WIDTH / 2;
				CursorPos.y = SCREEN_HEIGHT / 2;
				SetCursorPos(static_cast<int>(CursorPos.x), static_cast<int>(CursorPos.y));
				m_bCursorSenterWarp = true;
			}
			if (CursorPos.y > SCREEN_HEIGHT || CursorPos.y < 0.0f)
			{//カーソルを中心に移動
				CursorPos.x = SCREEN_WIDTH / 2;
				CursorPos.y = SCREEN_HEIGHT / 2;
				SetCursorPos(static_cast<int>(CursorPos.x), static_cast<int>(CursorPos.y));
				m_bCursorSenterWarp = true;
			}
		}
	}
	return CursorPos;
}
//================================================================

//======================================
//カーソルが動いた角度を取得
//======================================
bool CInputMouse::GetMouseMoveAngle(float& fYaw, float& fPitch, float fAdjust)
{
	// 画面の中心座標
	POINT center = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };

	POINT curPos;
	memset(&curPos, 0, sizeof(POINT));//カーソルの位置を初期化
	GetCursorPos(&curPos); // 現在のマウス位置を取得

	if (curPos.x == center.x && curPos.y == center.y)
	{//1f前と同じ位置の場合、マウスを動かしていないと仮定し、falseとする
		return false;
	}

	// マウスの移動量を計算
	float deltaX = (curPos.y - center.y) * fAdjust;
	float deltaY = (curPos.x - center.x) * fAdjust;

	// カメラの角度に反映（適宜変更）
	fYaw += deltaY;
	fPitch += deltaX;

	// マウスを中央にリセット
	SetCursorPos(static_cast<int>(center.x), static_cast<int>(center.y));
	return true;
}
//================================================================

//======================================
//クリックしたかどうかを取得(Press)
//======================================
bool CInputMouse::GetMouseLeftClickPress()
{
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		return true;
	}
	return false;
}
//================================================================

//======================================
//クリックしたかどうかを取得(Trigger)
//======================================
bool CInputMouse::GetMouseLeftClickTrigger()
{
	//トリガーフラグがtrueなら発動。次ボタンを話すまではtrueにならない！
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		if (m_bLeftClickTriggerFlag == true)
		{
		    m_bLeftClickTriggerFlag = false;
			return true;
		}
		m_bLeftClickTriggerFlag = false;
	}
	else
	{
		m_bLeftClickTriggerFlag = true;
	}
	return false;
}

//======================================
//クリックしたかどうかを取得(Repeat)
//======================================
bool CInputMouse::GetMouseLeftClickRepeat(int nRepeat)
{
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		m_nLeftClickRepeatCnt++;
		if (m_nLeftClickRepeatCnt == nRepeat)
		{
			m_nLeftClickRepeatCnt = 0;
			return true;
		}
	}
	return false;
}
//================================================================

//======================================
//右クリックしたかどうかを取得
//======================================
bool CInputMouse::GetMouseRightClickTrigger()
{
	//トリガーフラグがtrueなら発動。次ボタンを話すまではtrueにならない！
	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
	{
		if (m_bRightClickTriggerFlag == true)
		{
			m_bRightClickTriggerFlag = false;
			return true;
		}
		m_bRightClickTriggerFlag = false;
	}
	else
	{
		m_bRightClickTriggerFlag = true;
	}
	return false;
}
//================================================================
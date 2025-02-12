//====================================================
//
//５月２２日：C++で基盤づくりLV5[input.h]
//Author:ShinaTaiyo
//
//====================================================

#ifndef _INPUT_H_  
#define _INPUT_H_

//======================
//インクルード
//======================
#include "main.h"
//==========================================

//======================
//マクロ定義
//======================
#define MAX_KEY (256)//キーの最大数

//==========================================
//入力規定クラス(DirectInputの管理)
//==========================================
class CInput
{
public:
	CInput();                                                            //コンストラクタ
	virtual ~CInput();                                                   //デストラクタ
	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);                //初期化処理
	virtual void Uninit();                                               //終了処理
	virtual void Update() = 0;                                           //更新処理（入力端末ごとに実装）
protected:
	//全入力処理で共有
	static LPDIRECTINPUT8 m_pInput;
	LPDIRECTINPUTDEVICE8 m_pDevice;
private:
};
//=======================================================================================


//==========================================
//キーボードクラス
//==========================================
class CInputKeyboard : public CInput
{
public:
	CInputKeyboard();                                                    //コンストラクタ
	~CInputKeyboard();                                                   //デストラクタ
	HRESULT Init(HINSTANCE hInstance, HWND hWnd) override;               //初期化処理
	void Uninit() override;                                              //終了処理
	void Update() override;                                              //更新処理

	bool GetPress(int nKey);                                             //プレス状態取得
	bool GetTrigger(int nKey);                                           //トリガー状態取得
	bool GetRepeat(int nKey);                                            //リピート状態取得
private:
	static const int m_nMAX_REPEATCNT = 14;                              //リピート入力の際にトリガーの判定を行うカウント数
	BYTE m_aKeyState[MAX_KEY];
	BYTE m_aKeyStateTrigger[MAX_KEY];
	int m_nRepeatCnt;                                                    //リピート入力のカウント
};
//=======================================================================================

//==========================================
//ジョイパッドクラス
//==========================================
class CInputJoypad : public CInput
{
public:
	//==========================================
    //ジョイパッドのボタン番号
    //==========================================
	typedef enum
	{
		JOYKEY_UP = 0,	//上
		JOYKEY_DOWN,	//下
		JOYKEY_LEFT,	//左
		JOYKEY_RIGHT,	//右
		JOYKEY_START,	//スタート
		JOYKEY_BACK,	//バック
		JOYKEY_L3,		//L3(左スティック押込み)
		JOYKEY_R3,		//R3(右スティック押込み)
		JOYKEY_LB,		//LB
		JOYKEY_RB,		//RB
		JOYKEY_LT,		//LT
		JOYKEY_RT,		//RT
		JOYKEY_A,		//A
		JOYKEY_B,		//B
		JOYKEY_X,		//X
		JOYKEY_Y,		//Y
		JOYKEY_MAX
	}JOYKEY;
	//==============================================================================================================================

	CInputJoypad();                                                       //コンストラクタ
	~CInputJoypad();                                                      //デストラクタ
	HRESULT Init(HINSTANCE hInstance, HWND hWnd) override;                //初期化処理
	void Uninit() override;                                               //終了処理
	void Update() override;                                               //更新処理
 	bool GetPress(JOYKEY key);                                            //プレス状態獲得
	bool GetTrigger(JOYKEY key);                                          //トリガー状態獲得
	bool GetRT_Trigger();                                       //Lトリガーボタンのトリガー情報
	bool GetRT_Press();
private:
	XINPUT_STATE m_joykeyStatePress;      //ジョイパッドのプレス情報
	XINPUT_STATE m_joykeyStateTrigger;    //ジョイパッドのトリガー情報

};
#endif

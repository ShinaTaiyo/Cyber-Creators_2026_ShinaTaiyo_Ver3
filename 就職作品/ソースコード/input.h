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
#define EXPORT extern "C" _declspec(dllexport)
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
	LPDIRECTINPUTDEVICE8 m_pDevice;//!
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
	BYTE m_aKeyState[MAX_KEY];                                           //!
	BYTE m_aKeyStateTrigger[MAX_KEY];                                    //!
	int m_nRepeatCnt;                                                    //リピート入力のカウント!
};
//=======================================================================================

//==========================================
//ジョイパッドクラス
//==========================================
class CInputJoypad
{
public:
	//==========================================
    //ジョイパッドのボタン番号
    //==========================================
	enum class JOYKEY
	{
		UP = 0,	//上
		DOWN,	//下
		LEFT,	//左
		RIGHT,	//右
		START,	//スタート
		BACK,	//バック
		L3,		//L3(左スティック押込み)
		R3,		//R3(右スティック押込み)
		LB,		//LB
		RB,		//RB
		LT,		//LT
		RT,		//RT
		A,		//A
		B,		//B
		X,		//X
		Y,		//Y
		MAX
	};
	//==============================================================================================================================

	CInputJoypad();                                                       //コンストラクタ
	~CInputJoypad();                                                      //デストラクタ
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);                         //初期化処理
	void Uninit();                                                        //終了処理
	void Update();                                                        //更新処理
 	bool GetPress(JOYKEY key);                                            //プレス状態獲得
	bool GetTrigger(JOYKEY key);                                          //トリガー状態獲得
	bool GetRT_Trigger();                                                 //Rトリガーボタンのトリガー情報
 	bool GetRT_Press();                                                   //Rトリガーボタンのプレス情報
	bool GetRT_Repeat(const int nRepeatLoop);                             //Rトリガーボタンのリピート情報
	bool GetLT_Trigger();                                                 //Lトリガーボタンのトリガー情報
 	bool GetLT_Press();                                                   //Lトリガーボタンのプレス情報
	bool GetLT_Repeat(const int nRepeatLoop);                             //Lトリガーボタンのリピート情報
	bool GetLStickPress(const int nDivisionRot,float fDirectionRot);                          //Lスティックのプレス情報
	bool GetRStickPress(const int nDivisionRot);                          //Rスティックのプレス情報

	const float & GetLStickAimRot() const { return m_fLSitckAimRot; }     //Lスティックの目的の角度を取得する
	const float & GetRStickAimRot() const { return m_fRStickAimRot; }     //Rスティックの目的の角度を取得する
private:
	//*静的メンバ
	static const BYTE s_bRightTrigger_DeadZone;

	XINPUT_STATE m_joykeyStatePress;      //ジョイパッドのプレス情報!
	XINPUT_STATE m_joykeyStateTrigger;    //ジョイパッドのトリガー情報!

	float m_fLSitckAimRot;                //Lスティックの目的の角度を代入する
	float m_fRStickAimRot;                //Rスティックの目的の角度を代入する

	int m_nRTTrigger_RepeatCnt;           //Rトリガーのリピートカウント
	int m_nLTTrigger_RepeatCnt;           //Lトリガーのリピートカウント

};

//==========================================
//マウスクラス
//==========================================
class CInputMouse
{
public:
	CInputMouse();//コンストラクタ
	~CInputMouse();//デストラクタ
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);//初期化処理
	void Uninit();//終了処理
	void Update();//更新処理
	bool GetMouseMoveAngle(float & fYaw,float & fPitch,float fAdjust);//マウスが動いた角度を取得
	bool GetMouseLeftClickPress();//クリックしたかどうか
	bool GetMouseLeftClickTrigger();//トリガー
	bool GetMouseLeftClickRepeat(int nRepeat);//リピート入力

	bool GetMouseRightClickPress();  //右クリックプレス情報取得
	bool GetMouseRightClickTrigger();//右クリックトリガー情報取得
	bool GetMouseRightClickRepeat(int nRepeat); //右クリックリピート情報取得

	bool GetMouseMiddleClickPress();//ミドルクリックプレス情報取得
	bool GetMouseMiddleClickTrigger();//ミドルクリックトリガー情報取得
	bool GetMouseMiddleClickRepeat(int nRepeat);//ミドルクリックのリピート情報取得
private:
	D3DXVECTOR2 m_CursorPosOld;//1f前のカーソルの位置

	//左クリック
	bool m_bLeftClickTriggerFlag;//左クリックのトリガーフラグ
	int m_nLeftClickRepeatCnt;   //左クリックのリピートカウント
	bool m_bPrevLeftClick;       //左クリックをしたかどうか
	void GetStateLeftClick();//左クリックの情報を取得する

	//右クリック
	bool m_bRightClickTriggerFlag;//右クリックトリガーフラグ
	int m_nRightClickRepeatCnt;   //右クリックのリピートカウント
	bool m_bPrevRightClick;       //右クリックをしたかどうか
	void GetStateRightClick();//右クリックの情報を取得する

	//ミドルクリック
	bool m_bMiddleClickTriggerFlag;//ミドルクリックのトリガーフラグ
	int m_nMiddleClickRepeatCnt;   //ミドルクリックのリピートカウント
	bool m_bPrevMiddleClick;       //ミドルクリックをしたかどうか
	void GetStateMiddleClick();//ミドルクリックの情報を取得する

	bool m_bCursorSenterWarp;     //カーソルが中心にワープしたかどうか
};
#endif

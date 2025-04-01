//===============================================================================
//
//２０２５年３月３１日：ポーズ処理を実装する[pausemanager_state.h]
//Author:ShinaTaiyo
//
//===============================================================================

#ifndef _PAUSEMANAGER_STATE_H_  
#define _PAUSEMANAGER_STATE_H_

//========================================
//インクルード
//========================================
#include "main.h"
#include "pausemanager.h"
//===============================================================================

//========================================
//クラス
//========================================

//ポーズステートクラス
class CPauseState_Component
{
public:
	CPauseState_Component(CPauseManager* pPauseManager);         //コンストラクタ
	virtual ~CPauseState_Component();                            //デストラクタ
	virtual void Update(CPauseManager* pPauseManager);           //更新処理
private:
};

//ポーズ通常ステートクラス
class CPauseState_Normal : public CPauseState_Component
{
public:
	CPauseState_Normal(CPauseManager* pPauseManager);           //コンストラクタ
	~CPauseState_Normal() override;                             //デストラクタ
	void Update(CPauseManager* pPauseManager) override;         //更新処理
private:
	//===========================================
    //列挙型
    //===========================================
	enum class COMMANDTYPE
	{
		CONTINUE = 0,       //ゲームに戻る
		EXIT,               //タイトルに戻る
		CAMERASENSITIVITY,  //カメラ感度調整
		MAX
	};//コマンド列挙型
	//=====================================================================================================================

	//===========================================
	//静的メンバ変数
	//===========================================

	//=====================================================================================================================

	//===========================================
	//変数
	//===========================================
	CUi* m_pCommandUi[static_cast<int>(COMMANDTYPE::MAX)] = {};//コマンドUI
	int m_nChooseCommandNum = 0;                               //選んでいるコマンドの番号

	//=====================================================================================================================

	//===========================================
	//関数
	//===========================================
	void ChooseCommand();                                                  //コマンド選択
	void ShowSelectedCommand();                                            //選択したコマンドを表示する処理
	void ExecuteCommand(CPauseManager * pPauseManager);                    //コマンドを発動する処理
	//=====================================================================================================================

};

//カメラ感度調整クラス
class CPauseState_CameraSensitivity : public CPauseState_Component
{
public:
	CPauseState_CameraSensitivity(CPauseManager* pPauseManager);           //コンストラクタ
	~CPauseState_CameraSensitivity() override;                             //デストラクタ
	void Update(CPauseManager* pPauseManager) override;         //更新処理
private:
	//===========================================
    //列挙型
   //===========================================
	enum class COMMANDTYPE
	{
		SENSITIVITY = 0,       //カメラ感度調整をする
		RETURN,                //コマンド選択画面に戻る
		MAX
	};//コマンド列挙型
	//=====================================================================================================================

	//===========================================
    //変数
    //===========================================

	CUi * m_pSensitivityLevelStick = nullptr;                   //感度レベルを表す棒
	CUi* m_pNowSensitivityLevelVar = nullptr;                   //感度レベルを表すバー
	CUi* m_pReturnTex = nullptr;                                //戻るテクスチャ
	int m_nSensitivitLevel = 10;                                //現在の感度レベル
	int m_nChooseCommandNum = 0;                                //選んでいるコマンドの番号
	//=====================================================================================================================

	//===========================================
	//関数
	//===========================================
	void ChooseCommand();                                 //コマンド選択処理
	void CameraSensitivity();                             //カメラ感度調整
	void Return(CPauseManager * pPauseManager);           //コマンド選択画面に戻る
	//=====================================================================================================================
};

#endif
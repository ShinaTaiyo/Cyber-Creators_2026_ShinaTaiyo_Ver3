//===============================================================================
//
//２０２５年３月３１日：ポーズ処理を実装する[game.h]
//Author:ShinaTaiyo
//
//===============================================================================

#ifndef _GAME_H_  
#define _GAME_H_

//========================================
//インクルード
//========================================
#include "scene.h"
#include "stagemanager.h"
#include "bgModel.h"
#include "player.h"
#include "main.h"
#include "phasemanager.h"
#include "eventmanager.h"
#include "pausemanager.h"
#include "score.h"
#include "combo.h"
#include "tutorial.h"
//===============================================================================

//========================================
//ゲームクラス
//========================================
class CGame : public CScene
{
public:
	CGame(bool bUseGamePad);                                               //コンストラクタ
	~CGame() override;                                                     //デストラクタ
	HRESULT Init() override;                                               //初期化処理
	void Uninit() override;                                                //終了処理
	void Update() override;                                                //更新処理
	static CPlayer* GetPlayer() { return m_pPlayer; }                      //プレイヤーを取得する
	static void SetGameClear(bool bClear) { s_bGameClear = bClear; }       //ゲームをクリアしたかどうかを設定
	static const bool& GetGameClear() { return s_bGameClear; }             //ゲームをクリアしたかどうかを取得する
	static CPhaseManager* GetPhaseManager() { return m_pPhaseManager; }    //フェーズマネージャーを取得する
	static CStageManager* GetStageManager() { return m_pStageManager; }    //ステージマネージャーを取得する
	static CTutorial* GetTutorial() { return m_pTutorial; }                //チュートリアル
	static CScore* GetScore() { return s_pSCORE; }                         //スコアへのポインタを取得する
	static CCombo* GetCombo() { return s_pCOMBO; }                         //コンボへのポインタを取得する
	static const bool& GetPauseFlag() { return s_bPAUSE; }                 //ポーズ中かどうかを取得する
	static void SetChengePause() { s_bCHENGEPAUSE = true; }                //ポーズ状態変更フラグをONにする
private:

	//===========================================
	//静的メンバ変数
	//===========================================
	//通常
	static constexpr bool s_bUSETUTORIAL = true;                           //チュートリアルを使用するかどうか
	static CPlayer* m_pPlayer;                                             //プレイヤー
	static CStageManager* m_pStageManager;                                 //ステージマネージャー
	static CPhaseManager* m_pPhaseManager;                                 //フェーズマネージャー
	static CTutorial* m_pTutorial;                                         //チュートリアル
	static CScore * s_pSCORE;                                              //スコア
	static CCombo* s_pCOMBO;                                               //コンボ
	static int s_nPhaseNum;                                                //フェーズ番号
	static bool s_bGameClear;                                              //ゲームをクリアしたかどうか
	static bool s_bPAUSE;                                                  //ポーズするかどうか
	static bool s_bCHENGEPAUSE;                                            //ポーズ状態変更フラグ
	//====================================================================================================================-

	//===========================================
	//変数
	//===========================================
	CPauseManager* m_pPauseManager;                                        //ポーズマネージャー
	//====================================================================================================================-

	//===========================================
	//関数
	//===========================================
	void PauseProcess();                                                   //ポーズ処理
	//====================================================================================================================-

};
#endif
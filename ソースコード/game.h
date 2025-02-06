//===============================================================================
//
//６月１７日：画面遷移を実装する[game.h]
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
#include "tutorial.h"
//===============================================================================

//========================================
//ゲームクラス
//========================================
class CGame : public CScene
{
public:
	CGame(bool bUseGamePad);                                                               //コンストラクタ
	~CGame() override;                                                     //デストラクタ
	HRESULT Init() override;                                               //初期化処理
	void Uninit() override;                                                //終了処理
	void Update() override;                                                //更新処理
	static CPlayer* GetPlayer() { return m_pPlayer; }                      //プレイヤーを取得する
	static CPhaseManager* GetPhaseManager() { return m_pPhaseManager; }    //フェーズマネージャーを取得する
	static bool GetUseGamePad() { return s_bUseGamePad; }                    
	static CStageManager* GetStageManager() { return m_pStageManager; }    //ステージマネージャーを取得する
	static CTutorial* GetTutorial() { return m_pTutorial; }                //チュートリアル
private:
	static CPlayer* m_pPlayer;                                             //プレイヤー
	static CStageManager* m_pStageManager;                                 //ステージマネージャー
	static CPhaseManager* m_pPhaseManager;                                 //フェーズマネージャー
	static CTutorial* m_pTutorial;                                         //チュートリアル
	static int s_nPhaseNum;                                                //フェーズ番号
	static bool s_bUseGamePad;                                             //ゲームパッドを使用するかどうか

};
#endif
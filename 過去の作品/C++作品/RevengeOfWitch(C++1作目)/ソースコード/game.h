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
#include "bg3d.h"
#include "field.h"
#include "player.h"
#include "main.h"
#include "eventmanager.h"
#include "timer.h"
#include "difficulty.h"
//===============================================================================

//========================================
//ゲームクラス
//========================================
class CGame : public CScene
{
public:
	CGame();                                                               //コンストラクタ
	~CGame() override;                                                     //デストラクタ
	HRESULT Init() override;                                               //初期化処理
	void Uninit() override;                                                //終了処理
	void Update() override;                                                //更新処理
	static CPlayer* GetPlayer() { return m_pPlayer; }                      //プレイヤーを取得する
	static CStageManager* GetStageManager() { return m_pStageManager; }    //ステージマネージャーを取得する
	static void SetDifficulty(CDifficulty::DIFFICULTYTYPE DifficultyType) { m_DifficultyType = DifficultyType; }//難易度を設定する
	static CDifficulty::DIFFICULTYTYPE GetDifficulty() { return m_DifficultyType; }//難易度を取得する
private:
	static CStageManager* m_pStageManager;                                 //ステージマネージャー
	static CPlayer* m_pPlayer;                                             //プレイヤー
	static CBg3D* m_pBg3D;                                                 //3D背景
	static CField* m_pField;                                               //フィールド
	static CTimer* m_pTimer;                                               //タイマー
	static CEventManager* m_pEventManager;                                 //イベントマネージャー
	static CDifficulty::DIFFICULTYTYPE m_DifficultyType;                   //難易度

};
#endif
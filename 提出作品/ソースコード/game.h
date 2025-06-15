//===================================================================================================================
//
// ２０２５年６月８日：OBB実装[game.h]
// Author:ShinaTaiyo
//
//===================================================================================================================

//*******************************************************************************************************************
// ２重インクルード防止
//*******************************************************************************************************************
#ifndef _GAME_H_  
#define _GAME_H_

//*******************************************************************************************************************
// インクルード
//*******************************************************************************************************************
#include "main.h"
#include "scene.h"

//*******************************************************************************************************************
// 前方宣言
//*******************************************************************************************************************
class CPhaseManager; // フェーズマネージャー
class CStageManager; // ステージマネージャー
class CPauseManager; // ポーズマネージャー
class CTutorial;     // チュートリアル
class CScore;        // スコア
class CCombo;        // コンボ
class CPlayer;       // プレイヤー
class CAttackEnemy;  // 敵

//*******************************************************************************************************************
// ゲームクラス
//*******************************************************************************************************************
class CGame : public CScene
{
public:
	// === 特殊関数 ===

	CGame(bool bUseGamePad); // コンストラクタ
	~CGame() override;       // デストラクタ

	// === ライフサイクルメンバ関数 ===

	HRESULT Init() override; // 初期化処理
	void Uninit() override;  // 終了処理
	void Update() override;  // 更新処理

	// == メンバ関数 ===

	// ゲームをクリアしたかどうか
	static void SetGameClear(bool bClear); // 設定
	static const bool& GetGameClear();     // 取得

	static CPlayer* GetPlayer(); // プレイヤーへのポインタ取得
	static CPhaseManager* GetPhaseManager(); // フェーズマネージャー取得
	static CStageManager* GetStageManager(); // ステージマネージャーを取得する
	static CTutorial* GetTutorial(); // チュートリアル
	static CScore* GetScore(); // スコアへのポインタ取得する
	static CCombo* GetCombo(); // コンボへのポインタ取得する
	static const bool& GetPauseFlag(); // ポーズ中かどうかを取得する
	static void SetChengePause(); // ポーズ状態変更フラグをONにする
private:
	// === 静的メンバ変数 ===

	static constexpr bool s_bUSETUTORIAL = true; // チュートリアルを使用するかどうか
	static int s_nPhaseNum;   // フェーズ番号
	static bool 
		s_bGameClear,   // ゲームをクリアしたかどうか
	    s_bPAUSE,       // ポーズするかどうか
	    s_bCHENGEPAUSE; // ポーズ状態変更フラグ
	static CPlayer* m_pPlayer;  // プレイヤー
	static CStageManager* m_pStageManager; // ステージマネージャー
	static CPhaseManager* m_pPhaseManager; // フェーズマネージャー
	static CTutorial* m_pTutorial; // チュートリアル
	static CScore * s_pSCORE; // スコア
	static CCombo* s_pCOMBO;  // コンボ

	// === メンバ変数 ===

	CPauseManager* m_pPauseManager; // ポーズマネージャー
	CAttackEnemy* m_pAttackEnemy; // 敵の攻撃
	// === メンバ関数 ===

	void PauseProcess(); // ポーズ処理
};
#endif
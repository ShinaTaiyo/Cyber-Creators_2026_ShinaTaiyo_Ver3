//===============================================================================
//
//７月１１日：オブジェクトのRelease方法を変更[edit.h]
//Author:ShinaTaiyo
//
//===============================================================================

#ifndef _EDIT_H_  
#define _EDIT_H_


//========================================
//インクルード
//========================================
#include "scene.h"
#include "stagemanager.h"
#include "player.h"
#include "bg3d.h"
#include "field.h"
//===============================================================================

//========================================
//エディットクラス
//========================================
class CEdit : public CScene
{
public:
	CEdit();                           //コンストラクタ
	~CEdit() override;                 //デストラクタ
	HRESULT Init() override;           //初期化処理
	void Uninit() override;            //終了処理
	void Update() override;            //更新処理
	static CPlayer* GetPlayer() { return m_pPlayer; }           //プレイヤーを取得する
	static CStageManager* GetStageManager() { return m_pStageManager; }    //ステージマネージャーを取得する
private:
	static CStageManager* m_pStageManager;                                 //ステージマネージャー
	static CPlayer* m_pPlayer;                                             //プレイヤー
	static CBg3D* m_pBg3D;                                                 //3D背景
	static CField* m_pField;                                               //フィールド
};
#endif
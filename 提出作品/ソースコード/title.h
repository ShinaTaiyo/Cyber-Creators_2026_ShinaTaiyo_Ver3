//===============================================================================
//
//６月１７日：画面遷移を実装する[title.h]
//Author:ShinaTaiyo
//
//===============================================================================

#ifndef _TITLE_H_  
#define _TITLE_H_


//========================================
//インクルード
//========================================
#include "scene.h"
#include "bg3d.h"
#include "player.h"
#include "ui.h"
//===============================================================================

//========================================
//タイトルクラス
//========================================
class CTitle : public CScene
{
public:
	CTitle();                           //コンストラクタ
	~CTitle() override;                 //デストラクタ
	HRESULT Init() override;            //初期化処理
	void Uninit() override;             //終了処理
	void Update() override;             //更新処理
	void Draw() override;               //描画処理

	static CPlayer* GetPlayer() { return m_pPlayer; }
private:
	static CPlayer* m_pPlayer;                   //プレイヤー
	static CBg3D* m_pBg3D;                       //3D背景

	CUi* m_pTitleLogo;                           //タイトルロゴ
	bool m_bMoveSwitch;                          //移動スイッチ

    //====================================
	//プロトタイプ宣言
	//====================================
	void PlayerProcess();                        //プレイヤーの処理
};
#endif
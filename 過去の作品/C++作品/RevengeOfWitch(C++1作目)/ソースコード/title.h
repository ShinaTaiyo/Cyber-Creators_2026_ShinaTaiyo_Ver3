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
private:
	CUi* m_pPressEnterLogo;             //PressEnter表示用
};
#endif
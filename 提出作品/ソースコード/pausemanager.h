//===============================================================================
//
//２０２５年３月３１日：ポーズ処理を実装する[pausemanager.h]
//Author:ShinaTaiyo
//
//===============================================================================

#ifndef _PAUSEMANAGER_H_  
#define _PAUSEMANAGER_H_

//========================================
//インクルード
//========================================
#include "main.h"
#include "ui.h"
#include "object.h"
//===============================================================================

//========================================
//前方宣言
//========================================
class CPauseState_Component;//ポーズステートクラス
//===============================================================================

//========================================
//ポーズマネージャークラス
//========================================
class CPauseManager : public CObject
{
public:
	//通常
	CPauseManager(int nPriority = 0, bool bUseintPriority = false, TYPE Type = TYPE::NONE, OBJECTTYPE ObjType = OBJECTTYPE::OBJECTTYPE_NONE);//コンストラクタ
	~CPauseManager() override;        //デストラクタ
	HRESULT Init() override;          //初期化処理
	void Uninit() override;           //終了処理
	void Update() override;           //更新処理
	void Draw() override;             //描画処理
	void SetDeath() override;         //死亡フラグ設定処理
	static CPauseManager* Create();   //生成処理

	//個別
	void ChengeState(CPauseState_Component* pState);//ステート変更処理

private:
	CPauseState_Component* m_pPauseState = nullptr;//ポーズステート
};

#endif
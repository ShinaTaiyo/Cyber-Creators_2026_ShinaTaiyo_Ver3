//====================================================
//
//９月１５日：ボス出現イベントを作る[eventmanager.h]
//Author:ShinaTaiyo
//
//====================================================

#ifndef _EVENTMANAGER_H_  
#define _EVENTMANAGER_H_

//======================
//インクルード
//======================
#include "main.h"
#include "object.h"
//====================================================

//======================
//前方宣言
//======================
class CUi;
class CDirectionFade;

//==========================================

//==========================================
//イベントマネージャークラス
//==========================================
class CEventManager : public CObject
{
public:
	//===================================================
	//イベント列挙型
	//===================================================
	typedef enum
	{
		EVENTTYPE_NONE = 0,      //イベントなし
		EVENTTYPE_BOSSAPPEAR,    //ボス登場イベント
		EVENTTYPE_MAX
	}EVENTTYPE;
	//=========================================================================================

	//===================================================
	//ボス出現種類
	//===================================================
	typedef enum
	{
		BOSSAPPEAR_KINGSLIME = 0,//キングスライムが出現
		BOSSAPPEAR_DEATHENAGA,   //デスエナガが出現
		BOSSAPPEAR_MRSTRONG,     //Mrストロングが出現
		BOSSAPPEAR_RILEY,        //ライリーが出現
		BOSSAPPEAR_DRAIYA,       //ドライヤが出現
		BOSSAPPEAR_GABRIEL,      //ガブリエルが出現
		BOSSAPPEAR_MAX
	}BOSSAPPEARTYPE;

	CEventManager();
	~CEventManager();        //デストラクタ
	HRESULT Init() override;          //初期化処理
	void Uninit() override;           //終了処理
	void Update() override;           //更新処理
	void Draw() override;             //描画処理
	void SetDeath() override;         //死亡フラグ設定処理
	void SetEvent(EVENTTYPE type) { m_NowEventType = type; m_bEventProgress = true;}//イベントを設定する
	CEventManager* Create(EVENTTYPE type);//生成処理

	void SetBossAppearEvent(BOSSAPPEARTYPE BossAppearType) { m_NowEventType = EVENTTYPE_BOSSAPPEAR; m_BossAppearType = BossAppearType; m_bEventProgress = true; }
	EVENTTYPE GetEventType() { return m_NowEventType; }//現在のイベントを取得する
private:
	EVENTTYPE m_NowEventType;//現在のイベント
	int m_nEventPattern;      //イベントのパターン
	int m_nCntEventTime;     //イベントの時間を測る
	bool m_bEventProgress;   //イベントが進行中かどうか
	void SetDeathOnly();     //本体オブジェクト以外を消去する
	void NextPattern();      //パターンを次に進める
	void ResetPattern();     //パターンをリセットする

	void BossAppearEventProcess();//ボス登場イベントの処理
	CUi* m_pMagicCircle;          //魔法陣へのポインタ
	CUi* m_pBossBattleText;       //ボスバトルのテキストUiへのポインタ
	CUi* m_pBossCutIn;            //ボスのカットイン
	CUi* m_pBossNameUi;           //ボスの名前のUI
	CDirectionFade* m_pDirectionFade;//演出用フェード

	BOSSAPPEARTYPE m_BossAppearType;//出現させるボスの種類

};

#endif

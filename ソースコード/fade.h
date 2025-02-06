//===============================================================================
//
//６月１７日：フェード処理を実装する[fade.h]
//Author:ShinaTaiyo
//
//===============================================================================

#ifndef _FADE_H_  
#define _FADE_H_


//========================================
//インクルード
//========================================
#include "main.h"
#include "object2d.h"
#include "manager.h"
//===============================================================================

//========================================
//フェードクラス
//========================================
class CFade : public CObject2D
{
public:
	typedef enum
	{
		FADEMODE_NONE = 0,//フェードをしていない
		FADEMODE_IN,  //フェードイン
		FADEMODE_OUT, //フェードアウト
		FADEMODE_MAX
	}FADEMODE;
	CFade(int nPri = 0, bool bUseintPri = false, CObject::TYPE type = CObject::TYPE::FADE, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_2D);                          //コンストラクタ
	~CFade() override;                //デストラクタ
	HRESULT Init() override;          //初期化処理
	void Uninit() override;           //終了処理
	void ExtraUninit() override;      //別枠の終了処理
	void Update() override;           //更新処理
	void Draw() override;             //描画処理
	void SetDeath() override;         //死亡フラグを設定
	void SetFade(FADEMODE FadeMode);
	static CFade* Create();           //生成処理
protected:
	void SetFadeCnt(int nFadeCnt) { m_nFadeCnt = nFadeCnt; }
	int& GetFadeCnt() { return m_nFadeCnt; }
	void SetMaxFadeCnt(int nMaxFadeCnt) { m_nMaxFadeCnt = nMaxFadeCnt; }
	int& GetMaxFadeCnt() { return m_nMaxFadeCnt; }
	void SetFadeMode(FADEMODE mode) { m_FadeMode = mode;}
	void SetStartFade(bool bUse) { m_bStartFade = bUse; }
	const bool GetStartFade() const { return m_bStartFade; }
private:
	int m_nFadeCnt;                   //フェードをする時のカウント!
	int m_nMaxFadeCnt;                //フェードカウント最大値!
	float m_fAlpha;                   //フェードのアルファ値!
	bool m_bStartFade;                //フェードを開始するフラグ
	FADEMODE m_FadeMode;              //フェードのモード!
};
//====================================================================================================================

//========================================
//シーンフェードクラス
//========================================
class CSceneFade : public CFade
{
public:
	CSceneFade();                          //コンストラクタ
	~CSceneFade() override;                //デストラクタ
	HRESULT Init() override;               //初期化処理
	void Uninit() override;                //終了処理
	void Update() override;                //更新処理
	void Draw() override;                  //描画処理
	void SetDeath() override;              //死亡フラグを設定
	static CSceneFade* Create();           //生成処理
	void SetSceneFade(FADEMODE FadeMode, CScene::MODE mode);  //フェードのモードを設定する
private:
	CScene::MODE m_NextMode;               //次のモードを決める  !
};
//====================================================================================================================

//========================================
//演出フェードクラス
//========================================
class CDirectionFade : public CFade
{
public:
	CDirectionFade();                          //コンストラクタ
	~CDirectionFade() override;                //デストラクタ
	HRESULT Init() override;               //初期化処理
	void Uninit() override;                //終了処理
	void Update() override;                //更新処理
	void Draw() override;                  //描画処理
	void SetDeath() override;              //死亡フラグを設定
	static CDirectionFade* Create();           //生成処理
private:
};
//====================================================================================================================
#endif
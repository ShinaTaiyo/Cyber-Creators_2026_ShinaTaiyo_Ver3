//=====================================================================================
//
//９月１０日：タイトルを作る[ui.h]
//Author:ShinaTaiyo
//
//=====================================================================================1

#ifndef _UI_H_  
#define _UI_H_

//======================
//インクルード
//======================
#include "main.h"
#include "object2d.h"
#include "number.h"
#include "gauge.h"
//==========================================

//======================
//前方宣言
//======================
class CObject2D;
class CUi;

//==========================================
//UIステート
//==========================================
class CUiState
{
public:
	//UIの状態列挙型
	enum class UISTATE
	{
		NONE = 0,//なし
		NUMERIC, //数字保持
		GAUGE,   //ゲージ保持
		MAX
	};
	CUiState();                 //コンストラクタ
	virtual ~CUiState();        //デストラクタ
	virtual void Process(CUi* pUi);//処理      
	void SetUiState(UISTATE pUiState) { m_pUiState = pUiState; }
	UISTATE GetUiState() { return m_pUiState; }//UIの状態を取得する
private:
	UISTATE m_pUiState = UISTATE::NONE;//UIの状態を格納
};
//=======================================================================================

//==========================================
//UIステート：数字保持
//==========================================
class CUiState_Numeric : public CUiState
{
public:
	CUiState_Numeric(CUi* pUi, int nValue, float fWidth, float fHeight);//コンストラクタ
	~CUiState_Numeric() override;//デストラクタ
	void Process(CUi* pUi) override;//処理
	void SetValue(int nValue,CUi * pUi);//数値を設定する
	int GetValue() { return m_nValue; };
private:
	vector<CNumber*> m_VecNum;//数字のベクター
	int m_nValue = 0;         //数値
	float m_fWidth = 0.0f;           //横幅基準値
	float m_fHeight = 0.0f;          //高さ基準値
};
//=======================================================================================

//==========================================
//UIステート：ゲージ保持
//==========================================
class CUiState_Gauge : public CUiState
{
public:
	CUiState_Gauge(D3DXVECTOR3 GaugePos,D3DXCOLOR Col,CObject2D::POLYGONTYPE PolygonType,CGauge::GAUGETYPE GaugeType,float fMaxWidth, float fMaxHeight, int nValue, int nMaxValue);//コンストラクタ
	~CUiState_Gauge() override;//デストラクタ
	void Process(CUi* pUi);//処理
	CGauge* GetGauge() { return m_pGauge;}//ゲージを取得
private:
	CGauge* m_pGauge = nullptr;//ゲージ
};
//=======================================================================================

//==========================================
//UIクラス
//==========================================
class CUi : public CObject2D
{
public:
	enum class UITYPE
	{
		LOCKON = 0,      //ロックオン
		ACTIONMODEWINDOW,//アクションモードウインドウ
	    ACTIONMODE_GUN,  //ショットモードUI
		ACTIONMODE_DIVE, //ダイブモードUI
		PHASETEXT,       //フェーズ文字
		STAGETEXT,       //ステージ文字
	    TARGET_000,      //ターゲット
		POSSIBLEDIVENUMTEXT_000,//ダイブ可能回数文字
		TUTORIAL_TEX,        //チュートリアル用テクスチャ
		CHECKMARK_000,       //チェックマーク
		TITLELOGO_000,       //タイトルロゴ
		PRESSENTER_000,      //エンターキーを押す
		DIVEGAUGEFRAME_000,  //ダイブゲージのフレーム
		GAMECLEAR_000,       //ゲームクリアの文字のテクスチャ
		LOSE_000,            //負けの文字のテクスチャ
		MAX
	};

	typedef enum
	{
		UIMOVETYPE_NORMAL = 0,//普通の動き
		UIMOVETYPE_FLOATING,  //浮遊させる
		UIMOVETYPE_MAX
	}UIMOVETYPE;

	CUi(int nPri = 0, bool bUseintPri = false, CObject::TYPE type = CObject::TYPE::UI, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_2D);                      //コンストラクタ
	~CUi() override;             //デストラクタ
	HRESULT Init() override;    //初期化処理
	void Uninit() override;     //終了処理
	void Update() override;     //更新処理
	void Draw() override;       //描画処理
	void SetDeath() override;   //死亡フラグを設定
	static CUi * Create(UITYPE type,CObject2D::POLYGONTYPE PolygonType,float fWidth, float fHeight,int nLife,bool bUseLife,D3DXVECTOR3 Pos,D3DXVECTOR3 Move,D3DXVECTOR3 Rot,D3DXCOLOR col);//背景の生成
	void SetUiType(UITYPE type);
	void SetUseUiEffect(bool bUse, int nSetEffectLife, D3DXCOLOR col) { m_bUseUiEffect = bUse; m_nSetUiEffectLife = nSetEffectLife; m_SetUiEffectColor = col; }
	UITYPE GetUiType() { return m_Type; }//UIの種類を取得
	void SetNumericState(int nValue, float fWidth, float fHeight);//数字状態を設定する

	CUiState* GetUiState(CUiState::UISTATE UiState);//指定したUIの状態を取得する
	void PushUiState(CUiState* pUiState) { m_VecUiState.push_back(pUiState);}//新しいUIの状態を動的配列に格納
protected:
	static const string UI_FILENAME[int(UITYPE::MAX)];//UIのテクスチャファイル名
private:
	UITYPE m_Type;                 //背景の種類
	UIMOVETYPE m_MoveType;         //UIの動き方の種類
	bool m_bUseUiEffect;           //UIでエフェクトを出すかどうか

	D3DXCOLOR m_SetUiEffectColor;  //UIで出すエフェクトの色合い設定用
	int m_nSetUiEffectLife;        //UIのエフェクトの体力設定用
	vector<CUiState*> m_VecUiState;//UIの状態構造体
};
//=======================================================================================

//==========================================
//UIエフェクトクラス
//==========================================
class CUiEffect : public CUi
{
public:
	CUiEffect();                //コンストラクタ
	virtual ~CUiEffect();       //デストラクタ
	HRESULT Init() override;    //初期化処理
	void Uninit() override;     //終了処理
	void Update() override;     //更新処理
	void Draw() override;       //描画処理
	void SetDeath() override;   //死亡フラグを設定
	static CUiEffect* Create(UITYPE type, CObject2D::POLYGONTYPE PolygonType, float fWidth, float fHeight,int nLife,D3DXVECTOR3 Pos, D3DXVECTOR3 Move, D3DXVECTOR3 Rot, D3DXCOLOR col);//背景の生成
private:
};
//=======================================================================================
#endif
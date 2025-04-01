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
#include "ui_composite.h"
#include "gauge.h"
//==========================================

//==========================================
//UIクラス
//==========================================
class CUi : public CObject2D
{
public:
	enum class UITYPE
	{//UIの種類
		LOCKON = 0,                      //ロックオン
		ACTIONMODEWINDOW,                //アクションモードウインドウ
	    ACTIONMODE_GUN,                  //ショットモードUI
		ACTIONMODE_DIVE,                 //ダイブモードUI
		PHASETEXT,                       //フェーズ文字
		STAGETEXT,                       //ステージ文字
	    TARGET_000,                      //ターゲット
		POSSIBLEDIVENUMTEXT_000,         //ダイブ可能回数文字
		TUTORIAL_TEX,                    //チュートリアル用テクスチャ
		CHECKMARK_000,                   //チェックマーク
		TITLELOGO_000,                   //タイトルロゴ
		PRESSENTER_000,                  //エンターキーを押す
		DIVEGAUGEFRAME_000,              //ダイブゲージのフレーム
		GAMECLEAR_000,                   //ゲームクリアの文字のテクスチャ
		LOSE_000,                        //負けの文字のテクスチャ
		COMBOTEXT_000,                   //コンボの文字のテクスチャ
	    PAUSETEXT_000,                   //ポーズの文字のテクスチャ
	    EXITTEXT_000,                    //EXITの文字のテクスチャ
	    CONTINUETEXT_000,                //CONTINUEの文字のテクスチャ
	    CAMERASENSITIVITYTEXT_000,       //カメラ感度調整の文字のテクスチャ
	    RETURNTEXT_000,                  //戻るの文字のテクスチャ
		MAX
	};

	typedef enum
	{
		UIMOVETYPE_NORMAL = 0,//普通の動き
		UIMOVETYPE_FLOATING,  //浮遊させる
		UIMOVETYPE_MAX
	}UIMOVETYPE;

	CUi(int nPri = 0, bool bUseintPri = false, CObject::TYPE type = CObject::TYPE::UI, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_2D);//コンストラクタ
	~CUi() override;                                                                                                                                     //デストラクタ
	HRESULT Init() override;                                                                                                                             //初期化処理
	void Uninit() override;                                                                                                                              //終了処理
	void Update() override;                                                                                                                              //更新処理
	void Draw() override;                                                                                                                                //描画処理
	void SetDeath() override;                                                                                                                            //死亡フラグを設定
	static CUi * Create(UITYPE type,bool bPauseUi,CObject2D::POLYGONTYPE PolygonType,float fWidth, float fHeight,int nLife,bool bUseLife,D3DXVECTOR3 Pos,D3DXVECTOR3 Move,D3DXVECTOR3 Rot,D3DXCOLOR col);//背景の生成
	void SetUiType(UITYPE type);//UIの種類を設定
	void SetUseUiEffect(bool bUse, int nSetEffectLife, D3DXCOLOR col) { m_bUseUiEffect = bUse; m_nSetUiEffectLife = nSetEffectLife; m_SetUiEffectColor = col; }//UIにエフェクトを設定
	UITYPE GetUiType() { return m_Type; }                                    //UIの種類を取得
	CUIComposite_Container* GetUiCompositeContainer() { return m_pUiCompositeContainer; }//UIの機能を取得
protected:
	static const string UI_FILENAME[int(UITYPE::MAX)];                       //UIのテクスチャファイル名
private:
	UITYPE m_Type;                                                           //背景の種類
	UIMOVETYPE m_MoveType;                                                   //UIの動き方の種類
	bool m_bUseUiEffect;                                                     //UIでエフェクトを出すかどうか
								                                             
	D3DXCOLOR m_SetUiEffectColor;                                            //UIで出すエフェクトの色合い設定用
	int m_nSetUiEffectLife;                                                  //UIのエフェクトの体力設定用

	CUIComposite_Container * m_pUiCompositeContainer;                           //UIの機能を持っているコンポジットパターン
};
//=======================================================================================
#endif
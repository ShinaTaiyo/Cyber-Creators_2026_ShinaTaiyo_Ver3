//===============================================================================
//
//２０２５年３月１８日：コンボを実装する[combo.h]
//Author:ShinaTaiyo
//
//===============================================================================

//========================================
//２重インクルード防止
//========================================
#ifndef _COMBO_H_  
#define _COMBO_H_
//===============================================================================


//========================================
//インクルード
//========================================
#include "main.h"
#include "object.h"
#include "ui.h"
//===============================================================================

//========================================
//コンボクラス
//========================================
class CCombo : public CObject

{
public:
	//ランク列挙型
	enum class RANK
	{
		E = 0,
		D,
		C,
		B,
		A,
		S,
		SS,
		SSS,
		MAX
	};

	//通常
	CCombo(int nPri = 0, bool bUseintPri = false, CObject::TYPE type = CObject::TYPE::COMBO, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_NONE);//コンストラクタ
	~CCombo() override;                                            //デストラクタ
	HRESULT Init() override;                                       //初期化処理
	void Uninit() override;                                        //終了処理
	void Update() override;                                        //更新処理
	void Draw() override;                                          //描画処理
	void SetDeath() override;                                      //死亡フラグ設定処理
	static CCombo* Create(D3DXVECTOR3 Pos);                        //生成処理

	//個別
	void AddCombo(int nCombo);                                     //コンボ加算
	void ResetCombo();                                             //コンボリセット
	int GetComboValue() { return m_nCombo; }                       //コンボ数取得
private:
	//=================
	//静的メンバ
	//=================
	static constexpr float s_fCOMBODISP_WIDTH = 180.0f;      //コンボ表示の横幅
	static constexpr float s_fCOMBODISP_HEIGHT = 180.0f;     //コンボ表示の高さ
	static constexpr float s_fCOMBODISP_VALUEWIDTH = 45.0f;  //コンボ表示の数値の横幅
	static constexpr float s_fCOMBODISP_VALUEHEIGHT = 45.0f; //コンボ表示の数値の高さ
	static const D3DXVECTOR3 s_COMBODISP_POS;                //コンボ表示の位置
	//===========================================================

	//=================
	//関数
	//=================

	//===========================================================

	//=================
	//メンバ変数
	//=================
	int m_nCombo = 0;             //コンボ数
	RANK m_Rank = RANK::E;        //Eランク
	float m_fMagnification = 0.0f;//スコア倍率
	CUi* m_pComboDisp = nullptr;    //コンボ数表示UI
	//===========================================================
};
//===============================================================================

#endif
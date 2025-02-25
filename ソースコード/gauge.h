//====================================================
//
//６月１６日：プレイヤーのゲージを実装[gauge.h]
//Author:ShinaTaiyo
//
//====================================================

#ifndef _GAUGE_H_  
#define _GAUGE_H_

//======================
//インクルード
//======================
#include "main.h"
#include "object2d.h"
//==========================================

class CDamageGauge;
//==========================================

//==========================================
//ゲージクラス
//==========================================
class CGauge : public CObject2D
{
public:
	//===========================
	//ゲージの種類
	//===========================
	enum class GAUGETYPE
	{//ゲージのタイプ
		PLAYERHP = 0, //プレイヤーの体力
		DIVE,         //ダイブゲージ
		MAX
	};
	//======================================================================
	CGauge(int nPri = 0, bool bUseintPri = false, CObject::TYPE type = CObject::TYPE::GAUGE, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_2D);//コンストラクタ
	~CGauge() override;                                                                                     //デストラクタ
	HRESULT Init() override;                                                                                //初期化処理
	void Uninit() override;                                                                                 //終了処理
	void Update() override;                                                                                 //更新処理
	void Draw() override;                                                                                   //描画処理
	virtual void AddParam(int nParam);                                                                      //パラメータを増加する
	void SubParam(int nParam) { m_nParam -= nParam; }                                                       //パラメータを減少する　
	void SetDeath() override;                                                                               //死亡フラグを設定
	int& GetParam() { return m_nParam; }                                                                    //パラメータを取得
	int GetMaxParam() { return m_nMaxParam; }                                                               //最大パラメータを取得
	virtual void SetParam(int nParam);                                                                      //パラメータを設定する
	void SetMaxParam(int nParam) { m_nMaxParam = nParam; }                                                  //最大パラメータを設定
	void SetGaugeType(GAUGETYPE type) { m_type = type; }                                                    //ゲージの種類を設定
	static CGauge* Create(GAUGETYPE type, int nParam, float fWidth, float fHeight, D3DXVECTOR3 pos);        //ゲージを生成

	//====================================
	//オプション
	//====================================
	void SetShake(float fPower, int nTime) { m_fShakePower = fPower; m_nShakeTime = nTime;}                 //ゲージをシェイクするかどうかを設定
	void SetUseGaugeProcess(bool bUse) { m_bGaugeProcess = bUse; }                                          //ゲージの処理をするかどうかを設定                               

	//====================================
	//ゲージの状態
	//====================================
	const bool& GetFullGaugeFlag() const { return m_bIsGaugeFull; }
protected:
private:
	int m_nParam;                                          //ゲージで表記する現在のパラメータ
	int m_nParamOld;                                       //一つ前のパラメータ
	GAUGETYPE m_type;                                      //ゲージの種類
	int m_nMaxParam;                                       //ゲージで表記するパラメータの最大値
								                           
	float m_fShakePower;                                   //シェイクさせる力
	int m_nShakeTime;                                      //シェイクさせる時間
								                           
	bool m_bIsGaugeFull;                                   //ゲージがマックスになった瞬間を表す
	int m_nFullGaugeCntTime;                               //ゲージがマックスになった瞬間を表す 
								                           
	bool m_bGaugeProcess;                                  //ゲージの処理をするかどうか
	//====================================
	//プロトタイプ宣言
	//====================================
	void FullGaugeTiming();                                //ゲージがマックスになったタイミングを表す
};
//=======================================================================================================================
#endif

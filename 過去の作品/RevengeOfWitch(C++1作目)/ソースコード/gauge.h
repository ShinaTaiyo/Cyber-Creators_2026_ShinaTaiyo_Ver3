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
	typedef enum
	{
		GAUGETYPE_PLAYERHP = 0, //プレイヤーの体力
		GAUGETYPE_PLAYERMP,     //プレイヤーのMP
		GAUGETYPE_BOSSHP,       //ボスのHP
		GAUGETYPE_DAMAGE,       //ダメージ
		GAUGETYPE_PLAYERUSAGEMP,//プレイヤーMP消費量
		GAUGETYPE_MAX
	}GAUGETYPE;
	//======================================================================
	CGauge();                   //コンストラクタ
	~CGauge() override;         //デストラクタ
	HRESULT Init() override;    //初期化処理
	void Uninit() override;     //終了処理
	void Update() override;     //更新処理
	void Draw() override;       //描画処理
	virtual void AddParam(int nParam);//パラメータを増加する
	void SubParam(int nParam) { m_nParam -= nParam; }            //パラメータを減少する　
	void SetDeath() override;   //死亡フラグを設定
	int& GetParam() { return m_nParam; }//パラメータを取得
	int GetMaxParam() { return m_nMaxParam; }//最大パラメータを取得
	virtual void SetParam(int nParam);
	void SetMaxParam(int nParam) { m_nMaxParam = nParam; }//最大パラメータを設定
	void SetGaugeType(GAUGETYPE type) { m_type = type; }
	static CGauge* Create(GAUGETYPE type, int nParam, float fWidth, float fHeight, D3DXVECTOR3 pos);//ゲージを生成
protected:
	int m_nParam;               //ゲージで表記する現在のパラメータ
	int m_nParamOld;            //一つ前のパラメータ
private:
	GAUGETYPE m_type;           //ゲージの種類
	CDamageGauge* m_pDamageGauge;//ダメージゲージ
	int m_nMaxParam;            //ゲージで表記するパラメータの最大値
};
//=======================================================================================================================

//==========================================
//ダメージゲージクラス
//==========================================
class CDamageGauge : public CGauge
{
public:
	CDamageGauge();             //コンストラクタ
	~CDamageGauge() override;   //デストラクタ
	HRESULT Init() override;    //初期化処理
	void Uninit() override;     //終了処理
	void Update() override;     //更新処理
	void Draw() override;       //描画処理
	void AddParam(int nParam) override; //パラメータを増加
	void SetParam(int nParam) override;   //パラメータを設定
	void SetDeath() override;   //死亡フラグを設定
	void SetStartDamageParam(int nParam) { m_nStartDamageParam = nParam; }
	void SetStartParam(int nParam) { m_nStartParam = nParam; }//ダメージ表示を開始する時のパラメータを設定
	static CDamageGauge* Create(CGauge::GAUGETYPE type, int nParam, int nMaxParam, float fWidth, float fHeight, D3DXVECTOR3 pos, int nStartDamageParam);//ゲージを生成
private:
	static const int m_nSETSUBSTARTCNT = 90;
	int m_nSubStartCnt;//ゲージの減算を始めるまでのカウント
	bool m_bSubGauge;  //ゲージの減少をするかどうか 
	int m_nFirstParam;//最新のパラメータ
	int m_nAddParamCnt;//パラメータを増やした回数
	int m_nStartParam;          //ダメージ表示を開始する時のパラメータ
	int m_nStartDamageParam;    //ダメージ表示を開始するときのダメージ量
};
//=======================================================================================================================
#endif

//==================================================================================================================
//
//２０２５年３月９日：コンポジットパターンを使用しUIに機能を持たせる[ui_composite.h]
//Author:ShinaTaiyo
//
//==================================================================================================================

#ifndef _UI_COMPOSITE_H_  
#define _UI_COMPOSITE_H_

//======================
//インクルード
//======================
#include "main.h"
#include "object2d.h"
#include "number.h"
#include "gauge.h"
//==================================================================================================================

//======================
//前方宣言
//======================
class CUi;
//==================================================================================================================

//======================
//クラス
//======================

//コンポーネント
class CUIComposite_Component
{
public:

	//機能の種類
	enum class UIFUNCTION
	{
		NONE = 0,//なし
		NUMERIC, //数字保持
		GAUGE,   //ゲージ保持
		MAX,     //最大
	};

	//共通
	CUIComposite_Component(CUi * pUi);         //コンストラクタ
	virtual ~CUIComposite_Component();         //デストラクタ
	virtual void Update(CUi* pUi) = 0;         //更新処理
	virtual void Draw(CUi* pUi) = 0;           //描画処理

	//個別
	void SetFunction(UIFUNCTION Function) { m_Function = Function; }//機能の種類を設定
	UIFUNCTION GetFunction() { return m_Function; }                 //機能の種類を取得

private:
	UIFUNCTION m_Function = UIFUNCTION::NONE;  //UIの機能の種類
};

//数字保持
class CUIComposite_Numeric : public CUIComposite_Component
{
public:
	//共通
	CUIComposite_Numeric(CUi* pUi, int nValue, float fWidth, float fHeight);      //コンストラクタ
	~CUIComposite_Numeric() override;     //デストラクタ
	void Update(CUi* pUi) override;       //更新処理
	void Draw(CUi* pUI) override;         //描画処理

	//個別
	void SetValue(int nValue, CUi* pUi);  //数値を設定する
	int GetValue() { return m_nValue; };  //数値を取得する
private:
	static constexpr int s_nMAX_DIGIT = 8; //最大桁数
	static const int s_nMAX_VALUE;        //最大数値
	vector<CNumber*> m_VecNum;            //数字の動的配列
	int m_nValue = 0;                     //数値
	int m_nDigit = 0;                     //桁数
	float m_fWidth = 0.0f;                //横幅基準値
	float m_fHeight = 0.0f;               //高さ基準値
};

//ゲージ保持
class CUIComposite_Gauge : public CUIComposite_Component
{
public:
	//共通
	CUIComposite_Gauge(CUi * pUi,D3DXVECTOR3 GaugePos, D3DXCOLOR Col, CObject2D::POLYGONTYPE PolygonType, CGauge::GAUGETYPE GaugeType, float fMaxWidth, float fMaxHeight, int nValue, int nMaxValue);//コンストラクタ
	~CUIComposite_Gauge() override;//デストラクタ
	void Update(CUi* pUi) override;//更新処理
	void Draw(CUi* pUi) override;  //描画処理

	//個別
	CGauge* GetGauge() { return m_pGauge; } //ゲージを取得
private:
	CGauge* m_pGauge = nullptr;             //ゲージ

};

//コンテナ
class CUIComposite_Container : public CUIComposite_Component
{
public:
	//共通
	CUIComposite_Container(CUi* pUi);;                                   //コンストラクタ
	~CUIComposite_Container() override;                                  //デストラクタ
	void Update(CUi * pUi) override;                                     //更新処理
	void Draw(CUi * pUi) override;                                       //描画処理
														                 
	//個別											                     
	void Add(CUIComposite_Component* pComponent);                        //機能の追加
	template<typename T>
	T* GetChildren()                                                     //指定した機能を取得する
	{
		for (auto& child : m_Children)
		{
			if (auto CastChild = dynamic_cast<T*>(child))//指定した機能にキャストして型が存在していたら値が入る。値があればtrue、なかったらfalseなので大丈夫
			{
				return CastChild;
			}
		}
		return nullptr;//指定した機能が存在しなかった
	};                                                    
private:
	vector<CUIComposite_Component*> m_Children;//所持している機能
};

//==================================================================================================================

#endif
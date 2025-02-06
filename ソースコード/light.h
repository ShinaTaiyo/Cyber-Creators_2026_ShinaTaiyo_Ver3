//========================================================
//
//６月３日：３Dオブジェクトを描画する[light.h]
//Author:ShinaTaiyo
//
//=========================================================
#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "main.h"
//=========================================================
//ライトクラス
//=========================================================
class CLight
{
public:
	CLight();      //コンストラクタ
	~CLight();     //デストラクタ
	HRESULT Init();//初期化処理
	void Uninit(); //終了処理
	void Update(); //更新処理
private:
	static const int m_nMAXLIGHT = 6;//ライトの最大数
	D3DLIGHT9 m_aLight[m_nMAXLIGHT] = {};//!
};
#endif

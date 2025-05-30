//====================================================
//
//５月２４日：C++で基盤づくり[explosion.h]
//Author:ShinaTaiyo
//
//====================================================

#ifndef _EXPLOSION_H_  
#define _EXPLOSION_H_

//======================
//インクルード
//======================
#include "main.h"
#include "object2d.h"
//==========================================

//==========================================

//==========================================
//爆発クラス
//==========================================
class CExplosion : public CObject2D
{
public:
	CExplosion();                                                               //コンストラクタ
	~CExplosion() override;                                                     //デストラクタ
	HRESULT Init() override;                                                    //初期化処理
	void Uninit() override;                                                     //終了処理
	void Update() override;                                                     //更新処理
	void Draw() override;                                                       //描画処理
	void SetDeath() override;                                                   //死亡フラグを設定
	static void Create(int nLife, float fWidth, float fHeight, D3DXVECTOR3 pos);//バレットを生成
private:
	int m_nLife;                                                                                                  //体力
	bool m_bUse;                                                                                                  //使用状態
};

#endif

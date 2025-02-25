//====================================================
//
//９月１５日：ボス登場演出を実装[particle2d.h]
//Author:ShinaTaiyo
//
//====================================================

#ifndef _PARTICLE2D_H_  
#define _PARTICLE2D_H_

//======================
//インクルード
//======================
#include "main.h"
#include "object2d.h"
//==========================================

//==========================================

//==========================================
//パーティクル２Dクラス
//==========================================
class CParticle2D : public CObject2D
{
public:
	CParticle2D(int nPri = 0, bool bUseintPri = false, CObject::TYPE type = CObject::TYPE::PARTICLE2D, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_2D);//コンストラクタ
	~CParticle2D() override;     //デストラクタ
	HRESULT Init() override;     //初期化処理
	void Uninit() override;      //終了処理
	void Update() override;      //更新処理
	void Draw() override;        //描画処理
	void SetDeath() override;    //死亡フラグを設定
	static CParticle2D* Create(D3DXVECTOR3 pos,D3DXVECTOR3 Move,D3DXVECTOR3 Rot,CObject2D::POLYGONTYPE PolygonType,int nLife, float fWidth, float fHeight,D3DXCOLOR col);//生成処理
private:
};

#endif
#pragma once

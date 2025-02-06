//====================================================
//
//５月２７日：C++で基盤づくり[effect.h]
//Author:ShinaTaiyo
//
//====================================================

#ifndef _EFFECT_H_  
#define _EFFECT_H_

//======================
//インクルード
//======================
#include "main.h"
#include "billboard.h"
//==========================================

//==========================================

//==========================================
//エフェクトクラス
//==========================================
class CEffect : public CBillboard
{
public:
	enum class EFFECTTYPE
	{
		NORMAL = 0,//普通のエフェクト
		MAX        //最大
	};

	CEffect(int nPri = 0, bool bUseintPri = false, CObject::TYPE type = CObject::TYPE::EFFECT, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_BILLBOARD);              //コンストラクタ
	~CEffect() override;    //デストラクタ
	HRESULT Init() override;//初期化処理
	void Uninit() override; //終了処理
	void Update() override; //更新処理
	void Draw() override;   //描画処理
	void SetDeath() override;//死亡フラグを設定
	static CEffect * Create(EFFECTTYPE type,int nLife, float fWidth, float fHeight,D3DXVECTOR3 pos,D3DXCOLOR col);//エフェクトを生成
	
	//===========================================
	//移動方法
	//===========================================
	void SetBallMove(bool bUse, D3DXVECTOR2 InitialRot, D3DXVECTOR2 AddRot, float fSpeed)
	{ m_bBallMove = bUse; m_BallMoveRot = InitialRot; m_BallMoveAddRot = AddRot; m_fBallMoveSpeed = fSpeed; }//円状移動の設定
	//=================================================================================================
private:
	//===========================================
	//静的メンバ
	//===========================================
	static const string s_EffectFileName[static_cast<int>(EFFECTTYPE::MAX)];//エフェクトのファイル名
	//=================================================================================================

	//===========================================
	//変数
	//===========================================
	EFFECTTYPE m_Type;        //エフェクトの種類
	float m_fReductionWidth;  //横幅縮小スピード
	float m_fReductionHeight; //高さ縮小スピード

	bool m_bBallMove;         //円状に移動する
	float m_fBallMoveSpeed;   //円状に移動する速度
	D3DXVECTOR2 m_BallMoveRot;//円状に移動する向き
	D3DXVECTOR2 m_BallMoveAddRot;//円状に移動するときの加算する向き

	//=================================================================================================
};
//====================================================================================================================================
#endif

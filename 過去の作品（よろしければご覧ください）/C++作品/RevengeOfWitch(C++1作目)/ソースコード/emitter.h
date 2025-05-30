//=======================================================================================================
//
//７月９日：エミッター実装[emittter.h]
//Author:ShinaTaiyo
//
//=======================================================================================================

#ifndef _EMITTER_H_
#define _EMITTER_H_

//======================
//インクルード
//======================
#include "main.h"
#include "billboard.h"
//==========================================

//======================
//前方宣言
//======================
class CEffect;

//==========================================

//==========================================
//エミッタークラス
//==========================================
class CEmitter : public CBillboard
{
public:
	CEmitter();              //コンストラクタ
	~CEmitter() override;    //デストラクタ
	HRESULT Init() override; //初期化処理
	void Uninit() override;  //終了処理
	void Update() override;  //更新処理
	void Draw() override;    //描画処理
	void SetDeath() override;//死亡フラグを設定
	static CEmitter * Create(int nEffectType,int nLife,int nSetLife,float fLength, float fWidth, float fHeight,D3DXVECTOR3 pos,D3DXVECTOR3 move,D3DXCOLOR col);//エミッターを生成
	void SetUseRotate(bool bUse, float fLength, D3DXVECTOR3 Rot, D3DXVECTOR3 AddRot)
	{m_bUseRotate = bUse; m_fLength = fLength; m_Rot = Rot; m_AddRot = AddRot;}//回転を使用するかどうか
private:
	D3DXVECTOR3 m_AddRot;       //加算回転量

	int m_nEffectType;          //設定するエフェクトの種類
	bool m_bUse;                //使用状態
	
	bool m_bUseRotate;          //回転を使用するかどうか
	float m_fLength;            //距離計算用
	D3DXVECTOR3 m_Rot;          //向き

	float m_fSetEffectWidth;    //設定するエフェクトの横幅
	float m_fSetEffectHeight;   //設定するエフェクトの高さ
	int m_nSetEffectLife;       //設定するエフェクトの体力
	D3DXCOLOR m_SetEffectColor; //設定するエフェクトの色合い
};
//====================================================================================================================================
#endif

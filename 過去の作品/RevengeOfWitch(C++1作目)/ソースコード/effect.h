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
	typedef enum
	{
		EFFECTTYPE00_NORMAL = 0,//普通のエフェクト
		EFFECTTYPE01_THUNDER,   //雷エフェクト
		EFFECTTYPE_MAX
	}EFFECTTYPE;

	CEffect();              //コンストラクタ
	~CEffect() override;    //デストラクタ
	HRESULT Init() override;//初期化処理
	void Uninit() override; //終了処理
	void Update() override; //更新処理
	void Draw() override;   //描画処理
	void SetDeath() override;//死亡フラグを設定
	static void Unload();   //テクスチャのアンロード
	static void Create(EFFECTTYPE type,int nLife, float fWidth, float fHeight,D3DXVECTOR3 pos,D3DXCOLOR col);//エフェクトを生成
private:
	EFFECTTYPE m_Type;        //エフェクトの種類
	bool m_bUse;              //使用状態
	float m_fReductionWidth;  //横幅縮小スピード
	float m_fReductionHeight; //高さ縮小スピード
	static  LPDIRECT3DTEXTURE9 m_pTextureTemp;//テクスチャの情報
	static const char* m_aEFFECT_FILENAME[EFFECTTYPE_MAX];//エフェクトのファイル名
};
//====================================================================================================================================
#endif

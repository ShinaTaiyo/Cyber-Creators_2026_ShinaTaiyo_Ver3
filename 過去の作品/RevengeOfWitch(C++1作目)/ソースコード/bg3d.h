//====================================================
//
//６月２１日：C++で基盤づくり[bg3d.h]
//Author:ShinaTaiyo
//
//====================================================

#ifndef _BG3D_H_  
#define _BG3D_H_

//======================
//インクルード
//======================
#include "main.h"
#include "billboard.h"
#include "meshcylinder.h"
//==========================================

//==========================================

//==========================================
//３D背景クラス
//==========================================
class CBg3D : public CMeshCylinder
{
public:
	//===========================
	//弾の種類
	//===========================
	typedef enum
	{
		BG3DTYPE_STAGE00 = 0, //ステージ０背景
		BG3DTYPE_STAGE01,     //ステージ１背景
		BULLETTYPE_MAX
	}BG3DTYPE;
	//======================================================================
	CBg3D();              //コンストラクタ
	~CBg3D() override;    //デストラクタ
	HRESULT Init() override;//初期化処理
	void Uninit() override; //終了処理
	void ExtraUninit() override;//別枠の終了処理
	void Update() override; //更新処理
	void Draw() override;   //描画処理
	void SetDeath() override;//死亡フラグを設定
	static CBg3D * Create(BG3DTYPE type,D3DXVECTOR3 Pos,int nNumBlockXZ,int nNumBlockY,float fRadius,float fHeight);//3D背景を生成
	void SetBg3DType(BG3DTYPE type);

private:
	BG3DTYPE m_type;      //弾の種類
};

#endif

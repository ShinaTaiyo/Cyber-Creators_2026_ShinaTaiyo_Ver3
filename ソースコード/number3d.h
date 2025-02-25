//======================================================================
//
//６月２５日：ワープマーカーを実装する[number3d.h]
//Author:ShinaTaiyo
//
//======================================================================

#ifndef _NUMBER3D_H_  
#define _NUMBER3D_H_

//======================
//インクルード
//======================
#include "main.h"
#include "billboard.h"
//==========================================

//==========================================

//==========================================
//ナンバー3Dクラス
//==========================================
class CNumber3D : public CBillboard
{
public:

	CNumber3D(int nPri = 0, bool bUseintPri = false, CObject::TYPE type = CObject::TYPE::NUMBER3D, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_BILLBOARD);//コンストラクタ
	~CNumber3D() override;                                                                                  //デストラクタ
	HRESULT Init() override;                                                                                //初期化処理
	void Uninit() override;                                                                                 //終了処理
	void Update() override;                                                                                 //更新処理
	void Draw() override;                                                                                   //描画処理
	void ExtraDraw();                                                                                       //別枠の描画処理
	void SetDeath() override;                                                                               //死亡フラグを設定
	static CNumber3D * Create(float fWidth, float fHeight, D3DXVECTOR3 pos, D3DXVECTOR3 move,D3DXCOLOR col);//生成処理
private:
};

#endif

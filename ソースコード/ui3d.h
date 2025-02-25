//==================================================================================
//
//１０月２８日：狙った方向に弾を発射する[ui3d.h]
//Author:ShinaTaiyo
//
//===================================================================================
#ifndef _UI3D_H_
#define _UI3D_H_

//===================================================================================
//インクルード
//===================================================================================
#include "main.h"
#include "billboard.h"
//===================================================================================

//===================================================================================
//UI3Dクラス
//===================================================================================
class CUi3D : public CBillboard
{
public:
	enum class UI3DTYPE
	{//3DUIの種類
		LOCKON = 0,
		MAX
	};

	CUi3D(int nPri = 0, bool bUseintPri = false, CObject::TYPE type = CObject::TYPE::UI3D, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_BILLBOARD);//コンストラクタ
	~CUi3D();                                                                                                  //デストラクタ
	HRESULT Init(void) override;                                                                               //初期化処理
	void Uninit(void) override;                                                                                //終了処理
	void Update(void) override;                                                                                //更新処理
	void Draw(void) override;                                                                                  //描画処理
	void SetDeath() override;                                                                                  //死亡フラグ設定処理
	static CUi3D* Create(UI3DTYPE type,D3DXVECTOR3 Pos,D3DXVECTOR3 Move,int nLife,float fWidth,float fHeight); //生成処理
protected:
	static const string m_UI3D_FILENAME[static_cast<int>(UI3DTYPE::MAX)];                                      //3DUIのファイル名
	void SetUi3DType(UI3DTYPE type) { m_Type = type; }                                                         //種類を設定
	UI3DTYPE GetUi3DType() { return m_Type; }                                                                  //種類を取得
private:
	UI3DTYPE m_Type;
};
//===================================================================================


#endif
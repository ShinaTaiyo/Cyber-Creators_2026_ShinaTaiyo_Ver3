//=====================================================================================================
//
//１１月１９日：3D背景を描画する[bg3d.h]
//Author:ShinaTaiyo
//
//=====================================================================================================

#ifndef _BG3D_H_
#define _BG3D_H_


//====================================================
//インクルード
//====================================================
#include "main.h"
#include "objectX.h"
//=====================================================================================================

//====================================================
//3D背景クラス
//====================================================
class CBg3D : public CObjectX
{
public:
	enum class BG3DTYPE
	{//背景の種類
	    SKY = 0,
		MAX
	};

	CBg3D(int nPri = 0, bool bUseintPri = false, CObject::TYPE type = CObject::TYPE::BG3D, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_X);//コンストラクタ
	~CBg3D();                                                                                //デストラクタ
	HRESULT Init() override;                                                                 //初期化処理
	void Uninit() override;                                                                  //終了処理
	void Update() override;                                                                  //更新処理
	void Draw() override;                                                                    //描画処理
	void SetDeath() override;                                                                //死亡フラグ設定処理
	static CBg3D* Create(BG3DTYPE type,D3DXVECTOR3 Pos, D3DXVECTOR3 Rot,D3DXVECTOR3 Scale);  //生成処理
protected:
	static const string s_BG3D_FILENAME[static_cast<int>(BG3DTYPE::MAX)];//モデルパス
	void SetBg3DType(BG3DTYPE type) { m_Type = type; }//3D背景の種類を設定する
	const BG3DTYPE GetType() const{ return m_Type; }  //3D背景の種類を取得する
private:
	BG3DTYPE m_Type;                                  //3D背景の種類
};
//=====================================================================================================


#endif // !
//===========================================================================
//
//７月２９日：階層構造実装[modelparts.h]
//Author:ShinaTaiyo
//
//===========================================================================

//=====================================
//インクルード
//=====================================
#include "main.h"
#include "objectX.h"
//========================================================================

//=====================================
//モデルパーツクラス
//=====================================
class CModelParts : public CObjectX
{
public:
	CModelParts(int nPri = 0, bool bUseintPri = false, CObject::TYPE type = CObject::TYPE::NONE, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_X);    //コンストラクタ
	~CModelParts();                    //デストラクタ
	HRESULT Init() override;           //初期化処理
	void Uninit() override;            //終了処理
	void Update() override;            //更新処理
	void ExtraUpdate();                //別枠の更新処理（オブジェクトの派生だけど、このパーツが持っているオブジェクトの処理が呼ばれたときに呼びたいので、自動的に呼ばれないようにする）
	void Draw() override;              //描画処理
	void ExtraDraw();                  //別枠の描画処理（オブジェクトの派生だけど、このパーツが持っているオブジェクトの処理が呼ばれたときに呼びたいので、自動的に呼ばれないようにする）
	void SetDeath() override;          //死亡フラグ設定処理
	static CModelParts* Create(string String,CObject::TYPE Type);//生成処理

	void SetPartsIdx(int nIdx) { m_nPartsIdx = nIdx; }//パーツ番号を設定する
private:
	int m_nPartsIdx;//インデックス
};
//============================================================================================================

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
	CModelParts(int nPriority = 2);    //コンストラクタ
	~CModelParts();                    //デストラクタ
	HRESULT Init() override;           //初期化処理
	void Uninit() override;            //終了処理
	void SetDeath() override;          //死亡フラグ発動処理
	void Update() override;            //更新処理
	void Draw() override;              //描画処理
	void ExtraDraw();                  //別枠の描画処理
	static CModelParts* Create(char * aString,D3DXVECTOR3 OffsetPos,D3DXVECTOR3 OffSetRot);//生成処理
	void SetParentParts(D3DXMATRIX* pParentParts) { m_pMtxParentParts = pParentParts; }      //モデルパーツの親を設定
	D3DXVECTOR3 GetOffSetSupportPos() { return m_OffSetSupportPos; }                      //最初のオフセットの位置を設定
	D3DXVECTOR3 GetOffSetSupportRot() { return m_OffSetSupportRot; }                      //最初のオフセットの向きを設定
	void SetOffSetPos(D3DXVECTOR3 Pos) { m_OffSetPos = Pos; }                              //オフセットの位置を設定
	void SetOffSetRot(D3DXVECTOR3 Rot) { m_OffSetRot = Rot; }                              //オフセットの向きを設定
	D3DXVECTOR3 GetWorldPos() { return m_WorldPos; }
	void SetParent(D3DXMATRIX* pParent) { m_pMtxParent = pParent; }                             //親モデルを設定
	D3DXVECTOR3& GetOffSetPos() { return m_OffSetPos; }                                    //オフセットの位置を取得
	D3DXVECTOR3& GetOffSetRot() { return m_OffSetRot; }                                    //オフセットの向きを取得
private:
	D3DXVECTOR3 m_WorldPos;                                                                //ワールド座標
	D3DXVECTOR3 m_OffSetPos;                                                               //オフセット位置
	D3DXVECTOR3 m_OffSetSupportPos;                                                        //最初のオフセットの位置
	D3DXVECTOR3 m_OffSetRot;                                                               //オフセット向き
	D3DXVECTOR3 m_OffSetSupportRot;                                                        //最初のオフセットの向き
	D3DXMATRIX* m_pMtxParentParts;                                                           //親モデルパーツへのポインタ
	D3DXMATRIX* m_pMtxParent;                                                                 //親モデルパーツのマトリックス
};
//============================================================================================================

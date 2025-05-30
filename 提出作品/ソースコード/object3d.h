//==================================================================================
//
//６月３日：３Dオブジェクトクラスを作成する[3Dobject.h]
//Author:ShinaTaiyo
//
//===================================================================================
#ifndef _OBJECT3D_H_
#define _OBJECT3D_H_

//===================================================================================
//インクルード
//===================================================================================
#include "main.h"
#include "object.h"

//===================================================================================

//===================================================================================
//3Dオブジェクトクラス
//===================================================================================
class CObject3D : public CObject
{
public:
	CObject3D(int nPri = 0, bool bUseintPri = false, CObject::TYPE type = CObject::TYPE::NONE, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_3D);                                                           //コンストラクタ
	~CObject3D();                                                          //デストラクタ
	HRESULT Init(void) override;                                           //初期化処理
	void Uninit(void) override;                                            //終了処理
	void Update(void) override;                                            //更新処理
	void Draw(void) override;                                              //描画処理
	void SetDeath() override;                                              //死亡フラグ設定処理
	void bindTexture(LPDIRECT3DTEXTURE9 pTexture);                         //テクスチャを割り当てる

	void SetWidth(float fWidth) { m_fWidth = fWidth; }                     //横幅
	void SetHeight(float fHeight) { m_fHeight = fHeight; }                 //高さ
	void SetRot(D3DXVECTOR3 Rot) { m_Rot = Rot; }                          //向き
	void SetPos(D3DXVECTOR3 Pos) { m_Pos = Pos; }                          //位置を設定
	void SetTextureIndex(int nIdx) { m_nTextureIndex = nIdx; }             //テクスチャ番号をセットする
	int GetTextureIndex() { return m_nTextureIndex; }                      //テクスチャ番号を取得する

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;                                    //頂点バッファへのポインタ!
	LPDIRECT3DTEXTURE9 m_pTexture;                                         //テクスチャへのポインタ"!
	D3DXVECTOR3 m_Pos;                                                     //位置!
	D3DXVECTOR3 m_Rot;                                                     //向き!
	D3DXMATRIX m_mtxWorld;                                                 //ワールド変換行列!
	float m_fWidth;                                                        //横幅!
	float m_fHeight;                                                       //高さ!
	int m_nTextureIndex;                                                   //テクスチャ番号（全てのオブジェクト3D派生クラスが持っている）!
};
//===================================================================================


#endif
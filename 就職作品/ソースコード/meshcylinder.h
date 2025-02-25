//========================================================
//
//６月２１日：メッシュシリンダーで背景を出す[meshcylinder.h]
//Author:ShinaTaiyo
//
//=========================================================
#ifndef _MESHCYLINDER_H_
#define _MESHCYLINDER_H_

//========================================
//インクルード
//========================================
#include "main.h"
#include "object.h"
//===========================================================

class CMeshCylinder : public CObject
{
public:
	CMeshCylinder(const float fRadius,const float fHeight,const int nNumDivisionXZ,const int nNumDivisionY,const D3DXVECTOR3 Pos,const D3DXVECTOR3 Rot, int nPri = 0, bool bUseintPri = false, CObject::TYPE type = CObject::TYPE::NONE, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_3D);                               //コンストラクタ
	~CMeshCylinder() override;                                    //デストラクタ
	HRESULT Init() override;                                      //初期化処理
	void Uninit() override;                                       //終了処理
	void Update() override;                                       //更新処理
	void Draw() override;                                         //描画処理
	void SetDeath() override;                                     //死亡フラグ設定処理
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);                //テクスチャを割り当てる
	void SetTextureIndex(int nIdx) { m_nTextureIndex = nIdx; }    //テクスチャ番号を割り当てる4
	int GetTextureIndex() { return m_nTextureIndex; }             //テクスチャ番号を取得する

	//位置
	void SetPos(D3DXVECTOR3 Pos) { m_Pos = Pos; }
	const D3DXVECTOR3 & GetPos() const { return m_Pos; }

	//向き
	void SetRot(D3DXVECTOR3 Rot) { m_Rot = Rot; }
	const D3DXVECTOR3& GetRot() const { return m_Rot; }

	//高さ
	void SetHeight(float fHeight) { m_fHeight = fHeight; }
	const float& GetHeight() const { return m_fHeight; }              //高さ

	//描画するかどうか
	void SetUseDraw(bool bUse) { m_bUseDraw = bUse; }
	const bool& GetUseDraw() const { return m_bUseDraw; }

	//メッシュ情報
	LPDIRECT3DVERTEXBUFFER9 GetVtxBufferPointer() { return m_pVtxBuff; }//頂点バッファを取得する
	LPDIRECT3DINDEXBUFFER9 GetIdxBufferPointer() { return m_pIdxBuff; } //インデックスバッファを取得する
	LPDIRECT3DTEXTURE9 GetTexture() { return m_pTexture; }              //テクスチャを取得する

	//マトリックスワールド系
	void SetMtxChild(D3DXMATRIX* pMtxChild) { m_pMtxChild = pMtxChild; }	//子マトリックスのポインタを設定する
	D3DXMATRIX& GetMtxWorld() { return m_mtxWorld; }                        //マトリックスワールドを取得

	const int & GetNumDivisionXZ() const { return m_nNumDivisionXZ; } //XZ方向の分割数の取得
	const int & GetNumDivisionY() const { return m_nNumDivisionY; }   //Y方向の分割数の取得
	const int & GetNumVtx() const { return m_nNumVtx; }               //頂点数の取得
	const int & GetNumIdx() const { return m_nNumIdx; }               //インデックス数の取得
	const int & GetNumPolygon() const { return m_nNumPolygon; }       //ポリゴン数の取得
	const float& GetRadius() const { return m_fRadius; }              //半径を取得

	//それぞれのYの中心点を取得
	const D3DXVECTOR3 & GetSenterPos(const int nArray) const
	{
		if (nArray < 0 || nArray >= m_nNumDivisionY)
		{
			assert(false);
		}
		return m_pSenterPos[nArray];
	}//中心点の先頭ポインタを取得
	void SetSenterPos(int nArray, D3DXVECTOR3 Pos)
	{
		if (nArray < 0 || nArray >= m_nNumDivisionY)
		{
			assert(false);
		}
		m_pSenterPos[nArray] = Pos;
	}//それぞれの中心点を設定する

private:
	PDIRECT3DVERTEXBUFFER9 m_pVtxBuff;   //頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;   //インデックスバッファへのポインタ
	LPDIRECT3DTEXTURE9 m_pTexture;       //テクスチャへのポインタ

	//大きさ（動的に変化する可能性がある）
	float m_fRadius;                     //半径
	float m_fHeight;                     //高さ

	//分割数
	const int m_nNumDivisionXZ;          //XZ方向分割数
	const int m_nNumDivisionY;           //Y方向分割数

	//構成要素数
	int m_nNumPolygon;                   //ポリゴン数
	const int m_nNumVtx;                 //頂点数
	const int m_nNumIdx;                 //インデックス数

	//ワールド情報
	D3DXVECTOR3 m_Pos;                   //位置
	D3DXVECTOR3 m_Rot;                   //向き
	D3DXMATRIX m_mtxWorld;               //ワールドマトリックス
	D3DXMATRIX* m_pMtxChild;             //子マトリックス

	//テクスチャ
	int m_nTextureIndex;                 //テクスチャ番号を設定する

	//頂点確認用
	int m_nCheckVtx;
	int m_nCheckIdx;

	//それぞれの中心点
	D3DXVECTOR3* m_pSenterPos;

	bool m_bUseDraw;                     //描画するかどうか

	//=============================================
	//関数
	//=============================================
	void SetVertexInfo(VERTEX_3D* pVtx,float & fRatioXZ,float & fRatioY,int &nCntVtxXZ,int & nCntVtxY); //頂点情報を設定する
	void ChengeNumPolygon();                                                                            //ポリゴン数を変える処理
	void CheckMeshInfo();                                                                               //メッシュのそれぞれの情報を確認する
}; 
#endif


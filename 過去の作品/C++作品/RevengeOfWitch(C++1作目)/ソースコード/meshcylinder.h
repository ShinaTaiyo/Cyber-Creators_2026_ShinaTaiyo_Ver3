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
	CMeshCylinder();                               //コンストラクタ
	~CMeshCylinder() override;                     //デストラクタ
	HRESULT Init() override;                       //初期化処理
	void Uninit() override;                        //終了処理
	void Update() override;                        //更新処理
	void Draw() override;                          //描画処理
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture); //テクスチャを割り当てる
	void SetCylinderInfo(D3DXVECTOR3 Pos, D3DXVECTOR3 Rot, int nNumBlockXZ, int nNumBlockY, float fRadius, float fHeight);
	void SetTextureIndex(int nIdx) { m_nTextureIndex = nIdx; }//テクスチャ番号を割り当てる4
	int GetTextureIndex() { return m_nTextureIndex; }         //テクスチャ番号を取得する
private:
	typedef struct
	{
		int nNumBlockXZ;                   //X方向のブロック数
		int nNumBlockY;                    //Y方向のブロック数
		int nNumVtxXZ;                   //XZ方向分割数
		int nDivisionY;                    //Y方向分割数
		float fRadius;                     //半径
		float fHeight;                     //高さ
		int nNumVtx;                       //頂点数
		int nNumIdx;                       //インデックス数
		int nNumPolygon;                   //ポリゴン数
		PDIRECT3DVERTEXBUFFER9 pVtxBuff;   //頂点バッファへのポインタ
		LPDIRECT3DINDEXBUFFER9 pIdxBuff;   //インデックスバッファへのポインタ
		LPDIRECT3DTEXTURE9 pTexture;       //テクスチャへのポインタ
	}Info;

	D3DXVECTOR3 m_Pos;                   //位置
	D3DXVECTOR3 m_Rot;                   //向き
	D3DXMATRIX m_mtxWorld;               //ワールドマトリックス
	Info m_Info;                         //ポリゴン情報
	int m_nTextureIndex;                 //テクスチャ番号を設定する

}; 
#endif


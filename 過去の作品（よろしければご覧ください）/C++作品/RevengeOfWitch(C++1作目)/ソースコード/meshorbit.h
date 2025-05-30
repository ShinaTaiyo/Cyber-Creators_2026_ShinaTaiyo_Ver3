//========================================================
//
//８月２３日：軌跡を出す[meshorbit.h]
//Author:ShinaTaiyo
//
//=========================================================
#ifndef _MESHORBIT_H_
#define _MESHORBIT_H_

//========================================
//インクルード
//========================================
#include "main.h"
#include "object.h"
//===========================================================

class CMeshOrbit : public CObject
{
public:
	typedef enum
	{
		MESHORBITTYPE00_NORMAL = 0,//普通のテクスチャ
		MESHORBITTYPE_MAX
	}MESHORBITTYPE;

	CMeshOrbit();                                  //コンストラクタ
	~CMeshOrbit() override;                        //デストラクタ
	HRESULT Init() override;                       //初期化処理
	void Uninit() override;                        //終了処理
	void Update() override;                        //更新処理
	void Draw() override;                          //描画処理
	void SetDeath() override;                      //死亡フラグ設定処理
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture); //テクスチャを割り当てる
	void SetTextureIndex(int nIdx) { m_nTextureIndex = nIdx; }//テクスチャ番号を割り当てる
	static CMeshOrbit* Create(MESHORBITTYPE type);                              //生成処理
	int GetTextureIndex() { return m_nTextureIndex; }         //テクスチャ番号を取得する

	//===========================================
	//軌跡関係
	//===========================================
	void SetPos(D3DXVECTOR3 Pos) { m_Pos = Pos; }             //位置を設定
	D3DXVECTOR3& GetPos() { return m_Pos; }                   //位置を取得する
	void SetOffSetPos(D3DXVECTOR3 OffSetPos) { m_OffSetPos = OffSetPos; }//オフセット位置を設定する
	D3DXVECTOR3& GetOffSetPos() { return m_OffSetPos; }                  //オフセット位置を取得する

	void SetRot(D3DXVECTOR3 Rot) { m_Rot = Rot; }                        //向きを設定する
	//==================================================================================================================

private:
	static const int m_nNUMBLOCK_X = 30;                                                      //X方向のブロック数
	static const int m_nNUMBLOCK_Z = 1;                                                       //Z方向のブロック数
	static const int m_nTOTAL_BLOCK = m_nNUMBLOCK_X * m_nNUMBLOCK_Z;                          //合計ブロック数
																			                  
	static const int m_nNUMVERTEX_X = m_nNUMBLOCK_X + 1;                                      //X方向の頂点数
	static const int m_nNUMVERTEX_Z = m_nNUMBLOCK_Z + 1;                                      //Y方向の頂点数
																			                  
	static const int m_nTOTAL_VERTEX = m_nNUMVERTEX_X * m_nNUMVERTEX_Z;                       //合計頂点数

	static const int m_nTOTAL_POLYGON = m_nTOTAL_BLOCK * 2 + 4 * (m_nNUMBLOCK_Z - 1);         //合計ポリゴン数（合計ブロック数×２＋４×縮退計算数）

	static const int m_nTOTAL_INDEX = m_nTOTAL_POLYGON + 2;                                      //合計インデックス数

	PDIRECT3DVERTEXBUFFER9 m_pVtxBuff;   //頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;   //インデックスバッファへのポインタ

	LPDIRECT3DTEXTURE9 m_pTexture;       //テクスチャへのポインタ
	D3DXVECTOR3 m_Pos;                   //位置
	D3DXVECTOR3 m_OffSetPos;             //オフセット位置
	D3DXVECTOR3 m_Rot;                   //向き
	D3DXMATRIX m_mtxWorld;               //ワールドマトリックス

	int m_nTextureIndex;                 //テクスチャ番号を設定する

	MESHORBITTYPE m_type;                //種類

	static const char* m_apMESHORBIT_FILENAME[TYPE_MAX];

};
#endif


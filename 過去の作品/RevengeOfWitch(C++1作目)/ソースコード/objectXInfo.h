//=======================================================================================
//
//６月７日：オブジェクトX管理クラスを実装する[objectXInfo.h]
//Author:ShinaTaiyo
//
//=======================================================================================

#ifndef _OBJECTXINFO_H_  
#define _OBJECTXINFO_H_

////=============================================
//インクルード
//=============================================
#include "main.h"
#include "object.h"
#include "objectX.h"
//=======================================================================================

//=============================================
//オブジェクトX管理クラス
//=============================================
class CObjectXInfo
{
private:			
	static const int m_nMAX_MAT = 100;                               //マテリアルの最大数
	static const int m_nMAXWORD = 120;                               //文字の最大数
	static const int m_nMAX_INFO = 200;                               //一つ当たりのオブジェクト情報の最大数
	static int m_nNumAll;                                            //オブジェクトX総数
	//=================================							     
    //モデル情報構造体											     
    //=================================							     
	typedef struct												     
	{															     
		LPD3DXMESH pMesh;                                            //メッシュ（頂点情報）へのポインタ（モデルの種類数分）
		LPD3DXBUFFER pBuffMat;                                       //マテリアルへのポインタ
		DWORD dwNumMat;                                              //マテリアルの数
		LPDIRECT3DTEXTURE9 pTexture[m_nMAX_MAT];                     //テクスチャへのポインタ
		D3DCOLORVALUE Diffuse[m_nMAX_MAT];                          //色合い
		char aFileName[m_nMAXWORD];                                  //ファイル名
	}Modelnfo;
	//============================================================================================================
	Modelnfo m_apObjectXList[m_nMAX_INFO];     //オブジェクトXリスト
public:
	CObjectXInfo();                                                  //コンストラクタ
	~CObjectXInfo();                                                 //デストラクタ
	void Unload();                                                   //オブジェクトXの情報を破棄
	int Regist(CObjectX::OBJECTXTYPE type, const char* pTextureName);          //オブジェクトX登録

	LPD3DXMESH GetMesh(CObjectX::OBJECTXTYPE type,int nIdx);                   //オブジェクトX取得
	LPD3DXBUFFER GetBuffMat(CObjectX::OBJECTXTYPE type, int nIdx);             //マテリアルへのポインタを取得
    DWORD GetdwNumMat(CObjectX::OBJECTXTYPE type, int nIdx);                   //マテリアルへのポインタを取得
	LPDIRECT3DTEXTURE9 * GetTexture(CObjectX::OBJECTXTYPE type, int nIdx);     //マテリアルへのポインタを取得
	Modelnfo GetModelInfo(int nIdx) { return m_apObjectXList[nIdx]; }
	D3DCOLORVALUE* GetColorValue(CObjectX::OBJECTXTYPE type, int nIdx);        //色合いを取得する
};

#endif

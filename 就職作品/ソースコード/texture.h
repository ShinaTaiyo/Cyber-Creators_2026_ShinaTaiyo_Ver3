//=======================================================================================
//
//６月６日：テクスチャ管理クラスを作成する[texture.h]
//Author:ShinaTaiyo
//
//=======================================================================================

#ifndef _TEXTURE_H_  
#define _TEXTURE_H_

//=============================================
//インクルード
//=============================================
#include "main.h"
#include "object.h"
//=======================================================================================

//=============================================
//テクスチャ管理クラス
//=============================================
class CTexture
{
public:

	CTexture();                              //コンストラクタ
	~CTexture();                             //デストラクタ
	void Unload();//テクスチャ破棄
	int Regist(std::string pTextureName);    //テクスチャ登録
	LPDIRECT3DTEXTURE9 GetAddress(int nIdx); //テクスチャ取得
private:

	static const int m_nMAXTEXTURE = 100;                                 //テクスチャ最大数
	static const int m_nMAX_WORD = 120;                                   //文字列最大数
	static int m_nNumAll;                                                 //テクスチャ総数

	LPDIRECT3DTEXTURE9 m_pTextureList[m_nMAXTEXTURE];                     //テクスチャ!
	std::string m_aTextureName[m_nMAXTEXTURE];                                 //作成済みテクスチャの名前!
};

#endif

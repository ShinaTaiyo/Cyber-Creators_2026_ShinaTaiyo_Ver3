//=============================================================================================
//
//６月９日：Xオブジェクト情報管理クラスを実装する[objectXInfo.cpp]
//Author:ShinaTaiyo
//
//=============================================================================================

//===============================================
//インクルード
//===============================================
#include "objectXInfo.h"
#include "manager.h"
#include "renderer.h"
#include <iostream>
//==============================================================================================

//===============================================
//静的メンバ初期化
//===============================================
int CObjectXInfo::m_nNumAll = 0;//オブジェクトX情報の総数

//===============================================
//コンストラクタ
//===============================================
CObjectXInfo::CObjectXInfo() : m_Type(CObjectX::OBJECTXTYPE_BLOCK),m_apObjectXList()
{
	for (int nCntInfo = 0; nCntInfo < m_nMAX_INFO; nCntInfo++)
	{
		m_apObjectXList[nCntInfo].pBuffMat = {};              //マテリアルへのポインタ
		m_apObjectXList[nCntInfo].dwNumMat = {};              //マテリアルの数
		m_apObjectXList[nCntInfo].pMesh = {};                 //頂点情報へのポインタ
        for (int nCntMat = 0; nCntMat < m_nMAX_MAT; nCntMat++)
        {
            m_apObjectXList[nCntInfo].pTexture[nCntMat] = {}; //テクスチャへのポインタ
            m_apObjectXList[nCntInfo].Diffuse[nCntMat] = {};  //色合い
        }
		m_apObjectXList[nCntInfo].aFileName = {};//文字列の初期化
	}
}
//==============================================================================================

//===============================================
//デストラクタ
//===============================================
CObjectXInfo::~CObjectXInfo()
{

}
//==============================================================================================

//===============================================
//オブジェクトX情報を破棄する
//===============================================
void CObjectXInfo::Unload()
{

    for (int nCntInfo = 0; nCntInfo < m_nMAX_INFO; nCntInfo++)
    {
        if (m_apObjectXList[nCntInfo].pMesh != nullptr)
        {//頂点情報の破棄
            m_apObjectXList[nCntInfo].pMesh->Release();
            m_apObjectXList[nCntInfo].pMesh = nullptr;
        }

        if (m_apObjectXList[nCntInfo].pBuffMat != nullptr)
        {//マテリアル情報の破棄
            m_apObjectXList[nCntInfo].pBuffMat->Release();
            m_apObjectXList[nCntInfo].pBuffMat = nullptr;
        }

        for (int nCntMat = 0; nCntMat < m_nMAX_MAT; nCntMat++)
        {//テクスチャ情報の破棄
            if (m_apObjectXList[nCntInfo].pTexture[nCntMat] != nullptr)
            {
                m_apObjectXList[nCntInfo].pTexture[nCntMat]->Release();
                m_apObjectXList[nCntInfo].pTexture[nCntMat] = nullptr;
            }
        }
    }
}
//==============================================================================================

//===============================================
//オブジェクトX登録処理
//===============================================
int CObjectXInfo::Regist(std::string pTextureName)
{
	int nIdx = 0;//インデックス
    D3DCOLORVALUE Diffuse[m_nMAX_MAT] = {};
	for (int nCntObjX = 0; nCntObjX < m_nMAX_INFO; nCntObjX++)
	{
        if (m_apObjectXList[nCntObjX].pMesh == nullptr)
        {
            LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice(); //デバイスの取得
            D3DXMATERIAL* pMat;                                               //マテリアルへのポインタ

            //============================
            //モデル情報を取得
            //============================
            D3DXLoadMeshFromX(pTextureName.c_str(),
                D3DXMESH_SYSTEMMEM,
                pDevice,
                NULL,
                &m_apObjectXList[nCntObjX].pBuffMat,
                NULL,
                &m_apObjectXList[nCntObjX].dwNumMat,
                &m_apObjectXList[nCntObjX].pMesh);

            m_apObjectXList[nCntObjX].aFileName = pTextureName;//ファイルパスを保存する

            //================================================================================================================

            //==========================================
            //テクスチャの読み込み
            //==========================================	

            //マテリアルへのポインタを取得
            pMat = (D3DXMATERIAL*)m_apObjectXList[nCntObjX].pBuffMat->GetBufferPointer();

            for (int nCntMat = 0; nCntMat < (int)m_apObjectXList[nCntObjX].dwNumMat; nCntMat++)
            {
                m_apObjectXList[nCntObjX].Diffuse[nCntMat] = pMat[nCntMat].MatD3D.Diffuse;//マテリアルの色合いを格納する

                if (pMat[nCntMat].pTextureFilename != nullptr)
                {//テクスチャファイルが存在する
                    D3DXCreateTextureFromFile(pDevice,
                        pMat[nCntMat].pTextureFilename,
                        &m_apObjectXList[nCntObjX].pTexture[nCntMat]);
                }
            }

            //================================================================================================================

            nIdx = nCntObjX;//インデックスを保存
            break;
        }
        else if (m_apObjectXList[nCntObjX].aFileName == pTextureName)
        {//ファイル名が既にあるものと一致したら
            nIdx = nCntObjX;
            break;
        }
	}
	return nIdx;
}

//=========================================
//頂点情報を取得
//=========================================
LPD3DXMESH CObjectXInfo::GetMesh(int nIdx)
{
    if (m_apObjectXList[nIdx].pMesh != nullptr)
    {
        return m_apObjectXList[nIdx].pMesh;
    }

    return nullptr;
}
//========================================================================================

//=========================================
//マテリアルへのポインタを取得
//=========================================
LPD3DXBUFFER CObjectXInfo::GetBuffMat(int nIdx)
{
    if (m_apObjectXList[nIdx].pBuffMat != nullptr)
    {
        return m_apObjectXList[nIdx].pBuffMat;
    }
    return nullptr;

}
//========================================================================================

//=========================================
//マテリアルの数を取得
//=========================================
DWORD CObjectXInfo::GetdwNumMat(int nIdx)
{
    if (m_apObjectXList[nIdx].dwNumMat != NULL)
    {
        return m_apObjectXList[nIdx].dwNumMat;
    }
    return 0;
}
//========================================================================================

//=========================================
//テクスチャへのポインタを取得
//=========================================
LPDIRECT3DTEXTURE9 * CObjectXInfo::GetTexture(int nIdx)
{
    if (m_apObjectXList[nIdx].pTexture != nullptr)
    {
        return &m_apObjectXList[nIdx].pTexture[0];
    }
    return nullptr;
}
//========================================================================================

//==========================================
//色合いを取得する
//==========================================
D3DCOLORVALUE* CObjectXInfo::GetColorValue(int nIdx)
{
    if (m_apObjectXList[nIdx].Diffuse != nullptr)
    {
        return &m_apObjectXList[nIdx].Diffuse[0];
    }
    return nullptr;
}
//========================================================================================
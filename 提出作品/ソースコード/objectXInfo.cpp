//=============================================================================================
//
//�U���X���FX�I�u�W�F�N�g���Ǘ��N���X����������[objectXInfo.cpp]
//Author:ShinaTaiyo
//
//=============================================================================================

//===============================================
//�C���N���[�h
//===============================================
#include "objectXInfo.h"
#include "manager.h"
#include "renderer.h"
#include <iostream>
//==============================================================================================

//===============================================
//�ÓI�����o������
//===============================================
int CObjectXInfo::m_nNumAll = 0;//�I�u�W�F�N�gX���̑���

//===============================================
//�R���X�g���N�^
//===============================================
CObjectXInfo::CObjectXInfo() : m_Type(CObjectX::OBJECTXTYPE_BLOCK),m_apObjectXList()
{
	for (int nCntInfo = 0; nCntInfo < m_nMAX_INFO; nCntInfo++)
	{
		m_apObjectXList[nCntInfo].pBuffMat = {};              //�}�e���A���ւ̃|�C���^
		m_apObjectXList[nCntInfo].dwNumMat = {};              //�}�e���A���̐�
		m_apObjectXList[nCntInfo].pMesh = {};                 //���_���ւ̃|�C���^
        for (int nCntMat = 0; nCntMat < m_nMAX_MAT; nCntMat++)
        {
            m_apObjectXList[nCntInfo].pTexture[nCntMat] = {}; //�e�N�X�`���ւ̃|�C���^
            m_apObjectXList[nCntInfo].Diffuse[nCntMat] = {};  //�F����
        }
		m_apObjectXList[nCntInfo].aFileName = {};//������̏�����
	}
}
//==============================================================================================

//===============================================
//�f�X�g���N�^
//===============================================
CObjectXInfo::~CObjectXInfo()
{

}
//==============================================================================================

//===============================================
//�I�u�W�F�N�gX����j������
//===============================================
void CObjectXInfo::Unload()
{

    for (int nCntInfo = 0; nCntInfo < m_nMAX_INFO; nCntInfo++)
    {
        if (m_apObjectXList[nCntInfo].pMesh != nullptr)
        {//���_���̔j��
            m_apObjectXList[nCntInfo].pMesh->Release();
            m_apObjectXList[nCntInfo].pMesh = nullptr;
        }

        if (m_apObjectXList[nCntInfo].pBuffMat != nullptr)
        {//�}�e���A�����̔j��
            m_apObjectXList[nCntInfo].pBuffMat->Release();
            m_apObjectXList[nCntInfo].pBuffMat = nullptr;
        }

        for (int nCntMat = 0; nCntMat < m_nMAX_MAT; nCntMat++)
        {//�e�N�X�`�����̔j��
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
//�I�u�W�F�N�gX�o�^����
//===============================================
int CObjectXInfo::Regist(std::string pTextureName)
{
	int nIdx = 0;//�C���f�b�N�X
    D3DCOLORVALUE Diffuse[m_nMAX_MAT] = {};
	for (int nCntObjX = 0; nCntObjX < m_nMAX_INFO; nCntObjX++)
	{
        if (m_apObjectXList[nCntObjX].pMesh == nullptr)
        {
            LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice(); //�f�o�C�X�̎擾
            D3DXMATERIAL* pMat;                                               //�}�e���A���ւ̃|�C���^

            //============================
            //���f�������擾
            //============================
            D3DXLoadMeshFromX(pTextureName.c_str(),
                D3DXMESH_SYSTEMMEM,
                pDevice,
                NULL,
                &m_apObjectXList[nCntObjX].pBuffMat,
                NULL,
                &m_apObjectXList[nCntObjX].dwNumMat,
                &m_apObjectXList[nCntObjX].pMesh);

            m_apObjectXList[nCntObjX].aFileName = pTextureName;//�t�@�C���p�X��ۑ�����

            //================================================================================================================

            //==========================================
            //�e�N�X�`���̓ǂݍ���
            //==========================================	

            //�}�e���A���ւ̃|�C���^���擾
            pMat = (D3DXMATERIAL*)m_apObjectXList[nCntObjX].pBuffMat->GetBufferPointer();

            for (int nCntMat = 0; nCntMat < (int)m_apObjectXList[nCntObjX].dwNumMat; nCntMat++)
            {
                m_apObjectXList[nCntObjX].Diffuse[nCntMat] = pMat[nCntMat].MatD3D.Diffuse;//�}�e���A���̐F�������i�[����

                if (pMat[nCntMat].pTextureFilename != nullptr)
                {//�e�N�X�`���t�@�C�������݂���
                    D3DXCreateTextureFromFile(pDevice,
                        pMat[nCntMat].pTextureFilename,
                        &m_apObjectXList[nCntObjX].pTexture[nCntMat]);
                }
            }

            //================================================================================================================

            nIdx = nCntObjX;//�C���f�b�N�X��ۑ�
            break;
        }
        else if (m_apObjectXList[nCntObjX].aFileName == pTextureName)
        {//�t�@�C���������ɂ�����̂ƈ�v������
            nIdx = nCntObjX;
            break;
        }
	}
	return nIdx;
}

//=========================================
//���_�����擾
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
//�}�e���A���ւ̃|�C���^���擾
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
//�}�e���A���̐����擾
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
//�e�N�X�`���ւ̃|�C���^���擾
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
//�F�������擾����
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
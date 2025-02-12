//=======================================================================================
//
//�U���V���F�I�u�W�F�N�gX�Ǘ��N���X����������[objectXInfo.h]
//Author:ShinaTaiyo
//
//=======================================================================================

#ifndef _OBJECTXINFO_H_  
#define _OBJECTXINFO_H_

////=============================================
//�C���N���[�h
//=============================================
#include "main.h"
#include "object.h"
#include "objectX.h"
//=======================================================================================

//=============================================
//�I�u�W�F�N�gX�Ǘ��N���X
//=============================================
class CObjectXInfo
{
private:			
	static const int m_nMAX_MAT = 100;                               //�}�e���A���̍ő吔
	static const int m_nMAXWORD = 120;                               //�����̍ő吔
	static const int m_nMAX_INFO = 200;                               //�������̃I�u�W�F�N�g���̍ő吔
	static int m_nNumAll;                                            //�I�u�W�F�N�gX����
	//=================================							     
    //���f�����\����											     
    //=================================							     
	typedef struct												     
	{															     
		LPD3DXMESH pMesh;                                            //���b�V���i���_���j�ւ̃|�C���^�i���f���̎�ސ����j
		LPD3DXBUFFER pBuffMat;                                       //�}�e���A���ւ̃|�C���^
		DWORD dwNumMat;                                              //�}�e���A���̐�
		LPDIRECT3DTEXTURE9 pTexture[m_nMAX_MAT];                     //�e�N�X�`���ւ̃|�C���^
		D3DCOLORVALUE Diffuse[m_nMAX_MAT];                          //�F����
		char aFileName[m_nMAXWORD];                                  //�t�@�C����
	}Modelnfo;
	//============================================================================================================
	Modelnfo m_apObjectXList[m_nMAX_INFO];     //�I�u�W�F�N�gX���X�g
public:
	CObjectXInfo();                                                  //�R���X�g���N�^
	~CObjectXInfo();                                                 //�f�X�g���N�^
	void Unload();                                                   //�I�u�W�F�N�gX�̏���j��
	int Regist(CObjectX::OBJECTXTYPE type, const char* pTextureName);          //�I�u�W�F�N�gX�o�^

	LPD3DXMESH GetMesh(CObjectX::OBJECTXTYPE type,int nIdx);                   //�I�u�W�F�N�gX�擾
	LPD3DXBUFFER GetBuffMat(CObjectX::OBJECTXTYPE type, int nIdx);             //�}�e���A���ւ̃|�C���^���擾
    DWORD GetdwNumMat(CObjectX::OBJECTXTYPE type, int nIdx);                   //�}�e���A���ւ̃|�C���^���擾
	LPDIRECT3DTEXTURE9 * GetTexture(CObjectX::OBJECTXTYPE type, int nIdx);     //�}�e���A���ւ̃|�C���^���擾
	Modelnfo GetModelInfo(int nIdx) { return m_apObjectXList[nIdx]; }
	D3DCOLORVALUE* GetColorValue(CObjectX::OBJECTXTYPE type, int nIdx);        //�F�������擾����
};

#endif

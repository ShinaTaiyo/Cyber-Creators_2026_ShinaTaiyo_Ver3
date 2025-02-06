//=======================================================================================
//
//�U���U���F�e�N�X�`���Ǘ��N���X���쐬����[texture.h]
//Author:ShinaTaiyo
//
//=======================================================================================

#ifndef _TEXTURE_H_  
#define _TEXTURE_H_

//=============================================
//�C���N���[�h
//=============================================
#include "main.h"
#include "object.h"
//=======================================================================================

//=============================================
//�e�N�X�`���Ǘ��N���X
//=============================================
class CTexture
{
public:

	CTexture();                              //�R���X�g���N�^
	~CTexture();                             //�f�X�g���N�^
	void Unload();//�e�N�X�`���j��
	int Regist(std::string pTextureName);    //�e�N�X�`���o�^
	LPDIRECT3DTEXTURE9 GetAddress(int nIdx); //�e�N�X�`���擾
private:

	static const int m_nMAXTEXTURE = 100;                                 //�e�N�X�`���ő吔
	static const int m_nMAX_WORD = 120;                                   //������ő吔
	static int m_nNumAll;                                                 //�e�N�X�`������

	LPDIRECT3DTEXTURE9 m_pTextureList[m_nMAXTEXTURE];                     //�e�N�X�`��!
	std::string m_aTextureName[m_nMAXTEXTURE];                                 //�쐬�ς݃e�N�X�`���̖��O!
};

#endif

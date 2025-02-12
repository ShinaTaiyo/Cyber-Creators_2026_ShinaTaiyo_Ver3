//========================================================
//
//�W���Q�R���F�O�Ղ��o��[meshorbit.h]
//Author:ShinaTaiyo
//
//=========================================================
#ifndef _MESHORBIT_H_
#define _MESHORBIT_H_

//========================================
//�C���N���[�h
//========================================
#include "main.h"
#include "object.h"
//===========================================================

class CMeshOrbit : public CObject
{
public:
	typedef enum
	{
		MESHORBITTYPE00_NORMAL = 0,//���ʂ̃e�N�X�`��
		MESHORBITTYPE_MAX
	}MESHORBITTYPE;

	CMeshOrbit();                                  //�R���X�g���N�^
	~CMeshOrbit() override;                        //�f�X�g���N�^
	HRESULT Init() override;                       //����������
	void Uninit() override;                        //�I������
	void Update() override;                        //�X�V����
	void Draw() override;                          //�`�揈��
	void SetDeath() override;                      //���S�t���O�ݒ菈��
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture); //�e�N�X�`�������蓖�Ă�
	void SetTextureIndex(int nIdx) { m_nTextureIndex = nIdx; }//�e�N�X�`���ԍ������蓖�Ă�
	static CMeshOrbit* Create(MESHORBITTYPE type);                              //��������
	int GetTextureIndex() { return m_nTextureIndex; }         //�e�N�X�`���ԍ����擾����

	//===========================================
	//�O�Պ֌W
	//===========================================
	void SetPos(D3DXVECTOR3 Pos) { m_Pos = Pos; }             //�ʒu��ݒ�
	D3DXVECTOR3& GetPos() { return m_Pos; }                   //�ʒu���擾����
	void SetOffSetPos(D3DXVECTOR3 OffSetPos) { m_OffSetPos = OffSetPos; }//�I�t�Z�b�g�ʒu��ݒ肷��
	D3DXVECTOR3& GetOffSetPos() { return m_OffSetPos; }                  //�I�t�Z�b�g�ʒu���擾����

	void SetRot(D3DXVECTOR3 Rot) { m_Rot = Rot; }                        //������ݒ肷��
	//==================================================================================================================

private:
	static const int m_nNUMBLOCK_X = 30;                                                      //X�����̃u���b�N��
	static const int m_nNUMBLOCK_Z = 1;                                                       //Z�����̃u���b�N��
	static const int m_nTOTAL_BLOCK = m_nNUMBLOCK_X * m_nNUMBLOCK_Z;                          //���v�u���b�N��
																			                  
	static const int m_nNUMVERTEX_X = m_nNUMBLOCK_X + 1;                                      //X�����̒��_��
	static const int m_nNUMVERTEX_Z = m_nNUMBLOCK_Z + 1;                                      //Y�����̒��_��
																			                  
	static const int m_nTOTAL_VERTEX = m_nNUMVERTEX_X * m_nNUMVERTEX_Z;                       //���v���_��

	static const int m_nTOTAL_POLYGON = m_nTOTAL_BLOCK * 2 + 4 * (m_nNUMBLOCK_Z - 1);         //���v�|���S�����i���v�u���b�N���~�Q�{�S�~�k�ތv�Z���j

	static const int m_nTOTAL_INDEX = m_nTOTAL_POLYGON + 2;                                      //���v�C���f�b�N�X��

	PDIRECT3DVERTEXBUFFER9 m_pVtxBuff;   //���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;   //�C���f�b�N�X�o�b�t�@�ւ̃|�C���^

	LPDIRECT3DTEXTURE9 m_pTexture;       //�e�N�X�`���ւ̃|�C���^
	D3DXVECTOR3 m_Pos;                   //�ʒu
	D3DXVECTOR3 m_OffSetPos;             //�I�t�Z�b�g�ʒu
	D3DXVECTOR3 m_Rot;                   //����
	D3DXMATRIX m_mtxWorld;               //���[���h�}�g���b�N�X

	int m_nTextureIndex;                 //�e�N�X�`���ԍ���ݒ肷��

	MESHORBITTYPE m_type;                //���

	static const char* m_apMESHORBIT_FILENAME[TYPE_MAX];

};
#endif


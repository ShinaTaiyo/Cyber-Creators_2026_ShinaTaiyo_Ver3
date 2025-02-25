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
	enum class MESHORBITTYPE
	{
		NORMAL = 0,//����
		DEATHENAGA,//�f�X�G�i�K
		AUTSTRALIAUBAIMAN,//�I�[�X�g�����A�D���}��
		MAX                    
	};

	CMeshOrbit(int nPri = 0, bool bUseintPri = false, CObject::TYPE type = CObject::TYPE::MESHORBIT, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_3D);//�R���X�g���N�^
	~CMeshOrbit() override;                        //�f�X�g���N�^
	HRESULT Init() override;                       //����������
	void Uninit() override;                        //�I������
	void Update() override;                        //�X�V����
	void Draw() override;                          //�`�揈��
	void SetDeath() override;                      //���S�t���O�ݒ菈��
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture); //�e�N�X�`�������蓖�Ă�
	void SetTextureIndex(int nIdx) { m_nTextureIndex = nIdx; }//�e�N�X�`���ԍ������蓖�Ă�
	static CMeshOrbit* Create(MESHORBITTYPE type);            //��������
	int GetTextureIndex() { return m_nTextureIndex; }         //�e�N�X�`���ԍ����擾����

	//===========================================
	//�O�Պ֌W
	//===========================================
	void SetPos1(D3DXVECTOR3 Pos) { m_Pos1 = Pos; }            //�ʒu��ݒ�
	D3DXVECTOR3& GetPos1() { return m_Pos1; }                  //�ʒu���擾����
	void SetPos2(D3DXVECTOR3 OffSetPos) { m_Pos2 = OffSetPos; }//�I�t�Z�b�g�ʒu��ݒ肷��
	D3DXVECTOR3& GetPos2() { return m_Pos2; }                  //�I�t�Z�b�g�ʒu���擾����

	void SetRot(D3DXVECTOR3 Rot) { m_Rot = Rot; }              //������ݒ肷��
	//==================================================================================================================

private:
	static const int m_nNUMBLOCK_X = 20;                                                             //X�����̃u���b�N��
	static const int m_nNUMBLOCK_Z = 3;                                                              //Z�����̃u���b�N��
	static const int m_nTOTAL_BLOCK = m_nNUMBLOCK_X * m_nNUMBLOCK_Z;                                 //���v�u���b�N��
																			                         
	static const int m_nNUMVERTEX_X = m_nNUMBLOCK_X + 1;                                             //X�����̒��_��
	static const int m_nNUMVERTEX_Z = m_nNUMBLOCK_Z + 1;                                             //Y�����̒��_��
																			                         
	static const int m_nTOTAL_VERTEX = m_nNUMVERTEX_X * m_nNUMVERTEX_Z;                              //���v���_��

	static const int m_nTOTAL_INDEX = (m_nNUMVERTEX_X * 2) * m_nNUMBLOCK_Z + 2 * (m_nNUMBLOCK_Z - 1);//���v�C���f�b�N�X���i���̒��_���~�Q�~�c�̃u���b�N�F�� + �k�ތv�Z��
	                                                                                                 //(2�~�c�̃u���b�N�� - 1)
	static const int m_nTOTAL_POLYGON = m_nTOTAL_INDEX - 2;                                          //�C���f�b�N�X�R�ڂ���|���S�����ł���̂ŁA�C���f�b�N�X�� - 2


	PDIRECT3DVERTEXBUFFER9 m_pVtxBuff;   //���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;   //�C���f�b�N�X�o�b�t�@�ւ̃|�C���^

	LPDIRECT3DTEXTURE9 m_pTexture;       //�e�N�X�`���ւ̃|�C���^
	D3DXVECTOR3 m_Pos1;                  //�ʒu�P
	D3DXVECTOR3 m_Pos2;                  //�ʒu�Q
	D3DXVECTOR3 m_Rot;                   //����
	D3DXMATRIX m_mtxWorld;               //���[���h�}�g���b�N�X

	int m_nTextureIndex;                 //�e�N�X�`���ԍ���ݒ肷��

	MESHORBITTYPE m_type;                //���

	static const string m_aMESHORBIT_FILENAME[static_cast<int>(MESHORBITTYPE::MAX)];//�O�Ճ��b�V���̃t�@�C���p�X

};
#endif


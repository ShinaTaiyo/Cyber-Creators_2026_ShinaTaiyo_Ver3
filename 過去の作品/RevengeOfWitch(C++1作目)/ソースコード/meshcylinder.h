//========================================================
//
//�U���Q�P���F���b�V���V�����_�[�Ŕw�i���o��[meshcylinder.h]
//Author:ShinaTaiyo
//
//=========================================================
#ifndef _MESHCYLINDER_H_
#define _MESHCYLINDER_H_

//========================================
//�C���N���[�h
//========================================
#include "main.h"
#include "object.h"
//===========================================================

class CMeshCylinder : public CObject
{
public:
	CMeshCylinder();                               //�R���X�g���N�^
	~CMeshCylinder() override;                     //�f�X�g���N�^
	HRESULT Init() override;                       //����������
	void Uninit() override;                        //�I������
	void Update() override;                        //�X�V����
	void Draw() override;                          //�`�揈��
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture); //�e�N�X�`�������蓖�Ă�
	void SetCylinderInfo(D3DXVECTOR3 Pos, D3DXVECTOR3 Rot, int nNumBlockXZ, int nNumBlockY, float fRadius, float fHeight);
	void SetTextureIndex(int nIdx) { m_nTextureIndex = nIdx; }//�e�N�X�`���ԍ������蓖�Ă�4
	int GetTextureIndex() { return m_nTextureIndex; }         //�e�N�X�`���ԍ����擾����
private:
	typedef struct
	{
		int nNumBlockXZ;                   //X�����̃u���b�N��
		int nNumBlockY;                    //Y�����̃u���b�N��
		int nNumVtxXZ;                   //XZ����������
		int nDivisionY;                    //Y����������
		float fRadius;                     //���a
		float fHeight;                     //����
		int nNumVtx;                       //���_��
		int nNumIdx;                       //�C���f�b�N�X��
		int nNumPolygon;                   //�|���S����
		PDIRECT3DVERTEXBUFFER9 pVtxBuff;   //���_�o�b�t�@�ւ̃|�C���^
		LPDIRECT3DINDEXBUFFER9 pIdxBuff;   //�C���f�b�N�X�o�b�t�@�ւ̃|�C���^
		LPDIRECT3DTEXTURE9 pTexture;       //�e�N�X�`���ւ̃|�C���^
	}Info;

	D3DXVECTOR3 m_Pos;                   //�ʒu
	D3DXVECTOR3 m_Rot;                   //����
	D3DXMATRIX m_mtxWorld;               //���[���h�}�g���b�N�X
	Info m_Info;                         //�|���S�����
	int m_nTextureIndex;                 //�e�N�X�`���ԍ���ݒ肷��

}; 
#endif


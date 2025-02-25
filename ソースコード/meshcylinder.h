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
	CMeshCylinder(const float fRadius,const float fHeight,const int nNumDivisionXZ,const int nNumDivisionY,const D3DXVECTOR3 Pos,const D3DXVECTOR3 Rot, int nPri = 0, bool bUseintPri = false, CObject::TYPE type = CObject::TYPE::NONE, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_3D);                               //�R���X�g���N�^
	~CMeshCylinder() override;                                    //�f�X�g���N�^
	HRESULT Init() override;                                      //����������
	void Uninit() override;                                       //�I������
	void Update() override;                                       //�X�V����
	void Draw() override;                                         //�`�揈��
	void SetDeath() override;                                     //���S�t���O�ݒ菈��
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);                //�e�N�X�`�������蓖�Ă�
	void SetTextureIndex(int nIdx) { m_nTextureIndex = nIdx; }    //�e�N�X�`���ԍ������蓖�Ă�4
	int GetTextureIndex() { return m_nTextureIndex; }             //�e�N�X�`���ԍ����擾����

	//�ʒu
	void SetPos(D3DXVECTOR3 Pos) { m_Pos = Pos; }
	const D3DXVECTOR3 & GetPos() const { return m_Pos; }

	//����
	void SetRot(D3DXVECTOR3 Rot) { m_Rot = Rot; }
	const D3DXVECTOR3& GetRot() const { return m_Rot; }

	//����
	void SetHeight(float fHeight) { m_fHeight = fHeight; }
	const float& GetHeight() const { return m_fHeight; }              //����

	//�`�悷�邩�ǂ���
	void SetUseDraw(bool bUse) { m_bUseDraw = bUse; }
	const bool& GetUseDraw() const { return m_bUseDraw; }

	//���b�V�����
	LPDIRECT3DVERTEXBUFFER9 GetVtxBufferPointer() { return m_pVtxBuff; }//���_�o�b�t�@���擾����
	LPDIRECT3DINDEXBUFFER9 GetIdxBufferPointer() { return m_pIdxBuff; } //�C���f�b�N�X�o�b�t�@���擾����
	LPDIRECT3DTEXTURE9 GetTexture() { return m_pTexture; }              //�e�N�X�`�����擾����

	//�}�g���b�N�X���[���h�n
	void SetMtxChild(D3DXMATRIX* pMtxChild) { m_pMtxChild = pMtxChild; }	//�q�}�g���b�N�X�̃|�C���^��ݒ肷��
	D3DXMATRIX& GetMtxWorld() { return m_mtxWorld; }                        //�}�g���b�N�X���[���h���擾

	const int & GetNumDivisionXZ() const { return m_nNumDivisionXZ; } //XZ�����̕������̎擾
	const int & GetNumDivisionY() const { return m_nNumDivisionY; }   //Y�����̕������̎擾
	const int & GetNumVtx() const { return m_nNumVtx; }               //���_���̎擾
	const int & GetNumIdx() const { return m_nNumIdx; }               //�C���f�b�N�X���̎擾
	const int & GetNumPolygon() const { return m_nNumPolygon; }       //�|���S�����̎擾
	const float& GetRadius() const { return m_fRadius; }              //���a���擾

	//���ꂼ���Y�̒��S�_���擾
	const D3DXVECTOR3 & GetSenterPos(const int nArray) const
	{
		if (nArray < 0 || nArray >= m_nNumDivisionY)
		{
			assert(false);
		}
		return m_pSenterPos[nArray];
	}//���S�_�̐擪�|�C���^���擾
	void SetSenterPos(int nArray, D3DXVECTOR3 Pos)
	{
		if (nArray < 0 || nArray >= m_nNumDivisionY)
		{
			assert(false);
		}
		m_pSenterPos[nArray] = Pos;
	}//���ꂼ��̒��S�_��ݒ肷��

private:
	PDIRECT3DVERTEXBUFFER9 m_pVtxBuff;   //���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;   //�C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DTEXTURE9 m_pTexture;       //�e�N�X�`���ւ̃|�C���^

	//�傫���i���I�ɕω�����\��������j
	float m_fRadius;                     //���a
	float m_fHeight;                     //����

	//������
	const int m_nNumDivisionXZ;          //XZ����������
	const int m_nNumDivisionY;           //Y����������

	//�\���v�f��
	int m_nNumPolygon;                   //�|���S����
	const int m_nNumVtx;                 //���_��
	const int m_nNumIdx;                 //�C���f�b�N�X��

	//���[���h���
	D3DXVECTOR3 m_Pos;                   //�ʒu
	D3DXVECTOR3 m_Rot;                   //����
	D3DXMATRIX m_mtxWorld;               //���[���h�}�g���b�N�X
	D3DXMATRIX* m_pMtxChild;             //�q�}�g���b�N�X

	//�e�N�X�`��
	int m_nTextureIndex;                 //�e�N�X�`���ԍ���ݒ肷��

	//���_�m�F�p
	int m_nCheckVtx;
	int m_nCheckIdx;

	//���ꂼ��̒��S�_
	D3DXVECTOR3* m_pSenterPos;

	bool m_bUseDraw;                     //�`�悷�邩�ǂ���

	//=============================================
	//�֐�
	//=============================================
	void SetVertexInfo(VERTEX_3D* pVtx,float & fRatioXZ,float & fRatioY,int &nCntVtxXZ,int & nCntVtxY); //���_����ݒ肷��
	void ChengeNumPolygon();                                                                            //�|���S������ς��鏈��
	void CheckMeshInfo();                                                                               //���b�V���̂��ꂼ��̏����m�F����
}; 
#endif


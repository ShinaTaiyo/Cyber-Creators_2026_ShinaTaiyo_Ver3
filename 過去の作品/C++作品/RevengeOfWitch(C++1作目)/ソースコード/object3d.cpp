//==================================================================================
//
//�U���R���F�RD�I�u�W�F�N�g�N���X���쐬����[3Dobject.h]
//Author:ShinaTaiyo
//
//===================================================================================

//=================================
//�C���N���[�h
//=================================
#include "object3d.h"
#include "manager.h"
#include "renderer.h"
//===================================================================================

//================================================
//�R���X�g���N�^
//================================================
CObject3D::CObject3D(int nPriority) : CObject(nPriority)
{
	m_pTexture = nullptr;
	m_pVtxBuff = nullptr;
}
//================================================================================================================================================

//================================================
//�f�X�g���N�^
//================================================
CObject3D::~CObject3D()
{

}
//================================================================================================================================================

//================================================
//����������
//================================================
HRESULT CObject3D::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice;                     //�f�o�C�X�ւ̃|�C���^
	pDevice = CManager::GetRenderer()->GetDevice();//�f�o�C�X�̎擾
	VERTEX_3D* pVtx;                               //���_���ւ̃|�C���^

    //=================================
	//�ϐ��̏�����
	//=================================
	m_Pos = NULL_VECTOR3;                        //�ʒu
	m_Rot = NULL_VECTOR3;                        //����
	m_mtxWorld = {};                             //�}�g���b�N�X���[���h
	m_fWidth = 0.0f;                             //����
	m_fHeight = 0.0f;                            //����
	m_nTextureIndex = 0;                         //�e�N�X�`���ԍ�
	//=======================================================================

	//=================================
	//���_�o�b�t�@�̐���
	//=================================
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,//���_�̐�
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);
	//=======================================================================


	//=================================
	//���f�����̏�����
	//=================================

	//���_�o�b�t�@�̃��b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�i�����ʒu�j
	pVtx[0].pos = NULL_VECTOR3;
	pVtx[1].pos = NULL_VECTOR3;
	pVtx[2].pos = NULL_VECTOR3;
	pVtx[3].pos = NULL_VECTOR3;

	//nor�̐ݒ�i�����ݒ�j
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLOR�́A0.0f�`1.0f�̊Ԃ̐��l�ŐF��ς���B
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLOR�́A0.0f�`1.0f�̊Ԃ̐��l�ŐF��ς���B
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLOR�́A0.0f�`1.0f�̊Ԃ̐��l�ŐF��ς���B
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLOR�́A0.0f�`1.0f�̊Ԃ̐��l�ŐF��ς���B


	//�e�N�X�`�����W�̐ݒ�i�����ݒ�j
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@�̃A�����b�N
	m_pVtxBuff->Unlock();
	//==============================================================================================================

	return S_OK;
}
//================================================================================================================================================

//================================================
//�I������
//================================================
void CObject3D::Uninit(void)
{
	//�e�N�X�`���̔j��
	if (m_pTexture != nullptr)
	{
		m_pTexture = nullptr;
	}

	//���_�o�b�t�@�̔j��
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

}
//================================================================================================================================================

//================================================
//�X�V����
//================================================
void CObject3D::Update(void)
{
	VERTEX_3D* pVtx;                               //���_���ւ̃|�C���^

	//���_�o�b�t�@�̃��b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�i�����ʒu�j
	pVtx[0].pos = D3DXVECTOR3(-m_fWidth, 0.0f,m_fHeight);
	pVtx[1].pos = D3DXVECTOR3(m_fWidth, 0.0f,m_fHeight);
	pVtx[2].pos = D3DXVECTOR3(-m_fWidth, 0.0f, -m_fHeight);
	pVtx[3].pos = D3DXVECTOR3(m_fWidth, 0.0f, -m_fHeight);

	//nor�̐ݒ�i�����ݒ�j
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLOR�́A0.0f�`1.0f�̊Ԃ̐��l�ŐF��ς���B
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLOR�́A0.0f�`1.0f�̊Ԃ̐��l�ŐF��ς���B
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLOR�́A0.0f�`1.0f�̊Ԃ̐��l�ŐF��ς���B
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLOR�́A0.0f�`1.0f�̊Ԃ̐��l�ŐF��ς���B


	//�e�N�X�`�����W�̐ݒ�i�����ݒ�j
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@�̃A�����b�N
	m_pVtxBuff->Unlock();
	//==============================================================================================================


}
//================================================================================================================================================

//================================================
//�`�揈��
//================================================
void CObject3D::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();//�f�o�C�X�ւ̃|�C���^
	pDevice = CManager::GetRenderer()->GetDevice();                  //�f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans;                                     //�v�Z�p�}�g���b�N�X

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0,m_pTexture);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

}
//================================================================================================================================================

//================================================
//�e�N�X�`�����蓖�ď���
//================================================
void CObject3D::bindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}
//================================================================================================================================================

//================================================
//��������
//================================================
CObject3D* CObject3D::Create(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	CObject3D* pObject3D = DBG_NEW CObject3D;                        //3D�I�u�W�F�N�g�̓��I���������m��
	bool bSuccess = pObject3D->CObject::GetCreateSuccess();	     //�I�u�W�F�N�g�̐����������������ǂ������擾
	VERTEX_3D* pVtx;                                             
	if (bSuccess == true)
	{//�I�u�W�F�N�g�̐���������
		if (pObject3D != nullptr)
		{
			pObject3D->Init();                                   //����������
			pObject3D->m_fWidth = fWidth / 2.0f;                 //����
			pObject3D->m_fHeight = fHeight / 2.0f;               //����
			pObject3D->m_Pos = pos;                              //�ʒu

			//���_�o�b�t�@�̃��b�N
			pObject3D->m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			//���_���W�̐ݒ�i�����ʒu�j
			pVtx[0].pos = D3DXVECTOR3(- pObject3D->m_fWidth,0.0f,pObject3D->m_fHeight);
			pVtx[1].pos = D3DXVECTOR3(pObject3D->m_fWidth, 0.0f,pObject3D->m_fHeight);
			pVtx[2].pos = D3DXVECTOR3(- pObject3D->m_fWidth, 0.0f,- pObject3D->m_fHeight);
			pVtx[3].pos = D3DXVECTOR3(pObject3D->m_fWidth, 0.0f,- pObject3D->m_fHeight);

			//���_�o�b�t�@�̃A�����b�N
			pObject3D->m_pVtxBuff->Unlock();

			pObject3D->CObject::SetType(CObject::TYPE_PLAYER);   //�I�u�W�F�N�g�̎�ނ�ݒ�
		}
	}
	else
	{//�I�u�W�F�N�g�̐��������s
		delete pObject3D;
		pObject3D = nullptr;
		return nullptr;
	}
	return pObject3D;
}
//================================================================================================================================================
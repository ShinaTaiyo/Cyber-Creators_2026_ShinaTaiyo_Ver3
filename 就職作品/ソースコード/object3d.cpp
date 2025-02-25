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
CObject3D::CObject3D(int nPri, bool bUseintPri, CObject::TYPE type, CObject::OBJECTTYPE ObjType) : CObject(nPri,bUseintPri,type,ObjType),m_fWidth(0.0f),m_fHeight(0.0f),m_nTextureIndex(0),m_Pos(D3DXVECTOR3(0.0f,0.0f,0.0f)),m_Rot(D3DXVECTOR3(0.0f,0.0f,0.0f)),m_mtxWorld(),
m_pVtxBuff(nullptr),m_pTexture(nullptr)
{

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
	m_Pos = D3DXVECTOR3(0.0f,0.0f,0.0f);                        //�ʒu
	m_Rot = D3DXVECTOR3(0.0f,0.0f,0.0f);                        //����
	m_mtxWorld = {};                                            //�}�g���b�N�X���[���h
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
	pVtx[0].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	pVtx[3].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);

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

	CObject::Init();//�I�u�W�F�N�g����������

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

	CObject::Uninit();//�I�u�W�F�N�g�I������
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

	CObject::Update();//�I�u�W�F�N�g�̍X�V����
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

	CObject::Draw();//�I�u�W�F�N�g�̕`�揈��
}
//================================================================================================================================================

//================================================
//���S�t���O�ݒ菈��
//================================================
void CObject3D::SetDeath()
{
	CObject::SetDeath();//�I�u�W�F�N�g�̎��S�t���O�ݒ菈��
}
//================================================================================================================================================

//================================================
//�e�N�X�`�����蓖�ď���
//================================================
void CObject3D::bindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;//�e�N�X�`���A�h���X�����蓖�Ă�
}
//================================================================================================================================================
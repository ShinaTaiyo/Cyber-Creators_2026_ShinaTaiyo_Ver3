//=======================================================================================================================
//
//�U���Q�P���F���b�V���V�����_�[�Ŕw�i���o�������B[meshCylinder.cpp]
//Author:ShinaTaiyo
//
//=======================================================================================================================

//=================================================================================================================
//�C���N���[�h
//=================================================================================================================
#include "meshcylinder.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
//=================================================================================================================

//==================================================================================================================
//�R���X�g���N�^
//==================================================================================================================
CMeshCylinder::CMeshCylinder()
{
	m_Info.pTexture = nullptr;//�e�N�X�`��
	m_Info.pVtxBuff = nullptr;//���_�o�b�t�@�ւ̃|�C���^
	m_Info.pIdxBuff = nullptr;//�C���f�b�N�X�o�b�t�@�ւ̃|�C���^
}
//=========================================================================================================================================

//==================================================================================================================
//�f�X�g���N�^
//==================================================================================================================
CMeshCylinder::~CMeshCylinder()
{
}
//=========================================================================================================================================

//==================================================================================================================
//����������
//==================================================================================================================
HRESULT CMeshCylinder::Init()
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�ւ̃|�C���^

    //�f�o�C�X�̎擾
	pDevice = CManager::GetRenderer()->GetDevice();

	VERTEX_3D* pVtx;

	m_Info.nNumVtxXZ = m_Info.nNumBlockXZ + 1;                                                   //���̒��_��
	m_Info.nDivisionY = m_Info.nNumBlockY - 1;                                                   //Y����������
	m_Info.nNumVtx = m_Info.nNumVtxXZ * (m_Info.nNumBlockY + 1);                                 //���b�V���V�����_�[�̒��_���i�����_�� * �u���b�N���@+ 1)
	m_Info.nNumPolygon = ((m_Info.nNumBlockXZ * 2) * m_Info.nNumBlockY + 2 * m_Info.nDivisionY); //�|���S����
	m_Info.nNumIdx = m_Info.nNumPolygon + 2;                                                     //�C���f�b�N�X��

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_Info.nNumVtx,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_Info.pVtxBuff,
		NULL);

	//�C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * m_Info.nNumIdx,//�C���f�b�N�X��
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_Info.pIdxBuff,
		NULL);

	//=============================
	//���_�̐ݒ�
	//=============================

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_Info.pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	float fRatioRot = 0.0f;//�p�x�v�Z�p�ϐ�
	float fRatioHeight = 0.0f;//�����v�Z�p
	float fVecX = 0.0f;//�x�N�g��X�v�Z�p
	float fVecZ = 0.0f;//�x�N�g��Z�v�Z�p
	float fVec = 0.0f; //�x�N�g���v�Z�p
	int nNumVtx = 0;//���ݐݒ肵�Ă��钸�_�ԍ�
	int nCntArray = 0;//�z��J�E���g�p
	D3DXVECTOR3 MeasureNor = NULL_VECTOR3;//�@���v�Z�p
	D3DXVECTOR3 NormalizeNor = NULL_VECTOR3;//���K�������@����������
	for (int nCntVtxY = 0; nCntVtxY < m_Info.nNumBlockY + 1; nCntVtxY++)
	{//���_�̗񕪐ݒ肷��i�u���b�N���{�P�j
		for (int nCntVtxXZ = 0; nCntVtxXZ < m_Info.nNumVtxXZ; nCntVtxXZ++)
		{
			nCntArray = m_Info.nNumVtxXZ * nCntVtxY;//�O��for���܂łɐݒ肵���z����J�E���g����

			fRatioRot = (2.0f / m_Info.nNumBlockXZ) * (float)(nCntVtxXZ);  //���݂̒��_��ݒ肷����������߂�i�����j

			fRatioHeight = 1.0f - (1.0f / m_Info.nNumBlockY) * nCntVtxY;//���݂̒��_��ݒ肷�鍂�������߂�i�����j

			if (nCntVtxXZ + nCntArray < 0 || nCntVtxXZ + nCntArray >= m_Info.nNumVtx)
			{
				assert("���b�V���V�����_�[�̒��_�o�b�t�@���z��O�A�N�Z�X");
			}
			//�ʒu��ݒ�
			pVtx[(nCntVtxXZ)+nCntArray].pos = D3DXVECTOR3(sinf(D3DX_PI - D3DX_PI * -fRatioRot) * m_Info.fRadius,
				(m_Info.fHeight * fRatioHeight) - m_Info.fHeight * 0.5f, cosf(D3DX_PI - D3DX_PI * -fRatioRot) * m_Info.fRadius);

			//���_�ƒ��_�̃x�N�g�����v�Z
			fVecX = pVtx[(nCntVtxXZ)+nCntArray].pos.x - m_Pos.x;
			fVecZ = pVtx[(nCntVtxXZ)+nCntArray].pos.z - m_Pos.z;
			fVec = atan2f(fVecX, fVecZ);

			//�@���̌��������߂�
			MeasureNor = D3DXVECTOR3(fVecX, 0.0f, fVecZ);//�@�����v�Z	

			//�@���̃x�N�g���𐳋K������
			D3DXVec3Normalize(&NormalizeNor, &MeasureNor);

			//�@����ݒ�
			pVtx[(nCntVtxXZ)+nCntArray].nor = NormalizeNor;

			//���_�J���[�̐ݒ�
			pVtx[(nCntVtxXZ)+nCntArray].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//�e�N�X�`�����W�̐ݒ�
			pVtx[(nCntVtxXZ)+nCntArray].tex = D3DXVECTOR2((1.0f / m_Info.nNumBlockXZ) * (float)(nCntVtxXZ), -fRatioHeight);

			//���ݐݒ肵�Ă��钸�_�̔ԍ�����
			nNumVtx = (nCntVtxXZ)+nCntArray;
		}
	}

	//���_�o�b�t�@���A�����b�N���� 
	m_Info.pVtxBuff->Unlock();

	//========================================================================================================================================================================================


	//==============================
	//�C���f�b�N�X���̐ݒ�
	//==============================

	//�C���f�b�N�X�o�b�t�@�̃��b�N
	WORD* pIdx;
	m_Info.pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	int nCntIdx = 0;//�C���f�b�N�X�v�Z�p
	int nCntIdx2 = 0;//�C���f�b�N�X�v�Z�p�Q
	int nCntIdxX = 0;//X�����̃C���f�b�N�X�v�Z�p
	int nIdx = 0; //�C���f�b�N�X����p
	int nIdx2 = 0;//�C���f�b�N�X����p�Q
	int nCntArray2 = 0;//�z��v�Z�p�Q
	for (int nCntVtxY = 0; nCntVtxY < m_Info.nNumBlockY; nCntVtxY++)
	{//�u���b�N�����A�ݒ���s��

		nCntIdx = (m_Info.nNumVtxXZ * nCntVtxY);       //�C���f�b�N�X�v�Z
		nCntIdx2 = (m_Info.nNumVtxXZ * (nCntVtxY + 1));//�C���f�b�N�X�v�Z

		for (int nCntVtxXZ = 0; nCntVtxXZ < m_Info.nNumVtxXZ; nCntVtxXZ++)
		{
			if (nCntArray2 + 1 < 0 || nCntArray2 + 1 >= m_Info.nNumIdx)
			{
				assert("���b�V���V�����_�[�̃C���f�b�N�X���z��O�A�N�Z�X");
			}

			pIdx[nCntArray2] = nCntVtxXZ + nCntIdx2;   //9,10,11,12,13,14,15,16,17,18
			pIdx[nCntArray2 + 1] = nCntVtxXZ + nCntIdx;//0, 1, 2, 3, 4, 5, 6, 7, 8, 9

			nIdx = nCntArray2;     //���ݐݒ肵�Ă���C���f�b�N�X�̔ԍ�����
			nIdx2 = nCntArray2 + 1;//���ݐݒ肵�Ă���C���f�b�N�X�̔ԍ�����2

			nCntArray2 += 2;//�ݒ肵���C���f�b�N�X�ԍ������ɐi�߂�
		}
	}

	//�C���f�b�N�X�o�b�t�@�̃A�����b�N
	m_Info.pIdxBuff->Unlock();

	////==========================================================================================================================================
	return S_OK;
}
//=========================================================================================================================================

//==================================================================================================================
//�I������
//==================================================================================================================
void CMeshCylinder::Uninit()
{

	//���_�o�b�t�@�̔j��
	if (m_Info.pVtxBuff != nullptr)
	{
		m_Info.pVtxBuff->Release();
	    m_Info.pVtxBuff = nullptr;
	}

	//�C���f�b�N�X�o�b�t�@�̊J��
	if (m_Info.pIdxBuff != nullptr)
	{
		m_Info.pIdxBuff->Release();
		m_Info.pIdxBuff = nullptr;
	}

}
//=========================================================================================================================================

//==================================================================================================================
//�X�V����
//==================================================================================================================
void CMeshCylinder::Update()
{
	m_Rot.y += 0.001f;
	//m_Pos = CGame::GetPlayer()->GetPos();
}
//=========================================================================================================================================

//==================================================================================================================
//�`�揈��
//==================================================================================================================
void CMeshCylinder::Draw()
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = CManager::GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;//�v�Z�p�}�g���b�N�X

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans,m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);


	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0,m_Info.pVtxBuff, 0, sizeof(VERTEX_3D));//���_�o�b�t�@�ւ̃|�C���^�ƒ��_���̍\���̂̃T�C�Y

	//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(m_Info.pIdxBuff);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_Info.pTexture);

	//�|���S���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0,m_Info.nNumVtx, 0,m_Info.nNumPolygon);

}
//=========================================================================================================================================

//==================================================================================================================
//�e�N�X�`�����蓖�ď���
//==================================================================================================================
void CMeshCylinder::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_Info.pTexture = pTexture;
}
//=========================================================================================================================================

//==================================================================================================================
//���b�V���V�����_�[����ݒ肷��
//==================================================================================================================
void CMeshCylinder::SetCylinderInfo(D3DXVECTOR3 Pos, D3DXVECTOR3 Rot, int nNumBlockXZ, int nNumBlockY, float fRadius, float fHeight)
{
	m_Pos = Pos;
	m_Rot = Rot;
	m_Info.nNumBlockXZ = nNumBlockXZ;
	m_Info.nNumBlockY = nNumBlockY;
	m_Info.fRadius = fRadius;
	m_Info.fHeight = fHeight;
}
//=========================================================================================================================================
//=======================================================================================================================
//
//�P�P���P�T���F���b�V���V�����_�[�Ń��C���[��\���������̂ŕ׋����Ȃ���[meshCylinder.cpp]
//Author:ShinaTaiyo
//
//=======================================================================================================================

//=================================================================================================================
//�C���N���[�h
//=================================================================================================================
#include "meshcylinder.h"
#include "manager.h"
#include "debugtext.h"
#include "renderer.h"
#include "particle.h"
#include "input.h"
#include "game.h"
//=================================================================================================================

//==================================================================================================================
//�R���X�g���N�^
//==================================================================================================================
CMeshCylinder::CMeshCylinder(const float fRadius, const float fHeight, const int nNumDivisionXZ, const int nNumDivisionY,const D3DXVECTOR3 Pos,const D3DXVECTOR3 Rot
,int nPri, bool bUseintPri, CObject::TYPE type, CObject::OBJECTTYPE ObjType) : CObject(nPri,bUseintPri,type,ObjType),
m_fRadius(fRadius),m_fHeight(fHeight),//���a�A����
m_nNumDivisionXZ(nNumDivisionXZ),m_nNumDivisionY(nNumDivisionY),//XZ�����������AY����������
m_nNumVtx((m_nNumDivisionXZ + 1) * m_nNumDivisionY + 2),//���_���i��ʂ̒��S�_�@�{�@���ʂ̒��S�_�@�{�@�i (X�������_�� + 1)�@���@Y�������_�� �j��X������+1��UV�΍�
m_nNumIdx((m_nNumDivisionXZ + 1) * 2 * (m_nNumDivisionY + 1) + 2 * m_nNumDivisionY),//�C���f�b�N�X��(�P�ʂ́AXZ�������_���@���@�Q)�@���@�iY�������_�� + 1) + 2 * (�k�ތv�Z��)
m_nTextureIndex(0),m_pIdxBuff(nullptr),m_pVtxBuff(nullptr),m_pTexture(nullptr),
m_Pos(Pos),m_Rot(Rot),m_nCheckVtx(0),m_nCheckIdx(0),m_pMtxChild(nullptr),m_bUseDraw(true)
{
	m_nNumPolygon = m_nNumIdx - 2;//��F�C���f�b�N�X�o�b�t�@���i�O�A�P�A�Q�A�R�A�S�A�T�j�̏ꍇ
	                              //���|���S���P�i�O�A�P�A�Q�j�A�|���S���Q�i�P�A�Q�A�R�j�A�|���S���R�i�Q�A�R�A�S�j�A�|���S���S�i�R�A�S�A�T�j
	                              //�܂�A�U�@�[�@�Q�@���@�S
	m_pSenterPos = DBG_NEW D3DXVECTOR3[m_nNumDivisionY];//Y�����̕��������A���S���W���`����
}
//=========================================================================================================================================

//2 * (m_nNumDivisionXZ + 1) * (m_nNumDivisionY + 1) + 2 * 2 + 1 * (m_nNumDivisionY - 1) - 2

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

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nNumVtx,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	//�C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * m_nNumIdx,//�C���f�b�N�X��
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	 VERTEX_3D* pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	int nCntArray = 0;
	float fRatioXZ = 0.0f;
	float fRatioY = 0.0f;
	D3DXVECTOR3 MeasureNor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�@���v�Z�p
	for (int nCntVtxY = 0; nCntVtxY < m_nNumDivisionY; nCntVtxY++)
	{//Y������UV�͂��̂܂܎g��
		fRatioY = (1.0f / (m_nNumDivisionY - 1)) * nCntVtxY;
		for (int nCntVtxXZ = 0; nCntVtxXZ < m_nNumDivisionXZ + 1; nCntVtxXZ++)
		{//X������UV�͏d�Ȃ钸�_������̂ŁA+ 1
			fRatioXZ = (1.0f / (m_nNumDivisionXZ)) * nCntVtxXZ;//XZ�����̊��������߂�
			if (nCntVtxY == 0 && nCntVtxXZ == 0)
			{//�ŏ��̎��Ȃ̂ŏ�ʂ̒��S�_��ݒ肷��
			    pVtx[nCntArray].pos = m_Pos + D3DXVECTOR3(0.0f, m_fHeight, 0.0f);//�ʒu�̐ݒ�
				pVtx[nCntArray].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);         //�F����
				MeasureNor = pVtx[nCntArray].pos - m_Pos;                        //�@��
				D3DXVec3Normalize(&MeasureNor, &MeasureNor);                     //�@���𐳋K��
				pVtx[nCntArray].nor = MeasureNor;                                //�@����ݒ�
				pVtx[nCntArray].tex = D3DXVECTOR2(0.0f, fRatioY);                //�e�N�X�`����ݒ�
				nCntArray++;
			}

			SetVertexInfo(&pVtx[nCntArray], fRatioXZ, fRatioY, nCntVtxXZ, nCntVtxY);//���_����ݒ肷��

			if (nCntArray >= m_nNumVtx || nCntArray < 0)
			{//�z��O�`�F�b�N
				assert("�z��O�A�N�Z�X�I");
			}

			//�z��J�E���g
			nCntArray++;

			if (nCntVtxY == m_nNumDivisionY - 1 && nCntVtxXZ == m_nNumDivisionXZ)
			{//�Ō�
				pVtx[nCntArray].pos = m_Pos;//��ʂ̒��S�Ɉʒu��ݒ�
				pVtx[nCntArray].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//�F������ݒ�
				MeasureNor = pVtx[nCntArray].pos - m_Pos;//�@�����v�Z
				D3DXVec3Normalize(&MeasureNor, &MeasureNor);//���K������
				pVtx[nCntArray].nor = MeasureNor;//�ݒ�
				pVtx[nCntArray].tex = D3DXVECTOR2(0.0f, fRatioY);//�e�N�X�`�����W��ݒ�
			}
		}
	}
	//���_�o�b�t�@���A�����b�N���� 
	m_pVtxBuff->Unlock();

	//==============================
	//�C���f�b�N�X���̐ݒ�
	//==============================

	//�C���f�b�N�X�o�b�t�@�̃��b�N
	WORD* pIdx;
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	int nCntIdx = 0;//�C���f�b�N�X�v�Z�p
	int nCntArray2 = 0;//�z��v�Z�p�Q

	bool bSide = false;//���ʂ��ǂ���

	for (int nCntVtxY = 0; nCntVtxY < m_nNumDivisionY + 1; nCntVtxY++)//�P�͏�ʁA���ʂ̕�
	{//�u���b�N�����A�ݒ���s��
		nCntIdx = (m_nNumDivisionXZ + 1) * nCntVtxY + 1;//1,10,19�E�E�E�i��ʁ����ʁ����ʁ���ʁj
		if (nCntVtxY == 0)
		{//��ʂ̒��S
			bSide = false;
		}
		else if (nCntVtxY == m_nNumDivisionY)
		{//��ʂ̒��S
			bSide = false;
		}
		else
		{//����
			bSide = true;
		}

		if (nCntArray2 >= m_nNumIdx || nCntArray2 < 0)
		{
			assert("�z��O�A�N�Z�X�I");
		}

		if (bSide == true)
		{//���ʂɒ��_���\���i����)
			for (int nCntVtxXZ = 0; nCntVtxXZ < m_nNumDivisionXZ + 1; nCntVtxXZ++)
			{
				pIdx[nCntArray2] = nCntVtxXZ + nCntIdx;   //10,11,12,13,14,15,16,17,18
				pIdx[nCntArray2 + 1] = nCntIdx - m_nNumDivisionXZ - 1 + nCntVtxXZ;//1, 2, 3, 4, 5, 6, 7, 8, 9
				nCntArray2 += 2;//�ݒ肵���C���f�b�N�X�ԍ������ɐi�߂�
			}
		}
		else
		{//��ʂ���ʂȂ̂ł��ꂼ��̖ʂ̒��S�_����ɃC���f�b�N�X���\��
			for (int nCntVtxXZ = 0; nCntVtxXZ < m_nNumDivisionXZ + 1; nCntVtxXZ++)
			{
				if (nCntVtxY == 0)
				{//���
					pIdx[nCntArray2] = nCntVtxXZ + nCntIdx;   //1,2,3,4,5,6,7,8,9
					pIdx[nCntArray2 + 1] = 0;//0,0,0,0,0,0,0,0,0
				}
				else
				{//����
					pIdx[nCntArray2] = m_nNumVtx - m_nNumDivisionXZ - 2 + nCntVtxXZ;    //19,20,21,22,23,24,25,26,27
					pIdx[nCntArray2 + 1] = m_nNumVtx - 1;      //���_�� - 1(�z��ő吔)(��ʂ̒��S�_)28
				}
				nCntArray2 += 2;//�ݒ肵���C���f�b�N�X�ԍ������ɐi�߂�
			}
		}

		if (nCntVtxY != m_nNumDivisionY)
		{//�Ō�̍s�ȊO�Ȃ�k�ރ|���S����ݒ�
			if (nCntVtxY == 0)
			{//����ڂȂ璆�S�_���牺�̒i�̍ŏ��ɂȂ���
				pIdx[nCntArray2] = 0;                       //��ʂ̒��S��//0
				pIdx[nCntArray2 + 1] = m_nNumDivisionXZ + 2;//���̒i�̍ŏ�//10
			}
			else if (nCntVtxY == m_nNumDivisionY - 1)
			{//�Ō�̏k��
				pIdx[nCntArray2] = nCntVtxY * (m_nNumDivisionXZ + 1);//18
				pIdx[nCntArray2 + 1] = nCntVtxY * (m_nNumDivisionXZ + 1) + 1;//19
			}
			else
			{//�ŏ��ƍŌ�̎��ȊO�Ȃ�
				pIdx[nCntArray2] = nCntVtxY * (m_nNumDivisionXZ + 1);//9,18
				pIdx[nCntArray2 + 1] = (nCntVtxY + 1) * (m_nNumDivisionXZ + 1) + 1;//19,28
			}
			nCntArray2 += 2;
		}
	}

	//�C���f�b�N�X�o�b�t�@�̃A�����b�N
	m_pIdxBuff->Unlock();

	//////==========================================================================================================================================
	return S_OK;
}
//=========================================================================================================================================

//==================================================================================================================
//�I������
//==================================================================================================================
void CMeshCylinder::Uninit()
{
	//���_�o�b�t�@�̔j��
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
	    m_pVtxBuff = nullptr;
	}

	//�C���f�b�N�X�o�b�t�@�̊J��
	if (m_pIdxBuff != nullptr)
	{
		m_pIdxBuff->Release();
		m_pIdxBuff = nullptr;
	}

	//�e�N�X�`���̊J���i�ʂŊJ������)
	if (m_pTexture != nullptr)
	{
		m_pTexture = nullptr;
	}

	//���ꂼ��̒��S�_�̊J���i���I�z���j���j
	if (m_pSenterPos != nullptr)
	{
		delete[] m_pSenterPos;
		m_pSenterPos = nullptr;
	}
}
//=========================================================================================================================================

//==================================================================================================================
//�X�V����
//==================================================================================================================
void CMeshCylinder::Update()
{
#ifdef _DEBUG
	CheckMeshInfo();
#endif
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
	D3DXMatrixTranslation(&mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	if (m_pMtxChild != nullptr)
	{//�q�}�g���b�N�X�����݂���Ȃ�|�����킹��
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, m_pMtxChild);
	}

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//���ʂ�`�悷��
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));//���_�o�b�t�@�ւ̃|�C���^�ƒ��_���̍\���̂̃T�C�Y

	//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(m_pIdxBuff);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTexture);

	if (m_bUseDraw == true)
	{
		//�|���S���̕`��
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_nNumVtx, 0, m_nNumPolygon);
	}
	//�Жʂ����`�悷��
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}
//=========================================================================================================================================

//==================================================================================================================
//���S�t���O�ݒ菈��
//==================================================================================================================
void CMeshCylinder::SetDeath()
{
	CObject::SetDeath();//���S�t���O�ݒ菈��
}
//=========================================================================================================================================

//==================================================================================================================
//�e�N�X�`�����蓖�ď���
//==================================================================================================================
void CMeshCylinder::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;//�e�N�X�`���A�h���X���i�[
}
//=========================================================================================================================================

//==================================================================================================================
//���_����ݒ�
//==================================================================================================================
void CMeshCylinder::SetVertexInfo(VERTEX_3D* pVtx, float& fRatioXZ, float& fRatioY, int& nCntVtxXZ, int& nCntVtxY)
{
	//���ʂɒ��_��W�J
	pVtx->pos = D3DXVECTOR3(sinf((D3DX_PI * 2) * fRatioXZ) * m_fRadius,m_fHeight - (m_fHeight * fRatioY), cosf((D3DX_PI * 2) * fRatioXZ) * m_fRadius) + m_Pos;//1,2,3,4,5,6,7,8��10,11,12,13,14,15,16,17

	//�F����
	pVtx->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//�@�������߂�
	D3DXVECTOR3 Nor = pVtx->pos - m_Pos;//�x�N�g�������߂�
	D3DXVec3Normalize(&Nor, &Nor);//�@���𐳋K��
	pVtx->nor = Nor;//���

	//�e�N�X�`��
	pVtx->tex = D3DXVECTOR2(fRatioXZ, fRatioY);

}
//=========================================================================================================================================

//==================================================================================================================
//�|���S������ς��鏈��
//==================================================================================================================
void CMeshCylinder::ChengeNumPolygon()
{
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_U) == true)
	{
		m_nNumPolygon++;
	}
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_J) == true)
	{
		m_nNumPolygon--;
	}

	//�f�o�b�O�\��
	CManager::GetDebugText()->PrintDebugText("�|���S�����F%d\n", m_nNumPolygon);
	CManager::GetDebugText()->PrintDebugText("���_���F%d\n", m_nNumVtx);
	CManager::GetDebugText()->PrintDebugText("�C���f�b�N�X���F%d\n", m_nNumIdx);
}
//=========================================================================================================================================

//==================================================================================================================
//���b�V���̏����m�F����
//==================================================================================================================
void CMeshCylinder::CheckMeshInfo()
{
	VERTEX_3D* pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//=================================================================
	//���_���m�F
	//=================================================================
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_O) == true)
	{
		m_nCheckVtx++;
	}
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_L) == true)
	{
		m_nCheckVtx--;
	}

	if (m_nCheckVtx >= m_nNumVtx)
	{//�z��T�C�Y�𒴂���̂łO�ɖ߂�
		m_nCheckVtx = 0;
	}
	else if (m_nCheckVtx < 0)
	{//�z�񂪁[�ɂȂ�̂ōő�ɂ���
		m_nCheckVtx = m_nNumVtx - 1;
	}

	if (m_nCheckVtx >= m_nNumVtx || m_nCheckVtx < 0)
	{//�z�񔃂��A�N�Z�X�`�F�b�N
		assert("�z��O�A�N�Z�X�I");
	}
	//�f�o�b�O�\��
	CManager::GetDebugText()->PrintDebugText("�m�F���Ă��钸�_�ԍ��F%d�A���_�̈ʒu�F%f %f %f\n",
		m_nCheckVtx, pVtx[m_nCheckVtx].pos.x, pVtx[m_nCheckVtx].pos.y, pVtx[m_nCheckVtx].pos.z);

	//==============================
	//�C���f�b�N�X���̐ݒ�
	//==============================

	//�C���f�b�N�X�o�b�t�@�̃��b�N
	WORD* pIdx;
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);
	//�C���f�b�N�X�o�b�t�@�̃A�����b�N

	if (CManager::GetInputKeyboard()->GetTrigger(DIK_I) == true)
	{
		m_nCheckIdx++;
	}
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_K) == true)
	{
		m_nCheckIdx--;
	}

	if (m_nCheckIdx >= m_nNumIdx)
	{//�z��T�C�Y�𒴂���̂łO�ɖ߂�
		m_nCheckIdx = 0;
	}
	else if (m_nCheckIdx < 0)
	{//�z�񂪁[�ɂȂ�̂ōő�ɂ���

		m_nCheckIdx = m_nNumIdx - 1;
	}

	if (m_nCheckIdx >= m_nNumIdx || m_nCheckIdx < 0)
	{//�z��O�A�N�Z�X�`�F�b�N
		assert("�z��O�A�N�Z�X�I");
	}

	int nCheck = pIdx[m_nCheckIdx];

	//�f�o�b�O�\��
	CManager::GetDebugText()->PrintDebugText("�m�F���Ă���C���f�b�N�X�ԍ��F%d�A���_�̈ʒu�F%f %f %f\n", m_nCheckIdx, pVtx[nCheck].pos.x, pVtx[nCheck].pos.y, pVtx[nCheck].pos.z);

	//�C���f�b�N�X�o�b�t�@���A�����b�N
	m_pIdxBuff->Unlock();

	//���_�o�b�t�@���A�����b�N���� 
	m_pVtxBuff->Unlock();
	//==================================================================================================================

	//�|���S������ς���
	ChengeNumPolygon();
}
//=========================================================================================================================================
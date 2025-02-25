//===============================================================================================
//
//�W���Q�R���F�O�Ղ����[meshorbit.cpp]
//Author:ShinaTaiyo
//
//===============================================================================================

//=============================================================
//�C���N���[�h
//=============================================================
#include "meshorbit.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
//===============================================================================================

//=============================================================
//�ÓI�����o�錾
//=============================================================
const string CMeshOrbit::m_aMESHORBIT_FILENAME[static_cast<int>(CMeshOrbit::MESHORBITTYPE::MAX)] =
{
	"data\\TEXTURE\\Shadow000.jpg",
	"data\\TEXTURE\\UI\\CutIn_DeathEnaga.png",
	"data\\TEXTURE\\UI\\AustorariaConquestUbaiman_000.png",
};
//=============================================================
//�R���X�g���N�^
//=============================================================
CMeshOrbit::CMeshOrbit(int nPri, bool bUseintPri, CObject::TYPE type, CObject::OBJECTTYPE ObjType) :CObject(nPri,bUseintPri,type,ObjType),m_mtxWorld(),m_Pos2(D3DXVECTOR3(0.0f,0.0f,0.0f)),m_Rot(D3DXVECTOR3(0.0f,0.0f,0.0f)),m_pTexture(nullptr),m_pVtxBuff(nullptr),
m_nTextureIndex(0),m_pIdxBuff(nullptr),m_Pos1(D3DXVECTOR3(0.0f,0.0f,0.0f)),m_type(MESHORBITTYPE::NORMAL)
{

}
//===============================================================================================

//=============================================================
//�f�X�g���N�^
//=============================================================
CMeshOrbit::~CMeshOrbit()
{

}
//===============================================================================================

//=============================================================
//����������
//=============================================================
HRESULT CMeshOrbit::Init()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nTOTAL_VERTEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	//�C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * m_nTOTAL_INDEX,//�C���f�b�N�X��
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	VERTEX_3D* pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�C���f�b�N�X�o�b�t�@�̃��b�N
	WORD* pIdx;
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	float fRatioX = 0.0f;
	float fRatioZ = 0.0f;
	int nCntX = 0;
	int nCntZ = 0;

	for (int nCnt = 0; nCnt < m_nTOTAL_VERTEX; nCnt++)
	{
		fRatioX = static_cast<float>(nCntX) / static_cast<float>(m_nNUMBLOCK_X);//���ݐݒ肵�Ă��钸�_�̈ʒu�̊��������߂�
		fRatioZ = static_cast<float>(nCntZ) / static_cast<float>(m_nNUMBLOCK_Z);//���ݐݒ肵�Ă��钸�_�̈ʒu�̊��������߂�

		pVtx[nCnt].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);//�ʒu��ݒ�

		//�@����ݒ�
		pVtx[nCnt].nor = D3DXVECTOR3(0.0f,0.0f,-1.0f);

		//���_�J���[�̐ݒ�
		pVtx[nCnt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[nCnt].tex = D3DXVECTOR2(fRatioX,fRatioZ);

		nCntX++;

		if (nCntX > m_nNUMBLOCK_X)
		{
			nCntX = 0;
			nCntZ++;
		}
		if (nCntZ > m_nNUMBLOCK_Z)
		{
			nCntZ = 0;
		}
	}

	int nCntIdxX = 0;
	int nCntArray = 0;//�z��J�E���g�p
	//=======================================================================
	//�C���f�b�N�X�����߂�
	//=======================================================================
	for (int nCntBlockZ = 0; nCntBlockZ < m_nNUMBLOCK_Z; nCntBlockZ++)
	{
		for (int nCntBlockX = 0; nCntBlockX < m_nNUMBLOCK_X + 1; nCntBlockX++)
		{
			pIdx[nCntArray] = static_cast<WORD>(m_nNUMVERTEX_X + nCntIdxX);  //�C���f�b�N�X����         6,7,8,9,10,
			pIdx[nCntArray + 1] = static_cast<WORD>(nCntIdxX);               //�u���b�NX�̍s�̏㑤��ݒ�   0,1,2,3,4,5,

			nCntIdxX++;
			nCntArray += 2;
		}

		if (nCntBlockZ != (m_nNUMBLOCK_Z - 1))
		{//�Ō�̍s�����͏k�ރ|���S�����v�Z���Ȃ�
			pIdx[nCntArray] = static_cast<WORD>(m_nNUMVERTEX_X * (nCntBlockZ + 1) - 1);//
			pIdx[nCntArray + 1] = static_cast<WORD>(m_nNUMVERTEX_X * (nCntBlockZ + 2));
			nCntArray += 2;
		}
	}
	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	//�C���f�b�N�X�o�b�t�@�̃A�����b�N
	m_pIdxBuff->Unlock();

	return S_OK;
}
//===============================================================================================

//=============================================================
//�I������
//=============================================================
void CMeshOrbit::Uninit()
{

	if (m_pVtxBuff != nullptr)
	{//���_�o�b�t�@�̔j��
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	if (m_pIdxBuff != nullptr)
	{//�C���f�b�N�X�o�b�t�@�̔j��
		m_pIdxBuff->Release();
		m_pIdxBuff = nullptr;
	}

	if (m_pTexture != nullptr)
	{//�e�N�X�`���ւ̃|�C���^��������
		m_pTexture = nullptr;
	}
}
//===============================================================================================

//=============================================================
//�X�V����
//=============================================================
void CMeshOrbit::Update()
{
	VERTEX_3D* pVtx;

	float fRatioVtx = 0.0f;
	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	//int nCntArray = m_nTOTAL_VERTEX - 1;//�Ō�̔z��ԍ�

	int nCol1 = 0;//���_�ԍ���P
	int nCol2 = 0;//���_�ԍ���Q

	//int nCntVtx = 0;//���_�v�Z�p

	float fDivisionPosY = m_Pos2.y - m_Pos1.y;

	fDivisionPosY /= m_nNUMBLOCK_Z;

	for (int nCntZ = 0; nCntZ < m_nNUMBLOCK_Z; nCntZ++)
	{
		//3�~3�̏ꍇ�A�P�T�ځF0.4�A�Q�T�ځF4,8�A3�T�ځF8,12�E�E�E
		pVtx[0 + m_nNUMVERTEX_X * nCntZ].pos = m_Pos2 - D3DXVECTOR3(0.0f,fDivisionPosY * nCntZ,0.0f);                             //�[�̃u���b�N�̏㑤�̍X�V���钸�_�����߂�
		pVtx[m_nNUMVERTEX_X + m_nNUMVERTEX_X * nCntZ].pos = m_Pos2 - D3DXVECTOR3(0.0f, fDivisionPosY * (nCntZ + 1), 0.0f);        //�[�̃u���b�N�̉����̍X�V���钸�_�����߂�

		nCol1 = m_nNUMBLOCK_X + (nCntZ * m_nNUMVERTEX_X);//��ōX�V����u���b�N�̏㑤�̗�̈�ԉE�[�̒��_�ԍ������߂�
		nCol2 = m_nNUMBLOCK_X + (nCntZ * m_nNUMVERTEX_X) + m_nNUMVERTEX_X;//��ōX�V����u���b�N�̉����̗�̈�ԉE�[�̒��_�ԍ������߂�
		for (int nCntX = 0; nCntX < m_nNUMBLOCK_X; nCntX++)
		{
			fRatioVtx = float(nCntX) / float(m_nNUMBLOCK_X);

			if (nCntZ == 0)
			{//�ŏ��̈�T�ڂȂ̂ŁA�P�u���b�N����ݒ�
				//�P�u���b�N���̏���ݒ肷��
				pVtx[nCol1].pos = pVtx[nCol1 - 1].pos;  //�I�t�Z�b�g�s�@�Z�P�F�i�Q�@���@�P�j�A�Z�Q�F�i�P�@���@�O�j
				pVtx[nCol2].pos = pVtx[nCol2 - 1].pos;  //���_�s �Z�P�F�i�T�@���@�S�j�A�Z�Q�F�i�S�@���@�R�j

				//�F����
				pVtx[nCol1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fRatioVtx);  //�I�t�Z�b�g�s�@�Z�P�F�i�Q�@���@�P�j�A�Z�Q�F�i�P�@���@�O�j
				pVtx[nCol2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fRatioVtx);  //���_�s �Z�P�F�i�T�@���@�S�j�A�Z�Q�F�i�S�@���@�R�j
			    if ( nCol1 >= m_nTOTAL_VERTEX || nCol2 >= m_nTOTAL_VERTEX || 
					nCol1 < 0 || nCol2 < 0)
		        {
					assert(false);
		        }

			}
			else
			{//�Q�T�ڈȍ~�Ȃ̂ŁA��̃u���b�N�ɂȂ��钸�_�̈ʒu��ݒ�(nCol1)�͂����g��Ȃ�
				pVtx[nCol2].pos = pVtx[nCol2 - 1].pos;  //���_�s �Z�P�F�i�T�@���@�S�j�A�Z�Q�F�i�S�@���@�R�j
				pVtx[nCol2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fRatioVtx);  //�I�t�Z�b�g�s�@�Z�P�F�i�Q�@���@�P�j�A�Z�Q�F�i�P�@���@�O�j
				if (nCol1 >= m_nTOTAL_VERTEX || nCol2 >= m_nTOTAL_VERTEX || 
					nCol1 < 0 || nCol2 < 0)
				{
					assert(false);
				}
			}

			nCol1--;
			nCol2--;
		}
	}
	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}
//===============================================================================================

//=============================================================
//�`�揈��
//=============================================================
void CMeshOrbit::Draw()
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�ւ̃|�C���^

    //�f�o�C�X�̎擾
	pDevice = CManager::GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;//�v�Z�p�}�g���b�N�X

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans,0.0f, 0.0f, 0.0f);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);


	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0,m_pVtxBuff, 0, sizeof(VERTEX_3D));//���_�o�b�t�@�ւ̃|�C���^�ƒ��_���̍\���̂̃T�C�Y

	//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(m_pIdxBuff);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTexture);

	//���ʂ�`�悷��
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//�A���t�@�e�X�g��L��(�A���t�@�l���O���傫���ꍇ�ɕ`�悷��j
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	//�|���S���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_nTOTAL_VERTEX, 0, m_nTOTAL_POLYGON);

	//�A���t�@�e�X�g�����ɖ߂�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	//�Жʂ����`�悷��
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}
//===============================================================================================

//=============================================================
//���S�t���O�ݒ菈��
//=============================================================
void CMeshOrbit::SetDeath()
{
	CObject::SetDeath();//���S�t���O�ݒ菈��
}
//===============================================================================================

//=============================================================
//�e�N�X�`�������蓖�Ă�
//=============================================================
void CMeshOrbit::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;//�e�N�X�`��
}
//===============================================================================================

//=============================================================
//��������
//=============================================================
CMeshOrbit* CMeshOrbit::Create(MESHORBITTYPE type)
{
	CMeshOrbit* pMeshOrbit = DBG_NEW CMeshOrbit;                                                            //�O�Ճ��b�V���𐶐�
	CTexture* pTextureClass = CManager::GetTexture();                                                       //�e�N�X�`���N���X���擾
	pMeshOrbit->Init();                                                                                     //����������
	pMeshOrbit->m_type = type;                                                                              //���
	pMeshOrbit->SetTextureIndex(pTextureClass->Regist(m_aMESHORBIT_FILENAME[static_cast<int>(type)]));      //�e�N�X�`�������蓖�Ă�
	pMeshOrbit->BindTexture(pTextureClass->GetAddress(pMeshOrbit->GetTextureIndex()));                      //�e�N�X�`�����Z�b�g����@
	pMeshOrbit->SetUseDeath(false);                                                                         //���S�t���O���g�p���Ȃ�
	return pMeshOrbit;
}
//===============================================================================================
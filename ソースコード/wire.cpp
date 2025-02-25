//===================================================================================================================
//
//�P�P���P�V���F���C���[�����[wire.cpp]
//Author:ShinaTaiyo
//
//===================================================================================================================

//===============================================================
//�C���N���[�h
//===============================================================
#include "wire.h"
#include "texture.h"
#include "renderer.h"
#include "player.h"
#include "manager.h"
#include "particle.h"
#include "debugtext.h"
#include "calculation.h"
//===================================================================================================================

//===============================================================
//�f�X�g���N�^
//===============================================================
const string CWire::s_WIRE_FILENAME[static_cast<int>(CWire::WIRETYPE::MAX)] =
{
	"data\\TEXTURE\\Wire\\LakeField_000.png",
	"data\\TEXTURE\\Wire\\Rope_000.png"
};
//===================================================================================================================

//===============================================================
//�R���X�g���N�^
//===============================================================
CWire::CWire(WIRETYPE WireType, float fRadius, float fHeight,int nNumDivsionXZ, 
	D3DXVECTOR3 Pos, D3DXVECTOR3 Rot, 
	int nNumDivisionY, int nPri, bool bUseintPri, CObject::TYPE type, CObject::OBJECTTYPE ObjType) : CMeshCylinder(fRadius,fHeight,nNumDivsionXZ,nNumDivisionY,
		Pos,Rot,
		nPri,bUseintPri,type,ObjType),m_Type(WireType),m_bUseUpdate(true),m_pWireHead(nullptr),m_pPlayer(nullptr)
{}
//===================================================================================================================

//===============================================================
//�f�X�g���N�^
//===============================================================
CWire::~CWire()
{

}
//===================================================================================================================

//===============================================================
//����������
//===============================================================
HRESULT CWire::Init()
{
	CMeshCylinder::Init();//���b�V���V�����_�[�̏���������

	//���C���[�w�b�h�𐶐�
	m_pWireHead = CWireHead::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), 120);//��������
	m_pWireHead->GetLifeInfo().SetAutoSubLife(false);//�̗͂������I�Ɍ��炳�Ȃ�
	m_pWireHead->SetUseDeath(false);                 //���S�t���O���g�p���Ȃ�
	m_pWireHead->GetDrawInfo().SetUseDraw(true);     //�`�������

	return S_OK;
}
//===================================================================================================================

//===============================================================
//�I������
//===============================================================
void CWire::Uninit()
{
	CMeshCylinder::Uninit();//���b�V���V�����_�[�̏I������
}
//===================================================================================================================

//===============================================================
//�X�V����
//===============================================================
void CWire::Update()
{
	CMeshCylinder::Update();                                                                                             //���b�V���V�����_�[�̍X�V����
	const D3DXVECTOR3& Pos = GetPos();                                                                                   //�ʒu
	const int& nNumDivisionXZ = GetNumDivisionXZ();                                                                      //XZ������
	const int& nNumDivisionY = GetNumDivisionY();                                                                        //Y������
	const int& nNumVtx = GetNumVtx();                                                                                    //���_��
	const int& nNumIdx = GetNumIdx();                                                                                    //�C���f�b�N�X��
	const float& fRadius = GetRadius();                                                                                  //���a
	const float& fHeight = GetHeight();                                                                                  //����
	float fLength = CCalculation::CalculationLength(m_pWireHead->GetPosInfo().GetPos(),m_pPlayer->GetPosInfo().GetPos());//���C���[�̓��ƃv���C���[�̋��������߂�
	SetHeight(fLength);                                                                                                  //��̍s�ŋ��߂�������ݒ肷��
	D3DXVECTOR3 Dir = m_pWireHead->GetPosInfo().GetPos() - m_pPlayer->GetPosInfo().GetPos();                             //���C���[�̓��ƃv���C���[�̃x�N�g�������߂�
	D3DXVec3Normalize(&Dir, &Dir);                                                                                       //�x�N�g���𐳋K������
	VERTEX_3D* pVtx;                                                                                                     //3D���_���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBufferPointer();                                                            //���_�o�b�t�@�ւ̃|�C���^���擾
	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR3 Up = { 0.0f,1.0f,0.0f };//��ƂȂ������x�N�g���i�O�ς̏����Ŏg���j

	if (std::abs(Dir.x * Up.x + Dir.y * Up.y + Dir.z * Up.z) > 0.99f) {
		Up = { 1.0f, 0.0f, 0.0f }; // ���s�ɋ߂��ꍇ�͕ʂ̊(X����)
	}

	if (m_bUseUpdate == true && m_pPlayer != nullptr)
	{
		int nCntArray = 0;                                        //�z�񐔂��J�E���g
		float fRatioXZ = 0.0f;                                    //XZ�����̃J�E���g���̊���
		float fRatioY = 0.0f;                                     //Y�����̃J�E���g���̊���
		D3DXVECTOR3 V1 = { 0.0f,0.0f,0.0f };                      //�x�N�g���P
		D3DXVECTOR3 V2 = { 0.0f,0.0f,0.0f };                      //�x�N�g���Q
		float fMeasureAngle = 0.0f;                               //�p�x�v�Z�p
		float fMeasureX = 0.0f;		                              //X�����̊p�x��
		float fMeasureY = 0.0f;		                              

		//�~��W�J���镽�ʂ����߂��x�N�g�����v�Z����
		D3DXVec3Cross(&V1, &Dir, &Up);//X����
		D3DXVec3Normalize(&V1, &V1);//�x�N�g���P�𐳋K��
		D3DXVec3Cross(&V2,&Dir,&V1);//Y����
		D3DXVec3Normalize(&V2, &V2);//�x�N�g���Q�𐳋K��

		//���_�̈ʒu�̐ݒ���J�n
		for (int nCntVtxY = 0; nCntVtxY < nNumDivisionY; nCntVtxY++)
		{//Y������UV�͂��̂܂܎g��
			fRatioY = (1.0f / (nNumDivisionY - 1)) * nCntVtxY;
			for (int nCntVtxXZ = 0; nCntVtxXZ < nNumDivisionXZ + 1; nCntVtxXZ++)
			{//X������UV�͏d�Ȃ钸�_������̂ŁA+ 1
				fRatioXZ = (1.0f / (nNumDivisionXZ)) * nCntVtxXZ;
				if (nCntVtxY == 0 && nCntVtxXZ == 0)
				{//�ŏ��̎��Ȃ̂Œ�ʂ̒��S�_��ݒ肷��
					pVtx[nCntArray].pos = m_pPlayer->GetPosInfo().GetPos();
					nCntArray++;
				}

				if (nCntVtxY == 0)
				{//�ŏ��̎��Ŋ�_�����߂�i9,8,7,6,5,4,3,2,1)
					fMeasureAngle = (D3DX_PI * 2) * fRatioXZ;//�p�x�̊������v�Z����
					fMeasureX = fRadius * sinf(fMeasureAngle);//��𐳂Ƃ��Ă���̂ŁAsinf��X
					fMeasureY = fRadius * cosf(fMeasureAngle);//��𐳂Ƃ��Ă���̂ŁAcosf��Y
					pVtx[nCntArray].pos = m_pPlayer->GetPosInfo().GetPos() + V1 * fMeasureX + V2 * fMeasureY;//sinf��cosf�ŕ��ʂ�X������Y�����ɐi�ޔ�����߂��̂ō����Ă���
				}
				else
				{//��_�ɑ΂��ċO�Օ��ɒ��_�������Ă���(18 = 27)
					//�~��ړI�̃x�N�g������ɕ��ʂɓW�J����_�͋��܂����̂ŁA���Ƃ͂��̃x�N�g���Ɍ������Ēl�𑫂��Ă�������
					D3DXVECTOR3 AdjustPos = Dir * fLength * fRatioY;
					pVtx[nCntArray].pos = pVtx[1 + nCntVtxXZ].pos + AdjustPos;
				}


				//�z��J�E���g
				nCntArray++;

				if (nCntArray >= nNumVtx)
				{//�z��O�`�F�b�N
					assert("�z��O�A�N�Z�X");
				}

				if (nCntArray < 0 || nCntArray >= nNumVtx)
				{//�z��O�A�N�Z�X�`�F�b�N
					assert(false);
				}

				if (nCntVtxY == nNumDivisionY - 1 && nCntVtxXZ == nNumDivisionXZ)
				{//�Ō�̒��_�Ȃ̂ŁA���S�_�����C���[�̓��ɐݒ�
					pVtx[nCntArray].pos = m_pWireHead->GetPosInfo().GetPos();//��ʂ̒��S�Ɉʒu��ݒ�
				}
			}
		}
	}

	//���_�o�b�t�@���A�����b�N���� 
	pVtxBuff->Unlock();

}
//===================================================================================================================

//===============================================================
//�`�揈��
//===============================================================
void CWire::Draw()
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = CManager::GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;//�v�Z�p�}�g���b�N�X
	D3DXMATRIX& mtxWorld = GetMtxWorld();
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBufferPointer();
	LPDIRECT3DINDEXBUFFER9 pIdxBuff = GetIdxBufferPointer();
	LPDIRECT3DTEXTURE9 pTexture = GetTexture();
	const int& nNumPolygon = GetNumPolygon();
	const int& nNumVtx = GetNumVtx();

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxWorld);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot,0.0f,0.0f,0.0f);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans,0.0f,0.0f,0.0f);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	//���ʂ�`�悷��
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, pVtxBuff, 0, sizeof(VERTEX_3D));//���_�o�b�t�@�ւ̃|�C���^�ƒ��_���̍\���̂̃T�C�Y

	//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(pIdxBuff);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, pTexture);

	if (GetUseDraw() == true)
	{
		//�|���S���̕`��
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, nNumVtx, 0, nNumPolygon);
	}
	//�Жʂ����`�悷��
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}
//===================================================================================================================

//===============================================================
//���S�t���O�ݒ菈��
//===============================================================
void CWire::SetDeath()
{
	if (GetUseDeath())
	{//���S�t���O���g�p����Ȃ�
		if (m_pWireHead != nullptr)
		{//���C���[�̓���j��
			m_pWireHead->SetUseDeath(true);//���S�t���O���g�p����
			m_pWireHead->SetDeath();       //���S�t���O��ݒ肷��
			m_pWireHead = nullptr;         //�|�C���^��������
		}
		if (m_pPlayer != nullptr)
		{//�v���C���[�ւ̃|�C���^������(���C���[����͊J�����Ȃ��j
			m_pPlayer = nullptr;
		}
	}
	CMeshCylinder::SetDeath();//���b�V���V�����_�[�̎��S�t���O�ݒ菈��
}
//===================================================================================================================

//===============================================================
//��������
//===============================================================
CWire* CWire::Create(WIRETYPE Type, D3DXVECTOR3 Pos, D3DXVECTOR3 Rot, float fRadius, float fHeight, int nNumDivisionXZ, int nNumDivisionY)
{
	CTexture* pTexture = CManager::GetTexture();                                                  //�e�N�X�`�����ւ̃|�C���^
	CWire* pWire = DBG_NEW CWire(Type, fRadius, fHeight, nNumDivisionXZ, Pos, Rot, nNumDivisionY);//���C���[�̐���
	pWire->Init();                                                                                //����������  
	pWire->SetTextureIndex(pTexture->Regist(s_WIRE_FILENAME[static_cast<int>(Type)]));            //�e�N�X�`����o�^���A�e�N�X�`���ԍ���ݒ�
	pWire->BindTexture(pTexture->GetAddress(pWire->GetTextureIndex()));                           //�e�N�X�`�������蓖�Ă�

	return pWire;
}
//===================================================================================================================
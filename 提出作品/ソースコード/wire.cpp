//===================================================================================================================
//
// �Q�O�Q�T�N�U���P�U���F�v�Z�p�N���X�𖼑O��ԂɕύX[wire.cpp]
// Author:ShinaTaiyo
//
//===================================================================================================================

//*******************************************************************************************************************
// �C���N���[�h
//*******************************************************************************************************************
#include "wire.h"      // ���C���[
#include "texture.h"   // �e�N�X�`��
#include "renderer.h"  // �����_���[
#include "wire_head.h" // ���C���[�w�b�h
#include "player.h"    // �v���C���[
#include "manager.h"   // �}�l�[�W���[
#include "debugtext.h" // �f�o�b�O�\��
#include "calculation.h" // �v�Z�p

//*******************************************************************************************************************
// �ÓI�����o�錾
//*******************************************************************************************************************

// ���C���[�̃e�N�X�`���t�@�C����
const string CWire::s_WIRE_FILENAME[static_cast<int>(CWire::WIRETYPE::MAX)] =
{
	"data\\TEXTURE\\Wire\\LakeField_000.png",
	"data\\TEXTURE\\Wire\\Rope_000.png"
};

//=====================================================================================================================
// 
// public�����o 
//
//=====================================================================================================================

//=====================================================================================================================
// �R���X�g���N�^
//=====================================================================================================================
CWire::CWire(WIRETYPE WireType, float fRadius, float fHeight,int nNumDivsionXZ,
	D3DXVECTOR3 Pos, D3DXVECTOR3 Rot, 
	int nNumDivisionY, int nPri, bool bUseintPri, CObject::TYPE type, CObject::OBJECTTYPE ObjType) : 
	CMeshCylinder(fRadius,fHeight,nNumDivsionXZ,nNumDivisionY,Pos,Rot,nPri,bUseintPri,type,ObjType),
	m_Type(WireType),
	m_bUseUpdate(true),
	m_pWireHead(nullptr),
	m_pPlayer(nullptr)
{
	// �����Ȃ�
}

//=====================================================================================================================
// �f�X�g���N�^
//=====================================================================================================================
CWire::~CWire()
{
	// �����Ȃ�
}

//=====================================================================================================================
// ����������
//=====================================================================================================================
HRESULT CWire::Init()
{
	//���C���[�w�b�h�𐶐�
	m_pWireHead = CWireHead::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), 120);//��������
	m_pWireHead->GetLifeInfo().SetAutoSubLife(false);//�̗͂������I�Ɍ��炳�Ȃ�
	m_pWireHead->SetUseDeath(false);                 //���S�t���O���g�p���Ȃ�
	m_pWireHead->GetDrawInfo().SetUseDraw(true);     //�`�������

	CMeshCylinder::Init();//���b�V���V�����_�[�̏���������

	return S_OK;
}

//=====================================================================================================================
// �I������
//=====================================================================================================================
void CWire::Uninit()
{
	CMeshCylinder::Uninit(); // ���b�V���V�����_�[�̏I������
}

//=====================================================================================================================
// �X�V����
//=====================================================================================================================
void CWire::Update()
{
	GenerateCylindricalMeshOnWire(); // ���C���[��Ƀ��b�V����W�J

	CMeshCylinder::Update(); // ���b�V���V�����_�[�̍X�V����
}

//=====================================================================================================================
// �`�揈��
//=====================================================================================================================
void CWire::Draw()
{
	LPDIRECT3DDEVICE9 pDevice; // �f�o�C�X�ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = CManager::GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;          // �v�Z�p�}�g���b�N�X
	D3DXMATRIX& mtxWorld = GetMtxWorld(); // ���[���h�}�g���b�N�X
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBufferPointer(); // ���_�o�b�t�@
	LPDIRECT3DINDEXBUFFER9 pIdxBuff = GetIdxBufferPointer();  // �C���f�b�N�X�o�b�t�@
	LPDIRECT3DTEXTURE9 pTexture = GetTexture(); // �e�N�X�`���ւ̃|�C���^
	const int& nNumPolygon = GetNumPolygon();   // �|���S����
	const int& nNumVtx = GetNumVtx(); // ���_��

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
	pDevice->SetStreamSource(0, pVtxBuff, 0, sizeof(VERTEX_3D));

	//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(pIdxBuff);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, pTexture);

	// �`�悷��Ȃ�
	if (GetUseDraw() == true)
	{
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, nNumVtx, 0, nNumPolygon);
	}

	//�Жʂ����`�悷��
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

//=====================================================================================================================
// ���S�t���O�ݒ菈��
//=====================================================================================================================
void CWire::SetDeath()
{
	// ���S�t���O���g�p����Ȃ�
	if (GetUseDeath())
	{
		// ���C���[�̓��I��
		if (m_pWireHead != nullptr)
		{
			m_pWireHead->SetUseDeath(true); // ���S�t���O���g�p����
			m_pWireHead->SetDeath();        // ���S�t���O��ݒ肷��
			m_pWireHead = nullptr;          // �|�C���^��������
		}

		// �v���C���[�I��
		if (m_pPlayer != nullptr)
		{
			m_pPlayer = nullptr; //�v���C���[�ւ̃|�C���^�������i���C���[����͔j�����܂���B�j
		}
	}

	CMeshCylinder::SetDeath(); // ���b�V���V�����_�[�̎��S�t���O�ݒ菈��
}

//=====================================================================================================================
// ���C���[�w�b�h�ւ̃|�C���^�擾
//=====================================================================================================================
CWireHead* CWire::GetWireHead()
{
	return m_pWireHead;
}

//=====================================================================================================================
// �v���C���[�ւ̃|�C���^�ݒ�
//=====================================================================================================================
void CWire::SetPlayerPointer(CPlayer* pPlayer)
{
	m_pPlayer = pPlayer;
}

//=====================================================================================================================
// �X�V���邩�ǂ����ݒ�
//=====================================================================================================================
void CWire::SetUseUpdate(bool bUse)
{
	m_bUseUpdate = bUse;
}

//=====================================================================================================================
// ��������
//=====================================================================================================================
CWire* CWire::Create(WIRETYPE Type, D3DXVECTOR3 Pos, D3DXVECTOR3 Rot, float fRadius, float fHeight, int nNumDivisionXZ, int nNumDivisionY)
{
	CTexture* pTexture = CManager::GetTexture(); // �e�N�X�`�����ւ̃|�C���^
	CWire* pWire = DBG_NEW CWire(Type, fRadius, fHeight, nNumDivisionXZ, Pos, Rot, nNumDivisionY); // �C���X�^���X����
	pWire->Init(); // ����������  
	pWire->SetTextureIndex(pTexture->Regist(s_WIRE_FILENAME[static_cast<int>(Type)])); // �e�N�X�`����o�^���A�e�N�X�`���ԍ���ݒ�
	pWire->BindTexture(pTexture->GetAddress(pWire->GetTextureIndex())); // �e�N�X�`�������蓖�Ă�
	return pWire;
}

//=====================================================================================================================
// ���C���[��Ƀ��b�V����W�J���鏈��
//=====================================================================================================================
void CWire::GenerateCylindricalMeshOnWire()
{
	// === �����Ɏg�p�������錾�A������ ===

	const D3DXVECTOR3& Pos = GetPos(); // �ʒu
	const int& nNumDivisionXZ = GetNumDivisionXZ(); // XZ������
	const int& nNumDivisionY = GetNumDivisionY(); // Y������
	const int& nNumVtx = GetNumVtx();   // ���_��
	const int& nNumIdx = GetNumIdx();   // �C���f�b�N�X��
	const float& fRadius = GetRadius(); // ���a
	const float& fHeight = GetHeight(); // ����

	// === ���C���[��Ƀ��b�V����W�J���鏈�����J�n ===

	// ���C���[�̓��ƃv���C���[�̋��������߂�
	float fLength = Calculation::Length::ToGoalPos(m_pWireHead->GetPosInfo().GetPos(), m_pPlayer->GetPosInfo().GetPos());
	SetHeight(fLength); // ��̍s�ŋ��߂�������ݒ肷��i���� = ����)
	D3DXVECTOR3 Dir = m_pWireHead->GetPosInfo().GetPos() - m_pPlayer->GetPosInfo().GetPos(); // ���C���[�̓��ƃv���C���[�̃x�N�g�������߂�
	D3DXVec3Normalize(&Dir, &Dir); // �x�N�g���𐳋K������
	VERTEX_3D* pVtx; // 3D���_���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBufferPointer(); // ���_�o�b�t�@�ւ̃|�C���^���擾

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR3 Up = { 0.0f,1.0f,0.0f }; // ��ƂȂ������x�N�g���i�O�ς̏����Ŏg���j

	// ���s�ɋ߂��ꍇ�͕ʂ̊(X����)�i�ǂ���̊�x�N�g���ł����ʂɑ傫�ȍ��͂Ȃ��j
	if (std::abs(Dir.x * Up.x + Dir.y * Up.y + Dir.z * Up.z) > 0.99f) {
		Up = { 1.0f, 0.0f, 0.0f };
	}

	// �X�V���邩�v���C���[�ւ̃|�C���^�����݂���Ȃ烏�C���[��ɒ��_��W�J���鏈�����J�n
	if (m_bUseUpdate == true && m_pPlayer != nullptr)
	{
		// === �����Ɏg�p�������錾�A������ ===
		int nCntArray = 0;     // �z�񐔂��J�E���g
		float fRatioXZ = 0.0f; // XZ�����̃J�E���g���̊���
		float fRatioY = 0.0f;  // Y�����̃J�E���g���̊���
		D3DXVECTOR3 V1 = { 0.0f,0.0f,0.0f }; // �x�N�g���P
		D3DXVECTOR3 V2 = { 0.0f,0.0f,0.0f }; // �x�N�g���Q
		float fMeasureAngle = 0.0f;  // �p�x�v�Z�p
		float fMeasureX = 0.0f; // X�����̊p�x
		float fMeasureY = 0.0f; // Y�����̊p�x

		// �~��W�J���镽�ʂ����߂��x�N�g�����v�Z����
		D3DXVec3Cross(&V1, &Dir, &Up); // X����
		D3DXVec3Normalize(&V1, &V1);   // �x�N�g���P�𐳋K��
		D3DXVec3Cross(&V2, &Dir, &V1); // Y����
		D3DXVec3Normalize(&V2, &V2);   // �x�N�g���Q�𐳋K��

		// Y�����̒��_
		for (int nCntVtxY = 0; nCntVtxY < nNumDivisionY; nCntVtxY++)
		{
			// Y�������_����
			fRatioY = (1.0f / (nNumDivisionY - 1)) * nCntVtxY;

			// X�����̒��_�i�d�Ȃ蕔��������̂Ł{�P�j
			for (int nCntVtxXZ = 0; nCntVtxXZ < nNumDivisionXZ + 1; nCntVtxXZ++)
			{
				// X�������_����
				fRatioXZ = (1.0f / (nNumDivisionXZ)) * nCntVtxXZ;

				// �ŏ��̎��Ȃ̂Œ�ʂ̒��S�_��ݒ肷��
				if (nCntVtxY == 0 && nCntVtxXZ == 0)
				{
					pVtx[nCntArray].pos = m_pPlayer->GetPosInfo().GetPos();
					nCntArray++;
				}

				// �ŏ��̎��Ŋ�_�����߂�i9,8,7,6,5,4,3,2,1)
				if (nCntVtxY == 0)
				{
					fMeasureAngle = (D3DX_PI * 2) * fRatioXZ;  // �p�x�̊������v�Z����
					fMeasureX = fRadius * sinf(fMeasureAngle); // ��𐳂Ƃ��Ă���̂ŁAsinf��X
					fMeasureY = fRadius * cosf(fMeasureAngle); // ��𐳂Ƃ��Ă���̂ŁAcosf��Y

					// �~����̔�ɊO�ςŋ��߂��x�N�g�����|���ă��C���[��\��
					pVtx[nCntArray].pos = m_pPlayer->GetPosInfo().GetPos() + V1 * fMeasureX + V2 * fMeasureY;
				}
				// ��_�ɑ΂��ċO�Օ��ɒ��_�������Ă���(18 = 27)
				else
				{
					// �~��ړI�̃x�N�g������ɕ��ʂɓW�J����_�͋��܂����̂ŁA���Ƃ͂��̃x�N�g���Ɍ������Ēl�𑫂��Ă�������
					D3DXVECTOR3 AdjustPos = Dir * fLength * fRatioY;
					pVtx[nCntArray].pos = pVtx[1 + nCntVtxXZ].pos + AdjustPos;
				}

				//�z��J�E���g
				nCntArray++;

				// �z��O�`�F�b�N
				if (nCntArray >= nNumVtx)
				{
					assert(false && "�z��O�A�N�Z�X");
				}

				// �z��O�`�F�b�N
				if (nCntArray < 0 || nCntArray >= nNumVtx)
				{
					assert(false && "�z��O�A�N�Z�X");
				}

				// �Ō�̒��_�Ȃ̂ŁA���S�_�����C���[�̓��ɐݒ�
				if (nCntVtxY == nNumDivisionY - 1 && nCntVtxXZ == nNumDivisionXZ)
				{
					pVtx[nCntArray].pos = m_pWireHead->GetPosInfo().GetPos(); // ��ʂ̒��S�Ɉʒu��ݒ�
				}
			}
		}
	}

	// ���_�o�b�t�@���A�����b�N���� 
	pVtxBuff->Unlock();
}

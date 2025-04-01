//===========================================================================================
//
//�U���S���F���f����`�悷��[objectX.cpp]
//Author:ShinaTaiyo
//
//===========================================================================================

//===============================
//�C���N���[�h
//===============================
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "objectX.h"
#include "calculation.h"
#include "manager.h"
#include "camera.h"
#include "input.h"
#include "debugtext.h"
#include <stdio.h>
#include <string.h>
#include <iostream>
#include "collision.h"
#include "particle.h"
#include "damage.h"
#include <algorithm>
//===========================================================================================

//================================================
//�ÓI�����o�錾
//================================================
bool CObjectX::s_bCOMMON_DRAWSHADOW = true;

//================================================
//�R���X�g���N�^
//================================================
CObjectX::CObjectX(int nPri, bool bUseintPri, CObject::TYPE type, CObject::OBJECTTYPE ObjType) : CObject(nPri, bUseintPri, type, ObjType)
, m_ObjectXInfo(), m_nIndexObjectX(0), m_nManagerType(0), m_nObjXType(OBJECTXTYPE_BLOCK), m_fAxis(0.0f), m_VecAxis(D3DXVECTOR3(0.0f, 1.0f, 0.0f)),
m_nTypeNum(0), m_PosInfo({}), m_DrawInfo({}), m_RotInfo({}), m_SizeInfo({}), m_MoveInfo({}), m_LifeInfo({}), m_CollisionInfo({}),
m_bExtrusionCollisionSquareX(false),m_bExtrusionCollisionSquareY(false),m_bExtrusionCollisionSquareZ(false),m_bIsLanding(false)
{
	SetObjectType(CObject::OBJECTTYPE::OBJECTTYPE_X);//�I�u�W�F�N�g�^�C�v�ݒ�
	D3DXMatrixIdentity(&m_DrawInfo.GetMatrixWorld());        //���[���h�}�g���b�N�X��������
}
//================================================================================================================================================

//================================================
//�f�X�g���N�^
//================================================
CObjectX::~CObjectX()
{

}
//================================================================================================================================================

//================================================
//����������
//================================================
HRESULT CObjectX::Init()
{
	CObject::Init();//�I�u�W�F�N�g�̏���������

	return S_OK;
}
//================================================================================================================================================

//================================================
//�I������
//================================================
void CObjectX::Uninit()
{
	if (m_ObjectXInfo.Diffuse != nullptr)
	{
		delete[] m_ObjectXInfo.Diffuse;//�F�����̓��I���������J��
		m_ObjectXInfo.Diffuse = nullptr;
	}

	if (m_ObjectXInfo.pTexture != nullptr)
	{
		delete[] m_ObjectXInfo.pTexture;//�e�N�X�`���̓��I�����������
		m_ObjectXInfo.pTexture = nullptr;
	}

	if (m_ObjectXInfo.FormarDiffuse != nullptr)
	{
		delete[] m_ObjectXInfo.FormarDiffuse;//���̐F�����̓��I���������J��
		m_ObjectXInfo.FormarDiffuse = nullptr;
	}

	if (m_ObjectXInfo.pMesh != nullptr)
	{//���b�V���ւ̃|�C���^��������
		m_ObjectXInfo.pMesh = nullptr;
	}

	if (m_ObjectXInfo.pBuffMat != nullptr)
	{//�}�e���A���ւ̃|�C���^��������
		m_ObjectXInfo.pBuffMat = nullptr;
	}

	CObject::Uninit();//�I�u�W�F�N�g�̏I������
}
//================================================================================================================================================

//================================================
//�X�V����
//================================================
void CObjectX::Update()
{
	CalculateSenterPos();                        //���f���̒��S�_�����߂�
										         
	m_SizeInfo.AddScaleProcess();                //�g�嗦���Z����
	m_SizeInfo.MultiScaleProcess();              //�g�嗦��Z����
	m_SizeInfo.DecideVtxMaxMinProcess();         //���_�̍ő�ŏ������߂鏈��
										         
	m_DrawInfo.ChengeColorProcess(this);         //�F��ς��鏈��
	if (m_RotInfo.GetUseAddRot() == true)
	{//�����̉��Z����
		m_RotInfo.Rot += m_RotInfo.AddRot;
	}

	m_LifeInfo.AutoSubLifeProcess();             //�̗͂������I�Ɍ��炷����
	m_LifeInfo.AutoDeathProcess(this);           //�̗͂��O�ɂȂ����ꍇ�Ɏ��S�t���O�𔭓����鏈��
	m_LifeInfo.HitStopProcess();                 //���G���Ԃ��J�E���g���鏈��
	m_LifeInfo.RatioLifeAlphaColorProcess(this); //�̗͂̊����ɉ����ē����x��ς��鏈��

	m_MoveInfo.MultiSpeedProcess();              //��Z��������
	m_MoveInfo.AddSpeedProcess();                //��������
	m_MoveInfo.GravityProcess();                 //�d�͏���

	UpdatePos();                                 //�ʒu�̍X�V���s��

	CObject::Update();                           //�I�u�W�F�N�g�X�V����
}
//================================================================================================================================================

//================================================
//�`�揈��
//================================================
void CObjectX::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice(); //�f�o�C�X�ւ̃|�C���^�Ǝ擾
	D3DXMATRIX mtxRot, mtxTrans, mtxScale;                            //�v�Z�p�}�g���b�N�X
	D3DXQUATERNION quat;                                              //�N�H�[�^�j�I��
	D3DXVECTOR3 vecAxis = D3DXVECTOR3(0.0f, 0.0, 1.0f);               //��]��
	D3DMATERIAL9 matDef;                                              //���݂̃}�e���A���ۑ��p
	D3DXMATERIAL* pMat;                                               //�}�e���A���f�[�^�ւ̃|�C���^

	if (m_DrawInfo.bUseShadow == true && s_bCOMMON_DRAWSHADOW == true)
	{//�e��`�悷��Ȃ�
		DrawShadow();
	}
	//���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_DrawInfo.mtxWorld);

	//�傫���𔽉f
	D3DXMatrixScaling(&mtxScale, m_SizeInfo.Scale.x, m_SizeInfo.Scale.y, m_SizeInfo.Scale.z);
	D3DXMatrixMultiply(&m_DrawInfo.mtxWorld, &m_DrawInfo.mtxWorld, &mtxScale);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_RotInfo.Rot.y, m_RotInfo.Rot.x, m_RotInfo.Rot.z);
	D3DXMatrixMultiply(&m_DrawInfo.mtxWorld, &m_DrawInfo.mtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_PosInfo.Pos.x, m_PosInfo.Pos.y, m_PosInfo.Pos.z);
	D3DXMatrixMultiply(&m_DrawInfo.mtxWorld, &m_DrawInfo.mtxWorld, &mtxTrans);

	//�e�}�g���b�N�X���ݒ肳��Ă����ꍇ
	if (m_DrawInfo.bUseMatrixChild == true && m_DrawInfo.pMtxParent != nullptr)
	{
		D3DXMatrixMultiply(&m_DrawInfo.mtxWorld, &m_DrawInfo.mtxWorld,m_DrawInfo.pMtxParent);
	}

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_DrawInfo.mtxWorld);

	//���[���h���W�����߂�
	D3DXVECTOR3 PosZero = { 0.0f,0.0f,0.0f };
	D3DXVec3TransformCoord(&m_PosInfo.WorldPos, &PosZero, &m_DrawInfo.mtxWorld);


	//=======================================
	//�`��̒���
	//=======================================
	if (m_DrawInfo.Color.a < 1.0f)
	{
    	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);//TRUE���Ɠ����x�Ɣw�i�̃u�����h���s����AFALSE���Ɗ��S�ɕs�����ȕ`��ɂȂ�
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);     //�`�悷��I�u�W�F�N�g�̐F�̉e���x�����߂�,D3DBLEND_SRCALPHA�F(R,G,B) * a(�I�u�W�F�N�g�̓����x�ɉ������F)
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA); //���łɕ`�悳��Ă���w�i�F�̉e���x�����߂�,D3DBLEND_INVSRCALPHA:(R,G,B)�~(1 - a)<�w�i�̐F���c������>
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);             //���s�����������܂Ȃ�
	}
	else
	{//�s�����I�u�W�F�N�g��ʏ�ʂ�`�悷��
	//�A���t�@�e�X�g��L��(�A���t�@�l���O���傫���ꍇ�ɕ`�悷��j
		pDevice->SetRenderState(D3DRS_ALPHAREF, 0);                     //�����x��0�ȏ�̎��ɕ`��
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);       //�����x���O���傫���s�N�Z���̂ݕ`��
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);              //Z�o�b�t�@�ɏ�������
	}

	//�@���̒������P�ɂ���B�i�X�P�[���Ȃǂ��g�������́A�K���g���B)
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

	if (m_DrawInfo.bUseCulling == true)
	{
		//���ʂ�`�悷��
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	}
	//================================================================================================================

	if (m_DrawInfo.bUseDraw == true)
	{//�`�������Ȃ�
		if (m_ObjectXInfo.pBuffMat != nullptr && m_ObjectXInfo.pMesh != nullptr)
		{//�}�e���A���ƃ��b�V�������݂��Ă�����
			//�}�e���A���ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)m_ObjectXInfo.pBuffMat->GetBufferPointer();

			//==========================================================================
			//�}�e���A���̐����A�e�N�X�`����ǂݍ��ށB
			//==========================================================================
			for (int nCntMat = 0; nCntMat < (int)m_ObjectXInfo.dwNumMat; nCntMat++)
			{
				//�F�����̐ݒ�
				pMat[nCntMat].MatD3D.Diffuse = m_ObjectXInfo.Diffuse[nCntMat];

				//�}�e���A���̐ݒ�
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//�e�N�X�`����ݒ肷��
				pDevice->SetTexture(0, m_ObjectXInfo.pTexture[nCntMat]);

				//���f���i�p�[�c�j�̕`��
				m_ObjectXInfo.pMesh->DrawSubset(nCntMat);
			}
			//================================================================================================================
		}
	}

	//=======================================
	//�`��̒��������ɖ߂�
	//=======================================
	//�A���t�@�e�X�g�����ɖ߂�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	//�@���̒������P�ɂ���B�i�X�P�[���Ȃǂ��g�������́A�K���g���B)
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);

	//Z�o�b�t�@�ɏ�������
    pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	//�Жʂ����`�悷��
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	//================================================================================================================

	//�ۑ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);

	CObject::Draw();//�I�u�W�F�N�g�̕`�揈��

}
//================================================================================================================================================

//================================================
//���S�t���O�ݒ菈��
//================================================
void CObjectX::SetDeath()
{
	CObject::SetDeath();//�I�u�W�F�N�g�̎��S�t���O�ݒ菈��
}
//================================================================================================================================================

//================================================
//���f���������蓖�Ă�
//================================================
void CObjectX::BindObjectXInfo(LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD dwNumMat, LPDIRECT3DTEXTURE9* pTexture, D3DCOLORVALUE* pDiffuse)
{
	m_ObjectXInfo.pMesh = pMesh;                                       //���_���̐ݒ�
	m_ObjectXInfo.pBuffMat = pBuffMat;                                 //�}�e���A���̐ݒ�
	m_ObjectXInfo.dwNumMat = dwNumMat;                                 //�}�e���A���̐���ݒ�
	m_ObjectXInfo.pTexture = DBG_NEW LPDIRECT3DTEXTURE9[int(dwNumMat)];//�}�e���A���̓��I���������m��
	m_ObjectXInfo.Diffuse = DBG_NEW D3DCOLORVALUE[int(dwNumMat)];      //�}�e���A���̓��I���������m��
	m_ObjectXInfo.FormarDiffuse = DBG_NEW D3DCOLORVALUE[int(dwNumMat)];//���̃}�e���A���̓��I���������m��
	for (int nCnt = 0; nCnt < (int)(dwNumMat); nCnt++)
	{//�}�e���A���̐������ꂼ��̊֘A����ݒ�
		m_ObjectXInfo.pTexture[nCnt] = pTexture[nCnt];
		m_ObjectXInfo.Diffuse[nCnt] = pDiffuse[nCnt];
		m_ObjectXInfo.FormarDiffuse[nCnt] = pDiffuse[nCnt];
	}
}
//================================================================================================================================================

//================================================
//�F������ݒ肷��
//================================================
void CObjectX::SetColor(D3DXCOLOR col, int nColChengeTime, bool bChoose, bool bSetAlpha, bool bBlinking)
{
	m_DrawInfo.bColorChenge = true;               //�F��ς��邩�ǂ���
	m_DrawInfo.nChengeColorTime = nColChengeTime; //�F��ς��鎞��
	m_DrawInfo.Color = col;                       //�F
	m_DrawInfo.bBlinkingColor = bBlinking;        //�_��
	if (bChoose == true)
	{//�����x�������g�p���邩��I��
		if (bSetAlpha == false)
		{//�����x�ȊO��ݒ�
			for (int nCnt = 0; nCnt < (int)(m_ObjectXInfo.dwNumMat); nCnt++)
			{
				m_ObjectXInfo.Diffuse[nCnt].r = col.r;
				m_ObjectXInfo.Diffuse[nCnt].g = col.g;
				m_ObjectXInfo.Diffuse[nCnt].b = col.b;
			}
		}
		else
		{
			for (int nCnt = 0; nCnt < (int)(m_ObjectXInfo.dwNumMat); nCnt++)
			{//�����x������ݒ�

				m_ObjectXInfo.Diffuse[nCnt].a = col.a;
			}
		}
	}
	else
	{//���ʂɐF��ݒ肷��
		for (int nCnt = 0; nCnt < (int)(m_ObjectXInfo.dwNumMat); nCnt++)
		{//�F������ݒ肷��
			m_ObjectXInfo.Diffuse[nCnt] = col;
		}
	}
}
//================================================================================================================================================

//================================================
//�_���[�W��^���鏈��
//================================================
void CObjectX::SetDamage(int nDamage, int nHitStopTime)
{
	if (m_LifeInfo.bHitStop == false && nDamage > 0)
	{//�q�b�g�X�g�b�v��Ԃ���Ȃ����
		m_LifeInfo.bHitStop = true;              //�q�b�g�X�g�b�v��Ԃɂ���
		m_LifeInfo.nHitStopTime = nHitStopTime;  //�q�b�g�X�g�b�v����
		CDamage::Create(nDamage, GetPosInfo().GetSenterPos(), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 30.0f + nDamage * 1.0f, 30.0f + nDamage * 1.0f);//�_���[�W�̐���
		m_LifeInfo.nLife -= nDamage;//�̗͂����炷
	}
}
//================================================================================================================================================

//================================================
//���S�_�����߂�
//================================================
void CObjectX::CalculateSenterPos()
{
	m_PosInfo.SenterPos = m_PosInfo.Pos;//���S�_�����߂�
	m_PosInfo.SenterPos.y += (m_SizeInfo.VtxMax.y + m_SizeInfo.VtxMin.y) / 2;//�ő咸�_�ƍŏ����_�̋��������߁A���̔����𑫂�
}
//================================================================================================================================================

//================================================
//���̐F�����ɖ߂�
//================================================
void CObjectX::SetFormarColor()
{
	m_DrawInfo.Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//�F������1�ɂ���
	for (int nCnt = 0; nCnt < (int)(m_ObjectXInfo.dwNumMat); nCnt++)
	{//�ŏ��ɕۑ������F�����Ɍ��݂̐F������ݒ�
		m_ObjectXInfo.Diffuse[nCnt] = m_ObjectXInfo.FormarDiffuse[nCnt];
	}
}
//================================================================================================================================================

//================================================
//�F�����u�����v��ݒ肷��iSetColor�ɂ���@�\���g��Ȃ�)
//================================================
void CObjectX::SetOnlyColor(D3DXCOLOR Col)
{
	for (int nCnt = 0; nCnt < (int)(m_ObjectXInfo.dwNumMat); nCnt++)
	{
		m_ObjectXInfo.Diffuse[nCnt] = m_DrawInfo.Color;
	}
}
//================================================================================================================================================

//================================================
//���̐F�����Ɂu�����v�ݒ肷��iSetColor�ɂ���@�\���g��Ȃ�)
//================================================
void CObjectX::SetOnlyFormarColor()
{
	for (int nCnt = 0; nCnt < (int)(m_ObjectXInfo.dwNumMat); nCnt++)
	{
		m_ObjectXInfo.Diffuse[nCnt] = m_ObjectXInfo.FormarDiffuse[nCnt];
	}
}
//================================================================================================================================================

//================================================
//�ʒu�̍X�V
//================================================
void CObjectX::UpdatePos()
{
	if (m_MoveInfo.bUseUpdatePos == true)
	{//�ʒu�̍X�V������Ȃ�
		const D3DXVECTOR3& Pos = GetPosInfo().GetPos();

		//1f�O�̈ʒu��ݒ�
		GetPosInfo().SetPosOld(Pos);

		//�����̏���
		if (m_MoveInfo.bUseInteria == true)
		{
			m_MoveInfo.Move.x += (0.0f - m_MoveInfo.Move.x) * m_MoveInfo.fInertia;
			m_MoveInfo.Move.z += (0.0f - m_MoveInfo.Move.z) * m_MoveInfo.fInertia;
		}

		//�ʒu�̐ݒ�
		GetPosInfo().SetPos(Pos + m_MoveInfo.Move);

		//1f��̈ʒu�̐ݒ�
		GetPosInfo().SetPosFuture(Pos + m_MoveInfo.Move);
	}
}
//================================================================================================================================================

//================================================
//�T�C�Y��ݒ肷��
//================================================
void CObjectX::SetSize()
{
	BYTE* pVtxBuff;//���_�o�b�t�@�ւ̃|�C���^

	//���_���̎擾
	int nNumVtx = m_ObjectXInfo.pMesh->GetNumVertices();

	//���_�t�H�[�}�b�g�̃T�C�Y���擾
	DWORD sizeFVF = D3DXGetFVFVertexSize(m_ObjectXInfo.pMesh->GetFVF());

	//���_�o�b�t�@�̃��b�N
    m_ObjectXInfo.pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		//���_���W�̑��
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

		//====================================================
		//���_���W���r���ă��f���̍ŏ��l�ő�l���擾
		//====================================================
		if (vtx.x > m_SizeInfo.OriginVtxMax.x)
		{
			m_SizeInfo.OriginVtxMax.x = vtx.x;
		}
		else if (vtx.y > m_SizeInfo.OriginVtxMax.y)
		{//����ǂݍ��񂾒��_���A��ԑ傫�����_���傫���ꍇ
			m_SizeInfo.OriginVtxMax.y = vtx.y;
		}
		else if (vtx.z > m_SizeInfo.OriginVtxMax.z)
		{//����ǂݍ��񂾒��_���A��ԑ傫�����_���傫���ꍇ
			m_SizeInfo.OriginVtxMax.z = vtx.z;
		}
		else if (vtx.x < m_SizeInfo.OriginVtxMin.x)
		{//����ǂݍ��񂾒��_���A��ԏ��������_��菬�����ꍇ
			m_SizeInfo.OriginVtxMin.x = vtx.x;
		}
		else if (vtx.y < m_SizeInfo.OriginVtxMin.y)
		{//����ǂݍ��񂾒��_���A��ԏ��������_��菬�����ꍇ
			m_SizeInfo.OriginVtxMin.y = vtx.y;
		}
		else if (vtx.z < m_SizeInfo.OriginVtxMin.z)
		{//����ǂݍ��񂾒��_���A��ԏ��������_��菬�����ꍇ
			m_SizeInfo.OriginVtxMin.z = vtx.z;
		}
		//=============================================================================================================

		//���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
		pVtxBuff += sizeFVF;

	}
	//���_�o�b�t�@�̃A�����b�N
	m_ObjectXInfo.pMesh->UnlockVertexBuffer();

	//==========================================================
	//��̏����ŏo�����ŏ��ő�̒��_���g�嗦�Ōv�Z������
	//==========================================================
	m_SizeInfo.VtxMax.x = m_SizeInfo.OriginVtxMax.x * m_SizeInfo.Scale.x;
	m_SizeInfo.VtxMax.y = m_SizeInfo.OriginVtxMax.y * m_SizeInfo.Scale.y;
	m_SizeInfo.VtxMax.z = m_SizeInfo.OriginVtxMax.z * m_SizeInfo.Scale.z;
	m_SizeInfo.VtxMin.x = m_SizeInfo.OriginVtxMin.x * m_SizeInfo.Scale.x;
	m_SizeInfo.VtxMin.y = m_SizeInfo.OriginVtxMin.y * m_SizeInfo.Scale.y;
	m_SizeInfo.VtxMin.z = m_SizeInfo.OriginVtxMin.z * m_SizeInfo.Scale.z;
	//================================================================================================================================================

	//==========================================================
	//�ő�ŏ����Q�Ƃ��ăT�C�Y��ݒ肷��
	//==========================================================
	m_SizeInfo.Size = m_SizeInfo.VtxMax - m_SizeInfo.VtxMin;
	//================================================================================================================================================

}
//================================================================================================================================================

//===================================================================================================================
//�G�f�B�^����g�嗦��ύX����
//===================================================================================================================
void CObjectX::ChengeEditScale()
{
	D3DXVECTOR3& Scale = m_SizeInfo.GetScale();//�g�嗦

	ChengeEditScaleX();//�g�嗦X

	ChengeEditScaleY();//�g�嗦Y

	ChengeEditScaleZ();//�g�嗦Z

	//�f�o�b�O�\��
	CManager::GetDebugText()->PrintDebugText("�g�嗦(RTY�L�[) %f %f %f\n", Scale.x,Scale.y,Scale.z);
}
//================================================================================================================================================

//===================================================================================================================
//�G�f�B�^����g�嗦X��ύX����
//===================================================================================================================
void CObjectX::ChengeEditScaleX()
{
	D3DXVECTOR3& Scale = m_SizeInfo.GetScale();                                  //�g�嗦

	if (CManager::GetInputKeyboard()->GetPress(DIK_LCONTROL) == true)
	{//L�R���g���[���L�[�������Ȃ���i�l��傫���ϓ�������)
		if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
		{//�V�t�g�L�[�������Ȃ���E�E�E
			if (CManager::GetInputKeyboard()->GetTrigger(DIK_R) == true)
			{
				Scale.x -= 10.0f;
			}
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_R) == true)
		{
			Scale.x += 10.0f;
		}
	}
	else
	{//L�R���g���[���L�[�������Ă��Ȃ��i�l���������ϓ�������)
		if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
		{//�V�t�g�L�[�������Ȃ���E�E�E
			if (CManager::GetInputKeyboard()->GetTrigger(DIK_R) == true)
			{
				Scale.x -= 0.1f;
			}
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_R) == true)
		{
			Scale.x += 0.1f;
		}
	}

}
//================================================================================================================================================

//===================================================================================================================
//�G�f�B�^����g�嗦Y��ύX����
//===================================================================================================================
void CObjectX::ChengeEditScaleY()
{
	D3DXVECTOR3& Scale = m_SizeInfo.GetScale();                                //�g�嗦

	if (CManager::GetInputKeyboard()->GetPress(DIK_LCONTROL) == true)
	{//L�R���g���[���L�[�������Ȃ���i�l��傫���ϓ�������)
		if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
		{//�V�t�g�L�[�������Ȃ���E�E�E
			if (CManager::GetInputKeyboard()->GetTrigger(DIK_T) == true)
			{
				Scale.y -= 10.0f;
			}
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_T) == true)
		{
			Scale.y += 10.0f;
		}
	}
	else
	{//L�R���g���[���L�[�������Ă��Ȃ��i�l���������ϓ�������)
		if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
		{//�V�t�g�L�[�������Ȃ���E�E�E
			if (CManager::GetInputKeyboard()->GetTrigger(DIK_T) == true)
			{
				Scale.y -= 0.1f;
			}
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_T) == true)
		{
			Scale.y += 0.1f;
		}
	}
}
//================================================================================================================================================

//===================================================================================================================
//�G�f�B�^����g�嗦Z��ύX����
//===================================================================================================================
void CObjectX::ChengeEditScaleZ()
{
	D3DXVECTOR3& Scale = m_SizeInfo.GetScale();                                //�g�嗦
	if (CManager::GetInputKeyboard()->GetPress(DIK_LCONTROL) == true)
	{//L�R���g���[���L�[�������Ȃ���i�l��傫���ϓ�������)
		if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
		{//�V�t�g�L�[�������Ȃ���E�E�E
			if (CManager::GetInputKeyboard()->GetTrigger(DIK_Y) == true)
			{
				Scale.z -= 10.0f;
			}
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_Y) == true)
		{
			Scale.z += 10.0f;
		}
	}
	else
	{//L�R���g���[���L�[�������Ă��Ȃ��i�l���������ϓ�������)
		if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
		{//�V�t�g�L�[�������Ȃ���E�E�E
			if (CManager::GetInputKeyboard()->GetTrigger(DIK_Y) == true)
			{
				Scale.z -= 0.1f;
			}
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_Y) == true)
		{
			Scale.z += 0.1f;
		}
	}

}
//================================================================================================================================================

//===================================================================================================================
//�ʒu���ړ�������
//===================================================================================================================
void CObjectX::ChengeEditPos()
{
	D3DXVECTOR3 Move = D3DXVECTOR3(0.0f,0.0f,0.0f);
	SetColor(D3DXCOLOR(1.0f,0.0f,0.0f,0.5f),3,true,true,false);           //�F�𔼓����ɂ���
	CInputKeyboard* pInput = CManager::GetInputKeyboard();
	//===========================
	//�ʒu���x�_�ɌŒ�
	//===========================
	m_PosInfo.Pos = m_PosInfo.SupportPos;
	//========================================================================================

	//===========================
	//�ʒu��ύX
	//===========================
	if (pInput->GetPress(DIK_LSHIFT) == true)
	{//Y���ړ�
		if (pInput->GetPress(DIK_LCONTROL))
		{
			if (pInput->GetTrigger(DIK_W) == true)
			{
				m_PosInfo.Pos.y += 0.1f;
			}
			else if (pInput->GetTrigger(DIK_S) == true)
			{
				m_PosInfo.Pos.y -= 0.1f;
			}
		}
		else
		{
			if (pInput->GetPress(DIK_W) == true)
			{
				m_PosInfo.Pos.y += 5.0f;
			}
			else if (pInput->GetPress(DIK_S) == true)
			{
				m_PosInfo.Pos.y -= 5.0f;
			}
		}
	}
	else
	{//XZ���ʈړ�
		CCalculation::CaluclationMove(false,m_PosInfo.Pos,Move,5.0f, CCalculation::MOVEAIM_XZ, m_RotInfo.Rot.y);
	}

	//�x�_���ꏏ�Ɉړ�
	m_PosInfo.Pos += Move;
	m_PosInfo.SupportPos = m_PosInfo.Pos;

	//�f�o�b�O�\��
	CManager::GetDebugText()->PrintDebugText("�x�_�ʒu(���L�[) %f %f %f\n", m_PosInfo.SupportPos.x,m_PosInfo.SupportPos.y, m_PosInfo.SupportPos.z);
	CManager::GetDebugText()->PrintDebugText("�ړ��� �F %f %f %f\n", Move.x, Move.y, Move.z);
	CManager::GetDebugText()->PrintDebugText("����Z(FG�L�[) %f\n", m_RotInfo.Rot.z);
	CManager::GetCamera()->SetPosR(m_PosInfo.Pos);//�����_�����ݑ��삵�Ă��郂�f���ɌŒ�
	//================================================================================================================================================

}
//================================================================================================================================================

//===================================================================================================================
//�ʒu���ړ�������
//===================================================================================================================
void CObjectX::EditLife()
{
	if (CManager::GetInputKeyboard()->GetPress(DIK_LCONTROL) == true)
	{//L�R���g���[���L�[�������Ȃ���i�̗͂𑁂����₷)
		if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
		{//�V�t�g�L�[�������Ȃ���E�E�E
			if (CManager::GetInputKeyboard()->GetPress(DIK_3) == true)
			{
				m_LifeInfo.nMaxLife++;
			}
		}
		else if (CManager::GetInputKeyboard()->GetPress(DIK_3) == true)
		{
			m_LifeInfo.nMaxLife--;
		}
	}
	else
	{//L�R���g���[���L�[�������Ă��Ȃ��i�̗͂����������₷)
		if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
		{//�V�t�g�L�[�������Ȃ���E�E�E
			if (CManager::GetInputKeyboard()->GetTrigger(DIK_3) == true)
			{
				m_LifeInfo.nMaxLife++;
			}
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_3) == true)
		{
			m_LifeInfo.nMaxLife--;
		}
	}

	//�f�o�b�O�\��
	CManager::GetDebugText()->PrintDebugText("�ő�̗�(3)�F%d\n", m_LifeInfo.nMaxLife);
}
//================================================================================================================================================

//============================================================================
//�ő咸�_�ƍŏ����_�����ւ��邩�ǂ���
//============================================================================
void CObjectX::ChengeEditSwapVtxXZ()
{
	CInputKeyboard* pInputKeyBoard = CManager::GetInputKeyboard();//�L�[���͏����擾

	if (pInputKeyBoard->GetTrigger(DIK_7))
	{//XZ�̒��_��ύX
		m_SizeInfo.bSwapVtxXZ = m_SizeInfo.bSwapVtxXZ ? false : true;
	}

	//�f�o�b�O�\��
	CManager::GetDebugText()->PrintDebugText("���_��XZ�����ւ��邩�ǂ����i�V�j�F%d\n", m_SizeInfo.bSwapVtxXZ);
}
//================================================================================================================================================

//============================================================================
//�X�e�[�W�}�l�[�W���[�����𑀍삷��
//============================================================================
void CObjectX::ManagerChooseControlInfo()
{
	ChengeEditPos();                              //�ʒu��ς���
						                          
	ChengeEditScale();                            //�g�嗦��ς���
						                          
	ChengeEditSwapVtxXZ();                        //XZ�̒��_��ς���
						                          
	EditLife();                                   //�̗͂�ύX����

	CManager::GetCamera()->SetPosR(m_PosInfo.Pos);//�J�����̒����_�����݂̈ʒu�ɐݒ�
}
//================================================================================================================================================

//============================================================================
//�e�L�X�g�t�@�C���ɏ���ۑ�����
//============================================================================
void CObjectX::SaveInfoTxt(fstream & WritingFile)
{
	WritingFile << "SETOBJECTX" << endl;//�ǂݍ��݊J�n�p�e�L�X�g

	WritingFile << "POS = " << fixed << setprecision(3)<< m_PosInfo.Pos.x << " " <<
		fixed << setprecision(3) << m_PosInfo.Pos.y << " " << 
		fixed << setprecision(3) << m_PosInfo.Pos.z << " " << endl;//�ʒu�i�����_�R�j�܂�
	WritingFile << "ROT = " << fixed << setprecision(3) << m_RotInfo.Rot.x << " " <<
		fixed << setprecision(3) << m_RotInfo.Rot.y << " " <<
		fixed << setprecision(3) << m_RotInfo.Rot.z << " " << endl;//�����i�����_�R�j�܂�
	WritingFile << "SCALE = " << fixed << setprecision(3) << m_SizeInfo.Scale.x << " " <<
		fixed << setprecision(3) << m_SizeInfo.Scale.y << " " <<
		fixed << setprecision(3) << m_SizeInfo.Scale.z << " " << endl;//�g�嗦�i�����_�R�j�܂�

	WritingFile << "MOVE = " << (m_MoveInfo.Move.x) << " " << m_MoveInfo.Move.y << " " << m_MoveInfo.Move.z << endl;//�ړ���

	WritingFile << "LIFE = " << m_LifeInfo.nMaxLife << endl;//�̗͂�ݒ�

	WritingFile << "SWAPVTXXZ = " << m_SizeInfo.bSwapVtxXZ << endl;//���_��XZ��ύX���邩�ǂ���

	WritingFile << "END_SETOBJECTX" << endl;//�ǂݍ��ݏI���p�e�L�X�g
}
//================================================================================================================================================

//============================================================================
//�e�L�X�g�t�@�C���������ǂݍ���
//============================================================================
void CObjectX::LoadInfoTxt(fstream& LoadingFile, list<CObject*>& listSaveManager, string& Buff, CObject* pObj)
{
	int nLife = 0;                                   //�̗͊i�[�p
	int nMaxLife = 0;                                //�ő�̗͊i�[�p
	D3DXVECTOR3 Pos = { 0.0f,0.0f,0.0f };            //�ʒu�i�[�p
	D3DXVECTOR3 Move = { 0.0f,0.0f,0.0f };           //�ړ��ʊi�[�p
	D3DXVECTOR3 Rot = { 0.0f,0.0f,0.0f };            //�����i�[�p
	D3DXVECTOR3 Scale = { 0.0f,0.0f,0.0f };          //�g�嗦�i�[�p
	bool bSwapVtxXZ = false;                         //XZ���W�����ւ��邩�ǂ����i�[�p

	CObjectX* pObjX = dynamic_cast<CObjectX*>(pObj); //�I�u�W�F�N�gX�񂋃_�E���L���X�g

	if (pObjX != nullptr)
	{//�I�u�W�F�N�gX�ւ̃|�C���^�����݂��Ă���

		while (Buff != "END_SETOBJECTX")
		{
			LoadingFile >> Buff;           //�P���ǂݍ���
			if (Buff == "#")
			{//�s���X�L�b�v
				getline(LoadingFile, Buff);
			}

			else if (Buff == "LIFE")
			{
				LoadingFile >> Buff;       //�C�R�[��
				LoadingFile >> nMaxLife;   //�ő�̗�
				nLife = nMaxLife;          //�̗́i�����l�͍ő�̗͂Ɠ���)
			}
			else if (Buff == "MOVE")
			{
				LoadingFile >> Buff;       //�C�R�[��
				LoadingFile >> Move.x;     //�ړ���X
				LoadingFile >> Move.y;     //�ړ���Y
				LoadingFile >> Move.z;     //�ړ���Z
			}
			else if (Buff == "POS")
			{
				LoadingFile >> Buff;       //�C�R�[��
				LoadingFile >> Pos.x;      //�ʒuX
				LoadingFile >> Pos.y;      //�ʒuY
				LoadingFile >> Pos.z;      //�ʒuZ
			}
			else if (Buff == "ROT")
			{
				LoadingFile >> Buff;       //�C�R�[��
				LoadingFile >> Rot.x;      //�ʒuX
				LoadingFile >> Rot.y;      //�ʒuY
				LoadingFile >> Rot.z;      //�ʒuZ
			}
			else if (Buff == "SCALE")
			{
				LoadingFile >> Buff;       //�C�R�[��
				LoadingFile >> Scale.x;    //�g�嗦X
				LoadingFile >> Scale.y;    //�g�嗦Y
				LoadingFile >> Scale.z;    //�g�嗦Z
			}
			else if (Buff == "SWAPVTXXZ")
			{
				LoadingFile >> Buff;       //�C�R�[��
				LoadingFile >> bSwapVtxXZ; //XZ�̒��_�����ւ��邩�ǂ���
			}
		}

		PosInfo& ObjXPosInfo = GetPosInfo();           //�ʒu�����擾
		MoveInfo& ObjXMoveInfo = GetMoveInfo();        //�ړ������擾
		RotInfo& ObjXRotInfo = GetRotInfo();           //���������擾
		SizeInfo& ObjXSizeInfo = GetSizeInfo();        //�T�C�Y�����擾
		LifeInfo& ObjXLifeInfo = GetLifeInfo();        //�̗͏����擾

		ObjXPosInfo.SetPos(Pos);                       //�ʒu��ݒ�
		ObjXPosInfo.SetSupportPos(Pos);                //�ŏ��̈ʒu��ݒ�

		ObjXMoveInfo.SetMove(Move);                    //�ړ��ʂ�ݒ�

		ObjXRotInfo.SetRot(Rot);                       //������ݒ�

		ObjXSizeInfo.SetScale(Scale);                  //�g�嗦��ݒ�
		ObjXSizeInfo.SetUseSwapVtxXZ(bSwapVtxXZ);      //���_��XZ�����ւ��邩�ǂ�����ݒ�

		ObjXLifeInfo.SetLife(nLife);                   //�̗͂�ݒ�
		ObjXLifeInfo.SetMaxLife(nMaxLife);             //�ő�̗͂�ݒ�
	}
	else
	{//��O����
		assert("�I�u�W�F�N�gX�ւ̃|�C���^�����݂��Ă��Ȃ�");
	}
}
//================================================================================================================================================

//============================================================================
//�_�����}�e���A���ԍ��̃e�N�X�`�������蓖�Ă�
//============================================================================
void CObjectX::SetLockOnMatBindTexture(int nNumMat, LPDIRECT3DTEXTURE9 pTexture)
{
	m_ObjectXInfo.pTexture[nNumMat] = pTexture;//�w�肵���z��ԍ��̃e�N�X�`����ς���
}
//================================================================================================================================================

//============================================================================
//�e��`�悷��
//============================================================================
void CObjectX::DrawShadow()
{
	D3DXMATRIX mtxShadow,mtxScale,mtxRot,mtxTrans;                    //�v�Z�p�}�g���b�N�X
	D3DXPLANE plane;                                                  //�v���[��
	D3DXVECTOR4 vecLight;                                             //���C�g�t����
	D3DXVECTOR3 ShadowPos, ShadowNor;                                 //�e�̈ʒu�Ɩ@��
	D3DXMATERIAL* pMat;                                               //�}�e���A���f�[�^�ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice(); //�f�o�C�X�ւ̃|�C���^�Ǝ擾
	D3DMATERIAL9 matDef;                                              //���݂̃}�e���A���ۑ��p

	D3DXVECTOR3 RayCollisionPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);      //���C�����������ʒu�i�[�p
	D3DXVECTOR3 CalcRayCollisionPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  //���C������������ԋ߂��ʒu�i�[�p
	float fResultNearLength = 0.0f;

	//���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

    //�e�̃}�g���b�N�X��������
	D3DXMatrixIdentity(&mtxShadow);

	//���C�g�̋t�����x�N�g����ݒ�
	vecLight = D3DXVECTOR4(-100.0f, 300.0f,300.0f, 0.0f);

	ShadowPos = D3DXVECTOR3(0.0f,0.0f,0.0f);   //�e�̈ʒu
	ShadowNor = D3DXVECTOR3(0.0f, 1.0f, 0.0f); //�e�̖@���iY��1.0f�Ȃ̂ŁA�����ȃ|���S��)


	//�@���ƕ��ʏ�̈�_���畽�ʏ����쐬
	D3DXPlaneFromPointNormal(&plane, &ShadowPos, &ShadowNor);

	//���C�g�̕��ʏ�񂩂�e�s����쐬
	D3DXMatrixShadow(&mtxShadow, &vecLight, &plane);

	//�傫���𔽉f
	D3DXMatrixScaling(&mtxScale, m_SizeInfo.Scale.x, m_SizeInfo.Scale.y, m_SizeInfo.Scale.z);
	D3DXMatrixMultiply(&mtxShadow, &mtxShadow, &mtxScale);

	//��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_RotInfo.Rot.y, 0.0f, 0.0f);
	D3DXMatrixMultiply(&mtxShadow, &mtxShadow, &mtxRot);

	//==================================================================================
	//���Ƀ��C���΂��A���������I�u�W�F�N�g�̒��ň�ԋ߂��I�u�W�F�N�g�̈ʒu�����߂�
	//==================================================================================
	int nCntColRay = 0;//���C�����������񐔂��J�E���g����
	for (int nCntPri = 0; nCntPri < CObject::m_nMAXPRIORITY; nCntPri++)
	{//�I�u�W�F�N�g���X�g����������
		CObject* pObj = CObject::GetTopObject(nCntPri);//���X�g�̐擪�I�u�W�F�N�g���擾����

		while (pObj != nullptr)
		{
			CObject* pNext = pObj->GetNextObject();//���̃I�u�W�F�N�g���擾����

			if (pObj->GetType() == CObject::TYPE::BLOCK || pObj->GetType() == CObject::TYPE::BGMODEL)
			{//�I�u�W�F�N�g�^�C�v���u�u���b�N�v���́u�w�i���f���v��������
				CObjectX* pObjX = static_cast<CObjectX*>(pObj);//�I�u�W�F�N�gX�ɃL���X�g
				if (CCollision::RayIntersectsAABBCollisionPos(m_PosInfo.WorldPos + D3DXVECTOR3(0.0f,0.1f,0.0f), D3DXVECTOR3(0.0f, -1.0f, 0.0f),
					pObjX->GetPosInfo().GetPos() + pObjX->m_SizeInfo.GetVtxMin(),pObjX->GetPosInfo().GetPos() + pObjX->m_SizeInfo.GetVtxMax(), CalcRayCollisionPos))
				{//���C���������Ă�����
					float fLength = sqrtf(powf(CalcRayCollisionPos.y - m_PosInfo.WorldPos.y,2));//���C�����������ʒu��Y���̋��������
					nCntColRay++;                                                               //���C�����������I�u�W�F�N�g�������C���N�������g
					if (nCntColRay == 1)
					{//�ŏ��̓��������I�u�W�F�N�g�Ȃ̂ŁA�������ŋ����ƃ��C�����������ʒu���L�^����
						fResultNearLength = fLength;          
						RayCollisionPos = CalcRayCollisionPos;
					}
					else
					{
						if (fResultNearLength > fLength)
						{//���C�����������I�u�W�F�N�g�̒��ň�ԋ߂��̂ŁA�����ƃ��C�����������ʒu���X�V����
							fResultNearLength = fLength;
							RayCollisionPos = CalcRayCollisionPos;
						}
					}
				}
			}

			pObj = pNext;//���X�g�����ɐi�߂�
		}
	}
	//==================================================================================================================================================================

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans,m_PosInfo.WorldPos.x,RayCollisionPos.y, m_PosInfo.WorldPos.z);
	D3DXMatrixMultiply(&mtxShadow, &mtxShadow, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtxShadow);

	//=======================================
    //�`��̒���
    //=======================================

    //�@���̒������P�ɂ���B�i�X�P�[���Ȃǂ��g�������́A�K���g���B)
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
	//================================================================================================================

	if (GetDrawInfo().GetUseDraw() == true)
	{
		if (m_ObjectXInfo.pMesh != nullptr && m_ObjectXInfo.pBuffMat != nullptr)
		{
			CObject::TYPE Type = GetType();

			//�}�e���A���ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)m_ObjectXInfo.pBuffMat->GetBufferPointer();

			//==========================================================================
			//�}�e���A���̐����A�e�N�X�`����ǂݍ��ށB
			//==========================================================================
			for (int nCntMat = 0; nCntMat < (int)m_ObjectXInfo.dwNumMat; nCntMat++)
			{
				if (m_ObjectXInfo.Diffuse[nCntMat].a > 0.5f)
				{//a�l��0.5f�ُ�Ȃ�0.5f�ɌŒ�
					pMat[nCntMat].MatD3D.Diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);
				}
				else
				{//a�l��0.5f�ȉ��Ȃ�S�̐F�����̃f�[�^���璼��a�l���Q��
					pMat[nCntMat].MatD3D.Diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, m_ObjectXInfo.Diffuse[nCntMat].a);
				}

				//�}�e���A���̐ݒ�
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//�e�N�X�`����ݒ肷��
				pDevice->SetTexture(0, NULL);

				//���f���i�p�[�c�j�̕`��
				m_ObjectXInfo.pMesh->DrawSubset(nCntMat);
			}
			//================================================================================================================
		}
	}
	//=======================================
	//�`��̒��������ɖ߂�
	//=======================================

	//�@���̒������P�ɂ���B�i�X�P�[���Ȃǂ��g�������́A�K���g���B)
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
	//================================================================================================================

	//�ۑ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);
}
//================================================================================================================================================

//============================================================================
//�g�嗦�����Z���邩�ǂ���
//============================================================================
void CObjectX::SizeInfo::SetUseAddScale(D3DXVECTOR3 CopyAddScale, bool bUse)
{
	bUseAddScaling = bUse;       //�g�嗦�̉��Z�����邩�ǂ���
	AddScale = CopyAddScale;     //�g�嗦�̉��Z���s��
}
//================================================================================================================================================

//============================================================================
//�g�嗦���Z����
//============================================================================
void CObjectX::SizeInfo::AddScaleProcess()
{
	if (bUseAddScaling == true)
	{//�g�嗦�̉��Z���s���Ȃ�
		Scale += AddScale;
	}
}
//================================================================================================================================================

//============================================================================
//�g�嗦��Z����
//============================================================================
void CObjectX::SizeInfo::MultiScaleProcess()
{
	if (bUseMultiScale == true)
	{//�g�嗦�̏�Z���s���Ȃ�
		Scale.x *= MultiScale.x;
		Scale.y *= MultiScale.y;
		Scale.z *= MultiScale.z;
	}
}
//================================================================================================================================================

//============================================================================
//�g�嗦���Q�Ƃ��Ē��_�̍ő�ŏ������߂鏈��
//============================================================================
void CObjectX::SizeInfo::DecideVtxMaxMinProcess()
{
	//XZ�̒��_�����ւ��邩�ǂ����ɂ����x,z�̒l��ς���
	if (bSwapVtxXZ == false)
	{
		VtxMax.x = OriginVtxMax.x * Scale.x;
		VtxMax.y = OriginVtxMax.y * Scale.y;
		VtxMax.z = OriginVtxMax.z * Scale.z;
		VtxMin.x = OriginVtxMin.x * Scale.x;
		VtxMin.y = OriginVtxMin.y * Scale.y;
		VtxMin.z = OriginVtxMin.z * Scale.z;
	}
	else
	{
		VtxMax.x = OriginVtxMax.x * Scale.z;
		VtxMax.y = OriginVtxMax.y * Scale.y;
		VtxMax.z = OriginVtxMax.z * Scale.x;
		VtxMin.x = OriginVtxMin.x * Scale.z;
		VtxMin.y = OriginVtxMin.y * Scale.y;
		VtxMin.z = OriginVtxMin.z * Scale.x;
	}

	//�T�C�Y����
	Size = VtxMax - VtxMin;
}
//================================================================================================================================================

//============================================================================
//�d�͂̏���
//============================================================================
void CObjectX::MoveInfo::GravityProcess()
{
	if (bUseGravity == true)
	{//�d�͂��g�p����Ȃ�
		const D3DXVECTOR3& Move = GetMove();
		SetMove(Move + D3DXVECTOR3(0.0f, -fGravityPower, 0.0f));
	}
}

//==================================
//��Z��������
//==================================
void CObjectX::MoveInfo::MultiSpeedProcess()
{
	if (bUseMultiSpeed == true)
	{//���x�̏�Z���s���Ȃ�
		Move.x *= MultiSpeed.x;
		Move.y *= MultiSpeed.y;
		Move.z *= MultiSpeed.z;
	}
}

//==================================
//��Z��������
//==================================
void CObjectX::MoveInfo::AddSpeedProcess()
{
	if (bUseAddSpeed == true)
	{//���x�̉��Z���s���Ȃ�
		Move += AddSpeed;
	}
}
//================================================================================================================================================

//==================================
//�q�b�g�X�g�b�v����
//==================================
void CObjectX::LifeInfo::HitStopProcess()
{
	if (bHitStop == true)
	{
		if (nHitStopTime > 0)
		{
			nHitStopTime--;
		}
		else
		{//�q�b�g�X�g�b�v����
			bHitStop = false;
			nHitStopTime = 0;
		}
	}
}
//================================================================================================================================================

//==============================================
//�̗͂����炷
//==============================================
void CObjectX::LifeInfo::AutoSubLifeProcess()
{
	if (bAutoSubLife == true)
	{//�����I�ɑ̗͂����炷���ǂ���
		nLife--;
	}
}
//================================================================================================================================================

//==============================================
//�̗͂̊����ŐF�����̓����x��ς���
//==============================================
void CObjectX::LifeInfo::RatioLifeAlphaColorProcess(CObjectX* pObjX)
{
	if (bUseRatioLifeAlpha == true)
	{//�̗͂̊����ŐF�����̓����x��ς��邩�ǂ���
		float fRatio;
		fRatio = float(nLife) / float(nMaxLife);
		pObjX->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, fRatio), 3, true, true,false);
		CManager::GetDebugText()->PrintDebugText("�����x�����F%f\n", fRatio);
	}
}
//================================================================================================================================================

//==============================================
//�����I�Ɏ��S�t���O�𔭓����鏈��
//==============================================
void CObjectX::LifeInfo::AutoDeathProcess(CObjectX* pObjX)
{
	if (nLife < 1 && bAutoDeath == true && pObjX->GetUseDeath() == true)
	{//�̗͂��Ȃ��Ȃ����玩���I�Ɏ��S�t���O��ݒ肷�邩�ǂ���
		pObjX->SetDeath();
	}
}
//================================================================================================================================================

//==============================================
//�F�����̕ύX���s�����ǂ���
//==============================================
void CObjectX::DrawInfo::ChengeColorProcess(CObjectX* pObjX)
{
	if (bColorChenge == true)
	{//�F�����̕ύX
		nChengeColorTime--;

		if (bBlinkingColor == true)
		{
			//2f���Ƃɐݒ肳�ꂽ�F�ƌ��̐F���������݂ɕς���
			if (nChengeColorTime % 2 == 0)
			{
				pObjX->SetOnlyColor(Color);
			}
			else
			{
				pObjX->SetOnlyFormarColor();
			}
		}
	}

	if (nChengeColorTime <= 0 && bColorChenge == true)
	{//�F������ς��鎞�Ԃ��Ȃ��Ȃ����猳�̐F�����ɖ߂�
		nChengeColorTime = 0;//�F������ς���t���[���̃��Z�b�g
		pObjX->SetFormarColor();//���̐F�����ɖ߂�
		bColorChenge = false;//�F������ς��Ȃ�
	}
}
//================================================================================================================================================
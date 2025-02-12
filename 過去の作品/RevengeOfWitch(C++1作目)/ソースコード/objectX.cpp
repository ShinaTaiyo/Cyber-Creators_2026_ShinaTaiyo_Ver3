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
#include "manager.h"
#include "block.h"
#include "model.h"
#include "camera.h"
#include "bullet.h"
#include "input.h"
#include "debugtext.h"
#include "damage.h"
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <algorithm>
//===========================================================================================

//================================================
//�R���X�g���N�^
//================================================
CObjectX::CObjectX(int nPriority) : CObject(nPriority),m_bUseDraw(true)
{

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
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();//�f�o�C�X�ւ̃|�C���^�Ǝ擾
	//=================================
	//���f�����̏�����
	//=================================
	m_ObjectXInfo.dwNumMat = {};                                          //�}�e���A���̐�
	m_ObjectXInfo.pBuffMat = {};                                          //���_�o�b�t�@�ւ̃|�C�� �^
	m_ObjectXInfo.pMesh = {};                                             //���_���ւ̃|�C���^
	m_ObjectXInfo.pTexture = nullptr;                                     //�e�N�X�`���ւ̃|�C���^
	m_ObjectXInfo.Diffuse = nullptr;                                     //�F����
	m_ObjectXInfo.FormarDiffuse = nullptr;                               //���̐F����
	//===================================================================================================================================================

	//==========================
	//�ϐ��̏�����
	//==========================
	m_mtxWorld = {};                                   //���[���h�}�g���b�N�X
	m_bUse = false;                                    //�g�p���Ă��邩�ǂ���
	m_Pos = NULL_VECTOR3;                              //�ʒu
	m_nChengeColorTime = 0;                            //�F��ς��鎞��
	m_PosOld = NULL_VECTOR3;                           //1f�O�̈ʒu
	m_SupportPos = NULL_VECTOR3;                       //�ݒu�ʒu
	m_Rot = NULL_VECTOR3;                              //����
	m_nIdxShadow = -1;                                 //�e�̃C���f�b�N�X
	m_nManagerType = 0;                                //�}�l�[�W���[�ŌĂяo�����ۂ̃^�C�v�ԍ�
	m_bColorChenge = false;                            //�F������ς��Ă��邩�ǂ���
	m_Move = NULL_VECTOR3;                             //�ړ���
	m_fInertia = m_fNORMAL_INERTIA;                                 //����

	m_VtxMin = NULL_VECTOR3;                           //�ʒu�̍ŏ��l
	m_OriginVtxMin = NULL_VECTOR3;                     //���X�̈ʒu�̍ŏ��l
	m_VtxMax = NULL_VECTOR3;                           //�ʒu�̍ő�l
	m_OriginVtxMax = NULL_VECTOR3;                     //���X�̈ʒu�̍ő�l
	m_nNumVtx = 0;                                     //���_��
	m_SizeFVF;                                         //���_�t�H�[�}�b�g�̃T�C�Y
	m_pVtxBuff;                                        //���_�o�b�t�@�ւ̃|�C���^
	m_Scale = NULL_VECTOR3;                            //�g�嗦
	m_FormarScale = NULL_VECTOR3;                      //���̊g�嗦
	m_Size = NULL_VECTOR3;                             //�T�C�Y

	m_bIsLanding = false;                              //�n�ʂɂ��邩�ǂ���
	m_bUseInteria = true;                              //�����������邩�ǂ���
	m_bIsWalling = false;                              //�ǂɂ������Ă��邩�ǂ���
	m_bIsJumping = false;                              //�W�����v���Ă��邩�ǂ���

	m_nObjXType = (OBJECTXTYPE)(0);                    //���

	m_RotType = (ROTTYPE)(0);                          //�����̎��

	m_MoveType = (MOVETYPE)(0);                        //�ړ��^�C�v

	m_pLiftLandingObj = nullptr;                       //����Ă��郊�t�g�̃I�u�W�F�N�g���擾����
	m_bIsLiftLanding = false;                          //���t�g�̏�ɏ���Ă��邩�ǂ���
	//==================================================================================

	//===========================================
	//�I�t�Z�b�g�n
	//===========================================
	m_ParentSetPos = NULL_VECTOR3;                     //�e�̈ʒu
	m_ParentSetRot = NULL_VECTOR3;                     //�e�̌���
	m_ParentSetScale = NULL_VECTOR3;                   //�e�̊g�嗦
	m_bUseParent = false;                              //�e���g�p���邩�ǂ���
	m_ParentMtxWorld = {};                             //�e�̃}�g���b�N�X���[���h

	m_OffSetPos = NULL_VECTOR3;                        //�I�t�Z�b�g�̈ʒu
	m_OffSetRot = NULL_VECTOR3;                        //�I�t�Z�b�g�̌���
	m_OffSetScale = NULL_VECTOR3;                      //�I�t�Z�b�g�̊g�嗦
	//==================================================================================

	//===========================================
	//�̗͊֌W
	//===========================================
	m_nLife = 1;                                        //�̗�
	m_nMaxLife = 1;                                     //�ő�̗�
	m_bAutoSubLife = false;                             //�����I�ɑ̗͂����炵�����邩�ǂ���
	m_bHitStop = false;                                 //�q�b�g�X�g�b�v��Ԃ��ǂ���
	m_nHitStopTime = 0;                                 //�q�b�g�X�g�b�v����
	//==================================================================================

	//===========================================
	//�e�֌W
	//===========================================
	m_bUseShadow = true;
	//==================================================================================

	//===========================================
	//�d�͊֌W
	//===========================================
	m_bUseGravity = true;         //�d�͂��g�p���邩�ǂ���
	m_bTimeChengeGravity = false; //�d�͂�ONOFF�����Ԃŕς��邩�ǂ���
	m_nNotUseGravityTime = 0;     //�d�͂��g��Ȃ�����
	//==================================================================================

	//===========================================
	//�u���b�N�����蔻��֌W
	//===========================================
	m_bUseBlockCollision = true;//�u���b�N�Ƃ̓����蔻����g�p���邩�ǂ���
	m_bSuccessBlockCollisionX = false;//X�����̃u���b�N�Ƃ̓����蔻�肪�����������ǂ���
	m_bSuccessBlockCollisionY = false;//Y�����̃u���b�N�Ƃ̓����蔻�肪�����������ǂ���
	m_bSuccessBlockCollisionZ = false;//Z�����̃u���b�N�Ƃ̓����蔻�肪�����������ǂ���
	//==================================================================================

	//===========================================
	//�ʒu�X�V�֌W
	//===========================================
	m_bUseUpdatePos = true;//�ʒu�̍X�V���g�p���邩�ǂ���
	m_bUseAdjustOriginPos = true;//���_�ɓ��B�����Ƃ��Ɉʒu�̕␳���s�����ǂ���
	//==================================================================================

	//===========================================
	//�����֌W
	//===========================================
	m_bSummonObjectDeath = false; //���̃I�u�W�F�N�gX�����������I�u�W�F�N�g�������t���O
	//==================================================================================

	//===========================================
	//���S�_�֌W
	//===========================================
	m_SenterPos = NULL_VECTOR3;//���S�_
	//==================================================================================

	//===================================
	//��]�֌W
	//===================================
	m_bUseAddRot = false;
	m_AddRot = NULL_VECTOR3;
	//==================================================================================

	//===================================
	//�F�����֌W
	//===================================
	m_bUseRatioLifeAlpha = false;
	//==================================================================================

	m_bUseMultiSpeed = false;                 //��Z���������邩�ǂ���
	m_MultiSpeed = NULL_VECTOR3;              //��Z�����x

	m_bUseAddSpeed = false;                   //�������g�p���邩�ǂ���
	m_AddSpeed = NULL_VECTOR3;                //�����x

	m_bUseGravity = false;                    //�d�͂��g�p���邩�ǂ����@
	m_fGravityPower = 0.0f;                   //�d�͂̑傫��

	m_bUseAddScaling = false;                 //�g�嗦�̉��Z���g�p���邩�ǂ���
	m_AddScale = NULL_VECTOR3;                //�g�嗦�̉��Z��    

	//===========================================
	//����֌W
	//===========================================
	m_bDodge = false;
	//==================================================================================

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
		delete[] m_ObjectXInfo.FormarDiffuse;//�F�����̓��I���������J��
		m_ObjectXInfo.FormarDiffuse = nullptr;
	}
}
//================================================================================================================================================

//================================================
//�X�V����
//================================================
void CObjectX::Update()
{
	float fRatio = 0.0f;
	if (m_bUseParent == true)
	{//�q�}�g���b�N�X���g�p����ꍇ�ɁA���[���h���W�𔻒肷��
		D3DXVECTOR3 PosZero = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXVec3TransformCoord(&m_OffSetPos, &PosZero, &m_mtxWorld);//���̈����̃��[�J�����W�ɐ^�񒆂̈����̃��[���h���W��������
	}

	//==============================================
	//���S�_�����߂�
	//==============================================
	CalculateSenterPos();

	//==============================================
	//�̗͂����炷
	//==============================================
	if (m_bAutoSubLife == true)
	{
		m_nLife--;
	}
	//===========================================================

	//==============================================
	//�̗͂̊����ŐF�����̓����x��ς���
	//==============================================
	if (m_bUseRatioLifeAlpha == true)
	{
		fRatio = float(m_nLife) / float(m_nMaxLife);
		SetColorAlphaOnly(fRatio);
	}
	//===========================================================

	//==================================
	//��Z�����x��ON�ɂȂ��Ă�����
	//==================================
	if (m_bUseMultiSpeed == true)
	{
		m_Move.x *= m_MultiSpeed.x;
		m_Move.y *= m_MultiSpeed.y;
		m_Move.z *= m_MultiSpeed.z;
	}
	//==========================================================

	//==================================
	//������ON�ɂȂ��Ă�����
	//==================================
	if (m_bUseAddSpeed == true)
	{
		m_Move += m_AddSpeed;
	}
	//==========================================================

	//==================================
	//�g�嗦�̉��Z��ON�ɂȂ��Ă�����
	//==================================
	if (m_bUseAddScaling == true)
	{
		m_Scale += m_AddScale;
	}
	//==========================================================

	//==============================================
	//�d�͊֌W
	//==============================================
	ChengeGravityProcess();
	//===========================================================


	//==============================================
    //�q�b�g�X�g�b�v����
    //==============================================
	HitStopProcess();
	//===========================================================


	//��Ɋg�嗦���Q�Ƃ��čő�ŏ����_�����߂�
	m_VtxMax.x = m_OriginVtxMax.x * m_Scale.x;
	m_VtxMax.y = m_OriginVtxMax.y * m_Scale.y;
	m_VtxMax.z = m_OriginVtxMax.z * m_Scale.z;
	m_VtxMin.x = m_OriginVtxMin.x * m_Scale.x;
	m_VtxMin.y = m_OriginVtxMin.y * m_Scale.y;
	m_VtxMin.z = m_OriginVtxMin.z * m_Scale.z;

	m_Size = m_VtxMax - m_VtxMin;

	if (m_bColorChenge == true)
	{
		m_nChengeColorTime--;
	}

	if (m_nChengeColorTime <= 0 && m_bColorChenge == true )
	{
		m_nChengeColorTime = 0;
		SetFormarColor();//���̐F�����ɖ߂�
		m_bColorChenge = false;
	}

	if (m_bIsLiftLanding == true && m_pLiftLandingObj != nullptr && m_bUseBlockCollision == true)
	{//�����u���b�N�̏�Ɉʒu��␳
  		CBlock::LandingCorrection(m_Pos, m_pLiftLandingObj, m_VtxMax, m_VtxMin);
	}

	if (m_bUseUpdatePos == true)
	{//�ʒu�X�V����
		UpdatePos();
	}

	if (m_bUseAddRot == true)
	{//�����̉��Z����
		m_Rot += m_AddRot;
	}

	if (m_bUseBlockCollision == true)
	{//�u���b�N�����蔻��
		CommonBlockCollision();
	}

	if (GetStageManagerChoose() == true)
	{//�I�u�W�F�N�gX�̏���\��
	    DispInfo();
	}
}
//================================================================================================================================================

//================================================
//�`�揈��
//================================================
void CObjectX::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice(); //�f�o�C�X�ւ̃|�C���^�Ǝ擾
	D3DXMATRIX mtxRot, mtxTrans, mtxScale;                  //�v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;                                              //���݂̃}�e���A���ۑ��p
	D3DXMATERIAL* pMat;                                               //�}�e���A���f�[�^�ւ̃|�C���^

	//���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	if (m_bUseParent == true)
	{
		OffSetUpdate();
	}

	//�傫���𔽉f
	D3DXMatrixScaling(&mtxScale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	if (m_bUseParent == true)
	{
		//�e�̃}�g���b�N�X�Ƃ������킹��
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &m_ParentMtxWorld);
	}

	if (m_ObjectXInfo.Diffuse[0].a > 0.0f && m_Pos.y + m_VtxMax.y >= 0.0f && m_bUseShadow == true && CManager::GetCamera()->GetCameraType() == CCamera::CAMERATYPE_BOSSBATTLE)
	{
		//�e�̕`��
		DrawShadow(m_Pos);
	}
	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	if (GetType() == CObject::TYPE_ATTACK)
	{
		int n = 0;
	}
	//=======================================
	//�`��̒���
	//=======================================
	//�A���t�@�e�X�g��L��(�A���t�@�l���O���傫���ꍇ�ɕ`�悷��j
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	//�@���̒������P�ɂ���B�i�X�P�[���Ȃǂ��g�������́A�K���g���B)
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
	//================================================================================================================

	//�}�e���A���ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)m_ObjectXInfo.pBuffMat->GetBufferPointer();

	if (m_bUseDraw == true)
	{
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
	
	//=======================================
	//�`��̒��������ɖ߂�
	//=======================================
	//�A���t�@�e�X�g�����ɖ߂�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	//�@���̒������P�ɂ���B�i�X�P�[���Ȃǂ��g�������́A�K���g���B)
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
	//================================================================================================================

	//�ۑ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);

}
//================================================================================================================================================

//==========================================================================
//�d�͂��g�p����
//==========================================================================
void CObjectX::SetUseGravity(float fGravityPower)
{
	m_bUseGravity = true;
	m_fGravityPower = fGravityPower;
}
//================================================================================================================================================

//==========================================================================
//�g�嗦�̉��Z��ݒ�
//==========================================================================
void CObjectX::SetUseAddScale(D3DXVECTOR3 AddScale, bool bUse)
{
	m_bUseAddScaling = bUse;
	m_AddScale = AddScale;
}
//===================================================================================================================================================

//================================================
//���f���������蓖�Ă�
//================================================
void CObjectX::BindObjectXInfo(LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD dwNumMat, LPDIRECT3DTEXTURE9* pTexture, D3DCOLORVALUE* pDiffuse)
{
	m_ObjectXInfo.pMesh = pMesh;
	m_ObjectXInfo.pBuffMat = pBuffMat;
	m_ObjectXInfo.dwNumMat = dwNumMat;
	m_ObjectXInfo.pTexture = DBG_NEW LPDIRECT3DTEXTURE9[int(dwNumMat)];
	m_ObjectXInfo.Diffuse = DBG_NEW D3DCOLORVALUE[int(dwNumMat)];
	m_ObjectXInfo.FormarDiffuse = DBG_NEW D3DCOLORVALUE[int(dwNumMat)];
	for (int nCnt = 0; nCnt < (int)(dwNumMat); nCnt++)
	{
		m_ObjectXInfo.pTexture[nCnt] = pTexture[nCnt];
		m_ObjectXInfo.Diffuse[nCnt] = pDiffuse[nCnt];
		m_ObjectXInfo.FormarDiffuse[nCnt] = pDiffuse[nCnt];
	}
}
//================================================================================================================================================

//================================================
//�F������ݒ肷��
//================================================
void CObjectX::SetColor(COLORTYPE type, int nColChengeTime)
{
	m_bColorChenge = true;
	m_nChengeColorTime = nColChengeTime;
	switch (type)
	{
	case COLORTYPE_RED:
		for (int nCnt = 0; nCnt < (int)(m_ObjectXInfo.dwNumMat); nCnt++)
		{
			m_ObjectXInfo.Diffuse[nCnt].r = 1.0f;
			m_ObjectXInfo.Diffuse[nCnt].g = 0.0f;
			m_ObjectXInfo.Diffuse[nCnt].b = 0.0f;
			m_ObjectXInfo.Diffuse[nCnt].a = 1.0f;
		}
		break;
	case COLORTYPE_GREEN:
		for (int nCnt = 0; nCnt < (int)(m_ObjectXInfo.dwNumMat); nCnt++)
		{
			m_ObjectXInfo.Diffuse[nCnt].r = 0.0f;
			m_ObjectXInfo.Diffuse[nCnt].g = 1.0f;
			m_ObjectXInfo.Diffuse[nCnt].b = 0.0f;
			m_ObjectXInfo.Diffuse[nCnt].a = 1.0f;
		}
		break;
	case COLORTYPE_BRUE:
		for (int nCnt = 0; nCnt < (int)(m_ObjectXInfo.dwNumMat); nCnt++)
		{
			m_ObjectXInfo.Diffuse[nCnt].r = 0.0f;
			m_ObjectXInfo.Diffuse[nCnt].g = 0.0f;
			m_ObjectXInfo.Diffuse[nCnt].b = 1.0f;
			m_ObjectXInfo.Diffuse[nCnt].a = 1.0f;
		}
		break;
	case COLORTYPE_ALPHA:
		for (int nCnt = 0; nCnt < (int)(m_ObjectXInfo.dwNumMat); nCnt++)
		{
			m_ObjectXInfo.Diffuse[nCnt].a = 0.5f;
		}
		break;
	default:
		break;
	}
}
//================================================================================================================================================

//================================================
//�F�̃A���t�@�l������ς���
//================================================
void CObjectX::SetColorAlphaOnly(float fAlpha)
{
	for (int nCnt = 0; nCnt < (int)(m_ObjectXInfo.dwNumMat); nCnt++)
	{
		m_ObjectXInfo.Diffuse[nCnt].a = fAlpha;
	}
}
//================================================================================================================================================

//================================================
//�I���W�i���̐F������ݒ肷��
//================================================
void CObjectX::SetOriginalColor(D3DXCOLOR col, int nColChengeTime)
{
	m_bColorChenge = true;
	m_nChengeColorTime = nColChengeTime;
	for (int nCnt = 0; nCnt < (int)(m_ObjectXInfo.dwNumMat); nCnt++)
	{
		m_ObjectXInfo.Diffuse[nCnt].r = col.r;
		m_ObjectXInfo.Diffuse[nCnt].g = col.g;
		m_ObjectXInfo.Diffuse[nCnt].b = col.b;
		m_ObjectXInfo.Diffuse[nCnt].a = col.a;
	}

}
//================================================================================================================================================

//================================================
//���ʂ̃u���b�N�Ƃ̓����蔻��
//================================================
void CObjectX::CommonBlockCollision()
{
	if (m_bUseBlockCollision == true)
	{
		CBlock::CollisionSquare(this);
	}
}
//================================================================================================================================================

//================================================
//���S�_�����߂�
//================================================
void CObjectX::CalculateSenterPos()
{
	float fVtxMaxPosY = fabsf(m_VtxMax.y);
	float fVtxMinPosY = fabsf(m_VtxMin.y);

	float fAdjustPosY = 0.0f;

	m_SenterPos = m_Pos;
	m_SenterPos.y += (m_VtxMax.y + m_VtxMin.y) / 2;
}
//================================================================================================================================================

//================================================
//���̐F�����ɖ߂�
//================================================
void CObjectX::SetFormarColor()
{
	for (int nCnt = 0; nCnt < (int)(m_ObjectXInfo.dwNumMat); nCnt++)
	{
		m_ObjectXInfo.Diffuse[nCnt] = m_ObjectXInfo.FormarDiffuse[nCnt];
	}
}
//================================================================================================================================================

//================================================
//�I�t�Z�b�g�̏���ݒ肷��
//================================================
void CObjectX::OffSetUpdate()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice(); //�f�o�C�X�ւ̃|�C���^�Ǝ擾
	D3DXMATRIX mtxRot, mtxTrans, mtxScale;                  //�v�Z�p�}�g���b�N�X


	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_ParentMtxWorld);

	//�傫���𔽉f
	D3DXMatrixScaling(&mtxScale, m_ParentSetScale.x, m_ParentSetScale.y, m_ParentSetScale.z);
	D3DXMatrixMultiply(&m_ParentMtxWorld, &m_ParentMtxWorld, &mtxScale);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_ParentSetRot.y, m_ParentSetRot.x, m_ParentSetRot.z);
	D3DXMatrixMultiply(&m_ParentMtxWorld, &m_ParentMtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_ParentSetPos.x, m_ParentSetPos.y, m_ParentSetPos.z);
	D3DXMatrixMultiply(&m_ParentMtxWorld, &m_ParentMtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_ParentMtxWorld);

}
//================================================================================================================================================

//================================================
//�d�͂�ς��鏈��
//================================================
void CObjectX::ChengeGravityProcess()
{
	if (m_bTimeChengeGravity == true)
	{//��莞�ԏd�͂��g��Ȃ��ꍇ�Ɏg�p�BSet�ł��̏������Ăяo���Ȃ�����A���ڎg�p��Ԃ�ONOFF��ς����B
		if (m_nNotUseGravityTime > 0)
		{
			m_nNotUseGravityTime--;//�d�͂��g��Ȃ����Ԃ����炷
		}

		if (m_nNotUseGravityTime > 0)
		{
			m_bUseGravity = false;
		}
		else
		{
			m_bUseGravity = true;
			m_bTimeChengeGravity = false;//���Ԃŏd�͂�ς����Ԃ�����
		}
	}
}
//================================================================================================================================================

//================================================
//�q�b�g�X�g�b�v�̏���
//================================================
void CObjectX::HitStopProcess()
{
	if (m_bHitStop == true)
	{
		if (m_nHitStopTime > 0)
		{
			m_nHitStopTime--;
		}
		else
		{//�q�b�g�X�g�b�v����
			m_bHitStop = false;
			m_nHitStopTime = 0;
		}
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
		if (vtx.x > m_OriginVtxMax.x)
		{
			m_OriginVtxMax.x = vtx.x;
		}
		else if (vtx.y > m_OriginVtxMax.y)
		{//����ǂݍ��񂾒��_���A��ԑ傫�����_���傫���ꍇ
			m_OriginVtxMax.y = vtx.y;
		}
		else if (vtx.z > m_OriginVtxMax.z)
		{//����ǂݍ��񂾒��_���A��ԑ傫�����_���傫���ꍇ
			m_OriginVtxMax.z = vtx.z;
		}
		else if (vtx.x < m_OriginVtxMin.x)
		{//����ǂݍ��񂾒��_���A��ԏ��������_��菬�����ꍇ
			m_OriginVtxMin.x = vtx.x;
		}
		else if (vtx.y < m_OriginVtxMin.y)
		{//����ǂݍ��񂾒��_���A��ԏ��������_��菬�����ꍇ
			m_OriginVtxMin.y = vtx.y;
		}
		else if (vtx.z < m_OriginVtxMin.z)
		{//����ǂݍ��񂾒��_���A��ԏ��������_��菬�����ꍇ
			m_OriginVtxMin.z = vtx.z;
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
	m_VtxMax.x = m_OriginVtxMax.x * m_Scale.x;
	m_VtxMax.y = m_OriginVtxMax.y * m_Scale.y;
	m_VtxMax.z = m_OriginVtxMax.z * m_Scale.z;
	m_VtxMin.x = m_OriginVtxMin.x * m_Scale.x;
	m_VtxMin.y = m_OriginVtxMin.y * m_Scale.y;
	m_VtxMin.z = m_OriginVtxMin.z * m_Scale.z;
	//================================================================================================================================================

	//==========================================================
	//�ő�ŏ����Q�Ƃ��ăT�C�Y��ݒ肷��
	//==========================================================
	m_Size = m_VtxMax - m_VtxMin;
	//================================================================================================================================================

}
//================================================================================================================================================

//============================================================================
//�����̎�ނ�ݒ�
//============================================================================
void CObjectX::SetRotType(ROTTYPE RecvRotType)
{
	//m_RotType = RecvRotType;

	//if (m_RotType == ROTTYPE_NORMAL)
	//{
	//	m_Rot = NULL_VECTOR3;
	//	m_VtxMax.x = m_OriginVtxMax.x * m_Scale.x;
	//	m_VtxMax.z = m_OriginVtxMax.z * m_Scale.z;

	//	m_VtxMin.x = m_OriginVtxMin.x * m_Scale.x;
	//	m_VtxMin.z = m_OriginVtxMin.z * m_Scale.z;

	//	m_VtxMax.y = m_OriginVtxMax.y * m_Scale.y;
	//	m_VtxMin.y = m_OriginVtxMin.y * m_Scale.y;
	//	m_Size = m_VtxMax - m_VtxMin;
	//}
	//else if (m_RotType == ROTTYPE_Z)
	//{
	//	m_Rot.y = D3DX_PI / 2;
	//	m_Rot.x = 0.0f;
	//	m_Rot.z = 0.0f;

	//	m_VtxMax.x = m_OriginVtxMax.z * m_Scale.z;
	//	m_VtxMax.z = m_OriginVtxMax.x * m_Scale.x;

	//	m_VtxMin.x = m_OriginVtxMin.z * m_Scale.z;
	//	m_VtxMin.z = m_OriginVtxMin.x * m_Scale.x;

	//	m_VtxMax.y = m_OriginVtxMax.y * m_Scale.y;
	//	m_VtxMin.y = m_OriginVtxMin.y * m_Scale.y;

	//	m_Size = m_VtxMax - m_VtxMin;
	//}
}
//================================================================================================================================================

//===================================================================================================================
//�ʒu���X�V������i�ʒu���X�V������Ƀu���b�N�Ƃ̓����蔻����������̂ŁA�C�ӂ̃^�C�~���O�ŌĂׂ�悤�ɂ���j
//===================================================================================================================
void CObjectX::UpdatePos()
{
	m_PosOld = m_Pos;   
	m_Pos += m_Move;   //�ʒu�̍X�V
	if (m_bUseInteria == true)
	{
		m_Move.x += (0.0f - m_Move.x) * m_fInertia;
		m_Move.z += (0.0f - m_Move.z) * m_fInertia;
	}
}
//================================================================================================================================================

//===================================================================================================================
//�G�f�B�^����g�嗦��ύX����
//===================================================================================================================
void CObjectX::ChengeEditScale()
{
	D3DXVECTOR3& Scale = GetScale();//�g�嗦

	ChengeEditScaleX();

	ChengeEditScaleY();

	ChengeEditScaleZ();

	CManager::GetDebugText()->PrintDebugText("�g�嗦(YUI�L�[) %f %f %f\n", Scale.x,Scale.y,Scale.z);
}
//================================================================================================================================================

//===================================================================================================================
//�G�f�B�^����g�嗦X��ύX����
//===================================================================================================================
void CObjectX::ChengeEditScaleX()
{
	D3DXVECTOR3& Scale = GetScale();                                //�g�嗦

	if (CManager::GetInputKeyboard()->GetPress(DIK_LCONTROL) == true)
	{//L�R���g���[���L�[�������Ȃ���
		if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
		{//�V�t�g�L�[�������Ȃ���E�E�E
			if (CManager::GetInputKeyboard()->GetPress(DIK_Y) == true)
			{
				Scale.x -= 0.1f;
			}
		}
		else if (CManager::GetInputKeyboard()->GetPress(DIK_Y) == true)
		{
			Scale.x += 0.1f;
		}
	}
	else
	{//L�R���g���[���L�[�������Ă��Ȃ�
		if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
		{//�V�t�g�L�[�������Ȃ���E�E�E
			if (CManager::GetInputKeyboard()->GetTrigger(DIK_Y) == true)
			{
				Scale.x -= 0.1f;
			}
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_Y) == true)
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
	D3DXVECTOR3& Scale = GetScale();                                //�g�嗦

	if (CManager::GetInputKeyboard()->GetPress(DIK_LCONTROL) == true)
	{//L�R���g���[���L�[�������Ȃ���
		if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
		{//�V�t�g�L�[�������Ȃ���E�E�E
			if (CManager::GetInputKeyboard()->GetPress(DIK_U) == true)
			{
				Scale.y -= 0.1f;
			}
		}
		else if (CManager::GetInputKeyboard()->GetPress(DIK_U) == true)
		{
			Scale.y += 0.1f;
		}
	}
	else
	{//L�R���g���[���L�[�������Ă��Ȃ�
		if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
		{//�V�t�g�L�[�������Ȃ���E�E�E
			if (CManager::GetInputKeyboard()->GetTrigger(DIK_U) == true)
			{
				Scale.y -= 0.1f;
			}
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_U) == true)
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
	D3DXVECTOR3& Scale = GetScale();                                //�g�嗦
	if (CManager::GetInputKeyboard()->GetPress(DIK_LCONTROL) == true)
	{//L�R���g���[���L�[�������Ȃ���
		if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
		{//�V�t�g�L�[�������Ȃ���E�E�E
			if (CManager::GetInputKeyboard()->GetPress(DIK_I) == true)
			{
				Scale.z -= 0.1f;
			}
		}
		else if (CManager::GetInputKeyboard()->GetPress(DIK_I) == true)
		{
			Scale.z += 0.1f;
		}
	}
	else
	{//L�R���g���[���L�[�������Ă��Ȃ�
		if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
		{//�V�t�g�L�[�������Ȃ���E�E�E
			if (CManager::GetInputKeyboard()->GetTrigger(DIK_I) == true)
			{
				Scale.z -= 0.1f;
			}
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_I) == true)
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
	D3DXVECTOR3& Pos = GetPos();                                    //���f���̈ʒu�̎擾
	D3DXVECTOR3& Scale = GetScale();                                //�g�嗦
	D3DXVECTOR3& SupportPos = GetSupportPos();                      //���f���̎x�_�ʒu�̎擾
	D3DXVECTOR3& PosOld = GetPosOld();                              //1f�O�̈ʒu��ݒ�
	D3DXVECTOR3& Rot = GetRot();                                    //���f���̌����̎擾
	D3DXVECTOR3 Size = GetSize();                                   //�T�C�Y���擾
	float fMoveX = 0.0f;                                            //X�����̈ړ���
	float& fInertia = GetInertia();                                 //�������擾
	float fMoveZ = 0.0f;                                            //Z�����̈ړ���
	bool bMove = false;                                             //�ړ����Ă��邩�ǂ��� 
	 
	SetColor(CObjectX::COLORTYPE_ALPHA, 10);                         //�F�𔼓����ɂ���

	//�ʒu���x�_�ɌŒ�
	Pos = SupportPos;
	//=====================================================
    //�ړ�����
    //=====================================================
   	if (CManager::GetInputKeyboard()->GetPress(DIK_UP) == true)
	{
		fMoveZ = 1.0f;
	}
	else if (CManager::GetInputKeyboard()->GetPress(DIK_DOWN) == true)
	{
		fMoveZ = -1.0f;
	}

	if (CManager::GetInputKeyboard()->GetPress(DIK_RIGHT) == true)
	{
		fMoveX = 1.0f;
	}
	else if (CManager::GetInputKeyboard()->GetPress(DIK_LEFT) == true)
	{
		fMoveX = -1.0f;
	}

	//============================
	// �ړ��{�^���������Ă�����
	//============================
	if (fMoveX != 0.0f || fMoveZ != 0.0f)
	{
		bMove = true;//�ړ����
	}
	else
	{
		bMove = false;//�ҋ@���
	}
	if (bMove == true)
	{//�ړ����Ă�����
		Pos.x += sinf(atan2f(fMoveX, fMoveZ)) * 5.0f;
		Pos.y += cosf(atan2f(fMoveX, fMoveZ)) * 5.0f;
	}

	//===========================
	//RT�{�^���������Ă�����
	//===========================
	if (CManager::GetInputKeyboard()->GetPress(DIK_LCONTROL) == true)
	{//L�R���g���[���L�[�������Ȃ���
		if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
		{//�V�t�g�L�[�������Ȃ���E�E�E
			if (CManager::GetInputKeyboard()->GetPress(DIK_F) == true)
			{
				Rot.z -= 0.01f;
			}
		}
		else if (CManager::GetInputKeyboard()->GetPress(DIK_G) == true)
		{
			Rot.z += 0.01f;
		}
	}
	else
	{//L�R���g���[���L�[�������Ă��Ȃ�
		if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
		{//�V�t�g�L�[�������Ȃ���E�E�E
			if (CManager::GetInputKeyboard()->GetTrigger(DIK_F) == true)
			{
				Rot.z -= 0.01f;
			}
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_G) == true)
		{
			Rot.z += 0.01f;
		}
	}

	//�x�_���ꏏ�Ɉړ�
	SupportPos = Pos;
	CManager::GetDebugText()->PrintDebugText("�x�_�ʒu(���L�[) %f %f %f\n", SupportPos.x, SupportPos.y, SupportPos.z);
	CManager::GetDebugText()->PrintDebugText("����Z(FG�L�[) %f\n",Rot.z);
	//================================================================================================================================================

}
//================================================================================================================================================

//============================================================================
//�_���[�W��^����
//============================================================================
void CObjectX::SetDamage(int nDamage, int nHitStopTime)
{
	if (m_bHitStop == false && nDamage > 0)
	{//�q�b�g�X�g�b�v��Ԃ���Ȃ����
		m_bHitStop = true;              //�q�b�g�X�g�b�v��Ԃɂ���
		m_nHitStopTime = nHitStopTime;  //�q�b�g�X�g�b�v����
		for (int nCnt = 0; nCnt < 3; nCnt++)
		{
			float fRandSpeed = float(rand() % 100) / 10;//�����̗����l
			float fRandRot = float(rand() % 628) / 100;  //���ˌ����̗����l
			D3DXVECTOR3 Pos = CObjectX::GetPos();//�ʒu���擾
			//CParticle::Create(50, 30.0f, 30.0f, Pos, D3DXVECTOR3(sinf(fRandRot) * fRandSpeed, cosf(fRandRot) * fRandSpeed, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		}
		m_nLife -= nDamage;
		SetColor(COLORTYPE_RED, 10);
	}
}
//================================================================================================================================================

//============================================================================
//�񕜗ʂ����蓖�Ă�
//============================================================================
void CObjectX::SetHeal(int nHeal, D3DXCOLOR col, float fWidth, float fHeight)
{
	m_nLife += nHeal;
	CDamage::Create(nHeal, m_Pos, col, fWidth, fHeight);
}
//================================================================================================================================================

//============================================================================
//�_�����}�e���A���ԍ��̃e�N�X�`�������蓖�Ă�
//============================================================================
void CObjectX::SetLockOnMatBindTexture(int nNumMat, LPDIRECT3DTEXTURE9 pTexture)
{
	m_ObjectXInfo.pTexture[nNumMat] = pTexture;
}
//================================================================================================================================================

//============================================================================
//�e��`�悷��
//============================================================================
void CObjectX::DrawShadow(D3DXVECTOR3 Pos)
{
	D3DXMATRIX mtxShadow,mtxScale,mtxRot,mtxTrans;                  //�v�Z�p�}�g���b�N�X
	D3DXPLANE plane;                       //�v���[��
	D3DXVECTOR4 vecLight;                  //���C�g�t����
	D3DXVECTOR3 ShadowPos, ShadowNor;      //���ʏ�̈�]��
	D3DXMATERIAL* pMat;                                               //�}�e���A���f�[�^�ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice(); //�f�o�C�X�ւ̃|�C���^�Ǝ擾
	D3DMATERIAL9 matDef;                                              //���݂̃}�e���A���ۑ��p

    //�e�̃}�g���b�N�X��������
	D3DXMatrixIdentity(&mtxShadow);

	//���C�g�̋t�����x�N�g����ݒ�
	vecLight = D3DXVECTOR4(-100.0f, 300.0f,300.0f, 0.0f);
	ShadowPos = NULL_VECTOR3;
	ShadowNor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//�@���ƕ��ʏ�̈�_���畽�ʏ����쐬
	D3DXPlaneFromPointNormal(&plane, &ShadowPos, &ShadowNor);

	//���C�g�̕��ʏ�񂩂�e�s����쐬
	D3DXMatrixShadow(&mtxShadow, &vecLight, &plane);

	//�傫���𔽉f
	D3DXMatrixScaling(&mtxScale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixMultiply(&mtxShadow, &mtxShadow, &mtxScale);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot,m_Rot.y, m_Rot.x, m_Rot.z);
	//D3DXMatrixMultiply(&mtxShadow, &mtxShadow, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_Pos.x,0.0f, m_Pos.z);
	D3DXMatrixMultiply(&mtxShadow, &mtxShadow, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtxShadow);

	//=======================================
    //�`��̒���
    //=======================================

    //�@���̒������P�ɂ���B�i�X�P�[���Ȃǂ��g�������́A�K���g���B)
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
	//================================================================================================================

	//�}�e���A���ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)m_ObjectXInfo.pBuffMat->GetBufferPointer();

	//==========================================================================
	//�}�e���A���̐����A�e�N�X�`����ǂݍ��ށB
	//==========================================================================
	for (int nCntMat = 0; nCntMat < (int)m_ObjectXInfo.dwNumMat; nCntMat++)
	{
		//�F�����̐ݒ�
		pMat[nCntMat].MatD3D.Diffuse = D3DXCOLOR(0.0f,0.0f,0.0f,1.0f);

		//�}�e���A���̐ݒ�
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		//�e�N�X�`����ݒ肷��
		pDevice->SetTexture(0,NULL);

		//���f���i�p�[�c�j�̕`��
		m_ObjectXInfo.pMesh->DrawSubset(nCntMat);
	}
	//================================================================================================================

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
//�d�͂̏���
//============================================================================
void CObjectX::GravityProcess()
{
	if (m_bIsLanding == true)
	{
		m_Move.y = 0.0f;
	}
	if (m_bUseGravity == true)
	{
		m_Move.y += -m_fNORMAL_GRAVITY;
	}

}
//================================================================================================================================================

//============================================================================
//���_�ɓ��B�����Ƃ��Ɉʒu�̕␳���s�����ǂ���
//============================================================================
void CObjectX::AdjusOriginPosProcess()
{
	//if (m_Pos.y + m_VtxMin.y <= 0.0f && m_bUseAdjustOriginPos == true && CManager::GetCamera()->GetCameraType() == CCamera::CAMERATYPE_BOSSBATTLE)
	//{
	//	m_bIsLanding = true;
	//	m_Pos.y = fabsf(GetVtxMin().y);
	//}
}
//================================================================================================================================================

//============================================================================
//���\������
//============================================================================
void CObjectX::DispInfo()
{
	ChengeEditPos();//�ʒu��ς���

	ChengeEditScale();//�g�嗦��ς���
}
//================================================================================================================================================
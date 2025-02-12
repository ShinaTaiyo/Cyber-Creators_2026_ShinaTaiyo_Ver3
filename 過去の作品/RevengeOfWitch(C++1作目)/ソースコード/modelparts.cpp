//====================================================================================
//
//�V���Q�X���F�K�w�\�������[modelparts.cpp]
//Author:ShinaTaiyo
//
//====================================================================================

//============================================
//�C���N���[�h
//============================================
#include "modelparts.h"
#include "manager.h"
#include "renderer.h"
#include "objectXInfo.h"
//====================================================================================

//============================================
//�R���X�g���N�^
//============================================
CModelParts::CModelParts(int nPriority) : m_pMtxParent(),m_OffSetPos(NULL_VECTOR3),m_OffSetRot(NULL_VECTOR3)
{

}
//====================================================================================

//============================================
//�f�X�g���N�^
//============================================
CModelParts::~CModelParts()
{

}
//====================================================================================

//============================================
//����������
//============================================
HRESULT CModelParts::Init()
{
	CObjectX::Init();//�I�u�W�F�N�gX����������

	m_OffSetPos = NULL_VECTOR3;//�I�t�Z�b�g�ʒu
	m_OffSetSupportPos = NULL_VECTOR3;//�ŏ��̃I�t�Z�b�g�̈ʒu
	m_OffSetRot = NULL_VECTOR3;//�I�t�Z�b�g����
	m_OffSetSupportRot = NULL_VECTOR3;//�ŏ��̃I�t�Z�b�g�̌���
	m_pMtxParentParts = {};  //�e���f���p�[�c�ւ̃|�C���^
	m_pMtxParent = {};       //�e���f���ւ̃|�C���^
	m_WorldPos = NULL_VECTOR3; //���[���h���W
	return S_OK;
}
//====================================================================================

//============================================
//�I������
//============================================
void CModelParts::Uninit()
{
	CObjectX::Uninit();
}
//====================================================================================

//============================================
//���S�t���O�ݒ菈��
//============================================
void CModelParts::SetDeath()
{
	CObject::SetDeath();
}
//====================================================================================

//============================================
//�X�V����
//============================================
void CModelParts::Update()
{
	CObjectX::Update();//�I�u�W�F�N�gX�X�V����
}
//====================================================================================

//============================================
//�`�揈��
//============================================
void CModelParts::Draw()
{

}
//================================================================================================================================================

//====================================================================
//�ʘg�̕`�揈��
//====================================================================
void CModelParts::ExtraDraw()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice(); //�f�o�C�X�ւ̃|�C���^�Ǝ擾
	D3DXMATRIX mtxRot, mtxTrans, mtxScale;                            //�v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;                                              //���݂̃}�e���A���ۑ��p
	D3DXMATERIAL* pMat;                                               //�}�e���A���f�[�^�ւ̃|�C���^
	D3DXMATRIX& mtxWorld = CObjectX::GetMatrixWorld();                //���[���h�}�g���b�N�X�擾
	D3DXMATRIX mtxParentParts = {};                                   //�e�p�[�c�̃��[���h�}�g���b�N�X
	D3DXMATRIX mtxParent = {};                                        //�e�p�[�c�̃��[���h�}�g���b�N�X
	D3DXVECTOR3 Scale = GetScale();                                   //�g�嗦���擾����
	if (m_pMtxParentParts != nullptr)
	{//�p�[�c���m�̒��ɐe��������
		mtxParentParts = *m_pMtxParentParts;
	}
	else
	{//�p�[�c���m�ɐe�����Ȃ��i�e���I�u�W�F�N�g�Ȃǁj
		mtxParent = *m_pMtxParent;
	}

	//���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxWorld);

	//�傫���𔽉f
	D3DXMatrixScaling(&mtxScale, Scale.x, Scale.y, Scale.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScale);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_OffSetRot.y, m_OffSetRot.x, m_OffSetRot.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_OffSetPos.x, m_OffSetPos.y, m_OffSetPos.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

	if (m_pMtxParentParts != nullptr)
	{
		//�e�̃}�g���b�N�X�Ƃ������킹��
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxParentParts);
	}
	else
	{
		//�ŐV�̃��[���h�ϊ��s����擾�i�v���C���[�A�G�̍s��Ȃǁj
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxParent);
	}
	//if (CObjectX::GetObjectXInfo().Diffuse[0].a > 0.0f)
	//{
	//	//�e�̕`��
	//	DrawShadow(m_Pos);
	//}

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	D3DXVECTOR3 PosZero = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVec3TransformCoord(&m_WorldPos, &PosZero, &mtxWorld);//���̈����̃��[�J�����W�ɐ^�񒆂̈����̃��[���h���W��������

	//=======================================
	//�`��̒���
	//=======================================

	//�@���̒������P�ɂ���B�i�X�P�[���Ȃǂ��g�������́A�K���g���B)
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
	//================================================================================================================

	//�}�e���A���ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)CObjectX::GetObjectXInfo().pBuffMat->GetBufferPointer();

	//==========================================================================
	//�}�e���A���̐����A�e�N�X�`����ǂݍ��ށB
	//==========================================================================
	for (int nCntMat = 0; nCntMat < (int)CObjectX::GetObjectXInfo().dwNumMat; nCntMat++)
	{
		//�F�����̐ݒ�
		pMat[nCntMat].MatD3D.Diffuse = CObjectX::GetObjectXInfo().Diffuse[nCntMat];

		//�}�e���A���̐ݒ�
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		//�e�N�X�`����ݒ肷��
		pDevice->SetTexture(0, CObjectX::GetObjectXInfo().pTexture[nCntMat]);

		//���f���i�p�[�c�j�̕`��
		CObjectX::GetObjectXInfo().pMesh->DrawSubset(nCntMat);
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

//=====================================================================================================================
//��������
//=====================================================================================================================
CModelParts* CModelParts::Create(char* aString, D3DXVECTOR3 OffsetPos, D3DXVECTOR3 OffSetRot)
{
	CModelParts* pModelParts = DBG_NEW CModelParts;                                                                             //�v���C���[�𐶐�

	bool bSuccess = pModelParts->CObject::GetCreateSuccess();
	int nIdx = 0;//���f���̃C���f�b�N�X
	if (bSuccess == true)
	{
		if (pModelParts != nullptr)
		{
			pModelParts->Init();                             //����������
			pModelParts->m_OffSetPos = OffsetPos;            //�I�t�Z�b�g�ʒu
			pModelParts->m_OffSetSupportPos = OffsetPos;     //�ŏ��̈ʒu��ݒ�
			pModelParts->m_OffSetRot = OffSetRot;            //�I�t�Z�b�g����
			pModelParts->m_OffSetSupportRot = OffSetRot;     //�ŏ��̃I�t�Z�b�g�̌�����ݒ�
			pModelParts->SetScale(ONE_VECTOR3);              //�g�嗦��ݒ肷��
			pModelParts->SetUseDeath(false);                 //���S�t���O�𔭓����邩�ǂ�����ݒ肷��
			nIdx = CManager::GetObjectXInfo()->Regist(CObjectX::OBJECTXTYPE_PLAYER,aString);
			pModelParts->CObjectX::BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(CObjectX::OBJECTXTYPE_PLAYER, nIdx),
				CManager::GetObjectXInfo()->GetBuffMat(CObjectX::OBJECTXTYPE_PLAYER, nIdx),
				CManager::GetObjectXInfo()->GetdwNumMat(CObjectX::OBJECTXTYPE_PLAYER, nIdx),
				CManager::GetObjectXInfo()->GetTexture(CObjectX::OBJECTXTYPE_PLAYER, nIdx),
				CManager::GetObjectXInfo()->GetColorValue(CObjectX::OBJECTXTYPE_PLAYER, nIdx));                  //���f���������蓖�Ă�

			pModelParts->SetRotType((CObjectX::ROTTYPE)(0));                                                        //�����̎�ނ�ݒ�
			pModelParts->SetSize();                                                                                 //X�I�u�W�F�N�g�̃T�C�Y��ݒ肷��
		}
	}
	else
	{//�I�u�W�F�N�g�ɋ󂫂��Ȃ������̂Ŕj������
		delete pModelParts;
		pModelParts = nullptr;
	}

	return pModelParts;
}
//================================================================================================================================================
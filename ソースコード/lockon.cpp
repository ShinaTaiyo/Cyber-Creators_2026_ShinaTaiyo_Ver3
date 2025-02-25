//==============================================================================================================
//
//�P�O���Q�W���F���b�N�I�������ꏊ�ɒe����������[lockon.cpp]
//Author:ShinaTaiyo
//
//==============================================================================================================

//===============================================================
//�C���N���[�h
//===============================================================
#include "lockon.h"
#include "manager.h"
#include "camera.h"
#include "texture.h"
#include "collision.h"
#include "input.h"
#include "particle.h"
#include "particle2d.h"
#include "calculation.h"
#include "objectX.h"
#include "game.h"
#include "debugtext.h"
#include "player.h"
#include "ui.h"
//==============================================================================================================

//===============================================================
//�ÓI�����o�錾
//===============================================================

//�e�N�X�`���t�@�C����
const string CLockon::s_LOCKON_FILENAME[static_cast<int>(CLockon::TYPE::MAX)] =
{
	"data\\TEXTURE\\LockOn\\Target_000.png",
	"data\\TEXTURE\\LockOn\\DiveTarget_001.png",
	"data\\TEXTURE\\LockOn\\StuckShotTarget_000.png"
};

//===============================================================
//�R���X�g���N�^
//===============================================================
CLockon::CLockon(int nPri, bool bUseintPri, CObject::TYPE type, CObject::OBJECTTYPE ObjType) : CObject2D(nPri,bUseintPri,type,ObjType),
m_LockOnPos(D3DXVECTOR3(0.0f,0.0f,0.0f)),m_NowRay(D3DXVECTOR3(0.0f,0.0f,0.0f)),m_FrontPos(D3DXVECTOR3(0.0f,0.0f,0.0f)),m_EndState(ENDSTATE::NONE),m_NearRayColObjPos(D3DXVECTOR3(0.0f,0.0f,0.0f)),
m_bRayCollision(false),m_Type(TYPE::SHOT)
{

}
//==============================================================================================================

//===============================================================
//�f�X�g���N�^
//===============================================================
CLockon::~CLockon()
{

}
//==============================================================================================================

//===============================================================
//����������
//===============================================================
HRESULT CLockon::Init()
{
	CObject2D::Init();//�I�u�W�F�N�g2D
	return S_OK;
}
//==============================================================================================================

//===============================================================
//�I������
//===============================================================
void CLockon::Uninit()
{
	CObject2D::Uninit();//�I�u�W�F�N�g2D
}
//==============================================================================================================

//===============================================================
//�X�V����
//===============================================================
void CLockon::Update()
{
	D3DXVECTOR3 Rot = GetRot();//����
	Rot.z += 0.02f;//�|���S������]������
	SetRot(Rot);//������ݒ肷��

	//�ړ�����
	MoveProcess();

	//���C���_���Ă��鉜�̕ǂ̔���ʒu���T�[�`����
	BackWallRayCollisionPosSearch();

	//���C���v�Z����
	CalcRay();

	//��ԋ߂��I�u�W�F�N�g�̃��C�����������ʒu�����߂�
	RayCollisionToObject();

	//�I�u�W�F�N�g2D�X�V����
	CObject2D::Update();
}
//==============================================================================================================

//===============================================================
//�`�揈��
//===============================================================
void CLockon::Draw()
{
	CObject2D::Draw();//�I�u�W�F�N�g2D�̕`�揈��
}
//==============================================================================================================

//===============================================================
//���S�t���O�ݒ菈��
//===============================================================
void CLockon::SetDeath()
{
	CObject2D::SetDeath();//�I�u�W�F�N�g2D�̎��S�t���O�ݒ菈��
}
//==============================================================================================================

//===============================================================
//��������
//===============================================================
CLockon* CLockon::Create(TYPE Type, D3DXVECTOR3 Pos, CObject2D::POLYGONTYPE PolygonType, float fWidth, float fHeight, D3DXCOLOR col)
{
	CTexture* pTexture = CManager::GetTexture();       //�e�N�X�`���N���X
	CLockon* pLockOn = DBG_NEW CLockon;                //���b�N�I��

	pLockOn->Init();                                   //����������
	pLockOn->SetPos(Pos);                              //�ʒu�ݒ�
	pLockOn->SetSupportPos(Pos);                       //�x�_�ʒu�ݒ�
	pLockOn->SetAnimInfo(1, 1,false);                  //�A�j���[�V��������ݒ�i�K���j
	pLockOn->SetPolygonType(PolygonType);              //�|���S���̒��S�_�̎�ނ�ݒ肷��
	pLockOn->SetWidth(fWidth);                         //������ݒ�
	pLockOn->SetMaxWidth(fWidth);                      //�ő剡����ݒ�
	pLockOn->SetHeight(fHeight);                       //������ݒ�
	pLockOn->SetMaxHeight(fHeight);                    //�ő卂����ݒ�
	pLockOn->m_Type = Type;                            //��ނ�ݒ�

	//�e�N�X�`���ݒ�
	pLockOn->SetTextureIndex(pTexture->Regist(s_LOCKON_FILENAME[static_cast<int>(Type)])); //�e�N�X�`����o�^
	pLockOn->BindTexture(pTexture->GetAddress(pLockOn->GetTextureIndex()));                //�o�^�A�w�肵���e�N�X�`���ԍ��̃A�h���X���擾

	//�̗͂��g�p���Ȃ�
	pLockOn->SetUseLife(false, 1, 1);

	//�I�u�W�F�N�g�^�C�v�ݒ�
	pLockOn->SetType(CObject::TYPE::LOCKON);


	return pLockOn;
}
//==============================================================================================================

//===============================================================
//�e�N�X�`����ς���
//===============================================================
void CLockon::ChengeTexture(TYPE Type)
{
	m_Type = Type;//��ނ�ݒ�

	CTexture* pTexture = CManager::GetTexture();//�e�N�X�`���Ǘ��N���X�ւ̃|�C���^

	//�ς������b�N�I���̃e�N�X�`����傫���\�����鉉�o
	CLockon* pLockOn = CLockon::Create(m_Type, GetPos(), GetPolygonType(), GetWidth(), GetHeight(), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	pLockOn->SetUseAddScale(D3DXVECTOR2(s_fLOCKONSTAGING_ADDSCALE, s_fLOCKONSTAGING_ADDSCALE), true);//���o�p�ɕς������[�h���g�傷��
	pLockOn->SetUseLifeRatioColor(true);                                                             //�̗͂̊����ɉ����ē����x�𔖂�����
	pLockOn->SetUseLife(true, s_nLOCKONSTAGING_LIFE, s_nLOCKONSTAGING_LIFE);                         //�̗͂��g�p����
	pLockOn->SetUseDeath(true);                                                                      //���S�t���O���g�p����

	//�e�N�X�`���ݒ�
	SetTextureIndex(pTexture->Regist(s_LOCKON_FILENAME[static_cast<int>(m_Type)]));//�e�N�X�`����o�^
	BindTexture(pTexture->GetAddress(GetTextureIndex()));//�o�^�A�w�肵���e�N�X�`���ԍ��̃A�h���X���擾

}
//==============================================================================================================

//===============================================================
//�ړ�����
//===============================================================
void CLockon::MoveProcess()
{

}
//==============================================================================================================

//===============================================================
//�J�[�\���̐�ɂ���I�u�W�F�N�g���T�[�`
//===============================================================
void CLockon::BackWallRayCollisionPosSearch()
{
	D3DXVECTOR3 Pos = GetPos();//�ʒu

	CCalculation::CalcScreenToWorld(&m_LockOnPos,GetPos().x,GetPos().y, 1.0f,SCREEN_WIDTH,SCREEN_HEIGHT,CManager::GetCamera()->GetMtxView(),
		CManager::GetCamera()->GetMtxProjection()); //�`��͈͂̈�ԉ��̈ʒu
}
//==============================================================================================================

//===============================================================
//���C���v�Z����
//===============================================================
void CLockon::CalcRay()
{
	D3DXVECTOR3 FarPos = D3DXVECTOR3(0.0f,0.0f,0.0f); //��
	CCamera* pCamera = CManager::GetCamera();         //�J�����ւ̃|�C���^���擾
	//============================================
	//�J������O�Ɖ��̃��[���h���W�����߂�
	//============================================
	CCalculation::CalcScreenToWorld(&m_FrontPos,GetPos().x,GetPos().y, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT,
		CManager::GetCamera()->GetMtxView(), CManager::GetCamera()->GetMtxProjection());//��O

	//�f�o�b�O�\��
	CManager::GetDebugText()->PrintDebugText("�J������O���W�F%f %f %f\n", m_FrontPos.x, m_FrontPos.y, m_FrontPos.z);
	CManager::GetDebugText()->PrintDebugText("�J�����x�_�F%f %f %f\n", pCamera->GetPosV().x, pCamera->GetPosV().y, pCamera->GetPosV().z);

	CCalculation::CalcScreenToWorld(&FarPos,GetPos().x,GetPos().y, 1.0f, SCREEN_WIDTH, SCREEN_HEIGHT,
		CManager::GetCamera()->GetMtxView(), CManager::GetCamera()->GetMtxProjection());//��
	//============================================================================================================================

	m_NowRay = FarPos - m_FrontPos;          //��O���W�Ɖ����W�̃x�N�g�������߂�
	D3DXVec3Normalize(&m_NowRay, &m_NowRay); //�x�N�g���𐳋K������
}
//==============================================================================================================

//===============================================================
//���C���ǂ̃I�u�W�F�N�g�ɓ������Ă��邩���v�Z����
//===============================================================
void CLockon::RayCollisionToObject()
{
	//*�ϐ��錾
	bool bRayCollision = false;//���C�������������̏�Ԃ����Z�b�g
	vector<D3DXVECTOR3> VecCollisionSuccess;     //�����蔻�肪���������ʒu��vector
	CPlayer* pPlayer = CGame::GetPlayer();

	//���C�ƈ�v�����S�ẴI�u�W�F�N�g�����߁A���S�_��Vector�ɕۑ�
	for (int nCntPri = 0; nCntPri < CObject::m_nMAXPRIORITY; nCntPri++)
	{
		CObject* pObj = CObject::GetTopObject(nCntPri);//�擪�I�u�W�F�N�g���擾
		D3DXVECTOR3 CollisionStartPos = D3DXVECTOR3(0.0f,0.0f,0.0f);//�Փ˔���J�n�ʒu
		D3DXVECTOR3 CollisionEndPos = D3DXVECTOR3(0.0f,0.0f,0.0f);  //�Փ˔���I���ʒu
		while (pObj != nullptr)
		{
			CObject* pNext = pObj->GetNextObject();//���̃I�u�W�F�N�g�̃|�C���^���擾

			if (pObj->GetType() == CObject::TYPE::ENEMY || pObj->GetType() == CObject::TYPE::BLOCK || pObj->GetType() == CObject::TYPE::BGMODEL)
			{
				CObjectX* pObjX = dynamic_cast<CObjectX*>(pObj);
				//�w�肵�����f���̈ʒu
				bRayCollision = CCollision::RayIntersectsAABBCollisionPos(m_FrontPos, m_NowRay, pObjX->GetSizeInfo().GetVtxMin() + pObjX->GetPosInfo().GetPos(), pObjX->GetSizeInfo().GetVtxMax() + pObjX->GetPosInfo().GetPos(),
					CollisionStartPos);

				if (CCalculation::CalculationLength(m_FrontPos, CollisionStartPos) < CManager::GetCamera()->GetPosRToPosVLength())
				{//���������������J�����̒����_�Ǝ��_�̋��������߂����
					bRayCollision = false;//������Ȃ��������Ƃɂ���
				}

				// ���C����������
				if (bRayCollision == true)
				{
					D3DXVECTOR3 ScreenPos = CCalculation::CalcWorldToScreenNoViewport(pObjX->GetPosInfo().GetSenterPos(), *CManager::GetCamera()->GetMtxView(), *CManager::GetCamera()->GetMtxProjection(),
						float(SCREEN_WIDTH), float(SCREEN_HEIGHT));//�Ώۂ̃X�N���[�����W���擾
					VecCollisionSuccess.push_back(CollisionStartPos);//�����蔻�肪���������I�u�W�F�N�g�̔���J�n�ʒu��ۑ�����
				}

			}

			pObj = pNext;//�I�u�W�F�N�g�����ɐi�߂�
		}
	}

	D3DXVECTOR3 NearCollisionPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f); //�����蔻�肪���������ʒu�̒��ň�ԋ߂��ʒu

	//���C�̕�������v�����I�u�W�F�N�g�����݂�����A���̒��ň�ԋ������߂��I�u�W�F�N�g�̒��S�_�����߁A������ڊ|�����ړ��ʂ����߂�
	if (VecCollisionSuccess.size() != 0)
	{//�_���Ă���I�u�W�F�N�g�̒��S�_�Ɍ������Č���
		float fLength = 0.0f;//����
		float fMinLength = 0.0f;//��ԋ߂������i�[�p
		for (auto it = VecCollisionSuccess.begin(); it != VecCollisionSuccess.end(); ++it)
		{
			fLength = CCalculation::CalculationLength(m_FrontPos, *it);//���C�̔��肪���������I�u�W�F�N�g�̈ʒu�ƃv���C���[�̒��S�_�̋����𑪂�

			if (it == VecCollisionSuccess.begin())
			{
				fMinLength = fLength;
				NearCollisionPos = *it;
			}
			else
			{
				if (fLength < fMinLength)
				{//��ԋ߂��������߂�������
					NearCollisionPos = *it;//��ԋ߂��I�u�W�F�N�g���i�[
				}
			}
		}
		m_bRayCollision = true;//���C�̓����蔻�肪����
		m_NearRayColObjPos = NearCollisionPos;//���C������������ԋ߂��I�u�W�F�N�g�Ɍ������Č���
	}
	else
	{//�_���Ă�������̉��̕ǂɌ������Č���
		m_bRayCollision = false;//���C�̓����蔻�肪���s
		m_NearRayColObjPos = m_LockOnPos;//���C�����������I�u�W�F�N�g���Ȃ��̂ŁA���̕ǂɌ������Č���
	}
	//====================================================================================================================================================================

	//Vector���N���A
	VecCollisionSuccess.clear();         //�m�ۂ����z�����ɂ���
	VecCollisionSuccess.shrink_to_fit(); //���݂̃T�C�Y�ɍ��킹�ă������̈���R���p�N�g�ɂ���

}
//==============================================================================================================
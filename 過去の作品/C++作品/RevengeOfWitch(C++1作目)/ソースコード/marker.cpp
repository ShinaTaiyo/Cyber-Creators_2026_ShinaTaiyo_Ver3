//================================================================================
//
//�U���Q�T���F���[�v�}�[�J�[������[marker.cpp]
//Author:ShinaTaiyo
//
//================================================================================

//==============================
//�C���N���[�h
//==============================
#include "marker.h"
#include "manager.h"
#include "bullet.h"
#include "particle.h"
#include "texture.h"
#include "game.h"
#include "edit.h"
#include "manager.h"
#include "objectXInfo.h"
#include "stagemanager.h"
//=================================================

//==============================
//�ÓI�����o������
//==============================
int CMarker::m_nNumFile = CMarker::MARKERTYPE_MAX;//�t�@�C���̐�
const char* CMarker::m_MARKER_FILENAME[CMarker::MARKERTYPE_MAX] =
{ "data\\MODEL\\Marker\\WarpMarker_000.x"};//�}�[�J�[�̃��f���̃p�X
//=================================================

//==============================
//�R���X�g���N�^
//==============================
CMarker::CMarker()
{

}
//=================================================

//==============================
//�f�X�g���N�^
//==============================
CMarker::~CMarker()
{

}
//=================================================

//==============================
//����������
//==============================
HRESULT CMarker::Init()
{
	//===========================
	//�QD�I�u�W�F�N�g������
	//===========================
	if (FAILED(CObjectX::Init()))
	{
		return E_FAIL;
	}
	//====================================

	//===========================
	//�N���X�̕ϐ�������
	//===========================
	m_nCntTime = 0;          //�o�����Ă���̎��Ԃ𑪂�
	m_Aim = NULL_VECTOR3;    //�x�N�g��
	m_fTotalAim = 0.0f;      //�����x�N�g��
	m_nWarpMapNum = 0;       //���[�v����}�b�v�̔ԍ����i�[����
	m_WarpPos = NULL_VECTOR3;//���[�v����ʒu�����肷��
	for (int nCnt = 0; nCnt < m_nWARPMAPDIVISION; nCnt++)
	{
		m_pWarpMapDispNum[nCnt] = nullptr;
	}
	//====================================

	return S_OK;
}
//=================================================

//==============================
//�I������
//==============================
void CMarker::Uninit()
{
	CObjectX::Uninit();
}
//=================================================

//==============================
//�X�V����
//==============================
void CMarker::Update()
{
	//==================================
	//�I�u�W�F�N�g�QD��������擾
	//==================================
	D3DXVECTOR3 Pos = CObjectX::GetPos();
	D3DXVECTOR3 Rot = CObjectX::GetRot();
	//===========================================================

	//==================================
	//���W�X�V
	//==================================
	Rot += ONE_VECTOR3 * 0.1f;
	SetRot(Rot);
	//===========================================================

	//==================================
	//X�I�u�W�F�N�g�X�V
	//==================================
	CObjectX::Update();
	//===========================================================

	//==================================
	//�����蔻������
	//==================================
	Collision();
	//===========================================================
}
//=================================================

//==============================
//�`�揈��
//==============================
void CMarker::Draw()
{
	CObjectX::Draw();

	for (int nCnt = 0; nCnt < m_nWARPMAPDIVISION; nCnt++)
	{
		if (m_pWarpMapDispNum[nCnt] != nullptr)
		{
			m_pWarpMapDispNum[nCnt]->ExtraDraw();
		}
	}
}
//=================================================

//==============================
//���S�t���O��ݒ�
//==============================
void CMarker::SetDeath()
{
	for (int nCnt = 0; nCnt < m_nWARPMAPDIVISION; nCnt++)
	{
		if (m_pWarpMapDispNum[nCnt] != nullptr)
		{
			m_pWarpMapDispNum[nCnt]->SetUseDeath(true);
			m_pWarpMapDispNum[nCnt]->SetDeath();
			m_pWarpMapDispNum[nCnt] = nullptr;
		}
	}
	CObject::SetDeath();
}
//=================================================

//===============================
//�G�̐�������
//===============================
CMarker* CMarker::Create(MARKERTYPE type, D3DXVECTOR3 pos, D3DXVECTOR3 Scale, D3DXVECTOR3 WarpPos, int nMapNum)
{
	CMarker* pMarker = DBG_NEW CMarker;   //�G�𐶐�
	CNumber3D* pNumber3D = nullptr;//���l�擾�p
	bool bSuccess = pMarker->CObject::GetCreateSuccess();
	int nIdx = 0;//���f���̃C���f�b�N�X
	if (bSuccess == true)
	{
		if (pMarker != nullptr)
		{
			pMarker->Init();         //����������
			pMarker->SetUseDeath(true);                                                                  //���S�t���O�𔭓����邩�ǂ�����ݒ肷��
			pMarker->SetUseBlockCollision(false);
			pMarker->m_type = type;  //�e�̎��
			pMarker->SetUseGravity(false);
			pMarker->m_nWarpMapNum = nMapNum;//���[�v����}�b�v�̔ԍ����i�[����
			pMarker->m_WarpPos = WarpPos;    //���[�v����ʒu�����肷��
			pMarker->CObjectX::SetPos(pos);                                                                                  //�I�u�W�F�N�g�Q�c�̈ʒu��ݒ�
			pMarker->SetPosOld(pos);                                                                                         //1f�O�̈ʒu��ݒ�
			pMarker->SetSupportPos(pos);                                                                                     //�ݒu�ʒu
			pMarker->CObject::SetType(CObject::TYPE_MARKER);                                                                  //�I�u�W�F�N�g�̎�ނ����߂�
			pMarker->CObjectX::SetScale(Scale);                                                                              //�I�u�W�F�N�g�̊g�嗦
			pMarker->SetFormarScale(Scale);                                                                                  //���̊g�嗦��ݒ肷��
			pMarker->SetAutoSubLife(false);//�����G�ɑ̗͂����炷���ǂ���
			nIdx = CManager::GetObjectXInfo()->Regist(CObjectX::OBJECTXTYPE_MARKER, m_MARKER_FILENAME[0]);
			pMarker->CObjectX::BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(CObjectX::OBJECTXTYPE_MARKER, nIdx),
				CManager::GetObjectXInfo()->GetBuffMat(CObjectX::OBJECTXTYPE_MARKER, nIdx),
				CManager::GetObjectXInfo()->GetdwNumMat(CObjectX::OBJECTXTYPE_MARKER, nIdx),
				CManager::GetObjectXInfo()->GetTexture(CObjectX::OBJECTXTYPE_MARKER, nIdx),
				CManager::GetObjectXInfo()->GetColorValue(CObjectX::OBJECTXTYPE_MARKER, nIdx));                       //���f���������蓖�Ă�

			pMarker->CObjectX::SetObjXType(CObjectX::OBJECTXTYPE_MARKER);                                                    //�I�u�W�F�N�gX�̃^�C�v��ݒ�
			pMarker->CObjectX::SetTypeNum((int)(type));                                                                      //�I�u�W�F�N�gX���Ƃ̃^�C�v�ԍ���ݒ�
			pMarker->SetSize();                                                                                              //X�I�u�W�F�N�g�̃T�C�Y��ݒ肷��
			pMarker->CObjectX::SetRotType(CObjectX::ROTTYPE::ROTTYPE_NORMAL);                                                //�����̎�ނ�ݒ�
			pMarker->SetManagerType((int)(CStageManager::MANAGEROBJECT_MARKER));                                             //�}�l�[�W���[�ŌĂяo�����̎�ނ�ݒ�

			if (type == MARKERTYPE00_WARP)
			{
				int nCopy = pMarker->m_nWarpMapNum;
				int nNum = 0;
				for (int nCnt = 0; nCnt < m_nWARPMAPDIVISION; nCnt++)
				{
					pMarker->m_pWarpMapDispNum[nCnt] = CNumber3D::Create(40.0f, 40.0f,
						D3DXVECTOR3(pMarker->GetPos().x - 40.0f * nCnt, pMarker->GetPos().y + 50.0f, pMarker->GetPos().z),
						NULL_VECTOR3,
						NORMAL_COL);
					nNum = nCopy % 10;
					pMarker->m_pWarpMapDispNum[nCnt]->SetAnim(nNum);
					nCopy /= 10;
				}
			}
		}
		else
		{//�I�u�W�F�N�g�ɋ󂫂��Ȃ������̂Ŕj������
			delete pMarker;
			pMarker = nullptr;
			return nullptr;
		}
	}
	return pMarker;
}
//=====================================================================================================================================

//===================================================================
//�����蔻������
//===================================================================
void CMarker::Collision()
{
	//==================================
    //�I�u�W�F�N�gX��������擾
    //==================================
	D3DXVECTOR3 Pos = CObjectX::GetPos();                      //�I�u�W�F�N�g�̈ʒu���擾
	D3DXVECTOR3 Size = CObjectX::GetSize();                    //�I�u�W�F�N�g�̃T�C�Y���擾
	D3DXVECTOR3 VtxMax = GetVtxMax();                          //�I�u�W�F�N�g�̍ő咸�_���擾����
	D3DXVECTOR3 VtxMin = GetVtxMin();                          //�I�u�W�F�N�g�̍ŏ����_���擾����
	D3DXVECTOR3 ComparisonPos = NULL_VECTOR3;                  //�v���C���[�̈ʒu����p
	D3DXVECTOR3 ComparisonVtxMax = NULL_VECTOR3;               //����Ώۂ̍ő咸�_
	D3DXVECTOR3 ComparisonVtxMin = NULL_VECTOR3;               //����Ώۂ̍ŏ����_
	CPlayer* pPlayer = nullptr;                                //�v���C���[�N���X�擾�p
	int nMapNum = 0;                                           //���[�v�}�[�J�[�ɕۑ����ꂽ�}�b�v�ԍ��i�[�p�iReleaseAll���Ă΂��̂ŁA�ۑ������ԍ��������Ă��܂�����j
	D3DXVECTOR3 WarpPos = NULL_VECTOR3;                        //���[�v�}�[�J�[�ɕۑ����ꂽ�}�b�v�ԍ��i�[�p�iReleaseAll���Ă΂��̂ŁA�ۑ������ԍ��������Ă��܂�����j
	CObject* pObj = nullptr;                                   //�I�u�W�F�N�g�i�[�p
	//===========================================================

	//==================================
	//�A�C�e���̓����蔻��
	//==================================
	for (int nCntPriority = 0; nCntPriority < m_nMAXPRIORITY; nCntPriority++)
	{
		pObj = GetTopObject(nCntPriority);//�g�b�v�I�u�W�F�N�g���擾
		while (pObj != nullptr)
		{
			//���̃I�u�W�F�N�g���i�[
			CObject* pNext = pObj->GetNextObject();

			//��ނ̎擾�i�G�Ȃ瓖���蔻��j
			CObject::TYPE type = pObj->GetType();

			//�I�u�W�F�N�g�̎�ނ��G��������
			switch (type)
			{
			case CObject::TYPE::TYPE_PLAYER:
				pPlayer = (CPlayer*)pObj;                            //�I�u�W�F�N�g��G�̌^�Ƀ_�E���L���X�g
				ComparisonPos = pPlayer->GetPos();                   //�ʒu���擾
				ComparisonVtxMax = pPlayer->GetVtxMax();             //�ő咸�_���擾
				ComparisonVtxMin = pPlayer->GetVtxMin();             //�ŏ����_���擾
				if (Pos.x + VtxMin.x  <= ComparisonPos.x + ComparisonVtxMax.x &&
					Pos.x + VtxMax.x  >= ComparisonPos.x + ComparisonVtxMin.x &&
					Pos.y + VtxMin.y  <= ComparisonPos.y + ComparisonVtxMax.y &&
					Pos.y + VtxMax.y  >= ComparisonPos.y + ComparisonVtxMin.y &&
					Pos.z + VtxMin.z  <= ComparisonPos.z + ComparisonVtxMax.x &&
					Pos.z + VtxMax.z  >= ComparisonPos.z + ComparisonVtxMin.x)
				{//���[�v�}�[�J�[�ɓ���������
					if (m_type == MARKERTYPE00_WARP)
					{
						nMapNum = m_nWarpMapNum;                                //���[�v��̃}�b�v�ԍ����i�[
						WarpPos = m_WarpPos;                                    //���[�v��̈ʒu���i�[
						if (CScene::GetMode() == CScene::MODE_GAME)
						{
							CGame::GetPlayer()->SetPos(WarpPos);              //���[�v�}�[�J�[�ɋL�^���ꂽ�ʒu�Ƀ��[�v
							CGame::GetPlayer()->SetPosOld(WarpPos);       
							CGame::GetPlayer()->SetReSpawnPos(WarpPos); //�Z�[�u�|�C���g��ݒ肷��
							CGame::GetPlayer()->ResetRespawnCnt();
							ReleaseAll();
							CGame::GetStageManager()->LoadMapTxt(nMapNum);//���[�v�}�[�J�[�ɋL�^���ꂽ�}�b�v�ԍ���ǂݍ���
						}
						else if (CScene::GetMode() == CScene::MODE_EDIT)
						{
							CEdit::GetPlayer()->SetPos(WarpPos);              //���[�v�}�[�J�[�ɋL�^���ꂽ�ʒu�Ƀ��[�v
							CEdit::GetPlayer()->SetPosOld(WarpPos);
							CEdit::GetStageManager()->SaveMapTxt(CEdit::GetStageManager()->GetMapIndex());//���̃}�b�v�̏���ۑ�����
							CEdit::GetStageManager()->LoadMapTxt(nMapNum);//���[�v�}�[�J�[�ɋL�^���ꂽ�}�b�v�ԍ���ǂݍ���
						}
					}
				}
				break;
			default:
				//���X�g�����ɐi�߂�
				pObj = pNext;
				continue;
				break;
			}
			//���X�g�����ɐi�߂�
			pObj = pNext;
		}
	}

}
//=====================================================================================================================================

//===================================================================
//����������
//===================================================================
void CMarker::DeleteNum()
{

}
//=====================================================================================================================================
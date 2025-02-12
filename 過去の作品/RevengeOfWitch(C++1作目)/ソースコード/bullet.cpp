//==============================================================================================
//
//�V���Q���F�{�X����������[boss.cpp]
//Author:ShinaTaiyo
//
//==============================================================================================

//=========================
//�C���N���[�h
//=========================
#include "bullet.h"
#include "manager.h"
#include "explosion.h"
#include "player.h"
#include "enemy.h"
#include "game.h"
#include "block.h"
#include "model.h"
#include "sound.h"
#include "calculation.h"
#include "collision.h"
#include "texture.h"
#include "particle.h"
#include "boss.h"
//=================================================

//=========================
//�ÓI�����o
//=========================
const char* CBullet::m_apBULLET_FILENAME[CBullet::BULLETTEX_MAX] =
{
	"data\\TEXTURE\\Bullet\\PlayerBullet00.png",
	"data\\TEXTURE\\Bullet\\EnemyBullet00.png",
	"data\\TEXTURE\\Bullet\\WaterBullet.png",
};
//========================================================

//=========================
//�R���X�g���N�^
//=========================
CBullet::CBullet() : m_bBlockColliion(false),m_type(BULLETTYPE_PLAYER)
{

}
//=================================================

//=========================
//�f�X�g���N�^
//=========================
CBullet::~CBullet()
{

}
//=================================================

//=========================
//����������
//=========================
HRESULT CBullet::Init()
{
	//===========================
	//�r���{�[�h������
	//===========================
	CBillboard::Init();

	//==================================================================================

	m_bUse = false;                           //�g�p���
	m_fGravity = 0.0f;                        //�d��
	m_MoveType = MOVETYPE00_NORMAL;           //�ړ����@
	m_nPower = 0;                             //�З�
								              
	m_fLength = 0.0f;                         //�����̕␳

	m_bUseDivision = false;                   //������g�p���邩�ǂ���
	m_nNumDivision = 0;                       //������
	m_nLimitDivision = 0;                     //������

	m_bUseBlockCollision = false;             //�u���b�N�Ƃ̓����蔻����g�p���邩�ǂ���

	//==========================
	//MP�֘A
	//==========================
	m_nHitAddMp = 0;                          //�q�b�g��������MP�ϓ��l
	//==================================================================================

	//==========================
	//���ˊ֘A
	//==========================
	m_bUseReflection = false;                 //���˂��g�p���邩�ǂ���
	//==================================================================================

	if (m_type == BULLETTYPE_ENEMY)
	{
		CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_SHOT_002);
	}

	return S_OK;
}
//=================================================

//=========================
//�I������
//=========================
void CBullet::Uninit()
{
	CBillboard::Uninit();
	//SetDeath();//���S�t���O�ݒ�
}
//=================================================

//=========================
//�X�V����
//=========================
void CBullet::Update()
{
	//=======================
	//�ϐ��錾
	//=======================
	D3DXVECTOR3 & Pos = CBillboard::GetPos();                  //�r���{�[�h�̈ʒu���擾����
	D3DXVECTOR3& PosOld = GetPosOld();                          //1f�O�̈ʒu���擾
	D3DXVECTOR3& Move = GetMove();                             //�r���{�[�h�̈ړ��ʂ��擾����
	float &fWidth = CBillboard::GetWidth();                    //�r���{�[�h�̉������擾����
	float &fHeight = CBillboard::GetHeight();                  //�r���{�[�h�̍������擾����
	bool bIsJumping = false;
	bool bIsLanding = false;
	bool bIsWalling = false;
	bool bIsLiftLanding = false;
	bool bSuccessCollisionX = false;
	bool bSuccessCollisionY = false;
	CObject* pSaveObj = nullptr;
	D3DXVECTOR3 ComparisonVtxMax = NULL_VECTOR3;             //����̍ő咸�_����p
	D3DXVECTOR3 ComparisonVtxMin = NULL_VECTOR3;             //����̍ŏ����_����p
	D3DXVECTOR3 ComparisonPos = NULL_VECTOR3;                //����̈ʒu�̑���p

	CModel* pModel = nullptr;                                //���f���ւ̃|�C���^
	CBlock* pBlock = nullptr;                                //�u���b�N�ւ̃|�C���^

	bool bCollision = false;                                 //�����蔻�肪�����������ǂ���      

	int& nLife = GetLife();                                  //�̗�
	//===================================================
	m_fLength += 10.0f;

	if (nLife <= 0)
	{//�̗͂��O�ɂȂ�����E�E�E
		m_bUse = false;
	}


	//========================================
	//�r���{�[�h�̍X�V����
	//========================================
	CBillboard::Update();
	//===========================================================================================

	//========================================
	//�e�̓����蔻��
	//========================================
	Collision();
	//===========================================================================================

	//=======================================
	//����
	//=======================================
	if (m_bUseReflection == true)
	{
		if (bSuccessCollisionX == true)
		{
			Move.x *= -1;
		}
		if (bSuccessCollisionY == true)
		{
			Move.y *= -1;
		}
	}
	//===========================================================================================

	//=======================================
	//�e�̃u���b�N�Ƃ̓����蔻��
	//=======================================
	if (m_bBlockColliion == true)
	{
		BlockCollisionProcess();
	}
	//===========================================================================================

	//=======================================
	//�g�p��Ԃ��I�t�ɂȂ�����E�E�E
	//=======================================
	if (m_bUse == false)
	{
		//Uninit();
		SetDeath();
	}
	//===========================================================================================


}
//=================================================

//=========================
//�`�揈��
//=========================
void CBullet::Draw()
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = CManager::GetRenderer()->GetDevice();

	//================================================
	//�`��̒���
	//================================================
	//���C�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//Z�̔�r���@�ύX�B�i���f���������Ă��B��Ȃ��Ȃ�j
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

	////Z�o�b�t�@�ɏ������܂Ȃ��i�d�Ȃ���Ɉ�a�����Ȃ��Ȃ�j
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	//�A���t�@�e�X�g��L��(�A���t�@�l���O���傫���ꍇ�ɕ`�悷��j
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	//=======================================================================================================================================
	
	CBillboard::Draw();

	//================================================
	//�`��̒��������ɖ߂�
	//================================================

	//���C�g��L���ɖ߂�
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	////Z�̔�r���@�ύX�B
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	////Z�o�b�t�@�ɏ�������
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	//�A���t�@�e�X�g�����ɖ߂�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	//=======================================================================================================================================
}
//=======================================================================================================================================

//===============================================================
//���S�t���O��ݒ�
//===============================================================
void CBullet::SetDeath()
{
	CObject::SetDeath();
}
//=======================================================================================================================================

//=====================================================
//�o���b�g��������
//=====================================================
CBullet* CBullet::Create(BULLETTYPE type, BULLETTEX tex, int nLife, int nPower, float fWidth, float fHeight, D3DXVECTOR3 pos, D3DXVECTOR3 move)
{
	CBullet* pBullet = DBG_NEW CBullet;                             //�e�𐶐�
	CTexture* pTextureClass = CManager::GetTexture();           //�e�N�X�`���N���X���擾
	bool bSuccess = pBullet->CObject::GetCreateSuccess();       //�����������������ǂ������擾����
	if (bSuccess == true)
	{//����������������
		if (pBullet != nullptr)
		{
			pBullet->m_type = type;                           //�e�̎��
			pBullet->Init();                                  //����������
			pBullet->SetUseDeath(true);                 //���S�t���O�𔭓����邩�ǂ�����ݒ肷��
			pBullet->m_MoveType = MOVETYPE00_NORMAL;          //���ʂ̈ړ����@
			pBullet->SetLife(nLife);                          //�̗�
			pBullet->SetMaxLife(nLife);                       //�ő�̗�
			pBullet->SetMove(move);                           //�ړ��ʂ̐ݒ�
			pBullet->m_bUse = true;                           //�g�p���
			pBullet->m_nPower = nPower;                       //�З�
			pBullet->SetTextureIndex(pTextureClass->Regist(m_apBULLET_FILENAME[tex]));

			pBullet->CBillboard::bindTexture(pTextureClass->GetAddress(pBullet->GetTextureIndex()));     //�e�N�X�`�����Z�b�g����@
			pBullet->CBillboard::SetPos(pos);                                                            //�I�u�W�F�N�g�Q�c�̈ʒu��ݒ�
			pBullet->SetSupportPos(pos);                                                                 //�����ʒu��ݒ�
			pBullet->CBillboard::SetSize(fWidth, fHeight);                                               //�|���S���̃T�C�Y��ݒ�
			pBullet->CBillboard::SetFormarSize(fWidth, fHeight);                                         //�|���S���̃T�C�Y��ݒ�
			pBullet->CObject::SetType(CObject::TYPE_BULLET);                                             //�I�u�W�F�N�g�̎�ނ����߂�
			pBullet->SetAnimInfo(1,1,NORMAL_COL,false);                                                  //�A�j���[�V��������ݒ�

		}
	}
	else
	{//�I�u�W�F�N�g�ɋ󂫂��Ȃ������̂Ŕj������
		delete pBullet;
		pBullet = nullptr;
		return nullptr;
	}
	return pBullet;
}
//==================================================================================================================================================

//=====================================================
//�����蔻������
//=====================================================
void CBullet::Collision()
{
	//=================================
	//�r���{�[�h��������擾
	//==================================
	D3DXVECTOR3 Pos = NULL_VECTOR3;                    //�I�u�W�F�N�g�̈ʒu���擾)
	D3DXVECTOR3 Size = NULL_VECTOR3;                           //�I�u�W�F�N�g�̃T�C�Y
	Size.x = GetWidth();                                       //�������擾
	Size.y = GetHeight();                                       //�������擾

	if (GetUseMtxChild() == true)
	{//�q�}�g���b�N�X���g���Ă���Ȃ�
		Pos = GetMtxTransPos();
	}
	else
	{
		Pos = GetPos();
	}
	D3DXVECTOR3 ComparisonPos = NULL_VECTOR3;                  //�v���C���[�̈ʒu����p
	D3DXVECTOR3 ComparisonVtxMax = NULL_VECTOR3;               //����Ώۂ̍ő咸�_
	D3DXVECTOR3 ComparisonVtxMin = NULL_VECTOR3;               //����Ώۂ̍ŏ����_
	CObjectX* pObjX = nullptr;                                 //�I�u�W�F�N�gX
	CObject* pObj = nullptr;                                   //�I�u�W�F�N�g�i�[�p
	bool bCollision = false;                                   //�����蔻������邩�ǂ���
	//===========================================================

	//==================================
	//�e�̓����蔻��
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

			if (type == CObject::TYPE_ENEMY || type == CObject::TYPE_PLAYER || type == CObject::TYPE_BOSS || type == CObject::TYPE_BLOCK)
			{
				pObjX = (CObjectX*)pObj;

				ComparisonPos = pObjX->GetPos();
				ComparisonVtxMax = pObjX->GetVtxMax();
				ComparisonVtxMin = pObjX->GetVtxMin();
			}
			else
			{
				//���X�g�����ɐi�߂�
				pObj = pNext;
				continue;
			}

			float fLength = CCalculation::CalculationLength(pObjX->GetPos(), Pos);

			bCollision = false;
			if (fLength < Size.x * 3.0f)
			{
				bCollision = CCollision::CollisionSquare(Pos, D3DXVECTOR3(Size.x, Size.y, Size.x) * 0.5f, D3DXVECTOR3(-Size.x, -Size.y, -Size.x) * 0.5f,
					ComparisonPos, ComparisonVtxMax, ComparisonVtxMin);
			}
			if (bCollision == true)
			{//�e���v���C���[�ɓ���������
				switch (m_type)
				{
				case BULLETTYPE_ENEMY:
					if (CScene::GetMode() == CScene::MODE_GAME && type == CObject::TYPE::TYPE_PLAYER)
					{
						if (CGame::GetPlayer()->GetDodgeState() == false)
						{
							HitAddMpProcess();
							CGame::GetPlayer()->SetDamage(m_nPower, 1);
							m_bUse = false;//�g�p��Ԃ��I�t�ɂ���
						}
					}
					break;
				case BULLETTYPE_PLAYER:
					if (CScene::GetMode() == CScene::MODE_GAME)
					{
						if (type == CObject::TYPE::TYPE_ENEMY)
						{//�G�փ_���[�W��^����
							pObjX->SetDamage(m_nPower, 0);
							HitAddMpProcess();
							if (m_bUseDivision == true)
							{
								int nRadius = (int)(pObjX->GetSize().x);
								DivisionBulletProcess(nRadius);
							}
							m_bUse = false;//�g�p��Ԃ��I�t�ɂ���
						}
						else if (type == CObject::TYPE::TYPE_BOSS)
						{//�{�X�փ_���[�W��^����
							pObjX->SetDamage(m_nPower, 0);
							HitAddMpProcess();
							if (m_bUseDivision == true)
							{
								int nRadius = (int)(pObjX->GetSize().x);

								DivisionBulletProcess(nRadius);
							}
							m_bUse = false;//�g�p��Ԃ��I�t�ɂ���
						}
					}
					break;
				default:
					break;
				}
			}
			//���X�g�����ɐi�߂�
			pObj = pNext;
		}
	}

}
//================================================================================================================

//==============================================================
//MP�����蓖�Ă鏈��
//==============================================================
void CBullet::HitAddMpProcess()
{
	if (m_nHitAddMp != 0)
	{//�l���O�ȊO�Ȃ�
		if (CGame::GetPlayer() != nullptr)
		{
			CGame::GetPlayer()->SetMp(m_nHitAddMp);//MP��ϓ�������
		}
	}
}
//================================================================================================================

//==============================================================
//�����e�̏���
//==============================================================
void CBullet::DivisionBulletProcess(int nRadius)
{
	float fRandRot = 0.0f;
	CBullet* pBullet = nullptr;
	D3DXVECTOR3 Pos = GetPos();
	float fWidth = GetWidth();
	float fHeight = GetHeight();
	D3DXVECTOR3 RandPos = NULL_VECTOR3;
	fRandRot = float(rand() % 628) / 100;

	for (int nCnt = 0; nCnt < m_nNumDivision; nCnt++)
	{
		fRandRot = float(rand() % 628) / 100;
		RandPos.x = float(rand() % (nRadius * 8) - (nRadius * 4));
		RandPos.y = float(rand() % (nRadius * 8) - (nRadius * 4));
		pBullet = CBullet::Create(m_type, CBullet::BULLETTEX_PLAYER, 120,5,10.0f,10.0f, Pos + RandPos, D3DXVECTOR3(sinf(fRandRot) * 2.5f, cosf(fRandRot) * 2.5f, 0.0f));
		pBullet->SetUseAddSpeed(true, false, 0.01f);
		pBullet->SetHitAddMp(1);
		if (m_nLimitDivision > 0)
		{
			pBullet->SetUseDivision(true, 3, m_nLimitDivision - 1);
		}
	}
}
//================================================================================================================

//==============================================================
//�u���b�N�Ƃ̓����蔻��
//==============================================================
void CBullet::BlockCollisionProcess()
{
	CObject* pObj = nullptr;
	CBlock* pBlock = nullptr;
	bool bCollision = false;
	D3DXVECTOR3 Pos = GetPos();
	D3DXVECTOR3 VtxMax = D3DXVECTOR3(GetWidth() * 0.5f, GetHeight() * 0.5f, GetWidth() * 0.5f);
	D3DXVECTOR3 VtxMin = D3DXVECTOR3(-GetWidth() * 0.5f, -GetHeight() * 0.5f, -GetWidth() * 0.5f);
	//==================================
    //�g���b�v�̓����蔻��
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

			if (type == CObject::TYPE_BLOCK)
			{
				pBlock = (CBlock*)pObj;

				bCollision = CCollision::CollisionSquare(Pos, VtxMax, VtxMin, pBlock->GetPos(), pBlock->GetVtxMax(), pBlock->GetVtxMin());

				if (bCollision == true)
				{
					SetDeath();
				}
			}
			//���X�g�����ɐi�߂�
			pObj = pNext;
		}
	}
}
//================================================================================================================
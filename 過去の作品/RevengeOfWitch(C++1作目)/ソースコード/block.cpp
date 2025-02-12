//==========================================================================================================
//
//�V���P�Q���F�S�M�~�b�N����������[block.cpp]
//Author:ShinaTaiyo
//
//==========================================================================================================

//=========================
//�C���N���[�h
//=========================
#include "block.h"
#include "manager.h"
#include "explosion.h"
#include "enemy.h"
#include "effect.h"
#include "objectXInfo.h"
#include "calculation.h"
#include "stagemanager.h"
#include "player.h"
#include "game.h"
#include "edit.h"
#include "particle.h"
#include "input.h"
#include "debugtext.h"
#include <stdio.h>
#include <string.h>
//=================================================

//=========================
//�ÓI�����o
//=========================
int CBlock::m_nNumFile = CBlock::BLOCKTYPE_MAX;                                              //�t�@�C�������i�[����
const int CBlock::m_BLOCKSUBTYPENUM[CBlock::BLOCKTYPE_MAX] = { 1//0
,1//1
,1//2
,1//3
,3//4 ���t�g�u���b�N
,1//5
,1//6
,1//7
,1//8
,1//9
,1//10
,1//11
,2//12 ���u���b�N
,1//13
,1//14
,1//15
,1//16
,1};//17     //�u���b�N�̃T�u�^�C�v
const float CBlock::m_fBLOCKCORRECTIONCOLLISION = 3.0f;      //1,2,3,4,5,6,7,8,9,10,11,12,13                            //�u���b�N�ɓ����蔻��̗]�T�����������
const char* CBlock::m_BLOCK_FILENAME[CBlock::BLOCKTYPE_MAX] =
{ "data\\MODEL\\Block\\Block00_Normal.x", 
  "data\\MODEL\\Block\\Block01_Water.x",
 "data\\MODEL\\Block\\Block02_Grass.x" ,
 "data\\MODEL\\Block\\Block03_LongRenga.x",
"data\\MODEL\\Block\\Block04_Rift.x",
"data\\MODEL\\Block\\Block05_Fire.x",                                                    
"data\\MODEL\\Block\\Block06_Jump.x",                                                    
"data\\MODEL\\Block\\Block07_Crack.x",                                                      
"data\\MODEL\\Block\\Block08_SukeSuke.x",                                                   
"data\\MODEL\\Block\\Block09_BlinkingRed.x",                                                
"data\\MODEL\\Block\\Block10_BlinkingBrue.x",
"data\\MODEL\\Block\\Block11_Ice.x",                                                    
"data\\MODEL\\Block\\Block12_Arrow.x",                                                  
"data\\MODEL\\Block\\Block13_Swamp.x",
"data\\MODEL\\Block\\Block14_Tornado.x",                                                
"data\\MODEL\\Block\\Block15_Fall.x",
"data\\MODEL\\Block\\Block16_Save.x",
"data\\MODEL\\Block\\Block17_ChainBlock.x",
};                                                  //�u���b�N��X�t�@�C���ւ̃|�C���^
//========================================================

//=========================
//�R���X�g���N�^
//=========================
CBlock::CBlock() : CObjectX(2),m_FallBlock()
{

}
//=================================================

//=========================
//�f�X�g���N�^
//=========================
CBlock::~CBlock()
{

}
//=================================================

//=========================
//����������
//=========================
HRESULT CBlock::Init()
{
	//===========================
	//X�I�u�W�F�N�g������
	//===========================
	CObjectX::Init();
	//=========================================
	m_bMove = false;               //�ړ����邩�ǂ���
	m_bUse = false;                //�g�p���
	m_bTurn = false;               //�^�[���t���O
	m_type = BLOCKTYPE00_NORMAL;   //�u���b�N�̎��
	m_fRot = 0.0f;                 //�����̕␳�l
	m_nCntTime = 0;                //��������Ă���̎��Ԃ��J�E���g����
	m_nSubType = 0;                //�T�u�^�C�v
	m_bCollision = true;           //�����蔻������邩�ǂ���
	SetInertia(0.0f);              //������ݒ�
	m_fMoveLength = 200.0f;        //��������
	m_fRotSpeed = 0.1f;            //��]���x
	m_fSpeed = 5.0f;               //���x
	m_fStartRot = 0.0f;            //�J�n����
	//===========================
	//�_�Ńu���b�N�̏��
	//===========================
	m_BlinkingBlock.bBlinking = true;//�_�ł��Ă��邩�ǂ���
	m_BlinkingBlock.nBlinkingSpeed = 50;//�_�ő��x
	//==================================================================

	//===========================
	//���u���b�N�̏��
	//===========================
	m_ArrowBlock.fAddSpeed = 2.0f;//�����x
	//==================================================================
	
	//===========================
	//�W�����v�u���b�N�̏��
	//===========================
	m_JumpBlock.fJumpPower = 25.0f;
	//==================================================================

	//===========================
	//�t�H�[���u���b�N�̏��
	//===========================
	m_FallBlock.bFall = false;
	m_FallBlock.nReraiseCnt = 100;
	m_FallBlock.nRespawnCnt = m_nMAX_FALLBLOCKRESPAWNCNT;//�u���b�N���������Ƃ��ɕ�������܂ł̃J�E���g
    //==================================================================

	//===========================
	//�u���b�N���蕪���p
	//===========================
	m_bNearBlockX = false;
	m_bNearBlockY = false;
	//==================================================================

	return S_OK;
}
//=================================================

//=========================
//�I������
//=========================
void CBlock::Uninit()
{
	CObjectX::Uninit();//X�I�u�W�F�N�g�I������
	//SetDeath();//���S�t���O�ݒ�
}
//=================================================

//=========================
//�X�V����
//=========================
void CBlock::Update()
{

	//=======================
	//�ϐ��錾
	//=======================
	D3DXVECTOR3 &Pos = CObjectX::GetPos();
	D3DXVECTOR3 &PosOld = CObjectX::GetPosOld();
	D3DXVECTOR3& Rot = GetRot();
	D3DXVECTOR3& Move = GetMove();
	float fEnemyWidth = 0.0f;//�G�̉�������p
	float fEnemyHeight = 0.0f;//�G�̍�������p
	int nCntRideFrame = 0;    
	int& nLife = GetLife();//�̗�

	bool bJustTurn = false;
	//===================================================
	
	m_nCntTime++;//��������Ă���̎��Ԃ��J�E���g����

	GravityProcess();

	switch (m_type)
	{
	case BLOCKTYPE04_RIFT:
		LiftBlockPattern();
		break;
	case BLOCKTYPE09_BLINKINGRED:
		BlinkingBlockPattern();
		break;
	case BLOCKTYPE10_BLINKINGBRUE:
		BlinkingBlockPattern();
		break;
	case BLOCKTYPE12_ARROW:
		ArrowBlockPattern();
		break;
	case BLOCKTYPE15_FALL:
		FallBlockProcess();
		break;
	case BLOCKTYPE16_SAVE:
		Rot.y += 0.03f;
		break;
	default:
		break;
	}

	//========================================
    //X�I�u�W�F�N�g�̍X�V����
    //========================================
	CObjectX::Update();
	//===========================================================================================

	Collision();//�ڐG������s��

	if (GetStageManagerChoose() == true)
	{

	}

}
//=================================================

//===================================================
//�`�揈��
//===================================================
void CBlock::Draw()
{
	CObjectX::Draw();
}
//===========================================================================================

//===================================================
//���S�t���O��ݒ�
//===================================================
void CBlock::SetDeath()
{
	CObject::SetDeath();
}
//===========================================================================================

//==================================
//�u���b�N��������
//==================================
CBlock* CBlock::Create(BLOCKTYPE type, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, CObjectX::ROTTYPE RotType, int nSubType)
{
	CBlock* pBlock = DBG_NEW CBlock;                                                               //�u���b�N�𐶐�
	bool bSuccess = pBlock->CObject::GetCreateSuccess();
	LPDIRECT3DTEXTURE9 pResultTexture[m_nMAXOBJECT] = {};                                        //�ݒ肳�ꂽ��ނ̔z��̃e�N�X�`������p
	int nIdx = 0;//���f�����̃C���f�b�N�X
	if (bSuccess == true)
	{
		if (pBlock != nullptr)
		{
			pBlock->Init();                                                                                                    //����������
			switch (type)
			{
			case CBlock::BLOCKTYPE04_RIFT:
				pBlock->SetMoveType(MOVETYPE_TRUE);
				pBlock->m_bMove = true;
				break;
			default:
				break;
			}
			pBlock->SetUseGravity(false);
			pBlock->SetUseDeath(true);                                                                                         //���S�t���O�𔭓����邩�ǂ�����ݒ肷��
			pBlock->m_type = type;                                                                                             //�u���b�N�̎��
			pBlock->SetUseBlockCollision(false);
			pBlock->SetLife(nLife);                                                                                            //�u���b�N�̗̑�
			pBlock->SetMaxLife(nLife);                                                                                            //�u���b�N�̗̑�
			pBlock->SetAutoSubLife(false);              //�����I�ɑ̗͂����炷���ǂ���
			pBlock->m_bUse = true;                                                                                             //�g�p���
			pBlock->m_nSubType = nSubType;                                                                                     //�T�u�^�C�v��ݒ�
			pBlock->SetSupportPos(pos);                                                                                        //�ݒu�ʒu
			pBlock->CObjectX::SetPos(pos);                                                                                     //�I�u�W�F�N�gX�̈ʒu��ݒ�
			pBlock->SetPosOld(pos);                                                                                            //1f�O�̈ʒu��ݒ�
			pBlock->CObject::SetType(CObject::TYPE_BLOCK);                                                                     //�I�u�W�F�N�g�̎�ނ����߂�
			pBlock->SetScale(scale);                                                                                 //�I�u�W�F�N�g�̊g�嗦
			pBlock->SetFormarScale(scale);                                                                                     //���̊g�嗦��ݒ肷��
			nIdx = CManager::GetObjectXInfo()->Regist(CObjectX::OBJECTXTYPE_BLOCK, m_BLOCK_FILENAME[(int)(type)]);
			pBlock->CObjectX::BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(CObjectX::OBJECTXTYPE_BLOCK,nIdx),
				CManager::GetObjectXInfo()->GetBuffMat(CObjectX::OBJECTXTYPE_BLOCK,nIdx),
				CManager::GetObjectXInfo()->GetdwNumMat(CObjectX::OBJECTXTYPE_BLOCK,nIdx),
				CManager::GetObjectXInfo()->GetTexture(CObjectX::OBJECTXTYPE_BLOCK,nIdx),
				CManager::GetObjectXInfo()->GetColorValue(CObjectX::OBJECTXTYPE_BLOCK,nIdx));                          //���f���������蓖�Ă�

			pBlock->CObjectX::SetObjXType(CObjectX::OBJECTXTYPE_BLOCK);                                                        //�I�u�W�F�N�gX�̃^�C�v��ݒ�
			pBlock->CObjectX::SetTypeNum((int)(type));                                                                         //�I�u�W�F�N�gX���Ƃ̃^�C�v�ԍ���ݒ�
			pBlock->SetSize();                                                                                                 //X�I�u�W�F�N�g�̃T�C�Y��ݒ肷��
			pBlock->CObjectX::SetRotType(RotType);                                                                             //�����̎�ނ�ݒ�
			pBlock->SetManagerType((int)(CStageManager::MANAGEROBJECT_BLOCK));        //�}�l�[�W���[�ŌĂяo�����̎�ނ�ݒ�

		}
	}
	else
	{//�I�u�W�F�N�g�ɋ󂫂��Ȃ������̂Ŕj������
		delete pBlock;
		pBlock = nullptr;
		return nullptr;
	}
	return pBlock;
}
//================================================================================================

//===============================================================
//�u���b�N����������
//===============================================================
void CBlock::ReleaseBlock()
{
	Release();
}
//=========================================================================================================================

//===============================================================
//�u���b�N�̃^�C�v���擾����
//===============================================================
CBlock::BLOCKTYPE CBlock::GetType()
{
	return m_type;
}
//=========================================================================================================================

//===============================================================
//�u���b�N�Ƃ̓����蔻����s��
//===============================================================
void CBlock::Collision()
{
	//==================================
	//�I�u�W�F�N�gX��������擾
	//==================================
	D3DXVECTOR3 ComparisonPos = NULL_VECTOR3;                  //����Ώۂ̈ʒu���擾
	D3DXVECTOR3 ComparisonPosOld = NULL_VECTOR3;               //����Ώۂ�1f�O�̈ʒu���擾
	D3DXVECTOR3 ComparisonVtxMax = NULL_VECTOR3;               //����Ώۂ̍ő咸�_���擾
	D3DXVECTOR3 ComparisonVtxMin = NULL_VECTOR3;               //����Ώۂ̍ŏ����_���擾
	D3DXVECTOR3 MySize = NULL_VECTOR3;
	D3DXVECTOR3 BlockPos = GetPos();                          //�u���b�N�̈ʒu���擾
	D3DXVECTOR3 BlockVtxMax = GetVtxMax();                    //�u���b�N�̍ő咸�_���擾
	D3DXVECTOR3 BlockVtxMin = GetVtxMin();                    //�u���b�N�̍ŏ����_���擾
	//===========================================================

	//�I�u�W�F�N�g�擾
	CObject* pObj = nullptr;
	//==================================
	//�u���b�N�Ƃ̓����蔻��
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
			if (type == CObject::TYPE_PLAYER || type == CObject::TYPE_ENEMY || type == CObject::TYPE_BOSS)
			{
				ComparisonPos.x = (float)(floor(((CObjectX*)pObj)->GetPos().x));                   //�ʒu���擾
				ComparisonPos.y = (float)(floor(((CObjectX*)pObj)->GetPos().y));                   //�ʒu���擾
				ComparisonPos.z = (float)(floor(((CObjectX*)pObj)->GetPos().z));                   //�ʒu���擾]

				ComparisonVtxMax.x = (float)(floor(((CObjectX*)pObj)->GetVtxMax().x));             //�ő咸�_���擾
				ComparisonVtxMax.y = (float)(floor(((CObjectX*)pObj)->GetVtxMax().y));             //�ő咸�_���擾
				ComparisonVtxMax.z = (float)(floor(((CObjectX*)pObj)->GetVtxMax().z));             //�ő咸�_���擾

				ComparisonVtxMin.x = (float)(floor(((CObjectX*)pObj)->GetVtxMin().x));             //�ŏ����_���擾
				ComparisonVtxMin.y = (float)(floor(((CObjectX*)pObj)->GetVtxMin().y));             //�ŏ����_���擾
				ComparisonVtxMin.z = (float)(floor(((CObjectX*)pObj)->GetVtxMin().z));             //�ŏ����_���擾

				ComparisonPosOld.x = (float)(floor(((CObjectX*)pObj)->GetPosOld().x));             //1f�O�̈ʒu���擾
				ComparisonPosOld.y = (float)(floor(((CObjectX*)pObj)->GetPosOld().y));             //1f�O�̈ʒu���擾
				ComparisonPosOld.z = (float)(floor(((CObjectX*)pObj)->GetPosOld().z));             //1f�O�̈ʒu���擾

				MySize.x = (float)(floor(((CObjectX*)pObj)->GetSize().x));
				MySize.y = (float)(floor(((CObjectX*)pObj)->GetSize().y));
				MySize.z = (float)(floor(((CObjectX*)pObj)->GetSize().z));
				bool& bIsComparisonLanding = ((CObjectX*)pObj)->GetLanding();                         //����Ώۂ��u���b�N�ɏ���Ă��邩�ǂ���
				D3DXVECTOR3& ComparisonMove = ((CObjectX*)pObj)->GetMove();                           //�ړ��ʂ��擾
				//((CObjectX*)pObj)->SetLiftLanding(false);                     //���t�g�ɏ���Ă��Ȃ���Ԃɏ�����
				//��
				if (ComparisonPos.x + ComparisonVtxMax.x > BlockPos.x + BlockVtxMin.x
					&& ComparisonPos.x + ComparisonVtxMin.x < BlockPos.x + BlockVtxMax.x
					&& ComparisonPos.y + ComparisonVtxMin.y - m_fBLOCKCORRECTIONCOLLISION < BlockPos.y + BlockVtxMax.y
					&& ComparisonPosOld.y + ComparisonVtxMin.y + m_fBLOCKCORRECTIONCOLLISION >= BlockPos.y + BlockVtxMax.y
					&& ComparisonPos.z + ComparisonVtxMax.z > BlockPos.z + BlockVtxMin.z
					&& ComparisonPos.z + ComparisonVtxMin.z < BlockPos.z + BlockVtxMax.z)
				{//�Ώۂ̉��[�����f����+Y�ɓ����������̏���
					switch (GetType())
					{
					case BLOCKTYPE06_JUMP:
						ComparisonMove.y = m_JumpBlock.fJumpPower;
						bIsComparisonLanding = false;//����ĂȂ���Ԃɂ���
						break;
					case BLOCKTYPE11_ICE:
						if (type == CObject::TYPE_PLAYER)
						{
							((CPlayer*)pObj)->SetPlayerSlideInertia(0.05f);
						}
						break;
					case BLOCKTYPE12_ARROW:
						if (m_nSubType == 0)
						{
							ComparisonMove.x += m_ArrowBlock.fAddSpeed;
							if (type == CObject::TYPE_PLAYER)
							{
								if (((CPlayer*)pObj)->GetMove().x > 5.0f)
								{
									((CPlayer*)pObj)->SetPlayerSlideInertia(0.2f);
								}
							}
						}
						else if (m_nSubType == 1)
						{
							ComparisonMove.x += -m_ArrowBlock.fAddSpeed;
							if (type == CObject::TYPE_PLAYER)
							{
								if (((CPlayer*)pObj)->GetMove().x < -5.0f)
								{
									((CPlayer*)pObj)->SetPlayerSlideInertia(0.2f);
								}
							}
						}
						break;
					case BLOCKTYPE13_SWAMP:
						if (type == CObject::TYPE_PLAYER)
						{
							((CPlayer*)pObj)->SetDecelerationState(true);
						}
						break;
					default:
						break;
					}

				}
				if (ComparisonPos.x + ComparisonVtxMin.x - m_fBLOCKCORRECTIONCOLLISION <= BlockPos.x + BlockVtxMax.x &&
					ComparisonPos.x + ComparisonVtxMax.x + m_fBLOCKCORRECTIONCOLLISION >= BlockPos.x + BlockVtxMin.x &&
					ComparisonPos.y + ComparisonVtxMin.y <= BlockPos.y + BlockVtxMax.y &&
					ComparisonPos.y + ComparisonVtxMax.y >= BlockPos.y + BlockVtxMin.y &&
					ComparisonPos.z + ComparisonVtxMin.z - m_fBLOCKCORRECTIONCOLLISION <= BlockPos.z + BlockVtxMax.z &&
					ComparisonPos.z + ComparisonVtxMax.z + m_fBLOCKCORRECTIONCOLLISION >= BlockPos.z + BlockVtxMin.z)
				{//�u���b�N�ɓ���������
					switch (GetType())
					{
					case BLOCKTYPE05_FIRE:
						if (type == CObject::TYPE_PLAYER)
						{//�v���C���[�̂݌��ʂ𔭓�
							if (CScene::GetMode() == CScene::MODE_GAME)
							{
								CGame::GetPlayer()->SetDamage(1,1);
							}
							else if (CScene::GetMode() == CScene::MODE_EDIT)
							{
								CEdit::GetPlayer()->SetDamage(1,1);
							}
						}
						break;
					case BLOCKTYPE14_TORNADO:
						if (type == CObject::TYPE_PLAYER)
						{//�ǃW�����v��s�\�ɂ���
							((CPlayer*)pObj)->SetWallJumpBindTime(3);
						}
						break;
					case BLOCKTYPE16_SAVE:
						if (type == CObject::TYPE_PLAYER)
						{

						}
						break;
					default:
						break;

					}
				}
			}
			//���X�g�����ɐi�߂�
			pObj = pNext;
		}
	}

}
//=========================================================================================================================

//===============================================================
//�u���b�N�Ƃ̓����蔻����s��
//===============================================================
void CBlock::CollisionSquare(CObjectX* pObjX)
{
	D3DXVECTOR3 ComparisonPos = NULL_VECTOR3;        //��r�p�ʒu
	D3DXVECTOR3 ComparisonPosOld = NULL_VECTOR3;     //��r�p1f�O�̈ʒu
	D3DXVECTOR3 ComparisonVtxMax = NULL_VECTOR3;     //��r�p�ō����_
	D3DXVECTOR3 ComparisonVtxMin = NULL_VECTOR3;     //��r�p�Œᒸ�_
	D3DXVECTOR3 ComparisonSize = NULL_VECTOR3;       //��r�T�C�Y
	D3DXVECTOR3 ComparisonMove = NULL_VECTOR3;       //��r�ړ���


	MOVETYPE MoveType = MOVETYPE_NONE;
	//==========================================================================================================
	bool bDodgeState = pObjX->GetDodgeState();       //������Ă��邩�ǂ���
	bool& bIsLanding = pObjX->GetLanding();
	bool bIsLandingOld = pObjX->GetLanding();//1f�O�̒��n���
	bIsLanding = false;
	//bIsLanding = false;
	bool& bIsLiftLanding = pObjX->GetLiftLanding();  //���t�g�̏�ɏ���Ă��邩�ǂ���
	bIsLiftLanding = false;

	bool& bSuccessBlockCollisionY = pObjX->GetSuccessBlockCollisionY();//�u���b�N�Ƃ�X�����̓����蔻�肪�����������ǂ���
	bSuccessBlockCollisionY = false;//���̎��_�ł͓����������ǂ���������Ȃ��̂�false

	bool& bSuccessCollisionX = pObjX->GetSuccessBlockCollisionX();
	bSuccessCollisionX = false;

	bool& bIsWalling = pObjX->GetWalling();          //�ǂɂ������Ă��邩�ǂ���
	bool bIsWallingOld = pObjX->GetWalling();        //1f�O�̕ǂ��������
	bIsWalling = false;

	CObject*& pLandingObj = pObjX->GetLandingObj();
	pLandingObj = nullptr;

	bool bCollision = true;//�����蔻������邩�ǂ���

	D3DXVECTOR3 Scale = NULL_VECTOR3;

	//====================================================================
	//����J�n
	//====================================================================

	//NearBlockSurchProcess(pObjX->GetPos(),pObjX->GetSize());//�S�Ẵu���b�N���߂��̃u���b�N���T�[�`����
	D3DXVECTOR3& ObjXPos = pObjX->GetPos();
	for (int nCntPriority = 0; nCntPriority < m_nMAXPRIORITY; nCntPriority++)
	{
		CObject * pObj = GetTopObject(nCntPriority);//�g�b�v�I�u�W�F�N�g���擾
		while (pObj != nullptr)
		{
			//���̃I�u�W�F�N�g���i�[
			CObject* pNext = pObj->GetNextObject();

			//��ނ̎擾�i�G�Ȃ瓖���蔻��j
			CObject::TYPE type = pObj->GetType();
			if (type == CObject::TYPE_BLOCK)
			{
				CBlock* pBlock = (CBlock*)pObj;

				float fLength = CCalculation::CalculationLength(pObjX->GetPos(), pBlock->GetPos());
				if (pBlock->m_type == BLOCKTYPE08_SUKESUKE && bDodgeState == true)
				{//�X�P�X�P�u���b�N�������ԂȂ�
				//���X�g�����ɐi�߂�
					pObj = pNext;
					continue;
				}
				else
				{
					Scale = pBlock->GetScale();
					if (Scale.x >= Scale.y - 0.1f && Scale.x <= Scale.y + 0.1f)
					{//�g�嗦���قƂ�Ǔ����Ȃ�
						if (fLength > pBlock->GetSize().x * 3.0f)
						{
							bCollision = false;
						}
						else
						{
							bCollision = true;
						}
					}
					else
					{
						bCollision = true;
					}

					if (pBlock->m_bCollision == true && bCollision == true)
					{
						//if (pBlock->m_bNearBlockX = true && pBlock->m_bNearBlockY == true)
						//{
							if (bIsLandingOld == true)
							{
								//�㉺
								pBlock->ExtrusionCollisionY(pObjX, pBlock);

								//���E
								if (pBlock->m_bMove == false)
								{
									pBlock->ExtrusionCollisionX(pObjX, pBlock);
								}
							}
							else
							{
								//���E
								if (pBlock->m_bMove == false)
								{
									pBlock->ExtrusionCollisionX(pObjX, pBlock);
								}

								//�㉺
								pBlock->ExtrusionCollisionY(pObjX, pBlock);
							}
						//}
						//else
						//{
						//	if (pBlock->m_bNearBlockY == true)
						//	{
						//		//���E
						//		if (pBlock->m_bMove == false)
						//		{
						//			pBlock->ExtrusionCollisionX(pObjX, pBlock);
						//		}

						//		//�㉺
						//		pBlock->ExtrusionCollisionY(pObjX, pBlock);
						//	}
						//	else
						//	{
						//		//�㉺
						//		pBlock->ExtrusionCollisionY(pObjX, pBlock);

						//		//���E
						//		if (pBlock->m_bMove == false)
						//		{
						//			pBlock->ExtrusionCollisionX(pObjX, pBlock);
						//		}


						//	}
						//}
					}
				}
			}
			else
			{
				//���X�g�����ɐi�߂�
				pObj = pNext;
				continue;
			}

			//���X�g�����ɐi�߂�
			pObj = pNext;
		}
	}
}
//=========================================================================================================================

//===============================================================
//�w�艺�u���b�N�̏�ɏ���Ă���ꍇ�A�ʒu��␳����
//===============================================================
void CBlock::LandingCorrection(D3DXVECTOR3& Pos, CObject* pSaveObj, D3DXVECTOR3 VtxMax, D3DXVECTOR3 VtxMin)
{
	CObject* pObj = nullptr;
	CObject::TYPE type = (CObject::TYPE)(0);
	D3DXVECTOR3 ComparisonPos = NULL_VECTOR3;        //��r�p�ʒu
	D3DXVECTOR3 ComparisonPosOld = NULL_VECTOR3;     //��r�p1f�O�̈ʒu
	D3DXVECTOR3 ComparisonVtxMax = NULL_VECTOR3;     //��r�p�ō����_
	D3DXVECTOR3 ComparisonVtxMin = NULL_VECTOR3;     //��r�p�Œᒸ�_
	D3DXVECTOR3 ComparisonSize = NULL_VECTOR3;       //�T�C�Y
	D3DXVECTOR3 ComparisonMove = NULL_VECTOR3;
	CBlock* pBlock = nullptr;

	D3DXVECTOR3 MyVtxMin = NULL_VECTOR3;
	MyVtxMin.x = (float)(floor(VtxMin.x));
	MyVtxMin.y = (float)(floor(VtxMin.y));
	MyVtxMin.z = (float)(floor(VtxMin.z));

	if (pSaveObj != nullptr)
	{
		//��ނ̎擾
		type = pSaveObj->GetType();
		if (type == CObject::TYPE_BLOCK)
		{
			CBlock* pBlock = (CBlock*)pSaveObj;

			ComparisonPos.x = (float)(floor(pBlock->GetPos().x));
			ComparisonPos.y = (float)(floor(pBlock->GetPos().y));
			ComparisonPos.z = (float)(floor(pBlock->GetPos().z));

			ComparisonPosOld.x = (float)(floor(pBlock->GetPosOld().x));
			ComparisonPosOld.y = (float)(floor(pBlock->GetPosOld().y));
			ComparisonPosOld.z = (float)(floor(pBlock->GetPosOld().z));

			ComparisonVtxMax.x = (float)(floor(pBlock->GetVtxMax().x));
			ComparisonVtxMax.y = (float)(floor(pBlock->GetVtxMax().y));
			ComparisonVtxMax.z = (float)(floor(pBlock->GetVtxMax().z));

			ComparisonVtxMin.x = (float)(floor(pBlock->GetVtxMin().x));
			ComparisonVtxMin.y = (float)(floor(pBlock->GetVtxMin().y));
			ComparisonVtxMin.z = (float)(floor(pBlock->GetVtxMin().z));

			ComparisonSize.x = (float)(floor(pBlock->GetSize().x));
			ComparisonSize.y = (float)(floor(pBlock->GetSize().y));
			ComparisonSize.z = (float)(floor(pBlock->GetSize().z));
			 

			float fPosY = fabsf(MyVtxMin.y);
			Pos.y = ComparisonPos.y + ComparisonVtxMax.y + fPosY;
		}
	}

}
//=========================================================================================================================

//===============================================================
//�u���b�N�̏���ݒ肷��
//===============================================================
void CBlock::SetBlockInfo(float fStartRot, float fRotSpeed, float fMoveLength, float fSpeed, int nBlinkSpeed, float fAddSpeed, float fJumpPower)
{
	m_fStartRot = fStartRot;                       //��]�J�n����
	m_fRotSpeed = fRotSpeed;                       //��]���x
	m_fMoveLength = fMoveLength;                   //�ړ�����
	m_fSpeed = fSpeed;                             //�ړ����x
	m_BlinkingBlock.nBlinkingSpeed = nBlinkSpeed;  //�_�ő��x
	m_ArrowBlock.fAddSpeed = fAddSpeed;            //�����x
	m_JumpBlock.fJumpPower = fJumpPower;           //�W�����v��
}
//=========================================================================================================================

//===============================================================
//�u���b�N�̏�ɏ���Ă��邩�ǂ����̔��������
//===============================================================
void CBlock::RidingTopCollision(bool bJustTurn)
{
	//==========================
	//�ϐ�
	//==========================

	D3DXVECTOR3 ComparisonPos = NULL_VECTOR3;        //��r�p�ʒu
	D3DXVECTOR3 ComparisonPosOld = NULL_VECTOR3;     //��r�p1f�O�̈ʒu
	D3DXVECTOR3 ComparisonVtxMax = NULL_VECTOR3;     //��r�p�ō����_
	D3DXVECTOR3 ComparisonVtxMin = NULL_VECTOR3;     //��r�p�Œᒸ�_
	D3DXVECTOR3 ComparisonSize = NULL_VECTOR3;       //��r�p�ő�T�C�Y
	//====================
	//�Ώ�
	//====================
	D3DXVECTOR3 Pos = NULL_VECTOR3;
	D3DXVECTOR3 VtxMin = NULL_VECTOR3;
	D3DXVECTOR3 VtxMax = NULL_VECTOR3;
	D3DXVECTOR3 PosOld = NULL_VECTOR3;
	D3DXVECTOR3 Size = NULL_VECTOR3;
	D3DXVECTOR3 Move = NULL_VECTOR3;
	//====================================================================

	//====================
	//�u���b�N
	//====================
	ComparisonPos.x = (float)(floor(GetPos().x));
	ComparisonPos.y = (float)(floor(GetPos().y));
	ComparisonPos.z = (float)(floor(GetPos().z));

	ComparisonPosOld.x = (float)(floor(GetPosOld().x));
	ComparisonPosOld.y = (float)(floor(GetPosOld().y));
	ComparisonPosOld.z = (float)(floor(GetPosOld().z));

	ComparisonVtxMax.x = (float)(floor(GetVtxMax().x));
	ComparisonVtxMax.y = (float)(floor(GetVtxMax().y));
	ComparisonVtxMax.z = (float)(floor(GetVtxMax().z));

	ComparisonVtxMin.x = (float)(floor(GetVtxMin().x));
	ComparisonVtxMin.y = (float)(floor(GetVtxMin().y));
	ComparisonVtxMin.z = (float)(floor(GetVtxMin().z));

	ComparisonSize.x = (float)(floor(GetSize().x));
	ComparisonSize.y = (float)(floor(GetSize().y));
	ComparisonSize.z = (float)(floor(GetSize().z));
	//=========================================================

	//for (int nCntPriority = 0; nCntPriority < m_nMAXPRIORITY; nCntPriority++)
	//{
	//	for (int nCntObj = 0; nCntObj < m_nMAXOBJECT; nCntObj++)
	//	{
	//		CObject* pObj = CObject::GetCObject(nCntObj, nCntPriority);

	//		if (pObj != nullptr)
	//		{
	//			if (pObj->GetType() == CObject::TYPE_PLAYER)
	//			{
	//				Pos = ((CPlayer*)pObj)->GetPos();
	//				PosOld = ((CPlayer*)pObj)->GetPosOld();
	//				Size.x = (float)(floor(((CPlayer*)pObj)->GetSize().x));
	//				Size.y = (float)(floor(((CPlayer*)pObj)->GetSize().y));
	//				Size.z = (float)(floor(((CPlayer*)pObj)->GetSize().z));
	//				VtxMax.x = (float)(floor(((CPlayer*)pObj)->GetVtxMax().x));
	//				VtxMax.y = (float)(floor(((CPlayer*)pObj)->GetVtxMax().y));
	//				VtxMax.z = (float)(floor(((CPlayer*)pObj)->GetVtxMax().z));
	//				VtxMin.x = (float)(floor(((CPlayer*)pObj)->GetVtxMin().x));
	//				VtxMin.y = (float)(floor(((CPlayer*)pObj)->GetVtxMin().y));
	//				VtxMin.z = (float)(floor(((CPlayer*)pObj)->GetVtxMin().z));
	//				Move.x = (float)(floor(((CPlayer*)pObj)->GetVtxMin().x));
	//				Move.y = (float)(floor(((CPlayer*)pObj)->GetVtxMin().y));
	//				Move.z = (float)(floor(((CPlayer*)pObj)->GetVtxMin().z));

	//				////��
	//				//if (Pos.x + Size.x / 2 > ComparisonPos.x + ComparisonVtxMin.x
	//				//	&& Pos.x - Size.x / 2 < ComparisonPos.x + ComparisonVtxMax.x
	//				//	&& Pos.y <= ComparisonPos.y + ComparisonVtxMax.y
	//				//	&& PosOld.y >= ComparisonPos.y + ComparisonVtxMax.y
	//				//	&& Pos.z + Size.z / 2 > ComparisonPos.z + ComparisonVtxMin.z
	//				//	&& Pos.z - Size.z / 2 < ComparisonPos.z + ComparisonVtxMax.z)
	//				//{//�Ώۂ̉��[�����f����+Y�ɓ����������̏���
	//				//	Pos.x += m_move.x;
	//				//	((CPlayer*)pObj)->SetPos(Pos);
	//				//}
	//				//��
	//				if (ComparisonPos.x + ComparisonSize.x / 2 > Pos.x + VtxMin.x
	//					&& ComparisonPosOld.x + ComparisonSize.x / 2 - m_move.x <= Pos.x + VtxMin.x
	//					&& ComparisonPos.y + ComparisonSize.y > Pos.y
	//					&& ComparisonPos.y < Pos.y + VtxMax.y
	//					&& ComparisonPos.z + ComparisonSize.z / 2 > Pos.z + VtxMin.z
	//					&& ComparisonPos.z - ComparisonSize.z / 2 < Pos.z + VtxMax.z)
	//				{//�Ώۂ��u���b�N��-X�ɓ����������̏����i�v���C���[�̈ʒu�����炷�j
	//			       //Pos.x = ComparisonPos.x + ComparisonSize.x / 2 + Size.x / 2 + 5.0f;
	//					Pos.x += m_move.x;
	//				   ((CPlayer*)pObj)->SetPos(Pos);
	//				}
	//				//�E
	//				else if (ComparisonPos.x - ComparisonSize.x / 2 < Pos.x + VtxMax.x
	//					&& ComparisonPosOld.x - ComparisonSize.x / 2 - m_move.x >= Pos.x + VtxMax.x
	//					&& ComparisonPos.y + ComparisonSize.y > Pos.y
	//					&& ComparisonPos.y < Pos.y + VtxMax.y
	//					&& ComparisonPos.z + ComparisonSize.z / 2 > Pos.z + VtxMin.z
	//					&& ComparisonPos.z - ComparisonSize.z / 2 < Pos.z + VtxMax.z)
	//				{//�Ώۂ��u���b�N��+X�ɓ����������̏���
	//					Pos.x += m_move.x;
	//					((CPlayer*)pObj)->SetPos(Pos);
	//				}

	//			}

	//		}
	//	}
	//}
}
//======================================================================================================================================

//===============================================================
//���t�g�u���b�N�̃p�^�[��
//===============================================================
void CBlock::LiftBlockPattern()
{
	//=======================
    //�ϐ��錾
    //=======================
	D3DXVECTOR3 &Pos = CObjectX::GetPos();
	D3DXVECTOR3 &Move = GetMove();                                //�ړ��ʂ��擾
	D3DXVECTOR3 PosOld = CObjectX::GetPosOld();
	int nCntRideFrame = 0;
	float fRadius = 0.0f;
	float add_x = 0.0f;
	float add_y = 0.0f;
	//===================================================

	switch (m_nSubType)
	{
	case 0:
		LiftBlockRoundMove();
	break;
	case 1:
		// �O�p�֐����g�p���A�~�̈ʒu������o���B
		Move.x = cosf(m_fStartRot + m_fRot) * m_fSpeed;
		Move.y = sinf(m_fStartRot + m_fRot) * m_fSpeed;
		// ������ς���
		m_fRot += m_fRotSpeed;		
		break;
	case 2:
		// �O�p�֐����g�p���A�~�̈ʒu������o���B
		Move.x = cosf(m_fStartRot - m_fRot) * m_fSpeed;
		Move.y = sinf(m_fStartRot - m_fRot) * m_fSpeed;

		// ������ς���
		m_fRot += m_fRotSpeed;
		break;
	default:
		break;
	}

}
//======================================================================================================================================

//===============================================================
//���t�g�u���b�N�̉��̓���
//===============================================================
void CBlock::LiftBlockRoundMove()
{
	D3DXVECTOR3& Pos = CObjectX::GetPos();
	D3DXVECTOR3& Move = GetMove();
	bool bJustTurn = false;
	D3DXVECTOR3 SupportPos = GetSupportPos();

	//�x�_�ƌ��ݒn�̋����𑪂�
	float fLength = sqrtf(powf(Pos.x - SupportPos.x, 2) + powf(Pos.y - SupportPos.y, 2) + powf(Pos.z - SupportPos.z, 2));

	//�ړ������𒴂�����
	if (fLength > m_fMoveLength)
	{
        m_bTurn = m_bTurn ? false : true;
        bJustTurn = true;
	}

	//���������鏈��
	if (m_bTurn == false)
	{
		Move.x = cosf(m_fStartRot) * m_fSpeed;
		Move.y = sinf(m_fStartRot) * m_fSpeed;
	}
	else
	{
		Move.x = cosf(m_fStartRot) * -m_fSpeed;
		Move.y = sinf(m_fStartRot) * -m_fSpeed;
	}

}
//======================================================================================================================================

//===============================================================
//�_�Ńu���b�N�̃p�^�[��
//===============================================================
void CBlock::BlinkingBlockPattern()
{
	bool bCollisionPattern = false;

	if (m_nCntTime % m_BlinkingBlock.nBlinkingSpeed == 0)
	{
		m_BlinkingBlock.bBlinking = m_BlinkingBlock.bBlinking ? false : true;
	}

	if (m_type == BLOCKTYPE09_BLINKINGRED)
	{
		bCollisionPattern = false;
	}
	else if (m_type == BLOCKTYPE10_BLINKINGBRUE)
	{
		bCollisionPattern = true;
	}

	if (m_BlinkingBlock.bBlinking == bCollisionPattern)
	{
		SetOriginalColor(NORMAL_COL * 0.0f, 2);
		m_bCollision = false;
	}
	else
	{
		m_bCollision = true;
	}

}
//======================================================================================================================================

//===============================================================
//���u���b�N�̃p�^�[��
//===============================================================
void CBlock::ArrowBlockPattern()
{
	D3DXVECTOR3& Rot = GetRot();
	switch (m_nSubType)
	{
	case 0:
		Rot.y = 0.0f;
		break;
	case 1:
		Rot.y = D3DX_PI;
		break;
	default:
		break;
	}
}
//======================================================================================================================================

//===============================================================
//������u���b�N�̏���
//===============================================================
void CBlock::FallBlockProcess()
{
	D3DXVECTOR3& Pos = GetPos();
	if (m_FallBlock.bFall == true)
	{
		m_FallBlock.nRespawnCnt--;
		if (m_FallBlock.nRespawnCnt <= 0)
		{
			Pos = GetSupportPos();
			m_FallBlock.bFall = false;
			m_FallBlock.nReraiseCnt = 0;
			m_FallBlock.nRespawnCnt = m_nMAX_FALLBLOCKRESPAWNCNT;
			SetUseGravity(false);
			SetMove(NULL_VECTOR3);
			SetColorAlphaOnly(0.0f);
		}
	}
	else
	{
		if (m_FallBlock.nReraiseCnt < 100)
		{
			m_FallBlock.nReraiseCnt++;
			SetColorAlphaOnly(float(m_FallBlock.nReraiseCnt) * 0.01f);
		}
	}

}
//======================================================================================================================================

//=======================================================================
//���\������
//=======================================================================
void CBlock::DispInfo()
{
	m_fRot = 0.0f;
	//��]�J�n�ʒu
	if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
	{//�V�t�g�L�[�������Ȃ���E�E�E
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_1) == true)
		{
			m_fStartRot -= 0.1f;

		}
	}
	else if (CManager::GetInputKeyboard()->GetTrigger(DIK_1) == true)
	{
		m_fStartRot += 0.1f;
	}

	//��]���x
	if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
	{//�V�t�g�L�[�������Ȃ���E�E�E
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_2) == true)
		{
			m_fRotSpeed -= 0.01f;

		}
	}
	else if (CManager::GetInputKeyboard()->GetTrigger(DIK_2) == true)
	{
		m_fRotSpeed += 0.01f;
	}

	//�ړ�����
	if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
	{//�V�t�g�L�[�������Ȃ���E�E�E
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_3) == true)
		{
			m_fMoveLength -= 5.0f;

		}
	}
	else if (CManager::GetInputKeyboard()->GetTrigger(DIK_3) == true)
	{
		m_fMoveLength += 5.0f;
	}
	
	//�ړ����x
	if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
	{//�V�t�g�L�[�������Ȃ���E�E�E
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_4) == true)
		{
			m_fSpeed -= 0.1f;
		}
	}
	else if (CManager::GetInputKeyboard()->GetTrigger(DIK_4) == true)
	{
		m_fSpeed += 0.1f;
	}

	//�_�ő��x
	if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
	{//�V�t�g�L�[�������Ȃ���E�E�E
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_5) == true)
		{
			m_BlinkingBlock.nBlinkingSpeed -= 5;

		}
	}
	else if (CManager::GetInputKeyboard()->GetTrigger(DIK_5) == true)
	{
		m_BlinkingBlock.nBlinkingSpeed += 5;
	}

	//���u���b�N�̉����x
	if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
	{//�V�t�g�L�[�������Ȃ���E�E�E
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_6) == true)
		{
			m_ArrowBlock.fAddSpeed -= 0.2f;

		}
	}
	else if (CManager::GetInputKeyboard()->GetTrigger(DIK_6) == true)
	{
		m_ArrowBlock.fAddSpeed += 0.2f;
	}

	//�W�����v�u���b�N�̃W�����v��
	if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
	{//�V�t�g�L�[�������Ȃ���E�E�E
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_7) == true)
		{
			m_JumpBlock.fJumpPower -= 1.0f;
		}
	}
	else if (CManager::GetInputKeyboard()->GetTrigger(DIK_7) == true)
	{
		m_JumpBlock.fJumpPower += 1.0f;
	}

	CManager::GetDebugText()->PrintDebugText("��]�J�n�ʒu�i�P�j�F%f\n", m_fStartRot);
	CManager::GetDebugText()->PrintDebugText("��]���x�i�Q�j�F%f\n", m_fRotSpeed);
	CManager::GetDebugText()->PrintDebugText("�ړ������i�R�j�F%f\n", m_fMoveLength);
	CManager::GetDebugText()->PrintDebugText("�ړ����x�i�S�j�F%f\n", m_fSpeed);
	CManager::GetDebugText()->PrintDebugText("�_�ő��x�i�T�j�F%d\n", m_BlinkingBlock.nBlinkingSpeed);
	CManager::GetDebugText()->PrintDebugText("�����x�i�U�j�F%f\n", m_ArrowBlock.fAddSpeed);
	CManager::GetDebugText()->PrintDebugText("�W�����v�́i�V�j�F%f", m_JumpBlock.fJumpPower);

	CObjectX::DispInfo();

}
//======================================================================================================================================

//=======================================================================
//�߂��Ƀu���b�N�����邩�ǂ������T�[�`����
//=======================================================================
void CBlock::NearBlockSearch()
{
	CObject* pObj = nullptr;
	D3DXVECTOR3 Pos = GetPos();//�u���b�N�̈ʒu
	D3DXVECTOR3 VtxMax = GetVtxMax();//�ő咸�_
	D3DXVECTOR3 VtxMin = GetVtxMin();//�ŏ����_
	D3DXVECTOR3 Size = GetSize();    //�T�C�Y
	D3DXVECTOR3 ComparisonPos = NULL_VECTOR3;   //�Ώۂ̈ʒu
	D3DXVECTOR3 ComparisonVtxMax = NULL_VECTOR3;//�Ώۂ̍ő咸�_
	D3DXVECTOR3 ComparisonVtxMin = NULL_VECTOR3;//�Ώۂ̍ŏ����_

	float fLength = 0.0f;//�����v�Z�p
	float fLengthX = 0.0f;//X���W�̋����v�Z�p
	float fLengthY = 0.0f;//Y���W�̋����v�Z�p
	float fCheckLength = 0.0f;//��r�p
	bool bSuccess = false;
	bool bEnd = false;//���[�v�I���t���O
	for (int nCntPriority = 0; nCntPriority < m_nMAXPRIORITY; nCntPriority++)
	{
		pObj = GetTopObject(nCntPriority);//�g�b�v�I�u�W�F�N�g���擾
		if (bEnd == true)
		{
			break;
		}
		else
		{
			while (pObj != nullptr)
			{
				//���̃I�u�W�F�N�g���i�[
				CObject* pNext = pObj->GetNextObject();

				//��ނ̎擾�i�G�Ȃ瓖���蔻��j
				CObject::TYPE type = pObj->GetType();

				if (type == CObject::TYPE_BLOCK && pObj != this)
				{//�Ώۂ��u�u���b�N�v���A�����ȊO�Ȃ�v�Z���J�n����
					CBlock* pBlock = (CBlock*)pObj;
					ComparisonPos = pBlock->GetPos();
					ComparisonVtxMax = pBlock->GetVtxMax();
					ComparisonVtxMin = pBlock->GetVtxMin();

					if (Pos.x + VtxMin.x - 1.0f <= ComparisonPos.x + ComparisonVtxMax.x &&
						Pos.x + VtxMax.x + 1.0f >= ComparisonPos.x + ComparisonVtxMin.x &&
						Pos.y + VtxMin.y - 1.0f <= ComparisonPos.y + ComparisonVtxMax.y &&
						Pos.y + VtxMax.y + 1.0f >= ComparisonPos.y + ComparisonVtxMin.y)
					{//�u���b�N���m���d�Ȃ��Ă�����E�E�E
						fLengthX = sqrtf(powf(ComparisonPos.x - Pos.x, 2));
						fLengthY = sqrtf(powf(ComparisonPos.y - Pos.y, 2));

						if (fLengthY > fLengthX)
						{//Y�͕K���D��Ȃ̂ŁA�����ŏ������I���BY�̋����������̂ŁA
							m_bNearBlockY = true;
							bEnd = true;
						}
						else
						{//X�̋����������̂ŁA���ɕ���ł���
							m_bNearBlockX = true;
							bEnd = true;
						}
					}
				}
				else
				{
					pObj = pNext;
					continue;
				}
				//���X�g�����ɐi�߂�
				pObj = pNext;
			}
		}
	}

	float fRandRot = CCalculation::CalculationRandVecXY();
	//if (m_bNearBlockX == true)
	//{
	//	CParticle::Create(CParticle::TYPE00_NORMAL, 20, 20.0f, 20.0f, Pos, D3DXVECTOR3(sinf(fRandRot) * 20.0f, cosf(fRandRot) * 20.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), true);
	//}

	//if (m_bNearBlockY == true)
	//{
	//	CParticle::Create(CParticle::TYPE00_NORMAL, 20, 20.0f, 20.0f, Pos, D3DXVECTOR3(sinf(fRandRot) * 10.0f, cosf(fRandRot) * 10.0f, 0.0f), D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), true);
	//}

}
//======================================================================================================================================

//=======================================================================
//�߂��̃u���b�N��T������
//=======================================================================
void CBlock::NearBlockSurchProcess(D3DXVECTOR3 Pos, D3DXVECTOR3 Size)
{
	CObject* pObj = nullptr;
	float fLength = 0.0f;
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
				CBlock* pBlock = (CBlock*)pObj;
				pBlock->m_bNearBlockX = false;
				pBlock->m_bNearBlockY = false;
				
				float fLength = CCalculation::CalculationLength(Pos, pBlock->GetPos());
				float fBlockSize = 0.0f;
				float fObjXSize = 0.0f;

				if (pBlock->GetSize().x > pBlock->GetSize().y)
				{
					fBlockSize = pBlock->GetSize().x;
				}
				else
				{
					fBlockSize = pBlock->GetSize().y;
				}

				if (Size.x > Size.y)
				{
					fObjXSize = Size.x;
				}
				else
				{
					fObjXSize = Size.y;
				}

				if (fLength < fBlockSize + fObjXSize)
				{
					pBlock->NearBlockSearch();//�߂��̃u���b�N��T��
				}
			}
			else
			{
				pObj = pNext;
				continue;
			}

			//���X�g�����֐i�߂�
			pObj = pNext;
		}
	}
}
//======================================================================================================================================

//=======================================================================
//X�����̉����o��������s��
//=======================================================================
void CBlock::ExtrusionCollisionX(CObjectX* pMyObjX, CBlock* pBlock)
{
	D3DXVECTOR3 MyPos = pMyObjX->GetPos();
	D3DXVECTOR3& Pos = pMyObjX->GetPos();              //�ʒu���擾
	D3DXVECTOR3& Move = pMyObjX->GetMove();             //�ړ��ʂ��擾
	D3DXVECTOR3& PosOld = pMyObjX->GetPosOld();        //1f�O�̈ʒu���擾
	D3DXVECTOR3 MyVtxMax = NULL_VECTOR3;             //�������g�̍ő咸�_
	D3DXVECTOR3 MyVtxMin = NULL_VECTOR3;             //�������g�̍ŏ����_

	D3DXVECTOR3 ComPos = pBlock->GetPos();             //�u���b�N�̈ʒu
	D3DXVECTOR3 ComVtxMax = pBlock->GetVtxMax();       //�u���b�N�̍ő咸�_
	D3DXVECTOR3 ComVtxMin = pBlock->GetVtxMin();       //�u���b�N�̍ŏ����_
	D3DXVECTOR3 ComMove = pBlock->GetMove();           //�u���b�N�̍ŏ����_

	bool& bIsWalling = pMyObjX->GetWalling();          //�ǂɂ������Ă��邩�ǂ���

	bool& bSuccessCollisionX = pMyObjX->GetSuccessBlockCollisionX();

	//�����o���Ɏg���l�̏����_��؂�̂Ă�i�����ȍ��Ŏ��s���Ȃ����߁j
	MyVtxMax.x = (float)(floor(pMyObjX->GetVtxMax().x));
	MyVtxMax.y = (float)(floor(pMyObjX->GetVtxMax().y));
	MyVtxMax.z = (float)(floor(pMyObjX->GetVtxMax().z));

	MyVtxMin.x = (float)(floor(pMyObjX->GetVtxMin().x));
	MyVtxMin.y = (float)(floor(pMyObjX->GetVtxMin().y));
	MyVtxMin.z = (float)(floor(pMyObjX->GetVtxMin().z));

	ComPos.x = float(floor(pBlock->GetPos().x));
	ComPos.y = float(floor(pBlock->GetPos().y));
	ComPos.z = float(floor(pBlock->GetPos().z));

	ComVtxMax.x = (float)(floor(ComVtxMax.x));
	ComVtxMax.y = (float)(floor(ComVtxMax.y));
	ComVtxMax.z = (float)(floor(ComVtxMax.z));

	ComVtxMin.x = (float)(floor(ComVtxMin.x));
	ComVtxMin.y = (float)(floor(ComVtxMin.y));
	ComVtxMin.z = (float)(floor(ComVtxMin.z));
	// 					//��
	if (Pos.x + MyVtxMax.x > ComPos.x + ComVtxMin.x
		&& PosOld.x + MyVtxMax.x <= ComPos.x + ComVtxMin.x
		&& Pos.y + MyVtxMax.y > ComPos.y + ComVtxMin.y
		&& Pos.y + MyVtxMin.y < ComPos.y + ComVtxMax.y
		&& Pos.z + MyVtxMax.z > ComPos.z + ComVtxMin.z
		&& Pos.z + MyVtxMin.z < ComPos.z + ComVtxMax.z)
	{//�Ώۂ��u���b�N��-X�ɓ����������̏���
		Pos.x = ComPos.x + ComVtxMin.x - MyVtxMax.x;
		bIsWalling = true;
		bSuccessCollisionX = true;
	}

	//�E
	else if (Pos.x + MyVtxMin.x < ComPos.x + ComVtxMax.x
		&& PosOld.x + MyVtxMin.x >= ComPos.x + ComVtxMax.x
		&& Pos.y + MyVtxMax.y > ComPos.y + ComVtxMin.y
		&& Pos.y + MyVtxMin.y < ComPos.y + ComVtxMax.y
		&& Pos.z + MyVtxMax.z > ComPos.z + ComVtxMin.z
		&& Pos.z + MyVtxMin.z < ComPos.z + ComVtxMax.z)
	{//�Ώۂ��u���b�N��+X�ɓ����������̏���
		Pos.x = ComPos.x + ComVtxMax.x - MyVtxMin.x;
		bIsWalling = true;
		bSuccessCollisionX = true;
	}
}
//======================================================================================================================================

//=======================================================================
//Y�����̉����o��������s��
//=======================================================================
void CBlock::ExtrusionCollisionY(CObjectX* pMyObjX, CBlock* pBlock)
{
	D3DXVECTOR3 MyVtxMax = NULL_VECTOR3;             //�������g�̍ő咸�_
	D3DXVECTOR3 MyVtxMin = NULL_VECTOR3;             //�������g�̍ŏ����_

	D3DXVECTOR3 ComPos = pBlock->GetPos();             //�u���b�N�̈ʒu
	D3DXVECTOR3 ComVtxMax = pBlock->GetVtxMax();       //�u���b�N�̍ő咸�_
	D3DXVECTOR3 ComVtxMin = pBlock->GetVtxMin();       //�u���b�N�̍ŏ����_[
	D3DXVECTOR3 ComMove = pBlock->GetMove();           //�u���b�N�̍ŏ����_[

	D3DXVECTOR3& Pos = pMyObjX->GetPos();              //�ʒu���擾
	D3DXVECTOR3& Move = pMyObjX->GetMove();             //�ړ��ʂ��擾
	D3DXVECTOR3 PosOld = pMyObjX->GetPosOld();        //1f�O�̈ʒu���擾
	bool& bDodgeState = pMyObjX->GetDodgeState();      //�����Ԃ��ǂ������擾

	bool& bIsLiftLanding = pMyObjX->GetLiftLanding();  //���t�g�̏�ɏ���Ă��邩�ǂ���

	bool& bIsLanding = pMyObjX->GetLanding();          //�n�ʂɂ��邩�ǂ���

	bool& bIsJumping = pMyObjX->GetJumping();          //�W�����v���Ă��邩�ǂ���

	bool& bSuccessBlockCollisionY = pMyObjX->GetSuccessBlockCollisionY();//�u���b�N�Ƃ�X�����̓����蔻�肪�����������ǂ���

	//�����o�����s�΍�
	MyVtxMax.x = (float)(floor(pMyObjX->GetVtxMax().x));
	MyVtxMax.y = (float)(floor(pMyObjX->GetVtxMax().y));
	MyVtxMax.z = (float)(floor(pMyObjX->GetVtxMax().z));

	MyVtxMin.x = (float)(floor(pMyObjX->GetVtxMin().x));
	MyVtxMin.y = (float)(floor(pMyObjX->GetVtxMin().y));
	MyVtxMin.z = (float)(floor(pMyObjX->GetVtxMin().z));

	ComPos.x = float(floor(pBlock->GetPos().x));
	ComPos.y = float(floor(pBlock->GetPos().y));
	ComPos.z = float(floor(pBlock->GetPos().z));

	ComVtxMax.x = (float)(floor(ComVtxMax.x));
	ComVtxMax.y = (float)(floor(ComVtxMax.y));
	ComVtxMax.z = (float)(floor(ComVtxMax.z));

	ComVtxMin.x = (float)(floor(ComVtxMin.x));
	ComVtxMin.y = (float)(floor(ComVtxMin.y));
	ComVtxMin.z = (float)(floor(ComVtxMin.z));

	CObject*& pLandingObj = pMyObjX->GetLandingObj();

	//��
	if (Pos.x + MyVtxMax.x > ComPos.x + ComVtxMin.x
		&& Pos.x + MyVtxMin.x < ComPos.x + ComVtxMax.x
		&& Pos.y + MyVtxMin.y < ComPos.y + ComVtxMax.y
		&& PosOld.y + MyVtxMin.y - Move.y >= ComPos.y + ComVtxMax.y
		&& Pos.z + MyVtxMax.z > ComPos.z + ComVtxMin.z
		&& Pos.z + MyVtxMin.z < ComPos.z + ComVtxMax.z)
	{//�Ώۂ̉��[�����f����+Y�ɓ����������̏���
		switch (this->GetType())
		{
		case BLOCKTYPE04_RIFT:
 			bIsLiftLanding = true;
			pLandingObj = pBlock;
			break;
		case BLOCKTYPE15_FALL:
			if (pMyObjX->GetType() == TYPE_PLAYER)
			{
				bIsLiftLanding = true;
				pLandingObj = pBlock;

				if (m_FallBlock.bFall == false)
				{
					SetUseGravity(true);
					m_FallBlock.bFall = true;//������̂��J�n
				}
			}
			break;
		default:
			break;
		}

		float fPosY = fabsf(MyVtxMin.y);
		Pos.y = ComPos.y + ComVtxMax.y + fPosY;
		Pos += ComMove;

		bIsLanding = true;
		bSuccessBlockCollisionY = true;
	}
	//��
	else if (Pos.x + MyVtxMax.x > ComPos.x + ComVtxMin.x
		&& Pos.x + MyVtxMin.x < ComPos.x + ComVtxMax.x
		&& Pos.y + MyVtxMax.y > ComPos.y + ComVtxMin.y
		&& PosOld.y + MyVtxMax.y <= ComPos.y + ComVtxMin.y
		&& Pos.z + MyVtxMax.z > ComPos.z + ComVtxMin.z
		&& Pos.z + MyVtxMin.z < ComPos.z + ComVtxMax.z)
	{//�Ώۂ̉��[�����f����+Y�ɓ����������̏���
		Pos.y = ComPos.y + ComVtxMin.y - MyVtxMax.y;
		bIsJumping = false;

		if (pMyObjX->GetType() == CObject::TYPE_PLAYER)
		{
			Move.y = 0.0f;
		}

		bSuccessBlockCollisionY = true;
	}
}
//======================================================================================================================================
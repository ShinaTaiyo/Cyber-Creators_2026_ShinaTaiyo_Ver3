//=====================================================================
//
//�V���S���F����擾�A�C�e�������[item.cpp]
//Author:ShinaTaiyo
//
//=====================================================================

//======================================
//�C���N���[�h
//======================================
#include "item.h"
#include "main.h"
#include "game.h"
#include "edit.h"
#include "manager.h"
#include "renderer.h"
#include "particle.h"
#include "score.h"
#include "player.h"
#include "magic.h"
#include "fade.h"
#include "objectXInfo.h"
#include "collision.h"
#include "stagemanager.h"
//=====================================================================

//======================================
//�ÓI�����o������
//======================================
int CItem::m_nNumFile = CItem::ITEMTYPE_MAX;  //�t�@�C���̐�
const char* CItem::m_ITEMFILENAME[CItem::ITEMTYPE_MAX] = 
{ "data\\MODEL\\Item\\Item00_HealHeart.x",
"data\\MODEL\\Item\\Item01_Coin.x" ,
"data\\MODEL\\Item\\Item02_MagicPortion.x",
"data\\MODEL\\Item\\Item03_Crown.x",
"data\\MODEL\\Item\\Item04_DoubleJumpShoes.x",
 "data\\MODEL\\Item\\Item05_DodgeCrystal.x",
 "data\\MODEL\\Item\\Item06_WallJumpCrow.x",
 "data\\MODEL\\Item\\Item07_FireBallEnergy.x",
 "data\\MODEL\\Item\\Item08_BeamEnergy.x",
 "data\\MODEL\\Item\\Item09_IceEnergy.x",
 "data\\MODEL\\Item\\Item10_ThunderEnergy.x",
 "data\\MODEL\\Item\\Item11_MagicSwordEnergy.x",
 "data\\MODEL\\Item\\Item12_PlantShieldEnergy.x",
 "data\\MODEL\\Item\\Item13_QuickHealEnergy.x",
 "data\\MODEL\\Item\\Item14_CrossBombEnergy.x",
 "data\\MODEL\\Item\\Item15_BlackHoleEnergy.x",
 "data\\MODEL\\Item\\Item16_PhycoBurstEnergy.x",
 "data\\MODEL\\Item\\Item17_ChargeShotGun.x",
 "data\\MODEL\\Item\\Item18_RainShotGun.x",
 "data\\MODEL\\Item\\Item19_BurstShotGun.x",
 "data\\MODEL\\Item\\Item20_RapidShotGun.x",
};//�A�C�e���̃��f���̃p�X

//======================================
//�R���X�g���N�^
//======================================
CItem::CItem()
{

}
//=====================================================================

//======================================
//�f�X�g���N�^
//======================================
CItem::~CItem()
{

}
//=====================================================================

//======================================
//����������
//======================================
HRESULT CItem::Init()
{
	//=================================
	//X�I�u�W�F�N�g������
	//=================================
	CObjectX::Init();

	//=================================================================

	m_type = ITEMTYPE00_HEALHEART;//���
	m_bUse = true;              //�d�l���
	m_nCntTime = 0;              //�o�����Ă���̎���
	return S_OK;
}
//=====================================================================

//======================================
//�I������
//======================================
void CItem::Uninit()
{
	CObjectX::Uninit(); //X�I�u�W�F�N�g�̔j��
}
//=====================================================================

//======================================
//�X�V����
//======================================
void CItem::Update()
{
	//==================================
    //X�I�u�W�F�N�g��������擾
    //==================================
	D3DXVECTOR3 &Pos = CObjectX::GetPos();                      //�I�u�W�F�N�g�̈ʒu���擾
	D3DXVECTOR3 &Rot = CObjectX::GetRot();                      //�I�u�W�F�N�g�̌������擾
	//===========================================================

	//==================================
	//�A�C�e�����Ƃ̃p�^�[��
	//==================================
	m_nCntTime++;//�o�����Ă���̎��Ԃ𑪂�

	Rot.y += 0.01f;

	//==================================
	//X�I�u�W�F�N�g�X�V
	//==================================
	CObjectX::Update();
	//===========================================================

	//==================================
	//�����蔻����s��
	//==================================
	if (CScene::GetMode() == CScene::MODE_GAME)
	{
		Collision();
	}
	//===========================================================

	float fVXaim3D = 0.0f;
	float fVYaim3D = 0.0f;
	float fVZaim3D = 0.0f;
	float fVLaim3D = 0.0f;
	D3DXVECTOR3 RandPos = NULL_VECTOR3;
	//==================================
	//�A�C�e���̌��ʔ����Ɣj��
	//==================================
	if (m_bUse == false)
	{
		SetDeath();//���S�t���O��ݒ�
	}
	//===========================================================

}
//=====================================================================

//======================================
//�`�揈��
//======================================
void CItem::Draw()
{
	CObjectX::Draw();
}
//=====================================================================

//======================================
//���S�t���O��ݒ�
//======================================
void CItem::SetDeath()
{
	CObject::SetDeath();
}
//=====================================================================


//======================================
//�A�C�e����������
//======================================
CItem* CItem::Create(ITEMTYPE type, D3DXVECTOR3 Pos, D3DXVECTOR3 Rot, D3DXVECTOR3 Scale, int nRotType)
{
	CItem* pItem = DBG_NEW CItem;   //�G�𐶐�
	bool bSuccess = pItem->CObject::GetCreateSuccess();
	int nIdx = 0;//���f���̃C���f�b�N�X
	bSuccess = NotSummonProcess(type);
	
	pItem->Init();            //����������
	pItem->m_bUse = true;     //�g�p���
	pItem->m_type = type;     //�e�̎��
	pItem->SetUseGravity(false);
	pItem->SetUseBlockCollision(false);
	pItem->SetUseDeath(true); //���S�t���O�𔭓����邩�ǂ�����ݒ肷��
	pItem->SetLife(1);        //�̗͂�ݒ�
	pItem->SetMaxLife(1);     //�ő�̗�
	pItem->SetAutoSubLife(false);//�����I�ɑ̗͂����炷���ǂ���
	pItem->CObjectX::SetPos(Pos);                                                                                  //�I�u�W�F�N�g�Q�c�̈ʒu��ݒ�
	pItem->SetPosOld(Pos);                                                                                         //1f�O�̈ʒu��ݒ�
	pItem->SetSupportPos(Pos);                                                                                     //�ݒu�ʒu
	pItem->CObject::SetType(CObject::TYPE_ITEM);                                                                  //�I�u�W�F�N�g�̎�ނ����߂�
	pItem->CObjectX::SetScale(Scale);                                                                              //�I�u�W�F�N�g�̊g�嗦
	pItem->SetFormarScale(Scale);                                                                                  //���̊g�嗦��ݒ肷��
	nIdx = CManager::GetObjectXInfo()->Regist(CObjectX::OBJECTXTYPE_ITEM, m_ITEMFILENAME[(int)(type)]);
	pItem->CObjectX::BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(CObjectX::OBJECTXTYPE_ITEM, nIdx),
		CManager::GetObjectXInfo()->GetBuffMat(CObjectX::OBJECTXTYPE_ITEM, nIdx),
		CManager::GetObjectXInfo()->GetdwNumMat(CObjectX::OBJECTXTYPE_ITEM, nIdx),
		CManager::GetObjectXInfo()->GetTexture(CObjectX::OBJECTXTYPE_ITEM, nIdx),
		CManager::GetObjectXInfo()->GetColorValue(CObjectX::OBJECTXTYPE_ITEM, nIdx));                       //���f���������蓖�Ă�

	pItem->CObjectX::SetObjXType(CObjectX::OBJECTXTYPE_ITEM);                             //�I�u�W�F�N�gX�̃^�C�v��ݒ�
	pItem->CObjectX::SetTypeNum((int)(type));                                             //�I�u�W�F�N�gX���Ƃ̃^�C�v�ԍ���ݒ�
	pItem->SetSize();                                                                     //X�I�u�W�F�N�g�̃T�C�Y��ݒ肷��
	pItem->CObjectX::SetRotType(CObjectX::ROTTYPE::ROTTYPE_NORMAL);                       //�����̎�ނ�ݒ�
	pItem->SetManagerType((int)(CStageManager::MANAGEROBJECT_ITEM));                      //�}�l�[�W���[�ŌĂяo�����̎�ނ�ݒ�

	if (bSuccess == false)
	{
		pItem->Release();
	}
	return pItem;
}
//=====================================================================================================

//====================================================
//�����蔻����s��
//====================================================
void CItem::Collision(void)
{
}
//=====================================================================================================

//====================================================
//�������Ȃ�����
//====================================================
bool CItem::NotSummonProcess(ITEMTYPE type)
{
	bool bJudge = true;
	if (CScene::GetMode() == CScene::MODE_GAME)
	{
		if (CGame::GetPlayer() != nullptr)
		{
			int nNumPlayerMagic = CGame::GetPlayer()->GetNumPlayerMagic();//�v���C���[�̖��@�̑������擾����
			bool* bUnlockMagicPlayer = CGame::GetPlayer()->GetUnlockMagic();

			//�v���C���[���J���ς݂̖��@�̓X�|�[�������Ȃ�

			switch (type)
			{
			case ITEMTYPE07_FIREBALL_ENERGY:
				if (bUnlockMagicPlayer[CMagicDisp::MAGICDISP00_FIRE] == true)
				{
					bJudge = false;
				}
				break;
			case ITEMTYPE08_BEAMWHIP_ENERGY:
				if (bUnlockMagicPlayer[CMagicDisp::MAGICDISP01_BEAM] == true)
				{
					bJudge = false;
				}
				break;
			case ITEMTYPE09_ICE_ENERGY:
				if (bUnlockMagicPlayer[CMagicDisp::MAGICDISP02_ICE] == true)
				{
					bJudge = false;
				}
				break;
			case ITEMTYPE10_THUNDER_ENERGY:
				if (bUnlockMagicPlayer[CMagicDisp::MAGICDISP03_THUNDER] == true)
				{
					bJudge = false;
				}
				break;
			case ITEMTYPE11_MAGICSWORD_ENERGY:
				if (bUnlockMagicPlayer[CMagicDisp::MAGICDISP04_SWORD] == true)
				{
					bJudge = false;
				}
				break;
			case ITEMTYPE12_PLANTSHIELD_ENERGY:
				if (bUnlockMagicPlayer[CMagicDisp::MAGICDISP05_PLANTSHIELD] == true)
				{
					bJudge = false;
				}
				break;
			case ITEMTYPE13_QUICKHEAL_ENERGY:
				if (bUnlockMagicPlayer[CMagicDisp::MAGICDISP06_QUICKHEAL] == true)
				{
					bJudge = false;
				}
				break;
			case ITEMTYPE14_CROSSBOMB_ENERGY:
				if (bUnlockMagicPlayer[CMagicDisp::MAGICDISP07_CROSSBOMB] == true)
				{
					bJudge = false;
				}
				break;
			case ITEMTYPE15_BLACKHOLE_ENERGY:
				if (bUnlockMagicPlayer[CMagicDisp::MAGICDISP08_BLACKHOLE] == true)
				{
					bJudge = false;
				}
				break;
			case ITEMTYPE16_PHYCOBURST_ENERGY:
				if (bUnlockMagicPlayer[CMagicDisp::MAGICDISP09_PHYCOBURST] == true)
				{
					bJudge = false;
				}
				break;
			default:
				break;
			}
		}
	}
	return bJudge;
}
//=====================================================================================================
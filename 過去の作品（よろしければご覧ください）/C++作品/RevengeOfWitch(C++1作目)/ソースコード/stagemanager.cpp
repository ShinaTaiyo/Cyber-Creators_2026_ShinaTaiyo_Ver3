//==========================================================
//
//�V���P�V���F�I�u�W�F�N�g�̃��X�g�Ǘ�������[stagemanager.cpp]
//Author:ShinaTaiyo
//
//==========================================================

//================================
//�C���N���[�h
//================================
#include "stagemanager.h"
#include "effect.h"
#include "objectX.h"
#include "objectXInfo.h"
#include "input.h"
#include "manager.h"
#include "trap.h"
#include "object.h"
#include "camera.h"
#include "item.h"
#include "marker.h"
#include "particle.h"
#include "boss.h"
#include "debugtext.h"
#include "signboard.h"
#include "bg3d.h"
#include <stdio.h>
#include <string.h>
#include "calculation.h"
//==========================================================

//================================
//�O���錾
//================================
const char* CStageManager::m_apWORLDMAP_TXT[CStageManager::WORLDTYPE_MAX] =
{
	"data\\TEXTFILE\\Map\\EasyMap.txt",
	"data\\TEXTFILE\\Map\\NormalMap.txt",
	"data\\TEXTFILE\\Map\\BossRushMap.txt",
};

//================================
//�R���X�g���N�^
//================================
CStageManager::CStageManager() : m_nWorldIndex(0),m_pBg3D(nullptr)
{
	for (int nCnt = 0; nCnt < m_nMAXMANAGEROBJECT; nCnt++)
	{
		m_apObjectList[nCnt] = nullptr;
	}

	for (int nCnt = 0; nCnt < m_nMAX_MAP; nCnt++)
	{
		for (int nCntWord = 0; nCntWord < m_nMAX_WORD; nCntWord++)
		{
			m_aMapFilePass[nCnt][nCntWord] = {};
		}
	}
}
//==========================================================

//================================
//�f�X�g���N�^
//================================
CStageManager::~CStageManager()
{

}
//==========================================================

//================================
//����������
//================================
HRESULT CStageManager::Init()
{
	//===========================
	//�����o�ϐ�
	//===========================
	m_nCntTime = 0;                                      //�o�����Ă���̎��Ԃ��J�E���g����
	m_nManagerType = MANAGEROBJECT_BLOCK;                //�}�l�[�W���[�I�u�W�F�N�g�̃^�C�v�ԍ�
	m_nType = 0;                                         //�I�u�W�F�N�g���Ƃ̃^�C�v�ԍ�
	m_nMapIndex = 0;                                     //�}�b�v�̃C���f�b�N�X
	m_nMapNum = 0;                                       //�}�b�v�̑���
	m_nRotType = 0;                                      //�����̎�� 
	m_Move = NULL_VECTOR3;                               //�ړ���
	m_bMove = NULL_VECTOR3;                              //�ړ����Ă��邩�ǂ���
	m_Scale = ONE_VECTOR3;                               //�g�嗦
	m_Pos = NULL_VECTOR3;                                //�ʒu
	m_SaveBeforeChoosePos = NULL_VECTOR3;                //�I������������O�̈ʒu���L������
	m_fSpeed = 5.0f;                                     //�ړ����x
 	m_pManagerObject = nullptr;                          //�}�l�[�W���[�ɕ\������I�u�W�F�N�g
	m_nIndexObject = 0;                                  //�I�u�W�F�N�g�ԍ�
	m_ManagerMode = MANAGERMODE_ALREADYSTAGE;            //���݂̃X�e�[�W�}�l�[�V���[�̃��[�h
	m_nWarpMapNum = 0;                                   //���[�v�}�[�J�[�̃��[�v��}�b�v�̐ݒ�
	m_WarpPos = NULL_VECTOR3;                            //���[�v�}�[�J�[�̃��[�v�ʒu��ݒ�
	m_nSetLife = 10;                                     //�̗͂�ݒ肷��
	m_nSetSubType = 0;                                   //�ݒ肷��T�u�^�C�v�̔ԍ�  
	m_nMapCameraType = (int)(CCamera::CAMERATYPE_BESIDE);//�}�b�v���Ƃ̃J�����̎�ނ�ݒ�

	//=========================
	//�I���n
	//=========================
	m_pChooseObject = nullptr;                          //�I���I�u�W�F�N�g�ւ�
	m_nIndexChooseObject = 0;                           //�I�����Ă���I�u�W�F�N�g�̔ԍ�
	m_bChooseObject = false;                            //�I�u�W�F�N�g��I�����Ă��邩�ǂ���
	m_bMakeMapMode = false;                             //�}�b�v���샂�[�h���ǂ���
	//=======================================================================================
#ifdef _DEBUG

	if (CScene::GetMode() == CScene::MODE_EDIT)
	{
	    m_pManagerObject = CBlock::Create(CBlock::BLOCKTYPE00_NORMAL, 10, NULL_VECTOR3, NULL_VECTOR3, ONE_VECTOR3, CObjectX::ROTTYPE_NORMAL,m_nSetSubType);
		m_pManagerObject->SetUseDeath(false);
	    //m_pManagerObject = CModel::Create(CModel::MODELTYPE_BILL,NULL_VECTOR3, NULL_VECTOR3, NULL_VECTOR3, m_Scale, CObjectX::ROTTYPE_NORMAL);
	}
#endif // _DEBUG

	//===========================
	//�ړ����[�h�����߂�
	//===========================
	m_MoveMode = MOVEMODE00_XY;     //�ړ����@�̎��
	m_FocusType = FOCUSTYPE_NORMAL;//�J�������ǂ��ʒu�̎��
	m_bUseSizeMove = false;//���݂̃I�u�W�F�N�g�̃T�C�Y���ړ����邩�ǂ���
	//=======================================================================================

	//===========================
	//�I�u�W�F�N�gX����������
	//===========================
	CObject::Init();
	//=======================================================================================

	//===========================
	//3D�w�i�����
	//===========================
	m_pBg3D = CBg3D::Create(CBg3D::BG3DTYPE_STAGE00, NULL_VECTOR3, 20, 2, 8000.0f, 8000.0f);

	//======================================
	//�}�b�v�̃t�@�C���p�X�����[�h����
	//======================================
	LoadMapFilePass(WORLDTYPE00_EASY);
	return S_OK;
}
//==========================================================

//================================
//�I������
//================================
void CStageManager::Uninit()
{
	StageManagerObjectReleaseAll();
     
}
//==========================================================

//================================
//�ʘg�̏I������
//================================
void CStageManager::ExtraUninit()
{

}
//==========================================================

//================================
//�X�V����
//================================
void CStageManager::Update()
{

#ifdef _DEBUG

	if (CScene::GetMode() == CScene::MODE_EDIT)
	{
		if (m_bChooseObject == false)
		{
			//===============================================
			//�X�e�[�W�}�l�[�W���[�̐F��ς���
			//===============================================
			ColorChenge();
			//===========================================================================================

			//===============================================
			//�̗͂�ݒ肷��
			//===============================================
			LifeSet();
			//===========================================================================================

			//===============================================
			//�X�e�[�W�}�l�[�W���[�̈ʒu���ړ�����
			//===============================================
			MoveManager();
			//===========================================================================================

			//===============================================
			//���݂̃u���b�N�̎�ނ�ύX����
			//===============================================
			TypeChenge();
			//===========================================================================================

			//===============================================
			//�g�嗦�����Z�b�g���鏈��
			//===============================================
			ResetScale();
			//===========================================================================================

			//===============================================
			//�u���b�N��ݒ肷��
			//===============================================
			SetObjectX();
			//===========================================================================================

			////===============================================
			////�u���b�N����������
			////===============================================
			//DeleteObjectX();
			////===========================================================================================

			//===============================================
			//�}�b�v��ς��鏈��
			//===============================================
			MapChenge();
			//===========================================================================================

			//===============================================
			//�I�u�W�F�N�g�S��������
			//===============================================
			if (CManager::GetInputKeyboard()->GetTrigger(DIK_RSHIFT) == true)
			{
				StageManagerObjectReleaseAll();
			}
			//===========================================================================================
		}

		//===============================================
		//�I�u�W�F�N�g�I������
		//===============================================
		ChooseObject();
		//===========================================================================================

		//===============================================
		//���\������
		//===============================================
		DispInfo();
		//===========================================================================================

		//===============================================
		//�I�u�W�F�N�g�X�V
		//===============================================
		CObject::Update();
		//===========================================================================================
	}

#endif // _DEBUG
}
//==========================================================

//================================
//�`�揈��
//================================
void CStageManager::Draw()
{
#ifdef _DEBUG

	if (CScene::GetMode() == CScene::MODE_EDIT)
	{
        //�I�u�W�F�N�g�`��
	    CObject::Draw();
	}
#endif // _DEBUG

}
//============================================================================================================

//======================================================
//���S�t���O��ݒ�
//======================================================
void CStageManager::SetDeath()
{
	if (m_pManagerObject != nullptr)
	{
		m_pManagerObject->SetUseDeath(true);
		m_pManagerObject->SetDeath();
		m_pManagerObject = nullptr;
	}

	if (m_pBg3D != nullptr)
	{
		m_pBg3D->SetUseDeath(true);
		m_pBg3D->SetDeath();
		m_pBg3D = nullptr;
	}

	CObject::SetDeath();
}
//============================================================================================================

//============================================
//�}�b�v�����[�h���鏈��
//============================================
void CStageManager::LoadMapTxt(int nMapNum)
{
	//==================
	//�ϐ�
	//==================
	char aString[128] = {};
	m_nMapIndex = nMapNum;//�}�b�v�̃C���f�b�N�X��ݒ�
	FILE* pFile = nullptr;//�t�@�C���|�C���^��錾

	//=================================================
	//���݂̃��[�h�ɂ���ă��[�h����t�@�C����ς���
	//=================================================
	if (m_ManagerMode == MANAGERMODE_ALREADYSTAGE)
	{
		pFile = fopen(&m_aMapFilePass[m_nMapIndex][0], "r");
	}
	else
	{
		pFile = fopen(SAVE_TXT, "r");
	}
	//=====================================================================================================================

	int nManagerType = 0;                  //�}�l�[�W���[�I�u�W�F�N�g�̎��
	int nType = 0;                         //�I�u�W�F�N�g���Ƃ̎��
	int nRotType = 0;                      //�����̎��
	int nSubType = 0;                      //�T�u�^�C�v
	D3DXVECTOR3 pos = NULL_VECTOR3;        //�ʒu
	D3DXVECTOR3 WarpPos = NULL_VECTOR3;
	int nWarpMap = 0;//���[�v��̃}�b�v�ԍ�
	int nLife = 0;//�̗�
	D3DXVECTOR3 Scale = NULL_VECTOR3;      //�g�嗦
	D3DXVECTOR3 Rot = NULL_VECTOR3;        //����
	CObject* pObject = nullptr;            //�I�u�W�F�N�g�ւ̃|�C���^
	//======================================================

	//ReleaseAll();
	if (CScene::GetMode() == CScene::MODE_EDIT)
	{
		StageManagerObjectReleaseAll();
	}


	if (pFile != nullptr)
	{
		while (1)
		{

			fscanf(pFile, "%s", &aString[0]);

			if (aString[0] == '#')
			{
				fgets(&aString[0], 100, pFile);//�s���X�L�b�v
			}
			else if (strcmp(&aString[0], "SETCAMERATYPE") == 0)
			{
				fscanf(pFile, "%s", &aString[0]);  //�C�R�[���ǂݎ��
				fscanf(pFile, "%d", &m_nMapCameraType);//�J�����̎�ނ��擾
				CManager::GetCamera()->SetCameraType((CCamera::CAMERATYPE)(m_nMapCameraType));//�擾�����J�����̎�ނɐݒ�
			}
			else if (strcmp(&aString[0], "SETOBJECT") == 0)
			{
				while (1)
				{
					fscanf(pFile, "%s", &aString[0]);
					if (aString[0] == '#')
					{
						fgets(&aString[0], 100, pFile);//�s���X�L�b�v
					}
					else if (strcmp(&aString[0], "MANAGERTYPE") == 0)
					{
						fscanf(pFile, "%s", &aString[0]);  //�C�R�[���ǂݎ��
						fscanf(pFile, "%d", &nManagerType);//�}�l�[�W���[�I�u�W�F�N�g�̃^�C�v�擾
					}
					else if (strcmp(&aString[0], "TYPE") == 0)
					{
						fscanf(pFile, "%s", &aString[0]);  //�C�R�[���ǂݎ��
						fscanf(pFile, "%d", &nType);       //�I�u�W�F�N�g���Ƃ̃^�C�v�擾
					}
					else if (strcmp(&aString[0], "ROTTYPE") == 0)
					{
						fscanf(pFile, "%s", &aString[0]);  //�C�R�[���ǂݎ��
						fscanf(pFile, "%d", &nRotType);    //�I�u�W�F�N�g���Ƃ̃^�C�v�擾
					}
					else if (strcmp(&aString[0], "POS") == 0)
					{
						fscanf(pFile, "%s", &aString[0]);  //�C�R�[���ǂݎ��
						fscanf(pFile, "%f", &pos.x);       //X���W�ǂݎ��
						fscanf(pFile, "%f", &pos.y);       //Y���W�ǂݎ��
						fscanf(pFile, "%f", &pos.z);       //Z���W�ǂݎ��
					}
					else if (strcmp(&aString[0], "SCALE") == 0)
					{
						fscanf(pFile, "%s", &aString[0]);  //�C�R�[���ǂݎ��
						fscanf(pFile, "%f", &Scale.x);       //X���W�ǂݎ��
						fscanf(pFile, "%f", &Scale.y);       //Y���W�ǂݎ��
						fscanf(pFile, "%f", &Scale.z);       //Z���W�ǂݎ��
					}
					else if (strcmp(&aString[0], "ROT") == 0)
					{
						fscanf(pFile, "%s", &aString[0]);  //�C�R�[���ǂݎ��
						fscanf(pFile, "%f", &Rot.x);       //X���W�ǂݎ��
						fscanf(pFile, "%f", &Rot.y);       //Y���W�ǂݎ��
						fscanf(pFile, "%f", &Rot.z);       //Z���W�ǂݎ��
					}
					else if (strcmp(&aString[0], "LIFE") == 0)
					{
						fscanf(pFile, "%s", &aString[0]);  //�C�R�[���ǂݎ��
						fscanf(pFile, "%d", &nLife);       //Z���W�ǂݎ��
					}
					else if (strcmp(&aString[0], "SUBTYPE") == 0)
					{
						fscanf(pFile, "%s", &aString[0]);  //�C�R�[���ǂݎ��
						fscanf(pFile, "%d", &nSubType);    //�T�u�^�C�v
					}
					else if (strcmp(&aString[0], "END_SETOBJECT") == 0)
					{//X�I�u�W�F�N�g��ݒ�
						if (CScene::GetMode() == CScene::MODE_EDIT)
						{//�G�f�B�b�g���[�h�Ȃ�A�I�u�W�F�N�g�����X�g�ɕۑ����Ǘ�����
							switch (nManagerType)
							{
								case (int)(MANAGEROBJECT_BLOCK) :
									pObject = CBlock::Create((CBlock::BLOCKTYPE)(nType), 10, pos, Rot, Scale, CObjectX::ROTTYPE_NORMAL, nSubType);
									break;
									case (int)(MANAGEROBJECT_MODEL) :
										pObject = CModel::Create((CModel::MODELTYPE)(nType), pos, Rot, NULL_VECTOR3, Scale, CObjectX::ROTTYPE_NORMAL);
										break;
										case (int)(MANAGEROBJECT_ENEMY) :
											pObject = CEnemy::Create((CEnemy::ENEMYTYPE)(nType), nLife, pos, NULL_VECTOR3, Scale,NULL_VECTOR3,NULL_VECTOR3,ONE_VECTOR3);//��������
											break;
											case (int)(MANAGEROBJECT_ITEM) :
												pObject = CItem::Create((CItem::ITEMTYPE)(nType), pos, Rot, Scale, CObjectX::ROTTYPE_NORMAL);//��������
												break;
												case (int)(MANAGEROBJECT_MARKER) :
													pObject = CMarker::Create((CMarker::MARKERTYPE)(nType), pos, Scale, NULL_VECTOR3, 0);
													break;
													case (int)(MANAGEROBJECT_BOSS) :
														pObject = CBoss::SetCreateBoss((CBoss::BOSSTYPE)(nType),nLife,pos,Scale);
														break;
														case (int)(MANAGEROBJECT_SIGNBOARD):
															pObject = CSignBoard::Create(CSignBoard::SIGNBOARDTYPE(nType),
																(CSignBoard::SIGNBOARDMANUALTYPE)(nSubType),pos,Scale,Rot);													default:
														break;
							}

							//#ifdef _DEBUG
							if (pObject != nullptr)
							{	
								if (pObject->GetType() == CObject::TYPE_BOSS)
								{
									//
									int nCnt = 0;
								}

								pObject->SetStageManagerObjNum(m_nIndexObject);//�X�e�[�W�}�l�[�W���[�ŊǗ�����I�u�W�F�N�g�̔ԍ���ݒ�
								pObject->SetStageManagerObj();                 //�X�e�[�W�}�l�[�W���[�ŊǗ�����I�u�W�F�N�g�ɐݒ�	
								m_nIndexObject++;
							}
							//#endif // _DEBUG
						}
						else if (CScene::GetMode() == CScene::MODE_GAME)
						{//�Q�[�����[�h�Ȃ�A�I�u�W�F�N�g�𐶐����邾���ŁA���X�g�ɕۑ����Ȃ�
							switch (nManagerType)
							{
								case (int)(MANAGEROBJECT_BLOCK) :
									CBlock::Create((CBlock::BLOCKTYPE)(nType), 10, pos, Rot, Scale, CObjectX::ROTTYPE_NORMAL, nSubType);
									break;
									case (int)(MANAGEROBJECT_MODEL) :
										CModel::Create((CModel::MODELTYPE)(nType), pos, Rot, NULL_VECTOR3, Scale, CObjectX::ROTTYPE_NORMAL);
										break;
										case (int)(MANAGEROBJECT_ENEMY) :
											CEnemy::Create((CEnemy::ENEMYTYPE)(nType), nLife, pos, NULL_VECTOR3, Scale,NULL_VECTOR3,NULL_VECTOR3,ONE_VECTOR3);//��������
											break;
											case (int)(MANAGEROBJECT_ITEM) :
												CItem::Create((CItem::ITEMTYPE)(nType), pos, Rot, Scale, CObjectX::ROTTYPE_NORMAL);//��������
												break;
												case (int)(MANAGEROBJECT_MARKER) :
													CMarker::Create((CMarker::MARKERTYPE)(nType), pos, Scale, NULL_VECTOR3, 0);
													break;
													case (int)(MANAGEROBJECT_BOSS) :
														CBoss::SetCreateBoss((CBoss::BOSSTYPE)(nType), nLife, pos, Scale);
														break;
														case (int)(MANAGEROBJECT_SIGNBOARD) : CSignBoard::Create(CSignBoard::SIGNBOARDTYPE(nType), 
															(CSignBoard::SIGNBOARDMANUALTYPE)(nSubType), pos, Scale, Rot);
															break;
													default:
														break;
							}
						}
						break;
					}

				}
			}
			else if (strcmp(&aString[0], "SETMARKER") == 0)
			{
				while (1)
				{
					fscanf(pFile, "%s", &aString[0]);
					if (aString[0] == '#')
					{
						fgets(&aString[0], 100, pFile);//�s���X�L�b�v
					}
					else if (strcmp(&aString[0], "MANAGERTYPE") == 0)
					{
						fscanf(pFile, "%s", &aString[0]);  //�C�R�[���ǂݎ��
						fscanf(pFile, "%d", &nManagerType);//�}�l�[�W���[�I�u�W�F�N�g�̃^�C�v�擾
					}
					else if (strcmp(&aString[0], "TYPE") == 0)
					{
						fscanf(pFile, "%s", &aString[0]);  //�C�R�[���ǂݎ��
						fscanf(pFile, "%d", &nType);       //�I�u�W�F�N�g���Ƃ̃^�C�v�擾
					}
					else if (strcmp(&aString[0], "POS") == 0)
					{
						fscanf(pFile, "%s", &aString[0]);  //�C�R�[���ǂݎ��
						fscanf(pFile, "%f", &pos.x);       //X���W�ǂݎ��
						fscanf(pFile, "%f", &pos.y);       //Y���W�ǂݎ��
						fscanf(pFile, "%f", &pos.z);       //Z���W�ǂݎ��
					}
					else if (strcmp(&aString[0], "SCALE") == 0)
					{
						fscanf(pFile, "%s", &aString[0]);  //�C�R�[���ǂݎ��
						fscanf(pFile, "%f", &Scale.x);       //X���W�ǂݎ��
						fscanf(pFile, "%f", &Scale.y);       //Y���W�ǂݎ��
						fscanf(pFile, "%f", &Scale.z);       //Z���W�ǂݎ��
					}
					else if (strcmp(&aString[0], "ROT") == 0)
					{
						fscanf(pFile, "%s", &aString[0]);  //�C�R�[���ǂݎ��
						fscanf(pFile, "%f", &Rot.x);       //X���W�ǂݎ��
						fscanf(pFile, "%f", &Rot.y);       //Y���W�ǂݎ��
						fscanf(pFile, "%f", &Rot.z);       //Z���W�ǂݎ��
					}
					else if (strcmp(&aString[0], "WARPPOS") == 0)
					{
						fscanf(pFile, "%s", &aString[0]);  //�C�R�[���ǂݎ��
						fscanf(pFile, "%f", &WarpPos.x);       //X���W�ǂݎ��
						fscanf(pFile, "%f", &WarpPos.y);       //Y���W�ǂݎ��
						fscanf(pFile, "%f", &WarpPos.z);       //Z���W�ǂݎ��
					}
					else if (strcmp(&aString[0], "WARPMAPINDEX") == 0)
					{
						fscanf(pFile, "%s", &aString[0]);  //�C�R�[���ǂݎ��
						fscanf(pFile, "%d", &nWarpMap);
					}
					else if (strcmp(&aString[0], "SUBTYPE") == 0)
					{
						fscanf(pFile, "%s", &aString[0]);  //�C�R�[���ǂݎ��
						fscanf(pFile, "%d", &nSubType);    //�T�u�^�C�v
					}
					else if (strcmp(&aString[0], "END_SETMARKER") == 0)
					{//X�I�u�W�F�N�g��ݒ�
						if (CScene::GetMode() == CScene::MODE_EDIT)
						{//�G�f�B�b�g���[�h�Ȃ�A�I�u�W�F�N�g�����X�g�ɕۑ����Ǘ�����
							pObject = CMarker::Create((CMarker::MARKERTYPE)(nType), pos, Scale, WarpPos, nWarpMap);

							if (pObject != nullptr)
							{
								pObject->SetStageManagerObj();                 //�X�e�[�W�}�l�[�W���[�ŊǗ�����I�u�W�F�N�g�ɐݒ�	
								m_nIndexObject++;
							}
						}
						else if (CScene::GetMode() == CScene::MODE_GAME)
						{//�Q�[�����[�h�Ȃ�A�I�u�W�F�N�g�𐶐����邾���ŁA���X�g�ɕۑ����Ȃ�
							int n = 0;
							CMarker::Create((CMarker::MARKERTYPE)(nType), pos, Scale, WarpPos, nWarpMap);
						}
						break;
					}
				}
			}
			else if (strcmp(&aString[0], "SETTRAP") == 0)
			{
			    LoadTrapTxt(pFile);
			}
			else if (strcmp(&aString[0], "SETENEMY") == 0)
			{
			    LoadEnemyTxt(pFile);
			}
			else if (strcmp(&aString[0], "SETBLOCK") == 0)
			{
			    LoadBlockTxt(pFile);
			}
			else if (strcmp(&aString[0], "END_SCRIPT") == 0)
			{
				break;
				fclose(pFile);
			}
		}
	}
}
//============================================================================================================

//===========================================
//�}�b�v�̃t�@�C���p�X�����[�h����
//===========================================
void CStageManager::LoadMapFilePass(WORLDTYPE type)
{
	char aString[512] = {};
	//===============================================================================================

	switch (type)
	{
	case WORLDTYPE00_EASY:
		m_pBg3D->SetBg3DType(CBg3D::BG3DTYPE_STAGE00);
		break;
	case WORLDTYPE01_NORMAL:
		m_pBg3D->SetBg3DType(CBg3D::BG3DTYPE_STAGE01);
		break;
	case WORLDTYPE02_BOSSRUSH:
		m_pBg3D->SetBg3DType(CBg3D::BG3DTYPE_STAGE00);
	default:
		break;
	}

	m_nMapNum = 0;//�}�b�v������������

	FILE* pFileTxt = nullptr;
	pFileTxt = fopen(m_apWORLDMAP_TXT[type], "r");

	if (pFileTxt != nullptr)
	{
		while (1)
		{
			fscanf(pFileTxt, "%s", &aString[0]);
			if (aString[0] == '#')
			{
				fgets(&aString[0], 100, pFileTxt);//�s���X�L�b�v
			}
			else if (strcmp(&aString[0], "FILENAME") == 0)
			{
				fscanf(pFileTxt, "%s", &aString[0]);//�C�R�[���ǂݎ��p
				fscanf(pFileTxt, "%s", &m_aMapFilePass[m_nMapNum][0]);//�t�@�C�����ǂݎ��p
				m_nMapNum++;
			}
			else if (strcmp(&aString[0], "END_SCRIPT") == 0)
			{
				fclose(pFileTxt);
				break;
			}
		}

	}

}
//============================================================================================================

//============================================
//�}�b�v���Z�[�u���鏈��
//============================================
void CStageManager::SaveMapTxt(int nMapNum)
{
	D3DXVECTOR3 Pos = NULL_VECTOR3;
	D3DXVECTOR3 WarpPos = NULL_VECTOR3;
	D3DXVECTOR3 Scale = NULL_VECTOR3;
	D3DXVECTOR3 Rot = NULL_VECTOR3;
	int nWarpMap = 0;//���[�v��̃}�b�v�ԍ�
	int nManagerType;//�I�u�W�F�N�gX�̃^�C�v
	int nRotType; //�����̎��
	int nType = 0;//�I�u�W�F�N�gX���Ƃ̃^�C�v
	D3DXVECTOR3 move = NULL_VECTOR3;
	float fWidth = 0.0f;
	float fHeight = 0.0f;
	int nLife = 0;        //�̗�
	m_nMapIndex = nMapNum;
	int nSubType = 0;     //�T�u�^�C�v
	FILE* pFile = nullptr;
	CObject* pObj = nullptr;//�I�u�W�F�N�g�擾�p

	//=================================================
	//���݂̃��[�h�ɂ���ăZ�[�u����t�@�C����ς���
	//=================================================
	if (m_ManagerMode == MANAGERMODE_ALREADYSTAGE)
	{
		pFile = fopen(&m_aMapFilePass[m_nMapIndex][0], "w");
	}
	else
	{
		pFile = fopen(SAVE_TXT, "w");
	}
	//=====================================================================================================================

	if (pFile != nullptr)
	{
		fprintf(pFile, "SETCAMERATYPE = %d\n\n",m_nMapCameraType);//�J�����̎�ނ�ݒ�

		pObj = GetTopStageManagerObject();//�g�b�v�I�u�W�F�N�g���擾

		while (pObj != nullptr)
		{
			//���̃I�u�W�F�N�g���i�[
			CObject* pNext = pObj->GetNextStageManagerObject();

			//��ނ̎擾�i�G�Ȃ瓖���蔻��j
			CObject::TYPE type = pObj->GetType();

			//==============================================
			//�I�u�W�F�N�g���������o��
			//==============================================
			nLife = ((CObjectX*)pObj)->GetLife();
			nManagerType = ((CObjectX*)pObj)->GetManagerType(); //X�I�u�W�F�N�g�̃^�C�v���擾
			nType = ((CObjectX*)pObj)->GetTypeNum();            //X�I�u�W�F�N�g���Ƃ̃^�C�v���擾
			nRotType = ((CObjectX*)pObj)->GetRotType();         //�����̎�ނ��擾����
			Pos = ((CObjectX*)pObj)->GetSupportPos();           //�ʒu�������o��
			Scale = ((CObjectX*)pObj)->GetScale();				//�g�嗦�������o��
			Rot = ((CObjectX*)pObj)->GetRot();					//�����������o��

			//�T�u�^�C�v�擾
			switch (nManagerType)
			{
			case MANAGEROBJECT_BLOCK:
				nSubType = ((CBlock*)pObj)->GetSubType();
				break;
			case MANAGEROBJECT_TRAP:
				nSubType = ((CTrap*)pObj)->GetSubType();
				break;
			case MANAGEROBJECT_SIGNBOARD:
				nSubType = ((CSignBoard*)pObj)->GetSubType();
				break;
			default:
				nSubType = 0;
				break;
			}

			if (nManagerType == MANAGEROBJECT_MARKER)
			{
				WarpPos = ((CMarker*)pObj)->GetWarpPos();
				nWarpMap = ((CMarker*)pObj)->GetWarpMap();

				fprintf(pFile, "SETMARKER\n");
				fprintf(pFile, "MANAGERTYPE = %d\n", nManagerType);
				fprintf(pFile, "TYPE = %d\n", nType);
				fprintf(pFile, "POS = %.3f %.3f %.3f\n", Pos.x, Pos.y, Pos.z);
				fprintf(pFile, "SCALE = %.3f %.3f %.3f\n", Scale.x, Scale.y, Scale.z);
				fprintf(pFile, "ROT = %.3f %.3f %.3f\n", Rot.x, Rot.y, Rot.z);
				fprintf(pFile, "WARPPOS = %.3f %.3f %.3f\n", WarpPos.x, WarpPos.y, WarpPos.z);
				fprintf(pFile, "WARPMAPINDEX = %d\n", nWarpMap);
				fprintf(pFile, "SUBTYPE = %d\n", nSubType);
				fprintf(pFile, "END_SETMARKER\n\n");
			}
			else if (nManagerType == MANAGEROBJECT_TRAP)
			{//�g���b�v����ۑ�
				SaveTrapTxt(pFile, pObj);
			}
			else if (nManagerType == MANAGEROBJECT_BLOCK)
			{//�u���b�N����ۑ�
				SaveBlockTxt(pFile, pObj);
			}
			else if (nManagerType == MANAGEROBJECT_ENEMY)
			{
				SaveEnemyTxt(pFile, pObj);
			}
			else
			{//���ɓ�����ʂ��Ȃ��ꍇ
				fprintf(pFile, "SETOBJECT\n");
				fprintf(pFile, "MANAGERTYPE = %d\n", nManagerType);
				fprintf(pFile, "TYPE = %d\n", nType);
				fprintf(pFile, "ROTTYPE = %d\n", nRotType);
				fprintf(pFile, "POS = %.3f %.3f %.3f\n", Pos.x, Pos.y, Pos.z);
				fprintf(pFile, "SCALE = %.3f %.3f %.3f\n", Scale.x, Scale.y, Scale.z);
				fprintf(pFile, "ROT = %.3f %.3f %.3f\n", Rot.x, Rot.y, Rot.z);
				fprintf(pFile, "LIFE = %d\n", nLife);
				fprintf(pFile, "SUBTYPE = %d\n", nSubType);
				fprintf(pFile, "END_SETOBJECT\n\n");
			}

			pObj = pNext;//�I�u�W�F�N�g�����ɐi�߂�
		}
		//========================================================================
		fprintf(pFile, "END_SCRIPT");
		fclose(pFile);
	}
	//================================================================================

}
//======================================================================================================================

//==========================================================
//�}�b�v���o�C�i���t�@�C������ǂݍ���
//==========================================================
void CStageManager::LoadMapBin(int nMapNum)
{
	//======================================
	//�ϐ��錾
	//======================================
	D3DXVECTOR3 Pos = NULL_VECTOR3;
	D3DXVECTOR3 Scale = NULL_VECTOR3;
	D3DXVECTOR3 Rot = NULL_VECTOR3;
	int nType = 0;            //�I�u�W�F�N�gX���Ƃ̃^�C�v
	int nManagerType = 0;        //�I�u�W�F�N�gX�̃^�C�v
	int nRotType = 0;         //�����̃^�C�v
	D3DXVECTOR3 move = NULL_VECTOR3;
	float fWidth = 0.0f;
	float fHeight = 0.0f;
	int nData = 0;
	char aString[512] = {};
	char aStringMap[100] = {};
	CObject* pObject = nullptr;
	//===============================================================================================
	StageManagerObjectReleaseAll();
	FILE* pFile = nullptr;
	pFile = fopen(&m_aMapFilePass[nMapNum][0], "rb");
	//pFile = fopen(&m_aMapFilePass[0][0], "rb");
	if (pFile != nullptr)
	{
		//==============================================
		//�I�u�W�F�N�g�̏���ǂݍ���
		//==============================================
		fread(&m_nIndexObject, sizeof(int), 1, pFile);
		if (m_nIndexObject > 0)
		{
			for (int nCnt = 0; nCnt < m_nIndexObject; nCnt++)
			{//�I�u�W�F�N�g��ݒ�
				fread(&nManagerType, sizeof(int), 1, pFile);         //X�I�u�W�F�N�g�̃^�C�v��ǂݍ���
				fread(&nType, sizeof(int), 1, pFile);             //X�I�u�W�F�N�g���Ƃ̃^�C�v��ǂݍ���
				fread(&nRotType, sizeof(int), 1, pFile);          //�����̎�ނ�ǂݍ���
				fread(&Pos, sizeof(D3DXVECTOR3), 1, pFile);       //�ʒu��ǂݍ���
				fread(&Scale, sizeof(D3DXVECTOR3), 1, pFile);     //�g�嗦��ǂݍ���
				fread(&Rot, sizeof(D3DXVECTOR3), 1, pFile);       //������ǂݍ���

				switch (nManagerType)
				{
					case (int)(MANAGEROBJECT_BLOCK) :
						pObject = CBlock::Create(CBlock::BLOCKTYPE(nType), 10, Pos, Rot, Scale, (CObjectX::ROTTYPE)(nRotType), m_nSetSubType);//��������
						break;
						case (int)(MANAGEROBJECT_MODEL) :
							pObject = CModel::Create((CModel::MODELTYPE)(nType), Pos, Rot, NULL_VECTOR3, Scale, (CObjectX::ROTTYPE)(nRotType));//��������
							break;
								case (int)(MANAGEROBJECT_ENEMY) :
									pObject = CEnemy::Create((CEnemy::ENEMYTYPE)(nType), 10, Pos, NULL_VECTOR3, Scale,NULL_VECTOR3,NULL_VECTOR3,ONE_VECTOR3);//��������
									break;
									case (int)(MANAGEROBJECT_ITEM) :
										pObject = CItem::Create((CItem::ITEMTYPE)(nType), Pos, Rot, Scale, CObjectX::ROTTYPE_NORMAL);//��������
										break;
										case (int)(MANAGEROBJECT_MARKER) :
											pObject = CMarker::Create((CMarker::MARKERTYPE)(nType),Pos,Scale, NULL_VECTOR3, 0);
											break;
											case (int)(MANAGEROBJECT_BOSS) :
												pObject = CBoss::SetCreateBoss((CBoss::BOSSTYPE)(nType),10, Pos, Scale);
												break;


								default:
									break;
				}

				if (pObject != nullptr)
				{
					m_apObjectList[nCnt] = pObject;
				}
			}
		}
		//========================================================================
		fclose(pFile);
	}
}
	//======================================================================================================================

//==========================================================
//�}�b�v���o�C�i���t�@�C���ɏ����o��
//==========================================================
void CStageManager::SaveMapBin()
{
	//======================================
	//�ϐ��錾
	//======================================
	D3DXVECTOR3 Pos = NULL_VECTOR3;
	D3DXVECTOR3 Scale = NULL_VECTOR3;
	D3DXVECTOR3 Rot = NULL_VECTOR3;
	int nType = 0;             //�I�u�W�F�N�gX���Ƃ̃^�C�v
	int nManagerType = 0;             //�I�u�W�F�N�gX�̃^�C�v
	int nRotType = 0;              //�I�u�W�F�N�gX�̌����̃^�C�v
	D3DXVECTOR3 move = NULL_VECTOR3;
	float fWidth = 0.0f;
	float fHeight = 0.0f;
	FILE* pFile = fopen(SAVE_BIN, "wb");
	//===============================================================================================
	if (pFile != nullptr)
	{

		fwrite(&m_nIndexObject, sizeof(int),1, pFile);
		//==============================================
        //�u���b�N��X�I�u�W�F�N�g�̏��������o��
        //==============================================
		for (int nCnt = 0; nCnt < m_nIndexObject; nCnt++)
		{//�u���b�N��ݒ�
			if (m_apObjectList[nCnt] != nullptr)
			{
				nManagerType = ((CObjectX*)m_apObjectList[nCnt])->GetManagerType();  //X�I�u�W�F�N�g�̃^�C�v���擾
				nType = (int)(((CObjectX*)m_apObjectList[nCnt])->GetTypeNum());       //X�I�u�W�F�N�g���Ƃ̃^�C�v���擾
				nRotType = ((CObjectX*)m_apObjectList[nCnt])->GetRotType();                //�����̎�ނ��擾����
				Pos = ((CObjectX*)m_apObjectList[nCnt])->GetPos();                    //�ʒu�������o��
				Scale = ((CObjectX*)m_apObjectList[nCnt])->GetScale();				//�g�嗦�������o��
				Rot = ((CObjectX*)m_apObjectList[nCnt])->GetRot();					//�����������o��

				fwrite(&nManagerType, sizeof(int), 1, pFile);              //X�I�u�W�F�N�g�̃^�C�v�������o��
				fwrite(&nType, sizeof(int), 1, pFile);                  //X�I�u�W�F�N�g���Ƃ̃^�C�v�������o��
				fwrite(&nRotType, sizeof(int), 1, pFile);               //�����̎�ނ������o��
				fwrite(&Pos, sizeof(D3DXVECTOR3), 1, pFile);            //�ʒu�������o��
				fwrite(&Scale, sizeof(D3DXVECTOR3), 1, pFile);          //�g�嗦�������o��
				fwrite(&Rot, sizeof(D3DXVECTOR3), 1, pFile);            //�����������o��
			}
		}
		//========================================================================
		fclose(pFile);
	}
}
//======================================================================================================================

//==============================================
//�X�e�[�W�}�l�[�W���[��������
//==============================================
CStageManager* CStageManager::Create()
{
	CStageManager* pStageManager = DBG_NEW CStageManager;                                                                        //�v���C���[�𐶐�

	bool bSuccess = pStageManager->CObject::GetCreateSuccess();
	if (bSuccess == true)
	{//�󂫂��������̂Ő����J�n
		if (pStageManager != nullptr)
		{
			pStageManager->Init();                                                                                           //����������
			pStageManager->SetUseDeath(false);                                                                        //���S�t���O�𔭓����邩�ǂ�����ݒ肷��
			pStageManager->CObject::SetType(CObject::TYPE_STAGEMANAGER);                                                     //�I�u�W�F�N�g�̎�ނ����߂�   
			pStageManager->m_nType = 0;																	                     //�O�̈ʒu
			pStageManager->m_nManagerType = MANAGEROBJECT_BLOCK;																					 //�O�̈ʒu
		}
	}
	else
	{//�I�u�W�F�N�g�ɋ󂫂��Ȃ������̂Ŕj������
		delete pStageManager;
		pStageManager = nullptr;
	}

	return pStageManager;
}
//================================================================================

//============================================
//�X�e�[�W�}�l�[�W���[���ړ������鏈��
//============================================
void CStageManager::MoveManager()
{
	//=========================================
	//�ϐ��錾
	//=========================================
	
	float fSpeedX = float(rand() % 100 - 50) / 10;
	float fSpeedY = float(rand() % 100 - 50) / 10;
	float fSpeedZ = float(rand() % 100 - 50) / 10;

	float fRandRot = 0.0f;

	D3DXVECTOR3 MoveSize = ((CObjectX*)m_pManagerObject)->GetSize();//�ړ�����T�C�Y�����߂�

	CParticle::Create(CParticle::TYPE00_NORMAL, 45, 20.0f, 20.0f, m_WarpPos, D3DXVECTOR3(fSpeedX, fSpeedY, fSpeedZ), D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f),true);
	CEffect::Create(CEffect::EFFECTTYPE00_NORMAL,30, 20.0f, 20.0f, m_WarpPos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

	CInputKeyboard* pInputKeyboard = CManager::GetInputKeyboard();//�L�[�{�[�h�N���X�̎擾
	float fMoveX = 0.0f;                                          //X�����̈ړ���
	float fMoveZ = 0.0f;                                          //Z�����̈ړ���

	//=================================
	//�ړ��^�C�v��؂�ւ���
	//=================================
	if (pInputKeyboard->GetTrigger(DIK_MINUS) == true)
	{
		if (m_MoveMode == MOVEMODE00_XY)
		{
			m_MoveMode = MOVEMODE01_XZ;
		}
		else if (m_MoveMode == MOVEMODE01_XZ)
		{
			m_MoveMode = MOVEMODE00_XY;
		}
	}

	if (pInputKeyboard->GetTrigger(DIK_C) == true)
	{//�T�C�Y�����������ǂ���
		m_bUseSizeMove = m_bUseSizeMove ? false : true;
	}

	//==================================================================================================

	//=================================
	//�ʒu��߂�
	//=================================
	if (pInputKeyboard->GetTrigger(DIK_PERIOD))
	{
		m_Pos.z = 0.0f;
		m_Pos.y = 0.0f;
	}
	//==================================================================================================

	//=================================
	//�ړ����������߂�
	//=================================
	if (pInputKeyboard->GetPress(DIK_UP) == true)
	{
		fMoveZ = 1.0f;
	}
	else if (pInputKeyboard->GetPress(DIK_DOWN) == true)
	{
		fMoveZ = -1.0f;
	}

	if (pInputKeyboard->GetPress(DIK_RIGHT) == true)
	{
		fMoveX = 1.0f;
	}
	else if (pInputKeyboard->GetPress(DIK_LEFT) == true)
	{
		fMoveX = -1.0f;
	}
	//==================================================================================================

	//============================
	// �ړ��{�^���������Ă�����
	//============================
	if (fMoveX != 0.0f || fMoveZ != 0.0f)
	{
		m_bMove = true;//�ړ����
	}
	else
	{
		m_bMove = false;//�ҋ@���
		m_Move = NULL_VECTOR3;
	}


	if (m_bMove == true)
	{//�ړ����Ă�����
		if (m_bUseSizeMove == true)
		{
			m_Move = SizeMoveProcess(fMoveX, fMoveZ, MoveSize);
		}
		else
		{
			m_Move = NotSizeMoveProcess(fMoveX, fMoveZ);
		}
	}
	//==================================================================================================

	if (CManager::GetInputKeyboard()->GetPress(DIK_LCONTROL) == true)
	{
		//���[�v�ʒu�ړ�����
		m_WarpPos += m_Move;
		m_FocusType = FOCUSTYPE_WARPPOS;//���[�v��̈ʒu�ɏW������
	}
	else
	{   //�}�l�[�W���[�ړ�����
		m_Pos += m_Move;
		m_FocusType = FOCUSTYPE_NORMAL;//�I�u�W�F�N�g�̈ʒu�ɏW������
	    m_Pos.x = (float)(floor(m_Pos.x));
		m_Pos.y = (float)(floor(m_Pos.y));
		m_Pos.z = (float)(floor(m_Pos.z));

		fRandRot = CCalculation::CalculationRandVecXY();//�����x�N�g�����擾����

		//���݂̈ʒu�Ƀp�[�e�B�N������������
		CParticle::Create(CParticle::TYPE00_NORMAL, 30, 10.0f, 10.0f, m_Pos, D3DXVECTOR3(sinf(fRandRot) * 5.0f, cosf(fRandRot) * 5.0f, 0.0f), D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), true);

	    //�ʒu��ݒ肷��
	    ((CObjectX*)m_pManagerObject)->SetPos(m_Pos);
	}

}
//===========================================================================================

//=================================================
//�̗͂�ݒ肷��
//=================================================
void CStageManager::LifeSet()
{
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_F8) == true)
	{
		m_nSetLife += 10;
	}
	else if (CManager::GetInputKeyboard()->GetTrigger(DIK_F7) == true)
	{
		m_nSetLife -= 10;
	}
}
//===========================================================================================

//=================================================
//�I���u���b�N�̎�ނ�ς���
//=================================================
void CStageManager::TypeChenge()
{
	//=========================================
    //�ϐ��錾
    //=========================================

	//���ꂼ��̃I�u�W�F�N�g�^�C�v�̃t�@�C�������擾����
	int nNumFile[MANAGEROBJECT_MAX] = { CBlock::GetNumFile(),
		CModel::GetNumFile(),
	    CEnemy::GetNumFile(),
	    CItem::GetNumFile(),
	    CMarker::GetNumFile(),
	    CBoss::GetNumFile(),
	    CTrap::GetNumFile(),
	    CSignBoard::GetNumFile()};

	const int* pNumSubType = nullptr;//���ꂼ��̃I�u�W�F�N�g�̃T�u�^�C�v���擾����

	//=====================================================================
    //�I�u�W�F�N�gX�̎�ނ��ړ�����
    //=====================================================================
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_2) == true)
	{
		m_nManagerType++;
		m_nSetSubType = 0;//�T�u�^�C�v�ԍ������Z�b�g
		if (m_nManagerType >= (int)(MANAGEROBJECT_MAX))
		{
			m_nManagerType = 0;
		}
		m_nType = 0;//�I�u�W�F�N�g�ʃ^�C�v�ԍ����O�ɖ߂�
		ChengeObject();//�I�u�W�F�N�g�̎�ނ�ς���
	}
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_1) == true)
	{
		m_nManagerType--;
		m_nSetSubType = 0;//�T�u�^�C�v�ԍ������Z�b�g
		if (m_nManagerType < 0)
		{
			m_nManagerType = (int)(MANAGEROBJECT_MAX) - 1;
		}
		m_nType = 0;//�I�u�W�F�N�g�ʃ^�C�v�ԍ����O�ɖ߂�
		ChengeObject();//�I�u�W�F�N�g�̎�ނ�ς���
	}
	//=====================================================================================================================================

	//=====================================================================
	//�I�u�W�F�N�g���Ƃ̎�ނ��ړ�����
	//=====================================================================
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_0) == true)
	{
		m_nType++;
		if (m_nType >= nNumFile[m_nManagerType])
		{
			m_nType = 0;
		}
		m_nSetSubType = 0;//�T�u�^�C�v�ԍ������Z�b�g
		ChengeObject();//�I�u�W�F�N�g�̎�ނ�ς���
	}
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_9) == true)
	{
		m_nType--;
		m_nSetSubType = 0;//�T�u�^�C�v�ԍ������Z�b�g
		if (m_nType < 0)
		{
			m_nType = nNumFile[m_nManagerType] - 1;
		}
		ChengeObject();//�I�u�W�F�N�g�̎�ނ�ς���
	}
	//=====================================================================================================================================

	//=====================================================================
	//�T�u�^�C�v��ݒ肷��
	//=====================================================================

	//�T�u�^�C�v�擾
	switch (m_nManagerType)
	{
	case MANAGEROBJECT_BLOCK:
		pNumSubType = CBlock::GetSubTypeNum();
		break;
	case MANAGEROBJECT_TRAP:
		pNumSubType = CTrap::GetSubTypeNum();
		break;
	case MANAGEROBJECT_SIGNBOARD:
		pNumSubType = CSignBoard::GetSubTypeNum();
		break;
	default:
		pNumSubType = nullptr;
		m_nSetSubType = 0;//�T�u�^�C�v�ԍ������Z�b�g
		break;
	}

	if (pNumSubType != nullptr)
	{//�T�u�^�C�v��ݒ肷��
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_F9) == true)
		{
			m_nSetSubType++;
			if (m_nSetSubType >= pNumSubType[m_nType])
			{
				m_nSetSubType = 0;
			}
		}
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_F10) == true)
		{
			m_nSetSubType--;
			if (m_nSetSubType < 0)
			{
				m_nSetSubType = nNumFile[m_nType] - 1;
			}
		}
	}


	//=====================================================================================================================================

	//=====================================================================
	//�g�嗦��ς���
	//=====================================================================
	if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
	{//Y���k��
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_R) == true)
		{
			m_Scale.x -= 1.0f;
		}
	}
	else
	{//Y���g��
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_R) == true)
		{
			m_Scale.x += 1.0f;
		}
	}

	if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
	{//Y���k��
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_T) == true)
		{
			m_Scale.y -= 1.0f;
		}
	}
	else
	{//Y���g��
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_T) == true)
		{
			m_Scale.y += 1.0f;
		}
	}
	if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
	{//Z���k��
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_Y) == true)
		{
			m_Scale.z -= 1.0f;
		}
	}
	else
	{//Z���g��
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_Y) == true)
		{
			m_Scale.z += 1.0f;
		}
	}
	if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
	{//����k��
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_U) == true)
		{
			m_Scale -= ONE_VECTOR3;
		}
	}
	else
	{//����g��
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_U) == true)
		{
			m_Scale += ONE_VECTOR3;
		}
	}

	((CObjectX*)m_pManagerObject)->SetScale(m_Scale);
	//=====================================================================================================================================
	
	//======================================================================
	//�����̎�ނ�ς���
	//======================================================================
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_6) == true)
	{
		if (m_nRotType == (int)(CObjectX::ROTTYPE_NORMAL))
		{
			m_nRotType = (int)(CObjectX::ROTTYPE_Z);
		}
		else
		{
			m_nRotType = (int)(CObjectX::ROTTYPE_NORMAL);
		}
	}
	((CObjectX*)m_pManagerObject)->SetRotType((CObjectX::ROTTYPE)(m_nRotType));
    //=======================================================================================================================================

	//=======================================================================
	//���[�v��}�b�v�̃C���f�b�N�X��ς���
	//=======================================================================
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_F6) == true)
	{
		m_nWarpMapNum++;
		if (m_nWarpMapNum >= m_nMapNum)
		{
			m_nWarpMapNum = 0;
		}
	}
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_F5) == true)
	{
		m_nWarpMapNum--;
		if (m_nWarpMapNum < 0)
		{
			m_nWarpMapNum = m_nMapNum - 1;
		}
	}
	//=======================================================================================================================================

	//=======================================================================
	//�}�b�v�̃J�����̎�ނ�ς���
	//=======================================================================
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_F11) == true)
	{
		m_nMapCameraType++;
		if (m_nMapCameraType >= (int)(CCamera::CAMERATYPE_MAX))
		{
			m_nMapCameraType = 0;
		}
	}
	//=======================================================================================================================================

}
//===========================================================================================

//==================================================
//X�I�u�W�F�N�g�ݒ菈��
//==================================================
void CStageManager::SetObjectX()
{
	CObject* pObject = nullptr;
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_RETURN) == true)
	{
		switch (m_nManagerType)
		{
			case (int)(MANAGEROBJECT_BLOCK) :
		    pObject = CBlock::Create((CBlock::BLOCKTYPE)(m_nType),m_nSetLife, m_Pos, NULL_VECTOR3, m_Scale, (CObjectX::ROTTYPE)(m_nRotType), m_nSetSubType);
		    break;
		    case (int)(MANAGEROBJECT_MODEL) :
		    pObject = CModel::Create((CModel::MODELTYPE)(m_nType),m_Pos, NULL_VECTOR3,NULL_VECTOR3,m_Scale,(CObjectX::ROTTYPE)(m_nRotType));
		    break;
			case (int)(MANAGEROBJECT_ENEMY) :
			pObject = CEnemy::Create((CEnemy::ENEMYTYPE)(m_nType), m_nSetLife,m_Pos,NULL_VECTOR3,m_Scale, m_Pos,NULL_VECTOR3,ONE_VECTOR3);
			break;
			case (int)(MANAGEROBJECT_ITEM) :
			pObject = CItem::Create((CItem::ITEMTYPE)(m_nType), m_Pos, NULL_VECTOR3, m_Scale, (CObjectX::ROTTYPE)(m_nRotType));//��������
			break;
			case (int)(MANAGEROBJECT_MARKER) :
			pObject = CMarker::Create((CMarker::MARKERTYPE)(m_nType),m_Pos,m_Scale, m_WarpPos, m_nWarpMapNum);
			break;
			case (int)(MANAGEROBJECT_BOSS) :
			pObject = CBoss::SetCreateBoss((CBoss::BOSSTYPE)(m_nType),m_nSetLife, m_Pos, m_Scale);
			break;
			case (int)(MANAGEROBJECT_TRAP) :
			pObject = CTrap::Create((CTrap::TRAPTYPE)(m_nType), m_Pos, NULL_VECTOR3, m_Scale, CObjectX::ROTTYPE_NORMAL,m_nSetSubType);//��������
			break;
			case (int)(MANAGEROBJECT_SIGNBOARD) :
				pObject = CSignBoard::Create(CSignBoard::SIGNBOARDTYPE(m_nType), CSignBoard::SIGNBOARDMANUALTYPE(m_nSetSubType),
					m_Pos, m_Scale, NULL_VECTOR3);
			break;
		    default:
		    break;
		}
		//if (pObject != nullptr && m_nIndexObject < m_nMAXOBJECT)
		//{//�I�u�W�F�N�g�������X�g�ɕۑ�
		//	m_apObjectList[m_nIndexObject] = pObject;

		pObject->SetStageManagerObj();//�X�e�[�W�}�l�[�W���[�Ǘ��I�u�W�F�N�g�ɐݒ肷��
		m_nIndexObject++;//�C���f�b�N�X�𑝂₷

		//}

	}

}
//===========================================================================================

//==================================================
//X�I�u�W�F�N�g��������
//==================================================
void CStageManager::DeleteObjectX()
{
	//if (CManager::GetInputKeyboard()->GetTrigger(DIK_DELETE) == true)
	//{
	//	if (m_apObjectList[m_nIndexObject - 1] != nullptr && m_nIndexObject >= 1)
	//	{
	//		if (((CObjectX*)m_apObjectList[m_nIndexObject - 1])->GetObjXType() == CObjectX::OBJECTXTYPE_MARKER)
	//		{
	//			((CMarker*)m_apObjectList[m_nIndexObject - 1])->DeleteNum();
	//		}
	//		m_apObjectList[m_nIndexObject - 1]->SetDeath();    //�u���b�N���X�g�̓��I�m�ۃ��������J��
	//		m_apObjectList[m_nIndexObject - 1] = nullptr;          //�u���b�N���X�g�̃|�C���^��������
	//		m_nIndexObject--;                                     //�C���f�b�N�X���f�N�������g
	//	}
	//}
}
//=======================================================================================================================

//====================================================
//�ݒu�I�u�W�F�N�g�̎�ނ�ς���
//====================================================
void CStageManager::ChengeObject()
{
	//�I�u�W�F�N�g�������[�X����
	ReleaseObject();

	switch (m_nManagerType)
	{
		case (int)(MANAGEROBJECT_BLOCK) :
			m_pManagerObject = CBlock::Create((CBlock::BLOCKTYPE)(m_nType), 10, m_Pos, NULL_VECTOR3, m_Scale, (CObjectX::ROTTYPE)(m_nRotType), m_nSetSubType);
			break;
			case (int)(MANAGEROBJECT_MODEL) :
				m_pManagerObject = CModel::Create((CModel::MODELTYPE)(m_nType), m_Pos, NULL_VECTOR3, NULL_VECTOR3, m_Scale, (CObjectX::ROTTYPE)(m_nRotType));
				break;
				case (int)(MANAGEROBJECT_ENEMY) :
					m_pManagerObject = CEnemy::Create((CEnemy::ENEMYTYPE)(m_nType), 10, m_Pos, NULL_VECTOR3, m_Scale, m_Pos, NULL_VECTOR3, ONE_VECTOR3);
					break;
					case (int)(MANAGEROBJECT_ITEM) :
						m_pManagerObject = CItem::Create((CItem::ITEMTYPE)(m_nType), m_Pos, NULL_VECTOR3, m_Scale, (CObjectX::ROTTYPE)(m_nRotType));//��������
						break;
						case (int)(MANAGEROBJECT_MARKER) :
							m_pManagerObject = CMarker::Create((CMarker::MARKERTYPE)(m_nType), m_Pos, m_Scale, NULL_VECTOR3, 0);
							break;
							case (int)(MANAGEROBJECT_BOSS) :
								m_pManagerObject = CBoss::SetCreateBoss((CBoss::BOSSTYPE)(m_nType), m_nSetLife, m_Pos, m_Scale);
								break;
								case (int)(MANAGEROBJECT_TRAP) :
									m_pManagerObject = CTrap::Create((CTrap::TRAPTYPE)(m_nType), m_Pos, NULL_VECTOR3, m_Scale, CObjectX::ROTTYPE_NORMAL, m_nSetSubType);//��������
									break;
									case (int)(MANAGEROBJECT_SIGNBOARD) :
										m_pManagerObject = CSignBoard::Create(CSignBoard::SIGNBOARDTYPE(m_nType), CSignBoard::SIGNBOARDMANUALTYPE(m_nSetSubType),
											m_Pos, m_Scale, NULL_VECTOR3);
										break;

									default:
										break;
	}
	if (m_pManagerObject != nullptr)
	{
		m_pManagerObject->SetUseDeath(false);
	}
}
//=======================================================================================================================

//==================================================================
//�I�u�W�F�N�g�������[�X����
//==================================================================
void CStageManager::ReleaseObject()
{
	if (m_pManagerObject != nullptr)
	{
		m_pManagerObject->SetUseDeath(true);
		m_pManagerObject->SetDeath();
		m_pManagerObject = nullptr;
	}

}

//================================================================
//�F������ݒ肷��
//================================================================
void CStageManager::ColorChenge()
{

}
//=======================================================================================================================

//===========================================================
//�}�b�v��ς���
//===========================================================
void CStageManager::MapChenge()
{
	//===============================================================
	//���[�h��ς���
	//===============================================================
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_F4) == true)
	{
		SaveMapTxt(m_nMapIndex);//���݂̃}�b�v��ۑ�����

		if (m_ManagerMode == MANAGERMODE_ALREADYSTAGE)
		{
			m_ManagerMode = MANAGERMODE_NEWSTAGE;
		}
		else
		{
			m_ManagerMode = MANAGERMODE_ALREADYSTAGE;
		}

		LoadMapTxt(m_nMapIndex);
	}
	//=======================================================================================================

	if (CManager::GetInputKeyboard()->GetTrigger(DIK_F3) == true && m_ManagerMode == MANAGERMODE_ALREADYSTAGE)
	{
		SaveMapTxt(m_nMapIndex);//���݂̃}�b�v�����Z�[�u����
		m_nMapIndex++;
		if (m_nMapIndex >= m_nMapNum)
		{
			m_nMapIndex = 0;
		}

		LoadMapTxt(m_nMapIndex);//�ԍ��̃}�b�v��ǂݍ���
	}
	else if (CManager::GetInputKeyboard()->GetTrigger(DIK_F2) == true && m_ManagerMode == MANAGERMODE_ALREADYSTAGE)
	{
		SaveMapTxt(m_nMapIndex);//���݂̃}�b�v�����Z�[�u����
		m_nMapIndex--;
		if (m_nMapIndex < 0)
		{
			m_nMapIndex = m_nMapNum - 1;
		}
		LoadMapTxt(m_nMapIndex);//�ԍ��̃}�b�v��ǂݍ���
	}

	if (CManager::GetInputKeyboard()->GetTrigger(DIK_P) == true && m_ManagerMode == MANAGERMODE_ALREADYSTAGE)
	{
		SaveMapTxt(m_nMapIndex);//���݂̃}�b�v�����Z�[�u����

		m_nMapIndex = 0;//���[���h��ύX����̂Ń}�b�v�ԍ���������
		m_nMapNum = 0;  //�}�b�v������������

		m_nWorldIndex++;//���̃��[���h��I��

		if (m_nWorldIndex == WORLDTYPE_MAX)
		{
			m_nWorldIndex = 0;
		}

		//�}�b�v�������[�h����
		LoadMapFilePass(WORLDTYPE(m_nWorldIndex));

		//�}�b�v��ǂݍ���
		LoadMapTxt(m_nMapIndex);//�ԍ��̃}�b�v��ǂݍ���
	}

}
//=======================================================================================================================

//===========================================================
//���\������
//===========================================================
void CStageManager::DispInfo()
{
	char aChooseString[100] = {};//�I����Ԃ��ǂ������L�q
	char aCameraTypeName[CCamera::CAMERATYPE_MAX][100] = {};
	char aMapModeString[100] = {};
	//�}�b�v�̃J�������[�h��
	strcpy(&aCameraTypeName[CCamera::CAMERATYPE_BIRD][0], "�T�C�h�r���[");
	strcpy(&aCameraTypeName[CCamera::CAMERATYPE_BESIDE][0], "�r�T�C�h�r���[");
	strcpy(&aCameraTypeName[CCamera::CAMERATYPE_SIDE][0], "�T�C�h�r���[");
	strcpy(&aCameraTypeName[CCamera::CAMERATYPE_BOSSBATTLE][0], "�{�X�o�g��");
	strcpy(&aCameraTypeName[CCamera::CAMERATYPE_BOSSDEFEAT][0], "�{�X���j");


	if (m_bChooseObject == false)
	{
		strcpy(&aChooseString[0], "�I�����Ă��Ȃ�");
	}
	else
	{
		strcpy(&aChooseString[0], "�I�𒆁E�E�E");
	}

	if (m_ManagerMode == MANAGERMODE_ALREADYSTAGE)
	{
		strcpy(&aMapModeString[0], "�����̃X�e�[�W��ҏW");
	}
	else
	{
		strcpy(&aMapModeString[0], "�V�����X�e�[�W��ҏW");
	}

	CManager::GetDebugText()->PrintDebugText("//=================================\n");
	CManager::GetDebugText()->PrintDebugText("//�}�b�v�G�f�B�^�̏��\n");
	CManager::GetDebugText()->PrintDebugText("//=================================\n");
	CManager::GetDebugText()->PrintDebugText("�X�e�[�W�}�l�[�W���[�Ǘ��I�u�W�F�N�g�̑����F%d\n",GetTotalStageManagerObjNum());
	CManager::GetDebugText()->PrintDebugText("���݂̃��[���h�F%s\n",&m_apWORLDMAP_TXT[m_nWorldIndex][0]);
	CManager::GetDebugText()->PrintDebugText("���݂̃}�b�v�ԍ�(F2�AF3�ŕύX�j�F%d\n", m_nMapIndex);
	CManager::GetDebugText()->PrintDebugText("���݂̃}�b�v�t�@�C���p�X�F%s\n",&m_aMapFilePass[m_nMapIndex][0]);
	CManager::GetDebugText()->PrintDebugText("���݂̃��[�v��iF5�AF6�ŕύX�j�F%d\n", m_nWarpMapNum);
	CManager::GetDebugText()->PrintDebugText("���݂̗̑́iF7�AF8�ŕύX�j�F%d\n", m_nSetLife);
	CManager::GetDebugText()->PrintDebugText("���݂̃I�u�W�F�N�g�̃T�u�^�C�v�iF9�AF10�ŕύX�j�F%d\n", m_nSetSubType);
	CManager::GetDebugText()->PrintDebugText("�I�u�W�F�N�g�̈ʒu�F %f %f %f\n",m_Pos.x,m_Pos.y,m_Pos.z);
	CManager::GetDebugText()->PrintDebugText("�I�u�W�F�N�g�̊g�嗦���Z�b�g�FV\n");
	CManager::GetDebugText()->PrintDebugText("���݂̃��[�v��̈ʒu�F %f %f %f\n",m_WarpPos.x, m_WarpPos.y, m_WarpPos.z);
	CManager::GetDebugText()->PrintDebugText("���݂̃}�b�v���[�h�iF4�j�F%s\n",&aMapModeString[0]);
	CManager::GetDebugText()->PrintDebugText("�I�u�W�F�N�g��I���iL�j�F%s\n",&aChooseString[0]);
	CManager::GetDebugText()->PrintDebugText("�I���I�u�W�F�N�g�ύX�F�iN�AM�j\n");
	CManager::GetDebugText()->PrintDebugText("�I���I�u�W�F�N�g�����F�iDEL�j\n");
	CManager::GetDebugText()->PrintDebugText("�ړ����@��ύX�FALT�L�[\n");
	CManager::GetDebugText()->PrintDebugText("�ړ������ύX�F�i�}�C�i�X�L�[�j\n");
	CManager::GetDebugText()->PrintDebugText("Z���W�����_�ɖ߂��F�i�s���I�h�L�[�j\n");
	CManager::GetDebugText()->PrintDebugText("�S�Ǘ��I�u�W�F�N�g�����F�iLSHIFT�L�[�j\n");
	CManager::GetDebugText()->PrintDebugText("//=================================\n");

}
//=======================================================================================================================

//===========================================================
//�I�u�W�F�N�g�I������
//===========================================================
void CStageManager::ChooseObject()
{
	bool bChoose = false;                              //�I�u�W�F�N�g��I�����Ă��邩�ǂ���
	int nChooseIdxObject = 0;                          //���ݑI�����Ă���I�u�W�F�N�g�̔ԍ�

	if (GetTotalStageManagerObjNum() <= 0)
	{//�X�e�[�W�}�l�[�W���[�Ǘ��I�u�W�F�N�g���Ȃ��Ȃ����u�ԂɑI����Ԃ���������B
		m_bChooseObject = false;
		m_pChooseObject = nullptr;
	}

	if (CManager::GetInputKeyboard()->GetTrigger(DIK_L) == true && GetTotalStageManagerObjNum() > 0)
	{
		m_bChooseObject = m_bChooseObject ? false : true;

		if (m_bChooseObject == false)
		{
			m_pChooseObject->SetStageManagerChoose(false);     //�I�����Ă���I�u�W�F�N�g�̑I����Ԃ�����
			m_Pos = m_SaveBeforeChoosePos;                     //�ۑ����Ă����ʒu�ɖ߂�
		}
		else
		{
			m_SaveBeforeChoosePos = m_Pos;                     //�I����Ԃ𔭓�����O�̃I�u�W�F�N�g�̈ʒu���L������
			m_pChooseObject = GetCurStageManagerObject();      //�ŐV�̃I�u�W�F�N�g��I��
			m_pChooseObject->SetStageManagerChoose(false);     //�I�����Ă���I�u�W�F�N�g�̑I����Ԃ�����
		}
	}
	
	if (m_pChooseObject != nullptr)
	{//�I���I�u�W�F�N�g�����݂��Ă�����
		if (m_bChooseObject == true)
		{
			//�I���I�u�W�F�N�g�ύX
			if (CManager::GetInputKeyboard()->GetTrigger(DIK_N) == true)
			{
				m_pChooseObject->SetStageManagerChoose(false);//���ݑI��ł���I�u�W�F�N�g�̑I����Ԃ�����

				if (m_pChooseObject->GetPrevStageManagerObject() != nullptr)
				{//�O�̃I�u�W�F�N�g����������
					m_pChooseObject = m_pChooseObject->GetPrevStageManagerObject();//�O�̃I�u�W�F�N�g��I��
				}
				else
				{//�O�̃I�u�W�F�N�g���Ȃ�
					m_pChooseObject = GetCurStageManagerObject();//�ŐV�I�u�W�F�N�g��I��
				}
			}
			else if (CManager::GetInputKeyboard()->GetTrigger(DIK_M) == true)
			{
				m_pChooseObject->SetStageManagerChoose(false);//���ݑI��ł���I�u�W�F�N�g�̑I����Ԃ�����

				if (m_pChooseObject->GetNextStageManagerObject() != nullptr)
				{//���̃I�u�W�F�N�g������
					m_pChooseObject = m_pChooseObject->GetNextStageManagerObject();//���̃I�u�W�F�N�g��I��
				}
				else
				{//���̃I�u�W�F�N�g���Ȃ�
					m_pChooseObject = GetTopStageManagerObject();//�g�b�v�I�u�W�F�N�g��I��
				}
			}

			if (m_pChooseObject != nullptr && m_nIndexObject >= 1)
			{//�I�u�W�F�N�g��I����Ԃɂ���
				m_Pos = ((CObjectX*)m_pChooseObject)->GetPos();//�I���I�u�W�F�N�g�̈ʒu�Ɉړ�����
				m_pChooseObject->SetStageManagerChoose(true);
			}

			if (CManager::GetInputKeyboard()->GetTrigger(DIK_DELETE) == true)
			{//�I���I�u�W�F�N�g��j������
				m_pChooseObject->SetUseDeath(true);
				m_pChooseObject->SetDeath();

				//�I���I�u�W�F�N�g��ύX
				if (m_pChooseObject->GetNextStageManagerObject() != nullptr)
				{//���̃I�u�W�F�N�g������
					m_pChooseObject = m_pChooseObject->GetNextStageManagerObject();//���̃I�u�W�F�N�g��I��
				}
				else
				{//���̃I�u�W�F�N�g���Ȃ�
					m_pChooseObject = GetTopStageManagerObject();//�g�b�v�I�u�W�F�N�g��I��
				}
			}
		}
	}
}
//=======================================================================================================================

//===========================================================
//�g���b�v���e�L�X�g�t�@�C���ɕۑ�����
//===========================================================
void CStageManager::SaveTrapTxt(FILE* pFile, CObject* pObj)
{
	D3DXVECTOR3 Pos = NULL_VECTOR3;
	D3DXVECTOR3 WarpPos = NULL_VECTOR3;
	D3DXVECTOR3 Scale = NULL_VECTOR3;
	D3DXVECTOR3 Rot = NULL_VECTOR3;
	int nType = ((CObjectX*)pObj)->GetTypeNum();          //X�I�u�W�F�N�g���Ƃ̃^�C�v���擾
	int nSubType = ((CTrap*)pObj)->GetSubType();          //�T�u�^�C�v

	float& fSpeed = ((CTrap*)pObj)->GetSpeed();           //����
	float& fRotSpeed = ((CTrap*)pObj)->GetRotSpeed();     //��]���x
	float& fStartRot = ((CTrap*)pObj)->GetStartRot();     //��]�J�n����
	float& fMoveLength = ((CTrap*)pObj)->GetMoveLength(); //�ړ�����
	float& fTurnSpeed = ((CTrap*)pObj)->GetTurnSpeed();   //U�^�[���J�n���x
	int& nInterval = ((CTrap*)pObj)->GetInterval();       //�l�X�ȃC���^�[�o��
	float& fAddSpeed = ((CTrap*)pObj)->GetAddSpeed();     //�����x���擾
	Pos = ((CObjectX*)pObj)->GetSupportPos();                    //�ʒu�������o��
	Scale = ((CObjectX*)pObj)->GetScale();				//�g�嗦�������o��
	Rot = ((CObjectX*)pObj)->GetRot();					//�����������o��

	fprintf(pFile, "SETTRAP\n");
	fprintf(pFile, "TYPE = %d\n", nType);
	fprintf(pFile, "SUBTYPE = %d\n", nSubType);
	fprintf(pFile, "POS = %.3f %.3f %.3f\n", Pos.x, Pos.y, Pos.z);
	fprintf(pFile, "SCALE = %.3f %.3f %.3f\n", Scale.x, Scale.y, Scale.z);
	fprintf(pFile, "ROT = %.3f %.3f %.3f\n", Rot.x, Rot.y, Rot.z);
	fprintf(pFile, "SPEED = %.3f\n",fSpeed);
	fprintf(pFile, "ROTSPEED = %.3f\n",fRotSpeed);
	fprintf(pFile, "STARTROT = %.3f\n",fStartRot);
	fprintf(pFile, "MOVELENGTH = %.3f\n",fMoveLength);
	fprintf(pFile, "TURNSPEED = %.3f\n",fTurnSpeed);
	fprintf(pFile, "INTERVAL = %d\n", nInterval);
	fprintf(pFile, "ADDSPEED = %f\n", fAddSpeed);
	fprintf(pFile, "END_SETTRAP\n\n");

}
//=======================================================================================================================

//===========================================================
//�u���b�N���e�L�X�g�t�@�C���ɕۑ�����
//===========================================================
void CStageManager::SaveBlockTxt(FILE* pFile, CObject* pObj)
{
	D3DXVECTOR3 Pos = ((CObjectX*)pObj)->GetSupportPos(); //�x�_
	D3DXVECTOR3 Scale = ((CObjectX*)pObj)->GetScale();    //�g�嗦
	D3DXVECTOR3 Rot = ((CObjectX*)pObj)->GetRot();        //����
	int nType = ((CObjectX*)pObj)->GetTypeNum();          //X�I�u�W�F�N�g���Ƃ̃^�C�v���擾
	int nSubType = ((CBlock*)pObj)->GetSubType();         //�T�u�^�C�v

	float fStartRot = ((CBlock*)pObj)->GetStartRot();     //��]�J�n�ʒu
	float fRotSpeed = ((CBlock*)pObj)->GetRotSpeed();     //��]���x
	float fMoveLength = ((CBlock*)pObj)->GetMoveLength(); //�ړ�����
	float fSpeed = ((CBlock*)pObj)->GetSpeed();           //�ړ����x
	int nBlinkingSpeed = ((CBlock*)pObj)->GetBlinkSpeed();//�_�ő��x
	float fAddSpeed = ((CBlock*)pObj)->GetAddSpeed();     //�����x
	float fJumpPower = ((CBlock*)pObj)->GetJumpPower();   //�W�����v��

	fprintf(pFile, "SETBLOCK\n");
	fprintf(pFile, "TYPE = %d\n", nType);
	fprintf(pFile, "SUBTYPE = %d\n", nSubType);
	fprintf(pFile, "POS = %.3f %.3f %.3f\n", Pos.x, Pos.y, Pos.z);
	fprintf(pFile, "SCALE = %.3f %.3f %.3f\n", Scale.x, Scale.y, Scale.z);
	fprintf(pFile, "ROT = %.3f %.3f %.3f\n", Rot.x, Rot.y, Rot.z);

	fprintf(pFile, "STARTROT = %.3f\n", fStartRot);
	fprintf(pFile, "ROTSPEED = %.3f\n", fRotSpeed);
	fprintf(pFile, "MOVELENGTH = %.3f\n", fMoveLength);
	fprintf(pFile, "SPEED = %.3f\n", fSpeed);
	fprintf(pFile, "BLINKINGSPEED = %d\n", nBlinkingSpeed);
	fprintf(pFile, "ADDSPEED = %.3f\n", fAddSpeed);
	fprintf(pFile, "JUMPPOWER = %.3f\n", fJumpPower);
	fprintf(pFile, "END_SETBLOCK\n\n");

}
//=======================================================================================================================

//===========================================================
//�G�̏����e�L�X�g�t�@�C���ɏ����o��
//===========================================================
void CStageManager::SaveEnemyTxt(FILE* pFile, CObject* pObj)
{
	D3DXVECTOR3 Pos = ((CObjectX*)pObj)->GetSupportPos();           //�ʒu;
	D3DXVECTOR3 Scale = ((CObjectX*)pObj)->GetScale();				//�g�嗦;
	D3DXVECTOR3 Rot = ((CObjectX*)pObj)->GetRot();					//����;
	D3DXVECTOR3 MoveRangeAIPos = ((CEnemy*)pObj)->GetEnemyMoveRangeAIModel()->GetPos();//�ړ��͈�AI�̈ʒu
	D3DXVECTOR3 MoveRangeAIScale = ((CEnemy*)pObj)->GetEnemyMoveRangeAIModel()->GetScale();//�ړ��͈�AI�̊g�嗦
	int nType = ((CObjectX*)pObj)->GetTypeNum();        //X�I�u�W�F�N�g���Ƃ̃^�C�v���擾
	int nLife = ((CObjectX*)pObj)->GetLife();           //�̗�


	fprintf(pFile, "SETENEMY\n");
	fprintf(pFile, "TYPE = %d\n", nType);
	fprintf(pFile, "LIFE = %d\n", nLife);
	fprintf(pFile, "POS = %.3f %.3f %.3f\n", Pos.x, Pos.y, Pos.z);
	fprintf(pFile, "SCALE = %.3f %.3f %.3f\n", Scale.x, Scale.y, Scale.z);
	fprintf(pFile, "ROT = %.3f %.3f %.3f\n", Rot.x, Rot.y, Rot.z);
	fprintf(pFile, "MOVERANGEAIPOS = %.3f %.3f %.3f\n", MoveRangeAIPos.x, MoveRangeAIPos.y, MoveRangeAIPos.z);
	fprintf(pFile, "MOVERANGEAISCALE = %.3f %.3f %.3f\n", MoveRangeAIScale.x, MoveRangeAIScale.y, MoveRangeAIScale.z);
	fprintf(pFile, "END_SETENEMY\n\n");
}
//=======================================================================================================================

//===========================================================
//�g���b�v�̏����e�L�X�g�t�@�C������ǂݍ���
//===========================================================
void CStageManager::LoadTrapTxt(FILE* pFile)
{
	D3DXVECTOR3 Pos = NULL_VECTOR3;   //�ʒu
	D3DXVECTOR3 Scale = NULL_VECTOR3; //�g�嗦
	D3DXVECTOR3 Rot = NULL_VECTOR3;   //����
	int nType = 0;                    //X�I�u�W�F�N�g���Ƃ̃^�C�v���擾
	int nSubType = 0;                 //�T�u�^�C�v
	float fSpeed = 0.0f;              //����
	float fRotSpeed = 0.0f;           //��]���x
	float fStartRot = 0.0f;           //��]�J�n����
	float fMoveLength = 0.0f;         //�ړ�����
	float fTurnSpeed = 0.0f;          //U�^�[���J�n���x
	float fAddSpeed = 0.0f;           //�����x
	int nInterval = 0;                //�C���^�[�o��
	char aString[128] = {};           //������
	CObject* pObject = nullptr;       //�I�u�W�F�N�g�ւ̃|�C���^

	while (1)
	{
		fscanf(pFile, "%s", &aString[0]);
		if (aString[0] == '#')
		{
			fgets(&aString[0], 100, pFile);//�s���X�L�b�v
		}
		else if (strcmp(&aString[0], "TYPE") == 0)
		{
			fscanf(pFile, "%s", &aString[0]);  //�C�R�[���ǂݎ��
			fscanf(pFile, "%d", &nType);       //�I�u�W�F�N�g���Ƃ̃^�C�v�擾
		}
		else if (strcmp(&aString[0], "SUBTYPE") == 0)
		{
			fscanf(pFile, "%s", &aString[0]);  //�C�R�[���ǂݎ��
			fscanf(pFile, "%d", &nSubType);    //�T�u�^�C�v
		}
		else if (strcmp(&aString[0], "POS") == 0)
		{
			fscanf(pFile, "%s", &aString[0]);  //�C�R�[���ǂݎ��
			fscanf(pFile, "%f", &Pos.x);       //X���W�ǂݎ��
			fscanf(pFile, "%f", &Pos.y);       //Y���W�ǂݎ��
			fscanf(pFile, "%f", &Pos.z);       //Z���W�ǂݎ��
		}
		else if (strcmp(&aString[0], "SCALE") == 0)
		{
			fscanf(pFile, "%s", &aString[0]);  //�C�R�[���ǂݎ��
			fscanf(pFile, "%f", &Scale.x);       //X���W�ǂݎ��
			fscanf(pFile, "%f", &Scale.y);       //Y���W�ǂݎ��
			fscanf(pFile, "%f", &Scale.z);       //Z���W�ǂݎ��
		}
		else if (strcmp(&aString[0], "ROT") == 0)
		{
			fscanf(pFile, "%s", &aString[0]);  //�C�R�[���ǂݎ��
			fscanf(pFile, "%f", &Rot.x);       //X���W�ǂݎ��
			fscanf(pFile, "%f", &Rot.y);       //Y���W�ǂݎ��
			fscanf(pFile, "%f", &Rot.z);       //Z���W�ǂݎ��
		}
		else if (strcmp(&aString[0], "SPEED") == 0)
		{
			fscanf(pFile, "%s", &aString[0]);  //�C�R�[���ǂݎ��
			fscanf(pFile, "%f", &fSpeed);      //�ړ����x�ǂݎ��
		}
		else if (strcmp(&aString[0], "ROTSPEED") == 0)
		{
			fscanf(pFile, "%s", &aString[0]);  //�C�R�[���ǂݎ��
			fscanf(pFile, "%f", &fRotSpeed);   //��]���x�ǂݎ��
		}
		else if (strcmp(&aString[0], "STARTROT") == 0)
		{
			fscanf(pFile, "%s", &aString[0]);  //�C�R�[���ǂݎ��
			fscanf(pFile, "%f", &fStartRot);   //��]�J�n�����ǂݎ��
		}
		else if (strcmp(&aString[0], "MOVELENGTH") == 0)
		{
			fscanf(pFile, "%s", &aString[0]);  //�C�R�[���ǂݎ��
			fscanf(pFile, "%f", &fMoveLength); //�ړ������ǂݎ��
		}
		else if (strcmp(&aString[0], "TURNSPEED") == 0)
		{
			fscanf(pFile, "%s", &aString[0]);  //�C�R�[���ǂݎ��
			fscanf(pFile, "%f", &fTurnSpeed);  //U�^�[���J�n���x�ǂݎ��
		}
		else if (strcmp(&aString[0], "INTERVAL") == 0)
		{
			fscanf(pFile, "%s", &aString[0]);  //�C�R�[���ǂݎ��
			fscanf(pFile, "%d", &nInterval);   //�C���^�[�o���ǂݎ��
		}
		else if (strcmp(&aString[0], "ADDSPEED") == 0)
		{
			fscanf(pFile, "%s", &aString[0]);  //�C�R�[���ǂݎ��
			fscanf(pFile, "%f", &fAddSpeed);   //�C���^�[�o���ǂݎ��
		}
		else if (strcmp(&aString[0], "END_SETTRAP") == 0)
		{//X�I�u�W�F�N�g��ݒ�
			if (CScene::GetMode() == CScene::MODE_EDIT)
			{//�G�f�B�b�g���[�h�Ȃ�A�I�u�W�F�N�g�����X�g�ɕۑ����Ǘ�����
				pObject = CTrap::Create((CTrap::TRAPTYPE)(nType), Pos, Rot, Scale, 0, nSubType);
				((CTrap*)pObject)->SetInfo(fStartRot, fSpeed, fRotSpeed, fMoveLength, fTurnSpeed,nInterval,fAddSpeed);//���ݒ�
				if (pObject != nullptr)
				{
					pObject->SetStageManagerObj();                                                 //�X�e�[�W�}�l�[�W���[�ŊǗ�����I�u�W�F�N�g�ɐݒ�	
					m_nIndexObject++;
				}
			}
			else if (CScene::GetMode() == CScene::MODE_GAME)
			{//�Q�[�����[�h�Ȃ�A�I�u�W�F�N�g�𐶐����邾���ŁA���X�g�ɕۑ����Ȃ�
				pObject = CTrap::Create((CTrap::TRAPTYPE)(nType), Pos, Rot, Scale, 0, nSubType);
				((CTrap*)pObject)->SetInfo(fStartRot, fSpeed, fRotSpeed, fMoveLength, fTurnSpeed,nInterval, fAddSpeed);//���ݒ�
			}
			break;
		}
	}

}
//=======================================================================================================================

//===========================================================
//�g���b�v�̏����e�L�X�g�t�@�C������ǂݍ���
//===========================================================
void CStageManager::LoadBlockTxt(FILE* pFile)
{
	D3DXVECTOR3 Pos = NULL_VECTOR3;  //�x�_
	D3DXVECTOR3 Scale = NULL_VECTOR3;//�g�嗦
	D3DXVECTOR3 Rot = NULL_VECTOR3;  //����
	int nType = 0;                   //X�I�u�W�F�N�g���Ƃ̃^�C�v���擾
	int nSubType = 0;                //�T�u�^�C�v

	float fStartRot = 0.0f;          //��]�J�n�ʒu
	float fRotSpeed = 0.0f;          //��]���x
	float fMoveLength = 0.0f;        //�ړ�����
	float fSpeed = 0.0f;             //�ړ����x
	int nBlinkingSpeed = 0;          //�_�ő��x
	float fAddSpeed = 0.0f;          //�����x
	float fJumpPower = 0.0f;         //�W�����v��

	char aString[128] = {};           //������
	CObject* pObject = nullptr;       //�I�u�W�F�N�g�ւ̃|�C���^

	while (1)
	{
		fscanf(pFile, "%s", &aString[0]);
		if (aString[0] == '#')
		{
			fgets(&aString[0], 100, pFile);//�s���X�L�b�v
		}
		else if (strcmp(&aString[0], "TYPE") == 0)
		{
			fscanf(pFile, "%s", &aString[0]);  //�C�R�[���ǂݎ��
			fscanf(pFile, "%d", &nType);       //�I�u�W�F�N�g���Ƃ̃^�C�v�擾
		}
		else if (strcmp(&aString[0], "SUBTYPE") == 0)
		{
			fscanf(pFile, "%s", &aString[0]);  //�C�R�[���ǂݎ��
			fscanf(pFile, "%d", &nSubType);    //�T�u�^�C�v
		}
		else if (strcmp(&aString[0], "POS") == 0)
		{
			fscanf(pFile, "%s", &aString[0]);  //�C�R�[���ǂݎ��
			fscanf(pFile, "%f", &Pos.x);       //X���W�ǂݎ��
			fscanf(pFile, "%f", &Pos.y);       //Y���W�ǂݎ��
			fscanf(pFile, "%f", &Pos.z);       //Z���W�ǂݎ��
		}
		else if (strcmp(&aString[0], "SCALE") == 0)
		{
			fscanf(pFile, "%s", &aString[0]);  //�C�R�[���ǂݎ��
			fscanf(pFile, "%f", &Scale.x);       //X���W�ǂݎ��
			fscanf(pFile, "%f", &Scale.y);       //Y���W�ǂݎ��
			fscanf(pFile, "%f", &Scale.z);       //Z���W�ǂݎ��
		}
		else if (strcmp(&aString[0], "ROT") == 0)
		{
			fscanf(pFile, "%s", &aString[0]);  //�C�R�[���ǂݎ��
			fscanf(pFile, "%f", &Rot.x);       //X���W�ǂݎ��
			fscanf(pFile, "%f", &Rot.y);       //Y���W�ǂݎ��
			fscanf(pFile, "%f", &Rot.z);       //Z���W�ǂݎ��
		}
		else if (strcmp(&aString[0], "STARTROT") == 0)
		{
			fscanf(pFile, "%s", &aString[0]);  //�C�R�[���ǂݎ��
			fscanf(pFile, "%f", &fStartRot);   //��]�J�n�����ǂݎ��
		}
		else if (strcmp(&aString[0], "ROTSPEED") == 0)
		{
			fscanf(pFile, "%s", &aString[0]);  //�C�R�[���ǂݎ��
			fscanf(pFile, "%f", &fRotSpeed);   //��]���x�ǂݎ��
		}
		else if (strcmp(&aString[0], "MOVELENGTH") == 0)
		{
			fscanf(pFile, "%s", &aString[0]);  //�C�R�[���ǂݎ��
			fscanf(pFile, "%f", &fMoveLength); //�ړ������ǂݎ��
		}
		else if (strcmp(&aString[0], "SPEED") == 0)
		{
			fscanf(pFile, "%s", &aString[0]);  //�C�R�[���ǂݎ��
			fscanf(pFile, "%f", &fSpeed);      //�ړ����x�ǂݎ��
		}
		else if (strcmp(&aString[0], "BLINKINGSPEED") == 0)
		{
			fscanf(pFile, "%s", &aString[0]);    //�C�R�[���ǂݎ��
			fscanf(pFile, "%d", &nBlinkingSpeed);//�_�ő��x�ǂݎ��
		}
		else if (strcmp(&aString[0], "ADDSPEED") == 0)
		{
			fscanf(pFile, "%s", &aString[0]);    //�C�R�[���ǂݎ��
			fscanf(pFile, "%f", &fAddSpeed);     //�����x�ǂݎ��
		}
		else if (strcmp(&aString[0], "JUMPPOWER") == 0)
		{
			fscanf(pFile, "%s", &aString[0]);    //�C�R�[���ǂݎ��
			fscanf(pFile, "%f", &fJumpPower);     //�W�����v�͓ǂݎ��
		}
		else if (strcmp(&aString[0], "END_SETBLOCK") == 0)
		{//X�I�u�W�F�N�g��ݒ�
			if (CScene::GetMode() == CScene::MODE_EDIT)
			{//�G�f�B�b�g���[�h�Ȃ�A�I�u�W�F�N�g�����X�g�ɕۑ����Ǘ�����
				pObject = CBlock::Create((CBlock::BLOCKTYPE)(nType),10,Pos,Rot,Scale,CObjectX::ROTTYPE_NORMAL,nSubType);
				((CBlock*)pObject)->SetBlockInfo(fStartRot, fRotSpeed, fMoveLength, fSpeed, nBlinkingSpeed, fAddSpeed, fJumpPower);
				if (pObject != nullptr)
				{
					pObject->SetStageManagerObj();                                                 //�X�e�[�W�}�l�[�W���[�ŊǗ�����I�u�W�F�N�g�ɐݒ�	
					m_nIndexObject++;
				}
			}
			else if (CScene::GetMode() == CScene::MODE_GAME)
			{//�Q�[�����[�h�Ȃ�A�I�u�W�F�N�g�𐶐����邾���ŁA���X�g�ɕۑ����Ȃ�
				pObject = CBlock::Create((CBlock::BLOCKTYPE)(nType), 10, Pos, Rot, Scale, CObjectX::ROTTYPE_NORMAL, nSubType);
				((CBlock*)pObject)->SetBlockInfo(fStartRot, fRotSpeed, fMoveLength, fSpeed, nBlinkingSpeed, fAddSpeed, fJumpPower);
			}
			break;
		}
	}

}
//=======================================================================================================================

//===========================================================
//�G�̏����e�L�X�g�t�@�C������ǂݍ���
//===========================================================
void CStageManager::LoadEnemyTxt(FILE* pFile)
{
	int nManagerType = 0;                  //�}�l�[�W���[�I�u�W�F�N�g�̎��
	int nType = 0;                         //�I�u�W�F�N�g���Ƃ̎��
	int nRotType = 0;                      //�����̎��
	int nSubType = 0;                      //�T�u�^�C�v
	D3DXVECTOR3 pos = NULL_VECTOR3;        //�ʒu
	D3DXVECTOR3 WarpPos = NULL_VECTOR3;
	int nWarpMap = 0;//���[�v��̃}�b�v�ԍ�
	int nLife = 0;//�̗�
	D3DXVECTOR3 Scale = NULL_VECTOR3;      //�g�嗦
	D3DXVECTOR3 Rot = NULL_VECTOR3;        //����
	CObject* pObject = nullptr;            //�I�u�W�F�N�g�ւ̃|�C���^
	char aString[128] = {};

	D3DXVECTOR3 MoveRangeAIPos = NULL_VECTOR3;   //�ړ��͈�AI�̈ʒu
	D3DXVECTOR3 MoveRangeAIScale = NULL_VECTOR3; //�ړ��͈�AI�̊g�嗦

	while (1)
	{
		fscanf(pFile, "%s", &aString[0]);
		if (aString[0] == '#')
		{
			fgets(&aString[0], 100, pFile);//�s���X�L�b�v
		}
		else if (strcmp(&aString[0], "MANAGERTYPE") == 0)
		{
			fscanf(pFile, "%s", &aString[0]);  //�C�R�[���ǂݎ��
			fscanf(pFile, "%d", &nManagerType);//�}�l�[�W���[�I�u�W�F�N�g�̃^�C�v�擾
		}
		else if (strcmp(&aString[0], "TYPE") == 0)
		{
			fscanf(pFile, "%s", &aString[0]);  //�C�R�[���ǂݎ��
			fscanf(pFile, "%d", &nType);       //�I�u�W�F�N�g���Ƃ̃^�C�v�擾
		}
		else if (strcmp(&aString[0], "ROTTYPE") == 0)
		{
			fscanf(pFile, "%s", &aString[0]);  //�C�R�[���ǂݎ��
			fscanf(pFile, "%d", &nRotType);    //�I�u�W�F�N�g���Ƃ̃^�C�v�擾
		}
		else if (strcmp(&aString[0], "POS") == 0)
		{
			fscanf(pFile, "%s", &aString[0]);  //�C�R�[���ǂݎ��
			fscanf(pFile, "%f", &pos.x);       //X���W�ǂݎ��
			fscanf(pFile, "%f", &pos.y);       //Y���W�ǂݎ��
			fscanf(pFile, "%f", &pos.z);       //Z���W�ǂݎ��
		}
		else if (strcmp(&aString[0], "SCALE") == 0)
		{
			fscanf(pFile, "%s", &aString[0]);  //�C�R�[���ǂݎ��
			fscanf(pFile, "%f", &Scale.x);       //X���W�ǂݎ��
			fscanf(pFile, "%f", &Scale.y);       //Y���W�ǂݎ��
			fscanf(pFile, "%f", &Scale.z);       //Z���W�ǂݎ��
		}
		else if (strcmp(&aString[0], "ROT") == 0)
		{
			fscanf(pFile, "%s", &aString[0]);  //�C�R�[���ǂݎ��
			fscanf(pFile, "%f", &Rot.x);       //X���W�ǂݎ��
			fscanf(pFile, "%f", &Rot.y);       //Y���W�ǂݎ��
			fscanf(pFile, "%f", &Rot.z);       //Z���W�ǂݎ��
		}
		else if (strcmp(&aString[0], "LIFE") == 0)
		{
			fscanf(pFile, "%s", &aString[0]);  //�C�R�[���ǂݎ��
			fscanf(pFile, "%d", &nLife);       //Z���W�ǂݎ��
		}
		else if (strcmp(&aString[0], "SUBTYPE") == 0)
		{
			fscanf(pFile, "%s", &aString[0]);  //�C�R�[���ǂݎ��
			fscanf(pFile, "%d", &nSubType);    //�T�u�^�C�v
		}
		else if (strcmp(&aString[0], "MOVERANGEAIPOS") == 0)
		{
			fscanf(pFile, "%s", &aString[0]);             //�C�R�[���ǂݎ��
			fscanf(pFile, "%f", &MoveRangeAIPos.x);       //X���W�ǂݎ��
			fscanf(pFile, "%f", &MoveRangeAIPos.y);       //Y���W�ǂݎ��
			fscanf(pFile, "%f", &MoveRangeAIPos.z);       //Z���W�ǂݎ��
		}
		else if (strcmp(&aString[0], "MOVERANGEAISCALE") == 0)
		{
			fscanf(pFile, "%s", &aString[0]);             //�C�R�[���ǂݎ��
			fscanf(pFile, "%f", &MoveRangeAIScale.x);       //X���W�ǂݎ��
			fscanf(pFile, "%f", &MoveRangeAIScale.y);       //Y���W�ǂݎ��
			fscanf(pFile, "%f", &MoveRangeAIScale.z);       //Z���W�ǂݎ��
		}
		else if (strcmp(&aString[0], "END_SETENEMY") == 0)
		{//X�I�u�W�F�N�g��ݒ�
			if (CScene::GetMode() == CScene::MODE_EDIT)
			{//�G�f�B�b�g���[�h�Ȃ�A�I�u�W�F�N�g�����X�g�ɕۑ����Ǘ�����
				pObject = CEnemy::Create(CEnemy::ENEMYTYPE(nType), nLife, pos, NULL_VECTOR3, Scale, MoveRangeAIPos, NULL_VECTOR3, MoveRangeAIScale);
				if (pObject != nullptr)
				{
					pObject->SetStageManagerObj();                                                 //�X�e�[�W�}�l�[�W���[�ŊǗ�����I�u�W�F�N�g�ɐݒ�	
					m_nIndexObject++;
				}
			}
			else if (CScene::GetMode() == CScene::MODE_GAME)
			{//�Q�[�����[�h�Ȃ�A�I�u�W�F�N�g�𐶐����邾���ŁA���X�g�ɕۑ����Ȃ�
				CEnemy::Create(CEnemy::ENEMYTYPE(nType), nLife, pos, NULL_VECTOR3, Scale, MoveRangeAIPos, NULL_VECTOR3, MoveRangeAIScale);
			}
			break;
		}
	}
}
//=======================================================================================================================

//===========================================================
//�T�C�Y���������Ȃ��Ƃ��̏���
//===========================================================
D3DXVECTOR3 CStageManager::NotSizeMoveProcess(float fMoveX, float fMoveY)
{
	D3DXVECTOR3 Move = NULL_VECTOR3;

	if (CManager::GetCamera()->GetCameraType() == CCamera::CAMERATYPE_BIRD)
	{
		Move.x = sinf(atan2f(fMoveX, fMoveY)) * m_fSpeed;
		Move.z = cosf(atan2f(fMoveX, fMoveY)) * m_fSpeed;
	}
	else
	{
		if (m_MoveMode == MOVEMODE00_XY)
		{
			Move.x = sinf(atan2f(fMoveX, fMoveY) + CManager::GetCamera()->GetRot().y) * m_fSpeed;
			Move.y = cosf(atan2f(fMoveX, fMoveY) + CManager::GetCamera()->GetRot().y) * m_fSpeed;
		}
		else if (m_MoveMode == MOVEMODE01_XZ)
		{
			Move.x = sinf(atan2f(fMoveX, fMoveY) + CManager::GetCamera()->GetRot().y) * m_fSpeed;
			Move.z = cosf(atan2f(fMoveX, fMoveY) + CManager::GetCamera()->GetRot().y) * m_fSpeed;
		}
	}
	return Move;
}
//=======================================================================================================================

//===========================================================
//�T�C�Y���������Ƃ��̏���
//===========================================================
D3DXVECTOR3 CStageManager::SizeMoveProcess(float fMoveX, float fMoveY,D3DXVECTOR3 Size)
{
	D3DXVECTOR3 ResultSize = NULL_VECTOR3;

	ResultSize.x = float(floor(Size.x));
	ResultSize.y = float(floor(Size.y));
	ResultSize.z = float(floor(Size.z));

	D3DXVECTOR3 Move = NULL_VECTOR3;
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_DOWN) == true ||
		CManager::GetInputKeyboard()->GetTrigger(DIK_UP) == true ||
		CManager::GetInputKeyboard()->GetTrigger(DIK_RIGHT) == true ||
		CManager::GetInputKeyboard()->GetTrigger(DIK_LEFT) == true)
	{
		if (m_MoveMode == MOVEMODE00_XY)
		{
			if (fMoveX != 0.0f)
			{
				Move.x = sinf(atan2f(fMoveX, 0.0f)) * ResultSize.x;
			}
			else if (fMoveY != 0.0f)
			{
				Move.y = cosf(atan2f(0.0f, fMoveY)) * ResultSize.x;
			}
		}
	}


	return Move;
}
//=======================================================================================================================

//===========================================================
//�g�嗦�����Z�b�g���鏈��
//===========================================================
void CStageManager::ResetScale()
{
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_V) == true)
	{
		m_Scale = ONE_VECTOR3;
	}
}
//=======================================================================================================================

//===========================================================
//�S�Ẵ}�l�[�W���[�I�u�W�F�N�g��j������
//===========================================================
void CStageManager::AllReleaseObject()
{
	//CObject* pObj = nullptr;//�I�u�W�F�N�g�擾�p
	//pObj = GetTopStageManagerObject();

	//CObject::TYPE type;
	//while (pObj != nullptr)
	//{
	//	//���̃I�u�W�F�N�g���i�[
	//	CObject* pNext = pObj->GetNextStageManagerObject();
	//	pObj->SetUseDeath(true);
	//	pObj->SetDeath();
	//	type = pObj->GetType();
	//	m_nIndexObject--;
	//	pObj = pNext;
	//}

	////for (int nCnt = 0; nCnt < m_nMAXMANAGEROBJECT; nCnt++)
	////{
	////	if (m_apObjectList[nCnt] != nullptr)
	////	{
	////		m_apObjectList[nCnt]->SetDeath();
	////		m_apObjectList[nCnt] = nullptr;
	////		m_nIndexObject--;
	////	}
	//}
}
//=======================================================================================================================
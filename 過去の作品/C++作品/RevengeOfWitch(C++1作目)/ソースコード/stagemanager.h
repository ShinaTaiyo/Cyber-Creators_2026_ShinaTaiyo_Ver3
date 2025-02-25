//==========================================================================================
//
//�V���P�V���F�z�u�c�[�������[stagemanager.h]
//Author:ShinaTaiyo
//
//==========================================================================================

#ifndef _STAGEMANAGER_H_  
#define _STAGEMANAGER_H_

//============================
//�C���N���[�h
//============================
#include "block.h"
#include "model.h"
#include "player.h"
#include "enemy.h"
//========================================================

//============================
//�O���錾
//============================
class CObjectXInfo;
class CBg3D;
//======================
//�}�N����`
//======================
#define SAVE_BIN "data\\BINARYFILE\\Save.bin"
#define SAVE_TXT "data\\TEXTFILE\\stagemanager\\Save.txt"
//============================
//�X�e�[�W�Ǘ��N���X
//============================
class CStageManager : public CObject
{
public:
	typedef enum
	{
		MANAGEROBJECT_BLOCK = 0,//�u���b�N
		MANAGEROBJECT_MODEL,    //���f��
		MANAGEROBJECT_ENEMY,    //�G
		MANAGEROBJECT_ITEM,     //�A�C�e��
		MANAGEROBJECT_MARKER,   //�}�[�J�[
		MANAGEROBJECT_BOSS,     //�{�X
		MANAGEROBJECT_TRAP,     //�g���b�v
		MANAGEROBJECT_SIGNBOARD,//���ĊŔ�
		MANAGEROBJECT_MAX
	}MANAGEROBJECT;

	typedef enum
	{
		FOCUSTYPE_NORMAL = 0,//�J�������ǂ��ʒu������
		FOCUSTYPE_WARPPOS,   //�J���������[�v��̈ʒu��ǂ�
	}FOCUSTYPE;

	typedef enum
	{
		WORLDTYPE00_EASY = 0,
		WORLDTYPE01_NORMAL,
		WORLDTYPE02_BOSSRUSH,
		WORLDTYPE_MAX
	}WORLDTYPE;

	CStageManager();  //�R���X�g���N�^
	~CStageManager(); //�f�X�g���N�^
	HRESULT Init() override;   //����������
	void Uninit() override;     //�I������
	void ExtraUninit() override;//�ʘg�̏I������
	void Update() override;    //�X�V����
	void Draw() override;      //�`�揈��
	void SetDeath() override;  //���S�t���O��ݒ�
	void LoadMapTxt(int nMapNum);               //�}�b�v���e�L�X�g�t�@�C�����烍�[�h����
	void LoadMapFilePass(WORLDTYPE type);           //�}�b�v�̃t�@�C���p�X�����[�h����
	void SaveMapTxt(int nMapNum);               //�}�b�v���e�L�X�g�t�@�C���ɃZ�[�u����
	void LoadMapBin(int nMapNum);               //�}�b�v���o�C�i���t�@�C�����烍�[�h����
	void SaveMapBin();                          //�}�b�v���o�C�i���t�@�C���ɃZ�[�u����
	int GetMapIndex() { return m_nMapIndex; }   //���݂̃}�b�v�ԍ����擾����
	int GetWarpMapNum() { return m_nWarpMapNum; }//���݂̃��[�v��̃}�b�v�ԍ����擾����
	D3DXVECTOR3& GetPos() { return m_Pos; }      //�ʒu���擾����
	D3DXVECTOR3& GetWarpPos() { return m_WarpPos; }//���[�v��̈ʒu���擾����
	FOCUSTYPE GetFocusType() { return m_FocusType; }//�W������ʒu���擾����
	int GetLife() { return m_nSetLife; }         //�̗͂�ݒ肷��
	void AllReleaseObject();                    //�S�Ẵ}�l�[�W���[�I�u�W�F�N�g�������B
	int GetSubType() { return m_nSetSubType; }  //���݂̃T�u�^�C�v���擾����
	static CStageManager* Create();             //��������
	static const int m_nMAXMANAGEROBJECT = 1024;//�Ǘ�����u���b�N�̍ő吔
private:

	typedef enum
	{
		MANAGERMODE_ALREADYSTAGE = 0,//���ɂ���X�e�[�W��ҏW���郂�[�h
		MANAGERMODE_NEWSTAGE,        //�V�����X�e�[�W����郂�[�h
		MANAGERMODE_MAX
	}MANAGERMDOE;

	typedef enum
	{
		MOVEMODE00_XY = 0,//�ړ����[�h�O�FXY
		MOVEMODE01_XZ,    //�ړ����[�h�P�FXZ
		MOVEMODE_MAX
	}MOVEMODE;

	static const int m_nMAX_MAP = 20;
	static const int m_nMAX_WORD = 126;
	static const char* m_apWORLDMAP_TXT[WORLDTYPE_MAX];

	void MoveManager();                         //�X�e�[�W�}�l�[�W���[�𓮂���
	void LifeSet();                             //�̗͂�ݒ肷��
	void TypeChenge();                          //�I�u�W�F�N�gX�̎�ނ�ς���
	void SetObjectX();                          //�I�u�W�F�N�gX��ݒ肷��
	void DeleteObjectX();                       //�I�u�W�F�N�gX������
	void ChengeObject();                        //�I�u�W�F�N�g�̎�ނ�����
	void ReleaseObject();                       //�I�u�W�F�N�g�������[�X����
	void ColorChenge();                         //�X�e�[�W�}�l�[�W���[�̐F������ݒ肷��
	void MapChenge();                           //�}�b�v��ς���
	void DispInfo();                            //���݂̃}�b�v�G�f�B�^�̏���\��
	void ChooseObject();                        //�I�u�W�F�N�g�I������

	void SaveTrapTxt(FILE * pFile,CObject * pObj);//�g���b�v���e�L�X�g�t�@�C���ɕۑ�����
	void SaveBlockTxt(FILE* pFile, CObject* pObj);//�u���b�N���e�L�X�g�t�@�C���ɕۑ�����
	void SaveEnemyTxt(FILE* pFile, CObject* pObj);//�G���e�L�X�g�t�@�C���ɕۑ�����
	void LoadTrapTxt(FILE* pFile);                //�g���b�v�̏����e�L�X�g�t�@�C������ǂݍ���
	void LoadBlockTxt(FILE* pFile);               //�u���b�N�̏����e�L�X�g�t�@�C������ǂݍ���
	void LoadEnemyTxt(FILE* pFile);               //�G�̏����e�C�X�g�t�@�C������ǂݍ���
	CObject* m_apObjectList[m_nMAXMANAGEROBJECT];//�Ǘ�����I�u�W�F�N�g���
	int m_nIndexObject;                         //�Ǘ�����I�u�W�F�N�g�̔ԍ�

	D3DXVECTOR3 m_Move;                         //�ړ���
	D3DXVECTOR3 m_SaveBeforeChoosePos;          //�I������������O�̂���ʒu

	CObject* m_pManagerObject;                  //�F�X�Ȕh���N���X�ɃL���X�g����I�u�W�F�N�g
	int m_nMapCameraType;                       //�}�b�v���Ƃ̃J�����̎�ނ�ݒ�

	MANAGERMDOE m_ManagerMode;                  //���݂̃X�e�[�W�}�l�[�V���[�̃��[�h
	D3DXVECTOR3 m_Pos;                          //�ʒu
	D3DXVECTOR3 m_Scale;                        //�g�嗦
	char m_aMapFilePass[m_nMAX_MAP][m_nMAX_WORD];          //�}�b�v�̃t�@�C���p�X
	int m_nMapIndex;                            //�}�b�v�̃C���f�b�N�X
	int m_nMapNum;                              //�}�b�v�̑���
	int m_nWorldIndex;                          //���[���h�̃C���f�b�N�X

	int m_nWarpMapNum;                          //���[�v�}�[�J�[�̃��[�v��}�b�v�̐ݒ�
	D3DXVECTOR3 m_WarpPos;                      //���[�v�}�[�J�[�̃��[�v��̃��[�v�ʒu��ݒ�
	int m_nSetSubType;                          //�T�u�^�C�v��ݒ肷��

	//====================
	//�I���n
	//====================
	CObject* m_pChooseObject;                   //�I���I�u�W�F�N�g
	bool m_bChooseObject;                       //�I�u�W�F�N�g��I�����Ă��邩�ǂ���
	int m_nIndexChooseObject;                   //�I�����Ă���I�u�W�F�N�g�̔ԍ�
	bool m_bMakeMapMode;                        //�}�b�v����郂�[�h
	//========================================================================================

	//===================
	//�ړ����[�h�֌W
	//===================
	MOVEMODE m_MoveMode;//�ړ����[�h
	FOCUSTYPE m_FocusType;//�J�������ǂ����̎��
	bool m_bUseSizeMove;//���݂̃I�u�W�F�N�g�̃T�C�Y���ړ����邩�ǂ���

	D3DXVECTOR3 NotSizeMoveProcess(float fMoveX,float fMoveY);//�T�C�Y���������Ȃ��Ƃ��̏���
	D3DXVECTOR3 SizeMoveProcess(float fMoveX,float fMoveY,D3DXVECTOR3 Size);//�T�C�Y���������Ƃ��̏���
	//========================================================================================

	//===================
	//�g�嗦�֌W
	//===================
	void ResetScale();//�g�嗦�����Z�b�g���鏈��
	//========================================================================================

	//===================
	//3D�w�i
	//===================
	CBg3D* m_pBg3D;//�w�i�ւ̃|�C���^
	//========================================================================================

	int m_nRotType;                             //�����̎��
	bool m_bMove;                               //�ړ����Ă��邩�ǂ���
	float m_fSpeed;                             //�ړ����x
	int m_nManagerType;                         //�}�l�[�W���[�I�u�W�F�N�g�̎��
	int m_nType;                                //�I�u�W�F�N�g���Ƃ̎��
	int m_nCntTime;                             //�o�����Ă���̎��Ԃ��J�E���g����
	int m_nSetLife;                             //�̗͂�ݒ肷��
};
#endif
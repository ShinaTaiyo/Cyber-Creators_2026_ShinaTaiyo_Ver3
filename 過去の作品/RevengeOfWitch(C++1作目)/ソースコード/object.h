//====================================================
//
//�V���P�U���F�I�u�W�F�N�g�̃��X�g�Ǘ�[Object.cpp]
//Author:ShinaTaiyo
//
//====================================================

#ifndef _OBJECT_H_  
#define _OBJECT_H_

//======================
//�C���N���[�h
//======================
#include "main.h"

//==========================================

//==========================================
//�I�u�W�F�N�g�N���X
//==========================================
class CObject
{
public:
	//===========================
	//�I�u�W�F�N�g��ޗ񋓌^
	//===========================
	typedef enum
	{
		TYPE_NONE,            //�^�C�v�Ȃ�
		TYPE_PLAYER,          //�v���C���[3
		TYPE_ENEMY,           //�G2
		TYPE_BULLET,          //�e4
		TYPE_EXPLOSION,       //����1
		TYPE_EFFECT,          //�G�t�F�N�g1
		TYPE_PARTICLE,        //�p�[�e�B�N��1
		TYPE_BLOCK,           //�u���b�N0
		TYPE_ITEM,            //�A�C�e��2
		TYPE_MODEL,           //���f��
		TYPE_BG,              //�w�i
		TYPE_FIELD,           //��
		TYPE_STAGEMANAGER,    //�X�e�[�W�}�l�[�W���[
		TYPE_GAUGE,           //�Q�[�W2
		TYPE_NUMBER,          //����
		TYPE_FADE,            //�t�F�[�h4
		TYPE_BG3D,            //3D�w�i
		TYPE_MARKER,          //�}�[�J�[
		TYPE_NUMBER3D,        //3D����
		TYPE_MAGICWINDOW,     //���@�E�C���h�E
		TYPE_MAGICDISP,       //���@�\��
		TYPE_ATTACK,          //�U��
		TYPE_BOSS,            //�{�X
		TYPE_EMITTER,         //�G�~�b�^�[
		TYPE_TRAP,            //�g���b�v
		TYPE_MESHORBIT,       //���b�V���O��
		TYPE_TIMER,           //�^�C�}�[
		TYPE_SCORE,           //�X�R�A      
		TYPE_AIMODEL,         //AI���f��
		TYPE_DAMAGE,          //�_���[�W
		TYPE_UI,              //UI
		TYPE_UI3D,            //UI3D
		TYPE_PARTICLE2D,      //�p�[�e�B�N��2D
		TYPE_MAX
	}TYPE;
	//===========================================

	//===========================
	//�I�u�W�F�N�g���ޗ񋓌^
	//===========================
	typedef enum
	{
		OBJECTTYPE_2D = 0,
		OBJECTTYPE_3D,
		OBJECTTYPE_BILLBOARD,
		OBJECTTYPE_X,
	}OBJECTTYPE;

	CObject(int nPriority = 2);//�`��D��ݒ�(�T���Q�W��New!�F�f�t�H���g�����i�Ăяo�����Ɉ����ݒ肵�Ȃ���΂R���������ď��������
	virtual ~CObject();        //�f�X�g���N�^
	virtual HRESULT Init() = 0;//����������
	virtual void Uninit() = 0; //�I������
	virtual void ExtraUninit();//�ʘg�̏I������
	virtual void Update() = 0; //�X�V����
	virtual void Draw() = 0;   //�`�揈��
	bool GetUseDeath() { return m_bUseDeath; }//���S�t���O�𔭓����邩���߂�t���O���擾
	static void ReleaseAll();  //�S�I�u�W�F�N�g�J��
	static void UpdateAll();   //�S�I�u�W�F�N�g�X�V
	static void DrawAll();     //�S�I�u�W�F�N�g�`��
	virtual void SetDeath();                                              //���S�t���O��ݒ�
	//=====================
	//�擾�n
	//=====================
	static int GetNumAll();                              //�S�I�u�W�F�N�g�̑������擾����
	bool GetCreateSuccess() { return m_bCreateSuccess; } //�I�u�W�F�N�g�̐����ɐ����������ǂ���
	int GetId() { return m_nID; }                        //���݂�ID���擾����
	static CObject* GetCObject(int nIdx,int nPriority);  //�I�u�W�F�N�g�擾
	TYPE GetType();                                      //�^�C�v�擾
	//==============================================================================================
	void SetType(TYPE type);                             //�^�C�v�ݒ�
	void SetStageManagerObjNum(int nNum) { m_nStageManagerObjNum = nNum; }            //�X�e�[�W�}�l�[�W���[�ŊǗ�����Ă���I�u�W�F�N�g�̔ԍ���ݒ�
	int GetStageManagerObjNum() { return m_nStageManagerObjNum; }                     //�X�e�[�W�}�l�[�W���[�ŊǗ�����Ă���I�u�W�F�N�g�̔ԍ����擾����
	static const int m_nMAXOBJECT = 1024;                                             //�I�u�W�F�N�g�ő吔
	static const int m_nMAXPRIORITY = 9;                                              //�`�揇�ő吔
	static void ReleaseProcess();                                                     //���X�g�̔j�������鏈��
	void Release();                                                                   //�������g�̊J��
	void SetUseDeath(bool bDeath) { m_bUseDeath = bDeath; }                           //���S�t���O�𔭓����邩�ǂ�����ݒ肷��
	static void SetReleaseAll() { m_bActivationReleaseAll = true; }                   //ReleaseAll�𔭓�������t���O��ݒ肷��
	void SetObjectType(OBJECTTYPE type) { m_ObjectType = type; }                      //�I�u�W�F�N�g�̕��ނ����߂�
	OBJECTTYPE GetObjectType() { return m_ObjectType; }                               //�I�u�W�F�N�g�̕��ނ��擾����
	CObject* GetNextObject() {return m_pNext; }                                       //���̃I�u�W�F�N�g���擾����
	CObject* GetPrevObject() { return m_pPrev; }                                      //�O�̃I�u�W�F�N�g���擾����

	//==============================
	//�X�e�[�W�}�l�[�W���[�֌W
	//==============================
	CObject* GetNextStageManagerObject() { return m_pStageManagerObj_Next; }          //�X�e�[�W�}�l�[�W���[�ŊǗ����Ă���I�u�W�F�N�g�̎��̃I�u�W�F�N�g���擾����
	CObject* GetPrevStageManagerObject() { return m_pStageManagerObj_Prev; }          //�X�e�[�W�}�l�[�W���[�ŊǗ����Ă���I�u�W�F�N�g�̑O�̃I�u�W�F�N�g���擾����
	static CObject* GetTopStageManagerObject() { return m_pStageManagerObj_Top; }     //�X�e�[�W�}�l�[�W���[�ŊǗ����Ă���I�u�W�F�N�g�̃g�b�v�I�u�W�F�N�g���擾����
	static CObject* GetCurStageManagerObject() { return m_pStageManagerObj_Cur; }     //�X�e�[�W�}�l�[�W���[�ŊǗ����Ă���I�u�W�F�N�g�̃J�����g�I�u�W�F�N�g���擾����
	void SetStageManagerObj();                                                        //�X�e�[�W�}�l�[�W���[�ŊǗ�����I�u�W�F�N�g��ݒ�

	static int GetTotalStageManagerObjNum() { return m_nNumStageManagerObject; }      //�X�e�[�W�}�l�[�W���[�ŊǗ����Ă���I�u�W�F�N�g�̑������擾
	static void StageManagerObjectReleaseAll();                                            //�S�Ẵ}�l�[�W���[�Ǘ��̃I�u�W�F�N�g������    
	//==============================================================================================

	//==============================
	//���X�g�֌W
	//==============================
	static CObject* GetTopObject(int nPriority){ return m_pTop[nPriority];}           //�g�b�v�I�u�W�F�N�g���擾����
	static CObject* GetCurrentObject(int nPriority) { return m_pCur[nPriority];}      //�J�����g�I�u�W�F�N�g���擾����
	void SetStageManagerChoose(bool bChoose) { m_bStageManagerChooseState = bChoose; }//�X�e�[�W�}�l�[�W���[���I�����Ă��邩�ǂ���
	bool GetStageManagerChoose() { return m_bStageManagerChooseState; }               //�X�e�[�W�}�l�[�W���[���I�����Ă��邩�ǂ������擾����
	//==============================================================================================

	//=================================================
	//�t���[���֌W
	//=================================================
	int GetCntFrame() { return m_nCntFrame; }                                         //�o�����Ă���̃t���[�������J�E���g����
	//================================================================================================

	//=================================================
	//�T�E���h�֌W
	//=================================================
	void SetUseInitialSound(bool bUse) { m_bUseInitialSound = bUse; }//���������ɃT�E���h���Đ����邩�ǂ���
	bool GetUseInitialSound() { return m_bUseInitialSound; }         //���������ɃT�E���h�������������邩�ǂ������擾
	//================================================================================================
protected:
private:
	void ReleaseStageManagerObj();                             //�X�e�[�W�}�l�[�W���[�ŊǗ����Ă���I�u�W�F�N�g��j��
	bool m_bDeath;                                           //���S�t���O
	bool m_bUseDeath;                                        //���S�t���O�𔭓����邩�ǂ���

	static CObject* m_apObject[m_nMAXPRIORITY][m_nMAXOBJECT];//�I�u�W�F�N�g�Ǘ�
	static bool m_bActivationReleaseAll;                     //ReleaseAll�𔭓����邩�ǂ���

	//===============================================
	//���X�g�Ǘ�
	//===============================================
	static CObject* m_pTop[m_nMAXPRIORITY];                  //�擪�̃I�u�W�F�N�g�̃|�C���^
	static CObject* m_pCur[m_nMAXPRIORITY];                  //���݁i�Ō���j�̃|�C���^
	CObject* m_pNext;                                        //���̃I�u�W�F�N�g�ւ̃|�C���^
	CObject* m_pPrev;                                        //�O�̃I�u�W�F�N�g�ւ̃|�C���^
	//===============================================================================================

	//===============================================
	//�X�e�[�W�}�l�[�W���[�֌W
	//===============================================
	static CObject* m_pStageManagerObj_Top;                  //�X�e�[�W�}�l�[�W���[�ŊǗ����Ă���擪�I�u�W�F�N�g�ւ̃|�C���^
	static CObject* m_pStageManagerObj_Cur;                  //�X�e�[�W�}�l�[�W���[�ŊǗ����Ă��Ō���I�u�W�F�N�g�ւ̃|�C���^
	CObject* m_pStageManagerObj_Next;                        //�X�e�[�W�}�l�[�W���[�ŊǗ����Ă��鎟�̃I�u�W�F�N�g�ւ̃|�C���^
	CObject* m_pStageManagerObj_Prev;                        //�X�e�[�W�}�l�[�W���[�ŊǗ����Ă���O�̃I�u�W�F�N�g�ւ̃|�C���^
	bool m_bIsStageManagerObj;                               //�X�e�[�W�}�l�[�W���[�ŊǗ�����Ă���I�u�W�F�N�g���ǂ���
	int m_nStageManagerObjNum;                               //�X�e�[�W�}�l�[�W���[�ŊǗ�����Ă���I�u�W�F�N�g�̔ԍ�
	bool m_bStageManagerChooseState;                         //�X�e�[�W�}�l�[�W���[���I�����Ă��邩�ǂ���

	static int m_nNumStageManagerObject;                     //�X�e�[�W�}�l�[�W���[�ŊǗ�����Ă���I�u�W�F�N�g�̐�
	//================================================================================================

	TYPE m_type;                                             //�I�u�W�F�N�g���
	OBJECTTYPE m_ObjectType;                                 //�I�u�W�F�N�g����
	static int m_nNumAll;                                    //�I�u�W�F�N�g����
	int m_nID;                                               //�������g��ID
	int m_nPriority;                                         //�`�揇�𑗂�
	bool m_bCreateSuccess;                                   //�I�u�W�F�N�g�̐����������������ǂ���

	//=================================================
	//�t���[���֌W
	//=================================================
	int m_nCntFrame;                                         //�o�����Ă���̃t���[�������J�E���g����
	//================================================================================================

	//=================================================
	//�T�E���h�֌W
	//=================================================
	bool m_bUseInitialSound;//���������ɃT�E���h���Đ����邩�ǂ���
	//================================================================================================
};

#endif

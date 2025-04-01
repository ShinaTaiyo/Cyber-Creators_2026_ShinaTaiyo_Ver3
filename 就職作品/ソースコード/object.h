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
	enum class TYPE
	{
		NONE,            //�^�C�v�Ȃ�
		BG3D,            //3D�w�i
		FIELD,           //��
		BLOCK,           //�u���b�N
		BGMODEL,         //�w�i���f��
		AIMODEL,         //AI���f��
		ATTACK,          //�U��
		ENEMY,           //�G
		PLAYER,          //�v���C���[
		WIREHEAD,        //���C���[��
		WIRE,            //���C���[
		MESHORBIT,       //�O�Ճ��b�V��
		EFFECT,          //�G�t�F�N�g
		PARTICLE,        //�p�[�e�B�N��
		UI3D,            //UI3D
		BG,              //�w�i
		NUMBER,          //����
		NUMBER3D,        //����3D
	    PARTICLE2D,      //�p�[�e�B�N���QD
		LOCKON,          //���b�N�I��
		UI,              //UI
		GAUGE,           //�Q�[�W
		UI_PAUSE,        //�|�[�Y�pUI
		PHASEINFO,       //�t�F�[�Y���
		STAGEMANAGER,    //�X�e�[�W�}�l�[�W���[
		PHASEMANAGER,    //�t�F�[�Y�}�l�[�W���[
		EVENTMANAGER,    //�C�x���g�}�l�[�W���[
		COMBO,           //�R���{
		TUTORIAL,        //�`���[�g���A��
		SCORE,           //�X�R�A
		FADE,            //�t�F�[�h
		MAX
	};
	//===========================================

	//====================================================
	//�X�e�[�W�}�l�[�W���[�Ǘ��I�u�W�F�N�g�^�C�v�񋓌^�i���̃��[�h�����Ɏg�p����j
	//====================================================
	enum class MANAGEROBJECTTYPE
	{
		NONE = 0, //�Ȃ�
		BLOCK,    //�u���b�N
		BGMODEL,  //�w�i���f��
		SHOTWEAKENEMY,//�ˌ��Ɏア�G
		DIVEWEAKENEMY,//�_�C�u�Ɏア�G
		IDLEENEMY,    //�������Ȃ��G
		MAX
	};
	//===========================================

	//===========================
	//�I�u�W�F�N�g���ޗ񋓌^
	//===========================
	enum class OBJECTTYPE
	{
		OBJECTTYPE_NONE = 0,
		OBJECTTYPE_2D,
		OBJECTTYPE_3D,
		OBJECTTYPE_BILLBOARD,
		OBJECTTYPE_X,
		OBJECTTYPE_MAX
	};
	//===========================================

	CObject(int nPriority = 2,bool bUseintPriority = false,TYPE Type = TYPE::NONE,OBJECTTYPE ObjType = OBJECTTYPE::OBJECTTYPE_NONE);//�`��D��ݒ�(�T���Q�W��New!�F�f�t�H���g�����i�Ăяo�����Ɉ����ݒ肵�Ȃ���΂R���������ď��������
	virtual ~CObject();                                                               //�f�X�g���N�^
	virtual HRESULT Init() = 0;                                                       //����������
	virtual void Uninit() = 0;                                                        //�I������
	virtual void ExtraUninit();                                                       //�ʘg�̏I������
	virtual void Update() = 0;                                                        //�X�V����
	virtual void Draw() = 0;                                                          //�`�揈��
	bool GetUseDeath() { return m_bUseDeath; }                                        //���S�t���O�𔭓����邩���߂�t���O���擾
	static void ReleaseAll();                                                         //�S�I�u�W�F�N�g�J��
	static void UpdateAll();                                                          //�S�I�u�W�F�N�g�X�V
	static void DrawAll();                                                            //�S�I�u�W�F�N�g�`��
	virtual void SetDeath();                                                          //���S�t���O��ݒ�
	static int GetNumAll();                                                           //�S�I�u�W�F�N�g�̑������擾����
	bool GetCreateSuccess() { return m_bCreateSuccess; }                              //�I�u�W�F�N�g�̐����ɐ����������ǂ���
	static CObject* GetCObject(int nIdx,int nPriority);                               //�I�u�W�F�N�g�擾
	TYPE GetType();                                                                   //�^�C�v�擾
	void SetType(TYPE type);                                                          //�^�C�v�ݒ�
	static const int m_nMAXOBJECT = 1024;                                             //�I�u�W�F�N�g�ő吔
	static const int m_nMAXPRIORITY = static_cast<int>(TYPE::MAX);                    //�`�揇�ő吔
	static void ReleaseProcess();                                                     //���X�g�̔j�������鏈��
	void Release();                                                                   //�������g�̊J��
	void SetUseDeath(bool bDeath) { m_bUseDeath = bDeath; }                           //���S�t���O�𔭓����邩�ǂ�����ݒ肷��
	static void SetReleaseAll() { m_bActivationReleaseAll = true; }                   //ReleaseAll�𔭓�������t���O��ݒ肷��
	void SetObjectType(OBJECTTYPE type) { m_ObjectType = type; }                      //�I�u�W�F�N�g�̕��ނ����߂�
	OBJECTTYPE GetObjectType() { return m_ObjectType; }                               //�I�u�W�F�N�g�̕��ނ��擾����
	CObject* GetNextObject() {return m_pNext; }                                       //���̃I�u�W�F�N�g���擾����
	CObject* GetPrevObject() { return m_pPrev; }                                      //�O�̃I�u�W�F�N�g���擾����
	void SetIsStopUpdatePause(bool bPause) { m_bIsStopUpdatePause = bPause; }         //�|�[�Y���ɍX�V���~�߂邩�ǂ�����ݒ肷��
	const bool& GetIsStopUpdatePause() const { return m_bIsStopUpdatePause; }         //�|�[�Y���ɍX�V���~�߂邩�ǂ������擾����
	//==============================
	//�X�e�[�W�}�l�[�W���[�֌W
	//==============================
	//���ޗp
	void SetManagerObjectType(MANAGEROBJECTTYPE Type) { m_ManagerObjectType = Type; }     //�X�e�[�W�}�l�[�W���[�Ǘ��^�C�v��ݒ�
	const MANAGEROBJECTTYPE & GetManagerObjectType() const { return m_ManagerObjectType; }  //�X�e�[�W�}�l�[�W���[�Ǘ��^�C�v���擾
	//==============================================================================================

	//==============================
	//���X�g�֌W
	//==============================
	static CObject* GetTopObject(int nPriority){ return m_pTop[nPriority];}           //�g�b�v�I�u�W�F�N�g���擾����
	static CObject* GetCurrentObject(int nPriority) { return m_pCur[nPriority];}      //�J�����g�I�u�W�F�N�g���擾����
	//==============================================================================================

	//=================================================
	//�t���[���֌W
	//=================================================
	int GetCntFrame() { return m_nCntFrame; }                                         //�o�����Ă���̃t���[�������J�E���g����
	//================================================================================================

	//=================================================
	//�G�f�B�^�֌W
	//=================================================
	virtual void SaveInfoTxt(fstream& WritingFile);   //�e�L�X�g�t�@�C���ɏ���ۑ����邽�߂̊֐�
	virtual void LoadInfoTxt(fstream& LoadingFile, list<CObject*>& listSaveManager, string& Buff,CObject * pObj);//�e�L�X�g�t�@�C���������ǂݍ��ނ��߂̊֐�
	virtual void ManagerChooseControlInfo();          //��񑀍�
	virtual CObject * ManagerChengeObject(bool bAim); //�X�e�[�W�}�l�[�W���[�ɕύX�����I�u�W�F�N�g��n��
	virtual CObject * ManagerSaveObject();            //�X�e�[�W�}�l�[�W���[�ɍ��̃I�u�W�F�N�g��ۑ�����
	//================================================================================================
protected:
private:
	static CObject* m_apObject[m_nMAXPRIORITY][m_nMAXOBJECT];//�I�u�W�F�N�g�Ǘ�
	static bool m_bActivationReleaseAll;                     //ReleaseAll�𔭓����邩�ǂ���

    //�`��v���C�I���e�B
	static constexpr int s_nDrawPriority[static_cast<int>(TYPE::MAX)] = 
	{
		static_cast<int>(TYPE::BG3D),         
  		static_cast<int>(TYPE::FIELD),        
		static_cast<int>(TYPE::BGMODEL),      
		static_cast<int>(TYPE::BLOCK),        
		static_cast<int>(TYPE::AIMODEL),      
		static_cast<int>(TYPE::PLAYER),       
		static_cast<int>(TYPE::WIREHEAD),     
		static_cast<int>(TYPE::WIRE),         
		static_cast<int>(TYPE::ATTACK),       
		static_cast<int>(TYPE::ENEMY),        
		static_cast<int>(TYPE::UI3D),         
		static_cast<int>(TYPE::NUMBER3D),     
		static_cast<int>(TYPE::BG),           
		static_cast<int>(TYPE::PARTICLE),     
		static_cast<int>(TYPE::MESHORBIT),    
		static_cast<int>(TYPE::EFFECT),       
		static_cast<int>(TYPE::PARTICLE2D),   
		static_cast<int>(TYPE::LOCKON),       
		static_cast<int>(TYPE::UI),           
		static_cast<int>(TYPE::NUMBER),       
		static_cast<int>(TYPE::GAUGE),        
		static_cast<int>(TYPE::UI_PAUSE),        
		static_cast<int>(TYPE::PHASEINFO),    
		static_cast<int>(TYPE::STAGEMANAGER), 
		static_cast<int>(TYPE::PHASEMANAGER), 
		static_cast<int>(TYPE::EVENTMANAGER), 
		static_cast<int>(TYPE::COMBO),        
		static_cast<int>(TYPE::TUTORIAL),     
		static_cast<int>(TYPE::SCORE),        
		static_cast<int>(TYPE::FADE),         
		static_cast<int>(TYPE::NONE),         
	};//�`��v���C�I���e�B

	//===============================================
	//���X�g�Ǘ�
	//===============================================
	static CObject* m_pTop[m_nMAXPRIORITY];                  //�擪�̃I�u�W�F�N�g�̃|�C���^
	static CObject* m_pCur[m_nMAXPRIORITY];                  //���݁i�Ō���j�̃|�C���^

	CObject* m_pNext;                                        //���̃I�u�W�F�N�g�ւ̃|�C���^!
	CObject* m_pPrev;                                        //�O�̃I�u�W�F�N�g�ւ̃|�C���^!

	bool m_bDeath;                                           //���S�t���O!
	bool m_bUseDeath;                                        //���S�t���O�𔭓����邩�ǂ���!
	bool m_bIsStopUpdatePause;                                   //�|�[�Y���ɍX�V���~�߂邩�ǂ���
	//===============================================================================================

	//���ޗp
	MANAGEROBJECTTYPE m_ManagerObjectType;                   //�Ǘ��ԍ����ʗp

	//================================================================================================

	static int m_nNumAll;                                    //�I�u�W�F�N�g����

    //=================================================
	//��{�n
	//=================================================
	TYPE m_type;                                             //�I�u�W�F�N�g���!
	OBJECTTYPE m_ObjectType;                                 //�I�u�W�F�N�g����!
	int m_nPriority;                                         //�`�揇�𑗂�!
	bool m_bCreateSuccess;                                   //�I�u�W�F�N�g�̐����������������ǂ���!
	//================================================================================================

	//=================================================
	//�t���[���֌W
	//=================================================
	int m_nCntFrame;                                         //�o�����Ă���̃t���[�������J�E���g����!
	//================================================================================================
};

#endif

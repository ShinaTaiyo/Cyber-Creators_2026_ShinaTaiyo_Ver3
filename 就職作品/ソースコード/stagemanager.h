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
#include "main.h"
#include "object.h"
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

//======================
//�O���錾
//======================
class CStageManagerState;

//============================
//�X�e�[�W�Ǘ��N���X
//============================
class CStageManager : public CObject
{
public:
	enum class FOCUSTYPE
	{
		FOCUSTYPE_NORMAL = 0,//�J�������ǂ��ʒu������
		FOCUSTYPE_WARPPOS,   //�J���������[�v��̈ʒu��ǂ�
	};

	enum class WORLDTYPE
	{//���[���h�񋓌^
		STAGE01 = 0,
		STAGE02,
		STAGE03,
		MAX
	};

	CStageManager(int nPri = 0, bool bUseintPri = false, CObject::TYPE type = CObject::TYPE::STAGEMANAGER, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_NONE);//�R���X�g���N�^
	~CStageManager();                                                //�f�X�g���N�^
	HRESULT Init() override;                                         //����������
	void Uninit() override;                                          //�I������
	void Update() override;                                          //�X�V����
	void Draw() override;                                            //�`�揈��
	void SetDeath() override;                                        //���S�t���O��ݒ�
	void LoadMapTxt(int nMapNum);                                    //�}�b�v���e�L�X�g�t�@�C�����烍�[�h����
	void SaveMapTxt(int nMapNum);                                    //�}�b�v���e�L�X�g�t�@�C���ɃZ�[�u����
	int GetMapIndex() { return m_nMapIndex; }                        //���݂̃}�b�v�ԍ����擾����
	void MapChenge();                                                //�}�b�v��ύX����

	void SetSpawnPoint(D3DXVECTOR3 Pos) { m_SpawnPoint = Pos; }      //�X�|�[���ʒu��ݒ肷��
	const D3DXVECTOR3& GetSpawnPoint() const { return m_SpawnPoint; }//�X�|�[���ʒu���擾����

	list <CObject*> & GetStgObjList() { return m_StgObjList; }       //�Ǘ��I�u�W�F�N�g���X�g���擾����
	const D3DXVECTOR3& GetSavePos() const { return m_SavePos; }      //�j������I�u�W�F�N�g�̈ʒu���擾
	void SetSavePos(D3DXVECTOR3 Pos) { m_SavePos = Pos; }            //�j������I�u�W�F�N�g�̈ʒu��ݒ�
	const D3DXVECTOR3& GetSaveRot() const { return m_SaveRot; }      //�j������I�u�W�F�N�g�̌������擾
	void SetSaveRot(D3DXVECTOR3 Rot) { m_SaveRot = Rot; }            //�j������I�u�W�F�N�g�̌�����ݒ�
	const D3DXVECTOR3& GetSaveScale() const { return m_SaveScale; }  //�j������I�u�W�F�N�g�̊g�嗦���擾
	void SetSaveScale(D3DXVECTOR3 Scale) { m_SaveScale = Scale; }    //�j������I�u�W�F�N�g�̊g�嗦��ݒ�
	static CStageManager* Create();                                  //��������
private:

	//*�񋓌^
	enum class MANAGERMODE
	{
		ALREADYSTAGE = 0,//���ɂ���X�e�[�W��ҏW���郂�[�h
		NEWSTAGE,        //�V�����X�e�[�W����郂�[�h
		MAX
	};

	enum class STATE
	{//�X�e�[�g��
		NEWOBJECT = 0,//�V�����I�u�W�F�N�g
		PLACEDOBJECT, //�����̃I�u�W�F�N�g
		MAX
	};

	//*�ÓI�����o�ϐ�
	static const int m_nMAX_MAP = 20;                                       //�}�b�v�ő吔
	static const int m_nMAX_WORD = 126;                                     //������ő吔�F
	static const string m_aWORLDMAP_TXT[static_cast<int>(WORLDTYPE::MAX)];  //���[���h��
	static const string m_aSAVE_FILENAME;//�ۑ�����t�@�C����  

	//*�ϐ�

	//====================
	//��{�n
	//====================
	D3DXVECTOR3 m_SaveRot;                      //����
	D3DXVECTOR3 m_SavePos;                      //�ʒu
	D3DXVECTOR3 m_SaveScale;                    //�g�嗦
	D3DXVECTOR3 m_SaveBeforeChoosePos;          //�I������������O�̂���ʒu
	MANAGERMODE m_ManagerMode;                  //���݂̃X�e�[�W�}�l�[�V���[�̃��[�h
	D3DXVECTOR3 m_SpawnPoint;                   //�X�|�[���|�C���g
	//========================================================================================

	//====================
	//�ԍ�
	//====================
	int m_nMapIndex;                            //�}�b�v�̃C���f�b�N�X
	int m_nWorldIndex;                          //���[���h�̃C���f�b�N�X
	int m_nMapNum;                              //�}�b�v�̑���
	//========================================================================================
	
	//====================
	//�I���n
	//====================
	CObject* m_pChooseObject;                   //�I���I�u�W�F�N�g
	bool m_bChooseObject;                       //�I�u�W�F�N�g��I�����Ă��邩�ǂ���
	//========================================================================================

	//===================
	//���[�h�֌W
	//===================
	bool m_bMakeMapMode;                        //�}�b�v����郂�[�h
	bool m_bUseSizeMove;//���݂̃I�u�W�F�N�g�̃T�C�Y���ړ����邩�ǂ���
	//========================================================================================


	//===================
	//�I�u�W�F�N�g���X�g
	//===================
	list <CObject*> m_StgObjList;//�I�u�W�F�N�g�̃��X�g
	//========================================================================================

	//===================
	//3D�w�i
	//===================
	CBg3D* m_pBg3D;               //�w�i�ւ̃|�C���^
	//========================================================================================

	//===================
	//�X�e�[�g
	//===================
	CStageManagerState* m_pState;//�X�e�[�g�i�[�p
	STATE m_NowState;//���݂̏��
	//========================================================================================

	//*�֐�
	
	void ResetScale();  //�g�嗦�����Z�b�g���鏈��
	void DispInfo();    //���݂̃}�b�v�G�f�B�^�̏���\��

	void ChengeState(); //�X�e�[�g��ς���
};

//�X�e�[�W�}�l�[�W���[��ԃX�[�p�[�N���X
class CStageManagerState
{
public:
	CStageManagerState() {};                              //�R���X�g���N�^
	virtual ~CStageManagerState() {};                     //�f�X�g���N�^
	virtual void Process(CStageManager* pStageManager) {};//����
};

//�V�����I�u�W�F�N�g�̔z�u���s�����
class CStageManagerState_NewObject : public CStageManagerState
{
public:
	CStageManagerState_NewObject();                     //�R���X�g���N�^
	~CStageManagerState_NewObject() override;           //�f�X�g���N�^
	void Process(CStageManager* pStageManager) override;//����
private:
	CObject* m_pManagerObject;                                              //�ݒu�\��I�u�W�F�N�g�ւ̃|�C���^

	void TypeChenge(CStageManager* pStageManager);                          //�I�u�W�F�N�gX�̎�ނ�ς���
	void SetObjectX(CStageManager* pStageManager);                          //�I�u�W�F�N�gX��ݒ肷��
	void DeleteManagerObject(CStageManager* pStageManager);                 //�I�u�W�F�N�gX������
	void ChengeObject(CStageManager * pStageManager);                       //�I�u�W�F�N�g�̎�ނ�����
	void ReleaseObject(CStageManager * pStageManager);                      //�I�u�W�F�N�g�������[�X����

};

//�����̃I�u�W�F�N�g�̕ҏW���s�����
class CStageManagerState_PlacedObject : public CStageManagerState
{
public:
	CStageManagerState_PlacedObject();                                           //�R���X�g���N�^
	~CStageManagerState_PlacedObject() override;                                 //�f�X�g���N�^
	void Process(CStageManager* pStageManager) override;                         //����
private:
	void ChooseObject(CStageManager * pStageManager);                            //�I�u�W�F�N�g��I�ԏ���

	void ChengeIdx(CStageManager* pStageManager);                                //�I�u�W�F�N�g���X�g���w��A�h���X�܂Ői�߂邽�߂̔ԍ���ς���  
	void DeleteObject(list<CObject*>::iterator & it,list<CObject*> & StgObjList);//�I�u�W�F�N�g��j������
	int m_nChooseObjIdx;                                                         //���ݑI��ł���I�u�W�F�N�g�̔ԍ�
};

#endif
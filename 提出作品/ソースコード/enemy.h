//====================================================
//
//�P�O���Q�X���F�G�����[enemy.h]
//Author:ShinaTaiyo
//
//====================================================

#ifndef _ENEMY_H_  
#define _ENEMY_H_

//======================
//�C���N���[�h
//======================
#include "main.h"
#include "objectX.h"
#include "lockon.h"
#include "aimodel.h"
#include "attack.h"
//==========================================

//===========================================
//�O���錾
//===========================================
class CEnemyMove;
class CAttackEnemy;
class CUi;

//===========================================
//�G�l�~�[�N���X
//===========================================
class CEnemy : public CObjectX
{
public:
	struct MoveAiInfo
	{
		D3DXVECTOR3 Pos;
		D3DXVECTOR3 Rot;
		D3DXVECTOR3 Scale;
	};

	enum class ENEMYTYPE
	{
		SHOTWEAK = 0,//�ˌ��Ɏア�G
		DIVEWEAK,    //�_�C�u�Ɏア�G
		IDLE,        //�������Ȃ��G
		MAX
	};

	enum class STATE
	{
		NORMAL = 0,
		FRIGHTENDED,
		MAX
	};

	CEnemy(int nPri = 0, bool bUseintPri = false, CObject::TYPE type = CObject::TYPE::ENEMY, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_X);//�R���X�g���N�^
	~CEnemy();                                                              //�f�X�g���N�^
	HRESULT Init() override;                                                //����������
	void Uninit() override;                                                 //�I������
	void Update() override;                                                 //�X�V����
	void Draw() override;                                                   //�`�揈��
	void SetDeath() override;                                               //���S�t���O��ݒ�
	const ENEMYTYPE& GetEnemyType() const { return m_Type; }                //�G�̎�ނ��擾����
	//==========================================================
    //�G�f�B�^�֌W
    //==========================================================
    //�֐�
	void SaveInfoTxt(fstream& WritingFile) override;                                              //�e�L�X�g�t�@�C���ɏ���ۑ����邽�߂̊֐�
	//static void LoadInfoTxt(fstream& LoadingFile, list<CObject*>& listSaveManager, string& Buff); //�e�L�X�g�t�@�C���������ǂݍ��ނ��߂̊֐�   
	void LoadInfoTxt(fstream& LoadingFile, list<CObject*>& listSaveManager, string& Buff, CObject* pObj) override;//�e�L�X�g�t�@�C���������ǂݍ��ނ��߂̊֐�
	CObject* ManagerSaveObject() override;                                                        //�X�e�[�W�}�l�[�W���[�ɍ��̃I�u�W�F�N�g��ۑ�����
	void ManagerChooseControlInfo() override;                                                     //�X�e�[�W�}�l�[�W���[���瑀�삷��
	void SetPhaseNum(int nNum) { m_nPhaseNum = nNum; }                                            //�t�F�[�Y�ԍ���ݒ肷��
	const int& GetPhaseNum() const { return m_nPhaseNum; }                                        //�t�F�[�Y�ԍ����擾����   
 	//=================================================================================================================

	//==========================================================
	//�s��
	//==========================================================
	void SetVecMoveAiInfo(vector<CAIModel*> vec);                    //�ړ�AI�̓��I�z��ݒ�
	vector<CAIModel*>& GetVecAiModelInfo() { return m_VecMoveAi; }   //�ړ�AI�̓��I�z��̎擾
	virtual void AIMoveProcess();                                    //AI�ړ�����
	virtual void BattleMoveProcess();                                //�o�g���ړ�����
	void ChengeMove(CEnemyMove* pEnemyMove);                         //�ړ���Ԃ�ς���
	//=================================================================================================================

	//==========================================================
	//�o�g���֌W
	//==========================================================
	const int GetCntTime() const { return m_nCntTime; }               //���Ԃ��擾
	void SetCntTime(int nTime) { m_nCntTime = nTime; }                //���Ԃ�ݒ�i�^�C�~���O�����炷�Ȃǂ��ł���j
	//���m�˒�														  
	void SetSensingRange(float fRange) { m_fSensingRange = fRange; }  //���m�˒���ݒ�
	const float & GetSensingRange() const { return m_fSensingRange; } //���m�˒����擾

	//�ʏ�ړ����x
	void SetNormalSpeed(float fSpeed) { m_fNormalSpeed = fSpeed; }    //�ʏ�ړ����x��ݒ�
	const float& GetNormalSpeed() const { return m_fNormalSpeed; }    //�ʏ�ړ����x���擾

	//�|���ꂽ�U���^�C�v��ݒ肷��
	void SetDefeatAttack(CAttack::ATTACKTYPE Type) { m_DefeatAttackType = Type; }        //�|���ꂽ�U���^�C�v��ݒ�
	const CAttack::ATTACKTYPE GetDefeatAttackType() const { return m_DefeatAttackType; } //�|���ꂽ�U���^�C�v���擾

	const bool& GetCollisionWall() const { return m_bCollisionWall; }                    //�ǂɏՓ˂������ǂ������擾

	//�p�^�[��
	void SetAction(bool bAction) { m_Pattern.bAction = bAction; }                        //�U���𔭓������邩�ǂ�����ݒ�
	const bool& GetAction() const { return m_Pattern.bAction; }                          //�U���𔭓����Ă��邩�ǂ������擾
	 
	void EndAttackPattern();                                                             //�U���������I��������Ƃ��ɐݒ肷�����ݒ肷��
	const int GetAttackCoolTime() { return m_nAttackCoolTime; }                          //���̍U���𔭓�����܂ł̃N�[���^�C�����擾����
	void SetAttackCoolTime(int nTime) { m_nAttackCoolTime = nTime; }                     //���̍U���𔭓�����܂ł̃N�[���^�C����ݒ肷��
	static int GetNumEnemy() { return m_nNumEnemy; }                                     //���݂̓G�̐����擾����

	void SetState(STATE State) { m_State = State; }                                      //��Ԉُ��ݒ肷��
	const STATE& GetState() { return m_State; }                                          //��Ԉُ���擾����

	//�W�����v�܂ł̃N�[���^�C��
	void ResetJumpCoolTime() { m_nJumpCoolTime = 0; }                                    //�W�����v�\�ɂȂ�܂ł̃N�[���^�C�������Z�b�g����
	int GetJumpCoolTime() { return m_nJumpCoolTime; }                                    //�W�����v�\�ɂȂ�܂ł̃N�[���^�C�����擾����

	void SetPossibleAttack(bool bPossible) { m_bPossibleAttack = bPossible; }            //�U���\���ǂ�����ݒ肷��
	//=================================================================================================================
protected:

	struct Pattern
	{//�U���p�^�[�����\����
		int nPattern;
		int nSubPattern;
		int nPatternTime;
		bool bAction;
	};

	//================================================
	//�ϐ��錾
	//================================================
	void SetEnemyType(ENEMYTYPE type) { m_Type = type; }//�G�̎�ނ�ݒ�
	//===============================================================================================
	
	//================================================
	//�v���g�^�C�v�錾
	//================================================
	void SetPattern(int nPattern) { m_Pattern.nPattern = nPattern; }                     //�U���p�^�[���ԍ���ݒ�
	const int& GetPattern() const { return m_Pattern.nPattern; }                         //�U���p�^�[���ԍ����擾

	void SetSubPattern(int nSubPattern) { m_Pattern.nSubPattern = nSubPattern; }         //�U���̃T�u�p�^�[���ԍ���ݒ�
	const int& GetSubpattern() const { return m_Pattern.nSubPattern; }                   //�U���̃T�u�p�^�[���ԍ����擾

	void SetPatternTime(int nPatternTime) { m_Pattern.nPatternTime = nPatternTime; }     //�U���p�^�[���̎��Ԃ�ݒ�
	const int& GetPatternTime() const { return m_Pattern.nPatternTime; }                 //�U���p�^�[���̎��Ԃ��擾

	void SetUseCollisionDetection(bool bUse) { m_bCollisoinDetection = bUse; }           //���̃I�u�W�F�N�g�ɏd�Ȃ�Ȃ��悤�ɂ��邩�ǂ�����ݒ�

	const bool& GetCollisionDetection() const { return m_bActivateCollisionDetection; }  //���̃I�u�W�F�N�g�ɏd�Ȃ�Ȃ��悤�ɂ��Ă��邩�ǂ������擾

	void RayCollision();                                                                 //���C���I�u�W�F�N�g�ɓ������Ă��邩�ǂ���

	//�ړ�����
	void ChasePlayer();                                                                  //�v���C���[��ǂ������鏈��

	virtual void DefeatStaging();                                                        //�|���ꂽ�Ƃ��̉��o�i�h���N���X�ŋ����ύX)
	//===============================================================================================
private:
	//================================================
	//�ÓI�����o
	//================================================
	static int m_nNumEnemy;//�G�̑���
	//===============================================================================================

	//================================================
	//�ϐ��錾
	//================================================
	CEnemyMove* m_pEnemyMove;                //�G�̈ړ��X�e�[�g
	vector<CAIModel*> m_VecMoveAi;           //�ړ�AI���f�����I�z��
	ENEMYTYPE m_Type;                        //�G�̎��
	D3DXVECTOR3 m_MoveAiSavePos;             //�ړ�AI�̕ۑ�����ʒu
	int m_nIdxMoveAi;                        //�ړ�AI�̈ړ��ʒu�̔z����w�肷��
	int m_nPhaseNum;                         //�t�F�[�Y�ԍ�
	int m_nCntTime;                          //���Ԃ𑪂�
	float m_fRotMove;                        //�G�̌������z�[�~���O������Ƃ��Ɏg�p����
									         
	float m_fSensingRange;                   //���m�˒�
	float m_fNormalSpeed;                    //�ʏ�ړ����x
									         
	Pattern m_Pattern;                       //�p�^�[��
										     
	bool m_bCollisoinDetection;              //�Փ˔�����s�����ǂ���
	bool m_bActivateCollisionDetection;      //�Փ˔��蔭��

	CAttack::ATTACKTYPE m_DefeatAttackType;  //�|���ꂽ�U��

	bool m_bCollisionWall;                   //�ǂƏՓ˂������ǂ���
	int m_nAttackCoolTime;                   //�U���̃N�[���^�C���J�E���g�p

	int m_nJumpCoolTime;                     //�W�����v�܂ł̃N�[���^�C��

	bool m_bPossibleAttack;                  //�U���\���ǂ���
	bool m_bStartLanding;                    //�ŏ��ɒn�ʂɂ������ǂ����i�Ԃ��G���ŏ��ɏ�����I�u�W�F�N�g�̏�ɌŒ肷��Ƃ��ȂǂɎg��)
	STATE m_State;                           //��ԗ񋓌^
	//===============================================================================================

	//================================================
	//�v���g�^�C�v�錾
	//================================================
	void CollisionProcess();                //�����蔻����s��
	void SetMoveAiPoint();                  //�ړ�AI�̔z�u������
	void PhaseNumDecision();                //�t�F�[�Y�ԍ���ݒ肷��
	virtual void AttackProcess();           //�U������
	void CollisionDetectionProcess();       //�Փ˔��菈��
	void CheckGrroundDistance_AdjustPos();  //�n�ʂƂ̋��������߂āA���ȉ��Ȃ�1f�O�̈ʒu�ɖ߂�

	//*�X�e�[�^�X�G�f�B�b�g
	void EditNormalSpeed();                 //�ړ����x
	void EditSensingRange();                //���G�͈�
	//===============================================================================================
};

//===========================================
//�ˌ��Ɏア�G�l�~�[�N���X
//===========================================
class CShotWeakEnemy : public CEnemy
{
public:
	enum class SHOTWEAKENEMYTYPE
	{//�ˌ��Ɏア�G�̎��
		NORMAL = 0,//����
		MAX
	};

	CShotWeakEnemy(int nPri = 0, bool bUseintPri = false, CObject::TYPE type = CObject::TYPE::ENEMY, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_X);//�R���X�g���N�^                   //�R���X�g���N�^
	~CShotWeakEnemy();                  //�f�X�g���N�^
	HRESULT Init() override;            //����������
	void Uninit() override;             //�I������
	void Update() override;             //�X�V����
	void Draw() override;               //�`�揈��
	void SetDeath() override;           //���S�t���O��ݒ�
	static CShotWeakEnemy* Create(SHOTWEAKENEMYTYPE Type, int nLife,int nPhaseNum,D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 Scale);//��������

	//��
	void SetShotWeakEnemyType(SHOTWEAKENEMYTYPE Type) { m_ShotWeakEnemyType = Type; }//�ˌ��Ɏア�G�^�C�v�ݒ�
	SHOTWEAKENEMYTYPE GetShotWeakEnemyType() { return m_ShotWeakEnemyType; }         //�ˌ��Ɏア�G�^�C�v���擾

	//==========================================================
    //�G�f�B�^�֌W
    //==========================================================
    //�֐�
	void SaveInfoTxt(fstream& WritingFile) override;                                               //�e�L�X�g�t�@�C���ɏ���ۑ����邽�߂̊֐�
	//static void LoadInfoTxt(fstream& LoadingFile, list<CObject*>& listSaveManager, string& Buff);  //�e�L�X�g�t�@�C���������ǂݍ��ނ��߂̊֐�
	void LoadInfoTxt(fstream& LoadingFile, list<CObject*>& listSaveManager, string& Buff, CObject* pObj) override;//�e�L�X�g�t�@�C���������ǂݍ��ނ��߂̊֐�
	CObject* ManagerChengeObject(bool bAim) override;                                              //�X�e�[�W�}�l�[�W���[�ɕύX�����I�u�W�F�N�g��n��
	CObject* ManagerSaveObject() override;                                                         //�X�e�[�W�}�l�[�W���[�ɍ��̃I�u�W�F�N�g��ۑ�����
	//=================================================================================================================
protected:
private:
	//================================================
	//�ÓI�����o
	//================================================
	static const string s_aSHOTWEAKENEMY_FILENAME[static_cast<int>(SHOTWEAKENEMYTYPE::MAX)];       //�G�̃��f���t�@�C����
	static const float s_fATTACKSTART_LENGTH;                                                      //�ˌ��Ɏア�G�̍U���J�n����
	static const float s_fNORMAL_SENSIINGRANGE;                                                    //�ʏ���G�͈�
	static const int s_nATTACK_COOLTIME;                                                           //�U���̃N�[���^�C��
	//===============================================================================================

	//================================================
	//�ϐ��錾
	//================================================
	CAttackEnemy* m_pMagicSword;                                                                  //���@��
	SHOTWEAKENEMYTYPE m_ShotWeakEnemyType;                                                        //�G�̎��
	D3DXVECTOR3 m_SaveAimPos;                                                                     //�ړI�̈ʒu�ۑ��p
	//===============================================================================================

	//================================================
	//�v���g�^�C�v�錾
	//================================================

	//*�ړ�AI
	void BattleMoveProcess() override;                                                            //�o�g���ړ�����
	void AttackProcess() override;                                                                //�U������
	void DefeatStaging() override;                                                                //�|���ꂽ�Ƃ��̉��o
	void SwordCollision();                                                                        //���̓����蔻����s��

	//�ړ���ԊǗ��p�̃��C
	void RayCollisionJumpOverOnHit();                                                             //���C�������������щz���鏈���Ɉڍs������֐�
	//===============================================================================================
};

//===========================================
//�_�C�u�Ɏア�G�l�~�[�N���X
//===========================================
class CDiveWeakEnemy : public CEnemy
{
public:
	enum class DIVEWEAKENEMYTYPE
	{//�_�C�u�Ɏア�G�̎��
		NORMAL = 0,
		MAX
	};

	CDiveWeakEnemy(int nPri = 0, bool bUseintPri = false, CObject::TYPE type = CObject::TYPE::ENEMY, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_X);//�R���X�g���N�^
	~CDiveWeakEnemy();                  //�f�X�g���N�^
	HRESULT Init() override;            //����������
	void Uninit() override;             //�I������
	void Update() override;             //�X�V����
	void Draw() override;               //�`�揈��
	void SetDeath() override;           //���S�t���O��ݒ�
	static CDiveWeakEnemy* Create(DIVEWEAKENEMYTYPE Type, int nLife,int nPhaseNum,D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 Scale,int nDivisionNum);//��������

	//��
	void SetDiveWeakEnemyType(DIVEWEAKENEMYTYPE Type) { m_DiveWeakEnemyType = Type; }//�_�C�u�Ɏア�G�̎�ނ�ݒ�
	DIVEWEAKENEMYTYPE GetDiveWeakEnemyType() { return m_DiveWeakEnemyType; }         //�_�C�u�Ɏア�G�̎�ނ��擾
	//==========================================================
    //�G�f�B�^�֌W
    //==========================================================
    //�֐�
	void SaveInfoTxt(fstream& WritingFile) override;                                               //�e�L�X�g�t�@�C���ɏ���ۑ����邽�߂̊֐�
	//static void LoadInfoTxt(fstream& LoadingFile, list<CObject*>& listSaveManager, string& Buff);  //�e�L�X�g�t�@�C���������ǂݍ��ނ��߂̊֐�
	void LoadInfoTxt(fstream& LoadingFile, list<CObject*>& listSaveManager, string& Buff, CObject* pObj) override;//�e�L�X�g�t�@�C���������ǂݍ��ނ��߂̊֐�
	CObject* ManagerChengeObject(bool bAim) override;                                              //�X�e�[�W�}�l�[�W���[�ɕύX�����I�u�W�F�N�g��n��
	CObject* ManagerSaveObject() override;                                                         //�X�e�[�W�}�l�[�W���[�ɍ��̃I�u�W�F�N�g��ۑ�����
	void ManagerChooseControlInfo() override;                                                      //�X�e�[�W�}�l�[�W���[���瑀�삷��
	//=================================================================================================================

	//==========================================================
	//�X�e�[�^�X�֌W
	//==========================================================
	void SetDivisionNum(int nNum) {m_nDivisionNum = nNum;}                                         //����񐔂�ݒ肷��i�Ԃ��G�͎ˌ��œ|���ƕ��􂷂�)
	const int& GetDivisionNum() const { return m_nDivisionNum; }                                   //����񐔂��擾����
	//===============================================================================================
protected:
private:
	//================================================
	//�ÓI�����o
	//================================================
	static const string s_aDIVEWEAKENEMY_FILENAME[static_cast<int>(DIVEWEAKENEMYTYPE::MAX)]; //�G�̃��f���t�@�C����
	static const int s_nATTACK_FREQUENCY;                                                    //�U���p�x
	static const float s_fSENSINGRANGE;                                                      //���m�˒�
	static const float s_fNORMAL_SPEED;                                                      //�ʏ�̑��x
	//===============================================================================================

	//================================================
	//�ϐ��錾
	//================================================
	DIVEWEAKENEMYTYPE m_DiveWeakEnemyType;                                                   //�G�̎��
	bool m_bStartDeath;                                                                      //������J�n����
	int m_nDivisionNum;                                                                      //�����
	//===============================================================================================

	//================================================
	//�v���g�^�C�v�錾
	//================================================
	
	//*�U���ړ�
	void BattleMoveProcess() override;

	//�U������
	void AttackProcess() override;

	//AI�ړ�
	void AIMoveProcess() override;
	void DefeatStaging() override;//�|���ꂽ�Ƃ��̉��o

	//===============================================================================================
};

//===========================================
//�������Ȃ��G�N���X
//===========================================
class CIdleEnemy : public CEnemy
{
public:
	enum class IDLEENEMYTYPE
	{//�������Ȃ��G�̎��
		NORMAL = 0,
		MAX
	};

	//==========================================================
	//��{
	//==========================================================
	CIdleEnemy(int nPri = 0, bool bUseintPri = false, CObject::TYPE type = CObject::TYPE::ENEMY, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_X);//�R���X�g���N�^
	~CIdleEnemy();                      //�f�X�g���N�^
	HRESULT Init() override;            //����������
	void Uninit() override;             //�I������
	void Update() override;             //�X�V����
	void Draw() override;               //�`�揈��
	void SetDeath() override;           //���S�t���O��ݒ�
	static CIdleEnemy* Create(IDLEENEMYTYPE Type, int nLife, int nPhaseNum, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 Scale);//��������
	//=================================================================================================================

	//��
	void SetIdleEnemyType(IDLEENEMYTYPE Type) { m_IdleEnemyType = Type; }//�������Ȃ��G�̎�ނ�ݒ�
	IDLEENEMYTYPE GetIdleEnemyType() { return m_IdleEnemyType; }         //�������Ȃ��G�̎�ނ�ݒ�

	//==========================================================
    //�G�f�B�^�֌W
    //==========================================================
    //�֐�
	void SaveInfoTxt(fstream& WritingFile) override;                                               //�e�L�X�g�t�@�C���ɏ���ۑ����邽�߂̊֐�
	//static void LoadInfoTxt(fstream& LoadingFile, list<CObject*>& listSaveManager, string& Buff);  //�e�L�X�g�t�@�C���������ǂݍ��ނ��߂̊֐�
	void LoadInfoTxt(fstream& LoadingFile, list<CObject*>& listSaveManager, string& Buff, CObject* pObj) override;//�e�L�X�g�t�@�C���������ǂݍ��ނ��߂̊֐�
	CObject* ManagerChengeObject(bool bAim) override;                                              //�X�e�[�W�}�l�[�W���[�ɕύX�����I�u�W�F�N�g��n��
	CObject* ManagerSaveObject() override;                                                         //�X�e�[�W�}�l�[�W���[�ɍ��̃I�u�W�F�N�g��ۑ�����
	void ManagerChooseControlInfo() override;                                                      //�X�e�[�W�}�l�[�W���[���瑀�삷��
	//=================================================================================================================
private:
	//================================================
    //�ÓI�����o
    //================================================
	static const string s_aIDLEENEMY_FILENAME[static_cast<int>(IDLEENEMYTYPE::MAX)];  //�G�̃��f���t�@�C����
	//===============================================================================================

	//================================================
	//�ϐ��錾
	//================================================
	IDLEENEMYTYPE m_IdleEnemyType = IDLEENEMYTYPE::NORMAL;                            //�G�̎��
	//===============================================================================================
	
	//================================================
	//�v���g�^�C�v�錾
	//================================================
	
	//*�U���ړ�
	void BattleMoveProcess() override;

	//�U������
	void AttackProcess() override;

	//AI�ړ�
	void AIMoveProcess() override;

	//�|���ꂽ�Ƃ��̉��o
	void DefeatStaging() override;
	//===============================================================================================

};

//**************************************************************************************
//�G�̏�ԃN���X
//**************************************************************************************

//================================
//�ړ��N���X
//================================

//�X�[�p�[�N���X
class CEnemyMove
{
public:
	CEnemyMove();                         //�R���X�g���N�^
	virtual ~CEnemyMove();                //�f�X�g���N�^
	virtual void Process(CEnemy* pEnemy); //����
};

//�ړ��^�C�v�FAI
class CEnemyMove_AI : public CEnemyMove
{
public:
	CEnemyMove_AI();                        //�R���X�g���N�^
	~CEnemyMove_AI() override;              //�f�X�g���N�^
	void Process(CEnemy* pEnemy) override;  //����
};

//�ړ��^�C�v�F�o�g��
class CEnemyMove_Battle : public CEnemyMove
{
public:
	CEnemyMove_Battle();                    //�R���X�g���N�^
	~CEnemyMove_Battle() override;          //�f�X�g���N�^
	void Process(CEnemy* pEnemy) override;  //����
};

//�ړ��^�C�v�F�ǂ悯
class CEnemyMove_DodgeWall : public CEnemyMove
{
public:
	CEnemyMove_DodgeWall(CEnemy * pEnemy,D3DXVECTOR3 DodgeMove);  //�R���X�g���N�^
	~CEnemyMove_DodgeWall() override;                             //�f�X�g���N�^
	void Process(CEnemy* pEnemy)override;                         //����
private:
	D3DXVECTOR3 m_DodgeMove;                                      //����ړ���
};

//�ړ��^�C�v�F����
class CEnemyMove_Frightened : public CEnemyMove
{
public:
	CEnemyMove_Frightened(CEnemy * pEnemy,D3DXVECTOR3 StopPos,int nStateTime);//�R���X�g���N�^
	~CEnemyMove_Frightened() override;                                        //�f�X�g���N�^
	void Process(CEnemy* pEnemy) override;                                    //����
private:
	D3DXVECTOR3 m_StopPos;                                                    //�~�߂�ʒu
	CUi* m_pLockOn;                                                           //���b�N�I������
	int m_nStateTime;                                                         //������ԂɂȂ鎞�Ԃ�ݒ�
};

//�ړ��^�C�v�F�ǒ����W�����v
class CEnemyMove_OverJumpObj : public CEnemyMove
{
public:
	CEnemyMove_OverJumpObj(CEnemy* pEnemy,float fGoalheight);   //�R���X�g���N�^
	~CEnemyMove_OverJumpObj() override;                         //�f�X�g���N�^
	void Process(CEnemy* pEnemy) override;                      //����
private:
	float m_fGoalHeight = 0.0f;                                 //�ǂ𒴂��鎞�̖ڕW�̍���
	int m_nCntTime = 0;                                         //���̏�ԂɂȂ��Ă���̎��Ԃ��J�E���g����
};

//�ړ��^�C�v�F�Ȃ�
class CEnemyMove_None : public CEnemyMove
{
public:
	CEnemyMove_None();                                         //�R���X�g���N�^
	~CEnemyMove_None() override;                               //�f�X�g���N�^
	void Process(CEnemy* pEnemy) override;                     //����
};

//===================================================================================================================================

#endif

//====================================================
//
//�V���S���F�`���[�W�U������������[player.h]
//Author:ShinaTaiyo
//
//====================================================

#ifndef _PLAYER_H_  
#define _PLAYER_H_

//======================
//�C���N���[�h
//======================
#include "main.h"
#include "object2d.h"
#include "objectX.h"
#include "modelparts.h"
#include "Ui3D.h"
#include "meshorbit.h"
#include "ui.h"
#include "magic.h"
//==========================================

//======================
//�O���錾
//======================
class CInputKeyboard;
class CInputJoypad;
class CGauge;
class CNumber;
class CScore;
//==========================================

//===========================================
//�v���C���[3D�N���X
//===========================================
class CPlayer : public CObjectX
{
public:

	//===========================
    //�A�N�V�����̎�ޗ񋓌^
    //===========================
	typedef enum
	{
		ACTIONTYPE00_DOUBLEJUMP = 0,//2�i�W�����v
		ACTIONTYPE01_DODGE,         //���
		ACTIONTYPE02_CHARGEATTACK,  //�J�E���^�[
		ACTIONTYPE03_WALLJUMP,      //�ǃW�����v
		ACTIONTYPE_MAX
	}ACTIONTYPE;
	//========================================================================================

	//===========================
	//�o���b�g�Z��ޗ񋓌^
	//===========================
	typedef enum
	{
		BULLETACTION00_CHARGE = 0,//�`���[�W�V���b�g
		BULLETACTION01_RAINSHOT,  //���C���V���b�g
		BULLETACTION02_BURSTSHOT, //�o�[�X�g�V���b�g
		BULLETACTION03_RAPIDSHOT, //���s�b�h�V���b�g
		BULLETACTION_MAX
	}BULLETACTION;
	//========================================================================================

	//==========================
	//�R�}���h�����񋓌^
	//==========================
	typedef enum
	{
		PLAYERCOMMAND00_MOVE = 0,    //���������ǂ����𔻒�
		PLAYERCOMMAND01_JUMP,        //�W�����v�������ǂ����𔻒�
		PLAYERCOMMAND02_DASH,        //�_�b�V���������ǂ����𔻒�
		PLAYERCOMMAND03_WALLJUMP,    //�ǃW�����v���������ǂ����𔻒�
		PLAYERCOMMAND04_SHOT,        //�V���b�g���������ǂ����𔻒�
		PLAYERCOMMAND05_CHARGESHOT,  //�`���[�W�V���b�g���������ǂ����𔻒�
		PLAYERCOMMAND06_RAINSHOT,    //���C���V���b�g���������ǂ����𔻒�
		PLAYERCOMMAND07_BURSTSHOT,   //�o�[�X�g�V���b�g���������ǂ����𔻒�
		PLAYERCOMMAND08_RAPIDSHOT,   //���s�b�h�V���b�g�������ǂ����𔻒�
		PLAYERCOMMAND09_MAGIC,       //���@���g�p�������ǂ����𔻒�
		PLAYERCOMMAND10_MAGICCHENGE, //���@��ύX�������ǂ����𔻒�
		PLAYERCOMMAND11_DOUBLEJUMP,  //�Q�i�W�����v���������ǂ����𔻒�
		PLAYERCOMMAND_MAX,
	}PLAYERCOMMAND;

	CPlayer();                  //�R���X�g���N�^
	~CPlayer();                 //�f�X�g���N�^
	HRESULT Init() override;    //����������
	void Uninit() override;     //�I������
	void ExtraUninit() override;//�ʘg�̏I������
	void Update() override;     //�X�V����
	void Draw() override;       //�`�揈��
	void SetDeath() override;   //���S�t���O��ݒ�
	static void Unload();       //���f���̏��̔j��
	static CPlayer * Create(D3DXVECTOR3 pos,D3DXVECTOR3 rot,D3DXVECTOR3 move,D3DXVECTOR3 Scale);
	static int GetNumFile() { return m_nNumFile; }                                                                 //�t�@�C�����擾�p
	void SetAddMove(D3DXVECTOR3 move) { m_AddMove += move; }                                                       //���Z����ړ��ʂ�ݒ肷��
	void StopMove() { m_bMove = false; }                                                                           //���Z����ړ��ʂ�ݒ肷��
	void NotSetMove(bool bX, bool bY, bool bZ);                                                                    //���f���Ȃ��ړ��ʂ�ݒ肷��
	void SetComplusionPos(D3DXVECTOR3 pos);                                                                        //���Z����ʒu��ݒ肷��
	void ReleasePlayer();//�v���C���[��j������
	void SetSlideState(bool bState) { m_State.SlideState.bState = bState; }                                        //�����Ԃ�ݒ�

	//========================================
	//�X�e�[�^�X�֘A
	//========================================
	void SetDamage(int nDamage, int nHitStopTime) override;                                                        //�_���[�W��^����
	int& GetHp() { return m_Status.nHp; }                                                                          //�̗͂��擾����
	void SetMp(int nMp);                                                                                           //MP��ݒ肷��
	void SetDecelerationState(bool bState) { m_State.DecelerationState.bState = bState; m_State.DecelerationState.nStateCnt = m_nDECELERATIONCNT; }//������Ԃ�ݒ�
	void AddPlayerAtkBuff(float fAdd) { m_Status.fAtkBuff += fAdd; }          //�U���o�t�����Z����
	float GetPlayerAtkBuff() { return m_Status.fAtkBuff; }                    //�U���o�t���擾����
	//===================================================================================================================================================================

	//=======================================
	//�A�N�V�����֘A
	//=======================================
	void SetUnlockAction(ACTIONTYPE type) { m_bUnlockAction[type] = true; }                                        //�I�񂾃A�N�V�������J������
	void SetUnlockBulletAction(BULLETACTION type) { m_bUnlockBulletAction[type] = true; }                          //�e�̋Z���J������
    //===================================================================================================================================================================

	//=======================================
	//�X�R�A�֘A
	//=======================================
	CScore* GetScore() { return m_pScore; }//�v���C���[�������Ă���X�R�A���擾
	//===================================================================================================================================================================

	//=======================================
	//�R�}���h�֘A
	//=======================================
	bool GetJudgeCommand(PLAYERCOMMAND Command) { return m_bJudgeCommand[Command];}
	//===================================================================================================================================================================

	//=======================================
	//���@�֘A
	//=======================================
	void SetUnlockMagic(int nMagicNum,D3DXVECTOR3 Pos,bool bUseSignBoard);
	int GetNumPlayerMagic() { return m_nMagicNum; }//�v���C���[�̖��@�̑������擾����
	bool* GetUnlockMagic() { return &m_bUnlockMagic[0];}//�S�Ă̖��@���J���ς݂��ǂ������擾����
	//===================================================================================================================================================================

	//=======================================
	//���X�|�[���֌W
	//=======================================
	void SetReSpawnPos(D3DXVECTOR3 Pos) { m_ReSpawnPos = Pos; }//�v���C���[�̃��X�|�[���ʒu��ݒ肷��
	void ActivePlayerRespawn();                                //�v���C���[�̃��X�|�[�������𔭓�����
	void ResetRespawnCnt() { m_nRespawnCnt = 0; }
	//===================================================================================================================================================================

	//=======================================
	//�����֌W
	//=======================================
	void SetPlayerSlideInertia(float fInertia);//���点�邽�߂̊�����ݒ肷��
	//===================================================================================================================================================================
	
	//=======================================
	//�̗͊֘A
	//=======================================
	void SetHeal(int nHeal, D3DXCOLOR col, float fWidth, float fHeight) override;                  //�񕜗ʂ�^����()
	//===================================================================================================================================================================

	//=======================================
	//�ǃW�����v�֌W
	//=======================================
	void SetWallJumpBindTime(int nTime) { m_nWallJumpBindTime = nTime; }//�ǃW�����v�}�����Ԃ�ݒ肷��
	//===================================================================================================================================================================

	//=======================================
	//��֌W
	//=======================================
	typedef enum
	{
		PLAYERREACTION_NORMAL = 0,//���A�N�V����-����
		PLAYERREACTION_DAMAGE,    //���A�N�V����-�_���[�W
		PLAYERREACTION_GLAD,      //���A�N�V����-������
		PLAYERREACTION_MAX
	}PLAYERREACTION;
	void SetPlayerReaction(PLAYERREACTION ReactionType, int nReactionTime);//�v���C���[�̃��A�N�V������ݒ肷��
	//===================================================================================================================================================================
private:   
	static const float m_fNORMALJUMPPOWER;                    //�ʏ�̃W�����v��
	static const float m_fNORMALSPEED;        
	static const float m_fGRAVITYPOWER;                       //�d�͂̑傫��
	static const int m_nMAX_PLAYERPARTS = 11;                 //�v���C���[�̃p�[�c�̍ő吔
	static const int m_nMAX_KEYSET = 30;                      //�v���C���[�̃L�[�̍ő吔
	static const int m_nMAX_MOTION = 5;                       //���[�V�����ő吔
	static const int m_nDECELERATIONCNT = 20;                 //������Ԃ̎���
	static const int m_nMAX_MAGICNUM = 10;                                  //���Ă閂�@�̍ő吔

	//===============================
	//�o���b�g�֌W
	//===============================
	static const int m_nRAINSHOTBULLET = 12;                  //���C���V���b�g�̊�{�I�ȃo���b�g��
	static const int m_nRAINSHOT_POWER = 5;                   //���C���V���b�g�̈З�
	static const int m_nBURSTSHOTBULLET = 12;                 //�o�[�X�g�V���b�g�̊�{�I�ȃo���b�g��
	static const int m_nBURSTSHOT_POWER = 5;                  //�o�[�X�g�V���b�g�̈З�
	static const int m_nNORMALSHOT_POWER = 5;               //�m�[�}���V���b�g�̈З�
	static const int m_nRAPIDSHOT_POWER = 30;                 //���s�b�h�V���b�g�̈З�
	//===============================
	//�e�햂�@����MP
	//===============================
	static const int m_nPLAYERUSAGEMP[CMagicDisp::MAGICDISP_MAX];//���ꂼ��̖��@�̏���MP
	static const int m_nQUICKHEALONEMP = 40;//���̃q�[���̉񕜂Ɏg��MP
	//===========================================================================================================

	//===============================
	//�e���@�З�
	//===============================
	static const int m_nFIREBALL_POWER = 50;                  //�t�@�C�A�{�[���̈З�
	static const int m_nBEAMWHIP_POWER = 120;                 //�r�[���E�B�b�v�̈З�
	static const int m_nFLOWERDICE_POWER = 60;                //�t�����[�h�A�C�X�̈З�
	static const int m_nTHUNDERBURST_POWER = 3;               //�T���_�[�o�[�X�g�̈З�
	static const int m_nMAGICSWORD_POWER = 125;               //�}�W�b�N�\�[�h�̈З�
	static const int m_nPLANTSHIELD_POWER = 20;               //�v�����g�V�[���h�̈З�
	static const int m_nCROSSBOMB_POWER = 70;                 //�N���X�{���̈З�
	static const int m_nBLACKHOLE_POWER = 200;                //�u���b�N�z�[���̈З�
	static const int m_nPHYCOBURST_POWER = 60;                //�T�C�R�o�[�X�g�̈З�
	//==========================================================================================================

	//==============================
	//�X�e�[�^�X�֌W
	//==============================
	static const int m_nINIT_HP = 20; //����HP
	static const int m_nINIT_MP = 150;//����MP
	static constexpr float m_fINIT_ATTACKBUFF = 1.0f;//�����U���{��
	//==========================================================================================================

	//==============================
	//�_���[�W�֌W
	//==============================
	int DamageResult(int nDamage) { return (int)(nDamage * m_Status.fAtkBuff); }//�U���o�t�{�����Q�Ƃ����_���[�W���ʂ�Ԃ�
    //==========================================================================================================

	void BlockCollision();                                    //�u���b�N�Ƃ̓����蔻����s��
	void DecelerationProcess();                               //������Ԃ̏���
	typedef struct
	{
		int nHp;             //�̗�
		int nMaxHp;          //�ő�̗�
		int nMp;             //�}�W�b�N�|�C���g
		int nMaxMp;          //�ő�MP
		float fAtkBuff;      //�U���o�t�{��
	}Status;


	//������
	typedef struct
	{
		bool bState;
		int nStateCnt;
	}Slide;

	typedef struct
	{
		bool bState;
		int nStateCnt;
	}Deceleration;

	//��Ԉُ�
	typedef struct
	{
		Slide SlideState;
		Deceleration DecelerationState;
	}State;

	State m_State;                                                          //��ԍ\����

	static LPD3DXMESH m_pMeshTemp;                                          //���b�V���i���_���j�ւ̃|�C���^�i���f���̎�ސ����j
	static LPD3DXBUFFER m_pBuffMatTemp;                                     //�}�e���A���ւ̃|�C���^
	static DWORD m_dwNumMatTemp;                                            //�}�e���A���̐�
	static LPDIRECT3DTEXTURE9 m_pTextureTemp[CObjectX::m_nMAX_MAT];         //�e�N�X�`���ւ̃|�C���^
	static int m_nNumFile;                                                  //�t�@�C������ۑ�����
	float m_fRot;                                                           //�␳�p����
	int m_nUseMagicTime;                                                    //���@�g�p���Ԃ��J�E���g����
	int m_nNextUseMagic;                                                    //���ɖ��@���g�p�\�ɂȂ�܂ł̎���
	int m_nUseMagicNum;                                                     //�ǂ̖��@���g�p���Ă��邩[
	int m_nCntLanding;                                                      //�n�ʂɂ��鎞�Ԃ��J�E���g����
	bool m_bPossibleMagic;                                                  //���̖��@���g�p�\���ǂ���
	bool m_bMove;                                                           //�ړ������ǂ���
	bool m_bJustStopMove;                                                   //�ړ�����u�~�߂�t���O
	bool m_bCompulsionPos;                                                  //�ʒu�������
	D3DXVECTOR3 m_CompulsionPos;                                            //���Z����ʒu
	D3DXVECTOR3 m_SaveMove;                                                 //�ۑ�����ړ���
	D3DXVECTOR3 m_AddMove;                                                  //���Z�ړ���

	float m_fSpeed;                                                         //�ړ����x
	float m_fJump;                                                          //�W�����v��
	float m_fCorrectionInertia;                                             //�����̕␳
	int m_nCollisionCheck = 0;                                              //����m�F
	Status m_Status;                                                        //�X�e�[�^�X
	CNumber* m_pNumber;                                                     //�ԍ�

	int m_nStopMoveCnt;                                                     //�������~�߂鎞�Ԃ��J�E���g����

	//====================================
	//�R�}���h�֌W
	//====================================
	bool m_bJudgeCommand[PLAYERCOMMAND_MAX];//�v���C���[���ǂ̃R�}���h�𔭓��������𔻒�
	void ResetJudgeCommand();               //�R�}���h������Ԃ����Z�b�g
	//===============================================================================================================

	//====================================
	//�Q�[�W�֌W
	//====================================
	CGauge* m_pHpGauge;                                                     //�̗̓Q�[�W
	CGauge* m_pMpGauge;                                                     //MP�Q�[�W
	CGauge* m_pUsageMpGauge;                                                //MP����ʂ�\���Q�[�W
	CUi* m_pGaugeFrame;                                                     //�Q�[�W�p�t���[���𐶐�
	//================================================================================================================

	//====================================
	//��֌W
	//====================================
	CUi* m_pUiReaction;//���UI
	int m_nCntChengeUiReactionTime;
	bool m_bChengeUiReactionFlag;
	void UiReactionProcess();
	//=================================================================================================================

	//====================================
	//���@�֌W
	//====================================
	CMagicDisp* m_pMagicDisp;                                               //���@�\��
	CMagicWindow* m_pMagicWindow;                                           //���@�\���E�C���h�E
	int m_nNowMagicType;                                                    //���݂̖��@�^�C�v
	int m_nMagicNum;                                                        //�����Ă��閂�@�̐�
	int m_nMagicSlot[m_nMAX_MAGICNUM];                                      //���݂̖��@�̏���
	bool m_bUnlockMagic[CMagicDisp::MAGICDISP_MAX];                         //���ꂼ��̖��@���J���ς݂��ǂ���
	//===============================================================================================================

	void NormalAttack();                                                    //�ʏ�U��
	void MagicAttack();                                                     //���@�U��
	void ChengeMagic();                                                     //���@�ύX���� 

	void FireBallProcess();                                                 //�t�@�C�A�{�[���̏���
	void BeamProcess();                                                     //�r�[���̏���
	void FlowerdIceProcess();                                               //�t�����[�h�A�C�X�̏���
	void ThunderBurstProcess();                                             //�T���_�[�o�[�X�g�̏���
	void PsychoBladeProcess();                                              //�T�C�R�u���C�h�̏���
	void PlantShieldProcess();                                              //�v�����g�V�[���h�̏���

	void QuickHealProcess();                                                //�N�C�b�N�q�[���̏���
	void QucikHealStaging();                                                //�N�C�b�N�q�[���̉��o

	void CrossBombProcess();                                                //�N���X�{��

	void BlackHoleProcess();                                                //�u���b�N�z�[���̏���

	CUi3D* m_pLockOnUi;                                                     //���b�N�I��UI�ւ̃|�C���^
	void PhycoBurstProcess();                                               //�T�C�R�o�[�X�g�̏���

	void ResetUseMagic();                                                   //���@�g�p��Ԃ����Z�b�g���鏈��

	//===============================================================================================================

	//====================================
	//�A�N�V�����֌W
	//====================================

	static const int m_nMAX_JAMPPUSHCNT = 13;                               //���̃W�����v�Ń{�^����������ő�J�E���g��
	static const int m_nMAX_JAMPNUM = 2;                                    //�󒆂ŃW�����v�o�����
	static const int m_nREVIVALDODGETIME = 30;                             //����R�}���h����������܂ł̎���
	static const int m_nDODGETIME = 20;                                     //�����Ԃ��ێ����鎞��
	static const int m_nREVIVALCOUNTERTIME = 120;                           //�J�E���^�[�R�}���h����������܂ł̎���
	static const int m_nCOUNTERTIME = 20;                                   //�J�E���^�[��Ԉێ����鎞��
	static const int m_nMAX_CHARGEATTACKLEVEL = 5;                          //�`���[�W�U���̒i�K
	static const int m_nMAX_INCINCIBLETIME = 100;                           //���G��Ԃ̎��Ԃ̍ő吔
	static constexpr float m_fMAX_GRAVITYPOWER = -15.0f;                    //�d�͂̍ő吔
	static const int m_nMAX_CHARGETIME = 30;                                    //�`���[�W����
	static const int m_nCHARGEATTACKPOWER[m_nMAX_CHARGEATTACKLEVEL];        //�`���[�W�U���̂��ꂼ��̒i�K�̈З�
	//���ʂ̃W�����v
	int m_nJumpPushCnt;                                                     //�W�����v�{�^���������Ă��鎞�Ԃ��J�E���g����

	//�Q�i�W�����v
	int m_nJumpCnt;                                                         //�W�����v�񐔂��J�E���g����

	//���
	typedef struct
	{
		int nRevivalCnt;                                                 //����R�}���h����������܂ł̎��Ԃ��J�E���g����                                    
		bool bPossible;                                                  //����\���ǂ���
		bool bState;                                                     //�����Ԃ��ǂ���
		int nStateCnt;                                                   //�����Ԃ̃J�E���g������
	}Dodge;

	//�`���[�W�U��
	typedef struct
	{
		int nChargeCnt;                                                     //�`���[�W���Ԃ��J�E���g����
		int nPower;                                                         //�З�
		int nChargeLevel;                                                   //�`���[�W�i�K
		bool bState;                                                        //�`���[�W��Ԃ��ǂ���
	}ChargeAttack;

	//��
	typedef struct
	{
		int nHealCnt;//�q�[����ԂɂȂ��Ă���̃J�E���g
		bool bState; //�q�[����Ԃ��ǂ���
		bool bLastHeal;//�񕜂ɕK�v��MP������Ȃ��Ȃ������p�̃t���O
	}Heal;

	Dodge m_Dodge;                                                          //����A�N�V�����\����
	ChargeAttack m_ChargeAttack;                                            //�`���[�W�U���\����
	Heal m_Heal;                                                            //�񕜍\����
	//���̑�
	bool m_bUnlockAction[ACTIONTYPE_MAX];                                   //�A�N�V������������ꂽ���ǂ���

	//�֐�

	//�d��
	void GravityProcess() override;                                         //�d�͂̏���

	void AdjustJump();                                                      //�����W�����v
	void DoubleJump();                                                      //�Q�i�W�����v
	void DodgeAction(float fMoveX,float fMoveY);                            //���

	void ChargeAttack();                                                    //�`���[�W�U��
	void ChargeAttackParticle();                                            //�`���[�W�U���̃p�[�e�B�N����������
	void MaxChargeProcess();                                                //�`���[�W�U���̃`���[�W����Max�ɂȂ����Ƃ��̏���

	void WallJumpProcess();                                                 //�ǃW�����v�̏���
	void InvincibleProcess();                                               //���G���Ԃ̏���
	void SizeSet();                                                         //���f���p�[�c���m�̒��_�̍ő�ŏ����ׁA�T�C�Y��ݒ肷��
	//========================================================================================

	//=====================================
	//�O�Պ֌W
	//=====================================
	CMeshOrbit* m_pMeshOrbit;                                                //�O�Ղւ̃|�C���^
	void MeshOrbitProcess();                                                 //�O�Ղ̏���
	//========================================================================================

	//=====================================
    //�X�R�A�֌W
	//=====================================
	CScore* m_pScore;//�X�R�A�ւ̃|�C���^
    //========================================================================================

	//=====================================
	//�o���b�g�Z�֌W
	//=====================================
	bool m_bUnlockBulletAction[BULLETACTION_MAX];                            //�e�̋Z���J�����Ă��邩�ǂ���
	void ChargeShotProcess();                                                //�`���[�W�V���b�g�̏������s��
	void RainShotProcess();                                                  //���C���V���b�g�̏������s��
	void BurstShotProcess();                                                 //�o�[�X�g�V���b�g�̏������s��
	void RapidShotProcess();                                                 //���s�b�h�V���b�g�̏������s��
	bool CheckUnlockBulletAction();                                          //�o���b�g�A�N�V�������J�����Ă��邩�ǂ����̃`�F�b�N���s��
	//=====================================
	//�u���b�N�֌W
	//=====================================

	//=====================================
	//�J�����֌W
	//=====================================
	bool m_bReturnCamera;                                                    //�J������߂��t���O

	bool m_bStartFade;                                                       //�t�F�[�h�X�^�[�g

	//=====================================
	//���G��Ԋ֌W
	//=====================================
	bool m_bInvincible;                                                      //���G��Ԃ��ǂ���
	int m_nCntInvincible;                                                    //���G��Ԃ̎��Ԃ��J�E���g���� 

	//=====================================
	//���f���p�[�c�֌W
	//=====================================
	CModelParts * m_apModelParts[m_nMAX_PLAYERPARTS];                        //�v���C���[�̃p�[�c������
	void LoadModelParts();                                                   //���f���p�[�c�̃��[�h

	//=====================================
	//���[�V�����֌W
	//=====================================

	//�L�[�f�[�^
	struct KEY
	{
		//�ʒu
		D3DXVECTOR3 Pos;
		//����
		D3DXVECTOR3 Rot;
	};

	//�L�[���
	struct KEYSET
	{
		int nFrame;//�t���[����
		KEY aKey[m_nMAX_PLAYERPARTS];//�v���C���[�̃p�[�c�����[�V��������
	};

	//���[�V�������
	struct MOTION
	{
		bool bLoop;   //���[�v���邩�ǂ���
		int nCntKey;  //�L�[�J�E���g
		int nCntFrame;//�t���[���J�E���g
		int nNumKey;  //�L�[����
		KEYSET aKeySet[m_nMAX_KEYSET];//�L�[���
	};

	//���[�V�������񋓌^
	typedef enum
	{
		MOTIONTYPE00_NUTRAL = 0,//�j���[�g����
		MOTIONTYPE01_RUNNING,   //����
		MOTIONTYPE02_ATTACK,    //�U��
		MOTIONTYPE03_JUMP,      //�W�����v
		MOTIONTYPE04_LANDING,   //���n
		MOTIONTYPE_MAX
	}MOTIONTYPE;

	int m_nNowMotion;                         //���[�V�����J�E���^�[
	int m_nMotionOld;                         //1f�O�̃��[�V����
	int m_nNextMotion;                        //���̃��[�V����
	int m_nCntFrame;                          //���݂̃t���[����
	MOTION m_aMotion[m_nMAX_MOTION];          //���[�V����
	bool m_bChengeMotion;                     //���[�V���������ɕς��邩�ǂ���
	int m_nTargetMotion;                      //���ɕς��郂�[�V�����ԍ�                    
	bool m_bPriorityMotion[MOTIONTYPE_MAX];   //�D�悳���郂�[�V����


	void LoadMotion();                //���[�V�����������[�h����
	void MotionProcess();             //���[�V��������
	void MotionPriorityProcess();     //���[�V�����̗D��x���Ǘ�����

	//=====================================
	//���̑�
	//=====================================
	void DispPlayerInfo();            //�v���C���[�̏���\������

	//=====================================
	//�����֌W
	//=====================================
	void PlayerInertiaProcess();//�v���C���[�̊����̏���
	float m_fSlideInertia;//���点�邽�߂̊���
	float m_fReturnInertia;//�߂����߂̊���
	//=================================================================================

	//=====================================
	//���X�|�[���֌W
	//=====================================
	D3DXVECTOR3 m_ReSpawnPos;//���X�|�[������ʒu
	int m_nRespawnCnt;
	//=================================================================================

	//=====================================
	//�ǃW�����v�֌W
	//=====================================
	bool m_bPossibleWallJump;//�ǃW�����v���\���ǂ���
	int m_nWallJumpBindTime; //�ǃW�����v���s�\�Ȏ��Ԃ��J�E���g����
	void WallJumpBindProcess();//�ǃW�����v�𑩔����鏈��
	void WallJumpParticleProcess();//�ǃW�����v�̉��o�p�p�[�e�B�N��������
	//=================================================================================

	//=====================================
	//�����蔻��֌W
	//=====================================
	void CollisionEnemy();//�G�Ƃ̓����蔻��
	void CollisionItem(); //�A�C�e���Ƃ̓����蔻��
	void CollisionTrap(); //�g���b�v�Ƃ̓����蔻��
	//=================================================================================


};

#endif

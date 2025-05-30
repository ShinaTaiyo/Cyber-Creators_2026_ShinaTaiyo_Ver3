//====================================================
//
//�V���P�O���FC++�Ŋ�ՂÂ���[block.h]
//Author:ShinaTaiyo
//
//====================================================

#ifndef _BLOCK_H_  
#define _BLOCK_H_

//======================
//�C���N���[�h
//======================
#include "main.h"
#include "object2d.h"
#include "objectX.h"
#include "object.h"
//==========================================

//===================================
//�}�b�v�̃e�L�X�g�t�@�C���̃}�N��
//===================================
#define MAP00_SENTRAL_TXT "data\\TEXTFILE\\Map\\Sentral.txt"

//==========================================

//==========================================
//�u���b�N�N���X
//==========================================
class CBlock : public CObjectX
{
public:
	//===========================
	//�u���b�N�̎��
	//===========================
    typedef enum
	{
		BLOCKTYPE00_NORMAL = 0,    //���ʃu���b�N
		BLOCKTYPE01_WATER,         //���u���b�N
		BLOCKTYPE02_GRASS,         //���u���b�N
		BLOCKTYPE03_LONGRENGA,     //�����K�u���b�N
		BLOCKTYPE04_RIFT,          //���t�g�u���b�N
		BLOCKTYPE05_FIRE,          //�t�@�C�A�u���b�N
		BLOCKTYPE06_JUMP,          //�W�����v�u���b�N
		BLOCKTYPE07_CRACK,         //�Ђуu���b�N
		BLOCKTYPE08_SUKESUKE,      //�X�P�X�P�u���b�N
		BLOCKTYPE09_BLINKINGRED,   //�ԓ_�Ńu���b�N
		BLOCKTYPE10_BLINKINGBRUE,  //�_�Ńu���b�N
		BLOCKTYPE11_ICE,           //�A�C�X�u���b�N
		BLOCKTYPE12_ARROW,         //���u���b�N
		BLOCKTYPE13_SWAMP,         //���u���b�N
		BLOCKTYPE14_TORNADO,       //�g���l�[�h�u���b�N
		BLOCKTYPE15_FALL,          //�t�H�[���u���b�N
		BLOCKTYPE16_SAVE,          //�Z�[�u�u���b�N
		BLOCKTYPE17_CHAINBLOCK,    //���u���b�N
		BLOCKTYPE_MAX
	}BLOCKTYPE;
	//======================================================================

	//===========================
	//�}�b�v�̎��
	//===========================
	typedef enum
	{
		MAP00_SENTRAL = 0,
		MAP_MAX
	}MAPTYPE;
	//======================================================================

	CBlock();                 //�R���X�g���N�^
	~CBlock() override;       //�f�X�g���N�^
	HRESULT Init() override;  //����������
	void Uninit() override;   //�I������
	void Update() override;   //�X�V����
	void Draw() override;     //�`�揈��
	void SetDeath() override; //���S�t���O��ݒ�
	static CBlock * Create(BLOCKTYPE type, int nLife,D3DXVECTOR3 pos,D3DXVECTOR3 rot,D3DXVECTOR3 scale,CObjectX::ROTTYPE RotType,int nSubType);//�u���b�N�𐶐�
	void ReleaseBlock();                //�u���b�N������
	BLOCKTYPE GetType();//�u���b�N�̎�ނ��擾����
	void Collision();                                                             //�����蔻����s��
	static void CollisionSquare(CObjectX * pObjX);                                //�����`�̓����蔻����s��
	static void LandingCorrection(D3DXVECTOR3& Pos,CObject * pSaveObj,D3DXVECTOR3 VtxMax,D3DXVECTOR3 VtxMin);              //��Q�A��R�����Ŏw�肵���u���b�N�̏�ɏ���Ă���ꍇ�A�ʒu��␳����
	static int GetNumFile() { return m_nNumFile; }                                                                 //�t�@�C�������擾����
	BLOCKTYPE GetBlockType() { return m_type; }                                                                    //�u���b�N�̃^�C�v���擾����
	bool GetbCollision() { return m_bCollision; }                                  //����\���ǂ������擾����     
	static const int* GetSubTypeNum() { return &m_BLOCKSUBTYPENUM[0]; }            //�T�u�^�C�v�̂O�Ԗڂ��擾����
	int GetSubType() { return m_nSubType; }                                        //�T�u�^�C�v���擾

	float& GetStartRot() { return m_fStartRot; }                                   //��]�J�n�ʒu���擾
	float& GetRotSpeed() { return m_fRotSpeed; }                                   //��]���x���擾
	float& GetMoveLength() { return m_fMoveLength; }                               //�ړ��������擾
	float& GetSpeed() { return m_fSpeed; }                                         //�ړ����x���擾
	int& GetBlinkSpeed() { return m_BlinkingBlock.nBlinkingSpeed; }                //�_�ő��x���擾
	float& GetAddSpeed() { return m_ArrowBlock.fAddSpeed; }                        //�����x���擾
	float& GetJumpPower() { return m_JumpBlock.fJumpPower; }                       //�W�����v�͂��擾

	void SetBlockInfo(float fStartRot, float fRotSpeed, float fMoveLength, float fSpeed, int nBlinkSpeed, float fAddSpeed, float fJumpPower);//�u���b�N�̏��ݒ�
private:
	static const int m_nMAX_FALLBLOCKRESPAWNCNT = 150;

	void RidingTopCollision(bool bJustTurn);                                       //�u���b�N�̏�ɏ���Ă��邩�ǂ����𔻒肷��
	void LiftBlockPattern();                                                       //���t�g�u���b�N�̃p�^�[��
	void LiftBlockRoundMove();                                     //���t�g�u���b�N�̉����̓���
	void BlinkingBlockPattern();                                                   //�_�Ńu���b�N�̃p�^�[��
	void ArrowBlockPattern();                                                      //���u���b�N�̃p�^�[��
	void FallBlockProcess();                                                       //������u���b�N�̏���
	void DispInfo() override;                                                      //���\������
	//�e�N�X�`���t�@�C���̓ǂݍ���
	static const char* m_BLOCK_FILENAME[BLOCKTYPE_MAX];                            //�u���b�N�̃t�@�C���l�[��
	static const int m_BLOCKSUBTYPENUM[BLOCKTYPE_MAX];                                //�u���b�N�̃T�u�^�C�v
	static const float m_fBLOCKCORRECTIONCOLLISION;                                 //����ɗ]�T�����������
	BLOCKTYPE m_type;                                                              //�e�̎��
	bool m_bMove;                                                                  //�ړ����邩�ǂ���
	int m_nCntTime;                                                                //��������Ă���̎��Ԃ��J�E���g����
	bool m_bUse;                                                                   //�g�p���
	bool m_bTurn;                                                                  //�^�[���t���O
	static int m_nNumFile;                                                         //�t�@�C�������i�[����
	bool m_bCollision;                                                             //�����蔻������邩�ǂ���

	typedef struct
	{
		bool bBlinking;//�_�ł��Ă��邩�ǂ���
		int nBlinkingSpeed;//�_�ő��x
	}BlinkingBlock;

	typedef struct
	{
		float fAddSpeed;//�����x
	}ArrowBlock;

	typedef struct
	{
		float fJumpPower;//�W�����v�̑傫��
	}JumpBlock;

	typedef struct
	{
		bool bFall;//������̂��J�n���邩�ǂ���
		int nRespawnCnt;//���X�|�[������܂ł̃J�E���g
		int nReraiseCnt;//���������Ƃ��̃J�E���g
	}FallBlock;

	ArrowBlock m_ArrowBlock;      //���u���b�N�̏��
	BlinkingBlock m_BlinkingBlock;//�_�Ńu���b�N�̏��
	JumpBlock m_JumpBlock;        //�W�����v�u���b�N�̏��
	FallBlock m_FallBlock;        //������u���b�N�̏��

	//======================================
	//�u���b�N���蕪���p
	//======================================
	bool m_bNearBlockX;                         //X�����̋߂��Ƀu���b�N������
	bool m_bNearBlockY;                         //Y�񍐂̋߂��Ƀu���b�N������
	void NearBlockSearch();                     //�߂��Ƀu���b�N�����邩�ǂ������T�[�`����
	static void NearBlockSurchProcess(D3DXVECTOR3 Pos,D3DXVECTOR3 Size);//�߂��̃u���b�N��T������
	//======================================================================================================

	//======================================
	//�u���b�N�Ƃ̉����o������p
	//======================================
	void ExtrusionCollisionX(CObjectX* pMyObjX,CBlock * pBlock);//X�����̉����o��������s��

	void ExtrusionCollisionY(CObjectX* pMyObjX, CBlock* pBlock);//Y�����̉����o��������s��

	int m_nSubType;                                                                //�T�u�^�C�v�̔ԍ�
	float m_fRot;                                                                  //�����̕␳�l
	float m_fRotSpeed;                                                             //��]���x
	float m_fMoveLength;                                                           //�u���b�N����������
	float m_fStartRot;                                                             //�J�n����
	float m_fSpeed;                                                                //���x	
};

#endif

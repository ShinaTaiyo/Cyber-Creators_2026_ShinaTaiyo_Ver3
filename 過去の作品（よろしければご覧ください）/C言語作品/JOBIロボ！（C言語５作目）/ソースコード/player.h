//========================================================
//
//�v���C���[�̏����Ɋւ���w�b�_�[�t�@�C��
//Author:ShinaTaiyo
//
//=========================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"
#include "score.h"
#include "scrap.h"
//=======================================================================================================================================================================================================================
// �}�N����`
//=======================================================================================================================================================================================================================
#define MAX_PLAYERPARTS		(19)//�v���C���[�p�[�c��(���������)
#define MAX_PLAYERPARTS2	(MAX_PLAYERPARTS + 1)//�v���C���[�p�[�c��(������܂�) �i�v���C���[�̃p�[�c��(18) + ����R��(�p�[�c�� + 1) * MAX_WEAPON)
#define MAX_PLAYER_WEAPON	(3)//����̎��
#define MAX_MOTION			(7)	//���[�V������
#define MAX_KEY				(30)//���[�V�����̃L�[�̍ő吔
#define MAX_PLAYERMAT		(100)//�e���f���̃}�e���A�����̍ő吔
#define MAX_LEVEL			(20)//���x���̍ő�l
#define FIRST_PLAYERSPEED	(12.0f)//�v���C���[�̏����x
#define PLAYER_TXT			"data\\SetPlayer.txt"//�v���C���[�̃p�[�c���
#define WEAPON_TXT			"data\\SetWeapon.txt"//�v���C���[�̃p�[�c���
#define LEVEL_TXT			"data\\TEXTFILE\\Level.txt"
#define LEVELEDIT_TXT		"data\\TEXTFILE\\LevelEdit.txt"
#define PLAYERPARTS_MAT		(100)//�}�e���A���̍ő吔
#define COLLISION_WALL		(8.0f)//�ǂƂ̓����蔻�蒲���p

//=======================================================================================================================================================================================================================
// ���
//=======================================================================================================================================================================================================================

//=================================================================================================
//�v���C���[�̍U���̔ԍ��i���ꂼ��̍U���Ƀq�b�g�X�g�b�v�̔����t�������̂ŏ������B�j
//=================================================================================================
typedef enum
{
	PLAYERATTACK00_SWORD = 0,//���̍U���̔ԍ�
	PLAYERATTACK01_BOOMERANG,//�u�[�������U���̔ԍ�
	PLAYERATTACK02_SPEAKER,  //�X�s�[�J�[�U���̔ԍ�
	PLAYERATTACK_MAX
}PLAYERATTACK;

typedef enum
{
	PLAYERSTATE_NUTORAL = 0,//�j���[�g����
	PLAYERSTATE_MOVE,		//�ړ�
	PLAYERSTATE_ATTACK,		//�U��
	PLAYERSTATE_DAMAGE,		//�_���[�W��
	PLAYERSTATE_HEALWALL,	//�ǉ�
	PLAYERSTATE_HEALGROUND,	//����
	PLAYERSTATE_DODGE,		//���
	PLAYERSTATE_MAX
}PLAYERSTATE;

typedef enum
{
	PLAYERMODE_NORMAL = 0,	//���ʃ��[�h
	PLAYERMODE_EDIT,		//�G�f�B�b�g���[�h
	PLAYERMODE_MAX
}PLAYERMODE;

//=======================================================================================================================================================================================================================
// �\����
//=======================================================================================================================================================================================================================

//================================================================
// �e���f���̍\����
//================================================================

//================================================================
// ���̍\����
//================================================================
typedef struct
{
	int nLevel;				//���x��
	int nSwordScrap;		//���X�N���b�v�l����
	int nTotalSwordScrap;   //���X�N���b�v���v�l����
	int nMaxLevelUpPoint;	//���̃��x���܂łɕK�v�ȃX�N���b�v��
	int nSwordPower;		//���̍U����
	int nSwordAttackSpeed;	//���̍U�����x
}Sword;

//================================================================
// �u�[�������̍\����
//================================================================
typedef struct
{
	int nLevel;				  //���x��
	int nBoomerangScrap;	  //�u�[�������X�N���b�v�l����
	int nTotalBoomerangScrap; //�u�[�������X�N���b�v���v�l����
	int nMaxLevelUpPoint;	  //���̃��x���܂łɕK�v�ȃX�N���b�v��
	int nBoomerangPower;	  //�u�[�������̍U����
	float fBoomerangRenge;	  //�u�[�������̎˒�
	int nBoomerangPiece;	  //���ݎ����Ă���u�[�������̌�
	int nMaxBoomerangPiece;   //���Ă�u�[�������̍ő吔
	bool bPossibleBoomerang;  //�u�[�������U�����\���ǂ����@
	D3DXVECTOR3 BoomerangScale;//�u�[�������̑傫��
}Boomerang;

//================================================================
// �X�s�[�J�[�̍\����
//================================================================
typedef struct
{
	int nLevel;				//���x��
	int nSpeakerScrap;		//�X�s�[�J�[�X�N���b�v�l����
	int nTotalSpeakerScrap; //�X�s�[�J�[�X�N���b�v���v�l����
	int nMaxLevelUpPoint;	//���̃��x���܂łɕK�v�ȃX�N���b�v��
	float fSpeakerPower;	//�X�s�[�J�[�̍U����
	int nPowerConsumption;	//�X�s�[�J�[�̏���d��
}Speaker;

//================================
// �v���C���[����
//================================
typedef struct
{
	D3DXVECTOR3 pos;//�ʒu
	D3DXVECTOR3 rot;//����
	int nType;//�ǂݍ��ރ��f���̎��
	bool bUse;//�g�p���Ă��邩�ǂ���
	D3DXVECTOR3 vtxMax;//�ő咸�_�l
	D3DXVECTOR3 vtxMin;//�ŏ����_�l
	D3DXVECTOR3 Size;//�S�̂̑傫��
}Weapon;//�p�[�c���Ƃ̓���

//=========================================
// �L�[�̏��\����
//=========================================
typedef struct
{
	D3DXVECTOR3 pos;//�ʒu
	D3DXVECTOR3 rot;//����
}PlayerKey;

//=========================================
// �L�[�Z�b�g�̏��\����
//=========================================
typedef struct
{
	PlayerKey key[MAX_PLAYERPARTS2];
	int nMaxFrame;
}PlayerKeySet;

//=========================================
// ���[�V�����̏��\����
//=========================================
typedef struct
{
	int nCntKey;//�L�[���J�E���^
	int nCntFrame;//�t���[�����J�E���^
	int nMaxKey;//�e���[�V�����̍ő�L�[��
	int nMotionLoop;//�e���[�V���������[�v���邩�ǂ���
	PlayerKeySet keyset[MAX_PLAYERPARTS2];//�p�[�c�̏��
}PlayerMotionSet;

//================================
// �v���C���[�p�[�c
//================================
typedef struct
{
	D3DXVECTOR3 pos;//�ʒu
	D3DXVECTOR3 rot;//����
	D3DXVECTOR3 vtxMinModel;//�ʒu�̍ŏ��l
	D3DXVECTOR3 vtxMaxModel;//�ʒu�̍ő�l
	D3DCOLORVALUE DiffUse[MAX_PLAYERMAT];//�F����
	D3DCOLORVALUE FormarDiffUse[MAX_PLAYERMAT];//���̐F����

	BYTE* pVtxBuff;//���_�o�b�t�@�ւ̃|�C���^
	DWORD sizeFVF;//���_�t�H�[�}�b�g�̃T�C�Y
	D3DXMATRIX mtxWorld;//���[���h�}�g���b�N�X
	int nIdxModelParent;//�e���f���̃C���f�b�N�X
	int nType;//���
	bool bUse;//�g�p���Ă��邩�ǂ���
	LPD3DXMESH pMesh;//���b�V���i���_���j�ւ̃|�C���^
	LPD3DXBUFFER pBuffMat;//�}�e���A���ւ̃|�C���^
	DWORD dwNumMat;//�}�e���A���̐�
	D3DXVECTOR3 PartsPos;//�p�[�c�̈ʒu�i���[���h���W�j

}ModelParts;//�p�[�c���Ƃ̓���

//================================================================
// �v���C���[�̍\����
//================================================================
typedef struct
{
	D3DXVECTOR3 pos;//�ʒu
	D3DXVECTOR3 posOld;//�O�̈ʒu
	D3DXVECTOR3 rot;//����
	D3DXVECTOR3 targetrot;//�ړI�̌���
	D3DXVECTOR3 move;//�ړ���
	D3DXVECTOR3 vtxMax;//�v���C���[�S�̂̍ő咸�_�l
	D3DXVECTOR3 vtxMin;//�v���C���[�S�̂̍ŏ����_�l
	D3DXVECTOR3 size;//�T�C�Y
	D3DXMATRIX mtxWorld;//���[���h�}�g���b�N�X
	D3DCOLORVALUE DiffUse;//���f���̐F
	bool bMotionFlag;//���[�V���������t���O
	int nMode;//���[�h�̎��
	bool bUse;//�g�p���Ă��邩�ǂ���
	int nIdxShadow;//�e�̃C���f�b�N�X
	int nIdxRaderDisplay;//���[�_�[�f�B�X�v���C�̃C���f�b�N�X
	float fWidth;//�v���C���[�̕�
	float fHeight;//�v���C���[�̍���
	float fAutletLength;//�R���Z���g�Ƃ̋���
	bool bIsLanding;//�n�ʂɂ��邩�ǂ���
	PLAYERSTATE State;//�v���C���[�̏�Ԃ�\��
	D3DXVECTOR3 Size;//�v���C���[�S�̂̑傫��
	int nHp;//�v���C���[�̗̑�
	Weapon aWeapon[MAX_PLAYER_WEAPON];//�e����̏��
	PlayerMotionSet aMotionSet[MAX_PLAYER_WEAPON][MAX_MOTION];//�̂̃��[�V�����̍\���̂̏��
	PlayerMotionSet aMotionSet2[MAX_PLAYER_WEAPON][MAX_MOTION];//�r�̃��[�V�����̍\���̂̏��
	ModelParts aModel[MAX_PLAYERPARTS2][MAX_PLAYER_WEAPON];//�e�p�[�c�̍\���̂̏��
	int nStateOld;//�Pf�O�̏�Ԃ�ۑ�����
	bool bMove;//�ړ���Ԕ���
	bool bCompulsionAttack;//�����I�ɍU����Ԃɂ���B
	int nAttackTime;//�U�����Ă��鎞�Ԃ��v������
	bool bInvincible;//���G��Ԃɂ���
	int nInvincibleCnt;//���G��Ԃɂ��鎞��
	int nNumLandingModel;//�ǂ̃��f���̏�ɏ���Ă��邩
	float fMoveX;//X�����̈ړ���
	float fMoveZ;//Z�����̈ړ���
	float fCombo;//�R���{�{��
	int nCntCombo;//�R���{��

	//====================================
	//�x�N�g���ϐ�
	//====================================
	float fVXaim;
	float fVZaim;
	float fVaim;

	//====================================
	//�ア�U�����󂯂����̕ϐ�
	//====================================
	bool bWeakDamageReaction;//�ア�U�����󂯂�����true�ɂ��ĐF��ς����肷��
	int nWeakDamageReactionTime;//�ア�U�����󂯂��Ƃ��ɁA�F��ς��鎞�Ԃ��J�E���g����

	//========================
	//���U���g�֌W
	//========================
	int nTotalGetScrap;   //�X�N���b�v���v�l����
	int nTotalDefeatEnemy;//�G���v���j��

	//=================================
	//�o�b�e���[�����֘A
	//=================================
	int nMoveFrame;//�������t���[�������J�E���g����
	int nAttackCount;//�U���������ɃJ�E���g������
	int nDamageCountTime;//�_���[�W���󂯂Ă��鎞�Ԃ��J�E���g����
	bool bDamageState;//�_���[�W���󂯂Ă����Ԃ��ǂ���

	//=================================
	//�o�b�e���[�[�d�֘A
	//=================================

	//�Ǐ[�d
	bool bCompulsionHealWall;//�����I�ɕǉ񕜁i�[�d�j��Ԃɂ���
	int nHealWallTime;//�ǉ񕜂����Ă��鎞�Ԃ��v������
	D3DXVECTOR3 HealWallSaveRot;//�ǉ񕜒��̌�����ۑ�����
	bool bChargingFlag;//�[�d����true�ɂ���
	int nNumUseWallOutlet;//�g�p���Ă���ǃR���Z���g�̃��f���ԍ����L�^����

	//���[�d
	bool bCompulsionHealGround;//�����I�ɏ��񕜁i�[�d�j��Ԃɂ���
	int nHealGroundTime;//���񕜂����Ă��鎞�Ԃ��v������
	D3DXVECTOR3 HealGroundSaveRot;//�ǉ񕜒��̌�����ۑ�����
	bool bChargingGroundFlag;//�[�d����true�ɂ���
	int nNumUseGroundOutlet;//�g�p���Ă���ǃR���Z���g�̃��f���ԍ����L�^����

	//=================================
	//����֘A
	//=================================
	bool bCompulsionDodge;//�����I�ɉ����Ԃɂ���
	bool bPossibleDodge;  //����\���ǂ���
	int nDodgeTime;       //������Ă��鎞��
	int nDodgeCoolTime;   //������\�ɂȂ�܂ł̃N�[���^�C��

	//=================================
	// ���у��{�{�̂̃X�e�[�^�X
	//=================================
	int nLevel;					//���x��
	int nTibiroboScrap;			//���у��{�X�N���b�v�l����
	int nTotalTibiroboScrap;    //���у��{�X�N���b�v���v�l����
	int nMaxLevelUpPoint;		//���̃��x���܂łɕK�v�ȃX�N���b�v��
	int nBattery;				//���у��{�̃o�b�e���[�i�̗́j
	int nAllUpScrap;			//�S�ẴX�e�[�^�X�����X�N���b�v�l����
	int nTotalAllUpScrap;       //�S�ẴX�e�[�^�X�����X�N���b�v���v�l����
	int nWeapon;				//�������Ă��镐��
	float fSpeed;				//���у��{�̑���
	float fSuctionScrapRange;   //�X�N���b�v�z���͈�
	Sword aSwordInfo;			//���̃X�e�[�^�X
	Boomerang aBoomerangInfo;	//�u�[�������̃X�e�[�^�X
	Speaker aSpeakerInfo;		//�X�s�[�J�[�̃X�e�[�^�X
}Player;

//=======================================================================================================================================================================================================================
// �e�p�[�c��X�t�@�C��
//=======================================================================================================================================================================================================================
static const char* PLAYER_FILENAME[MAX_PLAYERPARTS] =
{
	"data\\PLAYER\\00_waist000.x",	//��[0]
	"data\\PLAYER\\01_body000.x",	//��[1]
	"data\\PLAYER\\02_hedD000.x",	//����[2]
	"data\\PLAYER\\03_hedU000.x",	//����[3]
	"data\\PLAYER\\04_urmUL000.x",	//����r[4]
	"data\\PLAYER\\05_urmDL000.x",	//���r[5]
	"data\\PLAYER\\06_handL000.x",	//����[6]
	"data\\PLAYER\\07_urmUR000.x",	//�E��r[7]
	"data\\PLAYER\\08_urmDR000.x",	//�E�r[8]
	"data\\PLAYER\\09_handR000.x",	//�E��[9]
	"data\\PLAYER\\10_legUL000.x",	//����[10]
	"data\\PLAYER\\11_legDL000.x",	//����[11]
	"data\\PLAYER\\12_legUR000.x",	//�E��[12]
	"data\\PLAYER\\13_legDR000.x",	//�E��[13]
	"data\\PLAYER\\14_code000.x",	//�R�[�h[14]
	"data\\PLAYER\\14_code000.x",	//�R�[�h[15]
	"data\\PLAYER\\14_code000.x",	//�R�[�h[16]
	"data\\PLAYER\\15_plug000.x",	//�v���O[17]
	"data\\PLAYER\\b.x",			//����(�{��)[18]
};

//=======================================================================================================================================================================================================================
// �e�����X�t�@�C��
//=======================================================================================================================================================================================================================
static const char* WEAPON_FILENAME[MAX_PLAYER_WEAPON] =
{
	"data\\PLAYER\\sword\\sword000.x",			//��
	"data\\PLAYER\\boomerang\\boomerang000.x",	//�u�[������
	"data\\PLAYER\\speaker\\speaker000.x",		//�X�s�[�J�[
};

//=======================================================================================================================================================================================================================
// �e�����X�t�@�C��
//=======================================================================================================================================================================================================================
static const char* MOTION_FILENAME[MAX_PLAYER_WEAPON] =
{
	"data\\PlayerMotion000.txt",//[0]��
	"data\\PlayerMotion001.txt",//[1]�u�[������
	"data\\PlayerMotion002.txt",//[2]�X�s�[�J�[
};

//=======================================================================================================================================================================================================================
// �v���g�^�C�v�錾
//=======================================================================================================================================================================================================================
void InitPlayer(void);//����������
void UninitPlayer(void);//�I������
void UpdatePlayer(void);//�X�V����
void DrawPlayer(void);//�`�揈��
Player* GetPlayer(void);//�\���̂̏��̎擾

//============================================
//�v���C���[�p�[�c�A���[�V�����֌W
//============================================
void SetPlayerParts(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nIdx);//�v���C���[�p�[�c�̐ݒ菈��
void SetPlayerWeapon(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nIdx, int nCnt);//�v���C���[�p�[�c�̐ݒ菈��
void LoadPlayer(void);//�v���C���[�p�[�c���̓ǂݍ��ݏ���
void LoadWeapon(void);//����p�[�c���̓ǂݍ��ݏ���
void LoadMotion(int nWeapon);//���[�V�����̓ǂݍ��ݏ���
void PlayerMotion(int nWeapon);//�̂̃��[�V��������
void PlayerMotion2(int nWeapon);//�r�̃��[�V��������

//===================================
//�Q�[���֌W
//===================================
void SetDamagePlayer(int nDamage);
bool bPlayerDeath(void);//�v���C���[�����񂾂��ǂ����̔����Ԃ�
void AddScrapPlayer(SCRAPTYPE ScrapType,int nScrap);//�X�N���b�v����������Ƃ��ɌĂԊ֐��B
void LevelPlayer(void);//�v���C���[�̃��x���Ɋւ��鏈�����s���B
void StatusPlayer(void);//�v���C���[�̃X�e�[�^�X�Ɋւ��鏈���B
void CalculateMatrixPlayer(void);//�v���C���[�̃}�g���b�N�X���v�Z����֐�
void AttackHitPlayer(int nSubBattery);//�v���C���[�ɍU�����q�b�g�������ɌĂԊ֐�
void ResetPlayerPos(void);//�v���C���[�̈ʒu���Z�b�g����

#endif

//================================================================================================================================================================================================================================
//
// �U�����f���̏����Ɋւ���w�b�_�[�t�@�C��
// Author : Atsumu Kuboichi
//
//================================================================================================================================================================================================================================
#ifndef _ATTACKMODEL_H_	//���̃}�N����`������Ȃ�������
#define _ATTACKMODEL_H_	//2�d�C���N���[�h�h�~�̃}�N����`

#include "main.h"

//================================================================================================================================================================================================================================
// �}�N����`
//================================================================================================================================================================================================================================
#define MAX_ATTACKMODEL		(1024)	//���f���̍ő吔
#define NUM_ATTACKMODEL		(10)		//���f���̎��
#define ATTACKMODEL_TEX		(100)	//�e�N�X�`���̍ő吔
#define ATTACKMODEL_MAT		(100)	//�}�e���A���̍ő吔
#define MAX_ATTACKMODELMAT	(100)	//�ő�}�e���A����

//================================================================================================================================================================================================================================
// �\����
//================================================================================================================================================================================================================================

typedef enum
{
	ATTACKMODEL00_BOOMERANG = 0,//�u�[�������U���iP�j
	ATTACKMODEL01_WAVE,         //�E�F�[�u�U�� (P)
	ATTACKMODEL02_SPHERE,       //��(E)
	ATTACKMODEL03_BOMB,         //�{��(E)
	ATTACKMODEL04_SHOCKWAVE,    //�Ռ��g(E)
	ATTACKMODEL05_TORNADO,      //�g���l�[�h(E)
	ATTACKMODEL06_THUNDER,      //�T���_�[(E)
	ATTACKMODEL07_METEO,        //���e�I(E)
	ATTACKMODEL08_SOUP,         //�X�[�v(E)
	ATTACKMODEL09_LASER,        //���[�U�[(E)
	ATTACKMODEL_MAX
}ATTACKMODEL;

typedef enum
{
	ATTACKMODELBEHAVIOR00_NORMAL = 0,//�U���̋���������
	ATTACKMODELBEHAVIOR01_ANOTHER,   //�U���̋�����betu 
	ATTACKMODELBEHAVIOR_MAX
}ATTACKMODELBEHAVIOR;

typedef struct
{
	LPD3DXMESH pMesh;			                   //���b�V����(���_���)�ւ̃|�C���^
	LPD3DXBUFFER pBuffMat;		                   //�}�e���A���̃|�C���^
	DWORD dwNumMat;				                   //�}�e���A���̐�
	LPDIRECT3DTEXTURE9 pTexture[MAX_ATTACKMODELMAT];//�e�N�X�`���ւ̃|�C���^
}AttackModelInfo;

typedef struct
{

	//=====================
	//�S��
	//====================
	D3DXMATRIX mtxWorld;		                  //���[���h�}�g���b�N�X
	LPDIRECT3DTEXTURE9 pTexture;                  //�e�N�X�`���ւ̃|�C���^
	D3DXVECTOR3 vtxMin;			                  //�ŏ��l
	D3DXVECTOR3 vtxMax;			                  //�ő�l
								                  
	D3DXVECTOR3 OriginallyVtxMax;                 //���X�̍ő�l
	D3DXVECTOR3 OriginallyVtxMin;                 //���X�̍ŏ��l

	D3DCOLORVALUE DiffUse[NUM_ATTACKMODEL][ATTACKMODEL_MAT];//���f���̐F�������邱�Ƃ��ł���ϐ�(pMat�ɂ������^������̂ŁA����ɑ�����ĐF��ς�����悤�ɂ���B)

	D3DXVECTOR3 pos;			                  //�ʒu
	D3DXVECTOR3 PosOld;                           //1f�O�̈ʒu
	D3DXVECTOR3 SavePos;                          //���������ʒu�̋L��
	D3DXVECTOR3 move;			                  //�ړ���
	D3DXVECTOR3 rot;			                  //����
	D3DXVECTOR3 Vrot;                             //�x�N�g�����l����������
	D3DXVECTOR3 Scale;                            //�g�嗦
	D3DXVECTOR3 PosLength;                        //���镨�̂Ƃ̋��������߂鎞�Ɏg��
	D3DXVECTOR3 Size;                             //���̂̑傫��
	int nType;					                  //���
	int nBehaviorType;                            //�����̎��
	int nLife;                                    //�̗�
	int nMaxLife;                                 //�ő�̗�
	bool bUse;					                  //�g�p���Ă��邩�ǂ���
	float fRot;                                   //�������������߂�
	float fExpansion;                             //���f�����g�傳���鑬��
	int nNumLandingModel;                         //�ǂ̃��f���̏�ɏ���Ă��邩
	int nIdxShadow;                               //�e�̃C���f�b�N�X

	float fVaim;                                  //�x�N�g��
	float fSupportVaim[4];                        //�x�_�ƂȂ�x�N�g��
	float fVXaim;                                 //X�����̃x�N�g��
	float fVZaim;                                 //Z�����̃x�N�g��
	float fSpeed;                                 //�X�s�[�h
	bool bIsLanding;                              //�n�ʂɂ��邩�ǂ���
	bool bCurveLaser;                             //���[�U�[���񂷌��������߂�
	//===========================================================================================

	//=======================
	//�u�[�������ŗL����
	//=======================
	bool bBack;                 //�߂��Ă���悤�ɂ���t���O
	D3DXVECTOR3 SupportPos;     //�ʂ�`���悤�ɂ��邽�߂̎x�_�ƂȂ�ʒu
	D3DXVECTOR3 SupportPosPlayer;//�v���C���[�̈ʒu����x�_�����߂�B
	D3DXVECTOR3 TargetPos;      //�ړI�̈ʒu�i��������)
	float fSaveRot;             //�v���C���[���u�[�������𓊂������̌������L�^����
	float fBoomerangRange;      //�u�[�������������������_�ł̎˒���ۑ�����
	float fTotalRotMove;            //�v���C���[���u�[�������𓊂��Ă����]�ړ����������v��
	bool bTurnFlag;             //�u�[���������^�[��������t���O
	int nTurnCnt;               //�u�[���������^�[�����n�߂���J�E���g���J�n
	//===========================================================================================
	int nCntTime;               //�U�����Ă���̎��Ԃ��J�E���g����

	//==============================
	//�X�t�B�A�ϐ�
	//==============================
	bool bSphereShot;//���C�t�������ɂȂ�ƁA�X�t�B�A�����@�_���œ�����

	//==============================
	//�O�������@�_���ϐ�
	//==============================
	float fVXaim3D;
	float fVYaim3D;
	float fVZaim3D;
	float fVLaim3D;

	//==========================================================
}AttackModel;

//================================================================================================================================================================================================================================
// �e��ނ�X�t�@�C��
//================================================================================================================================================================================================================================
static const char* ATTACKMODEL_FILENAME[NUM_ATTACKMODEL] =
{
	"data\\PLAYER\\boomerang\\boomerang000.x",				 //[0]�u�[�������U��
	"data\\MODEL\\Attack\\Wave_000.x",                       //[1]�X�s�[�J�[�U��
	"data\\MODEL\\Attack\\Sphere_000.x",                     //[2]�X�t�B�A�U��
	"data\\MODEL\\Attack\\bomb_000.x",                       //[3]�{���U��
	"data\\MODEL\\Attack\\ShockWave_000.x",                  //[4]�Ռ��g
	"data\\MODEL\\Attack\\Tornado_000.x",                    //[5]�g���l�[�h
	"data\\MODEL\\Attack\\thunder000.x",                     //[6]�T���_�[
	"data\\MODEL\\Attack\\Meteorite_000.x",                  //[7]���e�I
	"data\\MODEL\\Attack\\soup000.x",                        //[8]�X�[�v
	"data\\MODEL\\Attack\\laser000.x",                       //[9]���[�U�[
};

//================================================================================================================================================================================================================================
// �v���g�^�C�v�錾
//================================================================================================================================================================================================================================
void InitAttackModel(void);													//����������
void UninitAttackModel(void);												//�I������
void UpdateAttackModel(void);												//�X�V����
void DrawAttackModel(void);													//�`�揈��
void SetAttackModel(int nType,int nLife,D3DXVECTOR3 pos,D3DXVECTOR3 move, D3DXVECTOR3 rot,D3DXVECTOR3 Scale,
	                float fRot,float fSaveRot,float fExpansion,D3DXVECTOR3 SupportPos,int nBehaviorType);//�ݒ菈��
AttackModel* GetAttackModel(void);											//���̎擾

float fRotCollisionAModel(float x0,float x1,float x2,float z0,float z1,float z2);//�����Ƃ̓����蔻������

#endif

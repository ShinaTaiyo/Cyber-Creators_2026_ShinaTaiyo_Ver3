//================================================================================================================================================================================================================================
//
// ���f���̏����Ɋւ���w�b�_�[�t�@�C��
// Author : Atsumu Kuboichi
//
//================================================================================================================================================================================================================================
#ifndef _MODEL_H_	//���̃}�N����`������Ȃ�������
#define _MODEL_H_	//2�d�C���N���[�h�h�~�̃}�N����`

#include "main.h"

//================================================================================================================================================================================================================================
// �}�N����`
//================================================================================================================================================================================================================================
#define MAX_MODEL			(1000)	//���f���̍ő吔
#define NUM_MODEL			(56)	//���f���̎��
#define MODEL_TEX			(100)	//�e�N�X�`���̍ő吔
#define CREATE_POSX_TYPE1	(150.0f)//���̋r��X���W�ݒ�p
#define CREATE_POSX_TYPE3	(80.0f)	//�֎q(���k�p)�̋r��X���W�ݒ�p
#define CREATE_POSX_TYPE5	(400.0f)//����̋r��X���W�ݒ�p
#define CREATE_POSX_TYPE7	(70.0f)	//�֎q(���k�p)��X���W�ݒ�p
#define MAX_MODELMAT		(100)	//�ő�}�e���A����
#define OUTLET_SIZE			(100.0f)//�R���Z���g����͈̔�(���a)

//================================================================================================================================================================================================================================
// �\����
//================================================================================================================================================================================================================================
typedef struct
{
	LPD3DXMESH pMesh;			//���b�V����(���_���)�ւ̃|�C���^
	LPD3DXBUFFER pBuffMat;		//�}�e���A���̃|�C���^
	DWORD dwNumMat;				//�}�e���A���̐�
	D3DXMATRIX mtxWorld;		//���[���h�}�g���b�N�X
	LPDIRECT3DTEXTURE9 pTexture;//�e�N�X�`���ւ̃|�C���^
	D3DXVECTOR3 vtxMin;			//�ŏ��l
	D3DXVECTOR3 vtxMax;			//�ő�l
	D3DXVECTOR3 Size;           //�T�C�Y
	D3DXVECTOR3 pos;			//�ʒu
	D3DXVECTOR3 posOld;         //1f�O�̈ʒu
	D3DXVECTOR3 SavePos;        //�����ʒu��ۑ�����
	D3DXVECTOR3 move;			//�ړ���
	D3DXVECTOR3 rot;			//����
	int nType;					//���
	int nUse;					//�t�B�[���h�����у��{�n�E�X���̔���
	int nCntTime;               //�l�X�ȃ^�C�~���O�Ŏ��Ԃ𑪂�
	bool bUse;					//�g�p���Ă��邩�ǂ���
	bool bCollision;			//�����蔻��̗L��
	int nIdxRaderDisplay;       //���[�_�[�\���̃C���f�b�N�X

	//=================================
	//���t�g���菈��
	//=================================
	bool bUpDownLift;//�t���O�̐؂�ւ��Ń��t�g���㉺�ł���悤�ɂ���
	bool bStopLift;  //���t�g���~�߂�
}Model;

//================================================================================================================================================================================================================================
// �e��ނ�X�t�@�C��
//================================================================================================================================================================================================================================
static const char* MODEL_FILENAME[NUM_MODEL] =
{
	//�u��(�Q�[��)
	"data\\MODEL\\OBJECT\\desk000\\deskup000.x",						//��(��)[0]
	"data\\MODEL\\OBJECT\\desk000\\deskdown000.x",						//��(��)[1]
	"data\\MODEL\\OBJECT\\Chair_000\\ChairUP_000.x",					//�֎q(��)[2]
	"data\\MODEL\\OBJECT\\Chair_000\\ChairDown_000.x",					//�֎q(��)[3]
	"data\\MODEL\\OBJECT\\Lectern_000\\LecternUP_000.x",				//����(��)[4]
	"data\\MODEL\\OBJECT\\Lectern_000\\LecternDown_000.x",				//����(��)[5]
	"data\\MODEL\\OBJECT\\Chair_001\\ChairUP_001.x",					//�֎q(�搶�p�@��)[6]
	"data\\MODEL\\OBJECT\\Chair_001\\ChairLeftD_001.x",					//�֎q(�搶�p�@����)[7]
	"data\\MODEL\\OBJECT\\Chair_001\\ChairRightD_001.x",				//�֎q(�搶�p�@�E��)[8]
	"data\\MODEL\\OBJECT\\BookRack_000\\BookRack_000.x",				//���b�N[9]
	"data\\MODEL\\OBJECT\\locker000\\locker000.x",						//���b�J�[[10]
	"data\\MODEL\\OBJECT\\Pillar_000\\Pillar_000.x",					//��(�p)[11]
	"data\\MODEL\\OBJECT\\Pillar_001\\Pillar_001.x",					//��(����)[12]
	"data\\MODEL\\OBJECT\\TV_000\\TV_000.x",							//�e���r[13]
	"data\\MODEL\\OBJECT\\whiteboard000\\whiteboard000.x",				//�z���C�g�{�[�h[14]
	"data\\MODEL\\OBJECT\\ChibiRoboHouse\\all100.x",					//���у��{�n�E�X[15]
	"data\\MODEL\\OBJECT\\Chirikami_000\\Chirikami_000.x",				//�S�~[16]
	"data\\MODEL\\OBJECT\\Door_000\\Door_000.x",						//�h�A[17]
	"data\\MODEL\\OBJECT\\lift000\\lift000.x",							//���t�g[18]
	"data\\MODEL\\OBJECT\\Outlet_000\\Outlet_000.x",					//�R���Z���g(��)[19]
	"data\\MODEL\\OBJECT\\Pillar_000\\Pillar_000.x",					//��(�p)[20]
	"data\\MODEL\\OBJECT\\Pillar_001\\Pillar_001.x",					//��(����)[21]
	"data\\MODEL\\OBJECT\\Stove_000\\Stove_000.x",						//�X�g�[�u[22]
	"data\\MODEL\\OBJECT\\machine\\machine000.x",						//�@�B[23]
	"data\\MODEL\\OBJECT\\PC000\\PC000.x",								//PC[24]
	"data\\MODEL\\OBJECT\\Book_000\\Book_000.x",						//�{(��)[25]
	"data\\MODEL\\OBJECT\\Book_001\\Book_001.x",						//�{(��)[26]
	"data\\MODEL\\OBJECT\\Book_002\\Book_002.x",						//�{(��)[27]
	"data\\MODEL\\OBJECT\\Book_003\\Book_003.x",						//�{(��)[28]
	"data\\MODEL\\OBJECT\\FlowerVase_000\\FlowerVase_000.x",			//�ԕr[29]
	"data\\MODEL\\OBJECT\\RuckSack_000\\RuckSack_000.x",				//�����b�N(��)[30]
	"data\\MODEL\\OBJECT\\RuckSack_001\\RuckSack_001.x",				//�����b�N(��)[31]
	"data\\MODEL\\OBJECT\\RuckSack_002\\RuckSack_002.x",				//�����b�N(��)[32]
	"data\\MODEL\\OBJECT\\RuckSack_003\\RuckSack_003.x",				//�����b�N(��)[33]
	"data\\MODEL\\OBJECT\\RuckSack_004\\RuckSack_004.x",				//�����b�N(��)[34]
	"data\\MODEL\\OBJECT\\AntisepticSolution_000\\AntisepticSolution.x",//����[35]
	"data\\MODEL\\OBJECT\\NintendoSwitch_001\\NintendoSwitch_001.x",	//Switch[36]
	"data\\MODEL\\OBJECT\\TV_001\\TV_001.x",							//�e���r[37]
	"data\\MODEL\\OBJECT\\clock000\\clock000.x",						//���v[38]
	"data\\MODEL\\OBJECT\\Certificate\\certificate.x",					//�܏�[39]
	"data\\MODEL\\OBJECT\\JOBI_Logo_000\\JOBI_Logo_000.x",				//Jobi���S[40]
	"data\\MODEL\\OBJECT\\Outlet_001\\Outlet_001.x",					//�R���Z���g(��)[41]
	"data\\MODEL\\OBJECT\\shelf000\\shelf000.x",						//�I[42]
	"data\\MODEL\\OBJECT\\whiteboard000\\whiteboard000.x",				//�z���C�g�{�[�h[43]
	"data\\MODEL\\OBJECT\\Window_002\\Window_002.x",					//��[44]
	"data\\MODEL\\OBJECT\\Window_002\\Window_001.x",					//��(������)[45]
	"data\\MODEL\\OBJECT\\AirController_000\\AirController_000.x",		//�G�A�R��[46]
	"data\\MODEL\\OBJECT\\Airvent000\\Airvent000.x",					//���C��[47]
	"data\\MODEL\\OBJECT\\light000\\light000.x",						//�d��[48]
	"data\\MODEL\\OBJECT\\plane000\\plane000.x",						//��s�@�P[49]
	"data\\MODEL\\OBJECT\\plane001\\plane001.x",						//��s�@�Q[50]
	"data\\MODEL\\OBJECT\\plane002\\plane002.x",						//��s�@�R[51]
	"data\\MODEL\\OBJECT\\Seagull_000\\Seagull_000.x",					//�J�����P[52]
	"data\\MODEL\\OBJECT\\Seagull_001\\Seagull_001.x",					//�J�����Q[53]
	"data\\MODEL\\OBJECT\\Seagull_002\\Seagull_002.x",					//�J�����R[54]
	"data\\MODEL\\OBJECT\\board_000\\board.x",							//��[55]

	//���:56
};

//================================================================================================================================================================================================================================
// �v���g�^�C�v�錾
//================================================================================================================================================================================================================================
void InitModel(void);															//����������
void UninitModel(void);															//�I������
void UpdateModel(void);															//�X�V����
void DrawModel(void);															//�`�揈��
void SetModel(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nUse);			//�ݒ菈��
void LoadModeltxt(void);														//���f�����̓ǂݍ��ݏ���(txt)
void LoadModelbin(void);														//���f�����̓ǂݍ��ݏ���(bin)
Model* GetModel(void);															//���̎擾
bool bCollisionModel(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 Size,int &nNumLandingModel);//�����蔻�菈��

#endif

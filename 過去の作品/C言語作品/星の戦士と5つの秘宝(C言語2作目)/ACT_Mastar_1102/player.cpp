//========================================================
//
//�W���R�O���F�G�t�F�N�g������񌩒���[player.cpp]
//Author:ShinaTaiyo
//
//=========================================================


#include "main.h"
#include "player.h"//�쐬����polygon.h���C���N���[�h����
#include "input.h"
#include "score.h"
#include "sound.h"
#include <time.h>
#include "tutorial.h"
#include "block.h"
#include "health.h"
#include "Attack.h"
#include "Ability.h"
#include "bullet.h"

#define PLAYER_MOVE (7.5f)//�v���C���[�ړ����x
#define NUM_PLAYER_MOTION (3)//�v���C���[�̃��[�V�����̎��

//�O���[�o���ϐ�
Player g_aPlayer;//�v���C���[�̍\����

LPDIRECT3DTEXTURE9 g_pTexturePolygon[NUM_PLAYER_MOTION] = {};//�e�N�X�`���ւ̃|�C���^
//VERTEX_2D g_aVertex[4];//���_�����i�[�i�l�p�`����肽���j
PDIRECT3DVERTEXBUFFER9 g_pVtxBuffPolygon = NULL;//���_�o�b�t�@�ւ̃|�C���^



int g_nCounterAnimPlayer;//�A�j���[�V�����J�E���^�[
int g_nPatternAnimPlayer;//�ړ���

float g_fSpeedUp = 0;

int g_nData = 1;

float g_fRegBullet = 0;//�e�̕��������p�ϐ�


D3DXVECTOR3 g_StartPos;//�v���C���[�̏����ʒu���Z�[�u����B

//D3DXVECTOR3 g_aPlayer.pos;//�ʒu
//D3DXVECTOR3 g_aPlayer.move;//�ړ���
//D3DXVECTOR3 g_aPlayer.rot;//����
float g_fLengthPlayer;//�Ίp���̒���
float g_fAnglePlayer;//�Ίp���̊p�x
float g_fDiagonalMovex;//�΂߈ړ��p�ϐ�
float g_fDiagonalMovey;//�΂߈ړ��p�ϐ�
int g_nMotionMode;//���샂�[�h�̐ݒ�p�ϐ�


float g_fScale=1.0f;//�g�嗦���Ǘ�����ϐ�(�����l1.0f)


bool g_bTurnPoint=false;//�E�C���h�E�̒[�ɃL���������B�����ꍇ�A���������邽�߂̃t���O
bool g_bTurnPoint2 = false;
float g_fXmove = 0;//X�����ɓ����������̕ϐ�
float g_fYmove = 0;//Y�����ɓ����������̕ϐ�

int g_nAnimation = 0;
int g_nAnimationDelay = 0;//�A�j���[�V�����̒x�����s���ϐ�
float g_nAnimationColumn = 0;//�A�j���[�V������ǂݍ��ޗ��\���ϐ�
int g_nAnimationCount = 0;

float g_fRoling=0;//��]�p�ϐ�

int g_nStayCount = 0;
bool g_bStayFlag = false;

//=======================================================
//�|���S���̏���������
//=======================================================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�ւ̃|�C���^

	Tutorial* pTutorial = GetTutorial(); 
//GameCustom * pGameCustom = GetGameCustom();

	
	//�v���C���[�\���̂̏��̏�����==========================================
	g_aPlayer.state = PLAYERSTATE_NORMAL;
	g_aPlayer.nHitCount = 0;
	g_nMotionMode = 7;
	g_aPlayer.bInvincibility = false;//���G�t���O�̏������ioff)
	g_aPlayer.bDisp = true;//�v���C���[��\�����邩�ǂ����̃t���O��on�ɂ���B
	g_aPlayer.fGravity = 0.0f;//�d�͂̒l��������
	g_nCounterAnimPlayer = 0;//�J�E���^�[������������
	g_nPatternAnimPlayer = 0;//�p�^�[��No������������
	g_aPlayer.move = D3DXVECTOR3(0.0f,0.0f,0.0f);//�ړ��ʂ�����������
	g_aPlayer.rot = D3DXVECTOR3(0.0f, 0.0f,0.0f);//����������������i�����W��ς��邱�Ƃɂ��A������ς��邱�Ƃ��ł���j
	g_aPlayer.nCounterAnim = 0; //�A�j���[�V�����J�E���^�[
	g_aPlayer.nPatternAnim = 0;//�A�j���[�V�����p�^�[��
	g_aPlayer.nDirectionMove = 0;//�����𔻒�
	g_aPlayer.bIsJumping = false;//�W�����v���Ă��邩�ǂ���
	g_aPlayer.bUse = true;//�g�p���Ă��邩�ǂ���
	g_aPlayer.bIsLanding = false;//�u���b�N�ɏ���Ă��邩�ǂ���
	g_aPlayer.bIsWalling = false;//�ǂɓ������Ă��邩�ǂ���
	g_aPlayer.g_bTouchingBlock = false;//�u���b�N�ɐG��Ă����甭������t���O�B�X���b�v�_���[�W�����ȂǂŎg��
	g_aPlayer.bDirection = false;//�E�Ɍ����Ă��邱�Ƃɂ���
	g_aPlayer.nMotionType = PLAYERMOTION_STAND;//�g�p���Ă���v���C���[�̃��[�V�����̎��
	g_aPlayer.nMotionTypeOld = PLAYERMOTION_STAND;//�O�Ɏg�p���Ă��郂�[�V������ۑ�����
	//g_aPlayer.pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 80.0f, 0.0f);//�ʒu������������
	g_aPlayer.pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 80.0f, 0.0f);//�ʒu������������
	g_aPlayer.posOld = g_aPlayer.pos;
	g_aPlayer.bUseMotion = false;//���[�V�������g�p���Ă��邩�ǂ����̃t���O
	g_aPlayer.nMotionTime = 0;//���[�V�������g�p���Ă��鎞�Ԃ𑪂�
	g_aPlayer.fWidth = PLAYER_WIDTH;//��
	g_aPlayer.fHeight = PLAYER_HEIGHT;//����
	g_aPlayer.bDashL = false;//�������ւ̃_�b�V����Ԃ��ǂ���
	g_aPlayer.bDashR = false;//�E�����ւ̃_�b�V����Ԃ��ǂ���
	g_aPlayer.nDashCount = 0;//�_�b�V����ԂɂȂ�܂ł̎��Ԃ��J�E���g����
	g_aPlayer.nDashCount = 0;//�_�b�V�����邽�߂̃{�^�����������񐔂��J�E���g
	g_aPlayer.nCntDashButtonL = 0;
	g_aPlayer.nCntDashButtonR = 0;
	g_aPlayer.nInertia = 0;//�����𒲐�����
	g_aPlayer.bAutoMoveR = false;//�E�����֎����I�Ɉړ����邩�ǂ����̃t���O
	g_aPlayer.bAutoMoveL = false;//�������֎����I�Ɉړ����邩�ǂ����̃t���O
	//===============================================================================
	//�f�o�C�X�̎擾
	pDevice = GetDevice();
	
	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\ACT_Player_StandMotion.png",
		&g_pTexturePolygon[PLAYERMOTION_STAND]);//�ҋ@���[�V����

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\ACT_Player_RunMotion_000.png",
		&g_pTexturePolygon[PLAYERMOTION_MOVE]);//�ړ����[�V����

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\ACT_PlayerMotion_Punch_000.png",
		&g_pTexturePolygon[PLAYERMOTION_PUNCH]);//�ړ����[�V����


	int nMode;

	nMode = GetMode();
	

	 g_nStayCount = 0;
	 g_bStayFlag = false;


	//if (nMode >= 2)
	//{
	//	g_aPlayer.pos = D3DXVECTOR3(SCREEN_WIDTH/2,SCREEN_HEIGHT, 0.0f);//�ʒu������������
	//}

	g_StartPos = g_aPlayer.pos;
	//srand((unsigned int)time(NULL));//�Q�[�����J�n�������Ԗ��ɗ����̎��ݒ�

	//�Ίp���̒������Z�o����
	g_fLengthPlayer = sqrtf(PLAYER_WIDTH * PLAYER_WIDTH + PLAYER_HEIGHT * PLAYER_HEIGHT) / 2.0f;//�v���C���[�̕�2��{�v���C���[�̍����Q��/�Q

	//�Ίp���̊p�x���Z�o����
	g_fAnglePlayer = atan2f(PLAYER_WIDTH,PLAYER_HEIGHT);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPolygon,
		NULL);
    
	VERTEX_2D* pVtx;//���_���ւ̃|�C���^
	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);




	//���_���W�̐ݒ�
	//pVtx[0].pos = D3DXVECTOR3(500.0f, 150.0f, 0.0f);//X���W��
 //   pVtx[1].pos = D3DXVECTOR3(650.0f, 150.0f, 0.0f);//X���W�E
	//pVtx[2].pos = D3DXVECTOR3(500.0f, 300.0f, 0.0f);//Y���W��
	//pVtx[3].pos = D3DXVECTOR3(650.0f, 300.0f, 0.0f);//Y���W��

	/*pVtx[0].pos.x = g_aPlayer.pos.x + sinf(g_aPlayer.rot.z  + g_fAnglePlayer*-0.75) * g_fLengthPlayer;
	pVtx[0].pos.y = g_aPlayer.pos.y +cosf(g_aPlayer.rot.z + g_fAnglePlayer*-0.75) * g_fLengthPlayer;
	pVtx[0].pos.z= 0.0f;

	pVtx[1].pos.x = g_aPlayer.pos.x + sinf(g_aPlayer.rot.z + g_fAnglePlayer*0.75) * g_fLengthPlayer;
	pVtx[1].pos.y = g_aPlayer.pos.y + cosf(g_aPlayer.rot.z + g_fAnglePlayer*-0.75) * g_fLengthPlayer;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_aPlayer.pos.x + sinf(g_aPlayer.rot.z + g_fAnglePlayer*-0.25) * g_fLengthPlayer;
	pVtx[2].pos.y = g_aPlayer.pos.y + cosf(g_aPlayer.rot.z + g_fAnglePlayer*+0.25) * g_fLengthPlayer;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_aPlayer.pos.x + sinf(g_aPlayer.rot.z + g_fAnglePlayer*0.25) * g_fLengthPlayer;
	pVtx[3].pos.y = g_aPlayer.pos.y + cosf(g_aPlayer.rot.z + g_fAnglePlayer*0.25) * g_fLengthPlayer;
	pVtx[3].pos.z = 0.0f;*/


	/*if (pTutorial->bTutorialUse == true)
	{
		    pVtx[0].pos=
			pVtx[1].pos=
			pVtx[2].pos=
			pVtx[3].pos=
	}*/

	//���_���W�̐ݒ�
	pVtx[0].pos.x = g_aPlayer.pos.x - PLAYER_WIDTH;
	pVtx[0].pos.y = g_aPlayer.pos.y - PLAYER_HEIGHT;
	pVtx[0].pos.z = 0.0f;
				
	pVtx[1].pos.x = g_aPlayer.pos.x + PLAYER_WIDTH;
	pVtx[1].pos.y = g_aPlayer.pos.y - PLAYER_HEIGHT;
	pVtx[1].pos.z = 0.0f;
					
	pVtx[2].pos.x = g_aPlayer.pos.x - PLAYER_WIDTH;
	pVtx[2].pos.y = g_aPlayer.pos.y;
	pVtx[2].pos.z = 0.0f;
					
	pVtx[3].pos.x = g_aPlayer.pos.x + PLAYER_WIDTH;
	pVtx[3].pos.y = g_aPlayer.pos.y;
	pVtx[3].pos.z = 0.0f;

	//g_aPlayer.PosBeside = D3DXVECTOR3(g_aPlayer.pos.x - PLAYER_WIDTH, g_aPlayer.pos.y - PLAYER_HEIGHT / 2, 0.0f);//�v���C���[�̉E�����ɁA�X�ɒ��_��ǉ�

	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255,255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255,255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255,255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255,255);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.25f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.25f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPolygon->Unlock();
}

//========================================
//�|���S���̏I������
//========================================
void UninitPlayer(void)
{
	//�e�N�X�`���̔j��
	for (int nCnt = 0; nCnt < NUM_PLAYER_MOTION; nCnt++)
	{
		if (g_pTexturePolygon[nCnt]!= NULL)
		{
			g_pTexturePolygon[nCnt]->Release();
			g_pTexturePolygon[nCnt] = NULL;
		}
	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffPolygon != NULL)
	{
		g_pVtxBuffPolygon->Release();
		g_pVtxBuffPolygon = NULL;
	}
}
//==========================================================
//�|���S���̍X�V����
//==========================================================
void UpDatePlayer(void)//�P�t���[�����Ƃɏ��������B
{
	//srand((unsigned int)time(NULL));//�Q�[�����J�n�������Ԗ��ɗ����̎��ݒ�




	Tutorial* pTutorial = GetTutorial();

	


	//g_nXarrowShot_r = rand() % 100 + 1;//�X�L���R�iXarrow�j�̒e�̕�������

	//g_fXarrowShot_r = 0.001f * g_nXarrowShot_r;//�X�L���R(Xarrow)�̒e�̕�������

	//g_nXarrowShotSpeed_r = rand() % 30 + 1;

	//g_fXarrowShotSpeed_r = 1.0f * g_nXarrowShotSpeed_r;

	//g_nXarrowShotRot_r = rand() % 4 + 1;//�X�L���R�̒e�������i�S������j�̏���

	g_fRoling += 0.01f;//��]�p�x���C���N�������g��������

		//�Ίp���̒������Z�o����(STG)
	//g_fLengthPlayer = sqrtf((PLAYER_WIDTH * PLAYER_WIDTH)*g_fScale + (PLAYER_HEIGHT * PLAYER_HEIGHT)*g_fScale) / 2.0f;//�v���C���[�̕�2��{�v���C���[�̍����Q��/�Q*�g�嗦


	//�Ίp���̒�����I�o����(ACT)
	g_fLengthPlayer = sqrtf((PLAYER_WIDTH * PLAYER_WIDTH) * g_fScale + (PLAYER_HEIGHT * PLAYER_HEIGHT) * g_fScale) / 2.0f;//�v���C���[�̕�2��{�v���C���[�̍����Q��/�Q*�g�嗦




	

	g_aPlayer.nDashCount++;

	//�u���b�N�ɒ��n���Ȃ�����A�t���O��false�ɂ��邽�߂ɁA�u���b�N�����̑O�ɒu��
	g_aPlayer.bIsLanding = false;

	//�u���b�N�̉��ɓ������Ă��Ȃ�����A�t���O��false�ɂ��邽�߂ɁA�u���b�N�����̑O�ɒu��
	g_aPlayer.bIsWalling = false;

	bool b = false;


	

	g_aPlayer.bAutoMoveR = false;//�E�����֎����I�Ɉړ����邩�ǂ����̃t���O
	g_aPlayer.bAutoMoveL = false;//�������֎����I�Ɉړ����邩�ǂ����̃t���O

	g_aPlayer.nInertia = INERTIA_NORMAL;//���ʂ̊������

	g_aPlayer.bIsLanding = bCollisionBlock(&g_aPlayer.pos, &g_aPlayer.posOld, &g_aPlayer.move, g_aPlayer.fWidth, g_aPlayer.fHeight, &g_aPlayer.fGravity, COLLISIONTYPE_RADIUS, &b,COLLISIONUSE_PLAYER,&g_aPlayer.bIsWalling);

	if (g_aPlayer.bAutoMoveR == true)
	{
		g_aPlayer.move.x += 6.0f;
	}
	if (g_aPlayer.bAutoMoveL == true)
	{
		g_aPlayer.move.x += -6.0f;
	}

	/*if (pGameCustom->nDecideCount >= 2)
	{*/
	if (GetKeyboardPress(DIK_A) == true || GetJoypadPress(JOYKEY_LEFT) == true)
	{//A�L�[�������ꂽ

		if (g_aPlayer.bDashL == false)
		{
			//if (GetKeyboardPress(DIK_W) == true || GetJoypadPress(JOYKEY_UP))
			//{//W�L�[�������ꂽ
			//	//����ړ�

			//	g_aPlayer.pos.x += sinf(-D3DX_PI * 0.75) * PLAYER_MOVE;
			//	g_aPlayer.pos.y += cosf(-D3DX_PI * 0.75) * PLAYER_MOVE;
			//}
			//else if (GetKeyboardPress(DIK_S) == true || GetJoypadPress(JOYKEY_DOWN) == true)
			//{//S�L�[�������ꂽ
			//	//�����ړ�
			//	g_aPlayer.pos.x += sinf(-D3DX_PI * 0.25) * PLAYER_MOVE;
			//	g_aPlayer.pos.y += cosf(-D3DX_PI * 0.25) * PLAYER_MOVE;
			//}
			g_aPlayer.bDirection = true;//���Ɍ����Ă��邱�Ƃɂ���
			g_aPlayer.nMotionType = PLAYERMOTION_MOVE;
			g_aPlayer.move.x += sinf(-D3DX_PI * 0.5) * PLAYER_MOVE;
			/*g_aPlayer.pos.y += cosf(-D3DX_PI * 0.5) * PLAYER_MOVE;*/
		}
	}
	else if (GetKeyboardPress(DIK_D) == true || GetJoypadPress(JOYKEY_RIGHT) == true)
	{//D�L�[�������ꂽ
		if (g_aPlayer.bDashR == false)
		{
			//if (GetKeyboardPress(DIK_W) == true || GetJoypadPress(JOYKEY_UP) == true)
			//{//W���[�������ꂽ
			//	//�E��ړ�
			//	g_aPlayer.pos.x += sinf(D3DX_PI * 0.75) * PLAYER_MOVE;
			//	g_aPlayer.pos.y += cosf(D3DX_PI * 0.75) * PLAYER_MOVE;
			//}
			//else if (GetKeyboardPress(DIK_S) == true || GetJoypadPress(JOYKEY_DOWN) == true)
			//{//S���[�������ꂽs
			//	//�E���ړ�
			//	g_aPlayer.pos.x += sinf(D3DX_PI * 0.25) * PLAYER_MOVE;
			//	g_aPlayer.pos.y += cosf(D3DX_PI * 0.25) * PLAYER_MOVE;
			//}
			//else
			//{//�E�ړ�
			g_aPlayer.bDirection = false;//�E�Ɍ����Ă��邱�Ƃɂ���
			g_aPlayer.nMotionType = PLAYERMOTION_MOVE;
			g_aPlayer.move.x += sinf(D3DX_PI * 0.5) * PLAYER_MOVE;
			/*g_aPlayer.pos.y += cosf(D3DX_PI * 0.5) * PLAYER_MOVE;*/
			/*	}*/
		}
	}
	//else if (GetKeyboardPress(DIK_W) == true || GetJoypadPress(JOYKEY_UP) == true)
	//{//W�L�[�������ꂽ
	//	g_aPlayer.pos.x += sinf(D3DX_PI * 1.0f) * PLAYER_MOVE;
	//	g_aPlayer.pos.y += cosf(D3DX_PI * 1.0f) * PLAYER_MOVE;
	//}
	//else if (GetKeyboardPress(DIK_S) == true || GetJoypadPress(JOYKEY_DOWN) == true)
	//{//S�L�[�������ꂽ
	//	g_aPlayer.pos.x += sinf(D3DX_PI * 0.0f) * PLAYER_MOVE;
	//	g_aPlayer.pos.y += cosf(D3DX_PI * 0.0f) * PLAYER_MOVE;
	//}

	
	//====================================
	//�_�b�V�������P
	//====================================
		if (GetKeyboardTrigger(DIK_A) == true || GetJoyPadTrigger(JOYKEY_LEFT) == true)
		{
			g_aPlayer.nCntDashButtonL++;
		}
		if (g_aPlayer.nCntDashButtonL >= 2)
		{
			g_aPlayer.bDashL = true;
		}

		if (GetKeyboardTrigger(DIK_D) == true || GetJoyPadTrigger(JOYKEY_RIGHT) == true)
		{
			g_aPlayer.nCntDashButtonR++;
		}
		if (g_aPlayer.nCntDashButtonR >= 2)
		{
			g_aPlayer.bDashR = true;
		}
	//=================================================================================================

	//==================================
	//�_�b�V�����
	//==================================
	if (GetKeyboardPress(DIK_A) == true || GetJoypadPress(JOYKEY_LEFT) == true)
	{//A�L�[�������ꂽ

		if (g_aPlayer.bDashL == true)
		{
			//if (GetKeyboardPress(DIK_W) == true || GetJoypadPress(JOYKEY_UP))
			//{//W�L�[�������ꂽ
			//	//����ړ�

			//	g_aPlayer.pos.x += sinf(-D3DX_PI * 0.75) * PLAYER_MOVE;
			//	g_aPlayer.pos.y += cosf(-D3DX_PI * 0.75) * PLAYER_MOVE;
			//}
			//else if (GetKeyboardPress(DIK_S) == true || GetJoypadPress(JOYKEY_DOWN) == true)
			//{//S�L�[�������ꂽ
			//	//�����ړ�
			//	g_aPlayer.pos.x += sinf(-D3DX_PI * 0.25) * PLAYER_MOVE;
			//	g_aPlayer.pos.y += cosf(-D3DX_PI * 0.25) * PLAYER_MOVE;
			//}
			g_aPlayer.bDirection = true;//���Ɍ����Ă��邱�Ƃɂ���
			g_aPlayer.nMotionType = PLAYERMOTION_MOVE;
			g_aPlayer.move.x += sinf(-D3DX_PI * 0.5) * PLAYER_MOVE * 2;
		/*	g_aPlayer.pos.y += cosf(-D3DX_PI * 0.5) * PLAYER_MOVE * 2;*/

			


		}
	}
	else if (GetKeyboardPress(DIK_D) == true || GetJoypadPress(JOYKEY_RIGHT) == true)
	{//D�L�[�������ꂽ

		if (g_aPlayer.bDashR == true)
		{
			//if (GetKeyboardPress(DIK_W) == true || GetJoypadPress(JOYKEY_UP) == true)
			//{//W���[�������ꂽ
			//	//�E��ړ�
			//	g_aPlayer.pos.x += sinf(D3DX_PI * 0.75) * PLAYER_MOVE;
			//	g_aPlayer.pos.y += cosf(D3DX_PI * 0.75) * PLAYER_MOVE;
			//}
			//else if (GetKeyboardPress(DIK_S) == true || GetJoypadPress(JOYKEY_DOWN) == true)
			//{//S���[�������ꂽs
			//	//�E���ړ�
			//	g_aPlayer.pos.x += sinf(D3DX_PI * 0.25) * PLAYER_MOVE;
			//	g_aPlayer.pos.y += cosf(D3DX_PI * 0.25) * PLAYER_MOVE;
			//}
			//else
			//{//�E�ړ�
			g_aPlayer.bDirection = false;//�E�Ɍ����Ă��邱�Ƃɂ���
			g_aPlayer.nMotionType = PLAYERMOTION_MOVE;
			g_aPlayer.move.x += sinf(D3DX_PI * 0.5) * PLAYER_MOVE * 2;
			/*g_aPlayer.pos.y += cosf(D3DX_PI * 0.5) * PLAYER_MOVE * 2;*/
		}
	
	}

		//==============
		//�W�����v����=============================================================================================================
		//==============
     
	


		Block* pBlock = GetBlock();
		

		if (g_aPlayer.bIsLanding == true)
		{//�n�ʂɂ���
			g_aPlayer.move.y = 0.0f;//�n�ʂɂ���̂ŁA�d�͂������Ȃ�
			if (GetKeyboardTrigger(DIK_SPACE) == true || GetJoyPadTrigger(JOYKEY_A) == true)
			{//�W�����v����
				PlaySound(SOUND_LABEL_SE_JUMP);
				g_aPlayer.move.y = -10.0f;
			}

		}
		if (g_aPlayer.bIsLanding == false)
		{//�n�ʂɂ��Ȃ�

			if (GetKeyboardPress(DIK_SPACE) == true || GetJoypadPress(JOYKEY_A) == true)
			{//�������ō����W�����v����悤�ɂ���
				g_aPlayer.move.y += 2.0f + (g_aPlayer.fGravity * 4.5f);
				g_aPlayer.fGravity += 0.01f;
				g_aPlayer.move.y -= 2.1f + g_aPlayer.fGravity;
			}
			else
			{
				g_aPlayer.move.y += 2.0f;
			}
		}
		if (g_aPlayer.move.y >= 20.0f)
		{
			g_aPlayer.move.y = 20.0f;
		}


	

		//�����O�̍��W��������
		g_aPlayer.posOld.x = g_aPlayer.pos.x;

		g_aPlayer.posOld.y = g_aPlayer.pos.y;

		if (g_aPlayer.nInertia == INERTIA_NORMAL)
		{
		////�ړ��ʂ��X�V�i����������j
		g_aPlayer.move.x += (0.0f - g_aPlayer.move.x) * 0.5f;
		//g_aPlayer.move.y += (0.0f - g_aPlayer.move.y) * 0.5f;
		}
		else if (g_aPlayer.nInertia == INERTIA_ICE)
		{
			////�ړ��ʂ��X�V�i����������j
			g_aPlayer.move.x += (0.0f - g_aPlayer.move.x) * 0.3f;
			//g_aPlayer.move.y += (0.0f - g_aPlayer.move.y) * 0.5f;
		}


		//�ړ�����
		g_aPlayer.pos.x += g_aPlayer.move.x;
		g_aPlayer.pos.y += g_aPlayer.move.y;

		
		
		//=============================================================
		//�_�b�V�������Q
		//=============================================================
		if (g_aPlayer.move.x <= 0.1f && g_aPlayer.move.x >= -0.1f)
		{
			g_bStayFlag = true;
			g_aPlayer.nDashCount = 0;
			g_aPlayer.bDashL = false;
			g_aPlayer.nCntDashButtonL = 0;
			g_aPlayer.bDashR = false;
			g_aPlayer.nCntDashButtonR = 0;
		}
		//================================================================================
	
		//=========================================================================================================================
	

		//ACT�̉�ʒ[����=================================================================================================
		if (g_aPlayer.pos.x > SCREEN_WIDTH + PLAYER_WIDTH / 2)
		{//��ʉE�[�܂ňړ�������
			g_aPlayer.pos.x = 0.0f + PLAYER_WIDTH / 2;//���Α��ֈړ�����B
			//g_fSpeedUp+=0.1;//�ړ����x���㏸������
		}

		if (g_aPlayer.pos.x < 0.0f - PLAYER_WIDTH / 2)
		{//��ʍ��[�܂ňړ�������
			g_aPlayer.pos.x = SCREEN_WIDTH - PLAYER_WIDTH / 2;//���Α��ֈړ�����
			//g_fSpeedUp+=0.1;//�ړ����x���㏸������
		}

		if (g_aPlayer.pos.y < 0 + PLAYER_HEIGHT / 2)
		{//��ʏ�[�܂ňړ�������
			g_aPlayer.pos.y = 0 + PLAYER_HEIGHT / 2;//��ʊO�֏o�Ă����Ȃ��悤�Ɏ~�߂�
			//g_fSpeedUp+=0.1;//�ړ����x���㏸������
		}

		if (g_aPlayer.pos.y > SCREEN_HEIGHT - PLAYER_HEIGHT / 2)//��ʂ�艺�Ɉړ�������
		{//��ʉ��[�܂ňړ�������
			g_aPlayer.pos.y = SCREEN_HEIGHT - PLAYER_HEIGHT / 2;//��ʊO�֏o�Ă����Ȃ��悤�Ɏ~�߂�i�n�ʂɈʒu���Œ肷��j
			//g_fSpeedUp+=0.1;//�ړ����x���㏸������
		}
	

		//================================================================================================================

	


		//STG�̉�ʒ[����=============================================================================================
		//if (g_aPlayer.pos.x >= SCREEN_WIDTH - PLAYER_WIDTH / 2)
		//{//��ʉE�[�܂ňړ�������
		//	g_aPlayer.pos.x = SCREEN_WIDTH - PLAYER_WIDTH / 2;//��ʊO�֏o�Ă����Ȃ��悤�Ɏ~�߂�
		//	//g_fSpeedUp+=0.1;//�ړ����x���㏸������
		//}

		//if (g_aPlayer.pos.x <= 0 + PLAYER_WIDTH / 2)
		//{//��ʍ��[�܂ňړ�������
		//	g_aPlayer.pos.x = 0 + PLAYER_WIDTH / 2;//��ʊO�֏o�Ă����Ȃ��悤�Ɏ~�߂�
		//	//g_fSpeedUp+=0.1;//�ړ����x���㏸������
		//}

		//if (g_aPlayer.pos.y <= 0 + PLAYER_HEIGHT / 2)
		//{//��ʏ�[�܂ňړ�������
		//	g_aPlayer.pos.y = 0 + PLAYER_HEIGHT / 2;//��ʊO�֏o�Ă����Ȃ��悤�Ɏ~�߂�
		//	//g_fSpeedUp+=0.1;//�ړ����x���㏸������
		//}

		//if (g_aPlayer.pos.y >= SCREEN_HEIGHT - PLAYER_HEIGHT / 2)
		//{//��ʉ��[�܂ňړ�������
		//	g_aPlayer.pos.y = SCREEN_HEIGHT - PLAYER_HEIGHT / 2;//��ʊO�֏o�Ă����Ȃ��悤�Ɏ~�߂�
		//	//g_fSpeedUp+=0.1;//�ړ����x���㏸������
		//}
		//================================================================================================================



		



		







		//================================================
		//�g��k��
		//================================================
			//if (GetKeyboardPress(DIK_RIGHT) == true)//�g����s��
			//{//�E�L�[�������ꂽ�B
			//	g_fScale+=0.1f;
			//}
			//else if (GetKeyboardPress(DIK_LEFT) == true)//�k�����s��
			//{//���L�[�������ꂽ
			//	g_fScale -= 0.1f;
			//}
			//else if (GetKeyboardPress(DIK_R) == true)//�g�嗦�����ɖ߂�
			//{//R�L�[�������ꂽ
			//	g_fScale = 1.0f;
			//}
			//
			//if (g_fScale >= 5.0f)//�g�嗦��5�{�ȏ�ɂȂ����Ƃ�
			//{
			//	g_fScale = 5.0f;//�g�嗦�̍ő�l���T�{�ɌŒ�
			//}
			//if (g_fScale <= 0.1f)//�k�������P�O���̂P�ȉ��ɂȂ����Ƃ�
			//{
			//	g_fScale = 0.1f;//�k�����̍Œ�l���P�O���̂P�ɌŒ�B
			//}
			//============================================

			//===================================================
			//�e���ˏ���
			//===================================================
		if (GetKeyboardPress(DIK_F1) == true)
		{//F1�L�[�������ꂽ
			g_nMotionMode = 1;//�g���K�[���˃��[�h�Ƀ`�F���W
		}
		if (GetKeyboardPress(DIK_F2) == true)
		{//F2�L�[�������ꂽ
			g_nMotionMode = 2;//�v���X���˃��[�h�Ƀ`�F���W
		}
		if (GetKeyboardPress(DIK_F3) == true)
		{//F3�L�[�������ꂽ
			g_nMotionMode = 3;//�v���X���˃��[�h�Ƀ`�F���W

		}
		if (GetKeyboardPress(DIK_F4) == true)
		{//F4�L�[�������ꂽ
			g_nMotionMode = 4;//�v���X���˃��[�h�Ƀ`�F���W

		}
		if (GetKeyboardPress(DIK_F5) == true)
		{//F5�L�[�������ꂽ
			g_nMotionMode = 5;//�v���X���˃��[�h�Ƀ`�F���W

		}
		if (GetKeyboardPress(DIK_F6) == true)
		{//F6�L�[�������ꂽ
			g_nMotionMode = 6;//�v���X���˃��[�h�Ƀ`�F���W

		}
		if (GetKeyboardPress(DIK_F7) == true)
		{//F7�L�[�������ꂽ
			g_nMotionMode = 7;//�v���X���˃��[�h�Ƀ`�F���W

		}

		
		
			//===============================================================================================================================================

		//====================================
		//�v���C���[���[�V��������
		//====================================

		g_aPlayer.nMotionTypeOld = g_aPlayer.nMotionType;

		Ability* pAbility = GetAbility();

		if (GetKeyboardTrigger(DIK_J) == true || GetJoyPadTrigger(JOYKEY_X) == true)
		{
			if (pAbility->nAbilityType == ABILITYTYPE_NORMAL)
			{
				PlaySound(SOUND_LABEL_SE_PUNCH);

				g_aPlayer.nMotionType = PLAYERMOTION_PUNCH;
				g_aPlayer.bUseMotion = true;
				SetAttack(D3DXVECTOR3(g_aPlayer.pos.x, g_aPlayer.pos.y, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), ATTACKTYPE_PUNCH, -1, -1, -1, MOVETYPE_NORMAL
					, 120.0f, 120.0f, 20, 0.0f, -1);
			}
		}
		if (GetKeyboardTrigger(DIK_J) == true || GetJoyPadTrigger(JOYKEY_X) == true)
		{
			if (pAbility->nAbilityType == ABILITYTYPE_BULLET)
			{
				PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_000);
				if (g_aPlayer.bDirection == false)
				{
					SetBullet(D3DXVECTOR3(g_aPlayer.pos.x, g_aPlayer.pos.y - g_aPlayer.fHeight / 2, 0.0f), D3DXVECTOR3(sinf(-D3DX_PI * 1.5f) * 8.0f, cosf(-D3DX_PI * 1.5f) * 8.0f, 0.0f),
						GENERALLY_BULLETLIFE, BULLETTYPE_PLAYER, -1, -1, -1, true, MOVETYPE_NORMAL);
				}
				else if (g_aPlayer.bDirection == true)
				{
					SetBullet(D3DXVECTOR3(g_aPlayer.pos.x, g_aPlayer.pos.y - g_aPlayer.fHeight / 2, 0.0f), D3DXVECTOR3(sinf(-D3DX_PI * 0.5f) * 8.0f, cosf(-D3DX_PI * 0.5f) * 8.0f, 0.0f),
						GENERALLY_BULLETLIFE, BULLETTYPE_PLAYER, -1, -1, -1, true, MOVETYPE_NORMAL);
				}
			}
		}

		if (g_aPlayer.pos.x == g_aPlayer.posOld.x)
		{//���̈ړ��ʂ��O�ɂȂ�����
			g_aPlayer.nMotionType = PLAYERMOTION_STAND;
		}
		//===========================================================================================

	VERTEX_2D* pVtx;
	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);
	//pVtx[0].pos.x = g_aPlayer.pos.x - 70.0f;
	//pVtx[0].pos.y = g_aPlayer.pos.y - 100.0f;

	//���_���W�̐ݒ�
	//pVtx[0].pos = D3DXVECTOR3(fEnemyMove1[0],fEnemyMove1[2], 0.0f);
	//pVtx[1].pos = D3DXVECTOR3(fEnemyMove1[1],fEnemyMove1[2], 0.0f);
	//pVtx[2].pos = D3DXVECTOR3(fEnemyMove1[0],fEnemyMove1[3], 0.0f);
	//pVtx[3].pos = D3DXVECTOR3(fEnemyMove1[1],fEnemyMove1[3], 0.0f);

	/*pVtx[0].pos = D3DXVECTOR3(g_aPlayer.pos.x+fData1,g_aPlayer.pos.y+fData3, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aPlayer.pos.x+fData2,g_aPlayer.pos.y+fData3, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aPlayer.pos.x+fData1,g_aPlayer.pos.y+fData4, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aPlayer.pos.x+fData2,g_aPlayer.pos.y+fData4, 0.0f);*/

	//pVtx[0].pos.x = g_aPlayer.pos.x + sinf(g_aPlayer.rot.z - (D3DX_PI - g_fAnglePlayer)) * g_fLengthPlayer;//�ォ�獶�ɂS�T�x
	//pVtx[0].pos.y = g_aPlayer.pos.y + cosf(g_aPlayer.rot.z - (D3DX_PI - g_fAnglePlayer)) * g_fLengthPlayer;
	//pVtx[0].pos.z = 0.0f;

	//pVtx[1].pos.x = g_aPlayer.pos.x + sinf(g_aPlayer.rot.z + (D3DX_PI - g_fAnglePlayer)) * g_fLengthPlayer;//�ォ��E�ɂS�T�x
	//pVtx[1].pos.y = g_aPlayer.pos.y + cosf(g_aPlayer.rot.z + (D3DX_PI - g_fAnglePlayer)) * g_fLengthPlayer;
	//pVtx[1].pos.z = 0.0f;


	//pVtx[2].pos.x = g_aPlayer.pos.x + sinf(g_aPlayer.rot.z - g_fAnglePlayer) * g_fLengthPlayer;//�����獶�ɂS�T�x
	//pVtx[2].pos.y = g_aPlayer.pos.y + cosf(g_aPlayer.rot.z - g_fAnglePlayer) * g_fLengthPlayer;
	//pVtx[2].pos.z = 0.0f;

	//pVtx[3].pos.x = g_aPlayer.pos.x + sinf(g_aPlayer.rot.z + g_fAnglePlayer) * g_fLengthPlayer;//������E�ɂS�T�x
	//pVtx[3].pos.y = g_aPlayer.pos.y + cosf(g_aPlayer.rot.z + g_fAnglePlayer) * g_fLengthPlayer;
	//pVtx[3].pos.z = 0.0f;

	pVtx[0].pos.x = g_aPlayer.pos.x - PLAYER_WIDTH;
	pVtx[0].pos.y = g_aPlayer.pos.y - PLAYER_HEIGHT;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_aPlayer.pos.x + PLAYER_WIDTH;
	pVtx[1].pos.y = g_aPlayer.pos.y - PLAYER_HEIGHT;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_aPlayer.pos.x - PLAYER_WIDTH;
	pVtx[2].pos.y = g_aPlayer.pos.y;//���_�������ɂ��邽��
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_aPlayer.pos.x + PLAYER_WIDTH;
	pVtx[3].pos.y = g_aPlayer.pos.y;//���_�������ɂ��邽��
	pVtx[3].pos.z = 0.0f;

	g_aPlayer.PosBeside = D3DXVECTOR3(g_aPlayer.pos.x + PLAYER_WIDTH, g_aPlayer.pos.y - PLAYER_HEIGHT, 0.0f);//�v���C���[�̉E�[�ɁA�X�ɒ��_��ǉ�


	



	if (g_aPlayer.bUseMotion == true)
	{
		g_aPlayer.nMotionType = PLAYERMOTION_PUNCH;
	}
	
	if (g_aPlayer.nMotionType != g_aPlayer.nMotionTypeOld)
	{//�O�Ɏg���Ă������[�V�����ƁA���̃��[�V��������v���Ȃ��ꍇ�A�A�j���[�V�����J�E���^�[������������B
		g_aPlayer.nCounterAnim = 0;
	}

	if (g_aPlayer.nMotionType == PLAYERMOTION_PUNCH)
	{
		g_aPlayer.nCounterAnim++;//�A�j���[�V�����J�E���^�[
		g_aPlayer.nMotionTime++;
		g_aPlayer.nMotionType = PLAYERMOTION_PUNCH;
		

		if (g_aPlayer.nCounterAnim >= 5)
		{//�A�j���[�V�����̃X�s�[�h��������
			g_aPlayer.nPatternAnim++;
			g_aPlayer.nCounterAnim = 0;
		}

		if (g_aPlayer.nPatternAnim >= 5)
		{//���[�v�p����
			g_aPlayer.nPatternAnim = 0;
		}

		if (g_aPlayer.bDirection == false)
		{
			pVtx[0].tex = D3DXVECTOR2(0.0f + g_aPlayer.nPatternAnim * 0.2f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.2f + g_aPlayer.nPatternAnim * 0.2f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f + g_aPlayer.nPatternAnim * 0.2f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.2f + g_aPlayer.nPatternAnim * 0.2f, 1.0f);
		}
		else if (g_aPlayer.bDirection == true)
		{
			pVtx[1].tex = D3DXVECTOR2(0.0f + g_aPlayer.nPatternAnim * 0.2f, 0.0f);
			pVtx[0].tex = D3DXVECTOR2(0.2f + g_aPlayer.nPatternAnim * 0.2f, 0.0f);
			pVtx[3].tex = D3DXVECTOR2(0.0f + g_aPlayer.nPatternAnim * 0.2f, 1.0f);
			pVtx[2].tex = D3DXVECTOR2(0.2f + g_aPlayer.nPatternAnim * 0.2f, 1.0f);
		}

		if (g_aPlayer.nMotionTime >= 25)
		{
			g_aPlayer.nMotionType = PLAYERMOTION_STAND;
			g_aPlayer.nPatternAnim = 0;
			g_aPlayer.nCounterAnim = 0;//�A�j���[�V�����J�E���^�[
			g_aPlayer.nMotionTime = 0;
			g_aPlayer.bUseMotion = false;
		}

	}
	else if (g_aPlayer.nMotionType == PLAYERMOTION_STAND)
	{//�ҋ@���[�V����
		g_aPlayer.nCounterAnim++;//�A�j���[�V�����J�E���^�[
		if (g_aPlayer.nCounterAnim >= 10)
		{
			g_aPlayer.nPatternAnim++;
			g_aPlayer.nCounterAnim = 0;
		}

		if (g_aPlayer.nPatternAnim >= 4)
		{
			g_aPlayer.nPatternAnim = 0;
		}

		if (g_aPlayer.bDirection == false)
		{
			pVtx[0].tex = D3DXVECTOR2(0.0f + g_aPlayer.nPatternAnim * 0.25f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.25f + g_aPlayer.nPatternAnim * 0.25f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f + g_aPlayer.nPatternAnim * 0.25f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.25f + g_aPlayer.nPatternAnim * 0.25f, 1.0f);
		}
		else if (g_aPlayer.bDirection == true)
		{
			pVtx[1].tex = D3DXVECTOR2(0.0f + g_aPlayer.nPatternAnim * 0.25f, 0.0f);
			pVtx[0].tex = D3DXVECTOR2(0.25f + g_aPlayer.nPatternAnim * 0.25f, 0.0f);
			pVtx[3].tex = D3DXVECTOR2(0.0f + g_aPlayer.nPatternAnim * 0.25f, 1.0f);
			pVtx[2].tex = D3DXVECTOR2(0.25f + g_aPlayer.nPatternAnim * 0.25f, 1.0f);
		}
	}
	else if (g_aPlayer.nMotionType == PLAYERMOTION_MOVE)
	{//�ړ����[�V����
		g_aPlayer.nCounterAnim++;//�A�j���[�V�����J�E���^�[
		if (g_aPlayer.nCounterAnim >= 5)
		{
			g_aPlayer.nPatternAnim++;
			g_aPlayer.nCounterAnim = 0;
		}

		if (g_aPlayer.nPatternAnim >= 8)
		{
			g_aPlayer.nPatternAnim = 0;
		}

		if (g_aPlayer.bDirection == false)
		{
			pVtx[0].tex = D3DXVECTOR2(0.0f + g_aPlayer.nPatternAnim * 0.125f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.125f + g_aPlayer.nPatternAnim * 0.125f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f + g_aPlayer.nPatternAnim * 0.125f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.125f + g_aPlayer.nPatternAnim * 0.125f, 1.0f);
		}
		else if (g_aPlayer.bDirection == true)
		{
			pVtx[1].tex = D3DXVECTOR2(0.0f + g_aPlayer.nPatternAnim * 0.125f, 0.0f);
			pVtx[0].tex = D3DXVECTOR2(0.125f + g_aPlayer.nPatternAnim * 0.125f, 0.0f);
			pVtx[3].tex = D3DXVECTOR2(0.0f + g_aPlayer.nPatternAnim * 0.125f, 1.0f);
			pVtx[2].tex = D3DXVECTOR2(0.125f + g_aPlayer.nPatternAnim * 0.125f, 1.0f);
		}
	}


	switch(g_aPlayer.state)
		{
	case PLAYERSTATE_NORMAL:
			break;
	case PLAYERSTATE_DAMAGE:
		g_aPlayer.nCounterState--;
		if (g_aPlayer.nCounterState >= 1)
		{
			g_aPlayer.bInvincibility = true;//�_���[�W��ԂȂ�A���G��ԂɂȂ�B

			if (g_aPlayer.nCounterState % 2 == 0)
			{
				g_aPlayer.bDisp = false;//�v���C���[�̕`�����������\�����肷�邱�Ƃɂ��A�_�ł�\���B
			}
			else
			{
				g_aPlayer.bDisp = true;//�v���C���[�̕`�����������\�����肷�邱�Ƃɂ��A�_�ł�\���B
				pVtx[0].col = D3DCOLOR_RGBA(255,0,0,255);
				pVtx[1].col = D3DCOLOR_RGBA(255,0,0,255);
				pVtx[2].col = D3DCOLOR_RGBA(255,0,0,255);
				pVtx[3].col = D3DCOLOR_RGBA(255,0,0,255);
			}

		}
		if (g_aPlayer.nCounterState <= 0)
		{
			g_aPlayer.bDisp = true;
			g_aPlayer.bInvincibility = false;//���G��ԉ����B
			g_aPlayer.state = PLAYERSTATE_NORMAL;
			pVtx[0].col = D3DCOLOR_RGBA(255,255,255,255);
			pVtx[1].col = D3DCOLOR_RGBA(255,255,255,255);
			pVtx[2].col = D3DCOLOR_RGBA(255,255,255,255);
			pVtx[3].col = D3DCOLOR_RGBA(255,255,255,255);
		}
		break;
	case PLAYERSTATE_DEATH:
		g_aPlayer.nCounterState--;
		if (g_aPlayer.nCounterState <= 0)
		{
			return;//�֐����I��点����
		}
		
		
	}/*if (pTutorial->bTutorialUse == true)
	{
		if (g_aPlayer.pos.x <= g_StartPos.x - 50.0f)
		{
			g_aPlayer.pos.x = g_StartPos.x - 50.0f;
		}
		if (g_aPlayer.pos.x >= g_StartPos.x + 150.0f)
		{
			g_aPlayer.pos.x = g_StartPos.x + 150.0f;
		}
		if (g_aPlayer.pos.y <= g_StartPos.y)
		{
			g_aPlayer.pos.y = g_StartPos.y;
		}
		if (g_aPlayer.pos.y >= g_StartPos.y + 140.0f)
		{
			g_aPlayer.pos.y = g_StartPos.y +140.0f;
		}
	}*/

	//if (g_nAnimationDelay == 5)//�x��
	//{// �x���J�E���g��5�ɓ��B����
	//	g_nAnimation++;				// U�̍��W���C���N�������g
	//	g_nAnimationDelay = 0;		// �x���J�E���g�����Z�b�g
	//}

	//if ((g_nAnimation+1) % 5 == 0)
	//{// ��ԉE�̃R�}��`�ʂ���
	//	g_nAnimationCount++;		// V�̍��W���C���N�������g
	//}

	//if (g_nAnimationCount > 1)
	//{// ��ԉ�
	//	g_nAnimationCount = 0;		// V�̍��W�����Z�b�g
	//}

	//if (g_nAnimation >= 4)
	//{// ��ԉE
	//	g_nAnimation = 0;			// U�̍��W�����Z�b�g
	//}

	//g_nAnimationDelay++;//�A�j���[�V�����̑��x��x������ϐ�

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPolygon->Unlock();
}
//====================================================
//�|���S���̕`�揈��
//==========================================================
	void DrawPlayer(void)
	{
		LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�ւ̃|�C���^

		//�f�o�C�X�̎擾
		pDevice = GetDevice();

		if (g_aPlayer.bDisp == true)
		{
			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffPolygon, 0, sizeof(VERTEX_2D));//���_�o�b�t�@�ւ̃|�C���^�ƒ��_���̍\���̂̃T�C�Y

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_2D);

			//�e�N�X�`���̐ݒ�
			if (g_aPlayer.nMotionType == PLAYERMOTION_STAND)
			{
				pDevice->SetTexture(0, g_pTexturePolygon[PLAYERMOTION_STAND]);
			}
			else if (g_aPlayer.nMotionType == PLAYERMOTION_MOVE)
			{
				pDevice->SetTexture(0, g_pTexturePolygon[PLAYERMOTION_MOVE]);
			}
			else if (g_aPlayer.nMotionType == PLAYERMOTION_PUNCH)
			{
				pDevice->SetTexture(0, g_pTexturePolygon[PLAYERMOTION_PUNCH]);
			}

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);//0�́A�`�悷��ŏ��̒��_�C���f�b�N�X�ŁA2�́A�`�悷��v���~�e�B�u��
				//&g_pVtxBuffPolygon[0],//���_���̐擪�A�h���X
				//sizeof(VERTEX_2D));//���_���\���̂̃T�C�Y
		}
	}

	//�v���C���[�̃q�b�g����
	void HitPlayer(int nDamage)
	{
		int nCntDisp;
		VERTEX_2D* pVtx;
		//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		//g_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);

		////Health* pHealth = GetHealth();//�v���C���[�̗̑͂̏����擾����B

		//pHealth->fHealth += nDamage;

		//g_aPlayer.nHitCount++;
		//if (pHealth->fHealth >= 3)
		//{//�v���C���[�̗̑͂��Ȃ��Ȃ�����...
		//	SetExplosion(g_aPlayer.pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.0f), EXPLOSIONTYPE_NORMAL);
		//	g_aPlayer.bDisp = false;
		//	g_aPlayer.state = PLAYERSTATE_DEATH;
		//	g_aPlayer.nCounterState = 60;
		//}
		//else
		//{
		//	g_aPlayer.state = PLAYERSTATE_DAMAGE;
		//	
		//	g_aPlayer.nCounterState = 100;
		//}
		////���_�o�b�t�@���A�����b�N����
		//g_pVtxBuffPolygon->Unlock();
	}
	//�v���C���[�̎擾
	Player* GetPlayer(void)
	{
		return &g_aPlayer;
		
	}

	void SetPlayer(D3DXVECTOR3 pos)
	{
		g_aPlayer.pos = pos;
		g_aPlayer.posOld = pos;
	}
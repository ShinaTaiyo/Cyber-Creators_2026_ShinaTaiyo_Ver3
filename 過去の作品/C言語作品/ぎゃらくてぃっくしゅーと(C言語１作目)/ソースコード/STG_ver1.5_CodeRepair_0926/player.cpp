//========================================================
//
//�W���R�O���F�G�t�F�N�g������񌩒���[player.cpp]
//Author:ShinaTaiyo
//
//=========================================================


#include "main.h"
#include "player.h"//�쐬����polygon.h���C���N���[�h����
#include "input.h"
#include "bullet.h"
#include "explosion.h"
#include "score.h"
#include "Gage.h"
#include "enemy.h"
#include "SkillName.h"
#include "health.h"
#include "sound.h"
#include <time.h>
#include "SkillGage.h"
#include "tutorial.h"
#include "Difficulty.h"
#include "staging.h"
#define PLAYER_MOVE (5.5f)
#define PLAYER_WIDTH (50.0f)//�e�N�X�`���̉���
#define PLAYER_HEIGHT (50.0f)//�e�N�X�`���̗���

//�O���[�o���ϐ�
Player g_aPlayer;//�v���C���[�̍\����

LPDIRECT3DTEXTURE9 g_pTexturePolygon = NULL;//�e�N�X�`���ւ̃|�C���^
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

//==================================
//�X�L���R�u�wzrrow�v�̏���
//==================================
int g_nXarrowShot_r;
float g_fXarrowShot_r;

int g_nXarrowShotSpeed_r;
float g_fXarrowShotSpeed_r;

int g_nXarrowShotRot_r;
float g_fXarrowShotRot_r;
//==========================================================



//=======================================================
//�|���S���̏���������
//=======================================================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�ւ̃|�C���^

	Tutorial* pTutorial = GetTutorial(); 

	GameCustom * pGameCustom = GetGameCustom();

	g_aPlayer.state = PLAYERSTATE_NORMAL;

	g_aPlayer.nHitCount = 0;

	g_nMotionMode = 7;

	g_aPlayer.bInvincibility = false;//���G�t���O�̏������ioff)

	g_aPlayer.bDisp = true;//�v���C���[��\�����邩�ǂ����̃t���O��on�ɂ���B

	//�f�o�C�X�̎擾
	pDevice = GetDevice();
	
	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\Re_MainCharacter.png",
		&g_pTexturePolygon);

	g_nCounterAnimPlayer = 0;//�J�E���^�[������������
	g_nPatternAnimPlayer = 0;//�p�^�[��No������������
	g_aPlayer.pos = D3DXVECTOR3(1000.0f,150.0f,0.0f);//�ʒu������������
	g_aPlayer.move = D3DXVECTOR3(6.0f,8.0f,0.0f);//�ړ��ʂ�����������
	g_aPlayer.rot = D3DXVECTOR3(0.0f, 0.0f,0.0f);//����������������i�����W��ς��邱�Ƃɂ��A������ς��邱�Ƃ��ł���j

	int nMode;

	nMode = GetMode();

	if (nMode >= 2)
	{
		g_aPlayer.pos = D3DXVECTOR3(SCREEN_WIDTH/2,SCREEN_HEIGHT, 0.0f);//�ʒu������������
	}

	g_StartPos = g_aPlayer.pos;

	g_nXarrowShot_r = 0;

	g_fXarrowShot_r = 0.0f;

	g_nXarrowShotSpeed_r = 0;

	g_fXarrowShotSpeed_r = 0.0f;

	g_nXarrowShotRot_r = 0;
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

	pVtx[0].pos.x = g_aPlayer.pos.x + sinf(g_aPlayer.rot.z -(D3DX_PI -g_fAnglePlayer)) * g_fLengthPlayer;//�ォ�獶�ɂS�T�x
	pVtx[0].pos.y = g_aPlayer.pos.y + cosf(g_aPlayer.rot.z -(D3DX_PI -g_fAnglePlayer)) * g_fLengthPlayer;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_aPlayer.pos.x + sinf(g_aPlayer.rot.z + (D3DX_PI-g_fAnglePlayer)) * g_fLengthPlayer;//�ォ��E�ɂS�T�x
	pVtx[1].pos.y = g_aPlayer.pos.y + cosf(g_aPlayer.rot.z + (D3DX_PI-g_fAnglePlayer)) * g_fLengthPlayer;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_aPlayer.pos.x + sinf(g_aPlayer.rot.z - g_fAnglePlayer) * g_fLengthPlayer;//�����獶�ɂS�T�x
	pVtx[2].pos.y = g_aPlayer.pos.y + cosf(g_aPlayer.rot.z - g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_aPlayer.pos.x + sinf(g_aPlayer.rot.z + g_fAnglePlayer) * g_fLengthPlayer;//������E�ɂS�T�x
	pVtx[3].pos.y = g_aPlayer.pos.y + cosf(g_aPlayer.rot.z + g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[3].pos.z = 0.0f;

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
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPolygon->Unlock();
}

//========================================
//�|���S���̏I������
//========================================
void UninitPlayer(void)
{
	//�e�N�X�`���̔j��
	if (g_pTexturePolygon != NULL)
	{
		g_pTexturePolygon->Release();
		g_pTexturePolygon = NULL;
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

	
	SkillGage *pSkillGage = GetSkillGage();

	Tutorial* pTutorial = GetTutorial();

	g_nAnimationCount++;//�A�j���[�V�����̒x�����s���J�E���g�ig_nAnimationCount%n==0)�ȂǂŒx������

	g_nXarrowShot_r = rand() % 100 + 1;//�X�L���R�iXarrow�j�̒e�̕�������

	g_fXarrowShot_r= 0.001f * g_nXarrowShot_r;//�X�L���R(Xarrow)�̒e�̕�������

	g_nXarrowShotSpeed_r = rand() % 30 + 1;

	g_fXarrowShotSpeed_r = 1.0f * g_nXarrowShotSpeed_r;

	g_nXarrowShotRot_r = rand() % 4 + 1;//�X�L���R�̒e�������i�S������j�̏���

	g_fRoling += 0.01f;//��]�p�x���C���N�������g��������

		//�Ίp���̒������Z�o����
	g_fLengthPlayer = sqrtf((PLAYER_WIDTH * PLAYER_WIDTH)*g_fScale + (PLAYER_HEIGHT * PLAYER_HEIGHT)*g_fScale) / 2.0f;//�v���C���[�̕�2��{�v���C���[�̍����Q��/�Q*�g�嗦

	GameCustom* pGameCustom = GetGameCustom();
	/*if (pGameCustom->nDecideCount >= 2)
	{*/
		if (GetKeyboardPress(DIK_A) == true || GetJoypadPress(JOYKEY_LEFT) == true)
		{//A�L�[�������ꂽ
			if (GetKeyboardPress(DIK_W) == true || GetJoypadPress(JOYKEY_UP))
			{//W�L�[�������ꂽ
				//����ړ�

				g_aPlayer.pos.x += sinf(-D3DX_PI * 0.75) * PLAYER_MOVE;
				g_aPlayer.pos.y += cosf(-D3DX_PI * 0.75) * PLAYER_MOVE;
			}
			else if (GetKeyboardPress(DIK_S) == true || GetJoypadPress(JOYKEY_DOWN) == true)
			{//S�L�[�������ꂽ
				//�����ړ�
				g_aPlayer.pos.x += sinf(-D3DX_PI * 0.25) * PLAYER_MOVE;
				g_aPlayer.pos.y += cosf(-D3DX_PI * 0.25) * PLAYER_MOVE;
			}
			else
			{
				g_aPlayer.pos.x += sinf(-D3DX_PI * 0.5) * PLAYER_MOVE;
				g_aPlayer.pos.y += cosf(-D3DX_PI * 0.5) * PLAYER_MOVE;
			}
		}


		else if (GetKeyboardPress(DIK_D) == true || GetJoypadPress(JOYKEY_RIGHT) == true)
		{//D�L�[�������ꂽ

			if (GetKeyboardPress(DIK_W) == true || GetJoypadPress(JOYKEY_UP) == true)
			{//W���[�������ꂽ
				//�E��ړ�
				g_aPlayer.pos.x += sinf(D3DX_PI * 0.75) * PLAYER_MOVE;
				g_aPlayer.pos.y += cosf(D3DX_PI * 0.75) * PLAYER_MOVE;
			}
			else if (GetKeyboardPress(DIK_S) == true || GetJoypadPress(JOYKEY_DOWN) == true)
			{//S���[�������ꂽs
				//�E���ړ�
				g_aPlayer.pos.x += sinf(D3DX_PI * 0.25) * PLAYER_MOVE;
				g_aPlayer.pos.y += cosf(D3DX_PI * 0.25) * PLAYER_MOVE;
			}
			else
			{//�E�ړ�
				g_aPlayer.pos.x += sinf(D3DX_PI * 0.5) * PLAYER_MOVE;
				g_aPlayer.pos.y += cosf(D3DX_PI * 0.5) * PLAYER_MOVE;
			}
		}
		else if (GetKeyboardPress(DIK_W) == true || GetJoypadPress(JOYKEY_UP) == true)
		{//W�L�[�������ꂽw
			g_aPlayer.pos.x += sinf(D3DX_PI * 1.0f) * PLAYER_MOVE;
			g_aPlayer.pos.y += cosf(D3DX_PI * 1.0f) * PLAYER_MOVE;
		}
		else if (GetKeyboardPress(DIK_S) == true || GetJoypadPress(JOYKEY_DOWN) == true)
		{//S�L�[�������ꂽ
			g_aPlayer.pos.x += sinf(D3DX_PI * 0.0f) * PLAYER_MOVE;
			g_aPlayer.pos.y += cosf(D3DX_PI * 0.0f) * PLAYER_MOVE;
		}

		////�ʒu���X�V
		//g_aPlayer.pos.x += g_aPlayer.move.x;
		//g_aPlayer.pos.y += g_aPlayer.move.y;

		////�ړ��ʂ��X�V�i����������j
		//g_aPlayer.move.x += (0.0f - g_aPlayer.move.x) * 0.5f;
		//g_aPlayer.move.y += (0.0f - g_aPlayer.move.y) * 0.5f;


		if (g_aPlayer.pos.x >= SCREEN_WIDTH - PLAYER_WIDTH / 2)
		{//��ʉE�[�܂ňړ�������
			g_aPlayer.pos.x = SCREEN_WIDTH - PLAYER_WIDTH / 2;//��ʊO�֏o�Ă����Ȃ��悤�Ɏ~�߂�
			//g_fSpeedUp+=0.1;//�ړ����x���㏸������
		}

		if (g_aPlayer.pos.x <= 0 + PLAYER_WIDTH / 2)
		{//��ʍ��[�܂ňړ�������
			g_aPlayer.pos.x = 0 + PLAYER_WIDTH / 2;//��ʊO�֏o�Ă����Ȃ��悤�Ɏ~�߂�
			//g_fSpeedUp+=0.1;//�ړ����x���㏸������
		}

		if (g_aPlayer.pos.y <= 0 + PLAYER_HEIGHT / 2)
		{//��ʏ�[�܂ňړ�������
			g_aPlayer.pos.y = 0 + PLAYER_HEIGHT / 2;//��ʊO�֏o�Ă����Ȃ��悤�Ɏ~�߂�
			//g_fSpeedUp+=0.1;//�ړ����x���㏸������
		}

		if (g_aPlayer.pos.y >= SCREEN_HEIGHT - PLAYER_HEIGHT / 2)
		{//��ʉ��[�܂ňړ�������
			g_aPlayer.pos.y = SCREEN_HEIGHT - PLAYER_HEIGHT / 2;//��ʊO�֏o�Ă����Ȃ��悤�Ɏ~�߂�
			//g_fSpeedUp+=0.1;//�ړ����x���㏸������
		}

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

		
		if (GetKeyboardPress(DIK_SPACE) == true
			|| GetJoypadPress(JOYKEY_X))
		{//SPACE�L�[�������ꂽ
			//�ϓ���]���[�h
			if (g_nMotionMode == 7 && g_nAnimationCount % 4 == 0)
			{

				PlaySound(SOUND_LABEL_SE_SHOT);

				if (g_nAnimationCount % 2 == 0)
				{
					g_fRoling += 0.033f;//�P��]�����Ƃ��ɉ�]�p�ϐ���������
				}

				if (GetKeyboardPress(DIK_RIGHT) == true || GetJoypadPress(JOYKEY_R2) == true)
				{//�E�L�[�������ꂽ
					g_fRegBullet += 0.1f;
				}
				if (GetKeyboardPress(DIK_LEFT) == true || GetJoypadPress(JOYKEY_L2) == true)
				{//�E�L�[�������ꂽ
					g_fRegBullet -= 0.1f;
				}



				SetBullet(D3DXVECTOR3(g_aPlayer.pos.x, g_aPlayer.pos.y - 15.0f, 0.0f), D3DXVECTOR3((sinf(-D3DX_PI * 1.0f) * 16.0f), (cosf(-D3DX_PI * 1.0f) * 16.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_PLAYER, 0, -1, -1);
				SetBullet(D3DXVECTOR3(g_aPlayer.pos.x, g_aPlayer.pos.y - 15.0f, 0.0f), D3DXVECTOR3((sinf(-D3DX_PI * 0.98f + g_fRegBullet) * 16.0f), (cosf(-D3DX_PI * 0.98f + g_fRegBullet) * 16.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_PLAYER, 0, -1, -1);
				SetBullet(D3DXVECTOR3(g_aPlayer.pos.x, g_aPlayer.pos.y - 15.0f, 0.0f), D3DXVECTOR3((sinf(-D3DX_PI * 0.96f + g_fRegBullet) * 16.0f), (cosf(-D3DX_PI * 0.96f + g_fRegBullet) * 16.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_PLAYER, 0, -1, -1);
				SetBullet(D3DXVECTOR3(g_aPlayer.pos.x, g_aPlayer.pos.y - 15.0f, 0.0f), D3DXVECTOR3((sinf(D3DX_PI * 0.98f - g_fRegBullet) * 16.0f), (cosf(D3DX_PI * 0.98f - g_fRegBullet) * 16.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_PLAYER, 0, -1, -1);
				SetBullet(D3DXVECTOR3(g_aPlayer.pos.x, g_aPlayer.pos.y - 15.0f, 0.0f), D3DXVECTOR3((sinf(D3DX_PI * 0.96f - g_fRegBullet) * 16.0f), (cosf(D3DX_PI * 0.96f - g_fRegBullet) * 16.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_PLAYER, 0, -1, -1);

				SetBullet(D3DXVECTOR3(g_aPlayer.pos.x, g_aPlayer.pos.y + 15.0f, 0.0f), D3DXVECTOR3((sinf(-D3DX_PI * 0.0f) * 16.0f), (cosf(-D3DX_PI * 0.0f) * 16.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_PLAYER, 0, -1, -1);
				SetBullet(D3DXVECTOR3(g_aPlayer.pos.x, g_aPlayer.pos.y + 15.0f, 0.0f), D3DXVECTOR3((sinf(-D3DX_PI * 0.05f + g_fRegBullet) * 16.0f), (cosf(-D3DX_PI * 0.05f + g_fRegBullet) * 16.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_PLAYER, 0, -1, -1);
				SetBullet(D3DXVECTOR3(g_aPlayer.pos.x, g_aPlayer.pos.y + 15.0f, 0.0f), D3DXVECTOR3((sinf(D3DX_PI * 0.05f - g_fRegBullet) * 16.0f), (cosf(D3DX_PI * 0.05f - g_fRegBullet) * 16.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_PLAYER, 0, -1, -1);
			}
		}

		/*if (GetKeyboardPress(DIK_H) == true && g_nAnimationCount % 4 == 0)
		{
			int nCntEnemy;
			Enemy* pEnemy = GetEnemy();
			for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
			{


				if (pEnemy->bUse == true)
				{

					SetBullet(D3DXVECTOR3(g_aPlayer.pos.x, g_aPlayer.pos.y - 15.0f, 0.0f), D3DXVECTOR3((sinf(-D3DX_PI * 1.0f) * 16.0f), (cosf(-D3DX_PI * 1.0f) * 36.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_HORMING, 0, -1, nCntEnemy);
					SetBullet(D3DXVECTOR3(g_aPlayer.pos.x, g_aPlayer.pos.y - 15.0f, 0.0f), D3DXVECTOR3((sinf(-D3DX_PI * 0.98f + g_fRegBullet) * 16.0f), (cosf(-D3DX_PI * 0.98f + g_fRegBullet) * 36.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_HORMING, 1, -1, nCntEnemy);
					SetBullet(D3DXVECTOR3(g_aPlayer.pos.x, g_aPlayer.pos.y - 15.0f, 0.0f), D3DXVECTOR3((sinf(-D3DX_PI * 0.96f + g_fRegBullet) * 16.0f), (cosf(-D3DX_PI * 0.96f + g_fRegBullet) * 36.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_HORMING, 2, -1, nCntEnemy);
					SetBullet(D3DXVECTOR3(g_aPlayer.pos.x, g_aPlayer.pos.y - 15.0f, 0.0f), D3DXVECTOR3((sinf(D3DX_PI * 0.98f - g_fRegBullet) * 16.0f), (cosf(D3DX_PI * 0.98f - g_fRegBullet) * 36.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_HORMING, -1, -1, nCntEnemy);
					SetBullet(D3DXVECTOR3(g_aPlayer.pos.x, g_aPlayer.pos.y - 15.0f, 0.0f), D3DXVECTOR3((sinf(D3DX_PI * 0.96f - g_fRegBullet) * 16.0f), (cosf(D3DX_PI * 0.96f - g_fRegBullet) * 36.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_HORMING, -2, -1, nCntEnemy);
				}
			}
		}*/
		/*SetBullet(g_aPlayer.pos, (g_aPlayer.move) / 23.0f);*/
		//�e�̐ݒ�%
		/*SetBullet(g_aPlayer.pos, (g_aPlayer.move) / 23.0f);*/
	//	StopSound(SOUND_LABEL_SE_SHOT);


	//===================================================================================================================
	//�X�L���A�^�b�N
	//===================================================================================================================
		int nCntSkillGage;
		Skill* pSkill = GetSkill();
		Health* pHealth = GetHealth();
		for (nCntSkillGage = 0; nCntSkillGage < 2; nCntSkillGage++, pSkillGage++)
		{
			if (nCntSkillGage == 0)//��ڂ̃|�C���^�̃X�L���Q�[�W���g��
			{
				if (GetKeyboardTrigger(DIK_J) == true || GetJoyPadTrigger(JOYKEY_A) == true)
				{//SPACE�L�[�������ꂽ
					//�X�L���Q�[�W���P�O�O�ȏ�Ȃ�E�E�E
					
					

					//�e�̐ݒ�
					if (pSkillGage->fSkillGage >= 50.0f && pSkill->fSkillPattern == 0)
					{
						g_aPlayer.nSkillUseCount++;
						PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_003);
						SetBullet(g_aPlayer.pos, D3DXVECTOR3((sinf(-D3DX_PI * 1.0f) * 16.0f), (cosf(-D3DX_PI * 1.0f) * 16.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_SKILL1, 0, -1, -1);
						/*	if (pSkillGage->nType == GAGETYPE_SKILL)
							{*/
						pSkillGage->fSkillGage -= 50.0f;
						//}
					}
				}
				if (GetKeyboardTrigger(DIK_J) == true || GetJoyPadTrigger(JOYKEY_A) == true)
				{
					if (pSkillGage->fSkillGage >= 500.0f && pSkill->fSkillPattern == 4)
					{
						g_aPlayer.nSkillUseCount++;
						PlaySound(SOUND_LABEL_SE_HEAL_000);
						pHealth->fHealth--;//�̗͂��񕜂���I
						/*if (pSkillGage->nType == GAGETYPE_SKILL)
						{*/
						pSkillGage->fSkillGage -= 500.0f;
						//}
					}
				}
				if (GetKeyboardTrigger(DIK_J) == true || GetJoyPadTrigger(JOYKEY_A) == true)
				{//SPACE�L�[�������ꂽ
					//�X�L���Q�[�W���P�O�O�ȏ�Ȃ�E�E�E
					//�e�̐ݒ�
					if (pSkillGage->fSkillGage >= 100.0f && pSkill->fSkillPattern == 1)
					{
						g_aPlayer.nSkillUseCount++;
						PlaySound(SOUND_LABEL_SE_SLASH_000);



						SetBullet(g_aPlayer.pos, D3DXVECTOR3((sinf(-D3DX_PI * 0.0f) * 0.0f), (cosf(-D3DX_PI * 1.0f) * 0.0f), 0.0f), 90, BULLETTYPE_SKILL2_SLASH, 0, -1, -1);


						pSkillGage->fSkillGage -= 100.0f;
					}

				}
				if (GetKeyboardTrigger(DIK_J) == true || GetJoyPadTrigger(JOYKEY_A) == true)
				{//SPACE�L�[�������ꂽ
					//srand((unsigned int)time(NULL));//�Q�[�����J�n�������Ԗ��ɗ����̎��ݒ�
					//�X�L���Q�[�W���P�O�O�ȏ�Ȃ�E�E�E
					//�e�̐ݒ�
					if (pSkillGage->fSkillGage >= 180.0f && pSkill->fSkillPattern == 2)
					{
						g_aPlayer.nSkillUseCount++;
						PlaySound(SOUND_LABEL_SE_ARROWSHOT_000);

						pSkillGage->fSkillGage -= 180.0f;


						for (int nCnt = 0; nCnt < 40; nCnt++)
						{
							g_nXarrowShot_r = rand() % 100 + 1;//�X�L���R�iXarrow�j�̒e�̕�������
							g_fXarrowShot_r = 0.0001f * g_nXarrowShot_r;//�X�L���R(Xarrow)�̒e�̕�������

							g_nXarrowShotSpeed_r = rand() % 40 + 1;
							g_fXarrowShotSpeed_r = 1.0f * g_nXarrowShotSpeed_r;

							g_nXarrowShotRot_r = rand() % 4 + 1;//�X�L���R�̒e�������i�S������j�̏���

							if (g_nXarrowShotRot_r == 1)
							{
								SetBullet(g_aPlayer.pos, D3DXVECTOR3((sinf(-D3DX_PI * (0.25f + g_fXarrowShot_r)) * (5.0f + g_fXarrowShotSpeed_r)), (cosf(-D3DX_PI * (0.25f + g_fXarrowShot_r)) * (5.0f + g_fXarrowShotSpeed_r)), 0.0f), 120, BULLETTYPE_ARROW, 0, -1, -1);
							}
							else if (g_nXarrowShotRot_r == 2)
							{
								SetBullet(g_aPlayer.pos, D3DXVECTOR3((sinf(-D3DX_PI * (0.75f + g_fXarrowShot_r)) * (5.0f + g_fXarrowShotSpeed_r)), (cosf(-D3DX_PI * (0.75f + g_fXarrowShot_r)) * (5.0f + g_fXarrowShotSpeed_r)), 0.0f), 120, BULLETTYPE_ARROW, 0, -1, -1);
							}
							else if (g_nXarrowShotRot_r == 3)
							{
								SetBullet(g_aPlayer.pos, D3DXVECTOR3((sinf(-D3DX_PI * (1.25f + g_fXarrowShot_r)) * (5.0f + g_fXarrowShotSpeed_r)), (cosf(-D3DX_PI * (1.25f + g_fXarrowShot_r)) * (5.0f + g_fXarrowShotSpeed_r)), 0.0f), 120, BULLETTYPE_ARROW, 0, -1, -1);
							}
							else if (g_nXarrowShotRot_r == 4)
							{
								SetBullet(g_aPlayer.pos, D3DXVECTOR3((sinf(-D3DX_PI * (1.75f + g_fXarrowShot_r)) * (5.0f + g_fXarrowShotSpeed_r)), (cosf(-D3DX_PI * (1.75f + g_fXarrowShot_r)) * (5.0f + g_fXarrowShotSpeed_r)), 0.0f), 120, BULLETTYPE_ARROW, 0, -1, -1);
							}
						}
					}
				}
				if (GetKeyboardTrigger(DIK_J) == true || GetJoyPadTrigger(JOYKEY_A) == true)
				{//SPACE�L�[�������ꂽ
					if (pSkillGage->fSkillGage >= 750.0f && pSkill->fSkillPattern == 3)
					{
						g_aPlayer.nSkillUseCount++;
						Enemy* pEnemy = GetEnemy();
						//�X�L���Q�[�W���P�O�O�ȏ�Ȃ�E�E�E
						//�e�̐ݒ�
						PlaySound(SOUND_LABEL_SE_SLASH_000);
						int nCnt;
						float fRotNova = 1.0f / 50.0f;


						for (nCnt = 0; nCnt < 100; nCnt++, pEnemy++)
						{
							SetBullet(g_aPlayer.pos, D3DXVECTOR3((sinf(D3DX_PI * (fRotNova * nCnt)) * 20.0f), (cosf(D3DX_PI * (fRotNova * nCnt)) * 20.0f), 0.0f), 90, BULLETTYPE_NOVA, 0, -1, -1);

						}



						pSkillGage->fSkillGage -= 750.0f;

					}
				}
				if (GetKeyboardTrigger(DIK_J) == true || GetJoyPadTrigger(JOYKEY_A) == true)
				{
					if (pSkillGage->fSkillGage >= 200.0f && pSkill->fSkillPattern == 5)
					{
						g_aPlayer.nSkillUseCount++;
						PlaySound(SOUND_LABEL_SE_SHIELD_000);

						SetStaging(STAGINGTYPE_SHIELD, -1, g_aPlayer.pos);

						pSkillGage->fSkillGage -= 200.0f;
						//}
					}
				}
			}
		}
	//}
			//===============================================================================================================================================
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

	pVtx[0].pos.x = g_aPlayer.pos.x + sinf(g_aPlayer.rot.z - (D3DX_PI - g_fAnglePlayer)) * g_fLengthPlayer;//�ォ�獶�ɂS�T�x
	pVtx[0].pos.y = g_aPlayer.pos.y + cosf(g_aPlayer.rot.z - (D3DX_PI - g_fAnglePlayer)) * g_fLengthPlayer;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_aPlayer.pos.x + sinf(g_aPlayer.rot.z + (D3DX_PI - g_fAnglePlayer)) * g_fLengthPlayer;//�ォ��E�ɂS�T�x
	pVtx[1].pos.y = g_aPlayer.pos.y + cosf(g_aPlayer.rot.z + (D3DX_PI - g_fAnglePlayer)) * g_fLengthPlayer;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_aPlayer.pos.x + sinf(g_aPlayer.rot.z - g_fAnglePlayer) * g_fLengthPlayer;//�����獶�ɂS�T�x
	pVtx[2].pos.y = g_aPlayer.pos.y + cosf(g_aPlayer.rot.z - g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_aPlayer.pos.x + sinf(g_aPlayer.rot.z + g_fAnglePlayer) * g_fLengthPlayer;//������E�ɂS�T�x
	pVtx[3].pos.y = g_aPlayer.pos.y + cosf(g_aPlayer.rot.z + g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[3].pos.z = 0.0f;




	//pVtx[0].tex = D3DXVECTOR2(g_nAnimation / 5.0f, g_nAnimationCount / 2.0f);
	//pVtx[1].tex = D3DXVECTOR2((g_nAnimation + 1) / 5.0f, g_nAnimationCount / 2.0f);// g_nAnimation�u�{�P�v�ŕ������B
	//pVtx[2].tex = D3DXVECTOR2(g_nAnimation / 5.0f, (g_nAnimationCount + 1) / 2.0f);
	//pVtx[3].tex = D3DXVECTOR2((g_nAnimation + 1) / 5.0f, (g_nAnimationCount + 1) / 2.0f);

	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);


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
		
		
	}if (pTutorial->bTutorialUse == true)
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
	}

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
			pDevice->SetTexture(0, g_pTexturePolygon);

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
		g_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);

		Health* pHealth = GetHealth();//�v���C���[�̗̑͂̏����擾����B

		pHealth->fHealth += nDamage;

		g_aPlayer.nHitCount++;
		if (pHealth->fHealth >= 3)
		{//�v���C���[�̗̑͂��Ȃ��Ȃ�����...
			SetExplosion(g_aPlayer.pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.0f), EXPLOSIONTYPE_NORMAL);
			g_aPlayer.bDisp = false;
			g_aPlayer.state = PLAYERSTATE_DEATH;
			g_aPlayer.nCounterState = 60;
		}
		else
		{
			g_aPlayer.state = PLAYERSTATE_DAMAGE;
			
			g_aPlayer.nCounterState = 100;
		}
		//���_�o�b�t�@���A�����b�N����
		g_pVtxBuffPolygon->Unlock();
	}
	//�v���C���[�̎擾
	Player* GetPlayer(void)
	{
		return &g_aPlayer;
		
	}
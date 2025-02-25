//========================================================
//
//�P�O���P�Q���F�v���C���[�ɃR�}���h�Z��������[Attack.cpp]
//Author:ShinaTaiyo
//
//=========================================================
#include "main.h"
#include "Attack.h"
#include "input.h"
#include "explosion.h"
#include "enemy.h"
#include "player.h"
#include "health.h"
#include <time.h>
#include "effect.h"
#include "sound.h"
#include "particle.h"
#include "damage.h"
#include "block.h"
#include "bullet.h"


#define NUM_ATTACK (ATTACKTYPE_MAX)//�U���̎�ސ�
#define BIGAttack_LV1 (40.0f)
#define BIGAttack_LV2 (150.0f)//�傫���U��LV24
#define AttackSCALE_NOVA (100.0f)

#define AttackSIZE_SCATTERING (30.0f)


//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureAttack[NUM_ATTACK] = {};//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffAttack = NULL;//�U���̏��

//====================================
//�΂߂̓����蔻��Ɏg������
//====================================
D3DXVECTOR2 relative_position[MAX_ATTACK];
D3DXVECTOR2 transform_pos[MAX_ATTACK];


Attack g_aAttack[MAX_ATTACK];
//float g_fXMove;//X���W�̈ړ�
//float g_fYMove;//Y���W�̈ړ�

float g_fCurveAttack;
float g_fEnemy9Attack;
int g_nRandumAttack2_B;
float g_fRandumResultAttack2_B;//�����_���ōU���������������܂�B

int g_nDelayAttack;

//int g_nNumEnemy[MAX_ENEMY];//�G�l�~�[�̐��𐔂���ϐ�
D3DXVECTOR3 g_posAttack;//�ʒu
D3DXVECTOR3 g_moveAttack;//�ړ���
//D3DXVECTOR3 g_rotPl;//����

float g_fRandumScatteringAttack_Rot;//�΂�܂��g�U�U���̏���
float g_fRandumScatternigAttack_Speed;//�΂�܂��g�U�U���̃X�s�[�h


float g_fXVaimAttack[MAX_ATTACK];
float g_fYVaimAttack[MAX_ATTACK];
float g_VaimAttack[MAX_ATTACK];

//int g_nHitCount;//�v���C���[����U�������񐔂��J�E���g����ϐ�
//
//int g_nAnimatioinCounter_Slash[MAX_ATTACK];//�A�j���[�V���������̃e�N�X�`���ɂ���J�E���^�[
//float g_fAnimationPattern_Slash[MAX_ATTACK];//�A�j���[�V�����̃e�N�X�`�����Ǘ�����ϐ�

//�U���̏���������
void InitAttack(void)
{

	//srand((unsigned int)time(NULL));//�Q�[�����J�n�������Ԗ��ɗ����̎��ݒ�

	g_moveAttack = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�ړ��ʂ�����������

	g_fCurveAttack = 0.0f;
	g_fEnemy9Attack = 0.0f;

	g_nDelayAttack = 0;

	g_fRandumScatteringAttack_Rot = 0.0f;
	g_fRandumScatternigAttack_Speed = 0.0f;

	/*g_nHitCount = 0;*/

	//g_nDelayAttack;

	LPDIRECT3DDEVICE9 pDevice;
	int	nCntAttack;


	int nCntEnemy;
	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ��݁i�U���̃e�N�X�`����ǂݍ��ށj
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\AttackMotion_Punch.png",
		&g_pTextureAttack[ATTACKTYPE_PUNCH]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\AttackMotion_Slash_000.png",
		&g_pTextureAttack[ATTACKTYPE_SLASH_E]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\AttackMotion_IceExplosion.png",
		&g_pTextureAttack[ATTACKTYPE_ICE_EXPLOSION]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\AttackMotion_Thunder.png",
		&g_pTextureAttack[ATTACKTYPE_THUNDER_E4]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\AttackMotion_Bomu.png",
		&g_pTextureAttack[ATTACKTYPE_BOMU]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\AttackMotion_Needle.png",
		&g_pTextureAttack[ATTACKTYPE_NEEDLE]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\AttackMotion_Claw.png",
		&g_pTextureAttack[ATTACKTYPE_CLAW]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\AttackMotion_VacuumBlade.png",
		&g_pTextureAttack[ATTACKTYPE_VACUUMBLADE]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\AttackMotion_FallingBlock.png",
		&g_pTextureAttack[ATTACKTYPE_FALLINGBLOCK]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\AttackMotion_Beam.png",
		&g_pTextureAttack[ATTACKTYPE_BEAM]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\AttackMotion_IcicleClaw.png",
		&g_pTextureAttack[ATTACKTYPE_ICICLECLAW]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\AttackMotion_Absolute_blizzard.png",
		&g_pTextureAttack[ATTACKTYPE_ABSOLUTE_BLIZZARD]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\AttackMotion_Fire.png",
		&g_pTextureAttack[ATTACKTYPE_FIRE]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\AttackMotion_ShakingSword.png",
		&g_pTextureAttack[ATTACKTYPE_SHAKINGSWORD]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\AttackMotion_FlashBlade.png",
		&g_pTextureAttack[ATTACKTYPE_FLASHBLADE]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\AttackMotion_Thunder_002.png",
		&g_pTextureAttack[ATTACKTYPE_THUNDER_2]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\FinalBeam.png",
		&g_pTextureAttack[ATTACKTYPE_FINALBEAM]);
	//g_posAttack = D3DXVECTOR3(600.0f, 300.0f, 0.0f);//�ʒu������������


	//for (nCntAttack = 0; nCntAttack < MAX_CIRCLEAttack; nCntAttack++)
	//{//�~�`�e�U�U���̏�����
	//	g_fCircleAttack_B[nCntAttack] = 0;
	//}

	//�U�����̏�����
	for (nCntAttack = 0; nCntAttack < MAX_ATTACK; nCntAttack++)
	{
		g_aAttack[nCntAttack].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�ʒu
		g_aAttack[nCntAttack].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�O�̈ʒu
		g_aAttack[nCntAttack].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�U���̓���
		g_aAttack[nCntAttack].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//����
		g_aAttack[nCntAttack].fLength = 0.0f;//�Ίp���̒���
		g_aAttack[nCntAttack].fAngle = 0.0f;//�Ίp���̊p�x
		g_aAttack[nCntAttack].fWidth = 0.0f;//��
		g_aAttack[nCntAttack].fHeight = 0.0f;//����
		g_aAttack[nCntAttack].nLife = 0;//�U������
		g_aAttack[nCntAttack].bUse = false;//�g�p���Ă��Ȃ���Ԃɂ���
		g_aAttack[nCntAttack].type = ATTACKTYPE_PUNCH;//�U���̎��
		g_aAttack[nCntAttack].moveType = MOVETYPE_NORMAL;//�U���̓������̎��
		g_aAttack[nCntAttack].nTime = 0;//�U�����n�܂��Ă���̎��Ԃ��v������B
		g_aAttack[nCntAttack].nCntAnim = 0;//�A�j���[�V�����J�E���^�[
		g_aAttack[nCntAttack].nPatternAnim = 0;//�A�j���[�V�����p�^�[��
		g_aAttack[nCntAttack].nPatternAnimY = 0;//�A�j���[�V�������Q�i�ȏ�̎��ɒe���ڂ�����
		g_aAttack[nCntAttack].nHitCount = 0;//���̍U���Ńq�b�g����񐔂̐���
		g_aAttack[nCntAttack].nNumEnemy = 0;
		g_aAttack[nCntAttack].bIsLanding = false;//�n�ʂɂ��Ă���t���O
		g_aAttack[nCntAttack].bIsWalling = false;//�ǂɓ������Ă��邩
		g_aAttack[nCntAttack].fGravity = 0.0f;//�d�͕␳�l
		g_aAttack[nCntAttack].nColorChange = 0;//�F��ς���
		g_aAttack[nCntAttack].nCntUseTime = 0;//�U�����g���Ă��鎞�Ԃ��J�E���g����
		g_aAttack[nCntAttack].fXVaim = 0.0f;
		g_aAttack[nCntAttack].fYVaim = 0.0f;
		g_aAttack[nCntAttack].fVaim = 0.0f;
		g_aAttack[nCntAttack].fEffectiveScale = 0.0f;//�U���̌��ʔ͈͂����߂�
		g_aAttack[nCntAttack].fRotSpeed = 0.0f;//��]���x
	}

	//for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	//{
	//	g_nNumEnemy[nCntEnemy] = -1;//������
	//}
	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_ATTACK,//�S�͕K�v�Ȓ��_��
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffAttack,
		NULL);

	VERTEX_2D* pVtx;

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffAttack->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntAttack = 0; nCntAttack < MAX_ATTACK; nCntAttack++)
	{
		//�Ίp���̒������Z�o����
        g_aAttack[nCntAttack].fLength = sqrtf(g_aAttack[nCntAttack].fWidth * g_aAttack[nCntAttack].fWidth + (g_aAttack[nCntAttack].fHeight * 2.0f) * (g_aAttack[nCntAttack].fHeight * 2.0f)) / 2;
        
        //�Ίp���̊p�x���Z�o����
        g_aAttack[nCntAttack].fAngle = atan2f(g_aAttack[nCntAttack].fWidth, (g_aAttack[nCntAttack].fHeight * 2.0f));
        
        //���_���W�̐ݒ�
        pVtx[0].pos.y = g_aAttack[nCntAttack].pos.y + cosf(g_aAttack[nCntAttack].rot.z - (D3DX_PI - g_aAttack[nCntAttack].fAngle)) * g_aAttack[nCntAttack].fLength;
        pVtx[0].pos.x = g_aAttack[nCntAttack].pos.x + sinf(g_aAttack[nCntAttack].rot.z - (D3DX_PI - g_aAttack[nCntAttack].fAngle)) * g_aAttack[nCntAttack].fLength;
        pVtx[0].pos.z = 0.0f;
        
        pVtx[1].pos.x = g_aAttack[nCntAttack].pos.x + sinf(g_aAttack[nCntAttack].rot.z + (D3DX_PI - g_aAttack[nCntAttack].fAngle)) * g_aAttack[nCntAttack].fLength;
        pVtx[1].pos.y = g_aAttack[nCntAttack].pos.y + cosf(g_aAttack[nCntAttack].rot.z + (D3DX_PI - g_aAttack[nCntAttack].fAngle)) * g_aAttack[nCntAttack].fLength;
        pVtx[1].pos.z = 0.0f;
        
        pVtx[2].pos.x = g_aAttack[nCntAttack].pos.x + sinf(g_aAttack[nCntAttack].rot.z + -(D3DX_PI * 0.5f)) * g_aAttack[nCntAttack].fWidth / 2;
        pVtx[2].pos.y = g_aAttack[nCntAttack].pos.y + cosf(g_aAttack[nCntAttack].rot.z + -(D3DX_PI * 0.5f)) * g_aAttack[nCntAttack].fWidth / 2;
        pVtx[2].pos.z = 0.0f;
        
        pVtx[3].pos.x = g_aAttack[nCntAttack].pos.x + sinf(g_aAttack[nCntAttack].rot.z + (D3DX_PI * 0.5f)) * g_aAttack[nCntAttack].fWidth / 2;
        pVtx[3].pos.y = g_aAttack[nCntAttack].pos.y + cosf(g_aAttack[nCntAttack].rot.z + (D3DX_PI * 0.5f)) * g_aAttack[nCntAttack].fWidth / 2;
        pVtx[3].pos.z = 0.0f;
        
		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLOR�́A0.0f�`1.0f�̊Ԃ̐��l�ŐF��ς���B
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLOR�́A0.0f�`1.0f�̊Ԃ̐��l�ŐF��ς���B
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLOR�́A0.0f�`1.0f�̊Ԃ̐��l�ŐF��ς���B
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLOR�́A0.0f�`1.0f�̊Ԃ̐��l�ŐF��ς���B

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;//���_�f�[�^�̃|�C���^���S���i�߂�B
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffAttack->Unlock();
}

//�U���̏I������
void UninitAttack(void)
{
	//�e�N�X�`���̔j��
	int nCnt;
	for (nCnt = 0; nCnt < NUM_ATTACK; nCnt++)
	{
		if (g_pTextureAttack[nCnt] != NULL)
		{
			g_pTextureAttack[nCnt]->Release();
			g_pTextureAttack[nCnt] = NULL;
		}
	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffAttack != NULL)
	{
		g_pVtxBuffAttack->Release();
		g_pVtxBuffAttack = NULL;
	}
}

//�U���̍X�V����
void UpdateAttack(void)
{
	int nCntAttack3;

	//g_nRandumAttack2_B = rand() % 100 + 1;//�~�`�e�U�U���̔��˕����̒����p

	//g_fRandumResultAttack2_B = 0.01f * g_nRandumAttack2_B;//�~�`�g�U�U���̕�����������
	////g_fRandumResultAttack2_B = 0.01f;//�~�`�g�U�U���̕�����������


	//g_fRandumScatteringAttack_Rot = float((rand)() % 629 - 314) / 100.0f;//�U���𔭎˂�����������߂�



	g_nDelayAttack++;//�U�������x�点��J�E���g

	Player* pPlayer = GetPlayer();
	//g_aAttack.fCurveAttackX += 0.01f;
	//g_fRotAttackY += 0.02f;
	//
	//LPDIRECT3DDEVICE9 pDevice;
	int	nCntAttack;
	//g_fEnemy9Attack += 0.0001f;

	//Health* pHealth = GetHealth();

	Enemy* pEnemy = GetEnemy();

	VERTEX_2D* pVtx;


	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffAttack->Lock(0, 0, (void**)&pVtx, 0);



	for (nCntAttack = 0; nCntAttack < MAX_ATTACK; nCntAttack++)
	{
		if (g_aAttack[nCntAttack].bUse == true)
		{//�U�����g�p����Ă���
			int nCntEnemy;
			g_aAttack[nCntAttack].nLife -= 1;//�\�����鎞�ԁi�����j�����炵������


			//=====================================
			//���S�_���菈��
			//=====================================

		//�Ίp���̒������Z�o����
			g_aAttack[nCntAttack].fLength = sqrtf(g_aAttack[nCntAttack].fWidth * g_aAttack[nCntAttack].fWidth + (g_aAttack[nCntAttack].fHeight * 2.0f) * (g_aAttack[nCntAttack].fHeight * 2.0f)) / 2;

			//�Ίp���̊p�x���Z�o����
			g_aAttack[nCntAttack].fAngle = atan2f(g_aAttack[nCntAttack].fWidth, (g_aAttack[nCntAttack].fHeight * 2.0f));

			//���_���W�̐ݒ�
			pVtx[0].pos.y = g_aAttack[nCntAttack].pos.y + cosf(g_aAttack[nCntAttack].rot.z - (D3DX_PI - g_aAttack[nCntAttack].fAngle)) * g_aAttack[nCntAttack].fLength;
			pVtx[0].pos.x = g_aAttack[nCntAttack].pos.x + sinf(g_aAttack[nCntAttack].rot.z - (D3DX_PI - g_aAttack[nCntAttack].fAngle)) * g_aAttack[nCntAttack].fLength;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_aAttack[nCntAttack].pos.x + sinf(g_aAttack[nCntAttack].rot.z + (D3DX_PI - g_aAttack[nCntAttack].fAngle)) * g_aAttack[nCntAttack].fLength;
			pVtx[1].pos.y = g_aAttack[nCntAttack].pos.y + cosf(g_aAttack[nCntAttack].rot.z + (D3DX_PI - g_aAttack[nCntAttack].fAngle)) * g_aAttack[nCntAttack].fLength;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_aAttack[nCntAttack].pos.x + sinf(g_aAttack[nCntAttack].rot.z - (D3DX_PI * 0.5f)) * g_aAttack[nCntAttack].fWidth / 2;
			pVtx[2].pos.y = g_aAttack[nCntAttack].pos.y + cosf(g_aAttack[nCntAttack].rot.z - (D3DX_PI * 0.5f)) * g_aAttack[nCntAttack].fWidth / 2;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_aAttack[nCntAttack].pos.x + sinf(g_aAttack[nCntAttack].rot.z + (D3DX_PI * 0.5f)) * g_aAttack[nCntAttack].fWidth / 2;
			pVtx[3].pos.y = g_aAttack[nCntAttack].pos.y + cosf(g_aAttack[nCntAttack].rot.z + (D3DX_PI * 0.5f)) * g_aAttack[nCntAttack].fWidth / 2;
			pVtx[3].pos.z = 0.0f;


			//=========================================
			//�΂߂̓����蔻����g���ꍇ�̏���
			// ========================================


			// ��`�̒��S�����_�Ƃ������΍��W���쐬����
			relative_position[nCntAttack] = D3DXVECTOR2(pVtx[0].pos.x - g_aAttack[nCntAttack].pos.x, pVtx[0].pos.y - g_aAttack[nCntAttack].pos.y);

			// ���΍��W�ɑ΂��ċ�`�̉�]��ł������t�s����|����
			transform_pos[nCntAttack] = D3DXVECTOR2(
				cosf(g_aAttack[nCntAttack].rot.z) * relative_position[nCntAttack].x + sinf(g_aAttack[nCntAttack].rot.z) * relative_position[nCntAttack].y,
				-sinf(g_aAttack[nCntAttack].rot.z) * relative_position[nCntAttack].x + cosf(g_aAttack[nCntAttack].rot.z) * relative_position[nCntAttack].y
			);



			//===============================================================================================================================================================

			//=================================================
			//�p�ɂԂ�������U��������
			//=================================================
			//if ((g_aAttack[nCntAttack].pos.x >= SCREEN_WIDTH) || (g_aAttack[nCntAttack].pos.x <= 0) ||
			//	(g_aAttack[nCntAttack].pos.y >= SCREEN_HEIGHT) || (g_aAttack[nCntAttack].pos.y <= 0))
			//{
			//	g_aAttack[nCntAttack].bUse = false;//�g�p���Ă��Ȃ���Ԃɂ���B
			//}



			//==============================================================================================================================


			//=============================================================
			//��ȍU������
			//=============================================================
			if (g_aAttack[nCntAttack].type == ATTACKTYPE_PUNCH)
			{//�A�r���e�B�F�m�[�}���A�Z�F�p���`

				Enemy* pEnemy = GetEnemy();

				g_aAttack[nCntAttack].nCntAnim++;//�A�j���[�V�����J�E���^�[��i�߂�

				if (g_aAttack[nCntAttack].nCntAnim % 5 == 0)
				{
					g_aAttack[nCntAttack].nPatternAnim++;

				}
				if (g_aAttack[nCntAttack].nPatternAnim >= 4)
				{
					g_aAttack[nCntAttack].nPatternAnim = 0;
				}

				//�e�N�X�`�����W�̐ݒ�
				if (pPlayer->bDirection == false)
				{//�E�����Ă�����
					pVtx[0].tex = D3DXVECTOR2(0.0f + 0.25f * g_aAttack[nCntAttack].nPatternAnim, 0.0f);
					pVtx[1].tex = D3DXVECTOR2(0.25f + 0.25f * g_aAttack[nCntAttack].nPatternAnim, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(0.0f + 0.25f * g_aAttack[nCntAttack].nPatternAnim, 1.0f);
					pVtx[3].tex = D3DXVECTOR2(0.25f + 0.25f * g_aAttack[nCntAttack].nPatternAnim, 1.0f);

					g_aAttack[nCntAttack].pos.y = pPlayer->pos.y;
					g_aAttack[nCntAttack].pos.x = pPlayer->pos.x + PLAYER_WIDTH + 30.0f;
				}
				else if (pPlayer->bDirection == true)
				{//�������Ă�����
					pVtx[1].tex = D3DXVECTOR2(0.0f + 0.25f * g_aAttack[nCntAttack].nPatternAnim, 0.0f);
					pVtx[0].tex = D3DXVECTOR2(0.25f + 0.25f * g_aAttack[nCntAttack].nPatternAnim, 0.0f);
					pVtx[3].tex = D3DXVECTOR2(0.0f + 0.25f * g_aAttack[nCntAttack].nPatternAnim, 1.0f);
					pVtx[2].tex = D3DXVECTOR2(0.25f + 0.25f * g_aAttack[nCntAttack].nPatternAnim, 1.0f);

					g_aAttack[nCntAttack].pos.y = pPlayer->pos.y;
					g_aAttack[nCntAttack].pos.x = pPlayer->pos.x - PLAYER_WIDTH - 30.0f;
				}


				for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
				{
					if (pEnemy->bUse == true)
					{//�G���g�p����Ă���
						if (g_aAttack[nCntAttack].pos.x >= pEnemy->pos.x - 50.0f
							&& g_aAttack[nCntAttack].pos.x <= pEnemy->pos.x + 50.0f
							&& g_aAttack[nCntAttack].pos.y - g_aAttack[nCntAttack].fHeight / 2 >= pEnemy->pos.y - pEnemy->fHeight - 50.0f
							&& g_aAttack[nCntAttack].pos.y - g_aAttack[nCntAttack].fHeight / 2 <= pEnemy->pos.y - pEnemy->fHeight + 50.0f)
						{//�G�ƒe�����������i�G�̒��S���W�j�i�U���̒��S���W�j
							if (g_aAttack[nCntAttack].nHitCount <= 0)
							{//�q�b�g�������ȉ��Ȃ�
								HitEnemy(nCntEnemy, 15);
							}

							g_aAttack[nCntAttack].nHitCount++;//�q�b�g���̃J�E���g���P���₷
						}
					}

				}
			}
			else if (g_aAttack[nCntAttack].type == ATTACKTYPE_SLASH_E)
			{//�G�l�~�[�Q�̋Z�u�X���b�V���v

				Enemy* pEnemy = GetEnemy();
				Player* pPlayer = GetPlayer();
				g_aAttack[nCntAttack].nCntAnim++;//�A�j���[�V�����J�E���^�[��i�߂�

				g_aAttack[nCntAttack].nCntUseTime++;//�U�����g���Ă��鎞�Ԃ��J�E���g����
				if (g_aAttack[nCntAttack].nCntUseTime == 1)
				{
					PlaySound(SOUND_LABEL_SE_MINISLASH);
				}


				if (g_aAttack[nCntAttack].nCntAnim % 5 == 0)
				{
					g_aAttack[nCntAttack].nPatternAnim++;

				}
				if (g_aAttack[nCntAttack].nPatternAnim >= 5)
				{
					g_aAttack[nCntAttack].nPatternAnim = 0;
				}

				//�e�N�X�`�����W�̐ݒ�



				for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
				{
					if (pEnemy->bUse == true)
					{//�G���g�p����Ă���

						if (nCntEnemy == g_aAttack[nCntAttack].nNumEnemy)
						{
							if (pEnemy->bDirection == false)
							{//�E�����Ă���
								pVtx[0].tex = D3DXVECTOR2(0.0f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 0.0f);
								pVtx[1].tex = D3DXVECTOR2(0.2f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 0.0f);
								pVtx[2].tex = D3DXVECTOR2(0.0f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 1.0f);
								pVtx[3].tex = D3DXVECTOR2(0.2f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 1.0f);

								//�U���̍��W��G�̏����E���ɂ��炷
								g_aAttack[nCntAttack].pos.y = pEnemy->pos.y;
								g_aAttack[nCntAttack].pos.x = pEnemy->pos.x + pEnemy->fWidth + 20.0f;
							}
							else if (pEnemy->bDirection == true)
							{//�������Ă���
								pVtx[1].tex = D3DXVECTOR2(0.0f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 0.0f);
								pVtx[0].tex = D3DXVECTOR2(0.2f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 0.0f);
								pVtx[3].tex = D3DXVECTOR2(0.0f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 1.0f);
								pVtx[2].tex = D3DXVECTOR2(0.2f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 1.0f);

								//�U���̍��W��G�̏��������ɂ��炷
								g_aAttack[nCntAttack].pos.y = pEnemy->pos.y;
								g_aAttack[nCntAttack].pos.x = pEnemy->pos.x - pEnemy->fWidth - 20.0f;
							}
						}

					}

				}


				if (pPlayer->pos.x >= g_aAttack[nCntAttack].pos.x - g_aAttack[nCntAttack].fWidth / 2 &&
					pPlayer->pos.x <= g_aAttack[nCntAttack].pos.x + g_aAttack[nCntAttack].fWidth / 2 &&
					pPlayer->pos.y - pPlayer->fHeight / 2 >= g_aAttack[nCntAttack].pos.y - g_aAttack[nCntAttack].fHeight - pPlayer->fHeight / 2 &&
					pPlayer->pos.y - pPlayer->fHeight / 2 <= g_aAttack[nCntAttack].pos.y + pPlayer->fHeight / 2)
				{//�v���C���[�ɍU������������(�v���C���[�̈ʒu���ǂ��ɂ��邩�Ōv�Z�j
					if (g_nDelayAttack % 5 == 0)
					{
						if (g_aAttack[nCntAttack].nHitCount <= 0)
						{//�q�b�g�������ȉ��Ȃ�
							DamageHealth(8);
						}
					}
					g_aAttack[nCntAttack].nHitCount++;//�q�b�g���̃J�E���g���P���₷
				}
			}
			else if (g_aAttack[nCntAttack].type == ATTACKTYPE_ICE_EXPLOSION)
			{//�G�l�~�[�R�̋Z�u�X�����v

			g_aAttack[nCntAttack].nCntUseTime++;//�U�����g���Ă��鎞�Ԃ��J�E���g����
			if (g_aAttack[nCntAttack].nCntUseTime == 1)
			{
				PlaySound(SOUND_LABEL_SE_ICE_EXPLOSION);
			}

				Enemy* pEnemy = GetEnemy();
				Player* pPlayer = GetPlayer();
				g_aAttack[nCntAttack].nCntAnim++;//�A�j���[�V�����J�E���^�[��i�߂�

				if (g_aAttack[nCntAttack].nCntAnim % 2 == 0)
				{
					g_aAttack[nCntAttack].nPatternAnim++;

				}
				if (g_aAttack[nCntAttack].nPatternAnim >= 8)
				{
					g_aAttack[nCntAttack].nPatternAnim = 0;
					g_aAttack[nCntAttack].nPatternAnimY++;//�A�j���[�V�����̒i�����̒i�ֈړ�
				}
				if (g_aAttack[nCntAttack].nPatternAnim >= 6 && g_aAttack[nCntAttack].nPatternAnimY >= 1)
				{//�A�j���[�V�������ŏ��̈ʒu��
					g_aAttack[nCntAttack].nPatternAnim = 0;
					g_aAttack[nCntAttack].nPatternAnimY = 0;
				}

				//�e�N�X�`�����W�̐ݒ�




				pVtx[0].tex = D3DXVECTOR2(0.0f + 0.125f * g_aAttack[nCntAttack].nPatternAnim, 0.0f + 0.5f * g_aAttack[nCntAttack].nPatternAnimY);
				pVtx[1].tex = D3DXVECTOR2(0.125f + 0.125f * g_aAttack[nCntAttack].nPatternAnim, 0.0f + 0.5f * g_aAttack[nCntAttack].nPatternAnimY);
				pVtx[2].tex = D3DXVECTOR2(0.0f + 0.125f * g_aAttack[nCntAttack].nPatternAnim, 0.5f + 0.5f * g_aAttack[nCntAttack].nPatternAnimY);
				pVtx[3].tex = D3DXVECTOR2(0.125f + 0.125f * g_aAttack[nCntAttack].nPatternAnim, 0.5f + 0.5f * g_aAttack[nCntAttack].nPatternAnimY);




				if (pPlayer->pos.x >= g_aAttack[nCntAttack].pos.x - g_aAttack[nCntAttack].fWidth / 2 &&
					pPlayer->pos.x <= g_aAttack[nCntAttack].pos.x + g_aAttack[nCntAttack].fWidth / 2 &&
					pPlayer->pos.y - pPlayer->fHeight / 2 >= g_aAttack[nCntAttack].pos.y - g_aAttack[nCntAttack].fHeight &&
					pPlayer->pos.y - pPlayer->fHeight / 2 <= g_aAttack[nCntAttack].pos.y)
				{//�v���C���[�ɍU������������(�v���C���[�̒��S���ǂ̈ʒu�ɂ��邩�Ōv�Z�j
					if (g_nDelayAttack % 1 == 0)
					{
						if (g_aAttack[nCntAttack].nHitCount <= 0)
						{//�q�b�g�������ȉ��Ȃ�
							DamageHealth(18);

						}
					}
					g_aAttack[nCntAttack].nHitCount++;//�q�b�g���̃J�E���g���P���₷
				}
			}
			else if (g_aAttack[nCntAttack].type == ATTACKTYPE_THUNDER_E4)
			{//�G�l�~�[�S�̋Z�u�T���_�[�v
			g_aAttack[nCntAttack].nCntUseTime++;//�U�����g���Ă��鎞�Ԃ��J�E���g����
			if (g_aAttack[nCntAttack].nCntUseTime == 1)
			{
				PlaySound(SOUND_LABEL_SE_THUNDER);
			}

				Enemy* pEnemy = GetEnemy();
				Player* pPlayer = GetPlayer();
				g_aAttack[nCntAttack].nCntAnim++;//�A�j���[�V�����J�E���^�[��i�߂�

				if (g_aAttack[nCntAttack].nCntAnim % 5 == 0)
				{
					g_aAttack[nCntAttack].nPatternAnim++;

				}
				if (g_aAttack[nCntAttack].nPatternAnim >= 5)
				{
					g_aAttack[nCntAttack].nPatternAnim = 0;
				}

				//�e�N�X�`�����W�̐ݒ�



				for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
				{
					if (pEnemy->bUse == true)
					{//�G���g�p����Ă���

						if (nCntEnemy == g_aAttack[nCntAttack].nNumEnemy)
						{
							if (pEnemy->bDirection == false)
							{//�E�����Ă���
								pVtx[0].tex = D3DXVECTOR2(0.0f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 0.0f);
								pVtx[1].tex = D3DXVECTOR2(0.2f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 0.0f);
								pVtx[2].tex = D3DXVECTOR2(0.0f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 1.0f);
								pVtx[3].tex = D3DXVECTOR2(0.2f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 1.0f);

								////�U���̍��W�͍U���̉��[
								//g_aAttack[nCntAttack].pos.y = pEnemy->pos.y + g_aAttack[nCntAttack].fHeight / 2;
								//g_aAttack[nCntAttack].pos.x = pEnemy->pos.x;
							}
							else if (pEnemy->bDirection == true)
							{//�������Ă���
								pVtx[1].tex = D3DXVECTOR2(0.0f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 0.0f);
								pVtx[0].tex = D3DXVECTOR2(0.2f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 0.0f);
								pVtx[3].tex = D3DXVECTOR2(0.0f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 1.0f);
								pVtx[2].tex = D3DXVECTOR2(0.2f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 1.0f);

								//�U���̍��W�͍U���̉��[
							/*	g_aAttack[nCntAttack].pos.y = pEnemy->pos.y +
									g_aAttack[nCntAttack].fHeight / 2;
								g_aAttack[nCntAttack].pos.x = pEnemy->pos.x;*/
							}
						}

					}

				}


				if (pPlayer->pos.x >= g_aAttack[nCntAttack].pos.x - g_aAttack[nCntAttack].fWidth / 2 - 10.0f &&
					pPlayer->pos.x <= g_aAttack[nCntAttack].pos.x + g_aAttack[nCntAttack].fWidth / 2 + 10.0f &&
					pPlayer->pos.y - pPlayer->fHeight / 2 >= g_aAttack[nCntAttack].pos.y - g_aAttack[nCntAttack].fHeight &&
					pPlayer->pos.y - pPlayer->fHeight / 2 <= g_aAttack[nCntAttack].pos.y + g_aAttack[nCntAttack].fHeight)
				{//�v���C���[�ɍU������������(�v���C���[�̒��S���ǂ̈ʒu�ɂ��邩�Ōv�Z�j
					if (g_nDelayAttack % 1 == 0)
					{
						if (g_aAttack[nCntAttack].nHitCount <= 0)
						{//�q�b�g�������ȉ��Ȃ�
							DamageHealth(12);

						}
					}
					g_aAttack[nCntAttack].nHitCount++;//�q�b�g���̃J�E���g���P���₷
				}

			}

			else if (g_aAttack[nCntAttack].type == ATTACKTYPE_BOMU)
			{//�G�l�~�[�U�̋Z�u�{���v
				Enemy* pEnemy = GetEnemy();
				Player* pPlayer = GetPlayer();
				g_aAttack[nCntAttack].nCntAnim++;//�A�j���[�V�����J�E���^�[��i�߂�
				g_aAttack[nCntAttack].nColorChange++;//�F��ς�������
				if (g_aAttack[nCntAttack].nCntAnim % 3 == 0)
				{
					g_aAttack[nCntAttack].nPatternAnim++;

				}
				if (g_aAttack[nCntAttack].nPatternAnim >= 5)
				{
					g_aAttack[nCntAttack].nPatternAnim = 0;
				}

				//�e�N�X�`�����W�̐ݒ�

				//���_�J���[�̐ݒ�
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f - (0.01f * g_aAttack[nCntAttack].nColorChange), 1.0f - (0.01f * g_aAttack[nCntAttack].nColorChange), 1.0f);//D3DXCOLOR�́A0.0f�`1.0f�̊Ԃ̐��l�ŐF��ς���B
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f - (0.01f * g_aAttack[nCntAttack].nColorChange), 1.0f - (0.01f * g_aAttack[nCntAttack].nColorChange), 1.0f);//D3DXCOLOR�́A0.0f�`1.0f�̊Ԃ̐��l�ŐF��ς���B
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f - (0.01f * g_aAttack[nCntAttack].nColorChange), 1.0f - (0.01f * g_aAttack[nCntAttack].nColorChange), 1.0f);//D3DXCOLOR�́A0.0f�`1.0f�̊Ԃ̐��l�ŐF��ς���B
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f - (0.01f * g_aAttack[nCntAttack].nColorChange), 1.0f - (0.01f * g_aAttack[nCntAttack].nColorChange), 1.0f);//D3DXCOLOR�́A0.0f�`1.0f�̊Ԃ̐��l�ŐF��ς���B

				pVtx[0].tex = D3DXVECTOR2(0.0f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(0.2f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(0.2f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 1.0f);


				if (g_aAttack[nCntAttack].nLife <= 0)
				{//���C�t���O�ȉ��ɂȂ�����

					SetExplosion(D3DXVECTOR3(g_aAttack[nCntAttack].pos.x, g_aAttack[nCntAttack].pos.y - g_aAttack[nCntAttack].fHeight / 2.0f, 0.0f),
						D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
						EXPLOSIONTYPE_E6, 100.0f, 100.0f);

				}


			}
			else if (g_aAttack[nCntAttack].type == ATTACKTYPE_NEEDLE)
			{//�G�l�~�[7�̋Z�u�Ƃ��U���v


			g_aAttack[nCntAttack].nCntUseTime++;//�U�����g���Ă��鎞�Ԃ��J�E���g����
			if (g_aAttack[nCntAttack].nCntUseTime == 1)
			{
				PlaySound(SOUND_LABEL_SE_STAB);
			}

				Enemy* pEnemy = GetEnemy();
				Player* pPlayer = GetPlayer();
				g_aAttack[nCntAttack].nCntAnim++;//�A�j���[�V�����J�E���^�[��i�߂�

				if (g_aAttack[nCntAttack].nCntAnim % 4 == 0)
				{
					g_aAttack[nCntAttack].nPatternAnim++;

				}
				if (g_aAttack[nCntAttack].nPatternAnim >= 5)
				{
					g_aAttack[nCntAttack].nPatternAnim = 0;
					g_aAttack[nCntAttack].nPatternAnimY++;//�A�j���[�V�����̒i�����̒i�ֈړ�


					if (g_aAttack[nCntAttack].nPatternAnimY >= 2)
					{//�A�j���[�V�������ŏ��̈ʒu��
						g_aAttack[nCntAttack].nPatternAnim = 0;
						g_aAttack[nCntAttack].nPatternAnimY = 0;
						g_aAttack[nCntAttack].bUse = false;//�g�p���Ă��Ȃ���Ԃɂ���
					}
				}




				//�e�N�X�`�����W�̐ݒ�



				for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
				{
					if (pEnemy->bUse == true)
					{//�G���g�p����Ă���

						if (nCntEnemy == g_aAttack[nCntAttack].nNumEnemy)
						{
							if (pEnemy->bDirection == false)
							{//�E�����Ă���
								pVtx[0].tex = D3DXVECTOR2(0.0f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 0.0f + 0.5f * g_aAttack[nCntAttack].nPatternAnimY);
								pVtx[1].tex = D3DXVECTOR2(0.2f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 0.0f + 0.5f * g_aAttack[nCntAttack].nPatternAnimY);
								pVtx[2].tex = D3DXVECTOR2(0.0f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 0.5f + 0.5f * g_aAttack[nCntAttack].nPatternAnimY);
								pVtx[3].tex = D3DXVECTOR2(0.2f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 0.5f + 0.5f * g_aAttack[nCntAttack].nPatternAnimY);
							}
							else if (pEnemy->bDirection == true)
							{//�������Ă���
								pVtx[1].tex = D3DXVECTOR2(0.0f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 0.0f + 0.5f * g_aAttack[nCntAttack].nPatternAnimY);
								pVtx[0].tex = D3DXVECTOR2(0.2f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 0.0f + 0.5f * g_aAttack[nCntAttack].nPatternAnimY);
								pVtx[3].tex = D3DXVECTOR2(0.0f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 0.5f + 0.5f * g_aAttack[nCntAttack].nPatternAnimY);
								pVtx[2].tex = D3DXVECTOR2(0.2f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 0.5f + 0.5f * g_aAttack[nCntAttack].nPatternAnimY);
							}


						}

					}

				}


				if (pPlayer->pos.x >= g_aAttack[nCntAttack].pos.x - g_aAttack[nCntAttack].fWidth / 2 + 20.0f &&
					pPlayer->pos.x <= g_aAttack[nCntAttack].pos.x + g_aAttack[nCntAttack].fWidth / 2 - 20.0f &&
					pPlayer->pos.y - pPlayer->fHeight / 2 >= g_aAttack[nCntAttack].pos.y - g_aAttack[nCntAttack].fHeight + 20.0f &&
					pPlayer->pos.y - pPlayer->fHeight / 2 <= g_aAttack[nCntAttack].pos.y - 20.0f)
				{//�v���C���[�ɍU������������(�v���C���[�̒��S���ǂ̈ʒu�ɂ��邩�Ōv�Z�j
					if (g_nDelayAttack % 1 == 0)
					{
						if (g_aAttack[nCntAttack].nHitCount <= 0)
						{//�q�b�g�������ȉ��Ȃ�
							DamageHealth(10);

						}
					}
					g_aAttack[nCntAttack].nHitCount++;//�q�b�g���̃J�E���g���P���₷
				}


			}
			else if (g_aAttack[nCntAttack].type == ATTACKTYPE_VACUUMBLADE)
			{//�G�l�~�[8�̋Z�u�^��n�v

			g_aAttack[nCntAttack].nCntUseTime++;//�U�����g���Ă��鎞�Ԃ��J�E���g����
			if (g_aAttack[nCntAttack].nCntUseTime == 1)
			{
				PlaySound(SOUND_LABEL_SE_FLASHBLADE);
			}

				g_aAttack[nCntAttack].nCntUseTime++;//�U�����g���Ă��鎞�Ԃ��J�E���g����
				Enemy* pEnemy = GetEnemy();
				Player* pPlayer = GetPlayer();
				g_aAttack[nCntAttack].nCntAnim++;//�A�j���[�V�����J�E���^�[��i�߂�

				if (g_aAttack[nCntAttack].nCntAnim % 5 == 0)
				{
					g_aAttack[nCntAttack].nPatternAnim++;

				}
				if (g_aAttack[nCntAttack].nPatternAnim >= 5)
				{
					g_aAttack[nCntAttack].nPatternAnim = 0;
				}





				//�e�N�X�`�����W�̐ݒ�
				pVtx[0].tex = D3DXVECTOR2(0.0f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(0.2f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(0.2f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 1.0f);



				for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
				{
					if (pEnemy->bUse == true)
					{//�G���g�p����Ă���

						if (nCntEnemy == g_aAttack[nCntAttack].nNumEnemy)
						{
							if (pEnemy->bDirection == false && g_aAttack[nCntAttack].nCntUseTime <= 4)
							{//�E�����Ă���

								pEnemy->move.x = 30.0f;
							}
							else if (pEnemy->bDirection == true && g_aAttack[nCntAttack].nCntUseTime <= 4)
							{//�������Ă���

								pEnemy->move.x = -30.0f;
							}



						}

					}

				}


				if (pPlayer->pos.x >= g_aAttack[nCntAttack].pos.x - g_aAttack[nCntAttack].fWidth / 2 - 10.0f &&
					pPlayer->pos.x <= g_aAttack[nCntAttack].pos.x + g_aAttack[nCntAttack].fWidth / 2 + 10.0f &&
					pPlayer->pos.y - pPlayer->fHeight / 2 >= g_aAttack[nCntAttack].pos.y - g_aAttack[nCntAttack].fHeight + 10.0f &&
					pPlayer->pos.y - pPlayer->fHeight / 2 <= g_aAttack[nCntAttack].pos.y - 10.0f)
				{//�v���C���[�ɍU������������(�v���C���[�̒��S���ǂ̈ʒu�ɂ��邩�Ōv�Z�j
					if (g_nDelayAttack % 1 == 0)
					{
						if (g_aAttack[nCntAttack].nHitCount <= 0)
						{//�q�b�g�������ȉ��Ȃ�
							DamageHealth(25);

						}
					}
					g_aAttack[nCntAttack].nHitCount++;//�q�b�g���̃J�E���g���P���₷
				}
			}
			else if (g_aAttack[nCntAttack].type == ATTACKTYPE_CLAW)
			{//�G�l�~�[8�̋Z�u�܍U���v
				int nCntSaveEnemy = 0;

				g_aAttack[nCntAttack].nCntUseTime++;//�U�����g���Ă��鎞�Ԃ��J�E���g����
				if (g_aAttack[nCntAttack].nCntUseTime == 1)
				{
					PlaySound(SOUND_LABEL_SE_CLAW);
				}


				Enemy* pEnemy = GetEnemy();
				Player* pPlayer = GetPlayer();
				g_aAttack[nCntAttack].nCntAnim++;//�A�j���[�V�����J�E���^�[��i�߂�

				if (g_aAttack[nCntAttack].nCntAnim % 3 == 0)
				{
					g_aAttack[nCntAttack].nPatternAnim++;

				}
				if (g_aAttack[nCntAttack].nPatternAnim >= 5)
				{
					g_aAttack[nCntAttack].nPatternAnim = 0;
				}



				//�e�N�X�`�����W�̐ݒ�



				for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
				{
					if (pEnemy->bUse == true)
					{//�G���g�p����Ă���

						if (nCntEnemy == g_aAttack[nCntAttack].nNumEnemy)
						{
							nCntSaveEnemy = nCntEnemy;
							if (pEnemy->bDirection == false)
							{//�E�����Ă���
								pVtx[0].tex = D3DXVECTOR2(0.0f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 0.0f);
								pVtx[1].tex = D3DXVECTOR2(0.2f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 0.0f);
								pVtx[2].tex = D3DXVECTOR2(0.0f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 1.0f);
								pVtx[3].tex = D3DXVECTOR2(0.2f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 1.0f);

								//�U���̍��W�͓G�̉��[�̉E��
								g_aAttack[nCntAttack].pos.y = pEnemy->pos.y;
								g_aAttack[nCntAttack].pos.x = pEnemy->pos.x + pEnemy->fWidth + 20.0f;

								pEnemy->move.x = 3.0f;
							}
							else if (pEnemy->bDirection == true)
							{//�������Ă���
								pVtx[1].tex = D3DXVECTOR2(0.0f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 0.0f);
								pVtx[0].tex = D3DXVECTOR2(0.2f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 0.0f);
								pVtx[3].tex = D3DXVECTOR2(0.0f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 1.0f);
								pVtx[2].tex = D3DXVECTOR2(0.2f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 1.0f);
								//�U���̍��W�͓G�̉��[�̍���
								g_aAttack[nCntAttack].pos.y = pEnemy->pos.y;
								g_aAttack[nCntAttack].pos.x = pEnemy->pos.x - pEnemy->fWidth - 20.0f;

								pEnemy->move.x = -3.0f;
							}

							if (g_aAttack[nCntAttack].nLife == 1)
							{
								g_aAttack[nCntAttack].bUse == false;
								g_fXVaimAttack[nCntAttack] = pPlayer->pos.x - pPlayer->pos.x;//X���W�ւ̃x�N�g��
								g_fYVaimAttack[nCntAttack] = pPlayer->pos.y - pPlayer->pos.y;//Y���W�ւ̃x�N�g��
								g_VaimAttack[nCntAttack] = atan2(g_fXVaimAttack[nCntAttack], g_fYVaimAttack[nCntAttack]);//�e���������̌���

								float fXVaim;
								float fYVaim;
								float fVaim;

								fXVaim = pPlayer->pos.x - pEnemy->pos.x;//X���W�ւ̃x�N�g��
								fYVaim = pPlayer->pos.y - pEnemy->pos.y;//Y���W�ւ̃x�N�g��
								fVaim = atan2(fXVaim, fYVaim);//�U�����������̌���




								SetAttack(D3DXVECTOR3(pEnemy->pos.x, pEnemy->pos.y - pEnemy->fHeight / 2, 0.0f),
									D3DXVECTOR3(sinf(fVaim) * 8.0f, cosf(fVaim) * 8.0f, 0.0f),
									ATTACKTYPE_VACUUMBLADE, -1, nCntSaveEnemy, -1, MOVETYPE_NORMAL,
									120.0f, 120.0f, 300, 0.0f,-1);

							}
						}

					}

				}


				if (pPlayer->pos.x >= g_aAttack[nCntAttack].pos.x - g_aAttack[nCntAttack].fWidth / 2 - 10.0f &&
					pPlayer->pos.x <= g_aAttack[nCntAttack].pos.x + g_aAttack[nCntAttack].fWidth / 2 + 10.0f &&
					pPlayer->pos.y - pPlayer->fHeight / 2 >= g_aAttack[nCntAttack].pos.y - g_aAttack[nCntAttack].fHeight + 10.0f &&
					pPlayer->pos.y - pPlayer->fHeight / 2 <= g_aAttack[nCntAttack].pos.y - 10.0f)
				{//�v���C���[�ɍU������������(�v���C���[�̒��S���ǂ̈ʒu�ɂ��邩�Ōv�Z�j
					if (g_nDelayAttack % 1 == 0)
					{
						if (g_aAttack[nCntAttack].nHitCount <= 0)
						{//�q�b�g�������ȉ��Ȃ�
							DamageHealth(8);

						}
					}
					g_aAttack[nCntAttack].nHitCount++;//�q�b�g���̃J�E���g���P���₷
				}

				if (g_aAttack[nCntAttack].nLife < 1)
				{
					g_aAttack[nCntAttack].bUse = false;
				}
			}
			else if (g_aAttack[nCntAttack].type == ATTACKTYPE_FALLINGBLOCK)
			{//�G�l�~�[9�̋Z�u�u���b�N���Ƃ��U���v
				int nCntSaveEnemy = 0;
				g_aAttack[nCntAttack].nCntUseTime++;
				if (g_aAttack[nCntAttack].nCntUseTime == 1)
				{
					PlaySound(SOUND_LABEL_SE_FALLINGBLOCK);
				}

				Enemy* pEnemy = GetEnemy();
				Player* pPlayer = GetPlayer();
				g_aAttack[nCntAttack].nCntAnim++;//�A�j���[�V�����J�E���^�[��i�߂�

				if (g_aAttack[nCntAttack].nCntAnim % 3 == 0)
				{
					g_aAttack[nCntAttack].nPatternAnim++;

				}
				if (g_aAttack[nCntAttack].nPatternAnim >= 5)
				{
					g_aAttack[nCntAttack].nPatternAnim = 0;
				}



				//�e�N�X�`�����W�̐ݒ�
				pVtx[0].tex = D3DXVECTOR2(0.0f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(0.2f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(0.2f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 1.0f);




				if (pPlayer->pos.x >= g_aAttack[nCntAttack].pos.x - g_aAttack[nCntAttack].fWidth / 2 &&
					pPlayer->pos.x <= g_aAttack[nCntAttack].pos.x + g_aAttack[nCntAttack].fWidth / 2 &&
					pPlayer->pos.y - pPlayer->fHeight / 2 >= g_aAttack[nCntAttack].pos.y - g_aAttack[nCntAttack].fHeight &&
					pPlayer->pos.y - pPlayer->fHeight / 2 <= g_aAttack[nCntAttack].pos.y)
				{//�v���C���[�ɍU������������(�v���C���[�̒��S���ǂ̈ʒu�ɂ��邩�Ōv�Z�j
					if (g_nDelayAttack % 1 == 0)
					{
						if (g_aAttack[nCntAttack].nHitCount <= 0)
						{//�q�b�g�������ȉ��Ȃ�
							DamageHealth(15);

						}
					}
					g_aAttack[nCntAttack].nHitCount++;//�q�b�g���̃J�E���g���P���₷
				}
			}
			else if (g_aAttack[nCntAttack].type == ATTACKTYPE_BEAM ||
			g_aAttack[nCntAttack].type == ATTACKTYPE_FLASHBLADE ||
			g_aAttack[nCntAttack].type == ATTACKTYPE_THUNDER_2 ||
			g_aAttack[nCntAttack].type == ATTACKTYPE_FINALBEAM)
			{//�G�l�~�[�P�O�̋Z�u�r�[���U���v
			g_aAttack[nCntAttack].nCntUseTime++;//�U�����g���Ă��鎞�Ԃ��J�E���g����
				Enemy* pEnemy = GetEnemy();
				Player* pPlayer = GetPlayer();
				float fData = 0.0f;
				if (g_aAttack[nCntAttack].type == ATTACKTYPE_BEAM)
				{
					fData = 0.04f;
					if (g_aAttack[nCntAttack].nCntUseTime == 1)
					{
						PlaySound(SOUND_LABEL_SE_BEAM);
					}
				}
				else if (g_aAttack[nCntAttack].type == ATTACKTYPE_FLASHBLADE)
				{
					fData = 0.1f;
					if (g_aAttack[nCntAttack].nCntUseTime == 1)
					{
						PlaySound(SOUND_LABEL_SE_SLASH_000);
					}
				}
				else if (g_aAttack[nCntAttack].type == ATTACKTYPE_THUNDER_2)
				{
					if (g_aAttack[nCntAttack].nCntUseTime == 1)
					{
						PlaySound(SOUND_LABEL_SE_THUNDER2);
					}
					fData = 0.07f;
				}
				else if (g_aAttack[nCntAttack].type == ATTACKTYPE_FINALBEAM)
				{
					fData = 0.05f;
					if (g_aAttack[nCntAttack].nCntUseTime == 1)
					{
						PlaySound(SOUND_LABEL_SE_BEAM);
					}
				}

				g_aAttack[nCntAttack].nCntAnim++;//�A�j���[�V�����J�E���^�[��i�߂�

				if (g_aAttack[nCntAttack].nCntAnim % 1 == 0)
				{
					g_aAttack[nCntAttack].nPatternAnim++;

				}
				if (g_aAttack[nCntAttack].nPatternAnim >= 5)
				{
					g_aAttack[nCntAttack].nPatternAnim = 0;
					g_aAttack[nCntAttack].nPatternAnimY++;//�A�j���[�V�����̒i�����̒i�ֈړ�


					if (g_aAttack[nCntAttack].nPatternAnimY >= 2)
					{//�A�j���[�V�������ŏ��̈ʒu��
						g_aAttack[nCntAttack].nPatternAnim = 0;
						g_aAttack[nCntAttack].nPatternAnimY = 0;
						//g_aAttack[nCntAttack].bUse = false;//�g�p���Ă��Ȃ���Ԃɂ���
					}
				}

				//�e�N�X�`�����W�̐ݒ�
				pVtx[0].tex = D3DXVECTOR2(0.0f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 0.0f + 0.5f * g_aAttack[nCntAttack].nPatternAnimY);
				pVtx[1].tex = D3DXVECTOR2(0.2f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 0.0f + 0.5f * g_aAttack[nCntAttack].nPatternAnimY);
				pVtx[2].tex = D3DXVECTOR2(0.0f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 0.5f + 0.5f * g_aAttack[nCntAttack].nPatternAnimY);
				pVtx[3].tex = D3DXVECTOR2(0.2f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 0.5f + 0.5f * g_aAttack[nCntAttack].nPatternAnimY);



				g_aAttack[nCntAttack].rot.z += g_aAttack[nCntAttack].fRotSpeed;


				if (g_aAttack[nCntAttack].rot.z > D3DX_PI)
				{
					g_aAttack[nCntAttack].rot.z -= D3DX_PI * 2.0f;
				}
				else if (g_aAttack[nCntAttack].rot.z < -D3DX_PI)
				{
					g_aAttack[nCntAttack].rot.z += D3DX_PI * 2.0f;
				}
				//========================================
				//�U�������ւ̃x�N�g��
				//========================================

			//============
			//�O�̏���
			//============
			/*	if (pPlayer->pos.x < g_aAttack[nCntAttack].pos.x + g_aAttack[nCntAttack].fWidth / 2 &&
					pPlayer->pos.x > g_aAttack[nCntAttack].pos.x - g_aAttack[nCntAttack].fWidth / 2 &&
					pPlayer->pos.y - pPlayer->fHeight / 2 >= g_aAttack[nCntAttack].pos.y - g_aAttack[nCntAttack].fHeight &&
					pPlayer->pos.y - pPlayer->fHeight / 2 <= g_aAttack[nCntAttack].pos.y)*/
			//========================================================================================
				//========================================
				//�U������v���C���[�ւ̃x�N�g��
				//========================================
	            g_aAttack[nCntAttack].fXVaim = pPlayer->pos.x - g_aAttack[nCntAttack].pos.x;//X���W�ւ̃x�N�g��
				g_aAttack[nCntAttack].fYVaim = pPlayer->pos.y - g_aAttack[nCntAttack].pos.y;//Y���W�ւ̃x�N�g��
				g_aAttack[nCntAttack].fVaim = atan2f(g_aAttack[nCntAttack].fXVaim, g_aAttack[nCntAttack].fYVaim);//�v���C���[�ւ̊p�xg_aAttack[nCntAttack].fYVaim
				//=========================================================================================================================

				if (g_aAttack[nCntAttack].rot.z >= 0.0f && g_aAttack[nCntAttack].rot.z <= D3DX_PI)
				{
					if (g_aAttack[nCntAttack].fVaim > g_aAttack[nCntAttack].rot.z - 3.14f - fData &&
						g_aAttack[nCntAttack].fVaim < g_aAttack[nCntAttack].rot.z - 3.14f + fData)
					{//�v���C���[�ɍU������������(�v���C���[�̒� transform_pos[nCntAttack].x�S���ǂ̈ʒu�ɂ��邩�Ōv�Z�j

						if (g_nDelayAttack % 1 == 0)
						{
							if (g_aAttack[nCntAttack].type != ATTACKTYPE_FINALBEAM)
							{
								if (g_aAttack[nCntAttack].nHitCount <= 0)
								{//�q�b�g�������ȉ��Ȃ�
									DamageHealth(30);

								}
							}
							else
							{
								if (g_aAttack[nCntAttack].nHitCount <= 0)
								{//�q�b�g�������ȉ��Ȃ�
									DamageHealth(10);

								}
							}
						}
						if (g_aAttack[nCntAttack].type != ATTACKTYPE_FINALBEAM)
						{
							g_aAttack[nCntAttack].nHitCount++;//�q�b�g���̃J�E���g���P���₷
						}
					}
				}
				else if (g_aAttack[nCntAttack].rot.z >= -D3DX_PI && g_aAttack[nCntAttack].rot.z <= 0.0f)
				{
					if (g_aAttack[nCntAttack].fVaim > g_aAttack[nCntAttack].rot.z + 3.14f - fData &&
						g_aAttack[nCntAttack].fVaim < g_aAttack[nCntAttack].rot.z + 3.14f + fData)
					{//�v���C���[�ɍU������������(�v���C���[�̒� transform_pos[nCntAttack].x�S���ǂ̈ʒu�ɂ��邩�Ōv�Z�j

						if (g_nDelayAttack % 1 == 0)
						{
							if (g_aAttack[nCntAttack].nHitCount <= 0)
							{//�q�b�g�������ȉ��Ȃ�
								DamageHealth(30);

							}
						}
						if (g_aAttack[nCntAttack].type != ATTACKTYPE_FINALBEAM)
						{
							g_aAttack[nCntAttack].nHitCount++;//�q�b�g���̃J�E���g���P���₷
						}
					}
				}
							
			}
			else if (g_aAttack[nCntAttack].type == ATTACKTYPE_SHAKINGSWORD)
			{//�{�X�R�̋Z�u���U���v�A

			g_aAttack[nCntAttack].nCntUseTime++;
			if (g_aAttack[nCntAttack].nCntUseTime == 1)
			{
				PlaySound(SOUND_LABEL_SE_SHAKINGSWORD);
			}


				Enemy* pEnemy = GetEnemy();
				Player* pPlayer = GetPlayer();
				g_aAttack[nCntAttack].nCntAnim++;//�A�j���[�V�����J�E���^�[��i�߂�

				if (g_aAttack[nCntAttack].nCntAnim % 3 == 0)
				{
					g_aAttack[nCntAttack].nPatternAnim++;

				}
				if (g_aAttack[nCntAttack].nPatternAnim >= 5)
				{
					g_aAttack[nCntAttack].nPatternAnim = 0;
				}

				//�e�N�X�`�����W�̐ݒ�
				pVtx[0].tex = D3DXVECTOR2(0.0f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(0.2f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(0.2f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 1.0f);


				if (g_aAttack[nCntAttack].nNumEnemy == 1)
				{
					g_aAttack->move.x = sinf(D3DX_PI * g_aAttack[nCntAttack].nCntAnim) * 10.0f;
				}

				g_aAttack[nCntAttack].rot.z += g_aAttack[nCntAttack].fRotSpeed;



				if (g_aAttack[nCntAttack].rot.z > D3DX_PI)
				{
					g_aAttack[nCntAttack].rot.z -= D3DX_PI * 2.0f;
				}
				else if (g_aAttack[nCntAttack].rot.z < -D3DX_PI)
				{
					g_aAttack[nCntAttack].rot.z += D3DX_PI * 2.0f;
				}

				//========================================
				//�U�������ւ̃x�N�g��
				//========================================

			//============
			//�O�̏���
			//============
			/*	if (pPlayer->pos.x < g_aAttack[nCntAttack].pos.x + g_aAttack[nCntAttack].fWidth / 2 &&
					pPlayer->pos.x > g_aAttack[nCntAttack].pos.x - g_aAttack[nCntAttack].fWidth / 2 &&
					pPlayer->pos.y - pPlayer->fHeight / 2 >= g_aAttack[nCntAttack].pos.y - g_aAttack[nCntAttack].fHeight &&
					pPlayer->pos.y - pPlayer->fHeight / 2 <= g_aAttack[nCntAttack].pos.y)*/
					//========================================================================================
						//========================================
						//�U������v���C���[�ւ̃x�N�g��
						//========================================
				g_aAttack[nCntAttack].fXVaim = pPlayer->pos.x - g_aAttack[nCntAttack].pos.x;//X���W�ւ̃x�N�g��
				g_aAttack[nCntAttack].fYVaim = pPlayer->pos.y - g_aAttack[nCntAttack].pos.y;//Y���W�ւ̃x�N�g��
				g_aAttack[nCntAttack].fVaim = atan2f(g_aAttack[nCntAttack].fXVaim, g_aAttack[nCntAttack].fYVaim);//�v���C���[�ւ̊p�xg_aAttack[nCntAttack].fYVaim
				//=========================================================================================================================

				if (pPlayer->pos.x >= g_aAttack[nCntAttack].pos.x - g_aAttack[nCntAttack].fHeight &&
					pPlayer->pos.x <= g_aAttack[nCntAttack].pos.x + g_aAttack[nCntAttack].fHeight &&
					pPlayer->pos.y >= g_aAttack[nCntAttack].pos.y - g_aAttack[nCntAttack].fHeight &&
					pPlayer->pos.y <= g_aAttack[nCntAttack].pos.y + g_aAttack[nCntAttack].fHeight)
				{

					if (g_aAttack[nCntAttack].rot.z >= 0.0f && g_aAttack[nCntAttack].rot.z <= D3DX_PI)
					{
						if (g_aAttack[nCntAttack].fVaim > g_aAttack[nCntAttack].rot.z - 3.14f - 0.1f &&
							g_aAttack[nCntAttack].fVaim < g_aAttack[nCntAttack].rot.z - 3.14f + 0.1f)
						{//�v���C���[�ɍU������������(�v���C���[�̒� transform_pos[nCntAttack].x�S���ǂ̈ʒu�ɂ��邩�Ōv�Z�j

							if (g_nDelayAttack % 1 == 0)
							{
								if (g_aAttack[nCntAttack].nHitCount <= 10)
								{//�q�b�g�������ȉ��Ȃ�
									DamageHealth(20);

								}
							}
							g_aAttack[nCntAttack].nHitCount++;//�q�b�g���̃J�E���g���P���₷
						}
					}
					else if (g_aAttack[nCntAttack].rot.z >= -D3DX_PI && g_aAttack[nCntAttack].rot.z <= 0.0f)
					{
						if (g_aAttack[nCntAttack].fVaim > g_aAttack[nCntAttack].rot.z + 3.14f - 0.1f &&
							g_aAttack[nCntAttack].fVaim < g_aAttack[nCntAttack].rot.z + 3.14f + 0.1f)
						{//�v���C���[�ɍU������������(�v���C���[�̒� transform_pos[nCntAttack].x�S���ǂ̈ʒu�ɂ��邩�Ōv�Z�j

							if (g_nDelayAttack % 1 == 0)
							{
								if (g_aAttack[nCntAttack].nHitCount <= 10)
								{//�q�b�g�������ȉ��Ȃ�
									DamageHealth(20);

								}
							}
							g_aAttack[nCntAttack].nHitCount++;//�q�b�g���̃J�E���g���P���₷
						}
					}

				}
			}
			else if (g_aAttack[nCntAttack].type == ATTACKTYPE_ICICLECLAW)
			{//�G�l�~�[�P�P�̋Z�u�A�C�V�N���N���[�v
			g_aAttack[nCntAttack].nCntUseTime++;
			if (g_aAttack[nCntAttack].nCntUseTime == 1)
			{
				PlaySound(SOUND_LABEL_SE_ICICLECLAW);
			}
			Enemy* pEnemy = GetEnemy();
			Player* pPlayer = GetPlayer();
			g_aAttack[nCntAttack].nCntAnim++;//�A�j���[�V�����J�E���^�[��i�߂�

			if (g_aAttack[nCntAttack].nCntAnim % 3 == 0)
			{
				g_aAttack[nCntAttack].nPatternAnim++;

			}
			if (g_aAttack[nCntAttack].nPatternAnim >= 5)
			{
				g_aAttack[nCntAttack].nPatternAnim = 0;
				g_aAttack[nCntAttack].nPatternAnimY++;//�A�j���[�V�����̒i�����̒i�ֈړ�


				if (g_aAttack[nCntAttack].nPatternAnimY >= 2)
				{//�A�j���[�V�������ŏ��̈ʒu��
					g_aAttack[nCntAttack].nPatternAnim = 0;
					g_aAttack[nCntAttack].nPatternAnimY = 0;
					//g_aAttack[nCntAttack].bUse = false;//�g�p���Ă��Ȃ���Ԃɂ���
				}
			}

			//�e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 0.0f + 0.5f * g_aAttack[nCntAttack].nPatternAnimY);
			pVtx[1].tex = D3DXVECTOR2(0.2f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 0.0f + 0.5f * g_aAttack[nCntAttack].nPatternAnimY);
			pVtx[2].tex = D3DXVECTOR2(0.0f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 0.5f + 0.5f * g_aAttack[nCntAttack].nPatternAnimY);
			pVtx[3].tex = D3DXVECTOR2(0.2f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 0.5f + 0.5f * g_aAttack[nCntAttack].nPatternAnimY);



			/*	g_aAttack[nCntAttack].rot.z -= 0.01f;*/


			if (g_aAttack[nCntAttack].rot.z > D3DX_PI)
			{
				g_aAttack[nCntAttack].rot.z -= D3DX_PI * 2.0f;
			}
			else if (g_aAttack[nCntAttack].rot.z < -D3DX_PI)
			{
				g_aAttack[nCntAttack].rot.z += D3DX_PI * 2.0f;
			}
			//========================================
			//�U�������ւ̃x�N�g��
			//========================================

		//============
		//�O�̏���
		//============
		/*	if (pPlayer->pos.x < g_aAttack[nCntAttack].pos.x + g_aAttack[nCntAttack].fWidth / 2 &&
				pPlayer->pos.x > g_aAttack[nCntAttack].pos.x - g_aAttack[nCntAttack].fWidth / 2 &&
				pPlayer->pos.y - pPlayer->fHeight / 2 >= g_aAttack[nCntAttack].pos.y - g_aAttack[nCntAttack].fHeight &&
				pPlayer->pos.y - pPlayer->fHeight / 2 <= g_aAttack[nCntAttack].pos.y)*/
				//========================================================================================
					//========================================
					//�U������v���C���[�ւ̃x�N�g��
					//========================================
			g_aAttack[nCntAttack].fXVaim = pPlayer->pos.x - g_aAttack[nCntAttack].pos.x;//X���W�ւ̃x�N�g��
			g_aAttack[nCntAttack].fYVaim = pPlayer->pos.y - g_aAttack[nCntAttack].pos.y;//Y���W�ւ̃x�N�g��
			g_aAttack[nCntAttack].fVaim = atan2f(g_aAttack[nCntAttack].fXVaim, g_aAttack[nCntAttack].fYVaim);//�v���C���[�ւ̊p�xg_aAttack[nCntAttack].fYVaim
			//=========================================================================================================================

			if (pPlayer->pos.x >= g_aAttack[nCntAttack].pos.x - g_aAttack[nCntAttack].fHeight &&
				pPlayer->pos.x <= g_aAttack[nCntAttack].pos.x + g_aAttack[nCntAttack].fHeight &&
				pPlayer->pos.y >= g_aAttack[nCntAttack].pos.y - g_aAttack[nCntAttack].fHeight &&
				pPlayer->pos.y <= g_aAttack[nCntAttack].pos.y + g_aAttack[nCntAttack].fHeight)
			{

				if (g_aAttack[nCntAttack].rot.z >= 0.0f && g_aAttack[nCntAttack].rot.z <= D3DX_PI)
				{
					if (g_aAttack[nCntAttack].fVaim > g_aAttack[nCntAttack].rot.z - 3.14f - 0.1f &&
						g_aAttack[nCntAttack].fVaim < g_aAttack[nCntAttack].rot.z - 3.14f + 0.1f)
					{//�v���C���[�ɍU������������(�v���C���[�̒� transform_pos[nCntAttack].x�S���ǂ̈ʒu�ɂ��邩�Ōv�Z�j

						if (g_nDelayAttack % 1 == 0)
						{
							if (g_aAttack[nCntAttack].nHitCount <= 0)
							{//�q�b�g�������ȉ��Ȃ�
								DamageHealth(20);

							}
						}
						g_aAttack[nCntAttack].nHitCount++;//�q�b�g���̃J�E���g���P���₷
					}
				}
				else if (g_aAttack[nCntAttack].rot.z >= -D3DX_PI && g_aAttack[nCntAttack].rot.z <= 0.0f)
				{
					if (g_aAttack[nCntAttack].fVaim > g_aAttack[nCntAttack].rot.z + 3.14f - 0.1f &&
						g_aAttack[nCntAttack].fVaim < g_aAttack[nCntAttack].rot.z + 3.14f + 0.1f)
					{//�v���C���[�ɍU������������(�v���C���[�̒� transform_pos[nCntAttack].x�S���ǂ̈ʒu�ɂ��邩�Ōv�Z�j

						if (g_nDelayAttack % 1 == 0)
						{
							if (g_aAttack[nCntAttack].nHitCount <= 0)
							{//�q�b�g�������ȉ��Ȃ�
								DamageHealth(20);

							}
						}
						g_aAttack[nCntAttack].nHitCount++;//�q�b�g���̃J�E���g���P���₷
					}
				}

			}
			}
			else if (g_aAttack[nCntAttack].type == ATTACKTYPE_ABSOLUTE_BLIZZARD)
			{//�{�X�Q�̋Z�u�A�u�\�����[�g�E�u���U�[�h�v

			g_aAttack[nCntAttack].nCntUseTime++;
			if (g_aAttack[nCntAttack].nCntUseTime == 1)
			{
				PlaySound(SOUND_LABEL_SE_ABSOLUTEBLIZARD);
			}

				Enemy* pEnemy = GetEnemy();
				Player* pPlayer = GetPlayer();
				g_aAttack[nCntAttack].nCntAnim++;//�A�j���[�V�����J�E���^�[��i�߂�

				if (g_aAttack[nCntAttack].nCntAnim % 1 == 0)
				{
					g_aAttack[nCntAttack].nPatternAnim++;

				}
				if (g_aAttack[nCntAttack].nPatternAnim >= 5)
				{
					g_aAttack[nCntAttack].nPatternAnim = 0;
					g_aAttack[nCntAttack].nPatternAnimY++;//�A�j���[�V�����̒i�����̒i�ֈړ�


					if (g_aAttack[nCntAttack].nPatternAnimY >= 2)
					{//�A�j���[�V�������ŏ��̈ʒu��
						g_aAttack[nCntAttack].nPatternAnim = 0;
						g_aAttack[nCntAttack].nPatternAnimY = 0;
						//g_aAttack[nCntAttack].bUse = false;//�g�p���Ă��Ȃ���Ԃɂ���
					}
				}
				//�e�N�X�`�����W�̐ݒ�
				pVtx[0].tex = D3DXVECTOR2(0.0f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 0.0f + 0.5f * g_aAttack[nCntAttack].nPatternAnimY);
				pVtx[1].tex = D3DXVECTOR2(0.2f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 0.0f + 0.5f * g_aAttack[nCntAttack].nPatternAnimY);
				pVtx[2].tex = D3DXVECTOR2(0.0f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 0.5f + 0.5f * g_aAttack[nCntAttack].nPatternAnimY);
				pVtx[3].tex = D3DXVECTOR2(0.2f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 0.5f + 0.5f * g_aAttack[nCntAttack].nPatternAnimY);



				if (pPlayer->pos.x >= g_aAttack[nCntAttack].pos.x - g_aAttack[nCntAttack].fWidth / 2 - pPlayer->fWidth &&
					pPlayer->pos.x <= g_aAttack[nCntAttack].pos.x + g_aAttack[nCntAttack].fWidth / 2 + pPlayer->fWidth &&
					pPlayer->pos.y - pPlayer->fHeight / 2 >= g_aAttack[nCntAttack].pos.y - g_aAttack[nCntAttack].fHeight / 2 &&
					pPlayer->pos.y - pPlayer->fHeight / 2 <= g_aAttack[nCntAttack].pos.y + pPlayer->fHeight / 2)
				{//�v���C���[�ɍU������������(�v���C���[�̒��S���ǂ̈ʒu�ɂ��邩�Ōv�Z�j
					if (g_nDelayAttack % 1 == 0)
					{
						if (g_aAttack[nCntAttack].nHitCount <= 0)
						{//�q�b�g�������ȉ��Ȃ�
							DamageHealth(150);

						}
					}
					g_aAttack[nCntAttack].nHitCount++;//�q�b�g���̃J�E���g���P���₷
				}


			}
			else if (g_aAttack[nCntAttack].type == ATTACKTYPE_FIRE)
			{//�{�X�R�̋Z�u���U���v
				int nCntSaveEnemy = 0;
				g_aAttack[nCntAttack].nCntUseTime++;
				if (g_aAttack[nCntAttack].nCntUseTime == 1)
				{
					PlaySound(SOUND_LABEL_SE_FIRE);
				}

				if (g_aAttack[nCntAttack].rot.z == 0.2f)
				{
					if (g_aAttack[nCntAttack].pos.y >= 675.0f && g_aAttack[nCntAttack].nHitCount == 0)
					{
						SetAttack(g_aAttack[nCntAttack].pos,
							D3DXVECTOR3(0.0f, 0.0f, 0.0f),
							ATTACKTYPE_FIRE,
							-1, -1, -1, MOVETYPE_NORMAL, 70.0f, 200.0f, 80, 0.0f, -1);

						g_aAttack[nCntAttack].nHitCount++;
					}
				}

				Enemy* pEnemy = GetEnemy();
				Player* pPlayer = GetPlayer();
				g_aAttack[nCntAttack].nCntAnim++;//�A�j���[�V�����J�E���^�[��i�߂�

				if (g_aAttack[nCntAttack].nCntAnim % 3 == 0)
				{
					g_aAttack[nCntAttack].nPatternAnim++;

				}
				if (g_aAttack[nCntAttack].nPatternAnim >= 5)
				{
					g_aAttack[nCntAttack].nPatternAnim = 0;
				}

				float fRandumRot;
				float fRandumSpeed;
				fRandumRot = float((rand)() % 100 + 51) / 100.0f;
				fRandumSpeed = float((rand)() % 100) / 5.0f;

				if (g_aAttack[nCntAttack].rot.z == 0.1f)
				{
					if (g_nDelayAttack % 8 == 0)
					{
						SetBullet(g_aAttack[nCntAttack].pos, D3DXVECTOR3(sinf(-D3DX_PI * fRandumRot) * 10.0f, cosf(-D3DX_PI * fRandumRot) * 10.0f, 0.0f),
							500, BULLETTYPE_ENEMY, -1, -1, -1, true, MOVETYPE_GRAVITY);
					}
				}


				//�e�N�X�`�����W�̐ݒ�
				pVtx[0].tex = D3DXVECTOR2(0.0f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(0.2f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(0.2f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 1.0f);




				if (pPlayer->pos.x >= g_aAttack[nCntAttack].pos.x - g_aAttack[nCntAttack].fWidth / 2 - pPlayer->fWidth &&
					pPlayer->pos.x <= g_aAttack[nCntAttack].pos.x + g_aAttack[nCntAttack].fWidth / 2 &&
					pPlayer->pos.y - pPlayer->fHeight / 2 >= g_aAttack[nCntAttack].pos.y - g_aAttack[nCntAttack].fHeight &&
					pPlayer->pos.y - pPlayer->fHeight / 2 <= g_aAttack[nCntAttack].pos.y + pPlayer->fHeight / 2)
				{//�v���C���[�ɍU������������(�v���C���[�̒��S���ǂ̈ʒu�ɂ��邩�Ōv�Z�j
					if (g_nDelayAttack % 1 == 0)
					{
						if (g_aAttack[nCntAttack].nHitCount <= 0)
						{//�q�b�g�������ȉ��Ȃ�
							DamageHealth(1);

						}
					}
					//g_aAttack[nCntAttack].nHitCount++;//�q�b�g���̃J�E���g���P���₷
				}
			}
			//===============================
			//�U���̓����������߂鏈��
			//===============================

			bool b = false;
			//////�ړ��ʂ��X�V�i����������j
			//g_aAttack[nCntAttack].move.x += (0.0f - g_aAttack[nCntAttack].move.x) * 0.5f;
			////g_aPlayer.move.y += (0.0f - g_aPlayer.move.y) * 0.5f;

			if (g_aAttack[nCntAttack].moveType == MOVETYPE_GRAVITY)
			{

				g_aAttack[nCntAttack].bIsLanding = bCollisionBlock(&g_aAttack[nCntAttack].pos,
					&g_aAttack[nCntAttack].posOld,
					&g_aAttack[nCntAttack].move,
					g_aAttack[nCntAttack].fWidth,
					g_aAttack[nCntAttack].fHeight,
					&g_aAttack[nCntAttack].fGravity,
					COLLISIONTYPE_LENGTH, &b, COLLISIONUSE_ATTACK,
					&g_aAttack[nCntAttack].bIsWalling);
			}
			if (g_aAttack[nCntAttack].moveType == MOVETYPE_GRAVITY && g_aAttack[nCntAttack].bIsLanding == true)
			{
				g_aAttack[nCntAttack].move.y = 0.0f;
			}
			else if (g_aAttack[nCntAttack].moveType == MOVETYPE_GRAVITY && g_aAttack[nCntAttack].bIsLanding == false)
			{
				g_aAttack[nCntAttack].move.y += 2.0f;
			}

			g_aAttack[nCntAttack].posOld.x = g_aAttack[nCntAttack].pos.x;
			g_aAttack[nCntAttack].posOld.y = g_aAttack[nCntAttack].pos.y;

			g_aAttack[nCntAttack].pos.y += g_aAttack[nCntAttack].move.y;
			g_aAttack[nCntAttack].pos.x += g_aAttack[nCntAttack].move.x;


			//==========================================================================

			if (g_aAttack[nCntAttack].nLife <= 0)
			{//�\�����Ԃ��O�ȉ��ɂȂ�����i�U���̎������s�����j(��ނ͑S�āj

				g_aAttack[nCntAttack].bUse = false;//�g�p���Ă��Ȃ���Ԃɂ���
			}

		

			//============================================================================================================================================================
		}


		pVtx += 4;
	}


	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffAttack->Unlock();
}

//�U���̕`�揈��
void DrawAttack(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�ւ̃|�C���^

	int nCntAttack;

	Enemy* pEnemy = GetEnemy();
	Player* pPlayer = GetPlayer();

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffAttack, 0, sizeof(VERTEX_2D));//���_�o�b�t�@�ւ̃|�C���^�ƒ��_���̍\���̂̃T�C�Y

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	for (nCntAttack = 0; nCntAttack < MAX_ATTACK; nCntAttack++)
	{
		if (g_aAttack[nCntAttack].bUse == true)
		{
			if (g_aAttack[nCntAttack].type ==ATTACKTYPE_PUNCH)
			{
				pDevice->SetTexture(0, g_pTextureAttack[ATTACKTYPE_PUNCH]);
			}
			if (g_aAttack[nCntAttack].type == ATTACKTYPE_SLASH_E)
			{
				pDevice->SetTexture(0, g_pTextureAttack[ATTACKTYPE_SLASH_E]);
			}
			if (g_aAttack[nCntAttack].type == ATTACKTYPE_ICE_EXPLOSION)
			{
				pDevice->SetTexture(0, g_pTextureAttack[ATTACKTYPE_ICE_EXPLOSION]);
			}
			if (g_aAttack[nCntAttack].type == ATTACKTYPE_THUNDER_E4)
			{
				pDevice->SetTexture(0, g_pTextureAttack[ATTACKTYPE_THUNDER_E4]);
			}
			if (g_aAttack[nCntAttack].type == ATTACKTYPE_BOMU)
			{
				pDevice->SetTexture(0, g_pTextureAttack[ATTACKTYPE_BOMU]);
			}
			if (g_aAttack[nCntAttack].type == ATTACKTYPE_NEEDLE)
			{
				pDevice->SetTexture(0, g_pTextureAttack[ATTACKTYPE_NEEDLE]);
			}
			if (g_aAttack[nCntAttack].type == ATTACKTYPE_CLAW)
			{
				pDevice->SetTexture(0, g_pTextureAttack[ATTACKTYPE_CLAW]);
			}
			if (g_aAttack[nCntAttack].type == ATTACKTYPE_VACUUMBLADE)
			{
				pDevice->SetTexture(0, g_pTextureAttack[ATTACKTYPE_VACUUMBLADE]);
			}
			if (g_aAttack[nCntAttack].type == ATTACKTYPE_FALLINGBLOCK)
			{
				pDevice->SetTexture(0, g_pTextureAttack[ATTACKTYPE_FALLINGBLOCK]);
			}
			if (g_aAttack[nCntAttack].type == ATTACKTYPE_BEAM)
			{
				pDevice->SetTexture(0, g_pTextureAttack[ATTACKTYPE_BEAM]);
			}
			if (g_aAttack[nCntAttack].type == ATTACKTYPE_ICICLECLAW)
			{
				pDevice->SetTexture(0, g_pTextureAttack[ATTACKTYPE_ICICLECLAW]);
			}
			if (g_aAttack[nCntAttack].type == ATTACKTYPE_ABSOLUTE_BLIZZARD)
			{
				pDevice->SetTexture(0, g_pTextureAttack[ATTACKTYPE_ABSOLUTE_BLIZZARD]);
			}
			if (g_aAttack[nCntAttack].type == ATTACKTYPE_FIRE)
			{
				pDevice->SetTexture(0, g_pTextureAttack[ATTACKTYPE_FIRE]);
			}
			if (g_aAttack[nCntAttack].type == ATTACKTYPE_SHAKINGSWORD)
			{
				pDevice->SetTexture(0, g_pTextureAttack[ATTACKTYPE_SHAKINGSWORD]);
			}
			if (g_aAttack[nCntAttack].type == ATTACKTYPE_FLASHBLADE)
			{
				pDevice->SetTexture(0, g_pTextureAttack[ATTACKTYPE_FLASHBLADE]);
			}
			if (g_aAttack[nCntAttack].type == ATTACKTYPE_THUNDER_2)
			{
				pDevice->SetTexture(0, g_pTextureAttack[ATTACKTYPE_THUNDER_2]);
			}
			if (g_aAttack[nCntAttack].type == ATTACKTYPE_FINALBEAM)
			{
				pDevice->SetTexture(0, g_pTextureAttack[ATTACKTYPE_FINALBEAM]);
			}
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,//0�́A�`�悷��ŏ��̒��_�C���f�b�N�X�ŁA2�́A�`�悷��v���~�e�B�u��
				nCntAttack * 4,
				2);
		}

	}


}

//�U���̐ݒ菈��
void SetAttack(D3DXVECTOR3 pos, D3DXVECTOR3 move, ATTACKTYPE type, int nNum, int nNumEnemy, int nAimingEnemy, MOVETYPE moveType,
	float Width, float Height, int nLife, float rot, float fRotSpeed)//pos�́u���ˈʒu�v�Amove�́u�ړ��ʁv,damage�́u�e�̍U���́vnNumEney�́A���Ԗڂ̓G���U�����Ă��邩?
{
int nCntAttack;

	VERTEX_2D* pVtx;
	g_pVtxBuffAttack->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntAttack = 0; nCntAttack < MAX_ATTACK; nCntAttack++)
	{
	

		if (g_aAttack[nCntAttack].bUse == false)
		{//�U�����g�p����Ă��Ȃ�
		
			//=================================================
			//�Z�b�g�Őݒ肷�鏈��
			//=================================================
			g_aAttack[nCntAttack].pos = pos;//�ʒu
			g_aAttack[nCntAttack].posOld = pos;//�O�̈ʒu
			g_aAttack[nCntAttack].move = move;//����
			g_aAttack[nCntAttack].bUse = true;//�g�p���Ă����Ԃɂ���
			g_aAttack[nCntAttack].type = type;//�U���̎�ށitype)��������B
			g_aAttack[nCntAttack].nCntAnim = 0;//�A�j���[�V�����J�E���^�[
			g_aAttack[nCntAttack].nPatternAnim = 0;//�A�j���[�V�����p�^�[��
			g_aAttack[nCntAttack].nPatternAnimY = 0;//�A�j���[�V�������Q�i�ȏ�̎��ɒe���ڂ�����
			g_aAttack[nCntAttack].nHitCount = 0;//���̍U���Ńq�b�g����񐔂̐���
			g_aAttack[nCntAttack].nNumEnemy = nNumEnemy;//�ǂ̓G���U�����Ă��邩�𔻒肷��
			g_aAttack[nCntAttack].moveType = moveType;//�U���̓������̎��
			g_aAttack[nCntAttack].bIsLanding = false;//�n�ʂɂ��Ă���t���O
			g_aAttack[nCntAttack].bIsWalling = false;//�ǂɓ������Ă��邩
			g_aAttack[nCntAttack].fGravity = 0.0f;//�d�͕␳�l
			g_aAttack[nCntAttack].nColorChange = 0;//�F��ς���
			g_aAttack[nCntAttack].nCntUseTime = 0;//�U�����g���Ă��鎞�Ԃ��J�E���g����
			g_aAttack[nCntAttack].rot.z = 0.0f;
			g_aAttack[nCntAttack].nLife = nLife;//����
			g_aAttack[nCntAttack].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//����
			g_aAttack[nCntAttack].fWidth = Width;//��
			g_aAttack[nCntAttack].fHeight = Height;//����
			g_aAttack[nCntAttack].rot.z = rot;
			g_aAttack[nCntAttack].fXVaim = 0.0f;
			g_aAttack[nCntAttack].fYVaim = 0.0f;
			g_aAttack[nCntAttack].fVaim = 0.0f;
			g_aAttack[nCntAttack].fEffectiveScale = 0.0f;//�U���̌��ʔ͈͂����߂�
			g_aAttack[nCntAttack].fRotSpeed = fRotSpeed;//��]���x
			//=================================================================================
		
			//���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLOR�́A0.0f�`1.0f�̊Ԃ̐��l�ŐF��ς���B
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLOR�́A0.0f�`1.0f�̊Ԃ̐��l�ŐF��ς���B
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLOR�́A0.0f�`1.0f�̊Ԃ̐��l�ŐF��ς���B
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLOR�́A0.0f�`1.0f�̊Ԃ̐��l�ŐF��ς���B

			break;
		}
		pVtx += 4;
	}
	g_pVtxBuffAttack->Unlock();
}

//=====================================================
//�U���\���̂̏����擾
//=====================================================
Attack* GetAttack(void)
{
	return &g_aAttack[0];//�\���̂̐擪�A�h���X��Ԃ��B
}
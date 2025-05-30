//========================================================
//
//�P�O���Q�S���F�{�X�Q�̕K�E�Z�����[Special.cpp]
//Author:ShinaTaiyo
//
//=========================================================
#include "main.h"
#include "bullet.h"
#include "explosion.h"
#include <stdlib.h>
#include <time.h>
#include "score.h"
#include "game.h"
#include "result.h"
#include "player.h"
#include "special.h"
#include "sound.h"
#include "enemy.h"
#include "fade.h"
//�}�N����`
#include "input.h"
#define NUM_SpecialTEX (SPECIALTYPE_MAX)//���o�̎��
#define MAX_Special (5000)

#define SCOREBONUS_SIZE (300.0f)
#define APPER_SpecialSIZE (50.0f)
#define PRESSSTART_SpecialSIZE (120.0f)

#define SHIELD_SIZE (100.0f)


//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_apTextureSpecial[NUM_SpecialTEX] = {};
//D3DXVECTOR3 g_rotPI;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSpecial = NULL;
//Enemy g_Special[MAX_ENEMY];//�G�̏��
//Result g_aResult;

Special g_Special[MAX_Special];

//int g_nCountTime = 0;//���Ԃ�\���O���[�o���ϐ�

//=====================================================
//==========================
//�G�̏���������
//==========================
void InitSpecial(void)
{



	//srand((unsigned int)time(NULL));//�Q�[�����J�n�������Ԗ��ɗ����̎��ݒ�



	LPDIRECT3DDEVICE9 pDevice;
	int	nCntSpecial;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();



	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\SpecialAttack_Boss1.png",
		&g_apTextureSpecial[SPECIALTYPE_BOSS1_SPECIAL_ATTACK]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\SpecialAttack_Boss2.png",
		&g_apTextureSpecial[SPECIALTYPE_BOSS2_SPECIAL_ATTACK]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\SpecialAttack_Boss3.png",
		&g_apTextureSpecial[SPECIALTYPE_BOSS3_SPECIAL_ATTACK]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\SpecialAttack_Boss4.png",
		&g_apTextureSpecial[SPECIALTYPE_BOSS4_SPECIAL_ATTACK]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\SpecialAttack_FinalBoss.png",
		&g_apTextureSpecial[SPECIALTYPE_FINALBOSS_SPECIAL_ATTACK]);








	//�G���̏�����
	for (nCntSpecial = 0; nCntSpecial < MAX_Special; nCntSpecial++)
	{
		g_Special[nCntSpecial].nSpecialType = 0;
		g_Special[nCntSpecial].nLife = 0;
		g_Special[nCntSpecial].bUse = false;
		g_Special[nCntSpecial].bCountStartFlag = false;//���o���n�܂��Ă���̃J�E���g���J�n����t���O��錾����B
		g_Special[nCntSpecial].nSpecialCount = 0;//���o���n�܂��Ă���̃J�E���g������������B
		g_Special[nCntSpecial].fSpecialPatternCountY = 0.0;//�A�j���[�V������x�点��J�E���g
		g_Special[nCntSpecial].fSpecialPattern = 0.0f;//�A�j���[�V�����̎��
		g_Special[nCntSpecial].nAnimationRepeat = 0;//�A�j���[�V�������J��Ԃ��ϐ�
		g_Special[nCntSpecial].bDraw = true;//bUse��true���������Ƃɉ����A�X��bDraw��true���ǂ����ŕ`�悷�邩�ǂ��������߂�B
		g_Special[nCntSpecial].nCntEnemy = 0;
		g_Special[nCntSpecial].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Special[nCntSpecial].fColor_a = 0.0f;
		g_Special[nCntSpecial].bColorChange_a = false;
		g_Special[nCntSpecial].nAtackSummon = 0;//���o�������ꏊ�ɍU������������
		g_Special[nCntSpecial].rot = 0.0f;//��]
		g_Special[nCntSpecial].fScale = 0.0f;//�傫��
		g_Special[nCntSpecial].fAeffective_Scale = 0.0f;//���������U���̌��ʔ͈�
		g_Special[nCntSpecial].fA_Width = 0.0f;//���������U���̕�
		g_Special[nCntSpecial].fA_Height = 0.0f;//���������U���̍���
		g_Special[nCntSpecial].nA_Life = 0.0f;//���������U���̃��C�t
		g_Special[nCntSpecial].fA_rotSpeed = 0.0f;//��������U���̉�]���x
		g_Special[nCntSpecial].nCntTime = 0;//���Ԃ��v������
	}

	//g_Special->pos = D3DXVECTOR3(0.0f,0.0f, 0.0f);//�ʒu������������

	//�`���[�g���A�����̏�����
	//for (nCntSpecial = 0; nCntSpecial < NUM_SpecialTEX; nCntSpecial++)
	//{
	//	
	//}


	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_Special,// * NUM_SpecialTEX,//�S�͕K�v�Ȓ��_���B����g���摜����2�������A��̃|���S���ł��̂ŁA���_�S�ő��v�B
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffSpecial,
		NULL);

	VERTEX_2D* pVtx;

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffSpecial->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntSpecial = 0; nCntSpecial < MAX_Special; nCntSpecial++)
	{
		//���_���W�̐ݒ�(�G�̑傫���j
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

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
	g_pVtxBuffSpecial->Unlock();
}

//====================================
//�`���[�g���A���̏I������
//====================================
void UninitSpecial(void)
{
	//�e�N�X�`���̔j��
	int nCntSpecial;
	for (nCntSpecial = 0; nCntSpecial < NUM_SpecialTEX; nCntSpecial++)
	{
		if (g_apTextureSpecial[nCntSpecial] != NULL)
		{
			g_apTextureSpecial[nCntSpecial]->Release();
			g_apTextureSpecial[nCntSpecial] = NULL;
		}
	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffSpecial != NULL)
	{
		g_pVtxBuffSpecial->Release();
		g_pVtxBuffSpecial = NULL;
	}
}


//========================================
//�G�̍X�V����
//========================================
void UpdateSpecial(void)
{
	VERTEX_2D* pVtx;



	int nCntSpecial;


	Enemy* pEnemy = GetEnemy();


	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffSpecial->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntSpecial = 0; nCntSpecial < MAX_Special; nCntSpecial++)
	{
		if (g_Special[nCntSpecial].bUse == true)
		{
			g_Special[nCntSpecial].nLife--;



			g_Special[nCntSpecial].nSpecialCount++;


			if (g_Special[nCntSpecial].nLife <= 0)
			{
				g_Special[nCntSpecial].bUse = false;
			}

			
			if (g_Special[nCntSpecial].nSpecialType == SPECIALTYPE_BOSS1_SPECIAL_ATTACK ||
				g_Special[nCntSpecial].nSpecialType == SPECIALTYPE_BOSS2_SPECIAL_ATTACK ||
				g_Special[nCntSpecial].nSpecialType == SPECIALTYPE_BOSS3_SPECIAL_ATTACK ||
				g_Special[nCntSpecial].nSpecialType == SPECIALTYPE_BOSS4_SPECIAL_ATTACK ||
				g_Special[nCntSpecial].nSpecialType == SPECIALTYPE_FINALBOSS_SPECIAL_ATTACK)
			{
				if (g_Special[nCntSpecial].nSpecialCount == 1)
				{//�|�[�Y��Ԃɂ��āA���o���������s��
					SetPause(PAUSETYPE_STAGING);
				}
				//���_���W�̐ݒ�(�G�̑傫���j
				pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

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

				if (g_Special[nCntSpecial].nLife == 0)
				{
					g_Special[nCntSpecial].bUse = false;
					SetPause(PAUSETYPE_UNLOCK);//�|�[�Y��ԉ���
				}
			}
		


		}
		pVtx += 4;//���_�f�[�^�̃|�C���^���S���i�߂�B
	}
	//���_�o�b�t�@���A�����b�N����

	g_pVtxBuffSpecial->Unlock();


}


//=========================================
//�G�̕`�揈��
//=========================================
void DrawSpecial(void)
{

	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�ւ̃|�C���^

	Enemy* pEnemy = GetEnemy();

	int nCntSpecial;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffSpecial, 0, sizeof(VERTEX_2D));//���_�o�b�t�@�ւ̃|�C���^�ƒ��_���̍\���̂̃T�C�Y

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�

	for (nCntSpecial = 0; nCntSpecial < MAX_Special; nCntSpecial++)
	{
		if (g_Special[nCntSpecial].bUse == true)
		{
			if (g_Special[nCntSpecial].bDraw == true)
			{
				
				if (g_Special[nCntSpecial].nSpecialType == SPECIALTYPE_BOSS1_SPECIAL_ATTACK)
				{
					pDevice->SetTexture(0, g_apTextureSpecial[SPECIALTYPE_BOSS1_SPECIAL_ATTACK]);
				}
				if (g_Special[nCntSpecial].nSpecialType == SPECIALTYPE_BOSS2_SPECIAL_ATTACK)
				{
					pDevice->SetTexture(0, g_apTextureSpecial[SPECIALTYPE_BOSS2_SPECIAL_ATTACK]);
				}
				if (g_Special[nCntSpecial].nSpecialType == SPECIALTYPE_BOSS3_SPECIAL_ATTACK)
				{
					pDevice->SetTexture(0, g_apTextureSpecial[SPECIALTYPE_BOSS3_SPECIAL_ATTACK]);
				}
				if (g_Special[nCntSpecial].nSpecialType == SPECIALTYPE_BOSS4_SPECIAL_ATTACK)
				{
					pDevice->SetTexture(0, g_apTextureSpecial[SPECIALTYPE_BOSS4_SPECIAL_ATTACK]);
				}
				if (g_Special[nCntSpecial].nSpecialType == SPECIALTYPE_FINALBOSS_SPECIAL_ATTACK)
				{
					pDevice->SetTexture(0, g_apTextureSpecial[SPECIALTYPE_FINALBOSS_SPECIAL_ATTACK]);
				}
				//�|���S���̕`��
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntSpecial * 4, 2);//0�́A�`�悷��ŏ��̒��_�C���f�b�N�X�ŁA2�́A�`�悷��v���~�e�B�u��

				//&g_pVtxBuffPolygon[0],//���_���̐擪�A�h���X
				//sizeof(VERTEX_2D));//���_���\���̂̃T�C�Y
			}
		}
	}


}
void SetSpecial(SpecialTYPE type, D3DXVECTOR3 pos, int nLife)
{
	int nCntSpecial;

	VERTEX_2D* pVtx;

	/*Enemy* pEnemy = GetEnemy();*/

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffSpecial->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntSpecial = 0; nCntSpecial < MAX_Special; nCntSpecial++)
	{
		if (g_Special[nCntSpecial].bUse == false)
		{

			g_Special[nCntSpecial].pos = pos;//SetEnemy�̎��ɁA�G�̈ʒu��������ꂽ�ہA�ʒu�̈����������SetSpecial��pos�ɑ�����邱�Ƃɂ��A
			g_Special[nCntSpecial].nLife = nLife;
			g_Special[nCntSpecial].nSpecialType = type;//���o�̎�ނ��Z�b�g����B
			g_Special[nCntSpecial].bUse = true;
			g_Special[nCntSpecial].nSpecialCount = 0;
			g_Special[nCntSpecial].bCountStartFlag = true;
			g_Special[nCntSpecial].fSpecialPatternCountY = 0.0f;
			g_Special[nCntSpecial].fSpecialPattern = 0.0f;
			g_Special[nCntSpecial].nAnimationRepeat = 0;
			g_Special[nCntSpecial].bDraw = true;
			g_Special[nCntSpecial].fColor_a = 1.0f;
			g_Special[nCntSpecial].bColorChange_a = false;
			g_Special[nCntSpecial].nCntTime = 0;//���Ԃ��v������

			break;
		}
		pVtx += 4;
	}
	g_pVtxBuffSpecial->Unlock();
}

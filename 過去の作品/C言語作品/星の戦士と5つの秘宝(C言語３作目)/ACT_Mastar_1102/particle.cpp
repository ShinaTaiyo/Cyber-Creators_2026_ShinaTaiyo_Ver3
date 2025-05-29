//========================================================
//
//�W���R�O���F�G�t�F�N�g������񌩒���[Particle.cpp]
//Author:ShinaTaiyo
//
//=========================================================
#include "main.h"
#include "bullet.h"
#include "input.h"
#include "explosion.h"
#include "enemy.h"
#include "player.h"
#include "health.h"
#include <time.h>
#include "particle.h"

//#define MAX_PARTICLE (20000)//�G�t�F�N�g�̍ő吔
#define NUM_PARTICLE (1)//�G�t�F�N�g�̎��

//�p�[�e�B�N���\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;//�ʒu
	D3DXVECTOR3 move;//�ړ���
	D3DXCOLOR col;//�F
	float fRadius;//���a�i�傫���j
	int nLife;//�����i�\�����ԁj
	bool bUse;//�g�p���Ă��邩�ǂ���
	int nScaleType;//�G�t�F�N�g�̑傫����ς���X�s�[�h�����߂�B
	float fRot;//�p�[�e�B�N���𓮂����p�x�����߂�
	float fSpeed;//�p�[�e�B�N���𓮂����X�s�[�h�i�ړ��ʁj�����߂�B

}Particle;

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureParticle[NUM_PARTICLE] = {};//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffParticle = NULL;//�e�̏��
Particle g_aParticle[MAX_PARTICLE];//�G�t�F�N�g�̏��

float g_fRandumRot;
float  g_fRandumSpeed;

//�G�t�F�N�g�̏���������
void InitParticle(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int	nCntParticle;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ��݁i�e�̃e�N�X�`����ǂݍ��ށj
	D3DXCreateTextureFromFile(pDevice,
		"data\\effect000.jpg",
		&g_pTextureParticle[0]);

	//�G�t�F�N�g���̏�����
	for (nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		g_aParticle[nCntParticle].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aParticle[nCntParticle].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		g_aParticle[nCntParticle].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aParticle[nCntParticle].nLife = 0;
		g_aParticle[nCntParticle].bUse = false;//�g�p���Ă��Ȃ���Ԃɂ���
		g_aParticle[nCntParticle].fRadius = 0.0f;
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_PARTICLE,//�S�͕K�v�Ȓ��_��
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffParticle,
		NULL);

	VERTEX_2D* pVtx;

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		//���_���W�̐ݒ�(�G�t�F�N�g�̑傫���j
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(20.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 20.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(20.0f, 20.0f, 0.0f);

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
	g_pVtxBuffParticle->Unlock();
}

//�G�t�F�N�g�̏I������
void UninitParticle(void)
{
	//�e�N�X�`���̔j��
	int nCnt;
	for (nCnt = 0; nCnt < NUM_PARTICLE; nCnt++)
	{
		if (g_pTextureParticle[nCnt] != NULL)
		{
			g_pTextureParticle[nCnt]->Release();
			g_pTextureParticle[nCnt] = NULL;
		}
	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffParticle != NULL)
	{
		g_pVtxBuffParticle->Release();
		g_pVtxBuffParticle = NULL;
	}
}

//�G�t�F�N�g�̍X�V����
void UpdateParticle(void)
{




	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D* pVtx;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();
	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);

	int nCntParticle;
	for (nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].bUse == true)
		{//�G�t�F�N�g���g�p�����
			//���_�J���[�̐ݒ�
			g_aParticle[nCntParticle].fRadius -= 1.0f;
			g_aParticle[nCntParticle].nLife--;

		

	/*		g_aParticle[nCntParticle].move.y = cosf(g_fRandumRot) * g_fRandumSpeed;
			g_aParticle[nCntParticle].move.x = sinf(g_fRandumRot) * g_fRandumSpeed;*/

			g_aParticle[nCntParticle].pos.y += g_aParticle[nCntParticle].move.y;
			g_aParticle[nCntParticle].pos.x += g_aParticle[nCntParticle].move.x;

			

		pVtx[0].pos = D3DXVECTOR3(g_aParticle[nCntParticle].pos.x - g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].pos.y - g_aParticle[nCntParticle].fRadius, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aParticle[nCntParticle].pos.x + g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].pos.y - g_aParticle[nCntParticle].fRadius, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aParticle[nCntParticle].pos.x - g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].pos.y + g_aParticle[nCntParticle].fRadius, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aParticle[nCntParticle].pos.x + g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].pos.y + g_aParticle[nCntParticle].fRadius, 0.0f);


			pVtx[0].col = g_aParticle[nCntParticle].col;
			pVtx[1].col = g_aParticle[nCntParticle].col;
			pVtx[2].col = g_aParticle[nCntParticle].col;
			pVtx[3].col = g_aParticle[nCntParticle].col;


		
			/*if (g_aParticle[nCntParticle].nScaleType == ParticleSCALE_NORMAL)
			{
				g_aParticle[nCntParticle].fRadius -= 0.5f;
				pVtx[0].pos = D3DXVECTOR3(g_aParticle[nCntParticle].pos.x - g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].pos.y - g_aParticle[nCntParticle].fRadius, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aParticle[nCntParticle].pos.x + g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].pos.y - g_aParticle[nCntParticle].fRadius, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aParticle[nCntParticle].pos.x - g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].pos.y + g_aParticle[nCntParticle].fRadius, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aParticle[nCntParticle].pos.x + g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].pos.y + g_aParticle[nCntParticle].fRadius, 0.0f);
			}
			else if (g_aParticle[nCntParticle].nScaleType == ParticleSCALE_FAST)
			{
				g_aParticle[nCntParticle].fRadius -= 3.0f;
				pVtx[0].pos = D3DXVECTOR3(g_aParticle[nCntParticle].pos.x - g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].pos.y - g_aParticle[nCntParticle].fRadius, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aParticle[nCntParticle].pos.x + g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].pos.y - g_aParticle[nCntParticle].fRadius, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aParticle[nCntParticle].pos.x - g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].pos.y + g_aParticle[nCntParticle].fRadius, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aParticle[nCntParticle].pos.x + g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].pos.y + g_aParticle[nCntParticle].fRadius, 0.0f);
			}*/



			if (g_aParticle[nCntParticle].nLife <= 0)
			{//�p�[�e�B�N���̎������s����
				g_aParticle[nCntParticle].bUse = false;
			}
			if (g_aParticle[nCntParticle].fRadius <= 0.0f)
			{
				g_aParticle[nCntParticle].bUse = false;
			}


		}
		pVtx += 4;//�|�C���^�����̒l��
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffParticle->Unlock();
}

//�e�̕`�揈��
void DrawParticle(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�ւ̃|�C���^

	int nCntParticle;

	Enemy* pEnemy = GetEnemy();
	Player* pPlayer = GetPlayer();

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffParticle, 0, sizeof(VERTEX_2D));//���_�o�b�t�@�ւ̃|�C���^�ƒ��_���̍\���̂̃T�C�Y

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//a�u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//�e�N�X�`����S�Ẵ|���S���Ɂ������蓖�Ă遄��
	for (nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].bUse == true)
		{

			pDevice->SetTexture(0, g_pTextureParticle[0]);

			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,//0�́A�`�悷��ŏ��̒��_�C���f�b�N�X�ŁA2�́A�`�悷��v���~�e�B�u��
				nCntParticle * 4,
				2);
		}

	}
	//a�u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//�G�t�F�N�g�̐ݒ菈��
void SetParticle(D3DXVECTOR3 pos, D3DXCOLOR col, float fRadius, int nLife)
{
	int nCntParticle;

	VERTEX_2D* pVtx;
	g_pVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].bUse == false)
		{//�e���g�p����Ă��Ȃ�
		/*	if (BULLETTYPE_ENEMY || BULLETTYPE_PLAYER || BULLETTYPE_SKILL1||BULLETTYPE_REAFY)
			{*/

			g_aParticle[nCntParticle].pos = pos;
			/*g_aParticle[nCntParticle].nScaleType = nType;*/
			g_aParticle[nCntParticle].col = col;
			g_aParticle[nCntParticle].fRadius = fRadius;
			g_aParticle[nCntParticle].nLife = nLife;
			g_aParticle[nCntParticle].bUse = true;//�g�p���Ă����Ԃɂ���
		
			float fRandumRot;
			float fRandumSpeed;
			fRandumRot = float((rand)() % 629 - 314) / 100.0f;
			fRandumSpeed = float((rand)() % 100) / 5.0f;

			 g_aParticle[nCntParticle].fRot = fRandumRot;
			 g_aParticle[nCntParticle].fSpeed = fRandumSpeed;

			g_aParticle[nCntParticle].move.x = sinf(g_aParticle[nCntParticle].fRot) * fRandumSpeed;
			g_aParticle[nCntParticle].move.y = cosf(g_aParticle[nCntParticle].fRot) * fRandumSpeed;
			
						//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aParticle[nCntParticle].pos.x - g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].pos.y - g_aParticle[nCntParticle].fRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aParticle[nCntParticle].pos.x + g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].pos.y - g_aParticle[nCntParticle].fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aParticle[nCntParticle].pos.x - g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].pos.y + g_aParticle[nCntParticle].fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aParticle[nCntParticle].pos.x + g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].pos.y + g_aParticle[nCntParticle].fRadius, 0.0f);

			pVtx[0].col = g_aParticle[nCntParticle].col;//D3DXCOLOR�́A0.0f�`1.0f�̊Ԃ̐��l�ŐF��ς���B
			pVtx[1].col = g_aParticle[nCntParticle].col;//D3DXCOLOR�́A0.0f�`1.0f�̊Ԃ̐��l�ŐF��ς���B
			pVtx[2].col = g_aParticle[nCntParticle].col;//D3DXCOLOR�́A0.0f�`1.0f�̊Ԃ̐��l�ŐF��ς���B
			pVtx[3].col = g_aParticle[nCntParticle].col;//D3DXCOLOR�́A0.0f�`1.0f�̊Ԃ̐��l�ŐF��ς���B
			break;

		}
		pVtx += 4;//�|�C���^���S���i�߂�
	}
	g_pVtxBuffParticle->Unlock();
}
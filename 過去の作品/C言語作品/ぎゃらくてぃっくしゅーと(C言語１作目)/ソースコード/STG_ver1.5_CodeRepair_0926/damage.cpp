//========================================================
//
//�W���R�O���F�G�t�F�N�g������񌩒���[score.cpp]
//Author:ShinaTaiyo
//
//=========================================================

#include "main.h"
#include "damage.h"
#include "input.h"
#include "explosion.h"
#include "enemy.h"
#include "player.h"
#include "health.h"
#include "result.h"

#define MAX_DAMAGE (1024)//�_���[�W�\�L�̍ő吔
#define DAMAGE_DIGIT (3) //��񓖂���̃_���[�W�̍ő包��
#define DAMAGE_SCALE_X (10)
#define DAMAGE_SCALE_Y (5)
#define DAMAGE_SCALE (100)

#define DAMAGE_RADIUS (10.0f)


//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureDamage= NULL;//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffDamage = NULL;//�e�̏��
Damage g_aDamage[MAX_DAMAGE][DAMAGE_DIGIT];
//float g_fXMove;//X���W�̈ړ�
//float g_fYMove;//Y���W�̈ړ�
D3DXVECTOR3 g_posDamage;//�ʒu
D3DXVECTOR3 g_moveDamage;//�ړ���

//D3DXVECTOR3 g_rotPl;//����
float g_fDAMAGE_SCALE=0;





//�e�̏���������
void InitDamage(void)
{
	ResultType* pResultType = GetResultType();
		

	/*if (pResultType->nResultType == false)
	{*/
	
	//}
	//g_moveDamage = D3DXVECTOR3(0.0f, -10.0f, 0.0f);//�ړ��ʂ�����������

	LPDIRECT3DDEVICE9 pDevice;
	int	nCntDamage;
	int nCntDigit;
	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	        D3DXCreateTextureFromFile(pDevice,
			"data\\number_001.png",
			&g_pTextureDamage);
	//�e�N�X�`���̓ǂݍ��݁i�X�R�A�̃e�N�X�`����ǂݍ��ށj
	

	/*D3DXCreateTextureFromFile(pDevice,
		"data\\lightbrueball_enemy.png",
		&g_pTextureDamage[0]);*/


	/*g_nScore = 0;*/

	//�X�R�A���̏�����
	for (nCntDamage = 0; nCntDamage < MAX_DAMAGE; nCntDamage++)
	{
		for (nCntDigit = 0; nCntDigit < DAMAGE_DIGIT; nCntDigit++)
		{
			g_aDamage[nCntDamage][nCntDigit].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
			g_aDamage[nCntDamage][nCntDigit].bUse = false;//�g�p���Ă��Ȃ���Ԃɂ���

			g_aDamage[nCntDamage][nCntDigit].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aDamage[nCntDamage][nCntDigit].nDamagePosTexU = 0;
			g_aDamage[nCntDamage][nCntDigit].nLife = 0;
			g_aDamage[nCntDamage][nCntDigit].fDamageWrite = 0.0f;
		}
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_DAMAGE * DAMAGE_DIGIT,//�S�͕K�v�Ȓ��_��
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffDamage,
		NULL);

	VERTEX_2D* pVtx;

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffDamage->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntDamage = 0; nCntDamage < MAX_DAMAGE * DAMAGE_DIGIT; nCntDamage++)
	{
		//���_���W�̐ݒ�
	
		
			pVtx[0].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
			pVtx[1].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
			pVtx[2].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
			pVtx[3].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);

			/*	g_aDamage[nCntDamage].nLife = 1000;*/
		
			/*g_rotPl[nCntDamage] = 200;*/


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

		

			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);
		

			pVtx += 4;//���_�f�[�^�̃|�C���^���S���i�߂�B
		
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffDamage->Unlock();

	/*for (nCntDamage = 0; nCntDamage < MAX_DAMAGE; nCntDamage++)
	{
		SetScore(10);
	}*/
	/*g_mode = GetMode();*/

	/*if (g_mode == MODE_RESULT)
	{*/
	/*	g_nScore = g_nSaveScore;*/
	//}

	
	
}

//�e�̏I������
void UninitDamage(void)
{
	int nCntDamage;
	//�e�N�X�`���̔j��
	//for (nCntDamage = 0; nCntDamage < MAX_DAMAGE; nCntDamage++)
	//{
		if (g_pTextureDamage != NULL)
		{
			g_pTextureDamage->Release();
			g_pTextureDamage = NULL;
		}
	//}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffDamage != NULL)
	{
		g_pVtxBuffDamage->Release();
		g_pVtxBuffDamage = NULL;
	}
}

//�e�̍X�V����
void UpdateDamage(void)
{
	int nCntDamage;//�_���[�W�\�L�̐����J�E���g����
	int nCntDigit;//�_���[�W�̍ő包�����J�E���g����

	VERTEX_2D* pVtx;

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffDamage->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntDamage = 0; nCntDamage < MAX_DAMAGE; nCntDamage++)
	{
		//���_���W�̐ݒ�

		for (nCntDigit = 0; nCntDigit < DAMAGE_DIGIT; nCntDigit++)
		{
			if (g_aDamage[nCntDamage][nCntDigit].bUse == true)
			{

				g_aDamage[nCntDamage][nCntDigit].nLife-=1;//�_���[�W�\�����Ԃ����炵������
				g_aDamage[nCntDamage][nCntDigit].a_col -= 0.01f;//�A���t�@�l��ς������A�ǂ�ǂ�_���[�W�\�L�𓧖��ɂ��Ă�������

					pVtx[0].pos = D3DXVECTOR3(g_aDamage[nCntDamage][nCntDigit].pos.x - g_aDamage[nCntDamage][nCntDigit].fRadius - g_aDamage[nCntDamage][nCntDigit].fRadius * nCntDigit, g_aDamage[nCntDamage][nCntDigit].pos.y - g_aDamage[nCntDamage][nCntDigit].fRadius, 0.0f);
					pVtx[1].pos = D3DXVECTOR3(g_aDamage[nCntDamage][nCntDigit].pos.x + g_aDamage[nCntDamage][nCntDigit].fRadius - g_aDamage[nCntDamage][nCntDigit].fRadius * nCntDigit, g_aDamage[nCntDamage][nCntDigit].pos.y - g_aDamage[nCntDamage][nCntDigit].fRadius, 0.0f);
					pVtx[2].pos = D3DXVECTOR3(g_aDamage[nCntDamage][nCntDigit].pos.x - g_aDamage[nCntDamage][nCntDigit].fRadius - g_aDamage[nCntDamage][nCntDigit].fRadius * nCntDigit, g_aDamage[nCntDamage][nCntDigit].pos.y + g_aDamage[nCntDamage][nCntDigit].fRadius, 0.0f);
					pVtx[3].pos = D3DXVECTOR3(g_aDamage[nCntDamage][nCntDigit].pos.x + g_aDamage[nCntDamage][nCntDigit].fRadius - g_aDamage[nCntDamage][nCntDigit].fRadius * nCntDigit, g_aDamage[nCntDamage][nCntDigit].pos.y + g_aDamage[nCntDamage][nCntDigit].fRadius, 0.0f);



					//rhw�̐ݒ�
				pVtx[0].rhw = 1.0f;
				pVtx[1].rhw = 1.0f;
				pVtx[2].rhw = 1.0f;
				pVtx[3].rhw = 1.0f;



				//���_�J���[�̐ݒ�
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aDamage[nCntDamage][nCntDigit].a_col);//D3DXCOLOR�́A0.0f�`1.0f�̊Ԃ̐��l�ŐF��ς���B
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aDamage[nCntDamage][nCntDigit].a_col);//D3DXCOLOR�́A0.0f�`1.0f�̊Ԃ̐��l�ŐF��ς���B
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aDamage[nCntDamage][nCntDigit].a_col);//D3DXCOLOR�́A0.0f�`1.0f�̊Ԃ̐��l�ŐF��ς���B
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aDamage[nCntDamage][nCntDigit].a_col);//D3DXCOLOR�́A0.0f�`1.0f�̊Ԃ̐��l�ŐF��ς���B


				//�_���[�W�̓���
				g_aDamage[nCntDamage][nCntDigit].pos.x += g_aDamage[nCntDamage][nCntDigit].move.x;
				g_aDamage[nCntDamage][nCntDigit].pos.y += g_aDamage[nCntDamage][nCntDigit].move.y;

				if (g_aDamage[nCntDamage][nCntDigit].nLife < 1)
				{
					g_aDamage[nCntDamage][nCntDigit].bUse = false;
				}
			}
			pVtx += 4;//���_�f�[�^�̃|�C���^���S���i�߂�B
		}
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffDamage->Unlock();
}

//�e�̕`�揈��
void DrawDamage(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�ւ̃|�C���^

	int nCntDamage,nCntDigit;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffDamage, 0, sizeof(VERTEX_2D));//���_�o�b�t�@�ւ̃|�C���^�ƒ��_���̍\���̂̃T�C�Y

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);



	for (nCntDamage = 0; nCntDamage < MAX_DAMAGE; nCntDamage++)
	{
		for (nCntDigit = 0; nCntDigit < DAMAGE_DIGIT; nCntDigit++)
		{

			if (g_aDamage[nCntDamage][nCntDigit].bUse == true)
			{
				//�e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_pTextureDamage);

				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,//0�́A�`�悷��ŏ��̒��_�C���f�b�N�X�ŁA2�́A�`�悷��v���~�e�B�u��
					nCntDigit * 4 + ( DAMAGE_DIGIT * 4 ) * nCntDamage,
					2);
			}
		}

	}


}

//�X�R�A�̐ݒ菈��
void SetDamage(int nDamage,int nLife,D3DXVECTOR3 move,D3DXVECTOR3 pos, float fRadius)
{

	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D* pVtx;

	int nCntDamage;
	int nCntDigit;

	int nGetDamage = nDamage;
	


	int nCnt;
	int nCntDamageDigit=0;//�����_���[�W�\�L���g�p����Ă��Ȃ�������A�_���[�W�\�L�̌������̃J�E���g���J�n����B
	//�f�o�C�X�̎擾
	pDevice = GetDevice();
	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffDamage->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntDamage = 0; nCntDamage < MAX_DAMAGE; nCntDamage++)
	{
		
		for (nCntDigit = 0; nCntDigit < DAMAGE_DIGIT; nCntDigit++)
		{
			if (g_aDamage[nCntDamage][nCntDigit].bUse == false)
			{
				
				g_aDamage[nCntDamage][nCntDigit].bUse = true;
				g_aDamage[nCntDamage][nCntDigit].nLife = nLife;
				g_aDamage[nCntDamage][nCntDigit].move = move;
				g_aDamage[nCntDamage][nCntDigit].pos = pos;
				g_aDamage[nCntDamage][nCntDigit].fRadius = fRadius;
				g_aDamage[nCntDamage][nCntDigit].a_col = 0.8f;

				/*	for (nCnt = 0; nCnt < DAMAGE_DIGIT; nCnt++) 
					{*/
	          
				g_aDamage[nCntDamage][nCntDigit].nDamagePosTexU = nGetDamage % 10;
				nGetDamage = nGetDamage / 10;
				//}



			/*	for (nCntDigit= 0;  nCntDigit< DAMAGE_DIGIT; nCntDigit++)
				{*/
				pVtx[0].tex = D3DXVECTOR2(0.0f + (0.1f * g_aDamage[nCntDamage][nCntDigit].nDamagePosTexU), 0.0f);
				pVtx[1].tex = D3DXVECTOR2(0.1f + (0.1f * g_aDamage[nCntDamage][nCntDigit].nDamagePosTexU), 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f + (0.1f * g_aDamage[nCntDamage][nCntDigit].nDamagePosTexU), 1.0f);
				pVtx[3].tex = D3DXVECTOR2(0.1f + (0.1f * g_aDamage[nCntDamage][nCntDigit].nDamagePosTexU), 1.0f);

				nCntDamageDigit++;
			}
			if (nCntDamageDigit >= 3)
			{
				break;//for���̏I��
			}
			pVtx += 4;//���_�f�[�^�̃|�C���^���S���i�߂�B
			
		}
		if (nCntDamageDigit >= 3)
		{
			break;//for���̏I��
		}
		
	
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffDamage->Unlock();

	/*for (nCntDamage = 0; nCntDamage < MAX_DAMAGE; nCntDamage++)
	{
		SetScore(10);
	}*/
}



//===================================
//�X�R�A�̎擾
//===================================
//int GetDamage(void)
//{
//	return g_nScore;
//}
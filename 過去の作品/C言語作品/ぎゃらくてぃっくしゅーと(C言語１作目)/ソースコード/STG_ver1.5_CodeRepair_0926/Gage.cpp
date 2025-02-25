//========================================================
//
//�W���R�O���F�G�t�F�N�g������񌩒���[Gage.cpp]
//Author:ShinaTaiyo
//
//=========================================================
#include "main.h"
#include "game.h"
#include "enemy.h"
#include "Gage.h"
#include "input.h"
#include "player.h"
#include "sound.h"
//�}�N����`
#define NUM_GAGE (2)
#define MAX_GAGE (10)
#define HPGAGE_LENGTH (1000.0f)
//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_apTextureGage[NUM_GAGE] = {};//�e�N�X�`��3�����̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGage = NULL;//���_�o�b�t�@�ւ̃|�C���^

Gage g_aGage[MAX_GAGE];

//float g_fGage = 0;
//int g_nGage_Count = 0;
//Result_2 g_aResult_2;
//============================
//�̗͂̏���������
//============================
void InitGage(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntGage;

	/*g_nCountBoss1 = 0;*/

	/*g_aResult_2.bResultFlag2 = false;*/

	//g_bResultFlag2 = false;

	/* g_fGage = 0;
	 g_nGage_Count = 0;*/

	for (nCntGage = 0; nCntGage < MAX_GAGE; nCntGage++)
	{
		g_aGage[nCntGage].nGageCount = 0;
		g_aGage[nCntGage].fGage = 0;
		g_aGage[nCntGage].bUse = false;
		g_aGage[nCntGage].nType = nCntGage;
		g_aGage[nCntGage].fBoss1MaxHp = 0.0f;
		g_aGage[nCntGage].nCountBoss1 = 0;
	}



	/*g_aGage[0].bUse = true;
	g_aGage[0].nType = GAGETYPE_SKILL;*/
	/*g_fAnimationPattern = 0;*/

	//�f�o�C�X�̎擾
	pDevice = GetDevice();


	//D3DXCreateTextureFromFile(pDevice,
	//	"data\\Skill_Gage.png",
	//	&g_apTextureGage[0]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\BossHp_000.png",
		&g_apTextureGage[0]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\BossHp_frame_000.png",
		&g_apTextureGage[1]);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4*MAX_GAGE,//�S�͕K�v�Ȓ��_��
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffGage,
		NULL);

	VERTEX_2D* pVtx;
	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffGage->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntGage = 0; nCntGage < MAX_GAGE; nCntGage++)
	{
		pVtx[0].pos = D3DXVECTOR3(0.0f, 670.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 670.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);

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
	g_pVtxBuffGage->Unlock();
}

//==========================
//�̗͂̏I������
//==========================
void UninitGage(void)
{
	//�e�N�X�`���̔j��
	int nCntGage;
	
	for (nCntGage = 0; nCntGage < NUM_GAGE; nCntGage++)
	{
		if (g_apTextureGage[nCntGage] != NULL)
		{
			g_apTextureGage[nCntGage]->Release();
			g_apTextureGage[nCntGage] = NULL;
		}
	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffGage != NULL)
	{
		g_pVtxBuffGage->Release();
		g_pVtxBuffGage = NULL;
	}
}

//==================================
//�̗͂̍X�V����
//==================================
void UpdateGage(void)
{
	int nCntGage;
	int nCntEnemy;
	Player* pPlayer = GetPlayer();//�v���C���[�\���̂̏����Q�b�g����B
	Enemy* pEnemy = GetEnemy();//�G�l�~�[�\���̂̏����Q�b�g����B

	//g_aGage[0].nGageCount++;
	VERTEX_2D* pVtx;
	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffGage->Lock(0, 0, (void**)&pVtx, 0);



	for (nCntGage = 0; nCntGage < MAX_GAGE; nCntGage++)
	{



		if (g_aGage[nCntGage].bUse == true)
		{



			g_aGage[nCntGage].nCountBoss1++;



			/*	g_aGage[nCntGage].bUse = true;*/
			switch (g_aGage[nCntGage].nType)
			{
			case GAGETYPE_BOSSHP_FRAME:

			/*	if (g_aGage[nCntGage].bUse == true)
				{*/
					pVtx[0].pos = D3DXVECTOR3(0.0f, 15.0f, 0.0f);
					pVtx[1].pos = D3DXVECTOR3(HPGAGE_LENGTH, 15.0f, 0.0f);
					pVtx[2].pos = D3DXVECTOR3(0.0f, 65.0f, 0.0f);
					pVtx[3].pos = D3DXVECTOR3(HPGAGE_LENGTH, 65.0f, 0.0f);
				
					break;
			case GAGETYPE_BOSS1HP:
				for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
				{
					if (pEnemy->bUse == false && g_aGage[nCntGage].nSubjectEnemy == nCntEnemy)  //���̏���
					{
						/*if (g_aGage[nCntEnemy].nSubjectEnemy == nCntEnemy)
						{*/
						g_aGage[nCntGage].nCountBoss1 = 0;



							//�{�X�P���|���ꂽ��E�E�E
							StopSound(SOUND_LABEL_BGM003);
							for (nCntGage = 0; nCntGage < MAX_GAGE; nCntGage++)
							{//�S�ẴQ�[�W��false�ɂ���B
								g_aGage[nCntGage].bUse = false;
							}
					//	}
					}
					//if (pEnemy->bUse == false && g_aGage[nCntGage].nSubjectEnemy == nCntEnemy && pEnemy->nType == ENEMYTYPE_BOSS1_STARWARRIOR)  //���̏���
					//{//�{�X�P���|���ꂽ��E�E�E
					//	StopSound(SOUND_LABEL_BGM003);
					//	for (nCntGage = 0; nCntGage < MAX_GAGE; nCntGage++)
					//	{//�S�ẴQ�[�W��false�ɂ���B
					//		g_aGage[nCntGage].bUse = false;
					//	}
					//}

				/*	else if (g_aGage[nCntGage].bUse == true)
					{*/
						if (pEnemy->bUse == true && g_aGage[nCntGage].nSubjectEnemy == nCntEnemy)
						{
							if (g_aGage[nCntGage].nCountBoss1 == 1)
							{
								g_aGage[nCntGage].fBoss1MaxHp = pEnemy->fLife;
							}


							pVtx[0].pos = D3DXVECTOR3(0.0f, 20.0f, 0.0f);
							pVtx[1].pos = D3DXVECTOR3(0.0f + (HPGAGE_LENGTH / g_aGage[nCntGage].fBoss1MaxHp) * pEnemy->fLife, 20.0f, 0.0f);
							pVtx[2].pos = D3DXVECTOR3(0.0f, 60.0f, 0.0f);
							pVtx[3].pos = D3DXVECTOR3(0.0f + (HPGAGE_LENGTH / g_aGage[nCntGage].fBoss1MaxHp) * pEnemy->fLife, 60.0f, 0.0f);


							if (pEnemy->fLife <= g_aGage[nCntGage].fBoss1MaxHp * 0.25f)
							{
								pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);//D3DXCOLOR�́A0.0f�`1.0f�̊Ԃ̐��l�ŐF��ς���B
								pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);//D3DXCOLOR�́A0.0f�`1.0f�̊Ԃ̐��l�ŐF��ς���B
								pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);//D3DXCOLOR�́A0.0f�`1.0f�̊Ԃ̐��l�ŐF��ς���B
								pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);//D3DXCOLOR�́A0.0f�`1.0f�̊Ԃ̐��l�ŐF��ς���B
							}
							else if (pEnemy->fLife <= g_aGage[nCntGage].fBoss1MaxHp * 0.5f)
							{
								pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);//D3DXCOLOR�́A0.0f�`1.0f�̊Ԃ̐��l�ŐF��ς���B
								pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);//D3DXCOLOR�́A0.0f�`1.0f�̊Ԃ̐��l�ŐF��ς���B
								pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);//D3DXCOLOR�́A0.0f�`1.0f�̊Ԃ̐��l�ŐF��ς���B
								pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);//D3DXCOLOR�́A0.0f�`1.0f�̊Ԃ̐��l�ŐF��ς���B
							}

						}
				//	}
				
				}
				break;


			}
		
			

		}
		pVtx += 4;
	}

	
	


	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffGage->Unlock();
}

//==================================
//�̗͂̕`�揈��
//==================================
void DrawGage(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntGage;
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffGage, 0, sizeof(VERTEX_2D));//���_�o�b�t�@�ւ̃|�C���^�ƒ��_���̍\���̂̃T�C�Y

		//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntGage = 0; nCntGage < MAX_GAGE; nCntGage++)
	{
		if (g_aGage[nCntGage].bUse == true)
		{
			//�e�N�X�`���̐ݒ�
			if (g_aGage[nCntGage].nType == GAGETYPE_BOSS1HP)
			{
				pDevice->SetTexture(0, g_apTextureGage[0]);
			}
			if (g_aGage[nCntGage].nType == GAGETYPE_BOSSHP_FRAME)
			{
				pDevice->SetTexture(0, g_apTextureGage[1]);
			}

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
				nCntGage * 4,
				2);
		}
	}

}


void SetGage(int nCntEnemy, int nType)
{
	int nCntGage;

	VERTEX_2D* pVtx;
	g_pVtxBuffGage->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntGage = 0; nCntGage < MAX_GAGE; nCntGage++)
	{

		if (g_aGage[nCntGage].bUse == false)
		{//�e���g�p����Ă��Ȃ�
		

			g_aGage[nCntGage].bUse = true;//�g�p���Ă����Ԃɂ���
			g_aGage[nCntGage].nType = nType;//�e�̎�ށitype)��������B
			g_aGage[nCntGage].nSubjectEnemy = nCntEnemy;//�ǂ̓G��HP��\������Ώۂ�
			g_aGage[nCntGage].fGage = 0.0f;
			g_aGage[nCntGage].nCountBoss1 = 0;


		

			break;
			//}
		}
		pVtx += 4;//�|�C���^���S���i�߂�
	}
	g_pVtxBuffGage->Unlock();
}


Gage* GetGage(void)
{
	return&g_aGage[0];
}


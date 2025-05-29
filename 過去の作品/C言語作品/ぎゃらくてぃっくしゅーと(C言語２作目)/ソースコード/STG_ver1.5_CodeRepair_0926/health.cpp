//========================================================
//
//�W���R�O���F�G�t�F�N�g������񌩒���[health.cpp]
//Author:ShinaTaiyo
//
//=========================================================
#include "main.h"
#include "health.h"
#include "game.h"
#include "enemy.h"
#include "result.h"
#include "sound.h"
#include "staging.h"
#include "input.h"
#include "player.h"

#define HEALTH_WIDTH (100.0f)
#define HEALTH_HEIGHT (50.0f)

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_apTextureHealth = NULL;//�e�N�X�`��3�����̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffHealth = NULL;//���_�o�b�t�@�ւ̃|�C���^

float g_fAnimationCount=0;
bool g_bResultFlag2;
bool g_bWarning;//�c��HP��1�ȉ��ɂȂ�ƌx�����o���t���O�B
Health g_aHealth;
Result_2 g_aResult_2;
//============================
//�̗͂̏���������
//============================
void InitHealth(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntBG;

	g_bWarning = false;

	g_aResult_2.bResultFlag2 = false;

	g_bResultFlag2 = false;

	g_aHealth.fHealth = -2;

	g_aHealth.pos = D3DXVECTOR3(1100.0f, 670.0f, 0.0f);

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	D3DXCreateTextureFromFile(pDevice,
		"data\\re_player_health.png",
		&g_apTextureHealth);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,//�S�͕K�v�Ȓ��_��
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffHealth,
		NULL);

	VERTEX_2D* pVtx;
	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffHealth->Lock(0, 0, (void**)&pVtx, 0);
		pVtx[0].pos = D3DXVECTOR3(g_aHealth.pos.x-HEALTH_WIDTH,g_aHealth.pos.y-HEALTH_HEIGHT, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aHealth.pos.x+HEALTH_WIDTH,g_aHealth.pos.y-HEALTH_HEIGHT, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aHealth.pos.x-HEALTH_WIDTH,g_aHealth.pos.y+HEALTH_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aHealth.pos.x+HEALTH_WIDTH,g_aHealth.pos.y+HEALTH_HEIGHT, 0.0f);

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
		pVtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f,0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f,1.0f);

		pVtx += 4;//���_�f�[�^�̃|�C���^���S���i�߂�B

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffHealth->Unlock();
}

//==========================
//�̗͂̏I������
//==========================
void UninitHealth(void)
{
	//�e�N�X�`���̔j��

		if (g_apTextureHealth!= NULL)
		{
			g_apTextureHealth->Release();
			g_apTextureHealth= NULL;
		}
	
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffHealth != NULL)
	{
		g_pVtxBuffHealth->Release();
		g_pVtxBuffHealth = NULL;
	}
}

//==================================
//�̗͂̍X�V����
//==================================
void UpdateHealth(float fHealthDamage)
{

	ResultType* pResultType = GetResultType();
	Player* pPlayer = GetPlayer();


	int nCntBG;



	VERTEX_2D* pVtx;
	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffHealth->Lock(0, 0, (void**)&pVtx, 0);


	if (GetKeyboardTrigger(DIK_Z) == true)
	{
		PlaySound(SOUND_LABEL_SE_DAMAGE_000);
		g_aHealth.fHealth += 1.0f;
	}

	if (g_aHealth.fHealth == 2.0f&&g_bWarning==true)
	{
		g_bWarning = false;
		PlaySound(SOUND_LABEL_SE_WARNING_000);
		SetStaging(STAGINGTYPE_WARNING, -1, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}
	else if (g_aHealth.fHealth <= 1.0f)
	{
		g_bWarning = true;
	}

	/*for (nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
	{*/
		//�e�N�X�`�����W�̐ݒ�

		/*g_aPosTexV[nCntBG] -= 0.0025f;*/
	//g_aHealth.fHealth += fHealthDamage;//�v���C���[���_���[�W���󂯂��Ƃ��A�̗͂����炷�B(�_���[�W������player.cpp�ł��j

		pVtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f-((1.0f/3.0f)*g_aHealth.fHealth),0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f - ((1.0f / 3.0f) * g_aHealth.fHealth),1.0f);

		/*pVtx += 4;*/
		
	
			if (g_aHealth.fHealth >= 3.0f)//�R��ȏ�v���C���[����e�����ꍇ
			{
				g_aResult_2.bResultFlag2 = true;

				pResultType->nResultType = RESULTTYPE_CLEAR;//�Q�[���I�[�o�[�悤�̃��U���g��ʂ��ĂԁB

				StopSound(SOUND_LABEL_BGM001);

				SetGameState(GAMESTATE_END);
			}

			if (pPlayer->pos.x >= g_aHealth.pos.x - HEALTH_WIDTH && pPlayer->pos.x <= g_aHealth.pos.x + HEALTH_WIDTH
				&& pPlayer->pos.y >= g_aHealth.pos.y - HEALTH_HEIGHT && pPlayer->pos.y <= g_aHealth.pos.y + HEALTH_HEIGHT)
			{
				//���_�J���[�̐ݒ�
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);//D3DXCOLOR�́A0.0f�`1.0f�̊Ԃ̐��l�ŐF��ς���B
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);//D3DXCOLOR�́A0.0f�`1.0f�̊Ԃ̐��l�ŐF��ς���B
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);//D3DXCOLOR�́A0.0f�`1.0f�̊Ԃ̐��l�ŐF��ς���B
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);//D3DXCOLOR�́A0.0f�`1.0f�̊Ԃ̐��l�ŐF��ς���B
			}
			else
			{

				//���_�J���[�̐ݒ�
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLOR�́A0.0f�`1.0f�̊Ԃ̐��l�ŐF��ς���B
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLOR�́A0.0f�`1.0f�̊Ԃ̐��l�ŐF��ς���B
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLOR�́A0.0f�`1.0f�̊Ԃ̐��l�ŐF��ς���B
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLOR�́A0.0f�`1.0f�̊Ԃ̐��l�ŐF��ς���B
			}
		
	/*}*/
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffHealth->Unlock();
}

//==================================
//�̗͂̕`�揈��
//==================================
void DrawHealth(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntBG;
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffHealth, 0, sizeof(VERTEX_2D));//���_�o�b�t�@�ւ̃|�C���^�ƒ��_���̍\���̂̃T�C�Y

		//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_apTextureHealth);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
			0,
			2);
	
}

//======================================
//���U���g��ʂ��Ăяo���t���O
//======================================
Result_2* GetResult_2(void)
{
	return &g_aResult_2;
}

Health* GetHealth(void)
{
	return &g_aHealth;
}
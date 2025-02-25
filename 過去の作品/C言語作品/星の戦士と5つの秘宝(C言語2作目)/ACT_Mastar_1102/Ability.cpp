//========================================================
//
//�W���R�O���F�G�t�F�N�g������񌩒���[Ability.cpp]
//Author:ShinaTaiyo
//
//=========================================================

#include "main.h"
#include "Ability.h"
#include "input.h"
#include "player.h"
#include "Ability.h"
#include "result.h"
#include "sound.h"

#define MAX_ABILITY (3) //�A�r���e�B�̍ő包��
#define NUM_ABILITY (2) //�A�r���e�B�\���̎��

#define ABILITY_WIDTH (15.0f) //�A�r���e�B�̉���
#define ABILITY_HEIGHT (30.0f)//�A�r���e�B�̍���

#define ABILITY_RADIUS (40.0f) //�A�r���e�B�\���̐��l�̔��a�i�傫���j

#define ABILITY_UI_RADIUS (40.0f)//�A�r���e�BUI�̔��a
#define ABILITY_UI_RADIUS_X (100.0f)//�A�r���e�BUI�̉���
#define ABILITY_UI_RADIUS_Y (50.0f)//�A�r���e�BUI�̍���



//�O���[�o���ϐ�

//================================================================================
//�A�r���e�B��UI����
//================================================================================
LPDIRECT3DTEXTURE9 g_pTextureUIAbility = NULL;//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUIAbility = NULL;
D3DXVECTOR3 g_posUIAbility;//�ʒu
D3DXVECTOR3 g_moveUIAbility;//�ړ���

//=======================================================================

//�A�r���e�B�\������============================================================
LPDIRECT3DTEXTURE9 g_pTextureAbility[NUM_ABILITY] = {};//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffAbility = NULL;
Ability g_aAbility;
D3DXVECTOR3 g_posAbility;//�ʒu
D3DXVECTOR3 g_moveAbility;//�ړ���

int g_nAbility = 0;
float g_fHESLTHSCALE = 0;

int g_nSaveAbility = 0;


int g_nMaxAbility;//�ő�A�r���e�B�ۑ��p
//=========================================================================
//�A�r���e�B�̏���������
void InitAbility(void)
{
	ResultType* pResultType = GetResultType();
	g_nAbility = 100;//���U���g���[�h�ȊO�Ȃ�A�X�R�A������������
	LPDIRECT3DDEVICE9 pDevice;
	int	nCntAbility;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();


	//=======================================
	//�A�r���e�BUI����
	//=======================================


	g_posUIAbility = D3DXVECTOR3(300.0f, 50.0f, 0.0f);//UI�\���ꏊ

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Ability_DisPlay_ver1.0.png",
		&g_pTextureUIAbility);
	//�e�N�X�`���̓ǂݍ��݁i�X�R�A�̃e�N�X�`����ǂݍ��ށj

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,//�S�͕K�v�Ȓ��_��
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffUIAbility,
		NULL);

	VERTEX_2D* pVtx;

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffUIAbility->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_posUIAbility.x - ABILITY_UI_RADIUS, g_posUIAbility.y - ABILITY_UI_RADIUS, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_posUIAbility.x + ABILITY_UI_RADIUS, g_posUIAbility.y - ABILITY_UI_RADIUS, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_posUIAbility.x - ABILITY_UI_RADIUS, g_posUIAbility.y + ABILITY_UI_RADIUS, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_posUIAbility.x + ABILITY_UI_RADIUS, g_posUIAbility.y + ABILITY_UI_RADIUS, 0.0f);

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

	//�e�N�X�`���̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffUIAbility->Unlock();
	//===============================================================================================================================

	//=================================
	//�A�r���e�B�\������
	//=================================
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\NormalAbility.png",
		&g_pTextureAbility[ABILITYTYPE_NORMAL]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Ability_Bullet.png",
		&g_pTextureAbility[ABILITYTYPE_BULLET]);

	    //�X�R�A���̏�����
	    g_aAbility.nAbilityType = 0;
		g_aAbility.pos = D3DXVECTOR3(300.0f,50.0f,0.0f);
		g_aAbility.bUse = false;//�g�p���Ă��Ȃ���Ԃɂ���

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,//�S�͕K�v�Ȓ��_��
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffAbility,
		NULL);

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffAbility->Lock(0, 0, (void**)&pVtx, 0);

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aAbility.pos.x - ABILITY_RADIUS, g_aAbility.pos.y - ABILITY_RADIUS, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aAbility.pos.x + ABILITY_RADIUS, g_aAbility.pos.y - ABILITY_RADIUS, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aAbility.pos.x - ABILITY_RADIUS, g_aAbility.pos.y + ABILITY_RADIUS, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aAbility.pos.x + ABILITY_RADIUS, g_aAbility.pos.y + ABILITY_RADIUS, 0.0f);

		g_aAbility.bUse = true;//�g�p���Ă����Ԃɂ���

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
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffAbility->Unlock();

	//====================================================================================================================


}

//�A�r���e�B�̏I������
void UninitAbility(void)
{
	int nCntAbility;

	//===================================
	//�A�r���e�BUI�̏I������
	//===================================

	//�e�N�X�`���̔j��
	if (g_pTextureUIAbility != NULL)
	{
		g_pTextureUIAbility->Release();
		g_pTextureUIAbility = NULL;
	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffUIAbility != NULL)
	{
		g_pVtxBuffUIAbility->Release();
		g_pVtxBuffUIAbility = NULL;
	}
	//=======================================================================

	//===================================
	//�A�r���e�B�\���̏I������
	//===================================

	//�e�N�X�`���̔j��
	for (int nCntAbility = 0; nCntAbility < NUM_ABILITY; nCntAbility++)
	{
		if (g_pTextureAbility[nCntAbility] != NULL)
		{
			g_pTextureAbility[nCntAbility]->Release();
			g_pTextureAbility[nCntAbility] = NULL;
		}
	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffAbility != NULL)
	{
		g_pVtxBuffAbility->Release();
		g_pVtxBuffAbility = NULL;
	}
	//========================================================================
}

//�A�r���e�B�̍X�V����
void UpdateAbility(void)
{
	int nCntAbility;
	VERTEX_2D* pVtx;
	Player* pPlayer = GetPlayer();

	//======================================
	//�A�r���e�BUI�̍X�V����
	//======================================

	

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffUIAbility->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_posUIAbility.x - ABILITY_UI_RADIUS, g_posUIAbility.y - ABILITY_UI_RADIUS, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_posUIAbility.x + ABILITY_UI_RADIUS, g_posUIAbility.y - ABILITY_UI_RADIUS, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_posUIAbility.x - ABILITY_UI_RADIUS, g_posUIAbility.y + ABILITY_UI_RADIUS, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_posUIAbility.x + ABILITY_UI_RADIUS, g_posUIAbility.y + ABILITY_UI_RADIUS, 0.0f);

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

	//�e�N�X�`���̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffUIAbility->Unlock();
	//====================================================================================================================



	//=================================================
	//�A�r���e�B�\������
	//=================================================
	//int nGetAbility;

	if (GetKeyboardTrigger(DIK_K) == true || GetJoyPadTrigger(JOYKEY_R1) == true || GetJoyPadTrigger(JOYKEY_L1) == true)
	{
		PlaySound(SOUND_LABEL_SE_SELECT_000);
		g_aAbility.nAbilityType++;
	}
	/*if (g_aAbility.nAbilityType == 1)
	{
		g_aAbility.nAbilityType = 0;
	}*/
	if (g_aAbility.nAbilityType == 2)
	{
		g_aAbility.nAbilityType = 0;
	}

	//nGetAbility = g_nAbility;//�X�R�A��ۑ�����

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffAbility->Lock(0, 0, (void**)&pVtx, 0);

		//���_�J���[�̐ݒ�
		if (pPlayer->pos.x >= g_aAbility.pos.x - ABILITY_RADIUS && pPlayer->pos.x <= g_aAbility.pos.x + ABILITY_RADIUS
			&& pPlayer->pos.y >= g_aAbility.pos.y - ABILITY_RADIUS && pPlayer->pos.y <= g_aAbility.pos.y + ABILITY_RADIUS)
		{
			//���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f);
		}
		else
		{
			//���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}


		pVtx += 4;//���_�f�[�^�̃|�C���^���S���i�߂�B
	

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffAbility->Unlock();
	//==========================================================================================================================================
}

//�A�r���e�B�̕`�揈��
void DrawAbility(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�ւ̃|�C���^


	//===============================================
	//�A�r���e�BUI�`�揈��
	//===============================================

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffUIAbility, 0, sizeof(VERTEX_2D));//���_�o�b�t�@�ւ̃|�C���^�ƒ��_���̍\���̂̃T�C�Y

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureUIAbility);

	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,//0�́A�`�悷��ŏ��̒��_�C���f�b�N�X�ŁA2�́A�`�悷��v���~�e�B�u��
		0,
		2);

	//=============================================
	//�A�r���e�B�\���̕`�揈��
	//=============================================
	

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffAbility, 0, sizeof(VERTEX_2D));//���_�o�b�t�@�ւ̃|�C���^�ƒ��_���̍\���̂̃T�C�Y

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	if (g_aAbility.nAbilityType == ABILITYTYPE_NORMAL)
	{
		pDevice->SetTexture(0, g_pTextureAbility[ABILITYTYPE_NORMAL]);
	}
	else if (g_aAbility.nAbilityType == ABILITYTYPE_BULLET)
	{
		pDevice->SetTexture(0, g_pTextureAbility[ABILITYTYPE_BULLET]);
	}
		

		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,//0�́A�`�悷��ŏ��̒��_�C���f�b�N�X�ŁA2�́A�`�悷��v���~�e�B�u��
			0,
			2);

	
	//==============================================================================================================================================

}

//�A�r���e�B�̐ݒ菈��
void SetAbility(ABILITYTYPE type)
{

	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D* pVtx;
	
	g_aAbility.nAbilityType = type;//�A�r���e�B��ݒ肷��
}

//===================================
//�A�r���e�B���̎擾
//===================================
Ability * GetAbility(void)
{
	return &g_aAbility;
}
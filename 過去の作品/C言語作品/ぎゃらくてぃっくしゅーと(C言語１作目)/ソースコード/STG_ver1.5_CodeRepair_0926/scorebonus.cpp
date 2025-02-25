//========================================================
//
//�X���Q�S�F�E�F�[�u�N���A���̃X�R�A�{�[�i�X���ǂ̂��炢����������\������
//Author:ShinaTaiyo
//
//=========================================================

#include "main.h"
#include "ScoreBonus.h"
#include "input.h"
#include "explosion.h"
#include "enemy.h"
#include "player.h"
#include "health.h"
#include "result.h"
#include "score.h"

#define MAX_SCOREBONUS (8)//�e�̍ő吔
#define NUM_SCOREBONUS (1)
#define SCOREBONUS_SCALE (100)

#define ScoreBonus_RADIUS (20.0f)

//�e�\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;//�ʒu
	int nLife;//�����i�ǂ̂��炢�e�����ł������j
	bool bUse;//�X�R�A���g�p���Ă��邩�ǂ���
	int nPosTexU;//�e���̐������i�[

}ScoreBonus;

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureScoreBonus = NULL;//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScoreBonus = NULL;//�e�̏��
ScoreBonus g_aScoreBonus[MAX_SCOREBONUS];
//float g_fXMove;//X���W�̈ړ�
//float g_fYMove;//Y���W�̈ړ�


//D3DXVECTOR3 g_rotPl;//����
int g_nScoreBonus = 0;
float g_fSCOREBONUS_SCALE = 0;


int g_nSaveScoreBonus = 0;



//�e�̏���������
void InitScoreBonus(void)
{
	ResultType* pResultType = GetResultType();


	/*if (pResultType->nResultType == false)
	{*/
	g_nScoreBonus = 0;//���U���g���[�h�ȊO�Ȃ�A�X�R�A������������
//}
//g_moveScoreBonus = D3DXVECTOR3(0.0f, -10.0f, 0.0f);//�ړ��ʂ�����������

	LPDIRECT3DDEVICE9 pDevice;
	int	nCntScoreBonus;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	D3DXCreateTextureFromFile(pDevice,
		"data\\number_001.png",
		&g_pTextureScoreBonus);

		//�X�R�A���̏�����
	for (nCntScoreBonus = 0; nCntScoreBonus < MAX_SCOREBONUS; nCntScoreBonus++)
	{
		g_aScoreBonus[nCntScoreBonus].nPosTexU = 0;
		g_aScoreBonus[nCntScoreBonus].nLife = 0;
		g_aScoreBonus[nCntScoreBonus].pos = D3DXVECTOR3(SCREEN_WIDTH - ((nCntScoreBonus + 1) * 50.0f), 50.0f, 0.0f);
		g_aScoreBonus[nCntScoreBonus].bUse = false;//�g�p���Ă��Ȃ���Ԃɂ���
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_SCOREBONUS,//�S�͕K�v�Ȓ��_��
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffScoreBonus,
		NULL);

	VERTEX_2D* pVtx;

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffScoreBonus->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntScoreBonus = 0; nCntScoreBonus < MAX_SCOREBONUS; nCntScoreBonus++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);

		g_aScoreBonus[nCntScoreBonus].bUse = true;//�g�p���Ă����Ԃɂ���

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
	g_pVtxBuffScoreBonus->Unlock();
}

//�e�̏I������
void UninitScoreBonus(void)
{
	int nCntScoreBonus;
	//�e�N�X�`���̔j��
	//for (nCntScoreBonus = 0; nCntScoreBonus < MAX_SCOREBONUS; nCntScoreBonus++)
	//{
	if (g_pTextureScoreBonus != NULL)
	{
		g_pTextureScoreBonus->Release();
		g_pTextureScoreBonus = NULL;
	}
	//}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffScoreBonus != NULL)
	{
		g_pVtxBuffScoreBonus->Release();
		g_pVtxBuffScoreBonus = NULL;
	}
}

//�e�̍X�V����
void UpdateScoreBonus(void)
{
	int nCntScoreBouns;
	VERTEX_2D* pVtx;

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffScoreBonus->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntScoreBouns = 0; nCntScoreBouns < MAX_SCOREBONUS; nCntScoreBouns++)
	{
		if (g_aScoreBonus[nCntScoreBouns].bUse == true)
		{

			g_aScoreBonus[nCntScoreBouns].nLife -= 1;

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aScoreBonus[nCntScoreBouns].pos.x - 20.0f, g_aScoreBonus[nCntScoreBouns].pos.y - 20.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aScoreBonus[nCntScoreBouns].pos.x + 20.0f, g_aScoreBonus[nCntScoreBouns].pos.y - 20.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aScoreBonus[nCntScoreBouns].pos.x - 20.0f, g_aScoreBonus[nCntScoreBouns].pos.y + 20.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aScoreBonus[nCntScoreBouns].pos.x + 20.0f, g_aScoreBonus[nCntScoreBouns].pos.y + 20.0f, 0.0f);
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

			if (g_aScoreBonus[nCntScoreBouns].nLife < 1)
			{
				g_aScoreBonus[nCntScoreBouns].bUse = false;
			}
		}
			pVtx += 4;//���_�f�[�^�̃|�C���^���S���i�߂�B
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffScoreBonus->Unlock();
}

//�e�̕`�揈��
void DrawScoreBonus(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�ւ̃|�C���^

	int nCntScoreBonus;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffScoreBonus, 0, sizeof(VERTEX_2D));//���_�o�b�t�@�ւ̃|�C���^�ƒ��_���̍\���̂̃T�C�Y

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);



	for (nCntScoreBonus = 0; nCntScoreBonus < MAX_SCOREBONUS; nCntScoreBonus++)
	{
		if (g_aScoreBonus[nCntScoreBonus].bUse == true)
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureScoreBonus);

			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,//0�́A�`�悷��ŏ��̒��_�C���f�b�N�X�ŁA2�́A�`�悷��v���~�e�B�u��
				nCntScoreBonus * 4,
				2);
		}

	}


}

//�X�R�A�̐ݒ菈��
void SetScoreBonus(int nScoreBonus)
{

	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D* pVtx;
	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffScoreBonus->Lock(0, 0, (void**)&pVtx, 0);


	int nGetScoreBonus;
	//
	//nScoreBonus = 184545;
	g_nScoreBonus = nScoreBonus;



	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	nGetScoreBonus = g_nScoreBonus;//�X�R�A��ۑ�����

	int nCnt;
	int	nCntScoreBonus;
	for (nCnt = 0; nCnt < MAX_SCOREBONUS; nCnt++)
	{
		if (g_aScoreBonus[nCnt].bUse == false)
		{
			g_aScoreBonus[nCnt].nLife = 90;
			g_aScoreBonus[nCnt].bUse = true;//�g�p���Ă��Ȃ���Ԃɂ���
			g_aScoreBonus[nCnt].nPosTexU = nGetScoreBonus % 10;//���̈ʒu�̃e�N�X�`���̃X�R�A�̒l�����߂�B
			nGetScoreBonus = nGetScoreBonus / 10;
		}
	}

	for (nCntScoreBonus = 0; nCntScoreBonus < MAX_SCOREBONUS; nCntScoreBonus++)
	{
		pVtx[0].tex = D3DXVECTOR2(0.0f + (0.1f * g_aScoreBonus[nCntScoreBonus].nPosTexU), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + (0.1f * g_aScoreBonus[nCntScoreBonus].nPosTexU), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + (0.1f * g_aScoreBonus[nCntScoreBonus].nPosTexU), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + (0.1f * g_aScoreBonus[nCntScoreBonus].nPosTexU), 1.0f);

		pVtx += 4;//���_�f�[�^�̃|�C���^���S���i�߂�B
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffScoreBonus->Unlock();
}
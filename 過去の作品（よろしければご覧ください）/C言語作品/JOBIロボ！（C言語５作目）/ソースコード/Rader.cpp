//=====================================================================================================================
//
//�P�Q���Q�U���F�`�[������̊�ՂÂ���[Level.cpp]
//Author:ShinaTaiyo
//
// 
// ���ӁFfRWidth,fRHeight�́A���S�_����̔��a�ő傫���A����Ȃǂ�����Ă��܂��B�������̏������g���Ȃ�A�����Ӊ������B
// 
//======================================================================================================================

#include "main.h"
#include "sound.h"
#include "player.h"
#include "Rader.h"
#include "enemy.h"
#include "field.h"
#include "debugproc.h"
//�O���[�o���ϐ�

//=============================================================================================

//=================================
//���[�_�[�\���p
//=================================
#define RADER_WIDTH (100.0f)
#define RADER_HEIGHT (100.0f)

LPDIRECT3DTEXTURE9 g_pTextureRader = NULL;//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRader = NULL;//���x�����l�̏��
Rader g_aRader;//���[�_�[�\����

//=================================
//���[�_�[�f�B�X�v���C�\���p
//=================================
#define RADERDISPLAY_WIDTH (4.0f)
#define RADERDISPLAY_HEIGHT (4.0f)

LPDIRECT3DTEXTURE9 g_pTextureRaderDisPlay[NUM_RADERDISPLAY] = {};//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRaderDisplay = NULL;//���x���̏��
RaderDisplay g_aRaderDisPlay[MAX_RADERDISPLAY];//���[�_�[�f�B�X�v���C�\����

#define RADERPOSRATIOX (FIELD_WIDTH / RADER_WIDTH)//���[�_�[�ɕ\��������W�ƁA���[���h���W������A���������߂�
#define RADERPOSRATIOY (FIELD_HEIGHT / RADER_HEIGHT)//���[�_�[�ɕ\��������W�ƁA���[���h���W������A���������߂�


//============================
//�����̃��[�_�[����
//============================
void InitRaderCom(void)
{
	InitRader();
	InitRaderDisplay();
}

void UninitRaderCom(void)
{
	UninitRader();
	UninitRaderDisplay();
}

void UpdateRaderCom(void)
{
	UpdateRader();
	UpdateRaderDisplay();
}

void DrawRaderCom(void)
{
	DrawRader();
	DrawRaderDisplay();
}

//==================================================================

//============================
//���[�_�[�̏���������
//============================
void InitRader(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�ւ̃|�C���^

	pDevice = GetDevice();

	VERTEX_2D* pVtx;

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Rader_000.png",
		&g_pTextureRader);


	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRader,
		NULL);

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffRader->Lock(0, 0, (void**)&pVtx, 0);

	g_aRader.pos = D3DXVECTOR3(SCREEN_WIDTH - RADER_WIDTH,200.0f - RADER_HEIGHT, 0.0f);
	g_aRader.rot = NULL_VECTOR3;
	g_aRader.mtxWorld = {};

		pVtx[0].pos = D3DXVECTOR3(g_aRader.pos.x - RADER_WIDTH, g_aRader.pos.y - RADER_HEIGHT, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aRader.pos.x + RADER_WIDTH, g_aRader.pos.y - RADER_HEIGHT, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aRader.pos.x - RADER_WIDTH, g_aRader.pos.y + RADER_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aRader.pos.x + RADER_WIDTH, g_aRader.pos.y + RADER_HEIGHT, 0.0f);

		//rhw�̐ݒ�i�����ݒ�j
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[�̐ݒ�i�����ݒ�j
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//�e�N�X�`�����W�̐ݒ�i�����ݒ�j
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffRader->Unlock();

}

//========================================
//���[�_�[�f�B�X�v���C�̏���������
//========================================
void InitRaderDisplay(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�ւ̃|�C���^

	pDevice = GetDevice();

	VERTEX_2D* pVtx;

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Rader_Player_000.png",
		&g_pTextureRaderDisPlay[0]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Rader_Enemy_000.png",
		&g_pTextureRaderDisPlay[1]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\RaderDisplay_Boss_000.png",
		&g_pTextureRaderDisPlay[2]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\RaderDisplay_Outlet_000.png",
		&g_pTextureRaderDisPlay[3]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\RaderDisplay_Editor_000.png",
		&g_pTextureRaderDisPlay[4]);


	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_RADERDISPLAY,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRaderDisplay,
		NULL);

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffRaderDisplay->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntRaderDisplay = 0; nCntRaderDisplay < MAX_RADERDISPLAY; nCntRaderDisplay++)
	{
		g_aRaderDisPlay[nCntRaderDisplay].bUse = false;//�g�p���Ă��邩
		g_aRaderDisPlay[nCntRaderDisplay].nType = 0;//���
		g_aRaderDisPlay[nCntRaderDisplay].pos = NULL_VECTOR3;//�ʒu
		g_aRaderDisPlay[nCntRaderDisplay].rot = NULL_VECTOR3;//����
		g_aRaderDisPlay[nCntRaderDisplay].size = 0.0f;//�傫��
		g_aRaderDisPlay[nCntRaderDisplay].fAngle = 0.0f;//�A���O��
		g_aRaderDisPlay[nCntRaderDisplay].fWidth = 0.0f;
		g_aRaderDisPlay[nCntRaderDisplay].fHeight = 0.0f;
		pVtx[0].pos = NULL_VECTOR3;
		pVtx[1].pos = NULL_VECTOR3;
		pVtx[2].pos = NULL_VECTOR3;
		pVtx[3].pos = NULL_VECTOR3;

		//rhw�̐ݒ�i�����ݒ�j
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[�̐ݒ�i�����ݒ�j
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//�e�N�X�`�����W�̐ݒ�i�����ݒ�j
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffRaderDisplay->Unlock();

}


//==============================
//���[�_�[�̏I������
//==============================
void UninitRader(void)
{
	if (g_pTextureRader != NULL)
	{
		g_pTextureRader->Release();
		g_pTextureRader = NULL;
	}
	
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffRader != NULL)
	{
		g_pVtxBuffRader->Release();
		g_pVtxBuffRader = NULL;
	}
}


//==============================
//���[�_�[�̏I������
//==============================
void UninitRaderDisplay(void)
{
	//�e�N�X�`���̔j��
	for (int nCntRaderDisplay = 0; nCntRaderDisplay < NUM_RADERDISPLAY; nCntRaderDisplay++)
	{
		if (g_pTextureRaderDisPlay[nCntRaderDisplay] != NULL)
		{
			g_pTextureRaderDisPlay[nCntRaderDisplay]->Release();
			g_pTextureRaderDisPlay[nCntRaderDisplay] = NULL;
		}
	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffRaderDisplay != NULL)
	{
		g_pVtxBuffRaderDisplay->Release();
		g_pVtxBuffRaderDisplay = NULL;
	}
}

//============================
//���[�_�[�̍X�V����
//============================
void UpdateRader(void)
{



}

//============================
//���[�_�[�f�B�X�v���C�̍X�V����
//============================
void UpdateRaderDisplay(void)
{
	
}




//================================
//���[�_�[�̕`�揈��
//================================
void DrawRader(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffRader, 0, sizeof(VERTEX_2D));//���_�o�b�t�@�ւ̃|�C���^�ƒ��_���̍\���̂̃T�C�Y

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	pDevice->SetTexture(0,g_pTextureRader);
	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,0, 2);//0�́A�`�悷��ŏ��̒��_�C���f�b�N�X�ŁA2�́A�`�悷��v���~�e�B�u��

}


//================================
//���[�_�[�f�B�X�v���C�̕`�揈��
//================================
void DrawRaderDisplay(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffRaderDisplay, 0, sizeof(VERTEX_2D));//���_�o�b�t�@�ւ̃|�C���^�ƒ��_���̍\���̂̃T�C�Y

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntRaderDisplay = 0; nCntRaderDisplay < MAX_RADERDISPLAY; nCntRaderDisplay++)
	{
		if (g_aRaderDisPlay[nCntRaderDisplay].bUse == true)
		{
			pDevice->SetTexture(0, g_pTextureRaderDisPlay[g_aRaderDisPlay[nCntRaderDisplay].nType]);
			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntRaderDisplay * 4, 2);//0�́A�`�悷��ŏ��̒��_�C���f�b�N�X�ŁA2�́A�`�悷��v���~�e�B�u��
		}
	}

}

//================================
//���[�_�[�f�B�X�v���C�̃Z�b�g
//================================
int SetRaderDisplay(int nType)
{
	int nCntRaderDisplay = -1;

	for (int nCnt = 0; nCnt < MAX_RADERDISPLAY; nCnt++)
	{
		if (g_aRaderDisPlay[nCnt].bUse == false)
		{
			g_aRaderDisPlay[nCnt].bUse = true;//�g�p���Ă��邩�ǂ���
			g_aRaderDisPlay[nCnt].nType = nType;//���
			nCntRaderDisplay = nCnt;
			break;
		}
	}

	return nCntRaderDisplay;
}

//====================================
//���[�_�[�f�B�X�v���C�̈ʒu�̍X�V
//====================================
void SetPositionRaderDisPlay(int nIdxRaderDisPlay, D3DXVECTOR3 pos, float fRotZ)
{
	VERTEX_2D* pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffRaderDisplay->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntRaderDisPlay = 0; nCntRaderDisPlay < MAX_RADERDISPLAY; nCntRaderDisPlay++)
	{

		if (g_aRaderDisPlay[nIdxRaderDisPlay].bUse == true && nIdxRaderDisPlay == nCntRaderDisPlay)
		{
		    g_aRaderDisPlay[nIdxRaderDisPlay].rot.z = fRotZ;

			g_aRaderDisPlay[nIdxRaderDisPlay].pos.x = g_aRader.pos.x + (pos.x / RADERPOSRATIOX);//�t�B�[���h�ƃ��[�_�[�̔䗦�̍��W
			g_aRaderDisPlay[nIdxRaderDisPlay].pos.y = g_aRader.pos.y - (pos.z / RADERPOSRATIOY);

			//���_���W�̐ݒ�i�����ʒu�j
			pVtx[0].pos = NULL_VECTOR3;
			pVtx[1].pos = NULL_VECTOR3;
			pVtx[2].pos = NULL_VECTOR3;
			pVtx[3].pos = NULL_VECTOR3;


			if (g_aRaderDisPlay[nIdxRaderDisPlay].nType == RADERDISPLAY02_BOSS)
			{//�{�X�Ȃ�
					
				g_aRaderDisPlay[nIdxRaderDisPlay].fWidth = RADERDISPLAY_WIDTH * 5.0f;
				g_aRaderDisPlay[nIdxRaderDisPlay].fHeight = RADERDISPLAY_HEIGHT * 5.0f;
			}
			else if (g_aRaderDisPlay[nIdxRaderDisPlay].nType == RADERDISPLAY03_OUTLET)
			{
				g_aRaderDisPlay[nIdxRaderDisPlay].fWidth = RADERDISPLAY_WIDTH * 4.0f;
				g_aRaderDisPlay[nIdxRaderDisPlay].fHeight = RADERDISPLAY_HEIGHT * 4.0f;

			}
			else if(g_aRaderDisPlay[nIdxRaderDisPlay].nType == RADERDISPLAY00_PLAYER)
			{//�{�X�ȊO�Ȃ�
				
				g_aRaderDisPlay[nIdxRaderDisPlay].fWidth = RADERDISPLAY_WIDTH * 3.5f;
				g_aRaderDisPlay[nIdxRaderDisPlay].fHeight = RADERDISPLAY_HEIGHT * 3.5f;
			}
			else if (g_aRaderDisPlay[nIdxRaderDisPlay].nType == RADERDISPLAY01_ENEMY)
			{
				g_aRaderDisPlay[nIdxRaderDisPlay].fWidth = RADERDISPLAY_WIDTH * 2.5f;
				g_aRaderDisPlay[nIdxRaderDisPlay].fHeight = RADERDISPLAY_HEIGHT * 2.5f;
			}
			else if (g_aRaderDisPlay[nIdxRaderDisPlay].nType == RADERDISPLAY04_EDIT)
			{
				g_aRaderDisPlay[nIdxRaderDisPlay].fWidth = RADERDISPLAY_WIDTH * 3.5f;
				g_aRaderDisPlay[nIdxRaderDisPlay].fHeight = RADERDISPLAY_HEIGHT * 3.5f;
			}

			g_aRaderDisPlay[nIdxRaderDisPlay].fLength = sqrtf(g_aRaderDisPlay[nIdxRaderDisPlay].fWidth * g_aRaderDisPlay[nIdxRaderDisPlay].fWidth +
				g_aRaderDisPlay[nIdxRaderDisPlay].fHeight * g_aRaderDisPlay[nIdxRaderDisPlay].fHeight) / 2.0f;

			g_aRaderDisPlay[nIdxRaderDisPlay].fAngle = atan2f(g_aRaderDisPlay[nIdxRaderDisPlay].fWidth, g_aRaderDisPlay[nIdxRaderDisPlay].fHeight);
			//���_���W�̐ݒ�
			pVtx[0].pos.x = g_aRaderDisPlay[nIdxRaderDisPlay].pos.x + sinf(g_aRaderDisPlay[nIdxRaderDisPlay].rot.z - (D3DX_PI - g_aRaderDisPlay[nIdxRaderDisPlay].fAngle)) * g_aRaderDisPlay[nIdxRaderDisPlay].fLength;//�ォ�獶�ɂS�T�x
			pVtx[0].pos.y = g_aRaderDisPlay[nIdxRaderDisPlay].pos.y + cosf(g_aRaderDisPlay[nIdxRaderDisPlay].rot.z - (D3DX_PI - g_aRaderDisPlay[nIdxRaderDisPlay].fAngle)) * g_aRaderDisPlay[nIdxRaderDisPlay].fLength;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_aRaderDisPlay[nIdxRaderDisPlay].pos.x + sinf(g_aRaderDisPlay[nIdxRaderDisPlay].rot.z + (D3DX_PI - g_aRaderDisPlay[nIdxRaderDisPlay].fAngle)) * g_aRaderDisPlay[nIdxRaderDisPlay].fLength;//�ォ��E�ɂS�T�x
			pVtx[1].pos.y = g_aRaderDisPlay[nIdxRaderDisPlay].pos.y + cosf(g_aRaderDisPlay[nIdxRaderDisPlay].rot.z + (D3DX_PI - g_aRaderDisPlay[nIdxRaderDisPlay].fAngle)) * g_aRaderDisPlay[nIdxRaderDisPlay].fLength;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_aRaderDisPlay[nIdxRaderDisPlay].pos.x + sinf(g_aRaderDisPlay[nIdxRaderDisPlay].rot.z - g_aRaderDisPlay[nIdxRaderDisPlay].fAngle) * g_aRaderDisPlay[nIdxRaderDisPlay].fLength;//�����獶�ɂS�T�x
			pVtx[2].pos.y = g_aRaderDisPlay[nIdxRaderDisPlay].pos.y + cosf(g_aRaderDisPlay[nIdxRaderDisPlay].rot.z - g_aRaderDisPlay[nIdxRaderDisPlay].fAngle) * g_aRaderDisPlay[nIdxRaderDisPlay].fLength;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_aRaderDisPlay[nIdxRaderDisPlay].pos.x + sinf(g_aRaderDisPlay[nIdxRaderDisPlay].rot.z + g_aRaderDisPlay[nIdxRaderDisPlay].fAngle) * g_aRaderDisPlay[nIdxRaderDisPlay].fLength;//������E�ɂS�T�x
			pVtx[3].pos.y = g_aRaderDisPlay[nIdxRaderDisPlay].pos.y + cosf(g_aRaderDisPlay[nIdxRaderDisPlay].rot.z + g_aRaderDisPlay[nIdxRaderDisPlay].fAngle) * g_aRaderDisPlay[nIdxRaderDisPlay].fLength;
			pVtx[3].pos.z = 0.0f;
			

			if (pos.y > 280.0f)
			{
				//���_�J���[�̐ݒ�i�����ݒ�j
				pVtx[0].col = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);
				pVtx[1].col = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);
				pVtx[2].col = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);
				pVtx[3].col = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);
			}
			else
			{
				//���_�J���[�̐ݒ�i�����ݒ�j
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			}
			break;
		}

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffRaderDisplay->Unlock();
}

//================================
//���[�_�[�f�B�X�v���C�̔j��
//================================
void KillRaderDisplay(int nIdxRaderDisplay)
{
	g_aRaderDisPlay[nIdxRaderDisplay].bUse = false;
}




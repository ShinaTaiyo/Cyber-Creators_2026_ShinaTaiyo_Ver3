//=====================================================================================================================
//
// �P�Q���Q�U���F�`�[������̊�ՂÂ���[Battery.cpp]
// Author : ShinaTaiyo
// 
// ���ӁFfRWidth,fRHeight�́A���S�_����̔��a�ő傫���A����Ȃǂ�����Ă��܂��B�������̏������g���Ȃ�A�����Ӊ������B
// 
//======================================================================================================================
#include "battery.h"
#include "main.h"
#include "model.h"
#include "sound.h"
#include "player.h"
#include "fade.h"
#include "input.h"
#include "particle.h"
#include "game.h"

#define BATTERY_WIDTH (150.0f)
#define BATTERY_HEIGHT (75.0f)
#define BATTERYNUM_WIDTH (20.0f)
#define BATTERYNUM_HEIGHT (40.0f)

#define MAXBATTERY_WIDTH (100.0f)   //�o�b�e���[�ő�l�̉���
#define MAXBATTERY_HEIGHT (40.0f)   //�o�b�e���[�ő�l�̍���

#define MAXBATTERYNUM_WIDTH (15.0f) //�o�b�e���[�ő�l�̐��l�̉���
#define MAXBATTERYNUM_HEIGHT (35.0f)//�o�b�e���[�ő�l�̐��l�̍���

//�O���[�o���ϐ�
int g_nDelayBattery;

//=================================
//�o�b�e���[�t���[���\���p
//=================================
LPDIRECT3DTEXTURE9 g_pTextureBattery[NUM_BATTERY] = {};//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBattery = NULL;//�o�b�e���[�̏��
Battery g_aBattery[NUM_BATTERY];//�o�b�e���[�\���p

//=================================
//�o�b�e���[���l�\���p
//=================================
LPDIRECT3DTEXTURE9 g_pTextureBatteryNum = NULL;//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBatteryNum = NULL;//�o�b�e���[���l�̏��
BatteryNum g_aBatteryNum[BATTERYDIGIT];//�o�b�e���[���l�̍\����

//=================================
//�[�d��̈ʒu�\���p
//=================================
PDIRECT3DVERTEXBUFFER9 g_pVtxBuffOutletDisp = NULL;//���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureOutletDisp = NULL;//�e�N�X�`���ւ̃|�C���^
OutletDisp g_aOutletDisp[MAX_OUTLETDISP];//�[�d��ʒu�\���̍\����
int g_nDelayOutletDIsp;

//============================
//����������
//============================
void InitBattery(void)
{
	VERTEX_2D* pVtx;
	Player* pPlayer = GetPlayer();//�v���C���[�̏����擾

	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�ւ̃|�C���^

	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Battery_003.png",
		&g_pTextureBattery[BATTERYTYPE00_NOWBATTERY]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\MaxBattery_000.png",
		&g_pTextureBattery[BATTERYTYPE01_MAXBATTERY]);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_BATTERY,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBattery,
		NULL);

	g_nDelayBattery = 0;

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffBattery->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBattery = 0; nCntBattery < NUM_BATTERY; nCntBattery++)
	{
		switch (nCntBattery)
		{
		case BATTERYTYPE00_NOWBATTERY://���݂̃o�b�e���[�̏�����
			g_aBattery[nCntBattery].pos = D3DXVECTOR3(1150.0f, 580.0f, 0.0f);//�ʒu
			g_aBattery[nCntBattery].nBattery = pPlayer->nBattery;//���݂̃o�b�e���[
			g_aBattery[nCntBattery].bCautionBattery = false;//�x��
			g_aBattery[nCntBattery].nAnimationCnt = 0;//�A�j���[�V�����J�E���g
			g_aBattery[nCntBattery].nAnimationPattern = 0;//�A�j���[�V�����p�^�[��
			g_aBattery[nCntBattery].bAnimationTurn = 0;//�A�j���[�V�������t�ɂ���

			//�e�N�X�`�����W�̐ݒ�i�����ݒ�j
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.2f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 0.2f);
			pVtx[3].tex = D3DXVECTOR2(0.2f, 0.2f);

			pVtx[0].pos = D3DXVECTOR3(g_aBattery[nCntBattery].pos.x - BATTERY_WIDTH, g_aBattery[nCntBattery].pos.y - BATTERY_HEIGHT, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aBattery[nCntBattery].pos.x + BATTERY_WIDTH, g_aBattery[nCntBattery].pos.y - BATTERY_HEIGHT, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aBattery[nCntBattery].pos.x - BATTERY_WIDTH, g_aBattery[nCntBattery].pos.y + BATTERY_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aBattery[nCntBattery].pos.x + BATTERY_WIDTH, g_aBattery[nCntBattery].pos.y + BATTERY_HEIGHT, 0.0f);

			//rhw�̐ݒ�i�����ݒ�j
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			//���_�J���[�̐ݒ�i�����ݒ�j
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			break;

		case BATTERYTYPE01_MAXBATTERY://�o�b�e���[�ő�l�̏�����
			g_aBattery[nCntBattery].pos = D3DXVECTOR3(1155.0f, 675.0f, 0.0f);//�ʒu
			g_aBattery[nCntBattery].nBattery = pPlayer->nBattery;//�o�b�e���[�̍ő�l

			pVtx[0].pos = D3DXVECTOR3(g_aBattery[nCntBattery].pos.x - MAXBATTERY_WIDTH, g_aBattery[nCntBattery].pos.y - MAXBATTERY_HEIGHT, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aBattery[nCntBattery].pos.x + MAXBATTERY_WIDTH, g_aBattery[nCntBattery].pos.y - MAXBATTERY_HEIGHT, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aBattery[nCntBattery].pos.x - MAXBATTERY_WIDTH, g_aBattery[nCntBattery].pos.y + MAXBATTERY_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aBattery[nCntBattery].pos.x + MAXBATTERY_WIDTH, g_aBattery[nCntBattery].pos.y + MAXBATTERY_HEIGHT, 0.0f);

			//rhw�̐ݒ�i�����ݒ�j
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			//���_�J���[�̐ݒ�i�����ݒ�j
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

			//�e�N�X�`�����W�̐ݒ�i�����ݒ�j
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

			break;

		default:
			break;
		}


		pVtx += 4;

	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBattery->Unlock();

	//==========================
	//�o�b�e���[���l�̏���
	//==========================

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Number_Battery.png",
		&g_pTextureBatteryNum);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * BATTERYDIGIT * NUM_BATTERY,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBatteryNum,
		NULL);

	int nGetBattery = g_aBattery[0].nBattery;//�X�R�A��ۑ�����

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffBatteryNum->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntBattery = 0; nCntBattery < NUM_BATTERY; nCntBattery++)
	{
		for (int nCntDigit = 0; nCntDigit < BATTERYDIGIT; nCntDigit++)
		{
			switch (nCntBattery)
			{
			case BATTERYTYPE00_NOWBATTERY:
				g_aBattery[nCntBattery].aBatteryNum[nCntDigit].pos = D3DXVECTOR3(1225.0f - 50.0f * nCntDigit, 583.0f, 0.0f);
				if (nCntDigit == 1)
				{
					g_aBattery[nCntBattery].aBatteryNum[nCntDigit].pos = D3DXVECTOR3(1223.0f - 50.0f * nCntDigit, 583.0f, 0.0f);
				}
				if (nCntDigit == 2)
				{
					g_aBattery[nCntBattery].aBatteryNum[nCntDigit].pos = D3DXVECTOR3(1226.0f - 50.0f * nCntDigit, 583.0f, 0.0f);
				}
				if (nCntDigit == 3)
				{
					g_aBattery[nCntBattery].aBatteryNum[nCntDigit].pos = D3DXVECTOR3(1230.0f - 50.0f * nCntDigit, 583.0f, 0.0f);
				}
				//���_�̐ݒ�
				pVtx[0].pos = D3DXVECTOR3(g_aBattery[nCntBattery].aBatteryNum[nCntDigit].pos.x - BATTERYNUM_WIDTH, g_aBattery[nCntBattery].aBatteryNum[nCntDigit].pos.y - BATTERYNUM_HEIGHT, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aBattery[nCntBattery].aBatteryNum[nCntDigit].pos.x + BATTERYNUM_WIDTH, g_aBattery[nCntBattery].aBatteryNum[nCntDigit].pos.y - BATTERYNUM_HEIGHT, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aBattery[nCntBattery].aBatteryNum[nCntDigit].pos.x - BATTERYNUM_WIDTH, g_aBattery[nCntBattery].aBatteryNum[nCntDigit].pos.y + BATTERYNUM_HEIGHT, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aBattery[nCntBattery].aBatteryNum[nCntDigit].pos.x + BATTERYNUM_WIDTH, g_aBattery[nCntBattery].aBatteryNum[nCntDigit].pos.y + BATTERYNUM_HEIGHT, 0.0f);

				break;
			case BATTERYTYPE01_MAXBATTERY:
				g_aBattery[nCntBattery].aBatteryNum[nCntDigit].pos = D3DXVECTOR3(1235.0f - 30.0f * nCntDigit, 675.0f, 0.0f);
				if (nCntDigit == 1)
				{
					g_aBattery[nCntBattery].aBatteryNum[nCntDigit].pos = D3DXVECTOR3(1235.0f - 30.0f * nCntDigit, 675.0f, 0.0f);
				}
				if (nCntDigit == 2)
				{
					g_aBattery[nCntBattery].aBatteryNum[nCntDigit].pos = D3DXVECTOR3(1235.0f - 30.0f * nCntDigit, 675.0f, 0.0f);
				}
				if (nCntDigit == 3)
				{
					g_aBattery[nCntBattery].aBatteryNum[nCntDigit].pos = D3DXVECTOR3(1235.0f - 30.0f * nCntDigit, 675.0f, 0.0f);
				}

				//���_�̐ݒ�
				pVtx[0].pos = D3DXVECTOR3(g_aBattery[nCntBattery].aBatteryNum[nCntDigit].pos.x - MAXBATTERYNUM_WIDTH, g_aBattery[nCntBattery].aBatteryNum[nCntDigit].pos.y - MAXBATTERYNUM_HEIGHT, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aBattery[nCntBattery].aBatteryNum[nCntDigit].pos.x + MAXBATTERYNUM_WIDTH, g_aBattery[nCntBattery].aBatteryNum[nCntDigit].pos.y - MAXBATTERYNUM_HEIGHT, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aBattery[nCntBattery].aBatteryNum[nCntDigit].pos.x - MAXBATTERYNUM_WIDTH, g_aBattery[nCntBattery].aBatteryNum[nCntDigit].pos.y + MAXBATTERYNUM_HEIGHT, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aBattery[nCntBattery].aBatteryNum[nCntDigit].pos.x + MAXBATTERYNUM_WIDTH, g_aBattery[nCntBattery].aBatteryNum[nCntDigit].pos.y + MAXBATTERYNUM_HEIGHT, 0.0f);

				break;
			default:
				break;
			}
			//rhw�̐ݒ�i�����ݒ�j
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			//���_�J���[�̐ݒ�i�����ݒ�j
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

			g_aBattery[nCntBattery].aBatteryNum[nCntDigit].fPosTexU = (float)(nGetBattery % 10);
			nGetBattery /= 10;

			pVtx[0].tex = D3DXVECTOR2(0.0f + (0.1f * g_aBattery[nCntBattery].aBatteryNum[nCntDigit].fPosTexU), 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f + (0.1f * g_aBattery[nCntBattery].aBatteryNum[nCntDigit].fPosTexU), 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f + (0.1f * g_aBattery[nCntBattery].aBatteryNum[nCntDigit].fPosTexU), 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f + (0.1f * g_aBattery[nCntBattery].aBatteryNum[nCntDigit].fPosTexU), 1.0f);

			pVtx += 4;//���_���W�̃|�C���^���C���N�������g
		}
	}
	
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBatteryNum->Unlock();
}

//==============================
//�I������
//==============================
void UninitBattery(void)
{
	//============================
	//�o�b�e���[�t���[���̏���
	//============================

	//�e�N�X�`���̔j��

	for (int nCntBattery = 0; nCntBattery < NUM_BATTERY; nCntBattery++)
	{
		if (g_pTextureBattery[nCntBattery] != NULL)
		{
			g_pTextureBattery[nCntBattery]->Release();
			g_pTextureBattery[nCntBattery] = NULL;
		}
	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffBattery != NULL)
	{
		g_pVtxBuffBattery->Release();
		g_pVtxBuffBattery = NULL;
	}

	//==============================
	//�o�b�e���[���l�̏���
	//==============================

	if (g_pTextureBatteryNum != NULL)
	{
		g_pTextureBatteryNum->Release();
		g_pTextureBatteryNum = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffBatteryNum != NULL)
	{
		g_pVtxBuffBatteryNum->Release();
		g_pVtxBuffBatteryNum = NULL;
	}
}
//============================
//�X�V����
//============================
void UpdateBattery(void)
{
	Player* pPlayer = GetPlayer();
	VERTEX_2D* pVtx;
	g_nDelayBattery++;

	//========================================================================
	//���݂̃o�b�e���[�t���[����d�r���T�O�O�����������A�_�ł����鏈��
	//========================================================================

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffBattery->Lock(0, 0, (void**)&pVtx, 0);

		g_aBattery[BATTERYTYPE00_NOWBATTERY].nAnimationCnt++;

		if (g_aBattery[BATTERYTYPE00_NOWBATTERY].nBattery <= 500)
		{
			if (g_aBattery[BATTERYTYPE00_NOWBATTERY].nAnimationCnt % 5 == 0)
			{
				//�A�j���[�V�������^�[��������
				if (g_aBattery[BATTERYTYPE00_NOWBATTERY].bAnimationTurn == false)
				{
					g_aBattery[BATTERYTYPE00_NOWBATTERY].nAnimationPattern++;

					if (g_aBattery[BATTERYTYPE00_NOWBATTERY].nAnimationPattern >= 5)
					{
						g_aBattery[BATTERYTYPE00_NOWBATTERY].bAnimationTurn = true;
						g_aBattery[BATTERYTYPE00_NOWBATTERY].nAnimationPattern = 3;
					}
				}
				else
				{
					g_aBattery[BATTERYTYPE00_NOWBATTERY].nAnimationPattern--;
					if (g_aBattery[BATTERYTYPE00_NOWBATTERY].nAnimationPattern <= -1)
					{
						g_aBattery[BATTERYTYPE00_NOWBATTERY].bAnimationTurn = false;
						g_aBattery[BATTERYTYPE00_NOWBATTERY].nAnimationPattern = 1;
					}
				}
			}
		}
		else
		{
			g_aBattery[BATTERYTYPE00_NOWBATTERY].nAnimationPattern = 0;
			g_aBattery[BATTERYTYPE00_NOWBATTERY].bAnimationTurn = false;
		}

		//�e�N�X�`�����W�̐ݒ�i�����ݒ�j
		pVtx[0].tex = D3DXVECTOR2(0.0f + 0.2f * g_aBattery[BATTERYTYPE00_NOWBATTERY].nAnimationPattern, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.2f + 0.2f * g_aBattery[BATTERYTYPE00_NOWBATTERY].nAnimationPattern, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + 0.2f * g_aBattery[BATTERYTYPE00_NOWBATTERY].nAnimationPattern, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.2f + 0.2f * g_aBattery[BATTERYTYPE00_NOWBATTERY].nAnimationPattern, 1.0f);

	
    //���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBattery->Unlock();

	//==============================================
	//���݂̃o�b�e���[���l
	//==============================================

	if (GetKeyboardTrigger(DIK_B) == true)
	{
		g_aBattery[BATTERYTYPE00_NOWBATTERY].nBattery += 100;
	}

	if (g_aBattery[BATTERYTYPE00_NOWBATTERY].nBattery >= pPlayer->nBattery)
	{
		//�o�b�e���[�ʂ��ő�l�𒴂�����ő�l�ŌŒ肷��
		g_aBattery[BATTERYTYPE00_NOWBATTERY].nBattery = pPlayer->nBattery;
	}

	if (g_nDelayBattery % 300 == 0)
	{//�T�b���ƂɃo�b�e���[���P���炷
		SubBattery(1);
	}

	if (g_aBattery[BATTERYTYPE00_NOWBATTERY].nBattery <= 0)
	{//�o�b�e���[���O�ȉ��ɂȂ�����O�ɌŒ�
		g_aBattery[BATTERYTYPE00_NOWBATTERY].nBattery = 0;
	}

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffBatteryNum->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBattery = 0; nCntBattery < NUM_BATTERY; nCntBattery++)
	{
		//==================================================
		//���f�����鐔�l�̐ݒ�
		//==================================================
		int nGetBattery = 0;

		switch (nCntBattery)
		{
		case BATTERYTYPE00_NOWBATTERY:
			nGetBattery = g_aBattery[nCntBattery].nBattery;
			break;
		case BATTERYTYPE01_MAXBATTERY:
			nGetBattery = pPlayer->nBattery;
			break;
		default:
			break;
		}

		for (int nCntDigit = 0; nCntDigit < BATTERYDIGIT; nCntDigit++)
		{
			g_aBattery[nCntBattery].aBatteryNum[nCntDigit].fPosTexU = (float)(nGetBattery % 10);
			nGetBattery /= 10;

			pVtx[0].tex = D3DXVECTOR2(0.0f + (0.1f * g_aBattery[nCntBattery].aBatteryNum[nCntDigit].fPosTexU), 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f + (0.1f * g_aBattery[nCntBattery].aBatteryNum[nCntDigit].fPosTexU), 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f + (0.1f * g_aBattery[nCntBattery].aBatteryNum[nCntDigit].fPosTexU), 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f + (0.1f * g_aBattery[nCntBattery].aBatteryNum[nCntDigit].fPosTexU), 1.0f);

			pVtx += 4;
		}
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBatteryNum->Unlock();
}

//================================
//�`�揈��
//================================
void DrawBattery(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//===========================
	//�o�b�e���[�t���[���̏���
	//===========================

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffBattery, 0, sizeof(VERTEX_2D));//���_�o�b�t�@�ւ̃|�C���^�ƒ��_���̍\���̂̃T�C�Y

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntBattery = 0; nCntBattery < NUM_BATTERY; nCntBattery++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureBattery[nCntBattery]);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,nCntBattery * 4, 2);//0�́A�`�悷��ŏ��̒��_�C���f�b�N�X�ŁA2�́A�`�悷��v���~�e�B�u��
	}
	//=============================
	//�o�b�e���[���l�̏���
	//=============================

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffBatteryNum, 0, sizeof(VERTEX_2D));//���_�o�b�t�@�ւ̃|�C���^�ƒ��_���̍\���̂̃T�C�Y

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntBatteryNum = 0; nCntBatteryNum < BATTERYDIGIT * NUM_BATTERY; nCntBatteryNum++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureBatteryNum);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBatteryNum * 4, 2);
	}
}

//===========================
// �o�b�e���[�̏����擾
//===========================
Battery* GetBattery(void)//�X�N���b�v�\���̂̏����Q�b�g����
{
	return &g_aBattery[0];
}

//===========================
// �o�b�e���[��������
//===========================
void SubBattery(int nSubBattery)
{

	if (GetMode() == MODE_GAME)
	{//�Q�[����ʂ̏ꍇ
		if (g_aBattery[BATTERYTYPE00_NOWBATTERY].nBattery > 0)
		{//�o�b�e���[���c���Ă���ꍇ
			if (g_aBattery[BATTERYTYPE00_NOWBATTERY].nBattery > 500)
			{//�o�b�e���[��100�ȏ�̏ꍇ
				g_aBattery[BATTERYTYPE00_NOWBATTERY].bCautionBattery = false;
			}
			else if (g_aBattery[BATTERYTYPE00_NOWBATTERY].nBattery < 500 && !g_aBattery[BATTERYTYPE00_NOWBATTERY].bCautionBattery)
			{
				g_aBattery[BATTERYTYPE00_NOWBATTERY].bCautionBattery = true;
				PlaySound(SOUND_LABEL_SE_BATTERYCAUTION_000);//�x�������o��
			}

			g_aBattery[BATTERYTYPE00_NOWBATTERY].nBattery -= nSubBattery;//�o�b�e���[��1�グ��

			if (g_aBattery[BATTERYTYPE00_NOWBATTERY].nBattery <= 0)
			{//�o�b�e���[��0�ȉ��̏ꍇ
				SetFade(MODE_RESULT);	//���U���g��ʂ�

				StopSound(SOUND_LABEL_BGM015);
				StopSound(SOUND_LABEL_BGM013);
				
			}
		}
	}
}

void InitOutletDisp(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	g_nDelayOutletDIsp = 0;

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\RaderDisplay_Outlet_000.png",
		&g_pTextureOutletDisp);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_OUTLETDISP,//���_�̐�
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffOutletDisp,
		NULL);

	VERTEX_3D* pVtx;
	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffOutletDisp->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntOutlet = 0; nCntOutlet < MAX_OUTLETDISP; nCntOutlet++)
	{
		g_aOutletDisp[nCntOutlet].pos = NULL_VECTOR3;//�ʒu
		g_aOutletDisp[nCntOutlet].bUse = false;//�g�p���
		g_aOutletDisp[nCntOutlet].rot = NULL_VECTOR3;//����
		g_aOutletDisp[nCntOutlet].nNumModel = 0;//�ǂ̃��f��������������
		g_aOutletDisp[nCntOutlet].mtxWorld = {};//�}�g���b�N�X���[���h

		//���_���W�̐ݒ�i�����ʒu�j
		pVtx[0].pos = D3DXVECTOR3(-OUTLETDISP_WIDTH, OUTLETDISP_HEIGHT, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(OUTLETDISP_WIDTH, OUTLETDISP_HEIGHT, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-OUTLETDISP_WIDTH, -OUTLETDISP_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(OUTLETDISP_WIDTH, -OUTLETDISP_HEIGHT, 0.0f);

		//nor�̐ݒ�i�����ݒ�j
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLOR�́A0.0f�`1.0f�̊Ԃ̐��l�ŐF��ς���B
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLOR�́A0.0f�`1.0f�̊Ԃ̐��l�ŐF��ς���B
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLOR�́A0.0f�`1.0f�̊Ԃ̐��l�ŐF��ς���B
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLOR�́A0.0f�`1.0f�̊Ԃ̐��l�ŐF��ς���B

		//�e�N�X�`�����W�̐ݒ�i�����ݒ�j
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;//�|�C���^��i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffOutletDisp->Unlock();
}

void UninitOutletDisp(void)
{
	if (g_pTextureOutletDisp != NULL)
	{
		g_pTextureOutletDisp->Release();
		g_pTextureOutletDisp = NULL;
	}

	if (g_pVtxBuffOutletDisp != NULL)
	{
		g_pVtxBuffOutletDisp->Release();
		g_pVtxBuffOutletDisp = NULL;
	}
}

void UpdateOutletDisp(void)
{
	for (int nCntOutlet = 0; nCntOutlet < MAX_OUTLETDISP; nCntOutlet++)
	{
		if (g_aOutletDisp[nCntOutlet].bUse == true)
		{
			Model* pModel = GetModel();

			for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++, pModel++)
			{
				if (pModel->bUse == false && nCntModel == g_aOutletDisp[nCntOutlet].nNumModel)
				{//�g�p����Ȃ��Ȃ������f�����A�[�d��ʒu�\���������������f���ƈ�v������A�[�d��ʒu�\��������
					g_aOutletDisp[nCntOutlet].bUse = false;
					break;
				}
			}
		}
	}
}

void DrawOutletDisp(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�ւ̃|�C���^
	D3DXMATRIX mtxTrans;//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxView;//�r���[�}�g���b�N�X�擾�p

    //�f�o�C�X�̎擾
	pDevice = GetDevice();

	for (int nCntOutlet = 0; nCntOutlet < MAX_OUTLETDISP; nCntOutlet++)
	{
		if (g_aOutletDisp[nCntOutlet].bUse == true)
		{
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aOutletDisp[nCntOutlet].mtxWorld);

			//�r���[�}�g���b�N�X���擾
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//�|���S�����J�����ɑ΂��Đ��ʂɌ�����
			D3DXMatrixInverse(&g_aOutletDisp[nCntOutlet].mtxWorld, NULL, &mtxView);

			//�t�s������߂�
			g_aOutletDisp[nCntOutlet].mtxWorld._41 = 0.0f;
			g_aOutletDisp[nCntOutlet].mtxWorld._42 = 0.0f;
			g_aOutletDisp[nCntOutlet].mtxWorld._43 = 0.0f;

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aOutletDisp[nCntOutlet].pos.x, g_aOutletDisp[nCntOutlet].pos.y, g_aOutletDisp[nCntOutlet].pos.z);
			D3DXMatrixMultiply(&g_aOutletDisp[nCntOutlet].mtxWorld, &g_aOutletDisp[nCntOutlet].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aOutletDisp[nCntOutlet].mtxWorld);

			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffOutletDisp, 0, sizeof(VERTEX_3D));//���_�o�b�t�@�ւ̃|�C���^�ƒ��_���̍\���̂̃T�C�Y

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureOutletDisp);

			//===============================================================
			//�`��̒���
			//==============================================================
			//���C�g�𖳌��ɂ���
			pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

			////Z�̔�r���@�ύX�B
			//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

			////Z�o�b�t�@�ɏ������܂Ȃ�
			//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

			//�A���t�@�e�X�g��L��
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
			pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
			pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntOutlet * 4, 2);//0�́A�`�悷��ŏ��̒��_�C���f�b�N�X�ŁA2�́A�`�悷��v���~�e�B�u

			//============================
			//�`��̒��������Ƃɖ߂�
			//============================

			//���C�g��L���ɖ߂�
			pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

			////Z�̔�r���@�ύX�B
			//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

			////Z�o�b�t�@�ɏ�������
			//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

			//�A���t�@�e�X�g�𖳌��ɖ߂�
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		}
	}
}

void SetOutletDisp(D3DXVECTOR3 pos, int nNumModel)
{
	for (int nCntOutlet = 0; nCntOutlet < MAX_OUTLETDISP; nCntOutlet++)
	{
		if (g_aOutletDisp[nCntOutlet].bUse == false)
		{
			g_aOutletDisp[nCntOutlet].pos = pos;//�ʒu
			g_aOutletDisp[nCntOutlet].nNumModel = nNumModel;//�ǂ̃��f��������������
			g_aOutletDisp[nCntOutlet].bUse = true;//�g�p���
			break;
		}
	}
}



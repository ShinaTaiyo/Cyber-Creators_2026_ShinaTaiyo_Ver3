//=========================================================
//
// �����L���O��ʂ̏���
// Author : Atsumu Kuboichi
//
//=========================================================
#include "main.h"
#include "ranking.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "score.h"
#include <stdio.h>
#include <string.h>

//=========================================================
//�}�N����`
//=========================================================
#define MAX_RANK (5)//�����N�C�����鏇�ʂ̐�
#define MAX_SCOREDIGIT (6)//�����N�C���������ʂ̃X�R�A�̍ő包��
#define RANKSCORE_SIZE_X (35.0f)//X�����̃X�R�A�\�L�̃T�C�Y
#define RANKSCORE_SIZE_Y (35.0f)//Y�����̃X�R�A�\�L�̃T�C�Y
#define RANKING_TEXT "data\\TEXTFILE\\Ranking.txt" //�ǂݍ��ރe�L�X�g�t�@�C��
//=========================================================
// �O���[�o���ϐ�
//=========================================================
typedef struct
{
	D3DXVECTOR3 pos;//�X�R�A�̂��ꂼ��̌���\��������ʒu
	int nScore;//���ꂼ��̏��ʂ̃X�R�A
	int nScorePosTexU;//�X�R�A�̃e�N�X�`���̈ʒu
	int nBlinkingPoint;//�X�R�A��_�ł����邽�߂̏����Ɋւ��|�C���g
	bool bBlinkingFlag;//�����N�C�������X�R�A��_�ł�����t���O
}RankScore;

LPDIRECT3DTEXTURE9 g_pTextureRank = NULL;//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureRankBG = NULL;//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRank = NULL;//���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRankBG = NULL;//���_�o�b�t�@�ւ̃|�C���^

RankScore g_aRankScore[MAX_RANK][MAX_SCOREDIGIT];//�����L���O�X�R�A���BMAX_RANK�́A�P�ʁ`�T�ʁAMAX_SCOREDIGIT�́A�����B
int g_nRankUpdate = -1;//�X�V�����NNo
int g_nTimerRanking;//�����L���O��ʕ\���^�C�}�[
int g_nBlinkingCount;
bool g_bUseRankScore[MAX_RANK];//�X�R�A���g���Ă��邩�ǂ���
//=========================================================
// ����������
//=========================================================
void InitRanking(void)
{
	InitBG();

	g_nBlinkingCount = 0;

	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\timer000.png",
		&g_pTextureRank);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_RANK * MAX_SCOREDIGIT,//���_�̐�
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRank,
		NULL);

	VERTEX_2D* pVtx;
	//============================================================
	//�����L���O�ݒ菈��
	//============================================================
	for (int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{//�\���̂̏��̏�����

		g_bUseRankScore[nCntRank] = false;//�X�R�A���g���Ă��邩�ǂ���

		for (int nCntDigit = 0; nCntDigit < MAX_SCOREDIGIT; nCntDigit++)
		{
			g_aRankScore[nCntRank][nCntDigit].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aRankScore[nCntRank][nCntDigit].nBlinkingPoint = 0;
			g_aRankScore[nCntRank][nCntDigit].bBlinkingFlag = false;//�X�R�A�������N�C��������A���̃X�R�A��_�ł�����t���O
			g_aRankScore[nCntRank][nCntDigit].nScorePosTexU = 0;
			
		}
	}

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffRank->Lock(0, 0, (void**)&pVtx, 0);


	for (int nCntRank = 0; nCntRank < MAX_RANK * MAX_SCOREDIGIT; nCntRank++)
	{

		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

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
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		pVtx += 4;//���_���W�̃|�C���^���C���N�������g
	}

	RankingPos(D3DXVECTOR3(800.0f, 270.0f, 0.0f), D3DXVECTOR3(1.8f, 1.8f, 0.0f));
	RankingPos(D3DXVECTOR3(580.0f, 445.0f, 0.0f), D3DXVECTOR3(1.1f, 1.1f, 0.0f));
	RankingPos(D3DXVECTOR3(1170.0f, 445.0f, 0.0f), D3DXVECTOR3(1.1f, 1.1f, 0.0f));
	RankingPos(D3DXVECTOR3(580.0f, 620.0f, 0.0f), D3DXVECTOR3(1.1f, 1.1f, 0.0f));
	RankingPos(D3DXVECTOR3(1170.0f, 620.0f, 0.0f), D3DXVECTOR3(1.1f, 1.1f, 0.0f));

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffRank->Unlock();
	SetRanking(GetScore());
}

//=========================================================
// �I������
//=========================================================
void UninitRanking(void)
{
	UninitBG();	//�w�i

	UninitScore();//�X�R�A
	//�e�N�X�`���̔j��
	if (g_pTextureRank != NULL)
	{
		g_pTextureRank->Release();
		g_pTextureRank = NULL;
	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffRank != NULL)
	{
		g_pVtxBuffRank->Release();
		g_pVtxBuffRank = NULL;
	}
}

//=========================================================
// �X�V����
//=========================================================
void UpdateRanking(void)
{
	UpdateBG();	//�w�i

	g_nBlinkingCount++;
	VERTEX_2D* pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffRank->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{
			for (int nCntDigit = 0; nCntDigit < MAX_SCOREDIGIT; nCntDigit++)
			{
				if (g_bUseRankScore[nCntRank] == true)
				{

					//============================================
					//�����N�C�������X�R�A�_�ŏ���
					//============================================
					if (g_aRankScore[nCntRank][nCntDigit].bBlinkingFlag == true)
					{
						if (g_nBlinkingCount == 1)
						{//�X�R�A�����N�C�����Ɉ�x�����A���艹��炷
							//PlaySound(SOUND_LABEL_SE_APPLAUSE_000);
						}
						if (g_nBlinkingCount % 5 == 0)
						{
							//0���_�J���[�̐ݒ�i�����ݒ�j
							pVtx[0].col = D3DCOLOR_RGBA(255, 0, 0, 255);
							pVtx[1].col = D3DCOLOR_RGBA(255, 0, 0, 255);
							pVtx[2].col = D3DCOLOR_RGBA(255, 0, 0, 255);
							pVtx[3].col = D3DCOLOR_RGBA(255, 0, 0, 255);
						}
						else
						{
							//���_�J���[�̐ݒ�i�����ݒ�j
							pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
							pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
							pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
							pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
						}
					}
				}
				pVtx += 4;//���_���W�̃|�C���^���C���N�������g
			}
	}
	g_pVtxBuffRank->Unlock();

	if (GetKeyboardTrigger(DIK_RETURN) == true || GetJoypadTrigger(JOYKEY_A) == true)
	{
		PlaySound(SOUND_LABEL_SE_DECIDE_000);

		StopSound(SOUND_LABEL_BGM014);

		SetFade(MODE_TITLE);
	}
}

//=========================================================
// �`�揈��
//=========================================================
void DrawRanking(void)
{
	DrawBG();	//�w�i

	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�ւ̃|�C���^

    //�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffRank, 0, sizeof(VERTEX_2D));//���_�o�b�t�@�ւ̃|�C���^�ƒ��_���̍\���̂̃T�C�Y

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntRank = 0; nCntRank < MAX_RANK * MAX_SCOREDIGIT; nCntRank++)
	{
		pDevice->SetTexture(0, g_pTextureRank);
		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntRank * 4, 2);//0�́A�`�悷��ŏ��̒��_�C���f�b�N�X�ŁA2�́A�`�悷��v���~�e�B�u��
	}
}

//=========================================================
//�����L���O�ݒ菈��
//=========================================================
void SetRanking(int nScore)
{
	VERTEX_2D* pVtx;

	FILE* pFile;//�t�@�C���|�C���^��錾

	int nData[MAX_RANK];//�����L���O�̃X�R�A�������ϐ�
	int nCopyData;//���l�����ւ���Ƃ��̌��̒l�̎󂯎M�p�ϐ�
	int nAdditionData;//�ǉ�����l
	int nResultScoreData[MAX_RANK];//�ŏI�I�Ƀ����N�C�������X�R�A�̏���������ϐ�
	//===============================================================
	//��񏈗����玝���Ă�������
	//===============================================================
	pFile = fopen(RANKING_TEXT, "r");//�G���[�h�~

	if (pFile != NULL)
	{
		for (int nCount = 0; nCount < MAX_RANK; nCount++)
		{
			fscanf(pFile, "%d", &nData[nCount]);


			nResultScoreData[nCount] = 0;//���傫��
		}
		fclose(pFile);//�t�@�C�������
	}
	nAdditionData = nScore;//����̃X�R�A

	if (nAdditionData > nData[MAX_RANK - 1])
	{//����̃X�R�A�������N�C�����Ă����X�R�A�̍ŏ��l���傫���ꍇ
		nData[MAX_RANK - 1] = nAdditionData;
	}

	for (int nCountData = 0; nCountData < MAX_RANK - 1; nCountData++)//��ׂ鐔�l�i�v�f�P�j��MAX_RANK-1�́A�Ō�̒l�̈�O�̒l�ƍŌ�̒l���ׂ�(�S���ׂ�j
	{
		for (int nCount = nCountData + 1; nCount < MAX_RANK; nCount++)//�v�f�Q�i��ׂ��l�̎��̒l����j��nCountData+1�́A�v�f�P�̎��̒l����Ƃ����Ӗ�
		{
			if (nData[nCountData] < nData[nCount])//�v�f�P���v�f�Q��菬�����Ƃ�
			{
				nCopyData = nData[nCountData];//�󂯎M���[nCopyData]
				nData[nCountData] = nData[nCount];//���l�̓���ւ��i��ڂ̐��l�̏��ʂ��A����̏��ʂ́A��ڂ̏��ʂɕς��B�j
				nData[nCount] = nCopyData;//���l�̓���ւ��i����ւ������ڂ̏��ʂɁA����ւ���O�̈�ڂ̏��ʂ̐��l���󂯎M�������B�j
			}
		}
	}

	pFile = fopen(RANKING_TEXT, "w");

	if (pFile != NULL)
	{//�t�@�C�����J�����ꍇ
		//�t�@�C���ɕ�����������o��
		for (int nCount = 0; nCount < MAX_RANK; nCount++)
		{
			fprintf(pFile, "%d\n", nData[nCount]);//���׏I������̂ŁA��ɏ��ʂ�ۑ�����
		}

		//�t�@�C�������
		fclose(pFile);
	}
	
	for (int nCount = 0; nCount < MAX_RANK; nCount++)
	{//	�O���[�o���ϐ��ɃX�R�A��ۑ�
		nResultScoreData[nCount] = nData[nCount];//�ŏI�I�ȃX�R�A�̑���i�����N�C�������X�R�A���������ꍇ���܂߂āj�B�i�\�����鏇�ʕ��j
	}

	//==============================================================================================================
	//�����L���O�̃X�R�A�̐����̈ʒu��A�����N�C�������X�R�A�����点�鏈��
	//==============================================================================================================
	for (int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)//�����N�C�������X�R�A�T���̓ǂݍ��݁B
	{
		for (int nCntDigit = 0; nCntDigit < MAX_SCOREDIGIT; nCntDigit++)
		{
			g_aRankScore[nCntRank][nCntDigit].nScorePosTexU = nData[nCntRank] % 10;//�e���ʂ̂��ꂼ��̃X�R�A�����ꂼ��̏��ʂ̂W�����ɑ������B(�����N�C�����鏇�ʂ̐���score.cpp�ł��������������B�j
			nData[nCntRank] = nData[nCntRank] / 10;
		}
	}

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffRank->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{
		for (int nCntDigit = 0; nCntDigit < MAX_SCOREDIGIT; nCntDigit++)
		{
			pVtx[0].tex = D3DXVECTOR2(0.0f + (0.1f * g_aRankScore[nCntRank][nCntDigit].nScorePosTexU), 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f + (0.1f * g_aRankScore[nCntRank][nCntDigit].nScorePosTexU), 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f + (0.1f * g_aRankScore[nCntRank][nCntDigit].nScorePosTexU), 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f + (0.1f * g_aRankScore[nCntRank][nCntDigit].nScorePosTexU), 1.0f);

			//===========================================================================================================================================================================================
			//���点�鏈��
			//===========================================================================================================================================================================================
			if (nResultScoreData[nCntRank] == nScore)
			{//�����N�C�������X�R�A�̒��ɁA�������̂��Ȃ������X�R�A�̎�
				g_aRankScore[nCntRank][nCntDigit].bBlinkingFlag = true;
			}

			pVtx += 4;//���_�f�[�^�̃|�C���^���S���i�߂�B
		}
	}
	g_pVtxBuffRank->Unlock();
}

void RankingPos(D3DXVECTOR3 pos, D3DXVECTOR3 Scale)//�����L���O�̃X�R�A�\���̈ʒu�����߂�
{
	VERTEX_2D* pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffRank->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{
			for (int nCntDigit = 0; nCntDigit < MAX_SCOREDIGIT; nCntDigit++)
			{
				if (g_bUseRankScore[nCntRank] == false)
				{
					
					if (nCntRank == 0)
					{
						g_aRankScore[nCntRank][nCntDigit].pos = D3DXVECTOR3(pos.x - 100.0f * nCntDigit, pos.y, 0.0f);
					}
					else
					{
						g_aRankScore[nCntRank][nCntDigit].pos = D3DXVECTOR3(pos.x - 65.0f * nCntDigit, pos.y, 0.0f);

					}
					pVtx[0].pos = D3DXVECTOR3(g_aRankScore[nCntRank][nCntDigit].pos.x - (RANKSCORE_SIZE_X * Scale.x), g_aRankScore[nCntRank][nCntDigit].pos.y - (RANKSCORE_SIZE_Y * Scale.y), 0.0f);
					pVtx[1].pos = D3DXVECTOR3(g_aRankScore[nCntRank][nCntDigit].pos.x + (RANKSCORE_SIZE_X * Scale.x), g_aRankScore[nCntRank][nCntDigit].pos.y - (RANKSCORE_SIZE_Y * Scale.y), 0.0f);
					pVtx[2].pos = D3DXVECTOR3(g_aRankScore[nCntRank][nCntDigit].pos.x - (RANKSCORE_SIZE_X * Scale.x), g_aRankScore[nCntRank][nCntDigit].pos.y + (RANKSCORE_SIZE_Y * Scale.y), 0.0f);
					pVtx[3].pos = D3DXVECTOR3(g_aRankScore[nCntRank][nCntDigit].pos.x + (RANKSCORE_SIZE_X * Scale.x), g_aRankScore[nCntRank][nCntDigit].pos.y + (RANKSCORE_SIZE_Y * Scale.y), 0.0f);

				}
					pVtx += 4;
			}

			if (g_bUseRankScore[nCntRank] == false)
			{
				g_bUseRankScore[nCntRank] = true;
				break;
			}
	}

	g_pVtxBuffRank->Unlock();
}


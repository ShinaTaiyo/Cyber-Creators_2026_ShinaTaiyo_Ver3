//========================================================
//
// ���U���g�̃X�R�A�̏���
// Author : Atsumu Kuboichi
//
//=========================================================
#include "resultscore.h"
#include "score.h"
#include "player.h"

//=========================================================
// �O���[�o���ϐ�
//=========================================================
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResultScore = NULL;
LPDIRECT3DTEXTURE9 g_pTextureResultScore = NULL;				//�e�N�X�`���ւ̃|�C���^
ResultScore g_aResultScore[NUM_RESULTSCORE][MAX_RESULTSCORE];	//�\���̏��
int g_nNumResutScore;											//����

//=========================================================
// ����������
//=========================================================
void InitResultScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�̎擾
	VERTEX_2D* pVtx;						//���_���ւ̃|�C���^
	
	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\timer000.png",
		&g_pTextureResultScore);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * (NUM_RESULTSCORE * MAX_RESULTSCORE),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffResultScore,
		NULL);

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffResultScore->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntNumScore = 0; nCntNumScore < NUM_RESULTSCORE; nCntNumScore++)
	{
		for (int nCntResultScore = 0; nCntResultScore < MAX_RESULTSCORE; nCntResultScore++)
		{
			switch (nCntNumScore)
			{
			case 0:	//�Q�[���X�R�A
				g_aResultScore[nCntNumScore][nCntResultScore].fWidth = 100.0f;	//����
				g_aResultScore[nCntNumScore][nCntResultScore].pos = D3DXVECTOR3(630.0f + (g_aResultScore[nCntNumScore][nCntResultScore].fWidth * nCntResultScore), 230.0f, 0.0f);	//�ʒu
				g_aResultScore[nCntNumScore][nCntResultScore].bUse = true;	//�g�p����
				break;

			case 1:	//�G���j��
				g_aResultScore[nCntNumScore][nCntResultScore].fWidth = 90.0f;	//����
				g_aResultScore[nCntNumScore][nCntResultScore].pos = D3DXVECTOR3(530.0f + (g_aResultScore[nCntNumScore][nCntResultScore].fWidth * nCntResultScore), 470.0f, 0.0f);	//�ʒu

				if (nCntResultScore < MAX_RESULTSCORE - MAX_RESULTENEMY)
				{
					g_aResultScore[nCntNumScore][nCntResultScore].bUse = false;	//�g�p���Ȃ�
				}
				else
				{
					g_aResultScore[nCntNumScore][nCntResultScore].bUse = true;	//�g�p����
				}
				break;

			case 2:	//�X�N���b�v�����
				g_aResultScore[nCntNumScore][nCntResultScore].fWidth = 90.0f;	//����
				g_aResultScore[nCntNumScore][nCntResultScore].pos = D3DXVECTOR3(590.0f + (g_aResultScore[nCntNumScore][nCntResultScore].fWidth * nCntResultScore), 660.0f, 0.0f);	//�ʒu

				if (nCntResultScore < MAX_RESULTSCORE - MAX_RESULTSCRAP)
				{
					g_aResultScore[nCntNumScore][nCntResultScore].bUse = false;	//�g�p���Ȃ�
				}
				else
				{
					g_aResultScore[nCntNumScore][nCntResultScore].bUse = true;	//�g�p����
				}
				break;

			default:
				break;
			}

			g_aResultScore[nCntNumScore][nCntResultScore].fHeight = g_aResultScore[nCntNumScore][nCntResultScore].fWidth * 2.0f;//����

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aResultScore[nCntNumScore][nCntResultScore].pos.x - g_aResultScore[nCntNumScore][nCntResultScore].fWidth / 2.0f,
				g_aResultScore[nCntNumScore][nCntResultScore].pos.y - g_aResultScore[nCntNumScore][nCntResultScore].fWidth / 2.0f,						
				0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aResultScore[nCntNumScore][nCntResultScore].pos.x + g_aResultScore[nCntNumScore][nCntResultScore].fWidth / 2.0f, 
				g_aResultScore[nCntNumScore][nCntResultScore].pos.y - g_aResultScore[nCntNumScore][nCntResultScore].fWidth / 2.0f, 
				0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aResultScore[nCntNumScore][nCntResultScore].pos.x - g_aResultScore[nCntNumScore][nCntResultScore].fWidth / 2.0f, 
				g_aResultScore[nCntNumScore][nCntResultScore].pos.y + g_aResultScore[nCntNumScore][nCntResultScore].fWidth / 2.0f, 
				0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aResultScore[nCntNumScore][nCntResultScore].pos.x + g_aResultScore[nCntNumScore][nCntResultScore].fWidth / 2.0f, 
				g_aResultScore[nCntNumScore][nCntResultScore].pos.y + g_aResultScore[nCntNumScore][nCntResultScore].fWidth / 2.0f, 
				0.0f);

			//rhw�̐ݒ�
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			//���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//�e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

			pVtx += 4;	//���_�f�[�^�̃|�C���^���S���i�߂�B
		}
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffResultScore->Unlock();
}

//=========================================================
// �I������
//=========================================================
void UninitResultScore(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureResultScore != NULL)
	{//�e�N�X�`����񂪑��݂���ꍇ
		g_pTextureResultScore->Release();
		g_pTextureResultScore = NULL;
	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffResultScore != NULL)
	{//���_��񂪑��݂���ꍇ
		g_pVtxBuffResultScore->Release();
		g_pVtxBuffResultScore = NULL;
	}
}

//=========================================================
// �X�V����
//=========================================================
void UpdateResultScore(void)
{
	VERTEX_2D* pVtx;								//���_���ւ̃|�C���^
	int aPosTexU[NUM_RESULTSCORE][MAX_RESULTSCORE];	//�e���̐������i�[
	int nMulti = 1;									//�e���̔{��
	int nScore = 0;									//�X�R�A

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffResultScore->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntNumScore = 0; nCntNumScore < NUM_RESULTSCORE; nCntNumScore++)
	{
		switch (nCntNumScore)
		{//�擾����X�R�A
		case 0:	//�Q�[���X�R�A
			nScore = GetScore();//�Q�[���X�R�A�̎擾
			break;

		case 1:	//�G���j��
			nScore = GetPlayer()->nTotalDefeatEnemy;
			break;

		case 2:	//�X�N���b�v�����
			nScore = GetPlayer()->nTotalGetScrap;
			break;

		default:
			break;
		}

		//�X�R�A�������ƂɎ擾
		for (int nCntResultScore = 1; nCntResultScore <= MAX_RESULTSCORE; nCntResultScore++)
		{
			nMulti *= 10;
			aPosTexU[nCntNumScore][MAX_SCORE - nCntResultScore] = (nScore % nMulti) / (nMulti / 10);
		}

		nMulti = 1;	//�e���̔{�����Z�b�g

		for (int nCntResultScore = 0; nCntResultScore < MAX_RESULTSCORE; nCntResultScore++)
		{
			//�e�N�X�`�����W�̍X�V
			pVtx[0].tex = D3DXVECTOR2(aPosTexU[nCntNumScore][nCntResultScore] / 10.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2((aPosTexU[nCntNumScore][nCntResultScore] + 1) / 10.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(aPosTexU[nCntNumScore][nCntResultScore] / 10.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2((aPosTexU[nCntNumScore][nCntResultScore] + 1) / 10.0f, 1.0f);

			//���_�f�[�^�̃|�C���^��i�߂�
			pVtx += 4;
		}
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffResultScore->Unlock();
}

//=========================================================
// �`�揈��
//=========================================================
void DrawResultScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�̎擾

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffResultScore, 0, sizeof(VERTEX_2D));	//���_�o�b�t�@�ւ̃|�C���^�ƒ��_���̍\���̂̃T�C�Y

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntNumScore = 0; nCntNumScore < NUM_RESULTSCORE; nCntNumScore++)
	{
		for (int nCntResultScore = 0; nCntResultScore < MAX_RESULTSCORE; nCntResultScore++)
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureResultScore);

			if (g_aResultScore[nCntNumScore][nCntResultScore].bUse)
			{//�Y�����錅���g�p����Ă���ꍇ
				//�|���S���̕`��
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntResultScore + (nCntNumScore * MAX_RESULTSCORE)) * 4, 2);
			}
		}
	}
}

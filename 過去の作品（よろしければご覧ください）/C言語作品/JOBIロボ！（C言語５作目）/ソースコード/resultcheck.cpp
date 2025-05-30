//=======================================================================================================================================================================================================================
//
// ���U���g�̃~�b�V�����`�F�b�N����
// Author : Taiyo Shina
//
//=======================================================================================================================================================================================================================
#include "resultcheck.h"
#include "mission.h"
#include "score.h"

//=======================================================================================================================================================================================================================
// �O���[�o���ϐ�
//=======================================================================================================================================================================================================================
PDIRECT3DVERTEXBUFFER9 g_pVtxBuffResultCheck = NULL;//���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureResultCheck = NULL;	//�e�N�X�`���ւ̃|�C���^
ResultCheck g_aResultCheck[MAX_MISSION];		//���̍\����

//=======================================================================================================================================================================================================================
// ����������
//=======================================================================================================================================================================================================================
void InitResultCheck(void)
{
	VERTEX_2D* pVtx;						//���_���ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�̎擾
	Mission* pMission = GetMission();		//�~�b�V�����̏����擾

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\ResultCheck000.png",
		&g_pTextureResultCheck);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_MISSION,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffResultCheck,
		NULL);

	//===========================================
	//�~�b�V����0
	//===========================================
	if (pMission->aMission0.bMissionClear)
	{//�~�b�V�����B���ς݂̏ꍇ
		g_aResultCheck[0].bUse = true;//�g�p����
		AddScore(10000);//�{�[�i�X�X�R�A�����Z
	}
	else
	{//���B���̏ꍇ
		g_aResultCheck[0].bUse = false;//�g�p���Ȃ�
	}

	//===========================================
	//�~�b�V����1
	//===========================================
	if (pMission->aMission1.bMissionClear)
	{//�~�b�V�����B���ς݂̏ꍇ
		g_aResultCheck[1].bUse = true;//�g�p����
		AddScore(10000);//�{�[�i�X�X�R�A�����Z
	}
	else
	{//���B���̏ꍇ
		g_aResultCheck[1].bUse = false;//�g�p���Ȃ�
	}

	//===========================================
	//�~�b�V����2
	//===========================================
	if (pMission->aMission2.bMissionClear)
	{//�~�b�V�����B���ς݂̏ꍇ
		g_aResultCheck[2].bUse = true;//�g�p����
		AddScore(10000);//�{�[�i�X�X�R�A�����Z
	}
	else
	{//���B���̏ꍇ
		g_aResultCheck[2].bUse = false;//�g�p���Ȃ�
	}

	//===========================================
	//�~�b�V����3
	//===========================================
	if (pMission->aMission3.bMissionClear)
	{//�~�b�V�����B���ς݂̏ꍇ
		g_aResultCheck[3].bUse = true;//�g�p����
		AddScore(10000);//�{�[�i�X�X�R�A�����Z
	}
	else
	{//���B���̏ꍇ
		g_aResultCheck[3].bUse = false;//�g�p���Ȃ�
	}

	//===========================================
	//�~�b�V����4
	//===========================================
	if (pMission->aMission4.bMissionClear)
	{//�~�b�V�����B���ς݂̏ꍇ
		g_aResultCheck[4].bUse = true;//�g�p����
		AddScore(10000);//�{�[�i�X�X�R�A�����Z
	}
	else
	{//���B���̏ꍇ
		g_aResultCheck[4].bUse = false;//�g�p���Ȃ�
	}

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffResultCheck->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntCheck = 0; nCntCheck < MAX_MISSION; nCntCheck++)
	{
		g_aResultCheck[nCntCheck].pos = D3DXVECTOR3(520.0f, 445.0f + nCntCheck * 58.0f, 0.0f);	//�ʒu

		for (int nCntVtx = 0; nCntVtx < 4; nCntVtx++)
		{
			//rhw�̐ݒ�
			pVtx[nCntVtx].rhw = 1.0f;

			//���_�J���[�̐ݒ�
			pVtx[nCntVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aResultCheck[nCntCheck].pos.x - RESULTCHECK_WIDTH * 0.5f, g_aResultCheck[nCntCheck].pos.y - RESULTCHECK_HEIGHT * 0.5f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aResultCheck[nCntCheck].pos.x + RESULTCHECK_WIDTH * 0.5f, g_aResultCheck[nCntCheck].pos.y - RESULTCHECK_HEIGHT * 0.5f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aResultCheck[nCntCheck].pos.x - RESULTCHECK_WIDTH * 0.5f, g_aResultCheck[nCntCheck].pos.y + RESULTCHECK_HEIGHT * 0.5f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aResultCheck[nCntCheck].pos.x + RESULTCHECK_WIDTH * 0.5f, g_aResultCheck[nCntCheck].pos.y + RESULTCHECK_HEIGHT * 0.5f, 0.0f);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffResultCheck->Unlock();
}

//=======================================================================================================================================================================================================================
// �I������
//=======================================================================================================================================================================================================================
void UninitResultCheck(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureResultCheck != NULL)
	{//�e�N�X�`����񂪑��݂���ꍇ
		g_pTextureResultCheck->Release();
		g_pTextureResultCheck = NULL;
	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffResultCheck != NULL)
	{//���_��񂪑��݂���ꍇ
		g_pVtxBuffResultCheck->Release();
		g_pVtxBuffResultCheck = NULL;
	}
}

//=======================================================================================================================================================================================================================
// �X�V����
//=======================================================================================================================================================================================================================
void UpdateResultCheck(void)
{

}

//=======================================================================================================================================================================================================================
// �`�揈��
//=======================================================================================================================================================================================================================
void DrawResultCheck(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();									//�f�o�C�X�̎擾
	pDevice->SetStreamSource(0, g_pVtxBuffResultCheck, 0, sizeof(VERTEX_2D));	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);												//���_�t�H�[�}�b�g�̐ݒ�

	for (int nCntCheck = 0; nCntCheck < MAX_MISSION; nCntCheck++)
	{
		if (g_aResultCheck[nCntCheck].bUse)
		{//�g�p����Ă���ꍇ
			pDevice->SetTexture(0, g_pTextureResultCheck);					//�e�N�X�`���̐ݒ�
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntCheck * 4, 2);	//�|���S���̕`��
		}
	}
}

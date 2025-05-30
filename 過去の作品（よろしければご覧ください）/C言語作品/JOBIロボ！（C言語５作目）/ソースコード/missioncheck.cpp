//=======================================================================================================================================================================================================================
//
// �`�F�b�N�}�[�N�̏���
// Author : Atsumu Kuboichi
//
//=======================================================================================================================================================================================================================
#include "missioncheck.h"
#include "input.h"
#include "mission.h"

//=======================================================================================================================================================================================================================
// �O���[�o���ϐ�
//=======================================================================================================================================================================================================================
PDIRECT3DVERTEXBUFFER9 g_pVtxBuffMissionCheck = NULL;//���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureMissionCheck[NUM_CHECK] = {};//�e�N�X�`���ւ̃|�C���^
MissionCheck g_aMissionCheck[MAX_MISSION];//���̍\����

//=======================================================================================================================================================================================================================
// ����������
//=======================================================================================================================================================================================================================
void InitMissionCheck(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�̎擾

	//�e�N�X�`���̓ǂݍ���
	for (int nCntTex = 0; nCntTex < NUM_CHECK; nCntTex++)
	{
		D3DXCreateTextureFromFile(pDevice,
			CHECK_TEXTURE[nCntTex],
			&g_pTextureMissionCheck[nCntTex]);
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_MISSION,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMissionCheck,
		NULL);

	//���_���ւ̃|�C���^
	VERTEX_2D* pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffMissionCheck->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntCheck = 0; nCntCheck < MAX_MISSION; nCntCheck++)
	{
		g_aMissionCheck[nCntCheck].pos = D3DXVECTOR3(282.0f, 70.0f + nCntCheck * 38.0f, 0.0f);	//�ʒu
		g_aMissionCheck[nCntCheck].nType = -1;//���
		g_aMissionCheck[nCntCheck].nDisplayTime = 0;//�\������
		g_aMissionCheck[nCntCheck].fScale = 0.0f;//����
		g_aMissionCheck[nCntCheck].bUse = false;//�g�p���Ȃ�

		for (int nCntVtx = 0; nCntVtx < 4; nCntVtx++)
		{
			pVtx[nCntVtx].rhw = 1.0f;//rhw�̐ݒ�
			pVtx[nCntVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//���_�J���[�̐ݒ�
		}

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aMissionCheck[nCntCheck].pos.x - g_aMissionCheck[nCntCheck].fScale * 0.5f, g_aMissionCheck[nCntCheck].pos.y - g_aMissionCheck[nCntCheck].fScale * 0.5f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aMissionCheck[nCntCheck].pos.x + g_aMissionCheck[nCntCheck].fScale * 0.5f, g_aMissionCheck[nCntCheck].pos.y - g_aMissionCheck[nCntCheck].fScale * 0.5f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aMissionCheck[nCntCheck].pos.x - g_aMissionCheck[nCntCheck].fScale * 0.5f, g_aMissionCheck[nCntCheck].pos.y + g_aMissionCheck[nCntCheck].fScale * 0.5f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aMissionCheck[nCntCheck].pos.x + g_aMissionCheck[nCntCheck].fScale * 0.5f, g_aMissionCheck[nCntCheck].pos.y + g_aMissionCheck[nCntCheck].fScale * 0.5f, 0.0f);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffMissionCheck->Unlock();
}

//=======================================================================================================================================================================================================================
// �I������
//=======================================================================================================================================================================================================================
void UninitMissionCheck(void)
{
	//�e�N�X�`���̔j��
	for (int nCntTex = 0; nCntTex < NUM_CHECK; nCntTex++)
	{
		if (g_pTextureMissionCheck[nCntTex] != NULL)
		{//�e�N�X�`����񂪑��݂���ꍇ
			g_pTextureMissionCheck[nCntTex]->Release();
			g_pTextureMissionCheck[nCntTex] = NULL;
		}
	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffMissionCheck != NULL)
	{//���_��񂪑��݂���ꍇ
		g_pVtxBuffMissionCheck->Release();
		g_pVtxBuffMissionCheck = NULL;
	}
}

//=======================================================================================================================================================================================================================
// �X�V����
//=======================================================================================================================================================================================================================
void UpdateMissionCheck(void)
{
	for (int nCntCheck = 0; nCntCheck < MAX_MISSION; nCntCheck++)
	{
		if (g_aMissionCheck[nCntCheck].bUse)
		{//�g�p���Ă���ꍇ
			//�\�����Ԃ�����
			g_aMissionCheck[nCntCheck].nDisplayTime--;
			
			if (g_aMissionCheck[nCntCheck].nDisplayTime <= 0)
			{//�\�����Ԃ�0�ɂȂ����ꍇ
				g_aMissionCheck[nCntCheck].bUse = false;//�g�p���Ȃ�
			}
		}
	}
}

//=======================================================================================================================================================================================================================
// �`�揈��
//=======================================================================================================================================================================================================================
void DrawMissionCheck(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();									//�f�o�C�X�̎擾
	pDevice->SetStreamSource(0, g_pVtxBuffMissionCheck, 0, sizeof(VERTEX_2D));	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);												//���_�t�H�[�}�b�g�̐ݒ�

	for (int nCntCheck = 0; nCntCheck < MAX_MISSION; nCntCheck++)
	{
		if (g_aMissionCheck[nCntCheck].bUse)
		{//�g�p����Ă���ꍇ
			pDevice->SetTexture(0, g_pTextureMissionCheck[g_aMissionCheck[nCntCheck].nType]);//�e�N�X�`���̐ݒ�
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntCheck * 4, 2);//�|���S���̕`��
		}
	}
}

//=======================================================================================================================================================================================================================
// �ݒ菈��
//=======================================================================================================================================================================================================================
void SetMissionCheck(D3DXVECTOR3 pos, int nType, int nDisplatTime, float fScale)
{
	//���_���ւ̃|�C���^
	VERTEX_2D* pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffMissionCheck->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntCheck = 0; nCntCheck < MAX_MISSION; nCntCheck++)
	{
		if (!g_aMissionCheck[nCntCheck].bUse)
		{//�g�p���Ă��Ȃ��ꍇ
			g_aMissionCheck[nCntCheck].bUse = true;//�g�p����
			g_aMissionCheck[nCntCheck].pos = pos;//�ʒu
			g_aMissionCheck[nCntCheck].nType = nType;//���
			g_aMissionCheck[nCntCheck].nDisplayTime = nDisplatTime;//�\������
			g_aMissionCheck[nCntCheck].fScale = fScale;//�傫��

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aMissionCheck[nCntCheck].pos.x - g_aMissionCheck[nCntCheck].fScale * 0.5f, g_aMissionCheck[nCntCheck].pos.y - g_aMissionCheck[nCntCheck].fScale * 0.5f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aMissionCheck[nCntCheck].pos.x + g_aMissionCheck[nCntCheck].fScale * 0.5f, g_aMissionCheck[nCntCheck].pos.y - g_aMissionCheck[nCntCheck].fScale * 0.5f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aMissionCheck[nCntCheck].pos.x - g_aMissionCheck[nCntCheck].fScale * 0.5f, g_aMissionCheck[nCntCheck].pos.y + g_aMissionCheck[nCntCheck].fScale * 0.5f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aMissionCheck[nCntCheck].pos.x + g_aMissionCheck[nCntCheck].fScale * 0.5f, g_aMissionCheck[nCntCheck].pos.y + g_aMissionCheck[nCntCheck].fScale * 0.5f, 0.0f);

			break;
		}

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffMissionCheck->Unlock();
}

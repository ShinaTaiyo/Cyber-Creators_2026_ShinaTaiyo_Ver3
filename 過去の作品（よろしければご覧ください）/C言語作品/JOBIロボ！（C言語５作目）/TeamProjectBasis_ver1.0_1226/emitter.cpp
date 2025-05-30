//========================================================
//
//�X���Q�U���F�n�b�J�\���̊�Ս��[Emitter.cpp]
//Author:ShinaTaiyo
//
//=========================================================

#include "emitter.h"
#include "main.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "shadow.h"
#include "effect.h"
#include "attackmodel.h"
#include "boss.h"
#define EMITTER_SCALE (10.0f)
//�O���[�o���ϐ�
D3DXVECTOR3 g_posEmitter;//�ʒu
//D3DXVECTOR3 g_rotEmitter;//����
D3DXMATRIX g_mtxWorldEmitter;//���[���h�}�g���b�N�X(float4*4)

Emitter g_aEmitter[MAX_EMITTER];//�e�̍\���̏��



//�^�C�g����ʂ̏���������
void InitEmitter(void)
{

	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	for (int nCntEmitter = 0; nCntEmitter < MAX_EMITTER; nCntEmitter++)
	{
		g_aEmitter[nCntEmitter].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�ʒu
		g_aEmitter[nCntEmitter].SupportPos = NULL_VECTOR3;//���߂��ʒu�ŏ������������Ƃ��Ɏg��
		g_aEmitter[nCntEmitter].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�ړ���
		g_aEmitter[nCntEmitter].AddMove = NULL_VECTOR3;//�ړ��ʂ�ǉ�����
		g_aEmitter[nCntEmitter].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//����
		g_aEmitter[nCntEmitter].nLife = 0;//�̗�
		g_aEmitter[nCntEmitter].fRWidth = 0.0f;//���i���a�j
		g_aEmitter[nCntEmitter].fRHeight = 0.0f;//�����i���a�j
		g_aEmitter[nCntEmitter].nType = 0;//���
		g_aEmitter[nCntEmitter].bUse = false;//�g�p���Ă��邩�ǂ���
		g_aEmitter[nCntEmitter].nIdxShadow = -1;//�e�̃C���f�b�N�X
		g_aEmitter[nCntEmitter].fScale = 0.0f;//�傫��
		g_aEmitter[nCntEmitter].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);//�F
		g_aEmitter[nCntEmitter].fRedution = 0.0f;//�G�t�F�N�g������������X�s�[�h
		g_aEmitter[nCntEmitter].EmitterType = EMITTERTYPE_EFFECT;//�G�~�b�^�[�ŌĂяo�������̎��
		g_aEmitter[nCntEmitter].nSummonLife = 0;//�Ăяo�������̗̑�
		g_aEmitter[nCntEmitter].SummonMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEmitter[nCntEmitter].fRotMove = 0.0f;//���������̐��l�����߂�
		g_aEmitter[nCntEmitter].nNumAttack = -1;//�G�~�b�^�[���Ăяo���Ă���U���̔ԍ����i�[����
		g_aEmitter[nCntEmitter].fLength = 0.0f;//����
		g_aEmitter[nCntEmitter].nTargetType = 0;//�ǂ̃��f���𒆐S�ɃG�~�b�^�[��������
		g_aEmitter[nCntEmitter].fVXaim3D = 0.0f;
		g_aEmitter[nCntEmitter].fVYaim3D = 0.0f;
		g_aEmitter[nCntEmitter].fVZaim3D = 0.0f;
		g_aEmitter[nCntEmitter].fVLaim3D = 0.0f;
		g_aEmitter[nCntEmitter].nCntTime = 0;//�G�~�b�^�[���o�����Ă���̎��Ԃ��J�E���g����
	}
}

//==============================
//�^�C�g����ʂ̏I������
//==============================
void UninitEmitter(void)
{

}
//============================
//�^�C�g����ʂ̍X�V����
//============================
void UpdateEmitter(void)
{

	for (int nCntEmitter = 0; nCntEmitter < MAX_EMITTER; nCntEmitter++)
	{

		if (g_aEmitter[nCntEmitter].bUse == true)
		{
			AttackModel* pAttackModel = GetAttackModel();
			Boss* pBoss = GetBoss();
			//�̗͂����炷
			g_aEmitter[nCntEmitter].nLife -= 1;
			g_aEmitter[nCntEmitter].nCntTime++;

			if (g_aEmitter[nCntEmitter].nType == EMITTERTYPE_CURVE)
			{
				g_aEmitter[nCntEmitter].fRotMove += 0.005f;

				g_aEmitter[nCntEmitter].move.x = sinf(D3DX_PI * g_aEmitter[nCntEmitter].fRotMove) * 5.0f;
				g_aEmitter[nCntEmitter].move.z = cosf(D3DX_PI * g_aEmitter[nCntEmitter].fRotMove) * 5.0f;
			}
			else if (g_aEmitter[nCntEmitter].nType == EMITTERTYPE_TORNADO)
			{
				if (g_aEmitter[nCntEmitter].nTargetType == EMITTERTARGET_ATTACKMODEL)
				{
					for (int nCntAttackModel = 0; nCntAttackModel < MAX_ATTACKMODEL; nCntAttackModel++, pAttackModel++)
					{
						if (pAttackModel->bUse == true && nCntAttackModel == g_aEmitter[nCntEmitter].nNumAttack && pAttackModel->nType == ATTACKMODEL05_TORNADO)
						{
							g_aEmitter[nCntEmitter].fRotMove += 0.1f;

							g_aEmitter[nCntEmitter].pos.x = sinf(D3DX_PI * g_aEmitter[nCntEmitter].fRotMove) * 100.0f + pAttackModel->pos.x;
							g_aEmitter[nCntEmitter].pos.z = cosf(D3DX_PI * g_aEmitter[nCntEmitter].fRotMove) * 100.0f + pAttackModel->pos.z;
						}
					}
				}
				else if (g_aEmitter[nCntEmitter].nTargetType == EMITTERTARGET_BOSS)
				{
					if (pBoss->bUse == true)
					{
						if (g_aEmitter[nCntEmitter].nNumAttack == 0)
						{
							g_aEmitter[nCntEmitter].fRotMove += 0.1f;
						}
						else
						{
							g_aEmitter[nCntEmitter].fRotMove += -0.1f;
						}

						g_aEmitter[nCntEmitter].fLength *= 1.03f;

						g_aEmitter[nCntEmitter].pos.x = sinf(D3DX_PI * g_aEmitter[nCntEmitter].fRotMove) * g_aEmitter[nCntEmitter].fLength + g_aEmitter[nCntEmitter].SupportPos.x;
						g_aEmitter[nCntEmitter].pos.z = cosf(D3DX_PI * g_aEmitter[nCntEmitter].fRotMove) * g_aEmitter[nCntEmitter].fLength + g_aEmitter[nCntEmitter].SupportPos.z;
					}
				}

			}
			else if (g_aEmitter[nCntEmitter].nType == EMITTERTYPE_ROTATION)
			{
				for (int nCntAttackModel = 0; nCntAttackModel < MAX_ATTACKMODEL; nCntAttackModel++, pAttackModel++)
				{
					if (pAttackModel->bUse == true && nCntAttackModel == g_aEmitter[nCntEmitter].nNumAttack)
					{
						g_aEmitter[nCntEmitter].fRotMove += 0.04f;
						g_aEmitter[nCntEmitter].pos.x = sinf(D3DX_PI * g_aEmitter[nCntEmitter].fRotMove) * g_aEmitter[nCntEmitter].fLength + g_aEmitter[nCntEmitter].SupportPos.x;
						g_aEmitter[nCntEmitter].pos.z = cosf(D3DX_PI * g_aEmitter[nCntEmitter].fRotMove) * g_aEmitter[nCntEmitter].fLength + g_aEmitter[nCntEmitter].SupportPos.z;
						break;
					}
					else if (pAttackModel->bUse == false && nCntAttackModel == g_aEmitter[nCntEmitter].nNumAttack)
					{
						g_aEmitter[nCntEmitter].bUse = false;
						break;
					}
				}
			}
			else if (g_aEmitter[nCntEmitter].nType == EMITTERTYPE_GATHERING)
			{
				g_aEmitter[nCntEmitter].move *= 1.1f;

				if (g_aEmitter[nCntEmitter].pos.x >= g_aEmitter[nCntEmitter].SupportPos.x - 61.0f &&
					g_aEmitter[nCntEmitter].pos.x <= g_aEmitter[nCntEmitter].SupportPos.x + 61.0f &&
					g_aEmitter[nCntEmitter].pos.y >= g_aEmitter[nCntEmitter].SupportPos.y - 61.0f &&
					g_aEmitter[nCntEmitter].pos.y <= g_aEmitter[nCntEmitter].SupportPos.y + 61.0f &&
					g_aEmitter[nCntEmitter].pos.z >= g_aEmitter[nCntEmitter].SupportPos.z - 61.0f &&
					g_aEmitter[nCntEmitter].pos.z <= g_aEmitter[nCntEmitter].SupportPos.z + 61.0f)
				{
					g_aEmitter[nCntEmitter].bUse = false;
				}
			}
			else if (g_aEmitter[nCntEmitter].nType == EMITTERTYPE_ACCELEHORMING)
			{
				if (g_aEmitter[nCntEmitter].nCntTime == 1)
				{
					g_aEmitter[nCntEmitter].AddMove = ONE_VECTOR3;
				}
				g_aEmitter[nCntEmitter].AddMove *= 1.15f;
				D3DXVECTOR3 BulletMove = NULL_VECTOR3;
				D3DXVECTOR3 SaveMove = NULL_VECTOR3;

				for (int nCntModel = 0; nCntModel < MAX_ATTACKMODEL; nCntModel++, pAttackModel++)
				{
					if (pAttackModel->bUse == true && nCntModel == g_aEmitter[nCntEmitter].nNumAttack)
					{
						g_aEmitter[nCntEmitter].fVXaim3D = pAttackModel->SupportPos.x - g_aEmitter[nCntEmitter].pos.x;
						g_aEmitter[nCntEmitter].fVYaim3D = pAttackModel->SupportPos.y - g_aEmitter[nCntEmitter].pos.y;
						g_aEmitter[nCntEmitter].fVZaim3D = pAttackModel->SupportPos.z - g_aEmitter[nCntEmitter].pos.z;
						g_aEmitter[nCntEmitter].fVLaim3D = sqrtf((g_aEmitter[nCntEmitter].fVXaim3D * g_aEmitter[nCntEmitter].fVXaim3D) +
							(g_aEmitter[nCntEmitter].fVYaim3D * g_aEmitter[nCntEmitter].fVYaim3D) +
							(g_aEmitter[nCntEmitter].fVZaim3D * g_aEmitter[nCntEmitter].fVZaim3D));

						BulletMove.x = g_aEmitter[nCntEmitter].fVXaim3D / g_aEmitter[nCntEmitter].fVLaim3D * g_aEmitter[nCntEmitter].AddMove.x;
						BulletMove.y = g_aEmitter[nCntEmitter].fVYaim3D / g_aEmitter[nCntEmitter].fVLaim3D * g_aEmitter[nCntEmitter].AddMove.y;
						BulletMove.z = g_aEmitter[nCntEmitter].fVZaim3D / g_aEmitter[nCntEmitter].fVLaim3D * g_aEmitter[nCntEmitter].AddMove.z;

						break;
					}
				}

				g_aEmitter[nCntEmitter].move += BulletMove;

			}

			SetEffect(EFFECTTYPE_NORMAL, g_aEmitter[nCntEmitter].nSummonLife, g_aEmitter[nCntEmitter].fScale, g_aEmitter[nCntEmitter].fScale, g_aEmitter[nCntEmitter].fRedution,
				g_aEmitter[nCntEmitter].pos, NULL_VECTOR3, g_aEmitter[nCntEmitter].col,0.0f,0);

			//�ړ�����
			g_aEmitter[nCntEmitter].pos += g_aEmitter[nCntEmitter].move;

			if (g_aEmitter[nCntEmitter].nLife <= 0)
			{
				g_aEmitter[nCntEmitter].bUse = false;
			}

		}
			

	}
		
}



//================================
//�^�C�g����ʂ̕`�揈��
//================================
void DrawEmitter(void)
{

}

void SetEmitterEffect(int nType, int nTargetType, int nLife, int nSummonLife, float fScale, float fRedution, 
	D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRotMove, int nNumAttack, float fLength, D3DXVECTOR3 SupportPos)
{
	for (int nCntEmitter = 0; nCntEmitter < MAX_EMITTER; nCntEmitter++)
	{
		if (g_aEmitter[nCntEmitter].bUse == false)
		{
			g_aEmitter[nCntEmitter].bUse = true;//�g�p���Ă��邩�ǂ���
			g_aEmitter[nCntEmitter].pos = pos;//�ʒu
			g_aEmitter[nCntEmitter].move = move;//�ړ���
			g_aEmitter[nCntEmitter].AddMove = NULL_VECTOR3;//�ړ��ʂ�ǉ�����
			g_aEmitter[nCntEmitter].col = col;//�F
			g_aEmitter[nCntEmitter].nLife = nLife;//�̗�
			g_aEmitter[nCntEmitter].nType = nType;//���
			g_aEmitter[nCntEmitter].nTargetType = nTargetType;//�ǂ̃��f���𒆐S�ɃG�~�b�^�[��������
			g_aEmitter[nCntEmitter].fScale = fScale;//�傫��
			g_aEmitter[nCntEmitter].fRedution = fRedution;//�G�t�F�N�g������������X�s�[�h
			g_aEmitter[nCntEmitter].nSummonLife = nSummonLife;//�Ăяo�������̗̑�
			g_aEmitter[nCntEmitter].fRotMove = fRotMove;//���������̐��l�����߂�
			g_aEmitter[nCntEmitter].nNumAttack = nNumAttack;//�G�~�b�^�[���Ăяo���Ă���U���̔ԍ����i�[����
			g_aEmitter[nCntEmitter].fLength = fLength;//����
			g_aEmitter[nCntEmitter].SupportPos = SupportPos;//���߂��ʒu�ŏ��������鎞�Ɏg��
			g_aEmitter[nCntEmitter].fVXaim3D = 0.0f;
			g_aEmitter[nCntEmitter].fVYaim3D = 0.0f;
			g_aEmitter[nCntEmitter].fVZaim3D = 0.0f;
			g_aEmitter[nCntEmitter].fVLaim3D = 0.0f;
			g_aEmitter[nCntEmitter].nCntTime = 0;//�G�~�b�^�[���o�����Ă���̎��Ԃ��J�E���g����
			break;
		}
	}
}

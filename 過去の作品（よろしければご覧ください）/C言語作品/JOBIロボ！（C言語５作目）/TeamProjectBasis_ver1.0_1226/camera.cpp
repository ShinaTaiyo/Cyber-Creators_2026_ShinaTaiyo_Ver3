//========================================================
//
// �J�����̏���
// Author : ShinaTaiyo
//
//=========================================================
#include "camera.h"
#include "main.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "player.h"
#include "game.h"
#include "editEnemy.h"
#include "boss.h"
#include "emitter.h"
#include "field.h"

//=========================================================
// �O���[�o���ϐ�
//=========================================================
Camera g_Camera;//�J�����̏��
float g_fCameraPosVX;//�J������X�̎x�_�̈ʒu��ς���
float g_fCameraPosVY;//�J������Y�̎x�_�̈ʒu��ς���
float g_fCameraPosVZ;//�J������Z�̎x�_�̈ʒu��ς���
float g_fLength;//�J�����ƃv���C���[�̋���
float g_fAngle;//�J�����ƃv���C���[�̊p�x
int g_nCameraDelay;//�f�B���C�J�E���g

//=========================================================
// ����������
//=========================================================
void InitCamera(void)
{
	Player* pPlayer = GetPlayer();//�v���C���[�̍\���̂̏����擾

	g_nCameraDelay = 0;//�f�B���C�J�E���g
	g_Camera.PosV = D3DXVECTOR3(pPlayer->pos.x, 0.0f,800.0f);//��납�猩��̂ŁA�v���C���[��X�̏����n�_�����߂�Α��v�iY,Z�͒萔(���_����̋���)
	g_Camera.PosR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�����_
	g_Camera.VecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);//������x�N�g��		
	g_Camera.DecayRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�J�����̌�����ς��鎞�Ɍ��������鏈��
	g_Camera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Camera.move = NULL_VECTOR3;//�ړ���
	g_Camera.center = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�J�����ƃv���C���[�Ԃ̒��ԓ_
	g_Camera.fXVaim = 0.0f;
	g_Camera.fZVaim = 0.0f;
	g_Camera.fRot = 0.0f;
	g_Camera.nModeOld = 0;//1f�O�̃J�������[�h
	g_Camera.nCntModeTime = 0;//���̃J�������[�h�ɂȂ��Ă���̎���
	g_Camera.nMode = CAMERAMODE_REVOLUTION;//���]���[�h�ɏ���������
	g_Camera.bBossAppear = false;//�{�X���o��������g��
	g_Camera.fVXaim3D = 0.0f;//X�����x�N�g��
	g_Camera.fVYaim3D = 0.0f;//Y�����x�N�g��
	g_Camera.fVZaim3D = 0.0f;//Z�����x�N�g��
	g_Camera.fVLaim3D = 0.0f;//�����x�N�g��
}

//==============================
// �I������
//==============================
void UninitCamera(void)
{
	
}

//============================
// �X�V����
//============================
void UpdateCamera(void)
{
	g_nCameraDelay++;//�f�B���C�J�E���g

	Boss* pBoss = GetBoss();//�{�X�̍\���̂̏����擾
	Player* pPlayer = GetPlayer();//�v���C���[�̍\���̂̏����擾
	EnemyEditor* pEnemyEditor = GetEnemyEditor();//�G�G�f�B�^�̏����擾

	g_Camera.nModeOld = g_Camera.nMode;//1f�O�̃J�������[�h

	//===========================
	//�J�������[�h�����߂�
	//===========================
	if (GetMode() == MODE_TITLE)
	{
		g_Camera.nMode = CAMERAMODE_ROTATION;//���]���[�h
	}
	else if (GetMode() == MODE_GAME)
	{
		if (g_nCameraDelay == 1)
		{
			g_Camera.nMode = CAMERAMODE_BIRDSEYE;
		}

		if (pBoss->bUse == true && g_Camera.bBossAppear == false)
		{//�{�X���o�����A�{�X�o������t���O��false��������J�������[�h����x�����{�X�o�����o���[�h�ɂ���
			g_Camera.bBossAppear = true;
			g_Camera.nMode = CAMERAMODE_BOSSAPPEAR;
		}
	}

	if (GetMode() == MODE_TUTORIAL)
	{
		g_Camera.nMode = CAMERAMODE_REVOLUTION;
	}

	if (g_Camera.nModeOld != g_Camera.nMode)
	{//�J�������[�h��1f�O�ƈ������A���[�h�̃J�E���g���O�ɂ���
		g_Camera.nCntModeTime = 0;
		g_Camera.fRot = D3DX_PI;
	}

	//====================================================
	//�ʏ�J�������[�h
	//====================================================
	if (g_Camera.nMode == CAMERAMODE_REVOLUTION)
	{
		//posV.y��posV.z�������_����̋����ɂȂ邽�߁i�J�����͒��~�_�̌�납�猩�邩��j�AposV.y��posV.z��sqrtf����,�����Ƃ������������߂�B
		g_Camera.fLength = sqrtf(GAME_POSV_Y * GAME_POSV_Y + GAME_POSV_Z * GAME_POSV_Z) / 2.0f;

		//���_�̈ʒu���X�V
		g_Camera.PosV = D3DXVECTOR3(pPlayer->pos.x, pPlayer->pos.y + GAME_POSV_Y, pPlayer->pos.z - GAME_POSV_Z);

		//�J�����ƃv���C���[�Ԃ̒��ԓ_�����߂�
		g_Camera.center = D3DXVECTOR3(pPlayer->pos.x, (g_Camera.PosV.y - pPlayer->pos.y) / 2.0f, (pPlayer->pos.z - g_Camera.PosV.z) / 2.0f);

		if (GetEditEnemyMode() == false)
		{
			//�����_���v���C���[�̈ʒu�ɍX�V
			g_Camera.PosR.x = pPlayer->pos.x;
			g_Camera.PosR.y = pPlayer->vtxMax.y * 4.0f + pPlayer->pos.y;
			g_Camera.PosR.z = pPlayer->pos.z;

			//���_�̈ʒu�̍X�V
			g_Camera.PosV.x = sinf(g_Camera.rot.y + D3DX_PI) * g_Camera.fLength + pPlayer->pos.x;
			g_Camera.PosV.z = cosf(g_Camera.rot.y + D3DX_PI) * g_Camera.fLength + pPlayer->pos.z;

			//�J�����ƃv���C���[�Ԃ̒��ԓ_�����߂�
			g_Camera.center = D3DXVECTOR3((g_Camera.PosV.x - pPlayer->pos.x) / 2.0f, (g_Camera.PosV.y - pPlayer->pos.y) / 2.0f, (g_Camera.PosV.z - pPlayer->pos.z) / 2.0f);
		}
		else
		{//�G�G�f�B�^���[�h�������ꍇ
			g_Camera.PosR.x = pEnemyEditor->pos.x;
			g_Camera.PosR.y = pEnemyEditor->pos.y;
			g_Camera.PosR.z = pEnemyEditor->pos.z;

			//���_�̈ʒu�̍X�V
			g_Camera.PosV.x = sinf(g_Camera.rot.y + D3DX_PI) * g_Camera.fLength + pEnemyEditor->pos.x;
			g_Camera.PosV.z = cosf(g_Camera.rot.y + D3DX_PI) * g_Camera.fLength + pEnemyEditor->pos.z;

			//�J�����ƃv���C���[�Ԃ̒��ԓ_�����߂�
			g_Camera.center = D3DXVECTOR3((g_Camera.PosV.x - pPlayer->pos.x) / 2.0f, (g_Camera.PosV.y - pPlayer->pos.y) / 2.0f, (g_Camera.PosV.z - pPlayer->pos.z) / 2.0f);
		}
	}
	
	//=========================================
	//�v���C���[���_���n�����[�h
	//=========================================
	else if (g_Camera.nMode == CAMERAMODE_ROTATION)
	{
		if (GetMode() == MODE_TITLE)
		{
			g_Camera.rot = D3DXVECTOR3(10.0f, D3DX_PI, 0.0f);
			g_Camera.PosV = D3DXVECTOR3(pPlayer->pos.x - 25.0f, 35.0f, pPlayer->pos.z + 110.0f);

			//posV.y��posV.z�������_����̋����ɂȂ邽�߁i�J�����͒��~�_�̌�납�猩�邩��j�AposV.y��posV.z��sqrtf����,�����Ƃ������������߂�B
			g_Camera.fLength = sqrtf(TITLE_POSV_Y * TITLE_POSV_Y + TITLE_POSV_Z * TITLE_POSV_Z) / 2.0f;

			g_Camera.PosR.x = -sinf(g_Camera.rot.x) * g_Camera.fLength + g_Camera.PosV.x;
			g_Camera.PosR.y = -tanf(g_Camera.rot.y) * g_Camera.fLength + g_Camera.PosV.y;
			g_Camera.PosR.z = -cosf(g_Camera.rot.z) * g_Camera.fLength + g_Camera.PosV.z;
		}
		else
		{
			g_Camera.PosR = D3DXVECTOR3(sinf(D3DX_PI + g_Camera.rot.y) * g_Camera.fLength + g_Camera.PosV.x,
				0.0f,
				cosf(D3DX_PI + g_Camera.rot.y) * g_Camera.fLength + g_Camera.PosV.z);//���_
		}
	}
	
	//================================================
	//���Վ��_���[�h
	//================================================
	if (g_Camera.nMode == CAMERAMODE_BIRDSEYE)
	{
		if (g_nCameraDelay >= 1 && g_nCameraDelay <= 360)
		{
			g_Camera.fRot += ((D3DX_PI * 2.0f) / 360.0f);

			g_Camera.PosR.x = sinf(g_Camera.fRot) * 1500.0f;
			g_Camera.PosR.y = 0.0f;
			g_Camera.PosR.z = cosf(g_Camera.fRot) * 1500.0f;

			//���_�̈ʒu�̍X�V
			g_Camera.PosV.x = 0.0f;
			g_Camera.PosV.z = 10.0f;
			g_Camera.PosV.y = 1300.0f;
		}
		else if (g_nCameraDelay >= 361)
		{
			g_Camera.fRot = 0.0f;

			//=============================================
			//���_���ړ�������ڕW�̈ʒu�����߂�
			//=============================================
			D3DXVECTOR3 targetpos;
			targetpos.x = sinf(g_Camera.rot.y + D3DX_PI) * g_Camera.fLength + pPlayer->pos.x;
			targetpos.y = pPlayer->pos.y + GAME_POSV_Y;
			targetpos.z = cosf(g_Camera.rot.y + D3DX_PI) * g_Camera.fLength + pPlayer->pos.z;

			g_Camera.PosR = pPlayer->pos;

			//==========================
	        //�R�������@�_������
	        //==========================
			g_Camera.fVXaim3D = targetpos.x - g_Camera.PosV.x;
			g_Camera.fVYaim3D = targetpos.y - g_Camera.PosV.y;
			g_Camera.fVZaim3D = targetpos.z - g_Camera.PosV.z;

			D3DXVECTOR3 CameraPosVmove = NULL_VECTOR3;

			g_Camera.fVLaim3D = sqrtf((g_Camera.fVXaim3D * g_Camera.fVXaim3D) + (g_Camera.fVYaim3D * g_Camera.fVYaim3D)
				+ (g_Camera.fVZaim3D * g_Camera.fVZaim3D));

			CameraPosVmove.x = g_Camera.fVXaim3D / g_Camera.fVLaim3D * 30.0f;
			CameraPosVmove.y = g_Camera.fVYaim3D / g_Camera.fVLaim3D * 30.0f;
			CameraPosVmove.z = g_Camera.fVZaim3D / g_Camera.fVLaim3D * 30.0f;

			g_Camera.move = CameraPosVmove;
			g_Camera.PosV += g_Camera.move;

			if (g_Camera.PosV.x >= targetpos.x - 30.0f && g_Camera.PosV.x <= targetpos.x + 30.0f &&
				g_Camera.PosV.y >= targetpos.y - 30.0f && g_Camera.PosV.y <= targetpos.y + 30.0f &&
				g_Camera.PosV.z >= targetpos.z - 30.0f && g_Camera.PosV.z <= targetpos.z + 30.0f)
			{
				g_Camera.nMode = CAMERAMODE_REVOLUTION;//�ړI�̈ʒu�ɒB�����̂ŁA�J�������[�h��ς���
			}		
		}

		//�J�����ƃv���C���[�Ԃ̒��ԓ_�����߂�
		g_Camera.center = D3DXVECTOR3((g_Camera.PosV.x - pPlayer->pos.x) / 2.0f, (g_Camera.PosV.y - pPlayer->pos.y) / 2.0f, (g_Camera.PosV.z - pPlayer->pos.z) / 2.0f);
	}

	//=============================================
	//�{�X�o�����o���[�h
	//=============================================
	else if (g_Camera.nMode == CAMERAMODE_BOSSAPPEAR)
	{
		g_Camera.nCntModeTime++;

		if (g_Camera.nCntModeTime == 1)
		{
			pBoss->pos.y = -BOSS_SETPOS_Y;
			SetStagingField(STAGINGFIELD00_MAGICCIRCLE, 720, 300.0f, 300.0f, 0.12f, D3DXVECTOR3(pBoss->pos.x, 1.0f, pBoss->pos.z), NULL_VECTOR3);
		}

		g_Camera.PosR = D3DXVECTOR3(pBoss->pos.x,0.0f,pBoss->pos.z);

		if (g_Camera.nCntModeTime >= 0 && g_Camera.nCntModeTime <= BOSSAPPEAR_TIME)
		{//300f�̊ԁA�{�X�̎�����J����������
			g_Camera.fRot += ((D3DX_PI * 2.0f) / BOSSAPPEAR_TIME);
			pBoss->pos.y += (BOSS_SETPOS_Y / (float)(BOSSAPPEAR_TIME));

			g_Camera.PosV.x = sinf(g_Camera.fRot) * 400.0f + g_Camera.PosR.x;
			g_Camera.PosV.y = 200.0f + g_Camera.PosR.y;
			g_Camera.PosV.z = cosf(g_Camera.fRot) * 400.0f + g_Camera.PosR.z;

			float fColR = float(rand() % 100 + 1) / 100.0f;
			float fColG = float(rand() % 100 + 1) / 100.0f;
			float fColB = float(rand() % 100 + 1) / 100.0f;
			float fRandRot = float(rand() % 200 + 1) / 100.0f;
			int nLottery = rand() % 2;

			if (g_Camera.nCntModeTime % 4 == 0)
			{
				SetEmitterEffect(EMITTERTYPE_TORNADO, EMITTERTARGET_BOSS, 300, 50, 35.0f, 0.5f, NULL_VECTOR3,D3DXVECTOR3(0.0f,10.0f,0.0f),
					D3DXCOLOR(fColR, fColG, fColB, 1.0f), fRandRot, nLottery, 100.0f, D3DXVECTOR3(pBoss->pos.x, 0.0f, pBoss->pos.z));
			}
		}

		if (g_Camera.nCntModeTime >= BOSSAPPEAR_TIME + 60)
		{//360f�ȍ~�A�v���C���[�̏��ɃJ�������ړ������A�v���C���[�̋߂��ɗ�����{�X�o�����o���[�h���I��
			D3DXVECTOR3 targetpos;

			targetpos.x = sinf(g_Camera.rot.y + D3DX_PI) * g_Camera.fLength + pPlayer->pos.x;
			targetpos.y = pPlayer->pos.y + GAME_POSV_Y;
			targetpos.z = cosf(g_Camera.rot.y + D3DX_PI) * g_Camera.fLength + pPlayer->pos.z;
			g_Camera.PosR = D3DXVECTOR3(pBoss->pos.x,pBoss->pos.y + pBoss->Size.y / 2.0f,pBoss->pos.z);

			//==========================
			//�R�������@�_������
			//==========================
			g_Camera.fVXaim3D = targetpos.x - g_Camera.PosV.x;
			g_Camera.fVYaim3D = targetpos.y - g_Camera.PosV.y;
			g_Camera.fVZaim3D = targetpos.z - g_Camera.PosV.z;

			D3DXVECTOR3 CameraPosVmove = NULL_VECTOR3;

			g_Camera.fVLaim3D = sqrtf((g_Camera.fVXaim3D * g_Camera.fVXaim3D) + (g_Camera.fVYaim3D * g_Camera.fVYaim3D)
				+ (g_Camera.fVZaim3D * g_Camera.fVZaim3D));

			CameraPosVmove.x = g_Camera.fVXaim3D / g_Camera.fVLaim3D * 50.0f;
			CameraPosVmove.y = g_Camera.fVYaim3D / g_Camera.fVLaim3D * 50.0f;
			CameraPosVmove.z = g_Camera.fVZaim3D / g_Camera.fVLaim3D * 50.0f;

			g_Camera.move = CameraPosVmove;
			g_Camera.PosV += g_Camera.move;

			if (g_Camera.PosV.x >= targetpos.x - 30.0f && g_Camera.PosV.x <= targetpos.x + 30.0f &&
				g_Camera.PosV.y >= targetpos.y - 30.0f && g_Camera.PosV.y <= targetpos.y + 30.0f &&
				g_Camera.PosV.z >= targetpos.z - 30.0f && g_Camera.PosV.z <= targetpos.z + 30.0f)
			{
				g_Camera.nMode = CAMERAMODE_REVOLUTION;//�ړI�̈ʒu�ɒB�����̂ŁA�J�������[�h��ς���
				pBoss->bBossAppear = false;//�{�X�o�����o���I�����
				g_Camera.nMode = CAMERAMODE_REVOLUTION;
			}
		}
	}

	//==============================================
	//�ʏ탂�[�h
	//==============================================
	if (g_Camera.nMode == CAMERAMODE_REVOLUTION)
	{//�ʏ탂�[�h�܂ŃJ�����̑�����󂯕t���Ȃ��B
		if (GetKeyboardPress(DIK_O))
		{
			g_fCameraPosVY += 5.0f;
		}
		if (GetKeyboardPress(DIK_L))
		{
			g_fCameraPosVY -= 5.0f;
		}

		if (GetKeyboardPress(DIK_U) || GetJoypadPress(JOYKEY_RB) || (float)(GetStick().abAnglePress[STICKTYPE_RIGHT][STICKANGLE_RIGHT]) > 0.1f)
		{
			g_Camera.DecayRot.y += 0.08f;
		}
		if (GetKeyboardPress(DIK_J) || GetJoypadPress(JOYKEY_LB) || (float)(GetStick().abAnglePress[STICKTYPE_RIGHT][STICKANGLE_LEFT]) > 0.1f)
		{
			g_Camera.DecayRot.y -= 0.08f;
		}

		if (GetKeyboardTrigger(DIK_C))
		{
			g_Camera.nMode++;

			if (g_Camera.nMode == 2)
			{
				g_Camera.nMode = 0;
			}
		}

		if (GetKeyboardPress(DIK_Z))
		{
			g_Camera.fLength += 10.0f;
		}
		if (GetKeyboardPress(DIK_X))
		{
			g_Camera.fLength -= 10.0f;
		}
	}

	g_Camera.DecayRot.y += (0.0f - g_Camera.DecayRot.y) * 0.5f;//�J�����̌�����ς��鎞�Ɍ��������鏈���i���� = Decay�j
	g_Camera.rot.y += g_Camera.DecayRot.y;//�J�����̌�����ς���
}

//================================
// �`�揈��
//================================
void SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_Camera.mtxProjection);

	//�v���W�F�N�V�����}�g���b�N�X���쐬
	D3DXMatrixPerspectiveFovLH(&g_Camera.mtxProjection,
		D3DXToRadian(45.0f),
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
		10.0f,
		9000.0f);

	//�v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &g_Camera.mtxProjection);

	//�r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_Camera.mtxView);

	//�r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&g_Camera.mtxView,
		&g_Camera.PosV,
		&g_Camera.PosR,
		&g_Camera.VecU);

	//�r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &g_Camera.mtxView);
}

//====================
// �J�������̎擾
//====================
Camera* GetCamera(void)
{
	return &g_Camera;
}

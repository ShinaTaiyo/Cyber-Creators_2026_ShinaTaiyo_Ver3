//========================================================
//
//�X���Q�U���F�n�b�J�\���̊�Ս��[Camera.cpp]
//Author:ShinaTaiyo
//
//=========================================================

#include "camera.h"
#include "main.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "player.h"
//�O���[�o���ϐ�
//PDIRECT3DVERTEXBUFFER9 g_pVtxBuffCamera = NULL;//���_�o�b�t�@�ւ̃|�C���^
//LPDIRECT3DTEXTURE9 g_pTextureCamera = NULL;//�e�N�X�`���ւ̃|�C���^
//D3DXVECTOR3 g_posCamera;//�ʒu
//D3DXVECTOR3 g_rotCamera;//����
//D3DXMATRIX g_mtxWorldCamera;//���[���h�}�g���b�N�X(float4*4)

Camera g_Camera;//�J�����̏��

float g_fCameraPosVX;//�J������X�̎x�_�̈ʒu��ς���
float g_fCameraPosVY;//�J������Y�̎x�_�̈ʒu��ς���
float g_fCameraPosVZ;//�J������Z�̎x�_�̈ʒu��ς���

float g_fLength;//�J�����ƃv���C���[�̋���
float g_fAngle;//�J�����ƃv���C���[�̊p�x

//�^�C�g����ʂ̏���������
void InitCamera(void)
{
	Player* pPlayer = GetPlayer();

	g_fLength = 0.0f;
	g_fAngle = 0.0f;
	g_fCameraPosVX = 0.0f;//�J������X�̎x�_�̈ʒu��ς���
	g_fCameraPosVY = 350.0f;//�J������Y�̎x�_�̈ʒu��ς���
	g_fCameraPosVZ = 0.0f;//�J������Z�̎x�_�̈ʒu��ς���

	g_Camera.PosV = D3DXVECTOR3(pPlayer->pos.x, 200.0f,400.0f);//��납�猩��̂ŁA�v���C���[��X�̏����n�_�����߂�Α��v�iY,Z�͒萔(���_����̋���)
	g_Camera.PosR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Camera.targetPosV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�ړI�̃J�����̎��_
	g_Camera.targetPosR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�ړI�̃J�����̒����_
	g_Camera.VecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	g_Camera.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Camera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Camera.fXVaim = 0.0f;
	g_Camera.fZVaim = 0.0f;
	//posV.y��posV.z�������_����̋����ɂȂ邽�߁i�J�����͒��~�_�̌�납�猩�邩��j�AposV.y��posV.z��sqrtf����,�����Ƃ������������߂�B
	g_Camera.fLength = 550.0f /*sqrtf(g_Camera.PosV.y * g_Camera.PosV.y + g_Camera.PosV.z * g_Camera.PosV.z) / 2*/;
	g_Camera.nMode = CAMERAMODE_REVOLUTION;//���]���[�h�ɏ���������
}

//==============================
//�^�C�g����ʂ̏I������
//==============================
void UninitCamera(void)
{
	
}
//============================
//�^�C�g����ʂ̍X�V����
//============================
void UpdateCamera(void)
{

	////�Ίp���̒������Z�o����
	//g_fLengthPlayer = sqrtf(PLAYER_WIDTH * PLAYER_WIDTH + PLAYER_HEIGHT * PLAYER_HEIGHT) / 2.0f;//�v���C���[�̕�2��{�v���C���[�̍����Q��/�Q

	////�Ίp���̊p�x���Z�o����
	//g_fAnglePlayer = atan2f(PLAYER_WIDTH, PLAYER_HEIGHT);
	/*pVtx[0].pos.x = g_aPlayer.pos.x + sinf(g_aPlayer.rot.z  + g_fAnglePlayer*-0.75) * g_fLengthPlayer;
	pVtx[0].pos.y = g_aPlayer.pos.y +cosf(g_aPlayer.rot.z + g_fAnglePlayer*-0.75) * g_fLengthPlayer;
	pVtx[0].pos.z= 0.0f;

	pVtx[1].pos.x = g_aPlayer.pos.x + sinf(g_aPlayer.rot.z + g_fAnglePlayer*0.75) * g_fLengthPlayer;
	pVtx[1].pos.y = g_aPlayer.pos.y + cosf(g_aPlayer.rot.z + g_fAnglePlayer*-0.75) * g_fLengthPlayer;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_aPlayer.pos.x + sinf(g_aPlayer.rot.z + g_fAnglePlayer*-0.25) * g_fLengthPlayer;
	pVtx[2].pos.y = g_aPlayer.pos.y + cosf(g_aPlayer.rot.z + g_fAnglePlayer*+0.25) * g_fLengthPlayer;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_aPlayer.pos.x + sinf(g_aPlayer.rot.z + g_fAnglePlayer*0.25) * g_fLengthPlayer;
	pVtx[3].pos.y = g_aPlayer.pos.y + cosf(g_aPlayer.rot.z + g_fAnglePlayer*0.25) * g_fLengthPlayer;
	pVtx[3].pos.z = 0.0f;*/

	Player* pPlayer = GetPlayer();

	
	//g_Camera.rot.y;

	/*g_Camera.PosV = D3DXVECTOR3(pPlayer->pos.x + g_fCameraPosVX, 200.0f + pPlayer->pos.y + g_fCameraPosVY,-200.0f + pPlayer->pos.z + g_fCameraPosVZ);
	g_Camera.PosR = D3DXVECTOR3(pPlayer->pos.x,pPlayer->pos.y,pPlayer->pos.z);
	g_Camera.VecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);*/


	//g_fXVaim = pPlayer->pos.x - g_aEnemy[nCntEnemy].pos.x;//X���W�ւ̃x�N�g��
	//g_fYVaim = pPlayer->pos.y - g_aEnemy[nCntEnemy].pos.y;//Y���W�ւ̃x�N�g��
	//g_Vaim[nCntEnemy] = atan2(g_fXVaim[nCntEnemy], g_fYVaim[nCntEnemy]);//�e���������̌���

	//g_Camera;
	
//	g_Camera.PosV;
//
	//g_Camera.PosV = D3DXVECTOR3(sinf(-D3DX_PI * g_Camera.rot.y) * g_fLength, 200.0f, cosf(-D3DX_PI * g_Camera.rot.y) * g_fLength);
	//g_Camera.PosR = D3DXVECTOR3(0.0f,0.0f,0.0f);
	//g_Camera.VecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//if (GetKeyboardPress(DIK_RIGHT) == true)
	//{
	//	g_fCameraPosVX += 5.0f;
	//	//g_Camera.PosV.x++;
	//}
	//if (GetKeyboardPress(DIK_LEFT) == true)
	//{
	//	g_fCameraPosVX -= 5.0f;
	//	//g_Camera.PosV.x--;
	//}
	//if (GetKeyboardPress(DIK_UP) == true)
	//{
	//	g_fCameraPosVZ += 5.0f;
	//	//g_Camera.PosV.z++;
	//}
	//if (GetKeyboardPress(DIK_DOWN) == true)
	//{
	//	g_fCameraPosVZ -= 5.0f;
	//	//g_Camera.PosV.z--;
	//}
	if (GetKeyboardPress(DIK_O) == true)
	{
		g_fCameraPosVY += 5.0f;
	/*	g_Camera.PosV.y++;*/
	}
	if (GetKeyboardPress(DIK_L) == true)
	{
		g_fCameraPosVY -= 5.0f;
	/*	g_Camera.PosV.y--;*/
	}
	if (GetKeyboardPress(DIK_U) == true || GetJoypadPress(JOYKEY_R1) == true)
	{
	/*	g_Camera.rot.y += 0.1f;*/
		g_Camera.move.y += 0.1f;
	}
	if (GetKeyboardPress(DIK_J) == true || GetJoypadPress(JOYKEY_L1) == true)
	{
	/*	g_Camera.rot.y += -0.1f;*/
		g_Camera.move.y += -0.1f;
	}

	if (GetKeyboardTrigger(DIK_C) == true)
	{
		g_Camera.nMode++;
	}

	if (GetKeyboardPress(DIK_Z) == true)
	{
		g_Camera.fLength += 10.0f;
	}
	if (GetKeyboardPress(DIK_X) == true)
	{
		g_Camera.fLength -= 10.0f;
	}


	if (g_Camera.nMode == 2)
	{
		g_Camera.nMode = 0;
	}

	//////�ړ��ʂ��X�V�i����������j
 //   g_Camera.move.x += (0.0f - g_Camera.rot.y) * 0.1f;

	//g_aPlayer.move.y += -2.5f;
	//g_aPlayer.move.y += (0.0f - g_aPlayer.move.y) * 0.5f;
	//g_aPlayer.move.z += (0.0f - g_aPlayer.move.z) * 0.5f;
	//g_aPlayer.move.y += (0.0f - g_aPlayer.move.y) * 0.5f;

	g_Camera.move.y += (0.0f - g_Camera.move.y) * 0.5f;

	g_Camera.rot.y += g_Camera.move.y;




	//float fLengthX = (g_Camera.PosR.x - g_fCameraPosVX);
	//float fLengthZ = (g_Camera.PosR.z - g_fCameraPosVZ);
	//g_Camera.fLength = sqrt(fLengthX * fLengthX + fLengthZ * fLengthZ) / 2;//�������o���I

	if (g_Camera.nMode == CAMERAMODE_REVOLUTION)
	{
		g_Camera.targetPosR = D3DXVECTOR3(pPlayer->pos.x, pPlayer->pos.y, pPlayer->pos.z);//�����_
		g_Camera.targetPosV = D3DXVECTOR3(sinf(D3DX_PI + g_Camera.rot.y) * g_Camera.fLength + g_Camera.PosR.x,//��]������Ƃ������Ƃ́AZ�̈ʒu�����łȂ��AX�̈ʒu��
																									   //�ς��Ƃ������ƁB�̂ɁA���̎����g���B���_�͒����_����̋���
			200.0f + g_Camera.PosR.y + g_fCameraPosVY,
			cosf(D3DX_PI + g_Camera.rot.y) * g_Camera.fLength + g_Camera.PosR.z);//���_
		g_Camera.VecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);//������x�N�g��


		//===============
		//�����ړ�����
		//===============
		g_Camera.PosR.x += (g_Camera.targetPosR.x - g_Camera.PosR.x) * 0.1f;
		g_Camera.PosR.y += (g_Camera.targetPosR.y - g_Camera.PosR.y) * 0.1f;
		g_Camera.PosR.z += (g_Camera.targetPosR.z - g_Camera.PosR.z) * 0.1f;

		g_Camera.PosV.x += (g_Camera.targetPosV.x - g_Camera.PosV.x) * 0.1f;
		g_Camera.PosV.y += (g_Camera.targetPosV.y - g_Camera.PosV.y) * 0.1f;
		g_Camera.PosV.z += (g_Camera.targetPosV.z - g_Camera.PosV.z) * 0.1f;
		//==========================================================================
	}
	
	if (g_Camera.nMode == CAMERAMODE_ROTATION)
	{
		g_Camera.PosR = D3DXVECTOR3(sinf(D3DX_PI + g_Camera.rot.y) * g_Camera.fLength + g_Camera.PosV.x,//��]������Ƃ������Ƃ́AZ�̈ʒu�����łȂ��AX�̈ʒu��
																										   //�ς��Ƃ������ƁB�̂ɁA���̎����g���B���_�͒����_����̋���
		    0.0f,
			cosf(D3DX_PI + g_Camera.rot.y) * g_Camera.fLength + g_Camera.PosV.z);//���_
		g_Camera.PosV = g_Camera.PosV;//��]������Ƃ������Ƃ́AZ�̈ʒu�����łȂ��AX�̈ʒu��
		g_Camera.VecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);//������x�N�g��
	}

}



//================================
//�^�C�g����ʂ̕`�揈��
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
		4500.0f);

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
//�J�������̎擾
//====================
Camera* GetCamera(void)
{
	return &g_Camera;
}

//void SetPosCamera(D3DXVECTOR3 PosR, D3DXVECTOR3 PosV)
//{
//	g_Camera.targetPosR = PosR;
//	g_Camera.targetPosV = PosV;
//}

//=======================================================================================================================================================================================================================
//
// �J�����̏���
// Author : Atsumu Kuboichi
//
//=======================================================================================================================================================================================================================
#include "editcamera.h"
#include "player.h"
#include "game.h"
#include "input.h"
#include "edit.h"

//=======================================================================================================================================================================================================================
// �O���[�o���ϐ�
//=======================================================================================================================================================================================================================
EditCamera g_EditCamera;	//�J�����̏��

//=======================================================================================================================================================================================================================
// ����������
//=======================================================================================================================================================================================================================
void InitEditCamera(void)
{
	g_EditCamera.posV = D3DXVECTOR3(0.0f, EDITCAMERA_POSV_Y, EDITCAMERA_POSV_Z);	//���_�ʒu
	g_EditCamera.posR = D3DXVECTOR3(0.0f, 140.0f, 0.0f);	//�����_�ʒu
	g_EditCamera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		//�x�N�g��(�����)
	g_EditCamera.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ړ���
	g_EditCamera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//����
	g_EditCamera.nTurn = 1;									//��]���@
}

//=======================================================================================================================================================================================================================
// �I������
//=======================================================================================================================================================================================================================
void UninitEditCamera(void)
{

}

//=======================================================================================================================================================================================================================
// �X�V����
//=======================================================================================================================================================================================================================
void UpdateEditCamera(void)
{
	//���_���璍���_�܂ł̋���
	g_EditCamera.fLength = sqrtf(g_EditCamera.posV.y * g_EditCamera.posV.y + g_EditCamera.posV.z * g_EditCamera.posV.z) / 2.0f;

	//============================================================================
	// ��]
	//============================================================================
	if (GetKeyboardPress(DIK_RIGHT) == true)//���L�[�������ꂽ�ꍇ
	{//�E��]
		g_EditCamera.rot.y += EDITCAMERA_ROLL;
	}
	if (GetKeyboardPress(DIK_LEFT) == true)	//���L�[�������ꂽ�ꍇ
	{//����]
		g_EditCamera.rot.y -= EDITCAMERA_ROLL;
	}
	if (GetKeyboardPress(DIK_UP) == true) //���L�[�������ꂽ
	{//���]
		g_EditCamera.rot.x += EDITCAMERA_ROLL / 2.0f;
	}
	if (GetKeyboardPress(DIK_DOWN) == true) //���L�[�������ꂽ
	{//����]
		g_EditCamera.rot.x -= EDITCAMERA_ROLL / 2.0f;
	}

	//��]���l�̒���
	if (g_EditCamera.rot.x > D3DX_PI * 0.5f)
	{
		g_EditCamera.rot.x = D3DX_PI * 0.5f - 0.01f;
	}
	if (g_EditCamera.rot.x < -D3DX_PI * 0.5f)
	{
		g_EditCamera.rot.x = -D3DX_PI * 0.5f + 0.01f;
	}

	//�}�E�X�̏����擾
	DIMOUSESTATE pMouse = GetMouse();

	g_EditCamera.rot.x -= EDITCAMERA_ROLL * pMouse.lY * MOUSE_SINCE;//X����]
	g_EditCamera.rot.y += EDITCAMERA_ROLL * pMouse.lX * MOUSE_SINCE;//Y����]

	//============================================================================
	// �ړ�
	//============================================================================

	//��ړ�(Z��)
	if (GetKeyboardPress(DIK_W) == true) //W�L�[�������ꂽ
	{
		//����ړ�(Z��)
		if (GetKeyboardPress(DIK_A) == true) //A�L�[�������ꂽ
		{
			//�ړ��ʂ����Z
			g_EditCamera.move.x += sinf(g_EditCamera.rot.y + D3DX_PI * -0.25f) * EDITCAMERA_SPEED;	//X��
			g_EditCamera.move.z += cosf(g_EditCamera.rot.y + D3DX_PI * -0.25f) * EDITCAMERA_SPEED;	//Z��
		}
		//�E��ړ�(Z��)
		else if (GetKeyboardPress(DIK_D) == true) //D�L�[�������ꂽ
		{
			//�ړ��ʂ����Z
			g_EditCamera.move.x += sinf(g_EditCamera.rot.y + D3DX_PI * 0.25f) * EDITCAMERA_SPEED;	//X��
			g_EditCamera.move.z += cosf(g_EditCamera.rot.y + D3DX_PI * 0.25f) * EDITCAMERA_SPEED;	//Z��
		}
		//�^��ړ�(Z��)
		else
		{
			//�ړ��ʂ����Z
			g_EditCamera.move.x -= sinf(g_EditCamera.rot.y + D3DX_PI) * EDITCAMERA_SPEED;	//X��
			g_EditCamera.move.z -= cosf(g_EditCamera.rot.y + D3DX_PI) * EDITCAMERA_SPEED;	//Z��
		}
	}
	//���ړ�(Z��)
	else if (GetKeyboardPress(DIK_S) == true) //S�L�[�������ꂽ
	{
		//�����ړ�(Z��)
		if (GetKeyboardPress(DIK_A) == true) //A�L�[�������ꂽ
		{
			//�ړ��ʂ����Z
			g_EditCamera.move.x += sinf(g_EditCamera.rot.y + D3DX_PI * -0.75f) * EDITCAMERA_SPEED;	//X��
			g_EditCamera.move.z += cosf(g_EditCamera.rot.y + D3DX_PI * -0.75f) * EDITCAMERA_SPEED;	//Z��
		}
		//�E���ړ�(Z��)
		else if (GetKeyboardPress(DIK_D) == true) //D�L�[�������ꂽ
		{
			//�ړ��ʂ����Z
			g_EditCamera.move.x += sinf(g_EditCamera.rot.y + D3DX_PI * 0.75f) * EDITCAMERA_SPEED;	//X��
			g_EditCamera.move.z += cosf(g_EditCamera.rot.y + D3DX_PI * 0.75f) * EDITCAMERA_SPEED;	//Z��
		}
		//�^���ړ�(Z��)
		else
		{
			//�ړ��ʂ����Z
			g_EditCamera.move.x += sinf(g_EditCamera.rot.y + D3DX_PI) * EDITCAMERA_SPEED;	//X��
			g_EditCamera.move.z += cosf(g_EditCamera.rot.y + D3DX_PI) * EDITCAMERA_SPEED;	//Z��
		}
	}
	//��ړ�(Y��)
	else if (GetKeyboardPress(DIK_Z) == true) //W�L�[�������ꂽ
	{
		//�ړ��ʂ����Z
		g_EditCamera.move.y += EDITCAMERA_SPEED;
	}
	//���ړ�(Y��)
	else if (GetKeyboardPress(DIK_X) == true) //S�L�[�������ꂽ
	{
		//�ړ��ʂ����Z
		g_EditCamera.move.y -= EDITCAMERA_SPEED;
	}
	//���ړ�
	else if (GetKeyboardPress(DIK_A) == true) //A�L�[�������ꂽ
	{
		//�ړ��ʂ����Z
		g_EditCamera.move.x += sinf(g_EditCamera.rot.y + D3DX_PI / -2.0f) * EDITCAMERA_SPEED;	//X��
		g_EditCamera.move.z += cosf(g_EditCamera.rot.y + D3DX_PI / -2.0f) * EDITCAMERA_SPEED;	//Z��
	}
	//�E�ړ�
	else if (GetKeyboardPress(DIK_D) == true) //D�L�[�������ꂽ
	{
		//�ړ��ʂ����Z
		g_EditCamera.move.x += sinf(g_EditCamera.rot.y + D3DX_PI / 2.0f) * EDITCAMERA_SPEED;	//X��
		g_EditCamera.move.z += cosf(g_EditCamera.rot.y + D3DX_PI / 2.0f) * EDITCAMERA_SPEED;	//Z��
	}

	//�ړ��ʂ̉��Z
	g_EditCamera.posV += g_EditCamera.move;

	//�ړ��ʂ�����
	g_EditCamera.move.x += (0.0f - g_EditCamera.move.x);
	g_EditCamera.move.y += (0.0f - g_EditCamera.move.y);
	g_EditCamera.move.z += (0.0f - g_EditCamera.move.z);

	//�����_�̈ʒu�̍X�V
	g_EditCamera.posR.x = sinf(g_EditCamera.rot.y) * g_EditCamera.fLength + g_EditCamera.posV.x;
	g_EditCamera.posR.y = tanf(g_EditCamera.rot.x) * g_EditCamera.fLength + g_EditCamera.posV.y;
	g_EditCamera.posR.z = cosf(g_EditCamera.rot.y) * g_EditCamera.fLength + g_EditCamera.posV.z;
}

//=======================================================================================================================================================================================================================
// �ݒ菈��
//=======================================================================================================================================================================================================================
void SetEditCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�̎擾

	//�v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_EditCamera.mtxProjection);

	//�v���W�F�N�V�����}�g���b�N�X�𐶐�
	D3DXMatrixPerspectiveFovLH(&g_EditCamera.mtxProjection, D3DXToRadian(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 1.0f, 5000.0f);

	//�v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &g_EditCamera.mtxProjection);

	//�r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_EditCamera.mtxView);

	//�r���[�}�g���b�N�X�̐���
	D3DXMatrixLookAtLH(&g_EditCamera.mtxView, &g_EditCamera.posV, &g_EditCamera.posR, &g_EditCamera.vecU);

	//�r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &g_EditCamera.mtxView);
}

//=======================================================================================================================================================================================================================
// �Ώۂ̃I�u�W�F�N�g�Ɉړ�
//=======================================================================================================================================================================================================================
void SetPosEditCamera(int nCnt)
{
	Edit* pEdit = GetEdit();//�G�f�B�^�̍\���̂̏��̎擾

	//�Ώۂ̃I�u�W�F�N�g�܂ŃJ�E���g��i�߂�
	for (int nCntCamera = 0; nCntCamera < nCnt; nCntCamera++)
	{
		pEdit++;
	}

	//���_�̈ʒu�̍X�V
	g_EditCamera.posV.x = pEdit->pos.x;
	g_EditCamera.posV.y = pEdit->pos.y + 200.0f;
	g_EditCamera.posV.z = pEdit->pos.z - 400.0f;

	//�����_�̈ʒu�̍X�V
	g_EditCamera.posR = pEdit->pos;

	//�Ώۂ̃I�u�W�F�N�g����J�����̑Ίp���̊p�x�����߂�
	float fAngle = atan2f((pEdit->pos.y - g_EditCamera.posV.y), (pEdit->pos.z - g_EditCamera.posV.z));

	//���������Z�b�g
	g_EditCamera.rot = D3DXVECTOR3(fAngle, 0.0f, 0.0f);
}

//=======================================================================================================================================================================================================================
// ���̎擾
//=======================================================================================================================================================================================================================
EditCamera* GetEditCamera(void)
{
	return &g_EditCamera;
}

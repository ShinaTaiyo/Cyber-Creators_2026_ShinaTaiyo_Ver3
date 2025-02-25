//=============================================
//
//�X�N���[�����W�ϊ�����[camera.cpp]
//Author:ShinaTaiyo
//
//=============================================

#include "main.h"
#include "camera.h"



D3DXVECTOR2 g_CameraPos;

// �J����

//=============================
//����������
//=============================
void InitCamera(void)
{
	static D3DXVECTOR2 g_CameraPos{ SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f };
}

//=============================
//�I������
//=============================
void UninitCamera(void)
{

}

//=============================
//�X�V����
//=============================
void UpdateCamera(void)
{
	// �J�������W����X�N���[�����W�̌��_�ɕϊ�����
	D3DXVECTOR2 screen_origin_position =
	{
		g_CameraPos.x - SCREEN_WIDTH / 2.0f,
		g_CameraPos.y - SCREEN_HEIGHT / 2.0f
	};
}

//=============================
//�`�揈��
//=============================
void DrawCamera(void)
{
	// ���[���h���W����X�N���[�����W�ɕϊ�����
	//D3DXVECTOR2 screen_position = (pos.X - screen_origin_position.X, pos.Y - screen_origin_position.Y);
}

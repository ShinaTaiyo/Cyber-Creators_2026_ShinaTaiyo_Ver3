//========================================================
//
//�W���R�O���F�G�t�F�N�g������񌩒���[main.cpp]
//Author:ShinaTaiyo
//
//=========================================================

#ifndef _MAIN_H_ //���̃}�N����`������Ă��Ȃ�������
#define _MAIN_H_//�Q�d�C���N���[�h�h�~�̃}�N����`
#include <windows.h>
#include "d3dx9.h"//�`�揈���ɕK�v
#define DIRECTINPUT_VERSION (0x0800)//�r���h���̌x���Ώ��p�̃}�N��
#include "dinput.h"//���͏����ɕK�v
#include "Xinput.h"//�W���C�p�b�h�̏����ɕK�v
#include "xaudio2.h"//�T�E���h�����ɕK�v
#include "bg.h"//�w�i�p

//���C�u�����̃����N
#pragma comment(lib,"d3d9.lib")//�`�揈���ɕK�v
#pragma comment(lib,"d3dx9.lib")//[d3d9.lib]�̊g�����C�u����
#pragma comment(lib,"dxguid.lib")//DurectX�R���|�[�l���g�i���i�j�g�p�ɕK�v
#pragma comment(lib,"winmm.lib")//�V�X�e�������擾�ɕK�v
#pragma comment(lib, "dinput8.lib")//���͏����ɕK�v
#pragma comment(lib,"Xinput.lib")//�W���C�p�b�h�����ɕK�v


//�}�N����`
#define SCREEN_WIDTH (1280)//�E�C���h�E�̕�
#define SCREEN_HEIGHT (720)//�E�C���h�E�̍���
#define FVF_VERTEX_2D (D3DFVF_XYZRHW |D3DFVF_DIFFUSE |D3DFVF_TEX1)
#define ID_BUTTON_FINISH (100)
#define MAX_POLYGON (2)
#define MAX_TEXT	(256)					//�e�L�X�g�ǂݍ��ݐ�
#define MODEL_TXT	"data\\SetModel.txt"	//���f������txt�t�@�C��
#define MODEL_BIN	"data\\SetModel.bin"	//���f������bin�t�@�C��
#define NULL_VECTOR3 (D3DXVECTOR3(0.0f,0.0f,0.0f)) //�S��0.0f�ɂ��鎞�ɌĂяo��
#define ONE_VECTOR3 (D3DXVECTOR3(1.0f,1.0f,1.0f)) //�S��1.0f�ɂ��鎞�ɌĂяo��

#define FVF_VERTEX_3D (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)

//���_���[2D]�̍\���̂��`
typedef struct
{
	D3DXVECTOR3 pos;//���_���W
	float rhw;//���W�ϊ��p�ϐ��i1.0f�ŌŒ�)
	D3DCOLOR col;//���_�J���[
	D3DXVECTOR2 tex;//�e�N�X�`�����W
}VERTEX_2D;

typedef struct
{
	D3DXVECTOR3 pos;//���_���W
	D3DXVECTOR3 nor;//�@���x�N�g��
	D3DCOLOR col;//���_�J���[
	D3DXVECTOR2 tex;//�e�N�X�`�����W
}VERTEX_3D;

typedef enum
{
	MODE_TITLE=0,	//�^�C�g�����
	MODE_TUTORIAL,	//�`���[�g���A����ʂ��Ăяo��
	MODE_GAME,		//�Q�[�����
	MODE_RESULT,	//���U���g���
	MODE_RANKING,	//�����L���O���
	MODE_END,		// �I��
	MODE_MAX
}MODE;

//�v���g�^�C�v�錾
LPDIRECT3DDEVICE9 GetDevice(void);
void SetMode(MODE mode);
MODE GetMode(void);
void ToggleFullscreen(HWND hWnd);//�E�B���h�E���t���X�N���[���ɂ���

#endif

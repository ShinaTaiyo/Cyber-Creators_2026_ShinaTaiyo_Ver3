//================================================================================
//
//�T���P�V���FC++��Ս쐬�p�[�g�P�Fmain.cpp�݂̂Ń|���S�����o��[main.cpp]
//Author:ShinaTaiyo
//
//================================================================================

#ifndef _MAIN_H_ //���̃}�N����`������Ă��Ȃ�������
#define _MAIN_H_//�Q�d�C���N���[�h�h�~�̃}�N����`

//==============================
//�C���N���[�h
//==============================
#include <windows.h>
#include "d3dx9.h"//�`�揈���ɕK�v
#define DIRECTINPUT_VERSION (0x0800)//�r���h���̌x���Ώ��p�̃}�N��
#include "dinput.h"//���͏����ɕK�v
#include "Xinput.h"//�W���C�p�b�h�̏����ɕK�v
#include "xaudio2.h"//�T�E���h�����ɕK�v
#include <iostream> //�W�����o��
#include <fstream> //�t�@�C�����o�͂Ŏg��
#include <sstream> //�t�@�C�����o�͂Ŏg��
#include <algorithm>
#include <vector> //�x�N�^�[�Ǘ��Ŏg��
#include <list>
#include <iomanip>// setprecision���g�p����̂ɕK�v
#include <cassert>
#include <stdio.h>
#include <stdlib.h>

#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>
using namespace std;
//==============================
//���C�u�����̃����N
//==============================
#pragma comment(lib,"d3d9.lib")//�`�揈���ɕK�v
#pragma comment(lib,"d3dx9.lib")//[d3d9.lib]�̊g�����C�u����
#pragma comment(lib,"dxguid.lib")//DurectX�R���|�[�l���g�i���i�j�g�p�ɕK�v
#pragma comment(lib,"winmm.lib")//�V�X�e�������擾�ɕK�v
#pragma comment(lib, "dinput8.lib")//���͏����ɕK�v
#pragma comment(lib,"Xinput.lib")//�W���C�p�b�h�����ɕK�v

//�}�N����`
#define SCREEN_WIDTH (1280)//�E�C���h�E�̕�
#define BOSSSTAGE_WIDTH (720.0f)//�{�X�X�e�[�W�̉���
#define BOSSSTAGE_HALFWIDTH (360.0f)//�{�X�X�e�[�W�̉����̔���
#define BOSSSTAGE_HEIGHT (400.0f)  //�{�X�X�e�[�W�̍���
#define SCREEN_HEIGHT (720)//�E�C���h�E�̍���
#define ID_BUTTON_FINISH (100)
#define FVF_VERTEX_2D (D3DFVF_XYZRHW |D3DFVF_DIFFUSE |D3DFVF_TEX1)
//3D�Œǉ�����}�N����`
#define FVF_VERTEX_3D (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)

#define NULL_VECTOR3 (D3DXVECTOR3(0.0f,0.0f,0.0f))                            //�������p
#define NORMAL_COL (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))                           //���ʂ̐F
#define ONE_VECTOR3 (D3DXVECTOR3(1.0f,1.0f,1.0f) )                            //�S��1.0f
#define SENTER_VECTOR3 (D3DXVECTOR3(SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2,0.0f)) //Window�̒��S

//==================================================
//���������[�N���o�p
//==================================================
#ifdef _DEBUG
#define DBG_NEW new (_NORMAL_BLOCK,__FILE__,__LINE__)
#else
#define DBG_NEW new
#endif // _DEBUG
//==================================================================================================

//===================================
//���_���[2D]�̍\���̂��`
//===================================
typedef struct
{
	D3DXVECTOR3 pos;//���_���W
	float rhw;//���W�ϊ��p�ϐ��i1.0f�ŌŒ�)
	D3DCOLOR col;//���_�J���[
	D3DXVECTOR2 tex;//�e�N�X�`�����W
}VERTEX_2D;

//=============================================================================================

//===================================
//���_���[3D]�̍\���̂��`
//===================================
typedef struct
{
	D3DXVECTOR3 pos;//���_���W
	D3DXVECTOR3 nor;//�@���x�N�g��
	D3DCOLOR col;//���_�J���[
	D3DXVECTOR2 tex;//�e�N�X�`�����W
}VERTEX_3D;

int GetFPS();
#endif
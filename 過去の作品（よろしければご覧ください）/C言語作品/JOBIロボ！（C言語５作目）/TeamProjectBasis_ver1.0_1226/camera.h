//========================================================
//
// �J�����̏����Ɋւ���w�b�_�[�t�@�C��
// Author : ShinaTaiyo
//
//=========================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"

//=========================================================
// �}�N����`
//=========================================================
#define TITLE_POSV_Y	(50.0f)
#define TITLE_POSV_Z	(10.0f)
#define GAME_POSV_Y		(150.0f)
#define GAME_POSV_Z		(700.0f)
#define BOSSAPPEAR_TIME	(300)	//�{�X�o�����o����
#define BOSS_SETPOS_Y	(300.0f)//�{�X�o�����o�J�n���̈ʒu(Y���W)

//=========================================================
// �J�����\����
//=========================================================
typedef struct
{
	D3DXVECTOR3 PosV;//�x�_
	D3DXVECTOR3 PosR;//�����_
	D3DXVECTOR3 VecU;//������x�N�g��
	D3DXVECTOR3 rot;//����
	D3DXVECTOR3 move;//�ړ���
	D3DXVECTOR3 DecayRot;//�J�����̌�����ς��铮�������������鏈��;
	D3DXVECTOR3 center;//�J�����ƃv���C���[�Ԃ̒��ԓ_
	float fXVaim;//X�����x�N�g��
	float fZVaim;//Z�����x�N�g��
	float fLength;//����
	float fRot;//�ǂ̕����������Ă��邩
	int nMode;//�J�������[�h��ݒ肷��
	int nModeOld;//1f�O�̃J�������[�h
	int nCntModeTime;//���̃J�������[�h�ɂȂ��Ă���̎���
	bool bBossAppear;//�{�X���o��������g��
	D3DXMATRIX mtxProjection;//�v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX mtxView;//�r���[�}�g���b�N�X

	//======================================
	//�R�������@�_��
	//======================================
	float fVXaim3D;
	float fVYaim3D;
	float fVZaim3D;
	float fVLaim3D;

}Camera;

typedef enum
{
	CAMERAMODE_REVOLUTION = 0,//���]���[�h
	CAMERAMODE_ROTATION,//���]���[�h
	CAMERAMODE_BIRDSEYE,//���Վ��_���[�h
	CAMERAMODE_BOSSAPPEAR,//�{�X�o�����[�h
	CAMERAMODE_MAX//�J�������[�h�ő吔
}CAMERAMODE;

//=========================================================
// �v���g�^�C�v�錾
//=========================================================
void InitCamera(void);
void UninitCamera(void);
void UpdateCamera(void);
void SetCamera(void);
Camera* GetCamera(void);

#endif

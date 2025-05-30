//=========================================================
//
// �^�C�g����ʂ̏����Ɋւ���w�b�_�[�t�@�C��
// Author : Atsumu Kuboichi
//
//=========================================================
#ifndef _TITLE_H_
#define _TITLE_H

#include "main.h"

//=========================================================
// �}�N����`
//=========================================================
//++++++++++++++++++++++++++++++++
// �^�C�g�����S
//++++++++++++++++++++++++++++++++
// �^�C�g�����S�
#define TITLELOGO_POS (D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f))			// ���S�����ʒu
#define TITLELOGO_SIZE (D3DXVECTOR2(1200.0f, 700.0f))											// ���S�����T�C�Y
// �^�C�g�����S�o��
#define TITLELOGO_ARR_TIME (60)																	// ���S�o������
#define TITLELOGO_ARR_ROLL_POS D3DXVECTOR3(-SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0);	// ���S��]�o���ʒu
#define TITLELOGO_ARR_ROLL_ROT D3DXVECTOR3(0.0f, 0.0f, (D3DX_PI * 2) * 3.0f)					// ���S��]�o����]��
// �^�C�g�����S�k��
#define TITLELOGO_RED_START (120)																// ���S�k���J�n����
#define TITLELOGO_RED_TIME (60)																	// ���S�k������
#define TITLELOGO_RED_SCALE (0.5f)																// ���S�k������
#define TITLELOGO_RED_POS (D3DXVECTOR3(SCREEN_WIDTH * 0.25f, SCREEN_HEIGHT * 0.25f, 0.0f))		// ���S�k���ʒu
// �^�C�g�����S�A�j���[�V�����S��
#define TITLELOGO_TIME (TITLELOGO_ARR_TIME + TITLELOGO_RED_START + TITLELOGO_RED_TIME)			// ���S�A�j���[�V��������

//++++++++++++++++++++++++++++++++
// �^�C�g�����j���[
//++++++++++++++++++++++++++++++++
#define TITLEMENU_POS (D3DXVECTOR3(320.0f, 400.0f, 0.0f))		// �^�C�g���ʒu�ŏ�i
#define TITLEMENU_SIZE (D3DXVECTOR2(300, 80.0f))				// �^�C�g���ʒu�ŏ�i
#define TITLEMENU_COL_A (1.0f)
#define TITLEMENU_COL_NOTSELECT (D3DXCOLOR(1.0f, 1.0f, 1.0f, TITLEMENU_COL_A))
#define TITLEMENU_COL_SELECT (D3DXCOLOR(0.0f, 1.0f, 1.0f, TITLEMENU_COL_A))

#define TITLEMENU_TIME (30) 
#define TITLEMENU_FALL (10.0f)
//=========================================================
// �\���̒�`
//=========================================================
//++++++++++++++++++++++++++++++++
// �^�C�g�����S
//++++++++++++++++++++++++++++++++
typedef struct
{
	D3DXVECTOR3 pos;		// �ʒu
	D3DXVECTOR3 postarget;	// �ʒu(�ڕW)
	D3DXVECTOR3 rot;		// ����
	D3DXVECTOR3 rottarget;	// ����(�ڕW)
	D3DXVECTOR2 size;		// �傫��
	float fScale;			// �傫��(�{��)
	float fScaletarget;		// �傫��(�ڕW)
	float fAngle;			// ���_�p�x
	float fLeng;			// ���_����̒��_�܂ł̂̒���
	int nArrivalType;		// �o���^�C�v
}TITLELOGO;

//++++++++++++++++++++++++++++++++
// �^�C�g�����j���[
//++++++++++++++++++++++++++++++++
typedef struct
{
	D3DXVECTOR3 pos;		// �ʒu
	D3DXVECTOR2 size;		// �傫��
	D3DXCOLOR col;			// �F
	float fColaTarget;	// �F(�ڕW)
	int nType;				// ���
	bool bUse;				// �g�p��
}TITLEMENU;
//=========================================================
// �񋓌^��`
//=========================================================
//++++++++++++++++++++++++++++++++
// �^�C�g�����S���j���[
//++++++++++++++++++++++++++++++++
typedef enum
{
	TITLELOGO_NORMAL = 0,	// ����
	TITLEROGO_ROLL,			// ��]
	TITLEROGO_MAX,
}TITLELOGO_ARRIVAL_TYPE;	// �^�C�g���o���G�t�F�N�g

//++++++++++++++++++++++++++++++++
// �^�C�g�����j���[
//++++++++++++++++++++++++++++++++
typedef enum
{
	TITLEMENU_START = 0,	// �X�^�[�g
	TITLEMENU_RANKING,		// �����L���O
	TITLEMENU_END,			// �I��
	TITLEMENU_MAX,
}TITLEMENU_TYPE;	// �^�C�g���o���G�t�F�N�g

// �^�C�g�����j���[�e�N�X�`��
static const char* cTITLEMENU_TEXTURE_FILENAME[TITLEMENU_MAX] = {
	"data\\TEXTURE\\STRAT000.png",
	"data\\TEXTURE\\RANKING000.png",
	"data\\TEXTURE\\EXIT000.png",
};


//=========================================================
// �v���g�^�C�v�錾
//========================================================

// �S��
void InitTitle(void);		//����������
void UninitTitle(void);		//�I������
void UpdateTitle(void);		//�X�V����
void DrawTitle(void);		//�`�揈��
void SelectTitle(void);	//�I������
// ���S
void InitTitleLogo(void);	//����������
void UninitTitleLogo(void);	//�I������
void UpdateTitleLogo(void);	//�X�V����
void DrawTitleLogo(void);	//�`�揈��
// ���j���[
void InitTitleMenu(void);	//����������
void UninitTitleMenu(void);	//�I������
void UpdateTitleMenu(void);	//�X�V����
void DrawTitleMenu(void);	//�`�揈��
void SelectTitleMenu(void);	//�I������

#endif

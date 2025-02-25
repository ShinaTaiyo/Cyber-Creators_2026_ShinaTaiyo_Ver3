//========================================================
//
//�W���R�O���F�G�t�F�N�g������񌩒���[staging.h]
//Author:ShinaTaiyo
//
//=========================================================
#ifndef _ITEM_H_
#define _ITEM_H

#include "main.h"
//�v���g�^�C�v�錾
void InitItem(void);
void UninitItem(void);
void UpdateItem(void);
void DrawItem(void);

//�\���̒�`
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 move;
	bool bUse;//���o���g���Ă��邩�ǂ���
	int type;//���o�̎��]
	int HealPoint;//�񕜗�
}Item;

typedef struct
{
	float fmoveBg;//�w�i�𓮂������x���グ��B
	float fGameSpeed;//�v���C���[�̑��x���グ��B
}ItemEffect;


//�񋓌^��`
typedef enum
{
	ITEMTYPE_HEAL = 0,//�̗͉�
	ITEMTYPE_REAFORB,//���̃I�[�u
	ITEMTYPE_ICEORB,//�X�̃I�[�u
	ITEMTYPE_FIREORB,//���̃I�[�u
	ITEMTYPE_THUNDERORB,//���̃I�[�u
	ITEMTYPE_LASTORB,//�T�C�S�m�I�[�u
	ITEMTYPE_MAX
}ITEMTYPE;

void SetItem(ITEMTYPE type,D3DXVECTOR3 pos,int HealPoint);

Item* GetItem(void);

ItemEffect* GetItemEffect(void);
//Staging* GetStaging(void);
#endif


//========================================================
//
//�P�O���P�S���F�}�b�v�̐ݒ�[map.h]
//Author:ShinaTaiyo
//
//=========================================================
#ifndef _MAP_H_
#define _MAP_H

#include "main.h"

#define MAX_MAP (5000)
#define NORMAL_MAPSIZE (20.0f)//���ʂ̃u���b�N�̃T�C�Y



//bool CollisionMAP(void);

//�\���̒�`
//typedef struct
//{
//	D3DXVECTOR3 pos;
//	bool bUse;//���o���g���Ă��邩�ǂ���
//	int nType;//���o�̎��
//	int nLife;//���o��������܂ł̎���
//	float fXsize;//X�����̑傫��
//	float fYsize;//Y�����̑傫��
//	float fRadius;//���a�ő傫�������߂Ă���ꍇ�̑傫��
//	bool bDraw;//�`�悷�邩�ǂ�����bUse�ȊO�ł���ɔ��肷��t���O
//}Map;

typedef struct
{
	bool bDefeatBoss1;//�{�X�P��|�������ǂ������肷��t���O
	bool bDefeatBoss2;//�{�X�Q��|�������ǂ������肷��t���O
	bool bDefeatBoss3;//�{�X�R��|�������ǂ������肷��t���O
	bool bDefeatBoss4;//�{�X�S��|�������ǂ������肷��t���O
}Map_h_DefeatBoss;

typedef enum
{
	CHALLENGESTAGE_1 = 0,//�X�e�[�W�P�ɒ��킵��
	CHALLENGESTAGE_2,//�X�e�[�W�Q�ɒ��킵��
	CHALLENGESTAGE_3,//�X�e�[�W�R�ɒ��킵��
	CHALLENGESTAGE_4,//�X�e�[�W�S�ɒ��킵��
	CHALLENGESTAGE_MAX
}CHALLENGESTAGE;

//�񋓌^��`
typedef enum
{
	MAPTYPE_0 = 0,//�}�b�v�ԍ��O
	MAPTYPE_1,//�}�b�v�ԍ��P
	MAPTYPE_2,//�}�b�v�ԍ��Q
	MAPTYPE_3,//�}�b�v�ԍ��R
	MAPTYPE_4,//�}�b�v�ԍ��S
	MAPTYPE_5,//�}�b�v�ԍ��T
	MAPTYPE_6,//�}�b�v�ԍ��U
	MAPTYPE_7,//�}�b�v�ԍ��V
	MAPTYPE_8,//�}�b�v�ԍ��W
	MAPTYPE_9,//�}�b�v�ԍ��X
	MAPTYPE_10,//�}�b�v�ԍ��P�O
	MAPTYPE_11,//�}�b�v�ԍ��P�P
	MAPTYPE_12,//�}�b�v�ԍ��P�Q
	MAPTYPE_13,//�}�b�v�ԍ��P�R
	MAPTYPE_14,//�}�b�v�ԍ��P�S
	MAPTYPE_15,//�}�b�v�ԍ��P�T
	MAPTYPE_16,//�}�b�v�ԍ��P�U
	MAPTYPE_17,//�}�b�v�ԍ��P�V
	MAPTYPE_18,//�}�b�v�ԍ��P�W
	MAPTYPE_19,//�}�b�v�ԍ��P�X
	MAPTYPE_20,//�}�b�v�ԍ��Q�O
	MAPTYPE_21,//�}�b�v�ԍ��Q�P
	MAPTYPE_22,//�}�b�v�ԍ��Q�Q
	MAPTYPE_23,//�}�b�v�ԍ��Q�R
	MAPTYPE_24,//�}�b�v�ԍ��Q�S
	MAPTYPE_25,//�}�b�v�ԍ��Q�T
	MAPTYPE_26,//�}�b�v�ԍ��Q�U
	MAPTYPE_27,//�}�b�v�ԍ��Q�V
	MAPTYPE_28,//�}�b�v�ԍ��Q�W
	MAPTYPE_29,//�}�b�v�ԍ��Q�X
	MAPTYPE_30,//�}�b�v�ԍ��R�O
	MAPTYPE_31,//�}�b�v�ԍ��R�P
	MAPTYPE_32,//�}�b�v�ԍ��R�Q
	MAPTYPE_33,//�}�b�v�ԍ��R�R
	MAPTYPE_34,//�}�b�v�ԍ��R�S
	MAPTYPE_35,//�}�b�v�ԍ��R�T
	MAPTYPE_36,//�}�b�v�ԍ��R�U
	MAPTYPE_37,//�}�b�v�ԍ��R�V
	MAPTYPE_38,//�}�b�v�ԍ��R�W
	MAPTYPE_39,//�}�b�v�ԍ��R�X
	MAPTYPE_40,//�}�b�v�ԍ��S�O
	MAPTYPE_41,//�}�b�v�ԍ��S�P
	MAPTYPE_MAX
}MAPTYPE;

//�v���g�^�C�v�錾
void InitMap(void);
void SetMap(int ntype);
Map_h_DefeatBoss * GetDefeatBoss(void);
int * GetUseMap(void);
int * GetChallengeStage(void);
#endif




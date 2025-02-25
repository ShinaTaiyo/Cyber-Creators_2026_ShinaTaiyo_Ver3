//========================================================
//
//�W���R�O���F�G�t�F�N�g������񌩒���[Special.h]
//Author:ShinaTaiyo
//
//=========================================================
#ifndef _SPECIAL_H_
#define _SPECIAL_H

#include "main.h"
#include "Attack.h"
//�v���g�^�C�v�錾
void InitSpecial(void);
void UninitSpecial(void);
void UpdateSpecial(void);
void DrawSpecial(void);

//�\���̒�`
typedef struct
{
	int nSpecialType;//�X�L���̃^�C�v�����߂�ϐ�
	D3DXVECTOR3 pos;
	int nAtackSummon;//���o�𔭓��������ꏊ�ɍU���Z����������
	bool bUse;//���o���g���Ă��邩�ǂ���
	float fScale;//�傫��
	int nA_Life;//���������U���̃��C�t
	int type;//���o�̎��
	int nLife;//���o��������܂ł̎���
	int nSpecialCount;//���o���n�܂��Ă���̃J�E���g���J�n����B
	float fSpecialPattern;//�A�j���[�V�����̎��
	float fSpecialPatternCountY;//�A�j���[�V������Y���W�����炷�J�E���g
	bool bCountStartFlag;//���o���n�܂��Ă���̃J�E���g���J�n����t���O��錾����B
	int nAnimationRepeat;//�A�j���[�V���������s�[�g����ϐ�
	bool bDraw;//�`�悷�邩�ǂ�����bUse�ȊO�ł���ɔ��肷��t���O
	int nCntEnemy;//n�Ԗڂ̓G�̂���ʒu�𔻒肷��
	float fColor_a;//�F�̃A���t�@�l�𑀍삷��ϐ�
	bool bColorChange_a;//�F�̃A���t�@�l���C���N�������g���邩�A�f�N�������g���邩�����߂�t���O
	float rot;//��]�p
	float fAeffective_Scale;//���������U���̌��ʔ͈�
	float fA_Width;//���������U���̑傫��
	float fA_Height;//���������U���̍���
	float fA_rotSpeed;//��������U���̉�]���x
	int nCntTime;//���Ԃ��v������
}Special;



//�񋓌^��`
typedef enum
{
	SPECIALTYPE_BOSS1_SPECIAL_ATTACK,//�{�X�P�̕K�E�Z���o
	SPECIALTYPE_BOSS2_SPECIAL_ATTACK,//�{�X�Q�̕K�E�Z���o
	SPECIALTYPE_BOSS3_SPECIAL_ATTACK,//�{�X�R�̕K�E�Z���o
	SPECIALTYPE_BOSS4_SPECIAL_ATTACK,//�{�X�S�K�E�Z����
	SPECIALTYPE_FINALBOSS_SPECIAL_ATTACK,//���X�{�X�̕K�E�Z���o
	SPECIALTYPE_MAX
}SpecialTYPE;

void SetSpecial(SpecialTYPE type, D3DXVECTOR3 pos, int nLife);

//Special* GetSpecial(void);
#endif


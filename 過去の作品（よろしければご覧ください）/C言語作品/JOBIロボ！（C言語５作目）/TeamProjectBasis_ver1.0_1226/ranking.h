//=========================================================
//
// �����L���O��ʂ̏����Ɋւ���w�b�_�[�t�@�C��
// Author : Atsumu Kuboichi
//
//=========================================================
#ifndef _RANKING_H_
#define _RANKING_H

#include "main.h"

//=========================================================
// �v���g�^�C�v�錾
//=========================================================
void InitRanking(void);		                       //����������
void UninitRanking(void);	                       //�I������
void UpdateRanking(void);	                       //�X�V����
void DrawRanking(void);		                       //�`�揈��
void SetRanking(int nScore);                       //�����L���O�ݒ菈��
void RankingPos(D3DXVECTOR3 pos,D3DXVECTOR3 Scale);//�����L���O�̃X�R�A�\���̈ʒu�����߂�
#endif

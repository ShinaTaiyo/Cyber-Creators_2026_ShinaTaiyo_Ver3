//====================================================
//
//�X���V���F�^�C�}�[������ [timer.h]
//Author:ShinaTaiyo
//
//====================================================

#ifndef _TIMER_H_  
#define _TIMER_H_

//======================
//�C���N���[�h
//======================
#include "main.h"
#include "object2d.h"
//==========================================

//======================
//�O���錾
//======================
class CNumber;

//==========================================

//==========================================
//�^�C�}�[�N���X
//==========================================
class CTimer : public CObject
{
public:
	CTimer();                                //�R���X�g���N�^
	~CTimer() override;                      //�f�X�g���N�^
	HRESULT Init() override;                 //����������
	void Uninit() override;                  //�I������
	void Update() override;                  //�X�V����
	void Draw() override;                    //�`�揈��
	void SetDeath() override;                //���S�t���O��ݒ�
	static CTimer* Create(int nInitialTimer,D3DXVECTOR3 Pos,float fNumberWidth,float fNumberHeight);//��������
	void AddTimer(int nTimer);               //���Ԃ𑝂₷
	void SubTimer(int nTimer);               //���Ԃ����炷
private:
	static const int m_nMAX_TIMERDIGIT = 3;  //�X�R�A�̍ő包��
	static const int m_nMAX_TIME = 180;      //��������
	CNumber* m_pNumber[m_nMAX_TIMERDIGIT];   //�����ւ̃|�C���^
	int m_nTime;
};

#endif

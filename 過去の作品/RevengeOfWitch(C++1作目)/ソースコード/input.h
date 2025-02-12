//====================================================
//
//�T���Q�Q���FC++�Ŋ�ՂÂ���LV5[input.h]
//Author:ShinaTaiyo
//
//====================================================

#ifndef _INPUT_H_  
#define _INPUT_H_

//======================
//�C���N���[�h
//======================
#include "main.h"
//==========================================

//======================
//�}�N����`
//======================
#define MAX_KEY (256)//�L�[�̍ő吔

//==========================================
//���͋K��N���X(DirectInput�̊Ǘ�)
//==========================================
class CInput
{
public:
	CInput();                                                            //�R���X�g���N�^
	virtual ~CInput();                                                   //�f�X�g���N�^
	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);                //����������
	virtual void Uninit();                                               //�I������
	virtual void Update() = 0;                                           //�X�V�����i���͒[�����ƂɎ����j
protected:
	//�S���͏����ŋ��L
	static LPDIRECTINPUT8 m_pInput;
	LPDIRECTINPUTDEVICE8 m_pDevice;
private:
};
//=======================================================================================


//==========================================
//�L�[�{�[�h�N���X
//==========================================
class CInputKeyboard : public CInput
{
public:
	CInputKeyboard();                                                    //�R���X�g���N�^
	~CInputKeyboard();                                                   //�f�X�g���N�^
	HRESULT Init(HINSTANCE hInstance, HWND hWnd) override;               //����������
	void Uninit() override;                                              //�I������
	void Update() override;                                              //�X�V����

	bool GetPress(int nKey);                                             //�v���X��Ԏ擾
	bool GetTrigger(int nKey);                                           //�g���K�[��Ԏ擾
	bool GetRepeat(int nKey);                                            //���s�[�g��Ԏ擾
private:
	static const int m_nMAX_REPEATCNT = 14;                              //���s�[�g���͂̍ۂɃg���K�[�̔�����s���J�E���g��
	BYTE m_aKeyState[MAX_KEY];
	BYTE m_aKeyStateTrigger[MAX_KEY];
	int m_nRepeatCnt;                                                    //���s�[�g���͂̃J�E���g
};
//=======================================================================================

//==========================================
//�W���C�p�b�h�N���X
//==========================================
class CInputJoypad : public CInput
{
public:
	//==========================================
    //�W���C�p�b�h�̃{�^���ԍ�
    //==========================================
	typedef enum
	{
		JOYKEY_UP = 0,	//��
		JOYKEY_DOWN,	//��
		JOYKEY_LEFT,	//��
		JOYKEY_RIGHT,	//�E
		JOYKEY_START,	//�X�^�[�g
		JOYKEY_BACK,	//�o�b�N
		JOYKEY_L3,		//L3(���X�e�B�b�N������)
		JOYKEY_R3,		//R3(�E�X�e�B�b�N������)
		JOYKEY_LB,		//LB
		JOYKEY_RB,		//RB
		JOYKEY_LT,		//LT
		JOYKEY_RT,		//RT
		JOYKEY_A,		//A
		JOYKEY_B,		//B
		JOYKEY_X,		//X
		JOYKEY_Y,		//Y
		JOYKEY_MAX
	}JOYKEY;
	//==============================================================================================================================

	CInputJoypad();                                                       //�R���X�g���N�^
	~CInputJoypad();                                                      //�f�X�g���N�^
	HRESULT Init(HINSTANCE hInstance, HWND hWnd) override;                //����������
	void Uninit() override;                                               //�I������
	void Update() override;                                               //�X�V����
 	bool GetPress(JOYKEY key);                                            //�v���X��Ԋl��
	bool GetTrigger(JOYKEY key);                                          //�g���K�[��Ԋl��
	bool GetRT_Trigger();                                       //L�g���K�[�{�^���̃g���K�[���
	bool GetRT_Press();
private:
	XINPUT_STATE m_joykeyStatePress;      //�W���C�p�b�h�̃v���X���
	XINPUT_STATE m_joykeyStateTrigger;    //�W���C�p�b�h�̃g���K�[���

};
#endif

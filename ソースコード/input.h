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
#define EXPORT extern "C" _declspec(dllexport)
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
	LPDIRECTINPUTDEVICE8 m_pDevice;//!
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
	BYTE m_aKeyState[MAX_KEY];                                           //!
	BYTE m_aKeyStateTrigger[MAX_KEY];                                    //!
	int m_nRepeatCnt;                                                    //���s�[�g���͂̃J�E���g!
};
//=======================================================================================

//==========================================
//�W���C�p�b�h�N���X
//==========================================
class CInputJoypad
{
public:
	//==========================================
    //�W���C�p�b�h�̃{�^���ԍ�
    //==========================================
	enum class JOYKEY
	{
		UP = 0,	//��
		DOWN,	//��
		LEFT,	//��
		RIGHT,	//�E
		START,	//�X�^�[�g
		BACK,	//�o�b�N
		L3,		//L3(���X�e�B�b�N������)
		R3,		//R3(�E�X�e�B�b�N������)
		LB,		//LB
		RB,		//RB
		LT,		//LT
		RT,		//RT
		A,		//A
		B,		//B
		X,		//X
		Y,		//Y
		MAX
	};
	//==============================================================================================================================

	CInputJoypad();                                                       //�R���X�g���N�^
	~CInputJoypad();                                                      //�f�X�g���N�^
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);                         //����������
	void Uninit();                                                        //�I������
	void Update();                                                        //�X�V����
 	bool GetPress(JOYKEY key);                                            //�v���X��Ԋl��
	bool GetTrigger(JOYKEY key);                                          //�g���K�[��Ԋl��
	bool GetRT_Trigger();                                                 //R�g���K�[�{�^���̃g���K�[���
 	bool GetRT_Press();                                                   //R�g���K�[�{�^���̃v���X���
	bool GetRT_Repeat(const int nRepeatLoop);                             //R�g���K�[�{�^���̃��s�[�g���
	bool GetLT_Trigger();                                                 //L�g���K�[�{�^���̃g���K�[���
 	bool GetLT_Press();                                                   //L�g���K�[�{�^���̃v���X���
	bool GetLT_Repeat(const int nRepeatLoop);                             //L�g���K�[�{�^���̃��s�[�g���
	bool GetLStickPress(const int nDivisionRot,float fDirectionRot);                          //L�X�e�B�b�N�̃v���X���
	bool GetRStickPress(const int nDivisionRot);                          //R�X�e�B�b�N�̃v���X���

	const float & GetLStickAimRot() const { return m_fLSitckAimRot; }     //L�X�e�B�b�N�̖ړI�̊p�x���擾����
	const float & GetRStickAimRot() const { return m_fRStickAimRot; }     //R�X�e�B�b�N�̖ړI�̊p�x���擾����
private:
	//*�ÓI�����o
	static const BYTE s_bRightTrigger_DeadZone;

	XINPUT_STATE m_joykeyStatePress;      //�W���C�p�b�h�̃v���X���!
	XINPUT_STATE m_joykeyStateTrigger;    //�W���C�p�b�h�̃g���K�[���!

	float m_fLSitckAimRot;                //L�X�e�B�b�N�̖ړI�̊p�x��������
	float m_fRStickAimRot;                //R�X�e�B�b�N�̖ړI�̊p�x��������

	int m_nRTTrigger_RepeatCnt;           //R�g���K�[�̃��s�[�g�J�E���g
	int m_nLTTrigger_RepeatCnt;           //L�g���K�[�̃��s�[�g�J�E���g

};

//==========================================
//�}�E�X�N���X
//==========================================
class CInputMouse
{
public:
	CInputMouse();//�R���X�g���N�^
	~CInputMouse();//�f�X�g���N�^
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);//����������
	void Uninit();//�I������
	void Update();//�X�V����
	bool GetMouseMoveAngle(float & fYaw,float & fPitch,float fAdjust);//�}�E�X���������p�x���擾
	bool GetMouseLeftClickPress();//�N���b�N�������ǂ���
	bool GetMouseLeftClickTrigger();//�g���K�[
	bool GetMouseLeftClickRepeat(int nRepeat);//���s�[�g����

	bool GetMouseRightClickPress();  //�E�N���b�N�v���X���擾
	bool GetMouseRightClickTrigger();//�E�N���b�N�g���K�[���擾
	bool GetMouseRightClickRepeat(int nRepeat); //�E�N���b�N���s�[�g���擾

	bool GetMouseMiddleClickPress();//�~�h���N���b�N�v���X���擾
	bool GetMouseMiddleClickTrigger();//�~�h���N���b�N�g���K�[���擾
	bool GetMouseMiddleClickRepeat(int nRepeat);//�~�h���N���b�N�̃��s�[�g���擾
private:
	D3DXVECTOR2 m_CursorPosOld;//1f�O�̃J�[�\���̈ʒu

	//���N���b�N
	bool m_bLeftClickTriggerFlag;//���N���b�N�̃g���K�[�t���O
	int m_nLeftClickRepeatCnt;   //���N���b�N�̃��s�[�g�J�E���g
	bool m_bPrevLeftClick;       //���N���b�N���������ǂ���
	void GetStateLeftClick();//���N���b�N�̏����擾����

	//�E�N���b�N
	bool m_bRightClickTriggerFlag;//�E�N���b�N�g���K�[�t���O
	int m_nRightClickRepeatCnt;   //�E�N���b�N�̃��s�[�g�J�E���g
	bool m_bPrevRightClick;       //�E�N���b�N���������ǂ���
	void GetStateRightClick();//�E�N���b�N�̏����擾����

	//�~�h���N���b�N
	bool m_bMiddleClickTriggerFlag;//�~�h���N���b�N�̃g���K�[�t���O
	int m_nMiddleClickRepeatCnt;   //�~�h���N���b�N�̃��s�[�g�J�E���g
	bool m_bPrevMiddleClick;       //�~�h���N���b�N���������ǂ���
	void GetStateMiddleClick();//�~�h���N���b�N�̏����擾����

	bool m_bCursorSenterWarp;     //�J�[�\�������S�Ƀ��[�v�������ǂ���
};
#endif

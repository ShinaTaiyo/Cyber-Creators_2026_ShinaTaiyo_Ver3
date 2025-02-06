//================================================================================
//
//�T���P�V���F�|���S�����o�������ƁA���C���̏����̃N���X��[main.cpp]
//Author:ShinaTaiyo
//
//================================================================================

//=============================================
//�C���N���[�h
//=============================================
#include "main.h"
#include "object2d.h"
#include "renderer.h"
#include "player.h"
#include "manager.h"
#include <time.h>
#include <stdio.h>
#include <string.h>
//====================================================================================================================================

//=============================================
//�}�N����`
//=============================================
#define CLASS_NAME "WindowClass"    //�E�B���h�E�N���X�̖��O
#define WINDOW_NAME "AcrobaticGuns" //�E�C���h�E�̂Ȃ܂��i�L�����v�V�����ɕ\���j
#define MAX_MOTIONMODE (1)          //���샂�[�h�̐�

//====================================================================================================================================

//============================================
//�\����
//============================================

//=============================================
//�O���[�o���ϐ�
//=============================================
CManager* g_pManager = nullptr;
int g_nCountFPS = 0;//FPS�J�E���^
//======================================================================================================================================

//=============================================
//�v���g�^�C�v�錾�imain)
//=============================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);//�E�C���h�E�v���V�[�W��
//====================================================================================================================================

//============================================
//���C���֐�
//============================================
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hInstancePrev, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)//�啶�������������ŁA�������Ȃ��Ȃ�ꍇ������̂ł������菑�����B�������̑O��_In_�������Winmain�̔j����������B
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//int* buf = DBG_NEW int;
	//*buf = 10;
	//delete buf;
	//buf = nullptr;

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),//WNDCLASSEX�̃������T�C�Y
		CS_CLASSDC,//�E�C���h�E�̃X�^�C��
		WindowProc,//�E�C���h�E�v���V�[�W��
		0,//�O�ɂ���
		0,//�O�ɂ���@
		hInstance,//�C���X�^���X�n���h��
		LoadIcon(NULL,IDI_APPLICATION),//�^�X�N�o�[�̃A�C�R��
	LoadCursor(NULL,IDC_ARROW),//�}�E�X�J�[�\��
	(HBRUSH)(COLOR_WINDOW + 1),//�N���C�A���g�̈�̔w�i�F
	NULL,//���j���[�o�[
	CLASS_NAME,//�E�C���h�E�N���X�̖��O
	LoadIcon(NULL,IDI_APPLICATION)//�t�@�C���̃A�C�R��
	};

	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };//��ʃT�C�Y�̍\����

	HWND hWnd;//�E�C���h�E�n���h���i���ʎq�j
	MSG msg;//���b�Z�[�W���i�[����ϐ�

	//�E�C���h�E�̓o�^
	RegisterClassEx(&wcex);

	//�N���C�A���g�̈���w��̃T�C�Y�ɒ���
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	//�E�C���h�E�𐶐�
	hWnd = CreateWindowEx(0,//�g���E�C���h�E�X�^�C��
		CLASS_NAME,//�E�C���h�E�N���X�̖��O
		WINDOW_NAME,//�E�C���h�E�̖��O
		WS_OVERLAPPEDWINDOW,//�E�C���h�E�X�^�C��
		CW_USEDEFAULT,//�E�C���h�E�̍���X���W
		CW_USEDEFAULT,//�E�C���h�E�̍���Y���W
		(rect.right - rect.left),	//�E�C���h�E�̕�
		(rect.bottom - rect.top),//�E�C���h�E�̍���
		NULL,//�e�E�C���h�E�̃n���h��
		NULL,//���j���[�n���h��
		hInstance,//�C���X�^���X�n���h��
		NULL);//�E�C���h�E�쐬�f�[�^

	DWORD dwCurrentTime;//���ݎ���
	DWORD dwExecLastTime;//�Ō�ɏ�����������
	DWORD dwFrameCount;//�t���[���J�E���g
	DWORD dwFPSLastTime;//�Ō��FPS���v����������

	//=============================
	//�����_���[�̐�����������
	//=============================
	g_pManager = DBG_NEW CManager();//�����_���[����
	g_pManager->Init(hInstance, hWnd, TRUE);
	//==========================================================================

	//����\��ݒ�
	timeBeginPeriod(1);
	dwCurrentTime = 0;//����������
	dwExecLastTime = timeGetTime();//���ݎ������擾�i�ۑ��j
	dwFrameCount = 0;
	dwFPSLastTime = timeGetTime();

	//�E�C���h�E�̕\��
	ShowWindow(hWnd, nCmdShow);//�E�C���h�E�̕\����Ԃ�ݒ�B
	UpdateWindow(hWnd);//�N���C�A���g�̈���X�V

	//���b�Z�[�W���[�v
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{//windows�̏���
			if (msg.message == WM_QUIT)
			{//WM_QUIT���b�Z�[�W���󂯎�����烁�b�Z�[�W���[�v�𔲂���
				break;
			}
			else
			{
				//���b�Z�[�W�̐ݒ�
				TranslateMessage(&msg);//���z�L�[���b�Z�[�W�𕶎����b�Z�[�W�֕ϊ�
				DispatchMessage(&msg);//�E�C���h�E�v���V�[�W���փ��b�Z�[�W��n�o
			}
		}
		else
		{//DirectX�̏���
			dwCurrentTime = timeGetTime();//���ݎ������擾

			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{//0.5�b���v��
				//FPS���v��
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);
				dwFPSLastTime = dwCurrentTime;//FPS�𑪒肵��������ۑ�
				dwFrameCount = 0;//�t���[���J�E���g���N���A

			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{//�U�O���̂P�b�o��
				dwExecLastTime = dwCurrentTime;//�����J�n�̎���
				//���ݎ�����ۑ�

			    //�X�V����
				g_pManager->Update();
				//�`�揈��
				g_pManager->Draw();
				dwFrameCount++;
			}
		}
	}


	//����\��߂�
	timeEndPeriod(1);

	//�I������
	g_pManager->Uninit();
	delete g_pManager;
	g_pManager = nullptr;

	//�E�C���h�E�N���X�̓o�^������
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	//_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
 //   _CrtDumpMemoryLeaks();
	return (int)msg.wParam;
}
//==========================================================================================================================================

//=======================================================================================
//�E�C���h�E�v���V�[�W��
//=======================================================================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;//�Ԃ�l���i�[
	switch (uMsg)
	{
	case WM_DESTROY://�E�C���h�E�j���̃��b�Z�[�W
		//WM_QUIT���b�Z�[�W�𑗂�B
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN://�L�[�����̃��b�Z�[�W
		switch (wParam)
		{
		case VK_ESCAPE://�G�X�P�[�v�L�[�������ꂽ

			nID = MessageBox(hWnd, "�I�����܂����H", "�I�����b�Z�[�W", MB_YESNO);//����������hWnd�ɂ���ƁA���̃E�C���h�E���e�i�I�[�i�[�j�ɂȂ�A���̃��b�Z�[�W�{�b�N�X���������Ȃ�����A�ق��̏������ł��Ȃ��B
			if (nID == IDYES)
			{
				//�E�C���h�E��j������B
				DestroyWindow(hWnd);//
			}

			break;
		}
		break;
	case WM_CLOSE://����{�^�������̃��b�Z�[�W
		nID = MessageBox(hWnd, "�I�����܂����H", "�I�����b�Z�[�W", MB_YESNO);

		if (nID == IDYES)
		{
			//�E�C���h�E��j������B
			DestroyWindow(hWnd);
		}
		else
		{
			return 0;//�O��Ԃ��Ȃ��ƏI�����Ă��܂��B
		}
		
	}


	return DefWindowProc(hWnd, uMsg, wParam, lParam);//����̏�����Ԃ��B
}
//==========================================================================================================================================

int GetFPS()
{
	return g_nCountFPS;
}

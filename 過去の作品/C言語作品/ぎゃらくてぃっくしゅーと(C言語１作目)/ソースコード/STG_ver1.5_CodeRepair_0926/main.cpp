//========================================================
//
//�W���R�O���F�G�t�F�N�g������񌩒���[main.cpp]
//Author:ShinaTaiyo
//
//=========================================================


#include "main.h"
#include "polygon2.h"
#include "player.h"
#include "input.h"
#include "bullet.h"
#include "explosion.h"
#include "enemy.h"
#include "bg.h"
#include "score.h"
#include "title.h"
#include "game.h"
#include "result.h"
#include "tutorial.h"
#include "sound.h"
#include "fade.h"
#include <time.h>
#include "effect.h"
#include "particle.h"
#include "SkillGage.h"
#include "SkillName.h"
#include "ranking.h"
#include "staging.h"
#include "Difficulty.h"
#include "damage.h"
//�}�N����`
#define CLASS_NAME "WindowClass" //�E�B���h�E�N���X�̖��O
#define WINDOW_NAME "����炭�Ă���������[��" //�E�C���h�E�̂Ȃ܂��i�L�����v�����ɕ\���j
#define MAX_MOTIONMODE (11)//���샂�[�h�̐�

//�v���g�^�C�v�錾
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update();
void Draw(void);
void DrawFPS(void);

//�O���[�o���ϐ�
LPDIRECT3D9 g_pD3D = NULL;//Direct3D�I�u�W�F�N�g�ւ̃|�C���^
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;//Direct3D�f�o�C�X�ւ̃|�C���^
LPD3DXFONT g_pFont = NULL;//�t�H���g�ւ̃|�C���^
int g_nCountFPS = 0;//FPS�J�E���^
MODE g_mode = MODE_TITLE;//���݂̃��[�h
Enemy g_aEnemy;

//============================================
//���C���֐�
//============================================
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hInstancePrev, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)//�啶�������������ŁA�������Ȃ��Ȃ�ꍇ������̂ł������菑�����B�������̑O��_In_�������Winmain�̔j����������B
{
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


	//����������
	if (FAILED(Init(hInstance, hWnd, TRUE)))
	{//���������������s�����ꍇ
		return -1;//�i���̌�̏��������Ȃ��j
	}

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
				Update();
				//�`�揈��
				Draw();
				dwFrameCount++;
			}
		}
	}
	//�I������
	Uninit();
	//����\��߂�
	timeEndPeriod(1);

	//�E�C���h�E�N���X�̓o�^������
	UnregisterClass(CLASS_NAME, wcex.hInstance);
	return (int)msg.wParam;
}
//�E�C���h�E�v���V�[�W��
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;//�Ԃ�l���i�[
	HWND hWndButtonFinish;//�I���{�^���̃E�C���h�E�n���h���i���ʎq�j
	HWND hWndButtonExplanation;//�I���{�^���̃E�C���h�E�n���h���i���ʎq�j


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

//=================================================
//����������
//=================================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;//�f�B�X�v���C���[�h
	D3DPRESENT_PARAMETERS d3dpp;//�v���[���e�[�V�����p�����[�^

	//DirectX3D�I�u�W�F�N�g�̐���
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}
	//���݂̃f�B�X�v���C���[�h���擾
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}
	////���݂̃f�B�X�v���C���[�h���擾
	//if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	//{
	//	return E_FAIL;
	//}

	//�f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp));//�p�����[�^�̃[���N���A
	d3dpp.BackBufferWidth = SCREEN_WIDTH;//�Q�[����ʃT�C�Y�i���j
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;//�Q�[����ʃT�C�Y�i�����j
	d3dpp.BackBufferFormat = d3ddm.Format;//�o�b�N�o�b�t�@�̌`��
	d3dpp.BackBufferCount = 1;//�o�b�N�o�b�t�@�̐�
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;//�_�u���o�b�t�@�̐؂�ւ��i�f���M���ɓ����j
	d3dpp.EnableAutoDepthStencil = TRUE;//�f�v�X�o�b�t�@�ƃX�e���V���o�b�t�@���쐬
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;//�f�v�X�o�b�t�@�Ƃ���16bit���g��
	d3dpp.Windowed = bWindow;//�E�C���h�E���[�h
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;//���t���b�V�����[�g
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;//�C���^�[�o��

	//Direct3D�f�o�C�X�̐����i�`�揈���ƒ��_�������n�[�h�E�F�A�ōs���j
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&g_pD3DDevice)))
	{
		//Direct3D�f�o�C�X�̐����i�`�揈���̓n�[�h�E�F�A�A���_������CPU�ōs��
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&g_pD3DDevice)))
		{
			//Direct3D�f�o�C�X�̐����i�`�揈���ƒ��_������CPU�ōs���j
			if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&g_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}

	//�����_�[�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//�T���v���[�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	//�e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1,D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0,D3DTSS_ALPHAARG2,D3DTA_CURRENT);


	//�f�o�b�N�\���p�t�H���g�̐���
	D3DXCreateFont(g_pD3DDevice, 18, 0, 0, 0,
		FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH,
		"terminal", &g_pFont);

	//�L�[�{�[�h�̏���������
	if(FAILED(InitKeyboard(hInstance,hWnd)))
	{
		return E_FAIL;
	}

	//�W���C�p�b�h���̏���������
	if (FAILED(InitJoypad()))
	{
		return E_FAIL;
	}

	//�|���S���̏���������


	////�^�C�g������������
	//InitTitle();

	////�Q�[����ʏ���������
	//InitGame();

	////���U���g��ʏ���������
	//InitResult();

	//�G�̏���������
	/*InitEnemy();*/

	//�w�i����������
	//InitBG();

	srand((unsigned int)time(NULL));//�Q�[�����J�n�������Ԗ��ɗ����̎��ݒ�

	//�T�E���h�̏���������
	InitSound(hWnd);

	/*InitTitle();*/

	//���[�h�̐ݒ�
	SetMode(g_mode);
	return S_OK;
}

//================================================
//�I������
//================================================
void Uninit(void)
{

	//�^�C�g����ʏI������
	UninitTitle();

	/*InitTutorial();*/

	//�Q�[����ʂ̏I������
	UninitGame();

	//���U���g��ʂ̏I������
	UninitResult();

	//�T�E���h�̏I������
	UninitSound();
	//�t�F�[�h�I������
	UninitFade();

	UninitKeyboard();

	UninitJoypad();

	////�|���S���̏I������
	//UninitPlayer();
	//UninitPolygon_2();
	//UninitBullet();
	//UninitExplosion();
	//UninitEnemy();
	//UninitScore();
	//UninitBG();

	//�f�o�b�N�\���p�t�H���g�̔j��
	if (g_pFont != NULL)
	{
		g_pFont->Release();
		g_pFont = NULL;
	}

	UninitKeyboard();

	//Direct3D�f�o�C�X�̔j��
	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	//Direct3D�I�u�W�F�N�g�̔j��
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}
}

//====================================================
//�X�V����
//====================================================
void Update(void)
{
	//�L�[�{�[�h�̍X�V����
	UpdateKeyboard();

	//�W���C�p�b�h�̍X�V����
	UpdateJoypad();

	//�|���S���̍X�V����
	/*UpDatePlayer();
	UpDatePolygon_2();
	UpdateBullet();
	UpdateExplosion();
	UpdateEnemy();
	UpdateScore();
	UpDateBG();*/

	switch (g_mode)
	{
	case MODE_TITLE://�^�C�g�����
		UpdateTitle();
		UpdateStaging();
		UpdateFade();
		break;
	case MODE_TUTORIAL://�`���[�g���A�����
		UpdateEffect();
		UpdateParticle();
		UpdateEnemy();
		UpdateExplosion();
		UpdateBullet();
		UpDatePlayer();
		UpdateTutorial();
		UpdateStaging();
		UpdateSkillGage();
		UpdateSkillName();
		UpdateFade();
		break;
	case MODE_CHOOSE_DIFFICULTY://��Փx�I�����
		UpdateEffect();
		UpdateParticle();
		UpdateDamage();
		UpdateEnemy();
		UpdateExplosion();
		UpdateBullet();
		UpDatePlayer();
		UpdateScore();
		UpdateDifficulty();
		UpdateStaging();
		UpdateFade();
		break;
	case MODE_GAME://�Q�[�����
		UpdateGame();
		break;
	case MODE_RESULT://���U���g���
		/*UpdateScore();*/
		UpdateResult();
		UpdateStaging();
		UpdateFade();
		break;
	case MODE_RANKING://�����L���O���
		UpdateStaging();
		UpdateRanking();
	/*	UpdateScore();*/
		UpdateFade();
	}
	

}

//=====================================================
//�`�揈��
//=====================================================
void Draw(void)
{
	//��ʃN���A�i�o�b�N�o�b�t�@��Z�o�b�t�@�̃N���A)
	g_pD3DDevice->Clear(0, NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	//�`��J�n
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{//�`��J�n�����������ꍇ

		//�|���S���̕`�揈��
		/*DrawPolygon_2();
		DrawPlayer();
		DrawBullet();
		DrawExplosion();
		DrawEnemy();
		DrawScore();
		DrawBG();*/

		switch (g_mode)//���[�h�I��
		{
		case MODE_TITLE://�^�C�g�����
			DrawTitle();
			DrawStaging();
			DrawFade();
			break;
		case MODE_TUTORIAL:
			DrawEffect();
		    DrawParticle();
			DrawDamage();
			DrawEnemy();
			DrawExplosion();
			DrawBullet();
			DrawPlayer();
			DrawTutorial();//�`���[�g���A�����
			DrawStaging();
			DrawSkillGage();
			DrawSkillName();
			DrawFade();

			break;
		case MODE_CHOOSE_DIFFICULTY://��Փx�I�����
			DrawEffect();
			DrawParticle();
			DrawEnemy();
			DrawExplosion();
			DrawBullet();
			//DrawPlayer();
			DrawScore();
			DrawDifficulty();
			DrawStaging();
			DrawFade();
			break;
		case MODE_GAME://�Q�[�����
			DrawGame();
			break;
		case MODE_RESULT://���U���g���
			DrawScore();
			DrawResult();
			DrawStaging();
			DrawFade();
			break;
		case MODE_RANKING:
			DrawStaging();
			DrawRanking();
			/*DrawScore();*/
			DrawFade();
		}

		//FPS�̕\��
#ifdef _DEBUG
		DrawFPS();//�f�o�b�N�i�r���h�j�������\��
#endif 


		//�`��I��
		g_pD3DDevice->EndScene();
	}
	//�o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}
//======================================
//�f�o�C�X�̎擾
//======================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

//============================================
//FPS�̕\��
//============================================
void DrawFPS(void)
{
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[MAX_MOTIONMODE][256];
	int nCount;

    Defeat* pDefeat = GetDefeat();

	Bullet* pBullet = GetBullet();
	//������ɑ��
	wsprintf(&aStr[0][0], "FPS:%d", g_nCountFPS);
	wsprintf(&aStr[1][0], "\n�g���K�[���˂�F1�L�[");
	wsprintf(&aStr[2][0], "\n\n	�S�Ă�j�󃂁[�h��F2�L�[");
	wsprintf(&aStr[3][0], "\n\n\n��]���˂�F3�L�[");
	wsprintf(&aStr[4][0], "\n\n\n\n�ϓ���]���˂�F4�L�[");
	wsprintf(&aStr[5][0], "\n\n\n\n\n�t�ϓ���]���˂�F5�L�[");
	wsprintf(&aStr[6][0], "\n\n\n\n\n\n�������������������z�[�~���O��H�L�[�I����������������");
	wsprintf(&aStr[7][0], "\n\n\n\n\n\n\n�����^�Tway�e��F7�L�[�i�p�x�����u���L�[�A���L�[�v)");
	wsprintf(&aStr[8][0], "\n\n\n\n\n\n\n\n�X�L���̎g�p�́uJ�v�L�[");
	wsprintf(&aStr[9][0], "\n\n\n\n\n\n\n\n\n�X�L���̑I���́uI�v�L�[�A�uK�v�L�[");
	wsprintf(&aStr[10][0], "\n\n\n\n\n\n\n\n\n\n�G���j��%d",pDefeat->DefeatEnemyCount);

	//�e�L�X�g�̕`��
	for (nCount = 0; nCount < MAX_MOTIONMODE; nCount++)
	{
		g_pFont->DrawText(NULL, &aStr[nCount][0], -1, &rect, DT_LEFT,
			D3DCOLOR_RGBA(255, 255, 255, 255));
	}
}

//=======================================
//���[�h�ύX���s���֐�
//=======================================
void SetMode(MODE mode)
{
	//���݂̉�ʁi���[�h�j�̏I������
	switch (g_mode)
	{
	case MODE_TITLE://�^�C�g�����
		UninitTitle();
		UninitStaging();
		UninitFade();
		break;
	case MODE_TUTORIAL:
		UninitEffect();
		UninitParticle();
		UninitEnemy();
		UninitExplosion();
		UninitBullet();
		UninitPlayer();
		UninitTutorial();//�`���[�g���A�����
		UninitStaging();
		UninitSkillGage();
		UninitSkillName();
		UninitFade();
		break;
	case MODE_CHOOSE_DIFFICULTY://��Փx�I�����
		UninitEffect();
		UninitParticle();
		UninitDamage();
		UninitEnemy();
		UninitExplosion();
		UninitBullet();
		UninitPlayer();
		UninitScore();
		UninitDifficulty();
		UninitStaging();
		UninitFade();
		break;
	case MODE_GAME://�Q�[�����
		UninitGame();
		break;
	case MODE_RESULT://���U���g���
	/*	UninitScore();*/
	/*	UninitRanking();*/
		UninitResult();
		UninitStaging();
		UninitFade();
		break;
	case MODE_RANKING:
		UninitStaging();
		UninitRanking();
		UninitFade();
	}

//�V������ʁi���[�h�j�̏���������
	switch (mode)
	{
	case MODE_TITLE://�^�C�g�����
		InitTitle();
		InitStaging();
		InitFade();
		break;
	case MODE_TUTORIAL:
		InitEffect();
	    InitParticle();
		InitEnemy();
		InitExplosion();
		InitBullet();
		InitPlayer();
		InitTutorial();//�`���[�g���A�����
		InitStaging();
		InitSkillGage();
		InitSkillName();
		InitFade();
		SetStaging(STAGINGTYPE_PRESSENTER, -1, D3DXVECTOR3(SCREEN_WIDTH - 120.0f, SCREEN_HEIGHT - 130.0f, 0.0f));
		break;
	case MODE_CHOOSE_DIFFICULTY://��Փx�I�����
		InitEffect();
		InitParticle();
		InitDamage();
		InitEnemy();
		InitExplosion();
		InitBullet();
		InitPlayer();
		InitScore();
		InitDifficulty();
		InitStaging();
		InitFade();
		SetStaging(STAGINGTYPE_PRESSENTER, -1, D3DXVECTOR3(SCREEN_WIDTH - 120.0f, SCREEN_HEIGHT - 130.0f, 0.0f));
		break;
	case MODE_GAME://�Q�[�����
		InitGame();
		break;
	case MODE_RESULT://���U���g���
	/*	InitScore();*/
	/*	InitRanking();*/
		InitResult();
		InitStaging();
		InitFade();
		SetStaging(STAGINGTYPE_PRESSENTER, -1, D3DXVECTOR3(SCREEN_WIDTH - 120.0f, SCREEN_HEIGHT - 70.0f, 0.0f));
		break;
	case MODE_RANKING:

		InitStaging();
		InitRanking();
		InitFade();
		SetStaging(STAGINGTYPE_PRESSENTER, -1, D3DXVECTOR3(SCREEN_WIDTH - 120.0f, SCREEN_HEIGHT - 70.0f, 0.0f));
	}
	//�V������ʁi���[�h�j�̏���������

	g_mode = mode;//���݂̉�ʁi���[�h�j��؂�ւ���
}

//���[�h�̎擾
MODE GetMode(void)
{
	return g_mode;
}



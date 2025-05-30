//========================================================
//
//１２月２６日：チーム制作の基盤作成[main.cpp]
//Author:ShinaTaiyo
//
//=========================================================
#include "main.h"
#include "input.h"
#include "title.h"
#include "game.h"
#include "result.h"
#include "tutorial.h"
#include "sound.h"
#include "fade.h"
#include <time.h>
#include "ranking.h"
#include "camera.h"
#include "light.h"
#include <stdio.h>
#include "debugproc.h"

//マクロ定義
#define CLASS_NAME "WindowClass" //ウィンドウクラスの名前
#define WINDOW_NAME "JOBIロボ！" //ウインドウのなまえ（キャンプしょんに表示）
#define MAX_MOTIONMODE (20)//動作モードの数

//プロトタイプ宣言
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update();
void Draw(void);
void DrawFPS(void);

//グローバル変数
LPDIRECT3D9 g_pD3D = NULL;//Direct3Dオブジェクトへのポインタ
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;//Direct3Dデバイスへのポインタ
LPD3DXFONT g_pFont = NULL;//フォントへのポインタ
int g_nCountFPS = 0;//FPSカウンタ
MODE g_mode = MODE_TITLE;//現在のモード
bool g_bEnd= false;	//ゲームを終了するか
bool g_isFullscreen = false;//ウィンドウを切り替え用のフラグ
RECT g_windowRect;//ウィンドウを切り替え用

//============================================
//メイン関数
//============================================
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hInstancePrev, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)//大文字が小文字かで、反応しなくなる場合があるのでしっかり書こう。引き数の前に_In_をつけるとWinmainの破線を消せる。
{
	(void)lpCmdLine;
	(void)hInstancePrev;

	g_pD3D = NULL;//Direct3Dオブジェクトへのポインタ
	g_pD3DDevice = NULL;//Direct3Dデバイスへのポインタ
	g_pFont = NULL;//フォントへのポインタ
	g_nCountFPS = 0;//FPSカウンタ
	g_mode = MODE_TITLE;//現在のモード

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),//WNDCLASSEXのメモリサイズ
		CS_CLASSDC,//ウインドウのスタイル
		WindowProc,//ウインドウプロシージャ
		0,//０にする
		0,//０にする　
		hInstance,//インスタンスハンドル
		LoadIcon(NULL,IDI_APPLICATION),//タスクバーのアイコン
		LoadCursor(NULL,IDC_ARROW),//マウスカーソル
		(HBRUSH)(COLOR_WINDOW + 1),//クライアント領域の背景色
		NULL,//メニューバー
		CLASS_NAME,//ウインドウクラスの名前
		LoadIcon(NULL,IDI_APPLICATION)//ファイルのアイコン
	};

	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };//画面サイズの構造体

	HWND hWnd;//ウインドウハンドル（識別子）
	MSG msg;//メッセージを格納する変数

	//ウインドウの登録
	RegisterClassEx(&wcex);

	//クライアント領域を指定のサイズに調整
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	//ウインドウを生成
	hWnd = CreateWindowEx(0,//拡張ウインドウスタイル
		CLASS_NAME,//ウインドウクラスの名前
		WINDOW_NAME,//ウインドウの名前
		WS_OVERLAPPEDWINDOW,//ウインドウスタイル
		CW_USEDEFAULT,//ウインドウの左上X座標
		CW_USEDEFAULT,//ウインドウの左上Y座標
		(rect.right - rect.left),	//ウインドウの幅
		(rect.bottom - rect.top),//ウインドウの高さ
		NULL,//親ウインドウのハンドル
		NULL,//メニューハンドル
		hInstance,//インスタンスハンドル
		NULL);//ウインドウ作成データ

	DWORD dwCurrentTime;//現在時刻
	DWORD dwExecLastTime;//最後に処理した時刻
	DWORD dwFrameCount;//フレームカウント
	DWORD dwFPSLastTime;//最後にFPSを計測した時刻


	//初期化処理
	if (FAILED(Init(hInstance, hWnd, TRUE)))
	{//初期化処理が失敗した場合
		return -1;//（この後の処理をしない）
	}

	//分解能を設定
	timeBeginPeriod(1);
	dwCurrentTime = 0;//初期化する
	dwExecLastTime = timeGetTime();//現在時刻を取得（保存）
	dwFrameCount = 0;
	dwFPSLastTime = timeGetTime();

	//ウインドウの表示
	ShowWindow(hWnd, nCmdShow);//ウインドウの表示状態を設定。
	UpdateWindow(hWnd);//クライアント領域を更新

	//メッセージループ
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{//windowsの処理
			if (msg.message == WM_QUIT)
			{//WM_QUITメッセージを受け取ったらメッセージループを抜ける
				break;
			}
			else
			{
				//メッセージの設定
				TranslateMessage(&msg);//仮想キーメッセージを文字メッセージへ変換
				DispatchMessage(&msg);//ウインドウプロシージャへメッセージを創出
			}
		}
		else
		{//DirectXの処理
			dwCurrentTime = timeGetTime();//現在時刻を取得

			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{//0.5秒を計測
				//FPSを計測
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);
				dwFPSLastTime = dwCurrentTime;//FPSを測定した時刻を保存
				dwFrameCount = 0;//フレームカウントをクリア

			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{//６０分の１秒経過
				dwExecLastTime = dwCurrentTime;//処理開始の時刻
				//現在時刻を保存

				//更新処理
				Update();
				//描画処理
				Draw();
				dwFrameCount++;
			}

			// もしゲームを終了するなら
			if (g_bEnd == true)
			{
				//ウインドウを破棄する。
				DestroyWindow(hWnd);
			}
		}
	}

	//終了処理
	Uninit();
	//分解能を戻す
	timeEndPeriod(1);

	//ウインドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);
	return (int)msg.wParam;
}
//ウインドウプロシージャ
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;//返り値を格納
	//HWND hWndButtonFinish;//終了ボタンのウインドウハンドル（識別子）
	//HWND hWndButtonExplanation;//終了ボタンのウインドウハンドル（識別子）


	switch (uMsg)
	{
	case WM_DESTROY://ウインドウ破棄のメッセージ
		//WM_QUITメッセージを送る。
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN://キー押下のメッセージ
		switch (wParam)
		{
		case VK_ESCAPE://エスケープキーが押された

			nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO);//第一引き数をhWndにすると、このウインドウが親（オーナー）になり、このメッセージボックスを処理しない限り、ほかの処理ができない。
			if (nID == IDYES)
			{
				//ウインドウを破棄する。
				DestroyWindow(hWnd);//
			}

			break;

		case VK_F11:
			ToggleFullscreen(hWnd);
			break;
		}
		break;
	case WM_CLOSE://閉じるボタン押下のメッセージ
		nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO);

		if (nID == IDYES)
		{
			//ウインドウを破棄する。
			DestroyWindow(hWnd);
		}
		else
		{
			return 0;//０を返さないと終了してしまう。
		}
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);//既定の処理を返す。
}

//=================================================
//初期化処理
//=================================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;//ディスプレイモード
	D3DPRESENT_PARAMETERS d3dpp;//プレゼンテーションパラメータ

	//DirectX3Dオブジェクトの生成
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}
	//現在のディスプレイモードを取得
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	//デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));//パラメータのゼロクリア
	d3dpp.BackBufferWidth = SCREEN_WIDTH;//ゲーム画面サイズ（幅）
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;//ゲーム画面サイズ（高さ）
	d3dpp.BackBufferFormat = d3ddm.Format;//バックバッファの形式
	d3dpp.BackBufferCount = 1;//バックバッファの数
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;//ダブルバッファの切り替え（映像信号に同期）
	d3dpp.EnableAutoDepthStencil = TRUE;//デプスバッファとステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;//デプスバッファとして16bitを使う
	d3dpp.Windowed = bWindow;//ウインドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;//リフレッシュレート
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;//インターバル

	//Direct3Dデバイスの生成（描画処理と頂点処理をハードウェアで行う）
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&g_pD3DDevice)))
	{
		//Direct3Dデバイスの生成（描画処理はハードウェア、頂点処理はCPUで行う
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&g_pD3DDevice)))
		{
			//Direct3Dデバイスの生成（描画処理と頂点処理をCPUで行う）
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

	//レンダーステートの設定
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//サンプラーステートの設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	//テクスチャステージステートの設定
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1,D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0,D3DTSS_ALPHAARG2,D3DTA_CURRENT);


	//デバック表示用フォントの生成
	D3DXCreateFont(g_pD3DDevice, 18, 0, 0, 0,
		FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH,
		"terminal", &g_pFont);

	//キーボードの初期化処理
	if(FAILED(InitKeyboard(hInstance,hWnd)))
	{
		return E_FAIL;
	}

	//マウスの初期化処理
	if (FAILED(InitMouse(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	//ジョイパッドをの初期化処理
	if (FAILED(InitJoypad()))
	{
		return E_FAIL;
	}

	srand((unsigned int)time(NULL));//ゲームを開始した時間毎に乱数の種を設定

	//サウンドの初期化処理
	InitSound(hWnd);

	//デバッグ表示の初期化処理
	InitDebugProc();

	//フェードの初期化処理
	InitFade();

	//モードの設定
	SetMode(g_mode);
	return S_OK;
}

//================================================
//終了処理
//================================================
void Uninit(void)
{
	//サウンドの終了処理
	UninitSound();
	UninitKeyboard();
	UninitMouse();	
	UninitJoypad();
	UninitFade();

	//デバッグ表示の終了処理
	UninitDebugProc();

	//デバック表示用フォントの破棄
	if (g_pFont != NULL)
	{
		g_pFont->Release();
		g_pFont = NULL;
	}

	//Direct3Dデバイスの破棄
	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	//Direct3Dオブジェクトの破棄
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}
}

//====================================================
//更新処理
//====================================================
void Update(void)
{
	//キーボードの更新処理
	UpdateKeyboard();

	//マウスの更新処理
	UpdateMouse();

	//ジョイパッドの更新処理
	UpdateJoypad();

	//スティックの更新処理
	UpdateStick();

	//デバッグ表示の更新処理
	UpdateDebugProc();

	switch (g_mode)
	{
	case MODE_TITLE://タイトル画面
		UpdateTitle();
		break;
	case MODE_TUTORIAL://チュートリアル画面
		UpdateTutorial();
		break;
	case MODE_GAME://ゲーム画面
		UpdateGame();
		break;
	case MODE_RESULT://リザルト画面
		UpdateResult();
		break;
	case MODE_RANKING://ランキング画面
		UpdateRanking();
		break;
	}

	UpdateFade();
}

//=====================================================
//描画処理
//=====================================================
void Draw(void)
{
	//画面クリア（バックバッファ＆Zバッファのクリア)
	g_pD3DDevice->Clear(0, NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0,0,0,255), 1.0f, 0);

	//描画開始
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{//描画開始が成功した場合
		switch (g_mode)//モード選択
		{
		case MODE_TITLE://タイトル画面
			DrawTitle();
			break;
		case MODE_TUTORIAL:
			DrawTutorial();//チュートリアル画面
			break;
		case MODE_GAME://ゲーム画面
			DrawGame();
			break;
		case MODE_RESULT://リザルト画面
			DrawResult();
			break;
		case MODE_RANKING:
			DrawRanking();
			break;
		}

		//フェードの描画処理
		DrawFade();

		//デバッグ表示の描画処理
		DrawDebugProc();

		//描画終了
		g_pD3DDevice->EndScene();
	}
	//バックバッファとフロントバッファの入れ替え
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}
//======================================
//デバイスの取得
//======================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

//============================================
//FPSの表示、デバック表示
//============================================
void DrawFPS(void)
{
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[MAX_MOTIONMODE][256];
	int nCount;

	//テキストの描画
	for (nCount = 0; nCount < MAX_MOTIONMODE; nCount++)
	{
		g_pFont->DrawText(NULL, &aStr[nCount][0], -1, &rect, DT_LEFT,
			D3DCOLOR_RGBA(255, 0, 0, 255));
	}
}

//=======================================
//モード変更を行う関数
//=======================================
void SetMode(MODE mode)
{
	//現在の画面（モード）の終了処理
	switch (g_mode)
	{
	case MODE_TITLE://タイトル画面
		UninitTitle();
		break;
	case MODE_TUTORIAL:
		UninitTutorial();//チュートリアル画面
		break;
	case MODE_GAME://ゲーム画面
		UninitGame();
		break;
	case MODE_RESULT://リザルト画面
		UninitResult();
		break;
	case MODE_RANKING:
		UninitRanking();
		break;
	}

	//新しい画面（モード）の初期化処理
	g_mode = mode;//現在の画面（モード）を切り替える

//新しい画面（モード）の初期化処理
	switch (mode)
	{
	case MODE_TITLE:	//タイトル画面
		InitTitle();
		break;
	case MODE_TUTORIAL:
		InitTutorial();	//チュートリアル画面
		break;
	case MODE_GAME:		//ゲーム画面
		InitGame();
		break;
	case MODE_RESULT:	//リザルト画面
		InitResult();
		break;
	case MODE_RANKING:	//ランキング画面
		InitRanking();
		break;
	case MODE_END:		// 終了処理開始
		g_bEnd = true;
		break;
	}
}

//=======================================
//モードの取得
//=======================================
MODE GetMode(void)
{
	return g_mode;
}

//=======================================
//ウィンドウをフルスクリーンにする
//=======================================
void ToggleFullscreen(HWND hWnd)
{
	// 現在のウィンドウスタイルを取得
	DWORD dwStyle = GetWindowLong(hWnd, GWL_STYLE);

	if (g_isFullscreen)
	{
		// ウィンドウモードに切り替え
		SetWindowLong(hWnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPos(hWnd, HWND_TOP, g_windowRect.left, g_windowRect.top,
			g_windowRect.right - g_windowRect.left, g_windowRect.bottom - g_windowRect.top,
			SWP_FRAMECHANGED | SWP_NOACTIVATE);
		ShowWindow(hWnd, SW_NORMAL);
	}
	else
	{
		// フルスクリーンモードに切り替え
		GetWindowRect(hWnd, &g_windowRect);
		SetWindowLong(hWnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);
		SetWindowPos(hWnd, HWND_TOP, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
			SWP_FRAMECHANGED | SWP_NOACTIVATE);
		ShowWindow(hWnd, SW_MAXIMIZE);
	}

	g_isFullscreen = !g_isFullscreen;
}

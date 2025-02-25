//================================================================================
//
//５月１７日：ポリゴンを出す処理と、メインの処理のクラス化[main.cpp]
//Author:ShinaTaiyo
//
//================================================================================

//=============================================
//インクルード
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
//マクロ定義
//=============================================
#define CLASS_NAME "WindowClass"    //ウィンドウクラスの名前
#define WINDOW_NAME "AcrobaticGuns" //ウインドウのなまえ（キャンプションに表示）
#define MAX_MOTIONMODE (1)          //動作モードの数

//====================================================================================================================================

//============================================
//構造体
//============================================

//=============================================
//グローバル変数
//=============================================
CManager* g_pManager = nullptr;
int g_nCountFPS = 0;//FPSカウンタ
//======================================================================================================================================

//=============================================
//プロトタイプ宣言（main)
//=============================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);//ウインドウプロシージャ
//====================================================================================================================================

//============================================
//メイン関数
//============================================
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hInstancePrev, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)//大文字が小文字かで、反応しなくなる場合があるのでしっかり書こう。引き数の前に_In_をつけるとWinmainの破線を消せる。
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//int* buf = DBG_NEW int;
	//*buf = 10;
	//delete buf;
	//buf = nullptr;

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

	//=============================
	//レンダラーの生成＆初期化
	//=============================
	g_pManager = DBG_NEW CManager();//レンダラー生成
	g_pManager->Init(hInstance, hWnd, TRUE);
	//==========================================================================

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
				g_pManager->Update();
				//描画処理
				g_pManager->Draw();
				dwFrameCount++;
			}
		}
	}


	//分解能を戻す
	timeEndPeriod(1);

	//終了処理
	g_pManager->Uninit();
	delete g_pManager;
	g_pManager = nullptr;

	//ウインドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	//_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
 //   _CrtDumpMemoryLeaks();
	return (int)msg.wParam;
}
//==========================================================================================================================================

//=======================================================================================
//ウインドウプロシージャ
//=======================================================================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;//返り値を格納
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
		
	}


	return DefWindowProc(hWnd, uMsg, wParam, lParam);//既定の処理を返す。
}
//==========================================================================================================================================

int GetFPS()
{
	return g_nCountFPS;
}

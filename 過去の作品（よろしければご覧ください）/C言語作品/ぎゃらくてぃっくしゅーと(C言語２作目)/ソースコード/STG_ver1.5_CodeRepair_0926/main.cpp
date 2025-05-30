//========================================================
//
//８月３０日：エフェクトもう一回見直す[main.cpp]
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
//マクロ定義
#define CLASS_NAME "WindowClass" //ウィンドウクラスの名前
#define WINDOW_NAME "ぎゃらくてぃっくしゅーと" //ウインドウのなまえ（キャンプしょんに表示）
#define MAX_MOTIONMODE (11)//動作モードの数

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
Enemy g_aEnemy;

//============================================
//メイン関数
//============================================
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hInstancePrev, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)//大文字が小文字かで、反応しなくなる場合があるのでしっかり書こう。引き数の前に_In_をつけるとWinmainの破線を消せる。
{
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
	HWND hWndButtonFinish;//終了ボタンのウインドウハンドル（識別子）
	HWND hWndButtonExplanation;//終了ボタンのウインドウハンドル（識別子）


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
	////現在のディスプレイモードを取得
	//if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	//{
	//	return E_FAIL;
	//}

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

	//ジョイパッドをの初期化処理
	if (FAILED(InitJoypad()))
	{
		return E_FAIL;
	}

	//ポリゴンの初期化処理


	////タイトル初期化処理
	//InitTitle();

	////ゲーム画面初期化処理
	//InitGame();

	////リザルト画面初期化処理
	//InitResult();

	//敵の初期化処理
	/*InitEnemy();*/

	//背景初期化処理
	//InitBG();

	srand((unsigned int)time(NULL));//ゲームを開始した時間毎に乱数の種を設定

	//サウンドの初期化処理
	InitSound(hWnd);

	/*InitTitle();*/

	//モードの設定
	SetMode(g_mode);
	return S_OK;
}

//================================================
//終了処理
//================================================
void Uninit(void)
{

	//タイトル画面終了処理
	UninitTitle();

	/*InitTutorial();*/

	//ゲーム画面の終了処理
	UninitGame();

	//リザルト画面の終了処理
	UninitResult();

	//サウンドの終了処理
	UninitSound();
	//フェード終了処理
	UninitFade();

	UninitKeyboard();

	UninitJoypad();

	////ポリゴンの終了処理
	//UninitPlayer();
	//UninitPolygon_2();
	//UninitBullet();
	//UninitExplosion();
	//UninitEnemy();
	//UninitScore();
	//UninitBG();

	//デバック表示用フォントの破棄
	if (g_pFont != NULL)
	{
		g_pFont->Release();
		g_pFont = NULL;
	}

	UninitKeyboard();

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

	//ジョイパッドの更新処理
	UpdateJoypad();

	//ポリゴンの更新処理
	/*UpDatePlayer();
	UpDatePolygon_2();
	UpdateBullet();
	UpdateExplosion();
	UpdateEnemy();
	UpdateScore();
	UpDateBG();*/

	switch (g_mode)
	{
	case MODE_TITLE://タイトル画面
		UpdateTitle();
		UpdateStaging();
		UpdateFade();
		break;
	case MODE_TUTORIAL://チュートリアル画面
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
	case MODE_CHOOSE_DIFFICULTY://難易度選択画面
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
	case MODE_GAME://ゲーム画面
		UpdateGame();
		break;
	case MODE_RESULT://リザルト画面
		/*UpdateScore();*/
		UpdateResult();
		UpdateStaging();
		UpdateFade();
		break;
	case MODE_RANKING://ランキング画面
		UpdateStaging();
		UpdateRanking();
	/*	UpdateScore();*/
		UpdateFade();
	}
	

}

//=====================================================
//描画処理
//=====================================================
void Draw(void)
{
	//画面クリア（バックバッファ＆Zバッファのクリア)
	g_pD3DDevice->Clear(0, NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	//描画開始
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{//描画開始が成功した場合

		//ポリゴンの描画処理
		/*DrawPolygon_2();
		DrawPlayer();
		DrawBullet();
		DrawExplosion();
		DrawEnemy();
		DrawScore();
		DrawBG();*/

		switch (g_mode)//モード選択
		{
		case MODE_TITLE://タイトル画面
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
			DrawTutorial();//チュートリアル画面
			DrawStaging();
			DrawSkillGage();
			DrawSkillName();
			DrawFade();

			break;
		case MODE_CHOOSE_DIFFICULTY://難易度選択画面
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
		case MODE_GAME://ゲーム画面
			DrawGame();
			break;
		case MODE_RESULT://リザルト画面
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

		//FPSの表示
#ifdef _DEBUG
		DrawFPS();//デバック（ビルド）時だけ表示
#endif 


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
//FPSの表示
//============================================
void DrawFPS(void)
{
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[MAX_MOTIONMODE][256];
	int nCount;

    Defeat* pDefeat = GetDefeat();

	Bullet* pBullet = GetBullet();
	//文字列に代入
	wsprintf(&aStr[0][0], "FPS:%d", g_nCountFPS);
	wsprintf(&aStr[1][0], "\nトリガー発射はF1キー");
	wsprintf(&aStr[2][0], "\n\n	全てを破壊モードはF2キー");
	wsprintf(&aStr[3][0], "\n\n\n回転発射はF3キー");
	wsprintf(&aStr[4][0], "\n\n\n\n均等回転発射はF4キー");
	wsprintf(&aStr[5][0], "\n\n\n\n\n逆均等回転発射はF5キー");
	wsprintf(&aStr[6][0], "\n\n\n\n\n\n＜＜＜＜＜＜＜＜＜ホーミングはHキー！＞＞＞＞＞＞＞＞");
	wsprintf(&aStr[7][0], "\n\n\n\n\n\n\n調整型５way弾はF7キー（角度調整「→キー、←キー」)");
	wsprintf(&aStr[8][0], "\n\n\n\n\n\n\n\nスキルの使用は「J」キー");
	wsprintf(&aStr[9][0], "\n\n\n\n\n\n\n\n\nスキルの選択は「I」キー、「K」キー");
	wsprintf(&aStr[10][0], "\n\n\n\n\n\n\n\n\n\n敵撃破数%d",pDefeat->DefeatEnemyCount);

	//テキストの描画
	for (nCount = 0; nCount < MAX_MOTIONMODE; nCount++)
	{
		g_pFont->DrawText(NULL, &aStr[nCount][0], -1, &rect, DT_LEFT,
			D3DCOLOR_RGBA(255, 255, 255, 255));
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
		UninitTutorial();//チュートリアル画面
		UninitStaging();
		UninitSkillGage();
		UninitSkillName();
		UninitFade();
		break;
	case MODE_CHOOSE_DIFFICULTY://難易度選択画面
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
	case MODE_GAME://ゲーム画面
		UninitGame();
		break;
	case MODE_RESULT://リザルト画面
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

//新しい画面（モード）の初期化処理
	switch (mode)
	{
	case MODE_TITLE://タイトル画面
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
		InitTutorial();//チュートリアル画面
		InitStaging();
		InitSkillGage();
		InitSkillName();
		InitFade();
		SetStaging(STAGINGTYPE_PRESSENTER, -1, D3DXVECTOR3(SCREEN_WIDTH - 120.0f, SCREEN_HEIGHT - 130.0f, 0.0f));
		break;
	case MODE_CHOOSE_DIFFICULTY://難易度選択画面
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
	case MODE_GAME://ゲーム画面
		InitGame();
		break;
	case MODE_RESULT://リザルト画面
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
	//新しい画面（モード）の初期化処理

	g_mode = mode;//現在の画面（モード）を切り替える
}

//モードの取得
MODE GetMode(void)
{
	return g_mode;
}



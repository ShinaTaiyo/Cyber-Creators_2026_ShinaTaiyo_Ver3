//================================================================================
//
//５月１７日：C++基盤作成パート１：main.cppのみでポリゴンを出す[main.cpp]
//Author:ShinaTaiyo
//
//================================================================================

#ifndef _MAIN_H_ //このマクロ定義がされていなかったら
#define _MAIN_H_//２重インクルード防止のマクロ定義

//==============================
//インクルード
//==============================
#include <windows.h>
#include "d3dx9.h"//描画処理に必要
#define DIRECTINPUT_VERSION (0x0800)//ビルド時の警告対処用のマクロ
#include "dinput.h"//入力処理に必要
#include "Xinput.h"//ジョイパッドの処理に必要
#include "xaudio2.h"//サウンド処理に必要
#include <iostream> //標準入出力
#include <fstream> //ファイル入出力で使う
#include <sstream> //ファイル入出力で使う
#include <algorithm>
#include <vector> //ベクター管理で使う
#include <list>
#include <iomanip>// setprecisionを使用するのに必要
#include <cassert>
#include <stdio.h>
#include <stdlib.h>
#include <map>    //マップ型を使う
using namespace std;

//==============================
//ライブラリのリンク
//==============================
#pragma comment(lib,"d3d9.lib")//描画処理に必要
#pragma comment(lib,"d3dx9.lib")//[d3d9.lib]の拡張ライブラリ
#pragma comment(lib,"dxguid.lib")//DurectXコンポーネント（部品）使用に必要
#pragma comment(lib,"winmm.lib")//システム時刻取得に必要
#pragma comment(lib, "dinput8.lib")//入力処理に必要
#pragma comment(lib,"Xinput.lib")//ジョイパッド処理に必要

//マクロ定義
#define SCREEN_WIDTH (1280)//ウインドウの幅
#define SCREEN_HEIGHT (720)//ウインドウの高さ
#define BOSSSTAGE_HEIGHT (400.0f)  //ボスステージの高さ
#define ID_BUTTON_FINISH (100)
#define FVF_VERTEX_2D (D3DFVF_XYZRHW |D3DFVF_DIFFUSE |D3DFVF_TEX1)
//3Dで追加するマクロ定義
#define FVF_VERTEX_3D (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)

#define NORMAL_COL (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))                           //普通の色

//==================================================
//メモリリーク検出用
//==================================================
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifdef _DEBUG
#define DBG_NEW new (_NORMAL_BLOCK , __FILE__, __LINE__)
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif

//===================================
//頂点情報[2D]の構造体を定義
//===================================
typedef struct
{
	D3DXVECTOR3 pos;//頂点座標
	float rhw;//座標変換用変数（1.0fで固定)
	D3DCOLOR col;//頂点カラー
	D3DXVECTOR2 tex;//テクスチャ座標
}VERTEX_2D;

//=============================================================================================
			
//軸の列挙型
enum class AXIS
{
	X = 0,
	Y,
	Z,
	MAX
};

//===================================
//頂点情報[3D]の構造体を定義
//===================================
typedef struct
{
	D3DXVECTOR3 pos;//頂点座標
	D3DXVECTOR3 nor;//法線ベクトル
	D3DCOLOR col;//頂点カラー
	D3DXVECTOR2 tex;//テクスチャ座標
}VERTEX_3D;
int GetFPS();
#endif
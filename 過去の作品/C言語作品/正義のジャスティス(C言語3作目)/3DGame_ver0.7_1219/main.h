//========================================================
//
//８月３０日：エフェクトもう一回見直す[main.cpp]
//Author:ShinaTaiyo
//
//=========================================================

#ifndef _MAIN_H_ //このマクロ定義がされていなかったら
#define _MAIN_H_//２重インクルード防止のマクロ定義
#include <windows.h>
#include "d3dx9.h"//描画処理に必要
#define DIRECTINPUT_VERSION (0x0800)//ビルド時の警告対処用のマクロ
#include "dinput.h"//入力処理に必要
#include "Xinput.h"//ジョイパッドの処理に必要
#include "xaudio2.h"//サウンド処理に必要

//ライブラリのリンク
#pragma comment(lib,"d3d9.lib")//描画処理に必要
#pragma comment(lib,"d3dx9.lib")//[d3d9.lib]の拡張ライブラリ
#pragma comment(lib,"dxguid.lib")//DurectXコンポーネント（部品）使用に必要
#pragma comment(lib,"winmm.lib")//システム時刻取得に必要
#pragma comment(lib, "dinput8.lib")//入力処理に必要
#pragma comment(lib,"Xinput.lib")//ジョイパッド処理に必要


//マクロ定義
#define SCREEN_WIDTH (1280)//ウインドウの幅
#define SCREEN_HEIGHT (720)//ウインドウの高さ
#define FVF_VERTEX_2D (D3DFVF_XYZRHW |D3DFVF_DIFFUSE |D3DFVF_TEX1)
#define ID_BUTTON_FINISH (100)
#define MAX_POLYGON (2)

//3Dで追加するマクロ定義
#define FVF_VERTEX_3D (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)

//頂点情報[2D]の構造体を定義
typedef struct
{
	D3DXVECTOR3 pos;//頂点座標
	float rhw;//座標変換用変数（1.0fで固定)
	D3DCOLOR col;//頂点カラー
	D3DXVECTOR2 tex;//テクスチャ座標
}VERTEX_2D
;
typedef struct
{
	D3DXVECTOR3 pos_2;//頂点座標
	float rhw_2;//座標変換用変数（1.0fで固定)
	D3DCOLOR col_2;//頂点カラー
	D3DXVECTOR2 tex_2;//テクスチャ座標
}VERTEX_2D_2//二つ目のポリゴンの頂点の構造体
;

typedef struct
{
	D3DXVECTOR3 pos;//頂点座標
	D3DXVECTOR3 nor;//法線ベクトル
	D3DCOLOR col;//頂点カラー
	D3DXVECTOR2 tex;//テクスチャ座標
}VERTEX_3D;

typedef enum
{
	MODE_TITLE=0,//タイトル画面
	MODE_TUTORIAL,//チュートリアル画面を呼び出す
	MODE_CHOOSE_DIFFICULTY,//難易度選択モードを呼び出す。
	MODE_GAME,//ゲーム画面
	MODE_RESULT,//リザルト画面
	MODE_RANKING,//ランキング画面
	MODE_MAX
}MODE;

//プロトタイプ宣言
LPDIRECT3DDEVICE9 GetDevice(void);
void SetMode(MODE mode);
MODE GetMode(void);

#endif
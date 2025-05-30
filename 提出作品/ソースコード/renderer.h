//====================================================
//
//C++で基盤づくり
//Author:ShinaTaiyo
//
//====================================================

#ifndef _RENDERER_H_  
#define _RENDERER_H_

//======================
//インクルード
//======================
#include "d3dx9.h"//描画処理に必要
#include "main.h"
//==========================================

//==========================================
//レンダラークラス
//==========================================
class CRenderer
{
public:
	//================================
    //モードの列挙型
    //================================
	typedef enum
	{
		MODE_TITLE = 0,        //タイトル画面
		MODE_TUTORIAL,         //チュートリアル画面を呼び出す
		MODE_CHOOSE_DIFFICULTY,//難易度選択モードを呼び出す。
		MODE_GAME,             //ゲーム画面
		MODE_RESULT,           //リザルト画面
		MODE_RANKING,          //ランキング画面
		MODE_MAX
	}MODE;
	//====================================================================================

	CRenderer();                             //コンストラクタ
	~CRenderer();                            //デストラクタ
	HRESULT Init(HWND hWnd, BOOL bWindow);   //初期化処理
	void Uninit();                           //終了処理
	void Update();                           //更新処理
	void Draw();                             //描画処理
	void SetMode(MODE mode);                 //モードの設定
	LPDIRECT3DDEVICE9 GetDevice();           //デバイスの取得
	static const int m_nMAXDEBUG;            //デバッグ表示の数
private:
	LPD3DXFONT m_pFont;                      //フォントへのポインタ!
	LPDIRECT3D9 m_pD3D;                      //Direct3D!
	LPDIRECT3DDEVICE9 m_pD3DDevice;          //Direct3Dデバイス!
	MODE m_mode;                             //現在のモード!
	void DrawFPS();                          //デバッグ表示処理

};

#endif

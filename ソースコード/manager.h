//====================================================
//
//５月２２日：C++で基盤づくりLV4[manager.h]
//Author:ShinaTaiyo
//
//====================================================

#ifndef _MANAGER_H_  
#define _MANAGER_H_

//======================
//インクルード
//======================
#include "main.h"
#include "scene.h"
//==========================================

//======================
//前方宣言
//======================
class CRenderer;        //レンダラークラス前方宣言
class CInputKeyboard;   //キーボードクラスを前方宣言
class CInputJoypad;     //ジョイパッドクラスを前方宣言
class CSound;           //サウンドクラスを前方宣言
class CCamera;          //カメラクラスを前方宣言
class CLight;           //ライトクラスを前方宣言
class CTexture;         //テクスチャクラスを前方宣言
class CObjectXInfo;     //オブジェクトXクラスを前方宣言
class CSceneFade;       //フェードクラスを前方宣言
class CInputMouse;      //マウス
class CDebugText;       //デバッグ文字クラス
//==========================================

//==========================================
//マネージャクラス
//==========================================
class CManager
{
public:
	CManager();                                                         //コンストラクタ
	~CManager();                                                        //デストラクタ
	HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);         //初期化処理
	void Uninit();                                                      //終了処理
	void Update();                                                      //更新処理
	void Draw();                                                        //描画処理
	static CRenderer* GetRenderer();                                    //レンダラークラスを取得
	static CInputKeyboard* GetInputKeyboard();                          //キーボードクラスを取得
	static CInputJoypad* GetInputJoypad();                              //ジョイパッドクラスを取得
	static CSound* GetSound() { return m_pSound; }                      //サウンドクラスを取得
	static CCamera* GetCamera() { return m_pCamera; }                   //カメラクラスを取得
	static CLight* GetLight() { return m_pLight; }                      //ライトクラスを取得
	static CTexture* GetTexture() { return m_pTexture; }                //テクスチャクラスを取得
	static CObjectXInfo* GetObjectXInfo() { return m_pObjectXInfo; }    //オブジェクトXクラスを取得する
	static CSceneFade* GetSceneFade() { return m_pSceneFade; }                         //フェードクラスを取得する
	static CScene* GetScene() { return m_pScene; }                      //シーンクラスを取得する
	static CInputMouse* GetInputMouse() { return m_pInputMouse; }       //マウス入力情報を取得する
	static CDebugText* GetDebugText() { return m_pDebugText; }          //デバッグテキスト情報
	static void SetMode(CScene::MODE mode);                             //モードを設定する
private:
	static CRenderer* m_pRenderer;                                      //レンダラー
	static CInputKeyboard* m_pInputKeyboard;                            //キーボード入力
	static CInputJoypad* m_pInputJoypad;                                //ジョイパッド
	static CSound* m_pSound;                                            //サウンド            
	static CCamera* m_pCamera;                                          //カメラ情報
	static CLight* m_pLight;                                            //ライト情報
	static CTexture* m_pTexture;                                        //テクスチャ情報
	static CObjectXInfo* m_pObjectXInfo;                                //オブジェクトX情報
	static CScene* m_pScene;                                            //シーン情報
	static CSceneFade* m_pSceneFade;                                    //フェード情報	
	static CInputMouse* m_pInputMouse;                                  //マウス情報
	static CDebugText* m_pDebugText;                                    //デバッグテキスト情報
}; 
//=======================================================================================


#endif

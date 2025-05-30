//===============================================================================
//
//６月１７日：画面遷移を実装する
//Author:ShinaTaiyo
//
//===============================================================================

#ifndef _SCENE_H_  
#define _SCENE_H_


//========================================
//インクルード
//========================================
#include "main.h"
//===============================================================================

//========================================
//シーンクラス
//========================================
class CScene
{
public:
	//画面の種類
	typedef enum
	{
		MODE_TITLE = 0,                       //タイトル画面
		MODE_GAME,                            //ゲーム画面
		MODE_RESULT,                          //リザルト画面
		MODE_EDIT,                            //エディット画面
		MODE_MAX					          
	}MODE;							          
									          
	CScene();                                 //コンストラクタ
	virtual ~CScene();                        //デストラクタ
	virtual HRESULT Init();                   //初期化処理
	virtual void Uninit();                    //終了処理
	virtual void Update();                    //更新処理
	virtual void Draw();                      //描画処理
	static CScene* Create(MODE mode);         //生成処理
	static MODE GetMode() { return m_Mode; }  //現在のモードを取得する
private:							   
	static MODE m_Mode;                       //現在のモード
};
#endif
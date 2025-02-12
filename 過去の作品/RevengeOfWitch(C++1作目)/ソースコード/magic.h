//====================================================
//
//６月３０日：魔法を実装[magic.h]
//Author:ShinaTaiyo
//
//====================================================

#ifndef _MAGIC_H_  
#define _MAGIC_H_

//======================
//インクルード
//======================
#include "main.h"
#include "ui.h"
#include "object2d.h"
#include "objectX.h"
//==========================================

//==========================================

//==========================================
//魔法表示クラス
//==========================================
class CMagicDisp : public CObject2D
{
public:
	typedef enum
	{
		MAGICDISP00_FIRE = 0,   //魔法表示０：ファイア
		MAGICDISP01_BEAM,       //魔法表示１：ビーム
		MAGICDISP02_ICE,        //魔法表示２：アイス
		MAGICDISP03_THUNDER,    //魔法表示３：サンダー
		MAGICDISP04_SWORD,      //魔法表示４：剣
		MAGICDISP05_PLANTSHIELD,//魔法表示５：プラントシールド
		MAGICDISP06_QUICKHEAL,  //魔法表示６：クイックヒール
		MAGICDISP07_CROSSBOMB,  //魔法表示７：クロスボム
		MAGICDISP08_BLACKHOLE,  //魔法表示８：ブラックホール
		MAGICDISP09_PHYCOBURST, //魔法表示９：サイコバースト
		MAGICDISP_MAX
	}MAGICDISPTYPE;
	CMagicDisp();                                                            //コンストラクタ
	~CMagicDisp();                                                           //デストラクタ
	HRESULT Init() override;                                                 //初期化処理
	void Uninit() override;                                                  //終了処理
	void ExtraUninit() override;                                             //別枠の終了処理
	void Update() override;                                                  //更新処理
	void Draw() override;                                                    //描画処理
	void SetDeath() override;                                                //死亡フラグを設定
	static CMagicDisp* Create(MAGICDISPTYPE Type,float fWidth, float fHeight, D3DXVECTOR3 pos); //ゲージを生成
	void SetMagicDispType(MAGICDISPTYPE Type);                               //魔法表示の種類を変える
	MAGICDISPTYPE GetMagicDispType() { return m_Type; }                      //現在の魔法表示を取得する

private:
	MAGICDISPTYPE m_Type;                                                    //魔法表示の種類
	static const char* m_MAGICDISP_FILENAME[MAGICDISP_MAX];                  //魔法表示のファイルネーム
	CUi* m_pLongPushUi;                                                      //長押しの文字UIの表示
	
	//==============================
	//魔法リスト
	//==============================
	//static CMagicDisp* m_pMagicDisp_Top;                  //ステージマネージャーで管理している先頭オブジェクトへのポインタ
	//static CMagicDisp* m_pMagicDisp_Cur;                  //ステージマネージャーで管理してい最後尾オブジェクトへのポインタ
	//CMagicDisp* m_pMagicDisp_Next;                        //ステージマネージャーで管理している次のオブジェクトへのポインタ
	//CMagicDisp* m_pMagicDisp_Prev;                        //ステージマネージャーで管理している前のオブジェクトへのポインタ
	//bool m_bChooseState;                                    //選ばれている状態にする
	//================================================================================================
};
//============================================================================================================

//==========================================
//魔法ウインドウクラス
//==========================================
class CMagicWindow : public CObject2D
{
public:
	CMagicWindow();             //コンストラクタ
	~CMagicWindow() override;   //デストラクタ
	HRESULT Init() override;    //初期化処理
	void Uninit() override;     //終了処理
	void ExtraUninit() override;//別枠の終了処理
	void Update() override;     //更新処理
	void Draw() override;       //描画処理
	void SetDeath() override;   //死亡フラグを設定
	static CMagicWindow* Create(float fWidth, float fHeight, D3DXVECTOR3 pos);//魔法ウインドウを生成
	int m_nHaveMagic;             //現在持っている魔法
private:
};
//==========================================================================================================================

#endif

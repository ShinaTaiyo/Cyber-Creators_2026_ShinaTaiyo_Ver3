//====================================================
//
//５月２２日：C++で基盤づくり[bg.h]
//Author:ShinaTaiyo
//
//====================================================

#ifndef _BG_H_  
#define _BG_H_

//======================
//インクルード
//======================
#include "main.h"
#include "object2d.h"
#include "ui.h"
//==========================================

//==========================================
//背景クラス
//==========================================
class CBg :  public CObject2D
{
public:
	typedef enum
	{
		BG_TITLE = 0,//タイトル背景
		BG_DIFFICULTY,//難易度選択背景
		BG_RESULT,   //リザルト背景
		BG_MAX 
	}BG;
	CBg();                      //コンストラクタ
	virtual ~CBg();             //デストラクタ
	HRESULT Init() override;    //初期化処理
	void Uninit() override;     //終了処理
	void Update() override;     //更新処理
	void Draw() override;       //描画処理
	void SetDeath() override;   //死亡フラグを設定
	static void Create(BG type);//背景の生成
	void SetBgType(BG type) { m_Type = type; }//背景の種類を設定

	//=================================
	//テクスチャ移動関係
	//=================================
	void SetUseTexMove(bool bUse, D3DXVECTOR2 Pos, D3DXVECTOR2 Move) { m_bUseTexMove = bUse; m_PosTex = Pos; m_MoveTex = Move; }
	void SetPosTex(D3DXVECTOR2 Pos) { m_PosTex = Pos; }
	void SetMoveTex(D3DXVECTOR2 Move) { m_MoveTex = Move; }
	//==================================================================================

protected:
	BG GetType() { return m_Type; }//背景の種類を取得

	//============================
	//回転系
	//============================

	//回転力
	void SetRotationPower(float fPower) { m_fRotationPower = fPower; }
	float& GetRotationPower() { return m_fRotationPower; }

	//==================================================================================

	static const char* m_apBGFILE_NAME[BG_MAX];//背景のテクスチャファイル名
private:
	float m_fPosTexU;          //テクスチャ座標
	BG m_Type;                 //背景の種類
	float m_fRotationPower;    //回転力

	//=================================
	//テクスチャ移動関係
	//=================================
	bool m_bUseTexMove;        //背景を移動させるかどうか
	D3DXVECTOR2 m_PosTex;      //テクスチャ座標
	D3DXVECTOR2 m_MoveTex;     //テクスチャの移動量
	//==================================================================================
};
//=======================================================================================

//=====================================
//タイトル背景クラス
//=====================================
class CBgTitle : public CBg
{
public:
	CBgTitle();                      //コンストラクタ
	~CBgTitle() override;             //デストラクタ
	HRESULT Init() override;    //初期化処理
	void Uninit() override;     //終了処理
	void Update() override;     //更新処理
	void Draw() override;       //描画処理
	void SetDeath() override;   //死亡フラグを設定
	static CBgTitle * Create(BG type);//背景の生成
private:
	typedef enum
	{
		TITLEUI00_TITLELOGO = 0,
		TITLEUI01_PLAYERILLUST,
		TITLEUI02_SLIMEILLUST,
		TITLEUI03_MAGICCIRCLE,
		TITLEUI_MAX
	}TITLEUI;
	bool m_bRotationSwitch;//回転させる方向を決めるフラグ
	int m_nRotationNum;    //回転した回数
	CUi* m_pUi[TITLEUI_MAX];    //タイトルロゴ

	float m_TitleUiRotSpeed;
	D3DXVECTOR2 m_TitleUiScale;
};
//=======================================================================================

//=====================================
//難易度選択背景クラス
//=====================================
class CBgDifficulty : public CBg
{
public:
	CBgDifficulty();                      //コンストラクタ
	~CBgDifficulty() override;             //デストラクタ
	HRESULT Init() override;    //初期化処理
	void Uninit() override;     //終了処理
	void Update() override;     //更新処理
	void Draw() override;       //描画処理
	void SetDeath() override;   //死亡フラグを設定
	static CBgDifficulty* Create(BG type);//背景の生成
private:

};
//=======================================================================================


#endif

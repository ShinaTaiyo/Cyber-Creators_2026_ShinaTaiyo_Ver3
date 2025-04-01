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
//==========================================

//==========================================
//背景クラス
//==========================================
class CBg :  public CObject2D
{
public:
	typedef enum
	{//背景の種類
		BG_TITLE = 0, //タイトル背景
		BG_DIFFICULTY,//難易度選択背景
		BG_RESULT,    //リザルト背景
		BG_MAX 
	}BG;
	CBg(int nPri = 0, bool bUseintPri = false, CObject::TYPE type = CObject::TYPE::BG, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_2D);//コンストラクタ
	virtual ~CBg();                           //デストラクタ
	HRESULT Init() override;                  //初期化処理
	void Uninit() override;                   //終了処理
	void Update() override;                   //更新処理
	void Draw() override;                     //描画処理
	void SetDeath() override;                 //死亡フラグを設定
	static void Create(BG type);              //背景の生成
	void SetBgType(BG type) { m_Type = type; }//背景の種類を設定

	//=================================
	//テクスチャ移動関係
	//=================================
	void SetUseTexMove(bool bUse, D3DXVECTOR2 Pos, D3DXVECTOR2 Move) { m_bUseTexMove = bUse; m_PosTex = Pos; m_MoveTex = Move; }//テクスチャを移動させるかどうか?移動させるなら初期位置と移動量を設定する
	void SetPosTex(D3DXVECTOR2 Pos) { m_PosTex = Pos; }//テクスチャ座標を設定する
	void SetMoveTex(D3DXVECTOR2 Move) { m_MoveTex = Move; }//テクスチャの移動量を設定する
	//==================================================================================

protected:
	BG GetType() { return m_Type; }//背景の種類を取得

	//============================
	//回転系
	//============================

	//回転力
	void SetRotationPower(float fPower) { m_fRotationPower = fPower; }//回転速度を設定する
	float& GetRotationPower() { return m_fRotationPower; }//回転速度を取得する

	//==================================================================================

	static const std::string m_apBGFILE_NAME[BG_MAX];//背景のテクスチャファイル名
private:
	float m_fPosTexU;          //テクスチャ座標!
	BG m_Type;                 //背景の種類!
	float m_fRotationPower;    //回転力!

	//=================================
	//テクスチャ移動関係
	//=================================
	bool m_bUseTexMove;        //背景を移動させるかどうか!
	D3DXVECTOR2 m_PosTex;      //テクスチャ座標!
	D3DXVECTOR2 m_MoveTex;     //テクスチャの移動量!
	//==================================================================================
};
//=======================================================================================

//=====================================
//タイトル背景クラス
//=====================================
class CBgTitle : public CBg
{
public:
	CBgTitle();                       //コンストラクタ
	~CBgTitle() override;             //デストラクタ
	HRESULT Init() override;          //初期化処理
	void Uninit() override;           //終了処理
	void Update() override;           //更新処理
	void Draw() override;             //描画処理
	void SetDeath() override;         //死亡フラグを設定
	static CBgTitle * Create(BG type);//背景の生成
private:
};
//=======================================================================================

//=====================================
//難易度選択背景クラス
//=====================================
class CBgDifficulty : public CBg
{
public:
	CBgDifficulty();                      //コンストラクタ
	~CBgDifficulty() override;            //デストラクタ
	HRESULT Init() override;              //初期化処理
	void Uninit() override;               //終了処理
	void Update() override;               //更新処理
	void Draw() override;                 //描画処理
	void SetDeath() override;             //死亡フラグを設定
	static CBgDifficulty* Create(BG type);//背景の生成
private:

};
//=======================================================================================


#endif

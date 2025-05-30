//===================================================================================================================
//
// ２０２５年５月２５日：コードを綺麗にする[bg.h]
// Author:ShinaTaiyo
//
//===================================================================================================================

//*******************************************************************************************************************
// ２重インクルード防止
//*******************************************************************************************************************
#ifndef _BG_H_  
#define _BG_H_

//*******************************************************************************************************************
// インクルード
//*******************************************************************************************************************
#include "main.h"     // メイン
#include "object2d.h" // オブジェクト2D

//*******************************************************************************************************************
// 背景クラス
//*******************************************************************************************************************
class CBg :  public CObject2D
{
public:
	// === 列挙型 ===

	// 背景種類列挙型
	enum class TYPE
	{
		TITLE = 0,  // タイトル背景
		DIFFICULTY, // 難易度選択背景
		RESULT,     // リザルト背景
		MAX // 最大
	}
	;
	// === 特殊関数 ===

	//コンストラクタ
	CBg(
		int nPri = 0, // 自分で決めるint型プライオリティ
		bool bUseintPri = false, // プライオリティを自分で決めるかどうか
		CObject::TYPE type = CObject::TYPE::BG, // タイプ
		CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_2D // オブジェクトタイプ
	);
	~CBg() override; // デストラクタ

	// === ライフサイクルメンバ関数 ===

	HRESULT Init() override;  // 初期化処理
	void Uninit() override;   // 終了処理
	void Update() override;   // 更新処理
	void Draw() override;     // 描画処理
	void SetDeath() override; // 死亡フラグを設定

	// === メンバ関数 ===

	// 背景の種類
	const TYPE& GetType();   // 取得
	void SetType(TYPE type); // 設定

	// 回転速度
	void SetRotationPower(float fPower); // 設定
	float& GetRotationPower(); // 取得

	// テクスチャを移動させるかどうか
	void SetUseTexMove(
		bool bUse, // 使用するかどうか
		D3DXVECTOR2 Pos, // 位置
		D3DXVECTOR2 Move // 移動量
	);

	void SetPosTex(D3DXVECTOR2 Pos); // テクスチャ座標を設定する
	void SetMoveTex(D3DXVECTOR2 Move); // テクスチャの移動量を設定する

	// === 静的メンバ関数 ===

	static void Create(TYPE type); // 背景の生成
private:
	// === 静的メンバ変数 ===

	static const std::string m_apBGFILE_NAME[static_cast<int>(TYPE::MAX)]; // 背景のテクスチャファイル名

	// === メンバ変数 ===

	bool m_bUseTexMove; // 背景を移動させるかどうか
	float m_fPosTexU,   // テクスチャ座標
	      m_fRotationPower;  // 回転力
	D3DXVECTOR2 m_PosTex,    // テクスチャ座標
	            m_MoveTex;   // テクスチャ移動量
	TYPE m_Type; // 背景の種類
};

#endif

//===================================================================================================================
//
// ２０２５年６月１６日：計算用クラスを名前空間にする[bg.h]
// Author:ShinaTaiyo
//
//===================================================================================================================

//*******************************************************************************************************************
// ２重インクルード防止
//*******************************************************************************************************************
#ifndef _WIRE_H_
#define _WIRE_H_

//*******************************************************************************************************************
// インクルード
//*******************************************************************************************************************
#include "main.h"
#include "meshcylinder.h"

//*******************************************************************************************************************
// 前方宣言
//*******************************************************************************************************************
class CPlayer;   // プレイヤー
class CWireHead; // ワイヤーヘッド

//*******************************************************************************************************************
// ワイヤークラス
//*******************************************************************************************************************
class CWire : public CMeshCylinder
{
public:
	// === 列挙型 ===

	// ワイヤー種類
	enum class WIRETYPE
	{
		NORMAL = 0,
		ROPE,
		MAX
	};

	// === 特殊関数 ===

	// コンストラクタ
	CWire(
		WIRETYPE WireType, // ワイヤー種類
		float fRadius,     // 半径 
		float fHeight,     // 高さ
		int nNumDivsionXZ, // XZ分割数
		D3DXVECTOR3 Pos,   // 位置
		D3DXVECTOR3 Rot,   // 向き
		int nNumDivisionY, // 縦分割数
		int nPri = 0,      // 自分で決めるプライオリティ
		bool bUseintPri = false, // プライオリティを自分で決めるかどうか
		CObject::TYPE type = CObject::TYPE::WIRE, // タイプ
		CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_3D // オブジェクトタイプ
	);

	~CWire() override; // デストラクタ

	// === ライフサイクルメンバ関数 ===

	HRESULT Init() override;  // 初期化処理
	void Uninit() override;   // 終了処理
	void Update() override;   // 更新処理
	void Draw() override;     // 描画処理
	void SetDeath() override; // 死亡フラグ設定処理

	// === メンバ関数 ===

	// ワイヤーの頭取得
	CWireHead* GetWireHead();

	// プレイヤーのポインタ設定
	void SetPlayerPointer(CPlayer* pPlayer);

	// 更新するかどうか
	void SetUseUpdate(bool bUse);

	// === 静的メンバ関数 ===

	// 生成処理
	static CWire* Create(
		WIRETYPE Type,   // ワイヤー種類
		D3DXVECTOR3 Pos, // 位置 
		D3DXVECTOR3 Rot, // 向き
		float fRadius,   // 半径 
		float fHeight,   // 高さ
		int nNumDivisionXZ, // XZ分割数 
		int nNumDivisionY   // Y分割数
	);

protected:
	static const string s_WIRE_FILENAME[static_cast<int>(WIRETYPE::MAX)]; // ワイヤーテクスチャファイル名
private:
	// === メンバ変数 ===

	WIRETYPE m_Type;    // ワイヤーの種類
	CPlayer* m_pPlayer; // プレイヤーへのポインタ
	bool m_bUseUpdate;  // 更新するかどうか
	CWireHead* m_pWireHead; // ワイヤーの頭

	// === メンバ関数 ===

	void GenerateCylindricalMeshOnWire(); // ワイヤー状にメッシュを展開
};
#endif // !_WIRE_H_
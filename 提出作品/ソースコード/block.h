//===================================================================================================================
//
// ２０２５年６月３日：コードを綺麗にする[block.h]
// Author:ShinaTaiyo
//
//===================================================================================================================

//*******************************************************************************************************************
// ２重インクルード防止
//*******************************************************************************************************************
#ifndef _BLOCK_H_  
#define _BLOCK_H_

//*******************************************************************************************************************
// インクルード
//*******************************************************************************************************************
#include "main.h"
#include "objectX.h"

//*******************************************************************************************************************
// ブロッククラス
//*******************************************************************************************************************
class CBlock : public CObjectX
{
public:
	// === 列挙型 ===

	// 種類
	enum class TYPE
	{
		NORMAL = 0, // 普通ブロック
		WATER,      // 水ブロック
		RENGA,      // レンガブロック
		BLACK,      // 黒ブロック
		MAX         // 最大
	};

	// === 特殊関数 ===

	// コンストラクタ
	CBlock(
		int nPri = 0, // 自分で決めるプライオリティ
		bool bUseintPri = false, // 自分でプライオリティを決めるかどうか
		CObject::TYPE type = CObject::TYPE::BLOCK, // タイプ
		CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_X // オブジェクトタイプ
	);
	~CBlock() override;       // デストラクタ

	// === ライフサイクルメンバ関数 ===

	HRESULT Init() override;  // 初期化処理
	void Uninit() override;   // 終了処理
	void Update() override;   // 更新処理
	void Draw() override;     // 描画処理
	void SetDeath() override; // 死亡フラグを設定

	// === メンバ関数 ===

	void SetType(TYPE Type); // ブロックのタイプを設定する
	const TYPE & GetType();  // ブロックの種類を取得する
	const bool& GetCollision(); // 判定可能かどうかを取得する     
	void SaveInfoTxt(fstream& WritingFile) override; // テキストファイルに情報を保存するための関数

	// テキストファイルから情報を読み込むための関数
	void LoadInfoTxt(
		fstream& LoadingFile, // ファイル読み込み用
		list<CObject*>& listSaveManager, // ステージマネージャーオブジェクトリスト
		string& Buff, // 文字列
		CObject* pObj // 編集オブジェクトへのポインタ
	) override;

	CObject* ManagerChengeObject(bool bAim) override; // ステージマネージャーに変更したオブジェクトを渡す
	CObject* ManagerSaveObject() override; //ステージマネージャーに今のオブジェクトを保存する

	// === 静的メンバ関数 ===

	// 生成
	static CBlock * Create(
		TYPE type, int nLife,D3DXVECTOR3 pos,D3DXVECTOR3 rot,D3DXVECTOR3 scale,
		bool bSwapVtxXZ // 頂点XZを入れ替えるかどうか
	);
private:
	// === 静的メンバ変数 ===

	static const string s_BlockModelFileName[static_cast<int>(TYPE::MAX)]; // ブロックのモデルファイル名
	static const float m_fBLOCKCORRECTIONCOLLISION; // 判定に余裕を持たせる量

	// === メンバ変数 ===

	TYPE m_type;       // ブロックの種類
};

#endif

//===================================================================================================================
//
// ２０２５年５月２５日：コードを綺麗にする[bgModel.h]
// Author:ShinaTaiyo
//
//===================================================================================================================

//*******************************************************************************************************************
// ２重インクルード防止
//*******************************************************************************************************************
#ifndef _BGMODEL_H_  
#define _BGMODEL_H_

//*******************************************************************************************************************
// インクルード
//*******************************************************************************************************************
#include "main.h"    // メイン
#include "objectX.h" // オブジェクトX

//*******************************************************************************************************************
// 背景モデルクラス
//*******************************************************************************************************************
class CBgModel : public CObjectX
{
public:
	// === 列挙型 ===

	// 背景モデルタイプ
	enum class TYPE
	{
		BILL_00 = 0, //ビル０
		TREE_00,  //木	
		BILL_01,  //ビル１
		BILL_02,  //ビル２
		BILL_03,  //ビル３
		BILL_04,  //ビル４
		CHAIN_00, //チェーン
		HOUSE_00, //家０
		GRASS_00, //草
		GRASSGROUND_00, //草地面
		LOG_00, //丸太
		MAX // 最大
	};

	// === 特殊関数 ===

	// コンストラクタ
	CBgModel(
		int nPri = 0,// 自分で決めるint型プライオリティ
		bool bUseintPri = false, // 自分でプライオリティを決めるかどうか
		CObject::TYPE type = CObject::TYPE::BGMODEL, // タイプ
		CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_X // オブジェクトタイプ
	);
	~CBgModel(); // デストラクタ

	// === ライフサイクル関数 ===

	HRESULT Init() override;  // 初期化処理
	void Uninit() override;   // 終了処理
	void Update() override;   // 更新処理
	void Draw() override;     // 描画処理
	void SetDeath() override; // 死亡フラグを設定

	// === メンバ関数 ===

	void SaveInfoTxt(fstream& WritingFile) override; // テキストファイルに情報を保存するための関数

	 // テキストファイルから情報を読み込むための関数
	void LoadInfoTxt(
		fstream& LoadingFile, // 読み込みファイルへのポインタ
		list<CObject*>& listSaveManager, // 読み込みオブジェクト保存用リストへのポインタ
		string& Buff, // 文字列
		CObject* pObj // オブジェクトへのポインタ
	) override;

	CObject* ManagerChengeObject(bool bAim) override; // ステージマネージャーに変更したオブジェクトを渡す
	CObject* ManagerSaveObject() override; // ステージマネージャーに今のオブジェクトを保存する

	// 背景モデルの種類
	const TYPE& GetType() const; // 取得
	void SetType(TYPE Type);     //設定

	// === 静的メンバ関数 ===

	// 生成処理
	static CBgModel* Create(
		TYPE bgModelType,  // 背景モデルタイプ
		D3DXVECTOR3 pos,   // 位置
		D3DXVECTOR3 rot,   // 向き
		D3DXVECTOR3 Scale, // 拡大率
		bool bSwapVtxXZ    // 頂点XZを入れ替えるかどうか
	);
private:
	// === 静的メンバ変数 ===

	static const string BGMODEL_FILENAME[static_cast<int>(TYPE::MAX)]; // モデルファイル名 

	// === メンバ変数 ===

	TYPE m_Type; // 種類
};
//==================================================================================================================================================
#endif
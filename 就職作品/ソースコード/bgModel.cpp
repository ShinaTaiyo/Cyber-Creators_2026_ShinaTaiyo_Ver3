//===================================================================================================================
//
// ２０２５年５月２５日：コードを綺麗にする[bgModel.cpp]
// Author:ShinaTaiyo
//
//===================================================================================================================

//*******************************************************************************************************************
// インクルード
//*******************************************************************************************************************
#include "bgModel.h"     // 背景モデル
#include "manager.h"     // マネージャー 
#include "objectX.h"     // オブジェクトX
#include "objectXInfo.h" // オブジェクトX情報

//*******************************************************************************************************************
// 静的メンバ宣言
//*******************************************************************************************************************

// モデルファイル名
const string CBgModel::BGMODEL_FILENAME[static_cast<int>(CBgModel::TYPE::MAX)] =
{
	"data\\MODEL\\BgModel\\Bill_000.x",
	"data\\MODEL\\BgModel\\Tree_000.x",
	"data\\MODEL\\BgModel\\Bill_001.x",
	"data\\MODEL\\BgModel\\Bill_002.x",
	"data\\MODEL\\BgModel\\Bill_003.x",
	"data\\MODEL\\BgModel\\Bill_004.x",
	"data\\MODEL\\BgModel\\Chain_000.x",
	"data\\MODEL\\BgModel\\House_000.x",
	"data\\MODEL\\BgModel\\Grass_000.x",
	"data\\MODEL\\BgModel\\GrassGround_000.x",
	"data\\MODEL\\BgModel\\Log_000.x",
};

//===================================================================================================================
// 
// publicメンバ 
//
//===================================================================================================================

//===================================================================================================================
// コンストラクタ
//===================================================================================================================
CBgModel::CBgModel(int nPri, bool bUseintPri, CObject::TYPE type, CObject::OBJECTTYPE ObjType) :
CObjectX(nPri,bUseintPri,type,ObjType),
m_Type(TYPE::BILL_00)
{
	SetManagerObjectType(CObject::MANAGEROBJECTTYPE::BGMODEL); // ステージマネージャーで呼び出す時の種類を設定
}

//===================================================================================================================
// デストラクタ
//===================================================================================================================
CBgModel::~CBgModel()
{
	// 何もなし
}

//===================================================================================================================
// 初期化処理
//===================================================================================================================
HRESULT CBgModel::Init()
{
	CObjectX::Init(); //オブジェクトX初期化処理
	return S_OK;
}

//===================================================================================================================
// 終了処理
//===================================================================================================================
void CBgModel::Uninit()
{
	CObjectX::Uninit(); // オブジェクトX終了処理
}

//===================================================================================================================
// 更新処理
//===================================================================================================================
void CBgModel::Update()
{
	CObjectX::Update(); // オブジェクトX更新処理
}

//===================================================================================================================
// 描画処理
//===================================================================================================================
void CBgModel::Draw()
{
	CObjectX::Draw(); // オブジェクトX描画処理
}

//===================================================================================================================
// 死亡フラグ設定処理
//===================================================================================================================
void CBgModel::SetDeath()
{
	CObjectX::SetDeath(); //オブジェクトX死亡フラグ設定処理
}

//===================================================================================================================
// 生成処理
//===================================================================================================================
CBgModel* CBgModel::Create(TYPE bgModelType, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 Scale, bool bSwapVtxXZ)
{
	// === 変数 ===

	CBgModel* pBgModel = DBG_NEW CBgModel(); // 生成
	CObjectX::PosInfo& SelfPosInfo = pBgModel->GetPosInfo(); // 位置情報
	CObjectX::RotInfo& SelfRotInfo = pBgModel->GetRotInfo(); // 向き情報
	CObjectX::SizeInfo& SelfSizeInfo = pBgModel->GetSizeInfo(); // サイズ情報
	CObjectX::DrawInfo& SelfDrawInfo = pBgModel->GetDrawInfo(); // 描画情報
	CObjectX::LifeInfo& SelfLifeInfo = pBgModel->GetLifeInfo(); // 体力情報

	// === 処理 ===

	pBgModel->Init(); // 初期化処理
	pBgModel->SetType(bgModelType); // 背景モデルの種類を設定する
	SelfPosInfo.SetPos(pos);      // 位置を設定  
	SelfSizeInfo.SetScale(Scale); // 拡大率を設定
	SelfRotInfo.SetRot(rot);      // 向きを設定
	SelfPosInfo.SetSupportPos(pos); // 支点となる位置を設定
	SelfSizeInfo.SetUseSwapVtxXZ(bSwapVtxXZ); // XZの頂点のサイズを入れ替えるかどうか
	SelfDrawInfo.SetUseShadow(false);   // 影を使用しない
	SelfLifeInfo.SetAutoSubLife(false); // 体力を自動的に減らさない
	SelfLifeInfo.SetMaxLife(10); // 最大体力
	SelfLifeInfo.SetLife(10);    // 体力

	// XZのサイズを入れ替えるなら
	if (bSwapVtxXZ == true)
	{
		// === 変数 ===

		D3DXVECTOR3 VtxMax = SelfSizeInfo.GetOriginVtxMax(); // 元の最大頂点を取得
		D3DXVECTOR3 VtxMin = SelfSizeInfo.GetOriginVtxMin(); // 元の最小頂点を取得

		// === 処理 ===

		VtxMax.x = SelfSizeInfo.GetOriginVtxMax().z; // 最大頂点のXをZに
		VtxMax.z = SelfSizeInfo.GetOriginVtxMax().x; // 最大頂点のZをXに
		VtxMin.x = SelfSizeInfo.GetOriginVtxMin().z; // 最小頂点のXをZに
		VtxMin.z = SelfSizeInfo.GetOriginVtxMin().x; // 最小頂点のZをXに
		SelfSizeInfo.SetOriginVtxMax(VtxMax); // 元の最大頂点を現在設定した情報に更新
		SelfSizeInfo.SetOriginVtxMin(VtxMin); // 元の最小頂点を現在設定した情報に更新
	}
	return pBgModel;
}

//===================================================================================================================
// ステージマネージャーに情報を保存する処理
//===================================================================================================================
void CBgModel::SaveInfoTxt(fstream& WritingFile)
{
	WritingFile << "SETBGMODEL" << endl;
	WritingFile << "TYPE = " << static_cast<int>(m_Type);

	// タイプに応じて名前を記述
	switch (m_Type)
	{
	case TYPE::BILL_00:
		WritingFile << " # BILL00" << endl;
		break;
	case TYPE::TREE_00:
		WritingFile << " # TREE00" << endl;
		break;
	default:
		break;
	}

	CObjectX::SaveInfoTxt(WritingFile); // オブジェクトXの情報保存処理

	WritingFile << "END_SETBGMODEL" << endl;

}

//===================================================================================================================
// ステージマネージャーから情報を読み込む処理
//===================================================================================================================
void CBgModel::LoadInfoTxt(fstream& LoadingFile, list<CObject*>& listSaveManager, string& Buff, CObject* pObj)
{
	// === 変数 ===

	int nType = 0; // 種類
	CBgModel::TYPE Type = {}; // 背景モデルの種類

	// === 処理 ===

	// END_SETBGMODELまで繰り返す
	while (Buff != "END_SETBGMODEL")
	{
		// === 処理 ===

		LoadingFile >> Buff; // 単語を読み込む

		// #があったら行を読み込みスキップ
		if (Buff == "#")
		{
			getline(LoadingFile, Buff);
		}
		// タイプを設定
		else if (Buff == "TYPE")
		{
			LoadingFile >> Buff;  // イコール
			LoadingFile >> nType; // 種類
			Type = static_cast<CBgModel::TYPE>(nType); // 背景モデルのタイプにキャスト
			CBgModel::SetType(Type); // 背景モデルのタイプを設定
		}
		// オブジェクトX情報の設定
		else if (Buff == "SETOBJECTX")
		{
			CObjectX::LoadInfoTxt(LoadingFile, listSaveManager, Buff, this);
		}
	}

	listSaveManager.push_back(this); // ステージマネージャーのオブジェクトリストに情報を登録する
}

//===================================================================================================================
// ステージマネージャー管理オブジェクトの種類を変える
//===================================================================================================================
CObject* CBgModel::ManagerChengeObject(bool bAim)
{
	// === 変数 ===

	int nNewType = int(m_Type); // 新しいタイプ
	CBgModel::TYPE NewType = {};// 背景モデルのタイプ

	// === 処理 ===

	// trueならタイプ番号をインクリメント
	if (bAim == true)
	{
		nNewType++;
	}
	// falseならタイプ番号をデクリメント
	else
	{
		nNewType--;
	}

	// 範囲制限
	if (nNewType >= static_cast<int>(CBgModel::TYPE::MAX))
	{
		nNewType = 0;
	}
	if (nNewType < 0)
	{
		nNewType = static_cast<int>(CBgModel::TYPE::MAX) - 1;
	}

	NewType = static_cast<CBgModel::TYPE>(nNewType); // 上記で変更したタイプに設定

	// 上記で変更したタイプにするので破棄して新しいタイプのオブジェクトを返す
	SetUseDeath(true); // 死亡フラグを使用する
	SetDeath();        // 死亡フラグを設定
	return CBgModel::Create(NewType, GetPosInfo().GetPos(), GetRotInfo().GetRot(), GetSizeInfo().GetScale(),GetSizeInfo().GetUseSwapVtxXZ());
}

//===================================================================================================================
// ステージマネージャーにオブジェクトを保存する
//===================================================================================================================
CObject* CBgModel::ManagerSaveObject()
{
	return CBgModel::Create(m_Type, GetPosInfo().GetPos(), GetRotInfo().GetRot(), GetSizeInfo().GetScale(), GetSizeInfo().GetUseSwapVtxXZ());//生成したオブジェクトを返す
}

//===================================================================================================================
// 種類を取得
//===================================================================================================================
const CBgModel::TYPE& CBgModel::GetType() const
{
	return m_Type;
}

//===================================================================================================================
// 種類を設定
//===================================================================================================================
void CBgModel::SetType(TYPE Type)
{
	// === 処理 ===

	Uninit(); // 現在のモデル情報を終了する

	// === 変数 ===
	
	//モデル情報設定
	int nIdx = CManager::GetObjectXInfo()->Regist(BGMODEL_FILENAME[static_cast<int>(Type)]);
	m_Type = Type; // タイプ番号を格納

	// === 処理 ===

	// モデル情報を割り当てる
	BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(nIdx),
		CManager::GetObjectXInfo()->GetBuffMat(nIdx),
		CManager::GetObjectXInfo()->GetdwNumMat(nIdx),
		CManager::GetObjectXInfo()->GetTexture(nIdx),
		CManager::GetObjectXInfo()->GetColorValue(nIdx));

	SetSize(); // サイズを設定する
}
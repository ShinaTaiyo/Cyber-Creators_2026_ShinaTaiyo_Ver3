//===================================================================================================================
//
// ２０２５年６月５日：コードを綺麗にする[block.cpp]
// Author:ShinaTaiyo
//
//===================================================================================================================

//*******************************************************************************************************************
// インクルード
//*******************************************************************************************************************
#include "block.h"         // 元ヘッダ
#include "manager.h"       // マネージャー管理オブジェクトの取得
#include "objectXInfo.h"   // モデル情報の取得

//*******************************************************************************************************************
// 静的メンバ宣言
//*******************************************************************************************************************

// モデルファイル名
const string CBlock::s_BlockModelFileName[static_cast<int>(CBlock::TYPE::MAX)] =
{ "data\\MODEL\\Block\\Block00_Normal.x",
  "data\\MODEL\\Block\\Block01_Water.x", 
  "data\\MODEL\\Block\\Renga_000.x", 
  "data\\MODEL\\Block\\BlackBlock_000.x", 
};

//===================================================================================================================
// 
// publicメンバ 
//
//===================================================================================================================

//===================================================================================================================
// コンストラクタ
//===================================================================================================================
CBlock::CBlock(int nPri, bool bUseintPri, CObject::TYPE type, CObject::OBJECTTYPE ObjType) :
	CObjectX(nPri,bUseintPri,type,ObjType),
	m_type{TYPE::NORMAL}
{
	// 何もなし
}

//===================================================================================================================
// デストラクタ
//===================================================================================================================
CBlock::~CBlock()
{
	// 何もなし
}

//===================================================================================================================
// 初期化処理
//===================================================================================================================
HRESULT CBlock::Init()
{
	CObjectX::Init(); // オブジェクトXの初期化
	return S_OK;
}

//===================================================================================================================
// 終了処理
//===================================================================================================================
void CBlock::Uninit()
{
	CObjectX::Uninit(); // オブジェクトX終了処理
}

//===================================================================================================================
// 更新処理
//===================================================================================================================
void CBlock::Update()
{
	CObjectX::Update(); // オブジェクトX更新処理
}

//===================================================================================================================
// 描画処理
//===================================================================================================================
void CBlock::Draw()
{
	CObjectX::Draw(); // オブジェクトX描画処理
}

//===================================================================================================================
// 死亡フラグ設定処理
//===================================================================================================================
void CBlock::SetDeath()
{
	CObjectX::SetDeath(); //オブジェクトX死亡フラグ設定処理
}

//===================================================================================================================
// 生成処理
//===================================================================================================================
CBlock* CBlock::Create(TYPE type, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, bool bSwapVtxXZ)
{
	CBlock* pBlock = DBG_NEW CBlock(); // インスタンスを生成

	// インスタンスの生成に失敗
	if (pBlock == nullptr)
	{
		assert(false && "ブロックのインスタンスの生成に失敗！");
	}

	// === オブジェクトX基本情報取得 ===

	CObjectX::PosInfo& SelfPosInfo = pBlock->GetPosInfo(); // 位置情報
	CObjectX::RotInfo& SelfRotInfo = pBlock->GetRotInfo(); // 向き情報
	CObjectX::SizeInfo& SelfSizeInfo = pBlock->GetSizeInfo(); // サイズ情報
	CObjectX::DrawInfo& SelfDrawInfo = pBlock->GetDrawInfo(); // 描画情報
	CObjectX::LifeInfo& SelfLifeInfo = pBlock->GetLifeInfo(); // 体力情報

	// === 基本情報設定 ===

	pBlock->Init(); // 初期化処理
	pBlock->CObject::SetType(CObject::TYPE::BLOCK); // オブジェクトの種類設定
	pBlock->SetUseDeath(true); // 死亡フラグを発動するかどうかを設定
	pBlock->SetType(type);   // ブロックの種類設定
	SelfRotInfo.SetRot(rot); // 向きを設定
	SelfPosInfo.SetSupportPos(pos); // 設置位置設定
	SelfPosInfo.SetPos(pos);     // 位置設定
	SelfPosInfo.SetPosOld(pos);  //1f前の位置を設定
	SelfLifeInfo.SetLife(nLife); // 体力設定
	SelfLifeInfo.SetMaxLife(nLife);     // 最大体力設定
	SelfLifeInfo.SetAutoSubLife(false); // 自動的に体力を減らすかどうか設定
	SelfSizeInfo.SetUseSwapVtxXZ(bSwapVtxXZ); // 頂点XZを入れ替える
	SelfSizeInfo.SetScale(scale);       // 拡大率設定
	SelfSizeInfo.SetFormarScale(scale); //元となる拡大率設定
	SelfDrawInfo.SetUseShadow(false); // 影を描画するかどうか設定
	SelfDrawInfo.SetUseDraw(true);    //描画するかどうか設定													 
	pBlock->CObjectX::SetObjXType(CObjectX::OBJECTXTYPE_BLOCK);      // オブジェクトXのタイプを設定
	pBlock->CObjectX::SetTypeNum((int)(type)); // オブジェクトXごとのタイプ番号を設定
	pBlock->GetSizeInfo().ActiveSwapVtxMaxMin(); // 最大頂点と最小頂点を入れ替えれるようにする
	pBlock->SetManagerObjectType(CObject::MANAGEROBJECTTYPE::BLOCK); //マネージャーで呼び出す時の種類を設定
	return pBlock;
}

//===================================================================================================================
// ブロックタイプ設定
//===================================================================================================================
void CBlock::SetType(TYPE Type)
{
	int nType = static_cast<int>(Type); // タイプ番号を格納

	// 配列外アクセスチェック
	if (nType < 0 || nType >= static_cast<int>(TYPE::MAX))
	{
		// アサート（どちらかがfalseならassertが呼ばれるので、右辺はコメントとして使えます。）
		assert(false && "ブロックで配列外アクセス！");
	}
	// 配列外アクセスしなかった
	else
	{
		Uninit(); // 終了処理（モデルを更新するので)
		m_type = Type; // タイプを設定
		int nIdx = 0; // モデル番号       

		// モデル情報の登録、モデル番号の取得
		nIdx = CManager::GetObjectXInfo()->Regist(s_BlockModelFileName[(int)(Type)]);   

		// モデル情報を割り当てる
		CObjectX::BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(nIdx),       
			CManager::GetObjectXInfo()->GetBuffMat(nIdx),
			CManager::GetObjectXInfo()->GetdwNumMat(nIdx),
			CManager::GetObjectXInfo()->GetTexture(nIdx),
			CManager::GetObjectXInfo()->GetColorValue(nIdx));

		SetSize(); // サイズ設定
	}

}

//===================================================================================================================
// ブロックタイプ取得
//===================================================================================================================
const CBlock::TYPE & CBlock::GetType()
{
	return m_type;
}

//===================================================================================================================
// 情報をテキストファイルに保存する
//===================================================================================================================
void CBlock::SaveInfoTxt(fstream& WritingFile)
{
	WritingFile << "SETBLOCK" << endl;
	WritingFile << "TYPE = " << static_cast<int>(m_type);

	// タイプによって名前を記述
	switch (m_type)
	{
	case TYPE::NORMAL:
		WritingFile << " # NORMALBLOCK" << endl;
		break;
	case TYPE::WATER:
		WritingFile << " # WATERBLOCK" << endl;
		break;
	case TYPE::RENGA:
		WritingFile << " # RENGABLOCK" << endl;
		break;
	case TYPE::BLACK:
		WritingFile << " # BLACKBLOCK" << endl;
		break;
	default:
		break;
	}

	CObjectX::SaveInfoTxt(WritingFile); // オブジェクトXの情報をテキストファイルに保存する

	WritingFile << "END_SETBLOCK" << endl;
}

//===================================================================================================================
// 呼び出し元にタイプを変更したオブジェクトを返す
//===================================================================================================================
CObject* CBlock::ManagerChengeObject(bool bAim)
{
	int nNewType = int(m_type);  // インクリメントやデクリメントをしたいからintにキャスト
    TYPE NewType = TYPE::NORMAL; // 新しいブロックの種類を初期化

	nNewType += bAim ? 1 : -1; // 引数のフラグに応じてタイプを移動させる

	// === タイプ範囲制限 ====

	if (nNewType >= static_cast<int>(TYPE::MAX))
	{
		nNewType = static_cast<int>(TYPE::NORMAL);
	}
	if (nNewType < 0)
	{
		nNewType = static_cast<int>(TYPE::MAX) - 1;
	}

	// === 変更情報設定開始 ===

	NewType = TYPE(nNewType); // 上記で変更したタイプを新しいブロックのタイプにキャスト
	SetUseDeath(true); // 死亡フラグを使用する
	SetDeath();        // 死亡フラグを設定する

	return CBlock::Create(NewType, GetLifeInfo().GetMaxLife(), GetPosInfo().GetPos(), GetRotInfo().GetRot(), GetSizeInfo().GetScale(), GetSizeInfo().GetUseSwapVtxXZ());//生成したオブジェクトを返す
}

//===================================================================================================================
// テキストファイルから情報を読み込む処理
//===================================================================================================================
void CBlock::LoadInfoTxt(fstream& LoadingFile, list<CObject*>& listSaveManager, string& Buff, CObject* pObj)
{
	int nType = 0; // 種類（読み込み用)
	TYPE Type = TYPE::NORMAL; //ブロックの種類

	while (Buff != "END_SETBLOCK")
	{
		LoadingFile >> Buff; // 単語を読み込む

		// 行をスキップ
		if (Buff == "#")
		{
			getline(LoadingFile, Buff);
		}
		// タイプ読み込み
		else if (Buff == "TYPE")
		{
			LoadingFile >> Buff;  // イコール
			LoadingFile >> nType; // タイプ
			Type = TYPE(nType);   // 読み込んだタイプをブロックのタイプにキャスト
			SetType(Type);        // ブロックタイプ設定
		}
		// オブジェクトZの情報を読み込む
		else if (Buff == "SETOBJECTX")
		{
			CObjectX::LoadInfoTxt(LoadingFile, listSaveManager, Buff, this);
		}
	}
	listSaveManager.push_back(this); // ステージオブジェクトリストに情報を保存する

}

//===================================================================================================================
// 保存する情報を返す
//===================================================================================================================
CObject* CBlock::ManagerSaveObject()
{
	return CBlock::Create(m_type, GetLifeInfo().GetMaxLife(), GetPosInfo().GetPos(), GetRotInfo().GetRot(), GetSizeInfo().GetScale(),GetSizeInfo().GetUseSwapVtxXZ());//生成したオブジェクトを返す
}

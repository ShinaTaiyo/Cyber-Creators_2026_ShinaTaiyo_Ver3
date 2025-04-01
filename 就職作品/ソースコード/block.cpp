//==========================================================================================================
//
//７月１２日：全ギミックを実装する[block.cpp]
//Author:ShinaTaiyo
//
//==========================================================================================================

//=========================
//インクルード
//=========================
#include "block.h"
#include "manager.h"
#include "objectXInfo.h"
#include "calculation.h"
#include "stagemanager.h"
#include "player.h"
#include "game.h"
#include "edit.h"
#include "input.h"
#include <stdio.h>
#include <string.h>
//=================================================

//=========================
//静的メンバ
//=========================
int CBlock::m_nNumFile = static_cast<int>(CBlock::BLOCKTYPE::MAX);                                              //ファイル数を格納する
const float CBlock::m_fBLOCKCORRECTIONCOLLISION = 3.0f;      //1,2,3,4,5,6,7,8,9,10,11,12,13 
const char* CBlock::m_BLOCK_FILENAME[static_cast<int>(CBlock::BLOCKTYPE::MAX)] =
{ "data\\MODEL\\Block\\Block00_Normal.x",
  "data\\MODEL\\Block\\Block01_Water.x", 
  "data\\MODEL\\Block\\Renga_000.x", 
  "data\\MODEL\\Block\\BlackBlock_000.x", 
};                                                  //ブロックのXファイルへのポインタ
//========================================================

//=========================
//コンストラクタ
//=========================
CBlock::CBlock(int nPri, bool bUseintPri, CObject::TYPE type, CObject::OBJECTTYPE ObjType) : CObjectX(nPri,bUseintPri,type,ObjType),
m_bCollision(false),m_type(BLOCKTYPE::NORMAL)
{

}
//=================================================

//=========================
//デストラクタ
//=========================
CBlock::~CBlock()
{

}
//=================================================

//=========================
//初期化処理
//=========================
HRESULT CBlock::Init()
{
	CObjectX::Init();             //オブジェクトXの初期化
	m_type = BLOCKTYPE::NORMAL;   //ブロックの種類
	m_bCollision = true;          //当たり判定をするかどうか
	return S_OK;
}
//=================================================

//=========================
//終了処理
//=========================
void CBlock::Uninit()
{
	CObjectX::Uninit();//オブジェクトX終了処理
}
//=================================================

//=========================
//更新処理
//=========================
void CBlock::Update()
{
	CObjectX::Update();//オブジェクトX更新処理
}
//=================================================

//===================================================
//描画処理
//===================================================
void CBlock::Draw()
{
	CObjectX::Draw();//オブジェクトX描画処理
}
//===========================================================================================

//===================================================
//死亡フラグを設定
//===================================================
void CBlock::SetDeath()
{
	CObjectX::SetDeath();//オブジェクトX死亡フラグ設定処理
}
//===========================================================================================

//==================================
//ブロック生成処理
//==================================
CBlock* CBlock::Create(BLOCKTYPE type, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, bool bSwapVtxXZ)
{
	CBlock* pBlock = DBG_NEW CBlock;                                                         //ブロックを生成

	if (pBlock != nullptr)
	{
		pBlock->Init();                                                                      //初期化処理
		pBlock->GetRotInfo().SetRot(rot);                                                    //向きを設定
		pBlock->GetSizeInfo().SetUseSwapVtxXZ(bSwapVtxXZ);                                   //頂点XZを入れ替える
		pBlock->SetUseDeath(true);                                                           //死亡フラグを発動するかどうかを設定する
		pBlock->SetBlockType(type);                                                               //ブロックの種類
		pBlock->GetLifeInfo().SetLife(nLife);                                                //ブロックの体力
		pBlock->GetLifeInfo().SetMaxLife(nLife);                                             //ブロックの最大体力
		pBlock->GetLifeInfo().SetAutoSubLife(false);                                         //自動的に体力を減らすかどうか
		pBlock->GetPosInfo().SetSupportPos(pos);                                             //設置位置
		pBlock->GetDrawInfo().SetUseShadow(false);                                           //影を描画しない
		pBlock->GetDrawInfo().SetUseDraw(true);                                              //描画する
		pBlock->GetPosInfo().SetPos(pos);                                                    //オブジェクトXの位置を設定
		pBlock->GetPosInfo().SetPosOld(pos);                                                 //1f前の位置を設定
		pBlock->CObject::SetType(CObject::TYPE::BLOCK);                                      //オブジェクトの種類を決める
		pBlock->GetSizeInfo().SetScale(scale);                                               //オブジェクトの拡大率
		pBlock->GetSizeInfo().SetFormarScale(scale);                                         //元の拡大率を設定する

		pBlock->CObjectX::SetObjXType(CObjectX::OBJECTXTYPE_BLOCK);                          //オブジェクトXのタイプを設定
		pBlock->CObjectX::SetTypeNum((int)(type));                                           //オブジェクトXごとのタイプ番号を設定
		pBlock->SetManagerObjectType(CObject::MANAGEROBJECTTYPE::BLOCK);                     //マネージャーで呼び出す時の種類を設定
		pBlock->GetSizeInfo().ActiveSwapVtxMaxMin();                                         //最大頂点と最小頂点を入れ替えれるようにする
	}
	return pBlock;
}
//================================================================================================


//===============================================================
//ブロックのタイプを設定する
//===============================================================
void CBlock::SetBlockType(BLOCKTYPE Type)
{
	int nType = static_cast<int>(Type);//タイプ番号を格納
	if (nType < 0 || nType >= static_cast<int>(BLOCKTYPE::MAX))
	{//例外処理
		assert("ブロックで配列外アクセス！");
	}
	else
	{
		Uninit();//終了処理（モデルを更新するので)
		m_type = Type;//タイプを設定
		int nIdx = 0;                                                                            //モデル情報のインデックス
		nIdx = CManager::GetObjectXInfo()->Regist(m_BLOCK_FILENAME[(int)(Type)]);            //モデル情報を登録し、モデル番号を取得
		CObjectX::BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(nIdx),         //モデル情報を設定する
			CManager::GetObjectXInfo()->GetBuffMat(nIdx),
			CManager::GetObjectXInfo()->GetdwNumMat(nIdx),
			CManager::GetObjectXInfo()->GetTexture(nIdx),
			CManager::GetObjectXInfo()->GetColorValue(nIdx));

		SetSize();//サイズ設定
	}
}
//================================================================================================

//===============================================================
//ブロックのタイプを取得する
//===============================================================
CBlock::BLOCKTYPE CBlock::GetType()
{
	return m_type;//ブロックのタイプを取得する
}
//=========================================================================================================================

//=======================================================================
//必要な情報をテキストファイルに保存する
//=======================================================================
void CBlock::SaveInfoTxt(fstream& WritingFile)
{
	WritingFile << "SETBLOCK" << endl;
	WritingFile << "TYPE = " << static_cast<int>(m_type);
	switch (m_type)
	{
	case BLOCKTYPE::NORMAL:
		WritingFile << " # NORMALBLOCK" << endl;
		break;
	case BLOCKTYPE::WATER:
		WritingFile << " # WATERBLOCK" << endl;
		break;
	case BLOCKTYPE::RENGA:
		WritingFile << " # RENGABLOCK" << endl;
		break;
	case BLOCKTYPE::BLACK:
		WritingFile << " # BLACKBLOCK" << endl;
		break;
	default:
		break;
	}

	CObjectX::SaveInfoTxt(WritingFile);

	WritingFile << "END_SETBLOCK" << endl;
}
//=========================================================================================================================

//=======================================================================
//ステージマネージャーに変更したオブジェクトを返す
//=======================================================================
CObject* CBlock::ManagerChengeObject(bool bAim)
{
	int nNewType = int(m_type);//インクリメントやデクリメントをしたいからintにキャスト
	BLOCKTYPE NewType = BLOCKTYPE::NORMAL;//新しいブロックの種類を初期化

	//=======================================
	//種類を変える
    //=======================================
	if (bAim == true)
	{
		nNewType++;
	}
	else
	{
		nNewType--;
	}
	if (nNewType >= static_cast<int>(BLOCKTYPE::MAX))
	{
		nNewType = static_cast<int>(BLOCKTYPE::NORMAL);
	}
	if (nNewType < 0)
	{
		nNewType = static_cast<int>(BLOCKTYPE::MAX) - 1;
	}
	//======================================================================================

	//=======================================
	//設定する
	//=======================================
	NewType = BLOCKTYPE(nNewType);//上記で変更したタイプを新しいブロックのタイプにキャスト
	//======================================================================================

	//=======================================
	//死亡フラグを設定する
	//=======================================
	SetUseDeath(true);//死亡フラグを発動するかどうかをtrue1にする
	SetDeath();       //死亡フラグを設定する
	//======================================================================================

	return CBlock::Create(NewType, GetLifeInfo().GetMaxLife(), GetPosInfo().GetPos(), GetRotInfo().GetRot(), GetSizeInfo().GetScale(), GetSizeInfo().GetUseSwapVtxXZ());//生成したオブジェクトを返す
}
//=========================================================================================================================

////=======================================================================
////テキストファイルから読み込んだ情報を保存する処理
////=======================================================================
//void CBlock::LoadInfoTxt(fstream& LoadingFile, list<CObject*>& listSaveManager, string& Buff)
//{
//	int nType = 0;                                  //種類
//	int nLife = 0;                                  //体力
//	D3DXVECTOR3 Move = D3DXVECTOR3(0.0f,0.0f,0.0f); //移動量
//	D3DXVECTOR3 Pos = D3DXVECTOR3(0.0f,0.0f,0.0f);  //位置
//	D3DXVECTOR3 Scale = D3DXVECTOR3(0.0f,0.0f,0.0f);//拡大率
//	D3DXVECTOR3 Rot = D3DXVECTOR3(0.0f,0.0f,0.0f);  //向き
//	BLOCKTYPE Type = BLOCKTYPE::NORMAL;             //ブロックの種類
//	bool bSwapVtxXZ = false;                        //XZの頂点を入れ替えるかどうか
//	while (Buff != "END_SETBLOCK")
//	{
//		LoadingFile >> Buff;           //単語を読み込む
//		if (Buff == "#")
//		{//行をスキップ
//			getline(LoadingFile, Buff);
//		}
//		else if (Buff == "TYPE")
//		{
//			LoadingFile >> Buff;       //イコール
//			LoadingFile >> nType;      //種類
//		}
//		else if (Buff == "LIFE")
//		{
//			LoadingFile >> Buff;       //イコール
//			LoadingFile >> nLife;      //体力
//		}
//		else if (Buff == "MOVE")
//		{
//			LoadingFile >> Buff;       //イコール
//			LoadingFile >> Move.x;     //移動量X
//			LoadingFile >> Move.y;     //移動量Y
//			LoadingFile >> Move.z;     //移動量Z
//		}
//		else if (Buff == "POS")
//		{
//			LoadingFile >> Buff;       //イコール
//			LoadingFile >> Pos.x;      //位置X
//			LoadingFile >> Pos.y;      //位置Y
//			LoadingFile >> Pos.z;      //位置Z
//		}
//		else if (Buff == "ROT")
//		{
//			LoadingFile >> Buff;       //イコール
//			LoadingFile >> Rot.x;      //位置X
//			LoadingFile >> Rot.y;      //位置Y
//			LoadingFile >> Rot.z;      //位置Z
//		}
//		else if (Buff == "SCALE")
//		{
//			LoadingFile >> Buff;       //イコール
//			LoadingFile >> Scale.x;    //拡大率X
//			LoadingFile >> Scale.y;    //拡大率Y
//			LoadingFile >> Scale.z;    //拡大率Z
//		}
//		else if (Buff == "SWAPVTXXZ")
//		{
//			LoadingFile >> Buff;       //イコール
//			LoadingFile >> bSwapVtxXZ; //XZの頂点を入れ替えるかどうか
//		}
//	}
//	Type = BLOCKTYPE(nType);           //ブロックの種類を設定
//
//	listSaveManager.push_back(CBlock::Create(Type, nLife, Pos, Rot, Scale, bSwapVtxXZ));//ステージオブジェクトリストに情報を保存する
//}
////=========================================================================================================================

//=======================================================================
//テキストファイルから読み込んだ情報を保存する処理
//=======================================================================
void CBlock::LoadInfoTxt(fstream& LoadingFile, list<CObject*>& listSaveManager, string& Buff, CObject* pObj)
{
	int nType = 0;                                  //種類
	BLOCKTYPE Type = BLOCKTYPE::NORMAL;             //ブロックの種類
	while (Buff != "END_SETBLOCK")
	{
		LoadingFile >> Buff;           //単語を読み込む
		if (Buff == "#")
		{//行をスキップ
			getline(LoadingFile, Buff);
		}
		else if (Buff == "TYPE")
		{
			LoadingFile >> Buff;       //イコール
			LoadingFile >> nType;      //種類
			Type = BLOCKTYPE(nType);   //ブロックの種類を設定
			SetBlockType(Type);        //ブロックのタイプを設定
		}
		else if (Buff == "SETOBJECTX")
		{//オブジェクトXの情報を設定する
			CObjectX::LoadInfoTxt(LoadingFile, listSaveManager, Buff, this);
		}
	}
	listSaveManager.push_back(this);   //ステージオブジェクトリストに情報を保存する

}
//=========================================================================================================================

//=======================================================================
//ステージマネージャーにセーブする
//=======================================================================
CObject* CBlock::ManagerSaveObject()
{
	return CBlock::Create(m_type, GetLifeInfo().GetMaxLife(), GetPosInfo().GetPos(), GetRotInfo().GetRot(), GetSizeInfo().GetScale(),GetSizeInfo().GetUseSwapVtxXZ());//生成したオブジェクトを返す
}
//=========================================================================================================================
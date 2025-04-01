//======================================================================================================================
//
//１０月２４日：攻撃用のクラスを作る[BgModel.cpp]
//Author:ShinaTaiyo
//
//======================================================================================================================

//==================================================================
//インクルード
//==================================================================
#include "bgModel.h"
#include "manager.h"
#include "objectX.h"
#include "objectXInfo.h"
//======================================================================================================================


//==================================================================
//前方宣言
//==================================================================
const string CBgModel::BGMODEL_FILENAME[static_cast<int>(CBgModel::BGMODELTYPE::MAX)] =
{//背景モデルのファイル名
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
//======================================================================================================================

//==================================================================
//コンストラクタ
//==================================================================
CBgModel::CBgModel(int nPri, bool bUseintPri, CObject::TYPE type, CObject::OBJECTTYPE ObjType) : m_Type(BGMODELTYPE::BILL_00),
CObjectX(nPri,bUseintPri,type,ObjType)
{

}
//======================================================================================================================

//==================================================================
//デストラクタ
//==================================================================
CBgModel::~CBgModel()
{

}
//======================================================================================================================

//==================================================================
//初期化処理
//==================================================================
HRESULT CBgModel::Init()
{
	CObjectX::Init();//オブジェクトXの初期化処理
	return S_OK;
}
//======================================================================================================================

//==================================================================
//終了処理
//==================================================================
void CBgModel::Uninit()
{
	CObjectX::Uninit();//オブジェクトXの終了処理
}
//======================================================================================================================

//==================================================================
//更新処理
//==================================================================
void CBgModel::Update()
{
	CObjectX::Update();//オブジェクトXの更新処理
}
//======================================================================================================================

//==================================================================
//描画処理
//==================================================================
void CBgModel::Draw()
{
	CObjectX::Draw();//オブジェクトXの描画処理
}
//======================================================================================================================

//==================================================================
//死亡フラグ設定処理
//==================================================================
void CBgModel::SetDeath()
{
	CObjectX::SetDeath();//死亡フラグ設定処理
}
//======================================================================================================================

//==================================================================
//生成処理
//==================================================================
CBgModel* CBgModel::Create(BGMODELTYPE bgModelType, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 Scale, bool bSwapVtxXZ)
{
	CBgModel* pBgModel = DBG_NEW CBgModel();              //生成

	pBgModel->Init();                                     //初期化処理
	pBgModel->SetBgModelType(bgModelType);                //背景モデルの種類を設定する
	pBgModel->GetPosInfo().SetPos(pos);                   //位置を設定  
	pBgModel->GetPosInfo().SetSupportPos(pos);            //支点となる位置を設定
	pBgModel->GetRotInfo().SetRot(rot);                   //向きを設定
	pBgModel->GetSizeInfo().SetScale(Scale);              //拡大率を設定
	pBgModel->GetSizeInfo().SetUseSwapVtxXZ(bSwapVtxXZ);  //XZの頂点のサイズを入れ替えるかどうか
	pBgModel->GetDrawInfo().SetUseShadow(false);          //影を使用しない

	pBgModel->SetManagerObjectType(CObject::MANAGEROBJECTTYPE::BGMODEL);//ステージマネージャーで呼び出す時の種類を設定

	if (bSwapVtxXZ == true)
	{//XZのサイズを入れ替えるなら
		D3DXVECTOR3 VtxMax = pBgModel->GetSizeInfo().GetOriginVtxMax();//元の最大頂点を取得
		D3DXVECTOR3 VtxMin = pBgModel->GetSizeInfo().GetOriginVtxMin();//元の最小頂点を取得

		VtxMax.x = pBgModel->GetSizeInfo().GetOriginVtxMax().z;//最大頂点のXをZに
		VtxMax.z = pBgModel->GetSizeInfo().GetOriginVtxMax().x;//最大頂点のZをXに
		VtxMin.x = pBgModel->GetSizeInfo().GetOriginVtxMin().z;//最小頂点のXをZに
		VtxMin.z = pBgModel->GetSizeInfo().GetOriginVtxMin().x;//最小頂点のZをXに
		pBgModel->GetSizeInfo().SetOriginVtxMax(VtxMax);       //元の最大頂点を現在設定した情報に更新
		pBgModel->GetSizeInfo().SetOriginVtxMin(VtxMin);       //元の最小頂点を現在設定した情報に更新
	}
	return pBgModel;
}
//======================================================================================================================


//==================================================================
//ステージマネージャーに情報を保存する
//==================================================================
void CBgModel::SaveInfoTxt(fstream& WritingFile)
{
	WritingFile << "SETBGMODEL" << endl;
	WritingFile << "TYPE = " << static_cast<int>(m_Type);
	switch (m_Type)
	{
	case BGMODELTYPE::BILL_00:
		WritingFile << " # BILL00" << endl;
		break;
	case BGMODELTYPE::TREE_00:
		WritingFile << " # TREE00" << endl;
		break;
	default:
		break;
	}

	CObjectX::SaveInfoTxt(WritingFile);

	WritingFile << "END_SETBGMODEL" << endl;

}
//======================================================================================================================

////==================================================================
////ステージマネージャーから情報を読み込む
////==================================================================
//void CBgModel::LoadInfoTxt(fstream& LoadingFile, list<CObject*>& listSaveManager, string& Buff)
//{
//	int nType = 0;//種類
//	D3DXVECTOR3 Move = D3DXVECTOR3(0.0f,0.0f,0.0f);  //移動量
//	D3DXVECTOR3 Pos = D3DXVECTOR3(0.0f,0.0f,0.0f);   //位置
//	D3DXVECTOR3 Scale = D3DXVECTOR3(0.0f,0.0f,0.0f); //拡大率
//	D3DXVECTOR3 Rot = D3DXVECTOR3(0.0f,0.0f,0.0f);   //向き
//	BGMODELTYPE Type = {};            //背景モデルの種類
//	bool bSwapVtxXZ = false;//XZの頂点を
//	while (Buff != "END_SETBGMODEL")
//	{
//		LoadingFile >> Buff;//単語を読み込む
//		if (Buff == "#")
//		{
//			getline(LoadingFile, Buff);
//		}
//		else if (Buff == "TYPE")
//		{
//			LoadingFile >> Buff;//イコール
//			LoadingFile >> nType;      //種類
//		}
//		else if (Buff == "POS")
//		{
//			LoadingFile >> Buff;//イコール
//			LoadingFile >> Pos.x;      //位置X
//			LoadingFile >> Pos.y;      //位置Y
//			LoadingFile >> Pos.z;      //位置Z
//		}
//		else if (Buff == "ROT")
//		{
//			LoadingFile >> Buff;//イコール
//			LoadingFile >> Rot.x;      //位置X
//			LoadingFile >> Rot.y;      //位置Y
//			LoadingFile >> Rot.z;      //位置Z
//		}
//		else if (Buff == "SCALE")
//		{
//			LoadingFile >> Buff;//イコール
//			LoadingFile >> Scale.x;      //拡大率X
//			LoadingFile >> Scale.y;      //拡大率Y
//			LoadingFile >> Scale.z;      //拡大率Z
//		}
//		else if (Buff == "SWAPVTXXZ")
//		{
//			LoadingFile >> Buff;//イコール
//			LoadingFile >> bSwapVtxXZ;
//		}
//	}
//	Type = BGMODELTYPE(nType);
//
//	listSaveManager.push_back(CBgModel::Create(Type,Pos, Rot, Scale,bSwapVtxXZ));//vectorに情報を保存する
//
//}
////======================================================================================================================

//==================================================================
//ステージマネージャーから情報を読み込む
//==================================================================
void CBgModel::LoadInfoTxt(fstream& LoadingFile, list<CObject*>& listSaveManager, string& Buff, CObject* pObj)
{
	int nType = 0;                                       //種類
	BGMODELTYPE Type = {};                               //背景モデルの種類
	while (Buff != "END_SETBGMODEL")
	{
		LoadingFile >> Buff;//単語を読み込む
		if (Buff == "#")
		{
			getline(LoadingFile, Buff);
		}
		else if (Buff == "TYPE")
		{
			LoadingFile >> Buff;                    //イコール
			LoadingFile >> nType;                   //種類
			Type = static_cast<BGMODELTYPE>(nType); //背景モデルのタイプにキャスト
			SetBgModelType(Type);                   //背景モデルのタイプを設定
		}
		else if (Buff == "SETOBJECTX")
		{//オブジェクトXの情報を設定する
			CObjectX::LoadInfoTxt(LoadingFile, listSaveManager, Buff, this);
		}
	}
	listSaveManager.push_back(this);//ステージマネージャーのオブジェクトリストに情報を登録する
}
//======================================================================================================================

//==================================================================
//種類を変える
//==================================================================
CObject* CBgModel::ManagerChengeObject(bool bAim)
{
	int nNewType = int(m_Type);
	BGMODELTYPE NewType = {};
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
	if (nNewType >= static_cast<int>(BGMODELTYPE::MAX))
	{
		nNewType = 0;
	}
	if (nNewType < 0)
	{
		nNewType = static_cast<int>(BGMODELTYPE::MAX) - 1;
	}
	//======================================================================================

	//=======================================
	//設定する
	//=======================================
	NewType = BGMODELTYPE(nNewType);
	//======================================================================================

	//=======================================
	//死亡フラグを設定する
	//=======================================
	SetUseDeath(true);
	SetDeath();
	//======================================================================================

	return CBgModel::Create(NewType, GetPosInfo().GetPos(), GetRotInfo().GetRot(), GetSizeInfo().GetScale(),GetSizeInfo().GetUseSwapVtxXZ());//生成したオブジェクトを返す
}
//======================================================================================================================

//==================================================================
//情報を保存する
//==================================================================
CObject* CBgModel::ManagerSaveObject()
{
	return CBgModel::Create(m_Type, GetPosInfo().GetPos(), GetRotInfo().GetRot(), GetSizeInfo().GetScale(), GetSizeInfo().GetUseSwapVtxXZ());//生成したオブジェクトを返す
}
//======================================================================================================================

//==================================================================
//背景モデルの種類を設定する
//==================================================================
void CBgModel::SetBgModelType(BGMODELTYPE Type)
{
	int nType = static_cast<int>(Type);//タイプ番号を取得

	if (nType < 0 || nType >= static_cast<int>(BGMODELTYPE::MAX))
	{
		//例外処理
		assert("背景モデルで配列外アクセス！");
	}
	else
	{
		Uninit();
		//モデル情報設定
		int nIdx = CManager::GetObjectXInfo()->Regist(BGMODEL_FILENAME[static_cast<int>(nType)]);

		//モデル情報を割り当てる
		BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(nIdx),
			CManager::GetObjectXInfo()->GetBuffMat(nIdx),
			CManager::GetObjectXInfo()->GetdwNumMat(nIdx),
			CManager::GetObjectXInfo()->GetTexture(nIdx),
			CManager::GetObjectXInfo()->GetColorValue(nIdx));

		SetSize();//サイズを設定する

	}
}
//======================================================================================================================

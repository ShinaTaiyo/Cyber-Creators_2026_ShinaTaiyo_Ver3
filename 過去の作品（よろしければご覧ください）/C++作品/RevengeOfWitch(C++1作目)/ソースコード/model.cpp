//===========================================================================================================
//
//６月５日：モデルの当たり判定を行う[model.cpp]
//Author:ShinaTaiyo
//
//===========================================================================================================

//==========================================================
//インクルード
//==========================================================
#include "model.h"
#include "manager.h"
#include "renderer.h"
#include "objectXInfo.h"
#include "stagemanager.h"
//============================================================================================================

//==========================================================
//静的メンバ初期化
//==========================================================
int CModel::m_nNumFile = CModel::MODELTYPE_MAX;                                               //ファイル数を格納する
const char* CModel::m_BLOCK_FILENAME[CModel::MODELTYPE_MAX] =
{ "data\\MODEL\\bill.x", 
  "data\\MODEL\\TV_000.x",
  "data\\MODEL\\NintendoSwitch_000.x",
  "data\\MODEL\\NormalModel\\Ring_000.x"};                                                    //ブロックのXファイルへのポインタ


//==========================================================
//コンストラクタ
//==========================================================
CModel::CModel()
{

}
//============================================================================================================

//==========================================================
//デストラクタ
//==========================================================
CModel::~CModel()
{

}
//============================================================================================================

//==========================================================
//初期化処理
//==========================================================
HRESULT CModel::Init()
{
	//===========================
	//Xオブジェクト初期化
	//===========================
	CObjectX::Init();

	//=========================================

	m_Type = (MODELTYPE)(0);                       //モデル種類
	return S_OK;
}
//============================================================================================================

//==========================================================
//終了処理
//==========================================================
void CModel::Uninit()
{
	CObjectX::Uninit();//Xオブジェクト終了処理
}
//============================================================================================================

//==========================================================
//更新処理
//==========================================================
void CModel::Update()
{
	CObjectX::Update();
}
//============================================================================================================

//==========================================================
//描画処理
//==========================================================
void CModel::Draw()
{
	CObjectX::Draw();
}
//============================================================================================================

//==========================================================
//死亡フラグを設定
//==========================================================
void CModel::SetDeath()
{
	CObject::SetDeath();
}
//============================================================================================================

//==========================================================
//生成処理
//==========================================================
CModel* CModel::Create(MODELTYPE type, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, D3DXVECTOR3 Scale, CObjectX::ROTTYPE RotType)
{
	CModel* pModel = DBG_NEW CModel;                                                               //ブロックを生成
	bool bSuccess = pModel->CObject::GetCreateSuccess();
	LPDIRECT3DTEXTURE9 pResultTexture[m_nMAXOBJECT] = {};                                        //設定された種類の配列のテクスチャ代入用
	int nIdx = 0;//モデルのインデックス
	if (bSuccess == true)
	{
		if (pModel != nullptr)
		{
			pModel->Init();                                                                                                  //初期化処理
			pModel->SetUseDeath(true);                                                                  //死亡フラグを発動するかどうかを設定する
			pModel->SetAutoSubLife(false);
			pModel->SetUseBlockCollision(true);
			pModel->m_Type = type;                                                                                           //ブロックの種類
			pModel->CObjectX::SetPos(pos);                                                                                   //オブジェクトXの位置を設定
			pModel->SetPosOld(pos);                                                                                         //1f前の位置を設定
			pModel->SetSupportPos(pos);                                                                                     //設置位置
			pModel->CObjectX::SetScale(Scale);                                                                               //オブジェクトの拡大率
			pModel->SetFormarScale(Scale);                                                                                  //元の拡大率を設定する
			nIdx = CManager::GetObjectXInfo()->Regist(CObjectX::OBJECTXTYPE_MODEL, m_BLOCK_FILENAME[(int)(type)]);
			pModel->CObjectX::BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(CObjectX::OBJECTXTYPE_MODEL, nIdx),
				CManager::GetObjectXInfo()->GetBuffMat(CObjectX::OBJECTXTYPE_MODEL, nIdx),
				CManager::GetObjectXInfo()->GetdwNumMat(CObjectX::OBJECTXTYPE_MODEL, nIdx),
				CManager::GetObjectXInfo()->GetTexture(CObjectX::OBJECTXTYPE_MODEL, nIdx),
				CManager::GetObjectXInfo()->GetColorValue(CObjectX::OBJECTXTYPE_MODEL, nIdx));           //モデル情報を割り当てる

			pModel->SetType(CObject::TYPE_MODEL);//タイプ設定
			pModel->CObjectX::SetObjXType(CObjectX::OBJECTXTYPE_MODEL);                            //オブジェクトXのタイプを設定
			pModel->CObjectX::SetTypeNum((int)(type));                                             //オブジェクトXごとのタイプ番号を設定
 			pModel->SetSize();      //Xオブジェクトのサイズを設定する
			pModel->CObjectX::SetRotType(RotType);                                                 //向きの種類を設定
			pModel->SetManagerType((int)(CStageManager::MANAGEROBJECT_MODEL));        //マネージャーで呼び出す時の種類を設定
		}
	}
	else
	{//オブジェクトに空きがなかったので破棄する
		delete pModel;
		pModel = nullptr;
		return nullptr;
	}
	return pModel;
}
//============================================================================================================


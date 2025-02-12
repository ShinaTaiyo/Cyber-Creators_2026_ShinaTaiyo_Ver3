//====================================================================================================================
//
//９月７日：AI用のモデルを作る[aimodel.cpp]
//Author:ShinaTaiyo
//
//====================================================================================================================

//===========================================================
//インクルード
//===========================================================
#include "manager.h"
#include "renderer.h"
#include "aimodel.h"
#include "objectXInfo.h"
//====================================================================================================================

//===========================================================
//静的メンバ
//===========================================================
const char* CAIModel::m_apAIMODEL_FILENAME[CAIModel::AIMODELTYPE_MAX] =
{
	"data\\MODEL\\AIModel\\AIModel00_Square.x",
	"data\\MODEL\\AIModel\\AIModel01_Ball.x",
};//モデルファイル名

//===========================================================
//コンストラクタ
//===========================================================
CAIModel::CAIModel() : m_Type(AIMODELTYPE_SQUARE)
{

}
//====================================================================================================================

//===========================================================
//デストラクタ
//===========================================================
CAIModel::~CAIModel()
{
}
//====================================================================================================================

//===========================================================
//初期化処理
//===========================================================
HRESULT CAIModel::Init()
{
	CObjectX::Init();
	return S_OK;
}
//====================================================================================================================

//===========================================================
//終了処理
//===========================================================	
void CAIModel::Uninit()
{
	CObjectX::Uninit();
}
//====================================================================================================================

//===========================================================
//更新処理
//===========================================================	
void CAIModel::Update()
{
	CObjectX::Update();
}
//====================================================================================================================

//===========================================================
//描画処理
//===========================================================	
void CAIModel::Draw()
{
	CObjectX::Draw();
}
//====================================================================================================================

//===========================================================
//描画処理（オブジェクトのDrawで呼びたくないので別枠）
//===========================================================	
void CAIModel::ExtraDraw()
{
	//CObjectX::Draw();
}
//====================================================================================================================

//===========================================================
//死亡フラグ設定処理
//===========================================================	
void CAIModel::SetDeath()
{
	CObject::SetDeath();
}
//====================================================================================================================

CAIModel* CAIModel::Create(AIMODELTYPE type, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 Scale)
{
	CAIModel* pAiModel = DBG_NEW CAIModel;   //敵を生成
	bool bSuccess = pAiModel->CObject::GetCreateSuccess();
	int nIdx = 0;//モデルのインデックス
	if (bSuccess == true)
	{
		if (pAiModel != nullptr)
		{
			pAiModel->Init();         //初期化処理
			pAiModel->SetUseDeath(false);                                                   //死亡フラグを発動するかどうかを設定する
			pAiModel->m_Type = type;  //敵の種類
			pAiModel->SetLife(1);      //体力
			pAiModel->SetMaxLife(1);   //大体力
			pAiModel->SetAutoSubLife(false);//自動的に体力を減らすかどうか
			pAiModel->CObjectX::SetPos(pos);                                                                                  //オブジェクト２Ｄの位置を設定
			pAiModel->SetPosOld(pos);                               //1f前の位置を設定
			pAiModel->SetUseDraw(false);
			pAiModel->SetSupportPos(pos);                                                                                     //設置位置
			pAiModel->CObject::SetType(CObject::TYPE_AIMODEL);                                                                //オブジェクトの種類を決める
			pAiModel->CObjectX::SetScale(Scale);                                                                              //オブジェクトの拡大率
			pAiModel->SetFormarScale(Scale);                                                                                  //元の拡大率を設定する
			nIdx = CManager::GetObjectXInfo()->Regist(CObjectX::OBJECTXTYPE_AIMODEL, m_apAIMODEL_FILENAME[(int)(type)]);
			pAiModel->CObjectX::BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(CObjectX::OBJECTXTYPE_AIMODEL, nIdx),
				CManager::GetObjectXInfo()->GetBuffMat(CObjectX::OBJECTXTYPE_AIMODEL, nIdx),
				CManager::GetObjectXInfo()->GetdwNumMat(CObjectX::OBJECTXTYPE_AIMODEL, nIdx),
				CManager::GetObjectXInfo()->GetTexture(CObjectX::OBJECTXTYPE_AIMODEL, nIdx),
				CManager::GetObjectXInfo()->GetColorValue(CObjectX::OBJECTXTYPE_AIMODEL, nIdx));                       //モデル情報を割り当てる

			pAiModel->CObjectX::SetObjXType(CObjectX::OBJECTXTYPE_AIMODEL);                          //オブジェクトXのタイプを設定
			pAiModel->CObjectX::SetTypeNum((int)(type));                                             //オブジェクトXごとのタイプ番号を設定
			pAiModel->SetSize();                                                                     //Xオブジェクトのサイズを設定する
			pAiModel->CObjectX::SetRotType(CObjectX::ROTTYPE::ROTTYPE_NORMAL);                       //向きの種類を設定
		}
		else
		{//オブジェクトに空きがなかったので破棄する
			delete pAiModel;
			pAiModel = nullptr;
			return nullptr;
		}
	}
	return pAiModel;
}
//====================================================================================================================
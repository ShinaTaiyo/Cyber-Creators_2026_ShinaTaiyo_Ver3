//=====================================================================================================================
//
//９月３日：立て看板を実装する[signboard.cpp]
//Author:ShinaTaiyo
//
//=====================================================================================================================

//==================================================================
//インクルード
//==================================================================
#include "signboard.h"
#include "renderer.h"
#include "manager.h"
#include "objectXInfo.h"
#include "stagemanager.h"
#include "calculation.h"
#include "game.h"
#include "texture.h"
#include "scene.h"
//=====================================================================================================================

//==================================================================
//静的メンバ宣言
//==================================================================
const char* CSignBoard::m_apSIGNBOARD_TEXTURENAME[CSignBoard::SIGNBOARDMANUALTYPE_MAX] =
{
	"data\\TEXTURE\\SignBoard\\Manual_000_Move_NoSuccess.png",
	"data\\TEXTURE\\SignBoard\\Manual_001_Jump_NoSuccess.png",
	"data\\TEXTURE\\SignBoard\\Manual_002_Dash_NoSuccess.png",
	"data\\TEXTURE\\SignBoard\\Manual_003_WallJump_NoSuccess.png",
	"data\\TEXTURE\\SignBoard\\Manual_004_Shot_NoSuccess.png",
	"data\\TEXTURE\\SignBoard\\Manual_005_ChargeShot_NoSuccess.png",
	"data\\TEXTURE\\SignBoard\\Manual_006_RainShot_NoSuccess.png",
	"data\\TEXTURE\\SignBoard\\Manual_007_BurstShot_NoSuccess.png",
	"data\\TEXTURE\\SignBoard\\Manual_008_RapidShot_NoSuccess.png",
	"data\\TEXTURE\\SignBoard\\Manual_009_Magic_NoSuccess.png",
	"data\\TEXTURE\\SignBoard\\Manual_010_MagicChenge_NoSuccess.png",
	"data\\TEXTURE\\SignBoard\\Manual_011_DoubleJump_NoSuccess.png",
};

const char* CSignBoard::m_apSIGNBOARD_TEXTURENAME_SUCCESS[CSignBoard::SIGNBOARDMANUALTYPE_MAX] =
{
	"data\\TEXTURE\\SignBoard\\Manual_000_Move_Success.png",
	"data\\TEXTURE\\SignBoard\\Manual_001_Jump_Success.png",
	"data\\TEXTURE\\SignBoard\\Manual_002_Dash_Success.png",
	"data\\TEXTURE\\SignBoard\\Manual_003_WallJump_Success.png",
	"data\\TEXTURE\\SignBoard\\Manual_004_Shot_Success.png",
	"data\\TEXTURE\\SignBoard\\Manual_005_ChargeShot_Success.png",
	"data\\TEXTURE\\SignBoard\\Manual_006_RainShot_Success.png",
	"data\\TEXTURE\\SignBoard\\Manual_007_BurstShot_Success.png",
	"data\\TEXTURE\\SignBoard\\Manual_008_RapidShot_Success.png",
	"data\\TEXTURE\\SignBoard\\Manual_009_Magic_Success.png",
	"data\\TEXTURE\\SignBoard\\Manual_010_MagicChenge_Success.png",
	"data\\TEXTURE\\SignBoard\\Manual_011_DoubleJump_Success.png",
};

const char* CSignBoard::m_apSIGNBOARD_FILENAME[CSignBoard::SIGNBOARDTYPE_MAX] =
{
	"data\\MODEL\\SignBoard\\SignBoard_000.x",
};

//それぞれのサブタイプ
int CSignBoard::m_SIGNBOARDSUBTYPENUM[CSignBoard::SIGNBOARDTYPE_MAX] =
{
	(int)(CSignBoard::SIGNBOARDMANUALTYPE_MAX)
};

//==================================================================
//コンストラクタ
//==================================================================
CSignBoard::CSignBoard() : m_bSuccessJudge(false),m_ManualType(SIGNBOARDMANUALTYPE00_MOVE),m_SignBoardType(SIGNBOARDTYPE00_NORMAL),m_nSubType(0)
{

}
//=====================================================================================================================

//==================================================================
//デストラクタ
//==================================================================
CSignBoard::~CSignBoard()
{

}
//=====================================================================================================================

//==================================================================
//初期化処理
//==================================================================
HRESULT CSignBoard::Init()
{
	CObjectX::Init();
	m_ManualType = SIGNBOARDMANUALTYPE00_MOVE;
	m_nSubType = 0;
	return S_OK;
}
//=====================================================================================================================

//==================================================================
//終了処理
//==================================================================
void CSignBoard::Uninit()
{
	CObjectX::Uninit();
}
//=====================================================================================================================

//==================================================================
//更新処理
//==================================================================
void CSignBoard::Update()
{
	CObjectX::Update();
	if (CScene::GetMode() == CScene::MODE_GAME)
	{
		float fLength = CCalculation::CalculationLength(D3DXVECTOR3(GetPos().x,0.0f, 0.0f),
			D3DXVECTOR3(CGame::GetPlayer()->GetPos().x,0.0f, 0.0f));//プレイヤーとの距離を測る
		bool bUseCommand = CGame::GetPlayer()->GetJudgeCommand(CPlayer::PLAYERCOMMAND(m_ManualType));//プレイヤーが指定されたコマンドを使用しているかどうか
		CTexture* pTexture = CManager::GetTexture();

		if (fLength < GetSize().x / 2)
		{
			if (bUseCommand == true && m_bSuccessJudge == false)
			{
				m_bSuccessJudge = true;                                                                                             //求めた操作の判定が成功
				SetLockOnMatBindTexture(0, pTexture->GetAddress(pTexture->Regist(m_apSIGNBOARD_TEXTURENAME_SUCCESS[m_ManualType])));//マテリアル０番にテクスチャを反映させる
			}
		}
	}
}
//=====================================================================================================================

//==================================================================
//描画処理
//==================================================================
void CSignBoard::Draw()
{
	CObjectX::Draw();
}
//=====================================================================================================================

//==================================================================
//死亡フラグ設定処理
//==================================================================
void CSignBoard::SetDeath()
{
	CObject::SetDeath();
}
//=====================================================================================================================

//==================================================================
//生成処理
//==================================================================
CSignBoard* CSignBoard::Create(SIGNBOARDTYPE Type, SIGNBOARDMANUALTYPE ManualType, D3DXVECTOR3 pos, D3DXVECTOR3 Scale, D3DXVECTOR3 Rot)
{
	CSignBoard* pSignBoard = DBG_NEW CSignBoard;   //立て看板を生成
	CTexture* pTexture = CManager::GetTexture();
	bool bSuccess = pSignBoard->CObject::GetCreateSuccess();
	int nIdx = 0;//モデルのインデックス
	if (bSuccess == true)
	{
		if (pSignBoard != nullptr)
		{
			pSignBoard->Init();         //初期化処理
			pSignBoard->m_nSubType = int(ManualType);//サブタイプを設定
			pSignBoard->SetUseDeath(true);                                                                  //死亡フラグを発動するかどうかを設定する
			pSignBoard->SetUseBlockCollision(false);
			pSignBoard->SetManualType(ManualType);                                                                //説明の種類
			pSignBoard->SetRot(Rot);                                                                        //向きを設定
			pSignBoard->SetUseGravity(false);
			pSignBoard->CObjectX::SetPos(pos);                                                              //オブジェクト２Ｄの位置を設定
			pSignBoard->SetPosOld(pos);                                                                     //1f前の位置を設定
			pSignBoard->SetSupportPos(pos);                                                                 //設置位置
			pSignBoard->CObject::SetType(CObject::TYPE_MARKER);                                             //オブジェクトの種類を決める
			pSignBoard->CObjectX::SetScale(Scale);                                                          //オブジェクトの拡大率
			pSignBoard->SetFormarScale(Scale);                                                              //元の拡大率を設定する
			pSignBoard->SetAutoSubLife(false);//自動敵に体力を減らすかどうか
			nIdx = CManager::GetObjectXInfo()->Regist(CObjectX::OBJECTXTYPE_MARKER,"data\\MODEL\\SignBoard\\SignBoard_001.x");
			pSignBoard->CObjectX::BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(CObjectX::OBJECTXTYPE_MARKER, nIdx),
				CManager::GetObjectXInfo()->GetBuffMat(CObjectX::OBJECTXTYPE_MARKER, nIdx),
				CManager::GetObjectXInfo()->GetdwNumMat(CObjectX::OBJECTXTYPE_MARKER, nIdx),
				CManager::GetObjectXInfo()->GetTexture(CObjectX::OBJECTXTYPE_MARKER, nIdx),
				CManager::GetObjectXInfo()->GetColorValue(CObjectX::OBJECTXTYPE_MARKER, nIdx));                             //モデル情報を割り当てる

			pSignBoard->SetLockOnMatBindTexture(0, pTexture->GetAddress(pTexture->Regist(m_apSIGNBOARD_TEXTURENAME[ManualType])));//マテリアル０番にテクスチャを反映させる

			pSignBoard->CObjectX::SetObjXType(CObjectX::OBJECTXTYPE_SIGNBOARD);                 //オブジェクトXのタイプを設定
			pSignBoard->SetSize();                                                           //Xオブジェクトのサイズを設定する
			pSignBoard->CObjectX::SetRotType(CObjectX::ROTTYPE::ROTTYPE_NORMAL);             //向きの種類を設定
			pSignBoard->SetManagerType((int)(CStageManager::MANAGEROBJECT_SIGNBOARD));       //マネージャーで呼び出す時の種類を設定

			pSignBoard->SummonTestEnemy();//テスト用の敵を召喚する
		}
		else
		{//オブジェクトに空きがなかったので破棄する
			delete pSignBoard;
			pSignBoard = nullptr;
			return nullptr;
		}
	}
	return pSignBoard;
}
//=====================================================================================================================

//==================================================================
//立て看板説明書：移動の処理
//==================================================================
void CSignBoard::ManualShotProcess()
{
	float fLength = CCalculation::CalculationLength(D3DXVECTOR3(GetPos().x,GetPos().y,0.0f), 
		D3DXVECTOR3(CGame::GetPlayer()->GetPos().x, CGame::GetPlayer()->GetPos().y, 0.0f));//プレイヤーとの距離を測る
	bool bUseCommand = CGame::GetPlayer()->GetJudgeCommand(CPlayer::PLAYERCOMMAND00_MOVE);//プレイヤーが指定されたコマンドを使用しているかどうか
	CTexture* pTexture = CManager::GetTexture();

	if (fLength < GetSize().x / 2)
	{
		if (bUseCommand == true && m_bSuccessJudge == false)
		{
			m_bSuccessJudge = true;                                                                                             //求めた操作の判定が成功
			SetLockOnMatBindTexture(0, pTexture->GetAddress(pTexture->Regist(m_apSIGNBOARD_TEXTURENAME_SUCCESS[m_ManualType])));//マテリアル０番にテクスチャを反映させる
		}
	}
}
//=====================================================================================================================

//==================================================================
//ジャンプ処理
//==================================================================
void CSignBoard::ManualJumpProcess()
{
	float fLength = CCalculation::CalculationLength(D3DXVECTOR3(GetPos().x, GetPos().y, 0.0f),
		D3DXVECTOR3(CGame::GetPlayer()->GetPos().x, CGame::GetPlayer()->GetPos().y, 0.0f));//プレイヤーとの距離を測る
	bool bUseCommand = CGame::GetPlayer()->GetJudgeCommand(CPlayer::PLAYERCOMMAND01_JUMP);//プレイヤーが指定されたコマンドを使用しているかどうか
	CTexture* pTexture = CManager::GetTexture();

	if (fLength < GetSize().x / 2)
	{
		if (bUseCommand == true && m_bSuccessJudge == false)
		{
			m_bSuccessJudge = true;                                                                                             //求めた操作の判定が成功
			SetLockOnMatBindTexture(0, pTexture->GetAddress(pTexture->Regist(m_apSIGNBOARD_TEXTURENAME_SUCCESS[m_ManualType])));//マテリアル０番にテクスチャを反映させる
		}
	}
}
//=====================================================================================================================

//==================================================================
//ダッシュ説明看板の処理
//==================================================================
void CSignBoard::ManualDashProcess()
{
}
//=====================================================================================================================

//==================================================================
//テスト用の敵を召喚する処理
//==================================================================
void CSignBoard::SummonTestEnemy()
{

}
//=====================================================================================================================
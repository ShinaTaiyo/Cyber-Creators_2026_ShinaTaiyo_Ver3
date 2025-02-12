//=============================================================================================
//
//９月７日：タイマーを実装する[timer.cpp]
//Author:ShinaTaiyo
//
//=============================================================================================

//==============================================
//インクルード
//==============================================
#include "timer.h"
#include "manager.h"
#include "texture.h"
#include "fade.h"
#include "number.h"
//=============================================================================================

//==============================================
//コンストラクタ
//==============================================
CTimer::CTimer()
{

}
//=============================================================================================

//==============================================
//デストラクタ
//==============================================
CTimer::~CTimer()
{

}
//=============================================================================================

//==============================================
//初期化処理
//==============================================
HRESULT CTimer::Init()
{
	CObject::Init();
	m_nTime = m_nMAX_TIME;//スコア
	for (int nCnt = 0; nCnt < m_nMAX_TIMERDIGIT; nCnt++)
	{
		m_pNumber[nCnt] = nullptr;//数字へのポインタ 
	}
	return S_OK;
}
//=============================================================================================

//==============================================
//終了処理
//==============================================
void CTimer::Uninit()
{
	CObject::Uninit();
}
//=============================================================================================

//==============================================
//更新処理
//==============================================
void CTimer::Update()
{
	int nCntFrame = GetCntFrame();
	if (nCntFrame % 60 == 0)
	{//制限時間を減らす処理
		SubTimer(-1);
	}

	if (GetUseDeath() == true)
	{
		int nC = 0;
	}

	CObject::Update();
}
//=============================================================================================

//==============================================
//描画処理
//==============================================
void CTimer::Draw()
{
	//CObject2D::Draw();
}
//=============================================================================================

//==============================================
//死亡フラグ設定処理
//==============================================
void CTimer::SetDeath()
{
	for (int nCnt = 0; nCnt < m_nMAX_TIMERDIGIT; nCnt++)
	{
		if (m_pNumber[nCnt] != nullptr)
		{//数字の死亡フラグ設定処理
			m_pNumber[nCnt]->SetUseDeath(true);
			m_pNumber[nCnt]->SetDeath();
			m_pNumber[nCnt] = nullptr;
		}
	}

	CObject::SetDeath();
}
//=============================================================================================

//==============================================
//生成処理
//==============================================
CTimer* CTimer::Create(int nInitialTimer, D3DXVECTOR3 Pos, float fNumberWidth, float fNumberHeight)
{
	CTimer* pTimer = DBG_NEW CTimer;                           //弾を生成 
	bool bSuccess = pTimer->CObject::GetCreateSuccess();         //生成が成功したかどうかを取得する
	if (bSuccess == true)
	{//生成が成功したら
		if (pTimer != nullptr)
		{
			pTimer->Init();                                                                                 //初期化処理
			pTimer->SetUseDeath(false);                                                                     //死亡フラグを発動するかどうかを設定する
			pTimer->CObject::SetType(CObject::TYPE_TIMER);                                                  //オブジェクトの種類を決める
			//pTimer->SetTextureIndex(CManager::GetTexture()->Regist(""));       //テクスチャを割り当てる
			for (int nCnt = 0; nCnt < m_nMAX_TIMERDIGIT; nCnt++)
			{
				pTimer->m_pNumber[nCnt] = CNumber::Create(D3DXVECTOR3(Pos.x - fNumberWidth - fNumberWidth * nCnt,Pos.y + fNumberHeight, 0.0f), fNumberWidth, fNumberHeight);
			}
			//pTimer->BindTexture(CManager::GetTexture()->GetAddress(pTimer->GetTextureIndex()));            //テクスチャを設定する
			pTimer->m_nTime = nInitialTimer;//制限時間初期値
		}
	}
	else
	{//オブジェクトに空きがなかったので破棄する
		delete pTimer;
		pTimer = nullptr;
	}

	return pTimer;
}
//=============================================================================================

//==============================================
//時間を増やす処理
//==============================================
void CTimer::AddTimer(int nTimer)
{
	m_nTime += nTimer;
	int nTempTimer = m_nTime;
	int nTexPos = 0;

	for (int nCnt = 0; nCnt < m_nMAX_TIMERDIGIT; nCnt++)
	{
		nTexPos = nTempTimer % 10;
		nTempTimer /= 10;

		m_pNumber[nCnt]->SetAnim(nTexPos);
	}
}
//=============================================================================================\

//==============================================
//時間を減らす処理
//==============================================
void CTimer::SubTimer(int nTimer)
{
	m_nTime -= nTimer;
	int nTempTimer = m_nTime;
	int nTexPos = 0;

	for (int nCnt = 0; nCnt < m_nMAX_TIMERDIGIT; nCnt++)
	{
		nTexPos = nTempTimer % 10;
		nTempTimer /= 10;

		m_pNumber[nCnt]->SetAnim(nTexPos);
	}

	//if (m_nTime == 0)
	//{
	//	CManager::GetSceneFade()->SetSceneFade(CFade::FADEMODE_IN, CScene::MODE_RESULT);
	//}
}
//=============================================================================================\
//=============================================================================================================================
//
//１２月２０日：敵の攻撃の調整をする[damage.cpp]
//Author:ShinaTaiyo
//
//=============================================================================================================================

//========================================================================
//インクルード
//========================================================================
#include "damage.h"
#include "calculation.h"
#include "manager.h"
#include "renderer.h"
#include "sound.h"
#include "number3d.h"
#include "camera.h"
//=============================================================================================================================

//========================================================================
//コンストラクタ
//========================================================================
CDamage::CDamage() : m_nDamage(0), m_pNumber3D(), m_Pos(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
m_nDigit(0),m_nJumpCnt(m_nMAX_JUMPCNT),m_JumpMove(D3DXVECTOR3(0.0f,10.0f,0.0f)),m_bExeggration(false),m_fWidth(0.0f),
m_fHeight(0.0f)
{

}
//=============================================================================================================================

//========================================================================
//デストラクタ
//========================================================================
CDamage::~CDamage()
{

}
//=============================================================================================================================

//========================================================================
//初期化処理
//========================================================================
HRESULT CDamage::Init()
{
	CObject::Init();
	return S_OK;
}
//=============================================================================================================================

//========================================================================
//終了処理
//========================================================================
void CDamage::Uninit()
{
	CObject::Uninit();
}
//=============================================================================================================================

//========================================================================
//更新処理
//========================================================================
void CDamage::Update()
{
	CObject::Update();
	if (m_pNumber3D[0]->GetPos().y < m_pNumber3D[0]->GetSupportPos().y)
	{
		m_nJumpCnt--;
		m_JumpMove.y /= 2;
		for (int nCnt = 0; nCnt < m_nMAX_DAMAGEDIGIT; nCnt++)
		{
			if (m_pNumber3D[nCnt] != nullptr)
			{
				m_pNumber3D[nCnt]->SetMove(D3DXVECTOR3(0.0f, m_JumpMove.y, 0.0f));
				m_pNumber3D[nCnt]->SetPos(m_pNumber3D[nCnt]->GetSupportPos());
			}
		}
	}

	if (m_nJumpCnt == 0)
	{
		SetDeath();
	}
}
//=============================================================================================================================

//========================================================================
//描画処理
//========================================================================
void CDamage::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	CObject::Draw();

	for (int nCnt = 0; nCnt < m_nDigit; nCnt++)
	{
		if (m_pNumber3D[nCnt] != nullptr)
		{
			m_pNumber3D[nCnt]->ExtraDraw();//ダメージ量の桁数分描画する
		}
	}
}
//=============================================================================================================================

//========================================================================
//死亡フラグ設定処理
//========================================================================
void CDamage::SetDeath()
{
	if (GetUseDeath() == true)
	{
		for (int nCnt = 0; nCnt < m_nMAX_DAMAGEDIGIT; nCnt++)
		{
			if (m_pNumber3D[nCnt] != nullptr)
			{
				m_pNumber3D[nCnt]->SetUseDeath(true);
				m_pNumber3D[nCnt]->SetDeath();
				m_pNumber3D[nCnt] = nullptr;
			}
		}
	}
	CObject::SetDeath();
}
//=============================================================================================================================

//========================================================================
//生成処理
//========================================================================
CDamage* CDamage::Create(int nDamage, D3DXVECTOR3 Pos, D3DXCOLOR col, float fWidth, float fHeight, bool bExeggration)
{
	CDamage* pDamage = DBG_NEW CDamage;                           //弾を生成 
	bool bSuccess = pDamage->CObject::GetCreateSuccess();         //生成が成功したかどうかを取得する
	float fHalfWidth = fWidth * 0.5f;
	pDamage->m_fWidth = fHalfWidth;
	pDamage->m_fHeight = fHeight;
	if (bSuccess == true)
	{//生成が成功したら
		if (pDamage != nullptr)
		{
			if (nDamage > 999)
			{
				nDamage = 999;
			}
			pDamage->Init();                                                                                 //初期化処理
			pDamage->SetUseDeath(true);                                                                     //死亡フラグを発動するかどうかを設定する
			pDamage->m_nDamage = nDamage;//ダメージ量
			pDamage->m_nDigit = CCalculation::CalculationDigit(nDamage);//桁数を計算する
			pDamage->m_Pos = Pos;//位置
			pDamage->CObject::SetType(CObject::TYPE::NONE);                                                  //オブジェクトの種類を決める
			for (int nCnt = 0; nCnt < m_nMAX_DAMAGEDIGIT; nCnt++)
			{
				pDamage->m_pNumber3D[nCnt] = CNumber3D::Create(fWidth, fHeight, D3DXVECTOR3(Pos.x + sinf(CManager::GetCamera()->GetRot().y) * (fHalfWidth) * nCnt, Pos.y + fHeight,
					Pos.z + cosf(CManager::GetCamera()->GetRot().y) * (fHalfWidth) * nCnt),D3DXVECTOR3(0.0f,12.0f,0.0f), col);
				pDamage->m_pNumber3D[nCnt]->SetUseGravity(-0.5f);
			}
			pDamage->DamageDispProcess();
			pDamage->m_bExeggration = bExeggration;
		}
	}
	else
	{//オブジェクトに空きがなかったので破棄する
		delete pDamage;
		pDamage = nullptr;
	}

	return pDamage;

}
//=============================================================================================================================

//========================================================================
//ダメージを表示する処理
//========================================================================
void CDamage::DamageDispProcess()
{
	int nTempDamage = m_nDamage;
	int nTexPos = 0;
	for (int nCnt = 0; nCnt < m_nMAX_DAMAGEDIGIT; nCnt++)
	{
		nTexPos = nTempDamage % 10;
		nTempDamage /= 10;

		m_pNumber3D[nCnt]->SetAnim(nTexPos);
	}
}
//=============================================================================================================================
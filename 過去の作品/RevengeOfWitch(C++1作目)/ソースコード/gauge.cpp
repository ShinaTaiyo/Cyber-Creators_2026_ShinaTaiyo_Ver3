//==================================================================================================================================
//
//６月１６日：ゲージを実装
//Author:ShinaTaiyo
//
//==================================================================================================================================

//===================================================================
//インクルード
//===================================================================
#include "gauge.h"
#include "texture.h"
//==================================================================================================================================

//===================================================================
//コンストラクタ
//===================================================================
CGauge::CGauge() : CObject2D(7)
{
	m_nParam = 0;      //パラメータ
	m_nMaxParam = 0;   //最大パラメータ
	m_type = (GAUGETYPE)(0);//ゲージの種類
}
//==================================================================================================================================

//===================================================================
//デストラクタ
//===================================================================
CGauge::~CGauge()
{

}
//==================================================================================================================================

//===================================================================
//初期化処理
//===================================================================
HRESULT CGauge::Init()
{
	CObject2D::Init();        //2Dオブジェクト初期化
	m_nParam = 0;             //パラメータ
	m_nMaxParam = 0;          //最大パラメータ
	m_nParamOld = 0;          //一つ前のパラメータ
	m_type = (GAUGETYPE)(0);  //ゲージの種類
	m_pDamageGauge = nullptr; //ダメージゲージ
	return S_OK;
}
//==================================================================================================================================


//===================================================================
//終了処理
//===================================================================
void CGauge::Uninit()
{
	CObject2D::Uninit();//2Dオブジェクト終了
}
//==================================================================================================================================

//===================================================================
//更新処理
//===================================================================
void CGauge::Update()
{
	float fRatio;//現在のゲージの割合
	float fMaxWidth = GetMaxWidth();//2Dオブジェクトの横幅の最大値を取得する

	if (m_nParam > m_nMaxParam)
	{//パラメータが最大値を超えていたら、最大値に固定。
		m_nParam = m_nMaxParam;
	}

	fRatio = (float)(m_nParam) / (float)(m_nMaxParam);

	SetWidth(fMaxWidth * fRatio);

	if (m_pDamageGauge != nullptr)
	{
		if (m_pDamageGauge->GetParam() <= 0)
		{
			m_pDamageGauge->SetDeath();
			m_pDamageGauge = nullptr;
		}
	}

	CObject2D::Update();
}
//==================================================================================================================================

//===================================================================
//描画処理
//===================================================================
void CGauge::Draw()
{
	CObject2D::Draw();//2Dオブジェクト描画処理
}
//==================================================================================================================================

//===================================================================
//パラメータを増加させる
//===================================================================
void CGauge::AddParam(int nParam)
{
	m_nParam += nParam;
}
//==================================================================================================================================

//===================================================================
//死亡フラグを設定
//===================================================================
void CGauge::SetDeath()
{
	if (m_pDamageGauge != nullptr)
	{
		m_pDamageGauge->SetUseDeath(true);
		m_pDamageGauge->CObject::SetDeath();
		m_pDamageGauge = nullptr;
	}

	CObject::SetDeath();
}
//==================================================================================================================================

//===================================================================
//パラメータを設定
//===================================================================
void CGauge::SetParam(int nParam)
{
	m_nParamOld = m_nParam;//一つ前のパラメータを設定
	m_nParam = nParam;
	int nDamage = m_nParamOld - m_nParam;//一つ前のパラメータを比べる
	float fRatio = (float)(m_nParam) / (float)(m_nMaxParam);//現在のゲージの割合
	float fMaxWidth = GetMaxWidth();//2Dオブジェクトの横幅の最大値を取得する
	float fDamageRatio = 1.0f - fRatio;
	float fWidth = fMaxWidth * fRatio;
	float fSetWidth = fMaxWidth * fDamageRatio;
	SetWidth(fWidth);

	//if (m_pDamageGauge == nullptr && m_type == GAUGETYPE_BOSSHP)
	//{
	//	m_pDamageGauge = CDamageGauge::Create(GAUGETYPE_DAMAGE, nDamage,GetMaxParam(), GetMaxWidth(), GetHeight(),
	//		D3DXVECTOR3(GetPos().x + fWidth + (float)(nDamage / 2) - 5.0f, GetPos().y, 0.0f),0);
	//}
	//else if (m_pDamageGauge != nullptr && m_type == GAUGETYPE_BOSSHP)
	//{
	//	m_pDamageGauge->AddParam(nDamage);
	//}
}
//==================================================================================================================================

//===================================================================
//生成処理
//===================================================================
CGauge * CGauge::Create(GAUGETYPE type, int nParam, float fWidth, float fHeight, D3DXVECTOR3 pos)
{
	CGauge* pGauge = DBG_NEW CGauge;                             //弾を生成
	bool bSuccess = pGauge->CObject::GetCreateSuccess();         //生成が成功したかどうかを取得する
	if (bSuccess == true)
	{//生成が成功したら
		if (pGauge != nullptr)
		{
			pGauge->Init();                                                                                 //初期化処理
			pGauge->SetUseDeath(false);                                                                     //死亡フラグを発動するかどうかを設定する
			pGauge->m_type = type;                                                                          //弾の種類
			pGauge->m_nParam = nParam;                                                                      //パラメータ
			pGauge->m_nMaxParam = nParam;                                                                   //最大パラメータ
			pGauge->SetPos(pos);                                                                            //オブジェクト２Ｄの位置を設定
			switch (type)
			{
			case GAUGETYPE_PLAYERHP:
				pGauge->SetInfo(1, 1, fWidth, fHeight, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), POLYGONTYPE03_LEFT, false);
				break;
			case GAUGETYPE_PLAYERMP:
				pGauge->SetInfo(1, 1, fWidth, fHeight, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), POLYGONTYPE03_LEFT, false);
				break;
			case GAUGETYPE_BOSSHP:
				pGauge->SetInfo(1, 1, fWidth, fHeight, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), POLYGONTYPE03_LEFT, false);
				break;
			case GAUGETYPE_DAMAGE:
				pGauge->SetInfo(1, 1, fWidth, fHeight, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), POLYGONTYPE03_LEFT, false);
				break;
			case GAUGETYPE_PLAYERUSAGEMP:
				pGauge->SetInfo(1, 1, fWidth, fHeight, D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), POLYGONTYPE03_LEFT, false);
				pGauge->SetUseBlinking(true, 30, 0.0f);//点滅させる
				break;
			default:
				break;
			}
			pGauge->CObject::SetType(CObject::TYPE_GAUGE);                                                  //オブジェクトの種類を決める
		}
	}
	else
	{//オブジェクトに空きがなかったので破棄する
		delete pGauge;
		pGauge = nullptr;
	}

	return pGauge;
}
//==================================================================================================================================

//==========================================================================
//コンストラクタ
//==========================================================================
CDamageGauge::CDamageGauge()
{

}
//==================================================================================================================================

//==========================================================================
//デストラクタ
//==========================================================================
CDamageGauge::~CDamageGauge()
{

}
//==================================================================================================================================

//==========================================================================
//初期化処理
//==========================================================================
HRESULT CDamageGauge::Init()
{

	CGauge::Init();//ゲージの初期化処理
	m_nStartDamageParam = 0;
	m_nSubStartCnt = 0;//ゲージの減少を開始するまでのカウント
	m_bSubGauge = false;//ゲージの減少を開始するカウント
	m_nStartParam = 0;//ダメージ表示を開始する時のパラメータ
	m_nFirstParam = 0;//最新のパラメータ
	m_nAddParamCnt = 0;//パラメータを増やした回数
	return S_OK;
}
//==================================================================================================================================

//==========================================================================
//終了処理
//==========================================================================
void CDamageGauge::Uninit()
{
	CGauge::Uninit();//ゲージの終了処理
}
//==================================================================================================================================

//==========================================================================
//更新処理
//==========================================================================
void CDamageGauge::Update()
{
	float fRatio = 0.0f;//現在のゲージの割合
	float fDamageRatio = 0.0f;//ダメージの割合
	float fMaxWidth = GetMaxWidth();//2Dオブジェクトの横幅の最大値を取得する
	float fFirstRatio = 0.0f;
	float fFirstWidth = 0.0f;
	D3DXVECTOR3 SupportPos = GetSupportPos();
	D3DXVECTOR3 Pos = GetPos();
	m_nSubStartCnt++;//ゲージ減少開始までのカウントをする

	if (m_nSubStartCnt >= m_nSETSUBSTARTCNT && m_bSubGauge == false)
	{
		m_nSubStartCnt = 0;
		m_bSubGauge = true;
	}
	if (m_bSubGauge == true)
	{
		if (m_nSubStartCnt == 0)
		{
			SetPos(D3DXVECTOR3(SupportPos.x - GetWidth(), Pos.y, 0.0f));
			SetSupportPos(GetPos());
			SetInfo(1, 1, GetMaxWidth(), GetHeight(), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), POLYGONTYPE03_LEFT, false);
		}
		else
		{
			SubParam(5);
		}
	}

	fRatio = (float)(m_nParam) / (float)(GetMaxParam());
	SetWidth(fMaxWidth * fRatio);


	CObject2D::Update();
}
//==================================================================================================================================

//==========================================================================
//描画処理
//==========================================================================
void CDamageGauge::Draw()
{
	CGauge::Draw();
}
//==================================================================================================================================

//==========================================================================
//パラメータ増加処理
//==========================================================================
void CDamageGauge::AddParam(int nParam)
{
	m_nParam += nParam;
	m_nFirstParam = nParam;
	m_nSubStartCnt = 0;
}
//==================================================================================================================================

//==========================================================================
//パラメータを設定
//==========================================================================
void CDamageGauge::SetParam(int nParam)
{
	m_nAddParamCnt++;
	if (m_nAddParamCnt == 1)
	{
		m_nFirstParam = nParam;
	}
	m_nSubStartCnt = 0;//ダメージが更新されたので、減少開始のカウントをリセット
	m_nParam = nParam;
}
//==================================================================================================================================

//==========================================================================
//死亡フラグ設定処理
//==========================================================================
void CDamageGauge::SetDeath()
{
	
}
//==================================================================================================================================

//==========================================================================
//ダメージゲージ設定処理
//==========================================================================
CDamageGauge* CDamageGauge::Create(CGauge::GAUGETYPE type, int nParam, int nMaxParam, float fWidth, float fHeight, D3DXVECTOR3 pos, int nStartDamageParam)
{
	CDamageGauge* pDamageGauge = DBG_NEW CDamageGauge;           //ダメージゲージを生成
	bool bSuccess = pDamageGauge->CObject::GetCreateSuccess();         //生成が成功したかどうかを取得する
	if (bSuccess == true)
	{//生成が成功したら
		if (pDamageGauge != nullptr)
		{
			pDamageGauge->Init();                               //初期化処理
			pDamageGauge->m_nStartDamageParam = nStartDamageParam;//ダメージ表示を開始した時のダメージ量
			pDamageGauge->SetParam(nParam);
			pDamageGauge->SetUseDeath(false);                   //死亡フラグを発動するかどうかを設定する
			pDamageGauge->SetGaugeType(type);                   //ゲージの種類
			pDamageGauge->SetStartParam(nParam);                //ダメージ表示を開始する時のパラメータ
			pDamageGauge->SetMaxParam(nMaxParam);               //最大パラメータ
			pDamageGauge->SetPos(pos);                          //オブジェクト２Ｄの位置を設定
			pDamageGauge->SetSupportPos(pos);                   //召喚位置を設定
			switch (type)
			{
			case GAUGETYPE_PLAYERHP:
				pDamageGauge->SetInfo(1, 1, fWidth, fHeight, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), POLYGONTYPE04_RIGHT, false);
				break;
			case GAUGETYPE_PLAYERMP:
				pDamageGauge->SetInfo(1, 1, fWidth, fHeight, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), POLYGONTYPE04_RIGHT, false);
				break;
			case GAUGETYPE_BOSSHP:
				pDamageGauge->SetInfo(1, 1, fWidth, fHeight, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), POLYGONTYPE04_RIGHT, false);
				break;
			case GAUGETYPE_DAMAGE:
				pDamageGauge->SetInfo(1, 1, fWidth, fHeight, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), POLYGONTYPE04_RIGHT, false);
				break;
			default:
				break;
			}
			pDamageGauge->CObject::SetType(CObject::TYPE_GAUGE);                                                  //オブジェクトの種類を決める
		}
	}
	else
	{//オブジェクトに空きがなかったので破棄する
		delete pDamageGauge;
		pDamageGauge = nullptr;
	}

	return pDamageGauge;
}
//==================================================================================================================================
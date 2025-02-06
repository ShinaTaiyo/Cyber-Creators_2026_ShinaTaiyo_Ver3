//===================================================================================================
//
//９月１０日：タイトルを実装する[ui.cpp]
//Author:ShinaTaiyo
//
//===================================================================================================

//====================================================
//インクルード
//====================================================
#include "ui.h"
#include "texture.h"
#include "renderer.h"
#include "manager.h"
#include "sound.h"
#include "calculation.h"
//===================================================================================================

//====================================================
//静的メンバ宣言
//====================================================
const string CUi::UI_FILENAME[int(CUi::UITYPE::MAX)] =
{
	"data\\TEXTURE\\UI\\LockOn_001.png",
	"data\\TEXTURE\\UI\\ActionModeWindow_000.png",
	"data\\TEXTURE\\UI\\ShotModeDisp_000.png",
	"data\\TEXTURE\\UI\\DiveModeDisp_000.png",
	"data\\TEXTURE\\UI\\PhaseText_000.png",
	"data\\TEXTURE\\UI\\StageText_000.png",
	"data\\TEXTURE\\UI\\Target_000.png",
	"data\\TEXTURE\\UI\\PossibleDiveText_000.png",
	"data\\TEXTURE\\UI\\Tutorial_000.png",
	"data\\TEXTURE\\UI\\CheckMark_000.png",
	"data\\TEXTURE\\UI\\Title_001.png",
	"data\\TEXTURE\\UI\\PressEnterOrStart_Logo.png",
	"data\\TEXTURE\\UI\\DiveGaugeFrame_000.png"
};//テクスチャファイル名

//====================================================
//コンストラクタ
//====================================================
CUi::CUi(int nPri, bool bUseintPri, CObject::TYPE type, CObject::OBJECTTYPE ObjType) : CObject2D(nPri,bUseintPri,type,ObjType),
m_MoveType(UIMOVETYPE_NORMAL),m_Type(UITYPE::LOCKON),m_bUseUiEffect(false),m_nSetUiEffectLife(0),m_SetUiEffectColor(D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
{

}
//===================================================================================================

//====================================================
//デストラクタ
//====================================================
CUi::~CUi()
{

}
//===================================================================================================

//====================================================
//初期化処理
//====================================================
HRESULT CUi::Init()
{
	CObject2D::Init();
	return S_OK;
}
//===================================================================================================

//====================================================
//終了処理
//====================================================
void CUi::Uninit()
{
	CObject2D::Uninit();
}
//===================================================================================================

//====================================================
//更新処理
//====================================================
void CUi::Update()
{
	CObject2D::Update();

	if (m_bUseUiEffect == true)
	{
		CUiEffect::Create(m_Type, GetPolygonType(), GetWidth(), GetHeight(), m_nSetUiEffectLife, GetPos(), D3DXVECTOR3(0.0f,0.0f,0.0f), GetRot(), m_SetUiEffectColor);
	}


	if (m_VecUiState.size() > 0)
	{//UI状態の道程配列のデータがあれば
		for (auto it = m_VecUiState.begin(); it != m_VecUiState.end(); ++it)
		{
			(*it)->Process(this);//それぞれの状態の処理を開始する
		}
	}
}
//===================================================================================================

//====================================================
//描画処理
//====================================================
void CUi::Draw()
{
	CObject2D::Draw();
}
//===================================================================================================

//====================================================
//死亡フラグ設定処理
//====================================================
void CUi::SetDeath()
{
	if (GetUseDeath() == true)
	{
		for (auto it = m_VecUiState.begin(); it != m_VecUiState.end(); ++it)
		{
			if (*it != nullptr)
			{//イテレータが指し示すUIステートへのポインタを破棄し、nullptrにする
				delete* it;
				*it = nullptr;
			}
		}
		
		m_VecUiState.clear();//メモリの中の情報をクリア
		m_VecUiState.shrink_to_fit();//メモリを破棄
	}
	CObject::SetDeath();
}
//===================================================================================================

//====================================================
//生成処理
//====================================================
CUi* CUi::Create(UITYPE type, CObject2D::POLYGONTYPE PolygonType, float fWidth, float fHeight, int nLife, bool bUseLife, D3DXVECTOR3 Pos, D3DXVECTOR3 Move, D3DXVECTOR3 Rot, D3DXCOLOR col)
{
	CUi* pUi = DBG_NEW CUi;   //弾を生成
	bool bSuccess = pUi->CObject::GetCreateSuccess();
	CTexture* pTextureClass = CManager::GetTexture();           //テクスチャクラスを取得
	if (bSuccess == true)
	{
		if (pUi != nullptr)
		{
			pUi->SetUiType(type);                  //背景の種類を設定
			pUi->SetUseLife(bUseLife, nLife, nLife);//体力設定
			pUi->Init();                                                     //初期化処理
			pUi->SetMove(Move);//移動量
			pUi->SetRot(Rot);//向き
			pUi->SetPos(Pos);//位置
			pUi->SetSupportPos(Pos);//支点となる位置
			pUi->SetUseScale(true);//拡大率を使用する
			pUi->CObject2D::SetAnimInfo(1, 1,true);//ポリゴンとテクスチャ情報を設定
			pUi->SetWidth(fWidth);
			pUi->SetMaxWidth(fWidth);
			pUi->SetHeight(fHeight);
			pUi->SetMaxHeight(fHeight);
			pUi->SetPolygonType(PolygonType);
			pUi->SetColor(col,false,1.0f);
			pUi->SetUseDeath(true);                 //死亡フラグを発動するかどうかを設定する
			pUi->SetTextureIndex(pTextureClass->Regist(UI_FILENAME[int(type)]));
			pUi->CObject2D::BindTexture(pTextureClass->GetAddress(pUi->GetTextureIndex()));
			pUi->CObject::SetType(CObject::TYPE::UI);//オブジェクの種類を決める
		}
	}
	else
	{
		delete pUi;
		pUi = nullptr;
		return nullptr;
	}

	return pUi;
}
//===================================================================================================

//====================================================
//生成処理
//====================================================
void CUi::SetUiType(UITYPE type)
{
	m_Type = type;
	CTexture* pTextureClass = CManager::GetTexture();           //テクスチャクラスを取得
    SetTextureIndex(pTextureClass->Regist(UI_FILENAME[int(type)]));
    CObject2D::BindTexture(pTextureClass->GetAddress(GetTextureIndex()));

}
//===================================================================================================

//====================================================
//数字状態に設定する
//====================================================
void CUi::SetNumericState(int nValue, float fWidth, float fHeight)
{
	for (auto it = m_VecUiState.begin(); it != m_VecUiState.end(); ++it)
	{
		if (CUiState::UISTATE::NUMERIC == (*it)->GetUiState())
		{
			return;//既に情報が存在しているので、保存せずに終了する
		}
	}
	m_VecUiState.push_back(DBG_NEW CUiState_Numeric(this, nValue, fWidth, fHeight));
}
//===================================================================================================

//====================================================
//UIの状態を取得する
//====================================================
CUiState* CUi::GetUiState(CUiState::UISTATE UiState)
{
	for (auto it = m_VecUiState.begin(); it != m_VecUiState.end(); ++it)
	{
		if (UiState == (*it)->GetUiState())
		{//指定した状態があれば取得する
			return *it;
		}
	}
	return nullptr;
}
//===================================================================================================

//=======================================<<<<<<<<UIエフェクトクラス>>>>>>>>=========================================================================


//====================================================
//コンストラクタ
//====================================================
CUiEffect::CUiEffect() : CUi(5)
{

}
//===================================================================================================

//====================================================
//デストラクタ
//====================================================
CUiEffect::~CUiEffect()
{
}
//===================================================================================================

//====================================================
//初期化処理
//====================================================
HRESULT CUiEffect::Init()
{
	CUi::Init();
	return S_OK;
}
//===================================================================================================

//====================================================
//終了処理
//====================================================
void CUiEffect::Uninit()
{
	CUi::Uninit();
}
//===================================================================================================

//====================================================
//更新処理
//====================================================
void CUiEffect::Update()
{
	CUi::Update();
}
//===================================================================================================

//====================================================
//描画処理
//====================================================
void CUiEffect::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	//aブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	CUi::Draw();
	//aブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}
//===================================================================================================

//====================================================
//死亡フラグ設定処理
//====================================================
void CUiEffect::SetDeath()
{
	CUi::SetDeath();
}
//===================================================================================================

//====================================================
//生成処理
//====================================================
CUiEffect* CUiEffect::Create(UITYPE type, CObject2D::POLYGONTYPE PolygonType, float fWidth, float fHeight, int nLife, D3DXVECTOR3 Pos, D3DXVECTOR3 Move, D3DXVECTOR3 Rot, D3DXCOLOR col)
{
	CUiEffect* pUiEffect = DBG_NEW CUiEffect;   //弾を生成
	bool bSuccess = pUiEffect->CObject::GetCreateSuccess();
	CTexture* pTextureClass = CManager::GetTexture();           //テクスチャクラスを取得
	if (bSuccess == true)
	{
		if (pUiEffect != nullptr)
		{
			pUiEffect->SetUiType(type);                  //背景の種類を設定
			pUiEffect->Init();                                                     //初期化処理
			pUiEffect->SetMove(Move);//移動量
			pUiEffect->SetRot(Rot);//向き
			pUiEffect->SetPos(Pos);//位置
			pUiEffect->SetUseLife(true, nLife, nLife);//体力を使用する
			pUiEffect->SetSupportPos(Pos);//支点となる位置
			pUiEffect->SetUseLifeRatioColor(true);//体力に応じて色合いを変える
			pUiEffect->SetUseScale(true);//拡大率を使用する
			pUiEffect->CObject2D::SetAnimInfo(1, 1,false);//ポリゴンとテクスチャ情報を設定
			pUiEffect->SetWidth(fWidth);
			pUiEffect->SetMaxWidth(fWidth);
			pUiEffect->SetHeight(fHeight);
			pUiEffect->SetMaxHeight(fHeight);
			pUiEffect->SetPolygonType(PolygonType);
			pUiEffect->SetColor(col, false, 1.0f);
			pUiEffect->SetUseDeath(true);                 //死亡フラグを発動するかどうかを設定する
			pUiEffect->SetTextureIndex(pTextureClass->Regist(UI_FILENAME[int(type)]));
			pUiEffect->CObject2D::BindTexture(pTextureClass->GetAddress(pUiEffect->GetTextureIndex()));
			pUiEffect->CObject::SetType(CObject::TYPE::UI);//オブジェクの種類を決める
		}
	}
	else
	{
		delete pUiEffect;
		pUiEffect = nullptr;
		return nullptr;
	}

	return pUiEffect;
}
//===================================================================================================

//****************************************************
//UIステートクラス
//****************************************************

//====================================================
//コンストラクタ
//====================================================
CUiState::CUiState() : m_pUiState()
{

}
//===================================================================================================

//====================================================
//デストラクタ
//====================================================
CUiState::~CUiState()
{

}
//===================================================================================================

//====================================================
//処理
//====================================================
void CUiState::Process(CUi* pUi)
{

}
//===================================================================================================

//****************************************************
//UIステート_数字
//****************************************************

//====================================================
//コンストラクタ
//====================================================
CUiState_Numeric::CUiState_Numeric(CUi* pUi, int nValue, float fWidth, float fHeight)
{
	SetUiState(CUiState::UISTATE::NUMERIC);
	m_nValue = nValue;
	m_fWidth = fWidth;//横幅の基準値
	m_fHeight = fHeight;//高さの基準値
	int nDigit = CCalculation::CalculationDigit(m_nValue);
	if (m_nValue == 0)
	{//桁数を０にするわけにはいかないので、桁数を１とする
		nDigit = 1;
	}
	for (int nCnt = 0; nCnt < nDigit; nCnt++)
	{
		CNumber* pNumber = CNumber::Create(pUi->GetPos(), fWidth, fHeight);
		int nNum = CCalculation::getDigit(m_nValue, nCnt);
		pNumber->SetAnim(nNum);//指定した桁の数値を取得する
		pNumber->SetUseDeath(false);//死亡フラグを発動させない
		m_VecNum.push_back(pNumber);//Vectorに保存
	}
}
//===================================================================================================

//====================================================
//デストラクタ
//====================================================
CUiState_Numeric::~CUiState_Numeric()
{
	for (auto it : m_VecNum)
	{
		if (it != nullptr)
		{
			it->SetUseDeath(true);
			it->SetDeath();
			it = nullptr;
		}
	}

	m_VecNum.clear();
	m_VecNum.shrink_to_fit();
}
//===================================================================================================

//====================================================
//処理
//====================================================
void CUiState_Numeric::Process(CUi* pUi)
{
	int nSize = m_VecNum.size();
	int nCnt = 0;
	for (auto it = m_VecNum.begin();it != m_VecNum.end();it++,nCnt++)
	{//数字を横に並べ続ける
		(*it)->SetPos(pUi->GetPos() + D3DXVECTOR3(pUi->GetWidth() / 2 + (*it)->GetWidth() / 2 + (*it)->GetWidth() * (nSize - 1) - ((*it)->GetWidth() * nCnt), 0.0f, 0.0f));
	}
}
//===================================================================================================

//====================================================
//数値を設定する
//====================================================
void CUiState_Numeric::SetValue(int nValue,CUi * pUi)
{
	//数値の動的配列の中身を全て破棄
	for (auto it = m_VecNum.begin(); it != m_VecNum.end(); ++it)
	{
		if ((*it) != nullptr)
		{
			(*it)->SetUseDeath(true);
			(*it)->SetDeath();
			(*it) = nullptr;
		}
	}

	m_VecNum.clear();//メモリの中身を初期化

	m_nValue = nValue;//数値を格納
	int nDigit = CCalculation::CalculationDigit(m_nValue);//桁数を計算

	if (m_nValue == 0)
	{//桁数を０にするわけにはいかないので、桁数を１とする
		nDigit = 1;
	}

	//桁数分数値の表示を生成し動的配列に格納する
	for (int nCnt = 0; nCnt < nDigit; nCnt++)
	{
		CNumber* pNumber = CNumber::Create(pUi->GetPos(),m_fWidth,m_fHeight);
		int nNum = CCalculation::getDigit(m_nValue, nCnt);//指定した桁の数値を取得する
		pNumber->SetAnim(nNum);//アニメーションパターン
		pNumber->SetUseDeath(false);//死亡フラグを発動させない
		m_VecNum.push_back(pNumber);//動的配列に保存
	}
}
//===================================================================================================

//****************************************************
//UIステート_ゲージ保持
//****************************************************

//====================================================
//コンストラクタ
//====================================================
CUiState_Gauge::CUiState_Gauge(D3DXVECTOR3 GaugePos, D3DXCOLOR Col, CObject2D::POLYGONTYPE PolygonType, CGauge::GAUGETYPE GaugeType, float fMaxWidth, float fMaxHeight, int nValue, int nMaxValue)
{
	SetUiState(CUiState::UISTATE::GAUGE);//UIの状態を設定
	m_pGauge = CGauge::Create(GaugeType, nMaxValue, fMaxWidth, fMaxHeight, GaugePos);//生成
	m_pGauge->SetPolygonType(PolygonType);//ポリゴンの種類（中心点）を設定
	m_pGauge->SetParam(nValue);//パラメータを設定
	m_pGauge->SetColor(Col,false, 1.0f);//色合い
}
//===================================================================================================

//====================================================
//デストラクタ
//====================================================
CUiState_Gauge::~CUiState_Gauge()
{
	if (m_pGauge != nullptr)
	{//死亡フラグを設定する
		m_pGauge->SetUseDeath(true);
		m_pGauge->SetDeath();
		m_pGauge = nullptr;
	}
}
//===================================================================================================

//====================================================
//処理
//====================================================
void CUiState_Gauge::Process(CUi* pUi)
{

}
//===================================================================================================
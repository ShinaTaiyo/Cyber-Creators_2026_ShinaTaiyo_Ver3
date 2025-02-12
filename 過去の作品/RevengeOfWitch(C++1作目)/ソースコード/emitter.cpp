//================================================================
//
//７月９日：エミッターを実装する[Emitter.cpp]
//Author:ShinaTaiyo
//
//================================================================

//==========================================
//インクルード
//==========================================
#include "main.h"
#include "Emitter.h"
#include "manager.h"
#include "renderer.h"
#include "effect.h"
#include "texture.h"
//================================================================

//==========================================
//静的メンバ初期化
//==========================================

//==========================================
//コンストラクタ
//==========================================
CEmitter::CEmitter() : CBillboard(4)
{

}
//================================================================

//==========================================
//デストラクタ
//==========================================
CEmitter::~CEmitter()
{

}
//================================================================

//==========================================
//初期化処理
//==========================================
HRESULT CEmitter::Init()
{
	CBillboard::Init();             //2Dオブジェクトの初期化
					
	//エミッター情報
	m_bUse = false;                //使用状態
	m_Rot = NULL_VECTOR3;          //向き
	m_AddRot = NULL_VECTOR3;       //加算回転量
	m_fLength = 0.0f;              //距離計算用

	//エフェクト情報
	m_fSetEffectWidth = 0.0f;      //設定するエフェクトの横幅
	m_fSetEffectHeight = 0.0f;     //設定するエフェクトの高さ
	m_nSetEffectLife = 0;          //設定するエフェクトの体力
	m_SetEffectColor = {};         //設定するエフェクトの色合い
	m_nEffectType = 0;             //設定するエフェクトの種類
	return S_OK;
}
//================================================================

//==========================================
//終了処理
//==========================================
void CEmitter::Uninit()
{
	CBillboard::Uninit();//ビルボードの終了処理
	//SetDeath();//死亡フラグ設定
}
//================================================================

//==========================================
//更新処理
//==========================================
void CEmitter::Update()
{
	//=======================
	//変数宣言
	//=======================
	D3DXVECTOR3 & Pos = CBillboard::GetPos();
	D3DXVECTOR3 SummonPos = Pos;//エフェクト召喚位置
	float & fWidth = CBillboard::GetWidth();
	float & fHeight = CBillboard::GetHeight();
	float fAlpha = 1.0f;//色のアルファ値を決める
	int& nLife = GetLife();//体力
	int& nMaxLife = GetMaxLife();//最大体力
	//===================================================

	if (m_bUseRotate == true)
	{//回転を使用するなら
		m_Rot += m_AddRot;
		SummonPos = Pos + D3DXVECTOR3(sinf(m_Rot.y) * m_fLength, Pos.y, cosf(m_Rot.y) * m_fLength);
	}

	if (GetUseMtxChild() == false)
	{//子供じゃない場合
		//エフェクトを生成
		CEffect::Create((CEffect::EFFECTTYPE)(m_nEffectType), m_nSetEffectLife, m_fSetEffectWidth, m_fSetEffectHeight, SummonPos, m_SetEffectColor);
	}
	else
	{//子どもの場合
		//エフェクトを生成
		CEffect::Create((CEffect::EFFECTTYPE)(m_nEffectType), m_nSetEffectLife, m_fSetEffectWidth, m_fSetEffectHeight, GetMtxTransPos(), m_SetEffectColor);
	}
	if (nLife <= 0)
	{//体力が０になったら・・・
		m_bUse = false;
	}

	SetSize(fWidth, fHeight);

	//========================================
	//ビルボードの更新処理
	//========================================
	CBillboard::Update();
	//===========================================================================================


	//=======================================
	//使用状態がオフになったら・・・
	//=======================================
	if (m_bUse == false)
	{
		//Uninit();
		SetDeath();
	}
	//===========================================================================================

}
//===========================================================================================

//======================================================
//描画処理
//======================================================
void CEmitter::Draw()
{
	CBillboard::Draw();
}
//===========================================================================================

//======================================================
//死亡フラグを設定
//======================================================
void CEmitter::SetDeath()
{
	CObject::SetDeath();
}
//===========================================================================================

//==========================================
//エフェクトの生成処理
//==========================================
CEmitter * CEmitter::Create(int nEffectType, int nLife, int nSetLife, float fLength, float fWidth, float fHeight, D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col)
{
	CEmitter* pEmitter = DBG_NEW CEmitter;   //エフェクトを生成
	CTexture* pTexture = CManager::GetTexture();
	bool bSuccess = pEmitter->CObject::GetCreateSuccess();
	if (bSuccess == true)
	{
		if (pEmitter != nullptr)
		{
		
			pEmitter->Init();                                                              //初期化処理
			pEmitter->m_nEffectType = nEffectType;                                         //設定するエフェクトの種類
			pEmitter->SetUseDeath(true);                                                   //死亡フラグを発動するかどうかを設定する
			pEmitter->SetLife(nLife);                                                      //体力
			pEmitter->SetMaxLife(nLife);                                                   //最大体力
			pEmitter->m_fLength = fLength;                                                 //距離
			pEmitter->SetUseDraw(true);                                                   //描画するかどうかを設定
			pEmitter->m_bUse = true;                                                       //使用状態
			pEmitter->SetMove(move);                                                       //移動量
			pEmitter->m_nSetEffectLife = nSetLife;                                         //設定するエフェクトの体力
			pEmitter->m_fSetEffectWidth = fWidth;                                          //設定するエフェクトの横幅
			pEmitter->m_fSetEffectHeight = fHeight;                                        //設定するエフェクトの高さ
			pEmitter->m_SetEffectColor = col;                                              //設定するエフェクトの色合い
			pEmitter->SetPos(pos);                                                         //オブジェクト２Ｄの位置を設定
			pEmitter->SetSupportPos(pos);                                                                 //召喚位置を設定
			pEmitter->SetColor(col);                                                       //色合いを設定
			pEmitter->CObject::SetType(CObject::TYPE_EMITTER);                             //オブジェクトの種類を決める
			pEmitter->SetAnimInfo(1, 1, col, false);                                       //アニメーション情報を設定
		}
	}
	else
	{//オブジェクトに空きがなかったので破棄する
		delete pEmitter;
		pEmitter = nullptr;
		return nullptr;
	}

	return pEmitter;

}
//================================================================
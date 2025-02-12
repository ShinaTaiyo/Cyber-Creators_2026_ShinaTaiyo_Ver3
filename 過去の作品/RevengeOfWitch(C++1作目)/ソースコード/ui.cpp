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
//===================================================================================================

//====================================================
//静的メンバ宣言
//====================================================
const char* CUi::m_apUI_FILENAME[CUi::UITYPE_MAX] =
{
	"data\\TEXTURE\\UI\\PlayerIllust_000.png",
	"data\\TEXTURE\\UI\\SlimeIllust_000.png",
	"data\\TEXTURE\\UI\\MagicCircle_000.png",
	"data\\TEXTURE\\UI\\TitleLogo_000.png",
	"data\\TEXTURE\\UI\\PressEnter_Logo.png",
	"data\\TEXTURE\\UI\\PressEnterOrStart_Logo.png",
	"data\\TEXTURE\\UI\\CharacterGauge_000.png",
	"data\\TEXTURE\\UI\\FaceUI_Normal_000.png",
	"data\\TEXTURE\\UI\\FaceUI_Damage_000.png",
	"data\\TEXTURE\\UI\\FaceUI_Glad_000.png",
	"data\\TEXTURE\\UI\\TextUi_BossBattle.png",
	"data\\TEXTURE\\UI\\CutIn_KingSlime.png",
	"data\\TEXTURE\\UI\\TextUi_KingSlime.png",
	"data\\TEXTURE\\UI\\CutIn_DeathEnaga.png",
	"data\\TEXTURE\\UI\\TextUi_DeathEnaga.png",
	"data\\TEXTURE\\UI\\CutIn_MrStrong.png",
	"data\\TEXTURE\\UI\\TextUi_MrStrong.png",
	"data\\TEXTURE\\UI\\CutIn_Riley.png",
	"data\\TEXTURE\\UI\\TextUi_Riley.png",
	"data\\TEXTURE\\UI\\Cutin_Draiya.png",
	"data\\TEXTURE\\UI\\TextUi_Draiya.png",
	"data\\TEXTURE\\UI\\CutIn_Gabriel.png",
	"data\\TEXTURE\\UI\\TextUi_Gabriel.png",
	"data\\TEXTURE\\UI\\TextUi_Easy.png",
	"data\\TEXTURE\\UI\\TextUi_Normal.png",
	"data\\TEXTURE\\UI\\TextUi_ChooseDifficulty.png",
	"data\\TEXTURE\\UI\\TextUi_LongPush.png",
	"data\\TEXTURE\\UI\\TextUi_BossRush.png",
};//テクスチャファイル名

//====================================================
//コンストラクタ
//====================================================
CUi::CUi(int nPriority) : CObject2D(nPriority),m_MoveType(UIMOVETYPE_NORMAL),m_Type(UITYPE_PLAYER2DILUST),m_bUseUiEffect(false),m_nSetUiEffectLife(0),m_SetUiEffectColor(NORMAL_COL)
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
		CUiEffect::Create(m_Type, GetPolygonType(), GetWidth(), GetHeight(), m_nSetUiEffectLife, GetPos(), NULL_VECTOR3, GetRot(), m_SetUiEffectColor);
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
	CObject2D::SetDeath();
}
//===================================================================================================

//====================================================
//生成処理
//====================================================
CUi* CUi::Create(UITYPE type, CObject2D::POLYGONTYPE PolygonType, float fWidth, float fHeight, D3DXVECTOR3 Pos, D3DXVECTOR3 Move, D3DXVECTOR3 Rot, D3DXCOLOR col)
{
	CUi* pUi = DBG_NEW CUi;   //弾を生成
	bool bSuccess = pUi->CObject::GetCreateSuccess();
	CTexture* pTextureClass = CManager::GetTexture();           //テクスチャクラスを取得
	if (bSuccess == true)
	{
		if (pUi != nullptr)
		{
			pUi->SetUiType(type);                  //背景の種類を設定
			pUi->Init();                                                     //初期化処理
			pUi->SetMove(Move);//移動量
			pUi->SetRot(Rot);//向き
			pUi->SetPos(Pos);//位置
			pUi->SetSupportPos(Pos);//支点となる位置
			pUi->SetUseScale(true);//拡大率を使用する
			pUi->CObject2D::SetInfo(1, 1, fWidth, fHeight, col, PolygonType, true);//ポリゴンとテクスチャ情報を設定
			pUi->SetUseDeath(false);                 //死亡フラグを発動するかどうかを設定する
			pUi->SetTextureIndex(pTextureClass->Regist(m_apUI_FILENAME[type]));
			pUi->CObject2D::BindTexture(pTextureClass->GetAddress(pUi->GetTextureIndex()));
			pUi->CObject::SetType(CObject::TYPE_UI);//オブジェクの種類を決める
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
    SetTextureIndex(pTextureClass->Regist(m_apUI_FILENAME[type]));
    CObject2D::BindTexture(pTextureClass->GetAddress(GetTextureIndex()));

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
			pUiEffect->CObject2D::SetInfo(1, 1, fWidth, fHeight, col, PolygonType, true);//ポリゴンとテクスチャ情報を設定
			pUiEffect->SetUseDeath(true);                 //死亡フラグを発動するかどうかを設定する
			pUiEffect->SetTextureIndex(pTextureClass->Regist(m_apUI_FILENAME[type]));
			pUiEffect->CObject2D::BindTexture(pTextureClass->GetAddress(pUiEffect->GetTextureIndex()));
			pUiEffect->CObject::SetType(CObject::TYPE_UI);//オブジェクの種類を決める
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
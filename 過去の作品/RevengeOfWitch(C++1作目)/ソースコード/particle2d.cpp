//==================================================================================================================
//
//９月１６日：ボス出現イベントを作る[particle2d.cpp]
//Author:ShinaTaiyo
//
//==================================================================================================================

//===============================================================
//インクルード
//===============================================================
#include "particle2d.h"
#include "texture.h"
#include "manager.h"
#include "renderer.h"
//==================================================================================================================

//===============================================================
//コンストラクタ
//===============================================================
CParticle2D::CParticle2D()
{

}
//==================================================================================================================

//===============================================================
//デストラクタ
//===============================================================
CParticle2D::~CParticle2D()
{
}

//===============================================================
//初期化処理
//===============================================================
HRESULT CParticle2D::Init()
{
	CObject2D::Init();
	return S_OK;
}
//==================================================================================================================

//===============================================================
//終了処理
//===============================================================
void CParticle2D::Uninit()
{
	CObject2D::Uninit();
}
//==================================================================================================================

//===============================================================
//更新処理
//===============================================================
void CParticle2D::Update()
{
	CObject2D::Update();

	float fRatioLife = GetRatioLife();
	D3DXVECTOR2& Scale = GetScale();

	Scale = D3DXVECTOR2(1.0f, 1.0f) * fRatioLife;

}
//==================================================================================================================

//===============================================================
//描画処理
//===============================================================
void CParticle2D::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	//aブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	CObject2D::Draw();
	//aブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}
//==================================================================================================================

//===============================================================
//死亡フラグ設定処理
//===============================================================
void CParticle2D::SetDeath()
{
	CObject::SetDeath();
}
//==================================================================================================================

//===============================================================
//生成処理
//===============================================================
CParticle2D* CParticle2D::Create(D3DXVECTOR3 pos, D3DXVECTOR3 Move, D3DXVECTOR3 Rot, CObject2D::POLYGONTYPE PolygonType, int nLife, float fWidth, float fHeight, D3DXCOLOR col)
{
	CParticle2D* pParticle2D = DBG_NEW CParticle2D;                           //弾を生成 
	bool bSuccess = pParticle2D->CObject::GetCreateSuccess();         //生成が成功したかどうかを取得する
	if (bSuccess == true)
	{//生成が成功したら
		if (pParticle2D != nullptr)
		{
			pParticle2D->Init();                                                                                 //初期化処理
			pParticle2D->SetUseDeath(true);                                                                      //死亡フラグを発動するかどうかを設定する
			pParticle2D->SetUseLife(true, nLife, nLife);                                                         //体力を使用する
			pParticle2D->SetUseLifeRatioColor(true);                                                             //体力の割合で色合いを変える
			pParticle2D->SetPos(pos);                                                                            //オブジェクト２Ｄの位置を設定
			pParticle2D->SetMove(Move);                                                                          //移動量を設定する
			pParticle2D->SetRot(Rot);                                                                            //向きを設定する
			pParticle2D->SetUseScale(true);                                                                      //拡大率を使用するかどうか
			pParticle2D->SetScale(D3DXVECTOR2(1.0f,1.0f));                                                       //拡大率を設定する
			pParticle2D->CObject::SetType(CObject::TYPE_PARTICLE2D);                                             //オブジェクトの種類を決める
			pParticle2D->SetTextureIndex(CManager::GetTexture()->Regist("data\\TEXTURE\\shadow000.jpg"));        //テクスチャを割り当てる
			pParticle2D->BindTexture(CManager::GetTexture()->GetAddress(pParticle2D->GetTextureIndex()));        //テクスチャを設定する
			pParticle2D->SetInfo(1, 1, fWidth, fHeight, col, PolygonType, false);                          //ポリゴン情報を設定する
		}
	}
	else
	{//オブジェクトに空きがなかったので破棄する
		delete pParticle2D;
		pParticle2D = nullptr;
	}

	return pParticle2D;
}
//==================================================================================================================
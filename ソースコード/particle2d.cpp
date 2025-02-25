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
CParticle2D::CParticle2D(int nPri, bool bUseintPri, CObject::TYPE type, CObject::OBJECTTYPE ObjType) : CObject2D(nPri,bUseintPri,type,ObjType)
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
	CObject2D::Init();//オブジェクト2Dの初期化
	return S_OK;
}
//==================================================================================================================

//===============================================================
//終了処理
//===============================================================
void CParticle2D::Uninit()
{
	CObject2D::Uninit();//オブジェクト2Dの終了処理
}
//==================================================================================================================

//===============================================================
//更新処理
//===============================================================
void CParticle2D::Update()
{
	CObject2D::Update();//オブジェクト2Dの更新処理

	float fRatioLife = GetRatioLife();//体力の割合
	D3DXVECTOR2& Scale = GetScale();  //拡大率

	Scale = D3DXVECTOR2(1.0f, 1.0f) * fRatioLife;//拡大率を体力の割合で設定

}
//==================================================================================================================

//===============================================================
//描画処理
//===============================================================
void CParticle2D::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();//デバイスへのポインタ

	//aブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	CObject2D::Draw();//オブジェクト2Dの描画処理

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
	CObject2D::SetDeath();//オブジェクト2Dの死亡フラグ設定処理
}
//==================================================================================================================

//===============================================================
//生成処理
//===============================================================
CParticle2D* CParticle2D::Create(D3DXVECTOR3 pos, D3DXVECTOR3 Move, D3DXVECTOR3 Rot, CObject2D::POLYGONTYPE PolygonType, int nLife, float fWidth, float fHeight, D3DXCOLOR col)
{
	CParticle2D* pParticle2D = DBG_NEW CParticle2D;                                                      //パーティクル2Dを生成 
	pParticle2D->Init();                                                                                 //初期化処理
	pParticle2D->SetUseDeath(true);                                                                      //死亡フラグを発動するかどうかを設定する
	pParticle2D->SetUseLife(true, nLife, nLife);                                                         //体力を使用する
	pParticle2D->SetUseLifeRatioColor(true);                                                             //体力の割合で色合いを変える
	pParticle2D->SetPos(pos);                                                                            //オブジェクト２Ｄの位置を設定
	pParticle2D->SetMove(Move);                                                                          //移動量を設定する
	pParticle2D->SetRot(Rot);                                                                            //向きを設定する
	pParticle2D->SetUseScale(true);                                                                      //拡大率を使用するかどうか
	pParticle2D->SetScale(D3DXVECTOR2(1.0f,1.0f));                                                       //拡大率を設定する
	pParticle2D->CObject::SetType(CObject::TYPE::PARTICLE2D);                                            //オブジェクトの種類を決める
	pParticle2D->SetTextureIndex(CManager::GetTexture()->Regist("data\\TEXTURE\\shadow000.jpg"));        //テクスチャを割り当てる
	pParticle2D->BindTexture(CManager::GetTexture()->GetAddress(pParticle2D->GetTextureIndex()));        //テクスチャを設定する
	pParticle2D->SetAnimInfo(1, 1,false);                                                                //アニメーション情報を設定する
	pParticle2D->SetWidth(fWidth);                                                                       //横幅を設定                                
	pParticle2D->SetMaxWidth(fWidth);		                                                             //最大横幅を設定
	pParticle2D->SetColor(col, false, 1.0f);                                                             //色合いを設定
	pParticle2D->SetHeight(fHeight);		                                                             //高さを設定
	pParticle2D->SetMaxHeight(fHeight);		                                                             //最大高さを設定
	pParticle2D->SetPolygonType(PolygonType);                                                            //ポリゴンの種類を設定
	return pParticle2D;
}
//==================================================================================================================
//==============================================================================
//
//６月２５日：ワープマーカーを実装する[number3d.cpp
//Author:ShinaTaiyo
//
//==============================================================================

//=========================
//インクルード
//=========================
#include "number3d.h"
#include "manager.h"
#include "enemy.h"
#include "effect.h"
#include "sound.h"
#include "texture.h"
//=================================================

//=========================
//静的メンバ
//=========================

//========================================================

//=========================
//コンストラクタ
//=========================
CNumber3D::CNumber3D()
{

}
//=================================================

//=========================
//デストラクタ
//=========================
CNumber3D::~CNumber3D()
{

}
//=================================================

//=========================
//初期化処理
//=========================
HRESULT CNumber3D::Init()
{
	//===========================
	//ビルボード初期化
	//===========================
	CBillboard::Init();

	//=========================================

	return S_OK;
}
//=================================================

//=========================
//終了処理
//=========================
void CNumber3D::Uninit()
{
	CBillboard::Uninit();
}
//=================================================

//=========================
//更新処理
//=========================
void CNumber3D::Update()
{
	//========================================
	//ビルボードの更新処理
	//========================================
	CBillboard::Update();
	//===========================================================================================
}
//=================================================

//=========================
//描画処理
//=========================
void CNumber3D::Draw()
{

}
//===========================================================================================

//=========================
//別枠の描画処理
//=========================
void CNumber3D::ExtraDraw()
{
	CBillboard::Draw();
}
//===========================================================================================

//=========================
//死亡フラグを設定
//========================
void CNumber3D::SetDeath()
{
	CObject::SetDeath();
}
//===========================================================================================

//=========================
//3D数字生成処理
//=========================
CNumber3D* CNumber3D::Create(float fWidth, float fHeight, D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col)
{
	CNumber3D* pNumber3D = DBG_NEW CNumber3D;                             //弾を生成
	CTexture* pTextureClass = CManager::GetTexture();           //テクスチャクラスを取得
	bool bSuccess = pNumber3D->CObject::GetCreateSuccess();       //生成が成功したかどうかを取得する
	if (bSuccess == true)
	{//生成が成功したら
		if (pNumber3D != nullptr)
		{
			pNumber3D->Init();  //初期化処理
			pNumber3D->SetUseDeath(false);                                                                     //死亡フラグを発動するかどうかを設定する
			pNumber3D->SetTextureIndex(pTextureClass->Regist("data\\TEXTURE\\Number.png"));
			pNumber3D->CBillboard::bindTexture(pTextureClass->GetAddress(pNumber3D->GetTextureIndex()));//テクスチャをセットする　
			pNumber3D->CBillboard::SetPos(pos);                                                         //オブジェクト２Ｄの位置を設定
			pNumber3D->SetSupportPos(pos);                                                                 //召喚位置を設定
			pNumber3D->CBillboard::SetSize(fWidth, fHeight);                                            //ポリゴンのサイズを設定
			pNumber3D->CObject::SetType(CObject::TYPE_NUMBER3D);                                        //オブジェクトの種類を決める
			pNumber3D->CBillboard::SetFormarSize(fWidth, fHeight);                                        //ポリゴンの元のサイズを設定
			pNumber3D->SetAnimInfo(10, 1,col, false);                                           //アニメーション情報を設定
			pNumber3D->SetMove(move);
		}
	}
	else
	{//オブジェクトに空きがなかったので破棄する
		delete pNumber3D;
		pNumber3D = nullptr;
		return nullptr;
	}
	return pNumber3D;
}
//=================================================
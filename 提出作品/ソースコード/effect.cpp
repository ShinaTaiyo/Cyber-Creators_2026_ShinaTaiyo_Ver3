//================================================================
//
//５月２７日：基盤づくり（エフェクトを実装する）[Effect.cpp]
//Author:ShinaTaiyo
//
//================================================================

//==========================================
//インクルード
//==========================================
#include "main.h"
#include "effect.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
//================================================================

//==========================================
//静的メンバ初期化
//==========================================
const string CEffect::s_EffectFileName[static_cast<int>(CEffect::EFFECTTYPE::MAX)] =
{
	"data\\TEXTURE\\Shadow000.jpg",
};//テクスチャファイルネーム
//=================================================================

//==========================================
//コンストラクタ
//==========================================
CEffect::CEffect(int nPri, bool bUseintPri, CObject::TYPE type, CObject::OBJECTTYPE ObjType) : CBillboard(nPri,bUseintPri,type,ObjType), m_Type(EFFECTTYPE::NORMAL),m_fReductionHeight(0.0f),m_fReductionWidth(0.0f)
{

}
//================================================================

//==========================================
//デストラクタ
//==========================================
CEffect::~CEffect()
{

}
//================================================================

//==========================================
//初期化処理
//==========================================
HRESULT CEffect::Init()
{
	CBillboard::Init();                                    //ビルボードの初期化処理
								                           
	m_fReductionWidth = 0.0f;                              //横幅の縮小スピード
	m_fReductionHeight = 0.0f;                             //高さの縮小スピード
	m_Type = EFFECTTYPE::NORMAL;                           //エフェクトの種類
	return S_OK;
}
//================================================================

//==========================================
//終了処理
//==========================================
void CEffect::Uninit()
{
	CBillboard::Uninit();//ビルボードの終了処理
}
//================================================================

//==========================================
//更新処理
//==========================================
void CEffect::Update()
{
	//=======================
    //変数宣言
    //=======================
	D3DXVECTOR3 Pos = CBillboard::GetPos();                               //位置
	float fWidth = CBillboard::GetWidth();                                //横幅
	float fHeight = CBillboard::GetHeight();                              //高さ
	float fAlpha = 1.0f;                                                  //色のアルファ値を決める
	int& nLife = GetLife();                                               //体力
	int& nMaxLife = GetMaxLife();                                         //最大体力
	//===================================================
	
	fAlpha = (float)(nLife) / (float)(nMaxLife);                          //色のアルファ値を最大体力との割合で決める
	fWidth -= m_fReductionWidth;                                          //横幅を減らす
	fHeight -= m_fReductionHeight;                                        //高さを減らす
	SetPos(Pos);                                                          //位置を設定
	SetSize(fWidth, fHeight);                                             //ポリゴンサイズを設定
	SetColor(D3DXCOLOR(GetColor().r, GetColor().g, GetColor().b, fAlpha));//アルファ値

	CBillboard::Update();                                                 //ビルボードの更新処理
	if (nLife == 0)
	{//体力が０になったら
		SetDeath();//死亡フラグを設定する
	}
}
//================================================================

//==========================================
//描画処理
//==========================================
void CEffect::Draw()
{
	//レンダラークラスの情報を取得
	CRenderer* pRenderer = CManager::GetRenderer();
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//================================================
    //描画の調整
    //================================================
    //ライトを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//Zの比較方法変更。（モデルがあっても隠れなくなる）
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

	//Zバッファに書き込まない（重なり方に違和感がなくなる）
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	//アルファテストを有効(アルファ値が０より大きい場合に描画する）
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	//=======================================================================================================================================

	//aブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	CBillboard::Draw();

	//aブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//================================================
    //描画の調整を元に戻す
    //================================================
    
    //ライトを有効に戻す
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	////Zの比較方法変更。
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	//Zバッファに書き込む
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	//アルファテスト無効に戻す
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	//=======================================================================================================================================
}
//=======================================================================================================================================

//=========================================================================
//死亡フラグを設定
//=========================================================================
void CEffect::SetDeath()
{
	CBillboard::SetDeath();
}
//=======================================================================================================================================

//==========================================
//エフェクトの生成処理
//==========================================
CEffect* CEffect::Create(EFFECTTYPE type, int nLife, float fWidth, float fHeight, D3DXVECTOR3 pos, D3DXCOLOR col)
{
	CEffect* pEffect = DBG_NEW CEffect;   //エフェクトを生成
	CTexture* pTexture = CManager::GetTexture();
	if (pEffect != nullptr)
	{
		pEffect->Init();                                                              //初期化処理
		pEffect->SetUseDeath(true);                                                   //死亡フラグを発動するかどうかを設定する
		pEffect->CBillboard::SetFormarSize(fWidth, fHeight);                          //ポリゴンの元のサイズを設定
		pEffect->m_Type = type;                                                       //エフェクトの種類
		pEffect->SetLife(nLife);                                                      //体力
		pEffect->SetMaxLife(nLife);                                                   //最大体力
		pEffect->m_fReductionWidth = fWidth / (float)(nLife);                         //横幅縮小スピード
		pEffect->m_fReductionHeight = fHeight / (float)(nLife);                       //高さ縮小スピード
		pEffect->SetTextureIndex(pTexture->Regist(s_EffectFileName[static_cast<int>(type)]));  //テクスチャ割り当てとテクスチャ番号の設定
		pEffect->bindTexture(pTexture->GetAddress(pEffect->GetTextureIndex()));       //テクスチャをセットする　
		pEffect->SetPos(pos);                                                         //オブジェクト２Ｄの位置を設定[
		pEffect->SetSupportPos(pos);                                                                 //召喚位置を設定
		pEffect->SetSize(fWidth, fHeight);                                            //サイズを設定する
		pEffect->SetColor(col);                                                       //色合いを設定
		pEffect->SetAnimInfo(1, 1, col, false);                                       //アニメーション情報を設定
	}

	return pEffect;
}
//=======================================================================================================================================
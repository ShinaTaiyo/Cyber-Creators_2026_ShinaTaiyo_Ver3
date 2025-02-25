//================================================================
//
//６月２５日：ワープマーカーを実装する[particle.cpp]
//Author:ShinaTaiyo
//
//================================================================

//==========================================
//インクルード
//==========================================
#include "main.h"
#include "particle.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
//================================================================

//==========================================
//静的メンバ宣言
//==========================================
const char* CParticle::m_apPARTICLE_FILENAME[CParticle::TYPE_MAX] =
{
	"data\\TEXTURE\\Shadow000.jpg",
	"data\\TEXTURE\\Particle\\FireParticle_000.png",
	"data\\TEXTURE\\Particle\\IceParticle_000.png",
	"data\\TEXTURE\\Particle\\ThunderParticle_000.png",
	"data\\TEXTURE\\Particle\\HealParticle_000.png",
	"data\\TEXTURE\\Particle\\Partile_Wing.png",
};

//==========================================
//コンストラクタ
//==========================================
CParticle::CParticle(int nPri, bool bUseintPri, CObject::TYPE type, CObject::OBJECTTYPE ObjType) : CBillboard(nPri,bUseintPri,type,ObjType), m_bBranding(true), m_Type(TYPE00_NORMAL), m_fReductionWidth(0.0f),m_fReductionHeight(0.0f), m_bAddSpeed(false),m_fAddSpeed(0.0f),
m_bUseChargePartilce(false), m_bGravity(false), m_fGravity(0.0f),m_SupportPos(D3DXVECTOR3(0.0f,0.0f,0.0f))
{

}
//================================================================

//==========================================
//デストラクタ
//==========================================
CParticle::~CParticle()
{

}
//================================================================

//==========================================
//初期化処理
//==========================================
HRESULT CParticle::Init()
{
	CBillboard::Init();               //ビルボードの初期化
								      
	m_fReductionWidth = 0.0f;         //横幅の縮小スピード
	m_fReductionHeight = 0.0f;        //高さの縮小スピード
	m_bBranding = false;              //アルファブレンディングをするかどうか
	m_Type = CParticle::TYPE00_NORMAL;//パーティクルの種類
	m_fGravity = 0.0f;                //重力の強さ
	m_fAddSpeed = 0.0f;               //加速度
	return S_OK;
}
//================================================================

//==========================================
//終了処理
//==========================================
void CParticle::Uninit()
{
	CBillboard::Uninit();//ビルボードの終了処理
}
//================================================================

//==========================================
//更新処理
//==========================================
void CParticle::Update()
{
	//=======================
	//変数宣言
	//=======================
	D3DXVECTOR3& Pos = CBillboard::GetPos();//位置
	D3DXVECTOR3& Move = GetMove();          //移動量
	float& fWidth = GetWidth();             //横幅
	float& fHeight = GetHeight();           //高さ
	D3DXCOLOR col = GetColor();             //色合い
	int& nLife = GetLife();                 //体力
	int& nMaxLife = GetMaxLife();           //最大体力
	float fAlpha = 0.0f;                    //色の透明度
	//===========================================================================================

	//=======================================
	//更新処理
	//=======================================

	//加速がONだったら
	if (m_bAddSpeed == true)
	{
		Move *= m_fAddSpeed;
	}

	if (m_bGravity == true)
	{//重力を使用するなら
		Move.y += m_fGravity;
	}

	fAlpha = (float)(nLife) / (float)(nMaxLife);       //色の透明度を体力の割合で決定
	fWidth -= m_fReductionWidth;                       //横幅を減らす
	fHeight -= m_fReductionHeight;                     //高さを減らす
	SetColor(D3DXCOLOR(col.r, col.g, col.b, fAlpha));  //色合いの設定
	//===========================================================================================

	//========================================
	//ビルボードの更新処理
	//========================================
	CBillboard::Update();
	//===========================================================================================

	//========================================
	//チャージする挙動を使用するかどうか
	//========================================
	if (m_bUseChargePartilce == true)
	{
		if (Pos.x >= m_SupportPos.x - 20.0f && Pos.x <= m_SupportPos.x + 20.0f &&
			Pos.y >= m_SupportPos.y - 20.0f && Pos.y <= m_SupportPos.y + 20.0f &&
			Pos.z >= m_SupportPos.z - 20.0f && Pos.z <= m_SupportPos.z + 20.0f)
		{
			SetDeath();//目的地に達したら死亡フラグを設定
		}
	}
	//===========================================================================================

	//=======================================
	//体力が０になったら
	//=======================================
	if (nLife < 1)
	{
		SetDeath();//死亡フラグを設定する
	}
	//===========================================================================================

}
//================================================================

//==========================================
//描画処理
//==========================================
void CParticle::Draw()
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

	//Zバッファに書き込む
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	//アルファテスト無効に戻す
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	//=======================================================================================================================================
}
//================================================================

//============================================
//死亡フラグを設定
//============================================
void CParticle::SetDeath()
{
	CBillboard::SetDeath();//ビルボードの死亡フラグ設定処理
}
//================================================================

//============================================
//加速パーティクルにする
//============================================
void CParticle::SetAddSpeed(float fAddSpeed)
{
	m_bAddSpeed = true;     //加速状態にする
	m_fAddSpeed = fAddSpeed;//加速量を設定する
}
//================================================================

//============================================
//重力パーティクルにする
//============================================
void CParticle::SetGravity(float fGravity)
{
	m_bGravity = true;      //重力をONにする
	m_fGravity = fGravity;  //重力を設定
}
//================================================================

//==========================================
//パーティクルの生成処理
//==========================================
CParticle* CParticle::Create(TYPE Type, int nLife, float fWidth, float fHeight, D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, bool bBrending)
{
	CParticle* pParticle = DBG_NEW CParticle;                                    //エフェクトを生成
	CTexture* pTexture = CManager::GetTexture();                                 //テクスチャ情報を取得
	pParticle->Init();                                                           //初期化処理
	pParticle->SetUseDeath(true);                                                //死亡フラグを発動するかどうかを設定する
	pParticle->SetLife(nLife);                                                   //エフェクトの体力
	pParticle->SetMaxLife(nLife);                                                //最大体力
	pParticle->m_bBranding = bBrending;                                          //アルファブレンディングをするかどうか
	pParticle->m_fReductionWidth = fWidth / (float)(nLife);                      //横幅縮小スピード
	pParticle->m_fReductionHeight = fHeight / (float)(nLife);                    //高さ縮小スピード
	pParticle->SetMove(move);                                                    //移動量の設定
	pParticle->m_Type = Type;                                                    //種類
	pParticle->SetTextureIndex(pTexture->Regist(m_apPARTICLE_FILENAME[Type]));   //テクスチャ割り当てとテクスチャ番号の設定
	pParticle->bindTexture(pTexture->GetAddress(pParticle->GetTextureIndex()));  //テクスチャをセットする　
	pParticle->SetPos(pos);                                                      //オブジェクト２Ｄの位置を設定
	pParticle->SetSupportPos(pos);                                               //召喚位置を設定
	pParticle->SetSize(fWidth, fHeight);                                         //サイズを設定する
	pParticle->SetColor(col);                                                    //色合いを設定
	pParticle->CObject::SetType(CObject::TYPE::PARTICLE);                        //オブジェクトの種類を決める
	pParticle->SetAnimInfo(1, 1, col, false);                                    //アニメーション情報を設定
	return pParticle;
}
//==========================================================================================================================================

//==================================================================
//ランダム方向に射出するパーティクル召喚処理
//==================================================================
void CParticle::SummonParticle(TYPE type, int nNum, int nLife, float fWidth, float fHeight, int nRandSpeed, int nDivision, bool bRandumCol, D3DXVECTOR3 Pos, D3DXCOLOR col, bool bBreanding)
{
	D3DXVECTOR3 Speed = D3DXVECTOR3(0.0f,0.0f,0.0f);//速さ
	D3DXCOLOR RandCol = {};                         //色合い
	for (int nCnt = 0; nCnt < nNum; nCnt++)
	{
		//速度をランダムに設定
		Speed.x = float(rand() % nRandSpeed - (nRandSpeed / 2)) / nDivision;
		Speed.y = float(rand() % nRandSpeed - (nRandSpeed / 2)) / nDivision;
		Speed.z = float(rand() % nRandSpeed - (nRandSpeed / 2)) / nDivision;

		if (bRandumCol == false)
		{//ランダムな色合いがオフだったら
			CParticle::Create(type, nLife, fWidth, fHeight, Pos, Speed, col, bBreanding);
		}
		else
		{//それ以外
			RandCol.r = float(rand() % 100 + 1) / 100;
			RandCol.g = float(rand() % 100 + 1) / 100;
			RandCol.b = float(rand() % 100 + 1) / 100;
			CParticle::Create(type, nLife, fWidth, fHeight, Pos, Speed, RandCol, bBreanding);
		}
	}
}
//==========================================================================================================================================

//============================================================================================================
//チャージするような挙動のパーティクルを出す
//============================================================================================================
void CParticle::SummonChargeParticle(TYPE type, int nNum, int nLife, float fInitialSpeed, float fWidth, float fHeight,
	float fSummonLength, int nRandSpeed, int nDivision, bool bRandumCol, D3DXVECTOR3 SupportPos, D3DXCOLOR col, bool bBreanding)
{
	float fVXaim = 0.0f;                                 //X方向のベクトル
	float fVYaim = 0.0f;                                 //Y方向のベクトル
	float fVZaim = 0.0f;                                 //Z方向のベクトル
	float fVLaim = 0.0f;                                 //総合ベクトル
	float fSpeed = 0.0f;                                 //速さ
	D3DXVECTOR3 Speed = D3DXVECTOR3(0.0f,0.0f,0.0f);     //速さ
	D3DXVECTOR3 SummonPos = D3DXVECTOR3(0.0f,0.0f,0.0f); //召喚位置
	CParticle* pParticle = nullptr;                      //パーティクルへのポインタ
	int nSummonLength = static_cast<int>(fSummonLength);
	for (int nCnt = 0; nCnt < nNum; nCnt++)
	{
		fSpeed = float(rand() % nRandSpeed) / nDivision + fInitialSpeed;//速度を設定

		//召喚位置を設定
		SummonPos.x = SupportPos.x + float(rand() % nSummonLength - (nSummonLength / 2));
		SummonPos.y = SupportPos.y + float(rand() % nSummonLength - (nSummonLength / 2));
		SummonPos.z = SupportPos.z + float(rand() % nSummonLength - (nSummonLength / 2));
		//==========================
		//３次元ベクトルを取る
		//==========================
		fVXaim = SupportPos.x - SummonPos.x;
		fVYaim = SupportPos.y - SummonPos.y;
		fVZaim = SupportPos.z - SummonPos.z;

		fVLaim = sqrtf((fVXaim * fVXaim) + (fVYaim * fVYaim)
			+ (fVZaim * fVZaim));

		Speed.x = fVXaim / fVLaim * fSpeed;
		Speed.y = fVYaim / fVLaim * fSpeed;
		Speed.z = fVZaim / fVLaim * fSpeed;
		//===========================================================

		if (bRandumCol == true)
		{//色合いがランダムなら
			col.r = float(rand() % 100) / 100;
			col.g = float(rand() % 100) / 100;
			col.b = float(rand() % 100) / 100;
		}

	    pParticle = CParticle::Create(type, nLife, fWidth, fHeight, SummonPos, Speed, col, bBreanding);//パーティクルを生成
		pParticle->m_SupportPos = SupportPos;//目的地を設定
		pParticle->m_bUseChargePartilce = true;//チャージする挙動をする
	}
}
//==========================================================================================================================================
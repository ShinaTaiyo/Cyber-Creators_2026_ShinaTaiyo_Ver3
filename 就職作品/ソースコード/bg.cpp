//=========================================
//
//１０月１８日：新しいGitProject作成[bg.cpp]
//Author:ShinaTaiyo
//
//=========================================

//========================
//インクルード
//========================
#include "bg.h"
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "object2d.h"
#include "object.h"
#include "texture.h"
//=========================================================================

//========================
//静的メンバ
//========================
const std::string CBg::m_apBGFILE_NAME[CBg::BG_MAX] =
{
	"data\\TEXTURE\\UI\\Title_000.png",
	"data\\TEXTURE\\Bg\\Wall_000.jpg",
	"data\\TEXTURE\\Bg\\Result_BG.png"
};//背景のファイル名

//========================
//コンストラクタ
//========================
CBg::CBg(int nPri, bool bUseintPri, CObject::TYPE type, CObject::OBJECTTYPE ObjType) : CObject2D(nPri,bUseintPri,type,ObjType), m_bUseTexMove(false),m_PosTex(D3DXVECTOR2(0.0f,0.0f)), m_MoveTex(D3DXVECTOR2(0.0f,0.0f)),m_Type(BG_TITLE),m_fPosTexU(0.0f),m_fRotationPower(0.0f)
{

}
//=========================================================================

//========================
//デストラクタ
//========================
CBg::~CBg()
{

}
//=========================================================================

//========================
//初期化処理
//========================
HRESULT CBg::Init()
{
	CObject2D::Init();//2Dオブジェクトの初期化
	return S_OK;
}
//=========================================================================

//========================
//終了処理
//========================
void CBg::Uninit()
{
	CObject2D::Uninit();//2Dオブジェクトの終了処理
}
//=========================================================================

//========================
//更新処理
//========================
void CBg::Update()
{
	CObject2D::Update();//オブジェクト2Dの更新処理

	if (m_bUseTexMove == true)
	{//テクスチャ座標を移動させるかどうか
		LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVertexBuffer();
		VERTEX_2D* pVtx;//2D頂点情報へのポインタ
		//頂点バッファをロックし、頂点データへのポインタを取得
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f) + m_PosTex;
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f) + m_PosTex;
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f) + m_PosTex;
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f) + m_PosTex;

		//テクスチャ座標を移動させる
		m_PosTex += m_MoveTex;

		//頂点バッファをアンロックする
		pVtxBuff->Unlock();
	}
}
//=========================================================================

//========================
//描画処理
//========================
void CBg::Draw()
{
	CObject2D::Draw();//オブジェクト2Dの描画処理
}
//=========================================================================

//========================
//死亡フラグを設定
//========================
void CBg::SetDeath()
{
	CObject2D::SetDeath();//オブジェクト2Dの死亡フラグ設定処理
}
//=========================================================================

//=============================
//背景の生成処理
//=============================
void CBg::Create(BG type)
{
	CBg* pBg = DBG_NEW CBg;                                     //背景を生成
	CTexture* pTextureClass = CManager::GetTexture();           //テクスチャクラスを取得
	if (pBg != nullptr)
	{//背景が存在していたら
		pBg->Init();                                                                   //初期化処理a
		pBg->CObject2D::SetAnimInfo(1, 1, false);                                      //ポリゴンとテクスチャ情報を設定
		pBg->SetColor(D3DXCOLOR(1.0f,1.0f, 1.0f, 1.0f), false, 1.0f);                  //色合いを設定
		pBg->SetWidth(SCREEN_WIDTH / 2);                                               //ポリゴンの横幅を設定
		pBg->SetMaxWidth(SCREEN_WIDTH / 2);                                            //ポリゴンの最大横幅を設定
		pBg->SetHeight(SCREEN_HEIGHT / 2);                                             //ポリゴンの高さを設定
		pBg->SetMaxHeight(SCREEN_HEIGHT / 2);                                          //ポリゴンの最大高さを設定
		pBg->SetPolygonType(CObject2D::POLYGONTYPE::NORMAL);                           //ポリゴンの種類を設定
		pBg->SetTextureIndex(pTextureClass->Regist(m_apBGFILE_NAME[type]));            //テクスチャ番号の設定
		pBg->SetUseDeath(true);                                                        //死亡フラグを発動するかどうかを設定する
		pBg->CObject2D::BindTexture(pTextureClass->GetAddress(pBg->GetTextureIndex()));//テクスチャ情報の設定
		pBg->CObject2D::SetPos(D3DXVECTOR3(SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2,0.0f));  //ポリゴンとテクスチャ情報を設定
		pBg->m_Type = type;                                                            //背景の種類
		pBg->CObject::SetType(CObject::TYPE::BG);                                      //オブジェクトの種類を決める
	}
}
//=========================================================================

//======================================================<<<<<<<<タイトル背景クラス>>>>>>>>>>=========================================

//=====================================================
//コンストラクタ
//=====================================================
CBgTitle::CBgTitle()
{

}
//=========================================================================

//=====================================================
//デストラクタ
//=====================================================
CBgTitle::~CBgTitle()
{

}
//=========================================================================

//=====================================================
//初期化処理
//=====================================================
HRESULT CBgTitle::Init()
{
	CBg::Init();//背景の初期化
	return S_OK;
}
//=========================================================================

//=====================================================
//終了処理
//=====================================================
void CBgTitle::Uninit()
{
	CBg::Uninit();//背景の終了処理
}
//=========================================================================

//=====================================================
//更新処理
//=====================================================
void CBgTitle::Update()
{
	CBg::Update();//背景の更新処理
}
//=========================================================================

//=====================================================
//描画処理
//=====================================================
void CBgTitle::Draw()
{
	CBg::Draw();//背景の描画処理
}
//=========================================================================

//=====================================================
//死亡フラグ設定処理
//=====================================================
void CBgTitle::SetDeath()
{
	CBg::SetDeath();//背景の死亡フラグ設定処理
}
//=========================================================================

//=====================================================
//生成処理
//=====================================================
CBgTitle* CBgTitle::Create(BG type)
{
	CBgTitle* pBgTitle = DBG_NEW CBgTitle;                                                   //タイトル背景を生成
	CTexture* pTextureClass = CManager::GetTexture();                                        //テクスチャクラスを取得
	pBgTitle->Init();                                                                        //初期化処理
	pBgTitle->SetUseScale(true);                                                             //拡大率を使用する
	pBgTitle->CObject2D::SetAnimInfo(1, 1,false);                                            //ポリゴンとテクスチャ情報を設定
	pBgTitle->SetWidth(SCREEN_WIDTH / 2);                                                    //横幅を設定
	pBgTitle->SetMaxWidth(SCREEN_WIDTH / 2);                                                 //最大横幅を設定
	pBgTitle->SetHeight(SCREEN_HEIGHT / 2);                                                  //高さを設定
	pBgTitle->SetMaxHeight(SCREEN_HEIGHT / 2);                                               //最大高さを設定
	pBgTitle->SetPolygonType(CObject2D::POLYGONTYPE::NORMAL);                                //ポリゴンの種類を設定
	pBgTitle->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), false, 1.0f);                      //色合いを設定
	pBgTitle->SetTextureIndex(pTextureClass->Regist(m_apBGFILE_NAME[type]));                 //テクスチャ番号の設定
	pBgTitle->SetUseDeath(true);                                                             //死亡フラグを発動するかどうかを設定する
	pBgTitle->CObject2D::BindTexture(pTextureClass->GetAddress(pBgTitle->GetTextureIndex()));//テクスチャ情報の設定
	pBgTitle->CObject2D::SetPos(D3DXVECTOR3(SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2,0.0f));       //ポリゴンとテクスチャ情報を設定
	pBgTitle->SetBgType(type);                                                               //背景の種類を設定
	pBgTitle->CObject::SetType(CObject::TYPE::BG);                                           //オブジェクの種類を決める


	return pBgTitle;
}
//=========================================================================

//======================================================<<<<<<<<難易度選択背景クラス>>>>>>>>>>=========================================

//=====================================================
//コンストラクタ
//=====================================================
CBgDifficulty::CBgDifficulty()
{

}
//=========================================================================

//=====================================================
//デストラクタ
//=====================================================
CBgDifficulty::~CBgDifficulty()
{

}
//=========================================================================

//=====================================================
//初期化処理
//=====================================================
HRESULT CBgDifficulty::Init()
{
	CBg::Init();//背景の初期化処理
	return S_OK;
}
//=========================================================================

//=====================================================
//終了処理
//=====================================================
void CBgDifficulty::Uninit()
{
	CBg::Uninit();//背景の終了処理
}
//=========================================================================

//=====================================================
//更新処理
//=====================================================
void CBgDifficulty::Update()
{
	CBg::Update();//背景の更新処理
}
//=========================================================================

//=====================================================
//描画処理
//=====================================================
void CBgDifficulty::Draw()
{
	CBg::Draw();//背景の描画処理
}
//=========================================================================

//=====================================================
//死亡フラグ設定処理
//=====================================================
void CBgDifficulty::SetDeath()
{
	CBg::SetDeath();//背景の死亡フラグ設定処理
}
//=========================================================================

//=====================================================
//生成処理
//=====================================================
CBgDifficulty* CBgDifficulty::Create(BG type)
{
	CBgDifficulty* pBgDifficulty = DBG_NEW CBgDifficulty;                                                    //難易度選択の背景を生成
	CTexture* pTextureClass = CManager::GetTexture();                                                        //テクスチャクラスを取得
	if (pBgDifficulty != nullptr)								                                             
	{//難易度選択背景が存在したら							                                                 
		pBgDifficulty->Init();                                                                               //初期化処理
		pBgDifficulty->SetUseScale(true);                                                                    //拡大率を使用する
		pBgDifficulty->CObject2D::SetAnimInfo(1, 1, false);                                                  //ポリゴンとテクスチャ情報を設定
		pBgDifficulty->SetWidth(SCREEN_WIDTH);                                                               //横幅を設定
		pBgDifficulty->SetMaxWidth(SCREEN_WIDTH);                                                            //最大横幅を設定
		pBgDifficulty->SetHeight(SCREEN_HEIGHT);                                                             //高さを設定
		pBgDifficulty->SetMaxHeight(SCREEN_HEIGHT);                                                          //最大高さを設定
		pBgDifficulty->SetPolygonType(CObject2D::POLYGONTYPE::SENTERROLLING);                                //ポリゴンの種類を設定

		pBgDifficulty->SetTextureIndex(pTextureClass->Regist(m_apBGFILE_NAME[type]));                        //テクスチャ番号を設定
		pBgDifficulty->SetUseDeath(true);                                                                    //死亡フラグを発動させるかどうかを設定
		pBgDifficulty->CObject2D::BindTexture(pTextureClass->GetAddress(pBgDifficulty->GetTextureIndex()));  //テクスチャ情報を設定
		pBgDifficulty->CObject2D::SetPos(D3DXVECTOR3(SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2,0.0f));              //ポリゴンとテクスチャ情報を設定
		pBgDifficulty->SetBgType(type);                                                                      //背景の種類を設定
		pBgDifficulty->CObject::SetType(CObject::TYPE::BG);                                                  //オブジェクトの種類を決める
	}
	return pBgDifficulty;
}
//=========================================================================
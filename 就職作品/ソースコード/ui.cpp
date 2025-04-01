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
const string CUi::UI_FILENAME[static_cast<int>(CUi::UITYPE::MAX)] =
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
	"data\\TEXTURE\\UI\\DiveGaugeFrame_000.png",
	"data\\TEXTURE\\UI\\GameClear_000.png",
	"data\\TEXTURE\\UI\\Lose_000.png",
	"data\\TEXTURE\\UI\\Combo_000.png",
	"data\\TEXTURE\\UI\\PauseText_000.png",
	"data\\TEXTURE\\UI\\ExitText_000.png",
	"data\\TEXTURE\\UI\\ContinueText_000.png",
	"data\\TEXTURE\\UI\\CameraSensitivityText_000.png",
	"data\\TEXTURE\\UI\\ReturnText_000.png",
};//テクスチャファイル名

//====================================================
//コンストラクタ
//====================================================
CUi::CUi(int nPri, bool bUseintPri, CObject::TYPE type, CObject::OBJECTTYPE ObjType) : CObject2D(nPri,bUseintPri,type,ObjType),
m_MoveType(UIMOVETYPE_NORMAL),m_Type(UITYPE::LOCKON),m_bUseUiEffect(false),m_nSetUiEffectLife(0),m_SetUiEffectColor(D3DXCOLOR(1.0f,1.0f,1.0f,1.0f)),m_pUiCompositeContainer(DBG_NEW CUIComposite_Container(this))
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
	CObject2D::Init();//オブジェクト2D初期化処理
	return S_OK;
}
//===================================================================================================

//====================================================
//終了処理
//====================================================
void CUi::Uninit()
{
	CObject2D::Uninit();//オブジェクト2D終了処理

	if (m_pUiCompositeContainer != nullptr)
	{//UIの機能を破棄
		delete m_pUiCompositeContainer;
		m_pUiCompositeContainer = nullptr;
	}
}
//===================================================================================================

//====================================================
//更新処理
//====================================================
void CUi::Update()
{
	CObject2D::Update();//オブジェクト2D更新処理

	m_pUiCompositeContainer->Update(this);//機能の更新処理
}
//===================================================================================================

//====================================================
//描画処理
//====================================================
void CUi::Draw()
{
	CObject2D::Draw();//オブジェクト2D描画処理

	m_pUiCompositeContainer->Update(this);//機能の描画処理
}
//===================================================================================================

//====================================================
//死亡フラグ設定処理
//====================================================
void CUi::SetDeath()
{
	CObject2D::SetDeath();//オブジェクト死亡フラグ設定処理
}
//===================================================================================================

//====================================================
//生成処理
//====================================================
CUi* CUi::Create(UITYPE type, bool bPauseUi, CObject2D::POLYGONTYPE PolygonType, float fWidth, float fHeight, int nLife, bool bUseLife, D3DXVECTOR3 Pos, D3DXVECTOR3 Move, D3DXVECTOR3 Rot, D3DXCOLOR col)
{
	CUi* pUi = nullptr;                                                              //UIへのポインタを初期化
	CTexture* pTextureClass = CManager::GetTexture();                                //テクスチャ情報を取得
	if (bPauseUi)
	{//ポーズ用のUIにする
		pUi = DBG_NEW CUi(0, false, TYPE::UI_PAUSE);
	}
	else
	{//普通のUIにする
		pUi = DBG_NEW CUi();
	}
	pUi->SetUiType(type);                                                            //UIの種類を設定
	pUi->SetUseLife(bUseLife, nLife, nLife);                                         //体力設定
	pUi->Init();                                                                     //初期化処理
	pUi->SetMove(Move);                                                              //移動量
	pUi->SetRot(Rot);                                                                //向き
	pUi->SetPos(Pos);                                                                //位置
	pUi->SetSupportPos(Pos);                                                         //支点となる位置
	pUi->SetUseScale(true);                                                          //拡大率を使用する
	pUi->CObject2D::SetAnimInfo(1, 1,true);                                          //ポリゴンとテクスチャ情報を設定
	pUi->SetWidth(fWidth);                                                           //横幅を設定
	pUi->SetMaxWidth(fWidth);                                                        //最大横幅を設定
	pUi->SetHeight(fHeight);                                                         //高さを設定
	pUi->SetMaxHeight(fHeight);                                                      //最大高さを設定
	pUi->SetPolygonType(PolygonType);                                                //ポリゴンの種類を設定
	pUi->SetColor(col,false,1.0f);                                                   //色合いを設定
	pUi->SetUseDeath(true);                                                          //死亡フラグを発動するかどうかを設定する
	pUi->SetTextureIndex(pTextureClass->Regist(UI_FILENAME[int(type)]));             //テクスチャを登録し、テクスチャ番号を取得する
	pUi->CObject2D::BindTexture(pTextureClass->GetAddress(pUi->GetTextureIndex()));  //テクスチャを割り当てる
	pUi->CObject::SetType(CObject::TYPE::UI);                                        //オブジェクの種類を決める
	return pUi;
}
//===================================================================================================

//====================================================
//生成処理
//====================================================
void CUi::SetUiType(UITYPE type)
{
	m_Type = type;//種類を変更
	CTexture* pTextureClass = CManager::GetTexture();                     //テクスチャクラスを取得
    SetTextureIndex(pTextureClass->Regist(UI_FILENAME[int(type)]));       //テクスチャを登録し、テクスチャ番号を設定
    CObject2D::BindTexture(pTextureClass->GetAddress(GetTextureIndex())); //テクスチャを割り当てる

}
//===================================================================================================
//=========================================
//
//５月２２日：クラス化[bg.cpp]
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
const char* CBg::m_apBGFILE_NAME[CBg::BG_MAX] =
{
	"data\\TEXTURE\\Title\\Title_001.png",
	"data\\TEXTURE\\Bg\\Wall_000.jpg",
	"data\\TEXTURE\\Result_BG.png"
};//背景のファイル名

//========================
//コンストラクタ
//========================
CBg::CBg() : CObject2D(0), m_bUseTexMove(false),m_PosTex(D3DXVECTOR2(0.0f,0.0f)), m_MoveTex(D3DXVECTOR2(0.0f,0.0f))
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
	m_fPosTexU = 0.0f;//テクスチャ座標

	//==========================
	//回転系
	//==========================
	m_fRotationPower = 0.0f;//回転力
	//======================================================

	//======================================================
    //２Ｄオブジェクトの初期化
    //======================================================
	CObject2D::Init();
	//================================================================================================

	return S_OK;
}
//=========================================================================

//========================
//終了処理
//========================
void CBg::Uninit()
{
	CObject2D::Uninit();
}
//=========================================================================

//========================
//更新処理
//========================
void CBg::Update()
{
	D3DXVECTOR3& Rot = GetRot();

	//Rot.z += 0.01f;
	CObject2D::Update();

	if (m_bUseTexMove == true)
	{
		LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVertexBuffer();
		VERTEX_2D* pVtx;
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
	CObject2D::Draw();
}
//=========================================================================

//========================
//死亡フラグを設定
//========================
void CBg::SetDeath()
{
	CObject::SetDeath();
}
//=========================================================================

//=============================
//背景の生成処理
//=============================
void CBg::Create(BG type)
{
	CBg* pBg = DBG_NEW CBg;   //弾を生成
	bool bSuccess = pBg->CObject::GetCreateSuccess();
	CTexture* pTextureClass = CManager::GetTexture();           //テクスチャクラスを取得
	if (bSuccess == true)
	{
		if (pBg != nullptr)
		{
			pBg->Init();                                                     //初期化処理
			pBg->CObject2D::SetInfo(1, 1, SCREEN_WIDTH,SCREEN_HEIGHT,NORMAL_COL,POLYGONTYPE01_SENTERROLLING,true);//ポリゴンとテクスチャ情報を設定
			pBg->SetTextureIndex(pTextureClass->Regist(m_apBGFILE_NAME[type]));
			pBg->SetUseDeath(true);                 //死亡フラグを発動するかどうかを設定する
			pBg->CObject2D::BindTexture(pTextureClass->GetAddress(pBg->GetTextureIndex()));
			pBg->CObject2D::SetPos(SENTER_VECTOR3);//ポリゴンとテクスチャ情報を設定
			pBg->m_Type = type;  //弾の種類
			pBg->CObject::SetType(CObject::TYPE_BG);//オブジェクの種類を決める
		}
	}
	else
	{
		delete pBg;
		pBg = nullptr;
	}


}
//=========================================================================

//======================================================<<<<<<<<タイトル背景クラス>>>>>>>>>>=========================================

//=====================================================
//コンストラクタ
//=====================================================
CBgTitle::CBgTitle() : m_bRotationSwitch(false),m_nRotationNum(0),m_pUi(),m_TitleUiRotSpeed(0.0f),m_TitleUiScale(D3DXVECTOR2(0.0f,0.0f))
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
	CBg::Init();
	return S_OK;
}
//=========================================================================

//=====================================================
//終了処理
//=====================================================
void CBgTitle::Uninit()
{
	CBg::Uninit();
}
//=========================================================================

//=====================================================
//更新処理
//=====================================================
void CBgTitle::Update()
{
	float& fRotationPower = GetRotationPower();
	fRotationPower += (0.0f - fRotationPower) * 0.25f;//回転力を減衰させる

	m_TitleUiScale.x += fRotationPower / 1.5f;
	m_TitleUiScale.y += fRotationPower / 1.5f;
	if (fRotationPower <= 0.00002f)
	{
		m_bRotationSwitch = m_bRotationSwitch ? false : true;
		m_TitleUiScale = D3DXVECTOR2(1.0f, 1.0f);//拡大率をリセット
		if (m_nRotationNum == 0)
		{
			fRotationPower = 0.05f;
		}
		else
		{
			fRotationPower = 0.1f;
		}
		m_nRotationNum++;
	}



	if (m_bRotationSwitch == false)
	{//右に回転
		m_TitleUiRotSpeed = fRotationPower;
	}
	else
	{//左に回転
		m_TitleUiRotSpeed = -fRotationPower;
	}

	//m_pUi[TITLEUI00_TITLELOGO]->SetScale(m_TitleUiScale);
	//m_pUi[TITLEUI00_TITLELOGO]->SetPolygonRotSpeed(m_TitleUiRotSpeed);
	m_pUi[TITLEUI01_PLAYERILLUST]->SetScale(m_TitleUiScale);
	m_pUi[TITLEUI01_PLAYERILLUST]->SetPolygonRotSpeed(m_TitleUiRotSpeed);
	m_pUi[TITLEUI02_SLIMEILLUST]->SetScale(m_TitleUiScale);
	m_pUi[TITLEUI02_SLIMEILLUST]->SetPolygonRotSpeed(m_TitleUiRotSpeed);

	CBg::Update();
}
//=========================================================================

//=====================================================
//描画処理
//=====================================================
void CBgTitle::Draw()
{
	CBg::Draw();
}
//=========================================================================

//=====================================================
//死亡フラグ設定処理
//=====================================================
void CBgTitle::SetDeath()
{
	CObject::SetDeath();

	//タイトル背景UIの終了処理
	for (int nCnt = 0; nCnt < TITLEUI_MAX; nCnt++)
	{
		if (m_pUi[nCnt] != nullptr)
		{
			m_pUi[nCnt]->SetUseDeath(true);
			m_pUi[nCnt]->SetDeath();
			m_pUi[nCnt] = nullptr;
		}
	}
}
//=========================================================================

//=====================================================
//生成処理
//=====================================================
CBgTitle* CBgTitle::Create(BG type)
{
	CBgTitle* pBgTitle = DBG_NEW CBgTitle;   //弾を生成
	bool bSuccess = pBgTitle->CObject::GetCreateSuccess();
	CTexture* pTextureClass = CManager::GetTexture();           //テクスチャクラスを取得
	if (bSuccess == true)
	{
		if (pBgTitle != nullptr)
		{
			pBgTitle->Init();                                                     //初期化処理
			pBgTitle->SetUseScale(true);//拡大率を使用する
			pBgTitle->CObject2D::SetInfo(1, 1, SCREEN_WIDTH, SCREEN_HEIGHT, NORMAL_COL, POLYGONTYPE01_SENTERROLLING, true);//ポリゴンとテクスチャ情報を設定
			pBgTitle->SetTextureIndex(pTextureClass->Regist(m_apBGFILE_NAME[type]));
			pBgTitle->SetUseDeath(true);                 //死亡フラグを発動するかどうかを設定する
			pBgTitle->CObject2D::BindTexture(pTextureClass->GetAddress(pBgTitle->GetTextureIndex()));
			pBgTitle->CObject2D::SetPos(SENTER_VECTOR3);//ポリゴンとテクスチャ情報を設定
			pBgTitle->SetBgType(type);                  //背景の種類を設定
			pBgTitle->CObject::SetType(CObject::TYPE_BG);//オブジェクの種類を決める

			pBgTitle->m_pUi[CBgTitle::TITLEUI03_MAGICCIRCLE] = CUi::Create(CUi::UITYPE_MAGICCIRCLE, CObject2D::POLYGONTYPE01_SENTERROLLING,
				700.0f, 700.0f, SENTER_VECTOR3, NULL_VECTOR3, NULL_VECTOR3, NORMAL_COL);
			pBgTitle->m_pUi[CBgTitle::TITLEUI03_MAGICCIRCLE]->SetPolygonRotSpeed(0.02f);

			pBgTitle->m_pUi[CBgTitle::TITLEUI00_TITLELOGO] = CUi::Create(CUi::UITYPE_TITLELOGO, CObject2D::POLYGONTYPE01_SENTERROLLING,
				SCREEN_WIDTH - 200.0f, 200.0f, SENTER_VECTOR3 + D3DXVECTOR3(0.0f, -250.0f, 0.0f), NULL_VECTOR3, NULL_VECTOR3, NORMAL_COL);
			pBgTitle->m_pUi[CBgTitle::TITLEUI00_TITLELOGO]->SetUseFloating(0.0f,0.1f,2.0f, true);

			pBgTitle->m_pUi[CBgTitle::TITLEUI01_PLAYERILLUST] = CUi::Create(CUi::UITYPE_PLAYER2DILUST, CObject2D::POLYGONTYPE01_SENTERROLLING,
				200.0f, 200.0f, SENTER_VECTOR3 + D3DXVECTOR3(-400.0f,150.0f, 0.0f), NULL_VECTOR3, NULL_VECTOR3, NORMAL_COL);

			pBgTitle->m_pUi[CBgTitle::TITLEUI02_SLIMEILLUST] = CUi::Create(CUi::UITYPE_SLIMEILUST, CObject2D::POLYGONTYPE01_SENTERROLLING,
				100.0f, 100.0f, SENTER_VECTOR3 + D3DXVECTOR3(400.0f,150.0f, 0.0f), NULL_VECTOR3, NULL_VECTOR3, NORMAL_COL);
		}
	}
	else
	{
		delete pBgTitle;
		pBgTitle = nullptr;
		return nullptr;
	}

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
	CBg::Init();
	return S_OK;
}
//=========================================================================

//=====================================================
//終了処理
//=====================================================
void CBgDifficulty::Uninit()
{
	CBg::Uninit();
}
//=========================================================================

//=====================================================
//更新処理
//=====================================================
void CBgDifficulty::Update()
{
	CBg::Update();
}
//=========================================================================

//=====================================================
//描画処理
//=====================================================
void CBgDifficulty::Draw()
{
	CBg::Draw();
}
//=========================================================================

//=====================================================
//死亡フラグ設定処理
//=====================================================
void CBgDifficulty::SetDeath()
{
	CBg::SetDeath();
}
//=========================================================================

//=====================================================
//生成処理
//=====================================================
CBgDifficulty* CBgDifficulty::Create(BG type)
{
	CBgDifficulty* pBgDifficulty = DBG_NEW CBgDifficulty;   //弾を生成
	bool bSuccess = pBgDifficulty->CObject::GetCreateSuccess();
	CTexture* pTextureClass = CManager::GetTexture();           //テクスチャクラスを取得
	if (bSuccess == true)
	{
		if (pBgDifficulty != nullptr)
		{
			pBgDifficulty->Init();                                                     //初期化処理
			pBgDifficulty->SetUseScale(true);//拡大率を使用する
			pBgDifficulty->CObject2D::SetInfo(1, 1, SCREEN_WIDTH, SCREEN_HEIGHT, NORMAL_COL, POLYGONTYPE01_SENTERROLLING, true);//ポリゴンとテクスチャ情報を設定
			pBgDifficulty->SetTextureIndex(pTextureClass->Regist(m_apBGFILE_NAME[type]));
			pBgDifficulty->SetUseDeath(true);                 //死亡フラグを発動するかどうかを設定する
			pBgDifficulty->CObject2D::BindTexture(pTextureClass->GetAddress(pBgDifficulty->GetTextureIndex()));
			pBgDifficulty->CObject2D::SetPos(SENTER_VECTOR3);//ポリゴンとテクスチャ情報を設定
			pBgDifficulty->SetBgType(type);                  //背景の種類を設定
			pBgDifficulty->CObject::SetType(CObject::TYPE_BG);//オブジェクの種類を決める
		}
	}
	else
	{
		delete pBgDifficulty;
		pBgDifficulty = nullptr;
		return nullptr;
	}

	return pBgDifficulty;
}
//=========================================================================
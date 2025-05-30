//=====================================================================
//
//７月４日：回避取得アイテムを作る[item.cpp]
//Author:ShinaTaiyo
//
//=====================================================================

//======================================
//インクルード
//======================================
#include "item.h"
#include "main.h"
#include "game.h"
#include "edit.h"
#include "manager.h"
#include "renderer.h"
#include "particle.h"
#include "score.h"
#include "player.h"
#include "magic.h"
#include "fade.h"
#include "objectXInfo.h"
#include "collision.h"
#include "stagemanager.h"
//=====================================================================

//======================================
//静的メンバ初期化
//======================================
int CItem::m_nNumFile = CItem::ITEMTYPE_MAX;  //ファイルの数
const char* CItem::m_ITEMFILENAME[CItem::ITEMTYPE_MAX] = 
{ "data\\MODEL\\Item\\Item00_HealHeart.x",
"data\\MODEL\\Item\\Item01_Coin.x" ,
"data\\MODEL\\Item\\Item02_MagicPortion.x",
"data\\MODEL\\Item\\Item03_Crown.x",
"data\\MODEL\\Item\\Item04_DoubleJumpShoes.x",
 "data\\MODEL\\Item\\Item05_DodgeCrystal.x",
 "data\\MODEL\\Item\\Item06_WallJumpCrow.x",
 "data\\MODEL\\Item\\Item07_FireBallEnergy.x",
 "data\\MODEL\\Item\\Item08_BeamEnergy.x",
 "data\\MODEL\\Item\\Item09_IceEnergy.x",
 "data\\MODEL\\Item\\Item10_ThunderEnergy.x",
 "data\\MODEL\\Item\\Item11_MagicSwordEnergy.x",
 "data\\MODEL\\Item\\Item12_PlantShieldEnergy.x",
 "data\\MODEL\\Item\\Item13_QuickHealEnergy.x",
 "data\\MODEL\\Item\\Item14_CrossBombEnergy.x",
 "data\\MODEL\\Item\\Item15_BlackHoleEnergy.x",
 "data\\MODEL\\Item\\Item16_PhycoBurstEnergy.x",
 "data\\MODEL\\Item\\Item17_ChargeShotGun.x",
 "data\\MODEL\\Item\\Item18_RainShotGun.x",
 "data\\MODEL\\Item\\Item19_BurstShotGun.x",
 "data\\MODEL\\Item\\Item20_RapidShotGun.x",
};//アイテムのモデルのパス

//======================================
//コンストラクタ
//======================================
CItem::CItem()
{

}
//=====================================================================

//======================================
//デストラクタ
//======================================
CItem::~CItem()
{

}
//=====================================================================

//======================================
//初期化処理
//======================================
HRESULT CItem::Init()
{
	//=================================
	//Xオブジェクト初期化
	//=================================
	CObjectX::Init();

	//=================================================================

	m_type = ITEMTYPE00_HEALHEART;//種類
	m_bUse = true;              //仕様状態
	m_nCntTime = 0;              //出現してからの時間
	return S_OK;
}
//=====================================================================

//======================================
//終了処理
//======================================
void CItem::Uninit()
{
	CObjectX::Uninit(); //Xオブジェクトの破棄
}
//=====================================================================

//======================================
//更新処理
//======================================
void CItem::Update()
{
	//==================================
    //Xオブジェクトから情報を取得
    //==================================
	D3DXVECTOR3 &Pos = CObjectX::GetPos();                      //オブジェクトの位置を取得
	D3DXVECTOR3 &Rot = CObjectX::GetRot();                      //オブジェクトの向きを取得
	//===========================================================

	//==================================
	//アイテムごとのパターン
	//==================================
	m_nCntTime++;//出現してからの時間を測る

	Rot.y += 0.01f;

	//==================================
	//Xオブジェクト更新
	//==================================
	CObjectX::Update();
	//===========================================================

	//==================================
	//当たり判定を行う
	//==================================
	if (CScene::GetMode() == CScene::MODE_GAME)
	{
		Collision();
	}
	//===========================================================

	float fVXaim3D = 0.0f;
	float fVYaim3D = 0.0f;
	float fVZaim3D = 0.0f;
	float fVLaim3D = 0.0f;
	D3DXVECTOR3 RandPos = NULL_VECTOR3;
	//==================================
	//アイテムの効果発動と破棄
	//==================================
	if (m_bUse == false)
	{
		SetDeath();//死亡フラグを設定
	}
	//===========================================================

}
//=====================================================================

//======================================
//描画処理
//======================================
void CItem::Draw()
{
	CObjectX::Draw();
}
//=====================================================================

//======================================
//死亡フラグを設定
//======================================
void CItem::SetDeath()
{
	CObject::SetDeath();
}
//=====================================================================


//======================================
//アイテム生成処理
//======================================
CItem* CItem::Create(ITEMTYPE type, D3DXVECTOR3 Pos, D3DXVECTOR3 Rot, D3DXVECTOR3 Scale, int nRotType)
{
	CItem* pItem = DBG_NEW CItem;   //敵を生成
	bool bSuccess = pItem->CObject::GetCreateSuccess();
	int nIdx = 0;//モデルのインデックス
	bSuccess = NotSummonProcess(type);
	
	pItem->Init();            //初期化処理
	pItem->m_bUse = true;     //使用状態
	pItem->m_type = type;     //弾の種類
	pItem->SetUseGravity(false);
	pItem->SetUseBlockCollision(false);
	pItem->SetUseDeath(true); //死亡フラグを発動するかどうかを設定する
	pItem->SetLife(1);        //体力を設定
	pItem->SetMaxLife(1);     //最大体力
	pItem->SetAutoSubLife(false);//自動的に体力を減らすかどうか
	pItem->CObjectX::SetPos(Pos);                                                                                  //オブジェクト２Ｄの位置を設定
	pItem->SetPosOld(Pos);                                                                                         //1f前の位置を設定
	pItem->SetSupportPos(Pos);                                                                                     //設置位置
	pItem->CObject::SetType(CObject::TYPE_ITEM);                                                                  //オブジェクトの種類を決める
	pItem->CObjectX::SetScale(Scale);                                                                              //オブジェクトの拡大率
	pItem->SetFormarScale(Scale);                                                                                  //元の拡大率を設定する
	nIdx = CManager::GetObjectXInfo()->Regist(CObjectX::OBJECTXTYPE_ITEM, m_ITEMFILENAME[(int)(type)]);
	pItem->CObjectX::BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(CObjectX::OBJECTXTYPE_ITEM, nIdx),
		CManager::GetObjectXInfo()->GetBuffMat(CObjectX::OBJECTXTYPE_ITEM, nIdx),
		CManager::GetObjectXInfo()->GetdwNumMat(CObjectX::OBJECTXTYPE_ITEM, nIdx),
		CManager::GetObjectXInfo()->GetTexture(CObjectX::OBJECTXTYPE_ITEM, nIdx),
		CManager::GetObjectXInfo()->GetColorValue(CObjectX::OBJECTXTYPE_ITEM, nIdx));                       //モデル情報を割り当てる

	pItem->CObjectX::SetObjXType(CObjectX::OBJECTXTYPE_ITEM);                             //オブジェクトXのタイプを設定
	pItem->CObjectX::SetTypeNum((int)(type));                                             //オブジェクトXごとのタイプ番号を設定
	pItem->SetSize();                                                                     //Xオブジェクトのサイズを設定する
	pItem->CObjectX::SetRotType(CObjectX::ROTTYPE::ROTTYPE_NORMAL);                       //向きの種類を設定
	pItem->SetManagerType((int)(CStageManager::MANAGEROBJECT_ITEM));                      //マネージャーで呼び出す時の種類を設定

	if (bSuccess == false)
	{
		pItem->Release();
	}
	return pItem;
}
//=====================================================================================================

//====================================================
//当たり判定を行う
//====================================================
void CItem::Collision(void)
{
}
//=====================================================================================================

//====================================================
//召喚しない処理
//====================================================
bool CItem::NotSummonProcess(ITEMTYPE type)
{
	bool bJudge = true;
	if (CScene::GetMode() == CScene::MODE_GAME)
	{
		if (CGame::GetPlayer() != nullptr)
		{
			int nNumPlayerMagic = CGame::GetPlayer()->GetNumPlayerMagic();//プレイヤーの魔法の総数を取得する
			bool* bUnlockMagicPlayer = CGame::GetPlayer()->GetUnlockMagic();

			//プレイヤーが開放済みの魔法はスポーンさせない

			switch (type)
			{
			case ITEMTYPE07_FIREBALL_ENERGY:
				if (bUnlockMagicPlayer[CMagicDisp::MAGICDISP00_FIRE] == true)
				{
					bJudge = false;
				}
				break;
			case ITEMTYPE08_BEAMWHIP_ENERGY:
				if (bUnlockMagicPlayer[CMagicDisp::MAGICDISP01_BEAM] == true)
				{
					bJudge = false;
				}
				break;
			case ITEMTYPE09_ICE_ENERGY:
				if (bUnlockMagicPlayer[CMagicDisp::MAGICDISP02_ICE] == true)
				{
					bJudge = false;
				}
				break;
			case ITEMTYPE10_THUNDER_ENERGY:
				if (bUnlockMagicPlayer[CMagicDisp::MAGICDISP03_THUNDER] == true)
				{
					bJudge = false;
				}
				break;
			case ITEMTYPE11_MAGICSWORD_ENERGY:
				if (bUnlockMagicPlayer[CMagicDisp::MAGICDISP04_SWORD] == true)
				{
					bJudge = false;
				}
				break;
			case ITEMTYPE12_PLANTSHIELD_ENERGY:
				if (bUnlockMagicPlayer[CMagicDisp::MAGICDISP05_PLANTSHIELD] == true)
				{
					bJudge = false;
				}
				break;
			case ITEMTYPE13_QUICKHEAL_ENERGY:
				if (bUnlockMagicPlayer[CMagicDisp::MAGICDISP06_QUICKHEAL] == true)
				{
					bJudge = false;
				}
				break;
			case ITEMTYPE14_CROSSBOMB_ENERGY:
				if (bUnlockMagicPlayer[CMagicDisp::MAGICDISP07_CROSSBOMB] == true)
				{
					bJudge = false;
				}
				break;
			case ITEMTYPE15_BLACKHOLE_ENERGY:
				if (bUnlockMagicPlayer[CMagicDisp::MAGICDISP08_BLACKHOLE] == true)
				{
					bJudge = false;
				}
				break;
			case ITEMTYPE16_PHYCOBURST_ENERGY:
				if (bUnlockMagicPlayer[CMagicDisp::MAGICDISP09_PHYCOBURST] == true)
				{
					bJudge = false;
				}
				break;
			default:
				break;
			}
		}
	}
	return bJudge;
}
//=====================================================================================================
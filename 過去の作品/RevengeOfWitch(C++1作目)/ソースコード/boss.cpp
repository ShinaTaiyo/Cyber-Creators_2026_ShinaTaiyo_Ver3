//=================================================
//
//７月２日：ボスを実装[boss.cpp]
//Author:ShinaTaiyo
//
//=================================================

//==============================
//インクルード
//==============================
#include "boss.h"
#include "manager.h"
#include "game.h"
#include "aimodel.h"
#include "edit.h"
#include "bullet.h"
#include "particle.h"
#include "texture.h"
#include "emitter.h"
#include "manager.h"
#include "objectXInfo.h"
#include "debugtext.h"
#include "stagemanager.h"
#include "eventmanager.h"
#include "calculation.h"
#include "player.h"
#include "attack.h"
#include "Ui3D.h"
#include "camera.h"
#include "marker.h"
#include "sound.h"
#include "damage.h"
#include "item.h"
//=================================================


//==============================
//静的メンバ初期化
//==============================

int CBoss::m_nNumFile = CBoss::BOSSTYPE_MAX; //ファイルの数
const char* CBoss::m_BOSS_FILENAME[CBoss::BOSSTYPE_MAX] =
{ "data\\MODEL\\Enemy\\Boss\\Boss00_KingSlime.x",
  "data\\MODEL\\Enemy\\Boss\\Boss01_DeathEnaga.x",
  "data\\MODEL\\Enemy\\Boss\\Boss02_MrStrong.x",
  "data\\MODEL\\Enemy\\Boss\\Boss03_Riley.x",
  "data\\MODEL\\Enemy\\Boss\\Boss04_Draiya.x",
  "data\\MODEL\\Enemy\\Boss\\Boss05_Gabriel.x",
};

//デスエナガの移動先位置
const D3DXVECTOR3 CDeathEnaga::m_MOVEPOINT[CDeathEnaga::m_nMAX_MOVEPOINT] =
{
	D3DXVECTOR3(-300.0f,250.0f,0.0f),D3DXVECTOR3(0.0f,250.0f,0.0f),D3DXVECTOR3(300.0f,250.0f,0.0f),
	D3DXVECTOR3(-300.0f,0.0f,0.0f),D3DXVECTOR3(0.0f,0.0f,0.0f),D3DXVECTOR3(300.0f,0.0f,0.0f),
};
//=================================================

//==============================
//コンストラクタ
//==============================
CBoss::CBoss()
{

}
//=================================================

//==============================
//デストラクタ
//==============================
CBoss::~CBoss()
{

}
//=================================================

//==============================
//初期化処理
//==============================
HRESULT CBoss::Init()
{
	//===========================
	//Xオブジェクト初期化
	//===========================
	if (FAILED(CObjectX::Init()))
	{
		return E_FAIL;
	}
	//====================================

	//===========================
	//クラスの変数初期化
	//===========================
	m_nCntTime = 0;          //出現してからの時間を測る
	m_Aim = NULL_VECTOR3;    //ベクトル
	m_fTotalAim = 0.0f;      //総合ベクトル
	m_bUse = true;           //使用状態
	m_pGauge = nullptr;      //ゲージ
	m_PurposePos = NULL_VECTOR3;//目的の位置

	//===========================
	//行動パターン関係
	//===========================
	m_nPattern = 0;          //パターン
	m_nSubPattern = 0;       //サブパターン（行動パターンの中で更に分割）
	m_nPatternTime = 0;      //そのパターンになってからの時間
	m_nSkill = 0;            //現在のスキル
	m_bAction = false;       //行動しているかどうか
	m_nActionNum = 0;        //行動回数
	m_nDefeatStagingTime = 0;//倒れる演出の時間

	m_fSaveRot = 0.0f;       //向きを保存しておく
	m_bInsanity = false;     //発狂しているかどうか
	m_bSpecialAttack = false;//必殺技を使うかどうか
	m_nTurnCnt = 0;           //ターン数をカウント
	//====================================

	//===========================
	//移動パターン関係
	//===========================
	m_bUseMovePattern = true; //移動パターンを使っているかどうか
	m_nMovePatternNum = 0;    //現在の移動パターンの番号
	m_nMovePatternFrame = 0;//移動パターンを使っているフレーム数
	m_nMaxMovePatternFrame = 0;//移動パターンの最大フレーム数
	//=======================================================================================

	SetUseGravity(true);

	CManager::GetSound()->StopSound();
	CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_BGM_BOSS);
	return S_OK;
}
//=================================================

//==============================
//終了処理
//==============================
void CBoss::Uninit()
{
	CObjectX::Uninit();
	//SetDeath();//死亡フラグ設定
}
//=================================================

//==============================
//更新処理
//==============================
void CBoss::Update()
{
	if (CScene::GetMode() == CScene::MODE_GAME && CManager::GetEventManager()->GetEventType() != CEventManager::EVENTTYPE_BOSSAPPEAR)
	{
		m_nCntTime++;
		//==================================
		//オブジェクトXから情報を取得
		//==================================
		D3DXVECTOR3 &pos = GetPos();                                    //モデルの位置の取得
		D3DXVECTOR3& Move = GetMove();                                  //移動量を取得
		D3DXVECTOR3 PosOld = GetPosOld();                               //1f前の位置を設定
		D3DXVECTOR3 & Rot = GetRot();                                   //モデルの向きの取得
		D3DXVECTOR3 Size = GetSize();                                   //サイズを取得
		D3DXVECTOR3 Pos = CObjectX::GetPos();       
		D3DXVECTOR3 RandMove = NULL_VECTOR3;                            //乱数移動量  
		CParticle* pParticle = nullptr;                                 //パーティクル取得用
		CObject* pLandingObj = nullptr;                                 //現在乗っているオブジェクト格納用	
		int& nLife = GetLife();                                         //体力を取得
		int nLandingBlockNum = 0;                                       //どのブロックの上に乗っているか
		int nLandingBlockPriority = 0;                                  //乗っているブロックのプライオリティ
		bool bIsLiftLanding = false;                                    //リフトの上に乗っているかどうか
		bool bDodgeState = false;                                       //回避状態かどうか
		bool& bIsLanding = GetLanding();                                //地面にいるかどうか
		bool& bIsJumping = GetJumping();                                //ジャンプしているかどうか
		bool& bIsWalling = GetWalling();                                //壁にくっついているかどうか
		bool& bUseGravity = GetUseGravity();                            //重力を使用するかどうか
		bool& bUseBlockCollision = GetUseBlockCollision();              //ブロックとの当たり判定を使用するかどうか
		bool bSuccessBlockCollisionX = false;                           //ブロックのX方向の当たり判定が成功したかどうか
		bool bSuccessBlockCollisionY = false;                           //ブロックのY方向の当たり判定が成功したかどうか
		//===========================================================

		//=====================================
		//原点に到達したときの位置の補正
		//=====================================
		AdjusOriginPosProcess();
		//===========================================================

		if (bUseGravity == true)
		{

			if (CGame::GetPlayer()->GetHp() < 1)
			{//プレイヤーの体力がなくなったため、死亡フラグをONにする
				SetUseDeath(true);
			}

			GravityProcess();
		}

		//==================================
		//ボスの行動
		//==================================
		if (nLife > 0)
		{
			if (m_bUseMovePattern == true)
			{//移動パターンを呼ぶ
				MovePattern();
			}
			else
			{//移動パターンが呼ばれていないときに
				ActionPattern();
			}
		}
		//===========================================================

		//==================================
		//Xオブジェクト更新
		//==================================
		CObjectX::Update();
		//===========================================================


		//==================================
		//当たり判定
		//==================================
		Collision();
		//===========================================================

		//==================================
		//ボスの破棄
		//==================================
		if (nLife <= 0)
		{
			Move = NULL_VECTOR3; //動きを止める
			m_nDefeatStagingTime++;//倒す演出の時間を計測する
			DefeatStaging();//討伐演出
			if (m_nDefeatStagingTime >= m_nMAX_DEFEATSTAGINGTIME)
			{
				for (int nCnt = 0; nCnt < 120; nCnt++)
				{
					RandMove.x = float(rand() % 150 - 75) / 5;
					RandMove.y = float(rand() % 150 - 75) / 5;
					RandMove.z = float(rand() % 150 - 75) / 5;
					pParticle = CParticle::Create(CParticle::TYPE00_NORMAL, 120, 100.0f, 100.0f, Pos, RandMove, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), true);
					pParticle->SetGravity(-0.3f);//重力を設定
					pParticle = nullptr;
				}
				if (CDifficulty::GetDifficultyNum() != CDifficulty::DIFFICULTYTYPE_BOSSRUSH)
				{
					//報酬
					CItem::Create(CItem::ITEMTYPE03_CROWN, D3DXVECTOR3(0.0f, 150.0f, 0.0f), D3DXVECTOR3(0.2f, 0.0f, 0.0f), ONE_VECTOR3 * 2.0f, 0);
				}
				else
				{
					BossRushProcess();
				}
				//カメラを元に戻す
				CManager::GetCamera()->SetCameraType(CCamera::CAMERATYPE_BESIDE);
				SetDeath();
			}
		}
		//===========================================================
	}
	else if(CScene::GetMode() == CScene::MODE_EDIT)
	{
		//==================================
		//Xオブジェクト更新
		//==================================
		CObjectX::Update();
		//===========================================================
	}
}
//=================================================

//==============================
//描画処理
//==============================
void CBoss::Draw()
{
	CObjectX::Draw();

}
//=====================================================================================================================

//===============================================================
//死亡フラグを設定
//===============================================================
void CBoss::SetDeath()
{
	if (GetUseDeath() == true)
	{
		if (m_pGauge != nullptr)
		{
			m_pGauge->SetUseDeath(true);
			m_pGauge->SetDeath();
			m_pGauge = nullptr;
		}
		CObject::SetDeath();
	}
}
//=====================================================================================================================

//===============================================================
//ダメージを与える処理
//===============================================================
void CBoss::SetDamage(int nDamage, int nHitStopTime)
{
	bool& bHitStop = GetHitStop(); //ヒットストップ状態かどうかを取得
	int& nLife = GetLife();        //現在の体力を取得
	int& nHitStopTime2 = GetHitStopTime();//ヒットストップ時間を取得
	if (bHitStop == false)
	{//ヒットストップ状態じゃなければ
		if (nHitStopTime > 0)
		{
			bHitStop = true;              //ヒットストップ状態にする
			nHitStopTime2 = nHitStopTime; //ヒットストップ時間
		}
		nLife -= nDamage;
		SetColor(COLORTYPE_RED, 10);

		if (m_pGauge != nullptr)
		{
			m_pGauge->SetParam(nLife);
		}
		CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_DAMAGE_000);
		CDamage::Create(nDamage, GetPos(), NORMAL_COL, 40.0f, 40.0f);
	}
}
//=====================================================================================================================

//===============================
//ボスの生成処理
//===============================
CBoss* CBoss::Create(BOSSTYPE type, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 Scale)
{
	CBoss* pBoss = DBG_NEW CBoss;   //ボスを生成
	bool bSuccess = pBoss->CObject::GetCreateSuccess();
	int nIdx = 0;//モデルのインデックス
	if (bSuccess == true)
	{
		if (pBoss != nullptr)
		{
			pBoss->Init();         //初期化処理
			pBoss->SetUseDeath(true);                 //死亡フラグを発動するかどうかを設定する
			pBoss->m_bUse = true;  //使用状態
			pBoss->m_type = type;  //ボスの種類
			pBoss->SetLife(nLife);                     //体力
			pBoss->SetMaxLife(nLife);//最大体力
			pBoss->SetAutoSubLife(false);              //自動的に体力を減らすかどうか
			pBoss->CObjectX::SetPos(pos);                                                                                  //オブジェクト２Ｄの位置を設定
			pBoss->SetPosOld(pos);                                                                                         //1f前の位置を設定
			pBoss->SetSupportPos(pos);                                                                                     //設置位置
			pBoss->CObject::SetType(CObject::TYPE_BOSS);                                                                   //オブジェクトの種類を決める
			pBoss->CObjectX::SetScale(Scale);                                                                              //オブジェクトの拡大率
			nIdx = CManager::GetObjectXInfo()->Regist(CObjectX::OBJECTXTYPE_BOSS, m_BOSS_FILENAME[(int)(type)]);
			pBoss->CObjectX::BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(CObjectX::OBJECTXTYPE_BOSS, nIdx),
				CManager::GetObjectXInfo()->GetBuffMat(CObjectX::OBJECTXTYPE_BOSS, nIdx),
				CManager::GetObjectXInfo()->GetdwNumMat(CObjectX::OBJECTXTYPE_BOSS, nIdx),
				CManager::GetObjectXInfo()->GetTexture(CObjectX::OBJECTXTYPE_BOSS, nIdx),
				CManager::GetObjectXInfo()->GetColorValue(CObjectX::OBJECTXTYPE_BOSS, nIdx));                       //モデル情報を割り当てる
			pBoss->SetBossHpGauge(nLife);                                                                              //ボスの体力ゲージ
			pBoss->CObjectX::SetObjXType(CObjectX::OBJECTXTYPE_BOSS);                                                      //オブジェクトXのタイプを設定
			pBoss->CObjectX::SetTypeNum((int)(type));                                                                      //オブジェクトXごとのタイプ番号を設定
			pBoss->SetSize();                                                                                              //Xオブジェクトのサイズを設定する
			pBoss->CObjectX::SetRotType(CObjectX::ROTTYPE::ROTTYPE_NORMAL);                                                //向きの種類を設定
			pBoss->SetManagerType((int)(CStageManager::MANAGEROBJECT_BOSS));                                               //マネージャーで呼び出す時の種類を設定
		}
		else
		{//オブジェクトに空きがなかったので破棄する
			delete pBoss;
			pBoss = nullptr;
			return nullptr;
		}
	}
	return pBoss;
}
//===================================================================================================================================================

//====================================================
//サイズを調整する
//====================================================
void CBoss::AdjustSize()
{

}
//===================================================================================================================================================

//====================================================
//ボスを設定する
//====================================================
CObject * CBoss::SetCreateBoss(BOSSTYPE type, int nLife, D3DXVECTOR3 Pos, D3DXVECTOR3 Scale)
{
	CObject* pObj = nullptr;
	switch (type)
	{
	case BOSSTYPE00_KINGSLIME:
		pObj = CKingSlime::Create(CBoss::BOSSTYPE00_KINGSLIME, nLife, Pos, Scale);
		if (CScene::GetMode() == CScene::MODE_GAME)
		{
			CManager::GetEventManager()->SetBossAppearEvent(CEventManager::BOSSAPPEAR_KINGSLIME);
		}
		break;
	case BOSSTYPE01_DEATHENAGA:
		pObj = CDeathEnaga::Create(CBoss::BOSSTYPE01_DEATHENAGA, nLife, Pos, Scale);
		if (CScene::GetMode() == CScene::MODE_GAME)
		{
			CManager::GetEventManager()->SetBossAppearEvent(CEventManager::BOSSAPPEAR_DEATHENAGA);
		}
		break;
	case BOSSTYPE02_MRSTRONG:
		pObj = CMrStrong::Create(CBoss::BOSSTYPE02_MRSTRONG, nLife, Pos, Scale);
		if (CScene::GetMode() == CScene::MODE_GAME)
		{
			CManager::GetEventManager()->SetBossAppearEvent(CEventManager::BOSSAPPEAR_MRSTRONG);
		}
		break;
	case BOSSTYPE03_SNOWDEVIL_RILEY:
		pObj = CRiley::Create(CBoss::BOSSTYPE03_SNOWDEVIL_RILEY, nLife, Pos, Scale);
		if (CScene::GetMode() == CScene::MODE_GAME)
		{
			CManager::GetEventManager()->SetBossAppearEvent(CEventManager::BOSSAPPEAR_RILEY);
		}
		break;
	case BOSSTYPE04_DRAIYA:
		pObj = CDraiya::Create(CBoss::BOSSTYPE04_DRAIYA, nLife, Pos, Scale);
		if (CScene::GetMode() == CScene::MODE_GAME)
		{
			CManager::GetEventManager()->SetBossAppearEvent(CEventManager::BOSSAPPEAR_DRAIYA);
		}
		break;
	case BOSSTYPE05_GABRIEL:
		pObj = CGabriel::Create(CBoss::BOSSTYPE05_GABRIEL, nLife, Pos, Scale);
		if (CScene::GetMode() == CScene::MODE_GAME)
		{
			//CManager::GetEventManager()->SetBossAppearEvent(CEventManager::BOSSAPPEAR_GABRIEL);
		}
		break;
	default:
		break;
	}
    return pObj;
}
//===================================================================================================================================================

//====================================================
//ボスの体力ゲージを設定する
//====================================================
void CBoss::SetBossHpGauge(int nLife)
{
	m_pGauge = CGauge::Create(CGauge::GAUGETYPE_BOSSHP, nLife, 700.0f, 10.0f, D3DXVECTOR3(SCREEN_WIDTH - 850.0f, 30.0f, 0.0f));
}
//===================================================================================================================================================

//====================================================
//討伐演出
//====================================================
void CBoss::DefeatStaging()
{
	D3DXVECTOR3 Pos = GetPos();//位置
	D3DXVECTOR3 SummonPos = NULL_VECTOR3;//召喚位置
	float fVXaim = 0.0f;//X方向ベクトル
	float fVYaim = 0.0f;//Y方向ベクトル
	float fVZaim = 0.0f;//Z方向ベクトル
	float fVLaim = 0.0f;//総合ベクトル
	float fRandSpeed = 0.0f;
	float fRatioColor = 0.0f;//色の割合
	CParticle* pParticle = nullptr;//パーティクル取得用
	CManager::GetCamera()->SetCameraType(CCamera::CAMERATYPE_BOSSDEFEAT);

	SummonPos.x = Pos.x + float(rand() % 300 - 150);
	SummonPos.y = Pos.y + float(rand() % 300 - 150);
	SummonPos.z = Pos.z + float(rand() % 300 - 150);
	fRandSpeed = float(rand() % 300 + 1) / 10;
	//==========================
	//３次元ベクトルを取る
	//==========================
	fVXaim = Pos.x - SummonPos.x;
	fVYaim = Pos.y - SummonPos.y;
	fVZaim = Pos.z - SummonPos.z;

	D3DXVECTOR3 BulletMove = NULL_VECTOR3;

	fVLaim = sqrtf((fVXaim * fVXaim) + (fVYaim * fVYaim)
		+ (fVZaim * fVZaim));

	BulletMove.x = fVXaim / fVLaim * fRandSpeed;
	BulletMove.y = fVYaim / fVLaim * fRandSpeed;
	BulletMove.z = fVZaim / fVLaim * fRandSpeed;

	pParticle = CParticle::Create(CParticle::TYPE00_NORMAL, 30, 40.0f, 40.0f, SummonPos, BulletMove, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), true);
	pParticle->SetAddSpeed(0.99f);//加速量を設定
	pParticle = nullptr;
	//==================================================================================================================


	fRatioColor = (float)(m_nDefeatStagingTime) / (float)(m_nMAX_DEFEATSTAGINGTIME);
	SetOriginalColor(D3DXCOLOR(fRatioColor, 0.0f, 0.0f, 1.0f), 2);
}

//===============================
//ボスの行動
//===============================
void CBoss::ActionPattern()
{
	m_nPatternTime++;
}
//===================================================================================================================================================

//===============================
//移動パターン
//===============================
void CBoss::MovePattern()
{
	m_nMovePatternFrame++;//移動パターンを使用しているフレーム数をカウントする
}
//===================================================================================================================================================

//===============================
//リセットパターン
//===============================
void CBoss::ResetPattern()
{
	m_nActionNum = 0;
	m_nPattern = 0;
	m_nSubPattern = 0;
	m_nPatternTime = 0;
	m_bAction = false;
	m_bUseMovePattern = true;
}
//===================================================================================================================================================

//===========================================================
//ボスラッシュの処理
//===========================================================
void CBoss::BossRushProcess()
{
	switch (m_type)
	{
	case BOSSTYPE00_KINGSLIME:
		CMarker::Create(CMarker::MARKERTYPE00_WARP, D3DXVECTOR3(0.0f, 100.0f, 0.0f), ONE_VECTOR3, NULL_VECTOR3, 1);
		break;
	case BOSSTYPE01_DEATHENAGA:
		CMarker::Create(CMarker::MARKERTYPE00_WARP, D3DXVECTOR3(0.0f, 100.0f, 0.0f), ONE_VECTOR3, NULL_VECTOR3, 2);
		break;
	case BOSSTYPE02_MRSTRONG:
		CMarker::Create(CMarker::MARKERTYPE00_WARP, D3DXVECTOR3(0.0f, 100.0f, 0.0f), ONE_VECTOR3, NULL_VECTOR3, 3);
		break;
	case BOSSTYPE03_SNOWDEVIL_RILEY:
		CMarker::Create(CMarker::MARKERTYPE00_WARP, D3DXVECTOR3(0.0f, 100.0f, 0.0f), ONE_VECTOR3, NULL_VECTOR3, 4);
		break;
	case BOSSTYPE04_DRAIYA:
		CItem::Create(CItem::ITEMTYPE03_CROWN, D3DXVECTOR3(0.0f, 100.0f, 0.0f), NULL_VECTOR3, ONE_VECTOR3, 0);
		break;
	default:
		break;
	}
}
//===================================================================================================================================================

//===========================================================
//当たり判定
//===========================================================
void CBoss::Collision()
{

}
//===================================================================================================================================================

//==========================================================
//情報表示処理
//==========================================================
void CBoss::DispInfo()
{
	int& nMaxLife = GetMaxLife();
	CObjectX::DispInfo();
}
//===================================================================================================================================================

//==========================================================
//移動パターンを決める処理
//==========================================================
void CBoss::DecideMovePattern()
{

}
//===================================================================================================================================================

//============================================<<<キングスライムクラス>>>=============================================================================


//============================================================
//コンストラクタ
//============================================================
CKingSlime::CKingSlime()
{

}
//===================================================================================================================================================

//============================================================
//デストラクタ
//============================================================
CKingSlime::~CKingSlime()
{

}
//===================================================================================================================================================

//============================================================
//初期化処理
//============================================================
HRESULT CKingSlime::Init()
{
	CBoss::Init();
	return S_OK;
}
//===================================================================================================================================================

//============================================================
//終了処理
//============================================================
void CKingSlime::Uninit()
{
	CBoss::Uninit();//ボスの終了処理
}
//===================================================================================================================================================

//============================================================
//更新処理
//============================================================
void CKingSlime::Update()
{
	CBoss::Update();//ボスの更新処理
}
//===================================================================================================================================================

//============================================================
//描画処理
//============================================================
void CKingSlime::Draw()
{
	int nCntTime = GetCntTime();
	CBoss::Draw();
}
//===================================================================================================================================================

//============================================================
//死亡フラグを設定
//============================================================
void CKingSlime::SetDeath()
{
	CBoss::SetDeath();
}
//===================================================================================================================================================

//============================================================
//生成処理
//============================================================
CKingSlime* CKingSlime::Create(BOSSTYPE type, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 Scale)
{
	CKingSlime* pKingSlime = DBG_NEW CKingSlime;   //ボスを生成
	bool bSuccess = pKingSlime->CObject::GetCreateSuccess();
	int nIdx = 0;//モデルのインデックス
	if (bSuccess == true)
	{
		if (pKingSlime != nullptr)
		{
			pKingSlime->Init();                             //初期化処理
			pKingSlime->SetUseDeath(true);                  //死亡フラグを発動するかどうかを設定する
			pKingSlime->SetType(type);                      //ボスの種類
			pKingSlime->SetLife(nLife);                     //体力
			pKingSlime->SetMaxLife(nLife);//最大体力
			pKingSlime->SetAutoSubLife(false);              //自動的に体力を減らすかどうか
			pKingSlime->CObjectX::SetPos(pos);                                                                                  //オブジェクト２Ｄの位置を設定
			pKingSlime->SetPosOld(pos);                                                                                         //1f前の位置を設定
			pKingSlime->SetSupportPos(pos);                                                                                     //設置位置
			pKingSlime->CObject::SetType(CObject::TYPE_BOSS);                                                                   //オブジェクトの種類を決める
			pKingSlime->CObjectX::SetScale(Scale);                                                                              //オブジェクトの拡大率
			pKingSlime->SetFormarScale(Scale);                                                                                  //元の拡大率を設定する
			nIdx = CManager::GetObjectXInfo()->Regist(CObjectX::OBJECTXTYPE_BOSS, m_BOSS_FILENAME[(int)(type)]);
			pKingSlime->CObjectX::BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(CObjectX::OBJECTXTYPE_BOSS, nIdx),
				CManager::GetObjectXInfo()->GetBuffMat(CObjectX::OBJECTXTYPE_BOSS, nIdx),
				CManager::GetObjectXInfo()->GetdwNumMat(CObjectX::OBJECTXTYPE_BOSS, nIdx),
				CManager::GetObjectXInfo()->GetTexture(CObjectX::OBJECTXTYPE_BOSS, nIdx),
				CManager::GetObjectXInfo()->GetColorValue(CObjectX::OBJECTXTYPE_BOSS, nIdx));                                   //モデル情報を割り当てる
			pKingSlime->SetBossHpGauge(nLife);                                                                                  //体力ゲージを設定する
			pKingSlime->CObjectX::SetObjXType(CObjectX::OBJECTXTYPE_BOSS);                                                      //オブジェクトXのタイプを設定
			pKingSlime->CObjectX::SetTypeNum((int)(type));                                                                      //オブジェクトXごとのタイプ番号を設定
			pKingSlime->SetSize();                                                                                              //Xオブジェクトのサイズを設定する
			pKingSlime->CObjectX::SetRotType(CObjectX::ROTTYPE::ROTTYPE_NORMAL);                                                //向きの種類を設定
			pKingSlime->SetManagerType((int)(CStageManager::MANAGEROBJECT_BOSS));                                               //マネージャーで呼び出す時の種類を設定
		}
		else
		{//オブジェクトに空きがなかったので破棄する
			delete pKingSlime;
			pKingSlime = nullptr;
			return nullptr;
		}
	}
	return pKingSlime;
}
//===================================================================================================================================================

//============================================================
//キングスライムの行動パターン
//============================================================
void CKingSlime::ActionPattern()
{
	bool& bAction = GetAction();
	int& nSkill = GetSkill();

	if (bAction == false)
	{
		nSkill = rand() % 5;
		bAction = true;
	}

	if (bAction == true)
	{
		CBoss::ActionPattern();

		switch (nSkill)
		{
		case 0:
			Action0();
			break;
		case 1:
			Action1();
			break;
		case 2:
			Action2();
			break;
		case 3:
			Action3();
			break;
		case 4:
			Action4();
			break;
		default:
			break;
		}
	}

}
//===================================================================================================================================================

//============================================================
//キングスライムのアクション０
//============================================================
void CKingSlime::Action0()
{
	int& nPatternTime = GetPatternTime();                           //パターン時間を取得する
	bool& bAction = GetAction();                                    //行動しているかどうかを取得
	int& nPattern = GetPattern();                                   //現在のパターンを取得

	D3DXVECTOR3& Pos = GetPos();
	D3DXVECTOR3& Move = GetMove();                                  //移動量を取得
	D3DXVECTOR3& PlayerPos = CGame::GetPlayer()->GetPos();
	switch (nPattern)
	{
	case 0:
		if (PlayerPos.x >= Pos.x)
		{
			Move.x = 2.2f;
		}
		else
		{
			Move.x = -2.2f;
		}

		if (nPatternTime == 60)
		{
			CParticle::SummonChargeParticle(CParticle::TYPE00_NORMAL, 60, 60, 2.0f, 30.0f, 30.0f, 200, 50, 10,true, Pos, NORMAL_COL, true);
		}

		if (nPatternTime >= 120)
		{
			nPattern++;
			nPatternTime = 0;
		}
		break;
	case 1:
		if (PlayerPos.x >= Pos.x)
		{
			nPattern++;
		}
		else
		{
			nPattern += 2;
		}
		nPatternTime = 0;
		break;
	case 2:
		Move.x = 10.0f;
		if (nPatternTime % 20 == 0)
		{
			CBullet::Create(CBullet::BULLETTYPE_ENEMY, CBullet::BULLETTEX_ENEMY, 90, 7, 20.0f, 20.0f, Pos, D3DXVECTOR3(sinf(0.0f) * 5.0f, cosf(0.0f) * 5.0f, 0.0f));
		}

		if (nPatternTime >= 50)
		{
			ResetPattern();
		}
		break;
	case 3:
		Move.x = -10.0f;
		if (nPatternTime % 20 == 0)
		{
			CBullet::Create(CBullet::BULLETTYPE_ENEMY, CBullet::BULLETTEX_ENEMY, 90, 7, 20.0f, 20.0f, Pos, D3DXVECTOR3(sinf(0.0f) * 5.0f, cosf(0.0f) * 5.0f, 0.0f));
		}

		if (nPatternTime >= 50)
		{
			ResetPattern();
		}
		break;
	default:
		break;
	}
}
//===================================================================================================================================================

//============================================================
//キングスライムのアクション１
//============================================================
void CKingSlime::Action1()
{
	int& nPatternTime = GetPatternTime();                           //パターン時間を取得する
	bool& bAction = GetAction();                                    //行動しているかどうかを取得
	bool& bIsLanding = GetLanding();                                //地面にいるかどうかを取得
	int& nPattern = GetPattern();                                   //現在のパターンを取得
	int& nActionNum = GetActionNum();                               //アクション回数をカウント
	D3DXVECTOR3& Pos = GetPos();
	D3DXVECTOR3& Move = GetMove();                                  //移動量を取得
	D3DXVECTOR3& PlayerPos = CGame::GetPlayer()->GetPos();
	float fRandRot = 0.0f;
	CBullet* pBullet = nullptr;
	switch (nPattern)
	{
	case 0:
		if (nPatternTime == 1)
		{
			CParticle::SummonChargeParticle(CParticle::TYPE00_NORMAL,100,100, 5.0f, 30.0f, 30.0f, 250, 50, 10,true, Pos, NORMAL_COL, true);
		}

		if (nPatternTime == 50)
		{
			nPattern++;
		}
		break;
	case 1:
		if (bIsLanding == true)
		{
			Pos = D3DXVECTOR3(PlayerPos.x, PlayerPos.y + 300.0f, PlayerPos.z);
			CParticle::SummonChargeParticle(CParticle::TYPE00_NORMAL, 40, 45, 5.0f, 30.0f, 30.0f, 150, 100, 10, false, Pos, NORMAL_COL, true);
			nPattern++;
		}
		break;
	case 2:
		CParticle::SummonParticle(CParticle::TYPE00_NORMAL, 40, 45, 30.0f, 30.0f, 100, 10, false,D3DXVECTOR3(Pos.x,0.0f,Pos.z), NORMAL_COL, true);
		nActionNum++;
		nPattern = 1;
		if (nActionNum == 5)
		{
			ResetPattern();
		}
		break;
	default:
		break;
	}
}
//===================================================================================================================================================

//============================================================
//キングスライムのアクション２
//============================================================
void CKingSlime::Action2()
{
	int& nPatternTime = GetPatternTime();                           //パターン時間を取得する
	bool& bAction = GetAction();                                    //行動しているかどうかを取得
	int& nPattern = GetPattern();                                   //現在のパターンを取得
	int& nActionNum = GetActionNum();                               //アクション回数をカウント

	D3DXVECTOR3& Pos = GetPos();
	D3DXVECTOR3& Move = GetMove();                                  //移動量を取得
	D3DXVECTOR3& PlayerPos = CGame::GetPlayer()->GetPos();
	float fRandRot = 0.0f;
	D3DXVECTOR3 SummonPos = NULL_VECTOR3;
	CParticle* pParticle = nullptr;//パーティクル取得用
	float fVXaim = 0.0f;//X方向ベクトル
	float fVYaim = 0.0f;//Y方向ベクトル
	float fVZaim = 0.0f;//Z方向ベクトル
	float fVLaim = 0.0f;//総合ベクトル
	switch (nPattern)
	{
	case 0:
		if (nPatternTime >= 100)
		{
			nPattern++;
			nPatternTime = 0;
		}
		else
		{
			SummonPos.x = Pos.x + float(rand() % 300 - 150);
			SummonPos.y = Pos.y + float(rand() % 300 - 150);
			SummonPos.z = Pos.z + float(rand() % 300 - 150);

			//==========================
			//３次元ベクトルを取る
			//==========================
			fVXaim = Pos.x - SummonPos.x;
			fVYaim = Pos.y - SummonPos.y;
			fVZaim = Pos.z - SummonPos.z;

			D3DXVECTOR3 BulletMove = NULL_VECTOR3;

			fVLaim = sqrtf((fVXaim * fVXaim) + (fVYaim * fVYaim)
				+ (fVZaim * fVZaim));

			BulletMove.x = fVXaim / fVLaim * 10.0f;
			BulletMove.y = fVYaim / fVLaim * 10.0f;
			BulletMove.z = fVZaim / fVLaim * 10.0f;

			pParticle = CParticle::Create(CParticle::TYPE00_NORMAL, 60, 40.0f, 40.0f, SummonPos, BulletMove, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), true);
			pParticle->SetAddSpeed(1.01f);//加速量を設定
			pParticle = nullptr;
			//==================================================================================================================

		}
		break;
	case 1:

		Pos.y += 0.1f;
		Move.y += 30.0f;
		nPattern++;
		nPatternTime = 0;
		break;
	case 2:
		if (Pos.y <= 0.0f)
		{
			nPattern++;
			nPatternTime = 0;
			CManager::GetCamera()->SetShake(50, 0.3f);
		}
		break;
	case 3:
		if (nPatternTime % 10 == 0 && nPatternTime <= 140)
		{
			SummonPos.x = Pos.x + float(rand() % 1200 - 600);
			SummonPos.y = Pos.y + 700.0f;
			SummonPos.z = Pos.z;
			CAttackEnemy::Create(CAttack::ATTACKTYPE01_METEO, 5, 120, SummonPos,
				D3DXVECTOR3(sinf(D3DX_PI) * 10.0f, cosf(D3DX_PI) * 10.0f, 0.0f), ONE_VECTOR3 * 2.0f, NULL_VECTOR3, CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, D3DXVECTOR3(0.1f, 0.1f, 0.1f), true, 10);
		}

		if (nPatternTime >= 240)
		{
			ResetPattern();
		}
		break;
	}
}
//===================================================================================================================================================

//============================================================
//キングスライムのアクション３
//============================================================
void CKingSlime::Action3()
{
	int& nPatternTime = GetPatternTime();                           //パターン時間を取得する
	bool& bAction = GetAction();                                    //行動しているかどうかを取得
	int& nPattern = GetPattern();                                   //現在のパターンを取得
	int& nActionNum = GetActionNum();                               //アクション回数をカウント

	D3DXVECTOR3& Move = GetMove();                                  //移動量を取得
	D3DXVECTOR3& Pos = GetPos();
	D3DXVECTOR3& PlayerPos = CGame::GetPlayer()->GetPos();
	D3DXVECTOR3& Rot = GetRot();
	D3DXVECTOR3 GoalPos = D3DXVECTOR3(0.0f, 0.0f, 500.0f);
	D3DXVECTOR3 BulletMove = NULL_VECTOR3;
	float fVXaim = 0.0f;//X方向ベクトル
	float fVYaim = 0.0f;//Y方向ベクトル
	float fVZaim = 0.0f;//Z方向ベクトル
	float fVLaim = 0.0f;//総合ベクトル
	switch (nPattern)
	{
	case 0:
		if (nPatternTime >= 50)
		{
			Rot.y = D3DX_PI;
			nPattern++;
			nPatternTime = 0;
		}
		break;
	case 1:
		//==========================
		//目的の位置までの３次元ベクトルを取る
		//==========================
		fVXaim = GoalPos.x - Pos.x;
		fVYaim = GoalPos.y - Pos.y;
		fVZaim = GoalPos.z - Pos.z;

		fVLaim = sqrtf((fVXaim * fVXaim) + (fVYaim * fVYaim)
			+ (fVZaim * fVZaim));

		Move.x = fVXaim / fVLaim * 5.0f;
		Move.y = fVYaim / fVLaim * 5.0f;
		Move.z = fVZaim / fVLaim * 5.0f;

		if (Pos.x >= GoalPos.x - 20.0f && Pos.x <= GoalPos.x + 20.0f &&
			Pos.y >= GoalPos.y - 20.0f && Pos.y <= GoalPos.y + 20.0f &&
			Pos.z >= GoalPos.z - 20.0f && Pos.z <= GoalPos.z + 20.0f)
		{
			Pos = GoalPos;
			Rot.y = 0.0f;
			nPattern++;
			nPatternTime = 0;
		}
		//==================================================================================================================
		break;
	case 2:
		//==========================
		//目的の位置までの３次元ベクトルを取る
		//==========================
		fVXaim = PlayerPos.x - Pos.x;
		fVYaim = PlayerPos.y - Pos.y;
		fVZaim = PlayerPos.z - Pos.z;

		fVLaim = sqrtf((fVXaim * fVXaim) + (fVYaim * fVYaim)
			+ (fVZaim * fVZaim));

		BulletMove.x = fVXaim / fVLaim * 15.0f;
		BulletMove.y = fVYaim / fVLaim * 15.0f;
		BulletMove.z = fVZaim / fVLaim * 15.0f;

		if (nPatternTime % 3 == 0)
		{
			CBullet::Create(CBullet::BULLETTYPE_ENEMY, CBullet::BULLETTEX_ENEMY, 120, 13, 60.0f, 60.0f, Pos, BulletMove);
		}

		if (nPatternTime >= 180)
		{
			nPattern++;
			nPatternTime = 0;
		}
		//==================================================================================================================
		break;
	case 3:
		if (nPatternTime == 1)
		{
			Move.y = 50.0f;
		}

		if (Pos.y >= 300.0f)
		{

			if (nPatternTime < 180)
			{
				Move.y = 0.0f;
				Pos.y = 300.0f;
				//==========================
				//目的の位置までの３次元ベクトルを取る
				//==========================
				fVXaim = PlayerPos.x - Pos.x;
				fVYaim = PlayerPos.y - Pos.y;
				fVZaim = PlayerPos.z - Pos.z;

				fVLaim = sqrtf((fVXaim * fVXaim) + (fVYaim * fVYaim)
					+ (fVZaim * fVZaim));

				Rot.x = fVXaim / fVLaim * 20.0f;
				Rot.y = fVYaim / fVLaim * 20.0f;
				Rot.z = fVZaim / fVLaim * 20.0f;
			}
		}
		if (nPatternTime >= 180)
		{
			Move = Rot;

			if (Pos.z >= -20.0f && Pos.z <= 20.0f)
			{
				Move = NULL_VECTOR3;
				nPattern++;
				nPatternTime = 0;
				Pos.z = 0.0f;

				CAttackEnemy::Create(CAttack::ATTACKTYPE03_TORNADO, 11, 250, Pos, D3DXVECTOR3(sinf(D3DX_PI * 0.5f) * 7.0f, cosf(D3DX_PI * 0.5f) * 7.0f, 0.0f), ONE_VECTOR3 * 0.8f,
					NULL_VECTOR3, CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, D3DXVECTOR3(0.0f, 0.3f, 0.0f), true, 10);
				CAttackEnemy::Create(CAttack::ATTACKTYPE03_TORNADO, 11, 250, Pos, D3DXVECTOR3(sinf(D3DX_PI * -0.5f) * 7.0f, cosf(D3DX_PI * -0.5f) * 7.0f, 0.0f), ONE_VECTOR3 * 0.8f,
					NULL_VECTOR3, CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, D3DXVECTOR3(0.0f, -0.3f, 0.0f), true, 10);

			}
		}
		//==================================================================================================================
		break;
	case 4:
		if (nPatternTime >= 90)
		{
			ResetPattern();
			Rot = NULL_VECTOR3;
		}
		break;
	default:
		break;
	}
}
//===================================================================================================================================================

//============================================================
//キングスライムのアクション４
//============================================================
void CKingSlime::Action4()
{
	int& nPatternTime = GetPatternTime();                           //パターン時間を取得する
	bool& bAction = GetAction();                                    //行動しているかどうかを取得
	int& nPattern = GetPattern();                                   //現在のパターンを取得
	int& nActionNum = GetActionNum();                               //アクション回数をカウント

	D3DXVECTOR3& Pos = GetPos();
	D3DXVECTOR3& PlayerPos = CGame::GetPlayer()->GetPos();
	float fRandRot = 0.0f;
	float fRot = 0.0f;
	D3DXVECTOR3 SummonPos = NULL_VECTOR3;
	D3DXVECTOR3 RandMove = NULL_VECTOR3;
	CParticle* pParticle = nullptr;//パーティクル取得用
	CAttackEnemy* pAttackEnemy = nullptr;//敵攻撃取得用
	switch (nPattern)
	{
	case 0:
		if (nPatternTime >= 60)
		{
			for (int nCnt = 0; nCnt < 50; nCnt++)
			{
				RandMove.x = float(rand() % 150 - 75) / 5;
				RandMove.y = float(rand() % 150 - 75) / 5;
				RandMove.z = float(rand() % 150 - 75) / 5;
				pParticle = CParticle::Create(CParticle::TYPE00_NORMAL, 120, 60.0f, 60.0f, Pos, RandMove, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), true);
				pParticle->SetGravity(-0.2f);//重力を設定
				pParticle = nullptr;
			}

			nPattern++;
			nPatternTime = 0;
		}
		break;
	case 1:
		if (nPatternTime >= 60)
		{
			nPatternTime = 0;
			nPattern++;
		}
		break;
	case 2:
		if (nPatternTime == 1)
		{
			for (int nCnt = 0; nCnt < 6; nCnt++)
			{
				fRot = (D3DX_PI / 5) * nCnt;
				pAttackEnemy = CAttackEnemy::Create(CAttack::ATTACKTYPE02_NEEDLE, 8, 55, Pos, NULL_VECTOR3, D3DXVECTOR3(1.0f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, -D3DX_PI * 0.5f + fRot),
					CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, NULL_VECTOR3, true, 10);

				pAttackEnemy->SetUseAddScale(D3DXVECTOR3(0.0f, 0.1f, 0.0f),true);
			}
		}
		if (nPatternTime >= 80)
		{
			ResetPattern();
		}
		break;
	default:
		break;
	}
}
//================================================================================================================================

//=========================================================
//移動パターン
//=========================================================
void CKingSlime::MovePattern()
{
	CBoss::MovePattern();//移動パターンの共通処理
	int nMovePatternFrame = GetMovePatternFrame();
	bool& bUseMovePattern = GetUseMovePattern();//移動パターンを使用しているかどうか
	D3DXVECTOR3& Move = GetMove();
	
	Move.x = 0.0f;//移動パターン状態のときに動きを止める
	if (nMovePatternFrame > m_nMAX_MOVEPATTERNFRAME)
	{
		bUseMovePattern = false;
		nMovePatternFrame = 0;
	}
}
//================================================================================================================================

//============================================<<<デスエナガクラス>>>=============================================================================

//========================================================
//コンストラクタ
//========================================================
CDeathEnaga::CDeathEnaga()
{

}
//================================================================================================================================

//========================================================
//デストラクタ
//========================================================
CDeathEnaga::~CDeathEnaga()
{

}
//================================================================================================================================

//========================================================
//初期化処理
//========================================================
HRESULT CDeathEnaga::Init()
{
	CBoss::Init();            //ボスの初期化処理
	m_nPurposeMovePoint = 0;  //移動先の番号
	m_fMoveLength = 0.0f;     //移動する距離

	for (int nCnt = 0; nCnt < m_nMAX_TWINTORNADO; nCnt++)
	{
		m_apTwinTornado[nCnt] = nullptr;
	}
	return S_OK;
}
//================================================================================================================================

//========================================================
//終了処理
//========================================================
void CDeathEnaga::Uninit()
{
	CBoss::Uninit();
}
//================================================================================================================================

//========================================================
//更新処理
//========================================================
void CDeathEnaga::Update()
{
	CBoss::Update();
}
//================================================================================================================================

//========================================================
//描画処理
//========================================================
void CDeathEnaga::Draw()
{
	int nCntTime = GetCntTime();
	CBoss::Draw();
}
//================================================================================================================================

//========================================================
//死亡フラグを設定
//========================================================
void CDeathEnaga::SetDeath()
{
	for (int nCnt = 0; nCnt < m_nMAX_TWINTORNADO; nCnt++)
	{
		if (m_apTwinTornado[nCnt] != nullptr)
		{
			m_apTwinTornado[nCnt]->SetUseDeath(true);
			m_apTwinTornado[nCnt]->SetDeath();
			m_apTwinTornado[nCnt] = nullptr;
		}
	}

	CBoss::SetDeath();
}
//================================================================================================================================

//========================================================
//生成処理
//========================================================
CDeathEnaga* CDeathEnaga::Create(BOSSTYPE type, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 Scale)
{
	CDeathEnaga* pDeathEnaga = DBG_NEW CDeathEnaga;   //ボスを生成
	bool bSuccess = pDeathEnaga->CObject::GetCreateSuccess();
	int nIdx = 0;//モデルのインデックス
	if (bSuccess == true)
	{
		if (pDeathEnaga != nullptr)
		{
			pDeathEnaga->Init();                             //初期化処理
			pDeathEnaga->SetUseDeath(true);                  //死亡フラグを発動するかどうかを設定する
			pDeathEnaga->SetType(type);                      //ボスの種類
			pDeathEnaga->SetLife(nLife);                     //体力
			pDeathEnaga->SetUseGravity(false);               //重力を使わない
			pDeathEnaga->SetUseBlockCollision(false);        //ブロックとの当たり判定を使わない
			pDeathEnaga->SetMaxLife(nLife);                  //最大体力
			pDeathEnaga->SetAutoSubLife(false);              //自動的に体力を減らすかどうか
			pDeathEnaga->CObjectX::SetPos(pos);                                                                                  //オブジェクト２Ｄの位置を設定
			pDeathEnaga->SetPosOld(pos);                                                                                         //1f前の位置を設定
			pDeathEnaga->SetSupportPos(pos);                                                                                     //設置位置
			pDeathEnaga->CObject::SetType(CObject::TYPE_BOSS);                                                                   //オブジェクトの種類を決める
			pDeathEnaga->CObjectX::SetScale(Scale);                                                                              //オブジェクトの拡大率
			pDeathEnaga->SetFormarScale(Scale);                                                                                  //元の拡大率を設定する
			nIdx = CManager::GetObjectXInfo()->Regist(CObjectX::OBJECTXTYPE_BOSS, m_BOSS_FILENAME[(int)(type)]);
			pDeathEnaga->CObjectX::BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(CObjectX::OBJECTXTYPE_BOSS, nIdx),
				CManager::GetObjectXInfo()->GetBuffMat(CObjectX::OBJECTXTYPE_BOSS, nIdx),
				CManager::GetObjectXInfo()->GetdwNumMat(CObjectX::OBJECTXTYPE_BOSS, nIdx),
				CManager::GetObjectXInfo()->GetTexture(CObjectX::OBJECTXTYPE_BOSS, nIdx),
				CManager::GetObjectXInfo()->GetColorValue(CObjectX::OBJECTXTYPE_BOSS, nIdx));                                   //モデル情報を割り当てる
			pDeathEnaga->SetBossHpGauge(nLife);                                                                                  //体力ゲージを設定する
			pDeathEnaga->CObjectX::SetObjXType(CObjectX::OBJECTXTYPE_BOSS);                                                      //オブジェクトXのタイプを設定
			pDeathEnaga->CObjectX::SetTypeNum((int)(type));                                                                      //オブジェクトXごとのタイプ番号を設定
			pDeathEnaga->SetSize();                                                                                              //Xオブジェクトのサイズを設定する
			pDeathEnaga->CObjectX::SetRotType(CObjectX::ROTTYPE::ROTTYPE_NORMAL);                                                //向きの種類を設定
			pDeathEnaga->SetManagerType((int)(CStageManager::MANAGEROBJECT_BOSS));                                               //マネージャーで呼び出す時の種類を設定
		}
		else
		{//オブジェクトに空きがなかったので破棄する
			delete pDeathEnaga;
			pDeathEnaga = nullptr;
			return nullptr;
		}
	}
	return pDeathEnaga;
}
//================================================================================================================================

//========================================================
//移動パターン
//========================================================
void CDeathEnaga::MovePattern()
{
	int& nMovePatternFrame = GetMovePatternFrame();
	int& nMaxMovePatternFrame = GetMaxMovePatternFrame();//移動パターンの最大フレーム数
	bool& bUseMovePattern = GetUseMovePattern();//移動パターンを使用しているかどうか
	D3DXVECTOR3& Move = GetMove();
	D3DXVECTOR3 Pos = GetPos();
	float fLength = 0.0f;
	float fVLaim = 0.0f;//総合ベクトル
	float fVXaim = 0.0f;//X方向ベクトル
	float fVYaim = 0.0f;//Y方向ベクトル
	CEmitter* pEmitter = nullptr;
	CBoss::MovePattern();//移動パターンの共通処理

	int nNotPurpose = m_nPurposeMovePoint;//前回移動した目的地の番号を記録する

	if (nMovePatternFrame == 1)
	{//移動パターン開始時の処理
		nMaxMovePatternFrame = 100;//移動パターンの最大フレーム数を設定

		while (1)
		{
			m_nPurposeMovePoint = rand() % m_nMAX_MOVEPOINT;//移動先を設定する
			if (m_nPurposeMovePoint != nNotPurpose)
			{//移動場所が被らないようにする
				break;
			}
		}

		//移動位置までの距離を測る
		fLength = sqrtf(powf(m_MOVEPOINT[m_nPurposeMovePoint].x - Pos.x, 2) +
			powf(m_MOVEPOINT[m_nPurposeMovePoint].y - Pos.y, 2));

		m_fMoveLength = fLength / (float)(nMaxMovePatternFrame);//移動をするフレーム数で割り、1f当たりの移動量を求める

		//ワープ先にエミッターを召喚する
		pEmitter = CEmitter::Create((int)(CEffect::EFFECTTYPE00_NORMAL), nMaxMovePatternFrame, 80, 80.0f, 40.0f, 40.0f,
			m_MOVEPOINT[m_nPurposeMovePoint], NULL_VECTOR3, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		pEmitter->SetUseCurve(0.0f, 10.0f, 0.2f);
	}

	//XYベクトルを取る
	fVXaim = m_MOVEPOINT[m_nPurposeMovePoint].x - Pos.x;
	fVYaim = m_MOVEPOINT[m_nPurposeMovePoint].y - Pos.y;

	//総合ベクトルを求める
	fVLaim = atan2f(fVXaim, fVYaim);

	//目的の位置まで移動させる
	Move.x = sinf(fVLaim) * m_fMoveLength;
	Move.y = cosf(fVLaim) * m_fMoveLength;

	//移動パターンを終わらせる
	if (nMovePatternFrame > nMaxMovePatternFrame)
	{
		bUseMovePattern = false;
		nMovePatternFrame = 0;
		Move = NULL_VECTOR3;
	}
}
//================================================================================================================================

//========================================================
//行動パターン
//========================================================
void CDeathEnaga::ActionPattern()
{
	bool& bAction = GetAction();    //行動しているかどうか
	int& nSkill = GetSkill();       //スキル
	bool& bInsanity = GetInsanity();//発狂しているかどうか
	int nLife = GetLife();          //体力
	int& nCntTurn = GetTurnCnt();   //ターン数をカウント
	int nMaxLife = GetMaxLife();    //最大体力
	float fRatioLife = 0.0f;        //体力の割合

	fRatioLife = float(nLife) / float(nMaxLife);

	if (fRatioLife < 0.5f)
	{//体力が５０％を下回ったら発狂状態にする
		bInsanity = true;
	}

	if (bAction == false)
	{
		if (bInsanity == false)
		{//発狂状態じゃなければ
			nSkill = rand() % 3;
		}
		else
		{//発狂状態なら
			nCntTurn++;//ここからターン数をカウント
			nSkill = rand() % 5;
			if (nCntTurn == 1)
			{//発狂してから最初のターンに必殺技を発動！
				nSkill = 4;
			}
		}
		bAction = true;
	}

	if (bAction == true)
	{
		CBoss::ActionPattern();
		switch (nSkill)
		{
		case 0:
			Action0();
			break;
		case 1:
			Action1();
			break;
		case 2:
			Action2();
			break;
		case 3:
			Action3();
			break;
		case 4:
			Action4();
			break;
		default:
			break;
		}
	}

}
//================================================================================================================================

//========================================================
//行動０
//========================================================
void CDeathEnaga::Action0()
{
	int& nPatternTime = GetPatternTime();                           //パターン時間を取得する
	bool& bAction = GetAction();                                    //行動しているかどうかを取得
	int& nPattern = GetPattern();                                   //現在のパターンを取得
	int& nActionNum = GetActionNum();                               //アクション回数をカウント

	D3DXVECTOR3& Move = GetMove();                                 //移動量
	D3DXVECTOR3 SenterPos = GetSenterPos();                              //位置
	CBullet* pBullet = nullptr;                                    //バレットへのポインタ
	float fVLaim = 0.0f;
	float fVXaim = 0.0f;
	float fVYaim = 0.0f;

	D3DXVECTOR3 PlayerPos = CGame::GetPlayer()->GetPos() + D3DXVECTOR3(0.0f, CGame::GetPlayer()->GetSize().y / 2.0f, 0.0f);//プレイヤーの真ん中
	fVXaim = PlayerPos.x - SenterPos.x;
	fVYaim = PlayerPos.y - SenterPos.y;

	fVLaim = atan2f(fVXaim, fVYaim);

	switch (nPattern)
	{
	case 0:
		if (nPatternTime % 25 == 0 && nPatternTime <= 75)
		{
		    CBullet::Create(CBullet::BULLETTYPE_ENEMY, CBullet::BULLETTEX_ENEMY, 120, 1, 40.0f, 40.0f, SenterPos,
				D3DXVECTOR3(sinf(fVLaim) * 5.0f, cosf(fVLaim) * 5.0f, 0.0f));

			if (nPatternTime == 75)
			{
				CParticle::SummonChargeParticle(CParticle::TYPE00_NORMAL, 80, 120, 5.0f, 30.0f, 30.0f, 200, 100, 10, false, SenterPos, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f)
					, true);
			}
		}

		if (nPatternTime == 100)
		{
			for (int nCnt = 0; nCnt < 3; nCnt++)
			{
				pBullet = CBullet::Create(CBullet::BULLETTYPE_ENEMY, CBullet::BULLETTEX_ENEMY, 120, 1, 40.0f, 40.0f, SenterPos,
					D3DXVECTOR3(sinf(fVLaim - 0.6f + (0.6f * (float)(nCnt))) * 2.0f, 
						cosf(fVLaim - 0.6f + (0.6f * (float)(nCnt))) * 2.0f, 0.0f));

				pBullet->SetUseAddSpeed(true, true, 1.05f);
				pBullet->SetUseEffect(30, D3DXVECTOR2(40.0f, 40.0f), D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f));
			}
			nPatternTime = 0;//時間リセット
			nPattern++;      //パターンを進める
		}
		break;
	case 1:
		if (nPatternTime == 50)
		{
			ResetPattern();
		}
		break;
	default:
		break;
	}

}
//================================================================================================================================

//========================================================
//行動１
//========================================================
void CDeathEnaga::Action1()
{
	int& nPatternTime = GetPatternTime();         //パターン時間を取得する
	bool& bAction = GetAction();                  //行動しているかどうかを取得
	int& nPattern = GetPattern();                 //現在のパターンを取得
	int& nActionNum = GetActionNum();             //アクション回数をカウント

	D3DXVECTOR3& Pos = GetPos();                  //位置を取得
	D3DXVECTOR3& Move = GetMove();                //移動量を取得
	CBullet* pBullet = nullptr;                   //バレットへのポインタ

	float fVXaim = 0.0f;                          //Xベクトル
	float fVYaim = 0.0f;                          //Yベクトル
	float fVLaim = 0.0f;            			  //総合ベクトル

	int nRand = rand() % 2;                       //乱数
	switch (nPattern)
	{
	case 0://溜める
		if (nPatternTime == 30)
		{//赤チャージ
			CParticle::SummonChargeParticle(CParticle::TYPE00_NORMAL, 180, 120, 5.0f, 30.0f, 30.0f, 300, 100, 10, false,
				Pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), true);

			nPatternTime = 0;
			nPattern++;
		}
		break;
	case 1://溜め完了
		if (nPatternTime == 30)
		{
			nPatternTime = 0;
			nPattern++;
		}
		break;
	case 2://バレット発射
		for (int nCnt = 0; nCnt < 2; nCnt++)
		{
			float fRot = (2.0f / 2.0f) * float(nCnt);
			pBullet = CBullet::Create(CBullet::BULLETTYPE_ENEMY, CBullet::BULLETTEX_ENEMY, 180, 1, 40.0f, 40.0f,
				Pos + D3DXVECTOR3(0.0f, GetSize().y / 2.0f, 0.0f), NULL_VECTOR3);

			if (nRand  == 0)
			{
				pBullet->SetUseLengthCurve(true, D3DX_PI * fRot, 0.02f, 3.0f);
			}
			else
			{
				pBullet->SetUseLengthCurve(true, D3DX_PI * fRot, -0.02f, 3.0f);
			}
		}
		nPattern++;
		nPatternTime = 0;
		break;
	case 3://突進
		if (nPatternTime == 1)
		{
			Pos = D3DXVECTOR3(-500.0f,CGame::GetPlayer()->GetPos().y, 0.0f);
		}

		if (nPatternTime >= 40)
		{
			Move.x = 30.0f;
		}

		if (nPatternTime == 70)
		{
			Move.x = 0.0f;
			nPatternTime = 0;
			nPattern++;
		}
		break;
	case 4://突進
		if (nPatternTime == 1)
		{
			Pos = D3DXVECTOR3(500.0f, CGame::GetPlayer()->GetPos().y, 0.0f);
		}
		if (nPatternTime >= 30)
		{
			Move.x = -30.0f;
		}

		if (nPatternTime == 70)
		{
			Pos = NULL_VECTOR3 + D3DXVECTOR3(0.0f, 250.0f, 0.0f);
			CParticle::SummonParticle(CParticle::TYPE00_NORMAL, 40, 45, 30.0f, 30.0f, 100, 10, false,Pos, NORMAL_COL, true);
			ResetPattern();
		}
		break;
	default:
		break;
	}
}
//================================================================================================================================

//========================================================
//行動２
//========================================================
void CDeathEnaga::Action2()
{
	int& nPatternTime = GetPatternTime();         //パターン時間を取得する
	bool& bAction = GetAction();                  //行動しているかどうかを取得
	int& nPattern = GetPattern();                 //現在のパターンを取得
	int& nActionNum = GetActionNum();             //アクション回数をカウント

	D3DXVECTOR3& Pos = GetPos();                  //位置を取得
	D3DXVECTOR3& Move = GetMove();                //移動量を取得
	CBullet* pBullet = nullptr;                   //バレットへのポインタ

	switch (nPattern)
	{
	case 0://溜める
		if (nPatternTime == 30)
		{//青チャージ
			CParticle::SummonChargeParticle(CParticle::TYPE00_NORMAL, 180, 120, 5.0f, 30.0f, 30.0f, 300, 100, 10, false,
				Pos, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), true);

			nPatternTime = 0;
			nPattern++;
		}
		break;
	case 1://トルネード召喚
		if (nPatternTime == 30)
		{
			for (int nCntBullet = 0; nCntBullet < 5; nCntBullet++)
			{
				pBullet = CBullet::Create(CBullet::BULLETTYPE_ENEMY, CBullet::BULLETTEX_ENEMY, 180, 1, 20.0f, 20.0f, Pos + D3DXVECTOR3(0.0f, GetSize().y / 2.0f, 0.0f),
					D3DXVECTOR3(sinf(-0.6f + 0.3f * nCntBullet) * 10.0f, cosf(-0.6f + 0.3f * nCntBullet), 0.0f));

				pBullet->SetUseGravity(-0.4f);
			}

			for (int nCnt = 0; nCnt < m_nMAX_TWINTORNADO; nCnt++)
			{
				switch (nCnt)
				{
				case 0:
					m_apTwinTornado[nCnt] = CAttackEnemy::Create(CAttack::ATTACKTYPE03_TORNADO, 1, 180,
						Pos + D3DXVECTOR3(0.0f, GetSize().y / 2.0f, 0.0f), D3DXVECTOR3(sinf(0.1f) * 5.0f, cosf(0.1f) * 5.0f, 0.0f)
						, ONE_VECTOR3 * 1.0f, NULL_VECTOR3, CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, D3DXVECTOR3(0.0f, 0.4f, 0.0f), false, 10);
					break;
				case 1:
					m_apTwinTornado[nCnt] = CAttackEnemy::Create(CAttack::ATTACKTYPE03_TORNADO, 1, 180,
						Pos + D3DXVECTOR3(0.0f, GetSize().y / 2.0f, 0.0f), D3DXVECTOR3(sinf(-0.1f) * 5.0f, cosf(-0.1f) * 5.0f, 0.0f)
						, ONE_VECTOR3 * 1.0f, NULL_VECTOR3, CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, D3DXVECTOR3(0.0f, 0.4f, 0.0f), false, 10);
					break;
				case 2:
					m_apTwinTornado[nCnt] = CAttackEnemy::Create(CAttack::ATTACKTYPE03_TORNADO, 1, 180,
						Pos + D3DXVECTOR3(0.0f, GetSize().y / 2.0f, 0.0f), D3DXVECTOR3(sinf(0.1f + D3DX_PI) * 5.0f, cosf(0.1f + D3DX_PI) * 5.0f, 0.0f)
						, ONE_VECTOR3 * 1.0f, NULL_VECTOR3, CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, D3DXVECTOR3(0.0f, 0.4f, 0.0f), false, 10);
					break;
				case 3:
					m_apTwinTornado[nCnt] = CAttackEnemy::Create(CAttack::ATTACKTYPE03_TORNADO, 1, 180,
						Pos + D3DXVECTOR3(0.0f, GetSize().y / 2.0f, 0.0f), D3DXVECTOR3(sinf(-0.1f + D3DX_PI) * 5.0f, cosf(-0.1f + D3DX_PI) * 5.0f, 0.0f)
						, ONE_VECTOR3 * 1.0f, NULL_VECTOR3, CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, D3DXVECTOR3(0.0f, 0.4f, 0.0f), false, 10);
					break;
				case 4:
					m_apTwinTornado[nCnt] = CAttackEnemy::Create(CAttack::ATTACKTYPE03_TORNADO, 1, 180,
						Pos + D3DXVECTOR3(0.0f, GetSize().y / 2.0f, 0.0f), D3DXVECTOR3(sinf(0.1f) * 10.0f, cosf(0.1f) * 10.0f, 0.0f)
						, ONE_VECTOR3 * 1.0f, NULL_VECTOR3, CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, D3DXVECTOR3(0.0f, 0.4f, 0.0f), false, 10);
					break;
				case 5:
					m_apTwinTornado[nCnt] = CAttackEnemy::Create(CAttack::ATTACKTYPE03_TORNADO, 1, 180,
						Pos + D3DXVECTOR3(0.0f, GetSize().y / 2.0f, 0.0f), D3DXVECTOR3(sinf(-0.1f) * 10.0f, cosf(-0.1f) * 10.0f, 0.0f)
						, ONE_VECTOR3 * 1.0f, NULL_VECTOR3, CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, D3DXVECTOR3(0.0f, 0.4f, 0.0f), false, 10);
					break;
				case 6:
					m_apTwinTornado[nCnt] = CAttackEnemy::Create(CAttack::ATTACKTYPE03_TORNADO, 1, 180,
						Pos + D3DXVECTOR3(0.0f, GetSize().y / 2.0f, 0.0f), D3DXVECTOR3(sinf(0.1f + D3DX_PI) * 10.0f, cosf(0.1f + D3DX_PI) * 10.0f, 0.0f)
						, ONE_VECTOR3 * 1.0f, NULL_VECTOR3, CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, D3DXVECTOR3(0.0f, 0.4f, 0.0f), false, 10);
					break;
				case 7:
					m_apTwinTornado[nCnt] = CAttackEnemy::Create(CAttack::ATTACKTYPE03_TORNADO, 1, 180,
						Pos + D3DXVECTOR3(0.0f, GetSize().y / 2.0f, 0.0f), D3DXVECTOR3(sinf(-0.1f + D3DX_PI) * 10.0f, cosf(-0.1f + D3DX_PI) * 10.0f, 0.0f)
						, ONE_VECTOR3 * 1.0f, NULL_VECTOR3, CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, D3DXVECTOR3(0.0f, 0.4f, 0.0f), false, 10);
					break;
				default:
					break;
				}

				m_apTwinTornado[nCnt]->SetUseDeath(false);//死亡フラグをオフにする
			}
		}

		if (nPatternTime == 50)
		{//加速開始
			for (int nCnt = 0; nCnt < m_nMAX_TWINTORNADO; nCnt++)
			{
				if (nCnt % 2 == 0)
				{
					m_apTwinTornado[nCnt]->SetMove(D3DXVECTOR3(-1.0f,0.0f, 0.0f));
				}
				else
				{
					m_apTwinTornado[nCnt]->SetMove(D3DXVECTOR3(1.0f,0.0f, 0.0f));
				}

				m_apTwinTornado[nCnt]->SetUseMultiSpeed(true,ONE_VECTOR3 * 1.06f);
			}

			nPatternTime = 0;
			nPattern++;
		}
		break;
	case 2://パターン終了
		if (nPatternTime == 40)
		{
			for (int nCnt = 0; nCnt < m_nMAX_TWINTORNADO; nCnt++)
			{
				m_apTwinTornado[nCnt]->SetUseDeath(true);//トルネードを使い終わったので、死亡フラグをオンにする
				m_apTwinTornado[nCnt] = nullptr;
			}
			ResetPattern();
		}
		break;
	default:
		break;
	}
}
//================================================================================================================================

//========================================================
//行動３
//========================================================
void CDeathEnaga::Action3()
{
	int& nPatternTime = GetPatternTime();         //パターン時間を取得する
	bool& bAction = GetAction();                  //行動しているかどうかを取得
	int& nPattern = GetPattern();                 //現在のパターンを取得
	int& nActionNum = GetActionNum();             //アクション回数をカウント

	D3DXVECTOR3& Pos = GetPos();                  //位置を取得
	D3DXVECTOR3& PurposePos = GetPurposePos();    //目的の位置取得
	D3DXVECTOR3& Move = GetMove();                //移動量を取得
	CBullet* pBullet = nullptr;                   //バレットへのポインタ

	float fVXaim = 0.0f;                          //X方向ベクトル
	float fVYaim = 0.0f;                          //Y方向ベクトル
	float fVLaim = 0.0f;                          //目的への角度

	float fRandRot = 0.0f;                        //乱数向き	
	CAttackEnemy* pAttackEnemy = nullptr;         //敵攻撃へのポインタ

	fVXaim = PurposePos.x - Pos.x;
	fVYaim = PurposePos.y - Pos.y;
	fVLaim = atan2f(fVXaim, fVYaim);
	switch (nPattern)
	{
	case 0://溜める
		if (nPatternTime == 30)
		{//水色チャージ
			CParticle::SummonChargeParticle(CParticle::TYPE00_NORMAL, 180, 120, 5.0f, 30.0f, 30.0f, 300, 100, 10, false,
				Pos, D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), true);

			nPatternTime = 0;
			nPattern++;
		}
		break;
	case 1:
		if (nPatternTime == 1)
		{//目的地を現在のこの時点でのプレイヤーの位置に設定
			PurposePos = CGame::GetPlayer()->GetPos();
		}
		Move = D3DXVECTOR3(sinf(fVLaim) * 5.0f, cosf(fVLaim) * 5.0f, 0.0f);

		if (Pos.x > PurposePos.x - 10.0f && Pos.x < PurposePos.x + 10.0f &&
			Pos.y > PurposePos.y - 10.0f && Pos.y < PurposePos.y + 10.0f)
		{//目的地に達したら
			Move = NULL_VECTOR3;
			nPatternTime = 0;
			nPattern++;
		}
		break;
	case 2:
		if (nPatternTime == 30)
		{//赤チャージ
			CParticle::SummonChargeParticle(CParticle::TYPE00_NORMAL, 180, 120, 5.0f, 30.0f, 30.0f, 300, 100, 10,true,
				Pos, D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), true);
			nPatternTime = 0;
			nPattern++;
		}
		break;
	case 3://竜巻発射
		if (nPatternTime % 10 == 0)
		{
			if (nActionNum % 2 == 0)
			{
				fRandRot = float(rand() % 156 - 78) / 100 + D3DX_PI * 0.5f;
			}
			else
			{
				fRandRot = float(rand() % 156 - 78) / 100 + D3DX_PI * -0.5f;
			}
			pAttackEnemy = CAttackEnemy::Create(CAttack::ATTACKTYPE03_TORNADO, 1, 220,
				Pos, D3DXVECTOR3(sinf(fRandRot) * 10.0f,cosf(fRandRot) * 10.0f,0.0f), ONE_VECTOR3, NULL_VECTOR3,
					CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, D3DXVECTOR3(0.0f, 0.5f, 0.0f), false, 100);

			if (nActionNum % 2 == 0)
			{
				pAttackEnemy->SetUseAddSpeed(true,D3DXVECTOR3(-0.2f, 0.0f, 0.0f));
				pAttackEnemy->SetColor(CObjectX::COLORTYPE_BRUE, 300);
			}
			else
			{
				pAttackEnemy->SetUseAddSpeed(true, D3DXVECTOR3(0.2f, 0.0f, 0.0f));
				pAttackEnemy->SetColor(CObjectX::COLORTYPE_GREEN, 300);
			}
			nActionNum++;
		}

		if (nActionNum == 20)
		{
			ResetPattern();
		}
		break;
	}
}
//================================================================================================================================

//========================================================
//行動４
//========================================================
void CDeathEnaga::Action4()
{
	int& nPatternTime = GetPatternTime();         //パターン時間を取得する
	bool& bAction = GetAction();                  //行動しているかどうかを取得
	int& nPattern = GetPattern();                 //現在のパターンを取得
	int& nActionNum = GetActionNum();             //アクション回数をカウント

	D3DXVECTOR3& Pos = GetPos();                  //位置を取得
	D3DXVECTOR3& PurposePos = GetPurposePos();    //目的の位置取得
	D3DXVECTOR3& Move = GetMove();                //移動量を取得

	D3DXVECTOR3 PlayerPos = CGame::GetPlayer()->GetPos();//プレイヤーの位置
	D3DXVECTOR3 PlayerSize = CGame::GetPlayer()->GetSize();//プレイヤーのサイズ
	D3DXVECTOR3 PlayerSenterPos = PlayerPos + D3DXVECTOR3(0.0f, PlayerSize.y * 0.5f, 0.0f);

	float fVXaim = 0.0f;                          //X方向ベクトル
	float fVYaim = 0.0f;                          //Y方向ベクトル
	float fVLaim = 0.0f;                          //目的への角度

	float fRandRot = 0.0f;                        //乱数向き	

	CParticle* pParticle = nullptr;               //パーティクルへのポインタ
	CBullet* pBullet = nullptr;                   //バレットへのポインタ
	CAttackEnemy* pAttackEnemy = nullptr;         //敵攻撃へのポインタ

	fVXaim = PurposePos.x - Pos.x;
	fVYaim = PurposePos.z - Pos.z;
	fVLaim = atan2f(fVXaim, fVYaim);

	//慣性をオフにする
	SetUseInteria(false);

	switch (nPattern)
	{
	case 0://溜める
		if (nPatternTime == 30)
		{//黄色チャージ
			CParticle::SummonChargeParticle(CParticle::TYPE00_NORMAL, 180, 120, 5.0f, 30.0f, 30.0f, 300, 100, 10, false,
				Pos, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), true);

			nPatternTime = 0;
			nPattern++;
		}
		break;
	case 1://カメラの視点に突進
		if (nPatternTime == 1)
		{
			PurposePos = CManager::GetCamera()->GetPosV();
		}

		if (nPatternTime == 20)
		{
			Move = D3DXVECTOR3(sinf(fVLaim) * 15.0f,0.0f,cosf(fVLaim) * 15.0f);
			nPatternTime = 0;
			nPattern++;
		}
		break;
	case 2://カメラの後ろ側に着いたら
		if (Pos.z < CGame::GetPlayer()->GetPos().z - 650.0f)
		{
			nPatternTime = 0;
			nPattern++;
			Move = NULL_VECTOR3;
		}
		break;
	case 3:
		Pos.x = CGame::GetPlayer()->GetPos().x;
		Pos.y = CGame::GetPlayer()->GetPos().y;

		if (nPatternTime < 100)
		{//突進予定位置にパーティクルを召喚
			fRandRot = float(rand() % 628) / 100;
			pParticle = CParticle::Create(CParticle::TYPE00_NORMAL, 40, 40.0f, 40.0f,
				CGame::GetPlayer()->GetPos() + D3DXVECTOR3(PlayerSenterPos.x + sinf(fRandRot) * 100.0f,
					PlayerSenterPos.y + cosf(fRandRot) * 100.0f, 0.0f),
				D3DXVECTOR3(sinf(fRandRot) * 15.0f, cosf(fRandRot) * 15.0f, 0.0f), D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), true);

			pParticle->SetUseAddSpeed(true, true, 1.03f);
		}

		if (nPatternTime == 120)
		{
			Move.z = 10.0f;//前方に超加速
			nPatternTime = 0;
			nPattern++;
		}
		break;
	case 4://目的地に着いたら

		if (Pos.z > -20.0f && Pos.z < 20.0f && nActionNum < 1)
		{//Zが原点の近くに来たら
			nActionNum++;
			for (int nCnt = 0; nCnt < 16; nCnt++)
			{
				fRandRot = (2.0f / 16.0f) * float(nCnt);
				CBullet::Create(CBullet::BULLETTYPE_ENEMY, CBullet::BULLETTEX_ENEMY, 140, 10, 40.0f, 40.0f,
					D3DXVECTOR3(Pos.x, Pos.y, 0.0f),
					D3DXVECTOR3(sinf(D3DX_PI * fRandRot) * 10.0f, cosf(D3DX_PI * fRandRot) * 10.0f, 0.0f));
			}
		}

		if (Pos.z > 600.0f && Pos.z < 640.0f)
		{
			Move = NULL_VECTOR3;
			nPatternTime = 0;
			nActionNum = 0;
			nPattern++;
		}
		break;
	case 5://必殺技チャージ
		if (nPatternTime <= 120)
		{//黄色チャージ
			CParticle::SummonChargeParticle(CParticle::TYPE00_NORMAL, 10, 120, 10.0f, 30.0f, 30.0f, 400, 100, 10, true,
				Pos + D3DXVECTOR3(0.0f, GetSize().y * 0.5f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), true);
		}
		else
		{
			nPatternTime = 0;
			nPattern++;
		}
		break;
	case 6://必殺技
		if (nPatternTime % 30 == 0)
		{
			for (int nCnt = 0; nCnt < 8; nCnt++)
			{
				if (nActionNum % 2 == 0)
				{//普通に撃つ
					fRandRot = (2.0f / 8.0f) * float(nCnt);
				}
				else
				{//少しずらして撃つ
					fRandRot = (2.0f / 8.0f) * float(nCnt) + ((2.0f / 8.0f) / 2.0f);
				}
				CAttackEnemy::Create(CAttack::ATTACKTYPE03_TORNADO, 12, 300,
					Pos - D3DXVECTOR3(0.0f,300.0f,0.0f), D3DXVECTOR3(sinf(D3DX_PI * fRandRot) * 15.0f, 0.0f, cosf(D3DX_PI * fRandRot) * 15.0f), D3DXVECTOR3(2.0f, 10.0f, 2.0f),
					NULL_VECTOR3, CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, D3DXVECTOR3(0.0f, 0.6f, 0.0f), false, 120);
			}
			nActionNum++;//攻撃回数を増やす
		}

		if (nActionNum == 7)
		{
			nPatternTime = 0;
			nActionNum = 0;
			nPattern++;
		}
		break;
	case 7://座標を戻す
		Move.z = -20.0f;

		if (Pos.z > -20.0f && Pos.z < 20.0f)
		{
			SetInsanity(true);
			Move = NULL_VECTOR3;
			SetUseInteria(true);
			ResetPattern();
		}
		break;
	default:
		break;
	}
}
//===================================================================================================================================================

//============================================<<<ミスター・ストロングクラス>>>=============================================================================

//==================================================================
//コンストラクタ
//==================================================================
CMrStrong::CMrStrong()
{

}
//======================================================================================================================================

//==================================================================
//デストラクタ
//==================================================================
CMrStrong::~CMrStrong()
{

}
//======================================================================================================================================

//==================================================================
//初期化処理
//==================================================================
HRESULT CMrStrong::Init()
{
	CBoss::Init();
	m_pBurstMeteo = nullptr;//バースとメテオへのポインタ
	return S_OK;
}
//======================================================================================================================================

//==================================================================
//終了処理
//==================================================================
void CMrStrong::Uninit()
{
	CBoss::Uninit();
}
//======================================================================================================================================

//==================================================================
//更新処理
//==================================================================
void CMrStrong::Update()
{
	D3DXVECTOR3 Pos = GetPos();
	CBoss::Update();
	CManager::GetDebugText()->PrintDebugText("ボスの位置：%f %f %f\n", Pos.x, Pos.y, Pos.z);
}
//======================================================================================================================================

//==================================================================
//描画処理
//==================================================================
void CMrStrong::Draw()
{
	int nCntTime = GetCntTime();
	CBoss::Draw();
}
//======================================================================================================================================

//==================================================================
//死亡フラグ設定処理
//==================================================================
void CMrStrong::SetDeath()
{
	if (m_pBurstMeteo != nullptr)
	{
		m_pBurstMeteo->SetUseDeath(true);
		m_pBurstMeteo->SetDeath();
		m_pBurstMeteo = nullptr;
	}
	CBoss::SetDeath();
}
//======================================================================================================================================

//==================================================================
//生成処理
//==================================================================
CMrStrong* CMrStrong::Create(BOSSTYPE type, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 Scale)
{
	CMrStrong* pMrStrong = DBG_NEW CMrStrong;   //ボスを生成
	bool bSuccess = pMrStrong->CObject::GetCreateSuccess();
	int nIdx = 0;//モデルのインデックス
	if (bSuccess == true)
	{
		if (pMrStrong != nullptr)
		{
			pMrStrong->Init();                             //初期化処理
			pMrStrong->SetUseDeath(true);                  //死亡フラグを発動するかどうかを設定する
			pMrStrong->SetType(type);                      //ボスの種類
			pMrStrong->SetLife(nLife);                     //体力
			pMrStrong->SetMaxLife(nLife);//最大体力
			pMrStrong->SetAutoSubLife(false);              //自動的に体力を減らすかどうか
			pMrStrong->CObjectX::SetPos(pos);                                                                                  //オブジェクト２Ｄの位置を設定
			pMrStrong->SetPosOld(pos);                                                                                         //1f前の位置を設定
			pMrStrong->SetSupportPos(pos);                                                                                     //設置位置
			pMrStrong->CObject::SetType(CObject::TYPE_BOSS);                                                                   //オブジェクトの種類を決める
			pMrStrong->CObjectX::SetScale(Scale);                                                                              //オブジェクトの拡大率
			pMrStrong->SetFormarScale(Scale);                                                                                  //元の拡大率を設定する
			nIdx = CManager::GetObjectXInfo()->Regist(CObjectX::OBJECTXTYPE_BOSS, m_BOSS_FILENAME[(int)(type)]);
			pMrStrong->CObjectX::BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(CObjectX::OBJECTXTYPE_BOSS, nIdx),
				CManager::GetObjectXInfo()->GetBuffMat(CObjectX::OBJECTXTYPE_BOSS, nIdx),
				CManager::GetObjectXInfo()->GetdwNumMat(CObjectX::OBJECTXTYPE_BOSS, nIdx),
				CManager::GetObjectXInfo()->GetTexture(CObjectX::OBJECTXTYPE_BOSS, nIdx),
				CManager::GetObjectXInfo()->GetColorValue(CObjectX::OBJECTXTYPE_BOSS, nIdx));                                   //モデル情報を割り当てる
			pMrStrong->SetBossHpGauge(nLife);                                                                                  //体力ゲージを設定する
			pMrStrong->CObjectX::SetObjXType(CObjectX::OBJECTXTYPE_BOSS);                                                      //オブジェクトXのタイプを設定
			pMrStrong->CObjectX::SetTypeNum((int)(type));                                                                      //オブジェクトXごとのタイプ番号を設定
			pMrStrong->SetSize();                                                                                              //Xオブジェクトのサイズを設定する
			pMrStrong->CObjectX::SetRotType(CObjectX::ROTTYPE::ROTTYPE_NORMAL);                                                //向きの種類を設定
			pMrStrong->SetManagerType((int)(CStageManager::MANAGEROBJECT_BOSS));                                               //マネージャーで呼び出す時の種類を設定
			pMrStrong->AdjustSize();      //判定サイズ調整を行う
		}
		else
		{//オブジェクトに空きがなかったので破棄する
			delete pMrStrong;
			pMrStrong = nullptr;
			return nullptr;
		}
	}
	return pMrStrong;
}
//======================================================================================================================================

//==================================================================
//移動パターン
//==================================================================
void CMrStrong::MovePattern()
{
	int& nMovePatternFrame = GetMovePatternFrame();
	int& nMaxMovePatternFrame = GetMaxMovePatternFrame();
	bool& bUseMovePattern = GetUseMovePattern();//移動パターンを使用しているかどうか
	int& nMovePatternNum = GetMovePatternNum();//移動パターン番号を取得する
	D3DXVECTOR3& Move = GetMove();
	D3DXVECTOR3 Pos = GetPos();
	float fLength = 0.0f;

	CBoss::MovePattern();//移動パターンの共通処理

	DecideMovePattern();//移動パターン決定処理

	//移動パターンを開始する
	switch (nMovePatternNum)
	{
	case 0:
		MovePattern0();//移動パターン０
		break;
	case 1:
		MovePattern1();//移動パターン１
		break;
	default:
		break;
	}

	//移動パターンを終わらせる
	if (nMovePatternFrame > nMaxMovePatternFrame)
	{
		bUseMovePattern = false;//アクションパターンへ移行
		nMovePatternFrame = 0;
		Move = NULL_VECTOR3;
	}

}
//======================================================================================================================================

//==================================================================
//移動パターン決定処理
//==================================================================
void CMrStrong::DecideMovePattern()
{
	int& nMovePatternFrame = GetMovePatternFrame();
	int& nMaxMovePatternFrame = GetMaxMovePatternFrame();
	bool& bUseMovePattern = GetUseMovePattern();//移動パターンを使用しているかどうか
	int& nMovePatternNum = GetMovePatternNum();//移動パターン番号を取得する
	D3DXVECTOR3 Pos = GetPos();

	if (nMovePatternFrame == 1)
	{//最初に移動方法を決める
		nMovePatternNum = rand() % 2;

		if (Pos.x > 250.0f && CGame::GetPlayer()->GetPos().x > Pos.x)
		{//壁際にプレイヤーが追い込まれていたら・・・
			nMovePatternNum = 1;
		}
		else if (Pos.x < -250.0f && CGame::GetPlayer()->GetPos().x < Pos.x)
		{//壁際にプレイヤーが追い込まれていたら・・・
			nMovePatternNum = 1;
		}

		//それぞれの移動パターンの最大フレーム数を設定する
		switch (nMovePatternNum)
		{
		case 0:
			nMaxMovePatternFrame = 100;
			break;
		case 1:
			nMaxMovePatternFrame = 90;
			break;
		default:
			break;
		}
	}

}
//======================================================================================================================================

//==================================================================
//行動パターン
//==================================================================
void CMrStrong::ActionPattern()
{
	bool& bAction = GetAction();    //行動しているかどうか
	int& nSkill = GetSkill();       //スキル
	bool& bInsanity = GetInsanity();//発狂しているかどうか
	int nLife = GetLife();          //体力
	int& nCntTurn = GetTurnCnt();   //ターン数をカウント
	int nMaxLife = GetMaxLife();    //最大体力
	float fRatioLife = 0.0f;        //体力の割合

	fRatioLife = float(nLife) / float(nMaxLife);

	if (fRatioLife < 0.5f)
	{//体力が５０％を下回ったら発狂状態にする
		bInsanity = true;
	}

	if (bAction == false)
	{
		if (bInsanity == false)
		{//発狂状態じゃなければ
			nSkill = rand() % 4;
			//nSkill = 2;
		}
		else
		{//発狂状態なら
			nCntTurn++;//ここからターン数をカウント
			nSkill = rand() % 6;
			if (nCntTurn == 1)
			{//発狂してから最初のターンに必殺技を発動！
				nSkill = 5;
			}
		}
		bAction = true;
	}

	if (bAction == true)
	{
		CBoss::ActionPattern();
		switch (nSkill)
		{
		case 0:
			Action0();
			break;
		case 1:
			Action1();
			break;
		case 2:
			Action2();
			break;
		case 3:
			Action4();
			break;
		case 4:
			Action3();
			break;
		case 5:
			Action5();
			break;
		default:
			break;
		}
	}

}
//======================================================================================================================================

//==================================================================
//移動０（ゆっくり接近）
//==================================================================
void CMrStrong::MovePattern0()
{

	D3DXVECTOR3& Move = GetMove();
	D3DXVECTOR3& Pos = GetPos();
	D3DXVECTOR3 PlayerPos = CGame::GetPlayer()->GetPos();

	if (Pos.x < PlayerPos.x)
	{//プレイヤーが右側にいたら
		Move.x = 1.0f;
	}
	else
	{//プレイヤーが左側にいたら
		Move.x = -1.0f;
	}
	
}
//======================================================================================================================================

//==================================================================
//サイズを調整する
//==================================================================
void CMrStrong::AdjustSize()
{
	D3DXVECTOR3& VtxMax = GetOriginVtxMax();
	D3DXVECTOR3& VtxMin = GetOriginVtxMin();

	VtxMax.x -= 45.0f;
	VtxMin.x += 45.0f;
}
//======================================================================================================================================

//==================================================================
//移動１（ジャンプ踏みつけ）
//==================================================================
void CMrStrong::MovePattern1()
{
	D3DXVECTOR3& Move = GetMove();
	D3DXVECTOR3& Pos = GetPos();
	D3DXVECTOR3 PlayerPos = CGame::GetPlayer()->GetPos();
	int nMovePatternFrame = GetMovePatternFrame();//移動パターンフレーム
	SetUseInteria(false);//慣性オフ
	if (nMovePatternFrame == 1)
	{
		if (Pos.x < PlayerPos.x)
		{//プレイヤーが右側にいたら
			Move.x = 5.0f;
			Move.y = 25.0f;
		}
		else
		{//プレイヤーが左側にいたら
			Move.x = -5.0f;
			Move.y = 25.0f;
		}
	}
	if (nMovePatternFrame > 2)
	{
		if (Pos.y < 0.1f)
		{
			Move = NULL_VECTOR3;
		}
	}

	if (nMovePatternFrame == 89)
	{
		SetUseInteria(true);//慣性オン
	}

}
//======================================================================================================================================

//==================================================================
//行動０（岩投げ攻撃）
//==================================================================
void CMrStrong::Action0()
{
	int& nPatternTime = GetPatternTime();         //パターン時間を取得する
	bool& bAction = GetAction();                  //行動しているかどうかを取得
	int& nPattern = GetPattern();                 //現在のパターンを取得
	int& nActionNum = GetActionNum();             //アクション回数をカウント

	D3DXVECTOR3& Pos = GetPos();                  //位置を取得
	D3DXVECTOR3 PlayerPos = CGame::GetPlayer()->GetPos();//プレイヤーの位置
	CBullet* pBullet = nullptr;                   //バレットへのポインタ
	float fRandRot = 0.0f;                        //乱数向き	
	float fRandSpeed = 0.0f;                      //乱数速度
	CAttackEnemy* pAttackEnemy = nullptr;         //敵攻撃へのポインタ
	switch (nPattern)
	{
	case 0://溜める
		if (nPatternTime == 30)
		{//赤チャージ
			CParticle::SummonChargeParticle(CParticle::TYPE00_NORMAL, 180, 120, 5.0f, 30.0f, 30.0f, 300, 100, 10, false,
				Pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), true);

			nPatternTime = 0;
			nPattern++;
		}
		break;
	case 1://隕石発射
		if (nPatternTime == 40)
		{
			for (int nCnt = 0; nCnt < 6; nCnt++)
			{
				if (Pos.x < PlayerPos.x)
				{//プレイヤーが右側にいたら・・・
					fRandRot = float(rand() % 157) / -100 + D3DX_PI * 0.5f;
				}
				else
				{//プレイヤーが左側にいたら・・・
					fRandRot = float(rand() % 157) / 100 - D3DX_PI * 0.5f;
				}
				fRandSpeed = 5.0f + float(rand() % 100) / 10;
				pAttackEnemy = CAttackEnemy::Create(CAttack::ATTACKTYPE01_METEO, 10, 120, Pos, D3DXVECTOR3(sinf(fRandRot) * fRandSpeed, cosf(fRandRot) * fRandSpeed, 0.0f), ONE_VECTOR3,
					NULL_VECTOR3, CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, ONE_VECTOR3 * 0.05f, true, 30);

				pAttackEnemy->SetUseGravity(-0.4f);//重力をかける
			}

			nPatternTime = 0;
			nPattern++;
		}
		break;
	case 2://クールタイム
		if (nPatternTime == 20)
		{
			ResetPattern();
		}
		break;
	default:
		break;

	}
}
//======================================================================================================================================

//==================================================================
//行動１（ワンツー衝撃波）
//==================================================================
void CMrStrong::Action1()
{
	int& nPatternTime = GetPatternTime();         //パターン時間を取得する
	bool& bAction = GetAction();                  //行動しているかどうかを取得
	int& nPattern = GetPattern();                 //現在のパターンを取得
	int& nActionNum = GetActionNum();             //アクション回数をカウント

	D3DXVECTOR3& Pos = GetPos();                  //位置を取得
	D3DXVECTOR3 PlayerPos = CGame::GetPlayer()->GetPos();//プレイヤーの位置
	CBullet* pBullet = nullptr;                   //バレットへのポインタ
	float fRandRot = 0.0f;                        //乱数向き	
	float fRandSpeed = 0.0f;                      //乱数速度
	CAttackEnemy* pAttackEnemy = nullptr;         //敵攻撃へのポインタ
	switch (nPattern)
	{
	case 0://溜める
		if (nPatternTime == 45)
		{//緑チャージ
			CParticle::SummonChargeParticle(CParticle::TYPE00_NORMAL, 180, 120, 5.0f, 30.0f, 30.0f, 300, 100, 10, false,
				Pos, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), true);

			nPatternTime = 0;
			nPattern++;
		}
		break;
	case 1:
		if (nPatternTime == 60)
		{
			nPatternTime = 0;
			nPattern++;
		}
		break;
	case 2:
		if (nPatternTime % 20 == 0)
		{
			if (nActionNum % 2 == 0)
			{
				pAttackEnemy = CAttackEnemy::Create(CAttack::ATTACKTYPE10_SHOCKWAVE, 20, 150, Pos, D3DXVECTOR3(1.0f, 0.0f, 0.0f), ONE_VECTOR3 * 4.0f, NULL_VECTOR3,
					CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, NULL_VECTOR3, false, 60);

				pAttackEnemy->SetUseMultiSpeed(true, D3DXVECTOR3(1.1f, 0.0f, 0.0f));
			}
			else
			{
				pAttackEnemy = CAttackEnemy::Create(CAttack::ATTACKTYPE10_SHOCKWAVE, 20, 150, Pos, D3DXVECTOR3(-1.0f, 0.0f, 0.0f), ONE_VECTOR3 * 4.0f,D3DXVECTOR3(0.0f,D3DX_PI,0.0f),
					CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, NULL_VECTOR3, false, 60);

				pAttackEnemy->SetUseMultiSpeed(true, D3DXVECTOR3(1.1f, 0.0f, 0.0f));
			}
			nActionNum++;
		}

		if (nActionNum == 12)
		{
			ResetPattern();
		}
		break;
	default:
		break;
	}
}
//======================================================================================================================================

//==================================================================
//行動２（放物線プレス）
//==================================================================
void CMrStrong::Action2()
{
	int& nPatternTime = GetPatternTime();         //パターン時間を取得する
	bool& bAction = GetAction();                  //行動しているかどうかを取得
	int& nPattern = GetPattern();                 //現在のパターンを取得
	int& nActionNum = GetActionNum();             //アクション回数をカウント

	D3DXVECTOR3& Pos = GetPos();                  //位置を取得
	D3DXVECTOR3& Move = GetMove();                //移動量を取得
	D3DXVECTOR3 PlayerPos = CGame::GetPlayer()->GetPos();//プレイヤーの位置
	CBullet* pBullet = nullptr;                   //バレットへのポインタ
	float fRandRot = 0.0f;                        //乱数向き	
	float fRandSpeed = 0.0f;                      //乱数速度
	float fLength = 0.0f;                         //距離

	//プレイヤーとボスの距離を求める
	fLength = sqrtf(powf(PlayerPos.x - Pos.x, 2) + powf(PlayerPos.y - Pos.y, 2));

	CAttackEnemy* pAttackEnemy = nullptr;         //敵攻撃へのポインタ
	switch (nPattern)
	{
	case 0://溜める
		if (nPatternTime == 45)
		{//青チャージ
			CParticle::SummonChargeParticle(CParticle::TYPE00_NORMAL, 180, 120, 5.0f, 30.0f, 30.0f, 300, 100, 10, false,
				Pos, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), true);

			nPatternTime = 0;
			nPattern++;
		}
		break;
	case 1://放物線プレス
		if (nPatternTime == 5)
		{
			SetUseInteria(false);             //慣性を使用しない
			fRandRot = CCalculation::CalculationParabola(CCalculation::CalculationLength(Pos, PlayerPos),GetNormalGravity(), 25.0f, Pos, PlayerPos);
			Move = D3DXVECTOR3(sinf(fRandRot) * 25.0f, cosf(fRandRot) * 25.0f, 0.0f);
		}

		if (nPatternTime > 5)
		{
			if (Pos.y < 0.1f)
			{
				pAttackEnemy = CAttackEnemy::Create(CAttack::ATTACKTYPE10_SHOCKWAVE, 20, 150, Pos, D3DXVECTOR3(1.0f, 0.0f, 0.0f),
					ONE_VECTOR3 * 3.5f + (ONE_VECTOR3 * 0.2f) * float(nActionNum), NULL_VECTOR3,
					CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, NULL_VECTOR3, false, 60);
				pAttackEnemy->SetUseMultiSpeed(true, D3DXVECTOR3(1.1f, 0.0f, 0.0f));

				pAttackEnemy = CAttackEnemy::Create(CAttack::ATTACKTYPE10_SHOCKWAVE, 20, 150, Pos, D3DXVECTOR3(-1.0f, 0.0f, 0.0f),
					ONE_VECTOR3 * 3.5f + (ONE_VECTOR3 * 0.2f) * float(nActionNum), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
					CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, NULL_VECTOR3, false, 60);

				pAttackEnemy->SetUseMultiSpeed(true, D3DXVECTOR3(1.1f, 0.0f, 0.0f));

				nPatternTime = 0;
				nActionNum++;
				Move = NULL_VECTOR3;
			}
		}

		if (nActionNum == 5)
		{//5回跳ねる
			SetUseInteria(true);//慣性を使用する
			Move = NULL_VECTOR3;
			nPatternTime = 0;
			nPattern++;
			nActionNum = 0;
		}
		break;
	case 2://クールタイム
		if (nPatternTime == 45)
		{
			ResetPattern();
		}
		break;
	default:
		break;
	}

}
//======================================================================================================================================

//==================================================================
//行動３（バーストメテオ）
//==================================================================
void CMrStrong::Action3()
{
	int& nPatternTime = GetPatternTime();         //パターン時間を取得する
	bool& bAction = GetAction();                  //行動しているかどうかを取得
	int& nPattern = GetPattern();                 //現在のパターンを取得
	int& nActionNum = GetActionNum();             //アクション回数をカウント

	D3DXVECTOR3& Pos = GetPos();                  //位置を取得
	D3DXVECTOR3 AttackMove = NULL_VECTOR3;        //攻撃の移動量
	D3DXVECTOR3 PlayerPos = CGame::GetPlayer()->GetPos();//プレイヤーの位置
	CBullet* pBullet = nullptr;                   //バレットへのポインタ
	float fRandRot = 0.0f;                        //乱数向き	
	float fRandSpeed = 0.0f;                      //乱数速度
	float fLength = 0.0f;                         //距離

	//プレイヤーとボスの距離を求める
	fLength = sqrtf(powf(PlayerPos.x - Pos.x, 2) + powf(PlayerPos.y - Pos.y, 2));

	//放物線移動関係
	float fXG = fLength * GetNormalGravity();      //（距離、重力）
	float fV = 35.0f;                             //移動速度
	float fV2 = powf(fV, 2);                      //移動速度の２条
	float fResult = 0.0f;                         //計算結果
	CAttackEnemy* pAttackEnemy = nullptr;         //敵攻撃へのポインタ

	switch (nPattern)
	{
	case 0://溜める
		if (nPatternTime == 15)
		{//紫チャージ
			CParticle::SummonChargeParticle(CParticle::TYPE00_NORMAL, 180, 120, 5.0f, 30.0f, 30.0f, 300, 100, 10, false,
				Pos, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), true);
		}

		if (nPatternTime == 60)
		{
			nPatternTime = 0;
			nPattern++;
		}
		break;
	case 1:
		fResult = asinf(fXG / fV2);       //距離から逆算して発射角度を割り出す
		fRandRot = fResult / 2.0f;
		if (PlayerPos.x < Pos.x)
		{
			AttackMove = D3DXVECTOR3(sinf(-fRandRot) * fV, cosf(-fRandRot) * fV, 0.0f);
		}
		else
		{
			AttackMove = D3DXVECTOR3(sinf(fRandRot) * fV, cosf(fRandRot) * fV, 0.0f);
		}

		m_pBurstMeteo = CAttackEnemy::Create(CAttack::ATTACKTYPE01_METEO, 10, 150, Pos, AttackMove, ONE_VECTOR3 * 2.0f, NULL_VECTOR3,
			CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, D3DXVECTOR3(0.0f, 1.1f, 0.5f), false, 45);
		m_pBurstMeteo->SetUseGravity(-GetNormalGravity());
		m_pBurstMeteo->SetUseDeath(false);//死亡フラグを発動させない。

		nPatternTime = 0;
		nPattern++;
		break;
	case 2://着弾時に分裂させる
		if (m_pBurstMeteo->GetPos().y < 0.1f)
		{
			for (int nCnt = 0; nCnt < 4; nCnt++)
			{
				fRandRot = float(rand() % 100 - 50) / 100;
				fRandSpeed = float(rand() % 100) / 10.0f + 7.0f;
				pAttackEnemy = CAttackEnemy::Create(CAttack::ATTACKTYPE01_METEO, 10, 120, m_pBurstMeteo->GetPos(),
					D3DXVECTOR3(sinf(fRandRot) * fRandSpeed, cosf(fRandRot) * fRandSpeed, 0.0f), ONE_VECTOR3 * 0.5f, NULL_VECTOR3,
					CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, NULL_VECTOR3, true, 50);

				pAttackEnemy->SetUseGravity(-0.5f);
				pAttackEnemy->SetUseEffect(CEffect::EFFECTTYPE00_NORMAL, true, D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), 40, D3DXVECTOR2(30.0f, 30.0f));//エフェクトを使用
			}

			if (m_pBurstMeteo != nullptr)
			{
				m_pBurstMeteo->SetUseDeath(true);//死亡フラグを使用する
				m_pBurstMeteo->SetDeath();       //死亡フラグを設定する
				m_pBurstMeteo = nullptr;         //ポインタを初期化
			}

			if (nActionNum == 4)
			{
				nActionNum = 0;
				nPattern++;
			}
			else
			{
				nActionNum++;//アクション回数を増やす
				nPattern = 1;//パターンを戻す
			}
			nPatternTime = 0;//パターン時間を戻す
		}
		break;
	case 3://クールタイム
		if (nPatternTime == 60)
		{
			ResetPattern();
		}
		break;
	default:
		break;
	}
}
//======================================================================================================================================

//==================================================================
//行動４（追い詰めバレット）	
//==================================================================
void CMrStrong::Action4()
{
	int& nPatternTime = GetPatternTime();         //パターン時間を取得する
	bool& bAction = GetAction();                  //行動しているかどうかを取得
	int& nPattern = GetPattern();                 //現在のパターンを取得
	int& nActionNum = GetActionNum();             //アクション回数をカウント

	D3DXVECTOR3& Pos = GetPos();                  //位置を取得
	D3DXVECTOR3 SummonPos = NULL_VECTOR3;         //召喚位置
	D3DXVECTOR3& Scale = GetScale();              //拡大率
	D3DXVECTOR3 PlayerPos = CGame::GetPlayer()->GetPos();//プレイヤーの位置
	CBullet* pBullet = nullptr;                   //バレットへのポインタ
	float fRandRot = 0.0f;                        //乱数向き	
	float fRandSpeed = 0.0f;                      //乱数速度
	float fLength = 0.0f;                         //距離

	float fVXaim = CGame::GetPlayer()->GetPos().x - Pos.x;
	float fVYaim = CGame::GetPlayer()->GetPos().y - Pos.y;
	float fVLaim = atan2f(fVXaim,fVYaim);//目的地への角度
	switch (nPattern)
	{
	case 0://溜める
		if (nPatternTime == 15)
		{//水色チャージ
			CParticle::SummonChargeParticle(CParticle::TYPE00_NORMAL, 180, 120, 5.0f, 30.0f, 30.0f, 300, 100, 10, false,
				Pos, D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), true);
		}

		if (nPatternTime == 60)
		{
			nPatternTime = 0;
			nPattern++;
		}
		break;
	case 1://巨大化
		if (nPatternTime < 50)
		{
			Scale += ONE_VECTOR3 * 0.1f;
		}
		else
		{
			nPatternTime = 0;
			nPattern++;
		}
		break;
	case 2:
		if (nPatternTime < 45)
		{
			CParticle::SummonChargeParticle(CParticle::TYPE00_NORMAL, 5, 60, 5.0f, 30.0f, 30.0f, 300, 100, 10, false,
				Pos, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), true);
		}

		if (nPatternTime == 60)
		{
			nPatternTime = 0;
			nPattern++;
		}
		break;
	case 3://バレット発射
		if (nPatternTime < 120)
		{
			if (nPatternTime % 10 == 0)
			{
				SummonPos = D3DXVECTOR3(Pos.x, Pos.y + GetVtxMax().y, Pos.z);
				fVXaim = PlayerPos.x - SummonPos.x;
				fVYaim = (CGame::GetPlayer()->GetSize().y / 2.0f + PlayerPos.y) - SummonPos.y;
				fVLaim = atan2f(fVXaim, fVYaim);//目的地への角度

				for (int nCnt = 0; nCnt < 5; nCnt++)
				{
					pBullet =  CBullet::Create(CBullet::BULLETTYPE_ENEMY, CBullet::BULLETTEX_ENEMY, 100, 1, 30.0f,30.0f, SummonPos,
						D3DXVECTOR3(sinf(fVLaim - 0.6f + 0.3f * float(nCnt)) * 12.0f, cosf(fVLaim - 0.6f + 0.3f * float(nCnt)) * 12.0f, 0.0f));
					pBullet->SetUseEffect(20, D3DXVECTOR2(30.0f, 30.0f), D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
				}
			}
		}
		else
		{
			nPattern++;
			nPatternTime = 0;
		}
		break;
	case 4://拡大率を元に戻す
		if (nPatternTime < 50)
		{
			Scale -= ONE_VECTOR3 * 0.1f;
		}
		else
		{
			Scale = GetFormarScale();
			ResetPattern();
		}
		break;
	default:
		break;
	}
}
//======================================================================================================================================

//==================================================================
//行動５（ヘルフォール）	
//==================================================================
void CMrStrong::Action5()
{
	int& nPatternTime = GetPatternTime();         //パターン時間を取得する
	bool& bAction = GetAction();                  //行動しているかどうかを取得
	int& nPattern = GetPattern();                 //現在のパターンを取得
	int& nActionNum = GetActionNum();             //アクション回数をカウント
	int& nSubPattern = GetSubPattern();           //サブパターンを取得
	D3DXVECTOR3& PurposePos = GetPurposePos();    //目的地を設定する

	D3DXVECTOR3& Pos = GetPos();                  //位置を取得
	D3DXVECTOR3& Move = GetMove();                //移動量を取得
	D3DXVECTOR3 PlayerPos = CGame::GetPlayer()->GetPos();//プレイヤーの位置

	CBullet* pBullet = nullptr;                   //バレットへのポインタ
	CAttackEnemy* pAttackEnemy = nullptr;         //敵攻撃へのポインタ
	CMrStrongAttack* pMrStrongAttack = nullptr;   //ミスターストロングの攻撃
	float fVXaim = 0.0f;//Xベクトル
	float fVYaim = 0.0f;//Yベクトル
	float fVLaim = 0.0f;//目的地への角度
	switch (nPattern)
	{
	case 0://溜める
		if (nPatternTime == 15)
		{//虹色チャージ！
			SetUseInteria(false);
			CParticle::SummonChargeParticle(CParticle::TYPE00_NORMAL, 180, 120, 5.0f, 30.0f, 30.0f, 300, 100, 10, false,
				Pos, D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), true);
		}
		SetUseInteria(false);
		if (nPatternTime == 60)
		{
			nPatternTime = 0;
			nPattern++;
		}
		break;
	case 1:
		if (nPatternTime == 1)
		{
			PurposePos = D3DXVECTOR3(0.0f,120.0f,0.0f);
			nPatternTime = 0;
			nPattern++;
		}
		break;
	case 2://目的地へ移動
		fVXaim = PurposePos.x - Pos.x;
		fVYaim = PurposePos.y - Pos.y;
		fVLaim = atan2f(fVXaim, fVYaim);

		SetUseGravity(false);

		Move = D3DXVECTOR3(sinf(fVLaim) * 5.0f, cosf(fVLaim) * 5.0f, 0.0f);//中心へ移動

		if (Pos.x > PurposePos.x - 5.0f && Pos.x < PurposePos.x + 5.0f &&
			Pos.y > PurposePos.y - 5.0f && Pos.y < PurposePos.y + 5.0f)
		{//目的地へたどり着いた
			Move = NULL_VECTOR3;
			nPatternTime = 0;
			nPattern++;
		}
		break;
	case 3://もう一度チャージ
		if (nPatternTime < 60)
		{//赤色チャージ！
			CParticle::SummonChargeParticle(CParticle::TYPE00_NORMAL, 2, 120, 5.0f, 30.0f, 30.0f, 300, 100, 10, false,
				Pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), true);
		}
		else
		{
			nPatternTime = 0;
			nPattern++;
		}
		break;
	case 4:
		if (nSubPattern % 2 == 0)
		{
			if (nPatternTime % 2 == 0)
			{
				pAttackEnemy = CAttackEnemy::Create(CAttack::ATTACKTYPE01_METEO, 10, 100, Pos,
					D3DXVECTOR3(sinf(D3DX_PI * 0.5f - 0.1f * float(nActionNum)) * 15.0f, cosf(D3DX_PI * 0.5f - 0.1f * float(nActionNum)) * 15.0f, 0.0f),
					ONE_VECTOR3 * 2.0f, NULL_VECTOR3, CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, NULL_VECTOR3, true, 60);

				pAttackEnemy->SetUseGravity(-0.4f);

				nActionNum++;
			}

			if (nActionNum % 30 == 0 && nActionNum > 0)
			{
				nPatternTime = 0;
				nSubPattern++;
				nActionNum = 0;
			}
		}
		else
		{
			if (nPatternTime % 2 == 0)
			{
				pAttackEnemy = CAttackEnemy::Create(CAttack::ATTACKTYPE01_METEO, 10, 100, Pos,
					D3DXVECTOR3(sinf(-D3DX_PI * 0.5f + 0.1f * float(nActionNum)) * 15.0f, cosf(-D3DX_PI * 0.5f + 0.1f * float(nActionNum)) * 15.0f, 0.0f),
					ONE_VECTOR3 * 2.0f, NULL_VECTOR3, CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, NULL_VECTOR3, true, 60);

				pAttackEnemy->SetUseGravity(-0.4f);

				nActionNum++;
			}

			if (nActionNum % 30 == 0 && nActionNum > 0)
			{
				nPatternTime = 0;
				nSubPattern++;
				nActionNum = 0;
			}
		}

		if (nSubPattern == 5)
		{
			nPatternTime = 0;
			nSubPattern = 0;
			nActionNum = 0;
			nPattern++;
		}
		break;
	case 5://奥へ移動
		Move.z = 10.0f;

		if (Pos.z > 500.0f && Pos.z < 520.0f)
		{
			Move = NULL_VECTOR3;
			nPatternTime = 0;
			nPattern++;
		}
		break;
	case 6:
		if (nPatternTime % 120 == 0)
		{
			float fSummonWidth = BOSSSTAGE_WIDTH / 10;
			int nRand = rand() % 10;
			for (int nCnt = 0; nCnt < 10; nCnt++)
			{
				if (nRand != nCnt)
				{
					pMrStrongAttack = CMrStrongAttack::Create(CAttack::ATTACKTYPE17_BLOCK,10,600,Pos,NULL_VECTOR3,ONE_VECTOR3 * 2,NULL_VECTOR3,
						CAttack::COLLISIONTYPE02_TRUERECTANGLEXY,NULL_VECTOR3,false,20);

					pMrStrongAttack->SetPurposePos(D3DXVECTOR3(-BOSSSTAGE_HALFWIDTH + fSummonWidth * nCnt + fSummonWidth / 2, 300.0f,Pos.z));//ブロックを召喚
				}
			}
		}

		if (nPatternTime == 700)
		{
			nPattern++;
			nPatternTime = 0;
		}
		break;
	case 7:
		Move.z = -10.0f;

		if (Pos.z > -10.0f && Pos.z < 10.0f)
		{
			nPattern++;
			nPatternTime = 0;
			Move = NULL_VECTOR3;
		}
		break;
	case 8:
		if (nPatternTime == 90)
		{
			SetUseInteria(true);
			SetUseGravity(true);
			ResetPattern();
		}
		break;
	default:
		break;	      
	}
}
//======================================================================================================================================

//============================================<<<ライリークラス>>>=============================================================================

//==================================================================
//コンストラクタ
//==================================================================
CRiley::CRiley()
{

}
//======================================================================================================================================

//==================================================================
//デストラクタ
//==================================================================
CRiley::~CRiley()
{

}
//======================================================================================================================================

//==================================================================
//初期化処理
//==================================================================
HRESULT CRiley::Init()
{
	CBoss::Init();
	m_pSnowRamming = nullptr;//体当たりの時に使う攻撃

	for (int nCnt = 0; nCnt < m_nMAX_REFLECTIONSNOWBURST; nCnt++)
	{
		m_pReflectionSnowBurst[nCnt] = nullptr;
	}
	m_bSwitchScale = false;
	return S_OK;
}
//======================================================================================================================================

//==================================================================
//終了処理
//==================================================================
void CRiley::Uninit()
{
	CBoss::Uninit();
}
//======================================================================================================================================

//==================================================================
//更新処理
//==================================================================
void CRiley::Update()
{
	CBoss::Update();
}
//======================================================================================================================================

//==================================================================
//描画処理
//==================================================================
void CRiley::Draw()
{
	int nCntTime = GetCntTime();
	CBoss::Draw();
}
//======================================================================================================================================

//==================================================================
//死亡フラグ設定処理
//==================================================================
void CRiley::SetDeath()
{
	if (m_pSnowRamming != nullptr)
	{
		m_pSnowRamming->SetUseDeath(true);
		m_pSnowRamming->SetDeath();
		m_pSnowRamming = nullptr;
	}

	for (int nCnt = 0; nCnt < m_nMAX_REFLECTIONSNOWBURST; nCnt++)
	{
		if (m_pReflectionSnowBurst[nCnt] != nullptr)
		{
			m_pReflectionSnowBurst[nCnt]->SetUseDeath(true);
			m_pReflectionSnowBurst[nCnt]->SetDeath();
			m_pReflectionSnowBurst[nCnt] = nullptr;
		}
	}

	CBoss::SetDeath();
}
//======================================================================================================================================

//==================================================================
//生成処理
//==================================================================
CRiley* CRiley::Create(BOSSTYPE type, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 Scale)
{
	CRiley* pRiley = DBG_NEW CRiley;   //ボスを生成
	bool bSuccess = pRiley->GetCreateSuccess();
	int nIdx = 0;//モデルのインデックス
	if (bSuccess == true)
	{
		if (pRiley != nullptr)
		{
			pRiley->Init();                             //初期化処理
			pRiley->SetUseDeath(true);                  //死亡フラグを発動するかどうかを設定する
			pRiley->SetType(type);                      //ボスの種類
			pRiley->SetLife(nLife);                     //体力
			pRiley->SetMaxLife(nLife);//最大体力
			pRiley->SetAutoSubLife(false);              //自動的に体力を減らすかどうか
			pRiley->CObjectX::SetPos(pos);                                                                                  //オブジェクト２Ｄの位置を設定
			pRiley->SetPosOld(pos);                                                                                         //1f前の位置を設定
			pRiley->SetSupportPos(pos);                                                                                     //設置位置
			pRiley->CObject::SetType(CObject::TYPE_BOSS);                                                                   //オブジェクトの種類を決める
			pRiley->CObjectX::SetScale(Scale);                                                                              //オブジェクトの拡大率
			pRiley->SetFormarScale(Scale);                                                                                  //元の拡大率を設定する
			nIdx = CManager::GetObjectXInfo()->Regist(CObjectX::OBJECTXTYPE_BOSS, m_BOSS_FILENAME[(int)(type)]);
			pRiley->CObjectX::BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(CObjectX::OBJECTXTYPE_BOSS, nIdx),
				CManager::GetObjectXInfo()->GetBuffMat(CObjectX::OBJECTXTYPE_BOSS, nIdx),
				CManager::GetObjectXInfo()->GetdwNumMat(CObjectX::OBJECTXTYPE_BOSS, nIdx),
				CManager::GetObjectXInfo()->GetTexture(CObjectX::OBJECTXTYPE_BOSS, nIdx),
				CManager::GetObjectXInfo()->GetColorValue(CObjectX::OBJECTXTYPE_BOSS, nIdx));                                   //モデル情報を割り当てる
			pRiley->SetBossHpGauge(nLife);                                                                                  //体力ゲージを設定する
			pRiley->CObjectX::SetObjXType(CObjectX::OBJECTXTYPE_BOSS);                                                      //オブジェクトXのタイプを設定
			pRiley->CObjectX::SetTypeNum((int)(type));                                                                      //オブジェクトXごとのタイプ番号を設定
			pRiley->SetSize();                                                                                              //Xオブジェクトのサイズを設定する
			pRiley->CObjectX::SetRotType(CObjectX::ROTTYPE::ROTTYPE_NORMAL);                                                //向きの種類を設定
			pRiley->SetManagerType((int)(CStageManager::MANAGEROBJECT_BOSS));                                               //マネージャーで呼び出す時の種類を設定
			pRiley->AdjustSize();      //判定サイズ調整を行う
		}
		else
		{//オブジェクトに空きがなかったので破棄する
			delete pRiley;
			pRiley = nullptr;
			return nullptr;
		}
	}
	return pRiley;
}
//======================================================================================================================================

//==================================================================
//移動パターン
//==================================================================
void CRiley::MovePattern()
{
	int& nMovePatternFrame = GetMovePatternFrame();
	int& nMaxMovePatternFrame = GetMaxMovePatternFrame();
	bool& bUseMovePattern = GetUseMovePattern();//移動パターンを使用しているかどうか
	int& nMovePatternNum = GetMovePatternNum();//移動パターン番号を取得する
	D3DXVECTOR3& Move = GetMove();
	D3DXVECTOR3 Pos = GetPos();

	CBoss::MovePattern();//移動パターンの共通処理

	DecideMovePattern();//移動パターン決定処理

	//移動パターンを開始する
	switch (nMovePatternNum)
	{
	case 0:
		MovePattern0();//移動パターン０
		break;
	case 1:
		MovePattern1();//移動パターン１
		break;
	default:
		break;
	}

	//移動パターンを終わらせる
	if (nMovePatternFrame > nMaxMovePatternFrame)
	{
		bUseMovePattern = false;//アクションパターンへ移行
		nMovePatternFrame = 0;
		Move = NULL_VECTOR3;
	}

}
//======================================================================================================================================

//==================================================================
//移動パターンを決める処理
//==================================================================
void CRiley::DecideMovePattern()
{
	int& nMovePatternFrame = GetMovePatternFrame();
	int& nMaxMovePatternFrame = GetMaxMovePatternFrame();
	bool& bUseMovePattern = GetUseMovePattern();//移動パターンを使用しているかどうか
	int& nMovePatternNum = GetMovePatternNum();//移動パターン番号を取得する
	D3DXVECTOR3 Pos = GetPos();
	D3DXVECTOR3 PlayerPos = CGame::GetPlayer()->GetPos();

	//距離を取る
	float fLength = CCalculation::CalculationLength(Pos,PlayerPos);

	if (nMovePatternFrame == 1)
	{//最初に移動方法を決める

		if (fLength < m_fMOVEPATTERNLENGTH)
		{//距離が近ければ
			nMovePatternNum = 0;
		}
		else
		{//距離が遠ければ
			nMovePatternNum = 1;
		}

		//それぞれの移動パターンの最大フレーム数を設定する
		switch (nMovePatternNum)
		{
		case 0:
			nMaxMovePatternFrame = m_nMAX_MOVEPATTERN_0_FRAME;
			break;
		case 1:
			nMaxMovePatternFrame = m_nMAX_MOVEPATTERN_1_FRAME;
			break;
		default:
			break;
		}
	}
}
//======================================================================================================================================

//==================================================================
//移動０（距離を取る）
//==================================================================
void CRiley::MovePattern0()
{
	D3DXVECTOR3& Move = GetMove();
	D3DXVECTOR3& Pos = GetPos();
	D3DXVECTOR3 PlayerPos = CGame::GetPlayer()->GetPos();
	int nMovePatternFrame = GetMovePatternFrame();

	SetInertia(0.05f);//慣性を小さくする
	if (nMovePatternFrame == 30)
	{
		if (Pos.x > PlayerPos.x)
		{
			Move.x = 10.0f;
		}
		else
		{
			Move.x = -10.0f;
		}
	}

	if (nMovePatternFrame == m_nMAX_MOVEPATTERN_0_FRAME)
	{
		SetUndoInertia();//慣性を元に戻す
		nMovePatternFrame = 0;
	}
}
//======================================================================================================================================


//==================================================================
//移動１（近づく）
//==================================================================
void CRiley::MovePattern1()
{
	D3DXVECTOR3& Move = GetMove();
	D3DXVECTOR3& Pos = GetPos();
	D3DXVECTOR3 PlayerPos = CGame::GetPlayer()->GetPos();
	int nMovePatternFrame = GetMovePatternFrame();

	SetInertia(0.05f);//慣性を小さくする
	if (nMovePatternFrame == 30)
	{
		if (Pos.x > PlayerPos.x)
		{
			Move.x = -10.0f;
		}
		else
		{
			Move.x = 10.0f;
		}
	}

	if (nMovePatternFrame == m_nMAX_MOVEPATTERN_0_FRAME)
	{
		nMovePatternFrame = 0;
		SetUndoInertia();//慣性を元に戻す
	}
}
//======================================================================================================================================

//==================================================================
//行動パターン
//==================================================================
void CRiley::ActionPattern()
{
	bool& bAction = GetAction();    //行動しているかどうか
	int& nSkill = GetSkill();       //スキル
	bool& bInsanity = GetInsanity();//発狂しているかどうか
	int nLife = GetLife();          //体力
	int& nCntTurn = GetTurnCnt();   //ターン数をカウント
	int nMaxLife = GetMaxLife();    //最大体力
	float fRatioLife = 0.0f;        //体力の割合

	fRatioLife = float(nLife) / float(nMaxLife);

	if (fRatioLife < 0.5f)
	{//体力が５０％を下回ったら発狂状態にする
		bInsanity = true;
	}

	if (bAction == false)
	{
		if (bInsanity == false)
		{//発狂状態じゃなければ
			nSkill = rand() % 4;
			//nSkill = 5;//必殺技を発動
		}
		else
		{//発狂状態なら
			nCntTurn++;//ここからターン数をカウント
			nSkill = rand() % 6;
			if (nCntTurn == 1)
			{
				nSkill = 5;//必殺技を発動
			}
		}
		bAction = true;
	}

	if (bAction == true)
	{
		CBoss::ActionPattern();
		switch (nSkill)
		{
		case 0:
			Action0();
			break;
		case 1:
			Action1();
			break;
		case 2:
			Action2();
			break;
		case 3:
			Action4();
			break;
		case 4:
			Action3();
			break;
		case 5:
			Action5();
			break;
		default:
			break;
		}
	}
}
//======================================================================================================================================

//==================================================================
//サイズを調整する
//==================================================================
void CRiley::AdjustSize()
{
	D3DXVECTOR3& VtxMax = GetOriginVtxMax();
	D3DXVECTOR3& VtxMin = GetOriginVtxMin();

	VtxMax.x -= 20.0f;
	VtxMin.x += 20.0f;
	VtxMax.y -= 10.0f;

}
//======================================================================================================================================

//==================================================================
//行動０
//==================================================================
void CRiley::Action0()
{
	int& nPatternTime = GetPatternTime();         //パターン時間を取得する
	bool& bAction = GetAction();                  //行動しているかどうかを取得
	int& nPattern = GetPattern();                 //現在のパターンを取得
	int& nActionNum = GetActionNum();             //アクション回数をカウント

	D3DXVECTOR3& Pos = GetPos();                  //位置を取得
	D3DXVECTOR3& Rot = GetRot();                  //向きを取得
	D3DXVECTOR3& Move = GetMove();                //移動量を取得
	D3DXVECTOR3 PlayerPos = CGame::GetPlayer()->GetPos();//プレイヤーの位置
	CBullet* pBullet = nullptr;                   //バレットへのポインタ
	float fRandRot = 0.0f;                        //乱数向き	
	float fRandSpeed = 0.0f;                      //乱数速度
	float fLength = 0.0f;                         //距離

	//放物線移動関係
	switch (nPattern)
	{
	case 0://溜める
		if (nPatternTime == 45)
		{//青チャージ
			CParticle::SummonChargeParticle(CParticle::TYPE00_NORMAL, 120, 120, 5.0f, 30.0f, 30.0f, 300, 100, 10, false,
				Pos, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), true);

			nPatternTime = 0;
			nPattern++;
			m_pSnowRamming = CRileyAttack::Create(CAttack::ATTACKTYPE11_EXPLOSION, 20, 360, Pos, NULL_VECTOR3, ONE_VECTOR3, NULL_VECTOR3,
				CAttack::COLLISIONTYPE03_BALL, ONE_VECTOR3 * 0.1f, false, 10);
			m_pSnowRamming->SetUseDeath(false);
		}
		break;
	case 1:
		SetUseInteria(false);
		m_pSnowRamming->SetPos(Pos);
		if (Pos.x > PlayerPos.x)
		{
			Move.x -= 0.1f;
			Rot.z += 0.2f;
		}
		else
		{
			Move.x += 0.1f;
			Rot.z -= 0.2f;
		}
		if (nPatternTime % 45 == 0)
		{
			m_bSwitchScale = m_bSwitchScale ? false : true;
			nActionNum++;
		}

		if (m_bSwitchScale == false)
		{
			m_pSnowRamming->SetUseAddScale(ONE_VECTOR3 * 0.25f, true);
		}
		else
		{
			m_pSnowRamming->SetUseAddScale(ONE_VECTOR3 * -0.25f, true);
		}

		if (nActionNum == 6)
		{
			m_pSnowRamming->SetUseDeath(true);
			m_pSnowRamming->SetDeath();
			m_pSnowRamming = nullptr;
			Move = NULL_VECTOR3;
			Rot = NULL_VECTOR3;
			SetUseInteria(true);
			ResetPattern();
		}

		break;
	default:
		break;
	}
}
//=====================================================================================================================================

//==================================================================
//行動１（クロス体当たり）
//==================================================================
void CRiley::Action1()
{
	int& nPatternTime = GetPatternTime();         //パターン時間を取得する
	bool& bAction = GetAction();                  //行動しているかどうかを取得
	int& nPattern = GetPattern();                 //現在のパターンを取得
	int& nActionNum = GetActionNum();             //アクション回数をカウント
	int& nSubPattern = GetSubPattern();           //サブパターンを取得
	bool bIsLanding = GetLanding();               //地面にいるかどうかを取得
	D3DXVECTOR3& Pos = GetPos();                  //位置を取得
	D3DXVECTOR3& Rot = GetRot();                  //向きを取得
	D3DXVECTOR3& Move = GetMove();                //移動量を取得
	D3DXVECTOR3 PlayerPos = CGame::GetPlayer()->GetPos();//プレイヤーの位置
	CBullet* pBullet = nullptr;                   //バレットへのポインタ

	if (m_pSnowRamming != nullptr)
	{
		m_pSnowRamming->SetPos(Pos);//常にボスの位置に固定
	}
	SetUseInteria(false);
	switch (nPattern)
	{
	case 0://溜める
		if (nPatternTime < 15)
		{//赤チャージ
			CParticle::SummonChargeParticle(CParticle::TYPE00_NORMAL, 10, 120, 5.0f, 30.0f, 30.0f, 300, 100, 10, false,
				Pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), true);

			if (nPatternTime == 1)
			{
				m_pSnowRamming = CRileyAttack::Create(CAttack::ATTACKTYPE11_EXPLOSION, 20, 360, Pos, NULL_VECTOR3, ONE_VECTOR3, NULL_VECTOR3,
					CAttack::COLLISIONTYPE03_BALL, ONE_VECTOR3 * 0.1f, false, 10);
				m_pSnowRamming->SetUseDeath(false);
				m_pSnowRamming->SetUseAddScale(ONE_VECTOR3 * 0.12f, true);
			}
		}
		
		if (nPatternTime == 60)
		{
			m_pSnowRamming->SetUseAddScale(NULL_VECTOR3, false);
			nPattern++;
			nPatternTime = 0;
		}
		break;
	case 1:
		if (nPatternTime == 1)
		{
			if (Pos.x < PlayerPos.x)
			{//プレイヤーが右側にいる
				Move.x = 12.0f + 1.5f * float(nActionNum);
			}
			else
			{//プレイヤーが左側にいる
				Move.x = -12.0f - 1.5f * float(nActionNum);
			}
		}

		if (nPatternTime > 5)
		{
			if (Pos.x + GetVtxMax().x > BOSSSTAGE_HALFWIDTH || Pos.x + GetVtxMin().x < -BOSSSTAGE_HALFWIDTH)//画面端に到着したら・・・
			{
				CParticle::SummonChargeParticle(CParticle::TYPE00_NORMAL, 120, 120, 5.0f, 30.0f, 30.0f, 300, 100, 10, false,
					Pos, NORMAL_COL, true);//現在の位置にパーティクルを召喚
				Pos = PlayerPos + D3DXVECTOR3(0.0f, 400.0f, 0.0f);//プレイヤーの頭上にワープ
				Move = NULL_VECTOR3;//動きを止める
				CParticle::SummonParticle(CParticle::TYPE00_NORMAL, 100, 60, 30.0f, 30.0f, 100, 10, false, Pos, NORMAL_COL, true);//ワープ先にパーティクルを召喚

				nPattern++;
				nPatternTime = 0;
			}
		}
		break;
	case 2:
		if (bIsLanding == true)
		{
			if (nActionNum < 6)
			{
				nActionNum++;
				nPatternTime = 0;
				nPattern = 1;
			}
			else
			{
				m_pSnowRamming->SetUseDeath(true);
				m_pSnowRamming->SetDeath();
				m_pSnowRamming = nullptr;
				SetUseInteria(true);
				ResetPattern();
			}
		}
		break;
	default:
		break;
	}

}
//=====================================================================================================================================

//==================================================================
//行動２（反射バースト）
//==================================================================
void CRiley::Action2()
{
	int& nPatternTime = GetPatternTime();         //パターン時間を取得する
	bool& bAction = GetAction();                  //行動しているかどうかを取得
	int& nPattern = GetPattern();                 //現在のパターンを取得
	int& nActionNum = GetActionNum();             //アクション回数をカウント
	int& nSubPattern = GetSubPattern();           //サブパターンを取得
	bool bIsLanding = GetLanding();               //地面にいるかどうかを取得
	D3DXVECTOR3& Pos = GetPos();                  //位置を取得
	D3DXVECTOR3& Rot = GetRot();                  //向きを取得
	D3DXVECTOR3& Move = GetMove();                //移動量を取得
	D3DXVECTOR3 PlayerPos = CGame::GetPlayer()->GetPos();//プレイヤーの位置
	float fRandRot = 0.0f;
	bool bNextPattern = false;
	if (m_pSnowRamming != nullptr)
	{
		m_pSnowRamming->SetPos(Pos);//常にボスの位置に固定
	}
	switch (nPattern)
	{
	case 0://溜める
		if (nPatternTime == 30)
		{//緑チャージ
			CParticle::SummonChargeParticle(CParticle::TYPE00_NORMAL, 100, 120, 5.0f, 30.0f, 30.0f, 300, 100, 10, false,
				Pos, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), true);
		}

		if (nPatternTime == 80)
		{
			nPattern++;
			nPatternTime = 0;
		}
		break;
	case 1:
		for (int nCnt = 0; nCnt < m_nMAX_REFLECTIONSNOWBURST; nCnt++)
		{
			fRandRot = CCalculation::CalculationRandVecXY();
			m_pReflectionSnowBurst[nCnt] = CRileyAttack::Create(CAttack::ATTACKTYPE11_EXPLOSION, 12, 240, GetSenterPos(),
				D3DXVECTOR3(sinf(fRandRot) * 10.0f, cosf(fRandRot) * 10.0f, 0.0f), ONE_VECTOR3 * 2, NULL_VECTOR3, CAttack::COLLISIONTYPE03_BALL, NULL_VECTOR3, false, 10);
			m_pReflectionSnowBurst[nCnt]->SetUseDeath(false);
			m_pReflectionSnowBurst[nCnt]->SetUseMultiSpeed(true, ONE_VECTOR3 * 0.99f);
			m_pReflectionSnowBurst[nCnt]->SetUseReflection(true);
			m_pReflectionSnowBurst[nCnt]->SetUseBlockCollision(true);
		}

		nPattern++;
		nPatternTime = 0;
		break;
	case 2:
		bNextPattern = true;
		for (int nCntSnow = 0; nCntSnow < m_nMAX_REFLECTIONSNOWBURST; nCntSnow++)
		{
			if (m_pReflectionSnowBurst[nCntSnow] != nullptr)
			{
				if (m_pReflectionSnowBurst[nCntSnow]->GetLife() < 2)
				{
					for (int nCnt = 0; nCnt < m_nMAX_ACTION2BULLETNUM; nCnt++)
					{
						fRandRot = (2.0f / m_nMAX_ACTION2BULLETNUM) * nCnt;
						CBullet::Create(CBullet::BULLETTYPE_ENEMY, CBullet::BULLETTEX_ENEMY, 120, 10, 24.0f, 24.0f,
							m_pReflectionSnowBurst[nCntSnow]->GetSenterPos(), D3DXVECTOR3(sinf(D3DX_PI * fRandRot) * 12.0f, cosf(D3DX_PI * fRandRot) * 12.0f, 0.0f));
					}

					m_pReflectionSnowBurst[nCntSnow]->SetUseDeath(true);
					m_pReflectionSnowBurst[nCntSnow]->SetDeath();
					m_pReflectionSnowBurst[nCntSnow] = nullptr;
				}
			}

			if (m_pReflectionSnowBurst[nCntSnow] != nullptr)
			{
				bNextPattern = false;
			}
		}
		if (bNextPattern == true)
		{
			ResetPattern();
		}
		break;
	default:
		break;
	}
}
//=====================================================================================================================================

//==================================================================
//行動３（トライアタック）
//==================================================================
void CRiley::Action3()
{
	int& nPatternTime = GetPatternTime();         //パターン時間を取得する
	bool& bAction = GetAction();                  //行動しているかどうかを取得
	int& nPattern = GetPattern();                 //現在のパターンを取得
	int& nActionNum = GetActionNum();             //アクション回数をカウント
	int& nSubPattern = GetSubPattern();           //サブパターンを取得
	D3DXVECTOR3& Pos = GetPos();                  //位置を取得
	D3DXVECTOR3& Rot = GetRot();                  //向きを取得
	D3DXVECTOR3& Move = GetMove();                //移動量を取得
	D3DXVECTOR3 PlayerPos = CGame::GetPlayer()->GetPos();//プレイヤーの位置
	float fRandRot = 0.0f;
	CRileyAttack* pRileyAttack = nullptr;
	switch (nPattern)
	{
	case 0://溜める
		if (nPatternTime == 30)
		{//緑チャージ
			CParticle::SummonChargeParticle(CParticle::TYPE00_NORMAL, 100, 120, 5.0f, 30.0f, 30.0f, 300, 100, 10, false,
				Pos, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), true);
		}

		if (nPatternTime == 80)
		{
			nPattern++;
			nPatternTime = 0;
		}
		break;
	case 1:
		if (nPatternTime == 1)
		{
			for (int nCnt = 0; nCnt < m_nMAX_ACTION03_ATTACKNUM; nCnt++)
			{
				pRileyAttack = CRileyAttack::Create(CAttack::ATTACKTYPE11_EXPLOSION, 10, 240,
					D3DXVECTOR3(-BOSSSTAGE_HALFWIDTH, (BOSSSTAGE_HEIGHT / m_nMAX_ACTION03_ATTACKNUM) * nCnt + 30.0f, 0.0f)
					, NULL_VECTOR3, ONE_VECTOR3 * 2, NULL_VECTOR3, CAttack::COLLISIONTYPE03_BALL, NULL_VECTOR3, false, 30);
				pRileyAttack->SetSubType(rand() % CRileyAttack::m_nMAX_ACTION03_SUBTYPE);
				pRileyAttack->SetAttackMove(CRileyAttack::ATTACKMOVE_RILEY_01_ACTION03);
			}

			for (int nCnt = 0; nCnt < m_nMAX_ACTION03_ATTACKNUM; nCnt++)
			{
				pRileyAttack = CRileyAttack::Create(CAttack::ATTACKTYPE11_EXPLOSION, 10, 240,
					D3DXVECTOR3(BOSSSTAGE_HALFWIDTH, (BOSSSTAGE_HEIGHT / m_nMAX_ACTION03_ATTACKNUM) * nCnt + 30.0f, 0.0f)
					, NULL_VECTOR3, ONE_VECTOR3 * 2, NULL_VECTOR3, CAttack::COLLISIONTYPE03_BALL, NULL_VECTOR3, false, 30);
				pRileyAttack->SetSubType(rand() % CRileyAttack::m_nMAX_ACTION03_SUBTYPE);
				pRileyAttack->SetAttackMove(CRileyAttack::ATTACKMOVE_RILEY_01_ACTION03);
			}
		}

		if (nPatternTime == 100)
		{
			nPatternTime = 0;
			nPattern++;
		}
		break;
	case 2:
		if (nPatternTime == 1)
		{
			for (int nCnt = 0; nCnt < m_nMAX_ACTION03_ATTACKNUM * 2; nCnt++)
			{
				pRileyAttack = CRileyAttack::Create(CAttack::ATTACKTYPE11_EXPLOSION, 10, 240,
					D3DXVECTOR3(-BOSSSTAGE_HALFWIDTH + (BOSSSTAGE_WIDTH / (m_nMAX_ACTION03_ATTACKNUM * 2)) * nCnt,BOSSSTAGE_HEIGHT - 20.0f, 0.0f)
					, NULL_VECTOR3, ONE_VECTOR3 * 2, NULL_VECTOR3, CAttack::COLLISIONTYPE03_BALL, NULL_VECTOR3, false, 30);
				pRileyAttack->SetSubType(rand() % CRileyAttack::m_nMAX_ACTION03_SUBTYPE);
				pRileyAttack->SetAttackMove(CRileyAttack::ATTACKMOVE_RILEY_01_ACTION03);
			}

		}

		if (nPatternTime == 140)
		{
			ResetPattern();
		}
		break;
	default:
		break;
	}
}
//=====================================================================================================================================

//==================================================================
//行動４（アイスエイジ）
//==================================================================
void CRiley::Action4()
{
	int& nPatternTime = GetPatternTime();         //パターン時間を取得する
	bool& bAction = GetAction();                  //行動しているかどうかを取得
	int& nPattern = GetPattern();                 //現在のパターンを取得
	int& nActionNum = GetActionNum();             //アクション回数をカウント
	int& nSubPattern = GetSubPattern();           //サブパターンを取得
	D3DXVECTOR3& Pos = GetPos();                  //位置を取得
	D3DXVECTOR3& Rot = GetRot();                  //向きを取得
	D3DXVECTOR3& Move = GetMove();                //移動量を取得
	D3DXVECTOR3 PlayerPos = CGame::GetPlayer()->GetPos();//プレイヤーの位置
	bool& bIsLanding = GetLanding();
	float fRandRot = 0.0f;
	CRileyAttack* pRileyAttack = nullptr;
	switch (nPattern)
	{
	case 0://溜める
		if (nPatternTime == 30)
		{//黄色チャージ
			CParticle::SummonChargeParticle(CParticle::TYPE00_NORMAL, 100, 120, 5.0f, 30.0f, 30.0f, 300, 100, 10, false,
				Pos, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), true);
		}

		if (nPatternTime == 80)
		{
			nPattern++;
			nPatternTime = 0;
		}
		break;
	case 1:
		SetUseInteria(false);
		if (GetSenterPos().x < CGame::GetPlayer()->GetPos().x)
		{
			Move.x = 3.0f;
		}
		else
		{
			Move.x = -3.0f;
		}

		if (bIsLanding == true)
		{
			nActionNum++;
			Move.y = 10.0f * nActionNum;
		}

		if (nActionNum == 3)
		{
			nPattern++;
			nPatternTime = 0;
			nActionNum = 0;
		}
		break;
	case 2:
		if (bIsLanding == true)
		{
			if (nPatternTime % 5 == 0)
			{
				CManager::GetCamera()->SetShake(3, 0.4f);
				nActionNum++;
				if (nActionNum < m_nMAX_ICE_EIGE)
				{
					pRileyAttack = CRileyAttack::Create(CAttack::ATTACKTYPE05_ICE, 30, 15, Pos + D3DXVECTOR3(50.0f * nActionNum, -40.0f, 0.0f), NULL_VECTOR3,
						ONE_VECTOR3, NULL_VECTOR3, CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, NULL_VECTOR3, false, 100);
					pRileyAttack->SetUseAddScale(D3DXVECTOR3(0.0f, 0.2f * nActionNum, 0.0f), true);

					pRileyAttack = CRileyAttack::Create(CAttack::ATTACKTYPE05_ICE, 30, 15, Pos - D3DXVECTOR3(50.0f * nActionNum,40.0f, 0.0f), NULL_VECTOR3,
						ONE_VECTOR3, NULL_VECTOR3, CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, NULL_VECTOR3, false, 100);
					pRileyAttack->SetUseAddScale(D3DXVECTOR3(0.0f, 0.2f * nActionNum, 0.0f), true);
				}
				else
				{
					SetUseInteria(true);
					ResetPattern();
				}
			}
		}
		else
		{//地面に着くまでプレイヤーを追いかける
			if (nPatternTime < 30)
			{
				if (GetSenterPos().x < CGame::GetPlayer()->GetPos().x)
				{
					Move.x = 10.0f;
				}
				else
				{
					Move.x = -10.0f;
				}
			}
			else
			{
				Move.x = 0.0f;
			}
		}
		break;
	default:
		break;
	}
}
//=====================================================================================================================================

//==================================================================
//行動５
//==================================================================
void CRiley::Action5()
{
	int& nPatternTime = GetPatternTime();         //パターン時間を取得する
	bool& bAction = GetAction();                  //行動しているかどうかを取得
	int& nPattern = GetPattern();                 //現在のパターンを取得
	int& nActionNum = GetActionNum();             //アクション回数をカウント
	int& nSubPattern = GetSubPattern();           //サブパターンを取得
	D3DXVECTOR3& Pos = GetPos();                  //位置を取得
	D3DXVECTOR3& Rot = GetRot();                  //向きを取得
	D3DXVECTOR3& Move = GetMove();                //移動量を取得
	D3DXVECTOR3 PlayerPos = CGame::GetPlayer()->GetPos();//プレイヤーの位置

	D3DXVECTOR3& PurposePos = GetPurposePos();//目的地を使用
	bool& bIsLanding = GetLanding();
	bool& bIsWalling = GetWalling();
	float fRandRot = 0.0f;
	CRileyAttack* pRileyAttack = nullptr;
	if (m_pSnowRamming != nullptr)
	{
		m_pSnowRamming->SetPos(Pos);
	}


	SetUseInteria(false);
	SetUseGravity(false);
	switch (nPattern)
	{
	case 0://溜める
		if (nPatternTime == 30)
		{//虹色チャージ
			CParticle::SummonChargeParticle(CParticle::TYPE00_NORMAL, 100, 120, 5.0f, 30.0f, 30.0f, 300, 100, 10,true,
				Pos, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), true);

			m_pSnowRamming = CRileyAttack::Create(CAttack::ATTACKTYPE11_EXPLOSION, 20, 360, Pos, NULL_VECTOR3, ONE_VECTOR3, NULL_VECTOR3,
				CAttack::COLLISIONTYPE03_BALL, ONE_VECTOR3 * 0.1f, false, 10);
			m_pSnowRamming->SetUseDeath(false);
			m_pSnowRamming->SetUseAddScale(ONE_VECTOR3 * 0.2f, true);

		}

		if (nPatternTime == 80)
		{
			m_pSnowRamming->SetUseAddScale(NULL_VECTOR3, false);
			nPattern++;
			nPatternTime = 0;
		}
		break;
	case 1:
		PurposePos = D3DXVECTOR3(0.0f, 300.0f, 0.0f);
		fRandRot = CCalculation::CalculationXYaim(GetSenterPos(),PurposePos);
		Move.x = sinf(fRandRot) * 10.0f;
		Move.y = cosf(fRandRot) * 10.0f;

		if (GetSenterPos().x > PurposePos.x - 20.0f && GetSenterPos().x < PurposePos.x + 20.0f &&
			GetSenterPos().y > PurposePos.y - 20.0f && GetSenterPos().y < PurposePos.y + 20.0f)
		{
			Pos = PurposePos;
			Move = NULL_VECTOR3;
			nPattern++;
			nPatternTime = 0;
		}
		break;
	case 2:
		if (nPatternTime < 60)
		{
			CParticle::SummonChargeParticle(CParticle::TYPE00_NORMAL, 5, 60, 10.0f, 30.0f, 30.0f, 500, 100, 10, false,
				Pos, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), true);
		}

		if (nPatternTime == 120)
		{
			m_pSnowRamming->SetUseAttackEffect(true, 10, D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f));
			nPattern++;
			nPatternTime = 0;
		}
		break;
	case 3://攻撃開始
		Move.x = -20.0f;
		if (bIsWalling == true)
		{
			Pos.x = BOSSSTAGE_HALFWIDTH;
			nActionNum++;
		}
		fRandRot = CCalculation::CalculationRandVecXY();

		if (nPatternTime % 5 == 0)
		{
			CBullet::Create(CBullet::BULLETTYPE_ENEMY, CBullet::BULLETTEX_ENEMY, 120, 10, 20.0f, 20.0f, Pos, D3DXVECTOR3(sinf(fRandRot) * 5.0f, cosf(fRandRot) * 5.0f, 0.0f));
		}

		if (nActionNum == 10)
		{//10週したら次のパターンへ
			nPattern++;
			nPatternTime = 0;
			nActionNum = 0;
		}
		break;
	case 4:
		if (nPatternTime <= 1)
		{
			PurposePos.x = float(rand() % 720 - 360);
		}

		if (GetSenterPos().x > PurposePos.x - 20.0f && GetSenterPos().x < PurposePos.x + 20.0f &&
			GetSenterPos().y > PurposePos.y - 20.0f && GetSenterPos().y < PurposePos.y + 20.0f)
		{
			Move = NULL_VECTOR3;
			nPattern++;
			nPatternTime = 0;
		}
		break;
	case 5:
		if (nPatternTime < 30)
		{
			CParticle::SummonChargeParticle(CParticle::TYPE00_NORMAL, 5, 60, 10.0f, 30.0f, 30.0f, 500, 100, 10, false,
				Pos, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), true);
			m_pSnowRamming->SetUseAddScale(ONE_VECTOR3 * 0.2f, true);
		}

		if (nPatternTime == 70)
		{
			SetUseAdjustOriginPos(false);
			m_pSnowRamming->SetUseAddScale(NULL_VECTOR3,false);
			fRandRot = CCalculation::CalculationXYaim(Pos, PlayerPos);
			PurposePos = CGame::GetPlayer()->GetPos();
			Move.x = sinf(fRandRot) * 3.0f;
			Move.y = cosf(fRandRot) * 3.0f;
		}

		if (nPatternTime > 70)
		{
			if (bIsLanding == true)
			{
				SetUseAdjustOriginPos(true);
				Move = NULL_VECTOR3;
				SetUseGravity(true);
				SetUseInteria(true);
				m_pSnowRamming->SetUseDeath(true);
				m_pSnowRamming->SetDeath();
				m_pSnowRamming = nullptr;
				CParticle::SummonParticle(CParticle::TYPE02_ICE, 500, 60, 120.0f, 120.0f, 200, 10, true, Pos, NORMAL_COL, true);
				ResetPattern();
			}
		}
		break;
	default:
		break;
	}
}
//=====================================================================================================================================

//============================================<<<ドライヤクラス>>>=============================================================================	

//==================================================================
//コンストラクタ
//==================================================================
CDraiya::CDraiya() : m_nNumThunder(0),m_pSpecialThunder()
{

}
//=====================================================================================================================================

//==================================================================
//デストラクタ
//==================================================================
CDraiya::~CDraiya()
{

}
//=====================================================================================================================================

//==================================================================
//初期化処理
//==================================================================
HRESULT CDraiya::Init()
{
	CBoss::Init();//ボス初期化
	m_pAnnihilationThunder = nullptr;

	for (int nCnt = 0; nCnt < m_nMAX_ACTION2THUNDERNUM; nCnt++)
	{
		m_bAnnihilationThunderShot[nCnt] = false;
	}

	for (int nCnt = 0; nCnt < m_nMAX_ACTION03_RAMMING_NUM; nCnt++)
	{
		m_RammingSavePos[nCnt] = NULL_VECTOR3;
		m_fSaveRammingRot[nCnt] = 0.0f;
	}

	return S_OK;
}
//=====================================================================================================================================

//==================================================================
//終了処理
//==================================================================
void CDraiya::Uninit()
{
	CBoss::Uninit();//ボス終了処理
}
//=====================================================================================================================================

//==================================================================
//更新処理
//==================================================================
void CDraiya::Update()
{
	CBoss::Update();//ボス更新処理
}
//=====================================================================================================================================

//==================================================================
//描画処理
//==================================================================
void CDraiya::Draw()
{
	int nCntTime = GetCntTime();
	CBoss::Draw();//ボス描画処理
}
//=====================================================================================================================================

//==================================================================
//死亡フラグ設定処理
//==================================================================
void CDraiya::SetDeath()
{
	if (m_pAnnihilationThunder != nullptr)
	{
		m_pAnnihilationThunder->SetUseDeath(true);
		m_pAnnihilationThunder->SetDeath();
		m_pAnnihilationThunder = nullptr;
	}

	for (int nCnt = 0; nCnt < m_nNUMSPECIALATTACK; nCnt++)
	{
		if (m_pSpecialThunder[nCnt] != nullptr)
		{
			m_pSpecialThunder[nCnt]->SetUseDeath(true);
			m_pSpecialThunder[nCnt]->SetDeath();
			m_pSpecialThunder[nCnt] = nullptr;
		}
	}
	CBoss::SetDeath();//ボス死亡フラグ設定処理
}
//=====================================================================================================================================

//==================================================================
//生成処理
//==================================================================
CDraiya* CDraiya::Create(BOSSTYPE type, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 Scale)
{
	CDraiya* pDraiya = DBG_NEW CDraiya;   //ボスを生成
	bool bSuccess = pDraiya->GetCreateSuccess();
	int nIdx = 0;//モデルのインデックス
	if (bSuccess == true)
	{
		if (pDraiya != nullptr)
		{
			pDraiya->Init();                             //初期化処理
			pDraiya->SetUseDeath(true);                  //死亡フラグを発動するかどうかを設定する
			pDraiya->SetType(type);                      //ボスの種類
			pDraiya->SetLife(nLife);                     //体力
			pDraiya->SetMaxLife(nLife);//最大体力
			pDraiya->SetAutoSubLife(false);              //自動的に体力を減らすかどうか
			pDraiya->CObjectX::SetPos(pos);                                                                                  //オブジェクト２Ｄの位置を設定
			pDraiya->SetPosOld(pos);                                                                                         //1f前の位置を設定
			pDraiya->SetSupportPos(pos);                                                                                     //設置位置
			pDraiya->CObject::SetType(CObject::TYPE_BOSS);                                                                   //オブジェクトの種類を決める
			pDraiya->CObjectX::SetScale(Scale);                                                                              //オブジェクトの拡大率
			pDraiya->SetFormarScale(Scale);                                                                                  //元の拡大率を設定する
			nIdx = CManager::GetObjectXInfo()->Regist(CObjectX::OBJECTXTYPE_BOSS, m_BOSS_FILENAME[(int)(type)]);
			pDraiya->CObjectX::BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(CObjectX::OBJECTXTYPE_BOSS, nIdx),
				CManager::GetObjectXInfo()->GetBuffMat(CObjectX::OBJECTXTYPE_BOSS, nIdx),
				CManager::GetObjectXInfo()->GetdwNumMat(CObjectX::OBJECTXTYPE_BOSS, nIdx),
				CManager::GetObjectXInfo()->GetTexture(CObjectX::OBJECTXTYPE_BOSS, nIdx),
				CManager::GetObjectXInfo()->GetColorValue(CObjectX::OBJECTXTYPE_BOSS, nIdx));                                   //モデル情報を割り当てる
			pDraiya->SetBossHpGauge(nLife);                                                                                  //体力ゲージを設定する
			pDraiya->CObjectX::SetObjXType(CObjectX::OBJECTXTYPE_BOSS);                                                      //オブジェクトXのタイプを設定
			pDraiya->CObjectX::SetTypeNum((int)(type));                                                                      //オブジェクトXごとのタイプ番号を設定
			pDraiya->SetSize();                                                                                              //Xオブジェクトのサイズを設定する
			pDraiya->CObjectX::SetRotType(CObjectX::ROTTYPE::ROTTYPE_NORMAL);                                                //向きの種類を設定
			pDraiya->SetManagerType((int)(CStageManager::MANAGEROBJECT_BOSS));                                               //マネージャーで呼び出す時の種類を設定
			pDraiya->AdjustSize();      //判定サイズ調整を行う
		}
		else
		{//オブジェクトに空きがなかったので破棄する
			delete pDraiya;
			pDraiya = nullptr;
			return nullptr;
		}
	}
	return pDraiya;
}
//=====================================================================================================================================

//==================================================================
//移動パターン実行処理
//==================================================================
void CDraiya::MovePattern()
{
	int& nMovePatternFrame = GetMovePatternFrame();
	int& nMaxMovePatternFrame = GetMaxMovePatternFrame();
	bool& bUseMovePattern = GetUseMovePattern();//移動パターンを使用しているかどうか
	int& nMovePatternNum = GetMovePatternNum();//移動パターン番号を取得する
	D3DXVECTOR3& Move = GetMove();
	D3DXVECTOR3 Pos = GetPos();

	CBoss::MovePattern();//移動パターンの共通処理

	DecideMovePattern();//移動パターン決定処理

	//移動パターンを開始する
	switch (nMovePatternNum)
	{
	case 0:
		MovePattern0();//移動パターン０
		break;
	default:
		break;
	}

	//移動パターンを終わらせる
	if (nMovePatternFrame > nMaxMovePatternFrame)
	{
		bUseMovePattern = false;//アクションパターンへ移行
		nMovePatternFrame = 0;
		Move = NULL_VECTOR3;
	}

}
//=====================================================================================================================================

//==================================================================
//移動パターン０
//==================================================================
void CDraiya::MovePattern0()
{
	D3DXVECTOR3& Move = GetMove();
	D3DXVECTOR3& Pos = GetPos();
	D3DXVECTOR3 PlayerPos = CGame::GetPlayer()->GetPos();
	int nMovePatternFrame = GetMovePatternFrame();
	float fLength = CCalculation::CalculationLength(Pos, PlayerPos);
	if (nMovePatternFrame > 15)
	{
		if (fLength < 200.0f)
		{//距離が１２０より近ければ（近いので遠ざかる）
			if (Pos.x < PlayerPos.x)
			{//プレイヤーが右側にいたら・・・
				Move.x = -2.0f;
			}
			else
			{//プレイヤーが左側にいたら・・・
				Move.x = 2.0f;
			}
		}
		else
		{//距離が１２０より遠ければ（遠いので近づく）
			if (Pos.x < PlayerPos.x)
			{//プレイヤーが右側にいたら・・・
				Move.x = 2.0f;
			}
			else
			{//プレイヤーが左側にいたら・・・
				Move.x = -2.0f;
			}
		}

		if (fLength < 210.0f && fLength > 190.0f)
		{
			Move.x = 0.0f;
		}

		if (CGame::GetPlayer()->GetPos().y > 5.0f && GetLanding() == true)
		{
			Move.y = 20.0f;
		}
	}

	if (nMovePatternFrame == m_nMAX_MOVEPATTERN_0_FRAME)
	{
		SetUndoInertia();//慣性を元に戻す
		nMovePatternFrame = 0;
	}

}
//=====================================================================================================================================

//==================================================================
//移動パターン決定処理
//==================================================================
void CDraiya::DecideMovePattern()
{
	int& nMovePatternFrame = GetMovePatternFrame();
	int& nMaxMovePatternFrame = GetMaxMovePatternFrame();
	bool& bUseMovePattern = GetUseMovePattern();//移動パターンを使用しているかどうか
	int& nMovePatternNum = GetMovePatternNum();//移動パターン番号を取得する
	D3DXVECTOR3 Pos = GetPos();
	D3DXVECTOR3 PlayerPos = CGame::GetPlayer()->GetPos();

	if (nMovePatternFrame == 1)
	{//最初に移動方法を決める

		nMovePatternNum = 0;

		//それぞれの移動パターンの最大フレーム数を設定する
		switch (nMovePatternNum)
		{
		case 0:
			nMaxMovePatternFrame = m_nMAX_MOVEPATTERN_0_FRAME;
			break;
		default:
			break;
		}
	}

}
//=====================================================================================================================================

//==================================================================
//サイズ調整処理
//==================================================================
void CDraiya::AdjustSize()
{

}
//=====================================================================================================================================

//==================================================================
//行動パターン実行処理
//==================================================================
void CDraiya::ActionPattern()
{
	bool& bAction = GetAction();    //行動しているかどうか
	int& nSkill = GetSkill();       //スキル
	bool& bInsanity = GetInsanity();//発狂しているかどうか
	int nLife = GetLife();          //体力
	int& nCntTurn = GetTurnCnt();   //ターン数をカウント
	int nMaxLife = GetMaxLife();    //最大体力
	float fRatioLife = 0.0f;        //体力の割合

	fRatioLife = float(nLife) / float(nMaxLife);

	if (fRatioLife < 0.5f)
	{//体力が５０％を下回ったら発狂状態にする
		bInsanity = true;
	}

	if (bAction == false)
	{
		if (bInsanity == false)
		{//発狂状態じゃなければ
			nSkill = rand() % 4;
			//nSkill = 4;
		}
		else
		{//発狂状態なら
			nCntTurn++;//ここからターン数をカウント
			nSkill = rand() % 5;
			if (nCntTurn == 1)
			{
				nSkill = 4;
			}
		}
		bAction = true;
	}

	if (bAction == true)
	{
		CBoss::ActionPattern();
		switch (nSkill)
		{
		case 0:
			Action0();
			break;
		case 1:
			Action1();
			break;
		case 2:
			Action2();
			break;
		case 3:
			Action3();
			break;
		case 4:
			Action4();
			break;
		default:
			break;
		}
	}

}
//=====================================================================================================================================

//==================================================================
//行動パターン０
//==================================================================
void CDraiya::Action0()
{
	int& nPatternTime = GetPatternTime();         //パターン時間を取得する
	bool& bAction = GetAction();                  //行動しているかどうかを取得
	int& nPattern = GetPattern();                 //現在のパターンを取得
	int& nActionNum = GetActionNum();             //アクション回数をカウント
	int& nSubPattern = GetSubPattern();           //サブパターンを取得
	D3DXVECTOR3& Pos = GetPos();                  //位置を取得
	D3DXVECTOR3& Rot = GetRot();                  //向きを取得
	D3DXVECTOR3& Move = GetMove();                //移動量を取得
	D3DXVECTOR3 PlayerPos = CGame::GetPlayer()->GetPos();//プレイヤーの位置

	D3DXVECTOR3& PurposePos = GetPurposePos();//目的地を使用
	bool& bIsLanding = GetLanding();
	bool& bIsWalling = GetWalling();
	float fRandRot = 0.0f;
	CDraiyaAttack* pRileyAttack = nullptr;

	switch (nPattern)
	{
	case 0://溜める
		if (nPatternTime == 30)
		{//赤色チャージ
			CParticle::SummonChargeParticle(CParticle::TYPE00_NORMAL, 100, 120, 5.0f, 30.0f, 30.0f, 300, 100, 10, true,
				Pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), true);
		}

		if (nPatternTime == 90)
		{
			SetUseGravity(false);//重力を使用しない
			SetUseInteria(false);//慣性を使用しない
			nPattern++;
			nPatternTime = 0;
		}
		break;
	case 1://目的地まで移動
		if (nPatternTime == 20)
		{
			PurposePos = PlayerPos + D3DXVECTOR3(0.0f, 200.0f, 0.0f);

			if (PurposePos.x + GetVtxMin().x < -BOSSSTAGE_HALFWIDTH)
			{
				PurposePos.x = -BOSSSTAGE_HALFWIDTH - GetVtxMin().x;
			}

			if (PurposePos.x + GetVtxMax().x > BOSSSTAGE_HALFWIDTH)
			{
				PurposePos.x = BOSSSTAGE_HALFWIDTH - GetVtxMax().x;
			}

			fRandRot = CCalculation::CalculationXYaim(Pos,PurposePos);
			Move.x = sinf(fRandRot) * 20.0f;
			Move.y = cosf(fRandRot) * 20.0f;
			nPattern++;
			nPatternTime = 0;
		}
		break;
	case 2://目的地にたどり着く
		if (Pos.x > PurposePos.x - 20.0f && Pos.x < PurposePos.x + 20.0f &&
			Pos.y > PurposePos.y - 20.0f && Pos.y < PurposePos.y + 20.0f)
		{
			Move = NULL_VECTOR3;
			SetUseGravity(true);
			nPattern++;
			nPatternTime = 0;
		}
		break;
	case 3://地面に着いたら攻撃発動
		if (bIsLanding == true)
		{
			if (nPatternTime % 10 == 0)
			{
				nActionNum++;
				if (nActionNum == 1)
				{
					CUi3D::Create(CUi3D::UI3DTYPE01_WARNING, 60, 30.0f, 30.0f, Pos, NULL_VECTOR3);
					CDraiyaAttack::Create(CAttack::ATTACKTYPE06_THUNDER, 10, 250, Pos + D3DXVECTOR3(0.0f, 200.0f, 0.0f), D3DXVECTOR3(0.0f, -m_fACTION0_ATTACKSPEED, 0.0f),
						D3DXVECTOR3(1.0f, 5.0f, 1.0f), NULL_VECTOR3, CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, D3DXVECTOR3(0.0f, 0.6f, 0.0f), false, 20);
				}
				if (nActionNum <= 5 && nActionNum > 1)
				{
					CUi3D::Create(CUi3D::UI3DTYPE01_WARNING, 60, 30.0f, 30.0f, Pos + D3DXVECTOR3(40.0f * nActionNum, 30.0f, 0.0f), NULL_VECTOR3);
					CDraiyaAttack::Create(CAttack::ATTACKTYPE06_THUNDER, 10, 250, Pos + D3DXVECTOR3(40.0f * nActionNum, 200.0f, 0.0f), D3DXVECTOR3(0.0f, -m_fACTION0_ATTACKSPEED, 0.0f),
						D3DXVECTOR3(1.0f, 5.0f, 1.0f), NULL_VECTOR3, CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, D3DXVECTOR3(0.0f, 0.6f, 0.0f), false, 20);

					CUi3D::Create(CUi3D::UI3DTYPE01_WARNING, 60, 30.0f, 30.0f, Pos + D3DXVECTOR3(-40.0f * nActionNum, 30.0f, 0.0f), NULL_VECTOR3);
					CDraiyaAttack::Create(CAttack::ATTACKTYPE06_THUNDER, 10, 250, Pos + D3DXVECTOR3(-40.0f * nActionNum, 200.0f, 0.0f), D3DXVECTOR3(0.0f, -m_fACTION0_ATTACKSPEED, 0.0f),
						D3DXVECTOR3(1.0f, 5.0f, 1.0f), NULL_VECTOR3, CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, D3DXVECTOR3(0.0f, 0.6f, 0.0f), false, 20);

				}

				if (nActionNum == 5)
				{
					nActionNum = 0;
					nPattern++;
					nPatternTime = 0;
				}
			}
		}
		break;
	case 4:
		if (nPatternTime % 7 == 0)
		{
			nActionNum++;

			CUi3D::Create(CUi3D::UI3DTYPE01_WARNING, 60, 30.0f, 30.0f, Pos + D3DXVECTOR3(-700.0f + 40.0f * nActionNum, 30.0f, 0.0f), NULL_VECTOR3);
			CDraiyaAttack::Create(CAttack::ATTACKTYPE06_THUNDER, 10, 250, Pos + D3DXVECTOR3(-700.0f + 40.0f * nActionNum, 200.0f, 0.0f), D3DXVECTOR3(0.0f, -10.0f, 0.0f),
				D3DXVECTOR3(1.0f, 5.0f, 1.0f), NULL_VECTOR3, CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, D3DXVECTOR3(0.0f, 0.6f, 0.0f), false, 20);

			CUi3D::Create(CUi3D::UI3DTYPE01_WARNING, 60, 30.0f, 30.0f, Pos + D3DXVECTOR3(700.0f - 40.0f * nActionNum, 30.0f, 0.0f), NULL_VECTOR3);
			CDraiyaAttack::Create(CAttack::ATTACKTYPE06_THUNDER, 10, 250, Pos + D3DXVECTOR3(700.0f - 40.0f * nActionNum, 200.0f, 0.0f), D3DXVECTOR3(0.0f, -10.0f, 0.0f),
				D3DXVECTOR3(1.0f, 5.0f, 1.0f), NULL_VECTOR3, CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, D3DXVECTOR3(0.0f, 0.6f, 0.0f), false, 20);

			if (nActionNum == 12)
			{
				ResetPattern();
				SetUseInteria(true);//慣性を使用する
			}
		}
		break;
	default:
		break;
	}
}
//=====================================================================================================================================

//==================================================================
//行動パターン１
//==================================================================
void CDraiya::Action1()
{
	int& nPatternTime = GetPatternTime();         //パターン時間を取得する
	bool& bAction = GetAction();                  //行動しているかどうかを取得
	int& nPattern = GetPattern();                 //現在のパターンを取得
	int& nActionNum = GetActionNum();             //アクション回数をカウント
	int& nSubPattern = GetSubPattern();           //サブパターンを取得
	D3DXVECTOR3& Pos = GetPos();                  //位置を取得
	D3DXVECTOR3& Rot = GetRot();                  //向きを取得
	D3DXVECTOR3& Move = GetMove();                //移動量を取得
	D3DXVECTOR3 PlayerPos = CGame::GetPlayer()->GetPos();//プレイヤーの位置

	CBullet* pBullet = nullptr;//弾へのポインタ
	D3DXVECTOR3& PurposePos = GetPurposePos();//目的地を使用
	bool& bIsLanding = GetLanding();
	bool& bIsWalling = GetWalling();
	float fRandRot = 0.0f;
	float& fSaveRot = GetSaveRot();//保存する向きを取得する
	CDraiyaAttack* pRileyAttack = nullptr;

	switch (nPattern)
	{
	case 0://溜める
		if (nPatternTime == 30)
		{//青色チャージ
			CParticle::SummonChargeParticle(CParticle::TYPE00_NORMAL, 100, 120, 5.0f, 30.0f, 30.0f, 300, 100, 10,false,
				Pos, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), true);
		}

		if (nPatternTime == 90)
		{
			SetUseInteria(false);//慣性を使用しない
			nPattern++;
			nPatternTime = 0;
		}
		break;
	case 1:
		if (Pos.x < PlayerPos.x)
		{
			SetUseAddRot(true, D3DXVECTOR3(0.0f, 0.0f, 0.2f));
		}
		else
		{
			SetUseAddRot(true, D3DXVECTOR3(0.0f, 0.0f, -0.2f));
		}
		fRandRot = CCalculation::CalculationParabola(CCalculation::CalculationLength(Pos, PlayerPos), GetNormalGravity(), m_fACTION1_PARABORAJUMPSPEED, Pos, PlayerPos);

		Move.x = sinf(fRandRot) * m_fACTION1_PARABORAJUMPSPEED;
		Move.y = cosf(fRandRot) * m_fACTION1_PARABORAJUMPSPEED;

		nPattern++;
		nPatternTime = 0;
		break;
	case 2:
		CParticle::Create(CParticle::TYPE03_THUNDER, 30, 100.0f, 100.0f, GetSenterPos(), NULL_VECTOR3, NORMAL_COL, true);
		if (nPatternTime % 4 == 0)
		{
			pBullet = CBullet::Create(CBullet::BULLETTYPE_ENEMY, CBullet::BULLETTEX_ENEMY, 120, 10, 40.0f, 40.0f, Pos, D3DXVECTOR3(sinf(GetRot().z) * 1.0f, cosf(GetRot().z) * 1.0f, 0.0f));
			pBullet->SetUseAddSpeed(true, true, 1.05f);
			pBullet->SetUseEffect(10, D3DXVECTOR2(60.0f, 60.0f), D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
		}
		if (bIsLanding == true)
		{
			nPattern++;
			nPatternTime = 0;
			Move = NULL_VECTOR3;
			Rot = NULL_VECTOR3;
			SetUseAddRot(false, NULL_VECTOR3);
		}
		break;
	case 3:
		CParticle::Create(CParticle::TYPE03_THUNDER, 30, 100.0f, 100.0f, GetSenterPos(), NULL_VECTOR3, NORMAL_COL, true);

		if (nPatternTime == 1)
		{
			Move.y = 30.0f;
		}

		if (nPatternTime >= 10)
		{
			if (Move.y < 0.0f)
			{
				nPatternTime = 0;
				nPattern++;
			}
		}
		break;
	case 4:
		if (nPatternTime == 1)
		{
			SetUseGravity(false);
			fSaveRot = CCalculation::CalculationXYaim(Pos, PlayerPos);
			Move.x = sinf(fSaveRot) * 15.0f;
			Move.y = cosf(fSaveRot) * 15.0f;
		}

		if (nPatternTime % 12 == 0)
		{
			pBullet = CBullet::Create(CBullet::BULLETTYPE_ENEMY, CBullet::BULLETTEX_ENEMY, 120, 10, 40.0f, 40.0f, Pos, D3DXVECTOR3(sinf(fSaveRot + 1.0f) * 1.0f, cosf(fSaveRot + 1.0f) * 1.0f, 0.0f));
			pBullet->SetUseAddSpeed(true, true, 1.1f);
			pBullet->SetUseEffect(10, D3DXVECTOR2(60.0f, 60.0f), D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));

			pBullet = CBullet::Create(CBullet::BULLETTYPE_ENEMY, CBullet::BULLETTEX_ENEMY, 120, 10, 40.0f, 40.0f, Pos, D3DXVECTOR3(sinf(fSaveRot - 1.0f) * 1.0f, cosf(fSaveRot - 1.0f) * 1.0f, 0.0f));
			pBullet->SetUseAddSpeed(true, true, 1.1f);
			pBullet->SetUseEffect(10, D3DXVECTOR2(60.0f, 60.0f), D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
		}

		if (Pos.y + GetVtxMin().y <= 0.0f)
		{
			Move = NULL_VECTOR3;
			SetUseGravity(true);
			SetUseInteria(true);
			ResetPattern();
		}
		break;
	default:
		break;
	}

}
//=====================================================================================================================================

//==================================================================
//行動パターン２（殲滅レーザー）
//==================================================================
void CDraiya::Action2()
{
	int& nPatternTime = GetPatternTime();         //パターン時間を取得する
	bool& bAction = GetAction();                  //行動しているかどうかを取得
	int& nPattern = GetPattern();                 //現在のパターンを取得
	int& nActionNum = GetActionNum();             //アクション回数をカウント
	int& nSubPattern = GetSubPattern();           //サブパターンを取得
	D3DXVECTOR3& Pos = GetPos();                  //位置を取得
	D3DXVECTOR3& Rot = GetRot();                  //向きを取得
	D3DXVECTOR3& Move = GetMove();                //移動量を取得
	D3DXVECTOR3 PlayerPos = CGame::GetPlayer()->GetPos();//プレイヤーの位置

	int nRand = 0;//乱数用
	CBullet* pBullet = nullptr;//弾へのポインタ
	D3DXVECTOR3& PurposePos = GetPurposePos();//目的地を使用
	bool& bIsLanding = GetLanding();
	bool& bIsWalling = GetWalling();
	float fRandRot = 0.0f;
	float& fSaveRot = GetSaveRot();//保存する向きを取得する
	CDraiyaAttack* pRileyAttack = nullptr;

	switch (nPattern)
	{
	case 0://溜める
		if (nPatternTime == 30)
		{//緑色チャージ
			CParticle::SummonChargeParticle(CParticle::TYPE00_NORMAL, 100, 120, 5.0f, 30.0f, 30.0f, 300, 100, 10, false,
				Pos, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), true);
		}

		if (nPatternTime == 90)
		{
			SetUseInteria(false);//慣性を使用しない
			SetUseGravity(false);//重力を使用しない
			nPattern++;
			nPatternTime = 0;
		}
		break;
	case 1:
		fRandRot = CCalculation::CalculationXYaim(Pos, NULL_VECTOR3);
		Move.x = sinf(fRandRot) * 10.0f;
		Move.y = cosf(fRandRot) * 10.0f;

		if (Pos.x > -20.0f && Pos.x < 20.0f)
		{
			Move = NULL_VECTOR3;
			nPattern++;
			nPatternTime = 0;
		}
		break;
	case 2:
		if (nPatternTime % 30 == 0 && nActionNum < m_nMAX_ACTION2THUNDERNUM)
		{
			nRand = rand() % 2;

			if (nActionNum < 0 || nActionNum >= m_nMAX_ACTION2THUNDERNUM)
			{
				assert("ドライヤの技２が配列外アクセス");
			}

			if (nRand == 0)
			{
				m_bAnnihilationThunderShot[nActionNum] = false;
				CParticle::SummonParticle(CParticle::TYPE00_NORMAL, 120, 50, 50.0f, 50.0f, 200, 10, false, Pos, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), true);
				CUi3D::Create(CUi3D::UI3DTYPE01_WARNING, 15, 100.0f, 100.0f, D3DXVECTOR3(-BOSSSTAGE_HALFWIDTH * 0.5f, 200.0f, 0.0f), NULL_VECTOR3);
			}
			else
			{
				m_bAnnihilationThunderShot[nActionNum] = true;
				CUi3D::Create(CUi3D::UI3DTYPE01_WARNING, 15, 100.0f, 100.0f, D3DXVECTOR3(BOSSSTAGE_HALFWIDTH * 0.5f, 200.0f, 0.0f), NULL_VECTOR3);
				CParticle::SummonParticle(CParticle::TYPE00_NORMAL, 120, 50, 50.0f, 50.0f, 200, 10, false, Pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), true);
			}

			nActionNum++;
		}

		if (nActionNum == m_nMAX_ACTION2THUNDERNUM)
		{
			nPattern++;
			nActionNum = 0;
			nPatternTime = 0;
		}
		break;
	case 3:

		if (nPatternTime % m_nMAX_ACTION2THUNDER_SPAN == 0)
		{
			m_pAnnihilationThunder = CDraiyaAttack::Create(CAttack::ATTACKTYPE06_THUNDER, 10, 20,NULL_VECTOR3, NULL_VECTOR3, D3DXVECTOR3(2.0f, 10.0f, 2.0f), NULL_VECTOR3,
				CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, D3DXVECTOR3(0.0f, 0.5f, 0.0f), false, 60);
			m_pAnnihilationThunder->SetUseDeath(false);
			m_pAnnihilationThunder->SetUseAttackEffect(true, 20, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			m_pAnnihilationThunder->SetParentInfo(Pos, NULL_VECTOR3, ONE_VECTOR3, true);
		}

		if (m_pAnnihilationThunder != nullptr)
		{
			D3DXVECTOR3& ParentRot = m_pAnnihilationThunder->GetParentRot();

			if (nActionNum < 0 || nActionNum >= m_nMAX_ACTION2THUNDERNUM)
			{
				assert("ドライヤの技２が配列外アクセス");
			}
			if (m_bAnnihilationThunderShot[nActionNum] == false)
			{
				ParentRot.z += 0.1f;
			}
			else
			{
				ParentRot.z += -0.1f;
			}

			if (m_pAnnihilationThunder->GetLife() < 2)
			{
				m_pAnnihilationThunder->SetUseDeath(true);
				m_pAnnihilationThunder->SetDeath();
				m_pAnnihilationThunder = nullptr;
				nActionNum++;
				nPatternTime = 0;
			}
		}

		if (nActionNum == m_nMAX_ACTION2THUNDERNUM)
		{
			ResetPattern();
			SetUseInteria(true);//慣性を元に戻す
			SetUseGravity(true);//重力を元に戻す
		}
		break;
	default:
		break;
	}

}
//=====================================================================================================================================

//==================================================================
//行動パターン３（閃光の体当たり）
//==================================================================
void CDraiya::Action3()
{
	int& nPatternTime = GetPatternTime();         //パターン時間を取得する
	bool& bAction = GetAction();                  //行動しているかどうかを取得
	int& nPattern = GetPattern();                 //現在のパターンを取得
	int& nActionNum = GetActionNum();             //アクション回数をカウント
	int& nSubPattern = GetSubPattern();           //サブパターンを取得
	D3DXVECTOR3& Pos = GetPos();                  //位置を取得
	D3DXVECTOR3& Rot = GetRot();                  //向きを取得
	D3DXVECTOR3& Move = GetMove();                //移動量を取得
	D3DXVECTOR3 PlayerPos = CGame::GetPlayer()->GetPos();//プレイヤーの位置

	int nRand = 0;//乱数用
	CBullet* pBullet = nullptr;//弾へのポインタ
	D3DXVECTOR3& PurposePos = GetPurposePos();//目的地を使用
	bool& bIsLanding = GetLanding();
	bool& bIsWalling = GetWalling();
	float fRandRot = 0.0f;
	float& fSaveRot = GetSaveRot();//保存する向きを取得する
	CDraiyaAttack* pDraiyaAttack = nullptr;
	float fRandPosY = 0.0f;//乱数位置Y
	float fRandPosX = 0.0f;//乱数位置X

	switch (nPattern)
	{
	case 0://溜める
		if (nPatternTime == 30)
		{//水色チャージ
			CParticle::SummonChargeParticle(CParticle::TYPE00_NORMAL, 100, 120, 5.0f, 30.0f, 30.0f, 300, 100, 10, false,
				Pos, D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), true);
		}

		if (nPatternTime == 90)
		{
			SetUseInteria(false);//慣性を使用しない
			SetUseGravity(false);//重力を使用しない
			nPattern++;
			nPatternTime = 0;
		}
		break;
	case 1:
		if (nPatternTime % 30 == 0 && nActionNum < m_nMAX_ACTION03_RAMMING_NUM)
		{
			if (nActionNum < 4)
			{//横から

				if (nActionNum < 0 || nActionNum >= m_nMAX_ACTION03_RAMMING_NUM)
				{
					assert("ドライヤの技３が配列外アクセス");
				}
				m_fSaveRammingRot[nActionNum] = float(rand() % 40 - 20) / 100 + D3DX_PI * -0.5f;
				fRandPosY = float(rand() % int(BOSSSTAGE_HEIGHT));
				m_RammingSavePos[nActionNum] = D3DXVECTOR3(BOSSSTAGE_HALFWIDTH, fRandPosY, 0.0f);

				for (int nCnt = 0; nCnt < 20; nCnt++)
				{
					CUi3D::Create(CUi3D::UI3DTYPE01_WARNING, 40, 30.0f, 30.0f,
						m_RammingSavePos[nActionNum] + D3DXVECTOR3(sinf(m_fSaveRammingRot[nActionNum]) * (60.0f * nCnt), cosf(m_fSaveRammingRot[nActionNum]) * (60.0f * nCnt), 0.0f), NULL_VECTOR3);
				}
			}
			else
			{//上から
				if (nActionNum < 0 || nActionNum >= m_nMAX_ACTION03_RAMMING_NUM)
				{
					assert("ドライヤの技３が配列外アクセス");
				}
				m_fSaveRammingRot[nActionNum] = float(rand() % 40 - 20) / 100 + D3DX_PI * 1.0f;
				fRandPosX = float(rand() % int(BOSSSTAGE_WIDTH)) - BOSSSTAGE_HALFWIDTH;
				m_RammingSavePos[nActionNum] = D3DXVECTOR3(fRandPosX, BOSSSTAGE_HEIGHT, 0.0f);
				for (int nCnt = 0; nCnt < 20; nCnt++)
				{
					CUi3D::Create(CUi3D::UI3DTYPE01_WARNING, 40, 30.0f, 30.0f,
						m_RammingSavePos[nActionNum] + D3DXVECTOR3(sinf(m_fSaveRammingRot[nActionNum]) * (60.0f * nCnt), cosf(m_fSaveRammingRot[nActionNum]) * (60.0f * nCnt), 0.0f), NULL_VECTOR3);
				}
			}
			nActionNum++;
		}

		if (nActionNum == m_nMAX_ACTION03_RAMMING_NUM)
		{
			nPatternTime = 0;
			nPattern++;
			nActionNum = 0;
		}
		break;
	case 2:
		if (nPatternTime < 50)
		{
			CParticle::SummonChargeParticle(CParticle::TYPE00_NORMAL, 10, 60, 5.0f, 30.0f, 30.0f, 300, 100, 10, false,
				Pos, D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), true);
		}
		else
		{
			nPattern++;
			nPatternTime = 0;
		}
		break;
	case 3:
		if (nPatternTime == 1)
		{
			CParticle::SummonParticle(CParticle::TYPE00_NORMAL, 120, 50, 50.0f, 50.0f, 200, 10, false,GetSenterPos(),D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), true);
			Pos = m_RammingSavePos[nActionNum];
		}
		CParticle::Create(CParticle::TYPE03_THUNDER, 30, 100.0f, 100.0f, GetSenterPos(), NULL_VECTOR3, NORMAL_COL, true);
		Move.x = sinf(m_fSaveRammingRot[nActionNum]) * 20.0f;
		Move.y = cosf(m_fSaveRammingRot[nActionNum]) * 20.0f;

		if (nActionNum < m_nMAX_ACTION03_RAMMING_NUM)
		{
			if (nActionNum < 4)
			{
				if (bIsWalling == true)
				{
					nActionNum++;
					Pos = m_RammingSavePos[nActionNum];
				}
			}
			else
			{
				if (bIsLanding == true)
				{
 					nActionNum++;
					if (nActionNum < 0 || nActionNum >= m_nMAX_ACTION03_RAMMING_NUM)
					{
						assert("ドライヤの技３が配列外アクセス");
					}
					Pos = m_RammingSavePos[nActionNum];
					pDraiyaAttack = CDraiyaAttack::Create(CAttack::ATTACKTYPE11_EXPLOSION, 20, 120, GetSenterPos(), NULL_VECTOR3, ONE_VECTOR3, NULL_VECTOR3, CAttack::COLLISIONTYPE03_BALL,
						ONE_VECTOR3 * 0.1f, false, 20);

					CManager::GetCamera()->SetShake(20, 0.2f);
					pDraiyaAttack->SetUseRatioLifeAlpha(true);
					pDraiyaAttack->SetUseAddScale(ONE_VECTOR3 * 0.1f, true);
				}
			}
		}

		if (nActionNum == m_nMAX_ACTION03_RAMMING_NUM)
		{//最後だけ行う処理
			for (int nCnt = 0; nCnt < 7; nCnt++)
			{//バレット発射
				pBullet = CBullet::Create(CBullet::BULLETTYPE_ENEMY, CBullet::BULLETTEX_ENEMY, 120, 10, 30.0f, 30.0f, GetSenterPos(), NULL_VECTOR3);
				fRandRot = (2.0f / 7.0f) * nCnt;
				pBullet->SetUseLengthCurve(true, D3DX_PI* fRandRot, 0.02f, 5.0f);
				pBullet->SetUseEffect(40,D3DXVECTOR2(30.0f,30.0f),NORMAL_COL);
			}
			for (int nCnt = 0; nCnt < 7; nCnt++)
			{//バレット発射
				pBullet = CBullet::Create(CBullet::BULLETTYPE_ENEMY, CBullet::BULLETTEX_ENEMY, 120, 10, 30.0f, 30.0f, GetSenterPos(), NULL_VECTOR3);
				fRandRot = (2.0f / 7.0f) * nCnt;
				pBullet->SetUseLengthCurve(true, D3DX_PI * fRandRot, -0.02f, 5.0f);
				pBullet->SetUseEffect(40, D3DXVECTOR2(30.0f, 30.0f), NORMAL_COL);
			}

			Pos = GetPosOld();
			Move = NULL_VECTOR3;
			ResetPattern();
			SetUseGravity(true);
			SetUseInteria(true);
		}
		break;
	default:
		break;
	}

}
//=====================================================================================================================================

//==================================================================
//行動パターン４（必殺技）
//==================================================================
void CDraiya::Action4()
{
	int& nPatternTime = GetPatternTime();         //パターン時間を取得する
	bool& bAction = GetAction();                  //行動しているかどうかを取得
	int& nPattern = GetPattern();                 //現在のパターンを取得
	int& nActionNum = GetActionNum();             //アクション回数をカウント
	int& nSubPattern = GetSubPattern();           //サブパターンを取得
	D3DXVECTOR3& Pos = GetPos();                  //位置を取得
	D3DXVECTOR3& Rot = GetRot();                  //向きを取得
	D3DXVECTOR3& Move = GetMove();                //移動量を取得
	D3DXVECTOR3 PlayerPos = CGame::GetPlayer()->GetPos();//プレイヤーの位置
	D3DXVECTOR3 PlayerSenterPos = CGame::GetPlayer()->GetSenterPos();
	int nRand = 0;//乱数用
	CBullet* pBullet = nullptr;//弾へのポインタ
	D3DXVECTOR3& PurposePos = GetPurposePos();//目的地を使用
	bool& bIsLanding = GetLanding();
	bool& bIsWalling = GetWalling();
	float fRandRot = 0.0f;
	float& fSaveRot = GetSaveRot();//保存する向きを取得する
	CDraiyaAttack* pDraiyaAttack = nullptr;
	CUi3D* pUi3D = nullptr;
	float fRandPosY = 0.0f;//乱数位置Y
	float fRandPosX = 0.0f;//乱数位置X
	switch (nPattern)
	{
	case 0://溜める
		if (nPatternTime == 30)
		{//虹色チャージ
			CParticle::SummonChargeParticle(CParticle::TYPE00_NORMAL, 100, 120, 5.0f, 30.0f, 30.0f, 300, 100, 10, true,
				Pos, D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), true);
		}

		if (nPatternTime == 90)
		{
			SetUseInteria(false);//慣性を使用しない
			SetUseGravity(false);//重力を使用しない
			nPattern++;
			nPatternTime = 0;
		}
		break;
	case 1:
		if (nPatternTime == 1)
		{
			fRandRot = CCalculation::CalculationXYaim(Pos, NULL_VECTOR3);
			Move.x = sinf(fRandRot) * 10.0f;
			Move.y = cosf(fRandRot) * 10.0f;
		}

		if (Pos.x >= -20.0f && Pos.x <= 20.0f &&
			Pos.y >= -20.0f && Pos.y <= 20.0f)
		{
			Pos = NULL_VECTOR3;
			Move = NULL_VECTOR3;
			nPattern++;
			nPatternTime = 0;
		}
		break;
	case 2:
		Move.z = 10.0f;

		if (Pos.z > m_fACTION4_BACKMOCELENGTH - 20.0f && Pos.z < m_fACTION4_BACKMOCELENGTH + 20.0f)
		{
			Move = NULL_VECTOR3;
			nPattern++;
			nPatternTime = 0;
		}
		break;
	case 3:
		if (nPatternTime % m_nACTION4_THUNDERSUMMONSPAN == 0 && nPatternTime >= m_nACTION4_THUNDERSUMMONSPAN)
		{
			CUi3D::Create(CUi3D::UI3DTYPE01_WARNING, 120, 30.0f, 30.0f, PlayerSenterPos + D3DXVECTOR3(sinf(D3DX_PI * 0.25f) * 300.0f, cosf(D3DX_PI * 0.25f) * 300.0f, 0.0f),
				D3DXVECTOR3(sinf(D3DX_PI * 1.25f) * 30.0f, cosf(D3DX_PI * 1.25f) * 30.0f, 0.0f));
			CUi3D::Create(CUi3D::UI3DTYPE01_WARNING, 120, 30.0f, 30.0f, PlayerSenterPos + D3DXVECTOR3(sinf(D3DX_PI * -0.25f) * 300.0f, cosf(D3DX_PI * -0.25f) * 300.0f, 0.0f),
				D3DXVECTOR3(sinf(D3DX_PI * -1.25f) * 30.0f, cosf(D3DX_PI * -1.25f) * 30.0f, 0.0f));

			pDraiyaAttack = CDraiyaAttack::Create(CAttack::ATTACKTYPE06_THUNDER, 10, 120,
				NULL_VECTOR3,
				D3DXVECTOR3(0.0f,-m_fACTION4_THUNDERSPEED,0.0f),
				D3DXVECTOR3(1.0f, m_fACTION4_THUNDERSCALE_Y, 1.0f), NULL_VECTOR3,CAttack::COLLISIONTYPE02_TRUERECTANGLEXY,D3DXVECTOR3(0.0f,0.6f,0.0f), false, 30);

			pDraiyaAttack->SetParentInfo(PlayerSenterPos + D3DXVECTOR3(sinf(D3DX_PI * 0.25f) * m_fACTION4_SUMMONTHUNDERLENGTH, cosf(D3DX_PI * 0.25f) * m_fACTION4_SUMMONTHUNDERLENGTH, 0.0f),
				D3DXVECTOR3(0.0f, 0.0f, D3DX_PI * 1.75f), ONE_VECTOR3, true);

			pDraiyaAttack = CDraiyaAttack::Create(CAttack::ATTACKTYPE06_THUNDER, 10, 120,
				NULL_VECTOR3,
				D3DXVECTOR3(0.0f,-m_fACTION4_THUNDERSPEED,0.0f),
				D3DXVECTOR3(1.0f, m_fACTION4_THUNDERSCALE_Y, 1.0f), NULL_VECTOR3,CAttack::COLLISIONTYPE02_TRUERECTANGLEXY,D3DXVECTOR3(0.0f,0.6f,0.0f), false, 30);

			pDraiyaAttack->SetParentInfo(PlayerSenterPos + D3DXVECTOR3(sinf(D3DX_PI * -0.25f) * m_fACTION4_SUMMONTHUNDERLENGTH, cosf(D3DX_PI * -0.25f) * m_fACTION4_SUMMONTHUNDERLENGTH, 0.0f),
				D3DXVECTOR3(0.0f, 0.0f, -D3DX_PI * 1.75f), ONE_VECTOR3, true);
			nActionNum++;
		}

		if (nActionNum == m_nACTION4_THUNDERSHOTNUM)
		{
			nPatternTime = 0;
			nPattern++;
			nActionNum = 0;
		}
		break;
	case 4:
		if (nPatternTime % m_nACTION4_THUNDERSUMMONSPAN == 0)
		{
			CUi3D::Create(CUi3D::UI3DTYPE01_WARNING, 120, 30.0f, 30.0f, PlayerSenterPos + D3DXVECTOR3(sinf(D3DX_PI * 0.5f) * 300.0f, cosf(D3DX_PI * 0.5f) * 300.0f, 0.0f),
				D3DXVECTOR3(sinf(D3DX_PI * 1.5f) * 30.0f, cosf(D3DX_PI * 1.5f) * 30.0f, 0.0f));
			CUi3D::Create(CUi3D::UI3DTYPE01_WARNING, 120, 30.0f, 30.0f, PlayerSenterPos + D3DXVECTOR3(sinf(D3DX_PI * 0.0f) * 300.0f, cosf(D3DX_PI * 0.0f) * 300.0f, 0.0f),
				D3DXVECTOR3(sinf(D3DX_PI * 1.0f) * 30.0f, cosf(D3DX_PI * 1.0f) * 30.0f, 0.0f));

			pDraiyaAttack = CDraiyaAttack::Create(CAttack::ATTACKTYPE06_THUNDER, 10, 120,
				NULL_VECTOR3,
				D3DXVECTOR3(0.0f, -m_fACTION4_THUNDERSPEED, 0.0f),
				D3DXVECTOR3(1.0f, m_fACTION4_THUNDERSCALE_Y, 1.0f), NULL_VECTOR3, CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, D3DXVECTOR3(0.0f, 0.6f, 0.0f), false, 30);

			pDraiyaAttack->SetParentInfo(PlayerSenterPos + D3DXVECTOR3(sinf(D3DX_PI * 0.0f) * m_fACTION4_SUMMONTHUNDERLENGTH, cosf(D3DX_PI * 0.0f) * m_fACTION4_SUMMONTHUNDERLENGTH, 0.0f),
				D3DXVECTOR3(0.0f, 0.0f, D3DX_PI * 0.0f), ONE_VECTOR3, true);

			pDraiyaAttack = CDraiyaAttack::Create(CAttack::ATTACKTYPE06_THUNDER, 10, 120,
				NULL_VECTOR3,
				D3DXVECTOR3(0.0f, -m_fACTION4_THUNDERSPEED, 0.0f),
				D3DXVECTOR3(1.0f, m_fACTION4_THUNDERSCALE_Y, 1.0f), NULL_VECTOR3, CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, D3DXVECTOR3(0.0f, 0.6f, 0.0f), false, 30);

			pDraiyaAttack->SetParentInfo(PlayerSenterPos + D3DXVECTOR3(sinf(D3DX_PI * 0.5f) * m_fACTION4_SUMMONTHUNDERLENGTH, cosf(D3DX_PI * 0.5f) * m_fACTION4_SUMMONTHUNDERLENGTH, 0.0f),
				D3DXVECTOR3(0.0f, 0.0f, -D3DX_PI * 0.5f), ONE_VECTOR3, true);

			nActionNum++;
		}

		if (nActionNum == m_nACTION4_THUNDERSHOTNUM)
		{
			nActionNum = 0;
			nPatternTime = 0;
			nPattern++;
		}
		break;
	case 5:
		if (nPatternTime % 60 == 0)
		{
			nRand = rand() % 2;

			if (nRand == 0)
			{
				CUi3D::Create(CUi3D::UI3DTYPE01_WARNING, 120, 30.0f, 30.0f, PlayerSenterPos + D3DXVECTOR3(sinf(D3DX_PI * 0.25f) * 300.0f, cosf(D3DX_PI * 0.25f) * 300.0f, 0.0f),
					D3DXVECTOR3(sinf(D3DX_PI * 1.25f) * 30.0f, cosf(D3DX_PI * 1.25f) * 30.0f, 0.0f));
				CUi3D::Create(CUi3D::UI3DTYPE01_WARNING, 120, 30.0f, 30.0f, PlayerSenterPos + D3DXVECTOR3(sinf(D3DX_PI * -0.25f) * 300.0f, cosf(D3DX_PI * -0.25f) * 300.0f, 0.0f),
					D3DXVECTOR3(sinf(D3DX_PI * -1.25f) * 30.0f, cosf(D3DX_PI * -1.25f) * 30.0f, 0.0f));

				pDraiyaAttack = CDraiyaAttack::Create(CAttack::ATTACKTYPE06_THUNDER, 10, 120,
					NULL_VECTOR3,
					D3DXVECTOR3(0.0f, -m_fACTION4_THUNDERSPEED, 0.0f),
					D3DXVECTOR3(1.0f, m_fACTION4_THUNDERSCALE_Y, 1.0f), NULL_VECTOR3, CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, D3DXVECTOR3(0.0f, 0.6f, 0.0f), false, 30);

				pDraiyaAttack->SetParentInfo(PlayerSenterPos + D3DXVECTOR3(sinf(D3DX_PI * 0.25f) * m_fACTION4_SUMMONTHUNDERLENGTH, cosf(D3DX_PI * 0.25f) * m_fACTION4_SUMMONTHUNDERLENGTH, 0.0f),
					D3DXVECTOR3(0.0f, 0.0f, D3DX_PI * 1.75f), ONE_VECTOR3, true);

				pDraiyaAttack = CDraiyaAttack::Create(CAttack::ATTACKTYPE06_THUNDER, 10, 120,
					NULL_VECTOR3,
					D3DXVECTOR3(0.0f, -m_fACTION4_THUNDERSPEED, 0.0f),
					D3DXVECTOR3(1.0f, m_fACTION4_THUNDERSCALE_Y, 1.0f), NULL_VECTOR3, CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, D3DXVECTOR3(0.0f, 0.6f, 0.0f), false, 30);

				pDraiyaAttack->SetParentInfo(PlayerSenterPos + D3DXVECTOR3(sinf(D3DX_PI * -0.25f) * m_fACTION4_SUMMONTHUNDERLENGTH, cosf(D3DX_PI * -0.25f) * m_fACTION4_SUMMONTHUNDERLENGTH, 0.0f),
					D3DXVECTOR3(0.0f, 0.0f, -D3DX_PI * 1.75f), ONE_VECTOR3, true);
				nActionNum++;
			}
			else
			{
				CUi3D::Create(CUi3D::UI3DTYPE01_WARNING, 120, 30.0f, 30.0f, PlayerSenterPos + D3DXVECTOR3(sinf(D3DX_PI * 0.5f) * 300.0f, cosf(D3DX_PI * 0.5f) * 300.0f, 0.0f),
					D3DXVECTOR3(sinf(D3DX_PI * 1.5f) * 30.0f, cosf(D3DX_PI * 1.5f) * 30.0f, 0.0f));
				CUi3D::Create(CUi3D::UI3DTYPE01_WARNING, 120, 30.0f, 30.0f, PlayerSenterPos + D3DXVECTOR3(sinf(D3DX_PI * 0.0f) * 300.0f, cosf(D3DX_PI * 0.0f) * 300.0f, 0.0f),
					D3DXVECTOR3(sinf(D3DX_PI * 1.0f) * 30.0f, cosf(D3DX_PI * 1.0f) * 30.0f, 0.0f));

				pDraiyaAttack = CDraiyaAttack::Create(CAttack::ATTACKTYPE06_THUNDER, 10, 120,
					NULL_VECTOR3,
					D3DXVECTOR3(0.0f, -m_fACTION4_THUNDERSPEED, 0.0f),
					D3DXVECTOR3(1.0f, m_fACTION4_THUNDERSCALE_Y, 1.0f), NULL_VECTOR3, CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, D3DXVECTOR3(0.0f, 0.6f, 0.0f), false, 30);

				pDraiyaAttack->SetParentInfo(PlayerSenterPos + D3DXVECTOR3(sinf(D3DX_PI * 0.0f) * m_fACTION4_SUMMONTHUNDERLENGTH, cosf(D3DX_PI * 0.0f) * m_fACTION4_SUMMONTHUNDERLENGTH, 0.0f),
					D3DXVECTOR3(0.0f, 0.0f, D3DX_PI * 0.0f), ONE_VECTOR3, true);

				pDraiyaAttack = CDraiyaAttack::Create(CAttack::ATTACKTYPE06_THUNDER, 10, 120,
					NULL_VECTOR3,
					D3DXVECTOR3(0.0f, -m_fACTION4_THUNDERSPEED, 0.0f),
					D3DXVECTOR3(1.0f, m_fACTION4_THUNDERSCALE_Y, 1.0f), NULL_VECTOR3, CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, D3DXVECTOR3(0.0f, 0.6f, 0.0f), false, 30);

				pDraiyaAttack->SetParentInfo(PlayerSenterPos + D3DXVECTOR3(sinf(D3DX_PI * 0.5f) * m_fACTION4_SUMMONTHUNDERLENGTH, cosf(D3DX_PI * 0.5f) * m_fACTION4_SUMMONTHUNDERLENGTH, 0.0f),
					D3DXVECTOR3(0.0f, 0.0f, -D3DX_PI * 0.5f), ONE_VECTOR3, true);

				nActionNum++;
			}
		 
			if (nActionNum == m_nACTION4_THUNDERSHOTNUM * 3)
			{
				nPattern++;
				nPatternTime = 0;
				nActionNum = 0;
			}
		}
		break;
	case 6:
		if (nPatternTime == 1)
		{

			for (int nCnt = 0; nCnt < (nActionNum + 1) * 2; nCnt++)
			{
				if (nCnt < 0 || nCnt >= m_nNUMSPECIALATTACK)
				{
					assert("ドライヤの技４が配列外アクセス");
				}
				float fAddRot = float(rand() % 100 - 50) / 200;
				m_pSpecialThunder[nCnt] = CDraiyaAttack::Create(CAttack::ATTACKTYPE19_SENTERTHUNDER, 10, 9999, GetSenterPos(), NULL_VECTOR3,
					D3DXVECTOR3(2.0f, 10.0f, 2.0f), NULL_VECTOR3, CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, D3DXVECTOR3(0.0f, 0.0f, fAddRot), false, 10);
			}
		}

		for (int nCnt = 0; nCnt < (nActionNum + 1) * 2; nCnt++)
		{
			float& fAddRotThunder = m_pSpecialThunder[nCnt]->GetAddRot().z;
			fAddRotThunder *= 0.98f;
		}

		if (nPatternTime == 120)
		{
			for (int nCnt = 0; nCnt < (nActionNum + 1) * 2; nCnt++)
			{
				if (nCnt < 0 || nCnt >= m_nNUMSPECIALATTACK)
				{
					assert("ドライヤの技４が配列外アクセス");
				}
				m_pSpecialThunder[nCnt]->SetMove(D3DXVECTOR3(0.0f, 0.0f, -30.0f));
			}
		}

		if (nPatternTime == 180)
		{
			for (int nCnt = 0; nCnt < m_nNUMSPECIALATTACK; nCnt++)
			{
				if (m_pSpecialThunder[nCnt] != nullptr)
				{
					m_pSpecialThunder[nCnt]->SetUseDeath(true);
					m_pSpecialThunder[nCnt]->SetDeath();
					m_pSpecialThunder[nCnt] = nullptr;
				}
			}

			nActionNum++;
			nPatternTime = 0;

			if (nActionNum == 4)
			{
				nPattern++;
				nPatternTime = 0;
				nActionNum = 0;
			}
		}
		break;
	case 7:
		Move.z = -10.0f;
		if (Pos.z < 20.0f && Pos.z > -20.0f)
		{
			Move = NULL_VECTOR3;
			Pos.z = 0.0f;
			ResetPattern();
			SetUseInteria(true);
			SetUseGravity(true);
		}
		break;
	default:
		break;
	}

}
//=====================================================================================================================================

//============================================<<<ガブリエルクラス>>>=============================================================================	

//====================================================================
//コンストラクタ
//====================================================================
CGabriel::CGabriel() : m_fMoveLength(0.0f),m_fMoveRot(0.0f),m_bMoveAim(false),
m_SaveMoveFulcrumPos(NULL_VECTOR3),m_pCircleBullet(),m_pSubAiModel(nullptr)
{

}
//=====================================================================================================================================

//====================================================================
//デストラクタ
//====================================================================
CGabriel::~CGabriel()
{

}
//=====================================================================================================================================

//====================================================================
//初期化処理
//====================================================================
HRESULT CGabriel::Init()
{
	CBoss::Init();

	return S_OK;
}
//=====================================================================================================================================

//====================================================================
//終了処理
//====================================================================
void CGabriel::Uninit()
{
	CBoss::Uninit();
}
//======================================================================================================================================

//====================================================================
//更新処理
//====================================================================
void CGabriel::Update()
{
	CBoss::Update();
}
//======================================================================================================================================

//====================================================================
//描画処理
//====================================================================
void CGabriel::Draw()
{
	CBoss::Draw();
}
//=====================================================================================================================================

//====================================================================
//死亡フラグ設定処理
//====================================================================
void CGabriel::SetDeath()
{
	CBoss::SetDeath();

	for (int nCnt = 0; nCnt < m_nMAX_ACTION0BULLETNUM; nCnt++)
	{
		if (m_pCircleBullet[nCnt] != nullptr)
		{
			m_pCircleBullet[nCnt]->SetUseDeath(true);
			m_pCircleBullet[nCnt]->SetDeath();
			m_pCircleBullet[nCnt] = nullptr;
		}
	}

	if (m_pSubAiModel != nullptr)
	{
		m_pSubAiModel->SetUseDeath(true);
		m_pSubAiModel->SetDeath();
		m_pSubAiModel = nullptr;
	}
}
//====================================================================================================================================

//==================================================================
//生成処理
//==================================================================
CGabriel* CGabriel::Create(BOSSTYPE type, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 Scale)
{
	CGabriel* pGabriel = DBG_NEW CGabriel;   //ボスを生成
	bool bSuccess = pGabriel->GetCreateSuccess();
	int nIdx = 0;//モデルのインデックス
	if (bSuccess == true)
	{
		if (pGabriel != nullptr)
		{
			pGabriel->Init();                             //初期化処理
			pGabriel->SetUseDeath(true);                  //死亡フラグを発動するかどうかを設定する
			pGabriel->SetType(type);                      //ボスの種類
			pGabriel->SetLife(nLife);                     //体力
			pGabriel->SetMaxLife(nLife);//最大体力
			pGabriel->SetAutoSubLife(false);              //自動的に体力を減らすかどうか
			pGabriel->CObjectX::SetPos(pos);                                                                                  //オブジェクト２Ｄの位置を設定
			pGabriel->SetPosOld(pos);                                                                                         //1f前の位置を設定
			pGabriel->SetSupportPos(pos);                                                                                     //設置位置
			pGabriel->CObject::SetType(CObject::TYPE_BOSS);                                                                   //オブジェクトの種類を決める
			pGabriel->CObjectX::SetScale(Scale);                                                                              //オブジェクトの拡大率
			pGabriel->SetFormarScale(Scale);                                                                                  //元の拡大率を設定する
			nIdx = CManager::GetObjectXInfo()->Regist(CObjectX::OBJECTXTYPE_BOSS, m_BOSS_FILENAME[(int)(type)]);
			pGabriel->CObjectX::BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(CObjectX::OBJECTXTYPE_BOSS, nIdx),
				CManager::GetObjectXInfo()->GetBuffMat(CObjectX::OBJECTXTYPE_BOSS, nIdx),
				CManager::GetObjectXInfo()->GetdwNumMat(CObjectX::OBJECTXTYPE_BOSS, nIdx),
				CManager::GetObjectXInfo()->GetTexture(CObjectX::OBJECTXTYPE_BOSS, nIdx),
				CManager::GetObjectXInfo()->GetColorValue(CObjectX::OBJECTXTYPE_BOSS, nIdx));                                   //モデル情報を割り当てる
			pGabriel->SetBossHpGauge(nLife);                                                                                  //体力ゲージを設定する
			pGabriel->CObjectX::SetObjXType(CObjectX::OBJECTXTYPE_BOSS);                                                      //オブジェクトXのタイプを設定
			pGabriel->CObjectX::SetTypeNum((int)(type));                                                                      //オブジェクトXごとのタイプ番号を設定
			pGabriel->SetSize();                                                                                              //Xオブジェクトのサイズを設定する
			pGabriel->CObjectX::SetRotType(CObjectX::ROTTYPE::ROTTYPE_NORMAL);                                                //向きの種類を設定
			pGabriel->SetManagerType((int)(CStageManager::MANAGEROBJECT_BOSS));                                               //マネージャーで呼び出す時の種類を設定
			pGabriel->AdjustSize();      //判定サイズ調整を行う
		}
		else
		{//オブジェクトに空きがなかったので破棄する
			delete pGabriel;
			pGabriel = nullptr;
			return nullptr;
		}
	}
	return pGabriel;
}
//=====================================================================================================================================

//==================================================================
//移動パターン
//==================================================================
void CGabriel::MovePattern()
{
	int& nMovePatternFrame = GetMovePatternFrame();
	int& nMaxMovePatternFrame = GetMaxMovePatternFrame();
	bool& bUseMovePattern = GetUseMovePattern();//移動パターンを使用しているかどうか
	int& nMovePatternNum = GetMovePatternNum();//移動パターン番号を取得する
	D3DXVECTOR3& Move = GetMove();

	CBoss::MovePattern();//移動パターンの共通処理

	DecideMovePattern();//移動パターン決定処理

	//移動パターンを開始する
	switch (nMovePatternNum)
	{
	case 0:
		MovePattern0();//移動パターン０
		break;
	default:
		break;
	}

	//移動パターンを終わらせる
	if (nMovePatternFrame > nMaxMovePatternFrame)
	{
		bUseMovePattern = false;//アクションパターンへ移行
		nMovePatternFrame = 0;
		Move = NULL_VECTOR3;
	}

}
//=====================================================================================================================================

//==================================================================
//移動パターン０
//==================================================================
void CGabriel::MovePattern0()
{
	D3DXVECTOR3& Move = GetMove();
	D3DXVECTOR3& Pos = GetPos();
	D3DXVECTOR3 PlayerPos = CGame::GetPlayer()->GetPos();
	int nMovePatternFrame = GetMovePatternFrame();
	float fLength = CCalculation::CalculationLength(Pos, PlayerPos);
	bool bMove = true;
	if (nMovePatternFrame == 15)
	{
		CParticle::SummonChargeParticle(CParticle::TYPE03_THUNDER, 100, 60, 10.0f, 30.0f, 30.0f, 200, 100, 10, false, GetSenterPos(), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), true);
	}

	if (nMovePatternFrame == 30)
	{
		m_fMoveLength = CCalculation::CalculationLength(Pos, PlayerPos);
		if (Pos.x < PlayerPos.x)
		{//右にいる
			m_bMoveAim = true;
			m_fMoveRot = -D3DX_PI * 0.5f;
		}
		else
		{//左にいる
			m_bMoveAim = false;
			m_fMoveRot = D3DX_PI * 0.5f;
		}

		m_SaveMoveFulcrumPos = PlayerPos;
	}

	if (nMovePatternFrame > 40 && Pos.z >= -20.0f && Pos.z <= 20.0f)
	{
		bMove = false;
	}

	if (nMovePatternFrame > 30 && bMove == true)
	{
		if (m_bMoveAim == true)
		{
			m_fMoveRot += 0.05f;
		}
		else
		{
			m_fMoveRot -= 0.05f;
		}

		Pos.x = m_SaveMoveFulcrumPos.x + sinf(m_fMoveRot) * m_fMoveLength;
		Pos.z = m_SaveMoveFulcrumPos.z + cosf(m_fMoveRot) * m_fMoveLength;

		if (Pos.x + GetVtxMin().x < -BOSSSTAGE_HALFWIDTH)
		{
			Pos.x = -BOSSSTAGE_HALFWIDTH - GetVtxMin().x;
		}
		else if (Pos.x + GetVtxMax().x > BOSSSTAGE_HALFWIDTH)
		{
			Pos.x = BOSSSTAGE_HALFWIDTH - GetVtxMax().x;
		}
	}

	if (nMovePatternFrame == m_nMAX_MOVEPATTERN0_FRAME)
	{
		SetUndoInertia();//慣性を元に戻す
		m_fMoveRot = 0.0f;
		m_bMoveAim = false;
		m_SaveMoveFulcrumPos = NULL_VECTOR3;
		m_fMoveLength = 0.0f;
		nMovePatternFrame = 0;
	}

}
//=====================================================================================================================================

//==================================================================
//移動パターン決定処理
//==================================================================
void CGabriel::DecideMovePattern()
{
	int& nMovePatternFrame = GetMovePatternFrame();
	int& nMaxMovePatternFrame = GetMaxMovePatternFrame();
	bool& bUseMovePattern = GetUseMovePattern();//移動パターンを使用しているかどうか
	int& nMovePatternNum = GetMovePatternNum();//移動パターン番号を取得する
	D3DXVECTOR3 Pos = GetPos();
	D3DXVECTOR3 PlayerPos = CGame::GetPlayer()->GetPos();

	//距離を取る
	float fLength = CCalculation::CalculationLength(Pos, PlayerPos);

	if (nMovePatternFrame == 1)
	{//最初に移動方法を決める

		nMovePatternNum = 0;
		//それぞれの移動パターンの最大フレーム数を設定する
		switch (nMovePatternNum)
		{
		case 0:
			nMaxMovePatternFrame = m_nMAX_MOVEPATTERN0_FRAME;
			break;
		default:
			break;
		}
	}
}
//=====================================================================================================================================

//==================================================================
//サイズ調整処理
//==================================================================
void CGabriel::AdjustSize()
{
	D3DXVECTOR3& VtxMax = GetOriginVtxMax();
	D3DXVECTOR3& VtxMin = GetOriginVtxMin();

	VtxMax.y -= 25.0f;

	VtxMax.x -= 20.0f;
	VtxMin.x += 20.0f;

}
//=====================================================================================================================================

//==================================================================
//行動パターン
//==================================================================
void CGabriel::ActionPattern()
{
	bool& bAction = GetAction();    //行動しているかどうか
	int& nSkill = GetSkill();       //スキル
	bool& bInsanity = GetInsanity();//発狂しているかどうか
	int nLife = GetLife();          //体力
	int& nCntTurn = GetTurnCnt();   //ターン数をカウント
	int nMaxLife = GetMaxLife();    //最大体力
	float fRatioLife = 0.0f;        //体力の割合

	fRatioLife = float(nLife) / float(nMaxLife);

	if (fRatioLife < 0.5f)
	{//体力が５０％を下回ったら発狂状態にする
		bInsanity = true;
	}

	if (bAction == false)
	{
		if (bInsanity == false)
		{//発狂状態じゃなければ
			nSkill = rand() % 1;
			//nSkill = 4;
		}
		else
		{//発狂状態なら
			nCntTurn++;//ここからターン数をカウント
			nSkill = rand() % 1;
			if (nCntTurn == 1)
			{
				nSkill = 0;
			}
		}
		bAction = true;
	}

	if (bAction == true)
	{
		CBoss::ActionPattern();
		switch (nSkill)
		{
		case 0:
			Action0();
			break;
		default:
			break;
		}
	}

}
//=====================================================================================================================================

//==================================================================
//行動パターン０
//==================================================================
void CGabriel::Action0()
{
	int& nPatternTime = GetPatternTime();         //パターン時間を取得する
	bool& bAction = GetAction();                  //行動しているかどうかを取得
	int& nPattern = GetPattern();                 //現在のパターンを取得
	int& nActionNum = GetActionNum();             //アクション回数をカウント
	int& nSubPattern = GetSubPattern();           //サブパターンを取得
	D3DXVECTOR3& Pos = GetPos();                  //位置を取得
	D3DXVECTOR3 SenterPos = GetSenterPos();       //中心点を取得
	D3DXVECTOR3& Rot = GetRot();                  //向きを取得
	D3DXVECTOR3& Move = GetMove();                //移動量を取得
	D3DXVECTOR3 PlayerPos = CGame::GetPlayer()->GetPos();//プレイヤーの位置
	D3DXVECTOR3 PlayerSenterPos = CGame::GetPlayer()->GetSenterPos();//プレイヤーの中心の位置
	D3DXVECTOR3& PurposePos = GetPurposePos();//目的地を使用
	D3DXVECTOR3 AiRot = NULL_VECTOR3;

	bool& bIsLanding = GetLanding();
	bool bUseGravity = GetUseGravity();
	bool& bIsWalling = GetWalling();
	float fRandRot = 0.0f;
	float fRandRot2 = 0.0f;
	CGabrielAttack* pGabrielAttack = nullptr;

	switch (nPattern)
	{
	case 0://溜める
		if (nPatternTime == 30)
		{//赤色チャージ
			CParticle::SummonChargeParticle(CParticle::TYPE00_NORMAL, 100, 120, 5.0f, 30.0f, 30.0f, 300, 100, 10, true,
				Pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), true);
		}

		if (nPatternTime == 60)
		{
			SetUseGravity(false);//重力を使用しない
			nPattern++;
			nPatternTime = 0;
			Move = NULL_VECTOR3;
		}
		break;
	case 1:
		if (nPatternTime == 1)
		{
			m_fMoveLength = 200.0f;
			m_pSubAiModel = CAIModel::Create(CAIModel::AIMODELTYPE_BALL, PlayerPos, NULL_VECTOR3, ONE_VECTOR3);
			for (int nCnt = 0; nCnt < m_nMAX_ACTION0BULLETNUM; nCnt++)
			{
				if (m_pCircleBullet[nCnt] == nullptr)
				{
					fRandRot = (2.0f / m_nMAX_ACTION0BULLETNUM) * nCnt;
					m_pCircleBullet[nCnt] = CBullet::Create(CBullet::BULLETTYPE_ENEMY, CBullet::BULLETTEX_ENEMY, 9999, 10, 30.0f, 30.0f,
						D3DXVECTOR3(sinf(D3DX_PI * fRandRot) * m_fMoveLength, cosf(D3DX_PI * fRandRot) * m_fMoveLength, 0.0f), NULL_VECTOR3);
					m_pCircleBullet[nCnt]->SetUseDeath(false);
					m_pCircleBullet[nCnt]->SetUseMtxChild(&m_pSubAiModel->GetMatrixWorld(), true);
				}
			}

			nPatternTime = 0;
			nPattern++;
		}
		break;
	case 2:

		//============================================
		//弾の動き
		//============================================
		if (nPatternTime == 1)
		{
			m_pSubAiModel->SetUseAddRot(true, D3DXVECTOR3(0.0f, 0.0f, 0.01f));
			m_pSubAiModel->SetUseDraw(false);
		}
		if (nPatternTime % 30 == 0)
		{
			for (int nCnt = 0; nCnt < m_nMAX_ACTION0BULLETNUM; nCnt++)
			{
				fRandRot = CCalculation::CalculationRandVecXY();
				if (m_pCircleBullet[nCnt]->GetPos().y > 0.0f)
				{
					CBullet::Create(CBullet::BULLETTYPE_ENEMY, CBullet::BULLETTEX_ENEMY, 200, 10, 10.0f, 10.0f, m_pCircleBullet[nCnt]->GetMtxTransPos(),
						D3DXVECTOR3(sinf(fRandRot) * 1.0f, cosf(fRandRot) * 1.0f, 0.0f));
				}
			}
		}

		if (nPatternTime >= 400)
		{
			m_pSubAiModel->SetUseAddRot(true, NULL_VECTOR3);
			for (int nCnt = 0; nCnt < m_nMAX_ACTION0BULLETNUM; nCnt++)
			{
				fRandRot = (2.0f / m_nMAX_ACTION0BULLETNUM) * nCnt;
				m_pCircleBullet[nCnt]->SetPos(D3DXVECTOR3(sinf(D3DX_PI * fRandRot) * m_fMoveLength, cosf(D3DX_PI * fRandRot) * m_fMoveLength, 0.0f));

			}
			m_fMoveLength -= 8.0f;
		}
		else
		{
			m_pSubAiModel->SetPos(PlayerSenterPos);
		}

		if (nPatternTime == 500)
		{
			nPattern++;
			nPatternTime = 0;

			for (int nCnt = 0; nCnt < m_nMAX_ACTION0BULLETNUM; nCnt++)
			{
				m_pCircleBullet[nCnt]->SetUseDeath(true);
				m_pCircleBullet[nCnt]->SetDeath();
				m_pCircleBullet[nCnt] = nullptr;
			}
			m_pSubAiModel->SetUseDeath(true);
			m_pSubAiModel->SetDeath();
			m_pSubAiModel = nullptr;

			m_fMoveLength = 0.0f;

			SetUseGravity(true);
			SetUseInteria(true);

			ResetPattern();
		}
		//=======================================================================================================

		//=====================================
		//ボスの動き
		//=====================================

		if (nPatternTime % 50 == 0)
		{
			CParticle::SummonChargeParticle(CParticle::TYPE00_NORMAL, 100, 60, 10, 30.0f, 30.0f, 200, 100, 10, false, GetSenterPos(), NORMAL_COL, true);
			
			fRandRot2 = CCalculation::CalculationXYaim(SenterPos, PlayerSenterPos);
			pGabrielAttack = CGabrielAttack::Create(CAttack::ATTACKTYPE01_METEO, 20, 180,
				SenterPos, D3DXVECTOR3(sinf(fRandRot2) * 1.0f, cosf(fRandRot2) * 1.0f, 0.0f), ONE_VECTOR3 * 2, D3DXVECTOR3(0.0f, 0.0f, fRandRot2 - D3DX_PI * 0.5f),
				CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, NULL_VECTOR3, true, 100);
			pGabrielAttack->SetUseMultiSpeed(true, ONE_VECTOR3 * 1.05f);
			pGabrielAttack->SetUseAttackEffect(true, 5, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
			pGabrielAttack->SetUseAddRot(true, ONE_VECTOR3 * 0.1f);
				

			fRandRot2 = float(rand() % 314 - 157) / 100;
			float fRandLength = float(rand() % 200 + 200);
			Pos = PlayerSenterPos + D3DXVECTOR3(sinf(fRandRot2) * fRandLength, cosf(fRandRot2) * fRandLength, 0.0f);
			if (Pos.x + GetVtxMin().x < -BOSSSTAGE_HALFWIDTH)
			{
				Pos.x = -BOSSSTAGE_HALFWIDTH - GetVtxMin().x;
			}
			if (Pos.x + GetVtxMax().x > BOSSSTAGE_HALFWIDTH)
			{
				Pos.x = BOSSSTAGE_HALFWIDTH - GetVtxMax().x;
			}

			if (Pos.y < 0.0f)
			{
				Pos.y = 0.0f;
			}
			CParticle::SummonParticle(CParticle::TYPE00_NORMAL, 100, 120, 30.0f, 30.0f, 200, 10, false, Pos, NORMAL_COL, true);
		}
		break;
	default:
		break;
	}
}
//=====================================================================================================================================

//==================================================================
//行動パターン１
//==================================================================
void CGabriel::Action1()
{
	int& nPatternTime = GetPatternTime();         //パターン時間を取得する
	bool& bAction = GetAction();                  //行動しているかどうかを取得
	int& nPattern = GetPattern();                 //現在のパターンを取得
	int& nActionNum = GetActionNum();             //アクション回数をカウント
	int& nSubPattern = GetSubPattern();           //サブパターンを取得
	D3DXVECTOR3& Pos = GetPos();                  //位置を取得
	D3DXVECTOR3 SenterPos = GetSenterPos();       //中心点を取得
	D3DXVECTOR3& Rot = GetRot();                  //向きを取得
	D3DXVECTOR3& Move = GetMove();                //移動量を取得
	D3DXVECTOR3 PlayerPos = CGame::GetPlayer()->GetPos();//プレイヤーの位置
	D3DXVECTOR3 PlayerSenterPos = CGame::GetPlayer()->GetSenterPos();//プレイヤーの中心の位置
	D3DXVECTOR3& PurposePos = GetPurposePos();//目的地を使用
	D3DXVECTOR3 AiRot = NULL_VECTOR3;
	bool& bIsLanding = GetLanding();
	bool bUseGravity = GetUseGravity();
	bool& bIsWalling = GetWalling();
	float fRandRot = 0.0f;
	float fRandRot2 = 0.0f;
	CGabrielAttack* pGabrielAttack = nullptr;


	switch (nPattern)
	{
	case 0://溜める
		if (nPatternTime == 30)
		{//赤色チャージ
			CParticle::SummonChargeParticle(CParticle::TYPE00_NORMAL, 100, 120, 5.0f, 30.0f, 30.0f, 300, 100, 10, true,
				Pos, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), true);
		}

		if (nPatternTime == 60)
		{
			SetUseGravity(false);//重力を使用しない
			nPattern++;
			nPatternTime = 0;
			Move = NULL_VECTOR3;
		}
		break;
	case 1://トルネードメテオ

		break;
	default:
		break;
	}
}
//=====================================================================================================================================
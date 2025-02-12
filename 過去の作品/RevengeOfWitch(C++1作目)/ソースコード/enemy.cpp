//=================================================
//
//７月２５日：敵の行動AIを実装[enemy.cpp]
//Author:ShinaTaiyo
//
//=================================================

//==============================
//インクルード
//==============================
#include "enemy.h"
#include "manager.h"
#include "game.h"
#include "edit.h"
#include "camera.h"
#include "bullet.h"
#include "attack.h"
#include "particle.h"
#include "main.h"
#include "damage.h"
#include "sound.h"
#include "input.h"
#include "texture.h"
#include "debugtext.h"
#include "manager.h"
#include "objectXInfo.h"
#include "stagemanager.h"
#include "calculation.h"
//=================================================

//==============================
//静的メンバ初期化
//==============================
const int CEnemy::m_nMAXENEMY01BULLET = 8;       //敵０１の弾の発射数
int CEnemy::m_nNumFile = CEnemy::ENEMYTYPE_MAX;  //ファイルの数
const char* CEnemy::m_ENEMY_FILENAME[CEnemy::ENEMYTYPE_MAX] =
{ "data\\MODEL\\Enemy\\00_noobSlime.x",
"data\\MODEL\\Enemy\\01_angrySlime.x" ,
"data\\MODEL\\Enemy\\02_swordSlime.x" ,
"data\\MODEL\\Enemy\\03_hammerSlime.x",
"data\\MODEL\\Enemy\\04_HugeSlime.x",
"data\\MODEL\\Enemy\\05_ThunderSlime.x",
"data\\MODEL\\Enemy\\06_ScanningSlime.x",
"data\\MODEL\\Enemy\\07_NeedleSlime.x",
"data\\MODEL\\Enemy\\08_engelSlime.x",
"data\\MODEL\\Enemy\\09_darkSlime.x",
"data\\MODEL\\Enemy\\10_RainbowSlime.x",
}; //敵のモデルのパス
//=================================================

//==============================
//コンストラクタ
//==============================
CEnemy::CEnemy()
{

}
//=================================================

//==============================
//デストラクタ
//==============================
CEnemy::~CEnemy()
{

}
//=================================================

//==============================
//初期化処理
//==============================
HRESULT CEnemy::Init()
{
	//===========================
	//オブジェクトX初期化
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
	m_bUse = false;          //使用状態
	m_bIsJumping = false;    //ジャンプ中かどうか
	m_bIsWalling = false;    //壁に当たっているかどうか
	m_bAttack = false;       //攻撃をするかどうか

	m_nPattern = 0;          //パターン番号
	m_nSubPattern = 0;       //サブのパターン
	m_nPatternTime = 0;      //パターンに入ってからの時間
	m_bPattern = false;      //パターンを実行するかどうか
	m_bAction = false;       //行動をするかどうか
	m_nCntAction = 0;        //アクション回数カウント用

	m_fLength = 0.0f;        //プレイヤーとの距離

	m_fGravityPower = 1.0f;  //重力の大きさ

	m_fShotRot = 0.0f;       //バレットなどを発射する向き

	m_bSpeedSwitch = false;  //速さを変えるスイッチ

	m_pLandingObj = nullptr;//乗っているオブジェクト判定用
	//==============================================================================================

	//====================================
	//敵のAI用モデル
	//====================================
	m_pMoveRangeAI = nullptr;
	//==============================================================================================

	return S_OK;
}
//====================================================================================================================================================================

//==============================
//終了処理
//==============================
void CEnemy::Uninit()
{
	CObjectX::Uninit();
	//SetDeath();
}
//=================================================

//==============================
//更新処理
//==============================
void CEnemy::Update()
{
	//==================================
	//オブジェクトXから情報を取得
	//==================================
	D3DXVECTOR3 &pos = GetPos();                                  //モデルの位置の取得
	D3DXVECTOR3 &PosOld = GetPosOld();                            //1f前の位置を設定
	D3DXVECTOR3& Move = GetMove();                                //移動量を取得
	D3DXVECTOR3 &Rot = GetRot();                                  //モデルの向きの取得
	D3DXVECTOR3 Size = GetSize();                                 //サイズを取得
	bool& bIsLanding = GetLanding();                              //地面にいるかどうか
	bool& bUseGravity = GetUseGravity();                          //
	int& nLife = GetLife();
	//===========================================================

	if (CScene::GetMode() == CScene::MODE_GAME)
	{
		GravityProcess();//重力の処理
	}

	//==================================
	//敵ごとの攻撃パターン
	//==================================
	m_nCntTime++;//出現してからの時間を測る

	if (CScene::GetMode() == CScene::MODE_GAME)
	{
		CalculationLength();//距離を計算する

	    //==========================================
	    //行動条件
	    //==========================================
		if (m_fLength < 500.0f)
		{
			m_bAction = true;
		}
		else
		{
			m_bAction = false;
			//行動パターンのリセット
			ResetPattern();
			Move.x = 0.0f;
			Rot = NULL_VECTOR3;
			if (m_type == ENEMYTYPE08_ENGELSLIME)
			{
				Move.y = 0.0f;
			}
		}
		//===========================================================================

		if (m_bAction == true)
		{
			switch (m_type)
			{
			case ENEMYTYPE00_SLIME://スライム
				Enemy00Pattern();
				break;
			case ENEMYTYPE01_FIRESLIME://ファイアスライム
				Enemy01Pattern();
				break;
			case ENEMYTYPE02_SWORDSLIME://ソードスライム
				Enemy02Pattern();
				break;
			case ENEMYTYPE03_HAMMERSLIME://ハンマースライム
				Enemy03Pattern();
				break;
			case ENEMYTYPE04_ORANGESLIME://オレンジスライム
				Enemy04Pattern();
				break;
			case ENEMYTYPE05_THUNDERSLIME://サンダースライム
				Enemy05Pattern();
				break;
			case ENEMYTYPE06_SCANINGSLIME://スキャニングスライム
				Enemy06Pattern();
				break;
			case ENEMYTYPE07_NEEDLESLIME://ニードルスライム
				Enemy07Pattern();
				break;
			case ENEMYTYPE08_ENGELSLIME://エンジェルスライム
				Enemy08Pattern();
				break;
			case ENEMYTYPE09_DARKSLIME://ダークスライム
				Enemy09Pattern();
				break;
			case ENEMYTYPE10_RAINBOWSLIME://レインボースライム
				Enemy10Pattern();
				break;
			default:
				break;
			}
		}
	}
	//===========================================================

	//==================================
	//Xオブジェクト更新
	//==================================
	CObjectX::Update();
	//===========================================================
	
	//==================================
	//敵の位置調整処理
	//==================================
	AdjustEnemyPos();
	//===========================================================


	//==================================
	//当たり判定
	//==================================
	Collision();
	//===========================================================

	if (nLife < 1)
	{
		m_bUse = false;
	}

    //==================================
	//敵の破棄
	//==================================
	if (m_bUse == false)
	{
		SetDeath();
	}
	//===========================================================
}
//==================================================================================================================

//==============================
//描画処理
//==============================
void CEnemy::Draw()
{
	CObjectX::Draw();

	if (CScene::GetMode() == CScene::MODE_EDIT)
	{
		if (m_pMoveRangeAI != nullptr)//ステージマネージャーに選ばれていたら
		{
			if (GetStageManagerChoose() == true)
			{
				m_pMoveRangeAI->SetUseDraw(true);
			}
			else
			{
				m_pMoveRangeAI->SetUseDraw(false);
			}
		}
	}
}
//==================================================================================================================

//==============================
//ダメージを与える処理
//==============================
void CEnemy::SetDamage(int nDamage, int nHitStopTime)
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
		CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_DAMAGE_000);
		CDamage::Create(nDamage, GetPos(), NORMAL_COL, 40.0f, 40.0f);
	}
}
//==================================================================================================================

//================================================================
//死亡フラグを設定
//================================================================
void CEnemy::SetDeath()
{
	if (m_pMoveRangeAI != nullptr)
	{
		m_pMoveRangeAI->SetUseDeath(true);
		m_pMoveRangeAI->SetDeath();
		m_pMoveRangeAI = nullptr;
	}

	CObject::SetDeath();
}
//==================================================================================================================

//===============================
//敵の生成処理
//===============================
CEnemy* CEnemy::Create(ENEMYTYPE type, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 Scale,
	D3DXVECTOR3 MoveLemgthAIPos, D3DXVECTOR3 MoveLengthAIRot, D3DXVECTOR3 MoveLengthAIScale)
{
	CEnemy* pEnemy = DBG_NEW CEnemy;   //敵を生成
	bool bSuccess = pEnemy->CObject::GetCreateSuccess();
	int nIdx = 0;//モデルのインデックス
	if (bSuccess == true)
	{
		if (pEnemy != nullptr)
		{
			pEnemy->Init();         //初期化処理
			pEnemy->SetUseDeath(true);                                                   //死亡フラグを発動するかどうかを設定する
			pEnemy->m_bUse = true;  //使用状態
			pEnemy->m_type = type;  //敵の種類
			pEnemy->SetLife(nLife); //敵の体力
			pEnemy->SetMaxLife(nLife);//敵の最大体力
			pEnemy->SetAutoSubLife(false);//自動的に体力を減らすかどうか
			pEnemy->CObjectX::SetPos(pos);                                                                                  //オブジェクト２Ｄの位置を設定
			pEnemy->SetPosOld(pos);               //1f前の位置を設定
			pEnemy->SetUseGravity(true);
			pEnemy->SetSupportPos(pos);           //設置位置
			pEnemy->CObject::SetType(CObject::TYPE_ENEMY);                                                                  //オブジェクトの種類を決める
			pEnemy->CObjectX::SetScale(Scale);                                                                              //オブジェクトの拡大率
			pEnemy->SetFormarScale(Scale);                                                                                  //元の拡大率を設定する
			nIdx = CManager::GetObjectXInfo()->Regist(CObjectX::OBJECTXTYPE_ENEMY, m_ENEMY_FILENAME[(int)(type)]);
			pEnemy->CObjectX::BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(CObjectX::OBJECTXTYPE_ENEMY,nIdx),
				CManager::GetObjectXInfo()->GetBuffMat(CObjectX::OBJECTXTYPE_ENEMY,nIdx),
				CManager::GetObjectXInfo()->GetdwNumMat(CObjectX::OBJECTXTYPE_ENEMY,nIdx),
				CManager::GetObjectXInfo()->GetTexture(CObjectX::OBJECTXTYPE_ENEMY,nIdx),
				CManager::GetObjectXInfo()->GetColorValue(CObjectX::OBJECTXTYPE_ENEMY,nIdx));                       //モデル情報を割り当てる

			pEnemy->CObjectX::SetObjXType(CObjectX::OBJECTXTYPE_ENEMY);                            //オブジェクトXのタイプを設定
			pEnemy->CObjectX::SetTypeNum((int)(type));                                             //オブジェクトXごとのタイプ番号を設定
			pEnemy->SetSize();                                                                     //Xオブジェクトのサイズを設定する
			pEnemy->CObjectX::SetRotType(CObjectX::ROTTYPE::ROTTYPE_NORMAL);                       //向きの種類を設定
			pEnemy->SetManagerType((int)(CStageManager::MANAGEROBJECT_ENEMY));        //マネージャーで呼び出す時の種類を設定

			//===========================================
			//AIモデルの生成
			//===========================================
			pEnemy->m_pMoveRangeAI = CAIModel::Create(CAIModel::AIMODELTYPE_SQUARE, MoveLemgthAIPos, MoveLengthAIRot, MoveLengthAIScale);
			//======================================================================================================================================
		}
		else
		{//オブジェクトに空きがなかったので破棄する
			delete pEnemy;
			pEnemy = nullptr;
			return nullptr;
		}
	}
	return pEnemy;
}
//===================================================================================================================================

//=======================================================
//敵０のパターン
//=======================================================
void CEnemy::Enemy00Pattern()
{
	D3DXVECTOR3& Pos = GetPos();                                  //モデルの位置の取得
	D3DXVECTOR3& PosOld = GetPosOld();                            //1f前の位置を設定
	D3DXVECTOR3& Move = GetMove();                                //移動量を取得
	D3DXVECTOR3& Rot = GetRot();                                   //モデルの向きの取得
	D3DXVECTOR3 Size = GetSize();                                 //サイズを取得
	m_fTotalAim = float(rand() % 628) / 100;//３６０度のベクトルの乱数
	int nAIPattern = 0;                                           //AIの役割をする乱数
	int nPattern = 0;                                             //パターン

	//==========================================
	//行動開始
	//==========================================
	if (m_bAction == true)
	{
		if (m_bPattern == false)
		{
			if (m_fLength >= m_fENEMY00_NEARACTIONPATTERNLENGTH)
			{//距離が遠ければ
				nAIPattern = 0;
				nPattern = rand() % 5;

				if (nPattern == 0)
				{//低確率で攻撃
					m_nPattern = 1;
				}
				else if (nPattern >= 1 && nPattern <= 3)
				{//高確率で接近
					m_nPattern = 0;
				}
				else
				{//低確率でジャンプ
					m_nPattern = 2;
				}
			}
			else
			{//距離が近ければ
				nAIPattern = 1;
				nPattern = rand() % 6;

				if (nPattern >= 0 && nPattern <= 2)
				{//高確率で攻撃
					m_nPattern = 1;
				}
				else if (nPattern >= 3 && nPattern <= 4)
				{//中確率でジャンプ
					m_nPattern = 2;
				}
				else
				{//低確率で接近
					m_nPattern = 0;
				}
			}
			m_bPattern = true;
		}

		if (m_bPattern == true)
		{
			m_nPatternTime++;
			switch (m_nPattern)
			{
			case 0:
				//接近
				if (Pos.x <= CGame::GetPlayer()->GetPos().x)
				{
					Move.x = 0.5f;
				}
				else if (Pos.x >= CGame::GetPlayer()->GetPos().x - 20.0f && Pos.x <= CGame::GetPlayer()->GetPos().x + 20.0f)
				{
					Move.x = 0.0f;
				}
				else
				{
					Move.x = -0.5f;
				}

				if (m_nPatternTime >= 70)
				{
					//行動パターンのリセット
					ResetPattern();
				}
				break;
			case 1:
				//攻撃
				Move.x = 0.0f;//攻撃中は動かない

				//if (m_nPatternTime == 1)
				//{
				//	CParticle::SummonParticle(CParticle::TYPE00_NORMAL, 50, 20, 40.0f, 40.0f, 200, 10, false, Pos, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), true);
				//}

				//if (m_nPatternTime % 25 == 0 && m_nPatternTime <= 50)
				//{//バレット発射
				//	CBullet::Create(CBullet::BULLETTYPE_ENEMY, CBullet::BULLETTEX_ENEMY, 300, 6, 20.0f, 20.0f, Pos,
				//		D3DXVECTOR3(sinf(m_fTotalAim) * 3.0f, cosf(m_fTotalAim) * 3.0f, 0.0f));
				//}

				if (m_nPatternTime >= 80)
				{
					//行動パターンのリセット
					ResetPattern();
				}
				break;
			case 2:
				//ジャンプ
				if (m_nPatternTime == 1)
				{
					SetUseInteria(false);
					if (Pos.x <= CGame::GetPlayer()->GetPos().x)
					{//プレイヤーが右側にいる
						Move.x = 1.0f;
						Move.y = 4.0f;
					}
					else
					{//プレイヤーが左側にいる
						Move.x = -1.0f;
						Move.y = 4.0f;
					}
				}

				if (m_nPatternTime >= 45)
				{
					SetUseInteria(true);
					//行動パターンのリセット
					ResetPattern();
				}
				break;
			default:
				break;
			}
		}
	}
	//===========================================================================

}
//===================================================================================================================================

//====================================================================
//敵との当たり判定
//====================================================================
void CEnemy::Collision()
{

}
//===================================================================================================================================

//====================================================================
//プレイヤーとの距離を計算する
//====================================================================
void CEnemy::CalculationLength()
{
	D3DXVECTOR3 Pos = GetPos();//位置を取得

	//距離計算開始
	m_fLength = sqrtf(powf(CGame::GetPlayer()->GetPos().x - Pos.x, 2) + powf(CGame::GetPlayer()->GetPos().y - Pos.y, 2) + 
		powf(CGame::GetPlayer()->GetPos().z - Pos.z, 2));
}
//===================================================================================================================================

//======================================================================
//重力の処理
//======================================================================
void CEnemy::GravityProcess()
{
	D3DXVECTOR3& Pos = GetPos();                                  //モデルの位置の取得
	D3DXVECTOR3& PosOld = GetPosOld();                            //1f前の位置を設定
	D3DXVECTOR3& Move = GetMove();                                //移動量を取得
	bool& bIsLanding = GetLanding();                              //地面にいるかどうかを取得
	bool& bUseGravity = GetUseGravity();                          //重力を使用するかどうかを取得
	if (CScene::GetMode() == CScene::MODE_GAME)
	{
		if (bIsLanding == true)
		{
			Move.y = 0.0f;
		}
		if (bUseGravity == true)
		{
			Move.y += -m_fGravityPower;
		}
	}
}
//===================================================================================================================================

//=================================================================
//敵１のパターン
//=================================================================
void CEnemy::Enemy01Pattern()
{
	D3DXVECTOR3& Pos = GetPos();                                  //モデルの位置の取得
	D3DXVECTOR3& PosOld = GetPosOld();                            //1f前の位置を設定
	D3DXVECTOR3& Move = GetMove();                                //移動量を取得
	D3DXVECTOR3& Rot = GetRot();                                   //モデルの向きの取得
	D3DXVECTOR3 Size = GetSize();                                 //サイズを取得
	m_fTotalAim = float(rand() % 628) / 100;//３６０度のベクトルの乱数
	int nAIPattern = 0;                                           //AIの役割をする乱数
	int nPattern = 0;                                             //パターン
	float fAddRot = float(rand() % 100 - 50) / 100;               //加算する向き
	CBullet* pBullet = nullptr;                                   //バレットへのポインタ
	//==========================================
	//行動開始
	//==========================================
	if (m_bAction == true)
	{
		if (m_bPattern == false)
		{
			//=============================
			//AI
			//=============================
			if (m_fLength >= 180.0f)
			{//距離が遠ければ
				nAIPattern = 0;
				nPattern = rand() % 5;

				if (nPattern >= 0 && nPattern <= 1)
				{//中確率で急接近
					m_nPattern = 0;
				}
				else if (nPattern >= 2 && nPattern <= 3)
				{//中確率で円形各散弾
					m_nPattern = 1;
				}
				else
				{//低確率でファイアブレス
					m_nPattern = 2;
				}
			}
			else
			{//距離が近ければ
				nAIPattern = 1;
				nPattern = rand() % 5;

				if (nPattern == 0)
				{//低確率で急接近
					m_nPattern = 0;
				}
				else if (nPattern == 1)
				{//低確率で円形各散弾
					m_nPattern = 1;
				}
				else
				{//高確率でファイアブレス
					m_nPattern = 2;
				}
			}
			//================================================================================
			m_bPattern = true;
		}

		if (m_bPattern == true)
		{
			m_nPatternTime++;
			switch (m_nPattern)
			{
			case 0:
				//急接近
				if (Pos.x <= CGame::GetPlayer()->GetPos().x)
				{
					Move.x = m_fENEMY01_NORMALSPEED;
				}
				else if (Pos.x >= CGame::GetPlayer()->GetPos().x - 20.0f && Pos.x <= CGame::GetPlayer()->GetPos().x + 20.0f)
				{
					Move.x = 0.0f;
				}
				else
				{
					Move.x = -m_fENEMY01_NORMALSPEED;
				}

				if (m_nPatternTime >= m_nENEMY01_ACTION0_FRAME)
				{
					//行動パターンのリセット
					ResetPattern();
				}
				break;
			case 1:
				//円形拡散弾
				Move.x = 0.0f;
				if (m_nPatternTime == 1)
				{
					CParticle::SummonParticle(CParticle::TYPE00_NORMAL, 50, 20, 40.0f, 40.0f, 200, 10, false, Pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), true);
					for (int nCnt = 0; nCnt < 4; nCnt++)
					{
						float fAngle = (2.0f / 4.0f) * (float)(nCnt);//撃つ方向を分割する乱数
						CUi3D::Create(CUi3D::UI3DTYPE01_WARNING, 60, 60.0f, 60.0f, GetSenterPos(), D3DXVECTOR3(sinf(D3DX_PI * fAngle) * 20.0f, cosf(D3DX_PI * fAngle) * 20.0f, 0.0f));
					}
				}

					if (m_nPatternTime == 50)
					{
						for (int nCnt = 0; nCnt < 4; nCnt++)
						{
							float fAngle = (2.0f / 4.0f) * (float)(nCnt);//撃つ方向を分割する乱数
							CBullet::Create(CBullet::BULLETTYPE_ENEMY, CBullet::BULLETTEX_ENEMY, 60, 8, 20.0f, 20.0f, Pos,
								D3DXVECTOR3(sinf(D3DX_PI * fAngle) * 5.0f, cosf(D3DX_PI * fAngle) * 5.0f, 0.0f));
						}
					}

					if (m_nPatternTime >= 90)
					{
						//行動パターンのリセット
						ResetPattern();
					}
					break;
			case 2:
				//ファイアブレス
				if (m_nPatternTime == 1)
				{
					CParticle::SummonParticle(CParticle::TYPE00_NORMAL, 50, 20, 40.0f, 40.0f, 200, 10, false, Pos, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), true);
				}

				//少しだけ動く
				if (Pos.x <= CGame::GetPlayer()->GetPos().x)
				{//プレイヤーが右にいれば
					m_nPattern = 3;
				}
				else
				{//プレイヤーが左にいれば
					m_nPattern = 4;
				}
				break;
			case 3:
				Move.x = 1.0f;
				if (m_nPatternTime == 5)
				{
					CUi3D::Create(CUi3D::UI3DTYPE01_WARNING, 60, 120.0f, 120.0f, GetSenterPos() + D3DXVECTOR3(sinf(D3DX_PI * 0.5f) * 60.0f, cosf(D3DX_PI * 0.5f) * 60.0f, 0.0f), NULL_VECTOR3);
				}
				if (m_nPatternTime >= 40 && m_nPatternTime <= 90)
				{
					pBullet = CBullet::Create(CBullet::BULLETTYPE_ENEMY, CBullet::BULLETTEX_ENEMY, 30, 8, 20.0f, 20.0f, Pos,
						D3DXVECTOR3(sinf(D3DX_PI * 0.5f + fAddRot) * 5.0f, cosf(D3DX_PI * 0.5f + fAddRot) * 5.0f, 0.0f));
					//弾に拡大量を付与
					pBullet->SetScaling(0.02f);
				}

				if (m_nPatternTime >= 120)
				{
					//行動パターンのリセット
					ResetPattern();
				}

				break;
			case 4:
				Move.x = -1.0f;
				if (m_nPatternTime == 5)
				{
					CUi3D::Create(CUi3D::UI3DTYPE01_WARNING, 60, 120.0f, 120.0f, GetSenterPos() + D3DXVECTOR3(sinf(D3DX_PI * 0.5f) * -60.0f, cosf(D3DX_PI * 0.5f) * -60.0f, 0.0f), NULL_VECTOR3);
				}
				if (m_nPatternTime >= 40 && m_nPatternTime <= 90)
				{
					pBullet = CBullet::Create(CBullet::BULLETTYPE_ENEMY, CBullet::BULLETTEX_ENEMY, 30, 8, 20.0f, 20.0f, Pos,
						D3DXVECTOR3(sinf(D3DX_PI * 0.5f + fAddRot) * -5.0f, cosf(D3DX_PI * 0.5f + fAddRot) * -5.0f, 0.0f));
					//弾に拡大量を付与
					pBullet->SetScaling(0.02f);
				}
				if (m_nPatternTime >= 120)
				{
					//行動パターンのリセット
					ResetPattern();
				}
				break;
			default:
				break;
				}
			}

	}
	//===========================================================================
}
//===================================================================================================================================

//===================================================================
//敵２のパターン
//===================================================================
void CEnemy::Enemy02Pattern()
{
	D3DXVECTOR3& Pos = GetPos();                                  //モデルの位置の取得
	D3DXVECTOR3& PosOld = GetPosOld();                            //1f前の位置を設定
	D3DXVECTOR3& Move = GetMove();                                //移動量を取得
	D3DXVECTOR3& Rot = GetRot();                                   //モデルの向きの取得
	D3DXVECTOR3 Size = GetSize();                                 //サイズを取得
	m_fTotalAim = float(rand() % 628) / 100;                      //３６０度のベクトルの乱数
	int nPattern = 0;                                             //パターン
	float fAddSpeed = float(rand() % 40 + 1) / 10;                //速さの補正
	float fAddRot = float(rand() % 100 - 50) / 100;               //加算する向き
	CBullet* pBullet = nullptr;                                   //バレットへのポインタ

	float fVXaim = 0.0f;//X方向ベクトル
	float fVYaim = 0.0f;//Y方向ベクトル
	float fVLaim = 0.0f;//総合ベクトル
	bool& bIsLanding = GetLanding();                              //地面にいるかどうかを取得

	int nRand = 0;      //乱数

	CAttackEnemy* pAttackEnemy = nullptr;//敵の攻撃取得用

	float fAddAim = 0.0f;

	fVXaim = CGame::GetPlayer()->GetPos().x - Pos.x;
	fVYaim = (CGame::GetPlayer()->GetPos().y + CGame::GetPlayer()->GetSize().y * 0.5f) - Pos.y;//プレイヤーの中心を狙う
	fVLaim = (float)(atan2(fVXaim, fVYaim));


	//==========================================
	//行動開始
	//==========================================
	if (m_bAction == true)
	{
		if (m_bPattern == false)
		{
			//=============================
			//AI
			//=============================
			if (m_fLength >= 220.0f)
			{//距離が遠ければ
				nPattern = rand() % 5;

				if (nPattern >= 0 && nPattern <= 1)
				{//中確率で反復横跳び
					m_nPattern = 0;
				}
				else if (nPattern == 2)
				{//低確率で自機狙い弾
					m_nPattern = 1;
				}
				else
				{//中確率でサーベルカッター
					m_nPattern = 2;
				}
			}
			else
			{//距離が近ければ
				nPattern = rand() % 6;

				if (nPattern >= 0 && nPattern <= 1)
				{//中確率で反復横跳び
					m_nPattern = 0;
				}
				else if (nPattern >= 2 && nPattern <= 4)
				{//高確率で自機狙い
					m_nPattern = 1;
				}
				else
				{//低確率でサーベルカッター
					m_nPattern = 2;
				}
			}
			//================================================================================
			m_bPattern = true;
		}

		if (m_bPattern == true)
		{
			m_nPatternTime++;
			switch (m_nPattern)
			{
			case 0://反復横跳び
				if (m_nPatternTime == 1)
				{
					CUi3D::Create(CUi3D::UI3DTYPE01_WARNING, 50, 100.0f, 100.0f, Pos, NULL_VECTOR3);
				}
				if (m_nPatternTime <= 100 && m_nPatternTime >= 40)
				{
					switch (m_nSubPattern)
					{
					case 0:
						if (bIsLanding == true)
						{
							SetUseInteria(false);//慣性を使用しない
							Move.x = 2.0f + fAddSpeed;
							if (Pos.x <= CGame::GetPlayer()->GetPos().x)
							{//プレイヤーが右側にいたら
								Move.x += 2.0f;
							}
							else
							{//プレイヤーが左側にいたら
								Move.x -= 2.0f;
							}
							Move.y = m_fENEMY02_JUMPPOWER;
							m_nSubPattern++;
						}
						break;
					case 1:
						if (bIsLanding == true)
						{
							SetUseInteria(false);//慣性を使用しない
							Move.x = -2.0f - fAddSpeed;
							if (Pos.x <= CGame::GetPlayer()->GetPos().x)
							{//プレイヤーが右側にいたら
								Move.x += 2.0f;
							}
							else
							{//プレイヤーが左側にいたら
								Move.x -= 2.0f;
							}
							Move.y = m_fENEMY02_JUMPPOWER;
							m_nSubPattern = 0;
						}
						break;
					default:
						break;
					}
				}
				else
				{//20f動きを止める
					Move.x = 0.0f;
				}

				if (m_nPatternTime >= 120)
				{
					SetUseInteria(true);//慣性を使用する
					//行動パターンのリセット
					ResetPattern();
				}
				break;
			case 1://自機狙い

				if (m_nPatternTime == 1)
				{
					m_nSubPattern = rand() % 2;
					CParticle::SummonParticle(CParticle::TYPE00_NORMAL, 50, 20, 40.0f, 40.0f, 200, 10, false, Pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), true);
				}

				switch (m_nSubPattern)
				{
				case 0://右からスタート
					if (m_nPatternTime == 20)
					{
						SetInertia(0.1f);
						Move.x = m_fENEMY02_ACTION1_SPEED + fAddSpeed;
					}
					else if (m_nPatternTime == 50)
					{
						Move.x = -m_fENEMY02_ACTION1_SPEED - fAddSpeed;
					}
					else if (m_nPatternTime == 80)
					{
						Move.x = m_fENEMY02_ACTION1_SPEED / 2;
					}
					else if (m_nPatternTime == 110)
					{
						Move.y = m_fENEMY02_ACTION1_SPEED * 2;
					}
					break;
				case 1://左からスタート
					if (m_nPatternTime == 20)
					{
						SetInertia(0.1f);
						Move.x = -m_fENEMY02_ACTION1_SPEED + fAddSpeed;
					}
					else if (m_nPatternTime == 50)
					{
						Move.x = m_fENEMY02_ACTION1_SPEED - fAddSpeed;
					}
					else if (m_nPatternTime == 80)
					{
						Move.x = -m_fENEMY02_ACTION1_SPEED / 2;
					}
					else if (m_nPatternTime == 110)
					{
						Move.y = m_fENEMY02_ACTION1_SPEED * 2;
					}
					break;
				default:
					break;
				}
				if (m_nPatternTime % 30 == 0 && m_nPatternTime <= 120)
				{
					nRand = rand() % 2;

					if (nRand == 0 && m_nCntAction < 2)
					{//最大２回撃つ
						m_nCntAction++;
						pBullet = CBullet::Create(CBullet::BULLETTYPE_ENEMY, CBullet::BULLETTEX_ENEMY, 300, 10, 20.0f, 20.0f, Pos,
							D3DXVECTOR3(sinf(fVLaim) * 5.0f, cosf(fVLaim) * 5.0f, 0.0f));

						pBullet->SetUseEffect(30, D3DXVECTOR2(40.0f, 40.0f), D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f));//エフェクトを使用する
					}
				}

				if (m_nPatternTime >= 150)
				{//パターンリセット
					ResetPattern();
				}

				break;
			case 2://サーベルカッター
				if (m_nPatternTime == 15)
				{
					CParticle::SummonParticle(CParticle::TYPE00_NORMAL, 50, 20, 40.0f, 40.0f, 200, 10, false, Pos, D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), true);
					CUi3D::Create(CUi3D::UI3DTYPE01_WARNING, 60, 70.0f, 70.0f,CGame::GetPlayer()->GetPos(), NULL_VECTOR3);
				}

				if (m_nPatternTime == 81)
				{
					nRand = rand() % 2;

					if (nRand == 0)
					{//確率でジャンプ
						Move.y = 20.0f;
					}
				}

				if (m_nPatternTime == 95)
				{
					pAttackEnemy = CAttackEnemy::Create(CAttack::ATTACKTYPE08_VACUUMBLADE, 10, 120, Pos, D3DXVECTOR3(sinf(fVLaim) * 1.0f, cosf(fVLaim) * 1.0f, 0.0f), ONE_VECTOR3 * 2.0f,
						D3DXVECTOR3(0.0f, 0.0f,-fVLaim + D3DX_PI), CAttack::COLLISIONTYPE02_TRUERECTANGLEXY,NULL_VECTOR3, true, 20);

					pAttackEnemy->SetUseMultiSpeed(true,ONE_VECTOR3 * 1.05f);//加速度設定
				}

				if (m_nPatternTime == 100)
				{
					ResetPattern();
				}
				break;
			default:
				break;
			}
		}
	}
	//===========================================================================

}
//===================================================================================================================================

//===================================================================
//行動パターンのリセット
//===================================================================
void CEnemy::ResetPattern()
{
	m_bPattern = false;
	m_nPattern = 0;
	m_nPatternTime = 0;
	m_nSubPattern = 0;
	m_nCntAction = 0;
}
//===================================================================================================================================

//===================================================================
//敵３のパターン
//===================================================================
void CEnemy::Enemy03Pattern()
{
	D3DXVECTOR3& Pos = GetPos();                                  //モデルの位置の取得
	D3DXVECTOR3& PosOld = GetPosOld();                            //1f前の位置を設定
	D3DXVECTOR3& Move = GetMove();                                //移動量を取得
	D3DXVECTOR3& Rot = GetRot();                                   //モデルの向きの取得
	D3DXVECTOR3 Size = GetSize();                                 //サイズを取得
	m_fTotalAim = float(rand() % 628) / 100;                      //３６０度のベクトルの乱数
	int nPattern = 0;                                             //パターン
	float fAddSpeed = float(rand() % 40 + 1) / 10;                //速さの補正
	float fRandRot = float(rand() % 628 + 1) / 100;               //乱数向き
	CBullet* pBullet = nullptr;                                   //バレットへのポインタ
	CBullet* pParentBullet = nullptr;
	float fVXaim = 0.0f;//X方向ベクトル
	float fVYaim = 0.0f;//Y方向ベクトル
	float fVLaim = 0.0f;//総合ベクトル
	D3DXMATRIX mtxWorld = GetMatrixWorld();
	CAttackEnemy* pAttackEnemy = nullptr;                         //敵の攻撃取得用
	float fAddRot = 0.0f;                                         //加算向き
	float fAddAim = 0.0f;

	fAddRot = float(rand() % 25) / 100;

	//fVXaim = CGame::GetPlayer()->GetPos().x - Pos.x;
	//fVYaim = (CGame::GetPlayer()->GetPos().y + CGame::GetPlayer()->GetSize().y * 0.5f) - Pos.y;//プレイヤーの中心を狙う
	//fVLaim = (float)(atan2(fVXaim, fVYaim));


	//==========================================
	//行動開始
	//==========================================
	if (m_bAction == true)
	{
		if (m_bPattern == false)
		{
			//=============================
			//AI
			//=============================
			if (m_fLength >= 220.0f)
			{//距離が遠ければ
				nPattern = rand() % 7;

				if (nPattern >= 0 && nPattern <= 1)
				{//中確率で接近できる
					m_nPattern = 0;
				}
				else if (nPattern == 2)
				{//低確率で後ずさり
					m_nPattern = 1;
				}
				else if (nPattern == 3)
				{
					m_nPattern = 2;
				}
				else
				{//高確率でブロススロウ
					m_nPattern = 3;
				}
			}
			else
			{//距離が近ければ
				nPattern = rand() % 9;

				if (nPattern == 0)
				{//低確率で接近
					m_nPattern = 0;
				}
				else if (nPattern >= 1 && nPattern <= 3)
				{//高確率で後ずさり
					m_nPattern = 1;
				}
				else if (nPattern >= 4 && nPattern <= 6)
				{//高確率でバレットスラッシュ
					m_nPattern = 2;
				}
				else
				{//中確率でブロススロウ
					m_nPattern = 3;
				}
			}
			//================================================================================
			m_bPattern = true;
		}

		if (m_bPattern == true)
		{
			m_nPatternTime++;
			switch (m_nPattern)
			{
			case 0://接近
				SetUseInteria(false);//慣性を使用しない
				ApproachProcess(0.5f);
				if (m_nPatternTime >= 100)
				{
					SetUseInteria(true);//慣性を使用する
					//行動パターンのリセット
					ResetPattern();
				}
				break;
			case 1://後ずさり
				SetUseInteria(false);//慣性を使用しない
				ApproachProcess(-3.0f);
				if (m_nPatternTime >= 35)
				{
					SetUseInteria(true);//慣性を使用する

					//行動パターンのリセット
					ResetPattern();
				}
				break;
			case 2://バレットスラッシュ（マトリックスバレット）
				if (m_nPatternTime == 1)
				{
					CParticle::SummonParticle(CParticle::TYPE00_NORMAL, 50, 20, 40.0f, 40.0f, 200, 10, false, Pos, D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), true);

					for (int nCnt = 0; nCnt < 3; nCnt++)
					{
						pBullet = CBullet::Create(CBullet::BULLETTYPE_ENEMY, CBullet::BULLETTEX_ENEMY, 115, 3, 10.0f, 10.0f,NULL_VECTOR3,
							D3DXVECTOR3(sinf(0.0f) * (0.6f * (float)(nCnt + 1)), cosf(0.0f) * (0.6f * (float)(nCnt + 1)), 0.0f));
						pBullet->SetUseMtxChild(&GetMatrixWorld(),true);//バレットを子マトリックスに設定
						pBullet->SetUseEffect(30, D3DXVECTOR2(40.0f, 40.0f), D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
					}
				}

				if (m_nPatternTime >= 50)
				{
					if (Pos.x <= CGame::GetPlayer()->GetPos().x)
					{//プレイヤーが右側にいたら
						m_nSubPattern = 1;
					}
					else
					{//プレイヤーが左側にいたら
						m_nSubPattern = 2;
					}
				}

				switch (m_nSubPattern)
				{
				case 1:
					Rot.z += -0.03f;
					break;
				case 2:
					Rot.z += 0.03f;
					break;
				default:
					break;
				}

				if (m_nPatternTime >= 115)
				{
					Rot.z = 0.0f;
					SetUseInteria(true);//慣性を使用する

					//行動パターンのリセット
					ResetPattern();
				}
				break;
			case 3:
				//ブロススロウ
				if (m_nPatternTime == 15)
				{
					CParticle::SummonParticle(CParticle::TYPE00_NORMAL, 50, 20, 40.0f, 40.0f, 200, 10, false, Pos, D3DXCOLOR(1.0f, 0.5f, 0.5f, 1.0f), true);

					if (Pos.x <= CGame::GetPlayer()->GetPos().x)
					{//プレイヤーが右にいれば
						m_nSubPattern = 0;
					}
					else
					{//プレイヤーが左にいれば
						m_nSubPattern = 1;
					}
				}

				if (m_nPatternTime == 65)
				{//一回だけハンマーを投げる
					switch (m_nSubPattern)
					{
					case 0:
						pAttackEnemy = CAttackEnemy::Create(CAttack::ATTACKTYPE09_HAMMER, 22, 160, Pos, D3DXVECTOR3(sinf(D3DX_PI * (0.25f - fAddRot)) * 10.0f, cosf(D3DX_PI * (0.25f - fAddRot)) * 10.0f, 0.0f), ONE_VECTOR3 * 3.0f, NULL_VECTOR3,
							CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, D3DXVECTOR3(0.0f, 0.0f, -0.4f), true, 10);
							pAttackEnemy->SetUseGravity(-0.2f);//重力を設定
							break;
					case 1:
						pAttackEnemy = CAttackEnemy::Create(CAttack::ATTACKTYPE09_HAMMER, 22, 160, Pos, D3DXVECTOR3(sinf(D3DX_PI * (-0.25f + fAddRot)) * 10.0f, cosf(D3DX_PI * (-0.25f + fAddRot)) * 10.0f, 0.0f), ONE_VECTOR3 * 3.0f, NULL_VECTOR3,
							CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, D3DXVECTOR3(0.0f, 0.0f, 0.4f), true, 10);
						pAttackEnemy->SetUseGravity(-0.2f);//重力を設定
						break;
					default:
						break;
					}
				}

				if (m_nPatternTime == 115)
				{
					SetUseInteria(true);//慣性を使用する
					//行動パターンのリセット
					ResetPattern();
				}

				break;
			default:
				break;
			}
		}
	}
	//===========================================================================

}
//===================================================================================================================================

//====================================================================
//接近する処理
//====================================================================
void CEnemy::ApproachProcess(float fSpeed)
{
	D3DXVECTOR3& Pos = GetPos();                                  //モデルの位置の取得
	D3DXVECTOR3& Move = GetMove();                                //移動量を取得

	if (Pos.x <= CGame::GetPlayer()->GetPos().x)
	{//プレイヤーが右側にいたら
		Move.x = fSpeed;
	}
	else
	{//プレイヤーが左側にいたら
		Move.x = -fSpeed;
	}

	if (Pos.x >= CGame::GetPlayer()->GetPos().x - (GetSize().x / 2.0f) && Pos.x <= CGame::GetPlayer()->GetPos().x + (GetSize().x / 2.0f))
	{
		Move.x = 0.0f;
	}
}
//===================================================================================================================================

//====================================================================
//敵の位置を調整する処理
//====================================================================
void CEnemy::AdjustEnemyPos()
{
	if (m_pMoveRangeAI != nullptr)
	{//移動範囲用のモデルが存在したら・・・
		D3DXVECTOR3 &Pos = GetPos();
		D3DXVECTOR3& Move = GetMove();
		D3DXVECTOR3 VtxMax = GetVtxMax();
		D3DXVECTOR3 VtxMin = GetVtxMin();

		D3DXVECTOR3 AIPos = m_pMoveRangeAI->GetPos();
		D3DXVECTOR3 AIVtxMax = m_pMoveRangeAI->GetVtxMax();
		D3DXVECTOR3 AIVtxMin = m_pMoveRangeAI->GetVtxMin();


		if (AIPos.x + AIVtxMax.x < Pos.x + VtxMax.x)
		{//移動範囲より右に出たら
			Pos.x = AIPos.x + AIVtxMax.x - VtxMax.x;
			Move.x = 0.0f;
		}
		if (AIPos.x + AIVtxMin.x > Pos.x + VtxMin.x)
		{//移動範囲より左に出たら
			Pos.x = AIPos.x + AIVtxMin.x - VtxMin.x;
			Move.x = 0.0f;
		}
	}
}
//===================================================================================================================================

//========================================================================
//情報表示処理
//========================================================================
void CEnemy::DispInfo()
{
	D3DXVECTOR3 Size = GetSize();
	D3DXVECTOR3 Scale = GetScale();
	int nLife = GetLife();

	ChengeLifeProcess();//体力を変更する

	if (m_pMoveRangeAI != nullptr)
	{
		ChengeMoveLengthAIScale();//移動範囲の拡大率を変更する

		ChengeMoveLengthAIPos();//移動範囲の位置を変更する

	}

	CObjectX::DispInfo();
}
//===================================================================================================================================

//========================================================================
//体力を変える処理
//========================================================================
void CEnemy::ChengeLifeProcess()
{
	int& nLife = GetLife();

	if (CManager::GetInputKeyboard()->GetPress(DIK_LCONTROL) == true)
	{//Lコントロールキーを押しながら
		if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
		{//シフトキーを押しながら・・・
			if (CManager::GetInputKeyboard()->GetPress(DIK_1) == true)
			{
				nLife -= 5;
			}
		}
		else if (CManager::GetInputKeyboard()->GetPress(DIK_1) == true)
		{
			nLife += 5;
		}
	}
	else
	{//Lコントロールキーを押していない
		if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
		{//シフトキーを押しながら・・・
			if (CManager::GetInputKeyboard()->GetTrigger(DIK_1) == true)
			{
				nLife -= 5;
			}
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_1) == true)
		{
			nLife += 5;
		}
	}

	CManager::GetDebugText()->PrintDebugText("体力（1) : %d\n",nLife);
}
//===================================================================================================================================

//========================================================================
//移動範囲AIモデルの範囲を変える処理
//========================================================================
void CEnemy::ChengeMoveLengthAIScale()
{
	D3DXVECTOR3& Scale = m_pMoveRangeAI->GetScale();

	ChengeMoveLengthAIScaleX();
	ChengeMoveLengthAIScaleY();
	ChengeMoveLengthAIScaleZ();

	CManager::GetDebugText()->PrintDebugText("移動範囲の拡大率（2,3,4) : %f %f %f\n", Scale.x,Scale.y,Scale.z);
}
//===================================================================================================================================

//========================================================================
//移動範囲AIモデルのXの範囲を変える処理
//========================================================================
void CEnemy::ChengeMoveLengthAIScaleX()
{
	D3DXVECTOR3& Scale = m_pMoveRangeAI->GetScale();
	if (CManager::GetInputKeyboard()->GetPress(DIK_LCONTROL) == true)
	{//Lコントロールキーを押しながら
		if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
		{//シフトキーを押しながら・・・
			if (CManager::GetInputKeyboard()->GetPress(DIK_2) == true)
			{
				Scale.x -= 0.1f;
			}
		}
		else if (CManager::GetInputKeyboard()->GetPress(DIK_2) == true)
		{
			Scale.x += 0.1f;
		}
	}
	else
	{//Lコントロールキーを押していない
		if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
		{//シフトキーを押しながら・・・
			if (CManager::GetInputKeyboard()->GetTrigger(DIK_2) == true)
			{
				Scale.x -= 0.1f;
			}
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_2) == true)
		{
			Scale.x += 0.1f;
		}
	}
}
//===================================================================================================================================

//========================================================================
//移動範囲AIモデルのYの範囲を変える処理
//========================================================================
void CEnemy::ChengeMoveLengthAIScaleY()
{
	D3DXVECTOR3& Scale = m_pMoveRangeAI->GetScale();

	if (CManager::GetInputKeyboard()->GetPress(DIK_LCONTROL) == true)
	{//Lコントロールキーを押しながら
		if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
		{//シフトキーを押しながら・・・
			if (CManager::GetInputKeyboard()->GetPress(DIK_3) == true)
			{
				Scale.y -= 0.1f;
			}
		}
		else if (CManager::GetInputKeyboard()->GetPress(DIK_3) == true)
		{
			Scale.y += 0.1f;
		}
	}
	else
	{//Lコントロールキーを押していない
		if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
		{//シフトキーを押しながら・・・
			if (CManager::GetInputKeyboard()->GetTrigger(DIK_3) == true)
			{
				Scale.y -= 0.1f;
			}
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_3) == true)
		{
			Scale.y += 0.1f;
		}
	}
}
//===================================================================================================================================

//========================================================================
//移動範囲AIモデルのYの範囲を変える処理
//========================================================================
void CEnemy::ChengeMoveLengthAIScaleZ()
{
	D3DXVECTOR3& Scale = m_pMoveRangeAI->GetScale();

	if (CManager::GetInputKeyboard()->GetPress(DIK_LCONTROL) == true)
	{//Lコントロールキーを押しながら
		if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
		{//シフトキーを押しながら・・・
			if (CManager::GetInputKeyboard()->GetPress(DIK_4) == true)
			{
				Scale.z -= 0.1f;
			}
		}
		else if (CManager::GetInputKeyboard()->GetPress(DIK_4) == true)
		{
			Scale.z += 0.1f;
		}
	}
	else
	{//Lコントロールキーを押していない
		if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
		{//シフトキーを押しながら・・・
			if (CManager::GetInputKeyboard()->GetTrigger(DIK_4) == true)
			{
				Scale.z -= 0.1f;
			}
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_4) == true)
		{
			Scale.z += 0.1f;
		}
	}

}
//===================================================================================================================================

//========================================================================
//移動範囲AIモデルの位置を変える処理
//========================================================================
void CEnemy::ChengeMoveLengthAIPos()
{
	D3DXVECTOR3& AIPos = m_pMoveRangeAI->GetPos();//移動範囲AIの位置

	CCalculation::CaluclationMove(AIPos, 5.0f);

	CManager::GetDebugText()->PrintDebugText("移動範囲の位置（WASD) : %f %f %f\n", AIPos.x, AIPos.y, AIPos.z);
}
//===================================================================================================================================

//========================================================================
//敵４のパターン
//========================================================================
void CEnemy::Enemy04Pattern()
{
	D3DXVECTOR3& Pos = GetPos();                                  //モデルの位置の取得
	D3DXVECTOR3& PosOld = GetPosOld();                            //1f前の位置を設定
	D3DXVECTOR3& Move = GetMove();                                //移動量を取得
	D3DXVECTOR3& Rot = GetRot();                                   //モデルの向きの取得
	D3DXVECTOR3 Size = GetSize();                                 //サイズを取得
	int nPattern = 0;                                             //パターン
	float fAddSpeed = float(rand() % 40 + 1) / 10;                //速さの補正
	float fRandRot = float(rand() % 628 + 1) / 100;               //乱数向き
	CBullet* pBullet = nullptr;                                   //バレットへのポインタ
	float fVXaim = 0.0f;                                          //X方向ベクトル
	float fVYaim = 0.0f;                                          //Y方向ベクトル
	float fVLaim = 0.0f;                                          //総合ベクトル
	D3DXMATRIX mtxWorld = GetMatrixWorld();                       //マトリックスワールド
	CAttackEnemy* pAttackEnemy = nullptr;                         //敵の攻撃取得用
	float fAddRot = 0.0f;                                         //加算向き
	bool& bIsLanding = GetLanding();                              //地面にいるかどうかを取得

	fAddRot = float(rand() % 25) / 100;

	fVXaim = CGame::GetPlayer()->GetPos().x - Pos.x;
	fVYaim = (CGame::GetPlayer()->GetPos().y + CGame::GetPlayer()->GetSize().y * 0.5f) - Pos.y;//プレイヤーの中心を狙う
	fVLaim = (float)(atan2(fVXaim, fVYaim));


	//==========================================
	//行動開始
	//==========================================
	if (m_bAction == true)
	{
		if (m_bPattern == false)
		{
			//=============================
			//AI
			//=============================
			if (m_fLength >= 250.0f)
			{//距離が遠ければ
				nPattern = rand() % 5;

				if (nPattern >= 0 && nPattern <= 1)
				{//中確率でのそのそ接近
					m_nPattern = 0;
				}
				else if (nPattern >= 2 && nPattern <= 3)
				{//中確率で衝撃波
					m_nPattern = 1;
				}
				else
				{//低確率でビッグスリーウェイ
					m_nPattern = 2;
				}
			}
			else
			{//距離が近ければ
				nPattern = rand() % 5;

				if (nPattern == 0)
				{//低確率で接近
					m_nPattern = 0;
				}
				else if (nPattern == 1)
				{//低確率で衝撃波
					m_nPattern = 1;
				}
				else
				{//高確率でビッグスリーウェイ
					m_nPattern = 2;
				}
			}
			//================================================================================
			m_bPattern = true;
		}

		if (m_bPattern == true)
		{
			m_nPatternTime++;
			switch (m_nPattern)
			{
			case 0://のそのそ接近
				if (m_nPatternTime % 30 == 0)
				{
					SetInertia(0.1f);
					SetUseInteria(true);//慣性を使用する
					ApproachProcess(7.0f);
				}
				if (m_nPatternTime >= 100)
				{
					SetInertia(0.5f);
					//行動パターンのリセット
					ResetPattern();
				}
				break;
			case 1://衝撃波

				if (m_nPatternTime % 10 == 0 && m_nPatternTime <= 30)
				{//チャージ
					CParticle::SummonChargeParticle(CParticle::TYPE00_NORMAL, 50, 100, 1.0f, 40.0f, 40.0f, 200, 20, 1, true, Pos, NORMAL_COL, true);
				}

				if (m_nPatternTime == 45)
				{
					Move.y = 20.0f;
					CUi3D::Create(CUi3D::UI3DTYPE01_WARNING, 60, 70.0f, 70.0f,Pos,D3DXVECTOR3(20.0f,0.0f,0.0f));
					CUi3D::Create(CUi3D::UI3DTYPE01_WARNING, 60, 70.0f, 70.0f,Pos,D3DXVECTOR3(-20.0f,0.0f,0.0f));
				}

				if (m_nPatternTime >= 50)
				{
					if (bIsLanding == true)
					{
						pAttackEnemy = CAttackEnemy::Create(CAttack::ATTACKTYPE10_SHOCKWAVE, 25, 300, Pos, D3DXVECTOR3(sinf(D3DX_PI * 0.5f) * 5.0f, cosf(D3DX_PI * 0.5f) * 5.0f, 0.0f),
							ONE_VECTOR3 * 3, NULL_VECTOR3, CAttack::COLLISIONTYPE02_TRUERECTANGLEXY,
							NULL_VECTOR3, false, 11);

						pAttackEnemy->SetUseMultiSpeed(true, ONE_VECTOR3 * (1.05f + 0.004f * (float)(m_nSubPattern)));

						pAttackEnemy = CAttackEnemy::Create(CAttack::ATTACKTYPE10_SHOCKWAVE, 25, 300, Pos, D3DXVECTOR3(sinf(D3DX_PI * 0.5f) * -5.0f, cosf(D3DX_PI * 0.5f) * -5.0f, 0.0f),
							ONE_VECTOR3 * 3, D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), CAttack::COLLISIONTYPE02_TRUERECTANGLEXY,
							NULL_VECTOR3, false, 11);

						pAttackEnemy->SetUseMultiSpeed(true, ONE_VECTOR3 * (1.05f + 0.004f * (float)(m_nSubPattern)));
						ResetPattern();
					}
				}


				break;
			case 2://ビッグスリーウェイ
				if (m_nPatternTime == 30)
				{
					CParticle::SummonParticle(CParticle::TYPE00_NORMAL, 50, 20, 40.0f, 40.0f, 200, 10, false, Pos, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), true);
				}

				if (m_nPatternTime == 50)
				{
					for (int nCnt = 0; nCnt < 3; nCnt++)
					{
						CBullet::Create(CBullet::BULLETTYPE_ENEMY, CBullet::BULLETTEX_ENEMY, 100, 7, 60.0f, 60.0f, Pos, D3DXVECTOR3(sinf(fVLaim - m_fENEMY04_ACTION2THREEWAYAIM + m_fENEMY04_ACTION2THREEWAYAIM * (float)(nCnt)) * 5.0f,
							cosf(fVLaim - m_fENEMY04_ACTION2THREEWAYAIM + m_fENEMY04_ACTION2THREEWAYAIM * (float)(nCnt)) * 5.0f, 0.0f));
					}
				}

				if (m_nPatternTime == 90)
				{
					ResetPattern();
				}
				break;
			default:
				break;
			}
		}
	}
	//===========================================================================

}
//===================================================================================================================================

//=======================================================================
//敵５のパターン
//=======================================================================
void CEnemy::Enemy05Pattern()
{
	D3DXVECTOR3& Pos = GetPos();                                  //モデルの位置の取得
	D3DXVECTOR3& PosOld = GetPosOld();                            //1f前の位置を設定
	D3DXVECTOR3& Move = GetMove();                                //移動量を取得
	D3DXVECTOR3& Rot = GetRot();                                   //モデルの向きの取得
	D3DXVECTOR3 Size = GetSize();                                 //サイズを取得
	int nPattern = 0;                                             //パターン
	float fAddSpeed = float(rand() % 40 + 1) / 10;                //速さの補正
	float fRandRot = float(rand() % 628 + 1) / 100;               //乱数向き
	float fVXaim = 0.0f;                                          //X方向ベクトル
	float fVYaim = 0.0f;                                          //Y方向ベクトル
	float fVLaim = 0.0f;                                          //総合ベクトル
	bool& bUseGravity = GetUseGravity();                          //重力を使用するかどうか
	D3DXMATRIX mtxWorld = GetMatrixWorld();                       //マトリックスワールド
	CBullet* pBullet = nullptr;                                   //バレットへのポインタ
	CAttackEnemy* pAttackEnemy = nullptr;                         //敵の攻撃取得用
	float fAddRot = 0.0f;                                         //加算向き
	int nRand = 0;//乱数
	fAddRot = float(rand() % 25) / 100;

	fVXaim = CGame::GetPlayer()->GetPos().x - Pos.x;
	fVYaim = (CGame::GetPlayer()->GetPos().y + CGame::GetPlayer()->GetSize().y * 0.5f) - Pos.y;//プレイヤーの中心を狙う
	fVLaim = (float)(atan2(fVXaim, fVYaim));


	//==========================================
	//行動開始
	//==========================================
	if (m_bAction == true)
	{
		if (m_bPattern == false)
		{
			//=============================
			//AI
			//=============================
			if (m_fLength >= 260.0f)
			{//距離が遠ければ
				nPattern = rand() % 5;

				if (nPattern == 0)
				{//低確率でサンダーステップ
					m_nPattern = 0;
				}
				else if (nPattern >= 1 && nPattern <= 3)
				{//中確率でカーブ弾
					m_nPattern = 1;
				}
				else
				{//低確率でフォールンスパーク
					m_nPattern = 2;
				}
			}
			else
			{//距離が近ければ
				nPattern = rand() % 6;

				if (nPattern >= 0 && nPattern <= 1)
				{//中確率でサンダーステップ
					m_nPattern = 0;
				}
				else if (nPattern == 2)
				{//低確率でカーブ弾
					m_nPattern = 1;
				}
				else
				{//高確率でフォールンスパーク
					m_nPattern = 2;
				}
			}
			//================================================================================
			m_bPattern = true;
		}

		if (m_bPattern == true)
		{
			m_nPatternTime++;
			switch (m_nPattern)
			{
			case 0://サンダーステップ
				if (m_nPatternTime >= 30 && m_nPatternTime <= 85)
				{
					bUseGravity = false;//重力を使わない
					SetUseInteria(false);//慣性を使わない
					if (m_nPatternTime == 30)
					{
						CUi3D::Create(CUi3D::UI3DTYPE01_WARNING, 50, 200.0f, 200.0f, Pos, NULL_VECTOR3);
						CParticle::SummonParticle(CParticle::TYPE00_NORMAL, 50, 20, 40.0f, 40.0f, 200, 10, false, Pos, D3DXCOLOR(0.5f, 0.5f, 1.0f, 1.0f), true);
					}

					if (m_nPatternTime >= 75)
					{
						CEffect::Create(CEffect::EFFECTTYPE01_THUNDER, 100, 25.0f, 25.0f, Pos + D3DXVECTOR3(0.0f, GetSize().y * 0.5f, 0.0f), NORMAL_COL);
					}

					if (m_nPatternTime == 75)
					{
						m_fTotalAim = float(rand() % 314 - 157) / 100;                      //３６０度のベクトルの乱数
						Move.x = sinf(m_fTotalAim) * 20.0f;
						Move.y = cosf(m_fTotalAim) * 20.0f;
					}
					if (m_nPatternTime == 85)
					{
						m_fTotalAim = float(rand() % 314 - 157) / 100;                      //３６０度のベクトルの乱数
						Move.x = sinf(m_fTotalAim + D3DX_PI) * 20.0f;
						Move.y = cosf(m_fTotalAim + D3DX_PI) * 20.0f;
					}

					if (m_nPatternTime == 95)
					{
						Move.x = 0.0f;
					}
				}

				if (m_nPatternTime == 125)
				{
					bUseGravity = true;//重力を使わない
					SetUseInteria(true);//慣性を使わない
					m_fGravityPower = GetNormalGravity();
					ResetPattern();
				}
				break;
			case 1://カーブ弾
				if (m_nPatternTime == 30)
				{
					CParticle::SummonParticle(CParticle::TYPE00_NORMAL, 50, 20, 40.0f, 40.0f, 200, 10, false, Pos, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), true);
				}

				if (m_nPatternTime == 95)
				{
					int nRand = rand() % 2;
					for (int nCnt = 0; nCnt < m_nMAX_ENEMY05_BULLETNUM; nCnt++)
					{
						float fRot = ((D3DX_PI * 2.0f) / m_nMAX_ENEMY05_BULLETNUM) * nCnt;
						pBullet = CBullet::Create(CBullet::BULLETTYPE_ENEMY, CBullet::BULLETTEX_ENEMY, 150, 15, 30.0f, 30.0f, Pos, NULL_VECTOR3);
						if (nRand == 0)
						{
							pBullet->SetUseCurve(fRot, 5.0f, 0.02f);
							pBullet->SetUseEffect(30, D3DXVECTOR2(30.0f, 30.0f), D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f));
						}
						else
						{
							pBullet->SetUseCurve(fRot, 5.0f, -0.02f);
							pBullet->SetUseEffect(30, D3DXVECTOR2(30.0f, 30.0f), D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f));
						}
					}
				}

				if (m_nPatternTime == 155)
				{
					ResetPattern();
				}
				break;
			case 2://フォールンスパーク
				if (m_nPatternTime == 30)
				{
					CParticle::SummonParticle(CParticle::TYPE00_NORMAL, 50, 20, 40.0f, 40.0f, 200, 10, false, Pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), true);
					CUi3D::Create(CUi3D::UI3DTYPE01_WARNING, 50, 80.0f, 80.0f, Pos + D3DXVECTOR3(100.0f, 0.0f, 0.0f), NULL_VECTOR3);
					CUi3D::Create(CUi3D::UI3DTYPE01_WARNING, 50, 80.0f, 80.0f, Pos + D3DXVECTOR3(-100.0f, 0.0f, 0.0f), NULL_VECTOR3);
				}

				if (m_nPatternTime == 95)
				{
					pAttackEnemy = CAttackEnemy::Create(CAttack::ATTACKTYPE06_THUNDER, 10, 165, Pos + D3DXVECTOR3(100.0f, 600.0f, 0.0f),
						D3DXVECTOR3(sinf(D3DX_PI) * 15.0f, cosf(D3DX_PI) * 15.0f, 0.0f), D3DXVECTOR3(1.0f, 2.0f, 1.0f), NULL_VECTOR3, CAttack::COLLISIONTYPE02_TRUERECTANGLEXY,
						D3DXVECTOR3(0.0f,0.3f,0.0f), true, 25);

					pAttackEnemy->SetUseAddScale(D3DXVECTOR3(0.0f, 0.1f, 0.0f),true);

					pAttackEnemy = CAttackEnemy::Create(CAttack::ATTACKTYPE06_THUNDER, 10, 165, Pos + D3DXVECTOR3(-100.0f, 600.0f, 0.0f),
						D3DXVECTOR3(sinf(D3DX_PI) * 15.0f, cosf(D3DX_PI) * 15.0f, 0.0f), D3DXVECTOR3(1.0f, 2.0f, 1.0f), NULL_VECTOR3, CAttack::COLLISIONTYPE02_TRUERECTANGLEXY,
						D3DXVECTOR3(0.0f, 0.3f, 0.0f), true, 25);

					pAttackEnemy->SetUseAddScale(D3DXVECTOR3(0.0f, 0.1f, 0.0f),true);
				}

				if (m_nPatternTime == 145)
				{
					ResetPattern();
				}
				break;
			default:
				break;
			}
		}
	}
	//===========================================================================
}
//===================================================================================================================================

//============================================================================
//敵６のパターン
//============================================================================
void CEnemy::Enemy06Pattern()
{
	D3DXVECTOR3& Pos = GetPos();                                  //モデルの位置の取得
	D3DXVECTOR3& PosOld = GetPosOld();                            //1f前の位置を設定
	D3DXVECTOR3& Move = GetMove();                                //移動量を取得
	D3DXVECTOR3& Rot = GetRot();                                   //モデルの向きの取得
	D3DXVECTOR3 Size = GetSize();                                 //サイズを取得
	int nPattern = 0;                                             //パターン
	float fAddSpeed = float(rand() % 50 + 1) / 10;                //速さの補正
	float fRandRot = float(rand() % 628 + 1) / 100;               //乱数向き
	float fVXaim = 0.0f;                                          //X方向ベクトル
	float fVYaim = 0.0f;                                          //Y方向ベクトル
	float fVLaim = 0.0f;                                          //総合ベクトル
	D3DXMATRIX mtxWorld = GetMatrixWorld();                       //マトリックスワールド
	CBullet* pBullet = nullptr;                                   //バレットへのポインタ
	CAttackEnemy* pAttackEnemy = nullptr;                         //敵の攻撃取得用
	float fAddRot = 0.0f;                                         //加算向き
	bool& bIsLanding = GetLanding();                              //地面にいるかどうかを取得
	bool& bUseGravity = GetUseGravity();                          //重力を使用するかどうか

	fVXaim = CGame::GetPlayer()->GetPos().x - Pos.x;
	fVYaim = (CGame::GetPlayer()->GetPos().y + CGame::GetPlayer()->GetSize().y * 0.5f) - Pos.y;//プレイヤーの中心を狙う
	fVLaim = (float)(atan2(fVXaim, fVYaim));


	//==========================================
	//行動開始
	//==========================================
	if (m_bAction == true)
	{
		if (m_bPattern == false)
		{
			//=============================
			//AI
			//=============================
			if (m_fLength >= 200.0f)
			{//距離が遠ければ
				nPattern = rand() % 8;
				if (nPattern == 0)
				{//低確率でクルリンステップ
					m_nPattern = 0;
				}
				else if (nPattern == 1)
				{//低確率で接近
					m_nPattern = 1;
				}
				else if (nPattern >= 2 && nPattern <= 4)
				{//高確率でショットガン
					m_nPattern = 2;
				}
				else
				{//高確率でホーミング弾
					m_nPattern = 3;
				}
			}
			else
			{//距離が近ければ
				nPattern = rand() % 8;
				if (nPattern >= 0 && m_nPattern <= 3)
				{//超確率でクルリンステップ
					m_nPattern = 0;
				}
				else if (nPattern == 4)
				{//低確率で接近
					m_nPattern = 1;
				}
				else if (nPattern == 5)
				{//低確率でショットガン
					m_nPattern = 2;
				}
				else
				{//中確率でホーミング弾
					m_nPattern = 3;
				}
			}
			//================================================================================
			m_bPattern = true;
		}

		if (m_bPattern == true)
		{
			m_nPatternTime++;
			switch (m_nPattern)
			{
			case 0://クルリンステップ
				if (m_nPatternTime == 45)
				{
					CParticle::SummonParticle(CParticle::TYPE00_NORMAL, 50, 20, 40.0f, 40.0f, 200, 10, false, Pos, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), true);
				}

				if (m_nPatternTime >= 50)
				{
					SetUseInteria(false);
					if (bIsLanding == true)
					{
						m_nCntAction++;
						if (m_nCntAction < 3)
						{
							if (Pos.x <= CGame::GetPlayer()->GetPos().x)
							{//プレイヤーが右側にいたら
								Move.x = -5.0f;
								m_nSubPattern = 0;
							}
							else
							{//プレイヤーが左側にいたら
								Move.x = 5.0f;
								m_nSubPattern = 1;
							}

							Move.y = 15.0f;
						}
					}

					switch (m_nSubPattern)
					{
					case 0:
						Rot.z += 0.4f;
						break;
					case 1:
						Rot.z -= 0.4f;
						break;
					default:
						break;
					}
				}

				if (m_nCntAction == 3)
				{
					Rot.z = 0.0f;
					SetUseInteria(true);
					ResetPattern();//パターンをリセット
				}
				break;
			case 1://接近
				ApproachProcess(2.0f);//接近

				if (m_nPatternTime == 80)
				{
					ResetPattern();//パターンをリセット
				}
				break;
			case 2://ショットガン
				if (m_nPatternTime == 15)
				{
					CParticle::SummonChargeParticle(CParticle::TYPE00_NORMAL, 50, 150,1.0f, 40.0f, 40.0f, 200, 200,20, true, Pos,D3DXCOLOR(1.0f,0.0f,1.0f,1.0f),true);

					for (int nCnt = 0; nCnt < 7; nCnt++)
					{
						fAddRot = float(rand() % 80 - 40) / 100;
						fAddSpeed = float(rand() % 50 + 1) / 10;
						CUi3D::Create(CUi3D::UI3DTYPE01_WARNING, 50, 50.0f, 50.0f, GetSenterPos(), D3DXVECTOR3(sinf(fVLaim + fAddRot)* (3.0f + fAddSpeed), cosf(fVLaim + fAddRot)* (3.0f + fAddSpeed), 0.0f));
					}
				}

				if (m_nPatternTime == 120)
				{
					for (int nCnt = 0; nCnt < 7; nCnt++)
					{
						fAddRot = float(rand() % 80 - 40) / 100;
						fAddSpeed = float(rand() % 50 + 1) / 10;
						CBullet::Create(CBullet::BULLETTYPE_ENEMY, CBullet::BULLETTEX_ENEMY, 200, 4, 20.0f, 20.0f, Pos,
							D3DXVECTOR3(sinf(fVLaim + fAddRot) * (10.0f + fAddSpeed), cosf(fVLaim + fAddRot) * (10.0f + fAddSpeed),0.0f));
					}
				}

				if (m_nPatternTime == 160)
				{
					ResetPattern();
				}

				break;
			case 3://ホーミング弾
				if (m_nPatternTime == 15)
				{
					CParticle::SummonParticle(CParticle::TYPE00_NORMAL, 50, 20, 40.0f, 40.0f, 200, 10, false, Pos, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), true);
				}

				if (m_nPatternTime == 90)
				{
					pBullet = CBullet::Create(CBullet::BULLETTYPE_ENEMY, CBullet::BULLETTEX_ENEMY, 300, 5, 30.0f, 30.0f, Pos, NULL_VECTOR3);
					pBullet->SetUseHorming(5.0f);//ホーミングを使用する
					pBullet->SetUseEffect(60, D3DXVECTOR2(30.0f, 30.0f), D3DXCOLOR(0.0f, 1.0f, 0.5f, 1.0f));//エフェクトを使用する
				}

				if (m_nPatternTime == 130)
				{
					ResetPattern();
				}
				break;
			default:
				break;
			}
		}
	}
	//===========================================================================

}
//===================================================================================================================================

//===============================================================
//敵７のパターン
//===============================================================
void CEnemy::Enemy07Pattern()
{
	D3DXVECTOR3& Pos = GetPos();                                  //モデルの位置の取得
	D3DXVECTOR3& PosOld = GetPosOld();                            //1f前の位置を設定
	D3DXVECTOR3& Move = GetMove();                                //移動量を取得
	D3DXVECTOR3& Rot = GetRot();                                  //モデルの向きの取得
	D3DXVECTOR3 Size = GetSize();                                 //サイズを取得
	int nPattern = 0;                                             //パターン
	float fAddSpeed = float(rand() % 50 + 1) / 10;                //速さの補正
	float fRandRot = float(rand() % 628 + 1) / 100;               //乱数向き
	float fVXaim = 0.0f;                                          //X方向ベクトル
	float fVYaim = 0.0f;                                          //Y方向ベクトル
	float fVLaim = 0.0f;                                          //総合ベクトル
	D3DXMATRIX mtxWorld = GetMatrixWorld();                       //マトリックスワールド
	CBullet* pBullet = nullptr;                                   //バレットへのポインタ
	CAttackEnemy* pAttackEnemy = nullptr;                         //敵の攻撃取得用
	float fAddRot = 0.0f;                                         //加算向き
	bool& bUseGravity = GetUseGravity();                          //重力を使用するかどうか

	fVXaim = CGame::GetPlayer()->GetPos().x - Pos.x;
	fVYaim = (CGame::GetPlayer()->GetPos().y + CGame::GetPlayer()->GetSize().y * 0.5f) - Pos.y;//プレイヤーの中心を狙う
	fVLaim = (float)(atan2(fVXaim, fVYaim));


	//==========================================
	//行動開始
	//==========================================
	if (m_bAction == true)
	{
		if (m_bPattern == false)
		{
			//=============================
			//AI
			//=============================
			if (m_fLength >= 200.0f)
			{//距離が遠ければ
				nPattern = rand() % 8;
				if (nPattern >= 0 && nPattern <= 2)
				{
					m_nPattern = 0;
				}
				else if (nPattern >= 3 && nPattern <= 4)
				{
					m_nPattern = 1;
				}
				else if (nPattern == 5)
				{
					m_nPattern = 2;
				}
				else
				{
					m_nPattern = 3;
				}
			}
			else
			{//距離が近ければ
				nPattern = rand() % 8;
				if (nPattern >= 0 && nPattern <= 1)
				{
					m_nPattern = 0;
				}
				else if (nPattern >= 2 && nPattern <= 4)
				{
					m_nPattern = 1;
				}
				else if (nPattern >= 5 && nPattern <= 6)
				{
					m_nPattern = 2;
				}
				else
				{
					m_nPattern = 3;
				}
			}
			//================================================================================
			m_bPattern = true;
		}

		if (m_bPattern == true)
		{
			m_nPatternTime++;
			switch (m_nPattern)
			{
			case 0://超加速
				if (m_nPatternTime == 15)
				{
					CParticle::SummonParticle(CParticle::TYPE00_NORMAL, 50, 20, 40.0f, 40.0f, 200, 10, false, Pos, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), true);
				}

				if (m_nPatternTime == 30)
				{
					SetInertia(0.05f);
					ApproachProcess(10.0f);
				}

				if (m_nPatternTime == 80)
				{
					SetInertia(0.5f);
					ResetPattern();
				}
				break;
			case 1://ツインニードル
				if (m_nPatternTime == 20)
				{
					CParticle::SummonParticle(CParticle::TYPE00_NORMAL, 50, 20, 40.0f, 40.0f, 200, 10, false, Pos, D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), true);
					CUi3D::Create(CUi3D::UI3DTYPE01_WARNING, 60, 100.0f, 100.0f, GetSenterPos() + D3DXVECTOR3(120.0f,0.0f,0.0f),NULL_VECTOR3);
					CUi3D::Create(CUi3D::UI3DTYPE01_WARNING, 60, 100.0f, 100.0f, GetSenterPos() + D3DXVECTOR3(-120.0f,0.0f,0.0f),NULL_VECTOR3);
				}

				if (m_nPatternTime == 50)
				{
					pAttackEnemy = CAttackEnemy::Create(CAttack::ATTACKTYPE02_NEEDLE, 10, 50, Pos, NULL_VECTOR3, D3DXVECTOR3(1.0f,0.0f,1.0f), D3DXVECTOR3(0.0f, 0.0f, D3DX_PI * 0.5f),
						CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, NULL_VECTOR3, true, 10);
					pAttackEnemy->SetUseAddScale(D3DXVECTOR3(0.0f, 0.05f, 0.0f),true);

					pAttackEnemy = CAttackEnemy::Create(CAttack::ATTACKTYPE02_NEEDLE, 10, 50, Pos, NULL_VECTOR3, D3DXVECTOR3(1.0f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, D3DX_PI * -0.5f),
						CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, NULL_VECTOR3, true, 10);
					pAttackEnemy->SetUseAddScale(D3DXVECTOR3(0.0f, 0.05f, 0.0f),true);
				}

				if (m_nPatternTime == 80)
				{
					ResetPattern();
				}
				break;
			case 2://アップニードル
				if (m_nPatternTime == 20)
				{
					CParticle::SummonParticle(CParticle::TYPE00_NORMAL, 50, 20, 40.0f, 40.0f, 200, 10, false, Pos, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), true);
					CUi3D::Create(CUi3D::UI3DTYPE01_WARNING, 60, 100.0f, 100.0f, GetSenterPos() + D3DXVECTOR3(0.0f, 100.0f, 0.0f), NULL_VECTOR3);

				}

				if (m_nPatternTime == 50)
				{
					pAttackEnemy = CAttackEnemy::Create(CAttack::ATTACKTYPE02_NEEDLE, 10, 50, Pos, NULL_VECTOR3, D3DXVECTOR3(1.5f, 0.0f, 1.5f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
						CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, NULL_VECTOR3, true, 10);
					pAttackEnemy->SetUseAddScale(D3DXVECTOR3(0.0f, 0.07f, 0.0f),true);

				}

				if (m_nPatternTime == 90)
				{
					ResetPattern();
				}
				break;
			case 3://振りかぶりショット
				ApproachProcess(1.5f);
				if (m_nPatternTime == 20)
				{
					CParticle::SummonParticle(CParticle::TYPE00_NORMAL, 50, 20, 40.0f, 40.0f, 200, 10, false, Pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), true);
				}
				if (m_nPatternTime == 60)
				{
					if (Pos.x <= CGame::GetPlayer()->GetPos().x)
					{//プレイヤーが右側にいたら
						m_nSubPattern = 1;
					}
					else
					{//プレイヤーが左側にいたら
						m_nSubPattern = 2;
					}
				}

				switch (m_nSubPattern)
				{
				case 1:
					m_fShotRot += 0.05f;
					break;
				case 2:
					m_fShotRot -= 0.05f;
					break;
				default:
					break;
				}

				if (m_nPatternTime >= 60)
				{
					if (m_nPatternTime % 10 == 0)
					{
						pBullet = CBullet::Create(CBullet::BULLETTYPE_ENEMY, CBullet::BULLETTEX_ENEMY, 100, 2, 20.0f, 20.0f, Pos, D3DXVECTOR3(sinf(m_fShotRot) * 5.0f, cosf(m_fShotRot) * 5.0f, 0.0f));
						pBullet->SetUseEffect(30, D3DXVECTOR2(40.0f, 40.0f), D3DXCOLOR(1.0f, 1.0f, 0.0f,1.0f));
					}
				}

				if (m_nPatternTime >= 120)
				{
					m_fShotRot = 0.0f;
					ResetPattern();//パターンのリセット
				}
				break;
			default:
				break;
			}
		}
	}
	//===========================================================================
}
//===================================================================================================================================

//========================================================================
//敵８のパターン
//========================================================================
void CEnemy::Enemy08Pattern()
{
	D3DXVECTOR3& Pos = GetPos();                                  //モデルの位置の取得
	D3DXVECTOR3& PosOld = GetPosOld();                            //1f前の位置を設定
	D3DXVECTOR3& Move = GetMove();                                //移動量を取得
	D3DXVECTOR3& Rot = GetRot();                                  //モデルの向きの取得
	D3DXVECTOR3 Size = GetSize();                                 //サイズを取得
	int nPattern = 0;                                             //パターン
	float fVXaim = 0.0f;                                          //X方向ベクトル
	float fVYaim = 0.0f;                                          //Y方向ベクトル
	float fVLaim = 0.0f;                                          //総合ベクトル
	D3DXMATRIX mtxWorld = GetMatrixWorld();                       //マトリックスワールド
	CBullet* pBullet = nullptr;                                   //バレットへのポインタ
	CAttackEnemy* pAttackEnemy = nullptr;                         //敵の攻撃取得用
	float fAddRot = float(rand() % 314 + 1) / 100;                                         //加算向き
	bool& bUseGravity = GetUseGravity();                          //重力を使用するかどうか

	fVXaim = CGame::GetPlayer()->GetPos().x - Pos.x;
	fVYaim = (CGame::GetPlayer()->GetPos().y + CGame::GetPlayer()->GetSize().y * 0.5f) - Pos.y;//プレイヤーの中心を狙う
	fVLaim = (float)(atan2(fVXaim, fVYaim));

	SetUseGravity(false);
	//==============================
    //移動方法（上下運動）
    //==============================
	bUseGravity = false;
	if (m_bSpeedSwitch == false)
	{
		Move.y += 0.1f;
	}
	else
	{
		Move.y -= 0.1f;
	}

	if (Move.y >= 4.0f)
	{
		m_bSpeedSwitch = true;
	}

	if (Move.y <= -4.0f)
	{
		m_bSpeedSwitch = false;
	}
	//====================================================================

	//==========================================
	//行動開始
	//==========================================
	if (m_bAction == true)
	{

		if (m_bPattern == false)
		{
			m_nPattern = rand() % 2;
			m_bPattern = true;
		}

		if (m_bPattern == true)
		{
			m_nPatternTime++;
			switch (m_nPattern)
			{
			case 0:
				if (m_nPatternTime == 1)
				{
					CParticle::SummonParticle(CParticle::TYPE00_NORMAL, 50, 20, 40.0f, 40.0f, 200, 10, false, Pos, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), true);
				}
				if (m_nPatternTime % 60 == 0)
				{
					for (int nCnt = 0; nCnt < 2; nCnt++)
					{
						float fSpeed = 2.0f;
						CBullet::Create(CBullet::BULLETTYPE_ENEMY, CBullet::BULLETTEX_ENEMY, 120, 15, 30.0f, 30.0f, Pos,
							D3DXVECTOR3(sinf(fVLaim) * (fSpeed * (1.0f + (float)(nCnt))), cosf(fVLaim) * (fSpeed * (1.0f + (float)(nCnt))),0.0f));
					}
				}
				if (m_nPatternTime == 125)
				{
					ResetPattern();
				}
				break;
			case 1:
				if (m_nPatternTime == 15)
				{
					CParticle::SummonParticle(CParticle::TYPE00_NORMAL, 50, 20, 40.0f, 40.0f, 200, 10, false, Pos, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), true);
				}

				if (m_nPatternTime % 40 == 0)
				{
					for (int nCnt = 0; nCnt < 8; nCnt++)
					{
						float fRot = ((D3DX_PI * 2.0f) / 8.0f) * (float)(nCnt);
						pBullet = CBullet::Create(CBullet::BULLETTYPE_ENEMY, CBullet::BULLETTEX_ENEMY, 180, 15, 30.0f, 30.0f, Pos,
							D3DXVECTOR3(sinf(fRot + fAddRot) * 0.5f, cosf(fRot + fAddRot) * 0.5f, 0.0f));
						pBullet->SetUseAddSpeed(true, true, 1.03f);
						pBullet->SetUseEffect(10, D3DXVECTOR2(30.0f, 30.0f), D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
					}
				}

				if (m_nPatternTime == 125)
				{
					ResetPattern();
				}
				break;
			default:
				break;
			}
		}
	}
	//===========================================================================

}
//===================================================================================================================================

//======================================================================
//敵９のパターン
//======================================================================
void CEnemy::Enemy09Pattern()
{
	D3DXVECTOR3& Pos = GetPos();                                  //モデルの位置の取得
	D3DXVECTOR3& PosOld = GetPosOld();                            //1f前の位置を設定
	D3DXVECTOR3& Move = GetMove();                                //移動量を取得
	D3DXVECTOR3& Rot = GetRot();                                  //モデルの向きの取得
	D3DXVECTOR3 Size = GetSize();                                 //サイズを取得
	int nPattern = 0;                                             //パターン
	float fAddSpeed = float(rand() % 50 + 1) / 10;                //速さの補正
	float fRandRot = float(rand() % 628 + 1) / 100;               //乱数向き
	float fVXaim = 0.0f;                                          //X方向ベクトル
	float fVYaim = 0.0f;                                          //Y方向ベクトル
	float fVLaim = 0.0f;                                          //総合ベクトル
	D3DXMATRIX mtxWorld = GetMatrixWorld();                       //マトリックスワールド
	CBullet* pBullet = nullptr;                                   //バレットへのポインタ
	CAttackEnemy* pAttackEnemy = nullptr;                         //敵の攻撃取得用
	float fAddRot = 0.0f;                                         //加算向き
	D3DXVECTOR3 SummonPos = NULL_VECTOR3;                         //自分自身を召喚する位置
	fVXaim = CGame::GetPlayer()->GetPos().x - Pos.x;
	fVYaim = (CGame::GetPlayer()->GetPos().y + CGame::GetPlayer()->GetSize().y * 0.5f) - Pos.y;//プレイヤーの中心を狙う
	fVLaim = (float)(atan2(fVXaim, fVYaim));
	bool& bUseGravity = GetUseGravity();                          //重力を使用するかどうか

	fVXaim = CGame::GetPlayer()->GetPos().x - Pos.x;
	fVYaim = (CGame::GetPlayer()->GetPos().y + CGame::GetPlayer()->GetSize().y * 0.5f) - Pos.y;//プレイヤーの中心を狙う
	fVLaim = (float)(atan2(fVXaim, fVYaim));


	//==========================================
	//行動開始
	//==========================================
	if (m_bAction == true)
	{
		if (m_bPattern == false)
		{
			//=============================
			//AI
			//=============================
			if (m_fLength >= 300.0f)
			{//距離が遠ければ
				nPattern = rand() % 5;
				if (nPattern >= 0 && nPattern <= 1)
				{
					m_nPattern = 0;
				}
				else if (nPattern >= 2 && nPattern <= 3)
				{
					m_nPattern = 1;
				}
				else
				{
					m_nPattern = 2;
				}
			}
			else
			{//距離が近ければ
				nPattern = rand() % 5;
				if (nPattern == 0)
				{
					m_nPattern = 0;
				}
				else if (nPattern == 1)
				{
					m_nPattern = 1;
				}
				else
				{
					m_nPattern = 2;
				}
			}
			//================================================================================
			m_bPattern = true;
		}

		if (m_bPattern == true)
		{
			m_nPatternTime++;
			switch (m_nPattern)
			{
			case 0://接近
				if (m_nPatternTime == 15)
				{
					CParticle::SummonParticle(CParticle::TYPE00_NORMAL, 50, 20, 40.0f, 40.0f, 200, 10, false, Pos, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), true);
				}

				if (m_nPatternTime == 30)
				{
					SetUseInteria(false);
					ApproachProcess(3.0f);
				}

				if (m_nPatternTime == 80)
				{
					SetUseInteria(true);
					ResetPattern();
				}
				break;
			case 1://テレポートクラッシュ
				switch (m_nSubPattern)
				{
				case 0:
					if (m_nPatternTime == 35)
					{
						CParticle::SummonChargeParticle(CParticle::TYPE00_NORMAL, 50, 150,1.0f, 40.0f, 40.0f, 100, 20, 1, true, Pos, NORMAL_COL, true);
						SetOriginalColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), 50);
						CUi3D::Create(CUi3D::UI3DTYPE01_WARNING, 50, 100.0f, 100.0f,CGame::GetPlayer()->GetSenterPos(), NULL_VECTOR3);
					}

					if (m_nPatternTime == 85)
					{
						SummonPos.x = float(rand() % 400 - 200);
						SummonPos.y = float(rand() % 400 - 200);

						Pos = CGame::GetPlayer()->GetPos() + SummonPos;
						CParticle::SummonParticle(CParticle::TYPE00_NORMAL, 50, 20, 40.0f, 40.0f, 20, 1, false, Pos, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), true);
						m_nSubPattern++;
						m_nPatternTime = 0;
					}
					break;
				case 1:
					Move = NULL_VECTOR3;//移動量をなくす
					bUseGravity = false;//重力をなくす

					if (m_nPatternTime == 90)
					{
						pAttackEnemy = CAttackEnemy::Create(CAttack::ATTACKTYPE11_EXPLOSION, 10, 45, Pos, NULL_VECTOR3, NULL_VECTOR3, NULL_VECTOR3,
							CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, NULL_VECTOR3, false, 15);
						pAttackEnemy->SetUseAddScale(D3DXVECTOR3(0.3f,0.3f,0.3f),true);
						pAttackEnemy->SetUseThinColor(true);//色を体力によって変えるかどうか
						m_nSubPattern++;
						m_nPatternTime = 0;
					}
					break;
				case 2:
					if (m_nPatternTime == 60)
					{
						bUseGravity = true;
						ResetPattern();
					}
					break;
				default:
					break;
				}
				break;
			case 2://テレポートショット
				switch (m_nSubPattern)
				{
				case 0:
					if (m_nPatternTime == 35)
					{
						CParticle::SummonChargeParticle(CParticle::TYPE00_NORMAL, 50, 150,1.0f, 40.0f, 40.0f, 200, 20, 1, true, Pos, NORMAL_COL, true);
						SetOriginalColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), 50);
						CUi3D::Create(CUi3D::UI3DTYPE01_WARNING, 60, 120.0f, 120.0f, GetSenterPos(),NULL_VECTOR3);
					}

					if (m_nPatternTime == 85)
					{
						SummonPos.x = float(rand() % 400 - 200);
						SummonPos.y = float(rand() % 400 - 200);

						Pos = CGame::GetPlayer()->GetPos() + SummonPos;
						CParticle::SummonParticle(CParticle::TYPE00_NORMAL, 50, 20, 40.0f, 40.0f, 20, 1, false, Pos, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), true);
						m_nSubPattern++;
						m_nPatternTime = 0;
					}
					break;
				case 1:
					Move = NULL_VECTOR3;//移動量をなくす
					bUseGravity = false;//重力をなくす
					if (m_nSubPattern == 2)
					{
						CUi3D::Create(CUi3D::UI3DTYPE01_WARNING, 60, 60.0f, 60.0f, GetSenterPos(), D3DXVECTOR3(sinf(fVLaim) * 20.0f, cosf(fVLaim) * 20.0f, 0.0f));
					}
					if (m_nPatternTime == 90)
					{
						pBullet = CBullet::Create(CBullet::BULLETTYPE_ENEMY, CBullet::BULLETTEX_ENEMY, 120, 11, 40.0f, 40.0f, Pos, D3DXVECTOR3(sinf(fVLaim) * 10.0f, cosf(fVLaim) * 10.0f, 0.0f));
						pBullet->SetUseAddSpeed(true, true, 1.02f);
						pBullet->SetUseEffect(60, D3DXVECTOR2(40.0f, 40.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
						m_nSubPattern++;
						m_nPatternTime = 0;
					}
					break;
				case 2:
					if (m_nPatternTime == 60)
					{
						bUseGravity = true;
						ResetPattern();
					}
					break;
				default:
					break;
				}
				break;

			default:
				break;
			}
		}
	}
	//===========================================================================

}
//===================================================================================================================================

//======================================================================
//敵１０のパターン
//======================================================================
void CEnemy::Enemy10Pattern()
{
	D3DXVECTOR3& Pos = GetPos();                                  //モデルの位置の取得
	D3DXVECTOR3& PosOld = GetPosOld();                            //1f前の位置を設定
	D3DXVECTOR3& Move = GetMove();                                //移動量を取得
	D3DXVECTOR3& Rot = GetRot();                                  //モデルの向きの取得
	D3DXVECTOR3 Size = GetSize();                                 //サイズを取得
	int nPattern = 0;                                             //パターン
	float fAddSpeed = float(rand() % 50 + 1) / 10;                //速さの補正
	float fRandRot = float(rand() % 628 + 1) / 100;               //乱数向き
	float fVXaim = 0.0f;                                          //X方向ベクトル
	float fVYaim = 0.0f;                                          //Y方向ベクトル
	float fVLaim = 0.0f;                                          //総合ベクトル
	D3DXMATRIX mtxWorld = GetMatrixWorld();                       //マトリックスワールド
	CBullet* pBullet = nullptr;                                   //バレットへのポインタ
	CAttackEnemy* pAttackEnemy = nullptr;                         //敵の攻撃取得用
	float fAddRot = 0.0f;                                         //加算向き
	D3DXVECTOR3 SummonPos = NULL_VECTOR3;                         //自分自身を召喚する位置
	bool& bIsLanding = GetLanding();                              //地面にいるかどうかを取得
	bool& bUseGravity = GetUseGravity();                          //重力を使用するかどうか

	fVXaim = CGame::GetPlayer()->GetPos().x - Pos.x;
	fVYaim = (CGame::GetPlayer()->GetPos().y + CGame::GetPlayer()->GetSize().y * 0.5f) - Pos.y;//プレイヤーの中心を狙う
	fVLaim = (float)(atan2(fVXaim, fVYaim));


	//==========================================
	//行動開始
	//==========================================
	if (m_bAction == true)
	{
		if (m_bPattern == false)
		{
			//=============================
			//AI
			//=============================
			m_nPattern = rand() % 5;
			m_bPattern = true;
		}

		if (m_bPattern == true)
		{
			m_nPatternTime++;
			switch (m_nPattern)
			{
			case 0://直角移動
				switch (m_nSubPattern)
				{
				case 0:
					if (m_nPatternTime == 15)
					{
						CParticle::SummonParticle(CParticle::TYPE00_NORMAL, 50, 20, 40.0f, 40.0f, 200, 10, false, Pos, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), true);
					}

					if (m_nPatternTime == 45)
					{
						bUseGravity = false;
						SetUseInteria(false);
						ApproachProcess(10.0f);
						m_nSubPattern++;
						m_nPatternTime = 0;
					}
					break;
				case 1:
					if (m_nPatternTime == 30)
					{
						Move.x = 0.0f;
						m_nSubPattern++;
						m_nPatternTime = 0;
						if (Pos.y <= CGame::GetPlayer()->GetPos().y)
						{
							Move.y = 10.0f;
						}
						else
						{
							Move.y = -10.0f;
						}
					}
					CBullet::Create(CBullet::BULLETTYPE_ENEMY, CBullet::BULLETTEX_ENEMY, 30, 10, 40.0f, 40.0f, Pos + D3DXVECTOR3(0.0f, Size.y / 2.0f, 0.0f),
						D3DXVECTOR3(sinf(0.0f) * 5.0f, cosf(0.0f) * 5.0f, 0.0f));
					CBullet::Create(CBullet::BULLETTYPE_ENEMY, CBullet::BULLETTEX_ENEMY, 30, 10, 40.0f, 40.0f, Pos + D3DXVECTOR3(0.0f, Size.y / 2.0f, 0.0f),
						D3DXVECTOR3(sinf(0.0f) * -5.0f, cosf(0.0f) * -5.0f, 0.0f));
					break;
				case 2:
					CBullet::Create(CBullet::BULLETTYPE_ENEMY, CBullet::BULLETTEX_ENEMY, 30, 10, 40.0f, 40.0f, Pos + D3DXVECTOR3(0.0f, Size.y / 2.0f, 0.0f),
						D3DXVECTOR3(sinf(D3DX_PI * 0.5f) * 5.0f, cosf(D3DX_PI * 0.5f) * 5.0f, 0.0f));
					CBullet::Create(CBullet::BULLETTYPE_ENEMY, CBullet::BULLETTEX_ENEMY, 30, 10, 40.0f, 40.0f, Pos + D3DXVECTOR3(0.0f, Size.y / 2.0f, 0.0f),
						D3DXVECTOR3(sinf(D3DX_PI * 0.5f) * -5.0f, cosf(D3DX_PI * 0.5f) * -5.0f, 0.0f));

					if (m_nPatternTime == 30)
					{
						bUseGravity = true;
						SetUseInteria(true);
						ResetPattern();
					}
					break;
				default:
					break;
				}
				break;
			case 1://メテオブラスト
				if (bIsLanding == true)
				{
					Move.y = 20.0f;
					m_nCntAction++;

					for (int nCnt = 0; nCnt < 5; nCnt++)
					{
						SummonPos.x = float(rand() % 200 - 100) + Pos.x;
						SummonPos.y = float(rand() % 200 - 100) + Pos.y;
						fVXaim = CGame::GetPlayer()->GetPos().x - SummonPos.x;
						fVYaim = (CGame::GetPlayer()->GetPos().y + CGame::GetPlayer()->GetSize().y * 0.5f) - SummonPos.y;//プレイヤーの中心を狙う
						fVLaim = (float)(atan2(fVXaim, fVYaim));

						CParticle::SummonParticle(CParticle::TYPE00_NORMAL, 50, 20, 40.0f, 40.0f, 200, 10, false, SummonPos, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), true);
						pAttackEnemy = CAttackEnemy::Create(CAttack::ATTACKTYPE01_METEO, 15, 180, SummonPos,
							D3DXVECTOR3(sinf(fVLaim) * 1.0f, cosf(fVLaim) * 1.0f, 0.0f), ONE_VECTOR3 * 2.0f, NULL_VECTOR3,
							CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, ONE_VECTOR3 * 0.1f, true, 30);
						pAttackEnemy->SetUseMultiSpeed(true, ONE_VECTOR3 * 1.03f);
					}

				}

				if (m_nCntAction == 5)
				{
					ResetPattern();
				}
				break;
			case 2://スーパーカーブ弾
				if (m_nPatternTime == 25)
				{
					CParticle::SummonParticle(CParticle::TYPE00_NORMAL, 50, 20, 40.0f, 40.0f, 200, 10, false, Pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), true);
				}

				if (m_nPatternTime == 65)
				{
					for (int nCnt = 0; nCnt < 8; nCnt++)
					{
						float fRot = ((D3DX_PI * 2.0f) / 8.0f) * nCnt;
						pBullet = CBullet::Create(CBullet::BULLETTYPE_ENEMY, CBullet::BULLETTEX_ENEMY, 100, 15, 30.0f, 30.0f, Pos, NULL_VECTOR3);
						pBullet->SetUseCurve(fRot + fRandRot, 10.0f, 0.03f);
						pBullet->SetUseEffect(30, D3DXVECTOR2(30.0f, 30.0f), D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f));
					}

					for (int nCnt = 0; nCnt < 8; nCnt++)
					{
						float fRot = ((D3DX_PI * 2.0f) / 8.0f) * nCnt;
						pBullet = CBullet::Create(CBullet::BULLETTYPE_ENEMY, CBullet::BULLETTEX_ENEMY, 100, 15, 30.0f, 30.0f, Pos, NULL_VECTOR3);
						pBullet->SetUseCurve(fRot + fRandRot, 10.0f, -0.03f);
						pBullet->SetUseEffect(30, D3DXVECTOR2(30.0f,30.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
					}
				}

				if (m_nPatternTime == 100)
				{
					bUseGravity = true;
					SetUseInteria(true);
					ResetPattern();
				}
				break;
			case 3://エクストリームプレス
				if (m_nPatternTime == 25)
				{
					CParticle::SummonParticle(CParticle::TYPE00_NORMAL, 50, 20, 40.0f, 40.0f, 200, 10, false, Pos, D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), true);
				}

				if (m_nPatternTime >= 26)
				{
					ApproachProcess(3.0f);
					if (bIsLanding == true)
					{
						CManager::GetCamera()->SetShake(20, 0.1f);
						m_nSubPattern++;
						if (m_nSubPattern <= 10)
						{
							pAttackEnemy = CAttackEnemy::Create(CAttack::ATTACKTYPE10_SHOCKWAVE, 25, 300, Pos, D3DXVECTOR3(sinf(D3DX_PI * 0.5f) * 1.0f, cosf(D3DX_PI * 0.5f) * 1.0f, 0.0f),
								ONE_VECTOR3 * (float)(m_nSubPattern * 0.4f), NULL_VECTOR3, CAttack::COLLISIONTYPE02_TRUERECTANGLEXY,
								NULL_VECTOR3, true, 11);

							pAttackEnemy->SetUseMultiSpeed(true, ONE_VECTOR3 * (1.05f + 0.004f * (float(m_nSubPattern))));

							pAttackEnemy = CAttackEnemy::Create(CAttack::ATTACKTYPE10_SHOCKWAVE, 25, 300, Pos, D3DXVECTOR3(sinf(D3DX_PI * 0.5f) * -1.0f, cosf(D3DX_PI * 0.5f) * -1.0f, 0.0f),
								ONE_VECTOR3 * (float)(m_nSubPattern * 0.4f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), CAttack::COLLISIONTYPE02_TRUERECTANGLEXY,
								NULL_VECTOR3, true, 11);

							pAttackEnemy->SetUseMultiSpeed(true, ONE_VECTOR3* (1.05f + 0.004f * (float(m_nSubPattern))));
						}
						else
						{
							m_nCntAction++;
							Move.y = 5.0f + 5.0f * (float)(m_nCntAction);
							m_nSubPattern = 0;
						}
					}

					if (m_nCntAction == 5)
					{
						CParticle::SummonParticle(CParticle::TYPE00_NORMAL, 100, 20, 40.0f, 40.0f, 300, 10, false, Pos, D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), true);

						for (int nCnt = 0; nCnt < 10; nCnt++)
						{
							fAddRot = float(rand() % 150 - 75);
							pBullet = CBullet::Create(CBullet::BULLETTYPE_ENEMY, CBullet::BULLETTEX_ENEMY, 120, 12, 30.0f, 30.0f, Pos,
								D3DXVECTOR3(sinf(0.0f + fAddRot) * 10.0f, cosf(0.0f + fAddRot) * 10.0f, 0.0f));
							pBullet->SetUseGravity(-0.3f);
						}
						ResetPattern();
					}
				}
				break;
			case 4://クロスサンダー
				switch (m_nSubPattern)
				{
				case 0:
					if (m_nPatternTime == 25)
					{
						SetUseInteria(false);
						CParticle::SummonParticle(CParticle::TYPE00_NORMAL, 50, 60, 40.0f, 40.0f, 200, 10, false, Pos, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), true);
					}

					if (m_nPatternTime == 50)
					{
						bUseGravity = false;
						Move.y = 10.0f;
					}

					if (m_nPatternTime == 80)
					{
						m_nSubPattern++;
						m_nPatternTime = 0;
					}
					break;
				case 1:
					Rot.z = fVLaim;
					Move = NULL_VECTOR3;//動きを止める
					if (m_nPatternTime == 80)
					{
						Move.x = sinf(fVLaim) * 20.0f;
						Move.y = cosf(fVLaim) * 20.0f;
						m_nSubPattern++;
						m_nPatternTime = 0;
					}
					break;
				case 2:
					if (bIsLanding == true)
					{
						Move = NULL_VECTOR3;
						m_nPatternTime = 0;
						m_nSubPattern++;
					}
					break;
				case 3:
					if (m_nPatternTime % 10 == 0)
					{
						m_nCntAction++;

						if (m_nCntAction == 1)
						{
							pAttackEnemy = CAttackEnemy::Create(CAttack::ATTACKTYPE11_EXPLOSION, 2,50, Pos, NULL_VECTOR3, NULL_VECTOR3, NULL_VECTOR3,
								CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, ONE_VECTOR3 * 0.2f, false, 5);
							pAttackEnemy->SetUseAddScale(ONE_VECTOR3 * 0.3f,true);
							pAttackEnemy->SetOriginalColor(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f),50);
							pAttackEnemy->SetUseThinColor(true);
						}

						pAttackEnemy = CAttackEnemy::Create(CAttack::ATTACKTYPE06_THUNDER, 30, 150, Pos + D3DXVECTOR3(90.0f * (float)(m_nCntAction + 1), 300.0f, 0.0f),
							NULL_VECTOR3, ONE_VECTOR3, NULL_VECTOR3, CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, D3DXVECTOR3(0.0f, 0.3f, 0.0f), true, 10);
						pAttackEnemy->SetUseAddScale(D3DXVECTOR3(0.0f, 0.1f, 0.0f),true);

						pAttackEnemy = CAttackEnemy::Create(CAttack::ATTACKTYPE06_THUNDER, 30, 150, Pos + D3DXVECTOR3(-90.0f * (float)(m_nCntAction + 1), 300.0f, 0.0f),
							NULL_VECTOR3, ONE_VECTOR3, NULL_VECTOR3, CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, D3DXVECTOR3(0.0f, 0.3f, 0.0f), true, 10);
						pAttackEnemy->SetUseAddScale(D3DXVECTOR3(0.0f, 0.1f, 0.0f),true);
					}

					if (m_nCntAction == 10)
					{
						Rot.z = 0.0f;
						SetUseInteria(true);
						bUseGravity = true;
						ResetPattern();
					}
				default:
					break;
				}
			break;
			default:
				break;
			}
		}
	}
	//===========================================================================

}
//===================================================================================================================================
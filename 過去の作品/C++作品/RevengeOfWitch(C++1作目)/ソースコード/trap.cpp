//=====================================================================
//
//７月１６日：トラップを作る[trap.cpp]
//Author:ShinaTaiyo
//
//=====================================================================

//======================================
//インクルード
//======================================
#include "trap.h"
#include "main.h"
#include "game.h"
#include "edit.h"
#include "manager.h"
#include "renderer.h"
#include "particle.h"
#include "player.h"
#include "fade.h"
#include "block.h"
#include "objectXInfo.h"
#include "stagemanager.h"
#include "debugtext.h"
#include "input.h"
#include "bullet.h"
//=====================================================================

//======================================
//静的メンバ初期化
//======================================
int CTrap::m_nNumFile = CTrap::TRAPTYPE_MAX;  //ファイルの数
const int CTrap::m_nTRAPSUBTYPENUM[CTrap::TRAPTYPE_MAX] =
{ 5,//トゲボール
  2,//大砲
  1,//トゲこん棒
};
const char* CTrap::m_TRAPFILENAME[CTrap::TRAPTYPE_MAX] =
{ "data\\MODEL\\Trap\\Trap00_NeedleBall.x",
   "data\\MODEL\\Trap\\Trap01_Cannon.x",
   "data\\MODEL\\Trap\\Trap02_Thornclub.x",
};//トラップのモデルのパス

//======================================
//コンストラクタ
//======================================
CTrap::CTrap() : m_CollisionType(COLLISIONTYPE00_SQUARE)
{

}
//=====================================================================

//======================================
//デストラクタ
//======================================
CTrap::~CTrap()
{

}
//=====================================================================

//======================================
//初期化処理
//======================================
HRESULT CTrap::Init()
{
	//=================================
	//Xオブジェクト初期化
	//=================================
	CObjectX::Init();

	//=================================================================

	m_type = TRAPTYPE00_NEEDLEBALL;//種類
	m_bUse = true;                 //仕様状態
	m_nCntTime = 0;                //出現してからの時間
	m_nSubType = 0;                //サブタイプ
	m_bSpeedSwitch = false;        //速さを変えるフラグ
	m_bSpeedSwitch2 = false;       //上記その２
	m_bUseMove = true;             //移動量を使用するかどうか
	m_bTurn = false;               //ターン方向を変えるフラグ

	m_nInterval = 1;               //様々なインターバル
	m_fRotSpeed = 0.1f;            //回転スピード
	m_fSpeed = 0.2f;               //速さ
	m_fStartRot = 0.0f;            //開始向き
	m_fMoveLength = 100.0f;        //移動範囲
	m_fTurnSpeed = 5.0f;           //ターンするスピード
	m_nCntSetTime = 0;             //様々な時間を設定
	m_fAddSpeed = 0.0f;            //加速度
	return S_OK;
}
//=====================================================================

//======================================
//終了処理
//======================================
void CTrap::Uninit()
{
	CObjectX::Uninit(); //Xオブジェクトの破棄
}
//=====================================================================

//======================================
//更新処理
//======================================
void CTrap::Update()
{
	bool bStageManagerChoose = GetStageManagerChoose();//ステージマネージャーに選択されているかどうかを取得

	if (bStageManagerChoose == false)
	{//ステージマネージャーに選択されているかどうかを取得

		//==================================
		//Xオブジェクトから情報を取得
		//==================================
		D3DXVECTOR3& Pos = CObjectX::GetPos();                      //オブジェクトの位置を取得
		D3DXVECTOR3& Rot = CObjectX::GetRot();                      //オブジェクトの向きを取得
		D3DXVECTOR3& Move = CObjectX::GetMove();                    //オブジェクトの移動量を取得
		int& nLife = GetLife();                                     //体力
		//===========================================================

		//==================================
		//トラップごとのパターン
		//==================================
		m_nCntTime++;//出現してからの時間を測る

		//==================================
		//座標更新
		//==================================
		if (m_bUseMove == true)
		{
			Pos += Move;
		}
		//===========================================================

		//==================================
		//それぞれの挙動
		//==================================
		switch (m_type)
		{
		case TRAPTYPE00_NEEDLEBALL:
			NeedleBallProcess();
			break;
		case TRAPTYPE01_CANNON:
			CannonProcess();
			break;
		case TRAPTYPE02_THORNCLUB:
			ThornClubProcess();
			break;
		default:
			break;
		}
		//===========================================================

		//==================================
		//当たり判定を行う
		//==================================
		if (CScene::GetMode() == CScene::MODE_GAME)
		{
			switch (m_CollisionType)
			{
			case COLLISIONTYPE00_SQUARE:
				Collision();
				break;
			case COLLISIONTYPE01_RECTANGLESQUARE:

			default:
				break;
			}
		}
		//===========================================================

		//==================================
		//使用状態管理
		//==================================
		if (nLife <= 0)
		{//体力が０になったら・・・
			m_bUse = false;
		}
		//===========================================================

		//==================================
		//トラップの効果発動と破棄
		//==================================
		if (m_bUse == false)
		{
			SetDeath();
		}
		//===========================================================
	}
	else
	{//ステージマネージャーに選択されている時だけ情報を表示

	}

	//==================================
    //Xオブジェクト更新
    //==================================
	CObjectX::Update();
	//===========================================================

}
//=====================================================================

//======================================
//描画処理
//======================================
void CTrap::Draw()
{
	CObjectX::Draw();
}
//=====================================================================

//=======================================
//死亡フラグを設定
//=======================================
void CTrap::SetDeath()
{
	CObject::SetDeath();
}
//=====================================================================

//======================================
//トラップ生成処理
//======================================
CTrap* CTrap::Create(TRAPTYPE type, D3DXVECTOR3 Pos, D3DXVECTOR3 Rot, D3DXVECTOR3 Scale, int nRotType, int nSubType)
{
	CTrap* pTrap = DBG_NEW CTrap;   //敵を生成
	bool bSuccess = pTrap->CObject::GetCreateSuccess();
	int nIdx = 0;//モデルのインデックス
	if (bSuccess == true)
	{
		if (pTrap != nullptr)
		{
			pTrap->Init();         //初期化処理
			pTrap->SetRot(Rot);                                                                                     //向きを設定                    
			switch (type)
			{
			case TRAPTYPE02_THORNCLUB:
				pTrap->m_CollisionType = COLLISIONTYPE01_RECTANGLESQUARE;//斜めの判定を行う
				break;
			default:
				break;
			}

			pTrap->SetUseDeath(true);//死亡フラグを発動するかどうかを設定する
			pTrap->m_bUse = true;  //使用状態
			pTrap->SetUseGravity(false);
			pTrap->m_type = type;  //弾の種類
			pTrap->m_nSubType = nSubType;//サブタイプ
			pTrap->CObjectX::SetPos(Pos);               
			pTrap->SetUseBlockCollision(false);//位置を設定
			pTrap->SetLife(1);
			pTrap->SetMaxLife(1);
			pTrap->SetAutoSubLife(false);
			pTrap->SetPosOld(Pos);                                                                                  //1f前の位置を設定
			pTrap->SetSupportPos(Pos);                                                                              //設置位置
			pTrap->CObject::SetType(CObject::TYPE_TRAP);                                                            //オブジェクトの種類を決める
			pTrap->CObjectX::SetScale(Scale);                                                                       //オブジェクトの拡大率
			pTrap->SetFormarScale(Scale);                                                                           //元の拡大率を設定する
			nIdx = CManager::GetObjectXInfo()->Regist(CObjectX::OBJECTXTYPE_TRAP, m_TRAPFILENAME[(int)(type)]);
			pTrap->CObjectX::BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(CObjectX::OBJECTXTYPE_TRAP, nIdx),
				CManager::GetObjectXInfo()->GetBuffMat(CObjectX::OBJECTXTYPE_TRAP, nIdx),
				CManager::GetObjectXInfo()->GetdwNumMat(CObjectX::OBJECTXTYPE_TRAP, nIdx),
				CManager::GetObjectXInfo()->GetTexture(CObjectX::OBJECTXTYPE_TRAP, nIdx),
				CManager::GetObjectXInfo()->GetColorValue(CObjectX::OBJECTXTYPE_TRAP, nIdx));                       //モデル情報を割り当てる

			pTrap->CObjectX::SetObjXType(CObjectX::OBJECTXTYPE_TRAP);                                               //オブジェクトXのタイプを設定
			pTrap->CObjectX::SetTypeNum((int)(type));                                                               //オブジェクトXごとのタイプ番号を設定
			pTrap->SetSize();                                                                                       //Xオブジェクトのサイズを設定する
			pTrap->CObjectX::SetRotType(CObjectX::ROTTYPE::ROTTYPE_NORMAL);                                         //向きの種類を設定
			pTrap->SetManagerType((int)(CStageManager::MANAGEROBJECT_TRAP));                                        //マネージャーで呼び出す時の種類を設定
		}
		else
		{//オブジェクトに空きがなかったので破棄する
			delete pTrap;
			pTrap = nullptr;
			return nullptr;
		}
	}
	return pTrap;
}
//=====================================================================================================

//========================================================================================
//情報を設定する
//========================================================================================
void CTrap::SetInfo(float fStartRot, float fSpeed, float fRotSpeed, float fMoveLength, float fTurnSpeed, int nInterval, float fAddSpeed)
{
	m_fStartRot = fStartRot;     //回転開始位置
	D3DXVECTOR3& Rot = GetRot();
	switch (GetTrapType())
	{
	case TRAPTYPE02_THORNCLUB:
		Rot.z = fStartRot;
		break;
	default:
		break;
	}
	m_fSpeed = fSpeed;           //速さ
	m_fRotSpeed = fRotSpeed;     //回転速度
	m_fMoveLength = fMoveLength; //移動距離
	m_fTurnSpeed = fTurnSpeed;   //ターンするスピード
	m_nInterval = nInterval;     //様々なインターバル
	m_fAddSpeed = fAddSpeed;     //加速度
}
//=====================================================================================================

//====================================================
//当たり判定を行う
//====================================================
void CTrap::Collision(void)
{

}
//=====================================================================================================

//===============================================================
//トゲボールの処理
//===============================================================
void CTrap::NeedleBallProcess(void)
{
	D3DXVECTOR3& Pos = CObjectX::GetPos();                      //オブジェクトの位置を取得
	D3DXVECTOR3 SupportPos = CObjectX::GetSupportPos();         //支点の位置を取得
	D3DXVECTOR3& Rot = CObjectX::GetRot();                      //オブジェクトの向きを取得
	D3DXVECTOR3& Move = GetMove();                              //移動量を取得
    //支点と現在地の距離を測る
	float fLength = sqrtf(powf(Pos.x - SupportPos.x, 2) + powf(Pos.y - SupportPos.y, 2) + powf(Pos.z - SupportPos.z, 2));

	if (m_nCntTime == 1)
	{//最初の向きを設定
		Rot.z = m_fStartRot;
	}

	switch (m_nSubType)
	{
	case 0://回転移動
		Rot += ONE_VECTOR3 * m_fRotSpeed;
		Pos.x = GetSupportPos().x + sinf(m_fStartRot + Rot.z) * m_fMoveLength;
		Pos.y = GetSupportPos().y + cosf(m_fStartRot + Rot.z) * m_fMoveLength;
		break;
	case 1://XY別移動
		Rot += ONE_VECTOR3 * -0.1f;
		if (m_bSpeedSwitch == false)
		{
			Move.x -= sinf(m_fStartRot) * m_fSpeed;
			//Move.y -= cosf(m_fStartRot) * m_fSpeed;
		}
		else
		{
			Move.x += sinf(m_fStartRot) * m_fSpeed;
			//Move.y += cosf(m_fStartRot) * m_fSpeed;
		}

		if (m_bSpeedSwitch2 == false)
		{
			Move.y -= cosf(m_fStartRot) * m_fSpeed;
		}
		else
		{
			Move.y += cosf(m_fStartRot) * m_fSpeed;
		}

		if (Move.x <= -m_fTurnSpeed)
		{
			m_bSpeedSwitch = true;
		}
		else if (Move.x >= m_fTurnSpeed)
		{
			m_bSpeedSwitch = false;
		}

		if (Move.y <= -m_fTurnSpeed)
		{
			m_bSpeedSwitch2 = true;
		}
		else if (Move.y >= m_fTurnSpeed)
		{
			m_bSpeedSwitch2 = false;
		}
		break;
	case 2://加速ターン
		m_bUseMove = false;//移動量を使用しない

		Rot += ONE_VECTOR3 * -0.1f;
		if (m_bSpeedSwitch == false)
		{
			m_fSpeed -= m_fAddSpeed;

		}
		else
		{
			m_fSpeed += m_fAddSpeed;
		}

		Move.x = sinf(m_fStartRot) * m_fSpeed;
		Move.y = cosf(m_fStartRot) * m_fSpeed;

		if (fabsf(m_fSpeed) >= m_fTurnSpeed)
		{
			m_bSpeedSwitch = m_bSpeedSwitch ? false : true;
		}
		break;
	case 3://ターン移動

		Rot += ONE_VECTOR3 * 0.1f;
		//移動距離を超えたら
		if (fLength > m_fMoveLength)
		{
			m_bTurn = m_bTurn ? false : true;
		}
		//往復させる処理
		if (m_bTurn == false)
		{
			Move.x = cosf(m_fStartRot) * m_fSpeed;
			Move.y = sinf(m_fStartRot) * m_fSpeed;
		}
		else
		{
			Move.x = cosf(m_fStartRot) * -m_fSpeed;
			Move.y = sinf(m_fStartRot) * -m_fSpeed;
		}
		break;
	case 4:
		Rot += ONE_VECTOR3;
		break;
	default:
		break;
	}
}
//=====================================================================================================

//====================================================
//大砲の処理
//====================================================
void CTrap::CannonProcess(void)
{
	if (CScene::GetMode() == CScene::MODE_GAME)
	{
		float fVXaim = CGame::GetPlayer()->GetPos().x - GetPos().x;
		float fVYaim = CGame::GetPlayer()->GetPos().y - GetPos().y;
		float fVLaim = atan2f(fVXaim, fVYaim);
		D3DXVECTOR3& Scale = GetScale();
		D3DXVECTOR3& Rot = GetRot();
		CBullet* pBullet = nullptr;

		switch (m_nSubType)
		{
		case 0://自機狙い弾
			Rot.z = -fVLaim;
			if (m_nCntTime % m_nInterval == 0)
			{
				pBullet = CBullet::Create(CBullet::BULLETTYPE_ENEMY,CBullet::BULLETTEX_ENEMY, 120, 1, 40.0f * Scale.x, 40.0f * Scale.y, GetPos(), D3DXVECTOR3(sinf(fVLaim) * m_fSpeed, cosf(fVLaim) * m_fSpeed, 0.0f));
				pBullet->SetBlockCollision(true);
			}
			break;
		case 1://一方向弾
			if (m_nCntTime % m_nInterval == 0)
			{
			   pBullet = CBullet::Create(CBullet::BULLETTYPE_ENEMY, CBullet::BULLETTEX_ENEMY, 120, 1, 40.0f * Scale.x, 40.0f * Scale.y, GetPos(), D3DXVECTOR3(sinf(-Rot.z) * m_fSpeed, cosf(-Rot.z) * m_fSpeed, 0.0f));
			   pBullet->SetBlockCollision(true);
			}
			break;
		default:
			break;
		}
	}
}
//=====================================================================================================

//====================================================
//トゲこん棒の処理
//====================================================
void CTrap::ThornClubProcess()
{
	D3DXVECTOR3& Rot = GetRot();
	Rot.z += m_fRotSpeed;

}
//=====================================================================================================

//====================================================
//情報表示処理
//====================================================
void CTrap::DispInfo()
{
	D3DXVECTOR3& Pos = GetPos();//位置を取得
	D3DXVECTOR3& SupportPos = GetSupportPos();//支点となる位置を取得
	if (GetStageManagerChoose() == true)
	{//ステージマネージャーが選択していたら・・・

		//速度
		if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
		{//シフトキーを押しながら・・・
			if (CManager::GetInputKeyboard()->GetTrigger(DIK_1) == true)
			{
				m_fSpeed -= 0.1f;

			}
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_1) == true)
		{
			m_fSpeed += 0.1f;

		}

		//回転速度
		if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
		{//シフトキーを押しながら・・・
			if (CManager::GetInputKeyboard()->GetTrigger(DIK_2) == true)
			{
				m_fRotSpeed -= 0.01f;
			}
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_2) == true)
		{
			m_fRotSpeed += 0.01f;
		}

		//移動距離
		if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
		{//シフトキーを押しながら・・・
			if (CManager::GetInputKeyboard()->GetTrigger(DIK_3) == true)
			{
				m_fMoveLength -= 10.0f;
			}
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_3) == true)
		{
			m_fMoveLength += 10.0f;
		}

		//回転開始位置
		if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
		{//シフトキーを押しながら・・・
			if (CManager::GetInputKeyboard()->GetTrigger(DIK_4) == true)
			{
				m_fStartRot -= 0.1f;
			}
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_4) == true)
		{
			m_fStartRot += 0.1f;
		}

		//Uターンする速度
		if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
		{//シフトキーを押しながら・・・
			if (CManager::GetInputKeyboard()->GetTrigger(DIK_5) == true)
			{
				m_fTurnSpeed -= 0.1f;
			}
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_5) == true)
		{
			m_fTurnSpeed += 0.1f;
		}

		//インターバル
		if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
		{//シフトキーを押しながら・・・
			if (CManager::GetInputKeyboard()->GetTrigger(DIK_6) == true)
			{
				m_nInterval -= 1;
			}
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_6) == true)
		{
			m_nInterval += 1;
		}

		//加速度
		if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
		{//シフトキーを押しながら・・・
			if (CManager::GetInputKeyboard()->GetTrigger(DIK_7) == true)
			{
				m_fAddSpeed -= 0.05f;
			}
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_7) == true)
		{
			m_fAddSpeed += 0.05f;
		}

		if (m_nInterval <= 1)
		{
			m_nInterval = 1;
		}

		CObjectX::DispInfo();

		CManager::GetDebugText()->PrintDebugText("//=============================================\n");
		CManager::GetDebugText()->PrintDebugText("//選択トラップ情報\n");
		CManager::GetDebugText()->PrintDebugText("//=============================================\n");
		CManager::GetDebugText()->PrintDebugText("移動速度(1)：%f\n", m_fSpeed);
		CManager::GetDebugText()->PrintDebugText("回転速度(2)：%f\n", m_fRotSpeed);
		CManager::GetDebugText()->PrintDebugText("移動距離(3)：%f\n", m_fMoveLength);
		CManager::GetDebugText()->PrintDebugText("回転開始位置(4)：%f\n", m_fStartRot);
		CManager::GetDebugText()->PrintDebugText("ターン速度(5)：%f\n", m_fTurnSpeed);
		CManager::GetDebugText()->PrintDebugText("インターバル(6)：%d\n", m_nInterval);
		CManager::GetDebugText()->PrintDebugText("加速度(7)：%f\n", m_fAddSpeed);
		CManager::GetDebugText()->PrintDebugText("//=============================================\n");
	}
}
//=====================================================================================================
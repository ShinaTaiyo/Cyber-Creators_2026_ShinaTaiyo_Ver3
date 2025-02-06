//============================================================================================================================================
//
//１１月２５日：敵を作る[enemy.cpp]
//Author:ShinaTaiyo
//
//============================================================================================================================================

//====================================================================================
//インクルード
//====================================================================================
#include "enemy.h"
#include "manager.h"
#include "objectXInfo.h"
#include "game.h"
#include "calculation.h"
#include "debugtext.h"
#include "collision.h"
#include "damage.h"
#include "particle.h"
#include "phasemanager.h"
#include "input.h"
#include "effect.h"
#include "block.h"
#include "camera.h"
#include "attack.h"
//============================================================================================================================================

//====================================================================================
//静的メンバ宣言
//====================================================================================
//const string CEnemy::m_aENEMY_FILENAME[static_cast<int>(ENEMYTYPE::MAX)] =
//{
//	"data\\MODEL\\Enemy\\Normal\\noobSlime.x"
//};
int CEnemy::m_nNumEnemy = 0;

//====================================================================================
//コンストラクタ
//====================================================================================
CEnemy::CEnemy(int nPri, bool bUseintPri, CObject::TYPE type, CObject::OBJECTTYPE ObjType) : CObjectX(nPri, bUseintPri, type, ObjType),
m_Type(ENEMYTYPE::SHOTWEAK), m_VecMoveAi(), m_MoveAiSavePos(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),m_nCntTime(0),m_nIdxMoveAi(0), m_nPhaseNum(0),m_pEnemyMove(DBG_NEW CEnemyMove_AI()),
m_fRotMove(0.0f),m_fSensingRange(0.0f),m_fNormalSpeed(0.0f),m_Pattern(),m_bCollisoinDetection(true),m_bActivateCollisionDetection(false),m_bCollisionWall(false),
m_DefeatAttackType(CAttack::ATTACKTYPE::EXPLOSION),m_nAttackCoolTime(0),m_bPossibleAttack(true),m_State(CEnemy::STATE::NORMAL),m_bStartLanding(false),m_nJumpCoolTime(0)
{
	m_nNumEnemy++;//敵総数カウントアップ
}
//============================================================================================================================================

//====================================================================================
//デストラクタ
//====================================================================================
CEnemy::~CEnemy()
{
	m_nNumEnemy--;//敵総数カウントダウン
}
//============================================================================================================================================

//====================================================================================
//初期化処理
//====================================================================================
HRESULT CEnemy::Init()
{
	CObjectX::Init();

	GetMoveInfo().SetUseGravity(true, CObjectX::GetNormalGravity());
	GetMoveInfo().SetUseUpdatePos(true);
	return S_OK;
}
//============================================================================================================================================

//====================================================================================
//終了処理
//====================================================================================
void CEnemy::Uninit()
{
	CObjectX::Uninit();

	if (m_pEnemyMove != nullptr)
	{
		delete m_pEnemyMove;
		m_pEnemyMove = nullptr;
	}
}
//============================================================================================================================================

//====================================================================================
//更新処理
//====================================================================================
void CEnemy::Update()
{
	if (CScene::GetMode() == CScene::MODE_GAME)
	{
		const D3DXVECTOR3& Rot = GetRotInfo().GetRot();
		const D3DXVECTOR3& Pos = GetPosInfo().GetPos();
		const D3DXVECTOR3& PlayerPos = CGame::GetPlayer()->GetPosInfo().GetPos();
		//float fElevaRot = atan2f(PlayerPos.y - Pos.y, sqrtf(powf(PlayerPos.x - Pos.x, 2) + powf(PlayerPos.z - Pos.z, 2)));

		float fLength = CCalculation::CalculationLength(Pos, PlayerPos);

		m_nCntTime++;//時間をカウントする
		m_nJumpCoolTime++;  //ジャンプまでのクールタイムをカウントする
		m_nAttackCoolTime++;//クールタイムをカウントする

		if (GetLanding() == true)
		{
			GetMoveInfo().SetMove(D3DXVECTOR3(GetMoveInfo().GetMove().x,0.0f, GetMoveInfo().GetMove().z));
		}

		m_pEnemyMove->Process(this);//移動処理全般ステート

		if (m_bPossibleAttack == true)
		{
			AttackProcess();//攻撃処理
		}

		//敵の位置の調整
		CollisionDetectionProcess();

		if (m_Type == ENEMYTYPE::DIVEWEAK && m_bStartLanding == true)
		{//ダイブに弱い敵なら、最初に乗った地面から落ちない
			CheckGrroundDistance_AdjustPos();
		}

	    CObjectX::Update();

		if (GetLanding() == true && m_bStartLanding == false)
		{
			m_bStartLanding = true;
		}

		CollisionProcess();//当たり判定処理

		DefeatStaging();//倒されたときの演出を行う（派生クラスごとに変わる）


		if (GetPosInfo().GetPos().y < -100.0f)
		{
			SetDeath();
		}

	}
	if (CScene::GetMode() == CScene::MODE_EDIT)
	{
		GetDrawInfo().ChengeColorProcess(this);

		for (auto it : m_VecMoveAi)
		{
			if (it->GetDrawInfo().GetUseDraw())
			{
				it->GetDrawInfo().SetUseDraw(false);
			}
		}
	}

}
//============================================================================================================================================

//====================================================================================
//描画処理
//====================================================================================
void CEnemy::Draw()
{
	CObjectX::Draw();

	for (auto pObj : m_VecMoveAi)
	{
		pObj->ExtraDraw();
	}
}
//============================================================================================================================================

//====================================================================================
//死亡フラグ設定処理
//====================================================================================
void CEnemy::SetDeath()
{
	CObjectX::SetDeath();

	if (GetUseDeath() == true)
	{
		for (auto pObj : m_VecMoveAi)
		{
			if (pObj != nullptr)
			{
				pObj->SetUseDeath(true);
				pObj->SetDeath();
				pObj = nullptr;
			}
		}
		m_VecMoveAi.clear();
	}
}
//============================================================================================================================================

//====================================================================================
//生成処理
//====================================================================================
//CEnemy* CEnemy::Create(ENEMYTYPE Type, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 Scale)
//{
//	CEnemy* pEnemy = DBG_NEW CEnemy;
//
//	pEnemy->Init();            //初期化処理
//	int nIdx = CManager::GetObjectXInfo()->Regist(m_aENEMY_FILENAME[int(Type)]);
//	pEnemy->BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(nIdx),
//		CManager::GetObjectXInfo()->GetBuffMat(nIdx),
//		CManager::GetObjectXInfo()->GetdwNumMat(nIdx),
//		CManager::GetObjectXInfo()->GetTexture(nIdx),
//		CManager::GetObjectXInfo()->GetColorValue(nIdx));
//
//	pEnemy->SetType(CObject::TYPE::ENEMY);//オブジェクトタイプを設定
//	pEnemy->SetEnemyType(Type);//敵の種類を設定
//	pEnemy->SetLife(nLife);    //体力
//	pEnemy->SetMaxLife(nLife); //最大体力
//	pEnemy->SetPos(pos);       //位置
//	pEnemy->SetSupportPos(pos);//支点位置
//	pEnemy->SetRot(rot);       //向き
//	pEnemy->SetScale(Scale);   //拡大率
//	pEnemy->SetFormarScale(Scale);//元の拡大率を設定
//	pEnemy->SetSize();//モデルサイズを設定
//	pEnemy->SetAutoSubLife(false);//自動的に体力を減らすかどうか
//
//	return pEnemy;
//}
////============================================================================================================================================

//====================================================================================
//テキストファイルに情報をセーブする
//====================================================================================
void CEnemy::SaveInfoTxt(fstream& WritingFile)
{
	WritingFile << "ENEMYTYPE = " << static_cast<int>(m_Type);
	switch (m_Type)
	{
	case ENEMYTYPE::SHOTWEAK:
		WritingFile << " # SHOTWEAK" << endl;
		break;
	case ENEMYTYPE::DIVEWEAK:
		WritingFile << " # DIVEWEAK" << endl;
		break;
	case ENEMYTYPE::IDLE:
		WritingFile << " # IDLE" << endl;
		break;
	default:
		assert(false);
		break;
	}

	WritingFile << "NORMALSPEED = " << m_fNormalSpeed << endl;
	WritingFile << "SENSINGRANGE = " << m_fSensingRange << endl;

	//フェーズ番号を設定
	WritingFile << "PHASENUM = " << m_nPhaseNum << endl;

	//移動AIの位置を保存
	WritingFile << "SETMOVEAI" << endl;

	int nCnt = 0;
	for (auto it = m_VecMoveAi.begin(); it != m_VecMoveAi.end(); ++it)
	{
		WritingFile << "SETNUM = " << nCnt << endl;

		WritingFile << "POS = " << fixed << setprecision(3) << (*it)->GetPosInfo().GetPos().x << " " <<
			fixed << setprecision(3) << (*it)->GetPosInfo().GetPos().y << " " <<
			fixed << setprecision(3) << (*it)->GetPosInfo().GetPos().z << " " << endl;//位置
		WritingFile << "ROT = " << fixed << setprecision(3) << (*it)->GetRotInfo().GetRot().x << " " <<
			fixed << setprecision(3) << (*it)->GetRotInfo().GetRot().y << " " <<
			fixed << setprecision(3) << (*it)->GetRotInfo().GetRot().z << " " << endl;//向き
		WritingFile << "SCALE = " << fixed << setprecision(3) << (*it)->GetSizeInfo().GetScale().x << " " <<
			fixed << setprecision(3) << (*it)->GetSizeInfo().GetScale().y << " " <<
			fixed << setprecision(3) << (*it)->GetSizeInfo().GetScale().z << " " << endl;//拡大率

		WritingFile << "END_SETNUM" << endl;

		nCnt++;//カウントアップ
	}
	WritingFile << "END_SETMOVEAI" << endl;

	CObjectX::SaveInfoTxt(WritingFile);
}
//============================================================================================================================================

//====================================================================================
//テキストファイルから情報をロードする
//====================================================================================
void CEnemy::LoadInfoTxt(fstream& LoadingFile, list<CObject*>& listSaveManager, string& Buff)
{
	//int nType = 0;//種類
	//int nLife = 0;//体力
	//D3DXVECTOR3 Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  //移動量
	//D3DXVECTOR3 Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   //位置
	//D3DXVECTOR3 Scale = D3DXVECTOR3(0.0f, 0.0f, 0.0f); //拡大率
	//D3DXVECTOR3 Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   //向き
	//ENEMYTYPE Type = {};                               //ブロックの種類
	//while (Buff != "END_SETBLOCK")
	//{
	//	LoadingFile >> Buff;//単語を読み込む
	//	if (Buff == "#")
	//	{
	//		getline(LoadingFile, Buff);
	//	}
	//	else if (Buff == "TYPE")
	//	{
	//		LoadingFile >> Buff;//イコール
	//		LoadingFile >> nType;      //種類
	//	}
	//	else if (Buff == "LIFE")
	//	{
	//		LoadingFile >> Buff;//イコール
	//		LoadingFile >> nLife;      //体力
	//	}
	//	else if (Buff == "MOVE")
	//	{
	//		LoadingFile >> Buff;//イコール
	//		LoadingFile >> Move.x;      //移動量X
	//		LoadingFile >> Move.y;      //移動量Y
	//		LoadingFile >> Move.z;      //移動量Z
	//	}
	//	else if (Buff == "POS")
	//	{
	//		LoadingFile >> Buff;//イコール
	//		LoadingFile >> Pos.x;      //位置X
	//		LoadingFile >> Pos.y;      //位置Y
	//		LoadingFile >> Pos.z;      //位置Z
	//	}
	//	else if (Buff == "ROT")
	//	{
	//		LoadingFile >> Buff;//イコール
	//		LoadingFile >> Rot.x;      //位置X
	//		LoadingFile >> Rot.y;      //位置Y
	//		LoadingFile >> Rot.z;      //位置Z
	//	}
	//	else if (Buff == "SCALE")
	//	{
	//		LoadingFile >> Buff;//イコール
	//		LoadingFile >> Scale.x;      //拡大率X
	//		LoadingFile >> Scale.y;      //拡大率Y
	//		LoadingFile >> Scale.z;      //拡大率Z
	//	}
	//}
	//Type = static_cast<ENEMYTYPE>(nType);

	//

	//listSaveManager.push_back(CEnemy::Create(Type, nLife, Pos, Rot, Scale));//vectorに情報を保存する

}
//============================================================================================================================================

//====================================================================================
//テキストから情報をロードする
//====================================================================================
//CObject* CEnemy::ManagerChengeObject(bool bAim)
//{
//	int nNewType = static_cast<int>(m_Type);
//    ENEMYTYPE NewType = {};
//	//=======================================
//	//種類を変える
//	//=======================================
//	if (bAim == true)
//	{
//		nNewType++;
//	}
//	else
//	{
//		nNewType--;
//	}
//	if (nNewType >= static_cast<int>(ENEMYTYPE::MAX))
//	{
//		nNewType = 0;
//	}
//	if (nNewType < 0)
//	{
//		nNewType = static_cast<int>(ENEMYTYPE::MAX) - 1;
//	}
//	//======================================================================================
//
//	//=======================================
//	//設定する
//	//=======================================
//	NewType = ENEMYTYPE(nNewType);
//	//======================================================================================
//
//	//=======================================
//	//死亡フラグを設定する
//	//=======================================
//	SetUseDeath(true);
//	SetDeath();
//	//======================================================================================
//
//	return CEnemy::Create(NewType,GetLife,GetPos(), GetRot(), GetScale());//生成したオブジェクトを返す
//}
////============================================================================================================================================

//====================================================================================
//ステージマネージャーにセーブする
//====================================================================================
CObject* CEnemy::ManagerSaveObject()
{
	return nullptr;
}
//============================================================================================================================================

//====================================================================================
//ステージマネージャーから操作する
//====================================================================================
void CEnemy::ManagerChooseControlInfo()
{
	//ステージマネージャーに選択されている時だけ表示する
	for (auto it : m_VecMoveAi)
	{
		it->GetDrawInfo().SetUseDraw(true);
	}

	if (CObject::GetType() == CObject::TYPE::ENEMY)
	{
		CManager::GetDebugText()->PrintDebugText("敵が呼ばれている！\n");
	}

	SetMoveAiPoint();//移動AIの設定を行う

	PhaseNumDecision();//フェーズ番号の決定を行う

	EditNormalSpeed();//通常移動速度を編集

	EditSensingRange();//索敵範囲を編集

	CObjectX::ManagerChooseControlInfo();
}
//============================================================================================================================================

//====================================================================================
//移動AIの情報を設定する
//====================================================================================
void CEnemy::SetVecMoveAiInfo(vector<CAIModel*> vec)
{
	m_VecMoveAi = vec;
}
//============================================================================================================================================

//====================================================================================
//プレイヤーを追いかける処理
//====================================================================================
void CEnemy::ChasePlayer()
{
	float fLengthPlayer = CCalculation::CalculationLength(CGame::GetPlayer()->GetPosInfo().GetPos(), GetPosInfo().GetPos());
	const D3DXVECTOR3& PlayerPos = CGame::GetPlayer()->GetPosInfo().GetPos();
	const D3DXVECTOR3& Pos = GetPosInfo().GetPos();
	D3DXVECTOR3 Aim = PlayerPos - Pos;
	D3DXVec3Normalize(&Aim, &Aim);
	float fRot = atan2f(Aim.x, Aim.z);

	D3DXVECTOR3 Move = CCalculation::HormingVecRotXZ(m_fRotMove, GetPosInfo().GetPos(), CGame::GetPlayer()->GetPosInfo().GetSenterPos(), 0.1f, m_fNormalSpeed);
	GetRotInfo().SetRot(D3DXVECTOR3(GetRotInfo().GetRot().x, m_fRotMove, GetRotInfo().GetRot().z));
	GetMoveInfo().SetMove(D3DXVECTOR3(Move.x, GetMoveInfo().GetMove().y, Move.z));
}
//============================================================================================================================================

//====================================================================================
//敵が倒されたときの演出
//====================================================================================
void CEnemy::DefeatStaging()
{

}
//============================================================================================================================================

//====================================================================================
//当たり判定の処理を行う
//====================================================================================
void CEnemy::CollisionProcess()
{
	GetCollisionInfo().GetSquareInfo().ResetPushOutFirstFlag();//それぞれの軸の押し出し判定の優先フラグをリセット
	SetIsLanding(false);
	for (int nCntPri = 0; nCntPri < CObject::m_nMAXPRIORITY; nCntPri++)
	{
		CObject* pObj = CObject::GetTopObject(nCntPri);

		while (pObj != nullptr)
		{
			//次のオブジェクトを格納
			CObject* pNext = pObj->GetNextObject();

			//種類の取得（敵なら当たり判定）
			CObject::TYPE type = pObj->GetType();

			if (type == CObject::TYPE::BLOCK || type == CObject::TYPE::BGMODEL)
			{
				CObjectX* pObjX = static_cast<CObjectX*>(pObj);//オブジェクトXにダウンキャスト
				CCollision::ExtrusionCollisionSquarePushOutFirstDecide(this, pObjX);//正方形の押し出し判定のそれぞれの軸の順序の優先度を決める
			}
			//オブジェクトを次に進める
			pObj = pNext;
		}
	}
	//=======================================================================================

	//============================================================
	//押し出し判定開始
	//============================================================
	for (int nCntPri = 0; nCntPri < CObject::m_nMAXPRIORITY; nCntPri++)
	{
		CObject* pObj = CObject::GetTopObject(nCntPri);

		while (pObj != nullptr)
		{
			//次のオブジェクトを格納
			CObject* pNext = pObj->GetNextObject();

			//種類の取得（敵なら当たり判定）
			CObject::TYPE type = pObj->GetType();

			if (type == CObject::TYPE::BLOCK || type == CObject::TYPE::BGMODEL)
			{
				CObjectX* pObjX = static_cast<CObjectX*>(pObj);//オブジェクトXにダウンキャスト

				CCollision::ResolveExtrusionCollisionSquare(this, pObjX);//正方形の押し出し判定をする
			}

			pObj = pNext;
		}
	}
	//=======================================================================================

}
//============================================================================================================================================

//====================================================================================
//移動AIの配置をする
//====================================================================================
void CEnemy::SetMoveAiPoint()
{
	float fX = 0;
	float fZ = 0;
	bool bMove = false;
	bool bMoveY = false;
	float fAddPosY = 0.0f;
	auto Input = CManager::GetInputKeyboard();
	float fCameraRot = CManager::GetCamera()->GetRot().y;
	if (Input->GetPress(DIK_LSHIFT))
	{
		if (Input->GetPress(DIK_I))
		{
			fAddPosY = 5.0f;
			bMoveY = true;
		}
		else if (Input->GetPress(DIK_K))
		{
			fAddPosY = -5.0f;
			bMoveY = true;
		}
	}
	else
	{
		if (Input->GetPress(DIK_I))
		{
			fZ = 1;
			bMove = true;
		}
		else if (Input->GetPress(DIK_K))
		{
			fZ = -1;
			bMove = true;
		}
	}
	if (Input->GetPress(DIK_L))
	{
		fX = 1;
		bMove = true;
	}
	else if (Input->GetPress(DIK_J))
	{
		fX = -1;
		bMove = true;
	}

	GetMoveInfo().SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	CManager::GetDebugText()->PrintDebugText("移動AIの設置位置の移動：IKLJ\n");

	if (bMove == true)
	{
		float fRot = atan2f(fX, fZ);
		m_MoveAiSavePos += D3DXVECTOR3(sinf(fCameraRot + fRot) * 5.0f, 0.0f, cosf(fCameraRot + fRot) * 5.0f);
	}

	if (bMoveY == true)
	{
		m_MoveAiSavePos += D3DXVECTOR3(0.0f, fAddPosY, 0.0f);
	}

	D3DXVECTOR3 NowPos = m_MoveAiSavePos + GetPosInfo().GetPos();

	CManager::GetDebugText()->PrintDebugText("移動AIの位置：%f %f %f\n", NowPos.x,NowPos.y,NowPos.z);
	CManager::GetDebugText()->PrintDebugText("移動AIを保存：O\n");

	CParticle::SummonParticle(CParticle::TYPE::TYPE00_NORMAL, 1, 30, 40.0f, 40.0f, 100, 10, false, NowPos, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), true);

	if (Input->GetPress(DIK_LSHIFT))
	{
		if (m_VecMoveAi.size() > 0)
		{
			if (Input->GetTrigger(DIK_O))
			{
				auto it = m_VecMoveAi.end() - 1;
				(*it)->SetUseDeath(true);
				(*it)->SetDeath();
				m_VecMoveAi.pop_back();
			}
		}
	}
	else
	{
		if (Input->GetTrigger(DIK_O))
		{
			m_VecMoveAi.push_back(CAIModel::Create(CAIModel::AIMODELTYPE::MOVEPOINT, m_MoveAiSavePos + GetPosInfo().GetPos(), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(3.0f, 3.0f, 3.0f), nullptr));
		}
	}
}
//============================================================================================================================================

//====================================================================================
//AI移動処理
//====================================================================================
void CEnemy::AIMoveProcess()
{
	if (CScene::GetMode() == CScene::MODE_GAME)
	{
		//================
	    //使用する変数
		//================
		CObjectX::RotInfo& RotInfo = GetRotInfo();//向き情報を取得
		const D3DXVECTOR3& Rot = RotInfo.GetRot();//向きを取得
		float fAddRotY = 0.0f;//Y方向の加算向き
		float fLengthPlayer = CCalculation::CalculationLength(CGame::GetPlayer()->GetPosInfo().GetPos(), GetPosInfo().GetPos());//プレイヤーとの距離
		//=========================================================================================================================================

		if (m_VecMoveAi.size() > 0)
		{
			auto it = m_VecMoveAi.begin();//最初のポインタを取得

			advance(it, m_nIdxMoveAi);//指定している番号まで進める

			float fLength = CCalculation::CalculationLength(GetPosInfo().GetPos(), (*it)->GetPosInfo().GetPos());//距離を測る
			float fRot = atan2f((*it)->GetPosInfo().GetPos().x - GetPosInfo().GetPos().x, (*it)->GetPosInfo().GetPos().z - GetPosInfo().GetPos().z);

			RotInfo.SetRot(D3DXVECTOR3(Rot.x,CCalculation::CalculationCollectionRot2D(Rot.y, fRot, 0.1f, false),Rot.z));//向きを徐々に目的地へ合わせていく

			GetMoveInfo().SetMove(D3DXVECTOR3(sinf(fRot) * m_fNormalSpeed, GetMoveInfo().GetMove().y, cosf(fRot) * m_fNormalSpeed));

			if (fLength < m_fNormalSpeed + 50.0f)
			{
				m_nIdxMoveAi++;//目的地を次の位置に変更
			}

			int nSize = m_VecMoveAi.size();
			if (m_nIdxMoveAi >= nSize)
			{
				m_nIdxMoveAi = 0;
			}
			if (m_nIdxMoveAi < 0)
			{
				m_nIdxMoveAi = nSize - 1;
			}

		}
		if (fLengthPlayer < m_fSensingRange)
		{//プレイヤーが索敵範囲に入ったらバトルモードに変更
			ChengeMove(DBG_NEW CEnemyMove_Battle());
		}
	}
}
//============================================================================================================================================


//====================================================================================
//バトル移動処理
//====================================================================================
void CEnemy::BattleMoveProcess()
{
	float fLengthPlayer = CCalculation::CalculationLength(CGame::GetPlayer()->GetPosInfo().GetPos(), GetPosInfo().GetPos());

	ChasePlayer();//プレイヤーを追いかける処理

	if (fLengthPlayer > m_fSensingRange + 300.0f)
	{//距離が索敵範囲の+300.0fより遠くなったらAIモードに戻して徘徊させる
		ChengeMove(DBG_NEW CEnemyMove_AI());
	}
}
//============================================================================================================================================


//====================================================================================
//移動状態を変更する
//====================================================================================
void CEnemy::ChengeMove(CEnemyMove* pEnemyMove)
{
	//破棄する
	if (m_pEnemyMove != nullptr)
	{
		delete m_pEnemyMove;
		m_pEnemyMove = nullptr;
	}

	//代入する
	m_pEnemyMove = pEnemyMove;
}
//============================================================================================================================================

//====================================================================================
//レイが当たったかどうかを判定する
//====================================================================================
void CEnemy::RayCollision()
{
	if (CScene::GetMode() == CScene::MODE_GAME)
	{
		CObjectX* pObjX = nullptr;
		D3DXVECTOR3 Ray = CCalculation::RadToVec(D3DXVECTOR3(GetRotInfo().GetRot().x, GetRotInfo().GetRot().y,0.0f) - D3DXVECTOR3(D3DX_PI * 0.5f,0.0f,0.0f));//手前側にレイを飛ばす

		for (int nCntPri = 0; nCntPri < m_nMAXPRIORITY; nCntPri++)
		{
			CObject* pObj = GetTopObject(nCntPri);

			while (pObj != nullptr)
			{
				CObject* pNext = pObj->GetNextObject();
				D3DXVECTOR3 CollisionPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				if (pObj->GetType() == CObject::TYPE::BGMODEL || pObj->GetType() == CObject::TYPE::BLOCK)
				{
					CObjectX * pComObjX = static_cast<CObjectX*>(pObj);
					if (CCollision::RayIntersectsAABBCollisionPos(GetPosInfo().GetSenterPos(), Ray, pComObjX->GetPosInfo().GetPos() + pComObjX->GetSizeInfo().GetVtxMin(), pComObjX->GetPosInfo().GetPos() + pComObjX->GetSizeInfo().GetVtxMax(),
						CollisionPos))
					{
						float fLength = sqrtf(powf(CollisionPos.x - GetPosInfo().GetSenterPos().x, 2) +
							powf(CollisionPos.y - GetPosInfo().GetSenterPos().y, 2) + powf(CollisionPos.z - GetPosInfo().GetSenterPos().z,2));
						if (fLength < GetSizeInfo().GetSize().x + 50.0f)
						{
							//敵のオブジェクトのサイズの少し上より距離が小さくなった
							pObjX = pComObjX;
						}
					}
				}

				pObj = pNext;
			}
		}

		if (pObjX != nullptr)
		{//当たったオブジェクトがあった場合、プレイヤーとのベクトルの内積をとり、オブジェクトをよけるまで右側に動いたり左側に動いたりする
			D3DXVECTOR3 ObjectAim = pObjX->GetPosInfo().GetPos() - GetPosInfo().GetPos();
			D3DXVec3Normalize(&ObjectAim, &ObjectAim);

			float fDot = CCalculation::DetermineSide3D(GetPosInfo().GetPos(), ObjectAim, D3DXVECTOR3(0.0f, 1.0f, 0.0f), CGame::GetPlayer()->GetPosInfo().GetPos());
			const D3DXVECTOR3& PlayerPos = CGame::GetPlayer()->GetPosInfo().GetPos();
			const D3DXVECTOR3& Pos = GetPosInfo().GetPos();
			D3DXVECTOR3 Aim = PlayerPos - Pos;
			D3DXVec3Normalize(&Aim, &Aim);
			float fRot = atan2f(Aim.x, Aim.z);

			if (fDot > 0.0f)
			{//ベクトルに対してプレイヤーが右側にいたら
				//SetMove(D3DXVECTOR3(sinf(fRot - D3DX_PI * 0.5f) * m_fNormalSpeed, GetMove().y, cosf(fRot - D3DX_PI * 0.5f) * m_fNormalSpeed));
				ChengeMove(DBG_NEW CEnemyMove_DodgeWall(this, D3DXVECTOR3(sinf(fRot - D3DX_PI * 0.5f) * m_fNormalSpeed, GetMoveInfo().GetMove().y, cosf(fRot - D3DX_PI * 0.5f) * m_fNormalSpeed)));
			}
			else if (fDot < 0.0f)
			{//ベクトルに対してプレイヤーが左側にいたら
				ChengeMove(DBG_NEW CEnemyMove_DodgeWall(this, D3DXVECTOR3(sinf(fRot + D3DX_PI * 0.5f) * m_fNormalSpeed, GetMoveInfo().GetMove().y, cosf(fRot + D3DX_PI * 0.5f) * m_fNormalSpeed)));
			}
		}
	}
}
//============================================================================================================================================

//====================================================================================
//攻撃パターンを終了する
//====================================================================================
void CEnemy::EndAttackPattern()
{
	m_Pattern.nPattern = 0;
	m_Pattern.nPatternTime = 0; 
	m_Pattern.bAction = false;
	m_Pattern.nSubPattern = 0;
	m_nAttackCoolTime = 0;
	GetMoveInfo().SetUseGravity(true, GetNormalGravity());
	GetMoveInfo().SetUseInteria(false, GetNormalInertia());
}
//============================================================================================================================================


//====================================================================================
//フェーズ番号を決定する
//====================================================================================
void CEnemy::PhaseNumDecision()
{
	if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == false)
	{
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_4))
		{
			m_nPhaseNum++;
		}
	}
	else
	{
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_4))
		{
			m_nPhaseNum--;
		}
	}
	CManager::GetDebugText()->PrintDebugText("フェーズ番号の変更：%d\n", m_nPhaseNum);
}
//============================================================================================================================================


//====================================================================================
//攻撃処理
//====================================================================================
void CEnemy::AttackProcess()
{

}
//============================================================================================================================================

//====================================================================================
//衝突判定処理
//====================================================================================
void CEnemy::CollisionDetectionProcess()
{
	for (int nCntPri = 0; nCntPri < m_nMAXPRIORITY; nCntPri++)
	{
		CObject* pObj = GetTopObject(nCntPri);
		while (pObj != nullptr)
		{
			CObject* pNext = pObj->GetNextObject();
			if (pObj->GetType() == CObject::TYPE::ENEMY || pObj->GetType() == CObject::TYPE::PLAYER)
			{
				CObjectX* pObjX = static_cast<CObjectX*>(pObj);
				if (pObjX != this)
				{
					D3DXVECTOR3 AimVec = D3DXVECTOR3(pObjX->GetPosInfo().GetPos().x, 0.0f, pObjX->GetPosInfo().GetPos().z) -
						D3DXVECTOR3(GetPosInfo().GetPos().x, 0.0f, GetPosInfo().GetPos().z);//XZ方向だけのベクトルを取る
					float fLength = CCalculation::CalculationLength(GetPosInfo().GetPos(), pObjX->GetPosInfo().GetPos());
					const D3DXVECTOR3& ComPos = pObjX->GetPosInfo().GetPos();
					const D3DXVECTOR3& ComVtxMax = ComPos + D3DXVECTOR3(pObjX->GetSizeInfo().GetVtxMax().x, 0.0f, 0.0f);
					D3DXVec3Normalize(&AimVec, &AimVec);

					float fComCornarDistance = CCalculation::CalculationLength(ComPos,ComVtxMax);
					float fMyCornarDistance = CCalculation::CalculationLength(GetPosInfo().GetPos(), GetPosInfo().GetPos() + D3DXVECTOR3(GetSizeInfo().GetVtxMax().x, 0.0f, 0.0f));
					float fTotalLength = (fComCornarDistance + fMyCornarDistance);
					if (fLength < fTotalLength &&
						GetPosInfo().GetPos().y + GetSizeInfo().GetVtxMax().y >= pObjX->GetPosInfo().GetPos().y + pObjX->GetSizeInfo().GetVtxMin().y &&
						GetPosInfo().GetPos().y + GetSizeInfo().GetVtxMin().y <= pObjX->GetPosInfo().GetPos().y + pObjX->GetSizeInfo().GetVtxMax().y)
					{
						GetMoveInfo().SetMove(D3DXVECTOR3(GetMoveInfo().GetMove().x -AimVec.x * (fTotalLength - fLength) * 1.0f,GetMoveInfo().GetMove().y, GetMoveInfo().GetMove().z - AimVec.z * (fTotalLength - fLength) * 1.0f));//攻撃時の動きよりも優先的にこの移動量を割り当てる
					}

					
				}
			}

			pObj = pNext;
		}
	}
}

//====================================================================================
//衝突判定処理
//====================================================================================
void CEnemy::CheckGrroundDistance_AdjustPos()
{
	CObject* pObjBgModel = GetTopObject(static_cast<int>(CObject::TYPE::BGMODEL));//背景モデルのリストの先頭を取得
	CObject* pObjBlock = GetTopObject(static_cast<int>(CObject::TYPE::BLOCK));    //ブロックのリストの先頭を取得
	CObjectX::PosInfo& PosInfo = GetPosInfo();//位置情報
	CObjectX::SizeInfo& SizeInfo = GetSizeInfo();//サイズ情報
	CObjectX::MoveInfo& MoveInfo = GetMoveInfo();//移動情報
	const D3DXVECTOR3& Pos = PosInfo.GetPos();//位置
	const D3DXVECTOR3& PosOld = PosInfo.GetPosOld();//1f前の位置
	const D3DXVECTOR3& VtxMin = SizeInfo.GetVtxMin();//最小頂点
	const D3DXVECTOR3& VtxMax = SizeInfo.GetVtxMax();//最大頂点
	const D3DXVECTOR3& Move = MoveInfo.GetMove();//移動量
	D3DXVECTOR3 Dir = D3DXVECTOR3(0.0f, -1.0f, 0.0f);//下方向ベクトルを定義
	D3DXVECTOR3 OriginPos = Pos + D3DXVECTOR3(0.0f, VtxMin.y, 0.0f);//判定する位置（オブジェクトの底面）
	D3DXVECTOR3 RayCollisionPos = { 0.0f,0.0f,0.0f };//レイの判定場所計算用
	bool bCollision = false;
	float fTotalBestNearLength = 0.0f;//レイが当たったオブジェクトの距離の中で一番近い距離を決める
	int nCntRayCollosionObj = 0;//レイが当たったオブジェクトの数をカウントする
	//背景モデルリスト
	while (pObjBgModel != nullptr)
	{
		CObject* pNextBgMoodel = pObjBgModel->GetNextObject();
		CBgModel* pBgModel = static_cast<CBgModel*>(pObjBgModel);//背景モデルにキャスト
		CObjectX::PosInfo BgModelPosInfo = pBgModel->GetPosInfo();//背景モデルの位置情報
		CObjectX::SizeInfo BgModelSizeInfo = pBgModel->GetSizeInfo();//背景モデルのサイズ情報
		const D3DXVECTOR3& BgModelPos = BgModelPosInfo.GetPos();//背景モデルの位置
		const D3DXVECTOR3& BgModelPosOld = BgModelPosInfo.GetPosOld();//1f前の位置
		const D3DXVECTOR3& BgModelVtxMax = BgModelSizeInfo.GetVtxMax();//背景モデルの最小頂点
		const D3DXVECTOR3& BgModelVtxMin = BgModelSizeInfo.GetVtxMin();//背景モデルの最大頂
		if (CCollision::RayIntersectsAABBCollisionPos(OriginPos, Dir, BgModelPos + BgModelVtxMin, BgModelPos + BgModelVtxMax, RayCollisionPos))
		{
			float fLength = sqrtf(powf(RayCollisionPos.x - OriginPos.x, 2) +
				powf(RayCollisionPos.y - OriginPos.y, 2) +
				powf(RayCollisionPos.z - OriginPos.z, 2));//レイが当たった場所との距離

			if (nCntRayCollosionObj == 0)
			{//最初にレイが当たったオブジェクトをカウントし、一番近い距離とする
				nCntRayCollosionObj++;
				fTotalBestNearLength = fLength;
			}
			else
			{//2つめ以降に当たったオブジェクトの場合、一番近い距離と比較して更新する
				if (fLength < fTotalBestNearLength)
				{
					fTotalBestNearLength = fLength;
				}
			}
		}
		pObjBgModel = pNextBgMoodel;
	}

	//ブロックリスト
	while (pObjBlock != nullptr)
	{
		CObject* pNextBlock = pObjBlock->GetNextObject();
		CBlock* pBlock = static_cast<CBlock*>(pObjBlock);//ブロックにキャスト
		CObjectX::PosInfo BlockPosInfo = pBlock->GetPosInfo();//ブロックの位置情報
		CObjectX::SizeInfo BlockSizeInfo = pBlock->GetSizeInfo();//ブロックのサイズ情報
		const D3DXVECTOR3& BlockPos = BlockPosInfo.GetPos();//ブロックの位置
		const D3DXVECTOR3& BlockPosOld = BlockPosInfo.GetPosOld();//1f前の位置
		const D3DXVECTOR3& BlockVtxMax = BlockSizeInfo.GetVtxMax();//ブロックの最小頂点
		const D3DXVECTOR3& BlockVtxMin = BlockSizeInfo.GetVtxMin();//ブロックの最大頂
		if (CCollision::RayIntersectsAABBCollisionPos(OriginPos, Dir, BlockPos + BlockVtxMin, BlockPos + BlockVtxMax, RayCollisionPos))
		{
			float fLength = sqrtf(powf(RayCollisionPos.x - OriginPos.x, 2) +
				powf(RayCollisionPos.y - OriginPos.y, 2) +
				powf(RayCollisionPos.z - OriginPos.z, 2));//レイが当たった場所との距離

			if (nCntRayCollosionObj == 0)
			{//最初にレイが当たったオブジェクトをカウントし、一番近い距離とする
				nCntRayCollosionObj++;
				fTotalBestNearLength = fLength;
			}
			else
			{//2つめ以降に当たったオブジェクトの場合、一番近い距離と比較して更新する
				if (fLength < fTotalBestNearLength)
				{
					fTotalBestNearLength = fLength;
				}
			}
		}

		pObjBlock = pNextBlock;
	}


	if (fTotalBestNearLength > 50.0f)
	{//一番近い距離が50.0fより大きい場合、宙に浮いているとみなし、オブジェクトの上に位置を戻す処理を行う
		PosInfo.SetPos(PosOld);
		PosInfo.SetPosOld(Pos - Move);
		CManager::GetDebugText()->PrintDebugText("落ちるので戻る\n");
	}

}

//====================================================================================
//通常移動速度を編集
//====================================================================================
void CEnemy::EditNormalSpeed()
{
	CInputKeyboard * pInput = CManager::GetInputKeyboard();

	if (pInput->GetPress(DIK_LCONTROL) == true)
	{//Lコントロールキーを押しながら
		if (pInput->GetPress(DIK_LSHIFT) == true)
		{//シフトキーを押しながら・・・
			if (pInput->GetPress(DIK_5) == true)
			{
				m_fNormalSpeed -= 0.1f;
			}
		}
		else if (pInput->GetPress(DIK_5) == true)
		{
			m_fNormalSpeed += 0.1f;
		}
	}
	else
	{//Lコントロールキーを押していない
		if (pInput->GetPress(DIK_LSHIFT) == true)
		{//シフトキーを押しながら・・・
			if (pInput->GetTrigger(DIK_5) == true)
			{
				m_fNormalSpeed -= 0.1f;
			}
		}
		else if (pInput->GetTrigger(DIK_5) == true)
		{
			m_fNormalSpeed += 0.1f;
		}
	}

	CManager::GetDebugText()->PrintDebugText("通常移動速度変更（５）：%f\n",m_fNormalSpeed);
}
//============================================================================================================================================

//====================================================================================
//通常移動速度を編集
//====================================================================================
void CEnemy::EditSensingRange()
{
	CInputKeyboard* pInput = CManager::GetInputKeyboard();
	CObjectX::PosInfo& PosInfo = GetPosInfo();
	const D3DXVECTOR3& Pos = PosInfo.GetPos();
	if (pInput->GetPress(DIK_LCONTROL) == true)
	{//Lコントロールキーを押しながら
		if (pInput->GetPress(DIK_LSHIFT) == true)
		{//シフトキーを押しながら・・・
			if (pInput->GetPress(DIK_6) == true)
			{
				m_fSensingRange -= 10.0f;
			}
		}
		else if (pInput->GetPress(DIK_6) == true)
		{
			m_fSensingRange += 10.0f;
		}
	}
	else
	{//Lコントロールキーを押していない
		if (pInput->GetPress(DIK_LSHIFT) == true)
		{//シフトキーを押しながら・・・
			if (pInput->GetTrigger(DIK_6) == true)
			{
				m_fSensingRange -= 10.0f;
			}
		}
		else if (pInput->GetTrigger(DIK_6) == true)
		{
			m_fSensingRange += 10.0f;
		}
	}
	float fRadXZ = static_cast<float>(rand() % 628 - 314) * 0.01f;//XZ方向の角度(Yaw)を求める
	float fRadY = static_cast<float>(rand() % 628 - 314) * 0.01f; //Y方向の角度(Pitch)を求める
	D3DXVECTOR3 RandPos = { 0.0f,0.0f,0.0f };

	RandPos.x = m_fSensingRange * sinf(fRadXZ) * cosf(fRadY);//X方向の長さに対して、球の側面に出すために、Y方向の高さの比（高さを底面の比としてかける）
	RandPos.z = m_fSensingRange * cosf(fRadXZ) * cosf(fRadY);//Z方向の長さに対して、球の側面に出すために、Y方向の高さの比（高さを底面の比としてかける）
	RandPos.y = m_fSensingRange * sinf(fRadY);//Y方向の高さを求める

	//索敵範囲を表すためにパーティクルを索敵距離の位置にランダムで出す
	CParticle::Create(CParticle::TYPE00_NORMAL, 200, 30.0f, 30.0f,Pos + RandPos,
		D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), true);

	CManager::GetDebugText()->PrintDebugText("索敵範囲変更（６）：%f\n", m_fSensingRange);
}
//============================================================================================================================================

//********************************************************************************************************************************************
//射撃に弱い敵クラス
//********************************************************************************************************************************************

//====================================================================================
//静的メンバ変数宣言
//====================================================================================
const string CShotWeakEnemy::s_aSHOTWEAKENEMY_FILENAME[static_cast<int>(SHOTWEAKENEMYTYPE::MAX)] =
{
	"data\\MODEL\\Enemy\\ShotWeak\\noobSlime.x"
};
const float CShotWeakEnemy::s_fATTACKSTART_LENGTH = 450.0f;//攻撃開始距離
const float CShotWeakEnemy::s_fNORMAL_SENSIINGRANGE = 1050.0f;//通常感知射程
const int CShotWeakEnemy::s_nATTACK_COOLTIME = 60;//攻撃のクールタイム
//============================================================================================================================================

//====================================================================================
//コンストラクタ
//====================================================================================
CShotWeakEnemy::CShotWeakEnemy(int nPri, bool bUseintPri, CObject::TYPE type, CObject::OBJECTTYPE ObjType) : CEnemy(nPri,bUseintPri,type,ObjType),
m_ShotWeakEnemyType(SHOTWEAKENEMYTYPE::NORMAL),m_pMagicSword(nullptr), m_SaveAimPos(D3DXVECTOR3(0.0f,0.0f,0.0f))
{

}
//============================================================================================================================================

//====================================================================================
//デストラクタ
//====================================================================================
CShotWeakEnemy::~CShotWeakEnemy()
{

}
//============================================================================================================================================

//====================================================================================
//初期化処理
//====================================================================================
HRESULT CShotWeakEnemy::Init()
{
	CEnemy::Init();

	float fRatioRot = static_cast<float>(rand() % 200 - 100) / 100;
	bool bAim = rand() % 2;
	m_pMagicSword = CAttackEnemy::Create(CAttack::ATTACKTYPE::MAGICSWORD, CAttack::TARGETTYPE::PLAYER, CAttack::COLLISIONTYPE::RECTANGLE_XZ,
		false,false,1, 60, 200, GetPosInfo().GetPos(), D3DXVECTOR3(0.0f,D3DX_PI * fRatioRot, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	m_pMagicSword->SetUseDeath(false);

	//剣を回す方向を決める
	if (bAim == false)
	{
		m_pMagicSword->GetRotInfo().SetUseAddRot(true, D3DXVECTOR3(0.0f,0.1f, 0.0f));
	}
	else
	{
		m_pMagicSword->GetRotInfo().SetUseAddRot(true, D3DXVECTOR3(0.0f,-0.1f, 0.0f));

	}
	SetEnemyType(CEnemy::ENEMYTYPE::SHOTWEAK);//敵タイプを設定
	return S_OK;
}
//============================================================================================================================================

//====================================================================================
//終了処理
//====================================================================================
void CShotWeakEnemy::Uninit()
{
	CEnemy::Uninit();
}
//============================================================================================================================================

//====================================================================================
//更新処理
//====================================================================================
void CShotWeakEnemy::Update()
{
	CEnemy::Update();
	if (m_pMagicSword != nullptr)
	{
		m_pMagicSword->GetPosInfo().SetPos(GetPosInfo().GetPos());
	}
	SwordCollision();//剣の当たり判定を行う
}
//============================================================================================================================================

//====================================================================================
//描画処理
//====================================================================================
void CShotWeakEnemy::Draw()
{
	CEnemy::Draw();
}
//============================================================================================================================================

//====================================================================================
//死亡フラグ設定処理
//====================================================================================
void CShotWeakEnemy::SetDeath()
{
	CEnemy::SetDeath();
	if (GetUseDeath() == true)
	{
		if (m_pMagicSword != nullptr)
		{
			m_pMagicSword->SetUseDeath(true);
			m_pMagicSword->SetDeath();
		}
	}
}
//============================================================================================================================================

//====================================================================================
//生成処理
//====================================================================================
CShotWeakEnemy* CShotWeakEnemy::Create(SHOTWEAKENEMYTYPE Type, int nLife, int nPhaseNum, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 Scale)
{
	CShotWeakEnemy* pShotWeakEnemy = DBG_NEW CShotWeakEnemy;

	pShotWeakEnemy->Init();            //初期化処理
	int nIdx = CManager::GetObjectXInfo()->Regist(s_aSHOTWEAKENEMY_FILENAME[int(Type)]);
	pShotWeakEnemy->BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(nIdx),
		CManager::GetObjectXInfo()->GetBuffMat(nIdx),
		CManager::GetObjectXInfo()->GetdwNumMat(nIdx),
		CManager::GetObjectXInfo()->GetTexture(nIdx),
		CManager::GetObjectXInfo()->GetColorValue(nIdx));

	pShotWeakEnemy->SetPhaseNum(nPhaseNum);//フェーズ番号を設定する
	pShotWeakEnemy->m_ShotWeakEnemyType = Type;
	pShotWeakEnemy->GetLifeInfo().SetLife(nLife);    //体力
	pShotWeakEnemy->GetLifeInfo().SetMaxLife(nLife); //最大体力
	pShotWeakEnemy->GetLifeInfo().SetAutoSubLife(false);
	pShotWeakEnemy->GetPosInfo().SetPos(pos);       //位置
	pShotWeakEnemy->GetPosInfo().SetSupportPos(pos);//支点位置
	pShotWeakEnemy->GetRotInfo().SetRot(rot);       //向き
	pShotWeakEnemy->GetSizeInfo().SetScale(Scale);   //拡大率
	pShotWeakEnemy->GetSizeInfo().SetFormarScale(Scale);//元の拡大率を設定
	pShotWeakEnemy->SetSensingRange(1250.0f);//感知射程
	pShotWeakEnemy->SetNormalSpeed(10.0f);//通常移動速度
	pShotWeakEnemy->GetMoveInfo().SetUseInteria(false, GetNormalInertia());
	if (CScene::GetMode() == CScene::MODE_GAME)
	{//ゲームモードの時だけ剣を出す
		pShotWeakEnemy->m_pMagicSword->GetSizeInfo().SetScale(Scale * 0.5f);
	}
	pShotWeakEnemy->GetLifeInfo().SetAutoDeath(true);

	pShotWeakEnemy->SetSize();//モデルサイズを設定
	pShotWeakEnemy->SetManagerObjectType(CObject::MANAGEROBJECTTYPE::SHOTWEAKENEMY);           //マネージャーで呼び出す時の種類を設定
	return pShotWeakEnemy;
}
//============================================================================================================================================

//====================================================================================
//テキストファイルに情報を保存する
//====================================================================================
void CShotWeakEnemy::SaveInfoTxt(fstream& WritingFile)
{
	WritingFile << "SETSHOTWEAKENEMY" << endl;
	WritingFile << "SHOTWEAKENEMYTYPE = " << static_cast<int>(m_ShotWeakEnemyType);
	switch (m_ShotWeakEnemyType)
	{
	case SHOTWEAKENEMYTYPE::NORMAL:
		WritingFile << " # NORMAL" << endl;
		break;
	default:
		assert(false);
		break;
	}

	CEnemy::SaveInfoTxt(WritingFile);

	WritingFile << "END_SETSHOTWEAKENEMY" << endl;
}
//============================================================================================================================================

//====================================================================================
//テキストファイルから情報を読み込む
//====================================================================================
void CShotWeakEnemy::LoadInfoTxt(fstream& LoadingFile, list<CObject*>& listSaveManager, string& Buff)
{
	int nType = 0;                                     //種類
	int nShotWeakEnemyType = 0;                        //ショットに弱い敵タイプ   
	int nLife = 0;                                     //体力
	int nPhaseNum = 0;                                 //フェーズ番号
	int nDivisionNum = 0;                              //分裂回数
	D3DXVECTOR3 Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  //移動量
	D3DXVECTOR3 Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   //位置
	D3DXVECTOR3 Scale = D3DXVECTOR3(0.0f, 0.0f, 0.0f); //拡大率
	D3DXVECTOR3 Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   //向き
	SHOTWEAKENEMYTYPE ShotWeakEnemyType = {};          //背景モデルの種類
	ENEMYTYPE EnemyType = {};

	vector<CAIModel*> VecMoveAi = {};
	vector<MoveAiInfo> VecMoveAiInfo = {};

	int nCntMoveAi = 0;
	D3DXVECTOR3 MoveAiPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 MoveAiRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 MoveAiScale = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	float fNormalSpeed = 0.0f; //通常速度
	float fSensingRange = 0.0f;//索敵距離

	while (Buff != "END_SETSHOTWEAKENEMY")
	{
		LoadingFile >> Buff;//単語を読み込む
		if (Buff == "#")
		{
			getline(LoadingFile, Buff);
		}
		else if (Buff == "SHOTWEAKENEMYTYPE")
		{
			LoadingFile >> Buff;//イコール
			LoadingFile >> nShotWeakEnemyType;
		}
		else if (Buff == "ENEMYTYPE")
		{
			LoadingFile >> Buff;//イコール
			LoadingFile >> nType;
		}
		else if (Buff == "LIFE")
		{
			LoadingFile >> Buff;//イコール
			LoadingFile >> nLife;
		}
		else if (Buff == "POS")
		{
			LoadingFile >> Buff;//イコール
			LoadingFile >> Pos.x;      //位置X
			LoadingFile >> Pos.y;      //位置Y
			LoadingFile >> Pos.z;      //位置Z
		}
		else if (Buff == "ROT")
		{
			LoadingFile >> Buff;//イコール
			LoadingFile >> Rot.x;      //位置X
			LoadingFile >> Rot.y;      //位置Y
			LoadingFile >> Rot.z;      //位置Z
		}
		else if (Buff == "SCALE")
		{
			LoadingFile >> Buff;//イコール
			LoadingFile >> Scale.x;      //拡大率X
			LoadingFile >> Scale.y;      //拡大率Y
			LoadingFile >> Scale.z;      //拡大率Z
		}
		else if (Buff == "PHASENUM")
		{
			LoadingFile >> Buff;//イコール
			LoadingFile >> nPhaseNum;//フェーズ番号
		}
		else if (Buff == "NORMALSPEED")
		{
			LoadingFile >> Buff;//イコール
			LoadingFile >> fNormalSpeed;//通常速度
		}
		else if (Buff == "SENSINGRANGE")
		{
			LoadingFile >> Buff;//イコール
			LoadingFile >> fSensingRange;
		}
		else if (Buff == "SETMOVEAI")
		{
			while (1)
			{
				LoadingFile >> Buff;
				if (Buff == "SETNUM")
				{
					LoadingFile >> Buff;//イコール
					LoadingFile >> nCntMoveAi;//番号
					while (1)
					{
						LoadingFile >> Buff;

						if (Buff == "POS")
						{
							LoadingFile >> Buff;//イコール
							LoadingFile >> MoveAiPos.x;      //位置X
							LoadingFile >> MoveAiPos.y;      //位置Y
							LoadingFile >> MoveAiPos.z;      //位置Z
						}
						else if (Buff == "ROT")
						{
							LoadingFile >> Buff;//イコール
							LoadingFile >> MoveAiRot.x;      //位置X
							LoadingFile >> MoveAiRot.y;      //位置Y
							LoadingFile >> MoveAiRot.z;      //位置Z
						}
						else if (Buff == "SCALE")
						{
							LoadingFile >> Buff;//イコール
							LoadingFile >> MoveAiScale.x;      //拡大率X
							LoadingFile >> MoveAiScale.y;      //拡大率Y
							LoadingFile >> MoveAiScale.z;      //拡大率Z
						}
						else if (Buff == "END_SETNUM")
						{
							if (CScene::GetMode() == CScene::MODE_EDIT)
							{
								CAIModel* pAiModel = CAIModel::Create(CAIModel::AIMODELTYPE::MOVEPOINT, MoveAiPos, MoveAiRot, MoveAiScale, nullptr);
								pAiModel->GetDrawInfo().SetUseDraw(false);
								pAiModel->GetDrawInfo().SetUseShadow(false);
								VecMoveAi.push_back(pAiModel);
							}
							else if (CScene::GetMode() == CScene::MODE_GAME)
							{
								MoveAiInfo Info = {};
								Info.Pos = MoveAiPos;
								Info.Rot = MoveAiRot;
								Info.Scale = MoveAiScale;
								VecMoveAiInfo.push_back(Info);
							}
							break;
						}
					}
				}
				else if (Buff == "END_SETMOVEAI")
				{
					break;
				}
			}
		}
	}
	ShotWeakEnemyType = static_cast<SHOTWEAKENEMYTYPE>(nShotWeakEnemyType);
	EnemyType = static_cast<ENEMYTYPE>(nType);
	if (CScene::GetMode() == CScene::MODE_EDIT)
	{
		CShotWeakEnemy* pShotWeakEnemy = CShotWeakEnemy::Create(ShotWeakEnemyType,nLife,nPhaseNum,Pos,Rot,Scale);
		pShotWeakEnemy->SetVecMoveAiInfo(VecMoveAi);
		pShotWeakEnemy->SetNormalSpeed(fNormalSpeed);
		pShotWeakEnemy->SetSensingRange(fSensingRange);
		listSaveManager.push_back(pShotWeakEnemy);      //vectorに情報を保存する

	}
	else if (CScene::GetMode() == CScene::MODE_GAME)
	{
		CGame::GetPhaseManager()->PushPhaseInfo(Pos, Rot, Scale, nLife, static_cast<int>(EnemyType), nShotWeakEnemyType, nPhaseNum,fNormalSpeed,fSensingRange,0,VecMoveAiInfo);
	}

}
//============================================================================================================================================

//====================================================================================
//ステージマネージャーのオブジェクトをチェンジする
//====================================================================================
CObject* CShotWeakEnemy::ManagerChengeObject(bool bAim)
{
	int nNewType = int(m_ShotWeakEnemyType);
	SHOTWEAKENEMYTYPE NewType = {};
	//=======================================
	//種類を変える
	//=======================================
	if (bAim == true)
	{
		nNewType++;
	}
	else
	{
		nNewType--;
	}
	if (nNewType >= static_cast<int>(SHOTWEAKENEMYTYPE::MAX))
	{
		nNewType = 0;
	}
	if (nNewType < 0)
	{
		nNewType = static_cast<int>(SHOTWEAKENEMYTYPE::MAX) - 1;
	}
	//======================================================================================

	//=======================================
	//設定する
	//=======================================
	NewType = SHOTWEAKENEMYTYPE(nNewType);
	//======================================================================================

	//=======================================
	//死亡フラグを設定する
	//=======================================
	SetUseDeath(true);
	SetDeath();
	//======================================================================================

	return CShotWeakEnemy::Create(NewType, GetLifeInfo().GetMaxLife(), GetPhaseNum(), GetPosInfo().GetPos(), GetRotInfo().GetRot(), GetSizeInfo().GetScale());//生成したオブジェクトを返す
}
//============================================================================================================================================

//====================================================================================
//ステージマネージャーに現在のオブジェクトを保存する
//====================================================================================
CObject* CShotWeakEnemy::ManagerSaveObject()
{
	auto& Vec = GetVecAiModelInfo();
	auto Vec2 = move(Vec);
	CShotWeakEnemy* pShotWeakEnemy = CShotWeakEnemy::Create(m_ShotWeakEnemyType, GetLifeInfo().GetMaxLife(), GetPhaseNum(), GetPosInfo().GetPos(), GetRotInfo().GetRot(), GetSizeInfo().GetScale());//生成したオブジェクトを返す
	pShotWeakEnemy->SetVecMoveAiInfo(Vec2);
	pShotWeakEnemy->SetNormalSpeed(GetNormalSpeed());//現在の敵の通常速度を保存する
	pShotWeakEnemy->SetSensingRange(GetSensingRange());//現在の敵の索敵範囲を保存する
	return pShotWeakEnemy;
}
//============================================================================================================================================

//====================================================================================
//バトル移動処理
//====================================================================================
void CShotWeakEnemy::BattleMoveProcess()
{
	CEnemy::ChasePlayer();//プレイヤーを追いかける処理
	const D3DXVECTOR3& Move = GetMoveInfo().GetMove();//移動量を取得
	CManager::GetDebugText()->PrintDebugText("射撃に弱い敵の移動量：%f %f %f\n", Move.x, Move.y, Move.z);

	RayCollisionJumpOverOnHit();//レイが当たった時にジャンプさせるための処理
}
//============================================================================================================================================

//====================================================================================
//攻撃処理
//====================================================================================
void CShotWeakEnemy::AttackProcess()
{
	float fLength = CCalculation::CalculationLength(GetPosInfo().GetPos(), CGame::GetPlayer()->GetPosInfo().GetPos());
	float fLengthY = CGame::GetPlayer()->GetPosInfo().GetPos().y - GetPosInfo().GetPos().y;
	const bool& bAction = GetAction();
	const int& nPatternTime = GetPatternTime();
	const int& nPattern = GetPattern();

	if (fLength < s_fATTACKSTART_LENGTH && bAction == false && fLengthY < 400.0f && GetAttackCoolTime() > s_nATTACK_COOLTIME)
	{//攻撃が開始されていなければ（クールタイムごとに攻撃を行う。ターン性バトルみたいな動きになる）
		ChengeMove(DBG_NEW CEnemyMove_None());//AI移動と攻撃処理を入れ替える
		SetAction(true);
	}

	if (bAction == true)
	{
		switch (nPattern)
		{
		case 0://止まって突進方向を決める
			GetMoveInfo().SetMove(D3DXVECTOR3(0.0f, GetMoveInfo().GetMove().y, 0.0f));
			if (nPatternTime == 15)
			{
				SetPattern(nPattern + 1);
				SetPatternTime(0);
				m_SaveAimPos = CGame::GetPlayer()->GetPosInfo().GetPos();
			}
			break;
		case 1://チャージする
			CParticle::SummonChargeParticle(CParticle::TYPE::TYPE00_NORMAL, 1, 45, 5.0f, 20.0f, 20.0f, GetSizeInfo().GetSize().x, 100, 10, false,
				GetPosInfo().GetSenterPos(), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), true);

			if (nPatternTime == 45)
			{//狙った方向に突進する
				GetMoveInfo().SetUseInteria(true, 0.05f);

				GetMoveInfo().SetMove(CCalculation::Calculation3DVec(GetPosInfo().GetPos(),m_SaveAimPos, 30.0f));
				SetPattern(nPattern + 1);
				SetPatternTime(0);
			}
			break;
		case 2:
			if (nPatternTime == 60)
			{//攻撃パターンを終了する
				GetMoveInfo().SetUseInteria(false, GetNormalInertia());//慣性をオフに戻す
			    EndAttackPattern();//攻撃パターンを終了する
				ChengeMove(DBG_NEW CEnemyMove_AI());//AI移動処理に変える
			}
			break;
		default:
			break;
		}
		SetPatternTime(nPatternTime + 1);
	}
}
//============================================================================================================================================

//====================================================================================
//剣の当たり判定を行う
//====================================================================================
void CShotWeakEnemy::SwordCollision()
{
	if (m_pMagicSword != nullptr && CGame::GetPlayer() != nullptr)
	{

		if (CCollision::RectAngleCollisionXZ(m_pMagicSword, CGame::GetPlayer()))
		{
			D3DXVECTOR3 Aim = D3DXVECTOR3(CGame::GetPlayer()->GetPosInfo().GetPos().x, 0.0f, CGame::GetPlayer()->GetPosInfo().GetPos().z) - D3DXVECTOR3(GetPosInfo().GetPos().x, 0.0f, GetPosInfo().GetPos().z);

			CGame::GetPlayer()->SetDamage(5, 45);

			D3DXVec3Normalize(&Aim, &Aim);

			CGame::GetPlayer()->ChengeAbnormalState(DBG_NEW CPlayerAbnormalState_KnockBack(CGame::GetPlayer(), Aim * 100.0f, 0.1f));
		}
	}
}
//============================================================================================================================================

//====================================================================================
//倒されたときの演出
//====================================================================================
void CShotWeakEnemy::DefeatStaging()
{
	if (GetLifeInfo().GetLife() < 1)
	{
		for (int nCnt = 0; nCnt < 60; nCnt++)
		{
			CParticle* pParticle = CParticle::Create(CParticle::TYPE::TYPE00_NORMAL, 120, 80.0f, 80.0f, GetPosInfo().GetPos(), CCalculation::Rand3DVec(200, 10),
				D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), true);
			pParticle->SetUseAddSpeed(true, true, 0.9f);
		}
	}
}
//============================================================================================================================================

//====================================================================================
//レイがオブジェクトに当たった時にジャンプさせるための処理
//====================================================================================
void CShotWeakEnemy::RayCollisionJumpOverOnHit()
{
	//=======================
    //敵の変数
    //=======================
	CObjectX::PosInfo& PosInfo = GetPosInfo();//位置情報を取得
	CObjectX::SizeInfo& SizeInfo = GetSizeInfo();//サイズ情報を取得
	CObjectX::MoveInfo& MoveInfo = GetMoveInfo();//移動情報を取得
	CObjectX::RotInfo& RotInfo = GetRotInfo();//向き情報を取得

	const D3DXVECTOR3& Pos = PosInfo.GetPos();//位置
	const D3DXVECTOR3& SenterPos = PosInfo.GetSenterPos();//中心位置
	const D3DXVECTOR3& Rot = RotInfo.GetRot();//向き
	const D3DXVECTOR3& VtxMax = SizeInfo.GetVtxMax();//最大頂点
	const D3DXVECTOR3& VtxMin = SizeInfo.GetVtxMin();//最小頂点
	const D3DXVECTOR3& Size = SizeInfo.GetSize();//サイズ
	D3DXVECTOR3 RayOrigin = SenterPos + D3DXVECTOR3(sinf(Rot.y) * (Size.x / 2), 0.0f, cosf(Rot.y) * (Size.x / 2));//レイの視点を決める

	D3DXVECTOR3 RayDir = RayOrigin - SenterPos;
	D3DXVec3Normalize(&RayDir, &RayDir);
	D3DXVECTOR3 RayCollisionPos = { 0.0f,0.0f,0.0f };//レイが当たったところ格納用
	float fLength = 0.0f;//距離格納用
	//=====================================================================================

	for (int nCntPri = 0; nCntPri < CObject::m_nMAXPRIORITY; nCntPri++)
	{
		if (nCntPri == static_cast<int>(CObject::TYPE::BLOCK) || nCntPri == static_cast<int>(CObject::TYPE::BGMODEL))
		{
			CObject* pObj = CObject::GetTopObject(nCntPri);//先頭アドレスを取得
			while (pObj != nullptr)
			{
				CObject* pNext = pObj->GetNextObject();

				CObjectX* pObjX = static_cast<CObjectX*>(pObj);

				//=======================
				//比較対象の変数
				//=======================
				CObjectX::PosInfo& ComPosInfo = pObjX->GetPosInfo();//位置情報を取得
				CObjectX::RotInfo& ComRotInfo = pObjX->GetRotInfo();//向き情報を取得
				CObjectX::SizeInfo& ComSizeInfo = pObjX->GetSizeInfo();//サイズ情報を取得

				const D3DXVECTOR3& ComPos = ComPosInfo.GetPos();//位置
				const D3DXVECTOR3& ComRot = ComRotInfo.GetRot();//向き
				const D3DXVECTOR3& ComVtxMax = ComSizeInfo.GetVtxMax();//最大頂点
				const D3DXVECTOR3& ComVtxMin = ComSizeInfo.GetVtxMin();//最小頂点
				const D3DXVECTOR3& ComSize = ComSizeInfo.GetSize();//サイズ
				//=====================================================================================

				if (CCollision::RayIntersectsAABBCollisionPos(RayOrigin, RayDir, ComPos + ComVtxMin, ComPos + ComVtxMax, RayCollisionPos))
				{
					fLength = CCalculation::CalculationLength(RayOrigin, RayCollisionPos);//レイの支店とレイが当たった位置を出す
					if (fLength < 100.0f)
					{
						float fLengthY = (ComPos.y + ComVtxMax.y) - Pos.y;//Y軸の距離を取り、目標のジャンプ距離を求める

						if (GetJumpCoolTime() > 200)
						{
							ChengeMove(DBG_NEW CEnemyMove_OverJumpObj(this,fLengthY));//移動状態を「壁を超える」にする
						}
						//他のオブジェクトの比較はもういらないのでreturnする
						return;
					}
				}

				pObj = pNext;
			}
		}
	}
}
//============================================================================================================================================

//********************************************************************************************************************************************
//ダイブに弱い敵クラス
//********************************************************************************************************************************************

//====================================================================================
//静的メンバ変数宣言
//====================================================================================
const string CDiveWeakEnemy::s_aDIVEWEAKENEMY_FILENAME[static_cast<int>(CDiveWeakEnemy::DIVEWEAKENEMYTYPE::MAX)] =
{
	"data\\MODEL\\Enemy\\DiveWeak\\angrySlime.x"
};
const int CDiveWeakEnemy::s_nATTACK_FREQUENCY = 105;//攻撃頻度
const float CDiveWeakEnemy::s_fSENSINGRANGE = 2000.0f;
const float CDiveWeakEnemy::s_fNORMAL_SPEED = 3.0f;
//====================================================================================
//コンストラクタ
//====================================================================================
CDiveWeakEnemy::CDiveWeakEnemy(int nPri, bool bUseintPri, CObject::TYPE type, CObject::OBJECTTYPE ObjType) : CEnemy(nPri, bUseintPri, type, ObjType),
m_DiveWeakEnemyType(DIVEWEAKENEMYTYPE::NORMAL),m_nDivisionNum(0)
{

}
//============================================================================================================================================

//====================================================================================
//デストラクタ
//====================================================================================
CDiveWeakEnemy::~CDiveWeakEnemy()
{

}
//============================================================================================================================================

//====================================================================================
//初期化処理
//====================================================================================
HRESULT CDiveWeakEnemy::Init()
{
	CEnemy::Init();
	SetEnemyType(CEnemy::ENEMYTYPE::DIVEWEAK);//敵タイプを設定
	return S_OK;
}
//============================================================================================================================================

//====================================================================================
//終了処理
//====================================================================================
void CDiveWeakEnemy::Uninit()
{
	CEnemy::Uninit();
}
//============================================================================================================================================

//====================================================================================
//更新処理
//====================================================================================
void CDiveWeakEnemy::Update()
{
	CEnemy::Update();
}
//============================================================================================================================================

//====================================================================================
//描画処理
//====================================================================================
void CDiveWeakEnemy::Draw()
{
	CEnemy::Draw();
}
//============================================================================================================================================

//====================================================================================
//死亡フラグ設定処理
//====================================================================================
void CDiveWeakEnemy::SetDeath()
{
	if (m_nDivisionNum > 0 && GetDefeatAttackType() == CAttack::ATTACKTYPE::BULLET && GetLifeInfo().GetLife() < 1 && m_bStartDeath == false)
	{ 
		m_bStartDeath = true;
		float fPosX = static_cast<float>(rand() % 30 - 15);
		float fPosZ = static_cast<float>(rand() % 30 - 15);
		m_nDivisionNum--;
		if (m_nDivisionNum == 1)
		{//残り分裂回数が１の場合、だいぶ敵が増えてしまっているので、何もしない敵に分裂させる
			CIdleEnemy* pIdleEnemy = CIdleEnemy::Create(CIdleEnemy::IDLEENEMYTYPE::NORMAL, GetLifeInfo().GetMaxLife() / 2, 0, GetPosInfo().GetPos() + D3DXVECTOR3(fPosZ, 100.0f, fPosX), GetRotInfo().GetRot(), GetSizeInfo().GetScale() / 2);
			pIdleEnemy->SetSensingRange(9999.0f);
			pIdleEnemy = CIdleEnemy::Create(CIdleEnemy::IDLEENEMYTYPE::NORMAL, GetLifeInfo().GetMaxLife() / 2, 0, GetPosInfo().GetPos() + D3DXVECTOR3(fPosZ, 100.0f, fPosX), GetRotInfo().GetRot(), GetSizeInfo().GetScale() / 2);
			pIdleEnemy->SetSensingRange(9999.0f);
		}
		else
		{//残り分裂回数がまだ多いので、そんなに分裂していないとみなし、ダイブに弱い敵に分裂させる
			CDiveWeakEnemy* pDiveWeakEnemy = CDiveWeakEnemy::Create(DIVEWEAKENEMYTYPE::NORMAL, GetLifeInfo().GetMaxLife() / 2, 0, GetPosInfo().GetPos() + D3DXVECTOR3(fPosX, 100.0f, fPosZ), GetRotInfo().GetRot(), GetSizeInfo().GetScale() / 2, m_nDivisionNum);
			pDiveWeakEnemy->SetSensingRange(9999.0f);
			pDiveWeakEnemy->SetDivisionNum(m_nDivisionNum);
			pDiveWeakEnemy = CDiveWeakEnemy::Create(DIVEWEAKENEMYTYPE::NORMAL, GetLifeInfo().GetMaxLife() / 2, 0, GetPosInfo().GetPos() + D3DXVECTOR3(fPosZ, 100.0f, fPosX), GetRotInfo().GetRot(), GetSizeInfo().GetScale() / 2, m_nDivisionNum);
			pDiveWeakEnemy->SetSensingRange(9999.0f);
			pDiveWeakEnemy->SetDivisionNum(m_nDivisionNum);
			CParticle::SummonParticle(CParticle::TYPE::TYPE00_NORMAL, 30, 50, 30.0f, 30.0f, 100, 10, true, GetPosInfo().GetPos(), D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), true);
		}
	}

	CEnemy::SetDeath();
}
//============================================================================================================================================

//====================================================================================
//生成処理
//====================================================================================
CDiveWeakEnemy* CDiveWeakEnemy::Create(DIVEWEAKENEMYTYPE Type, int nLife, int nPhaseNum, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 Scale, int nDivisionNum)
{
	CDiveWeakEnemy* pDiveWeakEnemy = DBG_NEW CDiveWeakEnemy;

	pDiveWeakEnemy->Init();            //初期化処理
	int nIdx = CManager::GetObjectXInfo()->Regist(s_aDIVEWEAKENEMY_FILENAME[int(Type)]);
	pDiveWeakEnemy->BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(nIdx),
		CManager::GetObjectXInfo()->GetBuffMat(nIdx),
		CManager::GetObjectXInfo()->GetdwNumMat(nIdx),
		CManager::GetObjectXInfo()->GetTexture(nIdx),
		CManager::GetObjectXInfo()->GetColorValue(nIdx));

	pDiveWeakEnemy->SetPhaseNum(nPhaseNum);//フェーズ番号を設定する
	pDiveWeakEnemy->m_DiveWeakEnemyType = Type;
	pDiveWeakEnemy->GetLifeInfo().SetLife(nLife);    //体力
	pDiveWeakEnemy->GetLifeInfo().SetMaxLife(nLife); //最大体力
	pDiveWeakEnemy->GetPosInfo().SetPos(pos);       //位置
	pDiveWeakEnemy->GetPosInfo().SetSupportPos(pos);//支点位置
	pDiveWeakEnemy->GetRotInfo().SetRot(rot);       //向き
	pDiveWeakEnemy->GetSizeInfo().SetScale(Scale);   //拡大率
	pDiveWeakEnemy->GetSizeInfo().SetFormarScale(Scale);//元の拡大率を設定
	pDiveWeakEnemy->SetSensingRange(550.0f);//感知射程
	pDiveWeakEnemy->SetNormalSpeed(s_fNORMAL_SPEED);//通常移動速度
	pDiveWeakEnemy->GetMoveInfo().SetUseInteria(false, GetNormalInertia());
	pDiveWeakEnemy->SetCntTime(rand() % 100 + 1);
	pDiveWeakEnemy->SetDivisionNum(3);
	pDiveWeakEnemy->GetLifeInfo().SetAutoDeath(true);

	pDiveWeakEnemy->SetSize();//モデルサイズを設定
	pDiveWeakEnemy->GetLifeInfo().SetAutoSubLife(false);//自動的に体力を減らすかどうか
	pDiveWeakEnemy->SetManagerObjectType(CObject::MANAGEROBJECTTYPE::DIVEWEAKENEMY);           //マネージャーで呼び出す時の種類を設定
	return pDiveWeakEnemy;
}
//============================================================================================================================================

//====================================================================================
//テキストファイルに情報を保存する
//====================================================================================
void CDiveWeakEnemy::SaveInfoTxt(fstream& WritingFile)
{
	WritingFile << "SETDIVEWEAKENEMY" << endl;
	WritingFile << "DIVEWEAKENEMYTYPE = " << static_cast<int>(m_DiveWeakEnemyType);
	switch (m_DiveWeakEnemyType)
	{
	case DIVEWEAKENEMYTYPE::NORMAL:
		WritingFile << " # NORMAL" << endl;
		break;
	default:
		assert(false);
		break;
	}

	WritingFile << "DIVISIONNUM = " << m_nDivisionNum << endl;

	CEnemy::SaveInfoTxt(WritingFile);

	WritingFile << "END_SETDIVEWEAKENEMY" << endl;
}
//============================================================================================================================================

//====================================================================================
//テキストファイルから情報を読み込む
//====================================================================================
void CDiveWeakEnemy::LoadInfoTxt(fstream& LoadingFile, list<CObject*>& listSaveManager, string& Buff)
{
	int nType = 0;                                     //種類
	int nDiveWeakEnemyType = 0;                        //ダイブに弱い敵タイプ   
	int nLife = 0;                                     //体力
	int nPhaseNum = 0;                                 //フェーズ番号
	int nDivisionNum = 0;                              //分裂回数
 	D3DXVECTOR3 Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  //移動量
	D3DXVECTOR3 Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   //位置
	D3DXVECTOR3 Scale = D3DXVECTOR3(0.0f, 0.0f, 0.0f); //拡大率
	D3DXVECTOR3 Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   //向き
    DIVEWEAKENEMYTYPE DiveWeakEnemyType = {};          //背景モデルの種類
	ENEMYTYPE EnemyType = {};

	vector<CAIModel*> VecMoveAi = {};
	vector<MoveAiInfo> VecMoveAiInfo = {};

	int nCntMoveAi = 0;
	D3DXVECTOR3 MoveAiPos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	D3DXVECTOR3 MoveAiRot = D3DXVECTOR3(0.0f,0.0f,0.0f);
	D3DXVECTOR3 MoveAiScale = D3DXVECTOR3(0.0f,0.0f,0.0f);

	float fNormalSpeed = 0.0f; //通常速度
	float fSensingRange = 0.0f;//索敵距離
	while (Buff != "END_SETDIVEWEAKENEMY")
	{
		LoadingFile >> Buff;//単語を読み込む
		if (Buff == "#")
		{
			getline(LoadingFile, Buff);
		}
		else if (Buff == "DIVEWEAKENEMYTYPE")
		{
			LoadingFile >> Buff;//イコール
			LoadingFile >> nDiveWeakEnemyType;
		}
		else if (Buff == "ENEMYTYPE")
		{
			LoadingFile >> Buff;//イコール
			LoadingFile >> nType;
		}
		else if (Buff == "LIFE")
		{
			LoadingFile >> Buff;//イコール
			LoadingFile >> nLife;
		}
		else if (Buff == "POS")
		{
			LoadingFile >> Buff;//イコール
			LoadingFile >> Pos.x;      //位置X
			LoadingFile >> Pos.y;      //位置Y
			LoadingFile >> Pos.z;      //位置Z
		}
		else if (Buff == "ROT")
		{
			LoadingFile >> Buff;//イコール
			LoadingFile >> Rot.x;      //位置X
			LoadingFile >> Rot.y;      //位置Y
			LoadingFile >> Rot.z;      //位置Z
		}
		else if (Buff == "SCALE")
		{
			LoadingFile >> Buff;//イコール
			LoadingFile >> Scale.x;      //拡大率X
			LoadingFile >> Scale.y;      //拡大率Y
			LoadingFile >> Scale.z;      //拡大率Z
		}
		else if (Buff == "PHASENUM")
		{
			LoadingFile >> Buff;      //イコール
			LoadingFile >> nPhaseNum; //フェーズ番号
		}
		else if (Buff == "NORMALSPEED")
		{
			LoadingFile >> Buff;//イコール
			LoadingFile >> fNormalSpeed;//通常速度
		}
		else if (Buff == "SENSINGRANGE")
		{
			LoadingFile >> Buff;//イコール
			LoadingFile >> fSensingRange;
		}
		else if (Buff == "DIVISIONNUM")
		{
			LoadingFile >> Buff;//イコール
			LoadingFile >> nDivisionNum;//分裂回数
		}
		else if (Buff == "SETMOVEAI")
		{
			while (1)
			{
				LoadingFile >> Buff;
				if (Buff == "SETNUM")
				{
					LoadingFile >> Buff;//イコール
					LoadingFile >> nCntMoveAi;//番号
					while (1)
					{
						LoadingFile >> Buff;

						if (Buff == "POS")
						{
							LoadingFile >> Buff;//イコール
							LoadingFile >> MoveAiPos.x;      //位置X
							LoadingFile >> MoveAiPos.y;      //位置Y
							LoadingFile >> MoveAiPos.z;      //位置Z
						}
						else if (Buff == "ROT")
						{
							LoadingFile >> Buff;//イコール
							LoadingFile >> MoveAiRot.x;      //位置X
							LoadingFile >> MoveAiRot.y;      //位置Y
							LoadingFile >> MoveAiRot.z;      //位置Z
						}
						else if (Buff == "SCALE")
						{
							LoadingFile >> Buff;//イコール
							LoadingFile >> MoveAiScale.x;      //拡大率X
							LoadingFile >> MoveAiScale.y;      //拡大率Y
							LoadingFile >> MoveAiScale.z;      //拡大率Z
						}
						else if (Buff == "END_SETNUM")
						{
							if (CScene::GetMode() == CScene::MODE_EDIT)
							{
								CAIModel* pAiModel = CAIModel::Create(CAIModel::AIMODELTYPE::MOVEPOINT, MoveAiPos, MoveAiRot, MoveAiScale, nullptr);
								pAiModel->GetDrawInfo().SetUseDraw(true);
								pAiModel->GetDrawInfo().SetUseShadow(true);
								VecMoveAi.push_back(pAiModel);
							}
							else if (CScene::GetMode() == CScene::MODE_GAME)
							{
								MoveAiInfo Info = {};
								Info.Pos = MoveAiPos;
								Info.Rot = MoveAiRot;
								Info.Scale = MoveAiScale;
								VecMoveAiInfo.push_back(Info);
							}
							break;
						}
					}
				}
				else if (Buff == "END_SETMOVEAI")
				{
					break;
				}
			}
		}
	}

	DiveWeakEnemyType = static_cast<DIVEWEAKENEMYTYPE>(nDiveWeakEnemyType);
	EnemyType = static_cast<ENEMYTYPE>(nType);
	if (CScene::GetMode() == CScene::MODE_EDIT)
	{
		CDiveWeakEnemy* pDiveWeakEnemy = CDiveWeakEnemy::Create(DiveWeakEnemyType, nLife, nPhaseNum, Pos, Rot, Scale,3);
		pDiveWeakEnemy->GetDrawInfo().SetUseDraw(true);
		pDiveWeakEnemy->GetDrawInfo().SetUseShadow(true);
		pDiveWeakEnemy->SetVecMoveAiInfo(VecMoveAi);
		pDiveWeakEnemy->SetNormalSpeed(fNormalSpeed);
		pDiveWeakEnemy->SetSensingRange(fSensingRange);
		pDiveWeakEnemy->GetMoveInfo().SetUseUpdatePos(true);
		listSaveManager.push_back(pDiveWeakEnemy);      //vectorに情報を保存する
	}
	else if (CScene::GetMode() == CScene::MODE_GAME)
	{
		CGame::GetPhaseManager()->PushPhaseInfo(Pos, Rot, Scale, nLife, static_cast<int>(EnemyType), nDiveWeakEnemyType, nPhaseNum,fNormalSpeed,fSensingRange,3,VecMoveAiInfo);
	}
}
//============================================================================================================================================

//====================================================================================
//ステージマネージャーのオブジェクトをチェンジする
//====================================================================================
CObject* CDiveWeakEnemy::ManagerChengeObject(bool bAim)
{
	int nNewType = int(m_DiveWeakEnemyType);
	DIVEWEAKENEMYTYPE NewType = {};
	//=======================================
	//種類を変える
	//=======================================
	if (bAim == true)
	{
		nNewType++;
	}
	else
	{
		nNewType--;
	}
	if (nNewType >= static_cast<int>(DIVEWEAKENEMYTYPE::MAX))
	{
		nNewType = 0;
	}
	if (nNewType < 0)
	{
		nNewType = static_cast<int>(DIVEWEAKENEMYTYPE::MAX) - 1;
	}
	//======================================================================================

	//=======================================
	//設定する
	//=======================================
	NewType = static_cast<DIVEWEAKENEMYTYPE>(nNewType);
	//======================================================================================

	//=======================================
	//死亡フラグを設定する
	//=======================================
	SetUseDeath(true);
	SetDeath();
	//======================================================================================

	return CDiveWeakEnemy::Create(NewType, GetLifeInfo().GetLife(),GetPhaseNum(), GetPosInfo().GetPos(), GetRotInfo().GetRot(), GetSizeInfo().GetScale(),GetDivisionNum());//生成したオブジェクトを返す
}
//============================================================================================================================================

//====================================================================================
//ステージマネージャーに現在のオブジェクトを保存する
//====================================================================================
CObject* CDiveWeakEnemy::ManagerSaveObject()
{
	auto& Vec = GetVecAiModelInfo();
	auto Vec2 = move(Vec);
	CDiveWeakEnemy * pDiveWeakEnemy = CDiveWeakEnemy::Create(m_DiveWeakEnemyType, GetLifeInfo().GetMaxLife(),GetPhaseNum(), GetPosInfo().GetPos(), GetRotInfo().GetRot(), GetSizeInfo().GetScale(),GetDivisionNum());//生成したオブジェクトを返す
	pDiveWeakEnemy->SetSensingRange(GetSensingRange());//現在の敵の索敵範囲を保存する
	pDiveWeakEnemy->SetNormalSpeed(GetNormalSpeed());//現在の敵の通常速度を保存する
	pDiveWeakEnemy->SetVecMoveAiInfo(Vec2);
	return pDiveWeakEnemy;//生成したオブジェクトを返す
}
//============================================================================================================================================

//====================================================================================
//ステージマネージャーから操作する
//====================================================================================
void CDiveWeakEnemy::ManagerChooseControlInfo()
{
	CInputKeyboard* pKeyboard = CManager::GetInputKeyboard();

	if (CManager::GetInputKeyboard()->GetPress(DIK_LCONTROL) == true)
	{//Lコントロールキーを押しながら
		if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
		{//シフトキーを押しながら・・・
			if (CManager::GetInputKeyboard()->GetTrigger(DIK_U) == true)
			{
				m_nDivisionNum -= 1;
			}
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_U) == true)
		{
			m_nDivisionNum += 1;
		}
	}
	else
	{//Lコントロールキーを押していない
		if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
		{//シフトキーを押しながら・・・
			if (CManager::GetInputKeyboard()->GetTrigger(DIK_U) == true)
			{
				m_nDivisionNum -= 1;
			}
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_U) == true)
		{
			m_nDivisionNum += 1;
		}
	}

	CManager::GetDebugText()->PrintDebugText("分裂回数(U)：%d\n", m_nDivisionNum);

	CEnemy::ManagerChooseControlInfo();
}
//============================================================================================================================================

//====================================================================================
//バトル移動処理
//====================================================================================
void CDiveWeakEnemy::BattleMoveProcess()
{
	CEnemy::BattleMoveProcess();
}
//============================================================================================================================================

//====================================================================================
//攻撃処理
//====================================================================================
void CDiveWeakEnemy::AttackProcess()
{
	float fLength = CCalculation::CalculationLength(GetPosInfo().GetPos(), CGame::GetPlayer()->GetPosInfo().GetPos());
	const bool& bAction = GetAction();
	if (fLength < GetSensingRange() && bAction == false)
	{//索敵範囲にいる場合、攻撃を開始する
		ChengeMove(DBG_NEW CEnemyMove_None());//攻撃処理中は、移動も入るので、通常移動処理はさせない。
		SetAction(true);//攻撃を開始
	}
	if (bAction == true)
	{
		//==============
		//使用変数
		//==============
		CObjectX::RotInfo& RotInfo = GetRotInfo();        //向き情報を取得
		CObjectX::PosInfo& PosInfo = GetPosInfo();        //位置情報を取得
		CObjectX::SizeInfo& SizeInfo = GetSizeInfo();     //サイズ情報を取得
		CObjectX::MoveInfo& MoveInfo = GetMoveInfo();     //移動情報を取得
		CPlayer* pPlayer = CGame::GetPlayer();            //プレイヤーのポインタを取得
		CObjectX::PosInfo& PlayerPosInfo = pPlayer->GetPosInfo();//プレイヤーの位置情報を取得
		const D3DXVECTOR3& Pos = PosInfo.GetPos();        //位置を取得
		const D3DXVECTOR3& Move = MoveInfo.GetMove();     //移動量を取得
		const D3DXVECTOR3& PlayerPos = PlayerPosInfo.Pos;//プレイヤーの位置を取得
		D3DXVECTOR3 Rot = RotInfo.GetRot();               //向き
		const float & fNormalSpeed = GetNormalSpeed();    //通常移動速度
		float fRotAim = atan2f(PlayerPos.x - Pos.x, PlayerPos.z - Pos.z);//Z方向を基準にプレイヤーへの角度（目的の角度）を計算する
		//========================================================================================================

		RotInfo.SetRot(D3DXVECTOR3(Rot.x, CCalculation::CalculationCollectionRot2D(Rot.y, fRotAim, 0.007f, false), Rot.z));//向きをプレイヤーへ超少しずつ合わせていく
		MoveInfo.SetMove(D3DXVECTOR3(sinf(fRotAim) * fNormalSpeed, Move.y, cosf(fRotAim) * fNormalSpeed));//プレイヤーに向かって移動させる

		if (GetCntTime() % s_nATTACK_FREQUENCY == 0)
		{//攻撃を発射

		    //==============
		    //使用変数
		    //==============
			const D3DXVECTOR3& VtxMax = SizeInfo.GetVtxMax(); //最大頂点を取得
			D3DXVECTOR3 Aim = { 0.0f,0.0f,0.0f };             //狙う方向
			D3DXVECTOR3 ShotPos = Pos + D3DXVECTOR3(0.0f, VtxMax.y, 0.0f);//発射位置
			D3DXVECTOR3 AddRot = { 0.0f,0.0f,0.0f };//加算する向き
			D3DXVECTOR2 YawPitch = CCalculation::VectorToYawPitch(ShotPos, PlayerPos);//目的地への角度(YawとPitch)を取得
			//========================================================================================================

			//Yawの補正
			if (YawPitch.y > Rot.y + 0.5f)
			{
				YawPitch.y = Rot.y + 0.5f;
			}
			else if (YawPitch.y < Rot.y - 0.5f)
			{
				YawPitch.y = Rot.y - 0.5f;
			}

		    //発射方向計算
		    Aim.x = sinf(YawPitch.y) * 20.0f;//Z方向を軸にしているので、sinはXとする
		    Aim.z = cosf(YawPitch.y) * 20.0f;//Z方向を軸にしているので。cosはZとする
		    
		    //Pitchの補正
		    if (YawPitch.x > 0.3f)
		    {
		    	YawPitch.x = 0.3f;
		    }
		    else if (YawPitch.x < -0.3f)
		    {
		    	YawPitch.x = -0.3f;
		    }
		    
		    Aim.y = sinf(YawPitch.x) * 20.0f;//軸が平面なので、sinはYとする
		    
		    CAttackEnemy* pAttackEnemy = CAttackEnemy::Create(CAttack::ATTACKTYPE::EXPLOSION, CAttack::TARGETTYPE::PLAYER, CAttack::COLLISIONTYPE::SQUARE, true, true,
		    	1, 60, 200, ShotPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), Aim, GetSizeInfo().GetScale() * 0.5f);
		    
		    pAttackEnemy->GetBoundInfo().SetActive(true, D3DXVECTOR3(0.0f, 10.0f, 0.0f), true, 0.5f);//バウンドさせる
		    pAttackEnemy->SetExtrusionCollisioin(true);//押し出し判定を行い、
		    pAttackEnemy->SetHitOtherThanLibing(false);//敵やプレイヤー以外との当たり判定は行わない
		}
	}
}
//============================================================================================================================================

//====================================================================================
//AI移動処理
//====================================================================================
void CDiveWeakEnemy::AIMoveProcess()
{
	if (CScene::GetMode() == CScene::MODE_GAME)
	{
		CEnemy::AIMoveProcess();
	}
}
//============================================================================================================================================

//====================================================================================
//倒されたときの演出
//====================================================================================
void CDiveWeakEnemy::DefeatStaging()
{
	if (GetLifeInfo().GetLife() < 1)
	{
		for (int nCnt = 0; nCnt < 60; nCnt++)
		{
			CParticle* pParticle = CParticle::Create(CParticle::TYPE::TYPE00_NORMAL, 120, 80.0f, 80.0f, GetPosInfo().GetPos(), CCalculation::Rand3DVec(200, 10),
				D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), true);
			pParticle->SetUseAddSpeed(true, true, 0.9f);
		}
	}
}
//============================================================================================================================================

//********************************************************************************************************************************************
//敵移動タイプクラス
//********************************************************************************************************************************************

//====================================================================================
//コンストラクタ
//====================================================================================
CEnemyMove::CEnemyMove()
{

}
//============================================================================================================================================

//====================================================================================
//デストラクタ
//====================================================================================
CEnemyMove::~CEnemyMove()
{

}
//============================================================================================================================================

//====================================================================================
//処理
//====================================================================================
void CEnemyMove::Process(CEnemy* pEnemy)
{

}
//============================================================================================================================================

//********************************************************************************************************************************************
//敵移動タイプ：AIクラス
//********************************************************************************************************************************************

//====================================================================================
//コンストラクタ
//====================================================================================
CEnemyMove_AI::CEnemyMove_AI()
{

}
//============================================================================================================================================

//====================================================================================
//デストラクタ
//====================================================================================
CEnemyMove_AI::~CEnemyMove_AI()
{

}
//============================================================================================================================================

//====================================================================================
//処理
//====================================================================================
void CEnemyMove_AI::Process(CEnemy* pEnemy)
{
	pEnemy->AIMoveProcess();//AI移動処理を行う
}
//============================================================================================================================================

//********************************************************************************************************************************************
//敵移動タイプ：バトルクラス
//********************************************************************************************************************************************

//====================================================================================
//コンストラクタ
//====================================================================================
CEnemyMove_Battle::CEnemyMove_Battle()
{

}
//============================================================================================================================================

//====================================================================================
//デストラクタ
//====================================================================================
CEnemyMove_Battle::~CEnemyMove_Battle()
{

}
//============================================================================================================================================

//====================================================================================
//処理
//====================================================================================
void CEnemyMove_Battle::Process(CEnemy* pEnemy)
{
	pEnemy->BattleMoveProcess();
}
//============================================================================================================================================

//********************************************************************************************************************************************
//敵移動タイプ：なしクラス
//********************************************************************************************************************************************

//====================================================================================
//コンストラクタ
//====================================================================================
CEnemyMove_None::CEnemyMove_None()
{

}
//============================================================================================================================================

//====================================================================================
//デストラクタ
//====================================================================================
CEnemyMove_None::~CEnemyMove_None()
{

}
//============================================================================================================================================

//====================================================================================
//処理
//====================================================================================
void CEnemyMove_None::Process(CEnemy* pEnemy)
{

}
//============================================================================================================================================

//************************************************************************************
//怯え移動クラス
//************************************************************************************

//====================================================================================
//コンストラクタ
//====================================================================================
CEnemyMove_Frightened::CEnemyMove_Frightened(CEnemy* pEnemy, D3DXVECTOR3 StopPos, int nStateTime) : m_StopPos(StopPos),m_nStateTime(nStateTime),m_pLockOn(nullptr)
{
	D3DXVECTOR3 ScreenPos = D3DXVECTOR3(0.0f, 0.0f,0.0f);
	ScreenPos = CCalculation::CalcWorldToScreenNoViewport(pEnemy->GetPosInfo().GetSenterPos(), *CManager::GetCamera()->GetMtxView(), *CManager::GetCamera()->GetMtxProjection(),
		SCREEN_WIDTH,SCREEN_HEIGHT);

	pEnemy->SetState(CEnemy::STATE::FRIGHTENDED);//怯え状態にする

	m_pLockOn = CUi::Create(CUi::UITYPE::TARGET_000, CObject2D::POLYGONTYPE::SENTERROLLING, 200.0f, 200.0f, 100, true, D3DXVECTOR3(ScreenPos.x, ScreenPos.y, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f));

	pEnemy->SetAction(false);
	pEnemy->SetPossibleAttack(false);//攻撃を不能にする

	m_pLockOn->SetPolygonRotSpeed(0.1f);
	m_pLockOn->SetUseAddScale(D3DXVECTOR2(-0.01f, -0.01f), true);
	m_pLockOn->SetUseDeath(false);
}
//============================================================================================================================================

//====================================================================================
//デストラクタ
//====================================================================================
CEnemyMove_Frightened::~CEnemyMove_Frightened()
{
	if (m_pLockOn != nullptr)
	{
		m_pLockOn->SetUseDeath(true);
		m_pLockOn->SetDeath();
		m_pLockOn = nullptr;
	}
}
//============================================================================================================================================

//====================================================================================
//処理
//====================================================================================
void CEnemyMove_Frightened::Process(CEnemy* pEnemy)
{
	float fX = static_cast<float>(rand() % 50 - 25);
	float fY = static_cast<float>(rand() % 50 - 25);
	float fZ = static_cast<float>(rand() % 50 - 25);
	pEnemy->GetPosInfo().SetPos(m_StopPos + D3DXVECTOR3(fX, fY, fZ));//震えさせる
	pEnemy->GetMoveInfo().SetMove(D3DXVECTOR3(0.0f,0.0f,0.0f));
	m_nStateTime--;
	pEnemy->EndAttackPattern();//怯え状態の時は行動不能にしたいので、攻撃パターンを終了させつづける

	if (m_pLockOn != nullptr)
	{
		D3DXVECTOR3 ScreenPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		ScreenPos = CCalculation::CalcWorldToScreenNoViewport(pEnemy->GetPosInfo().GetSenterPos(), *CManager::GetCamera()->GetMtxView(), *CManager::GetCamera()->GetMtxProjection(),
			SCREEN_WIDTH, SCREEN_HEIGHT);
		m_pLockOn->SetPos(ScreenPos);//怯え状態にした敵のスクリーン座標にUIを表示
		if (m_pLockOn->GetScale().x < 0.0f && m_pLockOn->GetScale().y < 0.0f)
		{
			m_pLockOn->SetUseDeath(true);
			m_pLockOn->SetDeath();
			m_pLockOn = nullptr;
		}
	}
	if (m_nStateTime < 1)
	{
		pEnemy->SetPossibleAttack(true);//攻撃を可能に戻す
		pEnemy->GetPosInfo().SetPos(m_StopPos);
		pEnemy->SetState(CEnemy::STATE::NORMAL);//状態異常を普通に戻す
		pEnemy->ChengeMove(DBG_NEW CEnemyMove_AI());//AI移動処理に戻す
	}
}
//============================================================================================================================================

//************************************************************************************
//壁回避移動クラス
//************************************************************************************

//====================================================================================
//コンストラクタ
//====================================================================================
CEnemyMove_DodgeWall::CEnemyMove_DodgeWall(CEnemy* pEnemy, D3DXVECTOR3 DodgeMove)
{
	m_DodgeMove = DodgeMove;
}
//============================================================================================================================================

//====================================================================================
//デストラクタ
//====================================================================================
CEnemyMove_DodgeWall::~CEnemyMove_DodgeWall()
{

}
//============================================================================================================================================

//====================================================================================
//処理
//====================================================================================
void CEnemyMove_DodgeWall::Process(CEnemy* pEnemy)
{
	pEnemy->GetMoveInfo().SetMove(m_DodgeMove);

	if (pEnemy->GetCollisionWall() == false)
	{
		pEnemy->ChengeMove(DBG_NEW CEnemyMove_Battle());
	}
}
//============================================================================================================================================

//********************************************************************************************************************************************
//敵移動タイプ：なしクラス
//********************************************************************************************************************************************

//====================================================================================
//静的メンバ宣言
//====================================================================================
const string CIdleEnemy::s_aIDLEENEMY_FILENAME[static_cast<int>(CIdleEnemy::IDLEENEMYTYPE::MAX)] =
{
	"data\\MODEL\\Enemy\\GreenSlime_000.x"
};
//============================================================================================================================================

//====================================================================================
//コンストラクタ
//====================================================================================
CIdleEnemy::CIdleEnemy(int nPri, bool bUseintPri, CObject::TYPE type, CObject::OBJECTTYPE ObjType)
{

}
//============================================================================================================================================

//====================================================================================
//デストラクタ
//====================================================================================
CIdleEnemy::~CIdleEnemy()
{

}
//============================================================================================================================================

//====================================================================================
//初期化処理
//====================================================================================
HRESULT CIdleEnemy::Init()
{
	CEnemy::Init();//初期化処理
	return S_OK;
}
//============================================================================================================================================

//====================================================================================
//終了処理
//====================================================================================
void CIdleEnemy::Uninit()
{
	CEnemy::Uninit();//終了処理
}
//============================================================================================================================================

//====================================================================================
//更新処理
//====================================================================================
void CIdleEnemy::Update()
{
	CEnemy::Update();//更新処理
}
//============================================================================================================================================

//====================================================================================
//描画処理
//====================================================================================
void CIdleEnemy::Draw()
{
	CEnemy::Draw();//描画処理
}
//============================================================================================================================================

//====================================================================================
//死亡フラグ設定処理
//====================================================================================
void CIdleEnemy::SetDeath()
{
	CEnemy::SetDeath();//死亡フラグ設定処理
}
//============================================================================================================================================

//====================================================================================
//生成処理
//===================================================================================
CIdleEnemy* CIdleEnemy::Create(IDLEENEMYTYPE Type, int nLife, int nPhaseNum, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 Scale)
{
	CIdleEnemy* pIdleEnemy = DBG_NEW CIdleEnemy();//何もしない敵の動的メモリを確保

	//初期化処理
	pIdleEnemy->Init();

	//モデル情報を登録、読み込み
	int nIdx = CManager::GetObjectXInfo()->Regist(s_aIDLEENEMY_FILENAME[int(Type)]);
	pIdleEnemy->BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(nIdx),
		CManager::GetObjectXInfo()->GetBuffMat(nIdx),
		CManager::GetObjectXInfo()->GetdwNumMat(nIdx),
		CManager::GetObjectXInfo()->GetTexture(nIdx),
		CManager::GetObjectXInfo()->GetColorValue(nIdx));

	//ステータスを設定
	pIdleEnemy->SetPhaseNum(nPhaseNum);               //フェーズ番号を設定する
	pIdleEnemy->SetEnemyType(CEnemy::ENEMYTYPE::IDLE);//敵の種類を設定する
	pIdleEnemy->m_IdleEnemyType = Type;               //何もしない敵のタイプを設定する
	pIdleEnemy->GetLifeInfo().SetLife(nLife);         //体力
	pIdleEnemy->GetLifeInfo().SetMaxLife(nLife);      //最大体力
	pIdleEnemy->GetPosInfo().SetPos(pos);             //位置
	pIdleEnemy->GetPosInfo().SetSupportPos(pos);      //支点位置
	pIdleEnemy->GetRotInfo().SetRot(rot);             //向き
	pIdleEnemy->GetSizeInfo().SetScale(Scale);        //拡大率
	pIdleEnemy->GetSizeInfo().SetFormarScale(Scale);  //元の拡大率を設定
	pIdleEnemy->SetSensingRange(550.0f);              //感知射程
	pIdleEnemy->SetNormalSpeed(3.0f);                //通常移動速度
	pIdleEnemy->GetMoveInfo().SetUseInteria(false, GetNormalInertia());//慣性を設定（通常値）
	pIdleEnemy->SetCntTime(rand() % 100 + 1);         //攻撃タイミングをずらす
	pIdleEnemy->GetLifeInfo().SetAutoDeath(true);     //体力が０になったときに死亡フラグを発動
	pIdleEnemy->GetLifeInfo().SetAutoSubLife(false);  //体力を減らし続けない
	pIdleEnemy->SetSize();                            //モデルサイズを設定
	pIdleEnemy->SetManagerObjectType(CObject::MANAGEROBJECTTYPE::IDLEENEMY);//ステージマネージャーで呼び出す時の種類を設定

	return pIdleEnemy;
}
//============================================================================================================================================

//====================================================================================
//情報書き出し処理
//===================================================================================
void CIdleEnemy::SaveInfoTxt(fstream& WritingFile)
{
	WritingFile << "SETIDLEENEMY" << endl;//何もしない敵の読み込み開始用

	//何もしない敵のタイプを書き出す
	WritingFile << "IDLEENEMYTYPE = " << static_cast<int>(m_IdleEnemyType);
	switch (m_IdleEnemyType)
	{
	case IDLEENEMYTYPE::NORMAL://普通
		WritingFile << " # NORMAL" << endl;
		break;
	default://想定外の値なのでアサートを発動
		assert(false);
		break;
	}

	CEnemy::SaveInfoTxt(WritingFile);//敵の共通の情報を書き出す

	WritingFile << "END_SETIDLEENEMY" << endl;//何もしない敵の読み込み終了用

}
//============================================================================================================================================

//====================================================================================
//情報ロード処理
//===================================================================================
void CIdleEnemy::LoadInfoTxt(fstream& LoadingFile, list<CObject*>& listSaveManager, string& Buff)
{
	int nType = 0;                                           //種類
	int nIdleEnemyType = 0;                                  //何もしない敵タイプ   
	int nLife = 0;                                           //体力
	int nPhaseNum = 0;                                       //フェーズ番号
	D3DXVECTOR3 Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);        //移動量
	D3DXVECTOR3 Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);         //位置
	D3DXVECTOR3 Scale = D3DXVECTOR3(0.0f, 0.0f, 0.0f);       //拡大率
	D3DXVECTOR3 Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);         //向き
	IDLEENEMYTYPE IdleEnemyType = IDLEENEMYTYPE::NORMAL;     //何もしない敵の種類
	ENEMYTYPE EnemyType = {};                                //敵の分類
														     
	vector<CAIModel*> VecMoveAi = {};                        //移動AIの動的配列
	vector<MoveAiInfo> VecMoveAiInfo = {};                   //移動AI情報の動的配列
														     
	int nCntMoveAi = 0;                                      //移動AIの数
	D3DXVECTOR3 MoveAiPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   //移動AIの位置
	D3DXVECTOR3 MoveAiRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   //移動AIの向き
	D3DXVECTOR3 MoveAiScale = D3DXVECTOR3(0.0f, 0.0f, 0.0f); //移動AIの拡大率

	float fNormalSpeed = 0.0f;                               //通常速度
	float fSensingRange = 0.0f;                              //索敵距離（攻撃モードに入る距離)
	while (Buff != "END_SETIDLEENEMY")
	{
		LoadingFile >> Buff;//単語を読み込む
		if (Buff == "#")
		{
			getline(LoadingFile, Buff);
		}
		else if (Buff == "IDLEENEMYTYPE")
		{
			LoadingFile >> Buff;//イコール
			LoadingFile >> nIdleEnemyType;//何もしない敵タイプを読み込む
		}
		else if (Buff == "ENEMYTYPE")
		{
			LoadingFile >> Buff;//イコール
			LoadingFile >> nType;//敵の分類を読み込む
		}
		else if (Buff == "LIFE")
		{
			LoadingFile >> Buff;//イコール
			LoadingFile >> nLife;//体力を読み込む
		}
		else if (Buff == "POS")
		{
			LoadingFile >> Buff;//イコール
			LoadingFile >> Pos.x;      //位置X
			LoadingFile >> Pos.y;      //位置Y
			LoadingFile >> Pos.z;      //位置Z
		}
		else if (Buff == "ROT")
		{
			LoadingFile >> Buff;//イコール
			LoadingFile >> Rot.x;      //向きX
			LoadingFile >> Rot.y;      //向きY
			LoadingFile >> Rot.z;      //向きZ
		}
		else if (Buff == "SCALE")
		{
			LoadingFile >> Buff;//イコール
			LoadingFile >> Scale.x;      //拡大率X
			LoadingFile >> Scale.y;      //拡大率Y
			LoadingFile >> Scale.z;      //拡大率Z
		}
		else if (Buff == "PHASENUM")
		{
			LoadingFile >> Buff;      //イコール
			LoadingFile >> nPhaseNum; //フェーズ番号
		}
		else if (Buff == "NORMALSPEED")
		{
			LoadingFile >> Buff;//イコール
			LoadingFile >> fNormalSpeed;//通常速度
		}
		else if (Buff == "SENSINGRANGE")
		{
			LoadingFile >> Buff;//イコール
			LoadingFile >> fSensingRange;//索敵範囲を読み込む
		}
		else if (Buff == "SETMOVEAI")
		{//移動AIの情報を読み込む
			while (1)
			{
				LoadingFile >> Buff;
				if (Buff == "SETNUM")
				{
					LoadingFile >> Buff;//イコール
					LoadingFile >> nCntMoveAi;//番号
					while (1)
					{
						LoadingFile >> Buff;

						if (Buff == "POS")
						{
							LoadingFile >> Buff;//イコール
							LoadingFile >> MoveAiPos.x;      //位置X
							LoadingFile >> MoveAiPos.y;      //位置Y
							LoadingFile >> MoveAiPos.z;      //位置Z
						}
						else if (Buff == "ROT")
						{
							LoadingFile >> Buff;//イコール
							LoadingFile >> MoveAiRot.x;      //向きX
							LoadingFile >> MoveAiRot.y;      //向きY
							LoadingFile >> MoveAiRot.z;      //向きZ
						}
						else if (Buff == "SCALE")
						{
							LoadingFile >> Buff;//イコール
							LoadingFile >> MoveAiScale.x;      //拡大率X
							LoadingFile >> MoveAiScale.y;      //拡大率Y
							LoadingFile >> MoveAiScale.z;      //拡大率Z
						}
						else if (Buff == "END_SETNUM")
						{
							if (CScene::GetMode() == CScene::MODE_EDIT)
							{//エディットモードの場合（エディットモードでは最初から全ての敵と移動AIを表示したいので、生成する）
								CAIModel* pAiModel = CAIModel::Create(CAIModel::AIMODELTYPE::MOVEPOINT, MoveAiPos, MoveAiRot, MoveAiScale, nullptr);//生成
								pAiModel->GetDrawInfo().SetUseDraw(true);//描画をする
								pAiModel->GetDrawInfo().SetUseShadow(true);//影を描画する
								VecMoveAi.push_back(pAiModel);
							}
							else if (CScene::GetMode() == CScene::MODE_GAME)
							{//ゲームモードの場合（ゲームモードはフェーズ制で進むので、最初から移動AIを生成したいので、フェーズ情報に格納する)
								MoveAiInfo Info = {};//移動AIの情報を初期化
								Info.Pos = MoveAiPos;//移動AIの位置
								Info.Rot = MoveAiRot;//移動AIの向き
								Info.Scale = MoveAiScale;//移動AIの拡大率
								VecMoveAiInfo.push_back(Info);//移動AI情報の動的配列に格納
							}
							break;
						}
					}
				}
				else if (Buff == "END_SETMOVEAI")
				{//移動AIの読み込みを終了する
					break;
				}
			}
		}
	}

	IdleEnemyType = static_cast<IDLEENEMYTYPE>(nIdleEnemyType);//int型で読み込んだ何もしない敵のタイプ番号を列挙型にキャストして格納
	EnemyType = static_cast<ENEMYTYPE>(nType);                 //敵の分類を列挙型にキャスト

	if (CScene::GetMode() == CScene::MODE_EDIT)
	{//エディットモードの場合（フェーズ制は適用しないので、最初から全ての情報を生成）
		CIdleEnemy* pIdleEnemy = CIdleEnemy::Create(IdleEnemyType, nLife, nPhaseNum, Pos, Rot, Scale);//生成
		pIdleEnemy->GetDrawInfo().SetUseDraw(true);        //描画する
		pIdleEnemy->GetDrawInfo().SetUseShadow(true);      //影を描画する
		pIdleEnemy->SetVecMoveAiInfo(VecMoveAi);           //移動AI情報を設定
		pIdleEnemy->SetNormalSpeed(3.0f);          //通常速度を設定
		pIdleEnemy->SetSensingRange(fSensingRange);        //索敵範囲を設定
		pIdleEnemy->GetMoveInfo().SetUseUpdatePos(true);   //位置の更新を行う
		listSaveManager.push_back(pIdleEnemy);             //vectorに情報を保存する
	}
	else if (CScene::GetMode() == CScene::MODE_GAME)
	{//ゲームモードなら（フェーズ制でゲームが動くので、フェーズ情報に敵の情報を格納）
		CGame::GetPhaseManager()->PushPhaseInfo(Pos, Rot, Scale, nLife, static_cast<int>(EnemyType),nIdleEnemyType, nPhaseNum,3.0f, fSensingRange, 3, VecMoveAiInfo);
	}
}
//============================================================================================================================================

//====================================================================================
//タイプチェンジ処理
//===================================================================================
CObject* CIdleEnemy::ManagerChengeObject(bool bAim)
{
	int nNewType = static_cast<int>(m_IdleEnemyType);//何もしない敵のタイプ列挙型をint型にキャスト
	IDLEENEMYTYPE NewType = IDLEENEMYTYPE::NORMAL;   //新しい何もしない敵タイプ格納用


	//=======================================
	//種類を変える
	//=======================================
	if (bAim == true)
	{//インクリメントする
		nNewType++;
	}
	else
	{//デクリメントする
		nNewType--;
	}

	if (nNewType >= static_cast<int>(IDLEENEMYTYPE::MAX))
	{//タイプが最大に達したら
		nNewType = 0;
	}
	if (nNewType < 0)
	{//タイプが０いかになったら
		nNewType = static_cast<int>(IDLEENEMYTYPE::MAX) - 1;
	}
	//======================================================================================

	//=======================================
	//設定する
	//=======================================
	NewType = static_cast<IDLEENEMYTYPE>(nNewType);
	//======================================================================================

	//=======================================
	//死亡フラグを設定する
	//=======================================
	SetUseDeath(true);
	SetDeath();
	//======================================================================================

	return CIdleEnemy::Create(NewType, GetLifeInfo().GetLife(), GetPhaseNum(), GetPosInfo().GetPos(), GetRotInfo().GetRot(), GetSizeInfo().GetScale());//生成したオブジェクトを返す
}
//============================================================================================================================================

//====================================================================================
//ステージマネージャーに情報を保存する
//===================================================================================
CObject* CIdleEnemy::ManagerSaveObject()
{
	CIdleEnemy* pIdleEnemy = CIdleEnemy::Create(m_IdleEnemyType, GetLifeInfo().GetMaxLife(), GetPhaseNum(), GetPosInfo().GetPos(), GetRotInfo().GetRot(), GetSizeInfo().GetScale());//生成したオブジェクトを返す
	pIdleEnemy->SetSensingRange(GetSensingRange());//現在の敵の索敵範囲を保存する
	pIdleEnemy->SetNormalSpeed(GetNormalSpeed());//現在の敵の通常速度を保存する
	return pIdleEnemy;//生成したオブジェクトを返す
}
//============================================================================================================================================

//====================================================================================
//ステージマネージャーから情報を操作する処理
//===================================================================================
void CIdleEnemy::ManagerChooseControlInfo()
{
	CEnemy::ManagerChooseControlInfo();//敵の情報を操作する
}
//============================================================================================================================================

//====================================================================================
//戦闘モードの動き
//===================================================================================
void CIdleEnemy::BattleMoveProcess()
{
	CEnemy::BattleMoveProcess();//基本的な動きをする
}
//============================================================================================================================================

//====================================================================================
//攻撃処理
//===================================================================================
void CIdleEnemy::AttackProcess()
{
	//「何もしない」ので、攻撃はなし
}
//============================================================================================================================================

//====================================================================================
//AI移動処理
//===================================================================================
void CIdleEnemy::AIMoveProcess()
{
	if (CScene::GetMode() == CScene::MODE_GAME)
	{
		CEnemy::AIMoveProcess();
	}
}
//============================================================================================================================================

//====================================================================================
//撃破演出
//===================================================================================
void CIdleEnemy::DefeatStaging()
{
	if (GetLifeInfo().GetLife() < 1 && GetUseDeath() == true)
	{
		for (int nCnt = 0; nCnt < 60; nCnt++)
		{
			CParticle* pParticle = CParticle::Create(CParticle::TYPE::TYPE00_NORMAL, 120, 80.0f, 80.0f, GetPosInfo().GetPos(), CCalculation::Rand3DVec(200, 10),
				D3DXCOLOR(0.678f, 1.0f, 0.184f, 1.0f), true);
			pParticle->SetUseAddSpeed(true, true, 0.9f);
		}
	}
}
//============================================================================================================================================

//************************************************************************************
//撃破演出
//************************************************************************************

//====================================================================================
//コンストラクタ
//===================================================================================
CEnemyMove_OverJumpObj::CEnemyMove_OverJumpObj(CEnemy* pEnemy, float fGoalheight)
{
	m_fGoalHeight = fGoalheight;//目標のジャンプ高さ
}
//============================================================================================================================================

//====================================================================================
//デストラクタ
//===================================================================================
CEnemyMove_OverJumpObj::~CEnemyMove_OverJumpObj()
{

}
//============================================================================================================================================

//====================================================================================
//処理
//===================================================================================
void CEnemyMove_OverJumpObj::Process(CEnemy* pEnemy)
{
	CObjectX::PosInfo& PosInfo = pEnemy->GetPosInfo();
	const D3DXVECTOR3& Pos = PosInfo.GetPos();
	const D3DXVECTOR3& SenterPos = PosInfo.GetSenterPos();
	m_nCntTime++;

	CParticle::SummonChargeParticle(CParticle::TYPE::TYPE00_NORMAL, 1, 60, 5.0f, 40.0f, 40.0f, 200.0f, 100, 10, true, SenterPos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), true);

	if (m_nCntTime == 100)
	{
		CObjectX::MoveInfo& MoveInfo = pEnemy->GetMoveInfo();

		const D3DXVECTOR3& Move = MoveInfo.GetMove();

		MoveInfo.SetMove(D3DXVECTOR3(Move.x,CCalculation::GetInitialVelocityHeight(m_fGoalHeight,pEnemy->GetMoveInfo().GetGravity()) + 5.0f,Move.z));
		pEnemy->ResetJumpCoolTime();//次にジャンプ攻撃を発動するまでのクールタイムをリセットする
		pEnemy->ChengeMove(DBG_NEW CEnemyMove_Battle());
	}
}
//============================================================================================================================================
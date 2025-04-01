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
	CObjectX::Init();//オブジェクトX初期化処理

	GetMoveInfo().SetUseGravity(true, CObjectX::GetNormalGravity());  //重力を使用する
	GetMoveInfo().SetUseUpdatePos(true);                              //位置の更新をする
	return S_OK;
}
//============================================================================================================================================

//====================================================================================
//終了処理
//====================================================================================
void CEnemy::Uninit()
{
	CObjectX::Uninit();//オブジェクトX終了処理

	if (m_pEnemyMove != nullptr)
	{//敵の移動状態機械を破棄する
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
	{//シーンが「ゲーム」だったら
		const D3DXVECTOR3& Rot = GetRotInfo().GetRot();                           //向き
		const D3DXVECTOR3& Pos = GetPosInfo().GetPos();                           //位置
		const D3DXVECTOR3& PlayerPos = CGame::GetPlayer()->GetPosInfo().GetPos(); //プレイヤーの位置
		float fLength = CCalculation::CalculationLength(Pos, PlayerPos);          //距離

		m_nCntTime++;                                                             //時間をカウントする
		m_nJumpCoolTime++;                                                        //ジャンプまでのクールタイムをカウントする
		m_nAttackCoolTime++;                                                      //クールタイムをカウントする
		CObjectX::CollisionInfo& CollisionInfo = GetCollisionInfo();              //当たり判定情報を取得する
		CObjectX::CollisionInfo::State& CollisionState = CollisionInfo.GetState();//当たり判定状態を取得する

		if (CollisionState.GetLanding())
		{
			GetMoveInfo().SetMove(D3DXVECTOR3(GetMoveInfo().GetMove().x,0.0f, GetMoveInfo().GetMove().z));
		}

		m_pEnemyMove->Process(this);//移動処理全般ステート

		if (m_bPossibleAttack == true)
		{//攻撃が可能だったら
			AttackProcess();//攻撃処理
		}

		//敵の位置の調整
		CollisionDetectionProcess();

		if (m_Type == ENEMYTYPE::DIVEWEAK && m_bStartLanding == true)
		{//ダイブに弱い敵なら、最初に乗った地面から落ちない
			CheckGrroundDistance_AdjustPos();
		}

	    CObjectX::Update();//オブジェクトX更新処理

		if (CollisionState.GetLanding() && m_bStartLanding == false)
		{//地面に乗っているかつ、まだ地面に乗っていなかったら
			m_bStartLanding = true;//地面に初めて乗ったフラグをtrueにする
		}

		CollisionProcess();//当たり判定処理

		DefeatStaging();  //倒されたときの演出を行う（派生クラスごとに変わる）


		if (GetPosInfo().GetPos().y < -100.0f)
		{//一番下の地面より更に下にいたら死亡フラグを設定する
			SetDeath();
		}

	}
	if (CScene::GetMode() == CScene::MODE_EDIT)
	{//シーンが「エディット」
		GetDrawInfo().ChengeColorProcess(this);//敵を選んだときに色を変える処理

		for (auto it : m_VecMoveAi)
		{
			if (it->GetDrawInfo().GetUseDraw())
			{//エディットモード時だけ全ての移動AIモデルを描画する
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
	CObjectX::Draw();//オブジェクトXの描画処理

	for (auto pObj : m_VecMoveAi)
	{
		pObj->ExtraDraw();//AIモデルの描画（敵の描画処理)
	}
}
//============================================================================================================================================

//====================================================================================
//死亡フラグ設定処理
//====================================================================================
void CEnemy::SetDeath()
{
	CObjectX::SetDeath();//オブジェクトX死亡フラグ設定処理

	if (GetUseDeath() == true)
	{//敵の死亡フラグがtrueだったら移動AIの死亡フラグも設定する
		for (auto pObj : m_VecMoveAi)
		{
			if (pObj != nullptr)
			{
				pObj->SetUseDeath(true);
				pObj->SetDeath();
				pObj = nullptr;
			}
		}
		m_VecMoveAi.clear();//移動AIの動的配列をクリアする
	}
}
//============================================================================================================================================

//====================================================================================
//テキストファイルに情報をセーブする
//====================================================================================
void CEnemy::SaveInfoTxt(fstream& WritingFile)
{
	WritingFile << "SETENEMY" << endl;//読み込み開始用テキスト

	WritingFile << "ENEMYTYPE = " << static_cast<int>(m_Type);//敵のタイプ番号を書き出す
	switch (m_Type)
	{//敵のタイプ番号に応じて文字でタイプ名を書き出す
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
		assert(false);//想定外の値
		break;
	}

	WritingFile << "NORMALSPEED = " << m_fNormalSpeed << endl;   //通常速度を書き出す
	WritingFile << "SENSINGRANGE = " << m_fSensingRange << endl; //索敵範囲を書き出す

	//フェーズ番号を設定
	WritingFile << "PHASENUM = " << m_nPhaseNum << endl;

	//移動AIの位置を保存
	WritingFile << "SETMOVEAI" << endl;

	int nCnt = 0;//移動AIの数カウント用
	for (auto it = m_VecMoveAi.begin(); it != m_VecMoveAi.end(); ++it)
	{
		WritingFile << "SETNUM = " << nCnt << endl;//移動AIの番号を書き出す

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
	WritingFile << "END_SETMOVEAI" << endl;//移動AIの情報の保存を終了する

	CObjectX::SaveInfoTxt(WritingFile);//オブジェクトXの情報を書き出す

	WritingFile << "END_SETENEMY" << endl;//読み込み開始用テキスト
}
//============================================================================================================================================

//====================================================================================
//テキストファイルから情報をロードする
//====================================================================================
//void CEnemy::LoadInfoTxt(fstream& LoadingFile, list<CObject*>& listSaveManager, string& Buff)
//{
//	//int nType = 0;//種類
//	//int nLife = 0;//体力
//	//D3DXVECTOR3 Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  //移動量
//	//D3DXVECTOR3 Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   //位置
//	//D3DXVECTOR3 Scale = D3DXVECTOR3(0.0f, 0.0f, 0.0f); //拡大率
//	//D3DXVECTOR3 Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   //向き
//	//ENEMYTYPE Type = {};                               //ブロックの種類
//	//while (Buff != "END_SETBLOCK")
//	//{
//	//	LoadingFile >> Buff;//単語を読み込む
//	//	if (Buff == "#")
//	//	{
//	//		getline(LoadingFile, Buff);
//	//	}
//	//	else if (Buff == "TYPE")
//	//	{
//	//		LoadingFile >> Buff;//イコール
//	//		LoadingFile >> nType;      //種類
//	//	}
//	//	else if (Buff == "LIFE")
//	//	{
//	//		LoadingFile >> Buff;//イコール
//	//		LoadingFile >> nLife;      //体力
//	//	}
//	//	else if (Buff == "MOVE")
//	//	{
//	//		LoadingFile >> Buff;//イコール
//	//		LoadingFile >> Move.x;      //移動量X
//	//		LoadingFile >> Move.y;      //移動量Y
//	//		LoadingFile >> Move.z;      //移動量Z
//	//	}
//	//	else if (Buff == "POS")
//	//	{
//	//		LoadingFile >> Buff;//イコール
//	//		LoadingFile >> Pos.x;      //位置X
//	//		LoadingFile >> Pos.y;      //位置Y
//	//		LoadingFile >> Pos.z;      //位置Z
//	//	}
//	//	else if (Buff == "ROT")
//	//	{
//	//		LoadingFile >> Buff;//イコール
//	//		LoadingFile >> Rot.x;      //位置X
//	//		LoadingFile >> Rot.y;      //位置Y
//	//		LoadingFile >> Rot.z;      //位置Z
//	//	}
//	//	else if (Buff == "SCALE")
//	//	{
//	//		LoadingFile >> Buff;//イコール
//	//		LoadingFile >> Scale.x;      //拡大率X
//	//		LoadingFile >> Scale.y;      //拡大率Y
//	//		LoadingFile >> Scale.z;      //拡大率Z
//	//	}
//	//}
//	//Type = static_cast<ENEMYTYPE>(nType);
//
//	//
//
//	//listSaveManager.push_back(CEnemy::Create(Type, nLife, Pos, Rot, Scale));//vectorに情報を保存する
//
//}
////============================================================================================================================================

//====================================================================================
//テキストファイルから情報をロードする
//====================================================================================
void CEnemy::LoadInfoTxt(fstream& LoadingFile, list<CObject*>& listSaveManager, string& Buff, CObject* pObj)
{
	int nPhaseNum = 0;                                        //フェーズ番号
	float fNormalSpeed = 0.0f;                                //通常速度
	float fSensingRange = 0.0f;                               //索敵範囲
	vector<CAIModel*> VecMoveAi = {};                         //移動AIの動的配列
	vector<MoveAiInfo> VecMoveAiInfo = {};                    //移動AI情報の動的配列
	int nEnemyType = 0;
	ENEMYTYPE EnemyType = ENEMYTYPE::SHOTWEAK;                //敵タイプ
	int nCntMoveAi = 0;                                       //移動AIの数をカウント
	D3DXVECTOR3 MoveAiPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    //移動AIの位置
	D3DXVECTOR3 MoveAiRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    //移動AIの向き
	D3DXVECTOR3 MoveAiScale = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  //移動AIの拡大率
	while (Buff != "END_SETENEMY")
	{//文字列がEND_SETENEMYになるまで繰り返す
		LoadingFile >> Buff;//単語を読み込む
		if (Buff == "#")
		{//#ならその行をスキップ
			getline(LoadingFile, Buff);
		}
		else if (Buff == "ENEMYTYPE")
		{//敵タイプ
			LoadingFile >> Buff;                             //イコール
			LoadingFile >> nEnemyType;                       //フェーズ番号
			EnemyType = static_cast<ENEMYTYPE>(nEnemyType);  //敵タイプにキャスト
			SetEnemyType(EnemyType);                         //敵タイプを設定
		}
		else if (Buff == "PHASENUM")
		{//フェーズ番号
			LoadingFile >> Buff;                             //イコール
			LoadingFile >> nPhaseNum;                        //フェーズ番号
			SetPhaseNum(nPhaseNum);                          //フェーズ番号設定
		}
		else if (Buff == "NORMALSPEED")
		{//通常移動速度
			LoadingFile >> Buff;                             //イコール
			LoadingFile >> fNormalSpeed;                     //通常速度
			SetNormalSpeed(fNormalSpeed);                    //通常速度設定
		}
		else if (Buff == "SENSINGRANGE")
		{//索敵範囲
			LoadingFile >> Buff;                             //イコール
			LoadingFile >> fSensingRange;                    //索敵範囲
			SetSensingRange(fSensingRange);                  //索敵範囲を設定
		}
		else if (Buff == "SETMOVEAI")
		{//移動AI
			while (1)
			{
				LoadingFile >> Buff;
				if (Buff == "SETNUM")
				{//何個目か
					LoadingFile >> Buff;                     //イコール
					LoadingFile >> nCntMoveAi;               //番号
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
							{//エディットモードの時は普通に全ての移動AIを出したいため直接動的配列に格納
								CAIModel* pAiModel = CAIModel::Create(CAIModel::AIMODELTYPE::MOVEPOINT, MoveAiPos, MoveAiRot, MoveAiScale, nullptr);//AIモデルの生成
								pAiModel->GetDrawInfo().SetUseDraw(false);  //描画しない
								pAiModel->GetDrawInfo().SetUseShadow(false);//影を描画しない
								VecMoveAi.push_back(pAiModel);              //移動AIの動的配列に格納
							}
							else if (CScene::GetMode() == CScene::MODE_GAME)
							{//ゲームモードのときはまだ呼ばれていない敵の場合、移動AI自体を存在させたくないだけデータだけを格納し、呼ばれたときにPhaseManagerに保存したデータを使用して移動AIを召喚する
								MoveAiInfo Info = {};          //移動AI情報
								Info.Pos = MoveAiPos;          //位置
								Info.Rot = MoveAiRot;          //向き
								Info.Scale = MoveAiScale;      //拡大率
								VecMoveAiInfo.push_back(Info); //動的配列に格納
							}
							break;
						}
					}
				}
				else if (Buff == "END_SETMOVEAI")
				{//移動AIの情報の書き出し終了
					break;
				}
			}
		}
		else if (Buff == "SETOBJECTX")
		{//オブジェクトXの情報を読み込む
			CObjectX::LoadInfoTxt(LoadingFile, listSaveManager, Buff, pObj);
		}
	}

	if (CScene::GetMode() == CScene::MODE_GAME)
	{//ゲームモードだったら
		//フェーズマネージャーに情報を格納。召喚フェーズに入ったらフェーズマネージャーから敵が召喚される
		CGame::GetPhaseManager()->PushPhaseInfo(GetPosInfo().GetPos(), GetRotInfo().GetRot(), GetSizeInfo().GetScale(), GetLifeInfo().GetMaxLife(),
			static_cast<int>(EnemyType), 0, nPhaseNum, fNormalSpeed, fSensingRange, 3, VecMoveAiInfo);
	}
	else if (CScene::GetMode() == CScene::MODE_EDIT)
	{//エディットモードだったら
		SetVecMoveAiInfo(VecMoveAi);//移動AI情報を設定する
	}
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
	{//オブジェクトのタイプが敵だったら
		CManager::GetDebugText()->PrintDebugText("敵が呼ばれている！\n");
	}

	SetMoveAiPoint();//移動AIの設定を行う

	PhaseNumDecision();//フェーズ番号の決定を行う

	EditNormalSpeed();//通常移動速度を編集

	EditSensingRange();//索敵範囲を編集

	CObjectX::ManagerChooseControlInfo();//オブジェクトXの情報を操作する
}
//============================================================================================================================================

//====================================================================================
//移動AIの情報を設定する
//====================================================================================
void CEnemy::SetVecMoveAiInfo(vector<CAIModel*> vec)
{
	m_VecMoveAi = vec;//AIモデルの動的配列を設定
}
//============================================================================================================================================

//====================================================================================
//プレイヤーを追いかける処理
//====================================================================================
void CEnemy::ChasePlayer()
{
	const D3DXVECTOR3& PlayerPos = CGame::GetPlayer()->GetPosInfo().GetPos();//プレイヤーの位置
	const D3DXVECTOR3& Pos = GetPosInfo().GetPos();                          //敵の位置
	float fLengthPlayer = CCalculation::CalculationLength(PlayerPos, Pos);   //プレイヤーとの距離を求める
	D3DXVECTOR3 Aim = PlayerPos - Pos;                                       //ベクトルを求める
	D3DXVec3Normalize(&Aim, &Aim);                                           //求めたベクトルを正規化        
	float fRot = atan2f(Aim.x, Aim.z);                                       //ベクトルの角度を求める

	D3DXVECTOR3 Move = CCalculation::HormingVecRotXZ(m_fRotMove, GetPosInfo().GetPos(), CGame::GetPlayer()->GetPosInfo().GetSenterPos(), 0.1f, m_fNormalSpeed);//プレイヤーの位置に向かってホーミング風に移動する
	GetRotInfo().SetRot(D3DXVECTOR3(GetRotInfo().GetRot().x, m_fRotMove, GetRotInfo().GetRot().z));//徐々にプレイヤーに向きを合わせていく
	GetMoveInfo().SetMove(D3DXVECTOR3(Move.x, GetMoveInfo().GetMove().y, Move.z));                 //上記の処理で求めた移動量を設定
}
//============================================================================================================================================

//====================================================================================
//敵が倒されたときの演出
//====================================================================================
void CEnemy::DefeatStaging()
{
	CGame::GetCombo()->AddCombo(1);//コンボ数＋１
	CGame::GetScore()->AddScore(100);//スコア＋１００
}
//============================================================================================================================================

//====================================================================================
//当たり判定の処理を行う
//====================================================================================
void CEnemy::CollisionProcess()
{
	CObjectX::CollisionInfo& CollisionInfo = GetCollisionInfo();               //当たり判定情報を取得する
	CObjectX::CollisionInfo::State& CollisionState = CollisionInfo.GetState(); //当たり判定状態を取得する
	CollisionInfo.GetSquareInfo().ResetPushOutFirstFlag();                     //それぞれの軸の押し出し判定の優先フラグをリセット
	CollisionState.SetLanding(false);                                          //地面に乗っているかどうかをリセット 
	for (int nCntPri = 0; nCntPri < CObject::m_nMAXPRIORITY; nCntPri++)
	{//オブジェクトリストを検索する
		CObject* pObj = CObject::GetTopObject(nCntPri);//リストの先頭オブジェクトを取得する

		while (pObj != nullptr)
		{//オブジェクトがnullptrになるまで繰り返す
			CObject* pNext = pObj->GetNextObject();//次のオブジェクトを取得

			CObject::TYPE type = pObj->GetType();  //オブジェクトの種類を取得

			if (type == CObject::TYPE::BLOCK || type == CObject::TYPE::BGMODEL)
			{//オブジェクトの種類がブロック、又は背景モデルだったら
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
			{//オブジェクトのタイプが「ブロック」または「背景モデル」だったら
				CObjectX* pObjX = static_cast<CObjectX*>(pObj);//オブジェクトXにダウンキャスト

				CCollision::ResolveExtrusionCollisionSquare(this, pObjX);//上記のリスト検索で当たり判定の優先度を決めたので、正方形の押し出し判定をする
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
	float fX = 0;//X軸のベクトル
	float fZ = 0;//Z軸のベクトル
	bool bMove = false;//XZ平面の移動をしているかどうか
	bool bMoveY = false;//Y方向の移動をしているかどうか
	float fAddPosY = 0.0f;//Y座標の位置をずらす値
	auto Input = CManager::GetInputKeyboard();//キー入力情報へのポインタ
	float fCameraRot = CManager::GetCamera()->GetRot().y;//カメラの向きを取得
	if (Input->GetPress(DIK_LSHIFT))
	{//LSHIFTキーを押しているかどうか
		if (Input->GetPress(DIK_I))
		{//Iキーを押しているかどうか
			fAddPosY = 5.0f;//Y位置を加算する
			bMoveY = true;//Y方向の移動をしている
		}
		else if (Input->GetPress(DIK_K))
		{//Kキーを押していたら
			fAddPosY = -5.0f;//Y位置を減算する
			bMoveY = true;//Y方向の移動をしている
		}
	}
	else
	{//LSHIFTキーを押していないなら
		if (Input->GetPress(DIK_I))
		{//Iキーを押していたら
			fZ = 1;//Zベクトルを決める
			bMove = true;//XZ平面の移動をしている
		}
		else if (Input->GetPress(DIK_K))
		{//Kキーを押していたら
			fZ = -1;//Zベクトルを決める
			bMove = true;//XZ平面の移動をしている
		}
	}
	if (Input->GetPress(DIK_L))
	{//Lキーを押していたら
		fX = 1;//Xベクトルを決める
		bMove = true;//XZ平面の移動をしている
	}
	else if (Input->GetPress(DIK_J))
	{//Jキーを押していたら
		fX = -1;//Xベクトルを決める
		bMove = true;//XZ平面の移動をしている
	}

	GetMoveInfo().SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));//敵の移動量を０にする
	CManager::GetDebugText()->PrintDebugText("移動AIの設置位置の移動：IKLJ\n");//デバッグ表示

	if (bMove == true)
	{//XZ平面
		float fRot = atan2f(fX, fZ);//ベクトルの角度を求める
		m_MoveAiSavePos += D3DXVECTOR3(sinf(fCameraRot + fRot) * 5.0f, 0.0f, cosf(fCameraRot + fRot) * 5.0f);//カメラの向きを基準とした各江戸の向き
	}

	if (bMoveY == true)
	{//Y方向の移動をしていたら
		m_MoveAiSavePos += D3DXVECTOR3(0.0f, fAddPosY, 0.0f);
	}

	D3DXVECTOR3 NowPos = m_MoveAiSavePos + GetPosInfo().GetPos();//設定する移動AIの現在の位置を敵の位置を基準に決める

	//デバッグ表示
	CManager::GetDebugText()->PrintDebugText("移動AIの位置：%f %f %f\n", NowPos.x,NowPos.y,NowPos.z);
	CManager::GetDebugText()->PrintDebugText("移動AIを保存：O\n");

	//パーティクルを召喚
	CParticle::SummonParticle(CParticle::TYPE::TYPE00_NORMAL, 1, 30, 40.0f, 40.0f, 100, 10, false, NowPos, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), true);

	if (Input->GetPress(DIK_LSHIFT))
	{//LSHIFTキーを押していたら
		if (m_VecMoveAi.size() > 0)
		{//移動AIの数が0より大きければ
			if (Input->GetTrigger(DIK_O))
			{//Oキーを押していたら
				auto it = m_VecMoveAi.end() - 1;//一番後ろのイテレータを取得
				(*it)->SetUseDeath(true);       //死亡フラグを使用する
				(*it)->SetDeath();              //死亡フラグを設定する
				m_VecMoveAi.pop_back();         //一番後ろのオブジェクトを破棄
			}
		}
	}
	else
	{//LSHIFTキーを押していなければ
		if (Input->GetTrigger(DIK_O))
		{//Oキーを押していたら現在のAIモデル召喚位置にAIモデルを設定し動的配列に格納
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
	{//シーンが「ゲーム」なら
		//================
	    //使用する変数
		//================
		CObjectX::RotInfo& RotInfo = GetRotInfo();//向き情報を取得
		const D3DXVECTOR3& Rot = RotInfo.GetRot();//向きを取得
		float fAddRotY = 0.0f;//Y方向の加算向き
		float fLengthPlayer = CCalculation::CalculationLength(CGame::GetPlayer()->GetPosInfo().GetPos(), GetPosInfo().GetPos());//プレイヤーとの距離
		//=========================================================================================================================================

		if (m_VecMoveAi.size() > 0)
		{//移動AIの数が0より大きければ
			auto it = m_VecMoveAi.begin();//最初のポインタを取得

			advance(it, m_nIdxMoveAi);//指定している番号まで進める

			float fLength = CCalculation::CalculationLength(GetPosInfo().GetPos(), (*it)->GetPosInfo().GetPos());//距離を測る

			//現在目的地にしている移動AIへの角度を求める
			float fRot = atan2f((*it)->GetPosInfo().GetPos().x - GetPosInfo().GetPos().x, (*it)->GetPosInfo().GetPos().z - GetPosInfo().GetPos().z);

			//上記で求めた角度を敵の向いている方向とする
			RotInfo.SetRot(D3DXVECTOR3(Rot.x,CCalculation::CalculationCollectionRot2D(Rot.y, fRot, 0.1f, false),Rot.z));//向きを徐々に目的地へ合わせていく

			//現在向いている方向へ移動する
			GetMoveInfo().SetMove(D3DXVECTOR3(sinf(fRot) * m_fNormalSpeed, GetMoveInfo().GetMove().y, cosf(fRot) * m_fNormalSpeed));

			if (fLength < m_fNormalSpeed + 50.0f)
			{//距離が通常移動速度 + 50.0fより近くなれば
				m_nIdxMoveAi++;//目的地を次の位置に変更
			}

			int nSize = m_VecMoveAi.size();
			if (m_nIdxMoveAi >= nSize)
			{//目的地が移動AIの動的配列のサイズを超えたら
				m_nIdxMoveAi = 0;//0に戻す
			}
			if (m_nIdxMoveAi < 0)
			{//目的地が0以下になったら
				m_nIdxMoveAi = nSize - 1;//動的配列の一番後ろを指定
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
	float fLengthPlayer = CCalculation::CalculationLength(CGame::GetPlayer()->GetPosInfo().GetPos(), GetPosInfo().GetPos());//プレイヤーとの距離を求める

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
	{//シーンが「ゲーム」だったら
		CObjectX* pObjX = nullptr;//オブジェクトXへのポインタを初期化
		D3DXVECTOR3 Ray = CCalculation::RadToVec(D3DXVECTOR3(GetRotInfo().GetRot().x, GetRotInfo().GetRot().y,0.0f) - D3DXVECTOR3(D3DX_PI * 0.5f,0.0f,0.0f));//手前側にレイを飛ばす

		for (int nCntPri = 0; nCntPri < m_nMAXPRIORITY; nCntPri++)
		{//オブジェクトリストを検索する
			CObject* pObj = GetTopObject(nCntPri);//リストの先頭オブジェクトを取得する

			while (pObj != nullptr)
			{//オブジェクトがnullptrになるまで繰り返す
				CObject* pNext = pObj->GetNextObject();
				D3DXVECTOR3 CollisionPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//レイが当たる位置格納用
				if (pObj->GetType() == CObject::TYPE::BGMODEL || pObj->GetType() == CObject::TYPE::BLOCK)
				{//オブジェクトのタイプが「背景モデル」又は「ブロック」なら
					CObjectX * pComObjX = static_cast<CObjectX*>(pObj);
					if (CCollision::RayIntersectsAABBCollisionPos(GetPosInfo().GetSenterPos(), Ray, pComObjX->GetPosInfo().GetPos() + pComObjX->GetSizeInfo().GetVtxMin(), pComObjX->GetPosInfo().GetPos() + pComObjX->GetSizeInfo().GetVtxMax(),
						CollisionPos))
					{//レイがAABBに当たっていたら
						float fLength = sqrtf(powf(CollisionPos.x - GetPosInfo().GetSenterPos().x, 2) +
							powf(CollisionPos.y - GetPosInfo().GetSenterPos().y, 2) + powf(CollisionPos.z - GetPosInfo().GetSenterPos().z,2));//レイが当たった位置までの距離を計算する
						if (fLength < GetSizeInfo().GetSize().x + 50.0f)
						{//敵のX方向のサイズ+50.0fよりも小さければ
							//敵のオブジェクトのサイズの少し上より距離が小さくなった
							pObjX = pComObjX;//近いオブジェクトを格納
						}
					}
				}

				pObj = pNext;//リストを次に進める
			}
		}

		if (pObjX != nullptr)
		{//当たったオブジェクトがあった場合、プレイヤーとのベクトルの内積をとり、オブジェクトをよけるまで右側に動いたり左側に動いたりする
			D3DXVECTOR3 ObjectAim = pObjX->GetPosInfo().GetPos() - GetPosInfo().GetPos();//当たったオブジェクトと敵のベクトルを求める
			D3DXVec3Normalize(&ObjectAim, &ObjectAim);//上記で求めたベクトルを正規化する
			const D3DXVECTOR3& PlayerPos = CGame::GetPlayer()->GetPosInfo().GetPos();//プレイヤーの位置
			const D3DXVECTOR3& Pos = GetPosInfo().GetPos();//敵の位置

			float fDot = CCalculation::DetermineSide3D(Pos, ObjectAim, D3DXVECTOR3(0.0f, 1.0f, 0.0f), PlayerPos);//当たったオブジェクトに対してプレイヤーがどちら側にいるかを計算する
			D3DXVECTOR3 Aim = PlayerPos - Pos;//プレイヤーと敵のベクトルを取る
			D3DXVec3Normalize(&Aim, &Aim);//ベクトルを正規化する
			float fRot = atan2f(Aim.x, Aim.z);//XZ平面のベクトルの角度を求める

			if (fDot > 0.0f)
			{//ベクトルに対してプレイヤーが右側にいたら
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
	m_Pattern.nPattern = 0;        //パターン番号をリセット
	m_Pattern.nPatternTime = 0;    //パターン時間をリセット
	m_Pattern.bAction = false;     //行動しているかどうかをリセット 
	m_Pattern.nSubPattern = 0;     //サブパターン番号をリセット
	m_nAttackCoolTime = 0;         //クールタイムをリセット
	GetMoveInfo().SetUseGravity(true, GetNormalGravity());  //重力を使用する
	GetMoveInfo().SetUseInteria(false, GetNormalInertia()); //慣性を使用しない
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
		{//出現フェーズ番号をインクリメント
			m_nPhaseNum++;
		}
	}
	else
	{
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_4))
		{//出現フェーズ番号をデクリメント
			m_nPhaseNum--;
		}
	}

	//デバッグ表示
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
	D3DXVECTOR3 MyPos = GetPosInfo().GetPos();        //自分の位置
	D3DXVECTOR3 MyVtxMax = GetSizeInfo().GetVtxMax(); //自分の最大頂点

	//自分自身の位置とXの最大頂点との距離を取る
	float fMyCornarDistance = CCalculation::CalculationLength(MyPos,MyPos + D3DXVECTOR3(MyVtxMax.x, 0.0f, 0.0f));

	for (int nCntPri = 0; nCntPri < m_nMAXPRIORITY; nCntPri++)
	{//オブジェクトリストを検索する
		CObject* pObj = GetTopObject(nCntPri);//リストの先頭オブジェクトを取得する
		while (pObj != nullptr)
		{//オブジェクトがnullptrになるまで繰り返す
			CObject* pNext = pObj->GetNextObject();
			if (pObj->GetType() == CObject::TYPE::ENEMY || pObj->GetType() == CObject::TYPE::PLAYER)
			{//オブジェクトのタイプが敵又はプレイヤーだったら
				CObjectX* pObjX = static_cast<CObjectX*>(pObj);//オブジェクトXにキャスト
				if (pObjX != this)
				{//オブジェクトXが自分以外なら
					D3DXVECTOR3 AimVec = D3DXVECTOR3(pObjX->GetPosInfo().GetPos().x, 0.0f, pObjX->GetPosInfo().GetPos().z) -
						D3DXVECTOR3(MyPos.x, 0.0f, MyPos.z);//敵と相手のXZ方向のベクトルを取る
					D3DXVec3Normalize(&AimVec, &AimVec);//XZ方向のベクトルを正規化
					const D3DXVECTOR3& ComPos = pObjX->GetPosInfo().GetPos();                                            //相手の位置
					const D3DXVECTOR3& ComVtxMaxX = ComPos + D3DXVECTOR3(pObjX->GetSizeInfo().GetVtxMax().x, 0.0f, 0.0f);//相手のXの最大頂点の位置
					float fLength = CCalculation::CalculationLength(MyPos, pObjX->GetPosInfo().GetPos());//敵又はプレイヤーとの距離をとる

					float fComCornarDistance = CCalculation::CalculationLength(ComPos, ComVtxMaxX);//相手の位置と相手のXの最大頂点との距離を取る

					float fTotalLength = (fComCornarDistance + fMyCornarDistance);//自分と相手の中心点からの距離を足す
					if (fLength < fTotalLength &&
						GetPosInfo().GetPos().y + GetSizeInfo().GetVtxMax().y >= pObjX->GetPosInfo().GetPos().y + pObjX->GetSizeInfo().GetVtxMin().y &&
						GetPosInfo().GetPos().y + GetSizeInfo().GetVtxMin().y <= pObjX->GetPosInfo().GetPos().y + pObjX->GetSizeInfo().GetVtxMax().y)
					{//現在の相手との距離が自分と相手の中心点からの距離より小さければ
						//どれだけめり込んでるかによって必要なXZ移動量を足す（ベクトルの逆側に移動量を足す)
						GetMoveInfo().SetMove(D3DXVECTOR3(GetMoveInfo().GetMove().x -AimVec.x * (fTotalLength - fLength) * 1.0f,GetMoveInfo().GetMove().y, GetMoveInfo().GetMove().z - AimVec.z * (fTotalLength - fLength) * 1.0f));//攻撃時の動きよりも優先的にこの移動量を割り当てる
					}

					
				}
			}

			pObj = pNext;//リストを次に進める
		}
	}
}
//============================================================================================================================================

//====================================================================================
//衝突判定処理
//====================================================================================
void CEnemy::CheckGrroundDistance_AdjustPos()
{
	CObject* pObjBgModel = GetTopObject(static_cast<int>(CObject::TYPE::BGMODEL));//背景モデルのリストの先頭を取得
	CObject* pObjBlock = GetTopObject(static_cast<int>(CObject::TYPE::BLOCK));    //ブロックのリストの先頭を取得
	CObjectX::PosInfo& PosInfo = GetPosInfo();                                    //位置情報
	CObjectX::SizeInfo& SizeInfo = GetSizeInfo();                                 //サイズ情報
	CObjectX::MoveInfo& MoveInfo = GetMoveInfo();                                 //移動情報
	const D3DXVECTOR3& Pos = PosInfo.GetPos();                                    //位置
	const D3DXVECTOR3& PosOld = PosInfo.GetPosOld();                              //1f前の位置
	const D3DXVECTOR3& VtxMin = SizeInfo.GetVtxMin();                             //最小頂点
	const D3DXVECTOR3& VtxMax = SizeInfo.GetVtxMax();                             //最大頂点
	const D3DXVECTOR3& Move = MoveInfo.GetMove();                                 //移動量
	D3DXVECTOR3 Dir = D3DXVECTOR3(0.0f, -1.0f, 0.0f);                             //下方向ベクトルを定義
	D3DXVECTOR3 OriginPos = Pos + D3DXVECTOR3(0.0f, VtxMin.y, 0.0f);              //判定する位置（オブジェクトの底面）
	D3DXVECTOR3 RayCollisionPos = { 0.0f,0.0f,0.0f };                             //レイの判定場所計算用
	bool bCollision = false;                                                      //衝突したかどうか
	float fTotalBestNearLength = 0.0f;                                            //レイが当たったオブジェクトの距離の中で一番近い距離を決める
	int nCntRayCollosionObj = 0;                                                  //レイが当たったオブジェクトの数をカウントする
	//背景モデルリスト
	while (pObjBgModel != nullptr)
	{
		CObject* pNextBgMoodel = pObjBgModel->GetNextObject();
		CBgModel* pBgModel = static_cast<CBgModel*>(pObjBgModel);                 //背景モデルにキャスト
		CObjectX::PosInfo BgModelPosInfo = pBgModel->GetPosInfo();                //背景モデルの位置情報
		CObjectX::SizeInfo BgModelSizeInfo = pBgModel->GetSizeInfo();             //背景モデルのサイズ情報
		const D3DXVECTOR3& BgModelPos = BgModelPosInfo.GetPos();                  //背景モデルの位置
		const D3DXVECTOR3& BgModelPosOld = BgModelPosInfo.GetPosOld();            //1f前の位置
		const D3DXVECTOR3& BgModelVtxMax = BgModelSizeInfo.GetVtxMax();           //背景モデルの最小頂点
		const D3DXVECTOR3& BgModelVtxMin = BgModelSizeInfo.GetVtxMin();           //背景モデルの最大頂
		if (CCollision::RayIntersectsAABBCollisionPos(OriginPos, Dir, BgModelPos + BgModelVtxMin, BgModelPos + BgModelVtxMax, RayCollisionPos))
		{//レイが背景モデルに当たっていたら
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
		pObjBgModel = pNextBgMoodel;//リストを次に進める
	}

	//ブロックリスト
	while (pObjBlock != nullptr)
	{
		CObject* pNextBlock = pObjBlock->GetNextObject();             //次のオブジェクトを取得
		CBlock* pBlock = static_cast<CBlock*>(pObjBlock);             //ブロックにキャスト
		CObjectX::PosInfo BlockPosInfo = pBlock->GetPosInfo();        //ブロックの位置情報
		CObjectX::SizeInfo BlockSizeInfo = pBlock->GetSizeInfo();     //ブロックのサイズ情報
		const D3DXVECTOR3& BlockPos = BlockPosInfo.GetPos();          //ブロックの位置
		const D3DXVECTOR3& BlockPosOld = BlockPosInfo.GetPosOld();    //1f前の位置
		const D3DXVECTOR3& BlockVtxMax = BlockSizeInfo.GetVtxMax();   //ブロックの最小頂点
		const D3DXVECTOR3& BlockVtxMin = BlockSizeInfo.GetVtxMin();   //ブロックの最大頂
		if (CCollision::RayIntersectsAABBCollisionPos(OriginPos, Dir, BlockPos + BlockVtxMin, BlockPos + BlockVtxMax, RayCollisionPos))
		{//ブロックにレイが当たっていたら
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
		PosInfo.SetPos(PosOld);        //位置を設定
		PosInfo.SetPosOld(Pos - Move); //1f前の位置を設定
		CManager::GetDebugText()->PrintDebugText("落ちるので戻る\n");//デバッグ表示
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

	//デバッグ表示
	CManager::GetDebugText()->PrintDebugText("通常移動速度変更（５）：%f\n",m_fNormalSpeed);
}
//============================================================================================================================================

//====================================================================================
//通常移動速度を編集
//====================================================================================
void CEnemy::EditSensingRange()
{
	CInputKeyboard* pInput = CManager::GetInputKeyboard();//キー入力情報
	CObjectX::PosInfo& PosInfo = GetPosInfo();            //位置情報
	const D3DXVECTOR3& Pos = PosInfo.GetPos();            //位置
	if (pInput->GetPress(DIK_LCONTROL) == true)
	{//Lコントロールキーを押しながら
		if (pInput->GetPress(DIK_LSHIFT) == true)
		{//シフトキーを押しながら・・・
			if (pInput->GetPress(DIK_6) == true)
			{//索敵範囲を狭くする
				m_fSensingRange -= 10.0f;
			}
		}
		else if (pInput->GetPress(DIK_6) == true)
		{//索敵範囲を広くする
			m_fSensingRange += 10.0f;
		}
	}
	else
	{//Lコントロールキーを押していない
		if (pInput->GetPress(DIK_LSHIFT) == true)
		{//シフトキーを押しながら・・・
			if (pInput->GetTrigger(DIK_6) == true)
			{//索敵範囲を狭くする
				m_fSensingRange -= 10.0f;
			}
		}
		else if (pInput->GetTrigger(DIK_6) == true)
		{//索敵範囲を広くする
			m_fSensingRange += 10.0f;
		}
	}
	float fRadXZ = static_cast<float>(rand() % 628 - 314) * 0.01f;//XZ方向の角度(Yaw)を求める
	float fRadY = static_cast<float>(rand() % 628 - 314) * 0.01f; //Y方向の角度(Pitch)を求める
	D3DXVECTOR3 RandPos = { 0.0f,0.0f,0.0f };

	RandPos.x = m_fSensingRange * sinf(fRadXZ) * cosf(fRadY);//X方向の長さに対して、球の側面に出すために、Y方向の高さの比（高さを底面の比としてかける）
	RandPos.z = m_fSensingRange * cosf(fRadXZ) * cosf(fRadY);//Z方向の長さに対して、球の側面に出すために、Y方向の高さの比（高さを底面の比としてかける）
	RandPos.y = m_fSensingRange * sinf(fRadY);               //Y方向の高さを求める

	//索敵範囲を表すためにパーティクルを索敵距離の位置にランダムで出す
	CParticle::Create(CParticle::TYPE00_NORMAL, 200, 30.0f, 30.0f,Pos + RandPos,
		D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), true);

	//デバッグ表示
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
const float CShotWeakEnemy::s_fATTACKSTART_LENGTH = 450.0f;      //攻撃開始距離
const float CShotWeakEnemy::s_fNORMAL_SENSIINGRANGE = 1050.0f;   //通常感知射程
const int CShotWeakEnemy::s_nATTACK_COOLTIME = 60;               //攻撃のクールタイム
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
	CEnemy::Init();//敵の初期化処理

	float fRatioRot = static_cast<float>(rand() % 200 - 100) / 100;   //剣の最初の向きをランダムで決める
	bool bAim = rand() % 2;                                           //符号をプラスかマイナスか決める
	m_pMagicSword = CAttackEnemy::Create(CAttack::ATTACKTYPE::MAGICSWORD, CAttack::TARGETTYPE::PLAYER, CAttack::COLLISIONTYPE::RECTANGLE_XZ,
		false,false,1, 60, 200, GetPosInfo().GetPos(), D3DXVECTOR3(0.0f,D3DX_PI * fRatioRot, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f));//剣を生成
	m_pMagicSword->SetUseDeath(false);//剣の死亡フラグをオフにする

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
	CEnemy::Uninit();//敵の終了処理
}
//============================================================================================================================================

//====================================================================================
//更新処理
//====================================================================================
void CShotWeakEnemy::Update()
{
	CEnemy::Update();//敵の更新処理

	if (m_pMagicSword != nullptr)
	{//剣が存在していたら
		m_pMagicSword->GetPosInfo().SetPos(GetPosInfo().GetPos());//剣の中心を常に敵の位置に固定
	}

	SwordCollision();//剣の当たり判定を行う
}
//============================================================================================================================================

//====================================================================================
//描画処理
//====================================================================================
void CShotWeakEnemy::Draw()
{
	CEnemy::Draw();//敵の描画処理
}
//============================================================================================================================================

//====================================================================================
//死亡フラグ設定処理
//====================================================================================
void CShotWeakEnemy::SetDeath()
{
	CEnemy::SetDeath();//敵の死亡フラグを設定
	if (GetUseDeath() == true)
	{//敵死亡フラグを使用するなら
		if (m_pMagicSword != nullptr)
		{//剣が存在していたら
			m_pMagicSword->SetUseDeath(true);//剣の死亡フラグを使用する
			m_pMagicSword->SetDeath();       //死亡フラグを設定する
		}
	}
}
//============================================================================================================================================

//====================================================================================
//生成処理
//====================================================================================
CShotWeakEnemy* CShotWeakEnemy::Create(SHOTWEAKENEMYTYPE Type, int nLife, int nPhaseNum, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 Scale)
{
	CShotWeakEnemy* pShotWeakEnemy = DBG_NEW CShotWeakEnemy;                             //射撃に弱い敵の生成

	pShotWeakEnemy->Init();                                                              //初期化処
	int nType = static_cast<int>(Type);//タイプ番号を格納
	if (nType < 0 || nType >= static_cast<int>(SHOTWEAKENEMYTYPE::MAX))
	{
		assert("配列外アクセス！(CShotWeakEnemy)");
	}
	else
	{//例外処理
		int nIdx = CManager::GetObjectXInfo()->Regist(s_aSHOTWEAKENEMY_FILENAME[nType]); //モデル情報を登録し番号を取得
		pShotWeakEnemy->BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(nIdx),           //モデル情報を設定する
			CManager::GetObjectXInfo()->GetBuffMat(nIdx),
			CManager::GetObjectXInfo()->GetdwNumMat(nIdx),
			CManager::GetObjectXInfo()->GetTexture(nIdx),
			CManager::GetObjectXInfo()->GetColorValue(nIdx));
	}
	pShotWeakEnemy->SetPhaseNum(nPhaseNum);                                              //フェーズ番号を設定する
	pShotWeakEnemy->m_ShotWeakEnemyType = Type;                                          //射撃に弱い敵の種類
	pShotWeakEnemy->GetLifeInfo().SetLife(nLife);                                        //体力
	pShotWeakEnemy->GetLifeInfo().SetMaxLife(nLife);                                     //最大体力
	pShotWeakEnemy->GetLifeInfo().SetAutoSubLife(false);                                 //自動的に体力を減らすかどうか
	pShotWeakEnemy->GetPosInfo().SetPos(pos);                                            //位置
	pShotWeakEnemy->GetPosInfo().SetSupportPos(pos);                                     //支点位置
	pShotWeakEnemy->GetRotInfo().SetRot(rot);                                            //向き
	pShotWeakEnemy->GetSizeInfo().SetScale(Scale);                                       //拡大率
	pShotWeakEnemy->GetSizeInfo().SetFormarScale(Scale);                                 //元の拡大率を設定
	pShotWeakEnemy->SetSensingRange(1250.0f);                                            //感知射程
	pShotWeakEnemy->SetNormalSpeed(10.0f);                                               //通常移動速度
	pShotWeakEnemy->GetMoveInfo().SetUseInteria(false, GetNormalInertia());              //慣性を使用しない
	if (CScene::GetMode() == CScene::MODE_GAME)
	{//ゲームモードの時だけ剣を出す
		pShotWeakEnemy->m_pMagicSword->GetSizeInfo().SetScale(Scale * 0.5f);
	}
	pShotWeakEnemy->GetLifeInfo().SetAutoDeath(true);                                    //体力が０になったら死亡フラグを設定する

	pShotWeakEnemy->SetSize();                                                           //モデルサイズを設定
	pShotWeakEnemy->SetManagerObjectType(CObject::MANAGEROBJECTTYPE::SHOTWEAKENEMY);     //マネージャーで呼び出す時の種類を設定
	return pShotWeakEnemy;
}
//============================================================================================================================================

//====================================================================================
//テキストファイルに情報を保存する
//====================================================================================
void CShotWeakEnemy::SaveInfoTxt(fstream& WritingFile)
{
	WritingFile << "SETSHOTWEAKENEMY" << endl;//射撃に弱い敵の情報を書き出す
	WritingFile << "SHOTWEAKENEMYTYPE = " << static_cast<int>(m_ShotWeakEnemyType);//射撃に弱い敵のタイプ番号を設定
	switch (m_ShotWeakEnemyType)
	{//射撃に弱い敵のタイプ名を書き出す
	case SHOTWEAKENEMYTYPE::NORMAL://普通
		WritingFile << " # NORMAL" << endl;
		break;
	default:
		assert(false);
		break;
	}

	CEnemy::SaveInfoTxt(WritingFile);//敵の情報を書き出す

	WritingFile << "END_SETSHOTWEAKENEMY" << endl;//射撃に弱い敵の情報の設定を終了
}
//============================================================================================================================================

//====================================================================================
//テキストファイルから情報を読み込む
//====================================================================================
//void CShotWeakEnemy::LoadInfoTxt(fstream& LoadingFile, list<CObject*>& listSaveManager, string& Buff)
//{
//	int nShotWeakEnemyType = 0;                               //ショットに弱い敵タイプ   
//	int nLife = 0;                                            //体力
//	int nPhaseNum = 0;                                        //フェーズ番号
//	int nDivisionNum = 0;                                     //分裂回数
//	D3DXVECTOR3 Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);         //移動量
//	D3DXVECTOR3 Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);          //位置
//	D3DXVECTOR3 Scale = D3DXVECTOR3(0.0f, 0.0f, 0.0f);        //拡大率
//	D3DXVECTOR3 Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);          //向き
//	SHOTWEAKENEMYTYPE ShotWeakEnemyType = {};                 //背景モデルの種類
//	ENEMYTYPE EnemyType = {};                                 //敵のタイプ
//													          
//	vector<CAIModel*> VecMoveAi = {};                         //移動AIの動的配列
//	vector<MoveAiInfo> VecMoveAiInfo = {};                    //移動AI情報の動的配列
//													          
//	int nCntMoveAi = 0;                                       //移動AIの数をカウント
//	D3DXVECTOR3 MoveAiPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    //移動AIの位置
//	D3DXVECTOR3 MoveAiRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    //移動AIの向き
//	D3DXVECTOR3 MoveAiScale = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  //移動AIの拡大率
//
//	float fNormalSpeed = 0.0f;                                //通常速度
//	float fSensingRange = 0.0f;                               //索敵距離
//
//	while (Buff != "END_SETSHOTWEAKENEMY")
//	{//文字列がEND_SETSHOTWEAKENEMYになるまで繰り返す
//		LoadingFile >> Buff;//単語を読み込む
//		if (Buff == "#")
//		{//#ならその行をスキップ
//			getline(LoadingFile, Buff);
//		}
//		else if (Buff == "SHOTWEAKENEMYTYPE")
//		{//射撃に弱い敵の種類
//			LoadingFile >> Buff;//イコール
//			LoadingFile >> nShotWeakEnemyType;
//		}
//		else if (Buff == "ENEMYTYPE")
//		{//敵のタイプ
//			LoadingFile >> Buff;//イコール
//			LoadingFile >> nType;
//		}
//		else if (Buff == "LIFE")
//		{//体力
//			LoadingFile >> Buff;//イコール
//			LoadingFile >> nLife;
//		}
//		else if (Buff == "POS")
//		{//位置
//			LoadingFile >> Buff;//イコール
//			LoadingFile >> Pos.x;      //位置X
//			LoadingFile >> Pos.y;      //位置Y
//			LoadingFile >> Pos.z;      //位置Z
//		}
//		else if (Buff == "ROT")
//		{//向き
//			LoadingFile >> Buff;//イコール
//			LoadingFile >> Rot.x;      //位置X
//			LoadingFile >> Rot.y;      //位置Y
//			LoadingFile >> Rot.z;      //位置Z
//		}
//		else if (Buff == "SCALE")
//		{//拡大率
//			LoadingFile >> Buff;//イコール
//			LoadingFile >> Scale.x;      //拡大率X
//			LoadingFile >> Scale.y;      //拡大率Y
//			LoadingFile >> Scale.z;      //拡大率Z
//		}
//		else if (Buff == "PHASENUM")
//		{//フェーズ番号
//			LoadingFile >> Buff;//イコール
//			LoadingFile >> nPhaseNum;//フェーズ番号
//		}
//		else if (Buff == "NORMALSPEED")
//		{//通常移動速度
//			LoadingFile >> Buff;//イコール
//			LoadingFile >> fNormalSpeed;//通常速度
//		}
//		else if (Buff == "SENSINGRANGE")
//		{//索敵範囲
//			LoadingFile >> Buff;//イコール
//			LoadingFile >> fSensingRange;
//		}
//	}
//
//	ShotWeakEnemyType = static_cast<SHOTWEAKENEMYTYPE>(nShotWeakEnemyType);//射撃に弱い敵のタイプを格納
//	EnemyType = static_cast<ENEMYTYPE>(nType);                             //敵のタイプを格納
//	if (CScene::GetMode() == CScene::MODE_EDIT)
//	{//エディットモードだったら
//		CShotWeakEnemy* pShotWeakEnemy = CShotWeakEnemy::Create(ShotWeakEnemyType,nLife,nPhaseNum,Pos,Rot,Scale);//射撃に弱い敵を生成
//		pShotWeakEnemy->SetVecMoveAiInfo(VecMoveAi);    //移動AIの動的配列を設定
//		pShotWeakEnemy->SetNormalSpeed(fNormalSpeed);   //通常移動速度を設定
//		pShotWeakEnemy->SetSensingRange(fSensingRange); //索敵範囲を設定
//		listSaveManager.push_back(pShotWeakEnemy);      //vectorに情報を保存する
//
//	}
//	else if (CScene::GetMode() == CScene::MODE_GAME)
//	{//ゲームモードだったら
//
//		//フェーズマネージャーに情報を格納。召喚フェーズに入ったらフェーズマネージャーから敵が召喚される
//		CGame::GetPhaseManager()->PushPhaseInfo(Pos, Rot, Scale, nLife, static_cast<int>(EnemyType), nShotWeakEnemyType, nPhaseNum,fNormalSpeed,fSensingRange,0,VecMoveAiInfo);
//	}
//
//}
////============================================================================================================================================

//====================================================================================
//テキストファイルから情報を読み込む
//====================================================================================
void CShotWeakEnemy::LoadInfoTxt(fstream& LoadingFile, list<CObject*>& listSaveManager, string& Buff, CObject* pObj)
{
	int nShotWeakEnemyType = 0;                                            //射撃に弱い敵タイプ   
	SHOTWEAKENEMYTYPE ShotWeakEnemyType = {};                              //射撃に弱い敵タイプ
	CShotWeakEnemy* pShotWeakEnemy = dynamic_cast<CShotWeakEnemy*>(pObj);  //射撃に弱い敵にダウンキャスト
	
	if (pShotWeakEnemy != nullptr)
	{
		while (Buff != "END_SETSHOTWEAKENEMY")
		{//文字列がEND_SETSHOTWEAKENEMYになるまで繰り返す
			LoadingFile >> Buff;//単語を読み込む
			if (Buff == "#")
			{//#ならその行をスキップ
				getline(LoadingFile, Buff);
			}
			else if (Buff == "SHOTWEAKENEMYTYPE")
			{//射撃に弱い敵の種類
				LoadingFile >> Buff;//イコール
				LoadingFile >> nShotWeakEnemyType;
				ShotWeakEnemyType = static_cast<SHOTWEAKENEMYTYPE>(nShotWeakEnemyType);//射撃に弱い敵のタイプを格納
				SetShotWeakEnemyType(ShotWeakEnemyType);               //射撃に弱い敵タイプを設定
			}
			else if (Buff == "SETENEMY")
			{//敵の情報を設定
				CEnemy::LoadInfoTxt(LoadingFile, listSaveManager, Buff, pObj);
			}
		}

		if (CScene::GetMode() == CScene::MODE_EDIT)
		{//エディットモードだったら
			listSaveManager.push_back(this);      //ステージマネージャーに情報を保存する
		}
		else if (CScene::GetMode() == CScene::MODE_GAME)
		{//ゲームモードではフェーズマネージャーで召喚するので一旦消す
			SetUseDeath(true);
			SetDeath();
		}
	}
	else
	{//例外処理
		assert("射撃に弱い敵へのポインタが存在しない");
	}
}

//====================================================================================
//ステージマネージャーのオブジェクトをチェンジする
//====================================================================================
CObject* CShotWeakEnemy::ManagerChengeObject(bool bAim)
{
	int nNewType = int(m_ShotWeakEnemyType);//射撃に弱い敵タイプをint型にキャスト
	SHOTWEAKENEMYTYPE NewType = {};         //新しい射撃に弱い敵タイプ格納用
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
	SetUseDeath(true);//死亡フラグを使用する
	SetDeath();       //死亡フラグを設定する
	//======================================================================================

	return CShotWeakEnemy::Create(NewType, GetLifeInfo().GetMaxLife(), GetPhaseNum(), GetPosInfo().GetPos(), GetRotInfo().GetRot(), GetSizeInfo().GetScale());//生成したオブジェクトを返す
}
//============================================================================================================================================

//====================================================================================
//ステージマネージャーに現在のオブジェクトを保存する
//====================================================================================
CObject* CShotWeakEnemy::ManagerSaveObject()
{
	auto& Vec = GetVecAiModelInfo();//現在配置している敵の移動AI情報を取得
	auto Vec2 = move(Vec);          //現在配置している敵の移動AI情報を変数に移動
	CShotWeakEnemy* pShotWeakEnemy = CShotWeakEnemy::Create(m_ShotWeakEnemyType, GetLifeInfo().GetMaxLife(), GetPhaseNum(), GetPosInfo().GetPos(), GetRotInfo().GetRot(), GetSizeInfo().GetScale());//生成したオブジェクトを返す
	pShotWeakEnemy->SetVecMoveAiInfo(Vec2);            //移動AI情報を設定
	pShotWeakEnemy->SetNormalSpeed(GetNormalSpeed());  //現在の敵の通常速度を保存する
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

	//デバッグ表示
	CManager::GetDebugText()->PrintDebugText("射撃に弱い敵の移動量：%f %f %f\n", Move.x, Move.y, Move.z);

	RayCollisionJumpOverOnHit();//レイが当たった時にジャンプさせるための処理
}
//============================================================================================================================================

//====================================================================================
//攻撃処理
//====================================================================================
void CShotWeakEnemy::AttackProcess()
{
	float fLength = CCalculation::CalculationLength(GetPosInfo().GetPos(), CGame::GetPlayer()->GetPosInfo().GetPos()); //自分とプレイヤーの距離
	float fLengthY = CGame::GetPlayer()->GetPosInfo().GetPos().y - GetPosInfo().GetPos().y;                            //自分とプレイヤーのY軸の距離
	const bool& bAction = GetAction();                                                                                 //攻撃状態かどうか
	const int& nPatternTime = GetPatternTime();                                                                        //攻撃パターンに入ってからの時間を取得する
	const int& nPattern = GetPattern();                                                                                //パターン番号を取得
																													   
	if (fLength < s_fATTACKSTART_LENGTH && bAction == false && fLengthY < 400.0f && GetAttackCoolTime() > s_nATTACK_COOLTIME)
	{//攻撃が開始されていなければ（クールタイムごとに攻撃を行う。ターン性バトルみたいな動きになる）
		ChengeMove(DBG_NEW CEnemyMove_None());//AI移動と攻撃処理を入れ替える
		SetAction(true);                      //行動開始
	}

	if (bAction == true)
	{//行動が開始されていたら
		switch (nPattern)
		{
		case 0://止まって突進方向を決める
			GetMoveInfo().SetMove(D3DXVECTOR3(0.0f, GetMoveInfo().GetMove().y, 0.0f));
			if (nPatternTime == 15)
			{//このパターンに入ってから15フレーム後
				SetPattern(nPattern + 1);//パターン番号を次に進める
				SetPatternTime(0);       //パターン時間をリセット
				m_SaveAimPos = CGame::GetPlayer()->GetPosInfo().GetPos();//目的地をこの時点でのプレイヤーの位置に設定
			}
			break;
		case 1://チャージする

			//パーティクルを召喚(自分に向かって動く)
			CParticle::SummonChargeParticle(CParticle::TYPE::TYPE00_NORMAL, 1, 45, 5.0f, 20.0f, 20.0f, GetSizeInfo().GetSize().x, 100, 10, false,
				GetPosInfo().GetSenterPos(), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), true);
			if (nPatternTime == 45)
			{//このパターンに入ってから４５フレーム後、狙った方向に突進する
				GetMoveInfo().SetUseInteria(true, 0.05f);//慣性を使用し、かなり弱くする

				GetMoveInfo().SetMove(CCalculation::Calculation3DVec(GetPosInfo().GetPos(),m_SaveAimPos, 30.0f));//パターン０で狙った位置に突進
				SetPattern(nPattern + 1);//パターン番号を次に進める
				SetPatternTime(0);       //パターン時間をリセットする
			}
			break;
		case 2:
			if (nPatternTime == 60)
			{//このパターンに入って６０フレーム後、攻撃パターンを終了する
				GetMoveInfo().SetUseInteria(false, GetNormalInertia());//慣性をオフに戻す
			    EndAttackPattern();                                    //攻撃パターンを終了する
				ChengeMove(DBG_NEW CEnemyMove_AI());                   //AI移動処理に変える
			}
			break;
		default:
			break;
		}
		SetPatternTime(nPatternTime + 1);//パターン時間をカウントする
	}
}
//============================================================================================================================================

//====================================================================================
//剣の当たり判定を行う
//====================================================================================
void CShotWeakEnemy::SwordCollision()
{
	if (m_pMagicSword != nullptr && CGame::GetPlayer() != nullptr)
	{//剣とプレイヤーが存在していたら
		if (CCollision::RectAngleCollisionXZ(m_pMagicSword, CGame::GetPlayer()))
		{//XZ方向の斜めの当たり判定を行う

			//XZ方向のベクトルを求める
			D3DXVECTOR3 Aim = D3DXVECTOR3(CGame::GetPlayer()->GetPosInfo().GetPos().x, 0.0f, CGame::GetPlayer()->GetPosInfo().GetPos().z) - D3DXVECTOR3(GetPosInfo().GetPos().x, 0.0f, GetPosInfo().GetPos().z);

			//プレイヤーにダメージを与える
			CGame::GetPlayer()->SetDamage(5, 45);

			//ベクトルを正規化
			D3DXVec3Normalize(&Aim, &Aim);

			//プレイヤーにノックバック状態を付与し、上記で求めたベクトルに向かって吹っ飛ばす
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
	{//体力がなくなっていたら
		for (int nCnt = 0; nCnt < 60; nCnt++)
		{//パーティクルを召喚
			CParticle* pParticle = CParticle::Create(CParticle::TYPE::TYPE00_NORMAL, 120, 80.0f, 80.0f, GetPosInfo().GetPos(), CCalculation::Rand3DVec(200, 10),
				D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), true);
			pParticle->SetUseAddSpeed(true, true, 0.9f);//少しずつ減速させる
		}

		CEnemy::DefeatStaging();//基底処理
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
	CObjectX::PosInfo& PosInfo = GetPosInfo();              //位置情報を取得
	CObjectX::SizeInfo& SizeInfo = GetSizeInfo();           //サイズ情報を取得
	CObjectX::MoveInfo& MoveInfo = GetMoveInfo();           //移動情報を取得
	CObjectX::RotInfo& RotInfo = GetRotInfo();              //向き情報を取得

	const D3DXVECTOR3& Pos = PosInfo.GetPos();              //位置
	const D3DXVECTOR3& SenterPos = PosInfo.GetSenterPos();  //中心位置
	const D3DXVECTOR3& Rot = RotInfo.GetRot();              //向き
	const D3DXVECTOR3& VtxMax = SizeInfo.GetVtxMax();       //最大頂点
	const D3DXVECTOR3& VtxMin = SizeInfo.GetVtxMin();       //最小頂点
	const D3DXVECTOR3& Size = SizeInfo.GetSize();           //サイズ
	D3DXVECTOR3 RayOrigin = SenterPos + D3DXVECTOR3(sinf(Rot.y) * (Size.x / 2), 0.0f, cosf(Rot.y) * (Size.x / 2));//レイの支点は向いている方向の自分の端

	D3DXVECTOR3 RayDir = RayOrigin - SenterPos;             //レイのベクトルを求める
	D3DXVec3Normalize(&RayDir, &RayDir);                    //ベクトルを正規化
	D3DXVECTOR3 RayCollisionPos = { 0.0f,0.0f,0.0f };       //レイが当たったところ格納用
	float fLength = 0.0f;                                   //距離格納用
	//=====================================================================================

	for (int nCntPri = 0; nCntPri < CObject::m_nMAXPRIORITY; nCntPri++)
	{//オブジェクトリストを検索
		if (nCntPri == static_cast<int>(CObject::TYPE::BLOCK) || nCntPri == static_cast<int>(CObject::TYPE::BGMODEL))
		{//リストがブロック又は背景モデルだったら
			CObject* pObj = CObject::GetTopObject(nCntPri);//先頭アドレスを取得
			while (pObj != nullptr)
			{//オブジェクトがnullptrになるまで繰り返す
				CObject* pNext = pObj->GetNextObject();//次のオブジェクトを取得

				CObjectX* pObjX = static_cast<CObjectX*>(pObj);//オブジェクトXにキャスト

				//=======================
				//比較対象の変数
				//=======================
				CObjectX::PosInfo& ComPosInfo = pObjX->GetPosInfo();   //位置情報を取得
				CObjectX::RotInfo& ComRotInfo = pObjX->GetRotInfo();   //向き情報を取得
				CObjectX::SizeInfo& ComSizeInfo = pObjX->GetSizeInfo();//サイズ情報を取得

				const D3DXVECTOR3& ComPos = ComPosInfo.GetPos();       //位置
				const D3DXVECTOR3& ComRot = ComRotInfo.GetRot();       //向き
				const D3DXVECTOR3& ComVtxMax = ComSizeInfo.GetVtxMax();//最大頂点
				const D3DXVECTOR3& ComVtxMin = ComSizeInfo.GetVtxMin();//最小頂点
				const D3DXVECTOR3& ComSize = ComSizeInfo.GetSize();    //サイズ
				//=====================================================================================

				if (CCollision::RayIntersectsAABBCollisionPos(RayOrigin, RayDir, ComPos + ComVtxMin, ComPos + ComVtxMax, RayCollisionPos))
				{//レイがオブジェクトに当たっていたら
					fLength = CCalculation::CalculationLength(RayOrigin, RayCollisionPos);//レイの支店とレイが当たった位置を出す
					if (fLength < 100.0f)
					{
						float fLengthY = (ComPos.y + ComVtxMax.y) - Pos.y;//Y軸の距離を取り、目標のジャンプ距離を求める

						if (GetJumpCoolTime() > 200)
						{//ジャンプクールタイムが200f以上になったら
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
const int CDiveWeakEnemy::s_nATTACK_FREQUENCY = 105;     //攻撃頻度
const float CDiveWeakEnemy::s_fSENSINGRANGE = 2000.0f;   //索敵範囲
const float CDiveWeakEnemy::s_fNORMAL_SPEED = 3.0f;      //通常移動速度
//====================================================================================
//コンストラクタ
//====================================================================================
CDiveWeakEnemy::CDiveWeakEnemy(int nPri, bool bUseintPri, CObject::TYPE type, CObject::OBJECTTYPE ObjType) : CEnemy(nPri, bUseintPri, type, ObjType),
m_DiveWeakEnemyType(DIVEWEAKENEMYTYPE::NORMAL),m_nDivisionNum(0),m_bStartDeath(false)
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
	CEnemy::Init();                           //敵の初期化処理
	SetEnemyType(CEnemy::ENEMYTYPE::DIVEWEAK);//敵タイプを設定
	return S_OK;
}
//============================================================================================================================================

//====================================================================================
//終了処理
//====================================================================================
void CDiveWeakEnemy::Uninit()
{
	CEnemy::Uninit();//敵の終了処理
}
//============================================================================================================================================

//====================================================================================
//更新処理
//====================================================================================
void CDiveWeakEnemy::Update()
{
	CEnemy::Update();//敵の更新処理
}
//============================================================================================================================================

//====================================================================================
//描画処理
//====================================================================================
void CDiveWeakEnemy::Draw()
{
	CEnemy::Draw();//敵の描画処理
}
//============================================================================================================================================

//====================================================================================
//死亡フラグ設定処理
//====================================================================================
void CDiveWeakEnemy::SetDeath()
{
	if (m_nDivisionNum > 0 && GetDefeatAttackType() == CAttack::ATTACKTYPE::BULLET && GetLifeInfo().GetLife() < 1 && m_bStartDeath == false)
	{ //分裂回数が０回以上で倒された攻撃のタイプが「弾」で体力がなくなっていたら
		float fPosX = static_cast<float>(rand() % 30 - 15);//X軸の召喚位置
		float fPosZ = static_cast<float>(rand() % 30 - 15);//Z軸の召喚位置
		m_nDivisionNum--;                                  //分裂回数減少
		if (m_nDivisionNum == 1)
		{//残り分裂回数が１の場合、だいぶ敵が増えてしまっているので、何もしない敵に分裂させる
			CIdleEnemy* pIdleEnemy = CIdleEnemy::Create(CIdleEnemy::IDLEENEMYTYPE::NORMAL, GetLifeInfo().GetMaxLife() / 2, 0, GetPosInfo().GetPos() + D3DXVECTOR3(fPosZ, 100.0f, fPosX), GetRotInfo().GetRot(), GetSizeInfo().GetScale() / 2);
			pIdleEnemy->SetSensingRange(9999.0f);//索敵範囲を9999に設定
			pIdleEnemy = CIdleEnemy::Create(CIdleEnemy::IDLEENEMYTYPE::NORMAL, GetLifeInfo().GetMaxLife() / 2, 0, GetPosInfo().GetPos() + D3DXVECTOR3(fPosZ, 100.0f, fPosX), GetRotInfo().GetRot(), GetSizeInfo().GetScale() / 2);
			pIdleEnemy->SetSensingRange(9999.0f);//索敵範囲を9999に設定
		}
		else
		{//残り分裂回数がまだ多いので、そんなに分裂していないとみなし、ダイブに弱い敵に分裂させる
			CDiveWeakEnemy* pDiveWeakEnemy = CDiveWeakEnemy::Create(DIVEWEAKENEMYTYPE::NORMAL, GetLifeInfo().GetMaxLife() / 2, 0, GetPosInfo().GetPos() + D3DXVECTOR3(fPosX, 100.0f, fPosZ), GetRotInfo().GetRot(), GetSizeInfo().GetScale() / 2, m_nDivisionNum);
			pDiveWeakEnemy->SetSensingRange(9999.0f);//索敵範囲を9999に設定
			pDiveWeakEnemy->SetDivisionNum(m_nDivisionNum);//分裂回数を設定
			pDiveWeakEnemy = CDiveWeakEnemy::Create(DIVEWEAKENEMYTYPE::NORMAL, GetLifeInfo().GetMaxLife() / 2, 0, GetPosInfo().GetPos() + D3DXVECTOR3(fPosZ, 100.0f, fPosX), GetRotInfo().GetRot(), GetSizeInfo().GetScale() / 2, m_nDivisionNum);
			pDiveWeakEnemy->SetSensingRange(9999.0f);//索敵範囲を9999に設定
			pDiveWeakEnemy->SetDivisionNum(m_nDivisionNum);//分裂回数を設定

			//パーティクルを召喚
			CParticle::SummonParticle(CParticle::TYPE::TYPE00_NORMAL, 30, 50, 30.0f, 30.0f, 100, 10, true, GetPosInfo().GetPos(), D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), true);
		}
	}

	CEnemy::SetDeath();//敵の死亡フラグを設定
}
//============================================================================================================================================

//====================================================================================
//生成処理
//====================================================================================
CDiveWeakEnemy* CDiveWeakEnemy::Create(DIVEWEAKENEMYTYPE Type, int nLife, int nPhaseNum, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 Scale, int nDivisionNum)
{
	CDiveWeakEnemy* pDiveWeakEnemy = DBG_NEW CDiveWeakEnemy;//ダイブに弱い敵を生成

	pDiveWeakEnemy->Init();                                                                    //初期化処理
	int nType = static_cast<int>(Type);//タイプ番号を格納
	if (nType < 0 || nType >= static_cast<int>(DIVEWEAKENEMYTYPE::MAX))
	{//例外処理
		assert("配列外アクセス！(CDiveWeakEnemy)");
	}
	else
	{
		//情報を登録、読み込み
		int nIdx = CManager::GetObjectXInfo()->Regist(s_aDIVEWEAKENEMY_FILENAME[nType]);
		pDiveWeakEnemy->BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(nIdx),
			CManager::GetObjectXInfo()->GetBuffMat(nIdx),
			CManager::GetObjectXInfo()->GetdwNumMat(nIdx),
			CManager::GetObjectXInfo()->GetTexture(nIdx),
			CManager::GetObjectXInfo()->GetColorValue(nIdx));
	}
	pDiveWeakEnemy->SetPhaseNum(nPhaseNum);                                                    //フェーズ番号を設定する
	pDiveWeakEnemy->m_DiveWeakEnemyType = Type;                                                //ダイブに弱い敵の種類を設定
	pDiveWeakEnemy->GetLifeInfo().SetLife(nLife);                                              //体力
	pDiveWeakEnemy->GetLifeInfo().SetMaxLife(nLife);                                           //最大体力
	pDiveWeakEnemy->GetPosInfo().SetPos(pos);                                                  //位置
	pDiveWeakEnemy->GetPosInfo().SetSupportPos(pos);                                           //支点位置
	pDiveWeakEnemy->GetRotInfo().SetRot(rot);                                                  //向き
	pDiveWeakEnemy->GetSizeInfo().SetScale(Scale);                                             //拡大率
	pDiveWeakEnemy->GetSizeInfo().SetFormarScale(Scale);                                       //元の拡大率を設定
	pDiveWeakEnemy->SetSensingRange(550.0f);                                                   //感知射程
	pDiveWeakEnemy->SetNormalSpeed(s_fNORMAL_SPEED);                                           //通常移動速度
	pDiveWeakEnemy->GetMoveInfo().SetUseInteria(false, GetNormalInertia());                    //慣性を使用しない

	//召喚されてからの時間をランダムに設定（召喚されてからの時間が一定に達したら弾を発射するのでランダム性を持たせるため
	pDiveWeakEnemy->SetCntTime(rand() % 100 + 1);                

	pDiveWeakEnemy->SetDivisionNum(3);                                                         //分裂回数を設定
	pDiveWeakEnemy->GetLifeInfo().SetAutoDeath(true);                                          //体力が０になったら自動的に死亡フラグを設定する

	pDiveWeakEnemy->SetSize();                                                                 //モデルサイズを設定
	pDiveWeakEnemy->GetLifeInfo().SetAutoSubLife(false);                                       //自動的に体力を減らすかどうか
	pDiveWeakEnemy->SetManagerObjectType(CObject::MANAGEROBJECTTYPE::DIVEWEAKENEMY);           //マネージャーで呼び出す時の種類を設定
	return pDiveWeakEnemy;
}
//============================================================================================================================================

//====================================================================================
//テキストファイルに情報を保存する
//====================================================================================
void CDiveWeakEnemy::SaveInfoTxt(fstream& WritingFile)
{
	WritingFile << "SETDIVEWEAKENEMY" << endl;//ダイブに弱い敵の情報を設定
	WritingFile << "DIVEWEAKENEMYTYPE = " << static_cast<int>(m_DiveWeakEnemyType);//ダイブに弱い敵のタイプ番号を書き出す
	switch (m_DiveWeakEnemyType)
	{//ダイブに弱い敵のタイプ名を書き出す
	case DIVEWEAKENEMYTYPE::NORMAL:
		WritingFile << " # NORMAL" << endl;//普通
		break;
	default:
		assert(false);
		break;
	}

	WritingFile << "DIVISIONNUM = " << m_nDivisionNum << endl; //分裂回数を書き出す

	CEnemy::SaveInfoTxt(WritingFile);                          //敵の情報を書き出す

	WritingFile << "END_SETDIVEWEAKENEMY" << endl;             //ダイブに弱い敵の書き出しを終了する
}
//============================================================================================================================================

////====================================================================================
////テキストファイルから情報を読み込む
////====================================================================================
//void CDiveWeakEnemy::LoadInfoTxt(fstream& LoadingFile, list<CObject*>& listSaveManager, string& Buff)
//{
//	int nType = 0;                                           //種類
//	int nDiveWeakEnemyType = 0;                              //ダイブに弱い敵タイプ   
//	int nLife = 0;                                           //体力
//	int nPhaseNum = 0;                                       //フェーズ番号
//	int nDivisionNum = 0;                                    //分裂回数
// 	D3DXVECTOR3 Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);        //移動量
//	D3DXVECTOR3 Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);         //位置
//	D3DXVECTOR3 Scale = D3DXVECTOR3(0.0f, 0.0f, 0.0f);       //拡大率
//	D3DXVECTOR3 Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);         //向き
//    DIVEWEAKENEMYTYPE DiveWeakEnemyType = {};                //背景モデルの種類
//	ENEMYTYPE EnemyType = {};                                //敵タイプ
//														     
//	vector<CAIModel*> VecMoveAi = {};                        //AIモデルの動的配列
//	vector<MoveAiInfo> VecMoveAiInfo = {};                   //AIモデル情報の動的配列
//
//	int nCntMoveAi = 0;                                      //移動AIの数カウント用
//	D3DXVECTOR3 MoveAiPos = D3DXVECTOR3(0.0f,0.0f,0.0f);     //移動AIの位置
//	D3DXVECTOR3 MoveAiRot = D3DXVECTOR3(0.0f,0.0f,0.0f);     //移動AIの向き
//	D3DXVECTOR3 MoveAiScale = D3DXVECTOR3(0.0f,0.0f,0.0f);   //移動AIの拡大率
//
//	float fNormalSpeed = 0.0f;                               //通常速度
//	float fSensingRange = 0.0f;                              //索敵距離
//	while (Buff != "END_SETDIVEWEAKENEMY")
//	{//ダイブに弱い敵の設定が終わるまで繰り返す
//		LoadingFile >> Buff;//単語を読み込む
//		if (Buff == "#")
//		{//行のスキップ
//			getline(LoadingFile, Buff);
//		}
//		else if (Buff == "DIVEWEAKENEMYTYPE")
//		{//ダイブに弱い敵のタイプ
//			LoadingFile >> Buff;//イコール
//			LoadingFile >> nDiveWeakEnemyType;
//		}
//		else if (Buff == "ENEMYTYPE")
//		{//敵のタイプ
//			LoadingFile >> Buff;//イコール
//			LoadingFile >> nType;
//		}
//		else if (Buff == "LIFE")
//		{//体力
//			LoadingFile >> Buff;//イコール
//			LoadingFile >> nLife;
//		}
//		else if (Buff == "POS")
//		{//位置
//			LoadingFile >> Buff;//イコール
//			LoadingFile >> Pos.x;      //位置X
//			LoadingFile >> Pos.y;      //位置Y
//			LoadingFile >> Pos.z;      //位置Z
//		}
//		else if (Buff == "ROT")
//		{//向き
//			LoadingFile >> Buff;//イコール
//			LoadingFile >> Rot.x;      //位置X
//			LoadingFile >> Rot.y;      //位置Y
//			LoadingFile >> Rot.z;      //位置Z
//		}
//		else if (Buff == "SCALE")
//		{//拡大率
//			LoadingFile >> Buff;//イコール
//			LoadingFile >> Scale.x;      //拡大率X
//			LoadingFile >> Scale.y;      //拡大率Y
//			LoadingFile >> Scale.z;      //拡大率Z
//		}
//		else if (Buff == "PHASENUM")
//		{//フェーズ番号
//			LoadingFile >> Buff;      //イコール
//			LoadingFile >> nPhaseNum; //フェーズ番号
//		}
//		else if (Buff == "NORMALSPEED")
//		{//通常移動速度
//			LoadingFile >> Buff;//イコール
//			LoadingFile >> fNormalSpeed;//通常速度
//		}
//		else if (Buff == "SENSINGRANGE")
//		{//索敵範囲
//			LoadingFile >> Buff;//イコール
//			LoadingFile >> fSensingRange;
//		}
//		else if (Buff == "DIVISIONNUM")
//		{//分裂回数
//			LoadingFile >> Buff;//イコール
//			LoadingFile >> nDivisionNum;//分裂回数
//		}
//		else if (Buff == "SETMOVEAI")
//		{//移動AIの設定
//			while (1)
//			{
//				LoadingFile >> Buff;
//				if (Buff == "SETNUM")
//				{//数
//					LoadingFile >> Buff;//イコール
//					LoadingFile >> nCntMoveAi;//番号
//					while (1)
//					{
//						LoadingFile >> Buff;
//
//						if (Buff == "POS")
//						{//位置
//							LoadingFile >> Buff;//イコール
//							LoadingFile >> MoveAiPos.x;      //位置X
//							LoadingFile >> MoveAiPos.y;      //位置Y
//							LoadingFile >> MoveAiPos.z;      //位置Z
//						}
//						else if (Buff == "ROT")
//						{//向き
//							LoadingFile >> Buff;//イコール
//							LoadingFile >> MoveAiRot.x;      //位置X
//							LoadingFile >> MoveAiRot.y;      //位置Y
//							LoadingFile >> MoveAiRot.z;      //位置Z
//						}
//						else if (Buff == "SCALE")
//						{//拡大率
//							LoadingFile >> Buff;//イコール
//							LoadingFile >> MoveAiScale.x;      //拡大率X
//							LoadingFile >> MoveAiScale.y;      //拡大率Y
//							LoadingFile >> MoveAiScale.z;      //拡大率Z
//						}
//						else if (Buff == "END_SETNUM")
//						{//移動AIの設定
//							if (CScene::GetMode() == CScene::MODE_EDIT)
//							{//エディットモードの時は普通に全ての移動AIを出したいため直接動的配列に格納
//								CAIModel* pAiModel = CAIModel::Create(CAIModel::AIMODELTYPE::MOVEPOINT, MoveAiPos, MoveAiRot, MoveAiScale, nullptr);//AIモデルの生成
//								pAiModel->GetDrawInfo().SetUseDraw(false);  //描画しない
//								pAiModel->GetDrawInfo().SetUseShadow(false);//影を描画しない
//								VecMoveAi.push_back(pAiModel);              //移動AIの動的配列に格納
//							}
//							else if (CScene::GetMode() == CScene::MODE_GAME)
//							{//ゲームモードのときはまだ呼ばれていない敵の場合、移動AI自体を存在させたくないだけデータだけを格納し、呼ばれたときにPhaseManagerに保存したデータを使用して移動AIを召喚する
//								MoveAiInfo Info = {};          //移動AI情報
//								Info.Pos = MoveAiPos;          //位置
//								Info.Rot = MoveAiRot;          //向き
//								Info.Scale = MoveAiScale;      //拡大率
//								VecMoveAiInfo.push_back(Info); //動的配列に格納
//							}
//							break;
//						}
//					}
//				}
//				else if (Buff == "END_SETMOVEAI")
//				{//移動AIの設定終了
//					break;
//				}
//			}
//		}
//	}
//
//	DiveWeakEnemyType = static_cast<DIVEWEAKENEMYTYPE>(nDiveWeakEnemyType);//ダイブに弱い敵のタイプ
//	EnemyType = static_cast<ENEMYTYPE>(nType);                             //敵のタイプ
//	if (CScene::GetMode() == CScene::MODE_EDIT)
//	{//エディットモードなら
//		CDiveWeakEnemy* pDiveWeakEnemy = CDiveWeakEnemy::Create(DiveWeakEnemyType, nLife, nPhaseNum, Pos, Rot, Scale,3);//生成処理
//		pDiveWeakEnemy->GetDrawInfo().SetUseDraw(true);                                                                 //描画する
//		pDiveWeakEnemy->GetDrawInfo().SetUseShadow(true);                                                               //影を描画する
//		pDiveWeakEnemy->SetVecMoveAiInfo(VecMoveAi);                                                                    //移動AIの動的配列を設定
//		pDiveWeakEnemy->SetNormalSpeed(fNormalSpeed);                                                                   //通常移動速度を設定
//		pDiveWeakEnemy->SetSensingRange(fSensingRange);                                                                 //索敵範囲を設定
//		pDiveWeakEnemy->GetMoveInfo().SetUseUpdatePos(true);                                                            //位置を更新する
//		listSaveManager.push_back(pDiveWeakEnemy);                                                                      //ステージマネージャーに情報を保存する
//	}
//	else if (CScene::GetMode() == CScene::MODE_GAME)
//	{//ゲームモードならフェーズマネージャーに情報を保存
//		CGame::GetPhaseManager()->PushPhaseInfo(Pos, Rot, Scale, nLife, static_cast<int>(EnemyType), nDiveWeakEnemyType, nPhaseNum,fNormalSpeed,fSensingRange,3,VecMoveAiInfo);
//	}
//}
////============================================================================================================================================

//====================================================================================
//テキストファイルから情報を読み込む
//====================================================================================
void CDiveWeakEnemy::LoadInfoTxt(fstream& LoadingFile, list<CObject*>& listSaveManager, string& Buff, CObject* pObj)
{
	int nDiveWeakEnemyType = 0;                                            //ダイブに弱い敵タイプ   
	int nDivisionNum = 0;                                                  //分裂回数
	DIVEWEAKENEMYTYPE DiveWeakEnemyType = {};                              //ダイブに弱い敵タイプ
	
	while (Buff != "END_SETDIVEWEAKENEMY")
	{//文字列がEND_SETSHOTWEAKENEMYになるまで繰り返す
		LoadingFile >> Buff;//単語を読み込む
		if (Buff == "#")
		{//#ならその行をスキップ
			getline(LoadingFile, Buff);
		}
		else if (Buff == "DIVEWEAKENEMYTYPE")
		{//射撃に弱い敵の種類
			LoadingFile >> Buff;//イコール
			LoadingFile >> nDiveWeakEnemyType;
			DiveWeakEnemyType = static_cast<DIVEWEAKENEMYTYPE>(nDiveWeakEnemyType);//ダイブに弱い敵のタイプを格納
			SetDiveWeakEnemyType(DiveWeakEnemyType);//ダイブに弱い敵のタイプを設定
		}
		else if (Buff == "DIVISIONNUM")
		{//分裂回数
			LoadingFile >> Buff;         //イコール
			LoadingFile >> nDivisionNum; //分裂回数
			SetDivisionNum(nDivisionNum);//分裂回数を設定
		}
		else if (Buff == "SETENEMY")
		{//敵の情報を設定
			CEnemy::LoadInfoTxt(LoadingFile, listSaveManager, Buff, pObj);
		}
	}

	if (CScene::GetMode() == CScene::MODE_EDIT)
	{//エディットモードだったら
		listSaveManager.push_back(this);//ステージマネージャーに情報を保存する
	}
	else if (CScene::GetMode() == CScene::MODE_GAME)
	{//ゲームモードではフェーズマネージャーで召喚するので一旦消す
		SetUseDeath(true);
		SetDeath();
	}
}

//====================================================================================
//ステージマネージャーのオブジェクトをチェンジする
//====================================================================================
CObject* CDiveWeakEnemy::ManagerChengeObject(bool bAim)
{
	int nNewType = int(m_DiveWeakEnemyType);  //現在のタイプをintにキャスト
	DIVEWEAKENEMYTYPE NewType = {};           //新しいタイプ格納用
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
	SetUseDeath(true);//死亡フラグを使用する
	SetDeath();       //死亡フラグを設定する
	//======================================================================================

	return CDiveWeakEnemy::Create(NewType, GetLifeInfo().GetLife(),GetPhaseNum(), GetPosInfo().GetPos(), GetRotInfo().GetRot(), GetSizeInfo().GetScale(),GetDivisionNum());//生成したオブジェクトを返す
}
//============================================================================================================================================

//====================================================================================
//ステージマネージャーに現在のオブジェクトを保存する
//====================================================================================
CObject* CDiveWeakEnemy::ManagerSaveObject()
{
	auto& Vec = GetVecAiModelInfo();//ステージマネージャーで操作している敵のAIモデル情報の動的配列を取得
	auto Vec2 = move(Vec);          //ステージマネージャーで操作している方のAIモデル情報はもういらないので、生成する敵のために移動させておく
	CDiveWeakEnemy * pDiveWeakEnemy = CDiveWeakEnemy::Create(m_DiveWeakEnemyType, GetLifeInfo().GetMaxLife(),GetPhaseNum(), GetPosInfo().GetPos(), GetRotInfo().GetRot(), GetSizeInfo().GetScale(),GetDivisionNum());//生成したオブジェクトを返す
	pDiveWeakEnemy->SetSensingRange(GetSensingRange());//現在の敵の索敵範囲を保存する
	pDiveWeakEnemy->SetNormalSpeed(GetNormalSpeed());  //現在の敵の通常速度を保存する
	pDiveWeakEnemy->SetVecMoveAiInfo(Vec2);            //移動AI情報の動的配列を設定
	return pDiveWeakEnemy;                             //生成したオブジェクトを返す
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
		ChengeMove(DBG_NEW CEnemyMove_None());       //攻撃処理中は、移動も入るので、通常移動処理はさせない。
		SetAction(true);                             //攻撃を開始
	}
	if (bAction == true)
	{//攻撃を開始している場合
		//==============
		//使用変数
		//==============
		CObjectX::RotInfo& RotInfo = GetRotInfo();                          //向き情報を取得
		CObjectX::PosInfo& PosInfo = GetPosInfo();                          //位置情報を取得
		CObjectX::SizeInfo& SizeInfo = GetSizeInfo();                       //サイズ情報を取得
		CObjectX::MoveInfo& MoveInfo = GetMoveInfo();                       //移動情報を取得
		CPlayer* pPlayer = CGame::GetPlayer();                              //プレイヤーのポインタを取得
		CObjectX::PosInfo& PlayerPosInfo = pPlayer->GetPosInfo();           //プレイヤーの位置情報を取得
		const D3DXVECTOR3& Pos = PosInfo.GetPos();                          //位置を取得
		const D3DXVECTOR3& Move = MoveInfo.GetMove();                       //移動量を取得
		const D3DXVECTOR3& PlayerPos = PlayerPosInfo.GetPos();              //プレイヤーの位置を取得
		D3DXVECTOR3 Rot = RotInfo.GetRot();                                 //向き
		const float & fNormalSpeed = GetNormalSpeed();                      //通常移動速度
		float fRotAim = atan2f(PlayerPos.x - Pos.x, PlayerPos.z - Pos.z);   //Z方向を基準にプレイヤーへの角度（目的の角度）を計算する
		//========================================================================================================

		RotInfo.SetRot(D3DXVECTOR3(Rot.x, CCalculation::CalculationCollectionRot2D(Rot.y, fRotAim, 0.007f, false), Rot.z));//向きをプレイヤーへ超少しずつ合わせていく
		MoveInfo.SetMove(D3DXVECTOR3(sinf(fRotAim) * fNormalSpeed, Move.y, cosf(fRotAim) * fNormalSpeed));                 //プレイヤーに向かって移動させる

		if (GetCntTime() % s_nATTACK_FREQUENCY == 0)
		{//攻撃を発射

		    //==============
		    //使用変数
		    //==============
			const D3DXVECTOR3& VtxMax = SizeInfo.GetVtxMax();                          //最大頂点を取得
			D3DXVECTOR3 Aim = { 0.0f,0.0f,0.0f };                                      //狙う方向
			D3DXVECTOR3 ShotPos = Pos + D3DXVECTOR3(0.0f, VtxMax.y, 0.0f);             //発射位置
			D3DXVECTOR3 AddRot = { 0.0f,0.0f,0.0f };                                   //加算する向き
			D3DXVECTOR2 YawPitch = CCalculation::VectorToYawPitch(ShotPos, PlayerPos); //目的地への角度(YawとPitch)を取得
			//========================================================================================================

			//Yawの補正（狙える方向に制限をつける)
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
		    
		    //Pitchの補正(狙える方向に制限をつける)
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
		    	1, 60, 200, ShotPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), Aim, GetSizeInfo().GetScale() * 0.5f);//生成処理
		    
		    pAttackEnemy->GetBoundInfo().SetActive(true, D3DXVECTOR3(0.0f, 10.0f, 0.0f), true, 0.5f);     //バウンドさせる
		    pAttackEnemy->SetExtrusionCollisioin(true);                                                   //押し出し判定を行い、
		    pAttackEnemy->SetHitOtherThanLibing(false);                                                   //敵やプレイヤー以外との当たり判定は行わない
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
	{//ゲームモードならAI移動をする
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
	{//体力がなくなっていたら
		for (int nCnt = 0; nCnt < 60; nCnt++)
		{
			//パーティクルを召喚
			CParticle* pParticle = CParticle::Create(CParticle::TYPE::TYPE00_NORMAL, 120, 80.0f, 80.0f, GetPosInfo().GetPos(), CCalculation::Rand3DVec(200, 10),
				D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), true);

			//パーティクルを少しずつ原則させる
			pParticle->SetUseAddSpeed(true, true, 0.9f);
		}

		CEnemy::DefeatStaging();//基底処理
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
	pEnemy->BattleMoveProcess();//戦い移動処理を行う
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
	D3DXVECTOR3 ScreenPos = D3DXVECTOR3(0.0f, 0.0f,0.0f);//スクリーン座標格納用

	//敵のスクリーン座標を取得する
	ScreenPos = CCalculation::CalcWorldToScreenNoViewport(pEnemy->GetPosInfo().GetSenterPos(), *CManager::GetCamera()->GetMtxView(), *CManager::GetCamera()->GetMtxProjection(),
		SCREEN_WIDTH,SCREEN_HEIGHT);

	pEnemy->SetState(CEnemy::STATE::FRIGHTENDED);//怯え状態にする

	//ロックオン表示を上記で求めたスクリーン座標に召喚する
	m_pLockOn = CUi::Create(CUi::UITYPE::TARGET_000,false,CObject2D::POLYGONTYPE::SENTERROLLING, 200.0f, 200.0f, 100, true, D3DXVECTOR3(ScreenPos.x, ScreenPos.y, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f));

	pEnemy->SetAction(false);        //攻撃をオフにする
	pEnemy->SetPossibleAttack(false);//攻撃を不能にする
	 
	m_pLockOn->SetPolygonRotSpeed(0.1f);                         //ロックオン表示の回転速度を設定
	m_pLockOn->SetUseAddScale(D3DXVECTOR2(-0.01f, -0.01f), true);//ロックオン表示を少しずつ縮小する
	m_pLockOn->SetUseDeath(false);                               //ロックオン表示の死亡フラグを使用しない
}
//============================================================================================================================================

//====================================================================================
//デストラクタ
//====================================================================================
CEnemyMove_Frightened::~CEnemyMove_Frightened()
{
	if (m_pLockOn != nullptr)
	{//ロックオン表示が存在していたら
		m_pLockOn->SetUseDeath(true);//死亡フラグを使用する
		m_pLockOn->SetDeath();       //死亡フラグを設定する
		m_pLockOn = nullptr;         //ロックオン表示のポインタを初期化
	}
}
//============================================================================================================================================

//====================================================================================
//処理
//====================================================================================
void CEnemyMove_Frightened::Process(CEnemy* pEnemy)
{
	//怯えている間は怯え始めた位置を中心にランダムに位置を設定する
	float fX = static_cast<float>(rand() % 50 - 25);
	float fY = static_cast<float>(rand() % 50 - 25);
	float fZ = static_cast<float>(rand() % 50 - 25);
	pEnemy->GetPosInfo().SetPos(m_StopPos + D3DXVECTOR3(fX, fY, fZ)); //怯えさせる
	pEnemy->GetMoveInfo().SetMove(D3DXVECTOR3(0.0f,0.0f,0.0f));       //移動量を0にする
	m_nStateTime--;                                                   //この状態になっている時間をカウントダウンする
	pEnemy->EndAttackPattern();                                       //怯え状態の時は行動不能にしたいので、攻撃パターンを終了させつづける

	if (m_pLockOn != nullptr)
	{
		D3DXVECTOR3 ScreenPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//スクリーン座標格納用

		//敵の中心位置のスクリーン座標を取得
		ScreenPos = CCalculation::CalcWorldToScreenNoViewport(pEnemy->GetPosInfo().GetSenterPos(), *CManager::GetCamera()->GetMtxView(), *CManager::GetCamera()->GetMtxProjection(),
			SCREEN_WIDTH, SCREEN_HEIGHT);

		m_pLockOn->SetPos(ScreenPos);//怯え状態にした敵のスクリーン座標にUIを表示
		if (m_pLockOn->GetScale().x < 0.0f && m_pLockOn->GetScale().y < 0.0f)
		{//拡大率が０以下になったら
			m_pLockOn->SetUseDeath(true);//死亡フラグを使用する
			m_pLockOn->SetDeath();       //死亡フラグを設定する
			m_pLockOn = nullptr;         //ロックオン表示を初期化する
		}
	}

	if (m_nStateTime < 1)
	{//この状態になる残り時間がなくなったら
		pEnemy->SetPossibleAttack(true);            //攻撃を可能に戻す
		pEnemy->GetPosInfo().SetPos(m_StopPos);     //怯え状態になった位置に設定
		pEnemy->SetState(CEnemy::STATE::NORMAL);    //状態異常を普通に戻す
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
	m_DodgeMove = DodgeMove;//壁を回避する移動量を設定
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
	pEnemy->GetMoveInfo().SetMove(m_DodgeMove);//移動量を設定する

	if (pEnemy->GetCollisionWall() == false)
	{//壁に当たらなくなっていたら攻撃移動処理に戻す
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
	CEnemy::Init();//敵の初期化処理
	return S_OK;
}
//============================================================================================================================================

//====================================================================================
//終了処理
//====================================================================================
void CIdleEnemy::Uninit()
{
	CEnemy::Uninit();//敵の終了処理
}
//============================================================================================================================================

//====================================================================================
//更新処理
//====================================================================================
void CIdleEnemy::Update()
{
	CEnemy::Update();//敵の更新処理
}
//============================================================================================================================================

//====================================================================================
//描画処理
//====================================================================================
void CIdleEnemy::Draw()
{
	CEnemy::Draw();//敵の描画処理
}
//============================================================================================================================================

//====================================================================================
//死亡フラグ設定処理
//====================================================================================
void CIdleEnemy::SetDeath()
{
	CEnemy::SetDeath();//敵の死亡フラグ設定処理
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
	int nType = static_cast<int>(Type);//タイプ番号を格納
	if (nType < 0 || nType >= static_cast<int>(IDLEENEMYTYPE::MAX))
	{//例外処理
		assert("配列外アクセス！(CIdleEnemy)");
	}
	else
	{
		//情報を登録、読み込み
		int nIdx = CManager::GetObjectXInfo()->Regist(s_aIDLEENEMY_FILENAME[nType]);
		pIdleEnemy->BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(nIdx),
			CManager::GetObjectXInfo()->GetBuffMat(nIdx),
			CManager::GetObjectXInfo()->GetdwNumMat(nIdx),
			CManager::GetObjectXInfo()->GetTexture(nIdx),
			CManager::GetObjectXInfo()->GetColorValue(nIdx));
	}
	//ステータスを設定
	pIdleEnemy->SetPhaseNum(nPhaseNum);                                     //フェーズ番号を設定する
	pIdleEnemy->SetEnemyType(CEnemy::ENEMYTYPE::IDLE);                      //敵の種類を設定する
	pIdleEnemy->m_IdleEnemyType = Type;                                     //何もしない敵のタイプを設定する
	pIdleEnemy->GetLifeInfo().SetLife(nLife);                               //体力
	pIdleEnemy->GetLifeInfo().SetMaxLife(nLife);                            //最大体力
	pIdleEnemy->GetPosInfo().SetPos(pos);                                   //位置
	pIdleEnemy->GetPosInfo().SetSupportPos(pos);                            //支点位置
	pIdleEnemy->GetRotInfo().SetRot(rot);                                   //向き
	pIdleEnemy->GetSizeInfo().SetScale(Scale);                              //拡大率
	pIdleEnemy->GetSizeInfo().SetFormarScale(Scale);                        //元の拡大率を設定
	pIdleEnemy->SetSensingRange(550.0f);                                    //感知射程
	pIdleEnemy->SetNormalSpeed(3.0f);                                       //通常移動速度
	pIdleEnemy->GetMoveInfo().SetUseInteria(false, GetNormalInertia());     //慣性を設定（通常値）
	pIdleEnemy->SetCntTime(rand() % 100 + 1);                               //攻撃タイミングをずらす
	pIdleEnemy->GetLifeInfo().SetAutoDeath(true);                           //体力が０になったときに死亡フラグを発動
	pIdleEnemy->GetLifeInfo().SetAutoSubLife(false);                        //体力を減らし続けない
	pIdleEnemy->SetSize();                                                  //モデルサイズを設定
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

////====================================================================================
////情報ロード処理
////===================================================================================
//void CIdleEnemy::LoadInfoTxt(fstream& LoadingFile, list<CObject*>& listSaveManager, string& Buff)
//{
//	int nType = 0;                                           //種類
//	int nIdleEnemyType = 0;                                  //何もしない敵タイプ   
//	int nLife = 0;                                           //体力
//	int nPhaseNum = 0;                                       //フェーズ番号
//	D3DXVECTOR3 Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);        //移動量
//	D3DXVECTOR3 Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);         //位置
//	D3DXVECTOR3 Scale = D3DXVECTOR3(0.0f, 0.0f, 0.0f);       //拡大率
//	D3DXVECTOR3 Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);         //向き
//	IDLEENEMYTYPE IdleEnemyType = IDLEENEMYTYPE::NORMAL;     //何もしない敵の種類
//	ENEMYTYPE EnemyType = {};                                //敵の分類
//														     
//	vector<CAIModel*> VecMoveAi = {};                        //移動AIの動的配列
//	vector<MoveAiInfo> VecMoveAiInfo = {};                   //移動AI情報の動的配列
//														     
//	int nCntMoveAi = 0;                                      //移動AIの数
//	D3DXVECTOR3 MoveAiPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   //移動AIの位置
//	D3DXVECTOR3 MoveAiRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   //移動AIの向き
//	D3DXVECTOR3 MoveAiScale = D3DXVECTOR3(0.0f, 0.0f, 0.0f); //移動AIの拡大率
//
//	float fNormalSpeed = 0.0f;                               //通常速度
//	float fSensingRange = 0.0f;                              //索敵距離（攻撃モードに入る距離)
//	while (Buff != "END_SETIDLEENEMY")
//	{
//		LoadingFile >> Buff;//単語を読み込む
//		if (Buff == "#")
//		{
//			getline(LoadingFile, Buff);
//		}
//		else if (Buff == "IDLEENEMYTYPE")
//		{
//			LoadingFile >> Buff;//イコール
//			LoadingFile >> nIdleEnemyType;//何もしない敵タイプを読み込む
//		}
//		else if (Buff == "ENEMYTYPE")
//		{
//			LoadingFile >> Buff;//イコール
//			LoadingFile >> nType;//敵の分類を読み込む
//		}
//		else if (Buff == "LIFE")
//		{
//			LoadingFile >> Buff;//イコール
//			LoadingFile >> nLife;//体力を読み込む
//		}
//		else if (Buff == "POS")
//		{
//			LoadingFile >> Buff;//イコール
//			LoadingFile >> Pos.x;      //位置X
//			LoadingFile >> Pos.y;      //位置Y
//			LoadingFile >> Pos.z;      //位置Z
//		}
//		else if (Buff == "ROT")
//		{
//			LoadingFile >> Buff;//イコール
//			LoadingFile >> Rot.x;      //向きX
//			LoadingFile >> Rot.y;      //向きY
//			LoadingFile >> Rot.z;      //向きZ
//		}
//		else if (Buff == "SCALE")
//		{
//			LoadingFile >> Buff;//イコール
//			LoadingFile >> Scale.x;      //拡大率X
//			LoadingFile >> Scale.y;      //拡大率Y
//			LoadingFile >> Scale.z;      //拡大率Z
//		}
//		else if (Buff == "PHASENUM")
//		{
//			LoadingFile >> Buff;      //イコール
//			LoadingFile >> nPhaseNum; //フェーズ番号
//		}
//		else if (Buff == "NORMALSPEED")
//		{
//			LoadingFile >> Buff;//イコール
//			LoadingFile >> fNormalSpeed;//通常速度
//		}
//		else if (Buff == "SENSINGRANGE")
//		{
//			LoadingFile >> Buff;//イコール
//			LoadingFile >> fSensingRange;//索敵範囲を読み込む
//		}
//		else if (Buff == "SETMOVEAI")
//		{//移動AIの情報を読み込む
//			while (1)
//			{
//				LoadingFile >> Buff;
//				if (Buff == "SETNUM")
//				{
//					LoadingFile >> Buff;//イコール
//					LoadingFile >> nCntMoveAi;//番号
//					while (1)
//					{
//						LoadingFile >> Buff;
//
//						if (Buff == "POS")
//						{
//							LoadingFile >> Buff;//イコール
//							LoadingFile >> MoveAiPos.x;      //位置X
//							LoadingFile >> MoveAiPos.y;      //位置Y
//							LoadingFile >> MoveAiPos.z;      //位置Z
//						}
//						else if (Buff == "ROT")
//						{
//							LoadingFile >> Buff;//イコール
//							LoadingFile >> MoveAiRot.x;      //向きX
//							LoadingFile >> MoveAiRot.y;      //向きY
//							LoadingFile >> MoveAiRot.z;      //向きZ
//						}
//						else if (Buff == "SCALE")
//						{
//							LoadingFile >> Buff;//イコール
//							LoadingFile >> MoveAiScale.x;      //拡大率X
//							LoadingFile >> MoveAiScale.y;      //拡大率Y
//							LoadingFile >> MoveAiScale.z;      //拡大率Z
//						}
//						else if (Buff == "END_SETNUM")
//						{
//							if (CScene::GetMode() == CScene::MODE_EDIT)
//							{//エディットモードの場合（エディットモードでは最初から全ての敵と移動AIを表示したいので、生成する）
//								CAIModel* pAiModel = CAIModel::Create(CAIModel::AIMODELTYPE::MOVEPOINT, MoveAiPos, MoveAiRot, MoveAiScale, nullptr);//生成
//								pAiModel->GetDrawInfo().SetUseDraw(true);//描画をする
//								pAiModel->GetDrawInfo().SetUseShadow(true);//影を描画する
//								VecMoveAi.push_back(pAiModel);
//							}
//							else if (CScene::GetMode() == CScene::MODE_GAME)
//							{//ゲームモードの場合（ゲームモードはフェーズ制で進むので、最初から移動AIを生成したいので、フェーズ情報に格納する)
//								MoveAiInfo Info = {};//移動AIの情報を初期化
//								Info.Pos = MoveAiPos;//移動AIの位置
//								Info.Rot = MoveAiRot;//移動AIの向き
//								Info.Scale = MoveAiScale;//移動AIの拡大率
//								VecMoveAiInfo.push_back(Info);//移動AI情報の動的配列に格納
//							}
//							break;
//						}
//					}
//				}
//				else if (Buff == "END_SETMOVEAI")
//				{//移動AIの読み込みを終了する
//					break;
//				}
//			}
//		}
//	}
//
//	IdleEnemyType = static_cast<IDLEENEMYTYPE>(nIdleEnemyType);//int型で読み込んだ何もしない敵のタイプ番号を列挙型にキャストして格納
//	EnemyType = static_cast<ENEMYTYPE>(nType);                 //敵の分類を列挙型にキャスト
//
//	if (CScene::GetMode() == CScene::MODE_EDIT)
//	{//エディットモードの場合（フェーズ制は適用しないので、最初から全ての情報を生成）
//		CIdleEnemy* pIdleEnemy = CIdleEnemy::Create(IdleEnemyType, nLife, nPhaseNum, Pos, Rot, Scale);//生成
//		pIdleEnemy->GetDrawInfo().SetUseDraw(true);        //描画する
//		pIdleEnemy->GetDrawInfo().SetUseShadow(true);      //影を描画する
//		pIdleEnemy->SetVecMoveAiInfo(VecMoveAi);           //移動AI情報を設定
//		pIdleEnemy->SetNormalSpeed(3.0f);                  //通常速度を設定
//		pIdleEnemy->SetSensingRange(fSensingRange);        //索敵範囲を設定
//		pIdleEnemy->GetMoveInfo().SetUseUpdatePos(true);   //位置の更新を行う
//		listSaveManager.push_back(pIdleEnemy);             //vectorに情報を保存する
//	}
//	else if (CScene::GetMode() == CScene::MODE_GAME)
//	{//ゲームモードなら（フェーズ制でゲームが動くので、フェーズ情報に敵の情報を格納）
//		CGame::GetPhaseManager()->PushPhaseInfo(Pos, Rot, Scale, nLife, static_cast<int>(EnemyType),nIdleEnemyType, nPhaseNum,3.0f, fSensingRange, 3, VecMoveAiInfo);
//	}
//}
////============================================================================================================================================

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
	SetUseDeath(true);//死亡フラグを使用する
	SetDeath();       //死亡フラグを設定する
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
//テキストファイルから情報を読み込む
//===================================================================================
void CIdleEnemy::LoadInfoTxt(fstream& LoadingFile, list<CObject*>& listSaveManager, string& Buff, CObject* pObj)
{
	int nIdleWeakEnemyType = 0;                                            //何もしない敵タイプ   
	IDLEENEMYTYPE IdleWeakEnemyType = {};                                  //何もしない敵タイプ
	CIdleEnemy* pIdleEnemy = dynamic_cast<CIdleEnemy*>(pObj);              //何もしない敵にダウンキャスト

	while (Buff != "END_SETIDLEENEMY")
	{//文字列がEND_SETSHOTWEAKENEMYになるまで繰り返す
		LoadingFile >> Buff;//単語を読み込む
		if (Buff == "#")
		{//#ならその行をスキップ
			getline(LoadingFile, Buff);
		}
		else if (Buff == "IDLEENEMYTYPE")
		{//射撃に弱い敵の種類
			LoadingFile >> Buff;//イコール
			LoadingFile >> nIdleWeakEnemyType;
			IdleWeakEnemyType = static_cast<IDLEENEMYTYPE>(nIdleWeakEnemyType);//何もしない敵のタイプを格納
			SetIdleEnemyType(IdleWeakEnemyType);                               //何もしない敵のタイプを設定
		}
		else if (Buff == "SETENEMY")
		{//敵の情報を設定
			CEnemy::LoadInfoTxt(LoadingFile, listSaveManager, Buff, pObj);
		}
	}

	if (CScene::GetMode() == CScene::MODE_EDIT)
	{//エディットモードだったら
		listSaveManager.push_back(this);//ステージマネージャーに情報を保存する
	}
	else if (CScene::GetMode() == CScene::MODE_GAME)
	{//ゲームモードではフェーズマネージャーで召喚するので一旦消す
		SetUseDeath(true);
		SetDeath();
	}
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
	{//ゲームモードなら
		CEnemy::AIMoveProcess();//AI移動処理
	}
}
//============================================================================================================================================

//====================================================================================
//撃破演出
//===================================================================================
void CIdleEnemy::DefeatStaging()
{
	if (GetLifeInfo().GetLife() < 1 && GetUseDeath() == true)
	{//体力がなくなっていて、死亡フラグを使用するなら
		for (int nCnt = 0; nCnt < 60; nCnt++)
		{//パーティクルを召喚
			CParticle* pParticle = CParticle::Create(CParticle::TYPE::TYPE00_NORMAL, 120, 80.0f, 80.0f, GetPosInfo().GetPos(), CCalculation::Rand3DVec(200, 10),
				D3DXCOLOR(0.678f, 1.0f, 0.184f, 1.0f), true);
			pParticle->SetUseAddSpeed(true, true, 0.9f);//徐々に減速させる
		}

		CEnemy::DefeatStaging();//基底処理
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
	CObjectX::PosInfo& PosInfo = pEnemy->GetPosInfo();     //位置情報を取得
	const D3DXVECTOR3& Pos = PosInfo.GetPos();             //位置
	const D3DXVECTOR3& SenterPos = PosInfo.GetSenterPos(); //中心位置
	m_nCntTime++;//この状態になってからの時間をカウントする

	//パーティクルを召喚する
	CParticle::SummonChargeParticle(CParticle::TYPE::TYPE00_NORMAL, 1, 60, 5.0f, 40.0f, 40.0f, 200.0f, 100, 10, true, SenterPos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), true);

	if (m_nCntTime == 100)
	{//100fたったら
		CObjectX::MoveInfo& MoveInfo = pEnemy->GetMoveInfo();//移動情報を取得する

		const D3DXVECTOR3& Move = MoveInfo.GetMove();//移動量

		//移動量を設定
		MoveInfo.SetMove(D3DXVECTOR3(Move.x,CCalculation::GetInitialVelocityHeight(m_fGoalHeight,pEnemy->GetMoveInfo().GetGravity()) + 5.0f,Move.z));
		pEnemy->ResetJumpCoolTime();                      //次にジャンプを発動するまでのクールタイムをリセットする
		pEnemy->ChengeMove(DBG_NEW CEnemyMove_Battle()); //攻撃移動処理に戻す
	}
}
//============================================================================================================================================
//======================================================================================================================
//
//１１月２９日：フェーズマネージャーを作る[phasemanager.cpp]
//Author:ShinaTaiyo
//
//======================================================================================================================

//===============================================================
//インクルード
//===============================================================
#include "phasemanager.h"
#include "aimodel.h"
#include "manager.h"
#include "stagemanager.h"
#include "game.h"
#include "fade.h"
//======================================================================================================================

//===============================================================
//静的メンバ宣言
//===============================================================
list<CPhaseManager::PhaseSaveInfo> CPhaseManager::s_PhaseList = {};//フェーズ情報リスト
int CPhaseManager::s_nNowPhase = 0;                                //現在のフェーズ
int CPhaseManager::s_MaxPhase = 0;                                 //最大フェーズ
int CPhaseManager::s_nNowStage = 0;                                //現在のステージ
bool CPhaseManager::s_bStartFade = false;                          //フェードを開始するかどうか
//======================================================================================================================

//===============================================================
//コンストラクタ
//===============================================================
CPhaseManager::CPhaseManager(int nNumStage, int nPri, bool bUseintPri, CObject::TYPE type, CObject::OBJECTTYPE ObjType) : 
	CObject(nPri, bUseintPri, type, ObjType)
{
	s_nNowPhase = 0;//現在のフェーズを初期化
	s_MaxPhase = 0; //最大フェーズを初期化
	s_nNowStage = 0;//現在のステージを初期化

	if (nNumStage >= static_cast<int>(CStageManager::WORLDTYPE::MAX))
	{//ステージ数を設定
		nNumStage = static_cast<int>(CStageManager::WORLDTYPE::MAX) - 1;
	}
	s_bStartFade = false;
}
//======================================================================================================================

//===============================================================
//デストラクタ
//===============================================================
CPhaseManager::~CPhaseManager()
{

}
//======================================================================================================================

//===============================================================
//初期化処理
//===============================================================
HRESULT CPhaseManager::Init()
{
	CObject::Init();//オブジェクトの初期化処理

	//フェーズ移行イベントを生成
	CEventManager::Create(DBG_NEW CNowEvent_NextPhase(CUi::Create(CUi::UITYPE::STAGETEXT,false, CObject2D::POLYGONTYPE::SENTERROLLING, 200.0f, 100.0f, 100, false,
		D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT / 2 - 180.0f, 0.0f), D3DXVECTOR3(sinf(D3DX_PI * -0.5f) * 15.0f, cosf(D3DX_PI * -0.5f) * 15.0f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)), s_nNowStage, 80.0f, 80.0f));
	return S_OK;
}
//======================================================================================================================

//===============================================================
//終了処理
//===============================================================
void CPhaseManager::Uninit()
{
	CObject::Uninit();//オブジェクトの終了処理
}
//======================================================================================================================

//===============================================================
//更新処理
//===============================================================
void CPhaseManager::Update()
{
	AdvancePhase();//フェーズ移行処理
	CObject::Update();//オブジェクトの更新処理
}
//======================================================================================================================

//===============================================================
//描画処理
//===============================================================
void CPhaseManager::Draw()
{
	CObject::Draw();//オブジェクトの描画処理
}
//======================================================================================================================

//===============================================================
//死亡フラグ設定処理
//===============================================================
void CPhaseManager::SetDeath()
{
	if (GetUseDeath() == true)
	{//死亡フラグを使用するなら
		s_PhaseList.clear();//フェーズ情報をクリアする
	}
	CObject::SetDeath();//オブジェクトの死亡フラグ設定処理
}
//======================================================================================================================

//===============================================================
//フェーズマネージャーに情報を設定する
//===============================================================
void CPhaseManager::PushPhaseInfo(D3DXVECTOR3 Pos, D3DXVECTOR3 Rot, D3DXVECTOR3 Scale, 
	int nLife, int nEnemyType, int nTypeNum, int nPhaseNum, float fNormalSpeed, float fSensingRange, 
	int nNumDivision, vector<CEnemy::MoveAiInfo> VecMoveAi)
{
	PhaseSaveInfo Info = {};                     //フェーズ情報の初期化
	Info.Pos = Pos;                              //位置
	Info.Rot = Rot;                              //向き
	Info.Scale = Scale;                          //拡大率
	Info.nLife = nLife;                          //体力
	Info.nEnemyType = nEnemyType;                //敵のタイプ
	Info.nTypeNum = nTypeNum;                    //タイプ番号
	Info.nPhaseNum = nPhaseNum;                  //フェーズ番号
	Info.VecMoveAi = VecMoveAi;                  //移動AIの動的配列
	Info.fNormalSpeed = fNormalSpeed;            //通常速度
	Info.fSensingRange = fSensingRange;          //索敵範囲
	Info.nNumDivision = nNumDivision;            //分裂回数

	//フェーズ最大数を超えていたら更新
	if (nPhaseNum > s_MaxPhase)
	{
		s_MaxPhase = nPhaseNum;                  //最大フェーズ数を更新
	}

	s_PhaseList.push_back(Info);                 //フェーズリストに格納
}
//======================================================================================================================

//===============================================================
//次のフェーズに移行する処理
//===============================================================
void CPhaseManager::AdvancePhase()
{
	if (s_nNowStage < static_cast<int>(CStageManager::WORLDTYPE::MAX))
	{
		if (CEnemy::GetNumEnemy() <= 0 && s_nNowPhase <= s_MaxPhase)
		{

			CEnemy::ENEMYTYPE EnemyType = {};//敵タイプ格納用 
			CEnemy* pEnemy = nullptr;        //敵へのポインタ
			vector<CAIModel*> VecMoveAi = {};//移動AIの動的配列

			//================================
			//フェーズ情報を反映
			//================================
			for (auto it : s_PhaseList)
			{
				if (it.nPhaseNum == s_nNowPhase)
				{//フェーズリストに格納されている敵情報が現在のフェーズ番号と一致したら
					EnemyType = static_cast<CEnemy::ENEMYTYPE>(it.nEnemyType);//敵タイプを格納

					//移動AIが設定されていたら
					if (it.VecMoveAi.size() > 0)
					{//敵の移動AIの動的配列の中身が存在していたら
						for (const auto it2 : it.VecMoveAi)
						{//移動AIの動的配列の情報をもとに移動AIを生成
							CAIModel* pAiModel = CAIModel::Create(CAIModel::AIMODELTYPE::MOVEPOINT, it2.Pos, it2.Rot, it2.Scale, nullptr);
							pAiModel->GetDrawInfo().SetUseDraw(false);//描画しない
							pAiModel->GetDrawInfo().SetUseShadow(false);//影を描画しない
							VecMoveAi.push_back(pAiModel);//動的配列に情報を格納
						}
					}

					//敵の種類によって生成するものを変える
					switch (EnemyType)
					{
					case CEnemy::ENEMYTYPE::SHOTWEAK://射撃に弱い敵
						pEnemy = CShotWeakEnemy::Create(static_cast<CShotWeakEnemy::SHOTWEAKENEMYTYPE>(it.nTypeNum), it.nLife, it.nPhaseNum, it.Pos, it.Rot, it.Scale);
						break;
					case CEnemy::ENEMYTYPE::DIVEWEAK://ダイブに弱い敵
						pEnemy = CDiveWeakEnemy::Create(static_cast<CDiveWeakEnemy::DIVEWEAKENEMYTYPE>(it.nTypeNum), it.nLife, it.nPhaseNum, it.Pos, it.Rot, it.Scale, it.nNumDivision);
						break;
					case CEnemy::ENEMYTYPE::IDLE:    //何もしない敵
						pEnemy = CIdleEnemy::Create(static_cast<CIdleEnemy::IDLEENEMYTYPE>(it.nTypeNum), it.nLife, it.nPhaseNum, it.Pos, it.Rot, it.Scale);
						break;
					default:
						break;
					}

					pEnemy->SetNormalSpeed(it.fNormalSpeed);  //通常速度を設定
					pEnemy->SetSensingRange(it.fSensingRange);//索敵範囲を設定

					auto CopyVec = move(VecMoveAi);           //上記の情報から格納した移動AIの動的配列を設定させる変数に移動
					pEnemy->SetVecMoveAiInfo(CopyVec);        //移動AIの動的配列を設定
				}

			}
			//====================================================================================================================================================

			s_nNowPhase++;//現在のフェーズ番号を次に進める                                    
			if (s_nNowPhase <= s_MaxPhase)
			{//フェーズ数が最大を超えていなければフェーズ移行イベントを生成
				CEventManager::Create(DBG_NEW CNowEvent_NextPhase(CUi::Create(CUi::UITYPE::PHASETEXT,false,CObject2D::POLYGONTYPE::SENTERROLLING, 200.0f, 100.0f, 100, false,
					D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(sinf(D3DX_PI * -0.5f) * 10.0f, cosf(D3DX_PI * -0.5f) * 10.0f, 0.0f),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)), s_nNowPhase, 80.0f, 80.0f));
			}

		}
		if (CEnemy::GetNumEnemy() <= 0 && s_nNowPhase == s_MaxPhase + 1 && s_bStartFade == false)
		{//敵がいなくなっていて、現在のフェーズが最大フェーズじゃなければ

			s_PhaseList.clear();//次のステージに行くのでフェーズ情報を初期化
			s_nNowPhase = 0;    //次のステージに行くのでフェーズを最初から

			s_nNowStage++;      //ステージ番号を次に進める

			if (s_nNowStage == static_cast<int>(CStageManager::WORLDTYPE::MAX))
			{//配列的にインクリメント後にステージ数になったら終わりなので、リザルトに移行
				CManager::GetSceneFade()->SetSceneFade(CFade::FADEMODE_IN, CScene::MODE_RESULT);
				CGame::SetGameClear(true);//ゲームをクリアした！
			}
			else
			{//それ以外なら次のステージを呼ぶ
				CGame::GetStageManager()->LoadMapTxt(s_nNowStage);//次のステージをロードする
				CGame::GetPlayer()->GetPosInfo().SetPos(CGame::GetStageManager()->GetSpawnPoint());
				CEventManager::Create(DBG_NEW CNowEvent_NextPhase(CUi::Create(CUi::UITYPE::STAGETEXT,false,CObject2D::POLYGONTYPE::SENTERROLLING, 200.0f, 100.0f, 100, false,
					D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT / 2 - 180.0f, 0.0f), D3DXVECTOR3(sinf(D3DX_PI * -0.5f) * 15.0f, cosf(D3DX_PI * -0.5f) * 15.0f, 0.0f),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)), s_nNowStage, 80.0f, 80.0f));
			}
		}
	}
}
//======================================================================================================================

//===============================================================
//生成処理
//===============================================================
CPhaseManager* CPhaseManager::Create()
{
	CPhaseManager* pPhaseManager = DBG_NEW CPhaseManager(3);//フェーズマネージャーを生成

	pPhaseManager->Init();                                  //初期化処理

	return pPhaseManager;
}
//======================================================================================================================
//=============================================================================
//
//１１月１０日：ダイブ実装[player.cpp]
//Author:ShinaTaiyo
//
//=============================================================================

//========================
//インクルード
//========================
#include "player.h"
#include "objectX.h"
#include "renderer.h"
#include "attack.h"
#include "main.h"
#include "manager.h"
#include "camera.h"
#include "input.h"
#include "objectXInfo.h"
#include "calculation.h"
#include "fade.h"
#include "game.h"
#include "block.h"
#include "enemy.h"
#include "lockon.h"
#include "particle.h"
#include "ui.h"
#include "particle2d.h"
#include "debugtext.h"
#include "sound.h"
#include "tutorial.h"
#include "collision.h"
//==========================================================================================================

//====================================================
//静的メンバ宣言
//====================================================
const int CPlayer::s_nNORMAL_MAXLIFE = 100;//プレイヤーの最大体力
const int CPlayer::s_nMaxDiveNum = 5;      //プレイヤーの最大ダイブ数
//==========================================================================================================

//====================================================
//コンストラクタ
//====================================================
CPlayer::CPlayer(CPlayerMove* pPlayerMove, CPlayerAttack* pPlayerAttack,
    int nPri, bool bUseintPri, CObject::TYPE type, CObject::OBJECTTYPE ObjType) : CCharacter(nPri, bUseintPri, type, ObjType)
    , m_pMove(pPlayerMove), m_pAttack(pPlayerAttack),
    m_fRotAim(0.0f), m_pLockOn(nullptr), m_NowActionMode(ACTIONMODE::SHOT), m_pModeDisp(nullptr), m_bCollision(false),m_pWire(nullptr),
    m_pHpGauge(nullptr),m_pAbnormalState(DBG_NEW CPlayerAbnormalState()), m_pDivePossibleNum(nullptr),m_bDamage(false),m_pDiveGaugeFrame(nullptr),
    m_pPlayerActionMode(nullptr)
{

}
//==========================================================================================================

//====================================================
//デストラクタ
//====================================================
CPlayer::~CPlayer()
{

}
//==========================================================================================================

//====================================================
//初期化処理
//====================================================
HRESULT CPlayer::Init()
{
    CCharacter::Init();                 //Xオブジェクト初期化

    GetLifeInfo().SetAutoSubLife(false);//自動的に体力を減らすかどうか
    GetMoveInfo().SetUseGravity(true,1.0f);  //重力を使用する

    if (CScene::GetMode() == CScene::MODE::MODE_GAME)
    {
        m_pLockOn = CLockon::Create(CLockon::TYPE::SHOT,D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), CObject2D::POLYGONTYPE::SENTERROLLING, 100.0f, 100.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
        m_pLockOn->SetUseDeath(false);
        m_pLockOn->SetPolygonRotSpeed(0.01f);

        m_pModeDisp = CUi::Create(CUi::UITYPE::ACTIONMODE_GUN, CObject2D::POLYGONTYPE::SENTERROLLING, 100.0f, 100.0f, 1, false, D3DXVECTOR3(50.0f, 50.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
            D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
        m_pModeDisp->SetUseDeath(false);

        //ダイブゲージのフレームを生成
        m_pDiveGaugeFrame = CUi::Create(CUi::UITYPE::DIVEGAUGEFRAME_000, CObject2D::POLYGONTYPE::SENTERROLLING, 450.0f, 100.0f, 1, false, D3DXVECTOR3(SCREEN_WIDTH - 250.0f, 100.0f, 0.0f),
            D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
        m_pDiveGaugeFrame->SetUseDeath(false);//死亡フラグを発動させない
        m_pDiveGaugeFrame->PushUiState(DBG_NEW CUiState_Gauge(D3DXVECTOR3(SCREEN_WIDTH - 390.0f, 106.5f, 0.0f),D3DXCOLOR(1.0f,1.0f,0.0f,1.0f),CObject2D::POLYGONTYPE::LEFT, CGauge::GAUGETYPE::DIVE, 350.0f, 19.6f, 0, 20));//ゲージ保持の状態を付与する

        m_pDivePossibleNum = CUi::Create(CUi::UITYPE::POSSIBLEDIVENUMTEXT_000, CObject2D::POLYGONTYPE::SENTERROLLING, 200.0f, 100.0f, 1, false, D3DXVECTOR3(200.0f, 100.0f, 0.0f),
            D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 61.0f, 1.0f, 1.0f));
        m_pDivePossibleNum->SetNumericState(0, 50.0f, 50.0f);
        m_pDivePossibleNum->SetUseDeath(false);

        m_pWire = CWire::Create(CWire::WIRETYPE::ROPE, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 5.0f, 20.0f, 4, 5);
        m_pWire->SetUseDeath(false);
        m_pWire->SetPlayerPointer(this);//プレイヤーのポインタを設定
        m_pWire->SetUseDraw(false);

        m_pPlayerActionMode = DBG_NEW CPlayerActionMode_ShotMove(this);//プレイヤーのアクションモード（メインステート）を射撃移動に設定
    }
    return S_OK;
}
//==========================================================================================================

//====================================================
//終了処理
//====================================================
void CPlayer::Uninit()
{
    CCharacter::Uninit();//Xオブジェクト終了
}
//==========================================================================================================

//====================================================
//更新処理
//====================================================
void CPlayer::Update()
{
    CObjectX::PosInfo& PosInfo = GetPosInfo();//位置情報
    CObjectX::SizeInfo& SizeInfo = GetSizeInfo();//サイズ情報
    const D3DXVECTOR3& Pos = PosInfo.GetPos();//位置
    const D3DXVECTOR3& VtxMax = SizeInfo.GetVtxMax();//最大頂点
    if (CScene::GetMode() == CScene::MODE_GAME)
    {
        if (GetLanding())
        {//地面にいるなら重力を最小限に
            GetMoveInfo().SetMove(D3DXVECTOR3(GetMoveInfo().GetMove().x,0.0f, GetMoveInfo().GetMove().z));
        }

        if (m_pPlayerActionMode != nullptr)
        {//プレイヤーのアクションのメインステート
            m_pPlayerActionMode->Process(this);
        }

        if (m_pMove != nullptr)
        {
            m_pMove->MoveProcess(this);//現在のアクションモードの移動処理を実行
        }
        if (m_pAbnormalState != nullptr)
        {
            m_pAbnormalState->Process(this);//状態異常の処理を実行
        }
        AdjustRot();//向き調整処理

        ActionModeChengeProcess(); //現在のアクションモードを変更する
    }

    CCharacter::Update();//更新処理

    CollisionProcess();

    if (CScene::GetMode() == CScene::MODE_GAME)
    {
        CUiState_Numeric* pUiState_Numeric = dynamic_cast<CUiState_Numeric*>(m_pDivePossibleNum->GetUiState(CUiState::UISTATE::NUMERIC));//UIの数字情報取得
        if (m_pAttack != nullptr)
        {
            m_pAttack->AttackProcess(this);//現在のアクションモードの攻撃処理を実装
        }
        DiveGaugeMaxEffect();//ダイブゲージがマックスになった時の演出

        if (pUiState_Numeric->GetValue() == s_nMaxDiveNum)
        {//ダイブ可能回数が最大に達したら点滅をさせる
            m_pDivePossibleNum->SetUseBlinking(true, 20, 0.0f);//点滅させる
        }
        else
        {//点滅を止める
            m_pDivePossibleNum->SetUseBlinking(false, 20, 0.0f);
            m_pDivePossibleNum->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), false, 1.0f);
        }

        CManager::GetDebugText()->PrintDebugText("プレイヤーの位置：%f %f %f\n", GetPosInfo().GetPos().x, GetPosInfo().GetPos().y, GetPosInfo().GetPos().z);
        CManager::GetDebugText()->PrintDebugText("プレイヤーの体力：%d\n", GetLifeInfo().GetLife());

        if (GetLifeInfo().GetLife() < 1)
        {
            CManager::GetSceneFade()->SetSceneFade(CFade::FADEMODE_IN, CScene::MODE_RESULT);
        }
    }

    if (m_bDamage == true)
    {
        SetNextMotion(3);
        m_bDamage = false;
    }

    MotionProcess();//モーション処理を行う
}
//==========================================================================================================

//====================================================
//描画処理
//====================================================
void CPlayer::Draw()
{
    CCharacter::Draw();
}
//==========================================================================================================

//====================================================
//死亡フラグを設定
//====================================================
void CPlayer::SetDeath()
{
    if (GetUseDeath() == true)
    {
        if (m_pMove != nullptr)
        {//移動ステートの開放
            delete m_pMove;
            m_pMove = nullptr;
        }

        if (m_pAttack != nullptr)
        {//攻撃ステートの開放
            delete m_pAttack;
            m_pAttack = nullptr;
        }

        if (m_pAbnormalState != nullptr)
        {//状態ステートの開放
            delete m_pAbnormalState;
            m_pAbnormalState = nullptr;
        }

        if (m_pPlayerActionMode != nullptr)
        {//プレイヤーアクションモードのメインステートの解放
            delete m_pPlayerActionMode;
            m_pPlayerActionMode = nullptr;
        }

        if (m_pLockOn != nullptr)
        {//ロックオン
            m_pLockOn->SetUseDeath(true);
            m_pLockOn->SetDeath();
            m_pLockOn = nullptr;
        }

        if (m_pModeDisp != nullptr)
        {//モード表示の開放
            m_pModeDisp->SetUseDeath(true);
            m_pModeDisp->SetDeath();
            m_pModeDisp = nullptr;
        }

        if (m_pWire != nullptr)
        {//ワイヤーの開放
            m_pWire->SetUseDeath(true);
            m_pWire->SetDeath();
            m_pWire = nullptr;
        }

        if (m_pHpGauge != nullptr)
        {//体力ゲージの開放
            m_pHpGauge->SetUseDeath(true);
            m_pHpGauge->SetDeath();
            m_pHpGauge = nullptr;
        }

        if (m_pDivePossibleNum != nullptr)
        {//ダイブ可能回数UIの開放
            m_pDivePossibleNum->SetUseDeath(true);
            m_pDivePossibleNum->SetDeath();
            m_pDivePossibleNum = nullptr;
        }

        if (m_pDiveGaugeFrame != nullptr)
        {//ダイブゲージのフレームUI
            m_pDiveGaugeFrame->SetUseDeath(true);
            m_pDiveGaugeFrame->SetDeath();
            m_pDiveGaugeFrame = nullptr;
        }
    }
    CCharacter::SetDeath();
}
//===========================================================================================================

//====================================================
//プレイヤーの生成
//====================================================
CPlayer* CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, D3DXVECTOR3 Scale)
{
    CPlayer* pPlayer = DBG_NEW CPlayer(DBG_NEW CPlayerMove_Normal(),DBG_NEW CPlayerAttack_Shot());//プレイヤーを生成
    bool bSuccess = pPlayer->CObject::GetCreateSuccess();
    int nIdx = 0;//テクスチャのインデックス
    CObjectX::PosInfo& PosInfo = pPlayer->GetPosInfo();
    CObjectX::RotInfo& RotInfo = pPlayer->GetRotInfo();
    CObjectX::SizeInfo& SizeInfo = pPlayer->GetSizeInfo();
    CObjectX::LifeInfo& LifeInfo = pPlayer->GetLifeInfo();
    pPlayer->Init();                                                                 //初期化処理
    pPlayer->GetMoveInfo().SetMove(move);//移動量
    pPlayer->CObject::SetType(CObject::TYPE::PLAYER);                                 //オブジェクトの種類を決める
    pPlayer->SetObjXType(CObjectX::OBJECTXTYPE_PLAYER);                    //オブジェクトXのタイプを設定
    pPlayer->SetTypeNum(0);                                                //オブジェクトXごとのタイプ番号を設定
    pPlayer->RegistMotion("data\\MODEL\\Enemy\\MotionEnemy\\DiveWeakEnemy\\DiveWeakEnemyMotion.txt",pPlayer);//モーションファイルを割り当てる
    pPlayer->GetDrawInfo().SetUseDraw(false);                                                     //描画しない
    PosInfo.SetPos(pos);                                                            //位置の設定
    PosInfo.SetPosOld(pos);                                                         //1f前の位置を設定
    PosInfo.SetPosFuture(pos);                                                      //1f後の位置を設定
    PosInfo.SetSupportPos(pos);                                                     //設置位置
    RotInfo.SetRot(rot);                                                            //向きの設定
    SizeInfo.SetScale(Scale);                                                       //拡大率の設定
    SizeInfo.SetFormarScale(Scale);                                                  //元の拡大率を設定する
    pPlayer->GetLifeInfo().SetAutoDeath(false);                                                    //死亡フラグを自動で発動するかどうか
    pPlayer->GetDrawInfo().SetUseShadow(true);

    //体力
    LifeInfo.SetLife(s_nNORMAL_MAXLIFE);
    LifeInfo.SetMaxLife(s_nNORMAL_MAXLIFE);

    if (CScene::GetMode() == CScene::MODE::MODE_GAME)
    {
        pPlayer->m_pHpGauge = CGauge::Create(CGauge::GAUGETYPE::PLAYERHP, s_nNORMAL_MAXLIFE, 600.0f, 50.0f, D3DXVECTOR3(50.0f, SCREEN_HEIGHT - 50.0f, 0.0f));
        pPlayer->m_pHpGauge->SetParam(s_nNORMAL_MAXLIFE);//最初から体力を最大値に設定
        pPlayer->m_pHpGauge->SetUseDeath(false);
    }
	return pPlayer;
}
//==========================================================================================================

//====================================================
//モード表示を設定する
//====================================================
void CPlayer::SetModeDisp(CUi* pModeDisp)
{
    if (m_pModeDisp != nullptr)
    {
        m_pModeDisp->SetUseDeath(true);//死亡フラグを使用する
        m_pModeDisp->SetDeath();       //死亡フラグを設定する
        m_pModeDisp = nullptr;         //ポインタを初期化

        m_pModeDisp = pModeDisp;      
        m_pModeDisp->SetUseDeath(false);//死亡フラグを使用しない
    }
}
//==========================================================================================================

//========================================================
//攻撃開始
//========================================================
void CPlayer::ActionModeChengeProcess()
{
    if (CManager::GetInputJoypad()->GetTrigger(CInputJoypad::JOYKEY::X) || CManager::GetInputMouse()->GetMouseRightClickTrigger())
    {
        CGame::GetTutorial()->SetSuccessCheck(CTutorial::CHECK::MODECHENGE);

        //モードを切り替える
        if (m_NowActionMode == ACTIONMODE::SHOT)
        {//ショット→ダイブ
            SetInitialActionMode(ACTIONMODE::DIVE);
        }
        else
        {//ダイブ→ショット
            SetInitialActionMode(ACTIONMODE::SHOT);
        }
    }
}
//==========================================================================================================

//========================================================
//ダイブゲージがマックスになった時の演出を行う
//========================================================
void CPlayer::DiveGaugeMaxEffect()
{
    CDebugText* pDebugText = CManager::GetDebugText();
    CUiState_Numeric* pUiState_Numeric = dynamic_cast<CUiState_Numeric*>(m_pDivePossibleNum->GetUiState(CUiState::UISTATE::NUMERIC));//UIの数字状態を取得
    CUiState_Gauge* pUiState_Gauge = dynamic_cast<CUiState_Gauge*>(m_pDiveGaugeFrame->GetUiState(CUiState::UISTATE::GAUGE));//UIのゲージ情報を取得
    if (pUiState_Gauge != nullptr)
    {
        CGauge* pDiveGauge = pUiState_Gauge->GetGauge();//ダイブゲージを取得する
        if (pUiState_Numeric != nullptr)
        {
            pDebugText->PrintDebugText("ダイブ可能回数：%d\n", pUiState_Numeric->GetValue());
            if (pDiveGauge->GetFullGaugeFlag() == true)
            {//ゲージがマックスになった「瞬間」にフラグを発動＆＆最大ダイブ可能回数に達していなかったら
                CGauge* pGauge = CGauge::Create(CGauge::GAUGETYPE::PLAYERHP, pDiveGauge->GetParam(), pDiveGauge->GetWidth(), pDiveGauge->GetHeight(), pDiveGauge->GetPos());
                pGauge->SetUseLife(true, 50, 50);
                pGauge->SetPolygonType(pDiveGauge->GetPolygonType());
                pGauge->SetColor(pDiveGauge->GetColor(), false, 1.0f);
                pGauge->SetUseLifeRatioColor(true);
                pGauge->SetUseDeath(true);
                pGauge->SetUseAddScale(D3DXVECTOR2(0.3f, 0.3f), true);
                pGauge->SetUseScale(true);
                pGauge->SetScale(D3DXVECTOR2(1.0f, 1.0f));
                pDiveGauge->SetParam(0);//ダイブゲージをリセット
                pUiState_Numeric->SetValue(pUiState_Numeric->GetValue() + 1, m_pDivePossibleNum);
            }
        }
    }
}
//==========================================================================================================

//========================================================
//移動モードをチェンジ
//========================================================
void CPlayer::ChengeMoveMode(CPlayerMove* pPlayerMove)
{
    if (m_pMove != nullptr)
    {
        delete m_pMove;
        m_pMove = nullptr;

        m_pMove = pPlayerMove;
    }
}
//==========================================================================================================

//========================================================
//攻撃モードをチェンジ
//========================================================
void CPlayer::ChengeAttackMode(CPlayerAttack* pPlayerAttack)
{
    if (m_pAttack != nullptr)
    {
        delete m_pAttack;
        m_pAttack = nullptr;

        m_pAttack = pPlayerAttack;
    }
}
//==========================================================================================================

//========================================================
//プレイヤーのアクションモードをチェンジ
//========================================================
void CPlayer::ChengeActionMode(CPlayerActionMode* pPlayerActionMode)
{
    if (m_pPlayerActionMode != nullptr)
    {
        delete m_pPlayerActionMode;
        m_pPlayerActionMode = nullptr;

        m_pPlayerActionMode = pPlayerActionMode;
    }
}
//==========================================================================================================

//========================================================
//それぞれのアクションモードの初期状態に設定する
//========================================================
void CPlayer::SetInitialActionMode(ACTIONMODE ActionMode)
{
    m_NowActionMode = ActionMode;//アクションモードのデータを変える

    //モード生成
    switch (ActionMode)
    {
    case ACTIONMODE::SHOT://発射モード
        ChengeActionMode(DBG_NEW CPlayerActionMode_ShotMove(this));
        break;
    case ACTIONMODE::DIVE://ダイブモード
        ChengeActionMode(DBG_NEW CPlayerActionMode_PrepDive(this));
        break;
    default:
        break;
    }

    m_pWire->SetUseDraw(false);
    m_pWire->GetWireHead()->GetDrawInfo().SetUseDraw(false);

}
//==========================================================================================================

//========================================================
//当たり判定全般処理
//========================================================
void CPlayer::CollisionProcess()
{
    SetIsLanding(false);                           //地面に乗っているかどうかのフラグをリセット
    GetCollisionInfo().GetSquareInfo().ResetPushOutFirstFlag();//それぞれの軸の押し出し判定の優先フラグをリセット
    m_bCollision = false;                          //判定状態をリセット
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
//==========================================================================================================

//========================================================
//ブロックとの当たり判定
//========================================================
void CPlayer::CollisionBlock()
{

}
//==========================================================================================================

//========================================================
//ジャンプ処理
//========================================================
void CPlayer::JumpProcess()
{
    //
}
//==========================================================================================================

//========================================================
//状態異常を変える
//========================================================
void CPlayer::ChengeAbnormalState(CPlayerAbnormalState* pAbnormalState)
{
    if (m_pAbnormalState != nullptr)
    {
        delete m_pAbnormalState;
        m_pAbnormalState = pAbnormalState;
    }
}
//==========================================================================================================

//========================================================
//ダメージを与える
//========================================================
void CPlayer::SetDamage(int nDamage, int nHitStopTime)
{
    if (GetLifeInfo().GetHitStop() == false)
    {//ヒットストップ状態じゃなければ処理を実行する
        CCharacter::SetDamage(nDamage, nHitStopTime);
        CSound* pSound = CManager::GetSound();
        
        m_pHpGauge->SetParam(GetLifeInfo().GetLife());
        m_pHpGauge->SetShake(5.0f * nDamage, 30);

        CGauge* pGauge = CGauge::Create(CGauge::GAUGETYPE::PLAYERHP, m_pHpGauge->GetParam(), m_pHpGauge->GetWidth(), m_pHpGauge->GetHeight(), m_pHpGauge->GetPos());
        pGauge->SetUseLife(true, 10, 10);
        pGauge->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), false, 1.0f);
        pGauge->SetUseLifeRatioColor(true);
        pGauge->SetUseDeath(true);
        pGauge->SetUseAddScale(D3DXVECTOR2(0.1f, 0.1f), true);
        pGauge->SetUseScale(true);
        pGauge->SetScale(D3DXVECTOR2(1.0f, 1.0f));

        pSound->PlaySoundB(CSound::SOUND_LABEL::SE_DAMAGE_000);

        m_bDamage = true;//ダメージを受けた状態を明示的に示す
        SetNextMotion(2);
    }
}
//==========================================================================================================

//========================================================
//向き調整処理
//========================================================
void CPlayer::AdjustRot()
{
    const D3DXVECTOR3& CameraRot = CManager::GetCamera()->GetRot();
    GetRotInfo().SetRot(D3DXVECTOR3(GetRotInfo().GetRot().x,D3DX_PI + CameraRot.y, GetRotInfo().GetRot().z));
}
//==========================================================================================================

//========================================================
//位置調整処理
//========================================================
void CPlayer::AdjustPos()
{
    const D3DXVECTOR3& Pos = GetPosInfo().GetPos();
    D3DXVECTOR3 ScreenPos = CCalculation::CalcWorldToScreenNoViewport(Pos, *CManager::GetCamera()->GetMtxView(), *CManager::GetCamera()->GetMtxProjection(),
        static_cast<float>(SCREEN_WIDTH), static_cast<float>(SCREEN_HEIGHT));

    //画面外に出ないように補正
    if (ScreenPos.x > SCREEN_WIDTH || ScreenPos.x < 0.0f)
    {
        GetPosInfo().SetPos(GetPosInfo().GetPosOld());
    }
}
//==========================================================================================================

//*******************************************************
//状態異常ステート：スーパークラス
//*******************************************************

//=======================================================
//コンストラクタ
//=======================================================
CPlayerAbnormalState::CPlayerAbnormalState()
{

}
//==========================================================================================================

//=======================================================
//デストラクタ
//=======================================================
CPlayerAbnormalState::~CPlayerAbnormalState()
{

}
//==========================================================================================================

//=======================================================
//処理
//=======================================================
void CPlayerAbnormalState::Process(CPlayer* pPlayer)
{

}
//==========================================================================================================

//*******************************************************
//状態異常ステート：ノックバック
//*******************************************************

//=======================================================
//コンストラクタ
//=======================================================
CPlayerAbnormalState_KnockBack::CPlayerAbnormalState_KnockBack(CPlayer* pPlayer, D3DXVECTOR3 KnockBackMove, float fInertia) : m_KnockBackMove(KnockBackMove),m_fInertia(fInertia)
{

}
//==========================================================================================================

//=======================================================
//デストラクタ
//=======================================================
CPlayerAbnormalState_KnockBack::~CPlayerAbnormalState_KnockBack()
{

}
//==========================================================================================================

//=======================================================
//処理
//=======================================================
void CPlayerAbnormalState_KnockBack::Process(CPlayer* pPlayer)
{
    m_KnockBackMove.x += (0.0f - m_KnockBackMove.x) * m_fInertia;
    m_KnockBackMove.y += (0.0f - m_KnockBackMove.y) * m_fInertia;
    m_KnockBackMove.z += (0.0f - m_KnockBackMove.z) * m_fInertia;

    pPlayer->GetMoveInfo().SetMove(m_KnockBackMove);

    if (fabsf(m_KnockBackMove.x) < 1.0f && fabsf(m_KnockBackMove.y) < 1.0f && fabsf(m_KnockBackMove.z) < 1.0f)
    {
        pPlayer->ChengeAbnormalState(DBG_NEW CPlayerAbnormalState());
    }
}
//==========================================================================================================
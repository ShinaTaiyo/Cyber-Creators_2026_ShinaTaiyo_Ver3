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
    CCharacter::Init();                               //Xオブジェクト初期化
                                                      
    GetLifeInfo().SetAutoSubLife(false);              //自動的に体力を減らすかどうか
    GetMoveInfo().SetUseGravity(true,1.0f);           //重力を使用する

    if (CScene::GetMode() == CScene::MODE::MODE_GAME)
    {//ゲームシーンなら

        //ロックオンの生成
        m_pLockOn = CLockon::Create(CLockon::TYPE::SHOT,D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), CObject2D::POLYGONTYPE::SENTERROLLING, 100.0f, 100.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
        m_pLockOn->SetUseDeath(false);       //死亡フラグを使用しない
        m_pLockOn->SetPolygonRotSpeed(0.01f);//ポリゴンを回転させる

        //モード表示の生成
        m_pModeDisp = CUi::Create(CUi::UITYPE::ACTIONMODE_GUN,false,CObject2D::POLYGONTYPE::SENTERROLLING, 100.0f, 100.0f, 1, false, D3DXVECTOR3(50.0f, 50.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
            D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
        m_pModeDisp->SetUseDeath(false);     //死亡フラグを使用しない

        //*ダイブゲージのフレームを生成
        m_pDiveGaugeFrame = CUi::Create(CUi::UITYPE::DIVEGAUGEFRAME_000,false,CObject2D::POLYGONTYPE::SENTERROLLING, 450.0f, 100.0f, 1, false, D3DXVECTOR3(SCREEN_WIDTH - 250.0f, 100.0f, 0.0f),
            D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
        m_pDiveGaugeFrame->SetUseDeath(false);//死亡フラグを使用しない

        //ゲージの機能を持たせる
        m_pDiveGaugeFrame->GetUiCompositeContainer()->Add(DBG_NEW CUIComposite_Gauge(m_pDiveGaugeFrame,D3DXVECTOR3(SCREEN_WIDTH - 390.0f, 106.5f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), CObject2D::POLYGONTYPE::LEFT, CGauge::GAUGETYPE::DIVE, 350.0f, 19.6f, 0, 20));

        //*ダイブ可能回数のUIを生成
        m_pDivePossibleNum = CUi::Create(CUi::UITYPE::POSSIBLEDIVENUMTEXT_000,false,CObject2D::POLYGONTYPE::SENTERROLLING, 200.0f, 100.0f, 1, false, D3DXVECTOR3(200.0f, 100.0f, 0.0f),
            D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 61.0f, 1.0f, 1.0f));
        m_pDivePossibleNum->GetUiCompositeContainer()->Add(DBG_NEW CUIComposite_Numeric(m_pDivePossibleNum, 0, 50.0f, 50.0f));//数字の機能をコンポジットパターンのコンテナに格納
        m_pDivePossibleNum->SetUseDeath(false);                                                                               //死亡フラグを使用しない

        //*ワイヤーを生成
        m_pWire = CWire::Create(CWire::WIRETYPE::ROPE, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 5.0f, 20.0f, 4, 5);
        m_pWire->SetUseDeath(false);    //死亡フラグを使用しない
        m_pWire->SetPlayerPointer(this);//プレイヤーのポインタを設定
        m_pWire->SetUseDraw(false);     //描画しない

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
    CObjectX::PosInfo& PosInfo = GetPosInfo();            //位置情報
    CObjectX::SizeInfo& SizeInfo = GetSizeInfo();         //サイズ情報
    CObjectX::CollisionInfo& CollisionInfo = GetCollisionInfo();//当たり判定情報を取得する
    const D3DXVECTOR3& Pos = PosInfo.GetPos();            //位置
    const D3DXVECTOR3& VtxMax = SizeInfo.GetVtxMax();     //最大頂点
    if (CScene::GetMode() == CScene::MODE_GAME)
    {//ゲームシーンなら
        if (CollisionInfo.GetState().GetLanding())
        {//地面にいるなら重力を０に
            GetMoveInfo().SetMove(D3DXVECTOR3(GetMoveInfo().GetMove().x,0.0f, GetMoveInfo().GetMove().z));
        }

        if (m_pPlayerActionMode != nullptr)
        {//プレイヤーのアクションのメインステート
            m_pPlayerActionMode->Process(this);
        }

        if (m_pMove != nullptr)
        {//移動ステート
            m_pMove->MoveProcess(this);//現在のアクションモードの移動処理を実行
        }

        if (m_pAbnormalState != nullptr)
        {//状態異常ステート
            m_pAbnormalState->Process(this);//状態異常の処理を実行
        }

        AdjustRot();//向き調整処理

        ActionModeChengeProcess(); //現在のアクションモードを変更する
    }

    CCharacter::Update();//キャラクター更新処理

    CollisionProcess();//当たり判定処理

    if (CScene::GetMode() == CScene::MODE_GAME)
    {//ゲームシーンなら
        CUIComposite_Container* pDivePossibleUiCompositeContainer = m_pDivePossibleNum->GetUiCompositeContainer();//ダイブ可能回数のコンポジットパターンのコンテナを取得
        CUIComposite_Numeric* pDivePossibleUiNumeric = pDivePossibleUiCompositeContainer->GetChildren<CUIComposite_Numeric>();//数字の機能を取得
        if (m_pAttack != nullptr)
        {//攻撃ステート
            m_pAttack->AttackProcess(this);//現在のアクションモードの攻撃処理を実装
        }

        DiveGaugeMaxEffect();//ダイブゲージがマックスになった時の演出

        if (pDivePossibleUiNumeric->GetValue() == s_nMaxDiveNum)
        {//ダイブ可能回数が最大に達したら点滅をさせる
            m_pDivePossibleNum->SetUseBlinking(true, 20, 0.0f);//点滅させる
        }
        else
        {//点滅を止める
            m_pDivePossibleNum->SetUseBlinking(false, 20, 0.0f);
            m_pDivePossibleNum->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), false, 1.0f);
        }

        //デバッグ表示
        CManager::GetDebugText()->PrintDebugText("プレイヤーの位置：%f %f %f\n", GetPosInfo().GetPos().x, GetPosInfo().GetPos().y, GetPosInfo().GetPos().z);
        CManager::GetDebugText()->PrintDebugText("プレイヤーの体力：%d\n", GetLifeInfo().GetLife());

        if (GetLifeInfo().GetLife() < 1)
        {//体力がなくなったら
            CManager::GetSceneFade()->SetSceneFade(CFade::FADEMODE_IN, CScene::MODE_RESULT);
        }
    }

    if (m_bDamage == true)
    {//ダメージを与える
        SetNextMotion(3);//ダメージモーションにする
        m_bDamage = false;//ダメージ状態をオフに
    }

    MotionProcess();//モーション処理を行う
}
//==========================================================================================================

//====================================================
//描画処理
//====================================================
void CPlayer::Draw()
{
    CCharacter::Draw();//キャラクターの描画処理
}
//==========================================================================================================

//====================================================
//死亡フラグを設定
//====================================================
void CPlayer::SetDeath()
{
    if (GetUseDeath() == true)
    {//死亡フラグを使用するなら
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
    CCharacter::SetDeath();//キャラクターの死亡フラグを設定する
}
//===========================================================================================================

//====================================================
//プレイヤーの生成
//====================================================
CPlayer* CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, D3DXVECTOR3 Scale)
{
    CPlayer* pPlayer = DBG_NEW CPlayer(DBG_NEW CPlayerMove_Normal(),DBG_NEW CPlayerAttack_Shot());//プレイヤーを生成
    CObjectX::PosInfo& PosInfo = pPlayer->GetPosInfo();                                                      //位置情報
    CObjectX::RotInfo& RotInfo = pPlayer->GetRotInfo();                                                      //向き情報
    CObjectX::SizeInfo& SizeInfo = pPlayer->GetSizeInfo();                                                   //サイズ情報
    CObjectX::LifeInfo& LifeInfo = pPlayer->GetLifeInfo();                                                   //体力情報
    pPlayer->Init();                                                                                         //初期化処理
    pPlayer->GetMoveInfo().SetMove(move);                                                                    //移動量
    pPlayer->CObject::SetType(CObject::TYPE::PLAYER);                                                        //オブジェクトの種類を決める
    pPlayer->SetObjXType(CObjectX::OBJECTXTYPE_PLAYER);                                                      //オブジェクトXのタイプを設定
    pPlayer->SetTypeNum(0);                                                                                  //オブジェクトXごとのタイプ番号を設定
    pPlayer->RegistMotion("data\\MODEL\\Enemy\\MotionEnemy\\DiveWeakEnemy\\DiveWeakEnemyMotion.txt",pPlayer);//モーションファイルを割り当てる
    pPlayer->GetDrawInfo().SetUseDraw(false);                                                                //描画しない

    int nIdx = CManager::GetObjectXInfo()->Regist("data\\MODEL\\Enemy\\MotionEnemy\\DiveWeakEnemy\\DiveWeakEnemy00_CollisionModel.x"); //モデル情報を登録し番号を取得
    pPlayer->BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(nIdx),                                      //モデル情報を設定する（当たり判定用のモデル)
        CManager::GetObjectXInfo()->GetBuffMat(nIdx),
        CManager::GetObjectXInfo()->GetdwNumMat(nIdx),
        CManager::GetObjectXInfo()->GetTexture(nIdx),
        CManager::GetObjectXInfo()->GetColorValue(nIdx));

    pPlayer->SetSize();                                                                                      //サイズを設定

    PosInfo.SetPos(pos);                                                                                     //位置の設定
    PosInfo.SetPosOld(pos);                                                                                  //1f前の位置を設定
    PosInfo.SetPosFuture(pos);                                                                               //1f後の位置を設定
    PosInfo.SetSupportPos(pos);                                                                              //設置位置
    RotInfo.SetRot(rot);                                                                                     //向きの設定
    SizeInfo.SetScale(Scale);                                                                                //拡大率の設定
    SizeInfo.SetFormarScale(Scale);                                                                          //元の拡大率を設定する
    pPlayer->GetLifeInfo().SetAutoDeath(false);                                                              //死亡フラグを自動で発動するかどうか
    pPlayer->GetDrawInfo().SetUseShadow(true);                                                               //影を描画する

    //体力
    LifeInfo.SetLife(s_nNORMAL_MAXLIFE);                                                                     //体力
    LifeInfo.SetMaxLife(s_nNORMAL_MAXLIFE);                                                                  //最大体力

    if (CScene::GetMode() == CScene::MODE::MODE_GAME)
    {//ゲームシーンなら
        pPlayer->m_pHpGauge = CGauge::Create(CGauge::GAUGETYPE::PLAYERHP, s_nNORMAL_MAXLIFE, 600.0f, 50.0f, D3DXVECTOR3(50.0f, SCREEN_HEIGHT - 50.0f, 0.0f));//体力ゲージを生成
        pPlayer->m_pHpGauge->SetParam(s_nNORMAL_MAXLIFE);                                                                                                    //最初から体力を最大値に設定
        pPlayer->m_pHpGauge->SetUseDeath(false);                                                                                                             //死亡フラグを使用しない
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
    {//マウスの右クリック又はジョイパッドのXボタンを押したら
        CGame::GetTutorial()->SetSuccessCheck(CTutorial::CHECK::MODECHENGE);//モードを変更するチュートリアルを完了

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
    CDebugText* pDebugText = CManager::GetDebugText();//デバッグ情報
    
    //=======================================================
    //ダイブゲージの機能の取得
    //=======================================================
    CUIComposite_Container* pDiveGaugeFrameUiCompositeContainer = m_pDiveGaugeFrame->GetUiCompositeContainer();                //ゲージフレームのコンポジットパターンのコンテナを取得する
    CUIComposite_Gauge* pDiveGaugeUi_CompositeGauge = pDiveGaugeFrameUiCompositeContainer->GetChildren<CUIComposite_Gauge>();  //ゲージの機能を取得する
    //=========================================================================================================-
    
    //=======================================================
    //ダイブ可能回数の数字表示機能の取得
    //=======================================================
    CUIComposite_Container* pDivePossibleUiCompositeContainer = m_pDivePossibleNum->GetUiCompositeContainer();                         //ダイブ可能回数のUIのコンポジットパターンのコンテナを取得する
    CUIComposite_Numeric * pDivePossibleUiComposite_Numeric = pDivePossibleUiCompositeContainer->GetChildren<CUIComposite_Numeric>();  //数字表示の機能を取得する
    //=========================================================================================================-

    if (pDiveGaugeUi_CompositeGauge != nullptr)
    {
        CGauge* pDiveGauge = pDiveGaugeUi_CompositeGauge->GetGauge();//ダイブゲージを取得する
        if (pDivePossibleUiComposite_Numeric != nullptr)
        {
            //デバッグ表示
            pDebugText->PrintDebugText("ダイブ可能回数：%d\n", pDivePossibleUiComposite_Numeric->GetValue());
            if (pDiveGauge->GetFullGaugeFlag() == true)
            {//ゲージがマックスになった「瞬間」にフラグを発動＆＆最大ダイブ可能回数に達していなかったら
                //生成処理
                CGauge* pGauge = CGauge::Create(CGauge::GAUGETYPE::PLAYERHP, pDiveGauge->GetParam(), pDiveGauge->GetWidth(), pDiveGauge->GetHeight(), pDiveGauge->GetPos());
                pGauge->SetUseLife(true, 50, 50);                                                                                  //体力を使用する
                pGauge->SetPolygonType(pDiveGauge->GetPolygonType());                                                              //ポリゴンの種類を設定
                pGauge->SetColor(pDiveGauge->GetColor(), false, 1.0f);                                                             //色合いを設定
                pGauge->SetUseLifeRatioColor(true);                                                                                //体力の割合で色合いを変える
                pGauge->SetUseDeath(true);                                                                                         //死亡フラグ設定処理
                pGauge->SetUseAddScale(D3DXVECTOR2(0.3f, 0.3f), true);                                                             //拡大率の加算を使用する
                pGauge->SetUseScale(true);                                                                                         //拡大率を使用する
                pGauge->SetScale(D3DXVECTOR2(1.0f, 1.0f));                                                                         //拡大率を設定
                pDiveGauge->SetParam(0);                                                                                           //ダイブゲージをリセット
                pDivePossibleUiComposite_Numeric->SetValue(pDivePossibleUiComposite_Numeric->GetValue() + 1, m_pDivePossibleNum);  //ダイブ可能回数を増やす
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
    {//移動ステートが存在していたら破棄して新しい移動ステートを設定
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
    {//攻撃ステートが存在していたら破棄して新しい攻撃ステートを設定
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
    {//プレイヤーアクションモードステートが存在していたら破棄して新しいステートに設定
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

    m_pWire->SetUseDraw(false);//ワイヤーの描画をしない
    m_pWire->GetWireHead()->GetDrawInfo().SetUseDraw(false);//ワイヤーの頭の描画をしない

}
//==========================================================================================================

//========================================================
//当たり判定全般処理
//========================================================
void CPlayer::CollisionProcess()
{
    CObjectX::CollisionInfo& CollisionInfo = GetCollisionInfo();                //当たり判定情報を取得する
    CObjectX::CollisionInfo::State& CollisionState = CollisionInfo.GetState();  //当たり判定状態を取得する
    CollisionInfo.GetSquareInfo().ResetPushOutFirstFlag();                      //それぞれの軸の押し出し判定の優先フラグをリセット
    CollisionState.SetWallingOld(CollisionInfo.GetState().GetWalling());
    CollisionState.SetLandingOld(CollisionInfo.GetState().GetLanding());
    CollisionState.ResetState();
    m_bCollision = false;                                      //判定状態をリセット
    for (int nCntPri = 0; nCntPri < CObject::m_nMAXPRIORITY; nCntPri++)
    {//オブジェクトリストを検索
        CObject* pObj = CObject::GetTopObject(nCntPri);//リストの先頭オブジェクトを取得する

        while (pObj != nullptr)
        {//オブジェクトがnullptrになるまで
            //次のオブジェクトを格納
            CObject* pNext = pObj->GetNextObject();

            //種類の取得（敵なら当たり判定）
            CObject::TYPE type = pObj->GetType();

            if (type == CObject::TYPE::BLOCK || type == CObject::TYPE::BGMODEL)
            {//オブジェクトタイプが「ブロック」又は「背景モデル」だったら
                CObjectX* pObjX = static_cast<CObjectX*>(pObj);                     //オブジェクトXにキャスト
                CCollision::ExtrusionCollisionSquarePushOutFirstDecide(this, pObjX);//正方形の押し出し判定のそれぞれの軸の順序の優先度を決める
            }

            //オブジェクトを次に進める
            pObj = pNext;
        }
    }
    //=======================================================================================

    //============================================================
    //押し出し判定開始（上記の処理で決めた判定順序でAABB押し出し判定をする)
    //============================================================
    for (int nCntPri = 0; nCntPri < CObject::m_nMAXPRIORITY; nCntPri++)
    {//オブジェクトリストを検索
        CObject* pObj = CObject::GetTopObject(nCntPri);//リストの先頭オブジェクトを取得

        while (pObj != nullptr)
        {//オブジェクトが存在していたら
            //次のオブジェクトを格納
            CObject* pNext = pObj->GetNextObject();

            //種類の取得（敵なら当たり判定）
            CObject::TYPE type = pObj->GetType();

            if (type == CObject::TYPE::BLOCK || type == CObject::TYPE::BGMODEL)
            {//オブジェクトタイプが「ブロック」又は「背景モデル」だったら
                CObjectX* pObjX = static_cast<CObjectX*>(pObj);//オブジェクトXにダウンキャスト

                CCollision::ResolveExtrusionCollisionSquare(this, pObjX);//正方形の押し出し判定をする
            }

            pObj = pNext;//リストを次に進める
        }
    }
    //=======================================================================================
}
//==========================================================================================================

//========================================================
//状態異常を変える
//========================================================
void CPlayer::ChengeAbnormalState(CPlayerAbnormalState* pAbnormalState)
{
    if (m_pAbnormalState != nullptr)
    {//状態異常ステートが存在していたら新しいステートに変更
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
        CCharacter::SetDamage(nDamage, nHitStopTime);                    //キャラクターのダメージ処理
        CSound* pSound = CManager::GetSound();                           //サウンド情報
        CCombo* pCombo = CGame::GetCombo();                              //コンボへのポインタ
        m_pHpGauge->SetParam(GetLifeInfo().GetLife());                   //体力ゲージのパラメータを設定（キャラクターのダメージ処理で体力が変動)
        m_pHpGauge->SetShake(5.0f * nDamage, 30);                        //体力ゲージを振動させる

        //演出用ゲージを生成
        CGauge* pGauge = CGauge::Create(CGauge::GAUGETYPE::PLAYERHP, m_pHpGauge->GetParam(), m_pHpGauge->GetWidth(), m_pHpGauge->GetHeight(), m_pHpGauge->GetPos());
        pGauge->SetUseLife(true, 10, 10);//体力を使用する
        pGauge->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), false, 1.0f);//色合いを設定する
        pGauge->SetUseLifeRatioColor(true);                              //体力の割合に応じて透明度を変える
        pGauge->SetUseDeath(true);                                       //死亡フラグを使用する
        pGauge->SetUseAddScale(D3DXVECTOR2(0.1f, 0.1f), true);           //拡大率の加算を行う
        pGauge->SetUseScale(true);                                       //拡大率を使用する
        pGauge->SetScale(D3DXVECTOR2(1.0f, 1.0f));                       //拡大率を設定する

        pSound->PlaySoundB(CSound::SOUND_LABEL::SE_DAMAGE_000);          //ダメージサウンドを呼ぶ
        pCombo->ResetCombo();                                            //コンボ数をリセットする

        m_bDamage = true;                                                //ダメージを受けた状態を明示的に示す
        SetNextMotion(2);                                                //ダメージモーションにする
    }
}
//==========================================================================================================

//========================================================
//向き調整処理
//========================================================
void CPlayer::AdjustRot()
{
    const D3DXVECTOR3& CameraRot = CManager::GetCamera()->GetRot();                                           //カメラの向き
    GetRotInfo().SetRot(D3DXVECTOR3(GetRotInfo().GetRot().x,D3DX_PI + CameraRot.y, GetRotInfo().GetRot().z)); //カメラの向きを基準にプレイヤーの向きを設定
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
    //ノックバックの移動量を減衰させる
    m_KnockBackMove.x += (0.0f - m_KnockBackMove.x) * m_fInertia;
    m_KnockBackMove.y += (0.0f - m_KnockBackMove.y) * m_fInertia;
    m_KnockBackMove.z += (0.0f - m_KnockBackMove.z) * m_fInertia;

    //移動量の設定
    pPlayer->GetMoveInfo().SetMove(m_KnockBackMove);

    if (fabsf(m_KnockBackMove.x) < 1.0f && fabsf(m_KnockBackMove.y) < 1.0f && fabsf(m_KnockBackMove.z) < 1.0f)
    {//それぞれの軸の移動量が絶対値1.0fを下回ったら状態異常を元に戻す
        pPlayer->ChengeAbnormalState(DBG_NEW CPlayerAbnormalState());
    }
}
//==========================================================================================================
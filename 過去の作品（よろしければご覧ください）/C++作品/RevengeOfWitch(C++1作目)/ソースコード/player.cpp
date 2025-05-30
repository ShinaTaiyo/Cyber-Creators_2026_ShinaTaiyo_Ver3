//=============================================================================
//
//７月４日：回避アクション実装[player.cpp]
//Author:ShinaTaiyo
//
//=============================================================================

//========================
//インクルード
//========================
#include "player.h"
#include "objectX.h"
#include "renderer.h"
#include "main.h"
#include "manager.h"
#include "camera.h"
#include "bullet.h"
#include "block.h"
#include "input.h"
#include "sound.h"
#include "objectXInfo.h"
#include "item.h"
#include "gauge.h"
#include "number.h"
#include "magic.h"
#include "calculation.h"
#include "attack.h"
#include "particle.h"
#include "fade.h"
#include "game.h"
#include "damage.h"
#include "debugtext.h"
#include "collision.h"
#include "eventmanager.h"
#include "model.h"
#include "score.h"
#include "trap.h"
#include "emitter.h"
#include "signboard.h"
//==========================================================================================================

//====================================================
//静的メンバ変数の初期化
//====================================================
LPD3DXMESH CPlayer::m_pMeshTemp = nullptr;                             //メッシュ情報へのポインタ
LPD3DXBUFFER CPlayer::m_pBuffMatTemp = nullptr;                        //マテリアルへのポインタ
DWORD CPlayer::m_dwNumMatTemp = NULL;                                  //マテリアルの数
LPDIRECT3DTEXTURE9 CPlayer::m_pTextureTemp[CPlayer::m_nMAX_MAT] = {};  //テクスチャへのポインタ
int CPlayer::m_nNumFile = 1;                                           //ファイルの数を保存する
const float CPlayer::m_fNORMALJUMPPOWER = 9.0f;                        //通常のジャンプ力
const float CPlayer::m_fGRAVITYPOWER = 1.0f;                           //重力の大きさ
const float CPlayer::m_fNORMALSPEED = 2.25f;                           //通常の速さ
const int CPlayer::m_nPLAYERUSAGEMP[CMagicDisp::MAGICDISP_MAX] =
{
    5,
    10,
    30,
    30,
    50,
    25,
    40,
    45,
    100,
    40
};//それぞれの魔法の消費MP
const int CPlayer::m_nCHARGEATTACKPOWER[CPlayer::m_nMAX_CHARGEATTACKLEVEL] =
{ 35,72,110,150,200 };                                                 //チャージ攻撃の威力の基礎値
//==========================================================================================================

//====================================================
//コンストラクタ
//====================================================
CPlayer::CPlayer() : m_nRespawnCnt(0),m_pUsageMpGauge(nullptr)
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
    CObjectX::Init();                 //Xオブジェクト初期化
    m_nJumpPushCnt = 0;               //ジャンプボタンを押している時間のカウント数
    m_bMove = false;                  //移動中かどうか
    m_AddMove = NULL_VECTOR3;         //加算移動量
    m_CompulsionPos = NULL_VECTOR3;   //加算位置
    m_SaveMove = NULL_VECTOR3;        //保存する移動量
    m_nCollisionCheck = 0;            //判定チェック用
    m_bCompulsionPos = false;         //位置強制状態かどうか
    m_pNumber = nullptr;              //番号へのポインタ
    m_bJustStopMove = false;          //移動を一瞬止めるフラグ
    m_fRot = 0.0f;                    //向きの補正用
    m_fCorrectionInertia = 0.0f;      //慣性の補正
    m_fJump = m_fNORMALJUMPPOWER;     //ジャンプ力
    m_fSpeed = m_fNORMALSPEED;        //移動速度
    m_nCntLanding = 0;                //地面にいる時間をカウントする
    //==========================================================================================

    //=========================================
    //魔法関係
    //=========================================
    m_bPossibleMagic = true;          //魔法が使用可能かどうか
    m_nUseMagicNum = 0;               //使用している魔法の番号
    m_nNextUseMagic = 0;              //次に魔法が使用可能になるまでの時間
    m_nUseMagicTime = 0;              //魔法使用時間をカウントする
    m_pMagicDisp = nullptr;           //魔法表示へのポインタ
    m_pMagicWindow = nullptr;         //魔法表示ウインドウへのポインタ
    m_nNowMagicType = 0;              //現在の魔法のタイプ
    m_nMagicNum = 0;                  //現在持っている魔法の種類
    for (int nCnt = 0; nCnt < m_nMAX_MAGICNUM; nCnt++)
    {
        m_nMagicSlot[nCnt] = 0;//魔法の順番
        m_bUnlockMagic[nCnt] = false;//魔法を開放しているかどうか
#ifdef _DEBUG
        SetUnlockMagic(nCnt, NULL_VECTOR3, true);
#endif // _DEBUG
    }
    //==========================================================================================

    //=========================================
    //移動関係
    //=========================================
    m_nStopMoveCnt = 0;               //移動を止める時間をカウントする
    //==========================================================================================

    //=========================================
    //ステータス
    //=========================================
    m_Status.nHp = 0;                               //体力
    m_Status.nMaxHp = 0;                            //最大体力
    m_Status.nMp = 0;                               //マジックポイント
    m_Status.nMaxMp = 0;                            //最大マジックポイント
    m_Status.fAtkBuff = m_fINIT_ATTACKBUFF;         //攻撃バフ倍率
    //==========================================================================================

    //==========================================
    //アクション関係
    //==========================================
    m_nJumpCnt = 0;                                             //ジャンプした回数をカウントする

    //回避
    m_Dodge.nStateCnt = 0;                                      //回避状態の時間をカウントする
    m_Dodge.bPossible = false;                                  //回避可能かどうか
    m_Dodge.bState = false;                                     //回避状態かどうか
    m_Dodge.nRevivalCnt = 0;                                    //回避状態が復活するまでの時間
#ifdef _DEBUG
    for (int nCnt = 0; nCnt < ACTIONTYPE_MAX; nCnt++)
    {//全てのアクションを最初から使用可能にする
        m_bUnlockAction[nCnt] = true;
    }
#else
    for (int nCnt = 0; nCnt < ACTIONTYPE_MAX; nCnt++)
    {//全てのアクションを最初からは使用不可能にする
        m_bUnlockAction[nCnt] = false;
    }
#endif // DEBUG


    //==========================================================================================

    //==========================================
    //弾の技関係
    //==========================================
#ifdef _DEBUG
    for (int nCnt = 0; nCnt < BULLETACTION_MAX; nCnt++)
    {//全てのバレットアクションを最初から使用可能にする
        m_bUnlockBulletAction[nCnt] = true;
    }

#else
    for (int nCnt = 0; nCnt < BULLETACTION_MAX; nCnt++)
    {//全てのバレットアクションを最初からは使用不可能にする
        m_bUnlockBulletAction[nCnt] = false;
    }
#endif // DEBUG

    //==========================================================================================

    //==========================================
    //チャージ攻撃
    //==========================================
    m_ChargeAttack.nChargeCnt = 0;                              //チャージした時間をカウントする
    m_ChargeAttack.nPower = 0;                                  //チャージ攻撃の威力
    m_ChargeAttack.bState = false;                              //チャージ状態かどうか
    m_ChargeAttack.nChargeLevel = 0;                            //チャージ段階
    //==========================================================================================

    //=====================================
    //軌跡関係
    //=====================================
    m_pMeshOrbit = nullptr;
    //==========================================================================================

    //==========================================
    //ゲージ関係
    //==========================================
    m_pHpGauge = nullptr;             //体力ゲージへのポインタ
    m_pMpGauge = nullptr;             //MPゲージへのポインタ
    m_pGaugeFrame = nullptr;          //ゲージフレームへのポインタ
    //==========================================================================================

    //==========================================
    //カメラ関係
    //==========================================
    m_bReturnCamera = false;
    m_bStartFade = false;
    //==========================================================================================

    //==========================================
    //スコア関係
    //==========================================
    m_pScore = nullptr;//スコアへのポインタ
    //==========================================================================================

    //==========================================
    //リアクション関係
    //==========================================
    m_pUiReaction = nullptr;                      //リアクションへのポインタ
    m_nCntChengeUiReactionTime = 0;               //リアクションを変える時間をカウントする
    m_bChengeUiReactionFlag = false;              //リアクションを変化させるかどうか.
    //==================================================================================== ======

    //==========================================
    //コマンド関係
    //==========================================
    for (int nCnt = 0; nCnt < PLAYERCOMMAND_MAX; nCnt++)
    {//コマンドを発動したかどうかのフラグをfalseにする
        m_bJudgeCommand[nCnt] = false;
    }
    //==========================================================================================

    //==========================================
    //回復関係
    //==========================================
    m_Heal.nHealCnt = 0;  //ヒール状態のカウント
    m_Heal.bState = false;//ヒール状態かどうか
    m_Heal.bLastHeal = false;//回復に必要なMPが足りなくなった時用のフラグ
    //==========================================================================================

    //==========================================
    //状態異常
    //==========================================
    
    //滑る
    m_State.SlideState.nStateCnt = 0; //滑る状態になってからのカウント

    //減速
    m_State.DecelerationState.bState = false;//減速状態
    m_State.DecelerationState.nStateCnt = 0;//減速状態になってからのカウント

    //==========================================================================================

    //==========================================
    //サイコバースト関係
    //==========================================
    m_pLockOnUi = nullptr;
    //==========================================================================================

    //==========================================
    //無敵状態
    //==========================================
    m_bInvincible = false;              //無敵状態かどうか
    m_nCntInvincible = 0;               //無敵状態の時間をカウントする
    //==========================================================================================

    //==========================================
    //壁ジャンプ関係
    //==========================================
    m_nWallJumpBindTime = 0;//壁ジャンプが不可能な時間をカウントする
    m_bPossibleWallJump = true;//壁ジャンプが可能かどうか
    //==========================================================================================

    //==========================================
    //プレイヤーのパーツ関係
    //==========================================
    for (int nCnt = 0; nCnt < m_nMAX_PLAYERPARTS; nCnt++)
    {
        m_apModelParts[nCnt] = nullptr;//モデルパーツへのポインタ
    }
    //==========================================================================================

    //モデルパーツのロード処理
    LoadModelParts();

    //m_apModelParts[10]->SetScale(ONE_VECTOR3 * 3.0f);

    //==========================================================================================

    //==========================================
    //慣性関係
    //=========================================
    m_fSlideInertia = 0.0f;//滑らせるための慣性
    m_fReturnInertia = 0.0f;//戻すための慣性
    //===========================================================================================

    //==========================================
    //モーション関係
    //==========================================
    m_nNowMotion = 1;//現在のモーション
    m_nNextMotion = 0;//次のモーション
    m_nCntFrame = 0;//現在のフレーム数
    m_nMotionOld = 0;//1f前のモーション
    //初期化
    for (int nCntMotion = 0; nCntMotion < m_nMAX_MOTION; nCntMotion++)
    {
        m_aMotion[nCntMotion].bLoop = false;//ループするかどうか
        m_aMotion[nCntMotion].nCntFrame = 0;//フレームカウント
        m_aMotion[nCntMotion].nCntKey = 0;  //キーカウント
        m_aMotion[nCntMotion].nNumKey = 0;  //キー総数
        for (int nCntKeySet = 0; nCntKeySet < m_nMAX_KEYSET; nCntKeySet++)
        {
            m_aMotion[nCntMotion].aKeySet[nCntKeySet].nFrame = 0;//フレーム最大数
            for (int nCntKey = 0; nCntKey < m_nMAX_PLAYERPARTS; nCntKey++)
            {
                m_aMotion[nCntMotion].aKeySet[nCntKeySet].aKey[nCntKey].Pos = NULL_VECTOR3;//位置
                m_aMotion[nCntMotion].aKeySet[nCntKeySet].aKey[nCntKey].Rot = NULL_VECTOR3;//向き
            }
        }
    }

    for (int nCnt = 0; nCnt < MOTIONTYPE_MAX; nCnt++)
    {
        m_bPriorityMotion[nCnt] = false;//モーションの優先度によって実行するモーションを変える
    }

    //モーションのロード処理
    LoadMotion();

    //==========================================================================================

    if (CScene::GetMode() == CScene::MODE_GAME)
    {
        if (CGame::GetDifficulty() == CDifficulty::DIFFICULTYTYPE_BOSSRUSH)
        {
            //全てのアクションを開放する
            for (int nCnt = 0; nCnt < BULLETACTION_MAX; nCnt++)
            {//全てのバレットアクションを最初から使用可能にする
                m_bUnlockBulletAction[nCnt] = true;
            }
            for (int nCnt = 0; nCnt < ACTIONTYPE_MAX; nCnt++)
            {//全てのアクションを最初から使用可能にする
                m_bUnlockAction[nCnt] = true;
            }
            for (int nCnt = 0; nCnt < m_nMAX_MAGICNUM; nCnt++)
            {//新しい魔法を解放する
                SetUnlockMagic(nCnt,NULL_VECTOR3,false);
            }
        }
    }
	return S_OK;
}
//==========================================================================================================

//====================================================
//終了処理
//====================================================
void CPlayer::Uninit()
{
    //=================================
    //魔法表示の終了処理
    //=================================
    if (m_pMagicDisp != nullptr)
    {
        m_pMagicDisp = nullptr;
    }
    //=================================================================

    //=================================
    //魔法表示ウインドウの終了処理
    //=================================
    if (m_pMagicWindow != nullptr)
    {
        m_pMagicWindow = nullptr;
    }
    //=================================================================

    //=================================
    //数値表示の終了処理
    //=================================
    if (m_pNumber != nullptr)
    {
        m_pNumber = nullptr;
    }
    //=================================================================

    //=================================
    //HP表示の終了処理
    //=================================
    if (m_pHpGauge != nullptr)
    {
        m_pHpGauge = nullptr;
    }
    //=================================================================

    //=================================
    //MP表示の終了処理
    //=================================
    if (m_pMpGauge != nullptr)
    {
        m_pMpGauge = nullptr;
    }
    //=================================================================
    
    //=================================
    //ロックオンUIの終了処理
    //=================================
    if (m_pLockOnUi != nullptr)
    {
        m_pLockOnUi = nullptr;
    }

    //=================================
    //モデルパーツの終了処理
    //=================================
    for (int nCnt = 0; nCnt < m_nMAX_PLAYERPARTS; nCnt++)
    {
        if (m_apModelParts[nCnt] != nullptr)
        {
            m_apModelParts[nCnt] = nullptr;
        }
    }
    //=================================================================

    CObjectX::Uninit();//Xオブジェクト終了
}
//==========================================================================================================

//====================================================
//別枠の終了処理
//====================================================
void CPlayer::ExtraUninit()
{
}
//==========================================================================================================

//====================================================
//更新処理
//====================================================
void CPlayer::Update()
{
    if (CScene::GetMode() == CScene::MODE_GAME && CManager::GetEventManager()->GetEventType() != CEventManager::EVENTTYPE_BOSSAPPEAR)
    {
        m_nRespawnCnt++;
        D3DXVECTOR3& pos = GetPos();                                   //モデルの位置の取得
        D3DXVECTOR3& PosOld = GetPosOld();                               //1f前の位置を設定
        D3DXVECTOR3& Move = GetMove();                                   //移動量を取得する
        D3DXVECTOR3& Rot = GetRot();                                   //モデルの向きの取得
        D3DXVECTOR3 Size = GetSize();                                   //サイズを取得
        float& fInertia = GetInertia();                                 //慣性を取得
        float fMoveX = 0.0f;                                            //X方向の移動量
        float fMoveZ = 0.0f;                                            //Z方向の移動量
        bool bAddMove = false;
        bool& bIsLanding = GetLanding();                                //地面にいるかどうか
        bool& bIsWalling = GetWalling();                                //壁にくっついているかどうか
        bool& bIsLiftLanding = GetLiftLanding();                        //リフトに乗っているかどうか
        bool& bIsJumping = GetJumping();                                //ジャンプしているかどうか
        bool& bDodge = GetDodgeState();                                 //回避状態かどうか
        bool bSuccessBlockCollisionX = false;                           //ブロックのX方向の当たり判定が成功したかどうか
        bool bSuccessBlockCollisionY = false;                           //ブロックのY方向の当たり判定が成功したかどうか
        CObject*& pLandingObj = GetLandingObj();                         //乗っているオブジェクトを取得

        //コマンド判定リセット
        ResetJudgeCommand();

        //減速状態の処理
        DecelerationProcess();

        //無敵状態の処理
        InvincibleProcess();

        //UIリアクション処理
        UiReactionProcess();

        //=====================================
        //原点に到達したときの位置の補正
        //=====================================
        AdjusOriginPosProcess();
        //===========================================================

        //重力の処理
        GravityProcess();

        //壁ジャンプ抑制処理
        WallJumpBindProcess();

        //=====================================================
        //移動処理
        //=====================================================
        if (CManager::GetInputKeyboard()->GetPress(DIK_W) == true || CManager::GetInputJoypad()->GetPress(CInputJoypad::JOYKEY_UP) == true)
        {
            fMoveZ = 1.0f;
        }
        else if (CManager::GetInputKeyboard()->GetPress(DIK_S) == true || CManager::GetInputJoypad()->GetPress(CInputJoypad::JOYKEY_DOWN) == true)
        {
            fMoveZ = -1.0f;
        }

        if (CManager::GetInputKeyboard()->GetPress(DIK_D) == true || CManager::GetInputJoypad()->GetPress(CInputJoypad::JOYKEY_RIGHT) == true)
        {
            fMoveX = 1.0f;
        }
        else if (CManager::GetInputKeyboard()->GetPress(DIK_A) == true || CManager::GetInputJoypad()->GetPress(CInputJoypad::JOYKEY_LEFT) == true)
        {
            fMoveX = -1.0f;
        }

        //============================
        // 移動ボタンを押していたら
        //============================
        if (fMoveX != 0.0f || fMoveZ != 0.0f)
        {
            m_bMove = true;//移動状態
        }
        else
        {
            m_nNextMotion = 0;
            m_bMove = false;//待機状態
        }

        if (m_nStopMoveCnt > 0)
        {//移動を止めるカウントが０以上なら
            m_bMove = false;//動きを止める
            m_nStopMoveCnt--;//カウントをへらしつづける　
        }

        if (m_bMove == true && bDodge == false && m_Heal.bState == false)
        {//移動ボタンを押している、回避状態じゃない、回復状態じゃないなら
            if (CScene::GetMode() == CScene::MODE_EDIT)
            {
                Move.z += cosf(atan2f(fMoveX, fMoveZ) + CManager::GetCamera()->GetRot().y) * m_fSpeed;
            }
            if (fMoveX != 0.0f)
            {
                m_nNextMotion = 1;
                Move.x += sinf(atan2f(fMoveX, 0.0f) + CManager::GetCamera()->GetRot().y) * m_fSpeed;
                Rot.y = atan2f(fMoveX,0.0f) + CManager::GetCamera()->GetRot().y + D3DX_PI;
            }

            m_bJudgeCommand[PLAYERCOMMAND00_MOVE] = true;//移動をしている
        }
        //==================================================================================================================================================

        //========================================================
        //アクション！
        //========================================================
        if (m_bUnlockAction[ACTIONTYPE00_DOUBLEJUMP] == false)
        {
            if (bDodge == false)
            {
                AdjustJump();//調整ジャンプ
            }
        }
        else
        {//ダブルジャンプ開放済みなら
            if (bDodge == false)
            {
                DoubleJump();//２段ジャンプ
            }
        }

        if (m_bUnlockAction[ACTIONTYPE01_DODGE] == true)
        {
            DodgeAction(fMoveX, fMoveZ);     //回避
        }
        //================================================================================================================================


        if (bDodge == false)
        {
            PlayerInertiaProcess();//プレイヤーの慣性の処理

            SetUseUpdatePos(true);
        }
        else
        {//回避状態の挙動のため保存した移動量で加算し続ける
            SetUseUpdatePos(false);
            PosOld = pos;
            pos += m_SaveMove;           //位置の更新
        }


        //オブジェクトX更新処理
        CObjectX::Update();

        m_State.SlideState.bState = false;//滑る状態をオフにする

       //==================================
       //軌跡を出す
       //==================================
        MeshOrbitProcess();
        //==================================================================================
        SetRot(Rot); //向きの設定

        if (CScene::GetMode() == CScene::MODE_GAME)
        {//ゲームモードの時だけアクション可能

            //NormalAttack();//通常攻撃
            ChargeAttack();//チャージ可能通常攻撃

            //魔法変更
            ChengeMagic();

            //魔法攻撃
            MagicAttack();
        }

        if (CManager::GetInputKeyboard()->GetTrigger(DIK_I) == true)
        {//体力減らす
            m_Status.nHp -= 1;
            m_pHpGauge->SetParam(m_Status.nHp);
        }

        //モーション優先設定
        MotionPriorityProcess();

        //モーション処理
        MotionProcess();

        //プレイヤー情報表示処理
        DispPlayerInfo();

        //壁ジャンプの処理
        WallJumpProcess();

        //当たり判定
        CollisionEnemy();
        CollisionItem();
        CollisionTrap();

        //移動量調整処理
        if (Move.y <= -20.0f)
        {
            Move.y = -20.0f;
        }

        //死亡処理
        if (pos.y < -100.0f)
        {
            ActivePlayerRespawn();
        }

#ifdef _DEBUG
        if(CManager::GetInputJoypad())
        
#endif // _DEBUG


        if (m_Status.nHp <= 0 && m_bStartFade == false)
        {
            m_bStartFade = true;
            CManager::GetSceneFade()->SetSceneFade(CFade::FADEMODE_IN, CScene::MODE_RESULT);
        }

    }
}
//==========================================================================================================

//====================================================
//描画処理
//====================================================
void CPlayer::Draw()
{
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice(); //デバイスへのポインタと取得
    D3DXMATRIX mtxRot, mtxTrans, mtxScale;                  //計算用マトリックス
    D3DMATERIAL9 matDef;                                              //現在のマテリアル保存用
    D3DXVECTOR3& Pos = GetPos();    //位置
    D3DXVECTOR3& Rot = GetRot();    //向き
    D3DXVECTOR3& Scale = GetScale();//拡大率
    D3DXMATRIX& mtxWorld = GetMatrixWorld();
    //現在のマテリアルを取得
    pDevice->GetMaterial(&matDef);

    //ワールドマトリックスの初期化
    D3DXMatrixIdentity(&GetMatrixWorld());

    //大きさを反映
    D3DXMatrixScaling(&mtxScale, Scale.x, Scale.y, Scale.z);
    D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScale);

    //向きを反映
    D3DXMatrixRotationYawPitchRoll(&mtxRot, Rot.y, Rot.x, Rot.z);
    D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

    //位置を反映
    D3DXMatrixTranslation(&mtxTrans, Pos.x, Pos.y, Pos.z);
    D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

    //ワールドマトリックスの設定
    pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

    //=======================================
    //描画の調整
    //=======================================

    //法線の長さを１にする。（スケールなどを使った時は、必ず使う。)
    pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
    //================================================================================================================

    //=======================================
    //モデルパーツの描画
    //=======================================
    for (int nCnt = 0; nCnt < m_nMAX_PLAYERPARTS; nCnt++)
    {
        m_apModelParts[nCnt]->ExtraDraw();
    }
    //================================================================================================================


    ////マテリアルへのポインタを取得
    //pMat = (D3DXMATERIAL*)GetObjectXInfo().pBuffMat->GetBufferPointer();

    ////==========================================================================
    ////マテリアルの数分、テクスチャを読み込む。
    ////==========================================================================
    //for (int nCntMat = 0; nCntMat < (int)GetObjectXInfo().dwNumMat; nCntMat++)
    //{
    //    //色合いの設定
    //    pMat[nCntMat].MatD3D.Diffuse = GetObjectXInfo().Diffuse[nCntMat];

    //    //マテリアルの設定
    //    pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

    //    //テクスチャを設定する
    //    pDevice->SetTexture(0, GetObjectXInfo().pTexture[nCntMat]);

    //    //モデル（パーツ）の描画
    //    GetObjectXInfo().pMesh->DrawSubset(nCntMat);
    //}
    ////================================================================================================================

    //=======================================
    //描画の調整を元に戻す
    //=======================================

    //法線の長さを１にする。（スケールなどを使った時は、必ず使う。)
    pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
    //================================================================================================================

    ////保存していたマテリアルを戻す
    //pDevice->SetMaterial(&matDef);

}
//==========================================================================================================

//====================================================
//死亡フラグを設定
//====================================================
void CPlayer::SetDeath()
{
    if (GetUseDeath() == true)
    {
        //=================================
        //魔法表示の終了処理
        //=================================
        if (m_pMagicDisp != nullptr)
        {
            m_pMagicDisp->SetUseDeath(true);
            m_pMagicDisp->CObject::SetDeath();
            m_pMagicDisp = nullptr;
        }
        //=================================================================

        //=================================
        //魔法表示ウインドウの終了処理
        //=================================
        if (m_pMagicWindow != nullptr)
        {
            m_pMagicWindow->SetUseDeath(true);
            m_pMagicWindow->CObject::SetDeath();
            m_pMagicWindow = nullptr;
        }
        //=================================================================

        //=================================
        //数値表示の終了処理
        //=================================
        if (m_pNumber != nullptr)
        {
            m_pNumber->SetUseDeath(true);
            m_pNumber->CObject::SetDeath();
            m_pNumber = nullptr;
        }
        //=================================================================

        //=================================
        //HP表示の終了処理
        //=================================
        if (m_pHpGauge != nullptr)
        {
            m_pHpGauge->SetUseDeath(true);
            m_pHpGauge->CObject::SetDeath();
            m_pHpGauge = nullptr;
        }
        //=================================================================

        //=================================
        //MP表示の終了処理
        //=================================
        if (m_pMpGauge != nullptr)
        {
            m_pMpGauge->SetUseDeath(true);
            m_pMpGauge->CObject::SetDeath();
            m_pMpGauge = nullptr;
        }
        //=================================================================

        //==================================
        //モデルパーツの死亡フラグ設定処理
        //==================================
        for (int nCnt = 0; nCnt < m_nMAX_PLAYERPARTS; nCnt++)
        {
            if (m_apModelParts[nCnt] != nullptr)
            {
                m_apModelParts[nCnt]->SetUseDeath(true);
                m_apModelParts[nCnt]->SetDeath();
                m_apModelParts[nCnt] = nullptr;
            }
        }
        //=================================================================

        //==================================
        //スコアへのポインタ
        //==================================
        if (m_pScore != nullptr)
        {
            m_pScore->SetUseDeath(true);
            m_pScore->SetDeath();
            m_pScore = nullptr;
        }
        //=================================================================

        //==================================
        //軌跡の死亡フラグ設定処理
        //==================================
        if (m_pMeshOrbit != nullptr)
        {
            m_pMeshOrbit->SetUseDeath(true);
            m_pMeshOrbit->SetDeath();
            m_pMeshOrbit = nullptr;
        }
        //=================================================================

        //==================================
        //ロックオンUIの死亡フラグ設定処理
        //==================================
        if (m_pLockOnUi != nullptr)
        {
            m_pLockOnUi->SetUseDeath(true);
            m_pLockOnUi->SetDeath();
            m_pLockOnUi = nullptr;
        }
        //=================================================================

        //==================================
        //ゲージフレームの死亡フラグ設定処理
        //==================================
        if (m_pGaugeFrame != nullptr)
        {
            m_pGaugeFrame->SetUseDeath(true);
            m_pGaugeFrame->SetDeath();
            m_pGaugeFrame = nullptr;
        }
        //=================================================================

        //======================================
        //UIリアクションの死亡フラグ設定処理
        //======================================
        if (m_pUiReaction != nullptr)
        {
            m_pUiReaction->SetUseDeath(true);
            m_pUiReaction->SetDeath();
            m_pUiReaction = nullptr;
        }
        //=================================================================

        //======================================
        //消費MPゲージの死亡フラグ設定処理
        //======================================
        if (m_pUsageMpGauge != nullptr)
        {
            m_pUsageMpGauge->SetUseDeath(true);
            m_pUsageMpGauge->SetDeath();
            m_pUsageMpGauge = nullptr;
        }
        //=================================================================

        SetUseDeath(true);
        CObject::SetDeath();
    }
}
//===========================================================================================================

//====================================================
//モデル情報の破棄
//====================================================
void CPlayer::Unload()
{
    //メッシュの破棄
    if (m_pMeshTemp != NULL)
    {
        m_pMeshTemp->Release();
        m_pMeshTemp = NULL;
    }

    //マテリアルの破棄
    if (m_pBuffMatTemp != NULL)
    {     
        m_pBuffMatTemp->Release();
        m_pBuffMatTemp = NULL;
    }

    //テクスチャの破棄
    for (int nCntMat = 0; nCntMat < m_nMAX_MAT; nCntMat++)
    {
        if (m_pTextureTemp[nCntMat] != NULL)
        {
            m_pTextureTemp[nCntMat]->Release();
            m_pTextureTemp[nCntMat] = NULL;
        }
    }

}
//==========================================================================================================

//====================================================
//プレイヤーの生成
//====================================================
CPlayer* CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, D3DXVECTOR3 Scale)
{
    CPlayer* pPlayer = new CPlayer;                                                                             //プレイヤーを生成

    bool bSuccess = pPlayer->CObject::GetCreateSuccess();
    int nIdx = 0;//モデルのインデックス
    if (bSuccess == true)
    {
        if (pPlayer != nullptr)
        {
            pPlayer->Init();                                                                 //初期化処理
            pPlayer->SetUseDeath(false);                                                     //死亡フラグを発動するかどうかを設定する
            pPlayer->CObject::SetType(CObject::TYPE_PLAYER);                                 //オブジェクトの種類を決める
            pPlayer->CObjectX::SetObjXType(CObjectX::OBJECTXTYPE_PLAYER);                    //オブジェクトXのタイプを設定
            pPlayer->CObjectX::SetTypeNum(0);                                                //オブジェクトXごとのタイプ番号を設定
            pPlayer->SetReSpawnPos(pos);                                                     //リスポーン位置
            pPlayer->m_Status.nHp = m_nINIT_HP;                                              //体力
            pPlayer->m_Status.nMaxHp = m_nINIT_HP;                                           //最大体力
            pPlayer->m_Status.nMp = m_nINIT_MP;                                              //マジックポイント
            pPlayer->m_Status.nMaxMp = m_nINIT_MP;                                           //最大マジックポイント
            pPlayer->m_pMeshOrbit = CMeshOrbit::Create(CMeshOrbit::MESHORBITTYPE00_NORMAL);  //軌跡メッシュを生成
            pPlayer->SetLife(1);
            pPlayer->SetMaxLife(1);
            pPlayer->SetAutoSubLife(false);//自動的に体力を減らすかどうか
            //=============================
            //UIの生成
            //=============================
            pPlayer->m_pHpGauge = CGauge::Create(CGauge::GAUGETYPE_PLAYERHP, m_nINIT_HP, 290.0f, 12.0f, D3DXVECTOR3(141.0f, SCREEN_HEIGHT - 72.0f, 0.0f));//体力ゲージを設定
            pPlayer->m_pMpGauge = CGauge::Create(CGauge::GAUGETYPE_PLAYERMP, m_nINIT_MP, 290.0f, 12.0f, D3DXVECTOR3(141.0f, SCREEN_HEIGHT - 45.0f, 0.0f));//MPゲージを設定
            pPlayer->m_pMpGauge->SetParam(m_nINIT_MP);//初期パラメータをMAXにする
            pPlayer->m_pGaugeFrame = CUi::Create(CUi::UITYPE_CHARACTERGAUGEFRAME, CObject2D::POLYGONTYPE01_SENTERROLLING, 400.0f, 100.0f,
                SENTER_VECTOR3 + D3DXVECTOR3(-400.0f, 300.0f, 0.0f), NULL_VECTOR3, NULL_VECTOR3, NORMAL_COL);//ゲージフレームを生成
            pPlayer->m_pScore = CScore::Create(1000);//スコア初期値
            pPlayer->m_pUiReaction = CUi::Create(CUi::UITYPE_FACENORMAL, CObject2D::POLYGONTYPE01_SENTERROLLING, 80.0f, 80.0f, D3DXVECTOR3(90.0f, SCREEN_HEIGHT - 60.0f, 0.0f),
                NULL_VECTOR3, NULL_VECTOR3, NORMAL_COL);//リアクションのUIを生成
            //=====================================================================================================================================================================================

            pPlayer->SetPos(pos);                                                                               //位置の設定
            pPlayer->SetPosOld(pos);                                                                            //1f前の位置を設定
            pPlayer->SetSupportPos(pos);                                                                        //設置位置
            pPlayer->SetRot(rot);                                                                               //向きの設定
            pPlayer->SetScale(Scale);                                                                           //拡大率の設定
            pPlayer->SetFormarScale(Scale);                                                                                  //元の拡大率を設定する
            pPlayer->SetRotType((CObjectX::ROTTYPE)(0));                                                        //向きの種類を設定
            pPlayer->SizeSet();                                                                                 //モデルパーツ同士を比べてサイズを設定
        }
    }
    else
    {//オブジェクトに空きがなかったので破棄する
        delete pPlayer;
        pPlayer = nullptr;
    }

	return pPlayer;
}
//==========================================================================================================

//=======================================================
//プレイヤーにダメージを与える
//=======================================================
void CPlayer::SetDamage(int nDamage, int nHitStopTime)
{
    bool& bDodge = GetDodgeState();
    if (m_bInvincible == false && nDamage > 0 && bDodge == false && m_nRespawnCnt > 10)
    {//無敵状態じゃなかったら

        m_Status.nHp -= 1;
        m_pHpGauge->SetParam(m_Status.nHp);
        m_ChargeAttack.nChargeLevel = 0;//チャージをリセット
        for (int nCnt = 0; nCnt < m_nMAX_PLAYERPARTS; nCnt++)
        {
            m_apModelParts[nCnt]->SetColor(COLORTYPE_RED, 10);
        }
        m_bInvincible = true;//無敵状態にする

        SetPlayerReaction(CPlayer::PLAYERREACTION_DAMAGE, 60);

        CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_DAMAGE_001);
    }
}
//==========================================================================================================

//=======================================================
//MPを設定する
//=======================================================
void CPlayer::SetMp(int nMp)
{
    if (m_pMpGauge != nullptr)
    {//MPゲージが存在していたら
        if (m_Status.nMp < m_Status.nMaxMp)
        {//MPがMAXじゃなければ
            m_Status.nMp += nMp;
            if (m_Status.nMp > m_Status.nMaxMp)
            {
                m_Status.nMp = m_Status.nMaxMp;
            }

            m_pMpGauge->SetParam(m_Status.nMp);
        }
    }
}
//==========================================================================================================

//=======================================================
//反映しない移動量を設定する
//=======================================================
void CPlayer::NotSetMove(bool bX, bool bY, bool bZ)
{
    D3DXVECTOR3& Move = GetMove();                                   //移動量を取得する
    if (bX == true)
    {
        Move.x = 0.0f;
    }
    if (bY == true)
    {
        Move.y = 0.0f;
    }
    if (bZ == true)
    {
        Move.z = 0.0f;
    }
}
//==========================================================================================================

//=======================================================
//位置を強制する
//=======================================================
void CPlayer::SetComplusionPos(D3DXVECTOR3 pos)
{
    m_CompulsionPos = pos;
    m_bCompulsionPos = true;
}
//==========================================================================================================

//========================================================
//プレイヤーを破棄する処理
//========================================================
void CPlayer::ReleasePlayer()
{
    Release();
}
//==========================================================================================================

//========================================================
//プレイヤーを破棄する処理
//========================================================
void CPlayer::BlockCollision()
{

}
//==========================================================================================================

//========================================================
//減速状態の処理
//========================================================
void CPlayer::DecelerationProcess()
{
    if (m_State.DecelerationState.nStateCnt <= 0)
    {
        m_State.DecelerationState.nStateCnt = 0;
        m_State.DecelerationState.bState = false;
        m_fSpeed = m_fNORMALSPEED;
        m_fJump = m_fNORMALJUMPPOWER;
    }

    if (m_State.DecelerationState.bState == true)
    {
        m_State.DecelerationState.nStateCnt--;
        m_fSpeed = m_fNORMALSPEED / 2.0f;
        m_fJump = m_fNORMALJUMPPOWER / 4.0f;
    }
}
//==========================================================================================================

//========================================================
//コマンド判定をリセットする
//========================================================
void CPlayer::ResetJudgeCommand()
{
    for (int nCnt = 0; nCnt < PLAYERCOMMAND_MAX; nCnt++)
    {
        m_bJudgeCommand[nCnt] = false;
    }
}
//==========================================================================================================

//========================================================
//UIリアクションの処理
//========================================================
void CPlayer::UiReactionProcess()
{
    if (m_bChengeUiReactionFlag == true)
    {
        m_nCntChengeUiReactionTime--;

        if (m_nCntChengeUiReactionTime == 0)
        {//リアクションをリセットする
            m_pUiReaction->SetUiType(CUi::UITYPE_FACENORMAL);
            m_bChengeUiReactionFlag = false;
        }
    }
}
//===================================================================================================================================================

//========================================================
//通常攻撃処理
//========================================================
void CPlayer::NormalAttack()
{
    D3DXVECTOR3& Pos = GetPos();
    D3DXVECTOR3 &Rot = GetRot();
    if (CManager::GetInputKeyboard()->GetRepeat(DIK_N) == true)
    {//バレット発射
        CBullet::Create(CBullet::BULLETTYPE_PLAYER, CBullet::BULLETTEX_PLAYER, 300,5, 40.0f, 40.0f, Pos, D3DXVECTOR3(sinf(Rot.y - D3DX_PI) * 10.0f, 0.0f, cosf(Rot.y - D3DX_PI) * 10.0f));
        m_pMpGauge->SetParam(m_Status.nMp);
    }
}
//==========================================================================================================

//========================================================
//魔法攻撃処理
//========================================================
void CPlayer::MagicAttack()
{
    D3DXVECTOR3& pos = GetPos();                                   //モデルの位置の取得
    D3DXVECTOR3& Rot = GetRot();                                   //モデルの向きの取得
    D3DXVECTOR3 SummonPos = NULL_VECTOR3;
    SummonPos.x = pos.x + float(rand() % 300 - 150);
    SummonPos.y = pos.y + float(rand() % 300 - 150);
    SummonPos.z = pos.z;
    //技選択
    bool bSuccess = false;


    //============================================
    //現在選んでいる魔法の消費MPを表示する
    //============================================
    if (m_pUsageMpGauge != nullptr)
    {
        m_pUsageMpGauge->SetParam(m_nPLAYERUSAGEMP[m_pMagicDisp->GetMagicDispType()]);
 
    }


    if ((CManager::GetInputKeyboard()->GetTrigger(DIK_M) == true || CManager::GetInputJoypad()->GetTrigger(CInputJoypad::JOYKEY_B) == true) && m_pMagicDisp != nullptr &&
        m_bPossibleMagic == true)
    {
        m_bPriorityMotion[MOTIONTYPE02_ATTACK] = true;
        if (m_Status.nMp >= m_nPLAYERUSAGEMP[m_pMagicDisp->GetMagicDispType()])
        {
            if (m_pMagicDisp->GetMagicDispType() != CMagicDisp::MAGICDISP06_QUICKHEAL)
            {
                m_Status.nMp -= m_nPLAYERUSAGEMP[m_pMagicDisp->GetMagicDispType()];//MPを消費する
            }
            else
            {
                //
            }
            m_nUseMagicNum = int(m_pMagicDisp->GetMagicDispType());//現在使用している魔法番号を格納
            m_pMpGauge->SetParam(m_Status.nMp);                     //MPのパラメータを設定

            if (m_pMagicDisp->GetMagicDispType() == CMagicDisp::MAGICDISP00_FIRE && m_bPossibleMagic == true)
            {
                bSuccess = true;
                FireBallProcess();//ファイアボールの処理
            }
            else if (m_pMagicDisp->GetMagicDispType() == CMagicDisp::MAGICDISP01_BEAM && m_Status.nMp && m_bPossibleMagic == true)
            {
                bSuccess = true;
                BeamProcess();
            }
            else if (m_pMagicDisp->GetMagicDispType() == CMagicDisp::MAGICDISP02_ICE && m_bPossibleMagic == true)
            {//フラワードアイス
                m_bPossibleMagic = false;                               //魔法使用可能状態をオフに
                m_nNextUseMagic = 120;                                  //次の魔法が使えるまで
                bSuccess = true;
            }
            else if (m_pMagicDisp->GetMagicDispType() == CMagicDisp::MAGICDISP03_THUNDER && m_bPossibleMagic == true)
            {//サンダースパーク
                m_bPossibleMagic = false;                               //魔法使用可能状態をオフに
                bSuccess = true;
                m_nNextUseMagic = 120;                                  //次の魔法が使えるまで
            }
            else if (m_pMagicDisp->GetMagicDispType() == CMagicDisp::MAGICDISP04_SWORD && m_bPossibleMagic == true)
            {//サイコブレイド
                PsychoBladeProcess();
                bSuccess = true;
            }
            else if (m_pMagicDisp->GetMagicDispType() == CMagicDisp::MAGICDISP05_PLANTSHIELD && m_bPossibleMagic == true)
            {//プラントシールド
                PlantShieldProcess();
                bSuccess = true;
            }
            else if (m_pMagicDisp->GetMagicDispType() == CMagicDisp::MAGICDISP06_QUICKHEAL && m_bPossibleMagic == true)
            {//クイックヒール
                m_bPossibleMagic = false;                               //魔法使用可能状態をオフにする
                m_nNextUseMagic = 9999;
                bSuccess = true;
            }
            else if (m_pMagicDisp->GetMagicDispType() == CMagicDisp::MAGICDISP07_CROSSBOMB && m_bPossibleMagic == true)
            {//クロスボム
                bSuccess = true;

                CrossBombProcess();                 //処理
            }
            else if (m_pMagicDisp->GetMagicDispType() == CMagicDisp::MAGICDISP08_BLACKHOLE && m_bPossibleMagic == true)
            {//ブラックホール
                bSuccess = true;

                BlackHoleProcess();                 //処理
            }
            else if (m_pMagicDisp->GetMagicDispType() == CMagicDisp::MAGICDISP09_PHYCOBURST && m_bPossibleMagic == true)
            {//サイコバースト
                m_bPossibleMagic = false;           //魔法使用可能状態をオフにする
                m_nNextUseMagic = 9999;             //次に魔法が使用できるまでの時間    
                bSuccess = true;

                if (m_pLockOnUi == nullptr)
                {//技選択時にUIを生成
                    m_pLockOnUi = CUi3D::Create(CUi3D::UI3DTYPE00_LOCKON, 2000, 120.0f, 120.0f, pos, NULL_VECTOR3);
                    m_pLockOnUi->SetUseDeath(false);
                }
            }
        }

        if (bSuccess == true)
        {
            m_bJudgeCommand[PLAYERCOMMAND09_MAGIC] = true;
        }
    }

    //技発動
    if (m_bPossibleMagic == false)
    {
        m_nUseMagicTime++;//魔法使用時間をカウントする


        if (m_nUseMagicNum == 2)
        {
            FlowerdIceProcess();
        }
        else if (m_nUseMagicNum == 3)
        {
            ThunderBurstProcess();
        }
        else if (m_nUseMagicNum == 6)
        {
            QuickHealProcess();
        }
        else if(m_nUseMagicNum == 9)
        {
            PhycoBurstProcess();
        }

        if (m_nUseMagicTime >= m_nNextUseMagic)
        {
            //魔法使用状態のリセット
            ResetUseMagic();
        }
    }

}
//==========================================================================================================

//========================================================
//調整ジャンプ
//========================================================
void CPlayer::AdjustJump()
{
    bool& bIsLanding = GetLanding();                                 //地面にいるかどうか
    bool& bIsWalling = GetWalling();                                 //壁にくっついているかどうか
    bool& bIsJumping = GetJumping();                                 //ジャンプしているかどうか
    D3DXVECTOR3& Move = GetMove();                                   //移動量を取得する
    float fRandRot = 0.0f;
    D3DXVECTOR3 Pos = GetPos();                                      //位置を取得する
    D3DXVECTOR3 Rot = GetRot();                                      //向きを取得する
    D3DXVECTOR3 ParticleMove = NULL_VECTOR3;
    D3DXVECTOR3 VtxMax = GetVtxMax();
    D3DXVECTOR3 VtxMin = GetVtxMin();
    if (CManager::GetInputKeyboard()->GetTrigger(DIK_SPACE) == true || CManager::GetInputJoypad()->GetTrigger(CInputJoypad::JOYKEY_A) == true)
    {//ジャンプ開始のフラグをオンにする（プレスにすると、地面に着くたびジャンプしてしまうのでトリガーで！）
        if (bIsLanding == true)
        {
            m_bPriorityMotion[MOTIONTYPE03_JUMP] = true;
            bIsJumping = true;
            m_nJumpPushCnt = 0;
            CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_JUMP_000);
        }

        if (m_bUnlockAction[ACTIONTYPE03_WALLJUMP] == true)
        {//壁ジャンプが開放されていたら
            if (bIsWalling == true)
            {
                if (m_bPossibleWallJump == true)
                {//壁ジャンプが可能なら
                    m_bJudgeCommand[PLAYERCOMMAND03_WALLJUMP] = true;//壁ジャンプをしている
                    bIsJumping = true;
                    m_nJumpPushCnt = 0;
                    CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_JUMP_000);
                }
            }
        }
    }

    if (CManager::GetInputKeyboard()->GetPress(DIK_SPACE) == true || CManager::GetInputJoypad()->GetPress(CInputJoypad::JOYKEY_A) == true)
    {
        if (bIsJumping == true && m_nJumpPushCnt < m_nMAX_JAMPPUSHCNT)
        {//ジャンプ中にボタンを押す時間の分だけ高さが変わるようにする。ボタンを押す長さの最大カウントが最頂点
            Move.y = m_fJump;
            m_nJumpPushCnt++;
            m_bJudgeCommand[PLAYERCOMMAND01_JUMP] = true;//ジャンプをしている
        }
    }
    else
    {//ボタンを話した後にもう一回押すと、連続ジャンプが可能になってしまうのでボタンを話したら次地面に着くまでジャンプ出来なくする
        bIsJumping = false;
    }
}
//==========================================================================================================

//========================================================
//２段ジャンプ
//========================================================
void CPlayer::DoubleJump()
{
    D3DXVECTOR3& Move = GetMove();                                   //移動量を取得する
    bool& bIsWalling = GetWalling();                                 //壁にくっついているかどうか
    CModel* pModel = nullptr;
    bool& bIsJumping = GetJumping();                                 //ジャンプしているかどうか
    if (CManager::GetInputKeyboard()->GetTrigger(DIK_SPACE) == true || CManager::GetInputJoypad()->GetTrigger(CInputJoypad::JOYKEY_A) == true)
    {//ジャンプ開始のフラグをオンにする（プレスにすると、地面に着くたびジャンプしてしまうのでトリガーで！）
        if (m_nJumpCnt < m_nMAX_JAMPNUM)
        {
            if (m_nJumpCnt == 1)
            {
                m_bJudgeCommand[PLAYERCOMMAND11_DOUBLEJUMP] = true;
                pModel = CModel::Create(CModel::MODELTYPE_RING, GetPos(), NULL_VECTOR3, NULL_VECTOR3, ONE_VECTOR3 * 1.0f, CObjectX::ROTTYPE_NORMAL);
                pModel->SetUseAddScale(ONE_VECTOR3 * 0.2f, true);
                pModel->SetLife(30);
                pModel->SetMaxLife(30);
                pModel->SetAutoSubLife(true);
                pModel->SetUseRatioLifeAlpha(true);                
            }
            bIsJumping = true;
            m_nJumpPushCnt = 0;
            m_nJumpCnt++;      //ジャンプ回数のカウントをする
            m_bPriorityMotion[MOTIONTYPE03_JUMP] = true;
            m_bJudgeCommand[PLAYERCOMMAND01_JUMP] = true;//ジャンプをしている
            CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_JUMP_000);
        }

        if (m_bUnlockAction[ACTIONTYPE03_WALLJUMP] == true)
        {//壁ジャンプが開放されていたら
            if (bIsWalling == true)
            {
                if (m_bPossibleWallJump == true)
                {//壁ジャンプ可能なら
                    m_bJudgeCommand[PLAYERCOMMAND03_WALLJUMP] = true;//壁ジャンプをしている
                    bIsJumping = true;
                    m_nJumpPushCnt = 0;
                    m_nJumpCnt = 1;    //（壁ジャンプ発動時に、０にすると、３段ジャンプが可能になってしまうので、１で固定）
                    CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_JUMP_000);
                }
            }
        }
    }

    if (CManager::GetInputKeyboard()->GetPress(DIK_SPACE) == true || CManager::GetInputJoypad()->GetPress(CInputJoypad::JOYKEY_A) == true)
    {
        if (bIsJumping == true && m_nJumpPushCnt < m_nMAX_JAMPPUSHCNT)
        {//ジャンプ中にボタンを押す時間の分だけ高さが変わるようにする。ボタンを押す長さの最大カウントが最頂点
            Move.y = m_fJump;
            m_nJumpPushCnt++;
        }
    }
    else
    {//ボタンを話した後にもう一回押すと、連続ジャンプが可能になってしまうのでボタンを話したら次地面に着くまでジャンプ出来なくする
        bIsJumping = false;
        m_nJumpPushCnt = 0;
    }
}
//===================================================================================================================================================

//========================================================
//魔法変更処理
//========================================================
void CPlayer::ChengeMagic()
{
    bool bChenge = false;//魔法を選んだかどうか
    if (m_bPossibleMagic == true && m_pMagicDisp != nullptr)
    {//魔法使用可能状態なら
        if (CManager::GetInputKeyboard()->GetTrigger(DIK_L) == true || CManager::GetInputJoypad()->GetTrigger(CInputJoypad::JOYKEY_RB) == true)
        {
            bChenge = true;//魔法を選んだ
            m_nNowMagicType++;//魔法をインクリメント
        }
        if (CManager::GetInputKeyboard()->GetTrigger(DIK_K) == true || CManager::GetInputJoypad()->GetTrigger(CInputJoypad::JOYKEY_LB) == true)
        {
            bChenge = true;//魔法を選んだ
            m_nNowMagicType--;//魔法をデクリメント
        }

        if (bChenge == true)
        {
            m_bJudgeCommand[PLAYERCOMMAND10_MAGICCHENGE] = true;//魔法を変更

            CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_DECISION_001);

            if (m_nNowMagicType >= m_nMagicNum)
            {//持っている魔法の数の最大に達したら
                m_nNowMagicType = 0;
            }
            else if (m_nNowMagicType < 0)
            {//配列０番以下を指定したら
                m_nNowMagicType = m_nMagicNum - 1;
            }

            m_pMagicDisp->SetMagicDispType((CMagicDisp::MAGICDISPTYPE)(m_nMagicSlot[m_nNowMagicType]));
        }
    }
}
//===================================================================================================================================================

//========================================================
//ファイアボールの処理
//========================================================
void CPlayer::FireBallProcess()
{
    D3DXVECTOR3 Pos = GetPos();                                   //モデルの位置の取得
    D3DXVECTOR3 Rot = GetRot();                                   //モデルの向きの取得

    CAttackPlayer::Create(CAttack::ATTACKTYPE00_FIRE,DamageResult(m_nFIREBALL_POWER), 300, Pos, D3DXVECTOR3(sinf(Rot.y - D3DX_PI) * 10.0f, 0.0f, cosf(Rot.y - D3DX_PI) * 10.0f),
        ONE_VECTOR3, -Rot, CAttack::COLLISIONTYPE00_SQUARE, D3DXVECTOR3(0.0f, 0.0f, 0.2f), true, 5);
}
//===================================================================================================================================================

//========================================================
//ビームの処理
//========================================================
void CPlayer::BeamProcess()
{
    D3DXVECTOR3 Pos = GetPos();                                   //モデルの位置の取得
    D3DXVECTOR3 Rot = GetRot();                                   //モデルの向きの取得

    if (Rot.y >= D3DX_PI)
    {
        CAttackPlayer::Create(CAttack::ATTACKTYPE04_BEAM, DamageResult(m_nBEAMWHIP_POWER), 45, Pos, NULL_VECTOR3, D3DXVECTOR3(1.0f, 2.0f, 1.0f), NULL_VECTOR3, CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, D3DXVECTOR3(0.0f, 0.0f, -0.05f), true, 5);
    }
    else
    {
        CAttackPlayer::Create(CAttack::ATTACKTYPE04_BEAM, DamageResult(m_nBEAMWHIP_POWER), 45, Pos, NULL_VECTOR3, D3DXVECTOR3(1.0f, 2.0f, 1.0f), NULL_VECTOR3, CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, D3DXVECTOR3(0.0f, 0.0f, 0.05f), true, 5);
    }
}
//===================================================================================================================================================

//========================================================
//フラワードアイスの処理
//========================================================
void CPlayer::FlowerdIceProcess()
{
    D3DXVECTOR3& pos = GetPos();                                   //モデルの位置の取得
    D3DXVECTOR3& Rot = GetRot();                                   //モデルの向きの取得
    D3DXVECTOR3 SummonPos = NULL_VECTOR3;
    SummonPos.x = pos.x + float(rand() % 300 - 150);
    SummonPos.y = pos.y + float(rand() % 300 - 150);
    SummonPos.z = pos.z;


    if (m_nUseMagicTime % 4 == 0 && m_nUseMagicNum == 2)
    {
        CAttackPlayer::Create(CAttack::ATTACKTYPE05_ICE, DamageResult(m_nFLOWERDICE_POWER), 90, SummonPos, NULL_VECTOR3, NULL_VECTOR3, NULL_VECTOR3, CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, NULL_VECTOR3, true, 12);
    }

}
//===================================================================================================================================================

//========================================================
//サンダーバーストの処理
//========================================================
void CPlayer::ThunderBurstProcess()
{
    D3DXVECTOR3& pos = GetPos();                                   //モデルの位置の取得

    for (int nCnt = 0; nCnt < 6; nCnt++)
    {
        if (m_nUseMagicTime == 20 * nCnt)
        {
            CAttackPlayer::Create(CAttack::ATTACKTYPE06_THUNDER, DamageResult(m_nTHUNDERBURST_POWER), 80, D3DXVECTOR3(pos.x - 50.0f * nCnt, pos.y + 300.0f, pos.z), D3DXVECTOR3(sinf(D3DX_PI) * 14.0f, cosf(D3DX_PI) * 14.0f, 0.0f), ONE_VECTOR3, NULL_VECTOR3, CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, NULL_VECTOR3, false, 0);
            CAttackPlayer::Create(CAttack::ATTACKTYPE06_THUNDER, DamageResult(m_nTHUNDERBURST_POWER), 80, D3DXVECTOR3(pos.x + 50.0f * nCnt, pos.y + 300.0f, pos.z), D3DXVECTOR3(sinf(D3DX_PI) * 14.0f, cosf(D3DX_PI) * 14.0f, 0.0f), ONE_VECTOR3, NULL_VECTOR3, CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, NULL_VECTOR3, false, 0);
        }
    }

}
//===================================================================================================================================================

//========================================================
//サイコブレイドの処理
//========================================================
void CPlayer::PsychoBladeProcess()
{
    D3DXVECTOR3& pos = GetPos();                                   //モデルの位置の取得

    m_bPossibleMagic = false;                               //魔法使用可能状態をオフに
    m_nUseMagicNum = 4;                                     //使用魔法
    m_nNextUseMagic = 300;                                  //次の魔法が使えるまで

    CAttackPlayer::Create(CAttack::ATTACKTYPE07_MAGICSWORD, DamageResult(m_nMAGICSWORD_POWER), 300, pos, NULL_VECTOR3, D3DXVECTOR3(1.0f, 2.0f, 1.0f), NULL_VECTOR3, CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, NULL_VECTOR3, false, 5);
}
//===================================================================================================================================================

//=========================================================
//プラントシールドの処理
//=========================================================
void CPlayer::PlantShieldProcess()
{
    D3DXVECTOR3 Pos = GetPos();                                   //モデルの位置の取得
    D3DXVECTOR3 Rot = GetRot();                                   //モデルの向きの取得
    CAttackPlayer* pAttackPlayer = nullptr;                       //プレイヤーの攻撃代入用

    pAttackPlayer = CAttackPlayer::Create(CAttack::ATTACKTYPE12_VINE, DamageResult(m_nPLANTSHIELD_POWER), 450, D3DXVECTOR3(Pos.x - 70.0f, Pos.y, Pos.z), NULL_VECTOR3,
        D3DXVECTOR3(1.0f, 0.0f, 1.0f) , NULL_VECTOR3, CAttack::COLLISIONTYPE02_TRUERECTANGLEXY,NULL_VECTOR3, false, 20);

    pAttackPlayer->SetVineDelBulletNum(20);//消せる弾の数を２０に設定

    pAttackPlayer = CAttackPlayer::Create(CAttack::ATTACKTYPE12_VINE, DamageResult(m_nPLANTSHIELD_POWER), 450,D3DXVECTOR3(Pos.x + 70.0f,Pos.y,Pos.z),NULL_VECTOR3,
        D3DXVECTOR3(1.0f,0.0f,1.0f), NULL_VECTOR3, CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, NULL_VECTOR3, false, 20);

    pAttackPlayer->SetVineDelBulletNum(20);//消せる弾の数を２０に設定


}
//===================================================================================================================================================

//========================================================
//クイックヒールの処理
//========================================================
void CPlayer::QuickHealProcess()
{

    //備考：回復に必要なMPが足りないとそもそもこの処理は呼ばれない。必要なMPが４０だったとき、現在のMPが４５だったとき、５で止めたいので、回復中に、
    //必要MPを現在のMPが切ったとき、最後の回復というフラグを発動し、回復が完了した後、この処理を終わらせる。

    if (m_Status.nMp < m_nPLAYERUSAGEMP[CMagicDisp::MAGICDISP06_QUICKHEAL])
    {
        m_Heal.bLastHeal = true;
    }

   if (CManager::GetInputKeyboard()->GetPress(DIK_M) == true || CManager::GetInputJoypad()->GetPress(CInputJoypad::JOYKEY_B) == true)
   {//魔法ボタンを押している間だけ回復カウントを進める
       m_Status.nMp--;  //MPを消費し続ける
       m_Heal.nHealCnt++; //回復に必要なMPを保存
       m_Heal.bState = true;//回復状態にする
       m_pMpGauge->SetParam(m_Status.nMp);

       //クイックヒールの演出
       QucikHealStaging();

       if (m_Heal.nHealCnt == 1)
       {
           CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_CHARGE_001);
       }

       if (m_Heal.nHealCnt % m_nQUICKHEALONEMP == 0)
       {
           m_Status.nHp++;     //体力を増やす
           CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_HEAL);
           m_pHpGauge->SetParam(m_Status.nHp);
           if (m_Heal.bLastHeal == true)
           {//今あるMPで回復できるのが最後なら
               CManager::GetSound()->StopSound(CSound::SOUND_LABEL_SE_CHARGE_001);
               m_Heal.bState = false;
               m_Heal.nHealCnt = 0;
               m_Heal.bLastHeal = false;

               //魔法使用状態のリセット
               ResetUseMagic();
           }
       }
   }
   else
   {//ボタンを話したら回復カウントをリセット
       m_Heal.bState = false;
       m_Heal.nHealCnt = 0;
       m_Heal.bLastHeal = false;
       CManager::GetSound()->StopSound(CSound::SOUND_LABEL_SE_CHARGE_001);
       //魔法使用状態のリセット
       ResetUseMagic();
   }
}
//===================================================================================================================================================

//========================================================
//クイックヒールの演出
//========================================================
void CPlayer::QucikHealStaging()
{
    D3DXVECTOR3 Pos = GetPos();
    D3DXVECTOR3 Size = GetSize();
    float fRandRot = float(rand() % 628) / 100;

    CEmitter* pEmitter = nullptr;

    if (m_Heal.nHealCnt % 3 == 0)
    {
       pEmitter = CEmitter::Create((int)(CEffect::EFFECTTYPE00_NORMAL), 60, 30, 0.0f, 10.0f, 10.0f,
            D3DXVECTOR3(Pos.x + sinf(fRandRot) * 30.0f, Pos.y, Pos.z + cosf(fRandRot) * 30.0f),
            D3DXVECTOR3(0.0f, 0.2f, 0.0f), NORMAL_COL);

       pEmitter->SetUseAddSpeed(true, true, 1.05f);
    }

    if (m_Heal.nHealCnt % m_nQUICKHEALONEMP == 0)
    {//回復完了パーティクルを召喚
        CParticle::SummonParticle(CParticle::TYPE04_HEAL, 50, 30, 40.0f, 40.0f, 100, 10, false, Pos + D3DXVECTOR3(0.0f, Size.y / 2, 0.0f), NORMAL_COL, true);
    }
}
//===================================================================================================================================================

//========================================================
//クロスボムの処理
//========================================================
void CPlayer::CrossBombProcess()
{
    CAttackPlayer* pAttackPlayer = nullptr;
    D3DXVECTOR3& Pos = GetPos();
    D3DXVECTOR3 Size = GetSize();

    pAttackPlayer = CAttackPlayer::Create(CAttack::ATTACKTYPE13_CROSSBOMB, DamageResult(m_nCROSSBOMB_POWER), 120, Pos + D3DXVECTOR3(0.0f, Size.y / 2, 0.0f), NULL_VECTOR3, ONE_VECTOR3 * 2, NULL_VECTOR3,
        CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, NULL_VECTOR3, true, 5);
    pAttackPlayer->SetDelayDeath(true);//死亡フラグを遅らせる
}
//===================================================================================================================================================

//========================================================
//ブラックホールの処理
//========================================================
void CPlayer::BlackHoleProcess()
{
    CAttackPlayer* pAttackPlayer = nullptr;
    D3DXVECTOR3& Pos = GetPos();
    D3DXVECTOR3 Size = GetSize();

    CAttackPlayer::Create(CAttack::ATTACKTYPE15_BLACKHOLE, 0, 300, Pos, NULL_VECTOR3, ONE_VECTOR3 * 3.0f, NULL_VECTOR3, CAttack::COLLISIONTYPE02_TRUERECTANGLEXY,
        ONE_VECTOR3 * 0.3f, false,0);

}
//===================================================================================================================================================

//========================================================
//サイコバーストの処理
//========================================================
void CPlayer::PhycoBurstProcess()
{
    float fMoveX = 0.0f;                                            //X方向の移動量
    float fMoveY = 0.0f;                                            //Y方向の移動量
    bool bMove = false;                                             //移動しているかどうか
    CAttackPlayer* pAttackPlayer = nullptr;
    if (m_pLockOnUi != nullptr)
    {
        if (CManager::GetInputKeyboard()->GetPress(DIK_M) == true || CManager::GetInputJoypad()->GetPress(CInputJoypad::JOYKEY_B) == true)
        {
            m_nStopMoveCnt = 2;//このカウントが０より上だと動けない

            if (CManager::GetInputKeyboard()->GetPress(DIK_W) == true || CManager::GetInputJoypad()->GetPress(CInputJoypad::JOYKEY_UP) == true)
            {
                fMoveY = 1.0f;
            }
            else if (CManager::GetInputKeyboard()->GetPress(DIK_S) == true || CManager::GetInputJoypad()->GetPress(CInputJoypad::JOYKEY_DOWN) == true)
            {
                fMoveY = -1.0f;
            }

            if (CManager::GetInputKeyboard()->GetPress(DIK_D) == true || CManager::GetInputJoypad()->GetPress(CInputJoypad::JOYKEY_RIGHT) == true)
            {
                fMoveX = 1.0f;
            }
            else if (CManager::GetInputKeyboard()->GetPress(DIK_A) == true || CManager::GetInputJoypad()->GetPress(CInputJoypad::JOYKEY_LEFT) == true)
            {
                fMoveX = -1.0f;
            }

            if (fMoveX != 0.0f || fMoveY != 0.0f)
            {
                bMove = true;//移動状態
            }
            else
            {
                bMove = false;//待機状態
            }

            if (bMove == true)
            {//ロックオンUIを動かす
                m_pLockOnUi->GetPos().x += sinf(atan2f(fMoveX, fMoveY)) * 10.0f;
                m_pLockOnUi->GetPos().y += cosf(atan2f(fMoveX, fMoveY)) * 10.0f;
            }
        }
        else
        {//魔法ボタンを離したら

            //ロックオンしている位置に爆発を起こす
            pAttackPlayer = CAttackPlayer::Create(CAttack::ATTACKTYPE16_PHYCOBURST, DamageResult(m_nPHYCOBURST_POWER), 45, m_pLockOnUi->GetPos(), NULL_VECTOR3, ONE_VECTOR3, NULL_VECTOR3,
                CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, ONE_VECTOR3 * 0.01f, false, 3);

            //ロックオンUIを消す
            m_pLockOnUi->SetUseDeath(true);
            m_pLockOnUi->SetDeath();
            m_pLockOnUi = nullptr;

            ResetUseMagic();//魔法使用状態のリセット
        }
    }

}
//===================================================================================================================================================

//========================================================
//魔法使用状態をリセットする
//========================================================
void CPlayer::ResetUseMagic()
{
    m_bPossibleMagic = true;//魔法使用状態かどうか
    m_nUseMagicNum = 0;     //現在使用している魔法
    m_nUseMagicTime = 0;    //魔法を使用している時間
    m_nNextUseMagic = 0;    //次の魔法が使えるまでの時間
}
//===================================================================================================================================================

//========================================================
//回避アクション
//========================================================
void CPlayer::DodgeAction(float fMoveX, float fMoveY)
{
    bool& bDodge = GetDodgeState();
    D3DXVECTOR3& Pos = GetPos();
    D3DXVECTOR3 RandMove = NULL_VECTOR3;
    D3DXVECTOR3& Move = GetMove();                                   //移動量を取得する
    CParticle* pParticle = nullptr;    //パーティクル取得用
    bool& bIsLanding = GetLanding();                                //地面にいるかどうか
    bool& bIsJumping = GetJumping();                                //ジャンプしているかどうか
    bool& bIsWalling = GetWalling();                                //壁にくっついているかどうか
    float fRandRot = 0.0f;
    float fRandSpeed = 0.0f;
    //=====================================
    //回避が仕様可能になるまでの処理
    //=====================================
    if (m_Dodge.bPossible == false)
    {
        m_Dodge.nRevivalCnt++;

        if (m_Dodge.nRevivalCnt >= m_nREVIVALDODGETIME/* && (bIsLanding == true || bIsWalling == true)*/)
        {
            RandMove.x = float(rand() % 150 - 75) / 5;
            RandMove.y = float(rand() % 150 - 75) / 5;
            RandMove.z = float(rand() % 150 - 75) / 5;
            pParticle = CParticle::Create(CParticle::TYPE00_NORMAL, 120, 60.0f, 60.0f, Pos, RandMove, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), true);
            pParticle->SetGravity(-0.2f);//重力を設定
            pParticle = nullptr;
            m_Dodge.bPossible = true;
            m_Dodge.nRevivalCnt = 0;
        }
    }
    //===================================================================================================================================================

    //BYTE rtValue = CManager::GetInputJoypad()->GetTrigger();
    //if (rtValue > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
    //======================================================
    //回避状態発動処理
    //======================================================
    if ((CManager::GetInputKeyboard()->GetTrigger(DIK_J) == true || CManager::GetInputJoypad()->GetRT_Trigger() == true) && m_Dodge.bPossible == true)
    {

        bDodge = true;
        for (int nCnt = 0; nCnt < 10; nCnt++)
        {
            fRandRot = float(rand() % 80 - 40) / 100;
            fRandSpeed = float(rand() % 100) / 10 + 3.0f;
            pParticle = CParticle::Create(CParticle::TYPE05_WING, 45, 60.0f, 60.0f,
                GetSenterPos(), D3DXVECTOR3(sinf(GetRot().y + fRandRot) * fRandSpeed, cosf(GetRot().y + fRandRot) * fRandSpeed, 0.0f), NORMAL_COL, true);
            pParticle->SetUseAddSpeed(false, true, 1.03f);
            if (GetRot().y >= D3DX_PI * 0.5f - 0.1f && GetRot().y <= D3DX_PI * 0.5f + 0.1f)
            {
                pParticle->SetUsePolygonRot(true, 0.1f);
            }
            else
            {
                pParticle->SetUsePolygonRot(true, -0.1f);
            }
        }
    }
    //===================================================================================================================================================

    //======================================================
    //回避状態中の処理
    //======================================================
    if (bDodge == true)
    {
       m_Dodge.nStateCnt++;

        if (bIsLanding == true)
        {//地面に着いていたら
            m_SaveMove.y = 0.0f;
        }

        if (m_Dodge.nStateCnt == 1)
        {//回避状態になった瞬間に発動
            CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_DODGE_000);
            if (fMoveX != 0.0f || fMoveY != 0.0f)
            {
                Move.x = sinf(atan2f(fMoveX, fMoveY) + CManager::GetCamera()->GetRot().y) * m_fSpeed;
                Move.y = cosf(atan2f(fMoveX, fMoveY) + CManager::GetCamera()->GetRot().y) * m_fSpeed;
            }
            else
            {
                Move = NULL_VECTOR3;
            }
            m_SaveMove = Move * 4.2f;//現在の移動量を保存し、２倍にする
            bIsJumping = false;
            Move = NULL_VECTOR3;     //元の移動量を０にする（ジャンプ中に回避した場合、回避終了時にジャンプした時の移動量が反映されて変に見えるため）
        }

        if (m_Dodge.nStateCnt % 2 == 0)
        {
            for (int nCnt = 0; nCnt < m_nMAX_PLAYERPARTS; nCnt++)
            {
                m_apModelParts[nCnt]->SetOriginalColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 2);
            }
        }

        if (m_Dodge.nStateCnt >= m_nDODGETIME)
        {
            m_SaveMove = NULL_VECTOR3;
            m_Dodge.nStateCnt = 0;
            bDodge = false;
            m_Dodge.bPossible = false;
        }
        m_bJudgeCommand[PLAYERCOMMAND02_DASH] = true;//ダッシュをしている
    }
    //===================================================================================================================================================

}
//===================================================================================================================================================

//==========================================================================
//プレイヤーのモデルパーツを比べてサイズを設定する
//==========================================================================
void CPlayer::SizeSet()
{
    D3DXVECTOR3 VtxMax = NULL_VECTOR3;                   //最大頂点代入用
    D3DXVECTOR3 VtxMin = NULL_VECTOR3;                   //最小頂点代入用
    D3DXVECTOR3& PlayerOriginVtxMax = GetOriginVtxMax(); //プレイヤーの元の最大頂点
    D3DXVECTOR3& PlayerOriginVtxMin = GetOriginVtxMin(); //プレイヤーの元の最小頂点
    D3DXVECTOR3& PlayerVtxMax = GetVtxMax();             //プレイヤーの最大頂点
    D3DXVECTOR3& PlayerVtxMin = GetVtxMin();             //プレイヤーの最小頂点
    D3DXVECTOR3& PlayerScale = GetScale();               //プレイヤーの拡大率
    D3DXVECTOR3 PlayerSize = NULL_VECTOR3;              //プレイヤーのサイズ
    for (int nCntParts = 0; nCntParts < m_nMAX_PLAYERPARTS; nCntParts++)
    {
        VtxMax = m_apModelParts[nCntParts]->GetVtxMax();
        VtxMin = m_apModelParts[nCntParts]->GetVtxMin();

        //====================================================
        //頂点座標を比較してモデルの最小値最大値を取得
        //====================================================
        if (VtxMax.x > PlayerOriginVtxMax.x)
        {
            PlayerOriginVtxMax.x = VtxMax.x;
        }
        if (VtxMax.y > PlayerOriginVtxMax.y)
        {
            PlayerOriginVtxMax.y = VtxMax.y;
        }
        if (VtxMax.z > PlayerOriginVtxMax.z)
        {
            PlayerOriginVtxMax.z = VtxMax.z;
        }
        if (VtxMin.x < PlayerOriginVtxMin.x)
        {
            PlayerOriginVtxMin.x = VtxMin.x;
        }
        if (VtxMin.y < PlayerOriginVtxMin.y)
        {
            PlayerOriginVtxMin.y = VtxMin.y;
        }
        if (VtxMin.z < PlayerOriginVtxMin.z)
        {
            PlayerOriginVtxMin.z = VtxMin.z;
        }
        //=============================================================================================================
    }

    PlayerOriginVtxMax.y += 10.0f;
    PlayerOriginVtxMin.y += 5.0f;

    //==========================================================
    //上の処理で出した最小最大の頂点を拡大率で計算し直す
    //==========================================================
    PlayerVtxMax.x = PlayerOriginVtxMax.x * PlayerScale.x;
    PlayerVtxMax.y = PlayerOriginVtxMax.y * PlayerScale.y;
    PlayerVtxMax.z = PlayerOriginVtxMax.z * PlayerScale.z;
    PlayerVtxMin.x = PlayerOriginVtxMin.x * PlayerScale.x;
    PlayerVtxMin.y = PlayerOriginVtxMin.y * PlayerScale.y;
    PlayerVtxMin.z = PlayerOriginVtxMin.z * PlayerScale.z;
    //================================================================================================================================================

    //==========================================================
    //プレイヤーのサイズ
    //==========================================================
    PlayerSize = PlayerVtxMax - PlayerVtxMin;
    SetSizeAnother(PlayerSize);//サイズを設定する
    //================================================================================================================================================
}
//===================================================================================================================================================

//============================================================
//軌跡の処理
//============================================================
void CPlayer::MeshOrbitProcess()
{
    D3DXVECTOR3 Pos = GetPos();//位置
    if (m_pMeshOrbit != nullptr)
    {
        m_pMeshOrbit->SetPos(Pos);
        m_pMeshOrbit->SetOffSetPos(m_apModelParts[10]->GetWorldPos());
    }
}
//===================================================================================================================================================

//============================================================
//モデルパーツのロード処理
//============================================================
void CPlayer::LoadModelParts()
{
    FILE* pFile = nullptr;

    pFile = fopen("data\\TEXTFILE\\PlayerSet.txt", "r");//ファイル読み込み

    int nNumModel = 0;             //モデルパーツの数
    int nType = 0;                 //モデルの種類
    int nParent = 0;               //親の番号
    int nCntModelFileName = 0;         //モデルのファイル名を読み込んだ回数をカウント
    char aString[128] = {};        //文字列代入用
    char aModelFileName[m_nMAX_PLAYERPARTS][128] = {};//モデルファイル名代入用
    D3DXVECTOR3 Rot = NULL_VECTOR3;//向き代入用
    D3DXVECTOR3 Pos = NULL_VECTOR3;//位置代入用
    if (pFile != NULL)
    {//ファイルが開かれていたら
        while (1)
        {
            fscanf(pFile, "%s", &aString[0]);

            if (strcmp(&aString[0], "ENDSCRIPT") == 0)
            {//ファイル読み込みを終わらせる
                fclose(pFile);
                break;
            }
            else if (aString[0] == '#')
            {//その行をスキップ
                fgets(&aString[0], 100, pFile);
            }
            else if (strcmp(&aString[0], "NUM_MODEL") == 0)
            {//モデルの種類数
                fscanf(pFile, "%d", &nNumModel);
            }
            else if (strcmp(&aString[0], "MODEL_FILENAME") == 0)
            {//ロードするモデルの数かパーツ数を超えなければ
                fscanf(pFile, "%s", &aString[0]);                           //イコール読み取り
                fscanf(pFile, "%s", &aModelFileName[nCntModelFileName][0]); //モデルファイルネーム読み取り
                nCntModelFileName++;
            }
            else if (strcmp(&aString[0], "MODELSET") == 0)
            {//モデル情報をセット
                while (1)
                {
                    fscanf(pFile, "%s", &aString[0]);
                    if (aString[0] == '#')
                    {//その行をスキップ
                        fgets(&aString[0], 100, pFile);
                    }
                    else if (strcmp(&aString[0], "TYPE") == 0)
                    {//モデルの種類を決める
                        fscanf(pFile, "%d", &nType);
                    }
                    else if (strcmp(&aString[0], "IDX") == 0)
                    {//モデルの親を決める
                        fscanf(pFile, "%d", &nParent);
                    }
                    else if (strcmp(&aString[0], "POS") == 0)
                    {//位置
                        fscanf(pFile, "%f%f%f", &Pos.x, &Pos.y, &Pos.z);
                    }
                    else if (strcmp(&aString[0], "ROT") == 0)
                    {//向き
                        fscanf(pFile, "%f%f%f", &Rot.x, &Rot.y, &Rot.z);
                    }
                    else if (strcmp(&aString[0], "ENDMODELSET") == 0)
                    {//パーツを設定
                        if (nParent >= 0)
                        {//親がいる
                            m_apModelParts[nType] = CModelParts::Create(&aModelFileName[nType][0], Pos, Rot);
                            m_apModelParts[nType]->SetParentParts(&m_apModelParts[nParent]->GetMatrixWorld());
                        }
                        else
                        {//親がいない
                            m_apModelParts[nType] = CModelParts::Create(&aModelFileName[nType][0], Pos, Rot);
                            m_apModelParts[nType]->SetParent(&this->GetMatrixWorld());
                        }
                        break;
                    }
                }
            }
        }
    }

}
//===================================================================================================================================================

//============================================================
//モーション処理
//============================================================
void CPlayer::MotionProcess()
{
    D3DXVECTOR3 NowPos = NULL_VECTOR3;//現在の位置代入用
    D3DXVECTOR3 NowRot = NULL_VECTOR3;//現在の向き代入用

    D3DXVECTOR3 NextPos = NULL_VECTOR3;//次の位置代入用
    D3DXVECTOR3 NextRot = NULL_VECTOR3;//次の向き代入用

    D3DXVECTOR3 DifferencePos = NULL_VECTOR3;//位置の差分
    D3DXVECTOR3 DifferenceRot = NULL_VECTOR3;//向きの差分

    D3DXVECTOR3 DecisionPos = NULL_VECTOR3;//決定する位置
    D3DXVECTOR3 DecisionRot = NULL_VECTOR3;//決定する向き

    m_nMotionOld = m_nNowMotion;
    m_nNowMotion = m_nNextMotion;
    if (m_nNowMotion != m_nMotionOld)
    {//モーションが1f前と異なる場合、フレーム数とキーカウントをリセット
        m_nCntFrame = 0;
        m_aMotion[m_nMotionOld].nCntKey = 0;
    }
    int nNowMotion = m_nNowMotion;              //現在のモーション
    int nNowKey = m_aMotion[nNowMotion].nCntKey;//現在のキー
    int nNextKey = (nNowKey + 1) % m_aMotion[nNowMotion].nNumKey;//次のキー（最大キーが３で、現在のキーが２だった場合）（現在のキー(2) + 1 ) % (最大キー(3)) ＝「０」)
    float fRatioFrame = (float)(m_nCntFrame) / (float)(m_aMotion[nNowMotion].aKeySet[nNowKey].nFrame);//現在のフレームと最大フレームの割合を求める
    for (int nCntParts = 0; nCntParts < m_nMAX_PLAYERPARTS; nCntParts++)
    {
        //現在の位置、向き
        NowPos = m_aMotion[nNowMotion].aKeySet[nNowKey].aKey[nCntParts].Pos;
        NowRot = m_aMotion[nNowMotion].aKeySet[nNowKey].aKey[nCntParts].Rot;

        //次の位置、向き
        NextPos = m_aMotion[nNowMotion].aKeySet[nNextKey].aKey[nCntParts].Pos;
        NextRot = m_aMotion[nNowMotion].aKeySet[nNextKey].aKey[nCntParts].Rot;

        //差分の位置、向き
        DifferencePos = NextPos - NowPos;
        DifferenceRot = NextRot - NowRot;

        //上記の処理を踏まえてパーツの位置、向きを設定
        DecisionPos = (DifferencePos * fRatioFrame) + m_apModelParts[nCntParts]->GetOffSetSupportPos() + NowPos;
        DecisionRot = DifferenceRot * fRatioFrame + NowRot;
        //設定する
        m_apModelParts[nCntParts]->SetOffSetPos(DecisionPos);
        m_apModelParts[nCntParts]->SetOffSetRot(DecisionRot);
    }

    m_nCntFrame++;

    if (m_nCntFrame >= m_aMotion[nNowMotion].aKeySet[nNowKey].nFrame)
    {//フレームが現在のキー情報の最大に達したら・・・
         m_aMotion[nNowMotion].nCntKey++;//キーを次に進める
         m_nCntFrame = 0;//フレームカウントを０にする

        if (m_aMotion[nNowMotion].nCntKey >= m_aMotion[nNowMotion].nNumKey)
        {// キーの最大数を超えたら・・・
            m_aMotion[nNowMotion].nCntKey = 0;

            if (m_aMotion[nNowMotion].bLoop == false)
            {//ループしないなら・・・
                for (int nCnt = 0; nCnt < m_nMAX_MOTION; nCnt++)
                {
                    m_bPriorityMotion[nCnt] = false;//全てのモーション優先状態を解除
                }
                m_nNextMotion = 0;
            }
        }
    }
}
//===================================================================================================================================================

//============================================================
//モーション情報をロードする
//============================================================
void CPlayer::LoadMotion()
{
    FILE* pFile;//ファイルポインタ

    //=========================================
    //モーション情報の読み込みで使う変数
    //=========================================
    char aString[100] = {};  //文字を代入する
    int nNumMotion = 0;      //モーションの数
    int nNumKeySet = 0;      //キーセットの数
    int nNumKey = 0;         //キーの数
    int nLoop = 0;           //ループするかどうかを数値で読み取る
    //====================================================================

    //ファイルを開く
    pFile = fopen("data\\TEXTFILE\\PlayerMotion2.txt", "r");

    if (pFile != NULL)
    {//ファイルが開かれていたら
        while (1)
        {
            fscanf(pFile, "%s", &aString[0]);

            if (strcmp(&aString[0], "END_SCRIPT") == 0)
            {//ファイル読み込みを終わらせる
                fclose(pFile);
                break;
            }
            else if (aString[0] == '#')
            {//その行をスキップ
                fgets(&aString[0], 100, pFile);
            }
            else if (strcmp(&aString[0], "MOTIONSET") == 0)
            {//ファイル読み込みを終わらせる
                while (1)
                {
                    fscanf(pFile, "%s", &aString[0]);

                    if (aString[0] == '#')
                    {//その行をスキップ
                        fgets(&aString[0], 100, pFile);
                    }
                    else if (strcmp(&aString[0], "LOOP") == 0)
                    {//ループするかどうか
                        fscanf(pFile, "%s", &aString[0]);//イコール
                        fscanf(pFile, "%d", &nLoop);

                        if (nLoop == 0)
                        {//ループしない
                            m_aMotion[nNumMotion].bLoop = false;
                        }
                        else
                        {//ループする
                            m_aMotion[nNumMotion].bLoop = true;
                        }
                    }
                    else if (strcmp(&aString[0], "NUM_KEY") == 0)
                    {//キーの数
                        fscanf(pFile, "%s", &aString[0]);//イコール
                        fscanf(pFile, "%d", &m_aMotion[nNumMotion].nNumKey);
                    }
                    else if (strcmp(&aString[0], "KEYSET") == 0)
                    {//キー情報設定
                        while (1)
                        {
                            fscanf(pFile, "%s", &aString[0]);

                            if (aString[0] == '#')
                            {//その行をスキップ
                                fgets(&aString[0], 100, pFile);
                            }
                            else if (strcmp(&aString[0], "FRAME") == 0)
                            {//フレーム数
                                fscanf(pFile, "%s", &aString[0]);//イコール
                                fscanf(pFile, "%d", &m_aMotion[nNumMotion].aKeySet[nNumKeySet].nFrame);
                            }
                            else if (strcmp(&aString[0], "KEY") == 0)
                            {//キーデータ設定
                                while (1)
                                {
                                    fscanf(pFile, "%s", &aString[0]);

                                    if (aString[0] == '#')
                                    {//その行をスキップ
                                        fgets(&aString[0], 100, pFile);
                                    }
                                    else if (strcmp(&aString[0], "POS") == 0)
                                    {//位置
                                        fscanf(pFile, "%s", &aString[0]);//イコール
                                        fscanf(pFile, "%f%f%f", &m_aMotion[nNumMotion].aKeySet[nNumKeySet].aKey[nNumKey].Pos.x,
                                            &m_aMotion[nNumMotion].aKeySet[nNumKeySet].aKey[nNumKey].Pos.y,
                                            &m_aMotion[nNumMotion].aKeySet[nNumKeySet].aKey[nNumKey].Pos.z);
                                    }
                                    else if (strcmp(&aString[0], "ROT") == 0)
                                    {//向き
                                        fscanf(pFile, "%s", &aString[0]);//イコール
                                        fscanf(pFile, "%f%f%f", &m_aMotion[nNumMotion].aKeySet[nNumKeySet].aKey[nNumKey].Rot.x,
                                            &m_aMotion[nNumMotion].aKeySet[nNumKeySet].aKey[nNumKey].Rot.y,
                                            &m_aMotion[nNumMotion].aKeySet[nNumKeySet].aKey[nNumKey].Rot.z);
                                    }
                                    else if (strcmp(&aString[0], "END_KEY") == 0)
                                    {//キー終了
                                        nNumKey++;
                                        break;
                                    }
                                }
                            }
                            else if (strcmp(&aString[0], "END_KEYSET") == 0)
                            {//キーセット終了
                                nNumKeySet++;
                                nNumKey = 0;
                                break;
                            }
                        }
                    }
                    else if (strcmp(&aString[0], "END_MOTIONSET") == 0)
                    {//モーションセット終了
                        nNumMotion++;
                        nNumKey = 0;
                        nNumKeySet = 0;
                        break;
                    }


                }
            }
        }
    }

}
//===================================================================================================================================================

//========================================================
//モーション優先度を設定する
//========================================================
void CPlayer::MotionPriorityProcess()
{
    if (m_bPriorityMotion[MOTIONTYPE00_NUTRAL] == true)
    {//強制的に移動状態にする
        m_nNextMotion = MOTIONTYPE00_NUTRAL;
    }
    if (m_bPriorityMotion[MOTIONTYPE01_RUNNING] == true)
    {//強制的に移動状態にする（優先度＋１）
        m_nNextMotion = MOTIONTYPE01_RUNNING;
    }
    if (m_bPriorityMotion[MOTIONTYPE03_JUMP] == true)
    {//強制的にジャンプ状態にする（優先度＋２）
        m_nNextMotion = MOTIONTYPE03_JUMP;
    }
    if (m_bPriorityMotion[MOTIONTYPE04_LANDING] == true)
    {//強制的に着地状態にする（優先度＋３）
        m_nNextMotion = MOTIONTYPE04_LANDING;
    }
    if (m_bPriorityMotion[MOTIONTYPE02_ATTACK] == true)
    {//強制的に攻撃状態にする（優先度＋４）
        m_nNextMotion = MOTIONTYPE02_ATTACK;
    }
}
//===================================================================================================================================================

//========================================================
//プレイヤーの情報を表示する
//========================================================
void CPlayer::DispPlayerInfo()
{
    D3DXVECTOR3 Pos = GetPos();//位置を取得
    D3DXVECTOR3 Rot = GetRot();//向きを取得
    CManager::GetDebugText()->PrintDebugText("プレイヤーの位置：%f %f %f\n", Pos.x, Pos.y, Pos.z);
    CManager::GetDebugText()->PrintDebugText("プレイヤーの向き：%f %f %f", Rot.x, Rot.y, Rot.z);
    CManager::GetDebugText()->PrintDebugText("プレイヤーのHP：%d\n",m_Status.nHp);
    CManager::GetDebugText()->PrintDebugText("プレイヤーのMP：%d\n",m_Status.nMp);
}
//===================================================================================================================================================

//========================================================
//新しい魔法を開放する処理
//========================================================
void CPlayer::SetUnlockMagic(int nMagicNum, D3DXVECTOR3 Pos, bool bUseSignBoard)
{
    if (m_nMagicNum < m_nMAX_MAGICNUM)
    {//魔法所持数が上限に達していなければ
        m_nMagicSlot[m_nMagicNum] = nMagicNum;
        m_bUnlockMagic[nMagicNum] = true;//魔法を開放状態にする


        if (m_nMagicNum == 0)
        {//初回習得時の処理
            m_pMagicWindow = CMagicWindow::Create(100.0f, 100.0f, D3DXVECTOR3(SENTER_VECTOR3.x, SENTER_VECTOR3.y + 300.0f, 0.0f));//魔法表示ウインドウを生成
            m_pMagicDisp = CMagicDisp::Create(CMagicDisp::MAGICDISPTYPE(nMagicNum), 100.0f, 100.0f, D3DXVECTOR3(SENTER_VECTOR3.x, SENTER_VECTOR3.y + 300.0f, 0.0f));//魔法表示を生成

            if (bUseSignBoard == true && CDifficulty::GetDifficultyNum() != CDifficulty::DIFFICULTYTYPE_BOSSRUSH)
            {
                CSignBoard::Create(CSignBoard::SIGNBOARDTYPE00_NORMAL, CSignBoard::SIGNBOARDMANUALTYPE09_MAGIC,
                    Pos, ONE_VECTOR3 * 7, NULL_VECTOR3);
            }
            m_pUsageMpGauge = CGauge::Create(CGauge::GAUGETYPE_PLAYERUSAGEMP, m_nINIT_MP, 290.0f, 12.0f, D3DXVECTOR3(141.0f, SCREEN_HEIGHT - 45.0f, 0.0f));//MPゲージを設定
        }

        if (m_nMagicNum == 1)
        {//２つ目の魔法習得時の処理
            if (bUseSignBoard == true && CDifficulty::GetDifficultyNum() != CDifficulty::DIFFICULTYTYPE_BOSSRUSH)
            {
                CSignBoard::Create(CSignBoard::SIGNBOARDTYPE00_NORMAL, CSignBoard::SIGNBOARDMANUALTYPE10_MAGICCHENGE,
                    Pos, ONE_VECTOR3 * 7, NULL_VECTOR3);
            }
        }

        //魔法所持数をインクリメント
        m_nMagicNum++;
    }
}
//===================================================================================================================================================

//========================================================
//プレイヤーのリスポーン処理を発動する
//========================================================
void CPlayer::ActivePlayerRespawn()
{
    D3DXVECTOR3& Pos = GetPos();
    D3DXVECTOR3& PosOld = GetPosOld();
    bool& bIsLanding = GetLanding();
    bool& bIsWalling = GetWalling();

    bIsLanding = false;
    bIsWalling = false;
    //位置をリスポーン位置に移動
    Pos = m_ReSpawnPos;
    PosOld = m_ReSpawnPos;

    //プレイヤーにダメージを与える
    SetDamage(1,60);
}
//===================================================================================================================================================

//========================================================
//滑らせるようの慣性を付ける
//========================================================
void CPlayer::SetPlayerSlideInertia(float fInertia)
{
    float& fGetInertia = GetInertia();

    m_fSlideInertia = fInertia;//慣性を調整する
    m_fReturnInertia = 0.5f - m_fSlideInertia;//元の慣性との差分を求める
    m_fCorrectionInertia = 1.0f;//慣性調整用
}
//===================================================================================================================================================

//========================================================
//回復量を割り当てる
//========================================================
void CPlayer::SetHeal(int nHeal, D3DXCOLOR col, float fWidth, float fHeight)
{

    m_Status.nHp+= nHeal;     //体力を増やす

    if (m_Status.nHp > m_Status.nMaxHp)
    {
        m_Status.nHp = m_Status.nMaxHp;
    }
    m_pHpGauge->SetParam(m_Status.nHp);

    CDamage::Create(nHeal, GetPos(), col, fWidth, fHeight);
}
//===================================================================================================================================================

//========================================================
//プレイヤーのリアクションを設定する
//========================================================
void CPlayer::SetPlayerReaction(PLAYERREACTION ReactionType, int nReactionTime)
{
    switch (ReactionType)
    {
    case PLAYERREACTION_NORMAL:
        m_pUiReaction->SetUiType(CUi::UITYPE_FACENORMAL);
        break;
    case PLAYERREACTION_DAMAGE:
        m_pUiReaction->SetUiType(CUi::UITYPE_FACEDAMAGE);
        break;
    case PLAYERREACTION_GLAD:
        m_pUiReaction->SetUiType(CUi::UITYPE_FACEGLAD);
        break;
    default:
        break;
    }

    m_nCntChengeUiReactionTime = nReactionTime;//リアクションを変える時間を設定する
    m_bChengeUiReactionFlag = true;            //リアクションを変えるフラグをONにする
}
//===================================================================================================================================================

//========================================================
//プレイヤーの慣性の処理
//========================================================
void CPlayer::PlayerInertiaProcess()
{
    float& fInertia = GetInertia();

    if (m_fCorrectionInertia > 0.01f)
    {//慣性の調整量が0.01fより上なら
        m_State.SlideState.bState = true;
    }
    else
    {//元の速さに戻りそうになったら
        m_State.SlideState.bState = false;
    }

    if (m_State.SlideState.bState == true)
    {//滑る状態だったら

        m_fCorrectionInertia += (0.0f - m_fCorrectionInertia) * 0.05f;//戻す量に掛ける倍率
        fInertia = m_fSlideInertia + m_fReturnInertia * (1.0f - m_fCorrectionInertia);//徐々に戻していく
    }
    else
    {//通常状態なら
        fInertia = 0.5f;
    }
}
//===================================================================================================================================================

//========================================================
//壁ジャンプを抑制する処理
//========================================================
void CPlayer::WallJumpBindProcess()
{
    if (m_nWallJumpBindTime > 0)
    {//壁ジャンプ束縛時間が０より大きければ
        m_nWallJumpBindTime--;
        m_bPossibleWallJump = false;
    }
    else
    {//それ以外
        m_bPossibleWallJump = true;
    }
}
//===================================================================================================================================================

//========================================================
//壁ジャンプのパーティクルを召喚する処理
//========================================================
void CPlayer::WallJumpParticleProcess()
{
    D3DXVECTOR3 ParticleMove = NULL_VECTOR3;
    float fRandRot = CCalculation::CalculationRandVecXY();

    ParticleMove.x = sinf(fRandRot) * 4.0f;
    ParticleMove.y = 5.0f;
    ParticleMove.z = cosf(fRandRot) * 4.0f;
    D3DXVECTOR3 Pos = GetPos();                                      //位置を取得する
    D3DXVECTOR3 Rot = GetRot();                                      //向きを取得する

    CParticle::Create(CParticle::TYPE00_NORMAL, 30, 20.0f, 20.0f,
        D3DXVECTOR3(Pos.x + cosf(Rot.y) * (GetSize().x * 0.5f), Pos.y, Pos.z + sinf(Rot.y) * (GetSize().x * 0.5f)),
        ParticleMove, NORMAL_COL, true);

}
//===================================================================================================================================================

//========================================================
//敵との当たり判定を行う
//========================================================
void CPlayer::CollisionEnemy()
{
    //==================================
    //オブジェクトXから情報を取得
    //==================================
    D3DXVECTOR3 Pos = GetPos();                      //オブジェクトの位置を取得
    D3DXVECTOR3 VtxMax = GetVtxMax();                //オブジェクトの最大頂点を取得
    D3DXVECTOR3 VtxMin = GetVtxMin();                //オブジェクトの最小頂点を取得
    D3DXVECTOR3 ComparisonPos = NULL_VECTOR3;                  //プレイヤーの位置代入用
    D3DXVECTOR3 ComparisonVtxMax = NULL_VECTOR3;               //判定対象の最大頂点
    D3DXVECTOR3 ComparisonVtxMin = NULL_VECTOR3;               //判定対象の最小頂点
    CObject* pObj = nullptr;                                   //オブジェクト取得用
    CObjectX* pObjX = nullptr;                                 //オブジェクトX
    bool bCollision = false;                                   //当たり判定
    //===========================================================

    //==================================
    //敵の当たり判定
    //==================================
    for (int nCntPriority = 0; nCntPriority < m_nMAXPRIORITY; nCntPriority++)
    {

        pObj = GetTopObject(nCntPriority);//トップオブジェクトを取得
        while (pObj != nullptr)
        {
            //次のオブジェクトを格納
            CObject* pNext = pObj->GetNextObject();

            //種類の取得（敵なら当たり判定）
            CObject::TYPE type = pObj->GetType();

            if (type == CObject::TYPE_ENEMY || type == CObject::TYPE_BOSS)
            {
                pObjX = (CObjectX*)pObj;

                //オブジェクトの種類が敵だったら
                ComparisonPos = pObjX->GetPos();                   //位置を取得
                ComparisonVtxMax = pObjX->GetVtxMax();             //最大頂点を取得
                ComparisonVtxMin = pObjX->GetVtxMin();             //最小頂点を取得

                bCollision = CCollision::CollisionSquare(Pos, VtxMax, VtxMin, ComparisonPos, ComparisonVtxMax, ComparisonVtxMin);

                if (bCollision == true)
                {//アイテムに当たったら
                    if (CScene::GetMode() == CScene::MODE_GAME)
                    {
                        SetDamage(1, 10);//プレイヤーにダメージを与える
                    }
                }
            }

            //リストを次に進める
            pObj = pNext;
        }
    }

}
//===================================================================================================================================================

//========================================================
//アイテムとの当たり判定
//========================================================
void CPlayer::CollisionItem()
{
    //==================================
    //オブジェクトXから情報を取得
    //==================================
    D3DXVECTOR3 Pos = GetPos();                      //オブジェクトの位置を取得
    D3DXVECTOR3 Size = GetSize();                    //オブジェクトのサイズを取得
    D3DXVECTOR3 VtxMax = GetVtxMax();                //オブジェクトの最大頂点を取得
    D3DXVECTOR3 VtxMin = GetVtxMin();                //オブジェクトの最小頂点を取得
    D3DXVECTOR3 ComparisonPos = NULL_VECTOR3;                  //プレイヤーの位置代入用
    D3DXVECTOR3 ComparisonVtxMax = NULL_VECTOR3;               //判定対象の最大頂点
    D3DXVECTOR3 ComparisonVtxMin = NULL_VECTOR3;               //判定対象の最小頂点
    CPlayer* pPlayer = nullptr;                                //プレイヤークラス取得用
    CObject* pObj = nullptr;                                   //オブジェクト取得用
    bool bCollision = false;                                   //当たったかどうか
    CItem::ITEMTYPE ItemType = {};                             //アイテムの種類
    CItem* pItem = nullptr;                                    //アイテムへのポインタ
    //===========================================================

    //==================================
    //アイテムの当たり判定
    //==================================
    for (int nCntPriority = 0; nCntPriority < m_nMAXPRIORITY; nCntPriority++)
    {

        pObj = GetTopObject(nCntPriority);//トップオブジェクトを取得
        while (pObj != nullptr)
        {
            //次のオブジェクトを格納
            CObject* pNext = pObj->GetNextObject();

            //種類の取得（敵なら当たり判定）
            CObject::TYPE type = pObj->GetType();

            //オブジェクトの種類が敵だったら
            switch (type)
            {
            case CObject::TYPE_ITEM:
                pItem = (CItem*)pObj;                            //オブジェクトを敵の型にダウンキャスト
                ComparisonPos = pItem->GetPos();                   //位置を取得
                ComparisonVtxMax = pItem->GetVtxMax();             //最大頂点を取得
                ComparisonVtxMin = pItem->GetVtxMin();             //最小頂点を取得
                ItemType = pItem->GetItemType();                   //アイテムの種類を取得する
                bCollision = CCollision::CollisionSquare(GetPos(),GetVtxMax(),GetVtxMin(), ComparisonPos, ComparisonVtxMax, ComparisonVtxMin);
                if (bCollision == true)
                {//アイテムに当たったら
                    switch (ItemType)
                    {
                    case CItem::ITEMTYPE00_HEALHEART:
                        SetHeal(5, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 30.0f, 30.0f);
                        CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_HEAL);
                        break;
                    case CItem::ITEMTYPE01_COIN:
                        GetScore()->AddScore(100);
                        CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_COIN_000);
                        break;
                    case CItem::ITEMTYPE03_CROWN:
                        CManager::GetSceneFade()->SetSceneFade(CFade::FADEMODE_IN, CScene::MODE_RESULT);
                        CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_GETCLOWN_000);
                        break;
                    case CItem::ITEMTYPE04_DOUBLEJUMPSHOES:
                        SetUnlockAction(CPlayer::ACTIONTYPE00_DOUBLEJUMP);
                        CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_GETACTION_000);
                        break;
                    case CItem::ITEMTYPE05_DODGECRYSTAL:
                        SetUnlockAction(CPlayer::ACTIONTYPE01_DODGE);
                        CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_GETACTION_000);
                        break;
                    case CItem::ITEMTYPE06_WALLJUMPCROW:
                        SetUnlockAction(CPlayer::ACTIONTYPE03_WALLJUMP);
                        CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_GETACTION_000);
                        break;
                    case CItem::ITEMTYPE07_FIREBALL_ENERGY:
                        SetUnlockMagic(int(CMagicDisp::MAGICDISP00_FIRE), ComparisonPos,true);
                        CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_GETMAGIC_000);
                        break;
                    case CItem::ITEMTYPE08_BEAMWHIP_ENERGY:
                        SetUnlockMagic(int(CMagicDisp::MAGICDISP01_BEAM), ComparisonPos, true);
                        CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_GETMAGIC_000);
                        break;
                    case CItem::ITEMTYPE09_ICE_ENERGY:
                        SetUnlockMagic(int(CMagicDisp::MAGICDISP02_ICE), ComparisonPos, true);
                        CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_GETMAGIC_000);
                        break;
                    case CItem::ITEMTYPE10_THUNDER_ENERGY:
                        SetUnlockMagic(int(CMagicDisp::MAGICDISP03_THUNDER), ComparisonPos, true);
                        CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_GETMAGIC_000);
                        break;
                    case CItem::ITEMTYPE11_MAGICSWORD_ENERGY:
                        SetUnlockMagic(int(CMagicDisp::MAGICDISP04_SWORD), ComparisonPos, true);
                        CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_GETMAGIC_000);
                        break;
                    case CItem::ITEMTYPE12_PLANTSHIELD_ENERGY:
                        SetUnlockMagic(int(CMagicDisp::MAGICDISP05_PLANTSHIELD), ComparisonPos, true);
                        CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_GETMAGIC_000);
                        break;
                    case CItem::ITEMTYPE13_QUICKHEAL_ENERGY:
                        SetUnlockMagic(int(CMagicDisp::MAGICDISP06_QUICKHEAL), ComparisonPos, true);
                        CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_GETMAGIC_000);
                        break;
                    case CItem::ITEMTYPE14_CROSSBOMB_ENERGY:
                        SetUnlockMagic(int(CMagicDisp::MAGICDISP07_CROSSBOMB), ComparisonPos, true);
                        CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_GETMAGIC_000);
                        break;
                    case CItem::ITEMTYPE15_BLACKHOLE_ENERGY:
                        SetUnlockMagic(int(CMagicDisp::MAGICDISP08_BLACKHOLE), ComparisonPos, true);
                        CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_GETMAGIC_000);
                        break;
                    case CItem::ITEMTYPE16_PHYCOBURST_ENERGY:
                        SetUnlockMagic(int(CMagicDisp::MAGICDISP09_PHYCOBURST), ComparisonPos, true);
                        CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_GETMAGIC_000);
                        break;
                    case CItem::ITEMTYPE17_CHARGESHOT_GUN:
                        SetUnlockBulletAction(CPlayer::BULLETACTION00_CHARGE);
                        CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_GETSHOT_000);
                        break;
                    case CItem::ITEMTYPE18_RAINSHOT_GUN:
                        SetUnlockBulletAction(CPlayer::BULLETACTION01_RAINSHOT);
                        CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_GETSHOT_000);
                        break;
                    case CItem::ITEMTYPE19_BURSTSHOT_GUN:
                        SetUnlockBulletAction(CPlayer::BULLETACTION02_BURSTSHOT);
                        CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_GETSHOT_000);
                        break;
                    case CItem::ITEMTYPE20_RAPIDSHOT_GUN:
                        SetUnlockBulletAction(CPlayer::BULLETACTION03_RAPIDSHOT);
                        CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_GETSHOT_000);
                        break;
                    default:
                        break;
                    }

                    SetPlayerReaction(CPlayer::PLAYERREACTION_GLAD, 60);
                    pItem->SetUseDeath(true);
                    pItem->SetDeath();
                }
                break;
            default:
                //リストを次に進める
                pObj = pNext;
                continue;
                break;
            }
            //リストを次に進める
            pObj = pNext;
        }
    }

}
//===================================================================================================================================================

//========================================================
//トラップとの当たり判定
//========================================================
void CPlayer::CollisionTrap()
{
    //==================================
   //オブジェクトXから情報を取得
   //==================================
    D3DXVECTOR3 Pos = CObjectX::GetPos();                      //オブジェクトの位置を取得
    D3DXVECTOR3 Size = CObjectX::GetSize();                    //オブジェクトのサイズを取得
    D3DXVECTOR3 VtxMax = GetVtxMax();                          //オブジェクトの最大頂点を取得
    D3DXVECTOR3 VtxMin = GetVtxMin();                          //オブジェクトの最小頂点を取得

    D3DXVECTOR3 ComparisonPos = NULL_VECTOR3;                  //プレイヤーの位置代入用
    D3DXVECTOR3 ComparisonVtxMax = NULL_VECTOR3;               //判定対象の最大頂点
    D3DXVECTOR3 ComparisonVtxMin = NULL_VECTOR3;               //判定対象の最小頂点
    CObject* pObj = nullptr;                                   //オブジェクト取得用
    CTrap* pTrap = nullptr;                                    //トラップへのポインタ
    CTrap::TRAPTYPE TrapType;                                  //トラップの種類
    bool bCollision = false;                                   //当たり判定
    //===========================================================

    //==================================
    //トラップの当たり判定
    //==================================
    for (int nCntPriority = 0; nCntPriority < m_nMAXPRIORITY; nCntPriority++)
    {

        pObj = GetTopObject(nCntPriority);//トップオブジェクトを取得
        while (pObj != nullptr)
        {
            //次のオブジェクトを格納
            CObject* pNext = pObj->GetNextObject();

            //種類の取得（敵なら当たり判定）
            CObject::TYPE type = pObj->GetType();

            if (type == CObject::TYPE_TRAP)
            {
                //オブジェクトの種類が敵だったら
                pTrap = (CTrap*)pObj;                              //オブジェクトを敵の型にダウンキャスト
                TrapType = pTrap->GetTrapType();                   //トラップの種類
                ComparisonPos = pTrap->GetPos();                   //位置を取得
                ComparisonVtxMax = pTrap->GetVtxMax();             //最大頂点を取得
                ComparisonVtxMin = pTrap->GetVtxMin();             //最小頂点を取得[

                switch (pTrap->GetCollisionType())
                {
                case CTrap::COLLISIONTYPE00_SQUARE:
                    bCollision = CCollision::CollisionSquare(Pos, VtxMax, VtxMin, ComparisonPos, ComparisonVtxMax, ComparisonVtxMin);
                    break;
                case CTrap::COLLISIONTYPE01_RECTANGLESQUARE:
                    bCollision = CCollision::RectAngleCollisionXY(pTrap->GetPos(), pTrap->GetVtxMax(), pTrap->GetVtxMin(), pTrap->GetRot(), Pos, VtxMax, VtxMin, NULL_VECTOR3);
                    break;
                default:
                    break;
                }
                if (bCollision == true)
                {//トラップに当たったら
                    switch (TrapType)
                    {
                    case CTrap::TRAPTYPE00_NEEDLEBALL:
                        SetDamage(1, 10);
                        break;
                    case CTrap::TRAPTYPE02_THORNCLUB:
                        SetDamage(1, 10);
                        break;
                    default:
                        break;
                    }
                }
            }
            //リストを次に進める
            pObj = pNext;
        }
    }

}
//===================================================================================================================================================

//========================================================
//無敵時間の処理
//========================================================
void CPlayer::InvincibleProcess()
{
    if (m_bInvincible == true)
    {
        m_nCntInvincible++;

        if (m_nCntInvincible % 2 == 0)
        {
            for (int nCnt = 0; nCnt < m_nMAX_PLAYERPARTS; nCnt++)
            {
                m_apModelParts[nCnt]->SetOriginalColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), 2);
            }
        }
        else
        {
            for (int nCnt = 0; nCnt < m_nMAX_PLAYERPARTS; nCnt++)
            {
                m_apModelParts[nCnt]->SetColor(COLORTYPE_RED, 2);
            }
        }

        if (m_nCntInvincible >= m_nMAX_INCINCIBLETIME)
        {
            m_bInvincible = false;
            m_nCntInvincible = 0;
        }
    }
}
//===================================================================================================================================================

//========================================================
//チャージ攻撃
//========================================================
void CPlayer::ChargeAttack()
{
    D3DXVECTOR3& Pos = GetPos();
    D3DXVECTOR3 SenterPos = GetSenterPos();
    bool& bDodge = GetDodgeState();
    D3DXVECTOR3& Rot = GetRot();
    CBullet* pBullet = nullptr;//バレットへのポインタ
    CParticle* pParticle = nullptr;    //パーティクル取得用
    bool bUnlockChargeAttack = CheckUnlockBulletAction();  //チャージ攻撃が開放されているかどうかを判定
    //ランダムな色合い

    if (CManager::GetInputKeyboard()->GetTrigger(DIK_N) == true || CManager::GetInputJoypad()->GetTrigger(CInputJoypad::JOYKEY_X) == true)
    {//バレット発射
        m_bPriorityMotion[MOTIONTYPE02_ATTACK] = true;
        pBullet = CBullet::Create(CBullet::BULLETTYPE_PLAYER, CBullet::BULLETTEX_PLAYER, 45,DamageResult(m_nNORMALSHOT_POWER), 40.0f, 40.0f, SenterPos, D3DXVECTOR3(sinf(Rot.y - D3DX_PI) * 10.0f, 0.0f, cosf(Rot.y - D3DX_PI) * 10.0f));
        pBullet->SetHitAddMp(1);//１ヒットにつきMP＋１
        pBullet->SetUseReflection(true);

        m_ChargeAttack.bState = true;//通常攻撃をした瞬間に
        m_bJudgeCommand[PLAYERCOMMAND04_SHOT] = true;//ショットを発射
        CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_SHOT_000);
    }

    if (CManager::GetInputKeyboard()->GetTrigger(DIK_I) == true)
    {//キルバレット発射
        m_bPriorityMotion[MOTIONTYPE02_ATTACK] = true;
        pBullet = CBullet::Create(CBullet::BULLETTYPE_PLAYER, CBullet::BULLETTEX_PLAYER, 45, DamageResult(9999), 40.0f, 40.0f, SenterPos, D3DXVECTOR3(sinf(Rot.y - D3DX_PI) * 10.0f, 0.0f, cosf(Rot.y - D3DX_PI) * 10.0f));
        pBullet->SetHitAddMp(9999);//デバッグ用なのでカンストさせる
    }

    if (m_bInvincible == false && bUnlockChargeAttack == true)
    {//無敵状態じゃなければチャージ攻撃可能

        if ((CManager::GetInputKeyboard()->GetPress(DIK_N) == true || CManager::GetInputJoypad()->GetPress(CInputJoypad::JOYKEY_X) == true) && m_ChargeAttack.bState == true)
        {
            m_ChargeAttack.nChargeCnt++;

            if (m_ChargeAttack.nChargeCnt >= m_nMAX_CHARGETIME && m_ChargeAttack.nChargeLevel < m_nMAX_CHARGEATTACKLEVEL)
            {
                m_ChargeAttack.nChargeLevel++;
                m_ChargeAttack.nChargeCnt = 0;
            }

            if (m_ChargeAttack.nChargeLevel >= 1 && m_ChargeAttack.nChargeLevel < m_nMAX_CHARGEATTACKLEVEL)
            {//チャージが開始されたら
                if (m_ChargeAttack.nChargeLevel == 1 && m_ChargeAttack.nChargeCnt == 1)
                {
                    CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_CHARGE_000);
                }
                ChargeAttackParticle();
            }

            //=============================
            //チャージ最大表示処理
            //=============================
            if (m_ChargeAttack.nChargeLevel == m_nMAX_CHARGEATTACKLEVEL)
            {
                MaxChargeProcess();
            }
            //===============================================================================================
        }
        else
        {
            if (m_ChargeAttack.nChargeLevel > 0)
            {
                if ((CManager::GetInputKeyboard()->GetPress(DIK_W) == true || CManager::GetInputJoypad()->GetPress(CInputJoypad::JOYKEY_UP) == true) && m_bUnlockBulletAction[BULLETACTION01_RAINSHOT] == true)
                {//上長押し
                    RainShotProcess();
                    CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_SHOT_RAIN);
                    m_bJudgeCommand[PLAYERCOMMAND06_RAINSHOT] = true;//レインショットを発射
                }
                else if ((CManager::GetInputKeyboard()->GetPress(DIK_S) == true || CManager::GetInputJoypad()->GetPress(CInputJoypad::JOYKEY_DOWN) == true) && m_bUnlockBulletAction[BULLETACTION02_BURSTSHOT] == true)
                {//下長押し
                    BurstShotProcess();
                    CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_SHOT_BURST);
                    m_bJudgeCommand[PLAYERCOMMAND07_BURSTSHOT] = true;//バーストショットを発射
                }
                else if (bDodge == true && m_bUnlockBulletAction[BULLETACTION03_RAPIDSHOT] == true)
                {//回避しながら
                    RapidShotProcess();
                    CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_SHOT_ROGKET);
                    m_bJudgeCommand[PLAYERCOMMAND08_RAPIDSHOT] = true;//ラピッドショットを発射
                }
                else
                {//どのコマンドも押していない
                    if (m_bUnlockBulletAction[BULLETACTION00_CHARGE] == true)
                    {
                        ChargeShotProcess();
                        CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_SHOT_001);
                        m_bJudgeCommand[PLAYERCOMMAND05_CHARGESHOT] = true;//チャージショットを発射
                    }
                }
            }

            m_ChargeAttack.bState = false;
            m_ChargeAttack.nChargeCnt = 0;
            m_ChargeAttack.nChargeLevel = 0;

            CManager::GetSound()->StopSound(CSound::SOUND_LABEL_SE_CHARGE_000);
        }
    }
}
//===========================================================================================================================================================

//=================================================================================
//チャージ攻撃のパーティクル召喚処理
//=================================================================================
void CPlayer::ChargeAttackParticle()
{
    D3DXCOLOR RandCol = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
    D3DXVECTOR3& Pos = GetPos();
    D3DXVECTOR3& Rot = GetRot();

    switch (m_ChargeAttack.nChargeLevel)
    {
    case 1:
        CParticle::SummonChargeParticle(CParticle::TYPE00_NORMAL, 3, 180,5.0f, 40.0f, 40.0f, 300, 100, 10, false, Pos, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), true);
        break;
    case 2:
        CParticle::SummonChargeParticle(CParticle::TYPE00_NORMAL, 3, 180, 5.0f, 40.0f, 40.0f, 300, 100, 10, false, Pos, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), true);
        break;
    case 3:
        CParticle::SummonChargeParticle(CParticle::TYPE00_NORMAL, 3, 180, 5.0f, 40.0f, 40.0f, 300, 100, 10, false, Pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), true);
        break;
    case 4:
        RandCol.r = float(rand() % 100 + 1) / 100;
        RandCol.g = float(rand() % 100 + 1) / 100;
        RandCol.b = float(rand() % 100 + 1) / 100;
        CParticle::SummonChargeParticle(CParticle::TYPE00_NORMAL, 3, 180, 5.0f, 40.0f, 40.0f, 300, 100, 10, false, Pos, RandCol, true);
        break;
    default:
        break;
    }
}
//===========================================================================================================================================================

//=================================================================================
//チャージ攻撃のパーティクル召喚処理
//=================================================================================
void CPlayer::MaxChargeProcess()
{
    D3DXCOLOR RandCol = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

    if (m_Dodge.nStateCnt % 2 == 0)
    {
        RandCol.r = float(rand() % 100 + 1) / 100;
        RandCol.g = float(rand() % 100 + 1) / 100;
        RandCol.b = float(rand() % 100 + 1) / 100;
        for (int nCnt = 0; nCnt < m_nMAX_PLAYERPARTS; nCnt++)
        {//虹色に点滅
            m_apModelParts[nCnt]->SetOriginalColor(RandCol, 2);
            SetOriginalColor(RandCol, 2);
        }
    }
}
//===========================================================================================================================================================

//============================================================
//チャージショットの処理
//============================================================
void CPlayer::ChargeShotProcess()
{
    D3DXVECTOR3 Pos = GetPos();
    D3DXVECTOR3 Rot = GetRot();
    CBullet* pBullet = nullptr;//バレットへのポインタ
    m_bPriorityMotion[MOTIONTYPE02_ATTACK] = true;
    pBullet = CBullet::Create(CBullet::BULLETTYPE_PLAYER, CBullet::BULLETTEX_PLAYER, 300,DamageResult(m_nCHARGEATTACKPOWER[m_ChargeAttack.nChargeLevel - 1]), 40.0f * m_ChargeAttack.nChargeLevel, 40.0f * m_ChargeAttack.nChargeLevel, Pos, D3DXVECTOR3(sinf(Rot.y - D3DX_PI) * 10.0f, 0.0f, cosf(Rot.y - D3DX_PI) * 10.0f));
    pBullet->SetHitAddMp(4 * (1 + m_ChargeAttack.nChargeLevel));//チャージレベル×４分ヒット時にMPを増やす
}
//===================================================================================================================================================

//============================================================
//チャージショットの処理
//============================================================
void CPlayer::RainShotProcess()
{
    D3DXVECTOR3 Pos = GetPos();
    D3DXVECTOR3 SenterPos = GetSenterPos();
    D3DXVECTOR3 Rot = GetRot();
    float fRandRot = 0.0f;
    float fRandSpeed = 0.0f;
    float fRandGravity = 0.0f;
    CBullet* pBullet = nullptr;
    for (int nCnt = 0; nCnt < m_nRAINSHOTBULLET * m_ChargeAttack.nChargeLevel; nCnt++)
    {
        fRandRot = float(rand() % 400 - 200) / 1000;
        fRandSpeed = float(rand() % 100 + 50) / 10 + 1.0f;
        fRandGravity = float(rand() % 200) / 1000;
        pBullet = CBullet::Create(CBullet::BULLETTYPE_PLAYER, CBullet::BULLETTEX_RAIN, 100, DamageResult(m_nRAINSHOT_POWER), 40.0f, 40.0f, SenterPos,
            D3DXVECTOR3(sinf(fRandRot) * fRandSpeed, cosf(fRandRot) * fRandSpeed, 0.0f));
        pBullet->SetAddGravity(-0.02f);
        pBullet->SetUseGravity(-0.1f - fRandGravity);
        pBullet->SetHitAddMp(1);//ヒット時に増やすMPを設定
        //pBullet->SetUseEffect(15, D3DXVECTOR2(40.0f, 40.0f), D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
    }

}
//===================================================================================================================================================

//=================================================================================
//バーストショットの処理
//=================================================================================
void CPlayer::BurstShotProcess()
{
    D3DXVECTOR3 Pos = GetPos();
    D3DXVECTOR3 SenterPos = GetSenterPos();
    D3DXVECTOR3 Rot = GetRot();
    float fRandSpeed = 0.0f;
    float fRandGravity = 0.0f;
    float fRot = 0.0f;
    CBullet* pBullet = nullptr;

    for (int nCntCharge = 0; nCntCharge < m_ChargeAttack.nChargeLevel; nCntCharge++)
    {
        for (int nCntBullet = 0; nCntBullet < m_nBURSTSHOTBULLET; nCntBullet++)
        {
            fRot = (2.0f / m_nBURSTSHOTBULLET) * nCntBullet;
            pBullet = CBullet::Create(CBullet::BULLETTYPE_PLAYER, CBullet::BULLETTEX_PLAYER, 180, DamageResult(m_nBURSTSHOT_POWER), 50.0f, 50.0f, SenterPos,NULL_VECTOR3);
            //pBullet->SetUseEffect(10, D3DXVECTOR2(50.0f, 50.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
            pBullet->SetHitAddMp(1);//ヒット時に増やすMPを設定
            if (nCntCharge % 2 == 0)
            {
                pBullet->SetUseCurve(D3DX_PI * fRot, 3.0f + 1.5f * nCntCharge, -0.05f);
                pBullet->SetUseReflection(true);
            }
            else
            {
                pBullet->SetUseCurve(D3DX_PI * fRot, 3.0f + 1.5f * nCntCharge, 0.05f);
                pBullet->SetUseReflection(true);
            }
        }
    }
}
//===================================================================================================================================================

//=================================================================================
//ラピッドショットの処理
//=================================================================================
void CPlayer::RapidShotProcess()
{
    D3DXVECTOR3 SenterPos = GetSenterPos();
    D3DXVECTOR3 Pos = GetPos();
    D3DXVECTOR3 Rot = GetRot();
    float fRandSpeed = 0.0f;
    float fRandGravity = 0.0f;
    float fRot = 0.0f;
    CBullet* pBullet = nullptr;

    for (int nCnt = 0; nCnt < m_ChargeAttack.nChargeLevel; nCnt++)
    {
        pBullet = CBullet::Create(CBullet::BULLETTYPE_PLAYER, CBullet::BULLETTEX_PLAYER, 45, DamageResult(m_nRAPIDSHOT_POWER), 30.0f, 30.0f, SenterPos,
            D3DXVECTOR3(sinf(-Rot.y) * (10.0f + 1.0f * nCnt), cosf(-Rot.y) * (10.0f + 1.0f * nCnt), 0.0f));
        pBullet->SetUseDivision(true, 3,10);
        pBullet->SetHitAddMp(4);
    }

}
//===================================================================================================================================================

//=================================================================================
//バレットアクションを開放しているかどうかのチェックを行う
//=================================================================================
bool CPlayer::CheckUnlockBulletAction()
{
    bool bUnlock = false;

    for (int nCnt = 0; nCnt < BULLETACTION_MAX; nCnt++)
    {
        if (m_bUnlockBulletAction[nCnt] == true)
        {//どれか一つでも開放していたら
            bUnlock = true;
        }
    }

    return bUnlock;
}
//===================================================================================================================================================

//=================================================================================
//重力の処理
//=================================================================================
void CPlayer::GravityProcess()
{
    D3DXVECTOR3& pos = GetPos();                                   //モデルの位置の取得
    D3DXVECTOR3& Move = GetMove();                                   //移動量を取得する
    D3DXVECTOR3 Size = GetSize();                                   //サイズを取得
    float fMoveX = 0.0f;                                            //X方向の移動量
    float& fInertia = GetInertia();                                 //慣性を取得
    float fMoveZ = 0.0f;                                            //Z方向の移動量
    bool& bIsLanding = GetLanding();                                //地面にいるかどうか
    bool& bDodge = GetDodgeState();
    bool& bIsWalling = GetWalling();                                //壁にくっついているかどうか　
    bool& bIsJumping = GetJumping();                                //ジャンプしているかどうか
    //if (pos.y <= 0.0f && CManager::GetCamera()->GetCameraType() == CCamera::CAMERATYPE_BOSSBATTLE)
    //{
    //    Move.y = 0.0f;
    //    bIsLanding = true;
    //    pos.y = 0.0f;
    //}

    if (bIsLanding == true)
    {
        Move.y = 0.0f;
        m_nCntLanding++;
        if (m_nCntLanding == 1)
        {
            m_bPriorityMotion[MOTIONTYPE04_LANDING] = true;
        }
        bIsJumping = false;//ジャンプ中かどうか
        m_nJumpCnt = 0;      //ジャンプした回数
    }
    else
    {
        m_nCntLanding = 0;
        m_bPriorityMotion[MOTIONTYPE04_LANDING] = false;
    }

    if (bDodge == false)
    {//回避状態の時は重力を無効にする
        if (m_bUnlockAction[ACTIONTYPE03_WALLJUMP] == true)
        {//壁ジャンプが開放されていたら
            if (bIsWalling == false)
            {//壁にいなかったら
                Move.y += -1.0f;
            }
            else
            {//壁にいたら
                if (m_bPossibleWallJump == true)
                {
                    Move.y = -2.0f;
                }
                else
                {
                    Move.y += -1.0f;
                }
            }
        }
        else
        {//壁ジャンプが開放されていなかったら
            Move.y += -1.0f;
            if (Move.y <= m_fMAX_GRAVITYPOWER)
            {
                Move.y = m_fMAX_GRAVITYPOWER;
            }
        }
    }
    //===============================================================================================================================

}
//===========================================================================================================================================================

//=================================================================================
//壁ジャンプの処理
//=================================================================================
void CPlayer::WallJumpProcess()
{
    bool bIsWalling = GetWalling();
    bool bIsLanding = GetLanding();
    if (bIsWalling == true && m_bUnlockAction[ACTIONTYPE03_WALLJUMP] == true && bIsLanding == false && m_bPossibleWallJump == true)
    {
        WallJumpParticleProcess();
    }
}
//===========================================================================================================================================================
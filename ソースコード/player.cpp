//=============================================================================
//
//�P�P���P�O���F�_�C�u����[player.cpp]
//Author:ShinaTaiyo
//
//=============================================================================

//========================
//�C���N���[�h
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
//�ÓI�����o�錾
//====================================================
const int CPlayer::s_nNORMAL_MAXLIFE = 100;//�v���C���[�̍ő�̗�
const int CPlayer::s_nMaxDiveNum = 5;      //�v���C���[�̍ő�_�C�u��
//==========================================================================================================

//====================================================
//�R���X�g���N�^
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
//�f�X�g���N�^
//====================================================
CPlayer::~CPlayer()
{

}
//==========================================================================================================

//====================================================
//����������
//====================================================
HRESULT CPlayer::Init()
{
    CCharacter::Init();                 //X�I�u�W�F�N�g������

    GetLifeInfo().SetAutoSubLife(false);//�����I�ɑ̗͂����炷���ǂ���
    GetMoveInfo().SetUseGravity(true,1.0f);  //�d�͂��g�p����

    if (CScene::GetMode() == CScene::MODE::MODE_GAME)
    {
        m_pLockOn = CLockon::Create(CLockon::TYPE::SHOT,D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), CObject2D::POLYGONTYPE::SENTERROLLING, 100.0f, 100.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
        m_pLockOn->SetUseDeath(false);
        m_pLockOn->SetPolygonRotSpeed(0.01f);

        m_pModeDisp = CUi::Create(CUi::UITYPE::ACTIONMODE_GUN, CObject2D::POLYGONTYPE::SENTERROLLING, 100.0f, 100.0f, 1, false, D3DXVECTOR3(50.0f, 50.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
            D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
        m_pModeDisp->SetUseDeath(false);

        //�_�C�u�Q�[�W�̃t���[���𐶐�
        m_pDiveGaugeFrame = CUi::Create(CUi::UITYPE::DIVEGAUGEFRAME_000, CObject2D::POLYGONTYPE::SENTERROLLING, 450.0f, 100.0f, 1, false, D3DXVECTOR3(SCREEN_WIDTH - 250.0f, 100.0f, 0.0f),
            D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
        m_pDiveGaugeFrame->SetUseDeath(false);//���S�t���O�𔭓������Ȃ�
        m_pDiveGaugeFrame->PushUiState(DBG_NEW CUiState_Gauge(D3DXVECTOR3(SCREEN_WIDTH - 390.0f, 106.5f, 0.0f),D3DXCOLOR(1.0f,1.0f,0.0f,1.0f),CObject2D::POLYGONTYPE::LEFT, CGauge::GAUGETYPE::DIVE, 350.0f, 19.6f, 0, 20));//�Q�[�W�ێ��̏�Ԃ�t�^����

        m_pDivePossibleNum = CUi::Create(CUi::UITYPE::POSSIBLEDIVENUMTEXT_000, CObject2D::POLYGONTYPE::SENTERROLLING, 200.0f, 100.0f, 1, false, D3DXVECTOR3(200.0f, 100.0f, 0.0f),
            D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 61.0f, 1.0f, 1.0f));
        m_pDivePossibleNum->SetNumericState(0, 50.0f, 50.0f);
        m_pDivePossibleNum->SetUseDeath(false);

        m_pWire = CWire::Create(CWire::WIRETYPE::ROPE, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 5.0f, 20.0f, 4, 5);
        m_pWire->SetUseDeath(false);
        m_pWire->SetPlayerPointer(this);//�v���C���[�̃|�C���^��ݒ�
        m_pWire->SetUseDraw(false);

        m_pPlayerActionMode = DBG_NEW CPlayerActionMode_ShotMove(this);//�v���C���[�̃A�N�V�������[�h�i���C���X�e�[�g�j���ˌ��ړ��ɐݒ�
    }
    return S_OK;
}
//==========================================================================================================

//====================================================
//�I������
//====================================================
void CPlayer::Uninit()
{
    CCharacter::Uninit();//X�I�u�W�F�N�g�I��
}
//==========================================================================================================

//====================================================
//�X�V����
//====================================================
void CPlayer::Update()
{
    CObjectX::PosInfo& PosInfo = GetPosInfo();//�ʒu���
    CObjectX::SizeInfo& SizeInfo = GetSizeInfo();//�T�C�Y���
    const D3DXVECTOR3& Pos = PosInfo.GetPos();//�ʒu
    const D3DXVECTOR3& VtxMax = SizeInfo.GetVtxMax();//�ő咸�_
    if (CScene::GetMode() == CScene::MODE_GAME)
    {
        if (GetLanding())
        {//�n�ʂɂ���Ȃ�d�͂��ŏ�����
            GetMoveInfo().SetMove(D3DXVECTOR3(GetMoveInfo().GetMove().x,0.0f, GetMoveInfo().GetMove().z));
        }

        if (m_pPlayerActionMode != nullptr)
        {//�v���C���[�̃A�N�V�����̃��C���X�e�[�g
            m_pPlayerActionMode->Process(this);
        }

        if (m_pMove != nullptr)
        {
            m_pMove->MoveProcess(this);//���݂̃A�N�V�������[�h�̈ړ����������s
        }
        if (m_pAbnormalState != nullptr)
        {
            m_pAbnormalState->Process(this);//��Ԉُ�̏��������s
        }
        AdjustRot();//������������

        ActionModeChengeProcess(); //���݂̃A�N�V�������[�h��ύX����
    }

    CCharacter::Update();//�X�V����

    CollisionProcess();

    if (CScene::GetMode() == CScene::MODE_GAME)
    {
        CUiState_Numeric* pUiState_Numeric = dynamic_cast<CUiState_Numeric*>(m_pDivePossibleNum->GetUiState(CUiState::UISTATE::NUMERIC));//UI�̐������擾
        if (m_pAttack != nullptr)
        {
            m_pAttack->AttackProcess(this);//���݂̃A�N�V�������[�h�̍U������������
        }
        DiveGaugeMaxEffect();//�_�C�u�Q�[�W���}�b�N�X�ɂȂ������̉��o

        if (pUiState_Numeric->GetValue() == s_nMaxDiveNum)
        {//�_�C�u�\�񐔂��ő�ɒB������_�ł�������
            m_pDivePossibleNum->SetUseBlinking(true, 20, 0.0f);//�_�ł�����
        }
        else
        {//�_�ł��~�߂�
            m_pDivePossibleNum->SetUseBlinking(false, 20, 0.0f);
            m_pDivePossibleNum->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), false, 1.0f);
        }

        CManager::GetDebugText()->PrintDebugText("�v���C���[�̈ʒu�F%f %f %f\n", GetPosInfo().GetPos().x, GetPosInfo().GetPos().y, GetPosInfo().GetPos().z);
        CManager::GetDebugText()->PrintDebugText("�v���C���[�̗̑́F%d\n", GetLifeInfo().GetLife());

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

    MotionProcess();//���[�V�����������s��
}
//==========================================================================================================

//====================================================
//�`�揈��
//====================================================
void CPlayer::Draw()
{
    CCharacter::Draw();
}
//==========================================================================================================

//====================================================
//���S�t���O��ݒ�
//====================================================
void CPlayer::SetDeath()
{
    if (GetUseDeath() == true)
    {
        if (m_pMove != nullptr)
        {//�ړ��X�e�[�g�̊J��
            delete m_pMove;
            m_pMove = nullptr;
        }

        if (m_pAttack != nullptr)
        {//�U���X�e�[�g�̊J��
            delete m_pAttack;
            m_pAttack = nullptr;
        }

        if (m_pAbnormalState != nullptr)
        {//��ԃX�e�[�g�̊J��
            delete m_pAbnormalState;
            m_pAbnormalState = nullptr;
        }

        if (m_pPlayerActionMode != nullptr)
        {//�v���C���[�A�N�V�������[�h�̃��C���X�e�[�g�̉��
            delete m_pPlayerActionMode;
            m_pPlayerActionMode = nullptr;
        }

        if (m_pLockOn != nullptr)
        {//���b�N�I��
            m_pLockOn->SetUseDeath(true);
            m_pLockOn->SetDeath();
            m_pLockOn = nullptr;
        }

        if (m_pModeDisp != nullptr)
        {//���[�h�\���̊J��
            m_pModeDisp->SetUseDeath(true);
            m_pModeDisp->SetDeath();
            m_pModeDisp = nullptr;
        }

        if (m_pWire != nullptr)
        {//���C���[�̊J��
            m_pWire->SetUseDeath(true);
            m_pWire->SetDeath();
            m_pWire = nullptr;
        }

        if (m_pHpGauge != nullptr)
        {//�̗̓Q�[�W�̊J��
            m_pHpGauge->SetUseDeath(true);
            m_pHpGauge->SetDeath();
            m_pHpGauge = nullptr;
        }

        if (m_pDivePossibleNum != nullptr)
        {//�_�C�u�\��UI�̊J��
            m_pDivePossibleNum->SetUseDeath(true);
            m_pDivePossibleNum->SetDeath();
            m_pDivePossibleNum = nullptr;
        }

        if (m_pDiveGaugeFrame != nullptr)
        {//�_�C�u�Q�[�W�̃t���[��UI
            m_pDiveGaugeFrame->SetUseDeath(true);
            m_pDiveGaugeFrame->SetDeath();
            m_pDiveGaugeFrame = nullptr;
        }
    }
    CCharacter::SetDeath();
}
//===========================================================================================================

//====================================================
//�v���C���[�̐���
//====================================================
CPlayer* CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, D3DXVECTOR3 Scale)
{
    CPlayer* pPlayer = DBG_NEW CPlayer(DBG_NEW CPlayerMove_Normal(),DBG_NEW CPlayerAttack_Shot());//�v���C���[�𐶐�
    bool bSuccess = pPlayer->CObject::GetCreateSuccess();
    int nIdx = 0;//�e�N�X�`���̃C���f�b�N�X
    CObjectX::PosInfo& PosInfo = pPlayer->GetPosInfo();
    CObjectX::RotInfo& RotInfo = pPlayer->GetRotInfo();
    CObjectX::SizeInfo& SizeInfo = pPlayer->GetSizeInfo();
    CObjectX::LifeInfo& LifeInfo = pPlayer->GetLifeInfo();
    pPlayer->Init();                                                                 //����������
    pPlayer->GetMoveInfo().SetMove(move);//�ړ���
    pPlayer->CObject::SetType(CObject::TYPE::PLAYER);                                 //�I�u�W�F�N�g�̎�ނ����߂�
    pPlayer->SetObjXType(CObjectX::OBJECTXTYPE_PLAYER);                    //�I�u�W�F�N�gX�̃^�C�v��ݒ�
    pPlayer->SetTypeNum(0);                                                //�I�u�W�F�N�gX���Ƃ̃^�C�v�ԍ���ݒ�
    pPlayer->RegistMotion("data\\MODEL\\Enemy\\MotionEnemy\\DiveWeakEnemy\\DiveWeakEnemyMotion.txt",pPlayer);//���[�V�����t�@�C�������蓖�Ă�
    pPlayer->GetDrawInfo().SetUseDraw(false);                                                     //�`�悵�Ȃ�
    PosInfo.SetPos(pos);                                                            //�ʒu�̐ݒ�
    PosInfo.SetPosOld(pos);                                                         //1f�O�̈ʒu��ݒ�
    PosInfo.SetPosFuture(pos);                                                      //1f��̈ʒu��ݒ�
    PosInfo.SetSupportPos(pos);                                                     //�ݒu�ʒu
    RotInfo.SetRot(rot);                                                            //�����̐ݒ�
    SizeInfo.SetScale(Scale);                                                       //�g�嗦�̐ݒ�
    SizeInfo.SetFormarScale(Scale);                                                  //���̊g�嗦��ݒ肷��
    pPlayer->GetLifeInfo().SetAutoDeath(false);                                                    //���S�t���O�������Ŕ������邩�ǂ���
    pPlayer->GetDrawInfo().SetUseShadow(true);

    //�̗�
    LifeInfo.SetLife(s_nNORMAL_MAXLIFE);
    LifeInfo.SetMaxLife(s_nNORMAL_MAXLIFE);

    if (CScene::GetMode() == CScene::MODE::MODE_GAME)
    {
        pPlayer->m_pHpGauge = CGauge::Create(CGauge::GAUGETYPE::PLAYERHP, s_nNORMAL_MAXLIFE, 600.0f, 50.0f, D3DXVECTOR3(50.0f, SCREEN_HEIGHT - 50.0f, 0.0f));
        pPlayer->m_pHpGauge->SetParam(s_nNORMAL_MAXLIFE);//�ŏ�����̗͂��ő�l�ɐݒ�
        pPlayer->m_pHpGauge->SetUseDeath(false);
    }
	return pPlayer;
}
//==========================================================================================================

//====================================================
//���[�h�\����ݒ肷��
//====================================================
void CPlayer::SetModeDisp(CUi* pModeDisp)
{
    if (m_pModeDisp != nullptr)
    {
        m_pModeDisp->SetUseDeath(true);//���S�t���O���g�p����
        m_pModeDisp->SetDeath();       //���S�t���O��ݒ肷��
        m_pModeDisp = nullptr;         //�|�C���^��������

        m_pModeDisp = pModeDisp;      
        m_pModeDisp->SetUseDeath(false);//���S�t���O���g�p���Ȃ�
    }
}
//==========================================================================================================

//========================================================
//�U���J�n
//========================================================
void CPlayer::ActionModeChengeProcess()
{
    if (CManager::GetInputJoypad()->GetTrigger(CInputJoypad::JOYKEY::X) || CManager::GetInputMouse()->GetMouseRightClickTrigger())
    {
        CGame::GetTutorial()->SetSuccessCheck(CTutorial::CHECK::MODECHENGE);

        //���[�h��؂�ւ���
        if (m_NowActionMode == ACTIONMODE::SHOT)
        {//�V���b�g���_�C�u
            SetInitialActionMode(ACTIONMODE::DIVE);
        }
        else
        {//�_�C�u���V���b�g
            SetInitialActionMode(ACTIONMODE::SHOT);
        }
    }
}
//==========================================================================================================

//========================================================
//�_�C�u�Q�[�W���}�b�N�X�ɂȂ������̉��o���s��
//========================================================
void CPlayer::DiveGaugeMaxEffect()
{
    CDebugText* pDebugText = CManager::GetDebugText();
    CUiState_Numeric* pUiState_Numeric = dynamic_cast<CUiState_Numeric*>(m_pDivePossibleNum->GetUiState(CUiState::UISTATE::NUMERIC));//UI�̐�����Ԃ��擾
    CUiState_Gauge* pUiState_Gauge = dynamic_cast<CUiState_Gauge*>(m_pDiveGaugeFrame->GetUiState(CUiState::UISTATE::GAUGE));//UI�̃Q�[�W�����擾
    if (pUiState_Gauge != nullptr)
    {
        CGauge* pDiveGauge = pUiState_Gauge->GetGauge();//�_�C�u�Q�[�W���擾����
        if (pUiState_Numeric != nullptr)
        {
            pDebugText->PrintDebugText("�_�C�u�\�񐔁F%d\n", pUiState_Numeric->GetValue());
            if (pDiveGauge->GetFullGaugeFlag() == true)
            {//�Q�[�W���}�b�N�X�ɂȂ����u�u�ԁv�Ƀt���O�𔭓������ő�_�C�u�\�񐔂ɒB���Ă��Ȃ�������
                CGauge* pGauge = CGauge::Create(CGauge::GAUGETYPE::PLAYERHP, pDiveGauge->GetParam(), pDiveGauge->GetWidth(), pDiveGauge->GetHeight(), pDiveGauge->GetPos());
                pGauge->SetUseLife(true, 50, 50);
                pGauge->SetPolygonType(pDiveGauge->GetPolygonType());
                pGauge->SetColor(pDiveGauge->GetColor(), false, 1.0f);
                pGauge->SetUseLifeRatioColor(true);
                pGauge->SetUseDeath(true);
                pGauge->SetUseAddScale(D3DXVECTOR2(0.3f, 0.3f), true);
                pGauge->SetUseScale(true);
                pGauge->SetScale(D3DXVECTOR2(1.0f, 1.0f));
                pDiveGauge->SetParam(0);//�_�C�u�Q�[�W�����Z�b�g
                pUiState_Numeric->SetValue(pUiState_Numeric->GetValue() + 1, m_pDivePossibleNum);
            }
        }
    }
}
//==========================================================================================================

//========================================================
//�ړ����[�h���`�F���W
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
//�U�����[�h���`�F���W
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
//�v���C���[�̃A�N�V�������[�h���`�F���W
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
//���ꂼ��̃A�N�V�������[�h�̏�����Ԃɐݒ肷��
//========================================================
void CPlayer::SetInitialActionMode(ACTIONMODE ActionMode)
{
    m_NowActionMode = ActionMode;//�A�N�V�������[�h�̃f�[�^��ς���

    //���[�h����
    switch (ActionMode)
    {
    case ACTIONMODE::SHOT://���˃��[�h
        ChengeActionMode(DBG_NEW CPlayerActionMode_ShotMove(this));
        break;
    case ACTIONMODE::DIVE://�_�C�u���[�h
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
//�����蔻��S�ʏ���
//========================================================
void CPlayer::CollisionProcess()
{
    SetIsLanding(false);                           //�n�ʂɏ���Ă��邩�ǂ����̃t���O�����Z�b�g
    GetCollisionInfo().GetSquareInfo().ResetPushOutFirstFlag();//���ꂼ��̎��̉����o������̗D��t���O�����Z�b�g
    m_bCollision = false;                          //�����Ԃ����Z�b�g
    for (int nCntPri = 0; nCntPri < CObject::m_nMAXPRIORITY; nCntPri++)
    {
        CObject* pObj = CObject::GetTopObject(nCntPri);

        while (pObj != nullptr)
        {
            //���̃I�u�W�F�N�g���i�[
            CObject* pNext = pObj->GetNextObject();

            //��ނ̎擾�i�G�Ȃ瓖���蔻��j
            CObject::TYPE type = pObj->GetType();

            if (type == CObject::TYPE::BLOCK || type == CObject::TYPE::BGMODEL)
            {
                CObjectX* pObjX = static_cast<CObjectX*>(pObj);//�I�u�W�F�N�gX�Ƀ_�E���L���X�g
                CCollision::ExtrusionCollisionSquarePushOutFirstDecide(this, pObjX);//�����`�̉����o������̂��ꂼ��̎��̏����̗D��x�����߂�
            }
            //�I�u�W�F�N�g�����ɐi�߂�
            pObj = pNext;
        }
    }
    //=======================================================================================

    //============================================================
    //�����o������J�n
    //============================================================
    for (int nCntPri = 0; nCntPri < CObject::m_nMAXPRIORITY; nCntPri++)
    {
        CObject* pObj = CObject::GetTopObject(nCntPri);

        while (pObj != nullptr)
        {
            //���̃I�u�W�F�N�g���i�[
            CObject* pNext = pObj->GetNextObject();

            //��ނ̎擾�i�G�Ȃ瓖���蔻��j
            CObject::TYPE type = pObj->GetType();

            if (type == CObject::TYPE::BLOCK || type == CObject::TYPE::BGMODEL)
            {
                CObjectX* pObjX = static_cast<CObjectX*>(pObj);//�I�u�W�F�N�gX�Ƀ_�E���L���X�g

                CCollision::ResolveExtrusionCollisionSquare(this, pObjX);//�����`�̉����o�����������
            }

            pObj = pNext;
        }
    }
    //=======================================================================================
}
//==========================================================================================================

//========================================================
//�u���b�N�Ƃ̓����蔻��
//========================================================
void CPlayer::CollisionBlock()
{

}
//==========================================================================================================

//========================================================
//�W�����v����
//========================================================
void CPlayer::JumpProcess()
{
    //
}
//==========================================================================================================

//========================================================
//��Ԉُ��ς���
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
//�_���[�W��^����
//========================================================
void CPlayer::SetDamage(int nDamage, int nHitStopTime)
{
    if (GetLifeInfo().GetHitStop() == false)
    {//�q�b�g�X�g�b�v��Ԃ���Ȃ���Ώ��������s����
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

        m_bDamage = true;//�_���[�W���󂯂���Ԃ𖾎��I�Ɏ���
        SetNextMotion(2);
    }
}
//==========================================================================================================

//========================================================
//������������
//========================================================
void CPlayer::AdjustRot()
{
    const D3DXVECTOR3& CameraRot = CManager::GetCamera()->GetRot();
    GetRotInfo().SetRot(D3DXVECTOR3(GetRotInfo().GetRot().x,D3DX_PI + CameraRot.y, GetRotInfo().GetRot().z));
}
//==========================================================================================================

//========================================================
//�ʒu��������
//========================================================
void CPlayer::AdjustPos()
{
    const D3DXVECTOR3& Pos = GetPosInfo().GetPos();
    D3DXVECTOR3 ScreenPos = CCalculation::CalcWorldToScreenNoViewport(Pos, *CManager::GetCamera()->GetMtxView(), *CManager::GetCamera()->GetMtxProjection(),
        static_cast<float>(SCREEN_WIDTH), static_cast<float>(SCREEN_HEIGHT));

    //��ʊO�ɏo�Ȃ��悤�ɕ␳
    if (ScreenPos.x > SCREEN_WIDTH || ScreenPos.x < 0.0f)
    {
        GetPosInfo().SetPos(GetPosInfo().GetPosOld());
    }
}
//==========================================================================================================

//*******************************************************
//��Ԉُ�X�e�[�g�F�X�[�p�[�N���X
//*******************************************************

//=======================================================
//�R���X�g���N�^
//=======================================================
CPlayerAbnormalState::CPlayerAbnormalState()
{

}
//==========================================================================================================

//=======================================================
//�f�X�g���N�^
//=======================================================
CPlayerAbnormalState::~CPlayerAbnormalState()
{

}
//==========================================================================================================

//=======================================================
//����
//=======================================================
void CPlayerAbnormalState::Process(CPlayer* pPlayer)
{

}
//==========================================================================================================

//*******************************************************
//��Ԉُ�X�e�[�g�F�m�b�N�o�b�N
//*******************************************************

//=======================================================
//�R���X�g���N�^
//=======================================================
CPlayerAbnormalState_KnockBack::CPlayerAbnormalState_KnockBack(CPlayer* pPlayer, D3DXVECTOR3 KnockBackMove, float fInertia) : m_KnockBackMove(KnockBackMove),m_fInertia(fInertia)
{

}
//==========================================================================================================

//=======================================================
//�f�X�g���N�^
//=======================================================
CPlayerAbnormalState_KnockBack::~CPlayerAbnormalState_KnockBack()
{

}
//==========================================================================================================

//=======================================================
//����
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
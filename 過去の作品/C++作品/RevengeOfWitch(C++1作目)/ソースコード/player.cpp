//=============================================================================
//
//�V���S���F����A�N�V��������[player.cpp]
//Author:ShinaTaiyo
//
//=============================================================================

//========================
//�C���N���[�h
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
//�ÓI�����o�ϐ��̏�����
//====================================================
LPD3DXMESH CPlayer::m_pMeshTemp = nullptr;                             //���b�V�����ւ̃|�C���^
LPD3DXBUFFER CPlayer::m_pBuffMatTemp = nullptr;                        //�}�e���A���ւ̃|�C���^
DWORD CPlayer::m_dwNumMatTemp = NULL;                                  //�}�e���A���̐�
LPDIRECT3DTEXTURE9 CPlayer::m_pTextureTemp[CPlayer::m_nMAX_MAT] = {};  //�e�N�X�`���ւ̃|�C���^
int CPlayer::m_nNumFile = 1;                                           //�t�@�C���̐���ۑ�����
const float CPlayer::m_fNORMALJUMPPOWER = 9.0f;                        //�ʏ�̃W�����v��
const float CPlayer::m_fGRAVITYPOWER = 1.0f;                           //�d�͂̑傫��
const float CPlayer::m_fNORMALSPEED = 2.25f;                           //�ʏ�̑���
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
};//���ꂼ��̖��@�̏���MP
const int CPlayer::m_nCHARGEATTACKPOWER[CPlayer::m_nMAX_CHARGEATTACKLEVEL] =
{ 35,72,110,150,200 };                                                 //�`���[�W�U���̈З͂̊�b�l
//==========================================================================================================

//====================================================
//�R���X�g���N�^
//====================================================
CPlayer::CPlayer() : m_nRespawnCnt(0),m_pUsageMpGauge(nullptr)
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
    CObjectX::Init();                 //X�I�u�W�F�N�g������
    m_nJumpPushCnt = 0;               //�W�����v�{�^���������Ă��鎞�Ԃ̃J�E���g��
    m_bMove = false;                  //�ړ������ǂ���
    m_AddMove = NULL_VECTOR3;         //���Z�ړ���
    m_CompulsionPos = NULL_VECTOR3;   //���Z�ʒu
    m_SaveMove = NULL_VECTOR3;        //�ۑ�����ړ���
    m_nCollisionCheck = 0;            //����`�F�b�N�p
    m_bCompulsionPos = false;         //�ʒu������Ԃ��ǂ���
    m_pNumber = nullptr;              //�ԍ��ւ̃|�C���^
    m_bJustStopMove = false;          //�ړ�����u�~�߂�t���O
    m_fRot = 0.0f;                    //�����̕␳�p
    m_fCorrectionInertia = 0.0f;      //�����̕␳
    m_fJump = m_fNORMALJUMPPOWER;     //�W�����v��
    m_fSpeed = m_fNORMALSPEED;        //�ړ����x
    m_nCntLanding = 0;                //�n�ʂɂ��鎞�Ԃ��J�E���g����
    //==========================================================================================

    //=========================================
    //���@�֌W
    //=========================================
    m_bPossibleMagic = true;          //���@���g�p�\���ǂ���
    m_nUseMagicNum = 0;               //�g�p���Ă��閂�@�̔ԍ�
    m_nNextUseMagic = 0;              //���ɖ��@���g�p�\�ɂȂ�܂ł̎���
    m_nUseMagicTime = 0;              //���@�g�p���Ԃ��J�E���g����
    m_pMagicDisp = nullptr;           //���@�\���ւ̃|�C���^
    m_pMagicWindow = nullptr;         //���@�\���E�C���h�E�ւ̃|�C���^
    m_nNowMagicType = 0;              //���݂̖��@�̃^�C�v
    m_nMagicNum = 0;                  //���ݎ����Ă��閂�@�̎��
    for (int nCnt = 0; nCnt < m_nMAX_MAGICNUM; nCnt++)
    {
        m_nMagicSlot[nCnt] = 0;//���@�̏���
        m_bUnlockMagic[nCnt] = false;//���@���J�����Ă��邩�ǂ���
#ifdef _DEBUG
        SetUnlockMagic(nCnt, NULL_VECTOR3, true);
#endif // _DEBUG
    }
    //==========================================================================================

    //=========================================
    //�ړ��֌W
    //=========================================
    m_nStopMoveCnt = 0;               //�ړ����~�߂鎞�Ԃ��J�E���g����
    //==========================================================================================

    //=========================================
    //�X�e�[�^�X
    //=========================================
    m_Status.nHp = 0;                               //�̗�
    m_Status.nMaxHp = 0;                            //�ő�̗�
    m_Status.nMp = 0;                               //�}�W�b�N�|�C���g
    m_Status.nMaxMp = 0;                            //�ő�}�W�b�N�|�C���g
    m_Status.fAtkBuff = m_fINIT_ATTACKBUFF;         //�U���o�t�{��
    //==========================================================================================

    //==========================================
    //�A�N�V�����֌W
    //==========================================
    m_nJumpCnt = 0;                                             //�W�����v�����񐔂��J�E���g����

    //���
    m_Dodge.nStateCnt = 0;                                      //�����Ԃ̎��Ԃ��J�E���g����
    m_Dodge.bPossible = false;                                  //����\���ǂ���
    m_Dodge.bState = false;                                     //�����Ԃ��ǂ���
    m_Dodge.nRevivalCnt = 0;                                    //�����Ԃ���������܂ł̎���
#ifdef _DEBUG
    for (int nCnt = 0; nCnt < ACTIONTYPE_MAX; nCnt++)
    {//�S�ẴA�N�V�������ŏ�����g�p�\�ɂ���
        m_bUnlockAction[nCnt] = true;
    }
#else
    for (int nCnt = 0; nCnt < ACTIONTYPE_MAX; nCnt++)
    {//�S�ẴA�N�V�������ŏ�����͎g�p�s�\�ɂ���
        m_bUnlockAction[nCnt] = false;
    }
#endif // DEBUG


    //==========================================================================================

    //==========================================
    //�e�̋Z�֌W
    //==========================================
#ifdef _DEBUG
    for (int nCnt = 0; nCnt < BULLETACTION_MAX; nCnt++)
    {//�S�Ẵo���b�g�A�N�V�������ŏ�����g�p�\�ɂ���
        m_bUnlockBulletAction[nCnt] = true;
    }

#else
    for (int nCnt = 0; nCnt < BULLETACTION_MAX; nCnt++)
    {//�S�Ẵo���b�g�A�N�V�������ŏ�����͎g�p�s�\�ɂ���
        m_bUnlockBulletAction[nCnt] = false;
    }
#endif // DEBUG

    //==========================================================================================

    //==========================================
    //�`���[�W�U��
    //==========================================
    m_ChargeAttack.nChargeCnt = 0;                              //�`���[�W�������Ԃ��J�E���g����
    m_ChargeAttack.nPower = 0;                                  //�`���[�W�U���̈З�
    m_ChargeAttack.bState = false;                              //�`���[�W��Ԃ��ǂ���
    m_ChargeAttack.nChargeLevel = 0;                            //�`���[�W�i�K
    //==========================================================================================

    //=====================================
    //�O�Պ֌W
    //=====================================
    m_pMeshOrbit = nullptr;
    //==========================================================================================

    //==========================================
    //�Q�[�W�֌W
    //==========================================
    m_pHpGauge = nullptr;             //�̗̓Q�[�W�ւ̃|�C���^
    m_pMpGauge = nullptr;             //MP�Q�[�W�ւ̃|�C���^
    m_pGaugeFrame = nullptr;          //�Q�[�W�t���[���ւ̃|�C���^
    //==========================================================================================

    //==========================================
    //�J�����֌W
    //==========================================
    m_bReturnCamera = false;
    m_bStartFade = false;
    //==========================================================================================

    //==========================================
    //�X�R�A�֌W
    //==========================================
    m_pScore = nullptr;//�X�R�A�ւ̃|�C���^
    //==========================================================================================

    //==========================================
    //���A�N�V�����֌W
    //==========================================
    m_pUiReaction = nullptr;                      //���A�N�V�����ւ̃|�C���^
    m_nCntChengeUiReactionTime = 0;               //���A�N�V������ς��鎞�Ԃ��J�E���g����
    m_bChengeUiReactionFlag = false;              //���A�N�V������ω������邩�ǂ���.
    //==================================================================================== ======

    //==========================================
    //�R�}���h�֌W
    //==========================================
    for (int nCnt = 0; nCnt < PLAYERCOMMAND_MAX; nCnt++)
    {//�R�}���h�𔭓��������ǂ����̃t���O��false�ɂ���
        m_bJudgeCommand[nCnt] = false;
    }
    //==========================================================================================

    //==========================================
    //�񕜊֌W
    //==========================================
    m_Heal.nHealCnt = 0;  //�q�[����Ԃ̃J�E���g
    m_Heal.bState = false;//�q�[����Ԃ��ǂ���
    m_Heal.bLastHeal = false;//�񕜂ɕK�v��MP������Ȃ��Ȃ������p�̃t���O
    //==========================================================================================

    //==========================================
    //��Ԉُ�
    //==========================================
    
    //����
    m_State.SlideState.nStateCnt = 0; //�����ԂɂȂ��Ă���̃J�E���g

    //����
    m_State.DecelerationState.bState = false;//�������
    m_State.DecelerationState.nStateCnt = 0;//������ԂɂȂ��Ă���̃J�E���g

    //==========================================================================================

    //==========================================
    //�T�C�R�o�[�X�g�֌W
    //==========================================
    m_pLockOnUi = nullptr;
    //==========================================================================================

    //==========================================
    //���G���
    //==========================================
    m_bInvincible = false;              //���G��Ԃ��ǂ���
    m_nCntInvincible = 0;               //���G��Ԃ̎��Ԃ��J�E���g����
    //==========================================================================================

    //==========================================
    //�ǃW�����v�֌W
    //==========================================
    m_nWallJumpBindTime = 0;//�ǃW�����v���s�\�Ȏ��Ԃ��J�E���g����
    m_bPossibleWallJump = true;//�ǃW�����v���\���ǂ���
    //==========================================================================================

    //==========================================
    //�v���C���[�̃p�[�c�֌W
    //==========================================
    for (int nCnt = 0; nCnt < m_nMAX_PLAYERPARTS; nCnt++)
    {
        m_apModelParts[nCnt] = nullptr;//���f���p�[�c�ւ̃|�C���^
    }
    //==========================================================================================

    //���f���p�[�c�̃��[�h����
    LoadModelParts();

    //m_apModelParts[10]->SetScale(ONE_VECTOR3 * 3.0f);

    //==========================================================================================

    //==========================================
    //�����֌W
    //=========================================
    m_fSlideInertia = 0.0f;//���点�邽�߂̊���
    m_fReturnInertia = 0.0f;//�߂����߂̊���
    //===========================================================================================

    //==========================================
    //���[�V�����֌W
    //==========================================
    m_nNowMotion = 1;//���݂̃��[�V����
    m_nNextMotion = 0;//���̃��[�V����
    m_nCntFrame = 0;//���݂̃t���[����
    m_nMotionOld = 0;//1f�O�̃��[�V����
    //������
    for (int nCntMotion = 0; nCntMotion < m_nMAX_MOTION; nCntMotion++)
    {
        m_aMotion[nCntMotion].bLoop = false;//���[�v���邩�ǂ���
        m_aMotion[nCntMotion].nCntFrame = 0;//�t���[���J�E���g
        m_aMotion[nCntMotion].nCntKey = 0;  //�L�[�J�E���g
        m_aMotion[nCntMotion].nNumKey = 0;  //�L�[����
        for (int nCntKeySet = 0; nCntKeySet < m_nMAX_KEYSET; nCntKeySet++)
        {
            m_aMotion[nCntMotion].aKeySet[nCntKeySet].nFrame = 0;//�t���[���ő吔
            for (int nCntKey = 0; nCntKey < m_nMAX_PLAYERPARTS; nCntKey++)
            {
                m_aMotion[nCntMotion].aKeySet[nCntKeySet].aKey[nCntKey].Pos = NULL_VECTOR3;//�ʒu
                m_aMotion[nCntMotion].aKeySet[nCntKeySet].aKey[nCntKey].Rot = NULL_VECTOR3;//����
            }
        }
    }

    for (int nCnt = 0; nCnt < MOTIONTYPE_MAX; nCnt++)
    {
        m_bPriorityMotion[nCnt] = false;//���[�V�����̗D��x�ɂ���Ď��s���郂�[�V������ς���
    }

    //���[�V�����̃��[�h����
    LoadMotion();

    //==========================================================================================

    if (CScene::GetMode() == CScene::MODE_GAME)
    {
        if (CGame::GetDifficulty() == CDifficulty::DIFFICULTYTYPE_BOSSRUSH)
        {
            //�S�ẴA�N�V�������J������
            for (int nCnt = 0; nCnt < BULLETACTION_MAX; nCnt++)
            {//�S�Ẵo���b�g�A�N�V�������ŏ�����g�p�\�ɂ���
                m_bUnlockBulletAction[nCnt] = true;
            }
            for (int nCnt = 0; nCnt < ACTIONTYPE_MAX; nCnt++)
            {//�S�ẴA�N�V�������ŏ�����g�p�\�ɂ���
                m_bUnlockAction[nCnt] = true;
            }
            for (int nCnt = 0; nCnt < m_nMAX_MAGICNUM; nCnt++)
            {//�V�������@���������
                SetUnlockMagic(nCnt,NULL_VECTOR3,false);
            }
        }
    }
	return S_OK;
}
//==========================================================================================================

//====================================================
//�I������
//====================================================
void CPlayer::Uninit()
{
    //=================================
    //���@�\���̏I������
    //=================================
    if (m_pMagicDisp != nullptr)
    {
        m_pMagicDisp = nullptr;
    }
    //=================================================================

    //=================================
    //���@�\���E�C���h�E�̏I������
    //=================================
    if (m_pMagicWindow != nullptr)
    {
        m_pMagicWindow = nullptr;
    }
    //=================================================================

    //=================================
    //���l�\���̏I������
    //=================================
    if (m_pNumber != nullptr)
    {
        m_pNumber = nullptr;
    }
    //=================================================================

    //=================================
    //HP�\���̏I������
    //=================================
    if (m_pHpGauge != nullptr)
    {
        m_pHpGauge = nullptr;
    }
    //=================================================================

    //=================================
    //MP�\���̏I������
    //=================================
    if (m_pMpGauge != nullptr)
    {
        m_pMpGauge = nullptr;
    }
    //=================================================================
    
    //=================================
    //���b�N�I��UI�̏I������
    //=================================
    if (m_pLockOnUi != nullptr)
    {
        m_pLockOnUi = nullptr;
    }

    //=================================
    //���f���p�[�c�̏I������
    //=================================
    for (int nCnt = 0; nCnt < m_nMAX_PLAYERPARTS; nCnt++)
    {
        if (m_apModelParts[nCnt] != nullptr)
        {
            m_apModelParts[nCnt] = nullptr;
        }
    }
    //=================================================================

    CObjectX::Uninit();//X�I�u�W�F�N�g�I��
}
//==========================================================================================================

//====================================================
//�ʘg�̏I������
//====================================================
void CPlayer::ExtraUninit()
{
}
//==========================================================================================================

//====================================================
//�X�V����
//====================================================
void CPlayer::Update()
{
    if (CScene::GetMode() == CScene::MODE_GAME && CManager::GetEventManager()->GetEventType() != CEventManager::EVENTTYPE_BOSSAPPEAR)
    {
        m_nRespawnCnt++;
        D3DXVECTOR3& pos = GetPos();                                   //���f���̈ʒu�̎擾
        D3DXVECTOR3& PosOld = GetPosOld();                               //1f�O�̈ʒu��ݒ�
        D3DXVECTOR3& Move = GetMove();                                   //�ړ��ʂ��擾����
        D3DXVECTOR3& Rot = GetRot();                                   //���f���̌����̎擾
        D3DXVECTOR3 Size = GetSize();                                   //�T�C�Y���擾
        float& fInertia = GetInertia();                                 //�������擾
        float fMoveX = 0.0f;                                            //X�����̈ړ���
        float fMoveZ = 0.0f;                                            //Z�����̈ړ���
        bool bAddMove = false;
        bool& bIsLanding = GetLanding();                                //�n�ʂɂ��邩�ǂ���
        bool& bIsWalling = GetWalling();                                //�ǂɂ������Ă��邩�ǂ���
        bool& bIsLiftLanding = GetLiftLanding();                        //���t�g�ɏ���Ă��邩�ǂ���
        bool& bIsJumping = GetJumping();                                //�W�����v���Ă��邩�ǂ���
        bool& bDodge = GetDodgeState();                                 //�����Ԃ��ǂ���
        bool bSuccessBlockCollisionX = false;                           //�u���b�N��X�����̓����蔻�肪�����������ǂ���
        bool bSuccessBlockCollisionY = false;                           //�u���b�N��Y�����̓����蔻�肪�����������ǂ���
        CObject*& pLandingObj = GetLandingObj();                         //����Ă���I�u�W�F�N�g���擾

        //�R�}���h���胊�Z�b�g
        ResetJudgeCommand();

        //������Ԃ̏���
        DecelerationProcess();

        //���G��Ԃ̏���
        InvincibleProcess();

        //UI���A�N�V��������
        UiReactionProcess();

        //=====================================
        //���_�ɓ��B�����Ƃ��̈ʒu�̕␳
        //=====================================
        AdjusOriginPosProcess();
        //===========================================================

        //�d�͂̏���
        GravityProcess();

        //�ǃW�����v�}������
        WallJumpBindProcess();

        //=====================================================
        //�ړ�����
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
        // �ړ��{�^���������Ă�����
        //============================
        if (fMoveX != 0.0f || fMoveZ != 0.0f)
        {
            m_bMove = true;//�ړ����
        }
        else
        {
            m_nNextMotion = 0;
            m_bMove = false;//�ҋ@���
        }

        if (m_nStopMoveCnt > 0)
        {//�ړ����~�߂�J�E���g���O�ȏ�Ȃ�
            m_bMove = false;//�������~�߂�
            m_nStopMoveCnt--;//�J�E���g���ւ炵�Â���@
        }

        if (m_bMove == true && bDodge == false && m_Heal.bState == false)
        {//�ړ��{�^���������Ă���A�����Ԃ���Ȃ��A�񕜏�Ԃ���Ȃ��Ȃ�
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

            m_bJudgeCommand[PLAYERCOMMAND00_MOVE] = true;//�ړ������Ă���
        }
        //==================================================================================================================================================

        //========================================================
        //�A�N�V�����I
        //========================================================
        if (m_bUnlockAction[ACTIONTYPE00_DOUBLEJUMP] == false)
        {
            if (bDodge == false)
            {
                AdjustJump();//�����W�����v
            }
        }
        else
        {//�_�u���W�����v�J���ς݂Ȃ�
            if (bDodge == false)
            {
                DoubleJump();//�Q�i�W�����v
            }
        }

        if (m_bUnlockAction[ACTIONTYPE01_DODGE] == true)
        {
            DodgeAction(fMoveX, fMoveZ);     //���
        }
        //================================================================================================================================


        if (bDodge == false)
        {
            PlayerInertiaProcess();//�v���C���[�̊����̏���

            SetUseUpdatePos(true);
        }
        else
        {//�����Ԃ̋����̂��ߕۑ������ړ��ʂŉ��Z��������
            SetUseUpdatePos(false);
            PosOld = pos;
            pos += m_SaveMove;           //�ʒu�̍X�V
        }


        //�I�u�W�F�N�gX�X�V����
        CObjectX::Update();

        m_State.SlideState.bState = false;//�����Ԃ��I�t�ɂ���

       //==================================
       //�O�Ղ��o��
       //==================================
        MeshOrbitProcess();
        //==================================================================================
        SetRot(Rot); //�����̐ݒ�

        if (CScene::GetMode() == CScene::MODE_GAME)
        {//�Q�[�����[�h�̎������A�N�V�����\

            //NormalAttack();//�ʏ�U��
            ChargeAttack();//�`���[�W�\�ʏ�U��

            //���@�ύX
            ChengeMagic();

            //���@�U��
            MagicAttack();
        }

        if (CManager::GetInputKeyboard()->GetTrigger(DIK_I) == true)
        {//�̗͌��炷
            m_Status.nHp -= 1;
            m_pHpGauge->SetParam(m_Status.nHp);
        }

        //���[�V�����D��ݒ�
        MotionPriorityProcess();

        //���[�V��������
        MotionProcess();

        //�v���C���[���\������
        DispPlayerInfo();

        //�ǃW�����v�̏���
        WallJumpProcess();

        //�����蔻��
        CollisionEnemy();
        CollisionItem();
        CollisionTrap();

        //�ړ��ʒ�������
        if (Move.y <= -20.0f)
        {
            Move.y = -20.0f;
        }

        //���S����
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
//�`�揈��
//====================================================
void CPlayer::Draw()
{
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice(); //�f�o�C�X�ւ̃|�C���^�Ǝ擾
    D3DXMATRIX mtxRot, mtxTrans, mtxScale;                  //�v�Z�p�}�g���b�N�X
    D3DMATERIAL9 matDef;                                              //���݂̃}�e���A���ۑ��p
    D3DXVECTOR3& Pos = GetPos();    //�ʒu
    D3DXVECTOR3& Rot = GetRot();    //����
    D3DXVECTOR3& Scale = GetScale();//�g�嗦
    D3DXMATRIX& mtxWorld = GetMatrixWorld();
    //���݂̃}�e���A�����擾
    pDevice->GetMaterial(&matDef);

    //���[���h�}�g���b�N�X�̏�����
    D3DXMatrixIdentity(&GetMatrixWorld());

    //�傫���𔽉f
    D3DXMatrixScaling(&mtxScale, Scale.x, Scale.y, Scale.z);
    D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScale);

    //�����𔽉f
    D3DXMatrixRotationYawPitchRoll(&mtxRot, Rot.y, Rot.x, Rot.z);
    D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

    //�ʒu�𔽉f
    D3DXMatrixTranslation(&mtxTrans, Pos.x, Pos.y, Pos.z);
    D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

    //���[���h�}�g���b�N�X�̐ݒ�
    pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

    //=======================================
    //�`��̒���
    //=======================================

    //�@���̒������P�ɂ���B�i�X�P�[���Ȃǂ��g�������́A�K���g���B)
    pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
    //================================================================================================================

    //=======================================
    //���f���p�[�c�̕`��
    //=======================================
    for (int nCnt = 0; nCnt < m_nMAX_PLAYERPARTS; nCnt++)
    {
        m_apModelParts[nCnt]->ExtraDraw();
    }
    //================================================================================================================


    ////�}�e���A���ւ̃|�C���^���擾
    //pMat = (D3DXMATERIAL*)GetObjectXInfo().pBuffMat->GetBufferPointer();

    ////==========================================================================
    ////�}�e���A���̐����A�e�N�X�`����ǂݍ��ށB
    ////==========================================================================
    //for (int nCntMat = 0; nCntMat < (int)GetObjectXInfo().dwNumMat; nCntMat++)
    //{
    //    //�F�����̐ݒ�
    //    pMat[nCntMat].MatD3D.Diffuse = GetObjectXInfo().Diffuse[nCntMat];

    //    //�}�e���A���̐ݒ�
    //    pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

    //    //�e�N�X�`����ݒ肷��
    //    pDevice->SetTexture(0, GetObjectXInfo().pTexture[nCntMat]);

    //    //���f���i�p�[�c�j�̕`��
    //    GetObjectXInfo().pMesh->DrawSubset(nCntMat);
    //}
    ////================================================================================================================

    //=======================================
    //�`��̒��������ɖ߂�
    //=======================================

    //�@���̒������P�ɂ���B�i�X�P�[���Ȃǂ��g�������́A�K���g���B)
    pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
    //================================================================================================================

    ////�ۑ����Ă����}�e���A����߂�
    //pDevice->SetMaterial(&matDef);

}
//==========================================================================================================

//====================================================
//���S�t���O��ݒ�
//====================================================
void CPlayer::SetDeath()
{
    if (GetUseDeath() == true)
    {
        //=================================
        //���@�\���̏I������
        //=================================
        if (m_pMagicDisp != nullptr)
        {
            m_pMagicDisp->SetUseDeath(true);
            m_pMagicDisp->CObject::SetDeath();
            m_pMagicDisp = nullptr;
        }
        //=================================================================

        //=================================
        //���@�\���E�C���h�E�̏I������
        //=================================
        if (m_pMagicWindow != nullptr)
        {
            m_pMagicWindow->SetUseDeath(true);
            m_pMagicWindow->CObject::SetDeath();
            m_pMagicWindow = nullptr;
        }
        //=================================================================

        //=================================
        //���l�\���̏I������
        //=================================
        if (m_pNumber != nullptr)
        {
            m_pNumber->SetUseDeath(true);
            m_pNumber->CObject::SetDeath();
            m_pNumber = nullptr;
        }
        //=================================================================

        //=================================
        //HP�\���̏I������
        //=================================
        if (m_pHpGauge != nullptr)
        {
            m_pHpGauge->SetUseDeath(true);
            m_pHpGauge->CObject::SetDeath();
            m_pHpGauge = nullptr;
        }
        //=================================================================

        //=================================
        //MP�\���̏I������
        //=================================
        if (m_pMpGauge != nullptr)
        {
            m_pMpGauge->SetUseDeath(true);
            m_pMpGauge->CObject::SetDeath();
            m_pMpGauge = nullptr;
        }
        //=================================================================

        //==================================
        //���f���p�[�c�̎��S�t���O�ݒ菈��
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
        //�X�R�A�ւ̃|�C���^
        //==================================
        if (m_pScore != nullptr)
        {
            m_pScore->SetUseDeath(true);
            m_pScore->SetDeath();
            m_pScore = nullptr;
        }
        //=================================================================

        //==================================
        //�O�Ղ̎��S�t���O�ݒ菈��
        //==================================
        if (m_pMeshOrbit != nullptr)
        {
            m_pMeshOrbit->SetUseDeath(true);
            m_pMeshOrbit->SetDeath();
            m_pMeshOrbit = nullptr;
        }
        //=================================================================

        //==================================
        //���b�N�I��UI�̎��S�t���O�ݒ菈��
        //==================================
        if (m_pLockOnUi != nullptr)
        {
            m_pLockOnUi->SetUseDeath(true);
            m_pLockOnUi->SetDeath();
            m_pLockOnUi = nullptr;
        }
        //=================================================================

        //==================================
        //�Q�[�W�t���[���̎��S�t���O�ݒ菈��
        //==================================
        if (m_pGaugeFrame != nullptr)
        {
            m_pGaugeFrame->SetUseDeath(true);
            m_pGaugeFrame->SetDeath();
            m_pGaugeFrame = nullptr;
        }
        //=================================================================

        //======================================
        //UI���A�N�V�����̎��S�t���O�ݒ菈��
        //======================================
        if (m_pUiReaction != nullptr)
        {
            m_pUiReaction->SetUseDeath(true);
            m_pUiReaction->SetDeath();
            m_pUiReaction = nullptr;
        }
        //=================================================================

        //======================================
        //����MP�Q�[�W�̎��S�t���O�ݒ菈��
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
//���f�����̔j��
//====================================================
void CPlayer::Unload()
{
    //���b�V���̔j��
    if (m_pMeshTemp != NULL)
    {
        m_pMeshTemp->Release();
        m_pMeshTemp = NULL;
    }

    //�}�e���A���̔j��
    if (m_pBuffMatTemp != NULL)
    {     
        m_pBuffMatTemp->Release();
        m_pBuffMatTemp = NULL;
    }

    //�e�N�X�`���̔j��
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
//�v���C���[�̐���
//====================================================
CPlayer* CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, D3DXVECTOR3 Scale)
{
    CPlayer* pPlayer = new CPlayer;                                                                             //�v���C���[�𐶐�

    bool bSuccess = pPlayer->CObject::GetCreateSuccess();
    int nIdx = 0;//���f���̃C���f�b�N�X
    if (bSuccess == true)
    {
        if (pPlayer != nullptr)
        {
            pPlayer->Init();                                                                 //����������
            pPlayer->SetUseDeath(false);                                                     //���S�t���O�𔭓����邩�ǂ�����ݒ肷��
            pPlayer->CObject::SetType(CObject::TYPE_PLAYER);                                 //�I�u�W�F�N�g�̎�ނ����߂�
            pPlayer->CObjectX::SetObjXType(CObjectX::OBJECTXTYPE_PLAYER);                    //�I�u�W�F�N�gX�̃^�C�v��ݒ�
            pPlayer->CObjectX::SetTypeNum(0);                                                //�I�u�W�F�N�gX���Ƃ̃^�C�v�ԍ���ݒ�
            pPlayer->SetReSpawnPos(pos);                                                     //���X�|�[���ʒu
            pPlayer->m_Status.nHp = m_nINIT_HP;                                              //�̗�
            pPlayer->m_Status.nMaxHp = m_nINIT_HP;                                           //�ő�̗�
            pPlayer->m_Status.nMp = m_nINIT_MP;                                              //�}�W�b�N�|�C���g
            pPlayer->m_Status.nMaxMp = m_nINIT_MP;                                           //�ő�}�W�b�N�|�C���g
            pPlayer->m_pMeshOrbit = CMeshOrbit::Create(CMeshOrbit::MESHORBITTYPE00_NORMAL);  //�O�Ճ��b�V���𐶐�
            pPlayer->SetLife(1);
            pPlayer->SetMaxLife(1);
            pPlayer->SetAutoSubLife(false);//�����I�ɑ̗͂����炷���ǂ���
            //=============================
            //UI�̐���
            //=============================
            pPlayer->m_pHpGauge = CGauge::Create(CGauge::GAUGETYPE_PLAYERHP, m_nINIT_HP, 290.0f, 12.0f, D3DXVECTOR3(141.0f, SCREEN_HEIGHT - 72.0f, 0.0f));//�̗̓Q�[�W��ݒ�
            pPlayer->m_pMpGauge = CGauge::Create(CGauge::GAUGETYPE_PLAYERMP, m_nINIT_MP, 290.0f, 12.0f, D3DXVECTOR3(141.0f, SCREEN_HEIGHT - 45.0f, 0.0f));//MP�Q�[�W��ݒ�
            pPlayer->m_pMpGauge->SetParam(m_nINIT_MP);//�����p�����[�^��MAX�ɂ���
            pPlayer->m_pGaugeFrame = CUi::Create(CUi::UITYPE_CHARACTERGAUGEFRAME, CObject2D::POLYGONTYPE01_SENTERROLLING, 400.0f, 100.0f,
                SENTER_VECTOR3 + D3DXVECTOR3(-400.0f, 300.0f, 0.0f), NULL_VECTOR3, NULL_VECTOR3, NORMAL_COL);//�Q�[�W�t���[���𐶐�
            pPlayer->m_pScore = CScore::Create(1000);//�X�R�A�����l
            pPlayer->m_pUiReaction = CUi::Create(CUi::UITYPE_FACENORMAL, CObject2D::POLYGONTYPE01_SENTERROLLING, 80.0f, 80.0f, D3DXVECTOR3(90.0f, SCREEN_HEIGHT - 60.0f, 0.0f),
                NULL_VECTOR3, NULL_VECTOR3, NORMAL_COL);//���A�N�V������UI�𐶐�
            //=====================================================================================================================================================================================

            pPlayer->SetPos(pos);                                                                               //�ʒu�̐ݒ�
            pPlayer->SetPosOld(pos);                                                                            //1f�O�̈ʒu��ݒ�
            pPlayer->SetSupportPos(pos);                                                                        //�ݒu�ʒu
            pPlayer->SetRot(rot);                                                                               //�����̐ݒ�
            pPlayer->SetScale(Scale);                                                                           //�g�嗦�̐ݒ�
            pPlayer->SetFormarScale(Scale);                                                                                  //���̊g�嗦��ݒ肷��
            pPlayer->SetRotType((CObjectX::ROTTYPE)(0));                                                        //�����̎�ނ�ݒ�
            pPlayer->SizeSet();                                                                                 //���f���p�[�c���m���ׂăT�C�Y��ݒ�
        }
    }
    else
    {//�I�u�W�F�N�g�ɋ󂫂��Ȃ������̂Ŕj������
        delete pPlayer;
        pPlayer = nullptr;
    }

	return pPlayer;
}
//==========================================================================================================

//=======================================================
//�v���C���[�Ƀ_���[�W��^����
//=======================================================
void CPlayer::SetDamage(int nDamage, int nHitStopTime)
{
    bool& bDodge = GetDodgeState();
    if (m_bInvincible == false && nDamage > 0 && bDodge == false && m_nRespawnCnt > 10)
    {//���G��Ԃ���Ȃ�������

        m_Status.nHp -= 1;
        m_pHpGauge->SetParam(m_Status.nHp);
        m_ChargeAttack.nChargeLevel = 0;//�`���[�W�����Z�b�g
        for (int nCnt = 0; nCnt < m_nMAX_PLAYERPARTS; nCnt++)
        {
            m_apModelParts[nCnt]->SetColor(COLORTYPE_RED, 10);
        }
        m_bInvincible = true;//���G��Ԃɂ���

        SetPlayerReaction(CPlayer::PLAYERREACTION_DAMAGE, 60);

        CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_DAMAGE_001);
    }
}
//==========================================================================================================

//=======================================================
//MP��ݒ肷��
//=======================================================
void CPlayer::SetMp(int nMp)
{
    if (m_pMpGauge != nullptr)
    {//MP�Q�[�W�����݂��Ă�����
        if (m_Status.nMp < m_Status.nMaxMp)
        {//MP��MAX����Ȃ����
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
//���f���Ȃ��ړ��ʂ�ݒ肷��
//=======================================================
void CPlayer::NotSetMove(bool bX, bool bY, bool bZ)
{
    D3DXVECTOR3& Move = GetMove();                                   //�ړ��ʂ��擾����
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
//�ʒu����������
//=======================================================
void CPlayer::SetComplusionPos(D3DXVECTOR3 pos)
{
    m_CompulsionPos = pos;
    m_bCompulsionPos = true;
}
//==========================================================================================================

//========================================================
//�v���C���[��j�����鏈��
//========================================================
void CPlayer::ReleasePlayer()
{
    Release();
}
//==========================================================================================================

//========================================================
//�v���C���[��j�����鏈��
//========================================================
void CPlayer::BlockCollision()
{

}
//==========================================================================================================

//========================================================
//������Ԃ̏���
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
//�R�}���h��������Z�b�g����
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
//UI���A�N�V�����̏���
//========================================================
void CPlayer::UiReactionProcess()
{
    if (m_bChengeUiReactionFlag == true)
    {
        m_nCntChengeUiReactionTime--;

        if (m_nCntChengeUiReactionTime == 0)
        {//���A�N�V���������Z�b�g����
            m_pUiReaction->SetUiType(CUi::UITYPE_FACENORMAL);
            m_bChengeUiReactionFlag = false;
        }
    }
}
//===================================================================================================================================================

//========================================================
//�ʏ�U������
//========================================================
void CPlayer::NormalAttack()
{
    D3DXVECTOR3& Pos = GetPos();
    D3DXVECTOR3 &Rot = GetRot();
    if (CManager::GetInputKeyboard()->GetRepeat(DIK_N) == true)
    {//�o���b�g����
        CBullet::Create(CBullet::BULLETTYPE_PLAYER, CBullet::BULLETTEX_PLAYER, 300,5, 40.0f, 40.0f, Pos, D3DXVECTOR3(sinf(Rot.y - D3DX_PI) * 10.0f, 0.0f, cosf(Rot.y - D3DX_PI) * 10.0f));
        m_pMpGauge->SetParam(m_Status.nMp);
    }
}
//==========================================================================================================

//========================================================
//���@�U������
//========================================================
void CPlayer::MagicAttack()
{
    D3DXVECTOR3& pos = GetPos();                                   //���f���̈ʒu�̎擾
    D3DXVECTOR3& Rot = GetRot();                                   //���f���̌����̎擾
    D3DXVECTOR3 SummonPos = NULL_VECTOR3;
    SummonPos.x = pos.x + float(rand() % 300 - 150);
    SummonPos.y = pos.y + float(rand() % 300 - 150);
    SummonPos.z = pos.z;
    //�Z�I��
    bool bSuccess = false;


    //============================================
    //���ݑI��ł��閂�@�̏���MP��\������
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
                m_Status.nMp -= m_nPLAYERUSAGEMP[m_pMagicDisp->GetMagicDispType()];//MP�������
            }
            else
            {
                //
            }
            m_nUseMagicNum = int(m_pMagicDisp->GetMagicDispType());//���ݎg�p���Ă��閂�@�ԍ����i�[
            m_pMpGauge->SetParam(m_Status.nMp);                     //MP�̃p�����[�^��ݒ�

            if (m_pMagicDisp->GetMagicDispType() == CMagicDisp::MAGICDISP00_FIRE && m_bPossibleMagic == true)
            {
                bSuccess = true;
                FireBallProcess();//�t�@�C�A�{�[���̏���
            }
            else if (m_pMagicDisp->GetMagicDispType() == CMagicDisp::MAGICDISP01_BEAM && m_Status.nMp && m_bPossibleMagic == true)
            {
                bSuccess = true;
                BeamProcess();
            }
            else if (m_pMagicDisp->GetMagicDispType() == CMagicDisp::MAGICDISP02_ICE && m_bPossibleMagic == true)
            {//�t�����[�h�A�C�X
                m_bPossibleMagic = false;                               //���@�g�p�\��Ԃ��I�t��
                m_nNextUseMagic = 120;                                  //���̖��@���g����܂�
                bSuccess = true;
            }
            else if (m_pMagicDisp->GetMagicDispType() == CMagicDisp::MAGICDISP03_THUNDER && m_bPossibleMagic == true)
            {//�T���_�[�X�p�[�N
                m_bPossibleMagic = false;                               //���@�g�p�\��Ԃ��I�t��
                bSuccess = true;
                m_nNextUseMagic = 120;                                  //���̖��@���g����܂�
            }
            else if (m_pMagicDisp->GetMagicDispType() == CMagicDisp::MAGICDISP04_SWORD && m_bPossibleMagic == true)
            {//�T�C�R�u���C�h
                PsychoBladeProcess();
                bSuccess = true;
            }
            else if (m_pMagicDisp->GetMagicDispType() == CMagicDisp::MAGICDISP05_PLANTSHIELD && m_bPossibleMagic == true)
            {//�v�����g�V�[���h
                PlantShieldProcess();
                bSuccess = true;
            }
            else if (m_pMagicDisp->GetMagicDispType() == CMagicDisp::MAGICDISP06_QUICKHEAL && m_bPossibleMagic == true)
            {//�N�C�b�N�q�[��
                m_bPossibleMagic = false;                               //���@�g�p�\��Ԃ��I�t�ɂ���
                m_nNextUseMagic = 9999;
                bSuccess = true;
            }
            else if (m_pMagicDisp->GetMagicDispType() == CMagicDisp::MAGICDISP07_CROSSBOMB && m_bPossibleMagic == true)
            {//�N���X�{��
                bSuccess = true;

                CrossBombProcess();                 //����
            }
            else if (m_pMagicDisp->GetMagicDispType() == CMagicDisp::MAGICDISP08_BLACKHOLE && m_bPossibleMagic == true)
            {//�u���b�N�z�[��
                bSuccess = true;

                BlackHoleProcess();                 //����
            }
            else if (m_pMagicDisp->GetMagicDispType() == CMagicDisp::MAGICDISP09_PHYCOBURST && m_bPossibleMagic == true)
            {//�T�C�R�o�[�X�g
                m_bPossibleMagic = false;           //���@�g�p�\��Ԃ��I�t�ɂ���
                m_nNextUseMagic = 9999;             //���ɖ��@���g�p�ł���܂ł̎���    
                bSuccess = true;

                if (m_pLockOnUi == nullptr)
                {//�Z�I������UI�𐶐�
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

    //�Z����
    if (m_bPossibleMagic == false)
    {
        m_nUseMagicTime++;//���@�g�p���Ԃ��J�E���g����


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
            //���@�g�p��Ԃ̃��Z�b�g
            ResetUseMagic();
        }
    }

}
//==========================================================================================================

//========================================================
//�����W�����v
//========================================================
void CPlayer::AdjustJump()
{
    bool& bIsLanding = GetLanding();                                 //�n�ʂɂ��邩�ǂ���
    bool& bIsWalling = GetWalling();                                 //�ǂɂ������Ă��邩�ǂ���
    bool& bIsJumping = GetJumping();                                 //�W�����v���Ă��邩�ǂ���
    D3DXVECTOR3& Move = GetMove();                                   //�ړ��ʂ��擾����
    float fRandRot = 0.0f;
    D3DXVECTOR3 Pos = GetPos();                                      //�ʒu���擾����
    D3DXVECTOR3 Rot = GetRot();                                      //�������擾����
    D3DXVECTOR3 ParticleMove = NULL_VECTOR3;
    D3DXVECTOR3 VtxMax = GetVtxMax();
    D3DXVECTOR3 VtxMin = GetVtxMin();
    if (CManager::GetInputKeyboard()->GetTrigger(DIK_SPACE) == true || CManager::GetInputJoypad()->GetTrigger(CInputJoypad::JOYKEY_A) == true)
    {//�W�����v�J�n�̃t���O���I���ɂ���i�v���X�ɂ���ƁA�n�ʂɒ������уW�����v���Ă��܂��̂Ńg���K�[�ŁI�j
        if (bIsLanding == true)
        {
            m_bPriorityMotion[MOTIONTYPE03_JUMP] = true;
            bIsJumping = true;
            m_nJumpPushCnt = 0;
            CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_JUMP_000);
        }

        if (m_bUnlockAction[ACTIONTYPE03_WALLJUMP] == true)
        {//�ǃW�����v���J������Ă�����
            if (bIsWalling == true)
            {
                if (m_bPossibleWallJump == true)
                {//�ǃW�����v���\�Ȃ�
                    m_bJudgeCommand[PLAYERCOMMAND03_WALLJUMP] = true;//�ǃW�����v�����Ă���
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
        {//�W�����v���Ƀ{�^�����������Ԃ̕������������ς��悤�ɂ���B�{�^�������������̍ő�J�E���g���Œ��_
            Move.y = m_fJump;
            m_nJumpPushCnt++;
            m_bJudgeCommand[PLAYERCOMMAND01_JUMP] = true;//�W�����v�����Ă���
        }
    }
    else
    {//�{�^����b������ɂ�����񉟂��ƁA�A���W�����v���\�ɂȂ��Ă��܂��̂Ń{�^����b�����玟�n�ʂɒ����܂ŃW�����v�o���Ȃ�����
        bIsJumping = false;
    }
}
//==========================================================================================================

//========================================================
//�Q�i�W�����v
//========================================================
void CPlayer::DoubleJump()
{
    D3DXVECTOR3& Move = GetMove();                                   //�ړ��ʂ��擾����
    bool& bIsWalling = GetWalling();                                 //�ǂɂ������Ă��邩�ǂ���
    CModel* pModel = nullptr;
    bool& bIsJumping = GetJumping();                                 //�W�����v���Ă��邩�ǂ���
    if (CManager::GetInputKeyboard()->GetTrigger(DIK_SPACE) == true || CManager::GetInputJoypad()->GetTrigger(CInputJoypad::JOYKEY_A) == true)
    {//�W�����v�J�n�̃t���O���I���ɂ���i�v���X�ɂ���ƁA�n�ʂɒ������уW�����v���Ă��܂��̂Ńg���K�[�ŁI�j
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
            m_nJumpCnt++;      //�W�����v�񐔂̃J�E���g������
            m_bPriorityMotion[MOTIONTYPE03_JUMP] = true;
            m_bJudgeCommand[PLAYERCOMMAND01_JUMP] = true;//�W�����v�����Ă���
            CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_JUMP_000);
        }

        if (m_bUnlockAction[ACTIONTYPE03_WALLJUMP] == true)
        {//�ǃW�����v���J������Ă�����
            if (bIsWalling == true)
            {
                if (m_bPossibleWallJump == true)
                {//�ǃW�����v�\�Ȃ�
                    m_bJudgeCommand[PLAYERCOMMAND03_WALLJUMP] = true;//�ǃW�����v�����Ă���
                    bIsJumping = true;
                    m_nJumpPushCnt = 0;
                    m_nJumpCnt = 1;    //�i�ǃW�����v�������ɁA�O�ɂ���ƁA�R�i�W�����v���\�ɂȂ��Ă��܂��̂ŁA�P�ŌŒ�j
                    CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_JUMP_000);
                }
            }
        }
    }

    if (CManager::GetInputKeyboard()->GetPress(DIK_SPACE) == true || CManager::GetInputJoypad()->GetPress(CInputJoypad::JOYKEY_A) == true)
    {
        if (bIsJumping == true && m_nJumpPushCnt < m_nMAX_JAMPPUSHCNT)
        {//�W�����v���Ƀ{�^�����������Ԃ̕������������ς��悤�ɂ���B�{�^�������������̍ő�J�E���g���Œ��_
            Move.y = m_fJump;
            m_nJumpPushCnt++;
        }
    }
    else
    {//�{�^����b������ɂ�����񉟂��ƁA�A���W�����v���\�ɂȂ��Ă��܂��̂Ń{�^����b�����玟�n�ʂɒ����܂ŃW�����v�o���Ȃ�����
        bIsJumping = false;
        m_nJumpPushCnt = 0;
    }
}
//===================================================================================================================================================

//========================================================
//���@�ύX����
//========================================================
void CPlayer::ChengeMagic()
{
    bool bChenge = false;//���@��I�񂾂��ǂ���
    if (m_bPossibleMagic == true && m_pMagicDisp != nullptr)
    {//���@�g�p�\��ԂȂ�
        if (CManager::GetInputKeyboard()->GetTrigger(DIK_L) == true || CManager::GetInputJoypad()->GetTrigger(CInputJoypad::JOYKEY_RB) == true)
        {
            bChenge = true;//���@��I��
            m_nNowMagicType++;//���@���C���N�������g
        }
        if (CManager::GetInputKeyboard()->GetTrigger(DIK_K) == true || CManager::GetInputJoypad()->GetTrigger(CInputJoypad::JOYKEY_LB) == true)
        {
            bChenge = true;//���@��I��
            m_nNowMagicType--;//���@���f�N�������g
        }

        if (bChenge == true)
        {
            m_bJudgeCommand[PLAYERCOMMAND10_MAGICCHENGE] = true;//���@��ύX

            CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_DECISION_001);

            if (m_nNowMagicType >= m_nMagicNum)
            {//�����Ă��閂�@�̐��̍ő�ɒB������
                m_nNowMagicType = 0;
            }
            else if (m_nNowMagicType < 0)
            {//�z��O�Ԉȉ����w�肵����
                m_nNowMagicType = m_nMagicNum - 1;
            }

            m_pMagicDisp->SetMagicDispType((CMagicDisp::MAGICDISPTYPE)(m_nMagicSlot[m_nNowMagicType]));
        }
    }
}
//===================================================================================================================================================

//========================================================
//�t�@�C�A�{�[���̏���
//========================================================
void CPlayer::FireBallProcess()
{
    D3DXVECTOR3 Pos = GetPos();                                   //���f���̈ʒu�̎擾
    D3DXVECTOR3 Rot = GetRot();                                   //���f���̌����̎擾

    CAttackPlayer::Create(CAttack::ATTACKTYPE00_FIRE,DamageResult(m_nFIREBALL_POWER), 300, Pos, D3DXVECTOR3(sinf(Rot.y - D3DX_PI) * 10.0f, 0.0f, cosf(Rot.y - D3DX_PI) * 10.0f),
        ONE_VECTOR3, -Rot, CAttack::COLLISIONTYPE00_SQUARE, D3DXVECTOR3(0.0f, 0.0f, 0.2f), true, 5);
}
//===================================================================================================================================================

//========================================================
//�r�[���̏���
//========================================================
void CPlayer::BeamProcess()
{
    D3DXVECTOR3 Pos = GetPos();                                   //���f���̈ʒu�̎擾
    D3DXVECTOR3 Rot = GetRot();                                   //���f���̌����̎擾

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
//�t�����[�h�A�C�X�̏���
//========================================================
void CPlayer::FlowerdIceProcess()
{
    D3DXVECTOR3& pos = GetPos();                                   //���f���̈ʒu�̎擾
    D3DXVECTOR3& Rot = GetRot();                                   //���f���̌����̎擾
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
//�T���_�[�o�[�X�g�̏���
//========================================================
void CPlayer::ThunderBurstProcess()
{
    D3DXVECTOR3& pos = GetPos();                                   //���f���̈ʒu�̎擾

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
//�T�C�R�u���C�h�̏���
//========================================================
void CPlayer::PsychoBladeProcess()
{
    D3DXVECTOR3& pos = GetPos();                                   //���f���̈ʒu�̎擾

    m_bPossibleMagic = false;                               //���@�g�p�\��Ԃ��I�t��
    m_nUseMagicNum = 4;                                     //�g�p���@
    m_nNextUseMagic = 300;                                  //���̖��@���g����܂�

    CAttackPlayer::Create(CAttack::ATTACKTYPE07_MAGICSWORD, DamageResult(m_nMAGICSWORD_POWER), 300, pos, NULL_VECTOR3, D3DXVECTOR3(1.0f, 2.0f, 1.0f), NULL_VECTOR3, CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, NULL_VECTOR3, false, 5);
}
//===================================================================================================================================================

//=========================================================
//�v�����g�V�[���h�̏���
//=========================================================
void CPlayer::PlantShieldProcess()
{
    D3DXVECTOR3 Pos = GetPos();                                   //���f���̈ʒu�̎擾
    D3DXVECTOR3 Rot = GetRot();                                   //���f���̌����̎擾
    CAttackPlayer* pAttackPlayer = nullptr;                       //�v���C���[�̍U������p

    pAttackPlayer = CAttackPlayer::Create(CAttack::ATTACKTYPE12_VINE, DamageResult(m_nPLANTSHIELD_POWER), 450, D3DXVECTOR3(Pos.x - 70.0f, Pos.y, Pos.z), NULL_VECTOR3,
        D3DXVECTOR3(1.0f, 0.0f, 1.0f) , NULL_VECTOR3, CAttack::COLLISIONTYPE02_TRUERECTANGLEXY,NULL_VECTOR3, false, 20);

    pAttackPlayer->SetVineDelBulletNum(20);//������e�̐����Q�O�ɐݒ�

    pAttackPlayer = CAttackPlayer::Create(CAttack::ATTACKTYPE12_VINE, DamageResult(m_nPLANTSHIELD_POWER), 450,D3DXVECTOR3(Pos.x + 70.0f,Pos.y,Pos.z),NULL_VECTOR3,
        D3DXVECTOR3(1.0f,0.0f,1.0f), NULL_VECTOR3, CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, NULL_VECTOR3, false, 20);

    pAttackPlayer->SetVineDelBulletNum(20);//������e�̐����Q�O�ɐݒ�


}
//===================================================================================================================================================

//========================================================
//�N�C�b�N�q�[���̏���
//========================================================
void CPlayer::QuickHealProcess()
{

    //���l�F�񕜂ɕK�v��MP������Ȃ��Ƃ����������̏����͌Ă΂�Ȃ��B�K�v��MP���S�O�������Ƃ��A���݂�MP���S�T�������Ƃ��A�T�Ŏ~�߂����̂ŁA�񕜒��ɁA
    //�K�vMP�����݂�MP���؂����Ƃ��A�Ō�̉񕜂Ƃ����t���O�𔭓����A�񕜂�����������A���̏������I��点��B

    if (m_Status.nMp < m_nPLAYERUSAGEMP[CMagicDisp::MAGICDISP06_QUICKHEAL])
    {
        m_Heal.bLastHeal = true;
    }

   if (CManager::GetInputKeyboard()->GetPress(DIK_M) == true || CManager::GetInputJoypad()->GetPress(CInputJoypad::JOYKEY_B) == true)
   {//���@�{�^���������Ă���Ԃ����񕜃J�E���g��i�߂�
       m_Status.nMp--;  //MP�����������
       m_Heal.nHealCnt++; //�񕜂ɕK�v��MP��ۑ�
       m_Heal.bState = true;//�񕜏�Ԃɂ���
       m_pMpGauge->SetParam(m_Status.nMp);

       //�N�C�b�N�q�[���̉��o
       QucikHealStaging();

       if (m_Heal.nHealCnt == 1)
       {
           CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_CHARGE_001);
       }

       if (m_Heal.nHealCnt % m_nQUICKHEALONEMP == 0)
       {
           m_Status.nHp++;     //�̗͂𑝂₷
           CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_HEAL);
           m_pHpGauge->SetParam(m_Status.nHp);
           if (m_Heal.bLastHeal == true)
           {//������MP�ŉ񕜂ł���̂��Ō�Ȃ�
               CManager::GetSound()->StopSound(CSound::SOUND_LABEL_SE_CHARGE_001);
               m_Heal.bState = false;
               m_Heal.nHealCnt = 0;
               m_Heal.bLastHeal = false;

               //���@�g�p��Ԃ̃��Z�b�g
               ResetUseMagic();
           }
       }
   }
   else
   {//�{�^����b������񕜃J�E���g�����Z�b�g
       m_Heal.bState = false;
       m_Heal.nHealCnt = 0;
       m_Heal.bLastHeal = false;
       CManager::GetSound()->StopSound(CSound::SOUND_LABEL_SE_CHARGE_001);
       //���@�g�p��Ԃ̃��Z�b�g
       ResetUseMagic();
   }
}
//===================================================================================================================================================

//========================================================
//�N�C�b�N�q�[���̉��o
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
    {//�񕜊����p�[�e�B�N��������
        CParticle::SummonParticle(CParticle::TYPE04_HEAL, 50, 30, 40.0f, 40.0f, 100, 10, false, Pos + D3DXVECTOR3(0.0f, Size.y / 2, 0.0f), NORMAL_COL, true);
    }
}
//===================================================================================================================================================

//========================================================
//�N���X�{���̏���
//========================================================
void CPlayer::CrossBombProcess()
{
    CAttackPlayer* pAttackPlayer = nullptr;
    D3DXVECTOR3& Pos = GetPos();
    D3DXVECTOR3 Size = GetSize();

    pAttackPlayer = CAttackPlayer::Create(CAttack::ATTACKTYPE13_CROSSBOMB, DamageResult(m_nCROSSBOMB_POWER), 120, Pos + D3DXVECTOR3(0.0f, Size.y / 2, 0.0f), NULL_VECTOR3, ONE_VECTOR3 * 2, NULL_VECTOR3,
        CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, NULL_VECTOR3, true, 5);
    pAttackPlayer->SetDelayDeath(true);//���S�t���O��x�点��
}
//===================================================================================================================================================

//========================================================
//�u���b�N�z�[���̏���
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
//�T�C�R�o�[�X�g�̏���
//========================================================
void CPlayer::PhycoBurstProcess()
{
    float fMoveX = 0.0f;                                            //X�����̈ړ���
    float fMoveY = 0.0f;                                            //Y�����̈ړ���
    bool bMove = false;                                             //�ړ����Ă��邩�ǂ���
    CAttackPlayer* pAttackPlayer = nullptr;
    if (m_pLockOnUi != nullptr)
    {
        if (CManager::GetInputKeyboard()->GetPress(DIK_M) == true || CManager::GetInputJoypad()->GetPress(CInputJoypad::JOYKEY_B) == true)
        {
            m_nStopMoveCnt = 2;//���̃J�E���g���O���ゾ�Ɠ����Ȃ�

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
                bMove = true;//�ړ����
            }
            else
            {
                bMove = false;//�ҋ@���
            }

            if (bMove == true)
            {//���b�N�I��UI�𓮂���
                m_pLockOnUi->GetPos().x += sinf(atan2f(fMoveX, fMoveY)) * 10.0f;
                m_pLockOnUi->GetPos().y += cosf(atan2f(fMoveX, fMoveY)) * 10.0f;
            }
        }
        else
        {//���@�{�^���𗣂�����

            //���b�N�I�����Ă���ʒu�ɔ������N����
            pAttackPlayer = CAttackPlayer::Create(CAttack::ATTACKTYPE16_PHYCOBURST, DamageResult(m_nPHYCOBURST_POWER), 45, m_pLockOnUi->GetPos(), NULL_VECTOR3, ONE_VECTOR3, NULL_VECTOR3,
                CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, ONE_VECTOR3 * 0.01f, false, 3);

            //���b�N�I��UI������
            m_pLockOnUi->SetUseDeath(true);
            m_pLockOnUi->SetDeath();
            m_pLockOnUi = nullptr;

            ResetUseMagic();//���@�g�p��Ԃ̃��Z�b�g
        }
    }

}
//===================================================================================================================================================

//========================================================
//���@�g�p��Ԃ����Z�b�g����
//========================================================
void CPlayer::ResetUseMagic()
{
    m_bPossibleMagic = true;//���@�g�p��Ԃ��ǂ���
    m_nUseMagicNum = 0;     //���ݎg�p���Ă��閂�@
    m_nUseMagicTime = 0;    //���@���g�p���Ă��鎞��
    m_nNextUseMagic = 0;    //���̖��@���g����܂ł̎���
}
//===================================================================================================================================================

//========================================================
//����A�N�V����
//========================================================
void CPlayer::DodgeAction(float fMoveX, float fMoveY)
{
    bool& bDodge = GetDodgeState();
    D3DXVECTOR3& Pos = GetPos();
    D3DXVECTOR3 RandMove = NULL_VECTOR3;
    D3DXVECTOR3& Move = GetMove();                                   //�ړ��ʂ��擾����
    CParticle* pParticle = nullptr;    //�p�[�e�B�N���擾�p
    bool& bIsLanding = GetLanding();                                //�n�ʂɂ��邩�ǂ���
    bool& bIsJumping = GetJumping();                                //�W�����v���Ă��邩�ǂ���
    bool& bIsWalling = GetWalling();                                //�ǂɂ������Ă��邩�ǂ���
    float fRandRot = 0.0f;
    float fRandSpeed = 0.0f;
    //=====================================
    //������d�l�\�ɂȂ�܂ł̏���
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
            pParticle->SetGravity(-0.2f);//�d�͂�ݒ�
            pParticle = nullptr;
            m_Dodge.bPossible = true;
            m_Dodge.nRevivalCnt = 0;
        }
    }
    //===================================================================================================================================================

    //BYTE rtValue = CManager::GetInputJoypad()->GetTrigger();
    //if (rtValue > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
    //======================================================
    //�����Ԕ�������
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
    //�����Ԓ��̏���
    //======================================================
    if (bDodge == true)
    {
       m_Dodge.nStateCnt++;

        if (bIsLanding == true)
        {//�n�ʂɒ����Ă�����
            m_SaveMove.y = 0.0f;
        }

        if (m_Dodge.nStateCnt == 1)
        {//�����ԂɂȂ����u�Ԃɔ���
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
            m_SaveMove = Move * 4.2f;//���݂̈ړ��ʂ�ۑ����A�Q�{�ɂ���
            bIsJumping = false;
            Move = NULL_VECTOR3;     //���̈ړ��ʂ��O�ɂ���i�W�����v���ɉ�������ꍇ�A����I�����ɃW�����v�������̈ړ��ʂ����f����ĕςɌ����邽�߁j
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
        m_bJudgeCommand[PLAYERCOMMAND02_DASH] = true;//�_�b�V�������Ă���
    }
    //===================================================================================================================================================

}
//===================================================================================================================================================

//==========================================================================
//�v���C���[�̃��f���p�[�c���ׂăT�C�Y��ݒ肷��
//==========================================================================
void CPlayer::SizeSet()
{
    D3DXVECTOR3 VtxMax = NULL_VECTOR3;                   //�ő咸�_����p
    D3DXVECTOR3 VtxMin = NULL_VECTOR3;                   //�ŏ����_����p
    D3DXVECTOR3& PlayerOriginVtxMax = GetOriginVtxMax(); //�v���C���[�̌��̍ő咸�_
    D3DXVECTOR3& PlayerOriginVtxMin = GetOriginVtxMin(); //�v���C���[�̌��̍ŏ����_
    D3DXVECTOR3& PlayerVtxMax = GetVtxMax();             //�v���C���[�̍ő咸�_
    D3DXVECTOR3& PlayerVtxMin = GetVtxMin();             //�v���C���[�̍ŏ����_
    D3DXVECTOR3& PlayerScale = GetScale();               //�v���C���[�̊g�嗦
    D3DXVECTOR3 PlayerSize = NULL_VECTOR3;              //�v���C���[�̃T�C�Y
    for (int nCntParts = 0; nCntParts < m_nMAX_PLAYERPARTS; nCntParts++)
    {
        VtxMax = m_apModelParts[nCntParts]->GetVtxMax();
        VtxMin = m_apModelParts[nCntParts]->GetVtxMin();

        //====================================================
        //���_���W���r���ă��f���̍ŏ��l�ő�l���擾
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
    //��̏����ŏo�����ŏ��ő�̒��_���g�嗦�Ōv�Z������
    //==========================================================
    PlayerVtxMax.x = PlayerOriginVtxMax.x * PlayerScale.x;
    PlayerVtxMax.y = PlayerOriginVtxMax.y * PlayerScale.y;
    PlayerVtxMax.z = PlayerOriginVtxMax.z * PlayerScale.z;
    PlayerVtxMin.x = PlayerOriginVtxMin.x * PlayerScale.x;
    PlayerVtxMin.y = PlayerOriginVtxMin.y * PlayerScale.y;
    PlayerVtxMin.z = PlayerOriginVtxMin.z * PlayerScale.z;
    //================================================================================================================================================

    //==========================================================
    //�v���C���[�̃T�C�Y
    //==========================================================
    PlayerSize = PlayerVtxMax - PlayerVtxMin;
    SetSizeAnother(PlayerSize);//�T�C�Y��ݒ肷��
    //================================================================================================================================================
}
//===================================================================================================================================================

//============================================================
//�O�Ղ̏���
//============================================================
void CPlayer::MeshOrbitProcess()
{
    D3DXVECTOR3 Pos = GetPos();//�ʒu
    if (m_pMeshOrbit != nullptr)
    {
        m_pMeshOrbit->SetPos(Pos);
        m_pMeshOrbit->SetOffSetPos(m_apModelParts[10]->GetWorldPos());
    }
}
//===================================================================================================================================================

//============================================================
//���f���p�[�c�̃��[�h����
//============================================================
void CPlayer::LoadModelParts()
{
    FILE* pFile = nullptr;

    pFile = fopen("data\\TEXTFILE\\PlayerSet.txt", "r");//�t�@�C���ǂݍ���

    int nNumModel = 0;             //���f���p�[�c�̐�
    int nType = 0;                 //���f���̎��
    int nParent = 0;               //�e�̔ԍ�
    int nCntModelFileName = 0;         //���f���̃t�@�C������ǂݍ��񂾉񐔂��J�E���g
    char aString[128] = {};        //���������p
    char aModelFileName[m_nMAX_PLAYERPARTS][128] = {};//���f���t�@�C��������p
    D3DXVECTOR3 Rot = NULL_VECTOR3;//��������p
    D3DXVECTOR3 Pos = NULL_VECTOR3;//�ʒu����p
    if (pFile != NULL)
    {//�t�@�C�����J����Ă�����
        while (1)
        {
            fscanf(pFile, "%s", &aString[0]);

            if (strcmp(&aString[0], "ENDSCRIPT") == 0)
            {//�t�@�C���ǂݍ��݂��I��点��
                fclose(pFile);
                break;
            }
            else if (aString[0] == '#')
            {//���̍s���X�L�b�v
                fgets(&aString[0], 100, pFile);
            }
            else if (strcmp(&aString[0], "NUM_MODEL") == 0)
            {//���f���̎�ސ�
                fscanf(pFile, "%d", &nNumModel);
            }
            else if (strcmp(&aString[0], "MODEL_FILENAME") == 0)
            {//���[�h���郂�f���̐����p�[�c���𒴂��Ȃ����
                fscanf(pFile, "%s", &aString[0]);                           //�C�R�[���ǂݎ��
                fscanf(pFile, "%s", &aModelFileName[nCntModelFileName][0]); //���f���t�@�C���l�[���ǂݎ��
                nCntModelFileName++;
            }
            else if (strcmp(&aString[0], "MODELSET") == 0)
            {//���f�������Z�b�g
                while (1)
                {
                    fscanf(pFile, "%s", &aString[0]);
                    if (aString[0] == '#')
                    {//���̍s���X�L�b�v
                        fgets(&aString[0], 100, pFile);
                    }
                    else if (strcmp(&aString[0], "TYPE") == 0)
                    {//���f���̎�ނ����߂�
                        fscanf(pFile, "%d", &nType);
                    }
                    else if (strcmp(&aString[0], "IDX") == 0)
                    {//���f���̐e�����߂�
                        fscanf(pFile, "%d", &nParent);
                    }
                    else if (strcmp(&aString[0], "POS") == 0)
                    {//�ʒu
                        fscanf(pFile, "%f%f%f", &Pos.x, &Pos.y, &Pos.z);
                    }
                    else if (strcmp(&aString[0], "ROT") == 0)
                    {//����
                        fscanf(pFile, "%f%f%f", &Rot.x, &Rot.y, &Rot.z);
                    }
                    else if (strcmp(&aString[0], "ENDMODELSET") == 0)
                    {//�p�[�c��ݒ�
                        if (nParent >= 0)
                        {//�e������
                            m_apModelParts[nType] = CModelParts::Create(&aModelFileName[nType][0], Pos, Rot);
                            m_apModelParts[nType]->SetParentParts(&m_apModelParts[nParent]->GetMatrixWorld());
                        }
                        else
                        {//�e�����Ȃ�
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
//���[�V��������
//============================================================
void CPlayer::MotionProcess()
{
    D3DXVECTOR3 NowPos = NULL_VECTOR3;//���݂̈ʒu����p
    D3DXVECTOR3 NowRot = NULL_VECTOR3;//���݂̌�������p

    D3DXVECTOR3 NextPos = NULL_VECTOR3;//���̈ʒu����p
    D3DXVECTOR3 NextRot = NULL_VECTOR3;//���̌�������p

    D3DXVECTOR3 DifferencePos = NULL_VECTOR3;//�ʒu�̍���
    D3DXVECTOR3 DifferenceRot = NULL_VECTOR3;//�����̍���

    D3DXVECTOR3 DecisionPos = NULL_VECTOR3;//���肷��ʒu
    D3DXVECTOR3 DecisionRot = NULL_VECTOR3;//���肷�����

    m_nMotionOld = m_nNowMotion;
    m_nNowMotion = m_nNextMotion;
    if (m_nNowMotion != m_nMotionOld)
    {//���[�V������1f�O�ƈقȂ�ꍇ�A�t���[�����ƃL�[�J�E���g�����Z�b�g
        m_nCntFrame = 0;
        m_aMotion[m_nMotionOld].nCntKey = 0;
    }
    int nNowMotion = m_nNowMotion;              //���݂̃��[�V����
    int nNowKey = m_aMotion[nNowMotion].nCntKey;//���݂̃L�[
    int nNextKey = (nNowKey + 1) % m_aMotion[nNowMotion].nNumKey;//���̃L�[�i�ő�L�[���R�ŁA���݂̃L�[���Q�������ꍇ�j�i���݂̃L�[(2) + 1 ) % (�ő�L�[(3)) ���u�O�v)
    float fRatioFrame = (float)(m_nCntFrame) / (float)(m_aMotion[nNowMotion].aKeySet[nNowKey].nFrame);//���݂̃t���[���ƍő�t���[���̊��������߂�
    for (int nCntParts = 0; nCntParts < m_nMAX_PLAYERPARTS; nCntParts++)
    {
        //���݂̈ʒu�A����
        NowPos = m_aMotion[nNowMotion].aKeySet[nNowKey].aKey[nCntParts].Pos;
        NowRot = m_aMotion[nNowMotion].aKeySet[nNowKey].aKey[nCntParts].Rot;

        //���̈ʒu�A����
        NextPos = m_aMotion[nNowMotion].aKeySet[nNextKey].aKey[nCntParts].Pos;
        NextRot = m_aMotion[nNowMotion].aKeySet[nNextKey].aKey[nCntParts].Rot;

        //�����̈ʒu�A����
        DifferencePos = NextPos - NowPos;
        DifferenceRot = NextRot - NowRot;

        //��L�̏����𓥂܂��ăp�[�c�̈ʒu�A������ݒ�
        DecisionPos = (DifferencePos * fRatioFrame) + m_apModelParts[nCntParts]->GetOffSetSupportPos() + NowPos;
        DecisionRot = DifferenceRot * fRatioFrame + NowRot;
        //�ݒ肷��
        m_apModelParts[nCntParts]->SetOffSetPos(DecisionPos);
        m_apModelParts[nCntParts]->SetOffSetRot(DecisionRot);
    }

    m_nCntFrame++;

    if (m_nCntFrame >= m_aMotion[nNowMotion].aKeySet[nNowKey].nFrame)
    {//�t���[�������݂̃L�[���̍ő�ɒB������E�E�E
         m_aMotion[nNowMotion].nCntKey++;//�L�[�����ɐi�߂�
         m_nCntFrame = 0;//�t���[���J�E���g���O�ɂ���

        if (m_aMotion[nNowMotion].nCntKey >= m_aMotion[nNowMotion].nNumKey)
        {// �L�[�̍ő吔�𒴂�����E�E�E
            m_aMotion[nNowMotion].nCntKey = 0;

            if (m_aMotion[nNowMotion].bLoop == false)
            {//���[�v���Ȃ��Ȃ�E�E�E
                for (int nCnt = 0; nCnt < m_nMAX_MOTION; nCnt++)
                {
                    m_bPriorityMotion[nCnt] = false;//�S�Ẵ��[�V�����D���Ԃ�����
                }
                m_nNextMotion = 0;
            }
        }
    }
}
//===================================================================================================================================================

//============================================================
//���[�V�����������[�h����
//============================================================
void CPlayer::LoadMotion()
{
    FILE* pFile;//�t�@�C���|�C���^

    //=========================================
    //���[�V�������̓ǂݍ��݂Ŏg���ϐ�
    //=========================================
    char aString[100] = {};  //������������
    int nNumMotion = 0;      //���[�V�����̐�
    int nNumKeySet = 0;      //�L�[�Z�b�g�̐�
    int nNumKey = 0;         //�L�[�̐�
    int nLoop = 0;           //���[�v���邩�ǂ����𐔒l�œǂݎ��
    //====================================================================

    //�t�@�C�����J��
    pFile = fopen("data\\TEXTFILE\\PlayerMotion2.txt", "r");

    if (pFile != NULL)
    {//�t�@�C�����J����Ă�����
        while (1)
        {
            fscanf(pFile, "%s", &aString[0]);

            if (strcmp(&aString[0], "END_SCRIPT") == 0)
            {//�t�@�C���ǂݍ��݂��I��点��
                fclose(pFile);
                break;
            }
            else if (aString[0] == '#')
            {//���̍s���X�L�b�v
                fgets(&aString[0], 100, pFile);
            }
            else if (strcmp(&aString[0], "MOTIONSET") == 0)
            {//�t�@�C���ǂݍ��݂��I��点��
                while (1)
                {
                    fscanf(pFile, "%s", &aString[0]);

                    if (aString[0] == '#')
                    {//���̍s���X�L�b�v
                        fgets(&aString[0], 100, pFile);
                    }
                    else if (strcmp(&aString[0], "LOOP") == 0)
                    {//���[�v���邩�ǂ���
                        fscanf(pFile, "%s", &aString[0]);//�C�R�[��
                        fscanf(pFile, "%d", &nLoop);

                        if (nLoop == 0)
                        {//���[�v���Ȃ�
                            m_aMotion[nNumMotion].bLoop = false;
                        }
                        else
                        {//���[�v����
                            m_aMotion[nNumMotion].bLoop = true;
                        }
                    }
                    else if (strcmp(&aString[0], "NUM_KEY") == 0)
                    {//�L�[�̐�
                        fscanf(pFile, "%s", &aString[0]);//�C�R�[��
                        fscanf(pFile, "%d", &m_aMotion[nNumMotion].nNumKey);
                    }
                    else if (strcmp(&aString[0], "KEYSET") == 0)
                    {//�L�[���ݒ�
                        while (1)
                        {
                            fscanf(pFile, "%s", &aString[0]);

                            if (aString[0] == '#')
                            {//���̍s���X�L�b�v
                                fgets(&aString[0], 100, pFile);
                            }
                            else if (strcmp(&aString[0], "FRAME") == 0)
                            {//�t���[����
                                fscanf(pFile, "%s", &aString[0]);//�C�R�[��
                                fscanf(pFile, "%d", &m_aMotion[nNumMotion].aKeySet[nNumKeySet].nFrame);
                            }
                            else if (strcmp(&aString[0], "KEY") == 0)
                            {//�L�[�f�[�^�ݒ�
                                while (1)
                                {
                                    fscanf(pFile, "%s", &aString[0]);

                                    if (aString[0] == '#')
                                    {//���̍s���X�L�b�v
                                        fgets(&aString[0], 100, pFile);
                                    }
                                    else if (strcmp(&aString[0], "POS") == 0)
                                    {//�ʒu
                                        fscanf(pFile, "%s", &aString[0]);//�C�R�[��
                                        fscanf(pFile, "%f%f%f", &m_aMotion[nNumMotion].aKeySet[nNumKeySet].aKey[nNumKey].Pos.x,
                                            &m_aMotion[nNumMotion].aKeySet[nNumKeySet].aKey[nNumKey].Pos.y,
                                            &m_aMotion[nNumMotion].aKeySet[nNumKeySet].aKey[nNumKey].Pos.z);
                                    }
                                    else if (strcmp(&aString[0], "ROT") == 0)
                                    {//����
                                        fscanf(pFile, "%s", &aString[0]);//�C�R�[��
                                        fscanf(pFile, "%f%f%f", &m_aMotion[nNumMotion].aKeySet[nNumKeySet].aKey[nNumKey].Rot.x,
                                            &m_aMotion[nNumMotion].aKeySet[nNumKeySet].aKey[nNumKey].Rot.y,
                                            &m_aMotion[nNumMotion].aKeySet[nNumKeySet].aKey[nNumKey].Rot.z);
                                    }
                                    else if (strcmp(&aString[0], "END_KEY") == 0)
                                    {//�L�[�I��
                                        nNumKey++;
                                        break;
                                    }
                                }
                            }
                            else if (strcmp(&aString[0], "END_KEYSET") == 0)
                            {//�L�[�Z�b�g�I��
                                nNumKeySet++;
                                nNumKey = 0;
                                break;
                            }
                        }
                    }
                    else if (strcmp(&aString[0], "END_MOTIONSET") == 0)
                    {//���[�V�����Z�b�g�I��
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
//���[�V�����D��x��ݒ肷��
//========================================================
void CPlayer::MotionPriorityProcess()
{
    if (m_bPriorityMotion[MOTIONTYPE00_NUTRAL] == true)
    {//�����I�Ɉړ���Ԃɂ���
        m_nNextMotion = MOTIONTYPE00_NUTRAL;
    }
    if (m_bPriorityMotion[MOTIONTYPE01_RUNNING] == true)
    {//�����I�Ɉړ���Ԃɂ���i�D��x�{�P�j
        m_nNextMotion = MOTIONTYPE01_RUNNING;
    }
    if (m_bPriorityMotion[MOTIONTYPE03_JUMP] == true)
    {//�����I�ɃW�����v��Ԃɂ���i�D��x�{�Q�j
        m_nNextMotion = MOTIONTYPE03_JUMP;
    }
    if (m_bPriorityMotion[MOTIONTYPE04_LANDING] == true)
    {//�����I�ɒ��n��Ԃɂ���i�D��x�{�R�j
        m_nNextMotion = MOTIONTYPE04_LANDING;
    }
    if (m_bPriorityMotion[MOTIONTYPE02_ATTACK] == true)
    {//�����I�ɍU����Ԃɂ���i�D��x�{�S�j
        m_nNextMotion = MOTIONTYPE02_ATTACK;
    }
}
//===================================================================================================================================================

//========================================================
//�v���C���[�̏���\������
//========================================================
void CPlayer::DispPlayerInfo()
{
    D3DXVECTOR3 Pos = GetPos();//�ʒu���擾
    D3DXVECTOR3 Rot = GetRot();//�������擾
    CManager::GetDebugText()->PrintDebugText("�v���C���[�̈ʒu�F%f %f %f\n", Pos.x, Pos.y, Pos.z);
    CManager::GetDebugText()->PrintDebugText("�v���C���[�̌����F%f %f %f", Rot.x, Rot.y, Rot.z);
    CManager::GetDebugText()->PrintDebugText("�v���C���[��HP�F%d\n",m_Status.nHp);
    CManager::GetDebugText()->PrintDebugText("�v���C���[��MP�F%d\n",m_Status.nMp);
}
//===================================================================================================================================================

//========================================================
//�V�������@���J�����鏈��
//========================================================
void CPlayer::SetUnlockMagic(int nMagicNum, D3DXVECTOR3 Pos, bool bUseSignBoard)
{
    if (m_nMagicNum < m_nMAX_MAGICNUM)
    {//���@������������ɒB���Ă��Ȃ����
        m_nMagicSlot[m_nMagicNum] = nMagicNum;
        m_bUnlockMagic[nMagicNum] = true;//���@���J����Ԃɂ���


        if (m_nMagicNum == 0)
        {//����K�����̏���
            m_pMagicWindow = CMagicWindow::Create(100.0f, 100.0f, D3DXVECTOR3(SENTER_VECTOR3.x, SENTER_VECTOR3.y + 300.0f, 0.0f));//���@�\���E�C���h�E�𐶐�
            m_pMagicDisp = CMagicDisp::Create(CMagicDisp::MAGICDISPTYPE(nMagicNum), 100.0f, 100.0f, D3DXVECTOR3(SENTER_VECTOR3.x, SENTER_VECTOR3.y + 300.0f, 0.0f));//���@�\���𐶐�

            if (bUseSignBoard == true && CDifficulty::GetDifficultyNum() != CDifficulty::DIFFICULTYTYPE_BOSSRUSH)
            {
                CSignBoard::Create(CSignBoard::SIGNBOARDTYPE00_NORMAL, CSignBoard::SIGNBOARDMANUALTYPE09_MAGIC,
                    Pos, ONE_VECTOR3 * 7, NULL_VECTOR3);
            }
            m_pUsageMpGauge = CGauge::Create(CGauge::GAUGETYPE_PLAYERUSAGEMP, m_nINIT_MP, 290.0f, 12.0f, D3DXVECTOR3(141.0f, SCREEN_HEIGHT - 45.0f, 0.0f));//MP�Q�[�W��ݒ�
        }

        if (m_nMagicNum == 1)
        {//�Q�ڂ̖��@�K�����̏���
            if (bUseSignBoard == true && CDifficulty::GetDifficultyNum() != CDifficulty::DIFFICULTYTYPE_BOSSRUSH)
            {
                CSignBoard::Create(CSignBoard::SIGNBOARDTYPE00_NORMAL, CSignBoard::SIGNBOARDMANUALTYPE10_MAGICCHENGE,
                    Pos, ONE_VECTOR3 * 7, NULL_VECTOR3);
            }
        }

        //���@���������C���N�������g
        m_nMagicNum++;
    }
}
//===================================================================================================================================================

//========================================================
//�v���C���[�̃��X�|�[�������𔭓�����
//========================================================
void CPlayer::ActivePlayerRespawn()
{
    D3DXVECTOR3& Pos = GetPos();
    D3DXVECTOR3& PosOld = GetPosOld();
    bool& bIsLanding = GetLanding();
    bool& bIsWalling = GetWalling();

    bIsLanding = false;
    bIsWalling = false;
    //�ʒu�����X�|�[���ʒu�Ɉړ�
    Pos = m_ReSpawnPos;
    PosOld = m_ReSpawnPos;

    //�v���C���[�Ƀ_���[�W��^����
    SetDamage(1,60);
}
//===================================================================================================================================================

//========================================================
//���点��悤�̊�����t����
//========================================================
void CPlayer::SetPlayerSlideInertia(float fInertia)
{
    float& fGetInertia = GetInertia();

    m_fSlideInertia = fInertia;//�����𒲐�����
    m_fReturnInertia = 0.5f - m_fSlideInertia;//���̊����Ƃ̍��������߂�
    m_fCorrectionInertia = 1.0f;//���������p
}
//===================================================================================================================================================

//========================================================
//�񕜗ʂ����蓖�Ă�
//========================================================
void CPlayer::SetHeal(int nHeal, D3DXCOLOR col, float fWidth, float fHeight)
{

    m_Status.nHp+= nHeal;     //�̗͂𑝂₷

    if (m_Status.nHp > m_Status.nMaxHp)
    {
        m_Status.nHp = m_Status.nMaxHp;
    }
    m_pHpGauge->SetParam(m_Status.nHp);

    CDamage::Create(nHeal, GetPos(), col, fWidth, fHeight);
}
//===================================================================================================================================================

//========================================================
//�v���C���[�̃��A�N�V������ݒ肷��
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

    m_nCntChengeUiReactionTime = nReactionTime;//���A�N�V������ς��鎞�Ԃ�ݒ肷��
    m_bChengeUiReactionFlag = true;            //���A�N�V������ς���t���O��ON�ɂ���
}
//===================================================================================================================================================

//========================================================
//�v���C���[�̊����̏���
//========================================================
void CPlayer::PlayerInertiaProcess()
{
    float& fInertia = GetInertia();

    if (m_fCorrectionInertia > 0.01f)
    {//�����̒����ʂ�0.01f����Ȃ�
        m_State.SlideState.bState = true;
    }
    else
    {//���̑����ɖ߂肻���ɂȂ�����
        m_State.SlideState.bState = false;
    }

    if (m_State.SlideState.bState == true)
    {//�����Ԃ�������

        m_fCorrectionInertia += (0.0f - m_fCorrectionInertia) * 0.05f;//�߂��ʂɊ|����{��
        fInertia = m_fSlideInertia + m_fReturnInertia * (1.0f - m_fCorrectionInertia);//���X�ɖ߂��Ă���
    }
    else
    {//�ʏ��ԂȂ�
        fInertia = 0.5f;
    }
}
//===================================================================================================================================================

//========================================================
//�ǃW�����v��}�����鏈��
//========================================================
void CPlayer::WallJumpBindProcess()
{
    if (m_nWallJumpBindTime > 0)
    {//�ǃW�����v�������Ԃ��O���傫�����
        m_nWallJumpBindTime--;
        m_bPossibleWallJump = false;
    }
    else
    {//����ȊO
        m_bPossibleWallJump = true;
    }
}
//===================================================================================================================================================

//========================================================
//�ǃW�����v�̃p�[�e�B�N�����������鏈��
//========================================================
void CPlayer::WallJumpParticleProcess()
{
    D3DXVECTOR3 ParticleMove = NULL_VECTOR3;
    float fRandRot = CCalculation::CalculationRandVecXY();

    ParticleMove.x = sinf(fRandRot) * 4.0f;
    ParticleMove.y = 5.0f;
    ParticleMove.z = cosf(fRandRot) * 4.0f;
    D3DXVECTOR3 Pos = GetPos();                                      //�ʒu���擾����
    D3DXVECTOR3 Rot = GetRot();                                      //�������擾����

    CParticle::Create(CParticle::TYPE00_NORMAL, 30, 20.0f, 20.0f,
        D3DXVECTOR3(Pos.x + cosf(Rot.y) * (GetSize().x * 0.5f), Pos.y, Pos.z + sinf(Rot.y) * (GetSize().x * 0.5f)),
        ParticleMove, NORMAL_COL, true);

}
//===================================================================================================================================================

//========================================================
//�G�Ƃ̓����蔻����s��
//========================================================
void CPlayer::CollisionEnemy()
{
    //==================================
    //�I�u�W�F�N�gX��������擾
    //==================================
    D3DXVECTOR3 Pos = GetPos();                      //�I�u�W�F�N�g�̈ʒu���擾
    D3DXVECTOR3 VtxMax = GetVtxMax();                //�I�u�W�F�N�g�̍ő咸�_���擾
    D3DXVECTOR3 VtxMin = GetVtxMin();                //�I�u�W�F�N�g�̍ŏ����_���擾
    D3DXVECTOR3 ComparisonPos = NULL_VECTOR3;                  //�v���C���[�̈ʒu����p
    D3DXVECTOR3 ComparisonVtxMax = NULL_VECTOR3;               //����Ώۂ̍ő咸�_
    D3DXVECTOR3 ComparisonVtxMin = NULL_VECTOR3;               //����Ώۂ̍ŏ����_
    CObject* pObj = nullptr;                                   //�I�u�W�F�N�g�擾�p
    CObjectX* pObjX = nullptr;                                 //�I�u�W�F�N�gX
    bool bCollision = false;                                   //�����蔻��
    //===========================================================

    //==================================
    //�G�̓����蔻��
    //==================================
    for (int nCntPriority = 0; nCntPriority < m_nMAXPRIORITY; nCntPriority++)
    {

        pObj = GetTopObject(nCntPriority);//�g�b�v�I�u�W�F�N�g���擾
        while (pObj != nullptr)
        {
            //���̃I�u�W�F�N�g���i�[
            CObject* pNext = pObj->GetNextObject();

            //��ނ̎擾�i�G�Ȃ瓖���蔻��j
            CObject::TYPE type = pObj->GetType();

            if (type == CObject::TYPE_ENEMY || type == CObject::TYPE_BOSS)
            {
                pObjX = (CObjectX*)pObj;

                //�I�u�W�F�N�g�̎�ނ��G��������
                ComparisonPos = pObjX->GetPos();                   //�ʒu���擾
                ComparisonVtxMax = pObjX->GetVtxMax();             //�ő咸�_���擾
                ComparisonVtxMin = pObjX->GetVtxMin();             //�ŏ����_���擾

                bCollision = CCollision::CollisionSquare(Pos, VtxMax, VtxMin, ComparisonPos, ComparisonVtxMax, ComparisonVtxMin);

                if (bCollision == true)
                {//�A�C�e���ɓ���������
                    if (CScene::GetMode() == CScene::MODE_GAME)
                    {
                        SetDamage(1, 10);//�v���C���[�Ƀ_���[�W��^����
                    }
                }
            }

            //���X�g�����ɐi�߂�
            pObj = pNext;
        }
    }

}
//===================================================================================================================================================

//========================================================
//�A�C�e���Ƃ̓����蔻��
//========================================================
void CPlayer::CollisionItem()
{
    //==================================
    //�I�u�W�F�N�gX��������擾
    //==================================
    D3DXVECTOR3 Pos = GetPos();                      //�I�u�W�F�N�g�̈ʒu���擾
    D3DXVECTOR3 Size = GetSize();                    //�I�u�W�F�N�g�̃T�C�Y���擾
    D3DXVECTOR3 VtxMax = GetVtxMax();                //�I�u�W�F�N�g�̍ő咸�_���擾
    D3DXVECTOR3 VtxMin = GetVtxMin();                //�I�u�W�F�N�g�̍ŏ����_���擾
    D3DXVECTOR3 ComparisonPos = NULL_VECTOR3;                  //�v���C���[�̈ʒu����p
    D3DXVECTOR3 ComparisonVtxMax = NULL_VECTOR3;               //����Ώۂ̍ő咸�_
    D3DXVECTOR3 ComparisonVtxMin = NULL_VECTOR3;               //����Ώۂ̍ŏ����_
    CPlayer* pPlayer = nullptr;                                //�v���C���[�N���X�擾�p
    CObject* pObj = nullptr;                                   //�I�u�W�F�N�g�擾�p
    bool bCollision = false;                                   //�����������ǂ���
    CItem::ITEMTYPE ItemType = {};                             //�A�C�e���̎��
    CItem* pItem = nullptr;                                    //�A�C�e���ւ̃|�C���^
    //===========================================================

    //==================================
    //�A�C�e���̓����蔻��
    //==================================
    for (int nCntPriority = 0; nCntPriority < m_nMAXPRIORITY; nCntPriority++)
    {

        pObj = GetTopObject(nCntPriority);//�g�b�v�I�u�W�F�N�g���擾
        while (pObj != nullptr)
        {
            //���̃I�u�W�F�N�g���i�[
            CObject* pNext = pObj->GetNextObject();

            //��ނ̎擾�i�G�Ȃ瓖���蔻��j
            CObject::TYPE type = pObj->GetType();

            //�I�u�W�F�N�g�̎�ނ��G��������
            switch (type)
            {
            case CObject::TYPE_ITEM:
                pItem = (CItem*)pObj;                            //�I�u�W�F�N�g��G�̌^�Ƀ_�E���L���X�g
                ComparisonPos = pItem->GetPos();                   //�ʒu���擾
                ComparisonVtxMax = pItem->GetVtxMax();             //�ő咸�_���擾
                ComparisonVtxMin = pItem->GetVtxMin();             //�ŏ����_���擾
                ItemType = pItem->GetItemType();                   //�A�C�e���̎�ނ��擾����
                bCollision = CCollision::CollisionSquare(GetPos(),GetVtxMax(),GetVtxMin(), ComparisonPos, ComparisonVtxMax, ComparisonVtxMin);
                if (bCollision == true)
                {//�A�C�e���ɓ���������
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
                //���X�g�����ɐi�߂�
                pObj = pNext;
                continue;
                break;
            }
            //���X�g�����ɐi�߂�
            pObj = pNext;
        }
    }

}
//===================================================================================================================================================

//========================================================
//�g���b�v�Ƃ̓����蔻��
//========================================================
void CPlayer::CollisionTrap()
{
    //==================================
   //�I�u�W�F�N�gX��������擾
   //==================================
    D3DXVECTOR3 Pos = CObjectX::GetPos();                      //�I�u�W�F�N�g�̈ʒu���擾
    D3DXVECTOR3 Size = CObjectX::GetSize();                    //�I�u�W�F�N�g�̃T�C�Y���擾
    D3DXVECTOR3 VtxMax = GetVtxMax();                          //�I�u�W�F�N�g�̍ő咸�_���擾
    D3DXVECTOR3 VtxMin = GetVtxMin();                          //�I�u�W�F�N�g�̍ŏ����_���擾

    D3DXVECTOR3 ComparisonPos = NULL_VECTOR3;                  //�v���C���[�̈ʒu����p
    D3DXVECTOR3 ComparisonVtxMax = NULL_VECTOR3;               //����Ώۂ̍ő咸�_
    D3DXVECTOR3 ComparisonVtxMin = NULL_VECTOR3;               //����Ώۂ̍ŏ����_
    CObject* pObj = nullptr;                                   //�I�u�W�F�N�g�擾�p
    CTrap* pTrap = nullptr;                                    //�g���b�v�ւ̃|�C���^
    CTrap::TRAPTYPE TrapType;                                  //�g���b�v�̎��
    bool bCollision = false;                                   //�����蔻��
    //===========================================================

    //==================================
    //�g���b�v�̓����蔻��
    //==================================
    for (int nCntPriority = 0; nCntPriority < m_nMAXPRIORITY; nCntPriority++)
    {

        pObj = GetTopObject(nCntPriority);//�g�b�v�I�u�W�F�N�g���擾
        while (pObj != nullptr)
        {
            //���̃I�u�W�F�N�g���i�[
            CObject* pNext = pObj->GetNextObject();

            //��ނ̎擾�i�G�Ȃ瓖���蔻��j
            CObject::TYPE type = pObj->GetType();

            if (type == CObject::TYPE_TRAP)
            {
                //�I�u�W�F�N�g�̎�ނ��G��������
                pTrap = (CTrap*)pObj;                              //�I�u�W�F�N�g��G�̌^�Ƀ_�E���L���X�g
                TrapType = pTrap->GetTrapType();                   //�g���b�v�̎��
                ComparisonPos = pTrap->GetPos();                   //�ʒu���擾
                ComparisonVtxMax = pTrap->GetVtxMax();             //�ő咸�_���擾
                ComparisonVtxMin = pTrap->GetVtxMin();             //�ŏ����_���擾[

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
                {//�g���b�v�ɓ���������
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
            //���X�g�����ɐi�߂�
            pObj = pNext;
        }
    }

}
//===================================================================================================================================================

//========================================================
//���G���Ԃ̏���
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
//�`���[�W�U��
//========================================================
void CPlayer::ChargeAttack()
{
    D3DXVECTOR3& Pos = GetPos();
    D3DXVECTOR3 SenterPos = GetSenterPos();
    bool& bDodge = GetDodgeState();
    D3DXVECTOR3& Rot = GetRot();
    CBullet* pBullet = nullptr;//�o���b�g�ւ̃|�C���^
    CParticle* pParticle = nullptr;    //�p�[�e�B�N���擾�p
    bool bUnlockChargeAttack = CheckUnlockBulletAction();  //�`���[�W�U�����J������Ă��邩�ǂ����𔻒�
    //�����_���ȐF����

    if (CManager::GetInputKeyboard()->GetTrigger(DIK_N) == true || CManager::GetInputJoypad()->GetTrigger(CInputJoypad::JOYKEY_X) == true)
    {//�o���b�g����
        m_bPriorityMotion[MOTIONTYPE02_ATTACK] = true;
        pBullet = CBullet::Create(CBullet::BULLETTYPE_PLAYER, CBullet::BULLETTEX_PLAYER, 45,DamageResult(m_nNORMALSHOT_POWER), 40.0f, 40.0f, SenterPos, D3DXVECTOR3(sinf(Rot.y - D3DX_PI) * 10.0f, 0.0f, cosf(Rot.y - D3DX_PI) * 10.0f));
        pBullet->SetHitAddMp(1);//�P�q�b�g�ɂ�MP�{�P
        pBullet->SetUseReflection(true);

        m_ChargeAttack.bState = true;//�ʏ�U���������u�Ԃ�
        m_bJudgeCommand[PLAYERCOMMAND04_SHOT] = true;//�V���b�g�𔭎�
        CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_SHOT_000);
    }

    if (CManager::GetInputKeyboard()->GetTrigger(DIK_I) == true)
    {//�L���o���b�g����
        m_bPriorityMotion[MOTIONTYPE02_ATTACK] = true;
        pBullet = CBullet::Create(CBullet::BULLETTYPE_PLAYER, CBullet::BULLETTEX_PLAYER, 45, DamageResult(9999), 40.0f, 40.0f, SenterPos, D3DXVECTOR3(sinf(Rot.y - D3DX_PI) * 10.0f, 0.0f, cosf(Rot.y - D3DX_PI) * 10.0f));
        pBullet->SetHitAddMp(9999);//�f�o�b�O�p�Ȃ̂ŃJ���X�g������
    }

    if (m_bInvincible == false && bUnlockChargeAttack == true)
    {//���G��Ԃ���Ȃ���΃`���[�W�U���\

        if ((CManager::GetInputKeyboard()->GetPress(DIK_N) == true || CManager::GetInputJoypad()->GetPress(CInputJoypad::JOYKEY_X) == true) && m_ChargeAttack.bState == true)
        {
            m_ChargeAttack.nChargeCnt++;

            if (m_ChargeAttack.nChargeCnt >= m_nMAX_CHARGETIME && m_ChargeAttack.nChargeLevel < m_nMAX_CHARGEATTACKLEVEL)
            {
                m_ChargeAttack.nChargeLevel++;
                m_ChargeAttack.nChargeCnt = 0;
            }

            if (m_ChargeAttack.nChargeLevel >= 1 && m_ChargeAttack.nChargeLevel < m_nMAX_CHARGEATTACKLEVEL)
            {//�`���[�W���J�n���ꂽ��
                if (m_ChargeAttack.nChargeLevel == 1 && m_ChargeAttack.nChargeCnt == 1)
                {
                    CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_CHARGE_000);
                }
                ChargeAttackParticle();
            }

            //=============================
            //�`���[�W�ő�\������
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
                {//�㒷����
                    RainShotProcess();
                    CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_SHOT_RAIN);
                    m_bJudgeCommand[PLAYERCOMMAND06_RAINSHOT] = true;//���C���V���b�g�𔭎�
                }
                else if ((CManager::GetInputKeyboard()->GetPress(DIK_S) == true || CManager::GetInputJoypad()->GetPress(CInputJoypad::JOYKEY_DOWN) == true) && m_bUnlockBulletAction[BULLETACTION02_BURSTSHOT] == true)
                {//��������
                    BurstShotProcess();
                    CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_SHOT_BURST);
                    m_bJudgeCommand[PLAYERCOMMAND07_BURSTSHOT] = true;//�o�[�X�g�V���b�g�𔭎�
                }
                else if (bDodge == true && m_bUnlockBulletAction[BULLETACTION03_RAPIDSHOT] == true)
                {//������Ȃ���
                    RapidShotProcess();
                    CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_SHOT_ROGKET);
                    m_bJudgeCommand[PLAYERCOMMAND08_RAPIDSHOT] = true;//���s�b�h�V���b�g�𔭎�
                }
                else
                {//�ǂ̃R�}���h�������Ă��Ȃ�
                    if (m_bUnlockBulletAction[BULLETACTION00_CHARGE] == true)
                    {
                        ChargeShotProcess();
                        CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_SHOT_001);
                        m_bJudgeCommand[PLAYERCOMMAND05_CHARGESHOT] = true;//�`���[�W�V���b�g�𔭎�
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
//�`���[�W�U���̃p�[�e�B�N����������
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
//�`���[�W�U���̃p�[�e�B�N����������
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
        {//���F�ɓ_��
            m_apModelParts[nCnt]->SetOriginalColor(RandCol, 2);
            SetOriginalColor(RandCol, 2);
        }
    }
}
//===========================================================================================================================================================

//============================================================
//�`���[�W�V���b�g�̏���
//============================================================
void CPlayer::ChargeShotProcess()
{
    D3DXVECTOR3 Pos = GetPos();
    D3DXVECTOR3 Rot = GetRot();
    CBullet* pBullet = nullptr;//�o���b�g�ւ̃|�C���^
    m_bPriorityMotion[MOTIONTYPE02_ATTACK] = true;
    pBullet = CBullet::Create(CBullet::BULLETTYPE_PLAYER, CBullet::BULLETTEX_PLAYER, 300,DamageResult(m_nCHARGEATTACKPOWER[m_ChargeAttack.nChargeLevel - 1]), 40.0f * m_ChargeAttack.nChargeLevel, 40.0f * m_ChargeAttack.nChargeLevel, Pos, D3DXVECTOR3(sinf(Rot.y - D3DX_PI) * 10.0f, 0.0f, cosf(Rot.y - D3DX_PI) * 10.0f));
    pBullet->SetHitAddMp(4 * (1 + m_ChargeAttack.nChargeLevel));//�`���[�W���x���~�S���q�b�g����MP�𑝂₷
}
//===================================================================================================================================================

//============================================================
//�`���[�W�V���b�g�̏���
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
        pBullet->SetHitAddMp(1);//�q�b�g���ɑ��₷MP��ݒ�
        //pBullet->SetUseEffect(15, D3DXVECTOR2(40.0f, 40.0f), D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
    }

}
//===================================================================================================================================================

//=================================================================================
//�o�[�X�g�V���b�g�̏���
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
            pBullet->SetHitAddMp(1);//�q�b�g���ɑ��₷MP��ݒ�
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
//���s�b�h�V���b�g�̏���
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
//�o���b�g�A�N�V�������J�����Ă��邩�ǂ����̃`�F�b�N���s��
//=================================================================================
bool CPlayer::CheckUnlockBulletAction()
{
    bool bUnlock = false;

    for (int nCnt = 0; nCnt < BULLETACTION_MAX; nCnt++)
    {
        if (m_bUnlockBulletAction[nCnt] == true)
        {//�ǂꂩ��ł��J�����Ă�����
            bUnlock = true;
        }
    }

    return bUnlock;
}
//===================================================================================================================================================

//=================================================================================
//�d�͂̏���
//=================================================================================
void CPlayer::GravityProcess()
{
    D3DXVECTOR3& pos = GetPos();                                   //���f���̈ʒu�̎擾
    D3DXVECTOR3& Move = GetMove();                                   //�ړ��ʂ��擾����
    D3DXVECTOR3 Size = GetSize();                                   //�T�C�Y���擾
    float fMoveX = 0.0f;                                            //X�����̈ړ���
    float& fInertia = GetInertia();                                 //�������擾
    float fMoveZ = 0.0f;                                            //Z�����̈ړ���
    bool& bIsLanding = GetLanding();                                //�n�ʂɂ��邩�ǂ���
    bool& bDodge = GetDodgeState();
    bool& bIsWalling = GetWalling();                                //�ǂɂ������Ă��邩�ǂ����@
    bool& bIsJumping = GetJumping();                                //�W�����v���Ă��邩�ǂ���
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
        bIsJumping = false;//�W�����v�����ǂ���
        m_nJumpCnt = 0;      //�W�����v������
    }
    else
    {
        m_nCntLanding = 0;
        m_bPriorityMotion[MOTIONTYPE04_LANDING] = false;
    }

    if (bDodge == false)
    {//�����Ԃ̎��͏d�͂𖳌��ɂ���
        if (m_bUnlockAction[ACTIONTYPE03_WALLJUMP] == true)
        {//�ǃW�����v���J������Ă�����
            if (bIsWalling == false)
            {//�ǂɂ��Ȃ�������
                Move.y += -1.0f;
            }
            else
            {//�ǂɂ�����
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
        {//�ǃW�����v���J������Ă��Ȃ�������
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
//�ǃW�����v�̏���
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
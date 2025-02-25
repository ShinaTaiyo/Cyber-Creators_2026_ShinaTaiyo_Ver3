//====================================================
//
//�V���S���F�`���[�W�U������������[player.h]
//Author:ShinaTaiyo
//
//====================================================

#ifndef _PLAYER_H_  
#define _PLAYER_H_

//======================
//�C���N���[�h
//======================
#include "main.h"
#include "character.h"
#include "lockon.h"
#include "ui.h"
#include "player_actionmode.h"
#include "player_actionmode_mainstate.h"
#include "wire.h"
#include "gauge.h"
//==========================================

//======================
//�O���錾
//======================
class CPlayerAbnormalState;
class CPlayerAbnormalState_KnockBack;

//===========================================
//�v���C���[3D�N���X
//===========================================
class CPlayer : public CCharacter
{
public:
	//================================================
    //�A�N�V�������[�h�񋓌^
    //================================================
	enum class ACTIONMODE
	{
		SHOT = 0,
		DIVE,
		MAX
	};
	//===============================================================================================

	CPlayer(CPlayerMove* pPlayerMove, CPlayerAttack* pPlayerAttack,
		int nPri = 0, bool bUseintPri = false, CObject::TYPE type = CObject::TYPE::PLAYER, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_X);//�R���X�g���N�^
	~CPlayer();                                                                                    //�f�X�g���N�^
	HRESULT Init() override;                                                                       //����������
	void Uninit() override;                                                                        //�I������
	void Update() override;                                                                        //�X�V����
	void Draw() override;                                                                          //�`�揈��
	void SetDeath() override;                                                                      //���S�t���O��ݒ�
	static CPlayer* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, D3DXVECTOR3 Scale); //��������
	CLockon* GetLockOn() { return m_pLockOn; }                                                     //���b�N�I���̃C���X�^���X���擾

	//================================================
	//�����蔻��
	//================================================
	void SetSuccessCollision(bool bSuccess) { m_bCollision = bSuccess; }    //�����蔻�肪�����������ǂ�����ݒ�
	const bool& GetCollisionSuccess() const { return m_bCollision; }        //�����蔻�肪�����������ǂ������擾
	//===============================================================================================

	//================================================
	//���[�h�f�B�X�v
	//================================================
	void SetModeDisp(CUi* pModeDisp);                 //���[�h�\����ݒ�
	CUi* GetModeDisp() { return m_pModeDisp; }        //���[�h�\�����擾
	//===============================================================================================

	//================================================
	//���b�V��
	//================================================
	CWire* GetWire() { return m_pWire; }              //���C���[���擾
	//===============================================================================================

	//================================================
	//�Q�[�W
	//================================================
	CUi* GetDiveGaugeFrame() { return m_pDiveGaugeFrame; }   //�_�C�u�Q�[�W�̃t���[��
	CUi* GetDivePossibleNum() { return m_pDivePossibleNum; } //�_�C�u�񐔕\���pUI
	//===============================================================================================
	
	//================================================
	//���[�h�`�F���W
	//================================================
	void ChengeMoveMode(CPlayerMove* pPlayerMove);                      //�ړ����[�h��ς���
	void ChengeAttackMode(CPlayerAttack* pPlayerAttack);                //�U�����[�h��ς���
	void ChengeActionMode(CPlayerActionMode* pPlayerActionMode);        //�v���C���[�̃A�N�V�������[�h��ύX����
	void SetInitialActionMode(ACTIONMODE ActionMode);                   //�A�N�V�������[�h��ݒ肷��
	//===============================================================================================

	//================================================
	//��Ԉُ�`�F���W
	//================================================
	void ChengeAbnormalState(CPlayerAbnormalState* pAbnormalState);//��Ԉُ��ς���
	//===============================================================================================

	//=============================
	//�̗͌n
	//=============================
	void SetDamage(int nDamage, int nHitStopTime) override; //�_���[�W��^����
	const bool& IsDamaged() { return m_bDamage; }           //�_���[�W���󂯂����ǂ������󂯎��i�v���C���[�̍X�V�̍Ō��false�ɂȂ�̂ŁA�����Ǝ擾�ł���j
	//===============================================================================================

	//================================================
	//�ÓI�����o�擾
	//================================================
	static const int& GetMaxDiveNum() { return s_nMaxDiveNum; }//�ő�_�C�u�\�񐔂��擾����
	//===============================================================================================
private:
	//================================================
	//�ÓI�����o
	//================================================
	static constexpr float m_fNORMALATTACK_SPEED = 10.0f;   //�ʏ�U���̈ړ��ʂ�ݒ�
	static const int s_nNORMAL_MAXLIFE;                     //�v���C���[�̒ʏ�ő�̗�
	static const int s_nMaxDiveNum;                         //�v���C���[�̍ő�_�C�u��
	//===============================================================================================

	//================================================
	//�ϐ��錾
	//================================================
	float m_fRotAim;                             //�ړI�̌���
	bool m_bCollision;                           //�����蔻�肪�����������ǂ���
	bool m_bDamage;                              //�_���[�W���󂯂����ǂ���
										         
	//���N���X�C���X�^���X			   	         
	ACTIONMODE m_NowActionMode;                  //���݂̃A�N�V�������[�h
	CLockon* m_pLockOn;                          //���b�N�I���J�[�\��
	CUi* m_pModeDisp;                            //���[�h�\��UI
	CWire* m_pWire;                              //���C���[
	CGauge* m_pHpGauge;                          //�̗̓Q�[�W
	CUi* m_pDivePossibleNum;                     //�_�C�u�\��UI
	CUi* m_pDiveGaugeFrame;                      //�_�C�u�Q�[�W�̃t���[��
										         
	//�s�����							         
	CPlayerMove* m_pMove;                        //�ړ�����
	CPlayerAttack* m_pAttack;                    //�U������
	CPlayerActionMode* m_pPlayerActionMode;      //�v���C���[�A�N�V�������[�h(���C���̃X�e�[�g�N���X)
											     
	//��Ԉُ�X�e�[�g						     
	CPlayerAbnormalState* m_pAbnormalState;      //��Ԉُ�

	//===============================================================================================


	//================================================
	//�v���g�^�C�v�錾
	//================================================

	//=============================
	//�ʒu�n
	//=============================
	void AdjustRot();//������������
	//================================================

	//=============================
	//���[�h�`�F���W
	//=============================
	void ActionModeChengeProcess();//�A�N�V�������[�h��ύX����
	//==============================================================================================

	//=============================
	//���o
	//=============================
	void DiveGaugeMaxEffect();//�_�C�u�Q�[�W���}�b�N�X�ɂȂ������ɃG�t�F�N�g���o��
	//==============================================================================================

	//=============================
	//����n
	//=============================
	void CollisionProcess();//�����蔻�菈���S��
	//===============================================================================================
};

//===================================================
//��Ԉُ�X�e�[�g
//===================================================
class CPlayerAbnormalState
{
public:
	CPlayerAbnormalState();                   //�R���X�g���N�^
	virtual ~CPlayerAbnormalState();          //�f�X�g���N�^
	virtual void Process(CPlayer * pPlayer);  //����
};
//===============================================================================================

//===================================================
//��Ԉُ�F�Ԃ���΂�
//===================================================
class CPlayerAbnormalState_KnockBack : public CPlayerAbnormalState
{
public:
	CPlayerAbnormalState_KnockBack(CPlayer * pPlayer,D3DXVECTOR3 KnockBackMove,float fInertia);   //�R���X�g���N�^
	~CPlayerAbnormalState_KnockBack() override;                                                   //�f�X�g���N�^
	void Process(CPlayer* pPlayer) override;                                                      //����
private:																					      
	D3DXVECTOR3 m_KnockBackMove;                                                                  //�m�b�N�o�b�N����ړ���
	float m_fInertia;                                                                             //�����x
};
//===============================================================================================
#endif
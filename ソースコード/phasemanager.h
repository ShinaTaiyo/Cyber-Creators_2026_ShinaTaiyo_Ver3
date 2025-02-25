//==========================================================================================
//
//�P�P���Q�X���F�t�F�[�Y�}�l�[�W���[�����[phasemanager.h]
//Author:ShinaTaiyo
//
//==========================================================================================

#ifndef _PHASEMANAGER_H_  
#define _PHASEMANAGER_H_

//============================
//�C���N���[�h
//============================
#include "main.h"
#include "object.h"
#include "enemy.h"

//========================================================

//======================
//�O���錾
//======================

//============================
//�t�F�[�Y�Ǘ��N���X
//============================
class CPhaseManager : public CObject
{
public:
	CPhaseManager(int nNumStage,int nPri = 0, bool bUseintPri = false, CObject::TYPE type = CObject::TYPE::PHASEMANAGER, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_NONE);  //�R���X�g���N�^
	~CPhaseManager();                          //�f�X�g���N�^
	HRESULT Init() override;                   //����������
	void Uninit() override;                    //�I������
	void Update() override;                    //�X�V����
	void Draw() override;                      //�`�揈��
	void SetDeath() override;                  //���S�t���O��ݒ�
	static CPhaseManager * Create();           //��������
	void PushPhaseInfo(D3DXVECTOR3 Pos, D3DXVECTOR3 Rot, D3DXVECTOR3 Scale, int nLife, int nEnemyType, int nTypeNum, int nPhaseNum,float fNormalSpeed,float fSensingRange,int nNumDivision, vector<CEnemy::MoveAiInfo> VecMoveAi);//�t�F�[�Y����ݒ�
private:
	struct PhaseSaveInfo
	{
		D3DXVECTOR3 Pos;                      //�ʒu
		D3DXVECTOR3 Rot;                      //����
		D3DXVECTOR3 Scale;                    //�g�嗦
		int nLife;                            //�̗�
		int nEnemyType;                       //�G�^�C�v
		int nTypeNum;                         //�N���X���Ƃ̃^�C�v
		int nPhaseNum;                        //�t�F�[�Y�ԍ���ݒ�
		float fNormalSpeed;                   //���ʂ̑��x
		float fSensingRange;                  //���G����
		vector<CEnemy::MoveAiInfo> VecMoveAi; //�ړ�AI��Vector
		int nNumDivision;                     //�����
	};
	static list<PhaseSaveInfo>s_PhaseList;    //�t�F�[�Y���̃��X�g
	static int s_MaxPhase;                    //�t�F�[�Y�̍ő吔
	static int s_nNowPhase;                   //���݂̃t�F�[�Y�ԍ�
	static bool s_bStartFade;                 //�t�F�[�h���J�n����
	static int s_nNowStage;                   //���݂̃X�e�[�W�ԍ�
	void AdvancePhase();                      //���̃t�F�[�Y�Ɉڍs���鏈��
};
#endif
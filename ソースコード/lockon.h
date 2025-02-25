//====================================================
//
//�P�O���Q�W���F�_���������ɒe�𔭎˂�����[lockon.h]
//Author:ShinaTaiyo
//
//====================================================

#ifndef _LOCKON_H_  
#define _LOCKON_H_

//======================
//�C���N���[�h
//======================
#include "main.h"
#include "object2d.h"
#include "lockon.h"
//==========================================

//==========================================
//���b�N�I���N���X
//==========================================
class CLockon : public CObject2D
{
public:
	enum class ENDSTATE
	{
		NONE = 0,
		RIGHTEND,
		LEFTEND,
		UPEND,
		DOWNEND,
		MAX
	};

	enum class TYPE
	{//���
		SHOT = 0,
		DIVE,
		STUCKSHOT,
		MAX
	};

	CLockon(int nPri = 0, bool bUseintPri = false, CObject::TYPE type = CObject::TYPE::LOCKON, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_2D);//�`�揇�ݒ�p�R���X�g���N�^
	~CLockon() override;                                                                                                          //�f�X�g���N�^
	HRESULT Init() override;                                                                                                      //����������
	void Uninit() override;                                                                                                       //�I������
	void Update() override;                                                                                                       //�X�V����
	void Draw() override;                                                                                                         //�`�揈��
	void SetDeath() override;                                                                                                     //���S�t���O�ݒ菈��
	static CLockon* Create(TYPE Type,D3DXVECTOR3 Pos,CObject2D::POLYGONTYPE PolygonType,float fWidth,float fHeight,D3DXCOLOR col);//��������
	void ChengeTexture(TYPE Type);                                                                                                //�e�N�X�`����ς���
	const D3DXVECTOR3& GetLockOnPos() const { return m_LockOnPos; }                                                               //�_���Ă���ʒu���擾����
	const D3DXVECTOR3& GetNowRay() const { return m_NowRay; }                                                                     //���݂̃��C���擾����
	const D3DXVECTOR3& GetFrontPos() const { return m_FrontPos; }                                                                 //��O�̈ʒu���擾����
	const D3DXVECTOR3& GetNearRayColObjPos() const { return m_NearRayColObjPos; }                                                 //���C������������ԋ߂��I�u�W�F�N�g�̏Փˈʒu�����߂�
	const ENDSTATE& GetEndState() const { return m_EndState; }                                                                   //���b�N�I������ʓ��̂ǂ̒[�ɓ������Ă��邩���擾����
	const bool& GetSuccessRayCollision() const { return m_bRayCollision; }                                                       //���C�̓����蔻�肪�����������ǂ���
private:
	//=======================================
	//�ÓI�����o
	//=======================================
	static constexpr float m_fNORMAL_LOCKONMOVE = 20.0f;               //���b�N�I���̈ړ���
	static constexpr float s_fLOCKONSTAGING_ADDSCALE = 0.5f;           //���b�N�I�����o�̉��Z�g�嗦
	static constexpr int s_nLOCKONSTAGING_LIFE = 20;                   //���b�N�I�����o�̗̑�
	static const string s_LOCKON_FILENAME[static_cast<int>(TYPE::MAX)];//�e�N�X�`����
	//==========================================================================================================

	//=======================================
	//�ϐ�
	//=======================================
	D3DXVECTOR3 m_LockOnPos;       //�_���Ă���ʒu
	D3DXVECTOR3 m_NowRay;          //���݂̃��C
	D3DXVECTOR3 m_FrontPos;        //��O�̈ʒu
	ENDSTATE m_EndState;           //�ǂ̒[�ɂ��邩
	TYPE m_Type;                   //���
	D3DXVECTOR3 m_NearRayColObjPos;//��ԋ߂��I�u�W�F�N�g�̃��C���������Ă���ʒu�����߂�
	bool m_bRayCollision;          //���C���������Ă��邩�ǂ���
	//==========================================================================================================

	//=======================================
	//�v���g�^�C�v�錾
	//=======================================
	void MoveProcess();//�ړ�����
	void BackWallRayCollisionPosSearch();//���C���������Ă��鉜�̕ǂ̈ʒu���T�[�`
	void CalcRay();//���C���v�Z����
	void RayCollisionToObject();//�ǂ̃I�u�W�F�N�g�Ƀ��C���������Ă��邩�����߂�	
	//==========================================================================================================


};

#endif

//====================================================
//
//�U���P�U���F�v���C���[�̃Q�[�W������[gauge.h]
//Author:ShinaTaiyo
//
//====================================================

#ifndef _GAUGE_H_  
#define _GAUGE_H_

//======================
//�C���N���[�h
//======================
#include "main.h"
#include "object2d.h"
//==========================================

class CDamageGauge;
//==========================================

//==========================================
//�Q�[�W�N���X
//==========================================
class CGauge : public CObject2D
{
public:
	//===========================
	//�Q�[�W�̎��
	//===========================
	typedef enum
	{
		GAUGETYPE_PLAYERHP = 0, //�v���C���[�̗̑�
		GAUGETYPE_PLAYERMP,     //�v���C���[��MP
		GAUGETYPE_BOSSHP,       //�{�X��HP
		GAUGETYPE_DAMAGE,       //�_���[�W
		GAUGETYPE_PLAYERUSAGEMP,//�v���C���[MP�����
		GAUGETYPE_MAX
	}GAUGETYPE;
	//======================================================================
	CGauge();                   //�R���X�g���N�^
	~CGauge() override;         //�f�X�g���N�^
	HRESULT Init() override;    //����������
	void Uninit() override;     //�I������
	void Update() override;     //�X�V����
	void Draw() override;       //�`�揈��
	virtual void AddParam(int nParam);//�p�����[�^�𑝉�����
	void SubParam(int nParam) { m_nParam -= nParam; }            //�p�����[�^����������@
	void SetDeath() override;   //���S�t���O��ݒ�
	int& GetParam() { return m_nParam; }//�p�����[�^���擾
	int GetMaxParam() { return m_nMaxParam; }//�ő�p�����[�^���擾
	virtual void SetParam(int nParam);
	void SetMaxParam(int nParam) { m_nMaxParam = nParam; }//�ő�p�����[�^��ݒ�
	void SetGaugeType(GAUGETYPE type) { m_type = type; }
	static CGauge* Create(GAUGETYPE type, int nParam, float fWidth, float fHeight, D3DXVECTOR3 pos);//�Q�[�W�𐶐�
protected:
	int m_nParam;               //�Q�[�W�ŕ\�L���錻�݂̃p�����[�^
	int m_nParamOld;            //��O�̃p�����[�^
private:
	GAUGETYPE m_type;           //�Q�[�W�̎��
	CDamageGauge* m_pDamageGauge;//�_���[�W�Q�[�W
	int m_nMaxParam;            //�Q�[�W�ŕ\�L����p�����[�^�̍ő�l
};
//=======================================================================================================================

//==========================================
//�_���[�W�Q�[�W�N���X
//==========================================
class CDamageGauge : public CGauge
{
public:
	CDamageGauge();             //�R���X�g���N�^
	~CDamageGauge() override;   //�f�X�g���N�^
	HRESULT Init() override;    //����������
	void Uninit() override;     //�I������
	void Update() override;     //�X�V����
	void Draw() override;       //�`�揈��
	void AddParam(int nParam) override; //�p�����[�^�𑝉�
	void SetParam(int nParam) override;   //�p�����[�^��ݒ�
	void SetDeath() override;   //���S�t���O��ݒ�
	void SetStartDamageParam(int nParam) { m_nStartDamageParam = nParam; }
	void SetStartParam(int nParam) { m_nStartParam = nParam; }//�_���[�W�\�����J�n���鎞�̃p�����[�^��ݒ�
	static CDamageGauge* Create(CGauge::GAUGETYPE type, int nParam, int nMaxParam, float fWidth, float fHeight, D3DXVECTOR3 pos, int nStartDamageParam);//�Q�[�W�𐶐�
private:
	static const int m_nSETSUBSTARTCNT = 90;
	int m_nSubStartCnt;//�Q�[�W�̌��Z���n�߂�܂ł̃J�E���g
	bool m_bSubGauge;  //�Q�[�W�̌��������邩�ǂ��� 
	int m_nFirstParam;//�ŐV�̃p�����[�^
	int m_nAddParamCnt;//�p�����[�^�𑝂₵����
	int m_nStartParam;          //�_���[�W�\�����J�n���鎞�̃p�����[�^
	int m_nStartDamageParam;    //�_���[�W�\�����J�n����Ƃ��̃_���[�W��
};
//=======================================================================================================================
#endif

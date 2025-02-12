//====================================================
//
//�V�T���F����A�C�e��������[item.h]
//Author:ShinaTaiyo
//
//====================================================

#ifndef _ITEM_H_  
#define _ITEM_H_

//======================
//�C���N���[�h
//======================
#include "main.h"
#include "objectX.h"
//==========================================

//==========================================

//==========================================
//�A�C�e���N���X
//==========================================
class CItem : public CObjectX
{
public:
	//===========================
	//�A�C�e���̎��
	//===========================
	typedef enum
	{
		ITEMTYPE00_HEALHEART = 0,
		ITEMTYPE01_COIN,
		ITEMTYPE02_MAGICPORTION,
		ITEMTYPE03_CROWN,
		ITEMTYPE04_DOUBLEJUMPSHOES,
		ITEMTYPE05_DODGECRYSTAL,
		ITEMTYPE06_WALLJUMPCROW,
		ITEMTYPE07_FIREBALL_ENERGY,
		ITEMTYPE08_BEAMWHIP_ENERGY,
		ITEMTYPE09_ICE_ENERGY,
		ITEMTYPE10_THUNDER_ENERGY,
		ITEMTYPE11_MAGICSWORD_ENERGY,
		ITEMTYPE12_PLANTSHIELD_ENERGY,
		ITEMTYPE13_QUICKHEAL_ENERGY,
		ITEMTYPE14_CROSSBOMB_ENERGY,
		ITEMTYPE15_BLACKHOLE_ENERGY,
		ITEMTYPE16_PHYCOBURST_ENERGY,
		ITEMTYPE17_CHARGESHOT_GUN,
		ITEMTYPE18_RAINSHOT_GUN,
		ITEMTYPE19_BURSTSHOT_GUN,
		ITEMTYPE20_RAPIDSHOT_GUN,
		ITEMTYPE_MAX
	}ITEMTYPE;
	//======================================================================
	CItem();                    //�R���X�g���N�^
	~CItem() override;          //�f�X�g���N�^
	HRESULT Init() override;    //����������
	void Uninit() override;     //�I������
	void Update() override;     //�X�V����
	void Draw() override;       //�`�揈��
	void SetDeath() override;   //���S�t���O��ݒ�
	static CItem* Create(ITEMTYPE type,D3DXVECTOR3 Pos,D3DXVECTOR3 Rot,D3DXVECTOR3 Scale,int nRotType);//�G�𐶐�
	static int GetNumFile() { return m_nNumFile; }           //�t�@�C���̐����擾����
	ITEMTYPE GetItemType() { return m_type; }                //�A�C�e���̎�ނ��擾����
private:
	void Collision(void);                                    //�����蔻����s��
	static bool NotSummonProcess(ITEMTYPE type);
	ITEMTYPE m_type;                                         //�G�̎��
	bool m_bUse;                                             //�g�p���
	int m_nCntTime;                                          //�o�����Ă���̃t���[�����𑪂�
	static const char* m_ITEMFILENAME[ITEMTYPE_MAX];         //�A�C�e���̃t�@�C���̖��O
	static int m_nNumFile;                                   //�t�@�C���̐�
};

#endif

//====================================================
//
//�T���Q�Q���FC++�Ŋ�ՂÂ���[bg.h]
//Author:ShinaTaiyo
//
//====================================================

#ifndef _BG_H_  
#define _BG_H_

//======================
//�C���N���[�h
//======================
#include "main.h"
#include "object2d.h"
//==========================================

//==========================================
//�w�i�N���X
//==========================================
class CBg :  public CObject2D
{
public:
	typedef enum
	{//�w�i�̎��
		BG_TITLE = 0, //�^�C�g���w�i
		BG_DIFFICULTY,//��Փx�I��w�i
		BG_RESULT,    //���U���g�w�i
		BG_MAX 
	}BG;
	CBg(int nPri = 0, bool bUseintPri = false, CObject::TYPE type = CObject::TYPE::BG, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_2D);//�R���X�g���N�^
	virtual ~CBg();                           //�f�X�g���N�^
	HRESULT Init() override;                  //����������
	void Uninit() override;                   //�I������
	void Update() override;                   //�X�V����
	void Draw() override;                     //�`�揈��
	void SetDeath() override;                 //���S�t���O��ݒ�
	static void Create(BG type);              //�w�i�̐���
	void SetBgType(BG type) { m_Type = type; }//�w�i�̎�ނ�ݒ�

	//=================================
	//�e�N�X�`���ړ��֌W
	//=================================
	void SetUseTexMove(bool bUse, D3DXVECTOR2 Pos, D3DXVECTOR2 Move) { m_bUseTexMove = bUse; m_PosTex = Pos; m_MoveTex = Move; }//�e�N�X�`�����ړ������邩�ǂ���?�ړ�������Ȃ珉���ʒu�ƈړ��ʂ�ݒ肷��
	void SetPosTex(D3DXVECTOR2 Pos) { m_PosTex = Pos; }//�e�N�X�`�����W��ݒ肷��
	void SetMoveTex(D3DXVECTOR2 Move) { m_MoveTex = Move; }//�e�N�X�`���̈ړ��ʂ�ݒ肷��
	//==================================================================================

protected:
	BG GetType() { return m_Type; }//�w�i�̎�ނ��擾

	//============================
	//��]�n
	//============================

	//��]��
	void SetRotationPower(float fPower) { m_fRotationPower = fPower; }//��]���x��ݒ肷��
	float& GetRotationPower() { return m_fRotationPower; }//��]���x���擾����

	//==================================================================================

	static const std::string m_apBGFILE_NAME[BG_MAX];//�w�i�̃e�N�X�`���t�@�C����
private:
	float m_fPosTexU;          //�e�N�X�`�����W!
	BG m_Type;                 //�w�i�̎��!
	float m_fRotationPower;    //��]��!

	//=================================
	//�e�N�X�`���ړ��֌W
	//=================================
	bool m_bUseTexMove;        //�w�i���ړ������邩�ǂ���!
	D3DXVECTOR2 m_PosTex;      //�e�N�X�`�����W!
	D3DXVECTOR2 m_MoveTex;     //�e�N�X�`���̈ړ���!
	//==================================================================================
};
//=======================================================================================

//=====================================
//�^�C�g���w�i�N���X
//=====================================
class CBgTitle : public CBg
{
public:
	CBgTitle();                       //�R���X�g���N�^
	~CBgTitle() override;             //�f�X�g���N�^
	HRESULT Init() override;          //����������
	void Uninit() override;           //�I������
	void Update() override;           //�X�V����
	void Draw() override;             //�`�揈��
	void SetDeath() override;         //���S�t���O��ݒ�
	static CBgTitle * Create(BG type);//�w�i�̐���
private:
};
//=======================================================================================

//=====================================
//��Փx�I��w�i�N���X
//=====================================
class CBgDifficulty : public CBg
{
public:
	CBgDifficulty();                      //�R���X�g���N�^
	~CBgDifficulty() override;            //�f�X�g���N�^
	HRESULT Init() override;              //����������
	void Uninit() override;               //�I������
	void Update() override;               //�X�V����
	void Draw() override;                 //�`�揈��
	void SetDeath() override;             //���S�t���O��ݒ�
	static CBgDifficulty* Create(BG type);//�w�i�̐���
private:

};
//=======================================================================================


#endif

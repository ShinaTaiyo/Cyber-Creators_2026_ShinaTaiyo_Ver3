//===========================================================================
//
//�V���Q�X���F�K�w�\������[modelparts.h]
//Author:ShinaTaiyo
//
//===========================================================================

//=====================================
//�C���N���[�h
//=====================================
#include "main.h"
#include "objectX.h"
//========================================================================

//=====================================
//���f���p�[�c�N���X
//=====================================
class CModelParts : public CObjectX
{
public:
	CModelParts(int nPriority = 2);    //�R���X�g���N�^
	~CModelParts();                    //�f�X�g���N�^
	HRESULT Init() override;           //����������
	void Uninit() override;            //�I������
	void SetDeath() override;          //���S�t���O��������
	void Update() override;            //�X�V����
	void Draw() override;              //�`�揈��
	void ExtraDraw();                  //�ʘg�̕`�揈��
	static CModelParts* Create(char * aString,D3DXVECTOR3 OffsetPos,D3DXVECTOR3 OffSetRot);//��������
	void SetParentParts(D3DXMATRIX* pParentParts) { m_pMtxParentParts = pParentParts; }      //���f���p�[�c�̐e��ݒ�
	D3DXVECTOR3 GetOffSetSupportPos() { return m_OffSetSupportPos; }                      //�ŏ��̃I�t�Z�b�g�̈ʒu��ݒ�
	D3DXVECTOR3 GetOffSetSupportRot() { return m_OffSetSupportRot; }                      //�ŏ��̃I�t�Z�b�g�̌�����ݒ�
	void SetOffSetPos(D3DXVECTOR3 Pos) { m_OffSetPos = Pos; }                              //�I�t�Z�b�g�̈ʒu��ݒ�
	void SetOffSetRot(D3DXVECTOR3 Rot) { m_OffSetRot = Rot; }                              //�I�t�Z�b�g�̌�����ݒ�
	D3DXVECTOR3 GetWorldPos() { return m_WorldPos; }
	void SetParent(D3DXMATRIX* pParent) { m_pMtxParent = pParent; }                             //�e���f����ݒ�
	D3DXVECTOR3& GetOffSetPos() { return m_OffSetPos; }                                    //�I�t�Z�b�g�̈ʒu���擾
	D3DXVECTOR3& GetOffSetRot() { return m_OffSetRot; }                                    //�I�t�Z�b�g�̌������擾
private:
	D3DXVECTOR3 m_WorldPos;                                                                //���[���h���W
	D3DXVECTOR3 m_OffSetPos;                                                               //�I�t�Z�b�g�ʒu
	D3DXVECTOR3 m_OffSetSupportPos;                                                        //�ŏ��̃I�t�Z�b�g�̈ʒu
	D3DXVECTOR3 m_OffSetRot;                                                               //�I�t�Z�b�g����
	D3DXVECTOR3 m_OffSetSupportRot;                                                        //�ŏ��̃I�t�Z�b�g�̌���
	D3DXMATRIX* m_pMtxParentParts;                                                           //�e���f���p�[�c�ւ̃|�C���^
	D3DXMATRIX* m_pMtxParent;                                                                 //�e���f���p�[�c�̃}�g���b�N�X
};
//============================================================================================================

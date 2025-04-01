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
	CModelParts(int nPri = 0, bool bUseintPri = false, CObject::TYPE type = CObject::TYPE::NONE, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_X);    //�R���X�g���N�^
	~CModelParts();                    //�f�X�g���N�^
	HRESULT Init() override;           //����������
	void Uninit() override;            //�I������
	void Update() override;            //�X�V����
	void ExtraUpdate();                //�ʘg�̍X�V�����i�I�u�W�F�N�g�̔h�������ǁA���̃p�[�c�������Ă���I�u�W�F�N�g�̏������Ă΂ꂽ�Ƃ��ɌĂт����̂ŁA�����I�ɌĂ΂�Ȃ��悤�ɂ���j
	void Draw() override;              //�`�揈��
	void ExtraDraw();                  //�ʘg�̕`�揈���i�I�u�W�F�N�g�̔h�������ǁA���̃p�[�c�������Ă���I�u�W�F�N�g�̏������Ă΂ꂽ�Ƃ��ɌĂт����̂ŁA�����I�ɌĂ΂�Ȃ��悤�ɂ���j
	void SetDeath() override;          //���S�t���O�ݒ菈��
	static CModelParts* Create(string String,CObject::TYPE Type);//��������

	void SetPartsIdx(int nIdx) { m_nPartsIdx = nIdx; }//�p�[�c�ԍ���ݒ肷��
private:
	int m_nPartsIdx;//�C���f�b�N�X
};
//============================================================================================================

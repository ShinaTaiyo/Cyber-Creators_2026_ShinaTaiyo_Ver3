//==================================================================================
//
//�P�O���Q�W���F�_���������ɒe�𔭎˂���[ui3d.h]
//Author:ShinaTaiyo
//
//===================================================================================
#ifndef _UI3D_H_
#define _UI3D_H_

//===================================================================================
//�C���N���[�h
//===================================================================================
#include "main.h"
#include "billboard.h"
//===================================================================================

//===================================================================================
//UI3D�N���X
//===================================================================================
class CUi3D : public CBillboard
{
public:
	enum class UI3DTYPE
	{//3DUI�̎��
		LOCKON = 0,
		MAX
	};

	CUi3D(int nPri = 0, bool bUseintPri = false, CObject::TYPE type = CObject::TYPE::UI3D, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_BILLBOARD);//�R���X�g���N�^
	~CUi3D();                                                                                                  //�f�X�g���N�^
	HRESULT Init(void) override;                                                                               //����������
	void Uninit(void) override;                                                                                //�I������
	void Update(void) override;                                                                                //�X�V����
	void Draw(void) override;                                                                                  //�`�揈��
	void SetDeath() override;                                                                                  //���S�t���O�ݒ菈��
	static CUi3D* Create(UI3DTYPE type,D3DXVECTOR3 Pos,D3DXVECTOR3 Move,int nLife,float fWidth,float fHeight); //��������
protected:
	static const string m_UI3D_FILENAME[static_cast<int>(UI3DTYPE::MAX)];                                      //3DUI�̃t�@�C����
	void SetUi3DType(UI3DTYPE type) { m_Type = type; }                                                         //��ނ�ݒ�
	UI3DTYPE GetUi3DType() { return m_Type; }                                                                  //��ނ��擾
private:
	UI3DTYPE m_Type;
};
//===================================================================================


#endif
//=====================================================================================================
//
//�P�P���P�X���F3D�w�i��`�悷��[bg3d.h]
//Author:ShinaTaiyo
//
//=====================================================================================================

#ifndef _BG3D_H_
#define _BG3D_H_


//====================================================
//�C���N���[�h
//====================================================
#include "main.h"
#include "objectX.h"
//=====================================================================================================

//====================================================
//3D�w�i�N���X
//====================================================
class CBg3D : public CObjectX
{
public:
	enum class BG3DTYPE
	{//�w�i�̎��
	    SKY = 0,
		MAX
	};

	CBg3D(int nPri = 0, bool bUseintPri = false, CObject::TYPE type = CObject::TYPE::BG3D, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_X);//�R���X�g���N�^
	~CBg3D();                                                                                //�f�X�g���N�^
	HRESULT Init() override;                                                                 //����������
	void Uninit() override;                                                                  //�I������
	void Update() override;                                                                  //�X�V����
	void Draw() override;                                                                    //�`�揈��
	void SetDeath() override;                                                                //���S�t���O�ݒ菈��
	static CBg3D* Create(BG3DTYPE type,D3DXVECTOR3 Pos, D3DXVECTOR3 Rot,D3DXVECTOR3 Scale);  //��������
protected:
	static const string s_BG3D_FILENAME[static_cast<int>(BG3DTYPE::MAX)];//���f���p�X
	void SetBg3DType(BG3DTYPE type) { m_Type = type; }//3D�w�i�̎�ނ�ݒ肷��
	const BG3DTYPE GetType() const{ return m_Type; }  //3D�w�i�̎�ނ��擾����
private:
	BG3DTYPE m_Type;                                  //3D�w�i�̎��
};
//=====================================================================================================


#endif // !
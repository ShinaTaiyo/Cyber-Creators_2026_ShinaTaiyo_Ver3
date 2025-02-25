//====================================================
//
//�X���V���FAI�p�̃��f�������[aimodel.h]
//Author:ShinaTaiyo
//
//====================================================

#ifndef _AIMODEL_H_  
#define _AIMODEL_H_

//======================
//�C���N���[�h
//======================
#include "main.h"
#include "objectX.h"
//==========================================

//===========================================
//AI���f���N���X
//===========================================
class CAIModel : public CObjectX
{
public:
	enum class AIMODELTYPE
	{
		MOVEPOINT = 0,
		MAX
	};

	CAIModel(int nPri = 0, bool bUseintPri = false, CObject::TYPE type = CObject::TYPE::AIMODEL, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_X);                 //�R���X�g���N�^
	~CAIModel();                //�f�X�g���N�^
	HRESULT Init() override;    //����������
	void Uninit() override;     //�I������
	void Update() override;     //�X�V����
	void Draw() override;       //�`�揈��
	void ExtraDraw();           //�`�揈��
	void SetDeath() override;   //���S�t���O��ݒ�
	static CAIModel* Create(AIMODELTYPE type,D3DXVECTOR3 pos, D3DXVECTOR3 rot,D3DXVECTOR3 Scale,D3DXMATRIX * mtxWorld);
private:
	static const string m_aAIMODEL_FILENAME[static_cast<int>(AIMODELTYPE::MAX)];
	AIMODELTYPE m_Type;
};
//==============================================================================================================================================================

#endif

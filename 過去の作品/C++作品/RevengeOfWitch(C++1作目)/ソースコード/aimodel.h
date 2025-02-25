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
	typedef enum
	{
		AIMODELTYPE_SQUARE = 0,//�l�p�`
		AIMODELTYPE_BALL,      //�~
		AIMODELTYPE_MAX
	}AIMODELTYPE;

	CAIModel();                 //�R���X�g���N�^
	~CAIModel();                //�f�X�g���N�^
	HRESULT Init() override;    //����������
	void Uninit() override;     //�I������
	void Update() override;     //�X�V����
	void Draw() override;       //�`�揈��
	void ExtraDraw();           //�`�揈��
	void SetDeath() override;   //���S�t���O��ݒ�
	static CAIModel* Create(AIMODELTYPE type,D3DXVECTOR3 pos, D3DXVECTOR3 rot,D3DXVECTOR3 Scale);
private:
	AIMODELTYPE m_Type;
	static const char* m_apAIMODEL_FILENAME[AIMODELTYPE_MAX];
};
//==============================================================================================================================================================

#endif

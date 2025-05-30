//====================================================
//
//�U���T���F���f���̓����蔻����s��[model.h]
//Author:ShinaTaiyo
//
//====================================================

#ifndef _MODEL_H_  
#define _MODEL_H_

//======================
//�C���N���[�h
//======================
#include "main.h"
#include "object2d.h"
#include "objectX.h"
//==========================================

//===========================================
//���f���N���X
//===========================================
class CModel: public CObjectX
{
public:
	//=============================================
	//���f����ޗ񋓌^
	//=============================================
	typedef enum
	{
		MODELTYPE_BILL = 0,        //�r��[0]
		MODELTYPE_TV,              //�e���r[1]
		MODELTYPE_SWITCH,          //�j���e���h�[�X�C�b�`[2]
		MODELTYPE_RING,            //�����O[3]
		MODELTYPE_MAX
	}MODELTYPE;
	//========================================================================================
	CModel();                  //�R���X�g���N�^
	~CModel();                 //�f�X�g���N�^
	HRESULT Init() override;    //����������
	void Uninit() override;     //�I������
	void Update() override;     //�X�V����
	void Draw() override;       //�`�揈��
	void SetDeath() override;   //���S�t���O��ݒ�
	static CModel* Create(MODELTYPE type,D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, D3DXVECTOR3 Scale,CObjectX::ROTTYPE RotType);
	static int GetNumFile() { return m_nNumFile; }                                                                 //�t�@�C�������擾����
private:

	static const char* m_BLOCK_FILENAME[MODELTYPE_MAX];                                    //�u���b�N�̃t�@�C���l�[��
	MODELTYPE m_Type;                                                                      //���f�����
	static int m_nNumFile;                                                                 //�t�@�C�������i�[����
};
//==============================================================================================================================================================

#endif

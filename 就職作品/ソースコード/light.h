//========================================================
//
//�U���R���F�RD�I�u�W�F�N�g��`�悷��[light.h]
//Author:ShinaTaiyo
//
//=========================================================
#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "main.h"
//=========================================================
//���C�g�N���X
//=========================================================
class CLight
{
public:
	CLight();      //�R���X�g���N�^
	~CLight();     //�f�X�g���N�^
	HRESULT Init();//����������
	void Uninit(); //�I������
	void Update(); //�X�V����
private:
	static const int m_nMAXLIGHT = 6;//���C�g�̍ő吔
	D3DLIGHT9 m_aLight[m_nMAXLIGHT] = {};//!
};
#endif

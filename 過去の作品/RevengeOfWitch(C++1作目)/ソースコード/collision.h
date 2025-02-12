//====================================================
//
//�X���P�R���F�����蔻��p�̃N���X�����[collision.h]
//Author:ShinaTaiyo
//
//====================================================

#ifndef _COLLISION_H_  
#define _COLLISION_H_

//======================
//�C���N���[�h
//======================
#include "main.h"
//==========================================

//==========================================

//==========================================
//�����蔻��N���X
//==========================================
class CCollision
{
public:
	CCollision();                   //�R���X�g���N�^
	~CCollision();                  //�f�X�g���N�^

	//�����`�̓����蔻��
	static bool CollisionSquare(D3DXVECTOR3 MyPos, D3DXVECTOR3 MyVtxMax, D3DXVECTOR3 MyVtxMin,
		D3DXVECTOR3 ComparisonPos, D3DXVECTOR3 ComparisonVtxMax, D3DXVECTOR3 ComparisonVtxMin);

	//�~�̓����蔻��
	static bool CollisionBall(D3DXVECTOR3 MySenterPos, D3DXVECTOR3 MySize,
		D3DXVECTOR3 ComparisonSenterPos, D3DXVECTOR3 ComparisonSize);

	//�΂߂̓����蔻��XY
	static bool RectAngleCollisionXY(D3DXVECTOR3 MyPos, D3DXVECTOR3 MyVtxMax, D3DXVECTOR3 MyVtxMin, D3DXVECTOR3 Rot,
		D3DXVECTOR3 ComparisonPos, D3DXVECTOR3 ComparisonVtxMax, D3DXVECTOR3 ComparisonVtxMin, D3DXVECTOR3 ComparisonRot);

private:
};
#endif

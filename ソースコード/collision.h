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
#include "objectX.h"
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

	//�_��AABB�Ɠ����������ǂ����𔻒�
	static bool CollisionSquarePoint(const D3DXVECTOR3 & Point,CObjectX* pObjX);

	//�~�̓����蔻��
	static bool CollisionBall(D3DXVECTOR3 MySenterPos, D3DXVECTOR3 MySize,
		D3DXVECTOR3 ComparisonSenterPos, D3DXVECTOR3 ComparisonSize);

	//�΂߂̓����蔻��XY
	static bool RectAngleCollisionXY(D3DXVECTOR3 MyPos, D3DXVECTOR3 MyVtxMax, D3DXVECTOR3 MyVtxMin, D3DXVECTOR3 Rot,
		D3DXVECTOR3 ComparisonPos, D3DXVECTOR3 ComparisonVtxMax, D3DXVECTOR3 ComparisonVtxMin);

	//�΂߂̓����蔻��XZ
	static bool RectAngleCollisionXZ(CObjectX* pMyObj, CObjectX* pComObj);

	//�_��AABB�̒��ɓ����Ă��邩�ǂ���
	static bool IsPointInsideAABB(const D3DXVECTOR3 & Point,CObjectX * pComObjX);

	//==============================================
	//�����`�̉����o������
	//==============================================

	//�����`�̐V����X�����o������
	static void ResolveExtrusionCollisionSquare(CObjectX* pObjX, CObjectX* pComObjX); //�����`�̓����蔻�����������
	static void ExtrusionCollisionSquarePushOutFirstDecide(CObjectX* pObjX, CObjectX* pComObjX);//�����`�̉����o������̂��ꂼ��̎��̉����o���̗D��x�����߂�

	static bool NewExtrusionCollisionSquareX(CObjectX* pObjX, CObjectX* pComObjX);//X�����̉����o������
	static bool NewExtrusionCollisionSquareY(CObjectX* pObjX, CObjectX* pComObjX);//Y�����̉����o������
	static bool NewExtrusionCollisionSquareZ(CObjectX* pObjX, CObjectX* pComObjX);//Z�����̉����o������
	//=============================================================================================================================================================================

	//================================================
	//���C
	//================================================
	
	//���C��aabb�̓����蔻��Ɣ���ӏ������߂�
	static bool RayIntersectsAABBCollisionPos(const D3DXVECTOR3& origin, const D3DXVECTOR3& direction, const D3DXVECTOR3& min, const D3DXVECTOR3& max,
		D3DXVECTOR3 & CollisionPos);
	//==============================================================================================================================================================================


private:
};
#endif

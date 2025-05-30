//====================================================================================================================
//
//�X���P�R���F�����蔻��p�̃N���X�����[collision.cpp]
//Author:ShinaTaiyo
//
//====================================================================================================================

//================================================================
//�C���N���[�h
//================================================================
#include "collision.h"
#include "calculation.h"
//====================================================================================================================

//================================================================
//�R���X�g���N�^
//================================================================
CCollision::CCollision()
{

}
//====================================================================================================================

//================================================================
//�f�X�g���N�^
//================================================================
CCollision::~CCollision()
{

}
//====================================================================================================================

//================================================================
//�����`�̓����蔻������
//================================================================
bool CCollision::CollisionSquare(D3DXVECTOR3 MyPos, D3DXVECTOR3 MyVtxMax, D3DXVECTOR3 MyVtxMin,
	D3DXVECTOR3 ComparisonPos, D3DXVECTOR3 ComparisonVtxMax, D3DXVECTOR3 ComparisonVtxMin)
{
	if (MyPos.x + MyVtxMin.x <= ComparisonPos.x + ComparisonVtxMax.x &&
		MyPos.x + MyVtxMax.x >= ComparisonPos.x + ComparisonVtxMin.x &&
		MyPos.y + MyVtxMin.y <= ComparisonPos.y + ComparisonVtxMax.y &&
		MyPos.y + MyVtxMax.y >= ComparisonPos.y + ComparisonVtxMin.y &&
		MyPos.z + MyVtxMin.z <= ComparisonPos.z + ComparisonVtxMax.x &&
		MyPos.z + MyVtxMax.z >= ComparisonPos.z + ComparisonVtxMin.x)
	{
		return true;
	}
	return false;
}
//====================================================================================================================

//================================================================
//�����`�̓����蔻������
//================================================================
bool CCollision::CollisionBall(D3DXVECTOR3 MySenterPos, D3DXVECTOR3 MySize,
	D3DXVECTOR3 ComparisonSenterPos, D3DXVECTOR3 ComparisonSize)
{
	float fResultMySize = 0.0f;
	float fResultComprisonSize = 0.0f;
	if (MySize.x > MySize.y)
	{//�T�C�Y��X�̕����傫��
		fResultMySize = MySize.x;
	}
	else
	{//�T�C�Y��Y�̕����傫��
		fResultMySize = MySize.y;
	}

	if (ComparisonSize.x > ComparisonSize.y)
	{//�T�C�Y��X�̕����傫��
		fResultComprisonSize = ComparisonSize.x;
	}
	else
	{//�T�C�Y��Y�̕����傫��
		fResultComprisonSize = ComparisonSize.y;
	}

	float fCheckLength = fResultMySize / 2 + fResultComprisonSize / 2;

	//���S�_����̋����@�{�@���f���̒[�܂ł̋���
	float fLength = CCalculation::CalculationLength(MySenterPos, ComparisonSenterPos);

	if (fLength < fCheckLength)//��̔��a�̘a��苗�������������
	{
		return true;
	}

	return false;
}
//====================================================================================================================

//================================================================
//�΂߂̓����蔻��XY
//================================================================
bool CCollision::RectAngleCollisionXY(D3DXVECTOR3 MyPos, D3DXVECTOR3 MyVtxMax, D3DXVECTOR3 MyVtxMin, D3DXVECTOR3 Rot, D3DXVECTOR3 ComparisonPos, D3DXVECTOR3 ComparisonVtxMax, D3DXVECTOR3 ComparisonVtxMin, D3DXVECTOR3 ComparisonRot)
{
	D3DXVECTOR2 FourVtxPos[4] = {};   //�S���_�̈ʒu
	D3DXVECTOR2 FourVtxRotPos[4] = {};//��]�����S���_�̈ʒu
	D3DXVECTOR2 FourComparisonVtxPos[4] = {};    //��r�p�S���_
	bool bCollision = true;
	bool bEnd = false;        //���[�v���I��点��t���O

	//=====================================
	//��r�Ώۂ̎l�p�`�̂S���_
	//=====================================
	FourComparisonVtxPos[0].x = ComparisonPos.x + ComparisonVtxMin.x;
	FourComparisonVtxPos[0].y = ComparisonPos.y + ComparisonVtxMax.y;

	FourComparisonVtxPos[1].x = ComparisonPos.x + ComparisonVtxMax.x;
	FourComparisonVtxPos[1].y = ComparisonPos.y + ComparisonVtxMax.y;

	FourComparisonVtxPos[2].x = ComparisonPos.x + ComparisonVtxMin.x;
	FourComparisonVtxPos[2].y = ComparisonPos.y + ComparisonVtxMin.y;

	FourComparisonVtxPos[3].x = ComparisonPos.x + ComparisonVtxMax.x;
	FourComparisonVtxPos[3].y = ComparisonPos.y + ComparisonVtxMin.y;

	//=============================================================================================================================================================================

	//=====================================
	//�U���̎l�p�`�̂S���_
	//=====================================

	//���㒸�_
	FourVtxPos[0] = D3DXVECTOR2(MyPos.x + MyVtxMin.x, MyPos.y + MyVtxMax.y);

	//�E�㒸�_
	FourVtxPos[1] = D3DXVECTOR2(MyPos.x + MyVtxMax.x, MyPos.y + MyVtxMax.y);

	//�������_
	FourVtxPos[2] = D3DXVECTOR2(MyPos.x + MyVtxMin.x, MyPos.y + MyVtxMin.y);

	//�E�����_
	FourVtxPos[3] = D3DXVECTOR2(MyPos.x + MyVtxMax.x, MyPos.y + MyVtxMin.y);

	//=============================================================================================================================================================================

	//==================================
	//���S�_�ƂS���_�̋��������߂�
	//==================================
	float fLength[4] = {};
	for (int nCnVtx = 0; nCnVtx < 4; nCnVtx++)
	{
		fLength[nCnVtx] = sqrtf(powf((FourVtxPos[nCnVtx].x - MyPos.x), 2) + powf((FourVtxPos[nCnVtx].y - MyPos.y), 2));
	}
	//=============================================================================================================================================================================

	//==================================
	//���S�_����S���_�̊p�x�����߂�
	//==================================
	float fVaim[4] = {};
	for (int nCntVtx = 0; nCntVtx < 4; nCntVtx++)
	{
		fVaim[nCntVtx] = atan2f(FourVtxPos[nCntVtx].x - MyPos.x, FourVtxPos[nCntVtx].y - MyPos.y);
	}
	//=============================================================================================================================================================================

	//==================================
	//��]���_�̈ʒu�����߂�
	//==================================
	for (int nCntVtx = 0; nCntVtx < 4; nCntVtx++)
	{
		//��������킹�邽�߁ARot��-��t����
		FourVtxRotPos[nCntVtx] = D3DXVECTOR2(sinf(-Rot.z + fVaim[nCntVtx]) * fLength[nCntVtx] + MyPos.x, cosf(-Rot.z + fVaim[nCntVtx]) * fLength[nCntVtx] + MyPos.y);
	}
	//=============================================================================================================================================================================

	//=====================================================
	//��̎l�p�`�̂��ꂼ��̕ӂ̕��������߂�
	//=====================================================
	float fNormalizeAim[4] = {};
	fNormalizeAim[0] = sqrtf(powf((FourVtxRotPos[0].x - FourVtxRotPos[1].x), 2) + powf((FourVtxRotPos[0].y - FourVtxRotPos[1].y), 2));
	fNormalizeAim[1] = sqrtf(powf((FourVtxRotPos[1].x - FourVtxRotPos[3].x), 2) + powf((FourVtxRotPos[1].y - FourVtxRotPos[3].y), 2));
	fNormalizeAim[2] = sqrtf(powf((FourComparisonVtxPos[0].x - FourComparisonVtxPos[1].x), 2) + powf((FourComparisonVtxPos[0].y - FourComparisonVtxPos[1].y), 2));
	fNormalizeAim[3] = sqrtf(powf((FourComparisonVtxPos[1].x - FourComparisonVtxPos[3].x), 2) + powf((FourComparisonVtxPos[1].y - FourComparisonVtxPos[3].y), 2));

	//���K���x�N�g�������߂�
	D3DXVECTOR2 sideDirs[] =
	{
		(FourVtxRotPos[0] - FourVtxRotPos[1]) / fNormalizeAim[0], (FourVtxRotPos[1] - FourVtxRotPos[3]) / fNormalizeAim[1],
		(FourComparisonVtxPos[0] - FourComparisonVtxPos[1]) / fNormalizeAim[2], (FourComparisonVtxPos[1] - FourComparisonVtxPos[3]) / fNormalizeAim[3]
	};
	//==============================================================================================================================================================================


	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		float minA = D3DXVec2Dot(&FourVtxRotPos[0], &sideDirs[nCnt]);
		float maxA = minA;
		float minB = D3DXVec2Dot(&FourComparisonVtxPos[0], &sideDirs[nCnt]);
		float maxB = minB;

		for (int i = 1; i < 4; i++)
		{
			float projA = D3DXVec2Dot(&FourVtxRotPos[i], &sideDirs[nCnt]);

			if (projA < minA) { minA = projA; }
			else if (projA > maxA) { maxA = projA; }

			float projB = D3DXVec2Dot(&FourComparisonVtxPos[i], &sideDirs[nCnt]);

			if (projB < minB) { minB = projB; }
			else if (projB > maxB) { maxB = projB; }
		}

		if (maxB < minA || maxA < minB)
		{
			//return false;
			bCollision = false;
		}
	}

	//==================================================================
	//Z�͈͓̔��ɂ��邩�ǂ���
	//==================================================================
	if (ComparisonPos.z + ComparisonVtxMax.z >= MyPos.z + MyVtxMin.z &&
		ComparisonPos.z + ComparisonVtxMin.z <= MyPos.z + MyVtxMax.z && bCollision == true)
	{
		return true;
	}
	else
	{
	   return false;
	}
	//======================================================================================================
}
//====================================================================================================================
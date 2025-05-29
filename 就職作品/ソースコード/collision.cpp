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
#include "manager.h"
#include "calculation.h"
#include "particle.h"
#include "debugtext.h"
#include "camera.h"
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
		MyPos.z + MyVtxMin.z <= ComparisonPos.z + ComparisonVtxMax.z &&
		MyPos.z + MyVtxMax.z >= ComparisonPos.z + ComparisonVtxMin.z)
	{//�����`�͈͓̔��ɓ����Ă�����
		return true;
	}
	return false;
}

//================================================================
//�_�Ɛ����`�̓����蔻������
//================================================================
bool CCollision::CollisionSquarePoint(const D3DXVECTOR3 & Point,CObjectX* pObjX)
{
	if (Point.x <= pObjX->GetPosInfo().GetPos().x + pObjX->GetSizeInfo().GetVtxMax().x &&
		Point.x >= pObjX->GetPosInfo().GetPos().x + pObjX->GetSizeInfo().GetVtxMin().x &&
		Point.y <= pObjX->GetPosInfo().GetPos().y + pObjX->GetSizeInfo().GetVtxMax().y &&
		Point.y >= pObjX->GetPosInfo().GetPos().y + pObjX->GetSizeInfo().GetVtxMin().y &&
		Point.z <= pObjX->GetPosInfo().GetPos().z + pObjX->GetSizeInfo().GetVtxMax().z &&
		Point.z >= pObjX->GetPosInfo().GetPos().z + pObjX->GetSizeInfo().GetVtxMin().z)
	{//�_�������`�͈͓̔��ɓ����Ă�����
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
	float fResultMySize = 0.0f;        //�����̃T�C�Y
	float fResultComprisonSize = 0.0f; //����̃T�C�Y

	//====================================================================
	//X���̃T�C�Y��Y���̃T�C�Y���ׁA�傫���ق��𔼌a�Ƃ��Ē�`����
	//====================================================================
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
	//===============================================================================================================================-

	float fCheckLength = fResultMySize / 2 + fResultComprisonSize / 2;//���݂��̃��f���̒��S�_����̋����𑫂�

	//���݂��̃��f���̒��S�_����̋��������߂�
	float fLength = CCalculation::CalculationLength(MySenterPos, ComparisonSenterPos);

	if (fLength < fCheckLength)
	{//��̔��a�̘a��苗�������������true��Ԃ�
		return true;
	}

	return false;
}
//====================================================================================================================

//================================================================
//�΂߂̓����蔻��XY
//================================================================
bool CCollision::RectAngleCollisionXY(D3DXVECTOR3 MyPos, D3DXVECTOR3 MyVtxMax, D3DXVECTOR3 MyVtxMin, D3DXVECTOR3 Rot,
	D3DXVECTOR3 ComparisonPos, D3DXVECTOR3 ComparisonVtxMax, D3DXVECTOR3 ComparisonVtxMin)
{
	D3DXVECTOR2 FourVtxPos[4] = {};   //�S���_�̈ʒu
	D3DXVECTOR2 FourVtxRotPos[4] = {};//��]�����S���_�̈ʒu
	D3DXVECTOR2 FourComparisonVtxPos[4] = {};    //��r�p�S���_
	bool bCollision = true;

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

//================================================================
//�΂߂̓����蔻��XZ
//================================================================
bool CCollision::RectAngleCollisionXZ(CObjectX* pMyObj, CObjectX* pComObj)
{
	D3DXVECTOR2 FourVtxPos[4] = {};                                    //�S���_�̈ʒu
	D3DXVECTOR2 FourVtxRotPos[4] = {};                                 //��]�����S���_�̈ʒu
	D3DXVECTOR2 FourComparisonVtxPos[4] = {};                          //��r�p�S���_
	D3DXVECTOR3 MyPos = pMyObj->GetPosInfo().GetPos();                 //�����̈ʒu
	D3DXVECTOR3 MyVtxMin = pMyObj->GetSizeInfo().GetVtxMin();          //�����̍ŏ����_
	D3DXVECTOR3 MyVtxMax = pMyObj->GetSizeInfo().GetVtxMax();          //�����̍ő咸�_
	D3DXVECTOR3 MyRot = pMyObj->GetRotInfo().GetRot();                 //�����̌���
	D3DXVECTOR3 ComparisonPos = pComObj->GetPosInfo().GetPos();        //����̈ʒu
	D3DXVECTOR3 ComparisonVtxMin = pComObj->GetSizeInfo().GetVtxMin(); //����̍ŏ����_
	D3DXVECTOR3 ComparisonVtxMax = pComObj->GetSizeInfo().GetVtxMax(); //����̍ő咸�_

	//=====================================
	//��r�Ώۂ̎l�p�`�̂S���_
	//=====================================

	//����[0]
	FourComparisonVtxPos[0].x = ComparisonPos.x + ComparisonVtxMin.x;
	FourComparisonVtxPos[0].y = ComparisonPos.z + ComparisonVtxMax.z;

	//�E��[1]
	FourComparisonVtxPos[1].x = ComparisonPos.x + ComparisonVtxMax.x;
	FourComparisonVtxPos[1].y = ComparisonPos.z + ComparisonVtxMax.z;

	//����[2]
	FourComparisonVtxPos[2].x = ComparisonPos.x + ComparisonVtxMin.x;
	FourComparisonVtxPos[2].y = ComparisonPos.z + ComparisonVtxMin.z;

	//�E��[3]
	FourComparisonVtxPos[3].x = ComparisonPos.x + ComparisonVtxMax.x;
	FourComparisonVtxPos[3].y = ComparisonPos.z + ComparisonVtxMin.z;

	//=============================================================================================================================================================================

	//=====================================
	//�������g�̂S���_
	//=====================================

	//����[0]
	FourVtxPos[0] = D3DXVECTOR2(MyPos.x + MyVtxMin.x, MyPos.z + MyVtxMax.z);

	//�E��[1]
	FourVtxPos[1] = D3DXVECTOR2(MyPos.x + MyVtxMax.x, MyPos.z + MyVtxMax.z);

	//����[2]
	FourVtxPos[2] = D3DXVECTOR2(MyPos.x + MyVtxMin.x, MyPos.z + MyVtxMin.z);

	//�E��[3]
	FourVtxPos[3] = D3DXVECTOR2(MyPos.x + MyVtxMax.x, MyPos.z + MyVtxMin.z);

	//=============================================================================================================================================================================

	//==================================
	//�������g�̒��S�_�ƂS���_�̋��������߂�
	//==================================
	float fLength[4] = {};//���S�_�ƂS���_�̋�����������
	for (int nCnVtx = 0; nCnVtx < 4; nCnVtx++)
	{//�����̏�����XZ���ʂȂ̂ŁA�S���_��Y��Z�ƒ�`����
		fLength[nCnVtx] = sqrtf(powf((FourVtxPos[nCnVtx].x - MyPos.x), 2) + powf((FourVtxPos[nCnVtx].y - MyPos.z), 2));
	}
	//=============================================================================================================================================================================

	//==================================
	//�������g�̒��S�_����S���_�̊p�x�����߂�
	//==================================
	float fVaim[4] = {};
	for (int nCntVtx = 0; nCntVtx < 4; nCntVtx++)
	{//�����̏�����XZ���ʂȂ̂ŁA�S���_��Y��Z�ƒ�`����
		fVaim[nCntVtx] = atan2f(FourVtxPos[nCntVtx].x - MyPos.x, FourVtxPos[nCntVtx].y - MyPos.z);
	}
	//=============================================================================================================================================================================

	//==================================
	//��]���_�̈ʒu�����߂�
	//==================================
	for (int nCntVtx = 0; nCntVtx < 4; nCntVtx++)
	{//4���_�̒��S�_����̊p�x�������̌�������ɂ��A����𓥂܂����S���_�̈ʒu�����߂�
		FourVtxRotPos[nCntVtx] = D3DXVECTOR2(sinf(MyRot.y + fVaim[nCntVtx]) * fLength[nCntVtx] + MyPos.x, cosf(MyRot.y + fVaim[nCntVtx]) * fLength[nCntVtx] + MyPos.z);
	}
	//=============================================================================================================================================================================

	//=====================================================
	//��̎l�p�`�̂��ꂼ��̕ӂ̕��������߂�
	//=====================================================
	float fNormalizeAim[4] = {};//�����Ƒ����XZ���ʂ̃|���S���̉������Əc�����̃x�N�g���𐳋K������
	fNormalizeAim[0] = sqrtf(powf((FourVtxRotPos[0].x - FourVtxRotPos[1].x), 2) + powf((FourVtxRotPos[0].y - FourVtxRotPos[1].y), 2));//�����̉�����
	fNormalizeAim[1] = sqrtf(powf((FourVtxRotPos[1].x - FourVtxRotPos[3].x), 2) + powf((FourVtxRotPos[1].y - FourVtxRotPos[3].y), 2));//�����̏c����
	fNormalizeAim[2] = sqrtf(powf((FourComparisonVtxPos[0].x - FourComparisonVtxPos[1].x), 2) + powf((FourComparisonVtxPos[0].y - FourComparisonVtxPos[1].y), 2));//����̉�����
	fNormalizeAim[3] = sqrtf(powf((FourComparisonVtxPos[1].x - FourComparisonVtxPos[3].x), 2) + powf((FourComparisonVtxPos[1].y - FourComparisonVtxPos[3].y), 2));//����̏c����

	//���K���x�N�g�������߂�
	D3DXVECTOR2 sideDirs[] =
	{
		(FourVtxRotPos[0] - FourVtxRotPos[1]) / fNormalizeAim[0], (FourVtxRotPos[1] - FourVtxRotPos[3]) / fNormalizeAim[1],
		(FourComparisonVtxPos[0] - FourComparisonVtxPos[1]) / fNormalizeAim[2], (FourComparisonVtxPos[1] - FourComparisonVtxPos[3]) / fNormalizeAim[3]
	};
	//==============================================================================================================================================================================

	//=====================================================
	//�������藝
	// ���������藝�Ƃ́E�E�E�u�Q�̕��̂��Փ˂��Ă��Ȃ��ꍇ�A
	//�K�������̂����ꂩ�̕ӂɐ����Ȏ��i�������j�����݂��A
	//���e�������ɂ��̎���ŏd�Ȃ�Ȃ��v�Ƃ������_�B
	// �܂�E�E�E
	//�E�S�Ă̕������ɂ����āA���e���d�Ȃ��Ă����灨�Փ˂��Ă���
	//�E�P�ł��d�Ȃ�Ȃ������������灨�Փ˂��Ă��Ȃ��i���ꂪ"������"�j
	//=====================================================
	for (int nCnt = 0; nCnt < 4; nCnt++) // �S4���i�����Ƒ���̊e�ӂ̐������j�ɑ΂��ă��[�v
	{
		// �����̃|���S���̍ŏ��̒��_�����݂̎��ɐ��ˉe�i���ρj���AminA�����maxA��������
		float minA = D3DXVec2Dot(&FourVtxRotPos[0], &sideDirs[nCnt]);
		float maxA = minA;

		// ����̃|���S���̍ŏ��̒��_�����l�Ɏ��ɐ��ˉe���AminB�����maxB��������
		float minB = D3DXVec2Dot(&FourComparisonVtxPos[0], &sideDirs[nCnt]);
		float maxB = minB;

		// �����̎c���3���_�����݂̎��ɓ��e���āA�ŏ��l�ƍő�l���X�V
		for (int i = 1; i < 4; i++)
		{
			float projA = D3DXVec2Dot(&FourVtxRotPos[i], &sideDirs[nCnt]);
			if (projA < minA)
			{// ���e���̍ŏ��_A�X�V
				minA = projA; 
			}
			else if (projA > maxA) 
			{// ���e���̍ő�_A�X�V
				maxA = projA;
			}

			// ����̒��_�����l�ɓ��e���A�ŏ��E�ő�l���X�V
			float projB = D3DXVec2Dot(&FourComparisonVtxPos[i], &sideDirs[nCnt]);
			if (projB < minB) 
			{// ���e���̍ŏ��_B�X�V
				minB = projB;
			}
			else if (projB > maxB) 
			{// ���e���̍ő�_B�X�V
				maxB = projB;
			}
		}

		//������������̂œ������Ă��Ȃ�
		if (maxB < minA || maxA < minB)
		{
			return false;
		}
	}
	//==================================================================
	//Z�͈͓̔��ɂ��邩�ǂ���
	//==================================================================
	if (ComparisonPos.y + ComparisonVtxMax.y >= MyPos.y + MyVtxMin.y &&
		ComparisonPos.y + ComparisonVtxMin.y <= MyPos.y + MyVtxMax.y)
	{
		return true;
	}
	else
	{
		return false;
	}
	//======================================================================================================
}

//================================================================
//XZ������OBB�̓����蔻��
//================================================================
bool CCollision::RectOBB_XZ(CObjectX* pMyObj, CObjectX* pComObj)
{
	CObjectX::PosInfo& MyPosInfo = pMyObj->GetPosInfo();      //�������g�̈ʒu���
	CObjectX::RotInfo& MyRotInfo = pMyObj->GetRotInfo();      //�������g�̌������
	CObjectX::SizeInfo& MySizeInfo = pMyObj->GetSizeInfo();   //�������g�̃T�C�Y���

	CObjectX::PosInfo& ComPosInfo = pComObj->GetPosInfo();    //����̈ʒu���
	CObjectX::RotInfo& ComRotInfo = pComObj->GetRotInfo();    //����̌������
	CObjectX::SizeInfo& ComSizeInfo = pComObj->GetSizeInfo(); //����̃T�C�Y���

	const D3DXVECTOR3& MyPos = MyPosInfo.GetPos();            //�������g�̈ʒu
	const D3DXVECTOR3& MyRot = MyRotInfo.GetRot();            //�������g�̌���
	const D3DXVECTOR3& MyVtxMax = MySizeInfo.GetVtxMax();     //�������g�̍ő咸�_
	const D3DXVECTOR3& MyVtxMin = MySizeInfo.GetVtxMin();     //�������g�̍ŏ����_
	const D3DXVECTOR3& MySize = MySizeInfo.GetSize();         //�������g�̃T�C�Y�擾
	float fAdjustLength = MyVtxMax.z - MySize.z;              // ���S�_�𒲐����鋗��   
	const D3DXVECTOR3 MySenterPos = MyPos + D3DXVECTOR3(sinf(MyRot.y) * fAdjustLength, 0.0f,cosf(fAdjustLength) * fAdjustLength);//Y�����l�����Ȃ��������g�̒��S�_ 
	D3DXVECTOR3 MySenterPosXZ = D3DXVECTOR3(MySenterPos.x,0.0f,MySenterPos.z);     //Y�����l�����Ȃ��������g�̒��S�_
	D3DXVECTOR3 MyPosXZ = D3DXVECTOR3(MyPos.x, 0.0f, MyPos.z);                                              // Y�����l�����Ȃ��������g�̈ʒu
	CParticle::Create(CParticle::TYPE::TYPE00_NORMAL, 100, 10.0f, 10.0f, MySenterPos, D3DXVECTOR3(0.0f, 5.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), true);


	const D3DXVECTOR3& ComPos = ComPosInfo.GetPos();             // ����̈ʒu
	const D3DXVECTOR3& ComRot = ComRotInfo.GetRot();             // ����̌������
	const D3DXVECTOR3& ComVtxMax = ComSizeInfo.GetVtxMax();      // ����̍ő咸�_
	const D3DXVECTOR3& ComVtxMin = ComSizeInfo.GetVtxMin();      // ����̍ŏ����_
	const D3DXVECTOR3& ComSize = ComSizeInfo.GetSize();          // ����̃T�C�Y
	const D3DXVECTOR3 ComSenterPos = ComPosInfo.GetSenterPos(); // ����̒��S�_
	D3DXVECTOR3 ComSenterPosXZ = D3DXVECTOR3(ComSenterPos.x, 0.0f, ComSenterPos.z);//Y�����l�����Ȃ�����̒��S
	CParticle::Create(CParticle::TYPE::TYPE00_NORMAL, 100, 10.0f, 10.0f, ComSenterPos, D3DXVECTOR3(0.0f, 5.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), true);//�f�o�b�O�p�p�[�e�B�N��

	CDebugText* pDebugText = CManager::GetDebugText();        //�f�o�b�O�\���̎擾

	D3DXVECTOR3 MyRightPosXZ = MySenterPosXZ + D3DXVECTOR3(sinf(MyRot.y + D3DX_PI * 0.5f) * (MySize.x / 2), 0.0f, cosf(MyRot.y + D3DX_PI * 0.5f) * (MySize.x / 2));//���S�_����E�[�ւ̈ʒu�����߂�

	//�f�o�b�O�p�̃p�[�e�B�N���i�����̉E�x�N�g���j
	CParticle::Create(CParticle::TYPE::TYPE00_NORMAL, 100, 10.0f, 10.0f,D3DXVECTOR3(MyRightPosXZ.x,MyPos.y, MyRightPosXZ.z), D3DXVECTOR3(0.0f, 5.0f, 0.0f), D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), true);

	D3DXVECTOR3 ComPosXZ = D3DXVECTOR3(ComPos.x, 0.0f, ComPos.z);               //Y�����l�����Ȃ�����̈ʒu

	D3DXVECTOR3 DirSenterXZ = ComSenterPosXZ - MySenterPosXZ;//Y�����l�����Ȃ����S�_���m�̃x�N�g��
	float fLength = CCalculation::CalculationLength(MySenterPosXZ, ComSenterPosXZ);//Y�����l�����Ȃ����S�_���m�̋���


	//��L�ŋ��߂��ʒu�Ƃ̃x�N�g�������߂�FEA1
	D3DXVECTOR3 EA1_RightVec = MyRightPosXZ - MySenterPosXZ;       //�|���S��A�E�x�N�g��(EA1)
	D3DXVECTOR3 NormalizeEA1_RightVec = { 0.0f,0.0f,0.0f };  //�|���S��A"���K��"�E�x�N�g��
	D3DXVec3Normalize(&NormalizeEA1_RightVec, &EA1_RightVec);//���K��

	//�����̌������l�������O���̈ʒu�����߂�i�O�����O�ɂ���̂ŁAcos��Z�Ƃ���)
	D3DXVECTOR3 MyUpPosXZ = MySenterPosXZ + D3DXVECTOR3(sinf(MyRot.y) * (MySize.z / 2), 0.0f, cosf(MyRot.y) * (MySize.z / 2));
	//�f�o�b�O�p�̃p�[�e�B�N���i�����̏�x�N�g���j
	CParticle::Create(CParticle::TYPE::TYPE00_NORMAL, 100, 10.0f, 10.0f, D3DXVECTOR3(MyUpPosXZ.x, MyPos.y, MyUpPosXZ.z), D3DXVECTOR3(0.0f, 5.0f, 0.0f), D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), true);

    //��L�ŋ��߂��ʒu�Ƃ̃x�N�g�������߂�FEA2
	D3DXVECTOR3 EA2_UpVec = MyUpPosXZ - MySenterPosXZ;   //�|���S��A��x�N�g��(EA2)
	D3DXVECTOR3 NormalizeEA2_UpVec = { 0.0f,0.0f,0.0f }; //�|���S��A�h���K���h��x�N�g��
	D3DXVec3Normalize(&NormalizeEA2_UpVec, &EA2_UpVec);  //���K��

	//����̌������l�������E���̈ʒu�����߂�i�O�����O�ɂ���̂ŁAcos��Z�Ƃ��A�E���Ȃ̂Łu+90�x�v)
	D3DXVECTOR3 ComRightPosXZ = ComSenterPosXZ + D3DXVECTOR3(sinf(ComRot.y + D3DX_PI * 0.5f) * (ComSize.x / 2), 0.0f, cosf(ComRot.y + D3DX_PI * 0.5f) * (ComSize.x / 2));
	//�f�o�b�O�p�̃p�[�e�B�N���i����̉E�x�N�g���j
    CParticle::Create(CParticle::TYPE::TYPE00_NORMAL, 100, 10.0f, 10.0f, D3DXVECTOR3(ComRightPosXZ.x, ComPos.y, ComRightPosXZ.z), D3DXVECTOR3(0.0f, 5.0f, 0.0f), D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), true);

	//��L�ŋ��߂��ʒu�Ƃ̃x�N�g�������߂�FEB1
	D3DXVECTOR3 EB1_RightVec = ComRightPosXZ - ComSenterPosXZ;       //�|���S��B�E�x�N�g��(EB1)
	D3DXVECTOR3 NormalizeEB1_RightVec = { 0.0f,0.0f,0.0f };    //�|���S��B"���K��"�E�x�N�g��
	D3DXVec3Normalize(&NormalizeEB1_RightVec, &EB1_RightVec);  //���K��

	//����̌������l�������O���̈ʒu�����߂�i�O�����O�ɂ���̂ŁAcos��Z�Ƃ���)
	D3DXVECTOR3 ComUpPosXZ = ComSenterPosXZ + D3DXVECTOR3(sinf(ComRot.y) * (ComSize.z / 2), 0.0f, cosf(ComRot.y) * (ComSize.z / 2));

	//�f�o�b�O�p�̃p�[�e�B�N���i����̏�x�N�g���j
	CParticle::Create(CParticle::TYPE::TYPE00_NORMAL, 100, 10.0f, 10.0f, D3DXVECTOR3(ComUpPosXZ.x, ComPos.y, ComUpPosXZ.z), D3DXVECTOR3(0.0f, 5.0f, 0.0f), D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), true);
	//��L�ŋ��߂��ʒu�Ƃ̃x�N�g�������߂�FEB2
	D3DXVECTOR3 EB2_UpVec = ComUpPosXZ - ComSenterPosXZ;    //�|���S��B��x�N�g��(EB2)
	D3DXVECTOR3 NormalizeEB2_UpVec = { 0.0f,0.0f,0.0f };    //�|���S��B"���K��"��x�N�g��
	D3DXVec3Normalize(&NormalizeEB2_UpVec, &EB2_UpVec);     //���K��
	//�S�Ē����ł�
	float rA[4] = {};    //�������ɑ΂��锼�aA
	float rB[4] = {};    //�������ɑ΂��锼�aB
	float L[4] = {};     //�������ɑ΂��鋗��

	//==========================================
	//�������P�F�|���S��A�E�x�N�g��(A.e1)
	//==========================================
	rA[0] = CCalculation::CalculationLength(MySenterPosXZ, MyRightPosXZ);//��������ł̒��S�_����̒�����rA���a�ɂȂ�
	rB[0] = fabsf(D3DXVec3Dot(&EB1_RightVec, &NormalizeEA1_RightVec)) + fabsf(D3DXVec3Dot(&EB2_UpVec, &NormalizeEA1_RightVec));//������EA1�Ƀ|���S��B�̂Q���𓊉e��������
	L[0] = fabsf(D3DXVec3Dot(&DirSenterXZ, &NormalizeEA1_RightVec));//���S�_�Ԃ̋����𕪗���EA1�ɓ��e

	pDebugText->PrintDebugText("rA[0] = %f\n", rA[0]);
	pDebugText->PrintDebugText("rB[0] = %f\n", rB[0]);
	pDebugText->PrintDebugText("L[0] = %f\n", L[0]);

	if (L[0] > (rA[0] + rB[0]))
	{//���e�������S�_�Ԃ̋������������e�����̍��v���傫����Ε����������݂��Ȃ��̂ŁA�������Ă���
		return false;
	}

	//==========================================
	//�������Q�F�|���S��A��x�N�g��(A.e2)
	//==========================================
	rA[1] = CCalculation::CalculationLength(MySenterPosXZ, MyUpPosXZ);
	rB[1] = fabsf(D3DXVec3Dot(&EB1_RightVec, &NormalizeEA2_UpVec)) + fabsf(D3DXVec3Dot(&EB2_UpVec, &NormalizeEA2_UpVec));//������EA2�Ƀ|���S��B�̂Q���𓊉e��������
	L[1] = fabsf(D3DXVec3Dot(&DirSenterXZ, &NormalizeEA2_UpVec));//���S�_�Ԃ̋����𕪗���EA2�ɓ��e

	pDebugText->PrintDebugText("rA[1] = %f\n", rA[1]);
	pDebugText->PrintDebugText("rB[1] = %f\n", rB[1]);
	pDebugText->PrintDebugText("L[1] = %f\n", L[1]);

	if (L[1] > (rA[1] + rB[1]))
	{//���e�������S�_�Ԃ̋������������e�����̍��v���傫����Ε����������݂��Ȃ��̂ŁA�������Ă���
		return false;
	}

	//==========================================
	//�������R�F�|���S��B�E�x�N�g��(B.e1)
	//==========================================
	rA[2] = fabsf(D3DXVec3Dot(&EA1_RightVec, &NormalizeEB1_RightVec)) + fabsf(D3DXVec3Dot(&EA2_UpVec, &NormalizeEB1_RightVec));//������EB1�Ƀ|���S��A�̂Q���𓊉e��������
	rB[2] = CCalculation::CalculationLength(ComSenterPosXZ, ComRightPosXZ);
	L[2] = fabsf(D3DXVec3Dot(&DirSenterXZ, &NormalizeEB1_RightVec));//���S�_�Ԃ̋����𕪗���EB1�ɓ��e
	pDebugText->PrintDebugText("rA[2] = %f\n", rA[2]);
	pDebugText->PrintDebugText("rB[2] = %f\n", rB[2]);
	pDebugText->PrintDebugText("L[2] = %f\n", L[2]);
	if (L[2] > (rA[2] + rB[2]))
	{//���e�������S�_�Ԃ̋������������e�����̍��v���傫����Ε����������݂��Ȃ��̂œ������Ă���
		return false;
	}
	
	//==========================================
	//�������S�F�|���S��B��x�N�g��(B.e2)
	//==========================================
	rA[3] = fabsf(D3DXVec3Dot(&EA1_RightVec, &NormalizeEB2_UpVec)) + fabsf(D3DXVec3Dot(&EA2_UpVec, &NormalizeEB2_UpVec));//������EB2�Ƀ|���S��A�̂Q���𓊉e��������
	rB[3] = CCalculation::CalculationLength(ComSenterPosXZ, ComUpPosXZ);
	L[3] = fabsf(D3DXVec3Dot(&DirSenterXZ, &NormalizeEB2_UpVec));           //���S�_�Ԃ̋����𕪗���EB2�ɓ��e
	pDebugText->PrintDebugText("rA[3] = %f\n", rA[3]);
	pDebugText->PrintDebugText("rB[3] = %f\n", rB[3]);
	pDebugText->PrintDebugText("L[3] = %f\n", L[3]);
	if (L[3] > (rA[3] + rB[3]))
	{//���e�����S�_�Ԃ̋������������e�����̍��v���傫����Ε����������݂��Ȃ��̂œ������Ă��Ȃ�
		return false;
	}

	//==========================================
	//Y���̓����蔻����m�F
	//==========================================
	if (MyPos.y + MyVtxMax.y > ComPos.y + MyVtxMin.y &&
		MyPos.y + MyVtxMin.y < ComPos.y + MyVtxMax.y)
	{
		return true;//�Փ˂��Ă���i�Q�̃|���S���̕����������݂��Ȃ��̂œ������Ă���)6
	}
	else
	{
		return false;//�Փ˂��Ă��Ȃ��iY������)
	}
}
//====================================================================================================================

//================================================================
//�_��AABB�̒��ɓ����Ă��邩�ǂ���
//================================================================
bool CCollision::IsPointInsideAABB(const D3DXVECTOR3& Point, CObjectX* pComObjX)
{
	if (Point.x >= pComObjX->GetPosInfo().GetPos().x + pComObjX->GetSizeInfo().GetVtxMin().x &&
		Point.x <= pComObjX->GetPosInfo().GetPos().x + pComObjX->GetSizeInfo().GetVtxMax().x &&
		Point.y >= pComObjX->GetPosInfo().GetPos().y + pComObjX->GetSizeInfo().GetVtxMin().y &&
		Point.y <= pComObjX->GetPosInfo().GetPos().y + pComObjX->GetSizeInfo().GetVtxMax().y &&
		Point.z >= pComObjX->GetPosInfo().GetPos().z + pComObjX->GetSizeInfo().GetVtxMin().z &&
		Point.z <= pComObjX->GetPosInfo().GetPos().z + pComObjX->GetSizeInfo().GetVtxMax().z)
	{//�_��AABB�̒��ɓ����Ă�����
		return true;
	}
	return false;
}
//====================================================================================================================

//==============================================================
//�����`�̑S�Ẳ����o���������������֐��iExtrusionCollisionSquarePushOutFirstDecide�֐���1f��̈ʒu���ǂ̎��ɓ����邩�����߁A�Փ˔�����s�����̏��Ԃ����߂āA�����|�����j�~����)
//==============================================================
void CCollision::ResolveExtrusionCollisionSquare(CObjectX* pObjX, CObjectX* pComObjX)
{
	if (pObjX->GetCollisionInfo().GetState().GetLandingOld())
	{
		CCollision::NewExtrusionCollisionSquareY(pObjX, pComObjX);
		if (pObjX->GetCollisionInfo().GetSquareInfo().GetPushOutFlag(AXIS::X) == true)
		{
			CCollision::NewExtrusionCollisionSquareX(pObjX, pComObjX);
			CCollision::NewExtrusionCollisionSquareZ(pObjX, pComObjX);
		}
		else
		{
			CCollision::NewExtrusionCollisionSquareZ(pObjX, pComObjX);
			CCollision::NewExtrusionCollisionSquareX(pObjX, pComObjX);
		}
	}
	else if (pObjX->GetCollisionInfo().GetState().GetWallingOld())
	{
		if (pObjX->GetCollisionInfo().GetSquareInfo().GetPushOutFlag(AXIS::X) == true)
		{
			CCollision::NewExtrusionCollisionSquareX(pObjX, pComObjX);
			CCollision::NewExtrusionCollisionSquareZ(pObjX, pComObjX);
		}
		else
		{
			CCollision::NewExtrusionCollisionSquareZ(pObjX, pComObjX);
			CCollision::NewExtrusionCollisionSquareX(pObjX, pComObjX);
		}
		CCollision::NewExtrusionCollisionSquareY(pObjX, pComObjX);
	}
	else
	{
		CCollision::NewExtrusionCollisionSquareX(pObjX, pComObjX);
		CCollision::NewExtrusionCollisionSquareY(pObjX, pComObjX);
		CCollision::NewExtrusionCollisionSquareZ(pObjX, pComObjX);

	}
}
//====================================================================================================================

//==============================================================
//�����`�̉����o������̂��ꂼ��̎��̗D��x�����߂�
//==============================================================
void CCollision::ExtrusionCollisionSquarePushOutFirstDecide(CObjectX* pObjX, CObjectX* pComObjX)
{
	//*�ϐ�

	//����
	const D3DXVECTOR3& MyPos = pObjX->GetPosInfo().GetPos();              //�����̈ʒu
	const D3DXVECTOR3& MyPosOld = pObjX->GetPosInfo().GetPosOld();        //������1f�O�̈ʒu
	const D3DXVECTOR3& MyMove = pObjX->GetMoveInfo().GetMove();           //�����̈ړ���
	const D3DXVECTOR3& MyPosFuture = MyPos + MyMove;                      //������1f��̈ʒu
	const D3DXVECTOR3& MyVtxMax = pObjX->GetSizeInfo().GetVtxMax();       //�����̍ő咸�_
	const D3DXVECTOR3& MyVtxMin = pObjX->GetSizeInfo().GetVtxMin();       //�����̍ŏ����_

	//����
	const D3DXVECTOR3& ComPos = pComObjX->GetPosInfo().GetPos();          //����̈ʒu
	const D3DXVECTOR3& ComVtxMax = pComObjX->GetSizeInfo().GetVtxMax();   //����̍ő咸�_
	const D3DXVECTOR3& ComVtxMin = pComObjX->GetSizeInfo().GetVtxMin();   //����̍ŏ����_
	CObjectX::CollisionInfo& CollisionInfo = pObjX->GetCollisionInfo();   //�����蔻������擾����
	CObjectX::CollisionInfo::State& CollisionState = CollisionInfo.GetState();//�����蔻���Ԃ��擾����
	//*�����J�n
	
	//��
	if (MyPosFuture.x + MyVtxMax.x > ComPos.x + ComVtxMin.x
		&& MyPosFuture.x + MyVtxMin.x < ComPos.x + ComVtxMax.x
		&& MyPosFuture.y + MyVtxMin.y < ComPos.y + ComVtxMax.y
		&& MyPosOld.y + MyVtxMin.y >= ComPos.y + ComVtxMax.y
		&& MyPosFuture.z + MyVtxMax.z > ComPos.z + ComVtxMin.z
		&& MyPosFuture.z + MyVtxMin.z < ComPos.z + ComVtxMax.z)
	{//1f��ɃI�u�W�F�N�g�ɏ��Ȃ�
		CollisionInfo.GetSquareInfo().SetPushOutFirstFlag(AXIS::Y, true);
		CollisionInfo.GetSquareInfo().SetPushOutFirstFlag(AXIS::X, false);
		CollisionInfo.GetSquareInfo().SetPushOutFirstFlag(AXIS::Z, false);
	}
	//��
	else if (MyPosFuture.x + MyVtxMax.x > ComPos.x + ComVtxMin.x
		&& MyPosFuture.x + MyVtxMin.x < ComPos.x + ComVtxMax.x
		&& MyPosFuture.y + MyVtxMax.y > ComPos.y + ComVtxMin.y
		&& MyPosOld.y + MyVtxMax.y <= ComPos.y + ComVtxMin.y
		&& MyPosFuture.z + MyVtxMax.z > ComPos.z + ComVtxMin.z
		&& MyPosFuture.z + MyVtxMin.z < ComPos.z + ComVtxMax.z)
	{//1f��ɃI�u�W�F�N�g�̉��ɓ�����Ȃ�
		CollisionInfo.GetSquareInfo().SetPushOutFirstFlag(AXIS::Y, true);
		CollisionInfo.GetSquareInfo().SetPushOutFirstFlag(AXIS::X, false);
		CollisionInfo.GetSquareInfo().SetPushOutFirstFlag(AXIS::Z, false);
	}
	if (MyPosFuture.x + MyVtxMax.x > ComPos.x + ComVtxMin.x
		&& MyPosOld.x + MyVtxMax.x <= ComPos.x + ComVtxMin.x
		&& MyPosFuture.y + MyVtxMax.y > ComPos.y + ComVtxMin.y
		&& MyPosFuture.y + MyVtxMin.y < ComPos.y + ComVtxMax.y
		&& MyPosFuture.z + MyVtxMax.z > ComPos.z + ComVtxMin.z
		&& MyPosFuture.z + MyVtxMin.z < ComPos.z + ComVtxMax.z)
	{//1f��ɃI�u�W�F�N�g��X���̉E���ɓ�����Ȃ�
		CollisionInfo.GetSquareInfo().SetPushOutFirstFlag(AXIS::X, true);
		CollisionInfo.GetSquareInfo().SetPushOutFirstFlag(AXIS::Z, false);
	}
	else if (MyPosFuture.x + MyVtxMin.x < ComPos.x + ComVtxMax.x
		&& MyPosOld.x + MyVtxMin.x >= ComPos.x + ComVtxMax.x
		&& MyPosFuture.y + MyVtxMax.y > ComPos.y + ComVtxMin.y
		&& MyPosFuture.y + MyVtxMin.y < ComPos.y + ComVtxMax.y
		&& MyPosFuture.z + MyVtxMax.z > ComPos.z + ComVtxMin.z
		&& MyPosFuture.z + MyVtxMin.z < ComPos.z + ComVtxMax.z)
	{//1f��ɃI�u�W�F�N�g��X���̍����ɓ�����Ȃ�
		CollisionInfo.GetSquareInfo().SetPushOutFirstFlag(AXIS::X, true);
		CollisionInfo.GetSquareInfo().SetPushOutFirstFlag(AXIS::Z, false);
	}
	//�O
	if (MyPosFuture.z + MyVtxMax.z > ComPos.z + ComVtxMin.z
		&& MyPosOld.z + MyVtxMax.z <= ComPos.z + ComVtxMin.z
		&& MyPosFuture.y + MyVtxMax.y > ComPos.y + ComVtxMin.y
		&& MyPosFuture.y + MyVtxMin.y < ComPos.y + ComVtxMax.y
		&& MyPosFuture.x + MyVtxMax.x > ComPos.x + ComVtxMin.x
		&& MyPosFuture.x + MyVtxMin.x < ComPos.x + ComVtxMax.x)
	{//1f��ɃI�u�W�F�N�g�̎�O���ɓ�����Ȃ�
		CollisionInfo.GetSquareInfo().SetPushOutFirstFlag(AXIS::Z, true);
		CollisionInfo.GetSquareInfo().SetPushOutFirstFlag(AXIS::X, false);
	}
	//��
	else if (MyPosFuture.z + MyVtxMin.z < ComPos.z + ComVtxMax.z
		&& MyPosOld.z + MyVtxMin.z >= ComPos.z + ComVtxMax.z
		&& MyPosFuture.y + MyVtxMax.y > ComPos.y + ComVtxMin.y
		&& MyPosFuture.y + MyVtxMin.y < ComPos.y + ComVtxMax.y
		&& MyPosFuture.x + MyVtxMax.x > ComPos.x + ComVtxMin.x
		&& MyPosFuture.x + MyVtxMin.x < ComPos.x + ComVtxMax.x)
	{//1f��ɃI�u�W�F�N�g�̉����ɓ�����Ȃ�
		CollisionInfo.GetSquareInfo().SetPushOutFirstFlag(AXIS::Z, true);
		CollisionInfo.GetSquareInfo().SetPushOutFirstFlag(AXIS::X, false);
	}
}
//====================================================================================================================

//================================================================
//�V���������`�̉����o������X
//================================================================
bool CCollision::NewExtrusionCollisionSquareX(CObjectX* pObjX, CObjectX* pComObjX)
{
	const D3DXVECTOR3& MyPos = pObjX->GetPosInfo().GetPos();            //�����̈ʒu
	const D3DXVECTOR3& MyMove = pObjX->GetMoveInfo().GetMove();         //�����̈ړ���
	const D3DXVECTOR3& MyPosOld = pObjX->GetPosInfo().GetPosOld();      //������1f�O�̈ʒu
	const D3DXVECTOR3& MyVtxMax = pObjX->GetSizeInfo().GetVtxMax();     //�����̍ő咸�_
	const D3DXVECTOR3& MyVtxMin = pObjX->GetSizeInfo().GetVtxMin();     //�����̍ŏ����_

	const D3DXVECTOR3& ComPos = pComObjX->GetPosInfo().GetPos();        //����̈ʒu
	const D3DXVECTOR3& ComVtxMax = pComObjX->GetSizeInfo().GetVtxMax(); //����̍ő咸�_
	const D3DXVECTOR3& ComVtxMin = pComObjX->GetSizeInfo().GetVtxMin(); //����̍ŏ����_

	CObjectX::CollisionInfo& CollisionInfo = pObjX->GetCollisionInfo();        //�����蔻������擾����
	CObjectX::CollisionInfo::State& CollisionState = CollisionInfo.GetState(); //�����蔻���Ԃ��擾����

	if (MyPos.x + MyVtxMax.x > ComPos.x + ComVtxMin.x
		&& MyPosOld.x + MyVtxMax.x <= ComPos.x + ComVtxMin.x
		&& MyPos.y + MyVtxMax.y > ComPos.y + ComVtxMin.y
		&& MyPos.y + MyVtxMin.y < ComPos.y + ComVtxMax.y
		&& MyPos.z + MyVtxMax.z > ComPos.z + ComVtxMin.z
		&& MyPos.z + MyVtxMin.z < ComPos.z + ComVtxMax.z)
	{//�Ώۂ̉E�[���u���b�N�̍��[�ɓ����������̏���
		pObjX->GetPosInfo().SetPos(D3DXVECTOR3(ComPos.x + ComVtxMin.x - MyVtxMax.x - 0.1f,
			pObjX->GetPosInfo().GetPos().y,
			pObjX->GetPosInfo().GetPos().z));
		CollisionState.SetWalling(true);
		return true;
	}
	else if (MyPos.x + MyVtxMin.x < ComPos.x + ComVtxMax.x
		&& MyPosOld.x + MyVtxMin.x >= ComPos.x + ComVtxMax.x
		&& MyPos.y + MyVtxMax.y > ComPos.y + ComVtxMin.y
		&& MyPos.y + MyVtxMin.y < ComPos.y + ComVtxMax.y
		&& MyPos.z + MyVtxMax.z > ComPos.z + ComVtxMin.z
		&& MyPos.z + MyVtxMin.z < ComPos.z + ComVtxMax.z)
	{//�Ώۂ̍��[���u���b�N�̉E�[�ɓ����������̏���
		pObjX->GetPosInfo().SetPos(D3DXVECTOR3(ComPos.x + ComVtxMax.x - MyVtxMin.x + 0.1f,
			pObjX->GetPosInfo().GetPos().y,
			pObjX->GetPosInfo().GetPos().z));
		CollisionState.SetWalling(true);
		return true;
	}
	return false;
}
//====================================================================================================================

//================================================================
//�V���������`�̉����o������Y
//================================================================
bool CCollision::NewExtrusionCollisionSquareY(CObjectX* pObjX, CObjectX* pComObjX)
{
	const D3DXVECTOR3& MyPos = pObjX->GetPosInfo().GetPos();                   //�����̈ʒu
	const D3DXVECTOR3& MyMove = pObjX->GetMoveInfo().GetMove();                //�����̈ړ���
	const D3DXVECTOR3& MyPosOld = pObjX->GetPosInfo().GetPosOld();             //������1f�O�̈ʒu
	const D3DXVECTOR3& MyVtxMax = pObjX->GetSizeInfo().GetVtxMax();            //�����̍ő咸�_
	const D3DXVECTOR3& MyVtxMin = pObjX->GetSizeInfo().GetVtxMin();            //�����̍ŏ����_
																		       
	const D3DXVECTOR3& ComPos = pComObjX->GetPosInfo().GetPos();               //����̈ʒu
	const D3DXVECTOR3& ComVtxMax = pComObjX->GetSizeInfo().GetVtxMax();        //����̍ő咸�_
	const D3DXVECTOR3& ComVtxMin = pComObjX->GetSizeInfo().GetVtxMin();        //����̍ŏ����_
	CObjectX::CollisionInfo& CollisionInfo = pObjX->GetCollisionInfo();        //�����蔻������擾����
	CObjectX::CollisionInfo::State& CollisionState = CollisionInfo.GetState(); //�����蔻���Ԃ��擾����

	//��
	if (MyPos.x + MyVtxMax.x > ComPos.x + ComVtxMin.x
		&& MyPos.x + MyVtxMin.x < ComPos.x + ComVtxMax.x
		&& MyPos.y + MyVtxMin.y < ComPos.y + ComVtxMax.y
		&& MyPosOld.y + MyVtxMin.y - MyMove.y >= ComPos.y + ComVtxMax.y
		&& MyPos.z + MyVtxMax.z > ComPos.z + ComVtxMin.z
		&& MyPos.z + MyVtxMin.z < ComPos.z + ComVtxMax.z)
	{//�Ώۂ̉��[�����f���̏�[�ɓ����������̏���
		float fPosY = fabsf(MyVtxMin.y);
		//MyPos.y = ComPos.y + ComVtxMax.y + fPosY + 0.1f;
		pObjX->GetPosInfo().SetPos(D3DXVECTOR3(pObjX->GetPosInfo().GetPos().x,
			ComPos.y + ComVtxMax.y + fPosY + 0.1f,
			pObjX->GetPosInfo().GetPos().z));
		CollisionState.SetLanding(true);
		return true;
	}
	//��
	else if (MyPos.x + MyVtxMax.x > ComPos.x + ComVtxMin.x
		&& MyPos.x + MyVtxMin.x < ComPos.x + ComVtxMax.x
		&& MyPos.y + MyVtxMax.y > ComPos.y + ComVtxMin.y
		&& MyPosOld.y + MyVtxMax.y <= ComPos.y + ComVtxMin.y
		&& MyPos.z + MyVtxMax.z > ComPos.z + ComVtxMin.z
		&& MyPos.z + MyVtxMin.z < ComPos.z + ComVtxMax.z)
	{//�Ώۂ̏�[�����f���̉��[�ɓ����������̏���
		//MyPos.y = ComPos.y + ComVtxMin.y - MyVtxMax.y;
		pObjX->GetPosInfo().SetPos(D3DXVECTOR3(pObjX->GetPosInfo().GetPos().x,
			ComPos.y + ComVtxMin.y - MyVtxMax.y,
			pObjX->GetPosInfo().GetPos().z));
		return true;
	}

	return false;
}
//====================================================================================================================

//================================================================
//�V���������`�̉����o������Z
//================================================================
bool CCollision::NewExtrusionCollisionSquareZ(CObjectX* pObjX, CObjectX* pComObjX)
{
	const D3DXVECTOR3& MyPos = pObjX->GetPosInfo().GetPos();            //�����̈ʒu
	const D3DXVECTOR3& MyMove = pObjX->GetMoveInfo().GetMove();         //�����̈ړ���
	const D3DXVECTOR3& MyPosOld = pObjX->GetPosInfo().GetPosOld();      //������1f�O�̈ʒu
	const D3DXVECTOR3& MyVtxMax = pObjX->GetSizeInfo().GetVtxMax();     //�����̍ő咸�_
	const D3DXVECTOR3& MyVtxMin = pObjX->GetSizeInfo().GetVtxMin();     //�����̍ŏ����_

	const D3DXVECTOR3& ComPos = pComObjX->GetPosInfo().GetPos();        //����̈ʒu
	const D3DXVECTOR3& ComVtxMax = pComObjX->GetSizeInfo().GetVtxMax(); //����̍ő咸�_
	const D3DXVECTOR3& ComVtxMin = pComObjX->GetSizeInfo().GetVtxMin(); //����̍ŏ����_

	CObjectX::CollisionInfo& CollisionInfo = pObjX->GetCollisionInfo();        //�����蔻������擾����
	CObjectX::CollisionInfo::State& CollisionState = CollisionInfo.GetState(); //�����蔻���Ԃ��擾����

	if (MyPos.z + MyVtxMax.z > ComPos.z + ComVtxMin.z
		&& MyPosOld.z + MyVtxMax.z <= ComPos.z + ComVtxMin.z
		&& MyPos.y + MyVtxMax.y > ComPos.y + ComVtxMin.y
		&& MyPos.y + MyVtxMin.y < ComPos.y + ComVtxMax.y
		&& MyPos.x + MyVtxMax.x > ComPos.x + ComVtxMin.x
		&& MyPos.x + MyVtxMin.x < ComPos.x + ComVtxMax.x)
	{//�Ώۂ̉E�[���u���b�N�̍��[�ɓ����������̏���
		pObjX->GetPosInfo().SetPos(D3DXVECTOR3(pObjX->GetPosInfo().GetPos().x,
			pObjX->GetPosInfo().GetPos().y,
			ComPos.z + ComVtxMin.z - MyVtxMax.z - 0.1f));
		CollisionState.SetWalling(true);
		return true;
	}
	else if (MyPos.z + MyVtxMin.z < ComPos.z + ComVtxMax.z
		&& MyPosOld.z + MyVtxMin.z >= ComPos.z + ComVtxMax.z
		&& MyPos.y + MyVtxMax.y > ComPos.y + ComVtxMin.y
		&& MyPos.y + MyVtxMin.y < ComPos.y + ComVtxMax.y
		&& MyPos.x + MyVtxMax.x > ComPos.x + ComVtxMin.x
		&& MyPos.x + MyVtxMin.x < ComPos.x + ComVtxMax.x)
	{//�Ώۂ̍��[���u���b�N�̉E�[�ɓ����������̏���
		pObjX->GetPosInfo().SetPos(D3DXVECTOR3(pObjX->GetPosInfo().GetPos().x,
			pObjX->GetPosInfo().GetPos().y,
			ComPos.z + ComVtxMax.z - MyVtxMin.z + 0.1f));
		CollisionState.SetWalling(true);
		return true;
	}
	return false;
}
//====================================================================================================================

//================================================================
//���C��AABB�̓����蔻��A����ӏ������߂�i�S�Ă̎��̌����͈͂��ׂ�B�d�Ȃ��Ă���͈͂�����΁A���͈̔͂œ������Ă���j
//================================================================
bool CCollision::RayIntersectsAABBCollisionPos(const D3DXVECTOR3& origin,const D3DXVECTOR3& direction, const D3DXVECTOR3& min, const D3DXVECTOR3& max,
	D3DXVECTOR3& CollisionPos)
{
	D3DXVECTOR3 CopyDirection = direction;

	if (origin.x >= min.x && origin.x <= max.x &&
		origin.y >= min.y && origin.y <= max.y &&
		origin.z >= min.z && origin.z <= max.z)
	{//���C�̋N�_���{�b�N�X�̓����ɂ���ꍇ
		return false;
	}

	if (CopyDirection.x == 0.0f)
	{
		CopyDirection.x = 0.001f;//�O���Z�����
	}
	if (CopyDirection.y == 0.0f)
	{
		CopyDirection.y = 0.001f;//�O���Z�����
	}
	if (CopyDirection.z == 0.0f)
	{
		CopyDirection.z = 0.001f;//�O���Z�����
	}

	//�����ꂼ��̍ő咸�_�A�ŏ����_�i���̕��ʁj�Ƃ̌����͈͂����߁A��ԋ߂��ʂƂ̋��������߂�

	//X���̖ʂƂ̌����͈͂����߂�
	float tmin = (min.x - origin.x) / CopyDirection.x;
	float tmax = (max.x - origin.x) / CopyDirection.x;

	float t = 0.0f;

	//tmax����tmin�̕����������傫����΁A�ŏ��ő�̊֌W������Ȃ����߂ɓ���ւ���
	if (tmin > tmax) std::swap(tmin, tmax);

	//Y�������̖ʂƂ̌����͈͂����߂�
	float tymin = (min.y - origin.y) / CopyDirection.y;
	float tymax = (max.y - origin.y) / CopyDirection.y;

	//tymax����tymin�̕����������傫����΁A�ŏ��ő�̊֌W������Ȃ����߂ɓ���ւ���
	if (tymin > tymax) std::swap(tymin, tymax);

	//���݂̌����͈͂�Y���̌����͈͓��Ɏ��܂��Ă��Ȃ�������A���C�͓������Ă��Ȃ��iAABB�͎������s�ł���{���I�Ɍ����͈͂̍ő�ŏ��͓����ɂȂ�j
	if ((tmin > tymax) || (tymin > tmax)) return false;

	//Y�������̖ʂ�X�������̖ʂ����߂�������A�܂��͉�����������A�����͈͂��X�V����
	if (tymin > tmin) tmin = tymin;
	if (tymax < tmax) tmax = tymax;

	//Z�������̖ʂƂ̌����͈͂����߂�
	float tzmin = (min.z - origin.z) / CopyDirection.z;
	float tzmax = (max.z - origin.z) / CopyDirection.z;

	//tzmax����tzmin�̕����������傫����΁A�ŏ��ő�̊֌W������Ȃ����߂ɓ���ւ���
	if (tzmin > tzmax) std::swap(tzmin, tzmax);

	//���݂̌����͈͂�Z���̌����͈͓��Ɏ��܂��Ă��Ȃ�������A���C�͓������Ă��Ȃ�
	if ((tmin > tzmax) || (tzmin > tmax)) return false;

	//Z�������̖ʂ�XY�������̖ʂ����߂�������A�܂��͉�����������A�����͈͂��X�V����
	if (tzmin > tmin) tmin = tzmin;
	if (tzmax < tmax) tmax = tzmax;

	//��ԋ߂��ʂ��m��
	t = tmin;

	//���C�̎x�_�����ɐ����`�i�I�u�W�F�N�g�j������ꍇ�́A�Ӗ����Ȃ��̂Ŗ�������
	if (t < 0.0f) return false;

	//��ԋ߂��ʂƂ̋����������āA���C�̏Փˈʒu�����߂�j
	CollisionPos = D3DXVECTOR3(origin.x + CopyDirection.x * t,
		origin.y + CopyDirection.y * t,
		origin.z + CopyDirection.z * t);
	return true;
}
//====================================================================================================================
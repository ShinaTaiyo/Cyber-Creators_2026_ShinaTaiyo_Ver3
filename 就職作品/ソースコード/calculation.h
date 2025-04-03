//====================================================
//
//８月２９日：計算用のクラスを作る[calculation.h]
//Author:ShinaTaiyo
//
//====================================================

#ifndef _CALCULATION_H_  
#define _CALCULATION_H_

//======================
//インクルード
//======================
#include "main.h"
//==========================================

//==========================================

//==========================================
//計算クラス
//==========================================
class CCalculation
{
public:
    //==============================
	//移動方向列挙型
	//==============================
	typedef enum
	{
		MOVEAIM_XY = 0,//XY方向に動く
		MOVEAIM_XZ,    //XZ方向に動く
		MOVEAIM_ZY,    //ZY方向に動く
		MOVEAIM_MAX
	}MOVEAIM;
	//=============================================================================================================

	//==============================
	//プロトタイプ宣言
	//==============================
	CCalculation();                                                         //コンストラクタ
	~CCalculation();                                                        //デストラクタ
	static float CalculationLength(D3DXVECTOR3 Pos, D3DXVECTOR3 PurposePos);//2点の距離を計算する
	static float CalculationXYaim(D3DXVECTOR3 Pos, D3DXVECTOR3 PurposePos); //２次元の目的方向への角度を求める
	static float CalculationParabola(float fLength, float fGravity, float fSpeed,D3DXVECTOR3 Pos,D3DXVECTOR3 PurposePos);//距離、重力、速度をもとに２点の放物線移動を計算する
	static float CalculationRandVecXY();                                    //ランダムな２次元ベクトルを返す
	static float CalculationCollectionRot2D(float fMyRot, float fRotAim, float fDecayRot,bool bCameraOffSet); //２次元方向の向きの補正を行う
	static bool CaluclationMove(bool bUseStick,D3DXVECTOR3 & Pos,D3DXVECTOR3& Move, float fSpeed, MOVEAIM MoveAim,float & fRot);//移動に使う処理の角度を求める
	static D3DXVECTOR3 Calculation3DVec(D3DXVECTOR3 MyPos, D3DXVECTOR3 AimPos, float fSpeed);                 //目的への移動量を求める
	static D3DXVECTOR2 VectorToYawPitch(const D3DXVECTOR3& MyPos, const D3DXVECTOR3& AimPos);                 //目的地へのYawとPitchへ変換する
	static D3DXVECTOR3 Rand3DVec(int nMathSpeed,int nDivisionSpeed);                                          //ランダムな3次元空間の移動量を計算する
	static D3DXCOLOR CalRaibowColor();

	// スクリーン座標をワールド座標に変換
	static D3DXVECTOR3* CalcScreenToWorld(D3DXVECTOR3* pout,
		float Sx,  // スクリーンX座標
		float Sy,  // スクリーンY座標
		float fZ,  // 射影空間でのZ値（0～1）
		int Screen_w,
		int Screen_h,
		D3DXMATRIX* View,
		D3DXMATRIX* Prj
	);

	//ワールド座標をスクリーン座標に変換
	static D3DXVECTOR3 CalcWorldToScreenNoViewport(D3DXVECTOR3 worldPos, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, float screenWidth, float screenHeight);

	//桁数を計算する
	static int CalculationDigit(int nNum);

	//指定した桁数を取得する
	static int getDigit(int number, int position);

	//ベクトルを計算する
	static D3DXVECTOR3 CalcVec(D3DXVECTOR3 MyPos, D3DXVECTOR3 AimPos,bool bNormalize);

	//ラジアンを方向ベクトルに変える
	static D3DXVECTOR3 RadToVec(const D3DXVECTOR3& Rot);//ラジアンを方向ベクトルに変える

	//ベクトルに対して点が左右どちらにいるかを求める
	static float DetermineSide3D(const D3DXVECTOR3& origin, const D3DXVECTOR3& direction, const D3DXVECTOR3& up, const D3DXVECTOR3& point);

	//目的の高さへ移動するための初速度を求める
	static float GetInitialVelocityHeight(float fHeight,float fGravity);

	//目的の位置への角度をホーミング式に求める
	static D3DXVECTOR3 HormingVecRotXZ(float& fRotMove, const D3DXVECTOR3& MyPos, const D3DXVECTOR3& AimPos, float CorrectionRot,float fSpeed);

	//角度ラジアンの補正を行う
	static float CorrectionRot(float fRot);
	//=============================================================================================================
};
#endif

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
	CCalculation();                   //コンストラクタ
	~CCalculation();                  //デストラクタ
	static float CalculationLength(D3DXVECTOR3 Pos, D3DXVECTOR3 PurposePos);//2点の距離を計算する
	static float CalculationXYaim(D3DXVECTOR3 Pos, D3DXVECTOR3 PurposePos);//２次元の目的方向への角度を求める
	static float CalculationParabola(float fLength, float fGravity, float fSpeed,D3DXVECTOR3 Pos,D3DXVECTOR3 PurposePos);//距離、重力、速度をもとに２点の放物線移動を計算する
	static float CalculationRandVecXY();//ランダムな２次元ベクトルを返す
	static float CalculationCollectionRot2D(float fMyRot, float fRotAim, float fDecayRot,bool bCameraOffSet);//２次元方向の向きの補正を行う
	static bool CaluclationMove(bool bUseStick,D3DXVECTOR3& Move, float fSpeed, MOVEAIM MoveAim,float & fRot);//移動に使う処理の角度を求める
	static D3DXVECTOR3 Calculation3DVec(D3DXVECTOR3 MyPos, D3DXVECTOR3 AimPos, float fSpeed);  //目的への移動量を求める
	static D3DXVECTOR2 VectorToYawPitch(const D3DXVECTOR3& MyPos, const D3DXVECTOR3& AimPos);//目的地へのYawとPitchへ変換する
	static D3DXVECTOR3 Rand3DVec(int nMathSpeed,int nDivisionSpeed);//ランダムな3次元空間の移動量を計算する
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
	// XZ平面とスクリーン座標の交点算出関数
	static D3DXVECTOR3* CalcScreenToXZ(
		D3DXVECTOR3* pout,
		float Sx,
		float Sy,
		int Screen_w,
		int Screen_h,
		D3DXMATRIX* View,
		D3DXMATRIX* Prj
	);

	//レイと球の衝突判定
	static bool CalcRaySphere(
		D3DXVECTOR3 LayPos,//レイの支点
		D3DXVECTOR3 LayVec,//レイの方向ベクトル
		D3DXVECTOR3 SphereSenterPos,//球の中心点
		float r,                    //球の半径
		D3DXVECTOR3 & CollisionStartPos,//レイの衝突開始位置
		D3DXVECTOR3 & CollisoinEndPos   //レイの衝突終了位置
	);
	//目的の物体とレイの向きが一致しているかどうかを判定
	static bool CalcMatchRay(D3DXVECTOR3 AimPos, float fSx, float fSy, int nScreen_w, int nScreen_h, D3DXMATRIX* View, D3DXMATRIX * Prj);

	//点と四角形の各辺との距離を求める
	static float pointLineDistance(float cx, float cy, float x1, float y1, float x2, float y2);

	//四角形と円の当たり判定
	static bool checkCollisionCircleRectangle(float cx, float cy, float r,
		float x1, float y1, float x2, float y2,
		float x3, float y3, float x4, float y4);

	//桁数を計算する
	static int CalculationDigit(int nNum);

	//指定した桁数を取得する
	static int getDigit(int number, int position);

	//ベクトルを計算する
	static D3DXVECTOR3 CalcVec(D3DXVECTOR3 MyPos, D3DXVECTOR3 AimPos,bool bNormalize);

	//仰角を求める
	static float CalcElevationAngle(const D3DXVECTOR3& MyPos, const D3DXVECTOR3& AimPos);

	//二つのベクトルのなす角を求める
	static float GetAngleBetweenVectors(const D3DXVECTOR3& v1, const D3DXVECTOR3& v2);

	//プレイヤーから見て線形範囲にオブジェクトが入っているかどうかを判定
	static bool IsObjectInFieldOfView(const D3DXVECTOR3& PlayerPos, const D3DXVECTOR3& PlayerViewDir,
		const D3DXVECTOR3& ObjectPos, float fovAngle, float maxDistance);

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

	//================================================
	//YawとPitchを求め、目的の位置への角度を求める
	//================================================

	//目的の位置への向きを計算する
	static void CalcRotToTarget(const D3DXVECTOR3& MyPos, const D3DXVECTOR3& AimPos, float& OutYaw, float& OutPitch);

	//目的の位置への向きをYawとPitchを使用して求める
	static D3DXVECTOR3 CalcDirectionFromYawPitch(const float Yaw, const float Pitch);

	//目的の位置への角度をまとめて求める
	static D3DXVECTOR3 CalcSummarizeRotToTarget(const D3DXVECTOR3& MyPos, const D3DXVECTOR3& AimPos);
	//=======================================================================================================================


	//=============================================================================================================
private:
};
#endif

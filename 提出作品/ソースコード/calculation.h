//===================================================================================================================
//
// ２０２５年６月１３日：計算用クラスがクラスである必要ないのでnamespaceに変更[calculation.h]
// Author:ShinaTaiyo
//
//===================================================================================================================

//*******************************************************************************************************************
// ２重インクルード防止
//*******************************************************************************************************************
#ifndef _CALCULATION_H_  
#define _CALCULATION_H_

//*******************************************************************************************************************
// インクルード
//*******************************************************************************************************************
#include "main.h"

//*******************************************************************************************************************
// 計算用名前空間
//*******************************************************************************************************************
namespace Calculation
{

	// === 名前空間 ===

	// ベクトル名前空間
	namespace Vec
	{
		// ラジアンを方向ベクトルに変える
		D3DXVECTOR3 RadTo(const D3DXVECTOR3& Rot);

		// ベクトルに対して点が左右どちらにいるかを求める
		float DetermineSide(
			const D3DXVECTOR3& origin,    // ベクトル原点 
			const D3DXVECTOR3& direction, // ベクトル
			const D3DXVECTOR3& up,        // 上方向ベクトル
			const D3DXVECTOR3& point);    // 左右どちらにいるかの点

	}

	// 移動量名前空間 
	namespace Move
	{
		// === 列挙型 ===

        // 移動方向
		enum class MOVEAIM
		{
			XY = 0, // XY方向
			XZ,     // XZ方向
			ZY,     // ZY方向
			MAX     // 最大
		};

		// === 関数 ===

		// 三次元ベクトル乱数値を求める
		D3DXVECTOR3 Rand3D(
			int nMaxSpeed,        // 最大速度 
			int nDivisionMaxSpeed // 最大速度を割る値
		);

		// 目的地への移動量を求める
		D3DXVECTOR3 DirrectionToTarget(
			D3DXVECTOR3 SelfPos, // 自身の位置 
			D3DXVECTOR3 AimPos,  // 目的地
			float fSpeed         // 速度
		);

		// 入力による移動処理
		bool MovementInput(
			bool bUseStick,    // スティックを使用するかどうか
			D3DXVECTOR3& Pos,  // 位置
			D3DXVECTOR3& Move, // 移動量
			float fSpeed,      // 速度
			MOVEAIM MoveAim,   // 移動方向
			float fAddAimRot,  // 目的の向きに加算する量（プレイヤーが逆側を向いているときなどに使える）
			float& fRot        // 現在の角度を参照して変更する用
		);   

		// XZ方向で目的地へホーミングさせる
		D3DXVECTOR3 HormingToGoalPosXZ(
			float& fNowRot, // 現在の向き
			const D3DXVECTOR3& SelfPos,  // 自身の位置 
			const D3DXVECTOR3& GoalPos,  // 目的地
			float CorrectionRot, // 向き補正値
			float fSpeed // 速度
		);

		// 目的の高さへ移動するための初速度を求める
		float GetInitialVelocityHeight(
			float fHeight,  // 高さ
			float fGravity  // 重力
		);
	}

	// 向き名前空間
	namespace Rot
	{
		// 目的地へのベクトルをYawとPitchへ変換する
		D3DXVECTOR2 VectorToYawPitch(
			const D3DXVECTOR3& SelfPos,  // 自身の位置 
			const D3DXVECTOR3& GoalPos   // 目的地
		);

		// 現在の向きを目的の向きに線形補間する処理
		float Lerp(
			float fSelfRot,   // 自身の向き
			float fRotAim,    // 目的の向き
			float fDecayRot   // 補正値
		);

		// 向きを円周上(-D3DX_PI～D3DX_PI)に包む
		float WrapAngle(float fRot);

		// 目的地までの放物線角度を求める
		float ParabolaToGoalPos(
			float fLength,  // 距離
			float fGravity, // 重力
			float fSpeed,   // 速度
			D3DXVECTOR3 SelfPos, // 自身の位置
			D3DXVECTOR3 GoalPos  // 目的地
		); // 距離、重力、速度をもとに２点の放物線移動を計算する

	}

	// 距離名前空間
	namespace Length
	{
		// 目的地までの距離を求める処理
		float ToGoalPos(
			D3DXVECTOR3 SelfPos, // 自身の位置
			D3DXVECTOR3 GoalPos  // 目的地
		);
	}

	// 変換
	namespace Conversion
	{
		// スクリーン座標をワールド座標に変換
		D3DXVECTOR3* ScreenToWorld(D3DXVECTOR3* pout,
			float Sx,  // スクリーンX座標
			float Sy,  // スクリーンY座標
			float fZ,  // 射影空間でのZ値（0～1）
			int Screen_w, // スクリーン横幅
			int Screen_h, // スクリーン高さ
			D3DXMATRIX* View, // ビューマトリックス
			D3DXMATRIX* Prj   // プロジェクションマトリックス
		);

		// ワールド座標をスクリーン座標に変換
		D3DXVECTOR3 WorldToScreen(
			D3DXVECTOR3 worldPos,        // ワールド座標
			D3DXMATRIX viewMatrix,       // ビューマトリックス
			D3DXMATRIX projectionMatrix, // プロジェクションマトリックス
			float screenWidth,   // スクリーン横幅
			float screenHeight); // スクリーン高さ
	}

	// 値
	namespace Value
	{
		// 桁数を計算する
		int CalcDigit(
			int nValue // 数値
		);

		// 指定した桁数を取得する
		int GetDigit(int number, int position);
	}
};
#endif

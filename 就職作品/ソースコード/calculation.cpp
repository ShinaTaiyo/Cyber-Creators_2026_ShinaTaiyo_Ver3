//===========================================================================================================
//
//８月２９日：計算用のクラスを作る[calculation.cpp]
//Author:ShinaTaiyo
//
//===========================================================================================================

//=========================================================
//インクルード
//=========================================================
#include "calculation.h"
#include "manager.h"
#include "camera.h"
#include "renderer.h"
#include "particle.h"
#include "attack.h"
#include "input.h"
#include "debugtext.h"
//===========================================================================================================

//=========================================================
//コンストラクタ
//=========================================================
CCalculation::CCalculation()
{

}
//===========================================================================================================

//=========================================================
//デストラクタ
//=========================================================
CCalculation::~CCalculation()
{

}
//===========================================================================================================

//=========================================================
//距離を計算する
//=========================================================
float CCalculation::CalculationLength(D3DXVECTOR3 Pos, D3DXVECTOR3 PurposePos)
{
	float fLength = 0.0f;//距離格納用
	fLength = sqrtf(powf(PurposePos.x - Pos.x, 2) + powf(PurposePos.y - Pos.y, 2) + powf(PurposePos.z - Pos.z, 2));//目的の位置との距離を求める
	return fLength;
}
//==========================================================================================================

//=========================================================
//XYベクトルを計算する
//=========================================================
float CCalculation::CalculationXYaim(D3DXVECTOR3 Pos, D3DXVECTOR3 PurposePos)
{
	float fVXaim = PurposePos.x - Pos.x;//X方向のベクトルを求める
	float fVYaim = PurposePos.y - Pos.y;//Y方向のベクトルを求める
	float fVLaim = atan2f(fVXaim, fVYaim);//二つのベクトルの角度を求める
	return fVLaim;
}
//==========================================================================================================

//=========================================================
//放物線の角度を計算する
//=========================================================
float CCalculation::CalculationParabola(float fLength, float fGravity, float fSpeed, D3DXVECTOR3 Pos, D3DXVECTOR3 PurposePos)
{
	float fRot = 0.0f;             //角度

	float fXG = fLength * fGravity;//距離×重力
	float fV = fSpeed;             //速度
	float fV2 = powf(fV, 2);       //速度２条

	fRot = asinf(fXG / fV2);       
	fRot /= 2;

	if (PurposePos.x < Pos.x)
	{//角度が絶対値なので、目的の位置が自分の位置よりマイナス方向にある場合、負の値にする
		fRot *= -1;
	}

	return fRot;
}
//===========================================================================================================

//=========================================================
//XY方向の乱数ベクトルを用意する
//=========================================================
float CCalculation::CalculationRandVecXY()
{
	float fRandRot = float(rand() % 628 + 1) / 100;//D3DX_PI * 2（約6.28)をランダムで求める
	return fRandRot;
}
//===========================================================================================================

//=========================================================
//2D方向の向きの補正を行う
//=========================================================
float CCalculation::CalculationCollectionRot2D(float fMyRot, float fRotAim, float fDecayRot, bool bCameraOffSet)
{
	float fRotDiff = 0.0f;                                 //向きの差分
	float fCameraRot = CManager::GetCamera()->GetRot().y;  //カメラのYawを取得
	fRotDiff = fRotAim - fMyRot;

	//向きの差分の調整（ジンバルロックを回避するため)
	if (fRotDiff > D3DX_PI)
	{
		fRotDiff -= D3DX_PI * 2;
	}
	else if (fRotDiff < -D3DX_PI)
	{
		fRotDiff += D3DX_PI * 2;
	}

	//徐々に目的の向きへ合わせていく
	fMyRot += fRotDiff * fDecayRot;

	if (bCameraOffSet == true)
	{//カメラを基準に向きを決めるかどうか
		//向きの調整（カメラを基準に値を3.14～-3.14の中に固定したいので・・・）
		if (fMyRot >= D3DX_PI + fCameraRot)
		{//3.14→-3.14にする
			fMyRot -= D3DX_PI * 2;
		}
		else if (fMyRot <= -D3DX_PI + fCameraRot)
		{//-3.14→3.14にする
			fMyRot += D3DX_PI * 2;
		}
	}
	else
	{//普通に向きを決める
		//向きの調整
		if (fMyRot >= D3DX_PI)
		{//3.14→-3.14にする
			fMyRot -= D3DX_PI * 2;
		}
		else if (fMyRot <= -D3DX_PI)
		{//-3.14→3.14にする
			fMyRot += D3DX_PI * 2;
		}
	}
	return fMyRot;
}
//===========================================================================================================

//=========================================================
//移動方向への角度を計算する
//=========================================================
bool CCalculation::CaluclationMove(bool bUseStick, D3DXVECTOR3& Pos, D3DXVECTOR3& Move, float fSpeed, MOVEAIM MoveAim, float& fRot)
{
	float fCameraRot = CManager::GetCamera()->GetRot().y;           //カメラの向きを取得
	float fMoveX = 0.0f;                                            //X方向の移動量
	float fMoveZ = 0.0f;                                            //Z方向の移動量
	bool bMove = false;                                             //移動しているかどうか 
	bool bUseController = true;                                     //コントローラーを使用するかどうか
	bMove = CManager::GetInputJoypad()->GetLStickPress(8, 0.0f);    //コントローラーの入力
	D3DXVECTOR3 PosFuture = { 0.0f,0.0f,0.0f };                     //1f後の位置
	if (bMove == false)
	{//この時点でコントローラーの入力がされていない場合、キー入力の受付を開始
		bUseController = false;//コントローラーは使用しない
		if (CManager::GetInputKeyboard()->GetPress(DIK_W) == true || CManager::GetInputJoypad()->GetPress(CInputJoypad::JOYKEY::UP) == true)
		{//前へのベクトル
			fMoveZ = 1.0f;
		}
		else if (CManager::GetInputKeyboard()->GetPress(DIK_S) == true || CManager::GetInputJoypad()->GetPress(CInputJoypad::JOYKEY::DOWN) == true)
		{//後ろへのベクトル
			fMoveZ = -1.0f;
		}
		if (CManager::GetInputKeyboard()->GetPress(DIK_D) == true || CManager::GetInputJoypad()->GetPress(CInputJoypad::JOYKEY::RIGHT) == true)
		{//右へのベクトル
			fMoveX = 1.0f;
		}
		else if (CManager::GetInputKeyboard()->GetPress(DIK_A) == true || CManager::GetInputJoypad()->GetPress(CInputJoypad::JOYKEY::LEFT) == true)
		{//左へのベクトル
			fMoveX = -1.0f;
		}

		if (fMoveX != 0.0f || fMoveZ != 0.0f)
		{//上記の処理でボタンを入力していたら動いているとみなす
			bMove = true;//移動状態

		}
		else
		{
			bMove = false;//待機状態
		}
	}
	if (bMove == true)
	{//移動状態なら
		float fMoveRot = 0.0f;//移動する向きを決める
		//カメラを基準に向きを決める
		if (bUseController == true)
		{
			fMoveRot = CManager::GetInputJoypad()->GetLStickAimRot();                     //左スティックの向きを取得する
		}
		else
		{//比から角度を求める（Z軸の正方向が前なので、Z軸を基準「右引数」にX方向の角度「左引数」を求める）
			fMoveRot = atan2f(fMoveX, fMoveZ);
		}
		switch (MoveAim)
		{//どの面を軸に移動するかを決める
		case MOVEAIM_XY://XY方向を基準に移動する（移動方向をカメラ基準で決める)
			Move.x = sinf(fMoveRot + fCameraRot) * fSpeed;
			Move.y = cosf(fMoveRot + fCameraRot) * fSpeed;
			break;
		case MOVEAIM_XZ://XZ方向を基準に移動する
			Move.x = sinf(fMoveRot + fCameraRot) * fSpeed;
			Move.z = cosf(fMoveRot + fCameraRot) * fSpeed;
			break;
		case MOVEAIM_ZY://ZY方向を基準に移動する
			Move.z = sinf(fMoveRot + fCameraRot) * fSpeed;
			Move.y = cosf(fMoveRot + fCameraRot) * fSpeed;
			break;
		default:
			break;
		}

		//1f後の位置を計算
		PosFuture = Pos + Move;

		//1f後の位置と現在の位置のベクトルから向くべき角度を求める
		fRot = atan2f(PosFuture.x - Pos.x, PosFuture.z - Pos.z);
	}
	return bMove;

}
//===========================================================================================================


//=========================================================
//目的の位置への移動量を計算する
//=========================================================
D3DXVECTOR3 CCalculation::Calculation3DVec(D3DXVECTOR3 MyPos, D3DXVECTOR3 AimPos, float fSpeed)
{
	D3DXVECTOR3 VecAim = D3DXVECTOR3(0.0f,0.0f,0.0f);       //それぞれの方向のベクトル
	D3DXVECTOR3 ResultMove = D3DXVECTOR3(0.0f,0.0f,0.0f);   //結果の移動量
	float fVLaim = 0.0f;                                    //総合ベクトル

	//==========================
	//３次元ベクトルを取る
	//==========================

	//それぞれの軸のベクトルを計算
	VecAim.x = AimPos.x - MyPos.x;
	VecAim.y = AimPos.y - MyPos.y;
	VecAim.z = AimPos.z - MyPos.z;

	//ベクトルの大きさを求める
	fVLaim = sqrtf(powf(VecAim.x,2) + powf(VecAim.y,2) + powf(VecAim.z,2));

	//方向ベクトルを正規化し、それぞれの軸に対する移動量を求める
	ResultMove.x = VecAim.x / fVLaim * fSpeed;
	ResultMove.y = VecAim.y / fVLaim * fSpeed;
	ResultMove.z = VecAim.z / fVLaim * fSpeed;

	return ResultMove;
}
//===========================================================================================================

//=========================================================
//目的地へのベクトルをYawとPitchに変換する
//=========================================================
D3DXVECTOR2 CCalculation::VectorToYawPitch(const D3DXVECTOR3& MyPos, const D3DXVECTOR3& AimPos)
{
	D3DXVECTOR2 YawPitch = { 0.0f,0.0f };//YawとPitchを計算する

	YawPitch.y = atan2f(AimPos.x - MyPos.x, AimPos.z - MyPos.z);//XZ方向の発射角度を距離の比からYawを求める
	YawPitch.x = atan2f(AimPos.y - MyPos.y, sqrtf(powf(AimPos.x - MyPos.x, 2) + powf(AimPos.z - MyPos.z, 2)));//XZ方向の距離を軸にPitchを求める

	return YawPitch;
}
//===========================================================================================================

//=========================================================
//ランダムな３次元空間の速度を求める
//=========================================================
D3DXVECTOR3 CCalculation::Rand3DVec(int nMathSpeed, int nDivisionSpeed)
{
	D3DXVECTOR3 Speed = { 0.0f,0.0f,0.0f };
	bool bMinus = false;

	//X方向のランダムな向きを求める
	bMinus = rand() % 2;//右か左かを決める
	Speed.x = static_cast<float>(rand() % nMathSpeed) / nDivisionSpeed;
	if (bMinus == true)
	{
		Speed.x *= -1;
	}

	//Y方向のランダムな向きを決める
	bMinus = rand() % 2;//上か下を決める
	Speed.y = static_cast<float>(rand() % nMathSpeed) / nDivisionSpeed;
	if (bMinus == true)
	{
		Speed.y *= -1;
	}

	//Z方向のランダムな向きを決める
	bMinus = rand() % 2;//前か後ろかを決める
	Speed.z = static_cast<float>(rand() % nMathSpeed) / nDivisionSpeed;
	if (bMinus == true)
	{
		Speed.z *= -1;
	}
	return Speed;
}
//===========================================================================================================

//=========================================================
//虹色を計算する
//=========================================================
D3DXCOLOR CCalculation::CalRaibowColor()
{
    //RGBの値をランダムに決め続け虹色を表現する
	D3DXCOLOR RaibowColor = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	RaibowColor.r = float(rand() % 100 + 1) / 100;//R値を求める
	RaibowColor.g = float(rand() % 100 + 1) / 100;//G値を求める
	RaibowColor.b = float(rand() % 100 + 1) / 100;//B値を求める
	return RaibowColor;
}
//===========================================================================================================

//=========================================================
// スクリーン座標をワールド座標に変換
//=========================================================
D3DXVECTOR3* CCalculation::CalcScreenToWorld(D3DXVECTOR3* pout, float Sx, float Sy, float fZ, int Screen_w, int Screen_h, D3DXMATRIX* View, D3DXMATRIX* Prj)
{
	// 各行列の逆行列を算出（ビュー、プロジェクションマトリックスの逆行列をかけるのは、カメラの位置に2DのUIが出ていると定義できるから)
	//逆行列とは、値に値-1をかけ、掛け合わされる前に戻すこと
	D3DXMATRIX InvView, InvPrj, VP, InvViewport;
	D3DXMatrixInverse(&InvView, NULL, View);//ビューマトリックスとの逆光列をかけてワールド座標を求める
	D3DXMatrixInverse(&InvPrj, NULL, Prj);  //プロジェクションマトリックスとの逆行列（見え方（平行投影、視野角など）を変えているので、逆行列を掛け合わせ、もとに戻す必要がある）
	D3DXMatrixIdentity(&VP);

	//スケーリングの値を変えている。スクリーン座標の中心を画面中央にする
	VP._11 = Screen_w / 2.0f; VP._22 = -Screen_h / 2.0f;//スクリーン座標系では、通常上方向が正になるので、座標変換する際に-にしている
	VP._41 = Screen_w / 2.0f; VP._42 = Screen_h / 2.0f;//スクリーン座標系では、通常上方向が正になるので、座標変換する際に-にしている
	D3DXMatrixInverse(&InvViewport, NULL, &VP);

	//自分
	D3DXVECTOR3 MyPos = D3DXVECTOR3(Sx,Sy,fZ);

	// 逆変換
	D3DXMATRIX tmp = InvViewport * InvPrj * InvView;//ワールド座標を求める
	D3DXVec3TransformCoord(pout, &MyPos, &tmp);     //位置を求める
	return pout;
}
//===========================================================================================================

//=========================================================
// ワールド座標をスクリーン座標に変換する
//=========================================================
D3DXVECTOR3 CCalculation::CalcWorldToScreenNoViewport(D3DXVECTOR3 worldPos, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, float screenWidth, float screenHeight)
{
	//ワールド座標をクリップ座標に変換
	D3DXVECTOR4 ClipSpacePos;
	D3DXMATRIX mtxTrans;

	mtxTrans = viewMatrix * projectionMatrix;
	D3DXVec3Transform(&ClipSpacePos, &worldPos, &mtxTrans);

	//透視除算（クリップ座標からNDC空間へ）
	if (ClipSpacePos.w != 0.0f)
	{
		ClipSpacePos.x /= ClipSpacePos.w;
		ClipSpacePos.y /= ClipSpacePos.w;
		ClipSpacePos.z /= ClipSpacePos.w;
	}

	//スクリーン座標へ変換
	D3DXVECTOR3 ScreenPos;
	ScreenPos.x = (ClipSpacePos.x * 0.5f + 0.5f) * screenWidth;
	ScreenPos.y = (1.0f - (ClipSpacePos.y * 0.5f + 0.5f)) * screenHeight;
	ScreenPos.z = ClipSpacePos.z;//深度値（０～１）の範囲

	return ScreenPos;
}
//===========================================================================================================

//=========================================================
//桁数を計算する
//=========================================================
int CCalculation::CalculationDigit(int nNum)
{
	int nDigit = 0;
	if (nNum == 0)
	{//数値がないので1桁
		return 1;
	}

	while (nNum != 0) {//intを10で割って値が０になるまで繰り返し、桁数を計算する
		nNum /= 10;
		nDigit++;
	}
	return nDigit;
}

//=========================================================
//指定した桁数を取得する(numberが345、positionが2だったら)→345 / 100 = 3.45 = 3 % 10 = 3)
//=========================================================
int CCalculation::getDigit(int number, int position)
{
	int divisor = static_cast<int>(std::pow(10, position));
	return (number / divisor) % 10;
}
//===========================================================================================================

//=========================================================
//ベクトルを計算する
//=========================================================
D3DXVECTOR3 CCalculation::CalcVec(D3DXVECTOR3 MyPos, D3DXVECTOR3 AimPos, bool bNormalize)
{
	D3DXVECTOR3 Vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//ベクトルを求める
	Vec = AimPos - MyPos;//目的の位置とのベクトルを求める

	if (bNormalize == true)
	{//正規化するかどうか
		D3DXVec3Normalize(&Vec, &Vec);
	}
	return Vec;
}
//===========================================================================================================

//=====================================================================
//ラジアンを方向ベクトルに変換する
//=====================================================================
D3DXVECTOR3 CCalculation::RadToVec(const D3DXVECTOR3& Rot)
{
	D3DXVECTOR3 RotToVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//上を０とするため、Y軸はcosを基準にする。XZ平面と比を合わせるため、XZの位置を求めるときは、sinをかける
	RotToVec.x = sinf(Rot.x) * sinf(Rot.y);
	RotToVec.y = cosf(Rot.x);
	RotToVec.z = sinf(Rot.x) * cosf(Rot.y);

	D3DXVec3Normalize(&RotToVec, &RotToVec);
	return RotToVec;
}
//===========================================================================================================

//=====================================================================
//方向ベクトルに対して点が左右どちらにいるかを判定する
//=====================================================================
float CCalculation::DetermineSide3D(const D3DXVECTOR3& origin, const D3DXVECTOR3& direction, const D3DXVECTOR3& up, const D3DXVECTOR3& point)
{
	// 相対ベクトルを計算
	D3DXVECTOR3 Relative = { point.x - origin.x, point.y - origin.y, point.z - origin.z };

	// 右方向ベクトルを計算
	D3DXVECTOR3 Right = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVec3Cross(&Right,&direction,&up);

	// 右方向ベクトルと相対ベクトルの内積を計算
	float fDot = D3DXVec3Dot(&Right,&Relative);

	return fDot;
}
//===========================================================================================================

//=====================================================================
//目的の高さへ到達するための初速度を求める（v0 = √2gh)
//=====================================================================
float CCalculation::GetInitialVelocityHeight(float fHeight, float fGravity)
{
	float fAim = sqrtf(2 * fGravity * fHeight);
	return fAim;
}
//===========================================================================================================

//=====================================================================
//オブジェクトの向きをホーミングさせる
//=====================================================================
D3DXVECTOR3 CCalculation::HormingVecRotXZ(float& fRotMove, const D3DXVECTOR3& MyPos, const D3DXVECTOR3& AimPos, float CorrectionRot, float fSpeed)
{
	float fRotDest = 0.0f;//目的の位置への角度
	float fRotDiff = 0.0f;//現在の角度と目的の角度の差分
	float fVXaim = 0.0f;  //Xベクトル
	float fVZaim = 0.0f;  //Yベクトル
	D3DXVECTOR3 Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//移動量
	//===================================
	//XZベクトルを出す
	//===================================
	fVXaim = AimPos.x - MyPos.x;
	fVZaim = AimPos.z - MyPos.z;

	fRotDest = atan2f(fVXaim, fVZaim);

	//==========================================================================================================================================================

	//現在の移動方向の角度と、目的の移動方向の角度の差分を求める
	fRotDiff = fRotDest - fRotMove;

	//===============================
	//角度差分の修正
	//===============================
	//向きの差分の調整「ジンバルロックを回避」(3.14を超えたら近い向きに補正）
	if (fRotDiff > D3DX_PI)
	{
		fRotDiff -= D3DX_PI * 2;
	}
	else if (fRotDiff < -D3DX_PI)
	{
		fRotDiff += D3DX_PI * 2;
	}

	//==============================================================================================
	fRotMove += fRotDiff * CorrectionRot;//移動方向（角度補正）

	//向きの調整「ジンバルロックを回避」（カメラを基準に値を3.14～-3.14の中に固定したいので・・・）
	if (fRotMove >= D3DX_PI)
	{//3.14→-3.14にする
		fRotMove -= D3DX_PI * 2;
	}
	else if (fRotMove <= -D3DX_PI)
	{//-3.14→3.14にする
		fRotMove += D3DX_PI * 2;
	}

	//移動量を更新する
	Move.x = sinf(fRotMove) * fSpeed;
	Move.z = cosf(fRotMove) * fSpeed;

	return Move;
}
//===========================================================================================================


//=====================================================================
//向きの補正を行う
//=====================================================================
float CCalculation::CorrectionRot(float fRot)
{
	//ジンバルロックを回避する
	if (fRot > D3DX_PI)
	{
		fRot -= D3DX_PI * 2;
	}
	if (fRot < -D3DX_PI)
	{
		fRot += D3DX_PI * 2;
	}

	return fRot;
}
//===========================================================================================================
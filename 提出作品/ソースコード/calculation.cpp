//===========================================================================================================
//
//８月２９日：計算用のクラスを作る[calculation.cpp]
//Author:ShinaTaiyo
//
//===========================================================================================================

//*******************************************************************************************************************
// インクルード
//*******************************************************************************************************************
#include "calculation.h"
#include "manager.h"
#include "camera.h"
#include "renderer.h"
#include "particle.h"
#include "attack.h"
#include "input.h"
#include "debugtext.h"

////=========================================================
////距離を計算する
////=========================================================
//float CCalculation::CalculationLength(D3DXVECTOR3 Pos, D3DXVECTOR3 PurposePos)
//{
//	float fLength = 0.0f;//距離格納用
//	fLength = sqrtf(powf(PurposePos.x - Pos.x, 2) + powf(PurposePos.y - Pos.y, 2) + powf(PurposePos.z - Pos.z, 2));//目的の位置との距離を求める
//	return fLength;
//}
////==========================================================================================================

////=========================================================
////XYベクトルを計算する
////=========================================================
//float CCalculation::CalculationXYaim(D3DXVECTOR3 Pos, D3DXVECTOR3 PurposePos)
//{
//	float fVXaim = PurposePos.x - Pos.x;//X方向のベクトルを求める
//	float fVYaim = PurposePos.y - Pos.y;//Y方向のベクトルを求める
//	float fVLaim = atan2f(fVXaim, fVYaim);//二つのベクトルの角度を求める
//	return fVLaim;
//}
////==========================================================================================================

////=========================================================
////XY方向の乱数ベクトルを用意する
////=========================================================
//float CCalculation::CalculationRandVecXY()
//{
//	float fRandRot = float(rand() % 628 + 1) / 100;//D3DX_PI * 2（約6.28)をランダムで求める
//	return fRandRot;
//}
////===========================================================================================================

////=========================================================
////2D方向の向きの補正を行う
////=========================================================
//float CCalculation::CalculationCollectionRot2D(float fMyRot, float fRotAim, float fDecayRot, bool bCameraOffSet)
//{
//	float fRotDiff = 0.0f;                                 //向きの差分
//	float fCameraRot = CManager::GetCamera()->GetRot().y;  //カメラのYawを取得
//	fRotDiff = fRotAim - fMyRot;
//
//	//向きの差分の調整（ジンバルロックを回避するため)
//	if (fRotDiff > D3DX_PI)
//	{
//		fRotDiff -= D3DX_PI * 2;
//	}
//	else if (fRotDiff < -D3DX_PI)
//	{
//		fRotDiff += D3DX_PI * 2;
//	}
//
//	//徐々に目的の向きへ合わせていく
//	fMyRot += fRotDiff * fDecayRot;
//
//	if (bCameraOffSet == true)
//	{//カメラを基準に向きを決めるかどうか
//		//向きの調整（カメラを基準に値を3.14～-3.14の中に固定したいので・・・）
//		if (fMyRot >= D3DX_PI + fCameraRot)
//		{//3.14→-3.14にする
//			fMyRot -= D3DX_PI * 2;
//		}
//		else if (fMyRot <= -D3DX_PI + fCameraRot)
//		{//-3.14→3.14にする
//			fMyRot += D3DX_PI * 2;
//		}
//	}
//	else
//	{//普通に向きを決める
//		//向きの調整
//		if (fMyRot >= D3DX_PI)
//		{//3.14→-3.14にする
//			fMyRot -= D3DX_PI * 2;
//		}
//		else if (fMyRot <= -D3DX_PI)
//		{//-3.14→3.14にする
//			fMyRot += D3DX_PI * 2;
//		}
//	}
//	return fMyRot;
//}
////===========================================================================================================

////=========================================================
////移動方向への角度を計算する
////=========================================================
//bool CCalculation::CaluclationMove(bool bUseStick, D3DXVECTOR3& Pos, D3DXVECTOR3& Move, float fSpeed, MOVEAIM MoveAim, float fAddMoveRot, float& fRot)
//{
//	// === 変数 ===
//	float fMoveX = 0.0f; // X方向の移動量
//	float fMoveZ = 0.0f; // Z方向の移動量
//	bool bMove = false;  // 移動しているかどうか 
//	bool bUseController = true; // コントローラーを使用するかどうか
//	bMove = CManager::GetInputJoypad()->GetLStickPress(8, 0.0f); // コントローラーの入力
//	D3DXVECTOR3 PosFuture = { 0.0f,0.0f,0.0f }; // 1f後の位置
//
//	// === 処理 ===
//
//	// この時点でコントローラーの入力がされていない場合、キー入力の受付を開始
//	if (bMove == false)
//	{
//		bUseController = false;//コントローラーは使用しない
//		if (CManager::GetInputKeyboard()->GetPress(DIK_W) == true || CManager::GetInputJoypad()->GetPress(CInputJoypad::JOYKEY::UP) == true)
//		{//前へのベクトル
//			fMoveZ = 1.0f;
//		}
//		else if (CManager::GetInputKeyboard()->GetPress(DIK_S) == true || CManager::GetInputJoypad()->GetPress(CInputJoypad::JOYKEY::DOWN) == true)
//		{//後ろへのベクトル
//			fMoveZ = -1.0f;
//		}
//		if (CManager::GetInputKeyboard()->GetPress(DIK_D) == true || CManager::GetInputJoypad()->GetPress(CInputJoypad::JOYKEY::RIGHT) == true)
//		{//右へのベクトル
//			fMoveX = 1.0f;
//		}
//		else if (CManager::GetInputKeyboard()->GetPress(DIK_A) == true || CManager::GetInputJoypad()->GetPress(CInputJoypad::JOYKEY::LEFT) == true)
//		{//左へのベクトル
//			fMoveX = -1.0f;
//		}
//
//		if (fMoveX != 0.0f || fMoveZ != 0.0f)
//		{//上記の処理でボタンを入力していたら動いているとみなす
//			bMove = true;//移動状態
//
//		}
//		else
//		{
//			bMove = false;//待機状態
//		}
//	}
//	// 移動状態なら
//	if (bMove == true)
//	{
//		float fMoveRot = 0.0f;//移動する向きを決める
//
//		// コントローラーの左スティックから角度を求める
//		if (bUseController == true)
//		{
//			fMoveRot = CManager::GetInputJoypad()->GetLStickAimRot();
//		}
//		// 比から角度を求める（Z軸の正方向が前なので、Z軸を基準「右引数」にX方向の角度「左引数」を求める）
//		else
//		{
//			fMoveRot = atan2f(fMoveX, fMoveZ);
//		}
//
//		fMoveRot += fAddMoveRot; // 向きを加算し補正
//
//		// どの面を軸に移動するかを決める
//		switch (MoveAim)
//		{
//		case MOVEAIM_XY: // XY方向を基準に移動する
//			Move.x = sinf(fMoveRot) * fSpeed;
//			Move.y = cosf(fMoveRot) * fSpeed;
//			break;
//		case MOVEAIM_XZ: // XZ方向を基準に移動する
//			Move.x = sinf(fMoveRot) * fSpeed;
//			Move.z = cosf(fMoveRot) * fSpeed;
//			break;
//		case MOVEAIM_ZY: // ZY方向を基準に移動する
//			Move.z = sinf(fMoveRot) * fSpeed;
//			Move.y = cosf(fMoveRot) * fSpeed;
//			break;
//		default:
//			break;
//		}
//
//		// 1f後の位置を計算
//		PosFuture = Pos + Move;
//
//		// 1f後の位置と現在の位置のベクトルから向くべき角度を求める
//		fRot = atan2f(PosFuture.x - Pos.x, PosFuture.z - Pos.z);
//	}
//	return bMove;
//
//}
////===========================================================================================================


////=========================================================
////目的の位置への移動量を計算する
////=========================================================
//D3DXVECTOR3 CCalculation::Calculation3DVec(D3DXVECTOR3 MyPos, D3DXVECTOR3 AimPos, float fSpeed)
//{
//	// === 処理に使用する情報を宣言、初期化 ===
//
//	D3DXVECTOR3 VecAim = D3DXVECTOR3(0.0f,0.0f,0.0f);       // 目的地へのベクトル
//	D3DXVECTOR3 ResultMove = D3DXVECTOR3(0.0f,0.0f,0.0f);   // 移動量結果
//	float fVLaim = 0.0f; // ベクトル長さ
//
//	// === 計算開始 ===
//
//	// ベクトル計算
//	VecAim = AimPos - MyPos;
//
//	//ベクトルの大きさを求める
//	fVLaim = sqrtf(powf(VecAim.x,2) + powf(VecAim.y,2) + powf(VecAim.z,2));
//
//	//方向ベクトルを正規化し、それぞれの軸に対する移動量を求める
//	ResultMove.x = VecAim.x / fVLaim * fSpeed;
//	ResultMove.y = VecAim.y / fVLaim * fSpeed;
//	ResultMove.z = VecAim.z / fVLaim * fSpeed;
//
//	return ResultMove;
//}
////===========================================================================================================

////=========================================================
////目的地へのベクトルをYawとPitchに変換する
////=========================================================
//D3DXVECTOR2 CCalculation::VectorToYawPitch(const D3DXVECTOR3& MyPos, const D3DXVECTOR3& AimPos)
//{
//	D3DXVECTOR2 YawPitch = { 0.0f,0.0f };//YawとPitchを計算する
//
//	YawPitch.y = atan2f(AimPos.x - MyPos.x, AimPos.z - MyPos.z);//XZ方向の発射角度を距離の比からYawを求める
//	YawPitch.x = atan2f(AimPos.y - MyPos.y, sqrtf(powf(AimPos.x - MyPos.x, 2) + powf(AimPos.z - MyPos.z, 2)));//XZ方向の距離を軸にPitchを求める
//
//	return YawPitch;
//}
//
////=========================================================
////虹色を計算する
////=========================================================
//D3DXCOLOR CCalculation::CalRaibowColor()
//{
//    //RGBの値をランダムに決め続け虹色を表現する
//	D3DXCOLOR RaibowColor = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
//	RaibowColor.r = float(rand() % 100 + 1) / 100;//R値を求める
//	RaibowColor.g = float(rand() % 100 + 1) / 100;//G値を求める
//	RaibowColor.b = float(rand() % 100 + 1) / 100;//B値を求める
//	return RaibowColor;
//}
////===========================================================================================================

////=========================================================
//// スクリーン座標をワールド座標に変換
////=========================================================
//D3DXVECTOR3* CCalculation::CalcScreenToWorld(D3DXVECTOR3* pout, float Sx, float Sy, float fZ, int Screen_w, int Screen_h, D3DXMATRIX* View, D3DXMATRIX* Prj)
//{
//	// 各行列の逆行列を算出（ビュー、プロジェクションマトリックスの逆行列をかけるのは、カメラの位置に2DのUIが出ていると定義できるから)
//	// 逆行列とは、値に値-1をかけ、掛け合わされる前に戻すこと
//	D3DXMATRIX InvView, InvPrj, VP, InvViewport;
//	D3DXMatrixInverse(&InvView, NULL, View);//ビューマトリックスとの逆光列をかけてワールド座標を求める（ビューマトリックスの向きや位置に応じて変換されているので）
//	D3DXMatrixInverse(&InvPrj, NULL, Prj);  //プロジェクションマトリックスとの逆行列（見え方（平行投影、視野角など）を変えているので、逆行列を掛け合わせ、もとに戻す必要がある）
//	D3DXMatrixIdentity(&VP);
//
//	// スケーリングの値を変えている。スクリーン座標の中心を画面中央にする（NDC座標を使用して変換するので、スクリーン座標は０～１２８０、なお、
//	//NDC座標はー１～１だから、変換する際に合わせる必要がある(１２８０　＝　６４０）、（０　＝　ー６４０)
//	VP._11 = Screen_w / 2.0f; VP._22 = -Screen_h / 2.0f;//スケーリング成分（Xそのまま、Yは上を正に）
//	VP._41 = Screen_w / 2.0f; VP._42 = Screen_h / 2.0f; //平行移動成分（中心を原点に合わせる)
//	D3DXMatrixInverse(&InvViewport, NULL, &VP);//NDC座標に合わせる
//
//	//自分
//	D3DXVECTOR3 MyPos = D3DXVECTOR3(Sx,Sy,fZ);//スクリーン座標と深度値
//
//	// 逆変換
//	D3DXMATRIX tmp = InvViewport * InvPrj * InvView;// ワールド座標を求める（InvViewport「スクリーン座標→NDC座標」、InvPrj「NDC座標→ビュー空間」、InvView「ビュー空間→ワールド座標」)
//	                                                //この行列に深度値とスクリーン座標を合わせることで、深度値に合わせた座標変換が可能になる)
//	D3DXVec3TransformCoord(pout, &MyPos, &tmp);     //位置を求める
//	return pout;
//}
////===========================================================================================================

////=========================================================
//// ワールド座標をスクリーン座標に変換する
////=========================================================
//D3DXVECTOR3 CCalculation::CalcWorldToScreenNoViewport(D3DXVECTOR3 worldPos, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, float screenWidth, float screenHeight)
//{
//	//ワールド座標をクリップ座標に変換
//	D3DXVECTOR4 ClipSpacePos;
//	D3DXMATRIX mtxTrans;
//
//	mtxTrans = viewMatrix * projectionMatrix; // ビューポート、プロジェクション変換
//	D3DXVec3Transform(&ClipSpacePos, &worldPos, &mtxTrans); // 上記の変換した時のワールド座標の位置を求める
//
//	//透視除算（クリップ座標からNDC空間へ）（本来はGPUが勝手にやるが、データとしては出ないので自分で求める)
//	if (ClipSpacePos.w != 0.0f)
//	{//X、Y、Zは、カメラからの相対的な位置を表している（変換された位置)。
//	 //Wは、カメラからの距離（深度情報）を表している
//		ClipSpacePos.x /= ClipSpacePos.w;
//		ClipSpacePos.y /= ClipSpacePos.w;
//		ClipSpacePos.z /= ClipSpacePos.w;
//	}
//
//	//スクリーン座標へ変換
//	D3DXVECTOR3 ScreenPos;
//	ScreenPos.x = (ClipSpacePos.x * 0.5f + 0.5f) * screenWidth;           //真ん中を０にするため＋０．５ｆ、ー１なら左端、＋１なら右端となる
//	ScreenPos.y = (1.0f - (ClipSpacePos.y * 0.5f + 0.5f)) * screenHeight; //真ん中を０にするため＋０．５ｆ、下が正となり、上が０となるので、１の時は０になる、ー１の時は１となる
//	ScreenPos.z = ClipSpacePos.z;                                         //深度値（０～１）の範囲
//
//	return ScreenPos;
//}
////===========================================================================================================

////=========================================================
////桁数を計算する
////=========================================================
//int CCalculation::CalculationDigit(int nNum)
//{
//	int nDigit = 0;
//	if (nNum == 0)
//	{//数値がないので1桁
//		return 1;
//	}
//
//	while (nNum != 0) {//intを10で割って値が０になるまで繰り返し、桁数を計算する
//		nNum /= 10;
//		nDigit++;
//	}
//	return nDigit;
//}

////=========================================================
////指定した桁数を取得する(numberが345、positionが2だったら)→345 / 100 = 3.45 = 3 % 10 = 3)
////=========================================================
//int CCalculation::getDigit(int number, int position)
//{
//	int divisor = static_cast<int>(std::pow(10, position));
//	return (number / divisor) % 10;
//}
////===========================================================================================================

////=========================================================
////ベクトルを計算する
////=========================================================
//D3DXVECTOR3 CCalculation::CalcVec(D3DXVECTOR3 MyPos, D3DXVECTOR3 AimPos, bool bNormalize)
//{
//	D3DXVECTOR3 Vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//
//	//ベクトルを求める
//	Vec = AimPos - MyPos;//目的の位置とのベクトルを求める
//
//	if (bNormalize == true)
//	{//正規化するかどうか
//		D3DXVec3Normalize(&Vec, &Vec);
//	}
//	return Vec;
//}
////===========================================================================================================

////=====================================================================
////ラジアンを方向ベクトルに変換する
////=====================================================================
//D3DXVECTOR3 CCalculation::RadToVec(const D3DXVECTOR3& Rot)
//{
//	D3DXVECTOR3 RotToVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//	RotToVec.x = cosf(Rot.x) * sinf(Rot.y);
//	RotToVec.y = sinf(Rot.x);
//	RotToVec.z = cosf(Rot.x) * cosf(Rot.y);
//
//	D3DXVec3Normalize(&RotToVec, &RotToVec);
//	return RotToVec;
//}
////===========================================================================================================
//
////=====================================================================
////方向ベクトルに対して点が左右どちらにいるかを判定する
////=====================================================================
//float CCalculation::DetermineSide3D(const D3DXVECTOR3& origin, const D3DXVECTOR3& direction, const D3DXVECTOR3& up, const D3DXVECTOR3& point)
//{
//	// 相対ベクトルを計算
//	D3DXVECTOR3 Relative = { point.x - origin.x, point.y - origin.y, point.z - origin.z };
//
//	// 右方向ベクトルを計算
//	D3DXVECTOR3 Right = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//	D3DXVec3Cross(&Right,&direction,&up);
//
//	// 右方向ベクトルと相対ベクトルの内積を計算
//	float fDot = D3DXVec3Dot(&Right,&Relative);
//
//	return fDot;
//}
////===========================================================================================================

////=====================================================================
////オブジェクトの向きをホーミングさせる
////=====================================================================
//D3DXVECTOR3 CCalculation::HormingVecRotXZ(float& fRotMove, const D3DXVECTOR3& MyPos, const D3DXVECTOR3& AimPos, float CorrectionRot, float fSpeed)
//{
//	float fRotDest = 0.0f;//目的の位置への角度
//	float fRotDiff = 0.0f;//現在の角度と目的の角度の差分
//	float fVXaim = 0.0f;  //Xベクトル
//	float fVZaim = 0.0f;  //Yベクトル
//	D3DXVECTOR3 Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//移動量
//	//===================================
//	//XZベクトルを出す
//	//===================================
//	fVXaim = AimPos.x - MyPos.x;
//	fVZaim = AimPos.z - MyPos.z;
//
//	fRotDest = atan2f(fVXaim, fVZaim);
//
//	//==========================================================================================================================================================
//
//	//現在の移動方向の角度と、目的の移動方向の角度の差分を求める
//	fRotDiff = fRotDest - fRotMove;
//
//	//===============================
//	//角度差分の修正
//	//===============================
//	//向きの差分の調整「ジンバルロックを回避」(3.14を超えたら近い向きに補正）
//	if (fRotDiff > D3DX_PI)
//	{
//		fRotDiff -= D3DX_PI * 2;
//	}
//	else if (fRotDiff < -D3DX_PI)
//	{
//		fRotDiff += D3DX_PI * 2;
//	}
//
//	//==============================================================================================
//	fRotMove += fRotDiff * CorrectionRot;//移動方向（角度補正）
//
//	//向きの調整「ジンバルロックを回避」（カメラを基準に値を3.14～-3.14の中に固定したいので・・・）
//	if (fRotMove >= D3DX_PI)
//	{//3.14→-3.14にする
//		fRotMove -= D3DX_PI * 2;
//	}
//	else if (fRotMove <= -D3DX_PI)
//	{//-3.14→3.14にする
//		fRotMove += D3DX_PI * 2;
//	}
//
//	//移動量を更新する
//	Move.x = sinf(fRotMove) * fSpeed;
//	Move.z = cosf(fRotMove) * fSpeed;
//
//	return Move;
//}
////===========================================================================================================


////=====================================================================
////向きの補正を行う
////=====================================================================
//float CCalculation::CorrectionRot(float fRot)
//{
//	// 向きの値を円周内に固定
//	if (fRot > D3DX_PI)
//	{
//		fRot -= D3DX_PI * 2;
//	}
//	if (fRot < -D3DX_PI)
//	{
//		fRot += D3DX_PI * 2;
//	}
//
//	return fRot;
//}
////===========================================================================================================

//*******************************************************************************************************************
// ベクトル名前空間
//*******************************************************************************************************************

//===================================================================================================================
// 向きを方向ベクトルへ変える
//===================================================================================================================
D3DXVECTOR3 Calculation::Vec::RadTo(const D3DXVECTOR3& Rot)
{
	// === 処理に使用する情報を宣言、初期化 ===

	D3DXVECTOR3 RotToVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f); // ラジアンをベクトルにする変数 

	// === 計算開始 ===

	RotToVec.x = cosf(Rot.x) * sinf(Rot.y);
	RotToVec.y = sinf(Rot.x);
	RotToVec.z = cosf(Rot.x) * cosf(Rot.y);

	// 正規化する
	D3DXVec3Normalize(&RotToVec, &RotToVec);

	return RotToVec;
}

//===================================================================================================================
// ベクトルに対し左右どちらにいるかを判定する
//===================================================================================================================
float Calculation::Vec::DetermineSide(const D3DXVECTOR3& origin, const D3DXVECTOR3& direction, const D3DXVECTOR3& up, const D3DXVECTOR3& point)
{
	// 相対ベクトルを計算
	D3DXVECTOR3 Relative = { point.x - origin.x, point.y - origin.y, point.z - origin.z };

	// 右方向ベクトルを計算
	D3DXVECTOR3 Right = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVec3Cross(&Right, &direction, &up);

	// 右方向ベクトルと相対ベクトルの内積を計算
	float fDot = D3DXVec3Dot(&Right, &Relative);

	return fDot;
}

//*******************************************************************************************************************
// 移動量名前空間
//*******************************************************************************************************************

//===================================================================================================================
// 三次元移動量乱数値を求める
//===================================================================================================================
D3DXVECTOR3 Calculation::Move::Rand3D(int nMaxSpeed, int nDivisionMaxSpeed)
{
	// === 処理に使用する情報を宣言、初期化 ===

	D3DXVECTOR3 Speed = { 0.0f,0.0f,0.0f }; // 速度
	bool bMinus = false; // 値をマイナスにするかどうか

	// === X方向のランダムな速度を求める ===

	// X軸の速度の乱数値
	Speed.x = static_cast<float>(rand() % nMaxSpeed) / nDivisionMaxSpeed;

	bMinus = rand() % 2; // 右か左かを決める
	
	// 値にマイナスを掛ける
	if (bMinus)
	{
		Speed.x *= -1;
	}

	// === Y方向のランダムな速度を決める

	// Y軸の速度の乱数値
	Speed.y = static_cast<float>(rand() % nMaxSpeed) / nDivisionMaxSpeed;

	bMinus = rand() % 2; // 上か下を決める

	// 値にマイナスを掛ける
	if (bMinus)
	{
		Speed.y *= -1;
	}

	// === Z方向のランダムな向きを決める ===

	bMinus = rand() % 2; // 前か後ろかを決める

	// Z軸の速度の乱数値
	Speed.z = static_cast<float>(rand() % nMaxSpeed) / nDivisionMaxSpeed;

	// 値にマイナスを掛ける
	if (bMinus)
	{
		Speed.z *= -1;
	}

	return Speed;
}

//===================================================================================================================
// 目的地への移動量計算
//===================================================================================================================
D3DXVECTOR3 Calculation::Move::DirrectionToTarget(D3DXVECTOR3 SelfPos, D3DXVECTOR3 AimPos, float fSpeed)
{
	// === 処理に使用する情報を宣言、初期化 ===

	D3DXVECTOR3 
		VecAim = D3DXVECTOR3(0.0f, 0.0f, 0.0f),       // 目的地へのベクトル
	    ResultMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   // 移動量結果

	float fVLaim = 0.0f; // ベクトル長さ

	// === 計算開始 ===

	// ベクトル計算
	VecAim = AimPos - SelfPos;

	//ベクトルの大きさを求める
	fVLaim = sqrtf(powf(VecAim.x, 2) + powf(VecAim.y, 2) + powf(VecAim.z, 2));

	//方向ベクトルを正規化し、それぞれの軸に対する移動量を求める
	ResultMove.x = VecAim.x / fVLaim * fSpeed;
	ResultMove.y = VecAim.y / fVLaim * fSpeed;
	ResultMove.z = VecAim.z / fVLaim * fSpeed;

	return ResultMove;
}

//===================================================================================================================
// 入力による移動処理
//===================================================================================================================
bool Calculation::Move::MovementInput(bool bUseStick, D3DXVECTOR3& Pos, D3DXVECTOR3& Move, float fSpeed,
	MOVEAIM MoveAim, float fAddAimRot, float& fRot)
{
	// === 処理に使用する情報を宣言、初期化 ===

	float
		fMoveX = 0.0f, // X方向の移動量
	    fMoveZ = 0.0f; // Z方向の移動量

	bool 
		bMove = false,         // 移動しているかどうか 
        bUseController = true; // コントローラーを使用するかどうか


	D3DXVECTOR3 PosFuture = { 0.0f,0.0f,0.0f }; // 1f後の位置

	// === 入力による移動処理を開始 ===

	bMove = CManager::GetInputJoypad()->GetLStickPress(8, 0.0f); // コントローラーの入力

	// この時点でコントローラーの入力がされていない場合、キー入力の受付を開始
	if (bMove == false)
	{
		bUseController = false;//コントローラーは使用しない

		// 前方向に入力したなら前方向へのベクトル
		if (CManager::GetInputKeyboard()->GetPress(DIK_W) == true || CManager::GetInputJoypad()->GetPress(CInputJoypad::JOYKEY::UP) == true)
		{
			fMoveZ = 1.0f;
		}
		//後ろ方向に入力したなら後ろ方向へのベクトル
		else if (CManager::GetInputKeyboard()->GetPress(DIK_S) == true || CManager::GetInputJoypad()->GetPress(CInputJoypad::JOYKEY::DOWN) == true)
		{
			fMoveZ = -1.0f;
		}

		// 右方向に入力したなら右方向へのベクトル
		if (CManager::GetInputKeyboard()->GetPress(DIK_D) == true || CManager::GetInputJoypad()->GetPress(CInputJoypad::JOYKEY::RIGHT) == true)
		{
			fMoveX = 1.0f;
		}
		// 左方向に入力したなら左方向へのベクトル
		else if (CManager::GetInputKeyboard()->GetPress(DIK_A) == true || CManager::GetInputJoypad()->GetPress(CInputJoypad::JOYKEY::LEFT) == true)
		{
			fMoveX = -1.0f;
		}

		// 上記の処理でボタンを入力していたら動いているとみなす
		if (fMoveX != 0.0f || fMoveZ != 0.0f)
		{
			bMove = true;

		}
		// 入力を受け付けていないので動いていない
		else
		{
			bMove = false;
		}
	}

	// 移動状態なら
	if (bMove == true)
	{
		float fMoveRot = 0.0f; // 移動する向きを決める

		// コントローラーの左スティックから角度を求める
		if (bUseController == true)
		{
			fMoveRot = CManager::GetInputJoypad()->GetLStickAimRot();
		}
		// 比から角度を求める（Z軸の正方向が前なので、Z軸を基準「右引数」にX方向の角度「左引数」を求める）
		else
		{
			fMoveRot = atan2f(fMoveX, fMoveZ);
		}

		fMoveRot += fAddAimRot; // 向きを加算し補正

		// どの面を軸に移動するかを決める
		switch (MoveAim)
		{
		case Calculation::Move::MOVEAIM::XY: // XY方向を基準に移動する
			Move.x = sinf(fMoveRot) * fSpeed;
			Move.y = cosf(fMoveRot) * fSpeed;
			break;
		case Calculation::Move::MOVEAIM::XZ: // XZ方向を基準に移動する
			Move.x = sinf(fMoveRot) * fSpeed;
			Move.z = cosf(fMoveRot) * fSpeed;
			break;
		case Calculation::Move::MOVEAIM::ZY: // ZY方向を基準に移動する
			Move.z = sinf(fMoveRot) * fSpeed;
			Move.y = cosf(fMoveRot) * fSpeed;
			break;
		default:
			break;
		}

		// 1f後の位置を計算
		PosFuture = Pos + Move;

		// 1f後の位置と現在の位置のベクトルから向くべき角度を求める
		fRot = atan2f(PosFuture.x - Pos.x, PosFuture.z - Pos.z);
	}

	return bMove;
}

//===================================================================================================================
// XZ方向で目的地へホーミングさせる処理
//===================================================================================================================
D3DXVECTOR3 Calculation::Move::HormingToGoalPosXZ(float& fNowRot, const D3DXVECTOR3& SelfPos, const D3DXVECTOR3& GoalPos, float CorrectionRot, float fSpeed)
{
	// === 処理に使用する情報を宣言、初期化 ===

	float 
		fRotDest = 0.0f, // 目的の位置への角度
	    fRotDiff = 0.0f, // 角度差分
	    fVecX = 0.0f,  // Xベクトル
	    fVecZ = 0.0f;  // Zベクトル

	D3DXVECTOR3 Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f); // 移動量

	// === 目的地へのホーミング処理開始 ===

	fVecX = GoalPos.x - SelfPos.x;
	fVecZ = GoalPos.z - SelfPos.z;
	fRotDest = atan2f(fVecX, fVecZ);

	// 現在の移動方向の角度と、目的の移動方向の角度差分を求める
	fRotDiff = fRotDest - fNowRot;

	// 向きの差分を円周上に包む
	fRotDiff = Calculation::Rot::WrapAngle(fRotDiff);

	fNowRot += fRotDiff * CorrectionRot; // 向きを線形補間する

	// 現在の向きを円周上に包む
	fNowRot = Calculation::Rot::WrapAngle(fNowRot);

	// 移動量を更新する
	Move.x = sinf(fNowRot) * fSpeed;
	Move.z = cosf(fNowRot) * fSpeed;

	return Move;
}

//===================================================================================================================
// 目的の高さへ到達するための初速度を求める（v0 = √2gh)
//===================================================================================================================
float Calculation::Move::GetInitialVelocityHeight(float fHeight, float fGravity)
{
	float fAim = sqrtf(2 * fGravity * fHeight);
	return fAim;
}

//*******************************************************************************************************************
// 向き名前空間
//*******************************************************************************************************************

//===================================================================================================================
// ベクトルをYawとPitchに変換する処理
//===================================================================================================================
D3DXVECTOR2 Calculation::Rot::VectorToYawPitch(const D3DXVECTOR3& SelfPos, const D3DXVECTOR3& GoalPos)
{
	// === 処理に使用する情報を宣言、初期化 ===

	D3DXVECTOR2 YawPitch = { 0.0f,0.0f }; // YawとPitchを計算する

   // === 変換処理開始 ===

	// Yaw
	YawPitch.y = atan2f(GoalPos.x - SelfPos.x, GoalPos.z - SelfPos.z); 

	// Pitch
	YawPitch.x = atan2f(GoalPos.y - SelfPos.y, sqrtf(powf(GoalPos.x - SelfPos.x, 2) + powf(GoalPos.z - SelfPos.z, 2))); 

	return YawPitch;
}

//===================================================================================================================
// 現在の向きを目的の向きに線形補間する処理
//===================================================================================================================
float Calculation::Rot::Lerp(float fSelfRot, float fRotAim, float fDecayRot)
{
	float fRotDiff = 0.0f;                                 //向きの差分
	float fCameraRot = CManager::GetCamera()->GetRot().y;  //カメラのYawを取得
	fRotDiff = fRotAim - fSelfRot;

	// 向きの差分を円周上に包み込む
	fRotDiff = Calculation::Rot::WrapAngle(fRotDiff);

	//徐々に目的の向きへ合わせていく
	fSelfRot += fRotDiff * fDecayRot;

	// 向きを円周上に包み込む
	fSelfRot = Calculation::Rot::WrapAngle(fSelfRot);

	return fSelfRot;
}

//===================================================================================================================
// 向きを円周上に包み込む処理
//===================================================================================================================
float Calculation::Rot::WrapAngle(float fRot)
{
	// 向きがD3DX_PIを超えていたらD3DX_PIより下になるまで減らす
	while (fRot > D3DX_PI)
	{
		fRot -= D3DX_PI * 2;
	}

	// 向きが-D3DX_PIを下回っていたら-D3DX_PIより上になるまで増やす
	while (fRot < -D3DX_PI)
	{
		fRot += D3DX_PI * 2;
	}
		
	return fRot;
}

//===================================================================================================================
// 目的地までの放物線角度を求める
//===================================================================================================================
float Calculation::Rot::ParabolaToGoalPos(float fLength, float fGravity, float fSpeed, D3DXVECTOR3 SelfPos, D3DXVECTOR3 GoalPos)
{
	// === 処理に使用する変数を宣言、初期化 ===

	float fRot = 0.0f; // 角度
	float fXG = fLength * fGravity; // 距離×重力
	float fV = fSpeed; // 速度
	float fV2 = powf(fV, 2); // 速度２条

	fRot = asinf(fXG / fV2);

	// 0除算回避
	if (fRot > 0.00001f)
	{
		fRot /= 2;
	}

	// 角度が絶対値なので、目的の位置が自分の位置よりマイナス方向にある場合、負の値にする
	if (GoalPos.x < SelfPos.x)
	{
		fRot *= -1;
	}

	return fRot;
}

//*******************************************************************************************************************
// 距離名前空間
//*******************************************************************************************************************

//===================================================================================================================
// 目的地までの距離を求める処理
//===================================================================================================================
float Calculation::Length::ToGoalPos(D3DXVECTOR3 SelfPos, D3DXVECTOR3 GoalPos)
{
	float fLength = 0.0f;//距離格納用
	fLength = sqrtf(powf(GoalPos.x - SelfPos.x, 2) + powf(GoalPos.y - SelfPos.y, 2) + powf(GoalPos.z - SelfPos.z, 2));//目的の位置との距離を求める
	return fLength;
}

//*******************************************************************************************************************
// 変換名前空間
//*******************************************************************************************************************

//===================================================================================================================
// スクリーン座標をワールド座標に変換
//===================================================================================================================
D3DXVECTOR3* Calculation::Conversion::ScreenToWorld(D3DXVECTOR3* pout, float Sx, float Sy, float fZ, int Screen_w, int Screen_h, D3DXMATRIX* View, D3DXMATRIX* Prj)
{
	// 各行列の逆行列を算出（ビュー、プロジェクションマトリックスの逆行列をかけるのは、カメラの位置に2DのUIが出ていると定義できるから)
	// 逆行列とは、値に値-1をかけ、掛け合わされる前に戻すこと
	D3DXMATRIX InvView, InvPrj, VP, InvViewport; // 計算用マトリックス

	//ビューマトリックスとの逆光列をかけてワールド座標を求める（ビューマトリックスの向きや位置に応じて変換されているので）
	D3DXMatrixInverse(&InvView, NULL, View);

	// プロジェクションマトリックスとの逆行列（見え方（平行投影、視野角など）を変えているので、逆行列を掛け合わせ、もとに戻す必要がある）
	D3DXMatrixInverse(&InvPrj, NULL, Prj);  
	D3DXMatrixIdentity(&VP);

	// スケーリングの値を変えている。スクリーン座標の中心を画面中央にする（NDC座標を使用して変換するので、スクリーン座標は０～１２８０、なお、
	//NDC座標はー１～１だから、変換する際に合わせる必要がある(１２８０　＝　６４０）、（０　＝　ー６４０)
	VP._11 = Screen_w / 2.0f; VP._22 = -Screen_h / 2.0f; // スケーリング成分（Xそのまま、Yは上を正に）
	VP._41 = Screen_w / 2.0f; VP._42 = Screen_h / 2.0f;  // 平行移動成分（中心を原点に合わせる)
	D3DXMatrixInverse(&InvViewport, NULL, &VP); // NDC座標に合わせる

	//自分
	D3DXVECTOR3 MyPos = D3DXVECTOR3(Sx, Sy, fZ);//スクリーン座標と深度値

	// 逆変換
	D3DXMATRIX tmp = InvViewport * InvPrj * InvView;// ワールド座標を求める（InvViewport「スクリーン座標→NDC座標」、InvPrj「NDC座標→ビュー空間」、InvView「ビュー空間→ワールド座標」)
													//この行列に深度値とスクリーン座標を合わせることで、深度値に合わせた座標変換が可能になる)

	D3DXVec3TransformCoord(pout, &MyPos, &tmp); // 座標変換後の位置を求める
	return pout;
}

//===================================================================================================================
// ワールド座標をスクリーン座標に変換
//===================================================================================================================
D3DXVECTOR3 Calculation::Conversion::WorldToScreen(D3DXVECTOR3 worldPos, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, float screenWidth, float screenHeight)
{
	// === 処理に使用する情報を宣言、初期化 ===
	
	//ワールド座標をクリップ座標に変換
	D3DXVECTOR4 ClipSpacePos;
	D3DXMATRIX mtxTrans;

	// === ワールド座標をスクリーン座標に変換する処理を開始 ===

	mtxTrans = viewMatrix * projectionMatrix; // ビュー、プロジェクション変換
	D3DXVec3Transform(&ClipSpacePos, &worldPos, &mtxTrans); // 上記の変換した時のワールド座標の位置を求める

	//透視除算（クリップ座標からNDC空間へ）（本来はGPUが勝手にやるが、データとしては出ないので自分で求める)
	if (ClipSpacePos.w != 0.0f)
	{//X、Y、Zは、カメラからの相対的な位置を表している（変換された位置)。
	 //Wは、カメラからの距離（深度情報）を表している
		ClipSpacePos.x /= ClipSpacePos.w;
		ClipSpacePos.y /= ClipSpacePos.w;
		ClipSpacePos.z /= ClipSpacePos.w;
	}

	//スクリーン座標へ変換
	D3DXVECTOR3 ScreenPos;
	ScreenPos.x = (ClipSpacePos.x * 0.5f + 0.5f) * screenWidth; // 真ん中を０にするため＋０．５ｆ、ー１なら左端、＋１なら右端となる
	ScreenPos.y = (1.0f - (ClipSpacePos.y * 0.5f + 0.5f)) * screenHeight; // 真ん中を０にするため＋０．５ｆ、下が正となり、上が０となるので、１の時は０になる、ー１の時は１となる
	ScreenPos.z = ClipSpacePos.z; // 深度値（０～１）の範囲

	return ScreenPos;
}

//*******************************************************************************************************************
// 値名前空間
//*******************************************************************************************************************

//===================================================================================================================
// 桁数計算処理
//===================================================================================================================
int Calculation::Value::CalcDigit(int nValue)
{
	int nDigit = 0; // 桁数

	// 数値がないので１桁です
	if (nValue == 0)
	{
		return 1;
	}

	// １０で割って値が０になるまで繰り返し桁数を増やします
	while (nValue != 0)
	{
		nValue /= 10;
		nDigit++;
	}

	return nDigit;
}

//===================================================================================================================
// 指定した桁を取得
//===================================================================================================================
int Calculation::Value::GetDigit(int number, int position)
{
	int divisor = static_cast<int>(std::pow(10, position));
	return (number / divisor) % 10;
}

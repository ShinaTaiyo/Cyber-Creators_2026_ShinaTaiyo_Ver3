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
	float fLength = 0.0f;
	fLength = sqrtf(powf(PurposePos.x - Pos.x, 2) + powf(PurposePos.y - Pos.y, 2) + powf(PurposePos.z - Pos.z, 2));
	return fLength;
}
//==========================================================================================================

//=========================================================
//XYベクトルを計算する
//=========================================================
float CCalculation::CalculationXYaim(D3DXVECTOR3 Pos, D3DXVECTOR3 PurposePos)
{
	float fVXaim = PurposePos.x - Pos.x;
	float fVYaim = PurposePos.y - Pos.y;
	float fVLaim = atan2f(fVXaim, fVYaim);
	return fVLaim;
}
//==========================================================================================================

//=========================================================
//放物線の角度を計算する
//=========================================================
float CCalculation::CalculationParabola(float fLength, float fGravity, float fSpeed, D3DXVECTOR3 Pos, D3DXVECTOR3 PurposePos)
{
	float fRot = 0.0f;//角度

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
	float fRandRot = float(rand() % 628 + 1) / 100;
	return fRandRot;
}
//===========================================================================================================

//=========================================================
//2D方向の向きの補正を行う
//=========================================================
float CCalculation::CalculationCollectionRot2D(float fMyRot, float fRotAim, float fDecayRot, bool bCameraOffSet)
{
	float fRotDiff = 0.0f;//向きの差分
	float fCameraRot = CManager::GetCamera()->GetRot().y;
	fRotDiff = fRotAim - fMyRot;

	//向きの差分の調整
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
	{
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
	{
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
bool CCalculation::CaluclationMove(bool bUseStick, D3DXVECTOR3& Move, float fSpeed, MOVEAIM MoveAim, float& fRot)
{
	float fCameraRot = CManager::GetCamera()->GetRot().y;
	float fMoveX = 0.0f;                                            //X方向の移動量
	float fMoveZ = 0.0f;                                            //Z方向の移動量
	bool bMove = false;                                             //移動しているかどうか 
	bool bUseController = true;                                    //コントローラーを使用するかどうか
	bMove = CManager::GetInputJoypad()->GetLStickPress(8, 0.0f);//コントローラーの入力

	if (bMove == false)
	{//この時点でコントローラーの入力がされていない場合、キー入力の受付を開始
		bUseController = false;//コントローラーは使用しない
		if (CManager::GetInputKeyboard()->GetPress(DIK_W) == true || CManager::GetInputJoypad()->GetPress(CInputJoypad::JOYKEY::UP) == true)
		{
			fMoveZ = 1.0f;
		}
		else if (CManager::GetInputKeyboard()->GetPress(DIK_S) == true || CManager::GetInputJoypad()->GetPress(CInputJoypad::JOYKEY::DOWN) == true)
		{
			fMoveZ = -1.0f;
		}
		if (CManager::GetInputKeyboard()->GetPress(DIK_D) == true || CManager::GetInputJoypad()->GetPress(CInputJoypad::JOYKEY::RIGHT) == true)
		{
			fMoveX = 1.0f;
		}
		else if (CManager::GetInputKeyboard()->GetPress(DIK_A) == true || CManager::GetInputJoypad()->GetPress(CInputJoypad::JOYKEY::LEFT) == true)
		{
			fMoveX = -1.0f;
		}

		if (fMoveX != 0.0f || fMoveZ != 0.0f)
		{
			bMove = true;//移動状態
		}
		else
		{
			bMove = false;//待機状態
		}
	}
	if (bMove == true)
	{//移動状態なら
		//カメラを基準に向きを決める
		if (bUseController == true)
		{
			fRot = CManager::GetInputJoypad()->GetLStickAimRot();
			CManager::GetDebugText()->PrintDebugText("スティックの向き：%f\n",fRot);
			CManager::GetDebugText()->PrintDebugText("カメラの向き：%f\n",fCameraRot);
			fRot += fCameraRot;
			CManager::GetDebugText()->PrintDebugText("目的の向き：%f\n",fRot);
		}
		else
		{//比から角度を求める（Z軸の正方向が前なので、Z軸を基準「右引数」にX方向の角度「左引数」を求める）
			fRot = atan2f(fMoveX,fMoveZ) + fCameraRot;
			CManager::GetDebugText()->PrintDebugText("目的の向き：%f\n", fRot);
		}
		switch (MoveAim)
		{
		case MOVEAIM_XY:
			Move.x = sinf(fRot) * fSpeed;
			Move.y = cosf(fRot) * fSpeed;
			break;
		case MOVEAIM_XZ:
			Move.x = sinf(fRot) * fSpeed;
			Move.z = cosf(fRot) * fSpeed;
			break;
		case MOVEAIM_ZY:
			Move.z = sinf(fRot) * fSpeed;
			Move.y = cosf(fRot) * fSpeed;
			break;
		default:
			break;
		}
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
	float fVLaim = 0.0f;                     //総合ベクトル

	//==========================
	//３次元ベクトルを取る
	//==========================

	//方向ベクトルの計算
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

	bMinus = rand() % 2;
	Speed.x = static_cast<float>(rand() % nMathSpeed) / nDivisionSpeed;
	if (bMinus == true)
	{
		Speed.x *= -1;
	}
	bMinus = rand() % 2;
	Speed.y = static_cast<float>(rand() % nMathSpeed) / nDivisionSpeed;
	if (bMinus == true)
	{
		Speed.y *= -1;
	}
	bMinus = rand() % 2;
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
	D3DXCOLOR RaibowColor = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	RaibowColor.r = float(rand() % 100 + 1) / 100;
	RaibowColor.g = float(rand() % 100 + 1) / 100;
	RaibowColor.b = float(rand() % 100 + 1) / 100;
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
	D3DXMatrixInverse(&InvView, NULL, View);//ビューマトリックスとの逆光列（カメラの視点、注視点などを踏まえて、変換されているので戻す）
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
// XZ平面とスクリーン座標の交点算出関数
//=========================================================
D3DXVECTOR3* CCalculation::CalcScreenToXZ(D3DXVECTOR3* pout,float Sx,float Sy, int Screen_w, int Screen_h, D3DXMATRIX* View, D3DXMATRIX* Prj)
{
	D3DXVECTOR3 nearpos;
	D3DXVECTOR3 farpos;
	D3DXVECTOR3 ray;
	bool bCross = false;

	D3DXVECTOR3 Pos1 = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	CalcScreenToWorld(&nearpos, Sx, Sy, 0.0f, Screen_w, Screen_h, View, Prj);//（椎名）多分カメラの位置
	CalcScreenToWorld(&farpos, Sx, Sy, 1.0f, Screen_w, Screen_h, View, Prj); //（椎名）多分描画範囲の一番奥の位置
	ray = farpos - nearpos;

	D3DXVec3Normalize(&ray, &ray);

	nearpos *= -1;
	// 床との交差が起きている場合は交点を
	// 起きていない場合は遠くの壁との交点を出力
	//if (ray.y <= 0) {
	//	// 床交点
	//	float Lray = D3DXVec3Dot(&ray, &Pos1);
	//	float LP0 = D3DXVec3Dot(&nearpos, &Pos1);
	//	*pout = nearpos + (LP0 / Lray) * ray;
	//	bCross = true;
	//}
	//else {
	* pout = farpos;
	bCross = false;
	//}

	return pout;
}
//===========================================================================================================

//=========================================================
// レイと球の衝突判定
//=========================================================
bool CCalculation::CalcRaySphere(D3DXVECTOR3 LayPos, D3DXVECTOR3 LayVec, D3DXVECTOR3 SphereSenterPos, float r, D3DXVECTOR3& CollisionStartPos, D3DXVECTOR3& CollisoinEndPos)
{
	SphereSenterPos.x = SphereSenterPos.x - LayPos.x;
	SphereSenterPos.y = SphereSenterPos.y - LayPos.y;
	SphereSenterPos.z = SphereSenterPos.z - LayPos.z;

	float A = LayVec.x * LayVec.x + LayVec.y * LayVec.y + LayVec.z * LayVec.z;
	float B = LayVec.x * SphereSenterPos.x + LayVec.y * SphereSenterPos.y + LayVec.z * SphereSenterPos.z;
	float C = SphereSenterPos.x * SphereSenterPos.x + SphereSenterPos.y * SphereSenterPos.y + SphereSenterPos.z * SphereSenterPos.z - r * r;
	
	if (A == 0.0f)
		return false; // レイの長さが0

	float s = B * B - A * C;
	if (s < 0.0f)
		return false; // 衝突していない

	s = sqrtf(s);
	float a1 = (B - s) / A;
	float a2 = (B + s) / A;

	if (a1 < 0.0f || a2 < 0.0f)
		return false; // レイの反対で衝突

	//衝突開始位置を求める
	CollisionStartPos.x = LayPos.x + a1 * LayVec.x;
	CollisionStartPos.y = LayPos.y + a1 * LayVec.y;
	CollisionStartPos.z = LayPos.z + a1 * LayVec.z;

	//衝突終了位置を求める
	CollisoinEndPos.x = LayPos.x + a2 * LayVec.x;
	CollisoinEndPos.y = LayPos.y + a2 * LayVec.y;
	CollisoinEndPos.z = LayPos.z + a2 * LayVec.z;

	return true;
}
//===========================================================================================================

//=========================================================
//目的の位置と狙っている位置とのレイが一致しているかどうかを判定
//=========================================================
bool CCalculation::CalcMatchRay(D3DXVECTOR3 AimPos, float fSx, float fSy, int nScreen_w, int nScreen_h, D3DXMATRIX* View, D3DXMATRIX* Prj)
{
	D3DXVECTOR3 nearpos1;
	D3DXVECTOR3 farpos1;
	D3DXVECTOR3 ray1;

	D3DXVECTOR3 Pos1 = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	CalcScreenToWorld(&nearpos1,fSx,fSy, 0.0f, nScreen_w, nScreen_h, View, Prj);//（椎名）多分カメラの位置
	CalcScreenToWorld(&farpos1, fSx,fSy, 1.0f, nScreen_w, nScreen_h, View, Prj); //（椎名）多分描画範囲の一番奥の位置
	ray1 = farpos1 - nearpos1;

	D3DXVec3Normalize(&ray1, &ray1);

	D3DXVECTOR3 farpos2;
	D3DXVECTOR3 ray2;

	ray2 = AimPos - nearpos1;//目的の位置とカメラの位置のレイを求める

	D3DXVec3Normalize(&ray2, &ray2);

	if (ray1.x >= ray2.x - 0.05f && ray1.x <= ray2.x + 0.05f &&
		ray1.y >= ray2.y - 0.05f && ray1.y <= ray2.y + 0.05f &&
		ray1.z >= ray2.z - 0.05f && ray1.z <= ray2.z + 0.05f)
	{
		return true;
	}
	return false;
}
//===========================================================================================================

//=========================================================
//点と辺の距離を求める
//=========================================================
float CCalculation::pointLineDistance(float cx, float cy, float x1, float y1, float x2, float y2)
{
	float dx = x2 - x1;
	float dy = y2 - y1;

	// 円の中心から辺の端点までのベクトルを計算
	float t = ((cx - x1) * dx + (cy - y1) * dy) / (dx * dx + dy * dy);

	// tの範囲を[0, 1]に制限し、円の中心に最も近い点を見つける
	t = (std::max)(0.0f, (std::min)(1.0f, t));

	// 最短距離を計算
	float closestX = x1 + t * dx;
	float closestY = y1 + t * dy;
	float distanceX = cx - closestX;
	float distanceY = cy - closestY;

	return std::sqrt(distanceX * distanceX + distanceY * distanceY);
}
//===========================================================================================================

//=========================================================
//正方形と円の当たり判定（2D)
//=========================================================
bool CCalculation::checkCollisionCircleRectangle(float cx, float cy, float r, float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4)
{
	// 四角形の各辺に対して円との距離を計算
	if (pointLineDistance(cx, cy, x1, y1, x2, y2) <= r) return true;
	if (pointLineDistance(cx, cy, x2, y2, x3, y3) <= r) return true;
	if (pointLineDistance(cx, cy, x3, y3, x4, y4) <= r) return true;
	if (pointLineDistance(cx, cy, x4, y4, x1, y1) <= r) return true;

	// どの辺とも当たっていない場合はfalse
	return false;
}
//===========================================================================================================

//=========================================================
//桁数を計算する
//=========================================================
int CCalculation::CalculationDigit(int nNum)
{
	int nDigit = 0;
	while (nNum != 0) {
		nNum /= 10;
		nDigit++;
	}
	return nDigit;
}

//=========================================================
//指定した桁数を取得する
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
	Vec = AimPos - MyPos;

	if (bNormalize == true)
	{//正規化するかどうか
		D3DXVec3Normalize(&Vec, &Vec);
	}
	return Vec;
}
//===========================================================================================================

//=========================================================
//仰角を計算する
//=========================================================
float CCalculation::CalcElevationAngle(const D3DXVECTOR3& MyPos, const D3DXVECTOR3& AimPos)
{
	D3DXVECTOR3 Vec = CalcVec(MyPos, AimPos, false);
	float fElevAngle = atan2f(Vec.y, sqrtf(powf(Vec.x, 2) + powf(Vec.z, 2)));
	return fElevAngle;
}
//===========================================================================================================

//=========================================================
//二つのベクトルがなす角
//=========================================================
float CCalculation::GetAngleBetweenVectors(const D3DXVECTOR3& v1, const D3DXVECTOR3& v2)
{
	// ベクトルの正規化
	D3DXVECTOR3 v1_normalized = v1;
	D3DXVECTOR3 v2_normalized = v2;
	D3DXVec3Normalize(&v1_normalized, &v1);
	D3DXVec3Normalize(&v2_normalized, &v2);

	// 内積を計算
	float dotProduct = D3DXVec3Dot(&v1_normalized, &v2_normalized);

	// 内積から角度を計算 (acosでラジアンに変換)(v1 *v2 = |v1| * |v2| cosθ)なので、逆三角関数を求めれば良い
	float angle = acosf(dotProduct);
	return angle; // 結果はラジアン
}
//===========================================================================================================


//=====================================================================
//プレイヤーから見て線形範囲にオブジェクトが入っているかどうかを判定
//=====================================================================
bool CCalculation::IsObjectInFieldOfView(const D3DXVECTOR3& PlayerPos, const D3DXVECTOR3& PlayerViewDir, const D3DXVECTOR3& ObjectPos, float fovAngle, float maxDistance)
{

	// プレイヤーからオブジェクトへの方向ベクトルを計算
	D3DXVECTOR3 objectDir = ObjectPos - PlayerPos;

	// プレイヤーの視線とオブジェクトの方向ベクトルとの角度を計算
	float angle = GetAngleBetweenVectors(PlayerViewDir, objectDir);

	// オブジェクトが視野角度内に入っているかどうかを判定
	if (angle <= fovAngle) {
		// 視野範囲内にあるか確認（距離も判定）
		float distance = D3DXVec3Length(&objectDir);
		if (distance <= maxDistance) {
			return true; // 視野内かつ距離範囲内
		}
	}

	return false; // 視野角度外または距離が遠すぎる
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
	// 符号で判定
	//if (dot > 0) {
	//	return "右側";
	//}
	//else if (dot < 0) {
	//	return "左側";
	//}
	//else {
	//	return "平面上";
	//}
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
	//向きの差分の調整(3.14を超えたら近い向きに補正）
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

	//向きの調整（カメラを基準に値を3.14～-3.14の中に固定したいので・・・）
	if (fRotMove >= D3DX_PI)
	{//3.14→-3.14にする
		fRotMove -= D3DX_PI * 2;
	}
	else if (fRotMove <= -D3DX_PI)
	{//-3.14→3.14にする
		fRotMove += D3DX_PI * 2;
	}

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

//=========================================================
//目的の位置への向きを求める
//=========================================================
void CCalculation::CalcRotToTarget(const D3DXVECTOR3& MyPos, const D3DXVECTOR3& AimPos, float& OutYaw, float& OutPitch)
{
	D3DXVECTOR3 VecAim = D3DXVECTOR3(0.0f, 0.0f, 0.0f); // 方向ベクトル
	float fVLaim = 0.0f; // ベクトルの大きさ

	// 方向ベクトルを求める
	VecAim.x = AimPos.x - MyPos.x;
	VecAim.y = AimPos.y - MyPos.y;
	VecAim.z = AimPos.z - MyPos.z;

	// ベクトルの大きさを求める
	fVLaim = sqrtf(VecAim.x * VecAim.x + VecAim.y * VecAim.y + VecAim.z * VecAim.z);

	if (fVLaim != 0.0f) { // ゼロ除算を防ぐ
		VecAim.x /= fVLaim; // 正規化
		VecAim.y /= fVLaim;
		VecAim.z /= fVLaim;
	}

	// Yaw (水平回転)
	OutYaw = atan2f(VecAim.x, VecAim.z);

	// Pitch (垂直回転)
	OutPitch = asinf(-VecAim.y);
}
//===========================================================================================================

//=========================================================
//YawとPitchを用いて目的の角度を求める
//=========================================================
D3DXVECTOR3 CCalculation::CalcDirectionFromYawPitch(const float Yaw, const float Pitch)
{
	D3DXVECTOR3 direction;

	// Yaw と Pitch を使用して方向ベクトルを計算
	direction.x = cosf(Pitch) * sinf(Yaw);  // X成分
	direction.y = sinf(Pitch);              // Y成分
	direction.z = cosf(Pitch) * cosf(Yaw);  // Z成分

	return direction;
}
//===========================================================================================================

//=========================================================
//目的の位置への角度をまとめて求める
//=========================================================
D3DXVECTOR3 CCalculation::CalcSummarizeRotToTarget(const D3DXVECTOR3& MyPos, const D3DXVECTOR3& AimPos)
{
	float fYaw = 0.0f;
	float fPitch = 0.0f;
	D3DXVECTOR3 Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	CalcRotToTarget(MyPos, AimPos, fYaw, fPitch);
	Rot = CalcDirectionFromYawPitch(fYaw, fPitch);
	return Rot;
}
//===========================================================================================================
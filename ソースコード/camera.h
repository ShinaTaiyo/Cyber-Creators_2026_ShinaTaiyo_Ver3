//==========================================================================
//
//６月３日：3Dオブジェクトを描画する[camera.h]
//Author:ShinaTaiyo
//
//==========================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

//==========================================================================
//インクルード
//==========================================================================
#include "main.h"
//==========================================================================

//==========================================================================
//前方宣言
//==========================================================================
class CCameraState;
class CCameraLengthState;
//==========================================================================
//カメラクラス
//==========================================================================
class CCamera
{
public:

	//===================================================
	//カメラモード列挙型
	//===================================================
	typedef enum
	{
		CAMERATYPE_NONE = 0,  //なし
		CAMERATYPE_BIRD,      //バード
		CAMERATYPE_BESIDE,    //ビサイド
		CAMERATYPE_SIDE,      //サイド
		CAMERATYPE_MAX
	}CAMERATYPE;
	//=================================================================================================

	//===================================================
	//カメラ状態列挙型
	//===================================================
	enum class CAMERASTATE
	{
		NORMAL = 0,
		TURNAROUND,
		MAX
	};

	CCamera();            //コンストラクタ
	~CCamera();           //デストラクタ
	HRESULT Init();       //初期化処理
	void Uninit();        //終了処理
	void Update();        //更新処理
	void SetCamera();     //カメラの設定

	void SetPosR(D3DXVECTOR3 PosR) { m_PosR = PosR; }  //注視点を設定する
	void SetRot(const D3DXVECTOR3 Rot) { m_Rot = Rot; }//向きを設定
	const D3DXVECTOR3& GetRot() const{ return m_Rot; } //向きを取得する
	void SetShake(int nShakeFrame, float fShakePower); //カメラの揺れを設定する
	CAMERATYPE GetCameraType() { return m_CameraType; }//カメラタイプを取得する
	void SetCameraType(CAMERATYPE type) { m_CameraType = type; m_ModeTime = 0; }//カメラタイプを設定する
	void SetPosV(D3DXVECTOR3 PosV) { m_PosV = PosV; }//視点を設定
	D3DXVECTOR3& GetPosV() { return m_PosV; }      //視点を取得
	void SetAddPosV(D3DXVECTOR3 AddPosV) { m_AddPosV = AddPosV; }
	const D3DXVECTOR3& GetAddPosV() const { return m_AddPosV; }
	D3DXVECTOR3& GetPosR() { return m_PosR; }      //注視点を取得
	const D3DXVECTOR3& GetAddPosR() const { return m_AddPosR; }
	void SetAddPosR(D3DXVECTOR3 AddPosR) { m_AddPosR = AddPosR; }
	void SetTurningInfo(float fLength, float fRotSpeed, D3DXVECTOR3 PosR,float fPosVY,float fTurningSpeedY,float fAddLength)
	{
		m_fLength = fLength;            //注視点との距離
		m_fTurningRotSpeed = fRotSpeed; //回転速度
		m_PosR = PosR;                  //注視点
		m_PosV.y = fPosVY;              //支点のY位置
		m_fTurningSpeedY = fTurningSpeedY;//旋回中のY方向の移動量
		m_fAddLength = fAddLength;      //加算距離
	}
	void SetLength(float fLength) { m_fLength = fLength; }
	float& GetLength() { return m_fLength; }

	void SetTurningRotSpeed(float fTurningRotSpeed) { m_fTurningRotSpeed = fTurningRotSpeed; }
	float& GetTurningRotSpeed() { return m_fTurningRotSpeed; }

	void SetTurningSpeedY(float fTurningSpeedY) { m_fTurningSpeedY = fTurningSpeedY; }
	float& GetTurningSpeedY() { return m_fTurningSpeedY; }

	void SetAddLength(float fAddLength) { m_fAddLength = fAddLength; }
	float& GetAddLength() { return m_fAddLength; }

	void SetCustomMode(bool bActive) { m_bCustom = bActive; }

	void ChengeState(CCameraState * pCameraState);//ステートを変える
	void ChengeLengthState(CCameraLengthState* pCameraLengthState);//カメラの距離の状態を変える

	const float& GetPosRToPosVLength() const { return m_fLength; }//中止点と視点の距離を取得する

	static const float & GetInitialLength() { return s_fINITIAL_LENGTH; }//最初の距離を取得する

	//======================================
	//マトリックス系
	//======================================
	D3DXMATRIX* GetMtxView() { return &m_mtxView; }//ビューマトリックスを取得
	D3DXMATRIX* GetMtxProjection() { return &m_mtxProjection; }//プロジェクションマトリックスを取得
	//==========================================================================================================================================
private:
	//======================================
    //静的メンバ宣言
    //======================================
	static const float m_BESIDECAMERALENGTH;      //ビサイドモードのカメラの距離
	static const float s_fINITIAL_LENGTH;         //最初の距離
	//==========================================================================================================================================

	//======================================
	//変数宣言
	//======================================
	D3DXVECTOR3 m_PosV;                           //視点!
	D3DXVECTOR3 m_PosR;                           //注視点!
	D3DXVECTOR3 m_AddPosR;                        //加算注視点
	D3DXVECTOR3 m_AddPosV;                        //加算視点
	D3DXVECTOR3 m_VecU;                           //上方向ベクトル!
	D3DXVECTOR3 m_Rot;                            //向き!
	D3DXMATRIX m_mtxProjection;                   //プロジェクション行列!
	D3DXMATRIX m_mtxView;                         //ビューマトリックス!
	CAMERATYPE m_CameraType;                      //カメラモードの種類!
	D3DXVECTOR3 m_DifferenceLength;               //差分!
	D3DXVECTOR3 m_ZoomSpeed;                      //ズームする速さ!
	D3DXVECTOR3 m_SupportPos;                     //参考位置!

	CAMERASTATE m_State;                          //状態名
	CCameraState* m_pCameraState;                 //カメラの状態クラス
	CCameraLengthState* m_pCameraLengthState;     //カメラの距離の状態クラス

	float m_fLength;                              //距離!
	float m_fAddLength;                           //追加距離!
	float m_fTurningRotSpeed;                     //旋回速度!
	float m_fTurningSpeedY;                       //旋回中のY方向の移動量!

	int m_nShakeFrame;                            //カメラを揺らすフレーム数!
	int m_ModeTime;                               //カメラモードの時間!
	float m_fShakePower;                          //カメラを揺らす力!

	bool m_bCustom;                               //カメラの挙動をカスタムにするかどうか


	//==========================================================================================================================================

	//======================================
	//プロトタイプ宣言
	//======================================
	void BossDefeatCameraProcess();               //ボスを倒したときのカメラの処理を行う
	void TurningCameraProcess();                  //旋回のカメラの処理を行う
	void NormalCameraMove();                      //普通のカメラの注視点を設定し続ける
	void MakeTransparent();                       //すごく近いオブジェクトを透明にする処理
	//==========================================================================================================================================
};

class CCameraState
{
public:
	CCameraState() {};                          //コンストラクタ
	virtual ~CCameraState() {};                 //デストラクタ
	virtual void Process(CCamera* pCamera) {};  //処理
};

//普通の状態
class CCameraState_Normal : public CCameraState
{
public:
	CCameraState_Normal();//コンストラクタ
	~CCameraState_Normal();//デストラクタ
	void Process(CCamera* pCamera);//処理
private:
	static const float s_fNORMAL_AROUNDROTSPEED;  //カメラの回転速度
};

//狙った向きを向かせる
class CCameraState_TurnAround : public CCameraState
{
public:
	CCameraState_TurnAround(D3DXVECTOR3 AimRot,float fAdjustTurnSpeed);//コンストラクタ
	~CCameraState_TurnAround() override;//デストラクタ
	void Process(CCamera * pCamera) override;//処理
private:

	const D3DXVECTOR3 m_AimRot;     //目的の向き
	const float m_fAdjustTurnSpeed; //向きを変える速度の補正度
};

//距離を変える状態
class CCameraLengthState
{
public:
	CCameraLengthState();
	virtual ~CCameraLengthState();
	virtual void Process(CCamera * pCamera);
};

class CCameraLengthState_Gradually : public CCameraLengthState
{
public:
	CCameraLengthState_Gradually(float fLength,float fAdjustAddSpeed,int nChengeFrame);
	~CCameraLengthState_Gradually();
	void Process(CCamera* pCamera) override;
private:
	//*変数
	float m_fAimLength;//距離
	float m_fAdjustAddSpeed;//距離を増やす速度
	int m_nChengeLengthFrame;//カメラの距離を増やしている状態にするフレーム数
	bool m_bNowAddLength;    //カメラの距離を増やしているかどうか
	bool m_bStartReturnLength;//カメラの距離を戻す処理を開始するかどうか
	int m_nChengeFrameCnt;   //カメラの距離を増やしたままにするフレーム数をカウントする

	//*関数
	void AddLengthProcess(CCamera * pCamera);//距離を増やす処理
	void MaintainLengthFrameCntProcess();    //距離を維持するフレーム数をカウントする
	void ReturnLengthProcess(CCamera* pCamera);//距離を元に戻す処理
};
#endif

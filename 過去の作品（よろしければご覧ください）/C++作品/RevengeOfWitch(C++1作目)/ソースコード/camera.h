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
		CAMERATYPE_BIRD = 0,  //バードビュー
		CAMERATYPE_BESIDE,    //ビサイドビュー
		CAMERATYPE_SIDE,      //サイドビュー
		CAMERATYPE_BOSSBATTLE,//ボスバトル
		CAMERATYPE_BOSSDEFEAT,//ボスを倒したときの演出用カメラ
		CAMERATYPE_TURNINGXZ, //XZ旋回モード
		CAMERATYPE_NONE,      //カメラタイプ：なし
		CAMERATYPE_MAX
	}CAMERATYPE;
	//=================================================================================================

	CCamera();            //コンストラクタ
	~CCamera();           //デストラクタ
	HRESULT Init();       //初期化処理
	void Uninit();        //終了処理
	void Update();        //更新処理
	void SetCamera();     //カメラの設定

	void SetPosR(D3DXVECTOR3 PosR) { m_PosR = PosR; }  //注視点を設定する
	D3DXVECTOR3& GetRot() { return m_Rot; }             //向きを取得する
	void SetShake(int nShakeFrame, float fShakePower); //カメラの揺れを設定する
	CAMERATYPE GetCameraType() { return m_CameraType; }//カメラタイプを取得する
	void SetCameraType(CAMERATYPE type) { m_CameraType = type; m_ModeTime = 0; }//カメラタイプを設定する
	D3DXVECTOR3& GetPosV() { return m_PosV; }      //視点を取得
	D3DXVECTOR3& GetPosR() { return m_PosR; }      //注視点を取得
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
private:
	static const float m_BESIDECAMERALENGTH;      //ビサイドモードのカメラの距離
	D3DXVECTOR3 m_PosV;                           //視点
	D3DXVECTOR3 m_PosR;                           //注視点
	D3DXVECTOR3 m_VecU;                           //上方向ベクトル
	D3DXVECTOR3 m_Rot;                            //向き
	D3DXMATRIX m_mtxProjection;                   //プロジェクション行列
	D3DXMATRIX m_mtxView;                         //ビューマトリックス
	CAMERATYPE m_CameraType;                      //カメラモードの種類
	D3DXVECTOR3 m_DifferenceLength;               //差分
	D3DXVECTOR3 m_ZoomSpeed;                      //ズームする速さ
	
	D3DXVECTOR3 m_SupportPos;                     //参考位置
	float m_fLength;                              //距離
	float m_fAddLength;                           //追加距離
	float m_fTurningRotSpeed;                     //旋回速度
	float m_fTurningSpeedY;                       //旋回中のY方向の移動量

	int m_nShakeFrame;                            //カメラを揺らすフレーム数
	int m_ModeTime;                               //カメラモードの時間
	float m_fShakePower;                          //カメラを揺らす力

	void BossDefeatCameraProcess();               //ボスを倒したときのカメラの処理を行う
	void TurningCameraProcess();                  //旋回のカメラの処理を行う
	void NormalCameraSetPosR();                   //普通のカメラの注視点を設定し続ける
};
#endif

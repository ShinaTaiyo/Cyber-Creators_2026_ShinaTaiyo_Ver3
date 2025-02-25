#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"

//カメラ構造体
typedef struct
{
	D3DXVECTOR3 PosV;//支点
	D3DXVECTOR3 targetPosV;//目的のカメラの視点
	D3DXVECTOR3 PosR;//注視点
	D3DXVECTOR3 targetPosR;//目的のカメラの注視点
	D3DXVECTOR3 VecU;//上方向ベクトル
	D3DXVECTOR3 rot;//向き
	D3DXVECTOR3 move;//カメラの動き;
	float fXVaim;//X方向ベクトル
	float fZVaim;//Z方向ベクトル
	float fLength;//距離
	int nMode;//カメラモードを設定する
	D3DXMATRIX mtxProjection;//プロジェクションマトリックス
	D3DXMATRIX mtxView;//ビューマトリックス
}Camera;

typedef enum
{
	CAMERAMODE_REVOLUTION = 0,//公転モード
	CAMERAMODE_ROTATION,//自転モード
	CAMERAMODE_MAX//カメラモード最大数
}CAMERAMODE;

//プロトタイプ宣言
void InitCamera(void);
void UninitCamera(void);
void UpdateCamera(void);
void SetCamera(void);
Camera* GetCamera(void);
void SetPosCamera(D3DXVECTOR3 PosR, D3DXVECTOR3 PosV);

#endif

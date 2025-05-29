//========================================================
//
//１１月７日：Xファイルを読み込む[Motion.h]
//Author:ShinaTaiyo
//
//=========================================================
#ifndef _MOTION_H_
#define _MOTION_H_

#include "main.h"

#define NUM_MOTION (100)//モデルの種類
#define MAX_MOTION (5)//設定するモーションの最大数
#define MAX_KEY (30)//キーの最大数
#define MAX_KEYSET (50)//1つのモーションの最大数
#define MAX_MOTIONMODEL (20)//モーションを行うモデルの最大数
#define MOTIONSET "data\\TEXTFILE\\RealStarWarrior2.txt"

//typedef struct
//{
//	D3DXVECTOR3 pos;//位置
//	D3DXVECTOR3 rot;//向き
//	D3DXVECTOR3 vtxMinMotion;//それぞれの座標のモデルの位置の最小値
//	D3DXVECTOR3 vtxMaxMotion;//それぞれの座標のモデルの位置の最大値
//	int nNumVtx;//頂点数
//	DWORD sizeFVF;//頂点フォーマットのサイズ
//	BYTE* pVtxBuff;//頂点バッファへのポインタ
//	int nType;//種類
//	int nIdxShadow;//影のインデックス
//	bool bUse;//使用しているかどうか
//}Motion;

typedef struct
{
	D3DXVECTOR3 Pos;//位置
	D3DXVECTOR3 Rot;//向き
	D3DXVECTOR3 TargetPos;//目的の位置
	D3DXVECTOR3 TargetRot;//目的の向きまでパーツを回すための計算用の向き
	D3DXVECTOR3 SaveRot;//モーションごとに設定した初期値を保存する
}Key;

typedef struct
{
	int nFrame;//次のポーズに以降するまでのスピード
	Key aKey[MAX_KEY];//キーの最大数
}keySet;

typedef struct
{
	bool bUse;//モーションを使用するかどうか
	bool bLoop;//ループさせるかどうか
	int nNumKey;//キーの数
	keySet aKeySet[MAX_KEYSET];//１つのモーションに対するキーセットの最大数
}MotionSet;
//
//typedef enum
//{
//	MotionTYPE_BILL = 0,//ビル
//	MotionTYPE_BRIDDGE,//橋
//	MotionTYPE_MAX,//最大数
//}MOELTYPE;

//プロトタイプ宣言
void InitMotion(void);
//void UninitMotion(void);
void UpdateMotion(void);
//void DrawMotion(void);
void SetMotion(void);
//int GetNumMotion(void);//読み込むモデルの最大数を取得する
#endif



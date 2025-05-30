//================================================================================================================================================================================================================================
//
// 攻撃モデルの処理に関するヘッダーファイル
// Author : Atsumu Kuboichi
//
//================================================================================================================================================================================================================================
#ifndef _ATTACKMODEL_H_	//このマクロ定義がされなかったら
#define _ATTACKMODEL_H_	//2重インクルード防止のマクロ定義

#include "main.h"

//================================================================================================================================================================================================================================
// マクロ定義
//================================================================================================================================================================================================================================
#define MAX_ATTACKMODEL		(1024)	//モデルの最大数
#define NUM_ATTACKMODEL		(10)		//モデルの種類
#define ATTACKMODEL_TEX		(100)	//テクスチャの最大数
#define ATTACKMODEL_MAT		(100)	//マテリアルの最大数
#define MAX_ATTACKMODELMAT	(100)	//最大マテリアル数

//================================================================================================================================================================================================================================
// 構造体
//================================================================================================================================================================================================================================

typedef enum
{
	ATTACKMODEL00_BOOMERANG = 0,//ブーメラン攻撃（P）
	ATTACKMODEL01_WAVE,         //ウェーブ攻撃 (P)
	ATTACKMODEL02_SPHERE,       //玉(E)
	ATTACKMODEL03_BOMB,         //ボム(E)
	ATTACKMODEL04_SHOCKWAVE,    //衝撃波(E)
	ATTACKMODEL05_TORNADO,      //トルネード(E)
	ATTACKMODEL06_THUNDER,      //サンダー(E)
	ATTACKMODEL07_METEO,        //メテオ(E)
	ATTACKMODEL08_SOUP,         //スープ(E)
	ATTACKMODEL09_LASER,        //レーザー(E)
	ATTACKMODEL_MAX
}ATTACKMODEL;

typedef enum
{
	ATTACKMODELBEHAVIOR00_NORMAL = 0,//攻撃の挙動が普通
	ATTACKMODELBEHAVIOR01_ANOTHER,   //攻撃の挙動がbetu 
	ATTACKMODELBEHAVIOR_MAX
}ATTACKMODELBEHAVIOR;

typedef struct
{
	LPD3DXMESH pMesh;			                   //メッシュの(頂点情報)へのポインタ
	LPD3DXBUFFER pBuffMat;		                   //マテリアルのポインタ
	DWORD dwNumMat;				                   //マテリアルの数
	LPDIRECT3DTEXTURE9 pTexture[MAX_ATTACKMODELMAT];//テクスチャへのポインタ
}AttackModelInfo;

typedef struct
{

	//=====================
	//全体
	//====================
	D3DXMATRIX mtxWorld;		                  //ワールドマトリックス
	LPDIRECT3DTEXTURE9 pTexture;                  //テクスチャへのポインタ
	D3DXVECTOR3 vtxMin;			                  //最小値
	D3DXVECTOR3 vtxMax;			                  //最大値
								                  
	D3DXVECTOR3 OriginallyVtxMax;                 //元々の最大値
	D3DXVECTOR3 OriginallyVtxMin;                 //元々の最小値

	D3DCOLORVALUE DiffUse[NUM_ATTACKMODEL][ATTACKMODEL_MAT];//モデルの色をいじることができる変数(pMatにも同じ型があるので、それに代入して色を変えられるようにする。)

	D3DXVECTOR3 pos;			                  //位置
	D3DXVECTOR3 PosOld;                           //1f前の位置
	D3DXVECTOR3 SavePos;                          //召喚した位置の記憶
	D3DXVECTOR3 move;			                  //移動量
	D3DXVECTOR3 rot;			                  //向き
	D3DXVECTOR3 Vrot;                             //ベクトルを考慮した向き
	D3DXVECTOR3 Scale;                            //拡大率
	D3DXVECTOR3 PosLength;                        //ある物体との距離を求める時に使う
	D3DXVECTOR3 Size;                             //物体の大きさ
	int nType;					                  //種類
	int nBehaviorType;                            //挙動の種類
	int nLife;                                    //体力
	int nMaxLife;                                 //最大体力
	bool bUse;					                  //使用しているかどうか
	float fRot;                                   //動く向きを決める
	float fExpansion;                             //モデルを拡大させる速さ
	int nNumLandingModel;                         //どのモデルの上に乗っているか
	int nIdxShadow;                               //影のインデックス

	float fVaim;                                  //ベクトル
	float fSupportVaim[4];                        //支点となるベクトル
	float fVXaim;                                 //X方向のベクトル
	float fVZaim;                                 //Z方向のベクトル
	float fSpeed;                                 //スピード
	bool bIsLanding;                              //地面にいるかどうか
	bool bCurveLaser;                             //レーザーを回す向きを決める
	//===========================================================================================

	//=======================
	//ブーメラン固有処理
	//=======================
	bool bBack;                 //戻ってくるようにするフラグ
	D3DXVECTOR3 SupportPos;     //弧を描くようにするための支点となる位置
	D3DXVECTOR3 SupportPosPlayer;//プレイヤーの位置から支点を決める。
	D3DXVECTOR3 TargetPos;      //目的の位置（減衰処理)
	float fSaveRot;             //プレイヤーがブーメランを投げた時の向きを記録する
	float fBoomerangRange;      //ブーメランを召喚した時点での射程を保存する
	float fTotalRotMove;            //プレイヤーがブーメランを投げてから回転移動をした合計量
	bool bTurnFlag;             //ブーメランをターンさせるフラグ
	int nTurnCnt;               //ブーメランがターンを始めたらカウントを開始
	//===========================================================================================
	int nCntTime;               //攻撃してからの時間をカウントする

	//==============================
	//スフィア変数
	//==============================
	bool bSphereShot;//ライフが半分になると、スフィアを自機狙いで動かす

	//==============================
	//三次元自機狙い変数
	//==============================
	float fVXaim3D;
	float fVYaim3D;
	float fVZaim3D;
	float fVLaim3D;

	//==========================================================
}AttackModel;

//================================================================================================================================================================================================================================
// 各種類のXファイル
//================================================================================================================================================================================================================================
static const char* ATTACKMODEL_FILENAME[NUM_ATTACKMODEL] =
{
	"data\\PLAYER\\boomerang\\boomerang000.x",				 //[0]ブーメラン攻撃
	"data\\MODEL\\Attack\\Wave_000.x",                       //[1]スピーカー攻撃
	"data\\MODEL\\Attack\\Sphere_000.x",                     //[2]スフィア攻撃
	"data\\MODEL\\Attack\\bomb_000.x",                       //[3]ボム攻撃
	"data\\MODEL\\Attack\\ShockWave_000.x",                  //[4]衝撃波
	"data\\MODEL\\Attack\\Tornado_000.x",                    //[5]トルネード
	"data\\MODEL\\Attack\\thunder000.x",                     //[6]サンダー
	"data\\MODEL\\Attack\\Meteorite_000.x",                  //[7]メテオ
	"data\\MODEL\\Attack\\soup000.x",                        //[8]スープ
	"data\\MODEL\\Attack\\laser000.x",                       //[9]レーザー
};

//================================================================================================================================================================================================================================
// プロトタイプ宣言
//================================================================================================================================================================================================================================
void InitAttackModel(void);													//初期化処理
void UninitAttackModel(void);												//終了処理
void UpdateAttackModel(void);												//更新処理
void DrawAttackModel(void);													//描画処理
void SetAttackModel(int nType,int nLife,D3DXVECTOR3 pos,D3DXVECTOR3 move, D3DXVECTOR3 rot,D3DXVECTOR3 Scale,
	                float fRot,float fSaveRot,float fExpansion,D3DXVECTOR3 SupportPos,int nBehaviorType);//設定処理
AttackModel* GetAttackModel(void);											//情報の取得

float fRotCollisionAModel(float x0,float x1,float x2,float z0,float z1,float z2);//線分との当たり判定を取る

#endif

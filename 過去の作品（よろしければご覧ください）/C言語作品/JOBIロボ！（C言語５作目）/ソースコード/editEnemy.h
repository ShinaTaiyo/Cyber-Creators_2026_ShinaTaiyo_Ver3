//========================================================
//
//１１月７日：Xファイルを読み込む[EDITENEMY.h]
//Author:ShinaTaiyo
//
//=========================================================
#ifndef _EDITENEMY_H_
#define _EDITENEMY_H_

#include "main.h"

#define NUM_EDITENEMY (128)//モデルの種類
#define MAX_EDITENEMY (1024)//読み込むモデルの最大数
#define MAX_EDITEMAT (100)//マテリアルの最大数

typedef enum
{
	ENEMYEDITORLAYOUT00_WORLD = 0,//世界で配置
	ENEMYEDITORLAYOUT01_PLAYERAROUND,//プレイヤーの周りで配置
	ENEMYEDITORLAYOUT_MAX
}ENEMYEDITORLAYOUT;

typedef struct
{
	LPD3DXMESH pMesh;//メッシュ（頂点情報）へのポインタ（モデルの種類数分）
	LPD3DXBUFFER pBuffMat;//マテリアルへのポインタ
	DWORD dwNumMat;//マテリアルの数
}EnemyEditorModelInfo;

typedef struct
{
	D3DXVECTOR3 pos;//位置
	D3DXVECTOR3 rot;//向き
	D3DXVECTOR3 targetrot;//目的の向き

	D3DXMATRIX mtxWorld;//マトリックスワールド
	D3DCOLORVALUE Diffuse[MAX_EDITEMAT];//色合い

	int nType;//選んでいる種類
	int nLayoutType;//敵の配置方法の種類
	int nSummonTime;//選んでいる敵を召喚する時間
	int nIdxShadow;//影のインデックス
	int nIdxRaderDisp;//レーダー表示のインデックス
	int nNumSetEnemy;//何番目の敵をセットしているか
	bool bUse;//使用しているかどうか

}EnemyEditor;

typedef struct
{
	D3DXVECTOR3 pos;//位置
	D3DXVECTOR3 rot;//向き
	D3DXMATRIX mtxWorld;//マトリックスワールド
	D3DCOLORVALUE Diffuse[MAX_EDITEMAT];//色合い

	int nType;//選んでいる種類
	int nLayoutType;//敵の配置方法の種類
	int nSummonTime;//選んでいる敵を召喚する時間
	int nIdxShadow;//影のインデックス
	int nNumEnemy;//何番目の敵をセットしているか
	bool bUse;//使用しているかどうか

}EditEnemy;//設定した敵の情報


typedef enum
{
	EDITENEMYTYPE_BILL = 0,//ビル
	EDITENEMYTYPE_BRIDDGE,//橋
	EDITENEMYTYPE_MAX,//最大数
}EDITENEMYTYPE;

//プロトタイプ宣言

//============================
//敵エディタ
//============================
void InitEnemyEditor(void);
void UninitEnemyEditor(void);
void UpdateEnemyEditor(void);
void DrawEnemyEditor(void);
//==========================================



void SetEditEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nTime,int nLayoutType);

int GetEditTime(void);//敵の召喚時間を決める変数を取得する。

EnemyEditor* GetEnemyEditor(void);//構造体の情報を取得
#endif


#pragma once

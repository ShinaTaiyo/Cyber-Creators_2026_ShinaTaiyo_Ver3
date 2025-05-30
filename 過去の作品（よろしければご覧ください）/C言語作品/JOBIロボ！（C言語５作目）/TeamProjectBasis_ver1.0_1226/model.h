//================================================================================================================================================================================================================================
//
// モデルの処理に関するヘッダーファイル
// Author : Atsumu Kuboichi
//
//================================================================================================================================================================================================================================
#ifndef _MODEL_H_	//このマクロ定義がされなかったら
#define _MODEL_H_	//2重インクルード防止のマクロ定義

#include "main.h"

//================================================================================================================================================================================================================================
// マクロ定義
//================================================================================================================================================================================================================================
#define MAX_MODEL			(1000)	//モデルの最大数
#define NUM_MODEL			(56)	//モデルの種類
#define MODEL_TEX			(100)	//テクスチャの最大数
#define CREATE_POSX_TYPE1	(150.0f)//机の脚のX座標設定用
#define CREATE_POSX_TYPE3	(80.0f)	//椅子(生徒用)の脚のX座標設定用
#define CREATE_POSX_TYPE5	(400.0f)//教卓の脚のX座標設定用
#define CREATE_POSX_TYPE7	(70.0f)	//椅子(生徒用)のX座標設定用
#define MAX_MODELMAT		(100)	//最大マテリアル数
#define OUTLET_SIZE			(100.0f)//コンセントからの範囲(半径)

//================================================================================================================================================================================================================================
// 構造体
//================================================================================================================================================================================================================================
typedef struct
{
	LPD3DXMESH pMesh;			//メッシュの(頂点情報)へのポインタ
	LPD3DXBUFFER pBuffMat;		//マテリアルのポインタ
	DWORD dwNumMat;				//マテリアルの数
	D3DXMATRIX mtxWorld;		//ワールドマトリックス
	LPDIRECT3DTEXTURE9 pTexture;//テクスチャへのポインタ
	D3DXVECTOR3 vtxMin;			//最小値
	D3DXVECTOR3 vtxMax;			//最大値
	D3DXVECTOR3 Size;           //サイズ
	D3DXVECTOR3 pos;			//位置
	D3DXVECTOR3 posOld;         //1f前の位置
	D3DXVECTOR3 SavePos;        //初期位置を保存する
	D3DXVECTOR3 move;			//移動量
	D3DXVECTOR3 rot;			//向き
	int nType;					//種類
	int nUse;					//フィールドかちびロボハウスかの判定
	int nCntTime;               //様々なタイミングで時間を測る
	bool bUse;					//使用しているかどうか
	bool bCollision;			//当たり判定の有無
	int nIdxRaderDisplay;       //レーダー表示のインデックス

	//=================================
	//リフト限定処理
	//=================================
	bool bUpDownLift;//フラグの切り替えでリフトを上下できるようにする
	bool bStopLift;  //リフトを止める
}Model;

//================================================================================================================================================================================================================================
// 各種類のXファイル
//================================================================================================================================================================================================================================
static const char* MODEL_FILENAME[NUM_MODEL] =
{
	//置物(ゲーム)
	"data\\MODEL\\OBJECT\\desk000\\deskup000.x",						//机(上)[0]
	"data\\MODEL\\OBJECT\\desk000\\deskdown000.x",						//机(下)[1]
	"data\\MODEL\\OBJECT\\Chair_000\\ChairUP_000.x",					//椅子(上)[2]
	"data\\MODEL\\OBJECT\\Chair_000\\ChairDown_000.x",					//椅子(下)[3]
	"data\\MODEL\\OBJECT\\Lectern_000\\LecternUP_000.x",				//教卓(上)[4]
	"data\\MODEL\\OBJECT\\Lectern_000\\LecternDown_000.x",				//教卓(下)[5]
	"data\\MODEL\\OBJECT\\Chair_001\\ChairUP_001.x",					//椅子(先生用　上)[6]
	"data\\MODEL\\OBJECT\\Chair_001\\ChairLeftD_001.x",					//椅子(先生用　左下)[7]
	"data\\MODEL\\OBJECT\\Chair_001\\ChairRightD_001.x",				//椅子(先生用　右下)[8]
	"data\\MODEL\\OBJECT\\BookRack_000\\BookRack_000.x",				//ラック[9]
	"data\\MODEL\\OBJECT\\locker000\\locker000.x",						//ロッカー[10]
	"data\\MODEL\\OBJECT\\Pillar_000\\Pillar_000.x",					//柱(角)[11]
	"data\\MODEL\\OBJECT\\Pillar_001\\Pillar_001.x",					//柱(中央)[12]
	"data\\MODEL\\OBJECT\\TV_000\\TV_000.x",							//テレビ[13]
	"data\\MODEL\\OBJECT\\whiteboard000\\whiteboard000.x",				//ホワイトボード[14]
	"data\\MODEL\\OBJECT\\ChibiRoboHouse\\all100.x",					//ちびロボハウス[15]
	"data\\MODEL\\OBJECT\\Chirikami_000\\Chirikami_000.x",				//ゴミ[16]
	"data\\MODEL\\OBJECT\\Door_000\\Door_000.x",						//ドア[17]
	"data\\MODEL\\OBJECT\\lift000\\lift000.x",							//リフト[18]
	"data\\MODEL\\OBJECT\\Outlet_000\\Outlet_000.x",					//コンセント(床)[19]
	"data\\MODEL\\OBJECT\\Pillar_000\\Pillar_000.x",					//柱(角)[20]
	"data\\MODEL\\OBJECT\\Pillar_001\\Pillar_001.x",					//柱(中央)[21]
	"data\\MODEL\\OBJECT\\Stove_000\\Stove_000.x",						//ストーブ[22]
	"data\\MODEL\\OBJECT\\machine\\machine000.x",						//機械[23]
	"data\\MODEL\\OBJECT\\PC000\\PC000.x",								//PC[24]
	"data\\MODEL\\OBJECT\\Book_000\\Book_000.x",						//本(赤)[25]
	"data\\MODEL\\OBJECT\\Book_001\\Book_001.x",						//本(青)[26]
	"data\\MODEL\\OBJECT\\Book_002\\Book_002.x",						//本(緑)[27]
	"data\\MODEL\\OBJECT\\Book_003\\Book_003.x",						//本(黄)[28]
	"data\\MODEL\\OBJECT\\FlowerVase_000\\FlowerVase_000.x",			//花瓶[29]
	"data\\MODEL\\OBJECT\\RuckSack_000\\RuckSack_000.x",				//リュック(黄)[30]
	"data\\MODEL\\OBJECT\\RuckSack_001\\RuckSack_001.x",				//リュック(黒)[31]
	"data\\MODEL\\OBJECT\\RuckSack_002\\RuckSack_002.x",				//リュック(赤)[32]
	"data\\MODEL\\OBJECT\\RuckSack_003\\RuckSack_003.x",				//リュック(青)[33]
	"data\\MODEL\\OBJECT\\RuckSack_004\\RuckSack_004.x",				//リュック(緑)[34]
	"data\\MODEL\\OBJECT\\AntisepticSolution_000\\AntisepticSolution.x",//消毒[35]
	"data\\MODEL\\OBJECT\\NintendoSwitch_001\\NintendoSwitch_001.x",	//Switch[36]
	"data\\MODEL\\OBJECT\\TV_001\\TV_001.x",							//テレビ[37]
	"data\\MODEL\\OBJECT\\clock000\\clock000.x",						//時計[38]
	"data\\MODEL\\OBJECT\\Certificate\\certificate.x",					//賞状[39]
	"data\\MODEL\\OBJECT\\JOBI_Logo_000\\JOBI_Logo_000.x",				//Jobiロゴ[40]
	"data\\MODEL\\OBJECT\\Outlet_001\\Outlet_001.x",					//コンセント(壁)[41]
	"data\\MODEL\\OBJECT\\shelf000\\shelf000.x",						//棚[42]
	"data\\MODEL\\OBJECT\\whiteboard000\\whiteboard000.x",				//ホワイトボード[43]
	"data\\MODEL\\OBJECT\\Window_002\\Window_002.x",					//窓[44]
	"data\\MODEL\\OBJECT\\Window_002\\Window_001.x",					//窓(鍵無し)[45]
	"data\\MODEL\\OBJECT\\AirController_000\\AirController_000.x",		//エアコン[46]
	"data\\MODEL\\OBJECT\\Airvent000\\Airvent000.x",					//換気口[47]
	"data\\MODEL\\OBJECT\\light000\\light000.x",						//電灯[48]
	"data\\MODEL\\OBJECT\\plane000\\plane000.x",						//飛行機１[49]
	"data\\MODEL\\OBJECT\\plane001\\plane001.x",						//飛行機２[50]
	"data\\MODEL\\OBJECT\\plane002\\plane002.x",						//飛行機３[51]
	"data\\MODEL\\OBJECT\\Seagull_000\\Seagull_000.x",					//カモメ１[52]
	"data\\MODEL\\OBJECT\\Seagull_001\\Seagull_001.x",					//カモメ２[53]
	"data\\MODEL\\OBJECT\\Seagull_002\\Seagull_002.x",					//カモメ３[54]
	"data\\MODEL\\OBJECT\\board_000\\board.x",							//板[55]

	//種類:56
};

//================================================================================================================================================================================================================================
// プロトタイプ宣言
//================================================================================================================================================================================================================================
void InitModel(void);															//初期化処理
void UninitModel(void);															//終了処理
void UpdateModel(void);															//更新処理
void DrawModel(void);															//描画処理
void SetModel(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nUse);			//設定処理
void LoadModeltxt(void);														//モデル情報の読み込み処理(txt)
void LoadModelbin(void);														//モデル情報の読み込み処理(bin)
Model* GetModel(void);															//情報の取得
bool bCollisionModel(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 Size,int &nNumLandingModel);//当たり判定処理

#endif

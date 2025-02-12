//====================================================
//
//６月２５日：ワープマーカーを実装[marker.h]
//Author:ShinaTaiyo
//
//====================================================

#ifndef _MARKER_H_  
#define _MARKER_H_

//======================
//インクルード
//======================
#include "main.h"
#include "objectX.h"
#include "number3d.h"
//==========================================

//==========================================

//==========================================
//マーカークラス
//==========================================
class CMarker : public CObjectX
{
public:
	//===========================
	//マーカーの種類
	//===========================
	typedef enum
	{
		MARKERTYPE00_WARP = 0,//ワープマーカー
		MARKERTYPE_MAX
	}MARKERTYPE;
	//======================================================================
	CMarker();                   //コンストラクタ
	~CMarker() override;         //デストラクタ
	HRESULT Init() override;    //初期化処理
	void Uninit() override;     //終了処理
	void Update() override;     //更新処理
	void Draw() override;       //描画処理
	void SetDeath() override;   //死亡フラグを設定
	static int GetNumFile() { return m_nNumFile; }//ファイルの数を取得する
	static CMarker* Create(MARKERTYPE type,D3DXVECTOR3 pos,D3DXVECTOR3 Scale,D3DXVECTOR3 WarpPos,int nMapNum);//敵を生成
	D3DXVECTOR3 GetWarpPos() { return m_WarpPos; }//ワープする位置を取得する
	int GetWarpMap() { return m_nWarpMapNum; }    //ワープ先のマップを取得する
	void DeleteNum();       //番号を消す
private:
	static const int m_nWARPMAPDIVISION = 2;
	void Collision();       //当たり判定を取る
	MARKERTYPE m_type;      //敵の種類
	D3DXVECTOR3 m_Aim;      //ベクトル
	D3DXVECTOR3 m_WarpPos;  //ワープする位置を決める
	CNumber3D* m_pWarpMapDispNum[m_nWARPMAPDIVISION];//ワープ先マップの番号を表示
	int m_nWarpMapNum;      //ワープするマップの番号を格納
	float m_fTotalAim;      //総合したベクトル
	int m_nCntTime;         //出現してからのフレーム数を測る
	static const char* m_MARKER_FILENAME[MARKERTYPE_MAX];   //マーカーのファイルネーム
	static const int m_nMAXENEMY01BULLET;
	static int m_nNumFile;                                  //読み込んだファイルの数
};

#endif
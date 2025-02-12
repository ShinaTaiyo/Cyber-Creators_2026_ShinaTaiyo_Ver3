//=====================================================================================
//
//９月１０日：タイトルを作る[ui.h]
//Author:ShinaTaiyo
//
//=====================================================================================1

#ifndef _UI_H_  
#define _UI_H_

//======================
//インクルード
//======================
#include "main.h"
#include "object2d.h"
//==========================================

//======================
//前方宣言
//======================
class CObject2D;

//==========================================
//UIクラス
//==========================================
class CUi : public CObject2D
{
public:
	typedef enum
	{
		UITYPE_PLAYER2DILUST = 0,     //プレイヤーイラスト
		UITYPE_SLIMEILUST,            //スライムイラスト
		UITYPE_MAGICCIRCLE,           //魔法陣
		UITYPE_TITLELOGO,             //タイトルロゴ
		UITYPE_PRESSENTER_LOGO,       //PressEnterロゴ
		UITYPE_PRESSENTERORSTART_LOGO,//PressEnterOrStartロゴ
		UITYPE_CHARACTERGAUGEFRAME,   //キャラクターのゲージフレーム
		UITYPE_FACENORMAL,            //普通の顔
		UITYPE_FACEDAMAGE,            //ダメージを受けたときの顔
		UITYPE_FACEGLAD,              //嬉しい顔
		UITYPE_BOSSBATTLE_TEXT,       //ボスバトル文字表示
		UITYPE_CUTIN_KINGSLIME,       //キングスライムのカットイン
		UITYPE_KINGSLIME_TEXT,        //キングスライムの文字
		UITYPE_CUTIN_DEATHENAGA,      //デスエナガのカットイン
		UITYPE_DEATHENAGA_TEXT,       //デスエナガの文字
		UITYPE_CUTIN_MRSTRONG,        //Mrストロングのカットイン
		UITYPE_MRSTRONG_TEXT,         //Mrストロングの文字
		UITYPE_CUTIN_RILEY,           //ライリーのカットイン
		UITYPE_RILEY_TEXT,            //ライリーの文字
		UITYPE_CUTIN_DRAIYA,          //ドライヤのカットイン
		UITYPE_DRAIYA_TEXT,           //ドライヤの文字
		UITYPE_CUTIN_GABRIEL,         //ガブリエリのカットイン
		UITYPE_GABRIEL_TEXT,          //ガブリエルの文字
		UITYPE_EASY_TEXT,             //難易度EASYの文字
		UITYPE_NORMAL_TEXT,           //難易度NORMALの文字
		UITYPE_CHOOSEDIFFICULTY_TEXT, //難易度選択の文字
		UITYPE_LONGPUSH_TEXT,         //長押しの文字
		UITYPE_BOSSRUSH_TEXT,         //ボスラッシュの文字
		UITYPE_MAX
	}UITYPE;

	typedef enum
	{
		UIMOVETYPE_NORMAL = 0,//普通の動き
		UIMOVETYPE_FLOATING,  //浮遊させる
		UIMOVETYPE_MAX
	}UIMOVETYPE;

	CUi(int nPriority = 6);                      //コンストラクタ
	virtual ~CUi();             //デストラクタ
	HRESULT Init() override;    //初期化処理
	void Uninit() override;     //終了処理
	void Update() override;     //更新処理
	void Draw() override;       //描画処理
	void SetDeath() override;   //死亡フラグを設定
	static CUi * Create(UITYPE type, CObject2D::POLYGONTYPE PolygonType,float fWidth, float fHeight,D3DXVECTOR3 Pos,D3DXVECTOR3 Move,D3DXVECTOR3 Rot,D3DXCOLOR col);//背景の生成
	void SetUiType(UITYPE type);
	void SetUseUiEffect(bool bUse, int nSetEffectLife, D3DXCOLOR col) { m_bUseUiEffect = bUse; m_nSetUiEffectLife = nSetEffectLife; m_SetUiEffectColor = col; }
protected:
	UITYPE GetType() { return m_Type; }//UIの種類を取得
	static const char* m_apUI_FILENAME[UITYPE_MAX];//UIのテクスチャファイル名
private:
	UITYPE m_Type;                 //背景の種類
	UIMOVETYPE m_MoveType;         //UIの動き方の種類
	bool m_bUseUiEffect;           //UIでエフェクトを出すかどうか

	D3DXCOLOR m_SetUiEffectColor;  //UIで出すエフェクトの色合い設定用
	int m_nSetUiEffectLife;        //UIのエフェクトの体力設定用
};
//=======================================================================================

//==========================================
//UIエフェクトクラス
//==========================================
class CUiEffect : public CUi
{
public:
	CUiEffect();                //コンストラクタ
	virtual ~CUiEffect();       //デストラクタ
	HRESULT Init() override;    //初期化処理
	void Uninit() override;     //終了処理
	void Update() override;     //更新処理
	void Draw() override;       //描画処理
	void SetDeath() override;   //死亡フラグを設定
	static CUiEffect* Create(UITYPE type, CObject2D::POLYGONTYPE PolygonType, float fWidth, float fHeight,int nLife,D3DXVECTOR3 Pos, D3DXVECTOR3 Move, D3DXVECTOR3 Rot, D3DXCOLOR col);//背景の生成
private:
};
//=======================================================================================

#endif
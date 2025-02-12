//====================================================
//
//７月１０日：C++で基盤づくり[block.h]
//Author:ShinaTaiyo
//
//====================================================

#ifndef _BLOCK_H_  
#define _BLOCK_H_

//======================
//インクルード
//======================
#include "main.h"
#include "object2d.h"
#include "objectX.h"
#include "object.h"
//==========================================

//===================================
//マップのテキストファイルのマクロ
//===================================
#define MAP00_SENTRAL_TXT "data\\TEXTFILE\\Map\\Sentral.txt"

//==========================================

//==========================================
//ブロッククラス
//==========================================
class CBlock : public CObjectX
{
public:
	//===========================
	//ブロックの種類
	//===========================
    typedef enum
	{
		BLOCKTYPE00_NORMAL = 0,    //普通ブロック
		BLOCKTYPE01_WATER,         //水ブロック
		BLOCKTYPE02_GRASS,         //草ブロック
		BLOCKTYPE03_LONGRENGA,     //レンガブロック
		BLOCKTYPE04_RIFT,          //リフトブロック
		BLOCKTYPE05_FIRE,          //ファイアブロック
		BLOCKTYPE06_JUMP,          //ジャンプブロック
		BLOCKTYPE07_CRACK,         //ひびブロック
		BLOCKTYPE08_SUKESUKE,      //スケスケブロック
		BLOCKTYPE09_BLINKINGRED,   //赤点滅ブロック
		BLOCKTYPE10_BLINKINGBRUE,  //青点滅ブロック
		BLOCKTYPE11_ICE,           //アイスブロック
		BLOCKTYPE12_ARROW,         //矢印ブロック
		BLOCKTYPE13_SWAMP,         //沼ブロック
		BLOCKTYPE14_TORNADO,       //トルネードブロック
		BLOCKTYPE15_FALL,          //フォールブロック
		BLOCKTYPE16_SAVE,          //セーブブロック
		BLOCKTYPE17_CHAINBLOCK,    //鎖ブロック
		BLOCKTYPE_MAX
	}BLOCKTYPE;
	//======================================================================

	//===========================
	//マップの種類
	//===========================
	typedef enum
	{
		MAP00_SENTRAL = 0,
		MAP_MAX
	}MAPTYPE;
	//======================================================================

	CBlock();                 //コンストラクタ
	~CBlock() override;       //デストラクタ
	HRESULT Init() override;  //初期化処理
	void Uninit() override;   //終了処理
	void Update() override;   //更新処理
	void Draw() override;     //描画処理
	void SetDeath() override; //死亡フラグを設定
	static CBlock * Create(BLOCKTYPE type, int nLife,D3DXVECTOR3 pos,D3DXVECTOR3 rot,D3DXVECTOR3 scale,CObjectX::ROTTYPE RotType,int nSubType);//ブロックを生成
	void ReleaseBlock();                //ブロックを消す
	BLOCKTYPE GetType();//ブロックの種類を取得する
	void Collision();                                                             //当たり判定を行う
	static void CollisionSquare(CObjectX * pObjX);                                //正方形の当たり判定を行う
	static void LandingCorrection(D3DXVECTOR3& Pos,CObject * pSaveObj,D3DXVECTOR3 VtxMax,D3DXVECTOR3 VtxMin);              //第２、第３引数で指定したブロックの上に乗っている場合、位置を補正する
	static int GetNumFile() { return m_nNumFile; }                                                                 //ファイル数を取得する
	BLOCKTYPE GetBlockType() { return m_type; }                                                                    //ブロックのタイプを取得する
	bool GetbCollision() { return m_bCollision; }                                  //判定可能かどうかを取得する     
	static const int* GetSubTypeNum() { return &m_BLOCKSUBTYPENUM[0]; }            //サブタイプの０番目を取得する
	int GetSubType() { return m_nSubType; }                                        //サブタイプを取得

	float& GetStartRot() { return m_fStartRot; }                                   //回転開始位置を取得
	float& GetRotSpeed() { return m_fRotSpeed; }                                   //回転速度を取得
	float& GetMoveLength() { return m_fMoveLength; }                               //移動距離を取得
	float& GetSpeed() { return m_fSpeed; }                                         //移動速度を取得
	int& GetBlinkSpeed() { return m_BlinkingBlock.nBlinkingSpeed; }                //点滅速度を取得
	float& GetAddSpeed() { return m_ArrowBlock.fAddSpeed; }                        //加速度を取得
	float& GetJumpPower() { return m_JumpBlock.fJumpPower; }                       //ジャンプ力を取得

	void SetBlockInfo(float fStartRot, float fRotSpeed, float fMoveLength, float fSpeed, int nBlinkSpeed, float fAddSpeed, float fJumpPower);//ブロックの情報設定
private:
	static const int m_nMAX_FALLBLOCKRESPAWNCNT = 150;

	void RidingTopCollision(bool bJustTurn);                                       //ブロックの上に乗っているかどうかを判定する
	void LiftBlockPattern();                                                       //リフトブロックのパターン
	void LiftBlockRoundMove();                                     //リフトブロックの往復の動き
	void BlinkingBlockPattern();                                                   //点滅ブロックのパターン
	void ArrowBlockPattern();                                                      //矢印ブロックのパターン
	void FallBlockProcess();                                                       //落ちるブロックの処理
	void DispInfo() override;                                                      //情報表示処理
	//テクスチャファイルの読み込み
	static const char* m_BLOCK_FILENAME[BLOCKTYPE_MAX];                            //ブロックのファイルネーム
	static const int m_BLOCKSUBTYPENUM[BLOCKTYPE_MAX];                                //ブロックのサブタイプ
	static const float m_fBLOCKCORRECTIONCOLLISION;                                 //判定に余裕を持たせる量
	BLOCKTYPE m_type;                                                              //弾の種類
	bool m_bMove;                                                                  //移動するかどうか
	int m_nCntTime;                                                                //生成されてからの時間をカウントする
	bool m_bUse;                                                                   //使用状態
	bool m_bTurn;                                                                  //ターンフラグ
	static int m_nNumFile;                                                         //ファイル数を格納する
	bool m_bCollision;                                                             //当たり判定をするかどうか

	typedef struct
	{
		bool bBlinking;//点滅しているかどうか
		int nBlinkingSpeed;//点滅速度
	}BlinkingBlock;

	typedef struct
	{
		float fAddSpeed;//加速度
	}ArrowBlock;

	typedef struct
	{
		float fJumpPower;//ジャンプの大きさ
	}JumpBlock;

	typedef struct
	{
		bool bFall;//落ちるのを開始するかどうか
		int nRespawnCnt;//リスポーンするまでのカウント
		int nReraiseCnt;//復活したときのカウント
	}FallBlock;

	ArrowBlock m_ArrowBlock;      //矢印ブロックの情報
	BlinkingBlock m_BlinkingBlock;//点滅ブロックの情報
	JumpBlock m_JumpBlock;        //ジャンプブロックの情報
	FallBlock m_FallBlock;        //落ちるブロックの情報

	//======================================
	//ブロック判定分け用
	//======================================
	bool m_bNearBlockX;                         //X方向の近くにブロックがある
	bool m_bNearBlockY;                         //Y報告の近くにブロックがある
	void NearBlockSearch();                     //近くにブロックがあるかどうかをサーチする
	static void NearBlockSurchProcess(D3DXVECTOR3 Pos,D3DXVECTOR3 Size);//近くのブロックを探す処理
	//======================================================================================================

	//======================================
	//ブロックとの押し出し判定用
	//======================================
	void ExtrusionCollisionX(CObjectX* pMyObjX,CBlock * pBlock);//X方向の押し出し判定を行う

	void ExtrusionCollisionY(CObjectX* pMyObjX, CBlock* pBlock);//Y方向の押し出し判定を行う

	int m_nSubType;                                                                //サブタイプの番号
	float m_fRot;                                                                  //向きの補正値
	float m_fRotSpeed;                                                             //回転速度
	float m_fMoveLength;                                                           //ブロックが動く距離
	float m_fStartRot;                                                             //開始向き
	float m_fSpeed;                                                                //速度	
};

#endif

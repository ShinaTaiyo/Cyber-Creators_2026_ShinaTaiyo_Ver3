//====================================================
//
//１０月２９日：敵を作る[enemy.h]
//Author:ShinaTaiyo
//
//====================================================

#ifndef _ENEMY_H_  
#define _ENEMY_H_

//======================
//インクルード
//======================
#include "main.h"
#include "objectX.h"
#include "lockon.h"
#include "aimodel.h"
#include "attack.h"
//==========================================

//===========================================
//前方宣言
//===========================================
class CEnemyMove;
class CAttackEnemy;
class CUi;

//===========================================
//エネミークラス
//===========================================
class CEnemy : public CObjectX
{
public:
	struct MoveAiInfo
	{
		D3DXVECTOR3 Pos;
		D3DXVECTOR3 Rot;
		D3DXVECTOR3 Scale;
	};

	enum class ENEMYTYPE
	{
		SHOTWEAK = 0,//射撃に弱い敵
		DIVEWEAK,    //ダイブに弱い敵
		IDLE,        //何もしない敵
		MAX
	};

	enum class STATE
	{
		NORMAL = 0,
		FRIGHTENDED,
		MAX
	};

	CEnemy(int nPri = 0, bool bUseintPri = false, CObject::TYPE type = CObject::TYPE::ENEMY, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_X);//コンストラクタ
	~CEnemy();                                                              //デストラクタ
	HRESULT Init() override;                                                //初期化処理
	void Uninit() override;                                                 //終了処理
	void Update() override;                                                 //更新処理
	void Draw() override;                                                   //描画処理
	void SetDeath() override;                                               //死亡フラグを設定
	const ENEMYTYPE& GetEnemyType() const { return m_Type; }                //敵の種類を取得する
	//==========================================================
    //エディタ関係
    //==========================================================
    //関数
	void SaveInfoTxt(fstream& WritingFile) override;                                              //テキストファイルに情報を保存するための関数
	//static void LoadInfoTxt(fstream& LoadingFile, list<CObject*>& listSaveManager, string& Buff); //テキストファイルから情報を読み込むための関数   
	void LoadInfoTxt(fstream& LoadingFile, list<CObject*>& listSaveManager, string& Buff, CObject* pObj) override;//テキストファイルから情報を読み込むための関数
	CObject* ManagerSaveObject() override;                                                        //ステージマネージャーに今のオブジェクトを保存する
	void ManagerChooseControlInfo() override;                                                     //ステージマネージャーから操作する
	void SetPhaseNum(int nNum) { m_nPhaseNum = nNum; }                                            //フェーズ番号を設定する
	const int& GetPhaseNum() const { return m_nPhaseNum; }                                        //フェーズ番号を取得する   
 	//=================================================================================================================

	//==========================================================
	//行動
	//==========================================================
	void SetVecMoveAiInfo(vector<CAIModel*> vec);                    //移動AIの動的配列設定
	vector<CAIModel*>& GetVecAiModelInfo() { return m_VecMoveAi; }   //移動AIの動的配列の取得
	virtual void AIMoveProcess();                                    //AI移動処理
	virtual void BattleMoveProcess();                                //バトル移動処理
	void ChengeMove(CEnemyMove* pEnemyMove);                         //移動状態を変える
	//=================================================================================================================

	//==========================================================
	//バトル関係
	//==========================================================
	const int GetCntTime() const { return m_nCntTime; }               //時間を取得
	void SetCntTime(int nTime) { m_nCntTime = nTime; }                //時間を設定（タイミングをずらすなどができる）
	//感知射程														  
	void SetSensingRange(float fRange) { m_fSensingRange = fRange; }  //感知射程を設定
	const float & GetSensingRange() const { return m_fSensingRange; } //感知射程を取得

	//通常移動速度
	void SetNormalSpeed(float fSpeed) { m_fNormalSpeed = fSpeed; }    //通常移動速度を設定
	const float& GetNormalSpeed() const { return m_fNormalSpeed; }    //通常移動速度を取得

	//倒された攻撃タイプを設定する
	void SetDefeatAttack(CAttack::ATTACKTYPE Type) { m_DefeatAttackType = Type; }        //倒された攻撃タイプを設定
	const CAttack::ATTACKTYPE GetDefeatAttackType() const { return m_DefeatAttackType; } //倒された攻撃タイプを取得

	const bool& GetCollisionWall() const { return m_bCollisionWall; }                    //壁に衝突したかどうかを取得

	//パターン
	void SetAction(bool bAction) { m_Pattern.bAction = bAction; }                        //攻撃を発動させるかどうかを設定
	const bool& GetAction() const { return m_Pattern.bAction; }                          //攻撃を発動しているかどうかを取得
	 
	void EndAttackPattern();                                                             //攻撃処理を終了させるときに設定する情報を設定する
	const int GetAttackCoolTime() { return m_nAttackCoolTime; }                          //次の攻撃を発動するまでのクールタイムを取得する
	void SetAttackCoolTime(int nTime) { m_nAttackCoolTime = nTime; }                     //次の攻撃を発動するまでのクールタイムを設定する
	static int GetNumEnemy() { return m_nNumEnemy; }                                     //現在の敵の数を取得する

	void SetState(STATE State) { m_State = State; }                                      //状態異常を設定する
	const STATE& GetState() { return m_State; }                                          //状態異常を取得する

	//ジャンプまでのクールタイム
	void ResetJumpCoolTime() { m_nJumpCoolTime = 0; }                                    //ジャンプ可能になるまでのクールタイムをリセットする
	int GetJumpCoolTime() { return m_nJumpCoolTime; }                                    //ジャンプ可能になるまでのクールタイムを取得する

	void SetPossibleAttack(bool bPossible) { m_bPossibleAttack = bPossible; }            //攻撃可能かどうかを設定する
	//=================================================================================================================
protected:

	struct Pattern
	{//攻撃パターン情報構造体
		int nPattern;
		int nSubPattern;
		int nPatternTime;
		bool bAction;
	};

	//================================================
	//変数宣言
	//================================================
	void SetEnemyType(ENEMYTYPE type) { m_Type = type; }//敵の種類を設定
	//===============================================================================================
	
	//================================================
	//プロトタイプ宣言
	//================================================
	void SetPattern(int nPattern) { m_Pattern.nPattern = nPattern; }                     //攻撃パターン番号を設定
	const int& GetPattern() const { return m_Pattern.nPattern; }                         //攻撃パターン番号を取得

	void SetSubPattern(int nSubPattern) { m_Pattern.nSubPattern = nSubPattern; }         //攻撃のサブパターン番号を設定
	const int& GetSubpattern() const { return m_Pattern.nSubPattern; }                   //攻撃のサブパターン番号を取得

	void SetPatternTime(int nPatternTime) { m_Pattern.nPatternTime = nPatternTime; }     //攻撃パターンの時間を設定
	const int& GetPatternTime() const { return m_Pattern.nPatternTime; }                 //攻撃パターンの時間を取得

	void SetUseCollisionDetection(bool bUse) { m_bCollisoinDetection = bUse; }           //他のオブジェクトに重ならないようにするかどうかを設定

	const bool& GetCollisionDetection() const { return m_bActivateCollisionDetection; }  //他のオブジェクトに重ならないようにしているかどうかを取得

	void RayCollision();                                                                 //レイがオブジェクトに当たっているかどうか

	//移動処理
	void ChasePlayer();                                                                  //プレイヤーを追いかける処理

	virtual void DefeatStaging();                                                        //倒されたときの演出（派生クラスで挙動変更)
	//===============================================================================================
private:
	//================================================
	//静的メンバ
	//================================================
	static int m_nNumEnemy;//敵の総数
	//===============================================================================================

	//================================================
	//変数宣言
	//================================================
	CEnemyMove* m_pEnemyMove;                //敵の移動ステート
	vector<CAIModel*> m_VecMoveAi;           //移動AIモデル動的配列
	ENEMYTYPE m_Type;                        //敵の種類
	D3DXVECTOR3 m_MoveAiSavePos;             //移動AIの保存する位置
	int m_nIdxMoveAi;                        //移動AIの移動位置の配列を指定する
	int m_nPhaseNum;                         //フェーズ番号
	int m_nCntTime;                          //時間を測る
	float m_fRotMove;                        //敵の向きをホーミングさせるときに使用する
									         
	float m_fSensingRange;                   //感知射程
	float m_fNormalSpeed;                    //通常移動速度
									         
	Pattern m_Pattern;                       //パターン
										     
	bool m_bCollisoinDetection;              //衝突判定を行うかどうか
	bool m_bActivateCollisionDetection;      //衝突判定発動

	CAttack::ATTACKTYPE m_DefeatAttackType;  //倒された攻撃

	bool m_bCollisionWall;                   //壁と衝突したかどうか
	int m_nAttackCoolTime;                   //攻撃のクールタイムカウント用

	int m_nJumpCoolTime;                     //ジャンプまでのクールタイム

	bool m_bPossibleAttack;                  //攻撃可能かどうか
	bool m_bStartLanding;                    //最初に地面についたかどうか（赤い敵を最初に乗ったオブジェクトの上に固定するときなどに使う)
	STATE m_State;                           //状態列挙型
	//===============================================================================================

	//================================================
	//プロトタイプ宣言
	//================================================
	void CollisionProcess();                //当たり判定を行う
	void SetMoveAiPoint();                  //移動AIの配置をする
	void PhaseNumDecision();                //フェーズ番号を設定する
	virtual void AttackProcess();           //攻撃処理
	void CollisionDetectionProcess();       //衝突判定処理
	void CheckGrroundDistance_AdjustPos();  //地面との距離を求めて、一定以下なら1f前の位置に戻す

	//*ステータスエディット
	void EditNormalSpeed();                 //移動速度
	void EditSensingRange();                //索敵範囲
	//===============================================================================================
};

//===========================================
//射撃に弱いエネミークラス
//===========================================
class CShotWeakEnemy : public CEnemy
{
public:
	enum class SHOTWEAKENEMYTYPE
	{//射撃に弱い敵の種類
		NORMAL = 0,//普通
		MAX
	};

	CShotWeakEnemy(int nPri = 0, bool bUseintPri = false, CObject::TYPE type = CObject::TYPE::ENEMY, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_X);//コンストラクタ                   //コンストラクタ
	~CShotWeakEnemy();                  //デストラクタ
	HRESULT Init() override;            //初期化処理
	void Uninit() override;             //終了処理
	void Update() override;             //更新処理
	void Draw() override;               //描画処理
	void SetDeath() override;           //死亡フラグを設定
	static CShotWeakEnemy* Create(SHOTWEAKENEMYTYPE Type, int nLife,int nPhaseNum,D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 Scale);//生成処理

	//個別
	void SetShotWeakEnemyType(SHOTWEAKENEMYTYPE Type) { m_ShotWeakEnemyType = Type; }//射撃に弱い敵タイプ設定
	SHOTWEAKENEMYTYPE GetShotWeakEnemyType() { return m_ShotWeakEnemyType; }         //射撃に弱い敵タイプを取得

	//==========================================================
    //エディタ関係
    //==========================================================
    //関数
	void SaveInfoTxt(fstream& WritingFile) override;                                               //テキストファイルに情報を保存するための関数
	//static void LoadInfoTxt(fstream& LoadingFile, list<CObject*>& listSaveManager, string& Buff);  //テキストファイルから情報を読み込むための関数
	void LoadInfoTxt(fstream& LoadingFile, list<CObject*>& listSaveManager, string& Buff, CObject* pObj) override;//テキストファイルから情報を読み込むための関数
	CObject* ManagerChengeObject(bool bAim) override;                                              //ステージマネージャーに変更したオブジェクトを渡す
	CObject* ManagerSaveObject() override;                                                         //ステージマネージャーに今のオブジェクトを保存する
	//=================================================================================================================
protected:
private:
	//================================================
	//静的メンバ
	//================================================
	static const string s_aSHOTWEAKENEMY_FILENAME[static_cast<int>(SHOTWEAKENEMYTYPE::MAX)];       //敵のモデルファイル名
	static const float s_fATTACKSTART_LENGTH;                                                      //射撃に弱い敵の攻撃開始距離
	static const float s_fNORMAL_SENSIINGRANGE;                                                    //通常索敵範囲
	static const int s_nATTACK_COOLTIME;                                                           //攻撃のクールタイム
	//===============================================================================================

	//================================================
	//変数宣言
	//================================================
	CAttackEnemy* m_pMagicSword;                                                                  //魔法剣
	SHOTWEAKENEMYTYPE m_ShotWeakEnemyType;                                                        //敵の種類
	D3DXVECTOR3 m_SaveAimPos;                                                                     //目的の位置保存用
	//===============================================================================================

	//================================================
	//プロトタイプ宣言
	//================================================

	//*移動AI
	void BattleMoveProcess() override;                                                            //バトル移動処理
	void AttackProcess() override;                                                                //攻撃処理
	void DefeatStaging() override;                                                                //倒されたときの演出
	void SwordCollision();                                                                        //剣の当たり判定を行う

	//移動状態管理用のレイ
	void RayCollisionJumpOverOnHit();                                                             //レイが当たったら飛び越える処理に移行させる関数
	//===============================================================================================
};

//===========================================
//ダイブに弱いエネミークラス
//===========================================
class CDiveWeakEnemy : public CEnemy
{
public:
	enum class DIVEWEAKENEMYTYPE
	{//ダイブに弱い敵の種類
		NORMAL = 0,
		MAX
	};

	CDiveWeakEnemy(int nPri = 0, bool bUseintPri = false, CObject::TYPE type = CObject::TYPE::ENEMY, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_X);//コンストラクタ
	~CDiveWeakEnemy();                  //デストラクタ
	HRESULT Init() override;            //初期化処理
	void Uninit() override;             //終了処理
	void Update() override;             //更新処理
	void Draw() override;               //描画処理
	void SetDeath() override;           //死亡フラグを設定
	static CDiveWeakEnemy* Create(DIVEWEAKENEMYTYPE Type, int nLife,int nPhaseNum,D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 Scale,int nDivisionNum);//生成処理

	//個別
	void SetDiveWeakEnemyType(DIVEWEAKENEMYTYPE Type) { m_DiveWeakEnemyType = Type; }//ダイブに弱い敵の種類を設定
	DIVEWEAKENEMYTYPE GetDiveWeakEnemyType() { return m_DiveWeakEnemyType; }         //ダイブに弱い敵の種類を取得
	//==========================================================
    //エディタ関係
    //==========================================================
    //関数
	void SaveInfoTxt(fstream& WritingFile) override;                                               //テキストファイルに情報を保存するための関数
	//static void LoadInfoTxt(fstream& LoadingFile, list<CObject*>& listSaveManager, string& Buff);  //テキストファイルから情報を読み込むための関数
	void LoadInfoTxt(fstream& LoadingFile, list<CObject*>& listSaveManager, string& Buff, CObject* pObj) override;//テキストファイルから情報を読み込むための関数
	CObject* ManagerChengeObject(bool bAim) override;                                              //ステージマネージャーに変更したオブジェクトを渡す
	CObject* ManagerSaveObject() override;                                                         //ステージマネージャーに今のオブジェクトを保存する
	void ManagerChooseControlInfo() override;                                                      //ステージマネージャーから操作する
	//=================================================================================================================

	//==========================================================
	//ステータス関係
	//==========================================================
	void SetDivisionNum(int nNum) {m_nDivisionNum = nNum;}                                         //分裂回数を設定する（赤い敵は射撃で倒すと分裂する)
	const int& GetDivisionNum() const { return m_nDivisionNum; }                                   //分裂回数を取得する
	//===============================================================================================
protected:
private:
	//================================================
	//静的メンバ
	//================================================
	static const string s_aDIVEWEAKENEMY_FILENAME[static_cast<int>(DIVEWEAKENEMYTYPE::MAX)]; //敵のモデルファイル名
	static const int s_nATTACK_FREQUENCY;                                                    //攻撃頻度
	static const float s_fSENSINGRANGE;                                                      //感知射程
	static const float s_fNORMAL_SPEED;                                                      //通常の速度
	//===============================================================================================

	//================================================
	//変数宣言
	//================================================
	DIVEWEAKENEMYTYPE m_DiveWeakEnemyType;                                                   //敵の種類
	bool m_bStartDeath;                                                                      //分裂を開始する
	int m_nDivisionNum;                                                                      //分裂回数
	//===============================================================================================

	//================================================
	//プロトタイプ宣言
	//================================================
	
	//*攻撃移動
	void BattleMoveProcess() override;

	//攻撃処理
	void AttackProcess() override;

	//AI移動
	void AIMoveProcess() override;
	void DefeatStaging() override;//倒されたときの演出

	//===============================================================================================
};

//===========================================
//何もしない敵クラス
//===========================================
class CIdleEnemy : public CEnemy
{
public:
	enum class IDLEENEMYTYPE
	{//何もしない敵の種類
		NORMAL = 0,
		MAX
	};

	//==========================================================
	//基本
	//==========================================================
	CIdleEnemy(int nPri = 0, bool bUseintPri = false, CObject::TYPE type = CObject::TYPE::ENEMY, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_X);//コンストラクタ
	~CIdleEnemy();                      //デストラクタ
	HRESULT Init() override;            //初期化処理
	void Uninit() override;             //終了処理
	void Update() override;             //更新処理
	void Draw() override;               //描画処理
	void SetDeath() override;           //死亡フラグを設定
	static CIdleEnemy* Create(IDLEENEMYTYPE Type, int nLife, int nPhaseNum, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 Scale);//生成処理
	//=================================================================================================================

	//個別
	void SetIdleEnemyType(IDLEENEMYTYPE Type) { m_IdleEnemyType = Type; }//何もしない敵の種類を設定
	IDLEENEMYTYPE GetIdleEnemyType() { return m_IdleEnemyType; }         //何もしない敵の種類を設定

	//==========================================================
    //エディタ関係
    //==========================================================
    //関数
	void SaveInfoTxt(fstream& WritingFile) override;                                               //テキストファイルに情報を保存するための関数
	//static void LoadInfoTxt(fstream& LoadingFile, list<CObject*>& listSaveManager, string& Buff);  //テキストファイルから情報を読み込むための関数
	void LoadInfoTxt(fstream& LoadingFile, list<CObject*>& listSaveManager, string& Buff, CObject* pObj) override;//テキストファイルから情報を読み込むための関数
	CObject* ManagerChengeObject(bool bAim) override;                                              //ステージマネージャーに変更したオブジェクトを渡す
	CObject* ManagerSaveObject() override;                                                         //ステージマネージャーに今のオブジェクトを保存する
	void ManagerChooseControlInfo() override;                                                      //ステージマネージャーから操作する
	//=================================================================================================================
private:
	//================================================
    //静的メンバ
    //================================================
	static const string s_aIDLEENEMY_FILENAME[static_cast<int>(IDLEENEMYTYPE::MAX)];  //敵のモデルファイル名
	//===============================================================================================

	//================================================
	//変数宣言
	//================================================
	IDLEENEMYTYPE m_IdleEnemyType = IDLEENEMYTYPE::NORMAL;                            //敵の種類
	//===============================================================================================
	
	//================================================
	//プロトタイプ宣言
	//================================================
	
	//*攻撃移動
	void BattleMoveProcess() override;

	//攻撃処理
	void AttackProcess() override;

	//AI移動
	void AIMoveProcess() override;

	//倒されたときの演出
	void DefeatStaging() override;
	//===============================================================================================

};

//**************************************************************************************
//敵の状態クラス
//**************************************************************************************

//================================
//移動クラス
//================================

//スーパークラス
class CEnemyMove
{
public:
	CEnemyMove();                         //コンストラクタ
	virtual ~CEnemyMove();                //デストラクタ
	virtual void Process(CEnemy* pEnemy); //処理
};

//移動タイプ：AI
class CEnemyMove_AI : public CEnemyMove
{
public:
	CEnemyMove_AI();                        //コンストラクタ
	~CEnemyMove_AI() override;              //デストラクタ
	void Process(CEnemy* pEnemy) override;  //処理
};

//移動タイプ：バトル
class CEnemyMove_Battle : public CEnemyMove
{
public:
	CEnemyMove_Battle();                    //コンストラクタ
	~CEnemyMove_Battle() override;          //デストラクタ
	void Process(CEnemy* pEnemy) override;  //処理
};

//移動タイプ：壁よけ
class CEnemyMove_DodgeWall : public CEnemyMove
{
public:
	CEnemyMove_DodgeWall(CEnemy * pEnemy,D3DXVECTOR3 DodgeMove);  //コンストラクタ
	~CEnemyMove_DodgeWall() override;                             //デストラクタ
	void Process(CEnemy* pEnemy)override;                         //処理
private:
	D3DXVECTOR3 m_DodgeMove;                                      //回避移動量
};

//移動タイプ：怯え
class CEnemyMove_Frightened : public CEnemyMove
{
public:
	CEnemyMove_Frightened(CEnemy * pEnemy,D3DXVECTOR3 StopPos,int nStateTime);//コンストラクタ
	~CEnemyMove_Frightened() override;                                        //デストラクタ
	void Process(CEnemy* pEnemy) override;                                    //処理
private:
	D3DXVECTOR3 m_StopPos;                                                    //止める位置
	CUi* m_pLockOn;                                                           //ロックオンする
	int m_nStateTime;                                                         //怯え状態になる時間を設定
};

//移動タイプ：壁超えジャンプ
class CEnemyMove_OverJumpObj : public CEnemyMove
{
public:
	CEnemyMove_OverJumpObj(CEnemy* pEnemy,float fGoalheight);   //コンストラクタ
	~CEnemyMove_OverJumpObj() override;                         //デストラクタ
	void Process(CEnemy* pEnemy) override;                      //処理
private:
	float m_fGoalHeight = 0.0f;                                 //壁を超える時の目標の高さ
	int m_nCntTime = 0;                                         //この状態になってからの時間をカウントする
};

//移動タイプ：なし
class CEnemyMove_None : public CEnemyMove
{
public:
	CEnemyMove_None();                                         //コンストラクタ
	~CEnemyMove_None() override;                               //デストラクタ
	void Process(CEnemy* pEnemy) override;                     //処理
};

//===================================================================================================================================

#endif

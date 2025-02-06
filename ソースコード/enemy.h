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

	CEnemy(int nPri = 0, bool bUseintPri = false, CObject::TYPE type = CObject::TYPE::ENEMY, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_X);                   //コンストラクタ
	~CEnemy();                  //デストラクタ
	HRESULT Init() override;    //初期化処理
	void Uninit() override;     //終了処理
	void Update() override;     //更新処理
	void Draw() override;       //描画処理
	void SetDeath() override;   //死亡フラグを設定
	//static CEnemy* Create(ENEMYTYPE Type,int nLife,D3DXVECTOR3 pos, D3DXVECTOR3 rot,D3DXVECTOR3 Scale);

	const ENEMYTYPE& GetEnemyType() const { return m_Type; }
	//==========================================================
    //エディタ関係
    //==========================================================
    //関数
	void SaveInfoTxt(fstream& WritingFile) override;  //テキストファイルに情報を保存するための関数
	static void LoadInfoTxt(fstream& LoadingFile, list<CObject*>& listSaveManager, string& Buff);  //テキストファイルから情報を読み込むための関数   
	CObject* ManagerSaveObject() override;             //ステージマネージャーに今のオブジェクトを保存する
	void ManagerChooseControlInfo() override;          //ステージマネージャーから操作する
	void SetPhaseNum(int nNum) { m_nPhaseNum = nNum; } //フェーズ番号を設定する
	const int& GetPhaseNum() const { return m_nPhaseNum; }//フェーズ番号を取得する   
 	//=================================================================================================================

	//==========================================================
	//移動AI
	//==========================================================
	void SetVecMoveAiInfo(vector<CAIModel*> vec);
	vector<CAIModel*>& GetVecAiModelInfo() { return m_VecMoveAi; }
	virtual void AIMoveProcess();//AI移動処理
	virtual void BattleMoveProcess();//バトル移動処理
	void ChengeMove(CEnemyMove* pEnemyMove);//移動状態を変える
	//=================================================================================================================

	//==========================================================
	//バトル関係
	//==========================================================
	const int GetCntTime() const { return m_nCntTime; }
	void SetCntTime(int nTime) { m_nCntTime = nTime; }//時間を設定（タイミングをずらすなどができる）
	//感知射程
	void SetSensingRange(float fRange) { m_fSensingRange = fRange; }
	const float & GetSensingRange() const { return m_fSensingRange; }

	//通常移動速度
	void SetNormalSpeed(float fSpeed) { m_fNormalSpeed = fSpeed; }
	const float& GetNormalSpeed() const { return m_fNormalSpeed; }

	//倒された攻撃タイプを設定する
	void SetDefeatAttack(CAttack::ATTACKTYPE Type) { m_DefeatAttackType = Type; }
	const CAttack::ATTACKTYPE GetDefeatAttackType() const { return m_DefeatAttackType; }

	const bool& GetCollisionWall() const { return m_bCollisionWall; }
	//パターン
	void SetAction(bool bAction) { m_Pattern.bAction = bAction; }
	const bool& GetAction() const { return m_Pattern.bAction; }

	void EndAttackPattern();
	//=================================================================================================================
	const int GetAttackCoolTime() { return m_nAttackCoolTime; }
	void SetAttackCoolTime(int nTime) { m_nAttackCoolTime = nTime; }
	static int GetNumEnemy() { return m_nNumEnemy; }

	void SetState(STATE State) { m_State = State; }//状態異常を設定する
	const STATE& GetState() { return m_State; }    //状態異常を取得する

	//ジャンプまでのクールタイム
	void ResetJumpCoolTime() { m_nJumpCoolTime = 0; }
	int GetJumpCoolTime() { return m_nJumpCoolTime; }

	void SetPossibleAttack(bool bPossible) { m_bPossibleAttack = bPossible; }
protected:

	struct Pattern
	{
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
	void SetPattern(int nPattern) { m_Pattern.nPattern = nPattern; }
	const int& GetPattern() const { return m_Pattern.nPattern; }

	void SetSubPattern(int nSubPattern) { m_Pattern.nSubPattern = nSubPattern; }
	const int& GetSubpattern() const { return m_Pattern.nSubPattern; }

	void SetPatternTime(int nPatternTime) { m_Pattern.nPatternTime = nPatternTime; }
	const int& GetPatternTime() const { return m_Pattern.nPatternTime; }

	void SetUseCollisionDetection(bool bUse) { m_bCollisoinDetection = bUse; }

	const bool& GetCollisionDetection() const { return m_bActivateCollisionDetection; }

	void RayCollision();                               //レイがオブジェクトに当たっているかどうか

	//移動処理
	void ChasePlayer();//プレイヤーを追いかける処理
	//===============================================================================================

	virtual void DefeatStaging();//倒されたときの演出
private:
	//================================================
	//静的メンバ
	//================================================
	static int m_nNumEnemy;//敵の総数
	//===============================================================================================

	//================================================
	//変数宣言
	//================================================
	CEnemyMove* m_pEnemyMove;   //敵の移動ステート
	vector<CAIModel*> m_VecMoveAi;//移動AIモデル
	ENEMYTYPE m_Type;//敵の種類
	D3DXVECTOR3 m_MoveAiSavePos;//移動AIの保存する位置
	int m_nIdxMoveAi;           //移動AIの移動位置の配列を指定する
	int m_nPhaseNum;            //フェーズ番号
	int m_nCntTime;             //時間を測る
	float m_fRotMove;

	float m_fSensingRange;//感知射程
	float m_fNormalSpeed; //通常移動速度
	
	Pattern m_Pattern;    //パターン

	bool m_bCollisoinDetection;//衝突判定を行うかどうか
	bool m_bActivateCollisionDetection;//衝突判定発動

	CAttack::ATTACKTYPE m_DefeatAttackType;//倒された攻撃

	bool m_bCollisionWall;
	int m_nAttackCoolTime;

	int m_nJumpCoolTime;   //ジャンプまでのクールタイム

	bool m_bPossibleAttack;//攻撃可能かどうか
	bool m_bStartLanding;  //最初に地面についたかどうか
	STATE m_State;         //状態列挙型
	//===============================================================================================

	//================================================
	//プロトタイプ宣言
	//================================================
	void CollisionProcess();//当たり判定を行う
	void SetMoveAiPoint();  //移動AIの配置をする
	void PhaseNumDecision();//フェーズ番号を設定する
	virtual void AttackProcess();//攻撃処理
	void CollisionDetectionProcess();//衝突判定処理
	void CheckGrroundDistance_AdjustPos();//地面との距離を求めて、一定以下なら1f前の位置に戻す

	//*ステータスエディット
	void EditNormalSpeed();//移動速度
	void EditSensingRange();//索敵範囲
	//===============================================================================================
};

//===========================================
//射撃に弱いエネミークラス
//===========================================
class CShotWeakEnemy : public CEnemy
{
public:
	enum class SHOTWEAKENEMYTYPE
	{
		NORMAL = 0,
		MAX
	};

	CShotWeakEnemy(int nPri = 0, bool bUseintPri = false, CObject::TYPE type = CObject::TYPE::ENEMY, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_X);                   //コンストラクタ
	~CShotWeakEnemy();                  //デストラクタ
	HRESULT Init() override;            //初期化処理
	void Uninit() override;             //終了処理
	void Update() override;             //更新処理
	void Draw() override;               //描画処理
	void SetDeath() override;           //死亡フラグを設定
	static CShotWeakEnemy* Create(SHOTWEAKENEMYTYPE Type, int nLife,int nPhaseNum,D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 Scale);

	//==========================================================
    //エディタ関係
    //==========================================================
    //関数
	void SaveInfoTxt(fstream& WritingFile) override;  //テキストファイルに情報を保存するための関数
	static void LoadInfoTxt(fstream& LoadingFile, list<CObject*>& listSaveManager, string& Buff);  //テキストファイルから情報を読み込むための関数   
	CObject* ManagerChengeObject(bool bAim) override; //ステージマネージャーに変更したオブジェクトを渡す
	CObject* ManagerSaveObject() override;             //ステージマネージャーに今のオブジェクトを保存する
	//=================================================================================================================
protected:
private:
	//================================================
	//静的メンバ
	//================================================
	static const string s_aSHOTWEAKENEMY_FILENAME[static_cast<int>(SHOTWEAKENEMYTYPE::MAX)];//敵のモデルファイル名
	static const float s_fATTACKSTART_LENGTH;
	static const float s_fNORMAL_SENSIINGRANGE;
	static const int s_nATTACK_COOLTIME;//攻撃のクールタイム
	//===============================================================================================

	//================================================
	//変数宣言
	//================================================
	CAttackEnemy* m_pMagicSword;//魔法剣
	SHOTWEAKENEMYTYPE m_ShotWeakEnemyType;//敵の種類
	D3DXVECTOR3 m_SaveAimPos;//目的の位置保存用
	//===============================================================================================

	//================================================
	//プロトタイプ宣言
	//================================================

	//*移動AI
	void BattleMoveProcess() override;//バトル移動処理
	void AttackProcess() override;
	void SwordCollision();//剣の当たり判定を行う
	void DefeatStaging() override;//倒されたときの演出

	//移動状態管理用のレイ
	void RayCollisionJumpOverOnHit();//レイが当たったら飛び越える処理に移行させる関数
	//===============================================================================================
};

//===========================================
//ダイブに弱いエネミークラス
//===========================================
class CDiveWeakEnemy : public CEnemy
{
public:
	enum class DIVEWEAKENEMYTYPE
	{
		NORMAL = 0,
		MAX
	};

	CDiveWeakEnemy(int nPri = 0, bool bUseintPri = false, CObject::TYPE type = CObject::TYPE::ENEMY, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_X);                   //コンストラクタ
	~CDiveWeakEnemy();                  //デストラクタ
	HRESULT Init() override;            //初期化処理
	void Uninit() override;             //終了処理
	void Update() override;             //更新処理
	void Draw() override;               //描画処理
	void SetDeath() override;           //死亡フラグを設定
	static CDiveWeakEnemy* Create(DIVEWEAKENEMYTYPE Type, int nLife,int nPhaseNum,D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 Scale,int nDivisionNum);

	//==========================================================
    //エディタ関係
    //==========================================================
    //関数
	void SaveInfoTxt(fstream& WritingFile) override;  //テキストファイルに情報を保存するための関数
	static void LoadInfoTxt(fstream& LoadingFile, list<CObject*>& listSaveManager, string& Buff);  //テキストファイルから情報を読み込むための関数   
	CObject* ManagerChengeObject(bool bAim) override; //ステージマネージャーに変更したオブジェクトを渡す
	CObject* ManagerSaveObject() override;             //ステージマネージャーに今のオブジェクトを保存する
	void ManagerChooseControlInfo() override;          //ステージマネージャーから操作する
	//=================================================================================================================

	//==========================================================
	//ステータス関係
	//==========================================================
	void SetDivisionNum(int nNum) {m_nDivisionNum = nNum;}
	const int& GetDivisionNum() const { return m_nDivisionNum; }
	//===============================================================================================
protected:
private:
	//================================================
	//静的メンバ
	//================================================
	static const string s_aDIVEWEAKENEMY_FILENAME[static_cast<int>(DIVEWEAKENEMYTYPE::MAX)];//敵のモデルファイル名
	static const int s_nATTACK_FREQUENCY;//攻撃頻度
	static const float s_fSENSINGRANGE;  //感知射程
	static const float s_fNORMAL_SPEED;//通常の速度
	//===============================================================================================

	//================================================
	//変数宣言
	//================================================
	DIVEWEAKENEMYTYPE m_DiveWeakEnemyType;//敵の種類
	bool m_bStartDeath = false;           //分裂を開始する
	int m_nDivisionNum;                   //分裂回数
	//===============================================================================================

	//================================================
	//プロトタイプ宣言
	//================================================
	
	//*攻撃移動
	void BattleMoveProcess() override;//バトル移動処理]

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

	//タイプ列挙型
	enum class IDLEENEMYTYPE
	{
		NORMAL = 0,
		MAX
	};

	//==========================================================
	//基本
	//==========================================================
	CIdleEnemy(int nPri = 0, bool bUseintPri = false, CObject::TYPE type = CObject::TYPE::ENEMY, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_X);                   //コンストラクタ
	~CIdleEnemy();                  //デストラクタ
	HRESULT Init() override;            //初期化処理
	void Uninit() override;             //終了処理
	void Update() override;             //更新処理
	void Draw() override;               //描画処理
	void SetDeath() override;           //死亡フラグを設定
	static CIdleEnemy* Create(IDLEENEMYTYPE Type, int nLife, int nPhaseNum, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 Scale);
	//=================================================================================================================

	//==========================================================
    //エディタ関係
    //==========================================================
    //関数
	void SaveInfoTxt(fstream& WritingFile) override;  //テキストファイルに情報を保存するための関数
	static void LoadInfoTxt(fstream& LoadingFile, list<CObject*>& listSaveManager, string& Buff);  //テキストファイルから情報を読み込むための関数   
	CObject* ManagerChengeObject(bool bAim) override; //ステージマネージャーに変更したオブジェクトを渡す
	CObject* ManagerSaveObject() override;             //ステージマネージャーに今のオブジェクトを保存する
	void ManagerChooseControlInfo() override;          //ステージマネージャーから操作する
	//=================================================================================================================
private:
	//================================================
    //静的メンバ
    //================================================
	static const string s_aIDLEENEMY_FILENAME[static_cast<int>(IDLEENEMYTYPE::MAX)];//敵のモデルファイル名
	//===============================================================================================

	//================================================
	//変数宣言
	//================================================
	IDLEENEMYTYPE m_IdleEnemyType = IDLEENEMYTYPE::NORMAL;        //敵の種類
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
	CEnemyMove();//コンストラクタ
	virtual ~CEnemyMove();//デストラクタ
	virtual void Process(CEnemy* pEnemy);//処理
};

//移動タイプ：AI
class CEnemyMove_AI : public CEnemyMove
{
public:
	CEnemyMove_AI();//コンストラクタ
	~CEnemyMove_AI() override;//デストラクタ
	void Process(CEnemy* pEnemy) override;
};

//移動タイプ：バトル
class CEnemyMove_Battle : public CEnemyMove
{
public:
	CEnemyMove_Battle();//コンストラクタ
	~CEnemyMove_Battle() override;//デストラクタ
	void Process(CEnemy* pEnemy) override;
};

//移動タイプ：壁よけ
class CEnemyMove_DodgeWall : public CEnemyMove
{
public:
	CEnemyMove_DodgeWall(CEnemy * pEnemy,D3DXVECTOR3 DodgeMove);//コンストラクタ
	~CEnemyMove_DodgeWall() override;//デストラクタ
	void Process(CEnemy* pEnemy)override;
private:
	D3DXVECTOR3 m_DodgeMove;//回避移動量
};

//移動タイプ：怯え
class CEnemyMove_Frightened : public CEnemyMove
{
public:
	CEnemyMove_Frightened(CEnemy * pEnemy,D3DXVECTOR3 StopPos,int nStateTime);//コンストラクタ
	~CEnemyMove_Frightened() override;//デストラクタ
	void Process(CEnemy* pEnemy) override;//処理
private:
	D3DXVECTOR3 m_StopPos;//止める位置
	CUi* m_pLockOn;       //ロックオンする
	int m_nStateTime;     //怯え状態になる時間を設定
};

//移動タイプ：壁超えジャンプ
class CEnemyMove_OverJumpObj : public CEnemyMove
{
public:
	CEnemyMove_OverJumpObj(CEnemy* pEnemy,float fGoalheight);//コンストラクタ
	~CEnemyMove_OverJumpObj() override;//デストラクタ
	void Process(CEnemy* pEnemy) override;//処理
private:
	float m_fGoalHeight = 0.0f;//壁を超える時の目標の高さ
	int m_nCntTime = 0;//この状態になってからの時間をカウントする
};

//移動タイプ：なし
class CEnemyMove_None : public CEnemyMove
{
public:
	CEnemyMove_None();//コンストラクタ
	~CEnemyMove_None() override;//デストラクタ
	void Process(CEnemy* pEnemy) override;
};

//===================================================================================================================================

#endif

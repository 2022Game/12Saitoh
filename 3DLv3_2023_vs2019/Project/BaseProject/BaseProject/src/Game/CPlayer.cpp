//プレイヤークラスのインクルード
#include "CPlayer.h"
#include "CInput.h"
#include "CCamera.h"

// プレイヤーのインスタンス
CPlayer* CPlayer::spInstance = nullptr;

// プレイヤーのモデルデータのパス
#define MODEL_PATH "Character\\Player\\player.x"

// プレイヤーのアニメーションデータのテーブル
const CPlayer::AnimData CPlayer::ANIM_DATA[] =
{
	{ "",										true,	0.0f	},	// Tポーズ
	{ "Character\\Player\\anim\\idle.x",		true,	601.0f	},	// 待機
	{ "Character\\Player\\anim\\run_start.x",	false,	36.0f	},	// 走り開始
	{ "Character\\Player\\anim\\run_loop.x",	true,	40.0f	},	// 走る
	{ "Character\\Player\\anim\\run_end.x",		false,	50.0f	},	// 走り終了
	{ "Character\\Player\\anim\\fastrun_loop.x",true,	28.0f	},	// ダッシュ	
	//{ "Character\\Player\\anim\\jump_start.x",	false,	25.0f	},	// ジャンプ開始
	//{ "Character\\Player\\anim\\jump.x",		true,	1.0f	},	// ジャンプ中
	//{ "Character\\Player\\anim\\jump_end.x",	false,	26.0f	},	// ジャンプ終了
};

#define PLAYER_HEIGHT 16.0f
#define MOVE_SPEED 1.0f			// 走る速度
#define FASTMOOV_SPEED 1.5f		//ダッシュ速度
#define JUMP_SPEED 1.5f
#define GRAVITY 0.0625f
#define JUMP_END_Y 1.0f

// コンストラクタ
CPlayer::CPlayer()
	: CXCharacter(ETag::ePlayer, ETaskPriority::ePlayer)
	, mState(EState::eIdle)
	, mpRideObject(nullptr)
{
	//インスタンスの設定
	spInstance = this;

	// モデルデータ読み込み
	CModelX* model = new CModelX();
	model->Load(MODEL_PATH);

	// テーブル内のアニメーションデータを読み込み
	int size = ARRAY_SIZE(ANIM_DATA);
	for (int i = 0; i < size; i++)
	{
		const AnimData& data = ANIM_DATA[i];
		if (data.path.empty()) continue;
		model->AddAnimationSet(data.path.c_str());
	}
	// CXCharacterの初期化
	Init(model);

	// 最初は待機アニメーションを再生
	ChangeAnimation(EAnimType::eIdle);

	//線分コライダの設定
	mpColliderLine = new CColliderLine
	(
		this, ELayer::eField,
		CVector(0.0f, 0.0f, 0.0f),
		CVector(0.0f, PLAYER_HEIGHT, 0.0f)
	);
	mpColliderLine->SetCollisionLayers({ ELayer::eField });
}

CPlayer::~CPlayer()
{
	if (mpColliderLine != nullptr)
	{
		delete mpColliderLine;
		mpColliderLine = nullptr;
	}

	if (mpModel != nullptr)
	{
		delete mpModel;
		mpModel = nullptr;
	}
}

CPlayer* CPlayer::Instance()
{
	return spInstance;
}

// アニメーション切り替え
void CPlayer::ChangeAnimation(EAnimType type)
{
	if (!(EAnimType::None < type && type < EAnimType::Num)) return;
	AnimData data = ANIM_DATA[(int)type];
	CXCharacter::ChangeAnimation((int)type, data.loop, data.frameLength);
}

// 待機
void CPlayer::Update_Idle()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	
	if (mIsGrounded)
	{
		// 移動キーが押されたか判定
		if (CInput::PushKey('W') || CInput::PushKey('A') ||
			CInput::PushKey('S') || CInput::PushKey('D'))
		{
			// 走り出しのアニメーションを再生
			ChangeAnimation(EAnimType::eRunStart);
			mState = EState::eMove;
		}
		else
		{
			// 走り終わりのアニメーションが終了したら
			// アイドルアニメーションに切り替える
			if (IsAnimationFinished())
			{
				ChangeAnimation(EAnimType::eIdle);
			}
		}

		// 左クリックで攻撃状態へ移行
		if (CInput::PushKey(VK_LBUTTON))
		{
			mMoveSpeed.X(0.0f);
			mMoveSpeed.Z(0.0f);
			mState = EState::eAttack;
		}
		// SPACEキーでジャンプ開始へ移行
		else if (CInput::PushKey(VK_SPACE))
		{
			mState = EState::eJumpStart;
		}
	}
	else
	{
		// 待機状態に切り替え
		ChangeAnimation(EAnimType::eIdle);
	}
}

//移動
void CPlayer::Update_Move()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);

	//地面に接地しているか判定
	if (mIsGrounded)
	{
		// 移動処理
		// キーの入力ベクトルを取得
		CVector input;
		if (CInput::Key('W'))		input.Z(1.0f);
		else if (CInput::Key('S'))	input.Z(-1.0f);
		if (CInput::Key('A'))		input.X(-1.0f);
		else if (CInput::Key('D'))	input.X(1.0f);
		
		// 入力ベクトルの長さで入力されているか判定
		if (input.LengthSqr() > 0)
		{
			// カメラの向きに合わせた移動ベクトルに変換
			CVector move = CCamera::MainCamera()->Rotation() * input;
			move.Y(0.0f);
			move.Normalize();

			mMoveSpeed += move * MOVE_SPEED;

			//走り出しアニメーションが終了
			if (AnimationIndex() == (int)EAnimType::eRunStart &&
				IsAnimationFinished())
			{
				// 走りアニメーションに切り替え
				ChangeAnimation(EAnimType::eRun);
			}
			if (CInput::PushKey(VK_SHIFT))
			{
				mState = EState::eFastMove;
			}
		}
		// 移動キーが押されていない
		else
		{
			// 走り終わりのアニメーションを再生し、アイドル状態に戻す
			ChangeAnimation(EAnimType::eRunEnd);
			mState = EState::eIdle;
		}
	}
}

//ダッシュ移動
void CPlayer::Update_FastMove()
{

}

// 攻撃
void CPlayer::Update_Attack()
{
	// 攻撃アニメーションを開始
	ChangeAnimation(EAnimType::eAttack);
	// 攻撃終了待ち状態へ移行
	mState = EState::eAttackWait;
}

// 攻撃終了待ち
void CPlayer::Update_AttackWait()
{
	// 攻撃アニメーションが終了したら、
	if (IsAnimationFinished())
	{
		// 待機状態へ移行
		mState = EState::eIdle;
		ChangeAnimation(EAnimType::eIdle);
	}
}

// ジャンプ開始
void CPlayer::Update_JumpStart()
{
	ChangeAnimation(EAnimType::eJumpStart);
	mState = EState::eJump;

	mMoveSpeed += CVector(0.0f, JUMP_SPEED, 0.0f);
	mIsGrounded = false;
}

// ジャンプ中
void CPlayer::Update_Jump()
{
	if (mMoveSpeed.Y() <= 0.0f)
	{
		ChangeAnimation(EAnimType::eJumpEnd);
		mState = EState::eJumpEnd;
	}
}

// ジャンプ終了
void CPlayer::Update_JumpEnd()
{
	if (IsAnimationFinished())
	{
		mState = EState::eIdle;
	}
}

// 更新
void CPlayer::Update()
{
	SetParent(mpRideObject);
	mpRideObject = nullptr;

	// 状態に合わせて、更新処理を切り替える
	switch (mState)
	{
		// 待機状態
		case EState::eIdle:
			Update_Idle();
			break;
		// 移動状態
		case EState::eMove:
			Update_Move();
			break;
		// ダッシュ移動
		case EState::eFastMove:
			Update_FastMove();
			break;
		// 攻撃
		case EState::eAttack:
			Update_Attack();
			break;
		// 攻撃終了待ち
		case EState::eAttackWait:
			Update_AttackWait();
			break;
		// ジャンプ開始
		case EState::eJumpStart:
			Update_JumpStart();
			break;
		// ジャンプ中
		case EState::eJump:
			Update_Jump();
			break;
		// ジャンプ終了
		case EState::eJumpEnd:
			Update_JumpEnd();
			break;
	}

	mMoveSpeed -= CVector(0.0f, GRAVITY, 0.0f);

	// 移動
	Position(Position() + mMoveSpeed);

	// プレイヤーを移動方向へ向ける
	CVector current = VectorZ();
	CVector target = mMoveSpeed;
	target.Y(0.0f);
	target.Normalize();
	CVector forward = CVector::Slerp(current, target, 0.125f);
	Rotation(CQuaternion::LookRotation(forward));

	// キャラクターの更新
	CXCharacter::Update();

	mIsGrounded = false;
}

// 衝突処理
void CPlayer::Collision(CCollider* self, CCollider* other, const CVector& adjust)
{
	if (self == mpColliderLine)
	{
		if (other->Layer() == ELayer::eField)
		{
			mMoveSpeed.Y(0.0f);
			Position(Position() + adjust);
			mIsGrounded = true;

			if (other->Tag() == ETag::eRideableObject)
			{
				mpRideObject = other->Owner();
			}
		}
	}
}

// 描画
void CPlayer::Render()
{
	CXCharacter::Render();
}

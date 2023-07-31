#pragma once

//シーンの種類
enum class EScene
{
	eNone,	//どのシーンにも所属しない
	eTitle,	//タイトルシーン
	eGame,	//ゲームシーン
	eOver,	//ゲームオーバーシーン
	eClear	//ゲームクリアシーン
};
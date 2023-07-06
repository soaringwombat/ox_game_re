# include <Siv3D.hpp> // OpenSiv3D v0.6.10

Grid<int> board_status = { {0, 0, 0},{0, 0, 0},{0, 0, 0} };	//マスの状態:0.なし，1.o，2.x
bool player = true; // true:o, false:x
bool end_flag = false;
int onBoard[2] = { 0, 0 };

//初期化
void reset() {
	for (int8 i = 0; i < 3; i++) {
		for (int8 j = 0; j < 3; j++) {
			board_status[i][j] = 0;
		}
	}
	ClearPrint();
}

// 終了時の処理
void end_effect(int in) {
	if (in == 1) {
		if (player) {
			Print << U"〇の勝ち！！\n";
		}
		else {
			Print << U"×の勝ち！！\n";
		}
	}
	else if (in == 2) {
		Print << U"引き分け！！\n";
	}
	else {
		return;
	}
	Print << U"リセットする場合は\n'A'を押してください\n";
	Print << U"やめる場合は\n'S'を押してください\n";
	end_flag = true;
}

// マスの選択
void select_onBoard() {

	Rect{ Arg::center(250 + 150 * onBoard[1], 150 + 150 * onBoard[0]), 150, 150 }
	.drawFrame(0, 3, Palette::Black);
	//右移動
	if (KeyRight.down()) {
		onBoard[1] = Min(onBoard[1] + 1, 2);
	}
	//左移動
	if (KeyLeft.down()) {
		onBoard[1] = Max(onBoard[1] - 1, 0);
	}
	//下移動
	if (KeyDown.down()) {
		onBoard[0] = Min(onBoard[0] + 1, 2);
	}
	//上移動
	if (KeyUp.down()) {
		onBoard[0] = Max(onBoard[0] - 1, 0);
	}
}

//盤上のマス目，背景などの設定
void setup() {

	// 背景色を設定
	Scene::SetBackground(ColorF{ 1.0, 0.8, 1.0 });

	//盤を描写
	Rect{ Arg::center(400, 300),450,450 }.draw(Palette::Green);
	Line{ 175, 375, 625, 375 }.draw(4, Palette::Black);
	Line{ 175, 225, 625, 225 }.draw(4, Palette::Black);
	Line{ 325, 75, 325, 525 }.draw(4, Palette::Black);
	Line{ 475, 75, 475, 525 }.draw(4, Palette::Black);

}

//盤上の状態描写
void status_draw() {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (board_status[i][j] == 1) {
				Circle{ 250 + 150 * i, 150 + 150 * j, 60 }.drawFrame(10, Palette::Black);
			}
			else if (board_status[i][j] == 2) {
				Shape2D::Cross(60, 10, Vec2{ 250 + 150 * i, 150 + 150 * j }).draw(Palette::Black);
			}
		}
	}
}

// 石を返す
void turn_stone(bool turn) {
	if (turn) {
		board_status[onBoard[1]][onBoard[0]] = 1;
	}
	else {
		board_status[onBoard[1]][onBoard[0]] = 2;
	}
}

// 引き分け確認
bool draw_check() {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (board_status[i][j] == 0) {
				return false;
			}
		}
	}
	return true;
}

//列ができているか確認
int board_check() {
	int out_flag = 0;	//0:試合中, 1:どちらかの勝ち, 2:引き分け
	if (board_status[0][0] != 0 && board_status[0][0] == board_status[0][1] && board_status[0][1] == board_status[0][2]) {
		out_flag = 1;
	}
	if (board_status[1][0] != 0 && board_status[1][0] == board_status[1][1] && board_status[1][1] == board_status[1][2]) {
		out_flag = 1;
	}
	if (board_status[2][0] != 0 && board_status[2][0] == board_status[2][1] && board_status[2][1] == board_status[2][2]) {
		out_flag = 1;
	}
	if (board_status[0][0] != 0 && board_status[0][0] == board_status[1][0] && board_status[1][0] == board_status[2][0]) {
		out_flag = 1;
	}
	if (board_status[0][1] != 0 && board_status[0][1] == board_status[1][1] && board_status[1][1] == board_status[2][1]) {
		out_flag = 1;
	}
	if (board_status[0][2] != 0 && board_status[0][2] == board_status[1][2] && board_status[1][2] == board_status[2][2]) {
		out_flag = 1;
	}
	if (board_status[0][0] != 0 && board_status[0][0] == board_status[1][1] && board_status[1][1] == board_status[2][2]) {
		out_flag = 1;
	}
	if (board_status[0][2] != 0 && board_status[0][2] == board_status[1][1] && board_status[1][1] == board_status[2][0]) {
		out_flag = 1;
	}

	if (draw_check() && out_flag == 0) {
		out_flag = 2;
	}
	return out_flag;
}

void Main()
{
	while (System::Update())
	{
		setup();

		select_onBoard();

		status_draw();

		// "Licenses" ボタンが押されたら
		if (SimpleGUI::Button(U"Licenses", Vec2{ 600, 20 }))
		{
			// ライセンス情報を表示
			LicenseManager::ShowInBrowser();
		}
		// "Reset" ボタンが押されたら
		if (SimpleGUI::Button(U"Reset", Vec2{ 500, 20 }))
		{
			// 盤面のリセット
			reset();
		}

		if (end_flag) {
			if (KeyA.down()) {
				reset();
				end_flag = false;
			}
			if (KeyS.down()) {
				System::Exit();
			}
		}
		else if (KeySpace.down()) {
			if (board_status[onBoard[1]][onBoard[0]] == 0) {
				turn_stone(player);
				end_effect(board_check());
			}

			player = !player;
		}

		//コンピュータ処理,xをCPUとする．
		if (!player) {

		}

	}
}

//
// - Debug ビルド: プログラムの最適化を減らす代わりに、エラーやクラッシュ時に詳細な情報を得られます。
//
// - Release ビルド: 最大限の最適化でビルドします。
//
// - [デバッグ] メニュー → [デバッグの開始] でプログラムを実行すると、[出力] ウィンドウに詳細なログが表示され、エラーの原因を探せます。
//
// - Visual Studio を更新した直後は、プログラムのリビルド（[ビルド]メニュー → [ソリューションのリビルド]）が必要な場合があります。
//

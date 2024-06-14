# include "stdafx.h" // Siv3D v0.6.14


enum class SquareState :unsigned char{
	space, black, white
};

/*
	次の石の色を返す。
	@param[in]  c       現在の石の色
*/
inline SquareState next_color(SquareState c)
{
	return (c == SquareState::black) ? SquareState::white : SquareState::black;
}

class Start {
public:
	void View() {
		font(first).draw(Arg::center(400, 200));
		font(second).draw(Arg::center(400, 400));
	}
private:
	const Font font{ 60 };
	String first = U"Choose first move?\n Press 'F'";
	String second = U"Choose second move?\n Press 'S'";
};


class Over {
public:
	void View(const String& text1, int stw, int stb) {
		title(text1).draw(Arg::center(400, 100), Palette::Red);
		font(U"Black{}:White{}"_fmt(stb, stw)).draw(Arg::center(400, 200));
		font(restart).draw(Arg::center(400, 400));
		font(quit).draw(Arg::center(400, 500));
	}
private:
	const Font title{ 100 };
	const Font font{ 60 };
	String restart = U"Restart Press'R'";
	String quit = U"Quit Press 'Q'";
};


class Board {
public:
	void Draw() {
		board.drawFrame(3.0);
		board.stretched(-1).draw(Palette::Darkgreen);
		for (int y = board.y + 70; y <= board.bottomY(); y += 70) {
			Line(Vec2(board.x, y), Vec2(board.rightX(), y)).draw(1.5);
		}
		for (int x = board.x + 70; x <= board.rightX(); x += 70) {
			Line(Vec2(x, board.y), Vec2(x, board.bottomY())).draw(1.5);
		}
	}
	const RectF& Get_board()const {
		return board;
	}
	RectF& Get_board() {
		return board;
	}
private:
	RectF board = { Arg::center(400,300), 560};
};
class Stone: public Circle {
public:
	Stone(const Circle& _stone, ColorF _c) :Circle(_stone), color(_c){}
		ColorF color;
};
class Stonepos {
public:
	Stonepos() {
		for (int y = 0; y < 8; y++) {
			for (int x = 0; x < 8; x++) {
				board[y][x] = SquareState::space;
			}
		}
		board[3][3] = SquareState::white;
		board[4][4] = SquareState::white;
		board[3][4] = SquareState::black;
		board[4][3] = SquareState::black;
	}
	SquareState* operator[](int y){
		return board[y];
	}
	const SquareState* operator[](int y)const {
		return board[y];
	}
	bool IsSetDir(int px, int py, int dx, int dy, SquareState c, SquareState d) const{
		int count = 0;
		for (int y = py + dy, x = px + dx; y >= 0 && y < 8 && x >= 0 && x < 8; y += dy, x += dx) {
			if (board[y][x] == d) {
				count++;
			}
			else if (board[y][x] == c) {
				if (count == 0) break;
				else return true;
			}
			else {
				break;
			}
		}
		return false;
	}
	bool IsSet(int px, int py, SquareState c) const{
		if (board[py][px] != SquareState::space)return false;
		SquareState  d = (c == SquareState::black) ? SquareState::white : SquareState::black;
		for (int dy = -1; dy <= 1; dy++) {
			for (int dx = -1; dx <= 1; dx++) {
				if (dx == 0 && dy == 0)continue;
				if (IsSetDir(px, py, dx, dy, c, d) == true)return true;
			}
		}
		return false;
	}
	void Set(int px, int py, SquareState c) {
		SquareState  d = (c == SquareState::black) ? SquareState::white : SquareState::black;
		board[py][px] = c;

		for (int dy = -1; dy <= 1; dy++) {
			for (int dx = -1; dx <= 1; dx++) {
				if (dx == 0 && dy == 0)continue;
				if (IsSetDir(px, py, dx, dy, c, d) == true) {
					for (int y = py + dy, x = px + dx; y >= 0 && y < 8 && x >= 0 && x < 8; y += dy, x += dx) {
						if (board[y][x] == d) {
							board[y][x] = c;
						}
						else {
							break;
						}
					}
				}
			}
		}
	}
	bool PassChk(SquareState c)const {
		for (int y = 0; y < 8; y++) {
			for (int x = 0; x < 8; x++) {
				if (IsSet(x, y, c) == true) {
					return false;
				}
			}
		}
		return true;
	}
	
	int CountStone(SquareState c)const {
		int count = 0;
		for (int y = 0; y < 8; y++) {
			for (int x = 0; x < 8; x++) {
				if (board[y][x] == c) {
					count++;
				}
			}
		}
		return count;
	}
	bool Issue()const {
		if (CountStone(SquareState::black) == 0) {
			return true;
		}
		if (CountStone(SquareState::white) == 0) {
			return true;
		}
		if (CountStone(SquareState::space) == 0) {
			return true;
		}
		if (PassChk(SquareState::white) == true && PassChk(SquareState::black) == true) {
			return true;
		}
		return false;
	}
	String Winner(int pldata)const {
		if (CountStone(SquareState::black) == 0) {
			if (pldata == 1)return U"You Win";
			else return U"You Lose";
		}
		if (CountStone(SquareState::white) == 0) {
			if (pldata == 0)return U"You Win";
			else return U"You Lose";
		}
		if (CountStone(SquareState::space) == 0) {
			if (CountStone(SquareState::black) > CountStone(SquareState::white)) {
				if (pldata == 0)return U"You Win";
				else return U"You Lose";
			}
			else if (CountStone(SquareState::white) > CountStone(SquareState::black)) {
				if (pldata == 1)return U"You Win";
				else return U"You Lose";
			}
			else if (CountStone(SquareState::black) == CountStone(SquareState::white)) {
				return U"Draw";
			}
			if (PassChk(SquareState::white) == true && PassChk(SquareState::black) == true) {
				if (CountStone(SquareState::black) > CountStone(SquareState::white)) {
					if (pldata == 0)return U"You Win";
					else return U"You Lose";
				}
				else if (CountStone(SquareState::white) > CountStone(SquareState::black)) {
					if (pldata == 1)return U"You Win";
					else return U"You Lose";
				}
				else if (CountStone(SquareState::black) == CountStone(SquareState::white)) {
					return U"Draw";
				}
			}
		}
		return U"None";
	}
	void reset() {
		for (int y = 0; y < 8; y++) {
			for (int x = 0; x < 8; x++) {
				board[y][x] = SquareState::space;
			}
		}
		board[3][3] = SquareState::white;
		board[4][4] = SquareState::white;
		board[3][4] = SquareState::black;
		board[4][3] = SquareState::black;
	}
protected:
	SquareState board[8][8];
};
class StoneMngr {
public:
	StoneMngr(){
	}
	void Draw(const Board& bd) const{
		for (int y = 0; y < 8; y++) {
			for (int x = 0; x < 8; x++) {
				SquareState f = board[y][x];
				if (f != SquareState::space) {
					Circle{ Vec2(bd.Get_board().x + (x * 70) + 35,
						bd.Get_board().y + y * 70 + 35),30 }.
						draw((f == SquareState::black) ? Palette::Black : Palette::White);
				}
			}
		}
	}
	const Point& MousePos()const {
		return Cursor::Pos();
	}
	bool Issue()const {
		return board.Issue();
	}
	String Winner(int pldata)const {
		
		return board.Winner(pldata);
	}
	bool PassChk(SquareState c)const {
		return board.PassChk(c);
	}
	void SetandDraw(int px, int py, SquareState c, Board& desk) {
		SquareState  d = (c == SquareState::black) ? SquareState::white : SquareState::black;
		board[py][px] = c;
		desk.Draw();
		Draw(desk);
		System::Sleep(0.5s);
		System::Update();
		for (int dy = -1; dy <= 1; dy++) {
			for (int dx = -1; dx <= 1; dx++) {
				if (dx == 0 && dy == 0)continue;
				if (board.IsSetDir(px, py, dx, dy, c, d) == true) {
					for (int y = py + dy, x = px + dx; y >= 0 && y < 8 && x >= 0 && x < 8; y += dy, x += dx) {
						if (board[y][x] == d) {
							board[y][x] = c;
							desk.Draw();
							Draw(desk);
							System::Sleep(0.5s);
							System::Update();
						}
						else {
							break;
						}
					}
				}
			}
		}
	}
	void Set(Board& bd, const Point& pos, SquareState c, int& turn, String& game_state) {
		int x, y;
		x = (pos.x - bd.Get_board().x) / 70;
		y = (pos.y - bd.Get_board().y) / 70;
		if (x >= 0 && x < 8 && y >= 0 && y < 8) {
			if (board.IsSet(x, y, c) == true) {
				SetandDraw(x, y, c, bd);
				turn++;
			}
			if (Issue() == true) {
				game_state = U"Over";
			}

		}
	}
	bool Compute(SquareState c, int& turn, String& game_state, Board& desk);
	int Get_Stone(SquareState c) {
		return board.CountStone(c);
	}
	void reset() {
		board.reset();
	}

private:
	Stonepos board;
};

class StoneComp : public Stonepos{
std::vector<StoneComp> next; // 次の 1手を打った盤面
char px, py; // 本盤面 boardで打った位置(0~7, 0~7)
SquareState put_c; // 本盤面 boardで打った色(black, white)
char black_count; // 本盤面 boardの黒の数(0~64)
char white_count; // 本盤面 boardの白の数(0~64)
signed char rate; // 本盤面 boardの評価値(-64~64)、又は nextの各要素の最大評価値(-64~64)
static SquareState cpu_c;      // コンピュータ側の石の色
static int Level0;
enum :int { rate_pass = 65, rate_win = 66 };
static const int val[8][8];

public:// Constructor
	StoneComp(const Stonepos& org) :px(0), py(0), put_c(SquareState::space),
		black_count(2), white_count(2), rate(0), next() {
		for (int y = 0; y < 8; y++) {
			for (int x = 0; x < 8; x++) {
				board[y][x] = org[y][x];
			}
		}
	}// Draw用の StoneXから生成できるようにする
	StoneComp(const StoneComp& src) :next(), px(src.px), py(src.py), put_c(src.put_c),
		black_count(src.black_count), white_count(src.white_count), rate(src.rate) {
		for (int y = 0; y < 8; y++) {
			for (int x = 0; x < 8; x++) {
				board[y][x] = src[y][x];
			}
		}
	} // 親 srcから next格納用のコンストラクタ(srcの nextはコピーしない)
	StoneComp(const StoneComp& src, int x, int y, SquareState c):next(), px(x), py(y), put_c(c),
		black_count(src.black_count), white_count(src.white_count), rate(src.rate) {
		for (int j = 0; j < 8; j++) {
			for (int i = 0; i < 8; i++) {
				board[j][i] = src[j][i];
			}
		}
		Set(x, y, c);
		black_count = CountStone(SquareState::black);
		white_count = CountStone(SquareState::white);

	}
	bool operator < (const StoneComp& rhs) const {// sort, max_elementから呼び出される
			return rate + val[py][px] < rhs.rate + val[rhs.py][rhs.px];
	}
	void addnext(int x, int y, SquareState c, SquareState cpuc) {
		next.push_back(StoneComp(*this, x, y, c));
		if (cpuc == SquareState::white) {
			next.back().rate = next.back().white_count - next.back().black_count;
		}
		else {
			next.back().rate = - next.back().white_count + next.back().black_count;
		}

	}

	int compute_sub(int level, SquareState c);
	void compute(int level, SquareState c, int& x, int& y);

	
};
SquareState StoneComp::cpu_c = SquareState::black;
int StoneComp::Level0 = 0;
const int StoneComp::val[8][8] = {
	{16, -4, 8, 2, 2, 8, -4, 16},
	{-4, -16, -2, -2, -2, -2, -16, -8},
	{8, -2, 3, 1, 1, 3, -2, 8},
	{5, -2, 1, 3, 3, 1, -2, 5},
	{5, -2, 1, 3, 3, 1, -2, 5},
	{8, -2, 3, 1, 1, 3, -2, 8},
	{-4, -16, -2, -2, -2, -2, -16, -8},
	{16, -4, 8, 2, 2, 8, -4, 16}
};
/*
const int StoneComp::val[8][8] = {
	{170, -20, 20, 5, 5, 20, -20, 170},
	{-20, -40, -5, -5, -5, -5, -40, -20},
	{20, -5, 15, 3, 3, 15, -5, 20},
	{5, -5, 3, 3, 3, 3, -5, 5},
	{5, -5, 3, 3, 3, 3, -5, 5},
	{20, -5, 15, 3, 3, 15, -5, 20},
	{-20, -40, -5, -5, -5, -5, -40, -20},
	{170, -20, 20, 5, 5, 20, -20, 170}
};
*/
/*
	コンピュータ側の打つ手を総当たりで検索する。
	@param[in]  level   掘り下げて調べる手数(4, 3, 2, 1)
	@param[in]  c       打つ石の色
	@return 最大評価値(-64～64)を返す。
*/
int StoneComp::compute_sub(int level, SquareState c)
{
	for (int y = 0; y < 8; ++y) {
		for (int x = 0; x < 8; ++x) {
			if (IsSet(x, y, c)) {   // 打てる場所が見つかった
				addnext(x, y, c, cpu_c);  // 次の手としてnextに追加
				if (level == 0) {
					if (c == SquareState::black) {
						if (next.back().white_count == 0) {
							next.back().rate = rate_win;
							return rate_win;
						}
					}
					else  {
						if (next.back().black_count == 0) {
							next.back().rate = rate_win;
							return rate_win;
						}
					}
				}

				if (level < Level0) {
					// 今登録した手の先を検索する
					next.back().rate = next.back().compute_sub(level + 1, next_color(c));
				}
			}
		}
	}
	if (next.empty()) {
		// 打てる手が１つもなかった(パス or ゲーム終了)→パスとして登録
		next.push_back(StoneComp(*this));       // パスなので現在と同じ盤面情報を登録
		next.back().put_c = SquareState::space;   // パスである事を示すためにspaceを登録
		next.back().rate = next.back().white_count - next.back().black_count;  // 盤面評価値を登録(登録済かもしれない)
		if (cpu_c == SquareState::black) {
			next.back().rate = -next.back().rate;
		}
		if (level == 1) {
			next.back().rate = rate_pass;
			return rate_pass;
		}
		if (level < Level0) {
			// 今登録した手の先を検索する
			next.back().rate = next.back().compute_sub(level + 1, next_color(c));
		}
	}
	// nextの中で一番評価値が高い値を返す
	return std::max_element(next.begin(), next.end())->rate;
}

/*
	コンピュータ側の打つ手を決定する。
	@param[in]  level   掘り下げて調べる手数(4, 3, 2, 1)
	@param[in]  c       コンピュータの打つ石の色
	@param[out] x       コンピュータが打つ場所(0..7), パス=-1
	@param[out] y       コンピュータの打つ場所(0..7), パス=-1
*/
void StoneComp::compute(int level, SquareState c, int& x, int& y)
{
	Level0 = level;
	cpu_c = c;    // コンピュータの打つ石の色を登録
	compute_sub(0, c);	// levelだけほりさげて打つ手を評価
	auto it = std::max_element(next.begin(), next.end());  // 最大評価値の手を得る
	if (it->put_c == SquareState::space) {
		// パスだった
		x = -1;
		y = -1;
	}
	else {
		x = it->px;
		y = it->py;
	}
}

bool StoneMngr::Compute(SquareState c, int& turn, String& game_state, Board& desk) {
	StoneComp cpu(board);
	int spaces = board.CountStone(SquareState::space);
	SquareState d = (c != SquareState::white) ? SquareState::white : SquareState::black;
	int x, y;
	cpu.compute((spaces > 50 || spaces < 10) ? 6 : 4, c, x, y);
	if (x >= 0 && x < 8 && y >= 0 && y < 8) {
		if (board.IsSet(x, y, c) == true) {
			SetandDraw(x, y, c, desk);
			turn++;
		}
		if (Issue() == true) {
			System::Sleep(3s);
			game_state = U"Over";
		}
	}
	else if (x == -1 && y == -1) {
		if (board.PassChk(c) == true) {
			return true;
		}
	}
	return false;
}

void Main()
{
	// 背景の色を設定する | Set the background color
	Scene::SetBackground(ColorF{ 0, 0, 0 });
	System::SetTerminationTriggers(UserAction::CloseButtonClicked);
	Board board;
	StoneMngr stone;
	SquareState c, cpuc;
	Start gamebegin;
	Over gameover;
	int turn = 0, plside = 0;
	String Game_state = U"Start";
	Stopwatch stopwatch{ StartImmediately::No };
	Font side{ 20 };
	RectF passbutton;
	Font passtsiz{ 10 };
	String passt;
	Font pass_st{ 40 };
	passbutton = { 750, 450, 40, 20 };
	passt = U"PASS";
	while (System::Update())
	{
		if (Game_state == U"Start") {
			gamebegin.View();
			if (KeyF.pressed()) {
				plside = 0;
				cpuc = SquareState::white;
				Game_state = U"game";
			}
			else if (KeyS.pressed()) {
				plside = 1;
				cpuc = SquareState::black;
				Game_state = U"game";
			}
		}
		else if (Game_state == U"game") {
			c = (turn % 2 == 0) ? SquareState::black : SquareState::white;
			if (plside == turn % 2) {
				side(U"Your turn").draw(0, 570);
				if (stone.PassChk(c) == false) {
					if (MouseL.pressed()) {
						stone.Set(board, stone.MousePos(), c, turn, Game_state);
						side(U"CPU turn").draw(0, 570);
					}
				}
				else {
					passbutton.draw(passbutton.leftClicked() ? Palette::Red : Palette::Lightgreen);
					passtsiz(passt).draw(passbutton.center());
					if (passbutton.leftClicked()) {
						pass_st(U"PASS").draw(0, 0);
						System::Sleep(2s);
						turn++;
					}
				}

			}
			else {
				System::Sleep(2s);
				if (stone.Compute(cpuc, turn, Game_state, board) == true) {
					pass_st(U"PASS").draw(0, 0);
					System::Sleep(2s);
					turn++;
				}
			}

			board.Draw();
			stone.Draw(board);
		}
		else if (Game_state == U"Over") {
			stopwatch.start();
			if (stopwatch <= 3s) {
				board.Draw();
				stone.Draw(board);
			}
			else {
				gameover.View(stone.Winner(plside), stone.Get_Stone(SquareState::white), stone.Get_Stone(SquareState::black));
				if (KeyR.pressed()) {
					stone.reset();
					turn = 0;
					Game_state = U"Start";
				}
				if (KeyQ.pressed()) {
					System::Exit();
				}
			}
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

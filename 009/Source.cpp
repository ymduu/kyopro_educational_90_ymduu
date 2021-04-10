#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable: 4244) // 最悪をします
#include <iostream>
#include <cmath>
#include <algorithm>
#include <vector>
#include <numeric>
#include <queue>
#include <stack>
#include <map> 
#include <set>
#include <string>
#include <functional>
#include <list>
#include <random>
#include <time.h>
#include <iomanip>
#include <assert.h>
#include <numeric>
#include <sstream>
#define BIT(nr) (1UL << (nr))
#define int long long
//#define ll long long
#define double long double
#define mod 1000000007
#define MAXN (int)1e+5 * 2+1
#define LL_MAX 9223372036854775807	//ない環境用
#define LL_HALFMAX 9223372036854775807 / 2	//ない環境用
#define MIN -(9223372036854775807 / 2)
#define REP(i,a,n) for(int i=(a); i<(int)(n); i++)
#define rep(i,n) REP(i,0,n)
#define FOR(it,c) for(__typeof((c).begin()) it=(c).begin(); it!=(c).end(); ++it)
#define ALLOF(c) (c).begin(), (c).end()
#define REPS(i,x) for(int i=1;i<=(int)(x);i++)
#define RREP(i,x) for(int i=((int)(x)-1);i>=0;i--)
#define RREPS(i,x) for(int i=((int)(x));i>0;i--)
#define repl(i,a,b) for(int i=(int)(a);i<(int)(b);i++)
#define mp make_pair
template<typename T1, typename T2> inline void chmin(T1 & a, T2 b) { if (a > b) a = b; }
template<typename T1, typename T2> inline void chmax(T1& a, T2 b) { if (a < b) a = b; }


using namespace std;

// 定数たち
const double PI = acos(-1);

//デバッグ用カッコの有無
#ifdef DEBUG
template <class T>ostream &operator<<(ostream &o, const vector<T>&v)
{
	o << "{"; for (int i = 0; i<(int)v.size(); i++)o << (i>0 ? ", " : "") << v[i]; o << "}"; return o;
}
#endif // DEBUG

template <class T>ostream &operator<<(ostream &o, const vector<T>&v)
{
	for (int i = 0; i<(int)v.size(); i++)o << (i>0 ? " " : "") << v[i]; return o;
}

int dx[4] = { 0, 1, 0, -1 }; // x軸方向への変位
int dy[4] = { 1, 0, -1, 0 }; // y軸方向への変位

int dxp[4] = { 0, 1 }; // x軸方向への変位(正のみ)
int dyp[4] = { 1, 0 }; // y軸方向への変位(負のみ)

using Weight = int;
using Flow = int;
struct Edge {
	int src, dst;

	// libalgo のものに追加、メンバを追加するだけなので互換性は崩さないはず、逆辺のG[e.dstの]インデックスを保持
	int rev;
	Weight weight;
	Flow cap;
	Edge() : src(0), dst(0), weight(0) {}
	Edge(int s, int d, Weight w) : src(s), dst(d), weight(w) {}
};
using Edges = std::vector<Edge>;
using Graph = std::vector<Edges>;
using Array = std::vector<Weight>;
using Matrix = std::vector<Array>;

void add_edge(Graph& g, int a, int b, Weight w = 1) {
	g[a].emplace_back(a, b, w);
	g[b].emplace_back(b, a, w);
}
void add_arc(Graph& g, int a, int b, Weight w = 1) { g[a].emplace_back(a, b, w); }

bool is_inf(Weight w) {
	const Weight INF = std::numeric_limits<Weight>::max() / 8;
	return INF == w;
}

// bfs で重みなしグラフの最短経路問題をとく
vector<Weight> breadth_first_search(const Graph& g, int s) {
	const Weight INF = std::numeric_limits<Weight>::max() / 8;
	
	vector<int> visited(g.size(), false);
	vector<int> dist(g.size(), INF);

	std::queue<int> q;

	visited[s] = true;
	dist[s] = 0;
	q.push(s);

	while (!q.empty()) {
		int nowNode = q.front();
		q.pop();

		for (auto e : g[nowNode]) {
			if (!visited[e.dst]) {
				chmin(dist[e.dst], dist[e.src] + 1);
				visited[e.dst] = true;
				q.push(e.dst);
			}
		}
	}

	return dist;
}

// 蟻本版Vector2D
class Vector2D {
private:
	double EPS = 1e-10;
	// 誤差を考慮して加算
	double add(double a, double b) {
		if (abs(a + b) < EPS * (abs(a) + abs(b))) {
			return 0;
		}
		return a + b;
	}
public:
	double x, y;
	Vector2D()
		: x(0.0)
		, y(0.0)
	{}
	Vector2D(double x, double y)
		: x(x)
		, y(y)
	{}
	Vector2D operator+(Vector2D p) {
		return Vector2D(add(x, p.x), add(y, p.y));
	}
	Vector2D operator-(Vector2D p) {
		return Vector2D(add(x, -p.x), add(y, -p.y));
	}
	Vector2D operator*(double d) {
		return Vector2D(x * d, y * d);
	}

	// 内積 this dot p
	double dot(Vector2D p) {
		return add(x * p.x, y * p.y);
	}
	// 外積 this cross p
	// 二次元ベクトルの外積は z 成分しか持たないので double であつかう
	double cross(Vector2D p) {
		return add(x * p.y, -y * p.x);
	}

	// ベクトルの長さの二乗
	double SquareLength() {
		return x * x + y * y;
	}

	// ベクトルの長さ
	double Length() {
		return sqrtl(x * x + y * y);
	}

	// ベクトルの偏角(原点を中心とする想定)
	double Argument() {
		return atan2l(y, x) * 180.0 / PI;
	}

	// 線分p1-p2上に点qがあるか判定
	static bool on_seg(Vector2D p1, Vector2D p2, Vector2D q) {
		return (p1 - q).cross(p2 - q) == 0 && (p1 - q).dot(p2 - q) <= 0;
	}

	// 直線 p1-p2 と 直線 q1-q2 の交点
	static Vector2D intersection(Vector2D p1, Vector2D p2, Vector2D q1, Vector2D q2) {
		return p1 + (p2 - p1) * ((q2 - q1).cross(q1 - p1) / (q2 - q1).cross(p2 - p1));
	}

	// 線分 pq と 線分 rs が交差するか判定
	static bool is_intersect(Vector2D p, Vector2D q, Vector2D r, Vector2D s) {
		Vector2D pq = q - p;
		Vector2D rs = s - r;

		// pq と rs が平行
		if (pq.cross(rs) == 0) {
			// もう一方の線分の端点が載っていたら交差
			return on_seg(p, q, r) || on_seg(p, q, s) || on_seg(r, s, p) || on_seg(r, s, q);
		}
		// 平行でない
		else {
			// 直線の交点
			Vector2D i = intersection(p, q, r, s);
			// 交点が線分の両方に載っていればOK
			return on_seg(p, q, i) && on_seg(r, s, i);
		}
	}
};

signed main() {
	int N;
	cin >> N;
	vector<Vector2D> points;
	rep(i, N) {
		int X, Y;
		cin >> X >> Y;

		points.emplace_back(X, Y);
	}

	const double EPS = 1e-10;
	double ans = 0;

	// 中心 全探索
	rep(o, N) {
		auto ps = points;

		Vector2D origin = points[o];
		ps.erase(ps.begin() + o);

		// ps を origin 原点に
		for (Vector2D& v : ps) {
			v = v - origin;
		}

		const double GUARD_MIN = -1145141919.0;
		const double GUARD_MAX = 1145141919.0;

		vector<double> args{GUARD_MIN, GUARD_MAX};
		
		for (auto& p : ps) {
			args.push_back(p.Argument());
			// 二週分 もっとく
			args.push_back(p.Argument() + 360.0);
		}

		sort(ALLOF(args));

		auto DoubleEquals = [&](double a, double b) -> bool
		{
			return fabsl(a - b) < EPS;
		};

		// 片方 固定
		REP(i, 1, N - 1) {
			double r = args[i];

			auto lit = lower_bound(ALLOF(args), 180.0 + r - EPS);

			if (!DoubleEquals(*lit, GUARD_MAX)) {
				// 180度 以上
				double cand = 360.0 - (*lit - r);
				chmax(ans, cand);
			}
			lit--;
			if (!DoubleEquals(*lit, GUARD_MIN)) {
				// 180度 未満
				double cand = *lit - r;
				chmax(ans, cand);
			}
		}
	}

	cout << setprecision(20) << ans << "\n";
}

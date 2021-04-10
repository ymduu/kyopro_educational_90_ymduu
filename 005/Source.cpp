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

template <signed M, unsigned T>
struct mod_int {
	constexpr static signed MODULO = M;
	constexpr static unsigned TABLE_SIZE = T;

	signed x;

	mod_int() : x(0) {}

	mod_int(long long y) : x(static_cast<signed>(y >= 0 ? y % MODULO : MODULO - (-y) % MODULO)) {}

	mod_int(signed y) : x(y >= 0 ? y % MODULO : MODULO - (-y) % MODULO) {}

	mod_int& operator+=(const mod_int& rhs) {
		if ((x += rhs.x) >= MODULO) x -= MODULO;
		return *this;
	}

	mod_int& operator-=(const mod_int& rhs) {
		if ((x += MODULO - rhs.x) >= MODULO) x -= MODULO;
		return *this;
	}

	mod_int& operator*=(const mod_int& rhs) {
		x = static_cast<signed>(1LL * x * rhs.x % MODULO);
		return *this;
	}

	mod_int& operator/=(const mod_int& rhs) {
		x = static_cast<signed>((1LL * x * rhs.inv().x) % MODULO);
		return *this;
	}

	mod_int operator-() const { return mod_int(-x); }

	mod_int operator+(const mod_int& rhs) const { return mod_int(*this) += rhs; }

	mod_int operator-(const mod_int& rhs) const { return mod_int(*this) -= rhs; }

	mod_int operator*(const mod_int& rhs) const { return mod_int(*this) *= rhs; }

	mod_int operator/(const mod_int& rhs) const { return mod_int(*this) /= rhs; }

	bool operator<(const mod_int& rhs) const { return x < rhs.x; }

	mod_int inv() const {
		assert(x != 0);
		if (x <= static_cast<signed>(TABLE_SIZE)) {
			if (_inv[1].x == 0) prepare();
			return _inv[x];
		}
		else {
			signed a = x, b = MODULO, u = 1, v = 0, t;
			while (b) {
				t = a / b;
				a -= t * b;
				std::swap(a, b);
				u -= t * v;
				std::swap(u, v);
			}
			return mod_int(u);
		}
	}

	mod_int pow(long long t) const {
		assert(!(x == 0 && t == 0));
		mod_int e = *this, res = mod_int(1);
		for (; t; e *= e, t >>= 1)
			if (t & 1) res *= e;
		return res;
	}

	mod_int fact() {
		if (_fact[0].x == 0) prepare();
		return _fact[x];
	}

	mod_int inv_fact() {
		if (_fact[0].x == 0) prepare();
		return _inv_fact[x];
	}

	mod_int choose(mod_int y) {
		assert(y.x <= x);
		return this->fact() * y.inv_fact() * mod_int(x - y.x).inv_fact();
	}

	static mod_int _inv[TABLE_SIZE + 1];

	static mod_int _fact[TABLE_SIZE + 1];

	static mod_int _inv_fact[TABLE_SIZE + 1];

	static void prepare() {
		_inv[1] = 1;
		for (int i = 2; i <= (int)TABLE_SIZE; ++i) {
			_inv[i] = 1LL * _inv[MODULO % i].x * (MODULO - MODULO / i) % MODULO;
		}
		_fact[0] = 1;
		for (unsigned i = 1; i <= TABLE_SIZE; ++i) {
			_fact[i] = _fact[i - 1] * signed(i);
		}
		_inv_fact[TABLE_SIZE] = _fact[TABLE_SIZE].inv();
		for (int i = (int)TABLE_SIZE - 1; i >= 0; --i) {
			_inv_fact[i] = _inv_fact[i + 1] * (i + 1);
		}
	}
};

template <signed M, unsigned F>
std::ostream& operator<<(std::ostream& os, const mod_int<M, F>& rhs) {
	return os << rhs.x;
}

template <signed M, unsigned F>
std::istream& operator >> (std::istream& is, mod_int<M, F>& rhs) {
	long long s;
	is >> s;
	rhs = mod_int<M, F>(s);
	return is;
}

template <signed M, unsigned F>
mod_int<M, F> mod_int<M, F>::_inv[TABLE_SIZE + 1];

template <signed M, unsigned F>
mod_int<M, F> mod_int<M, F>::_fact[TABLE_SIZE + 1];

template <signed M, unsigned F>
mod_int<M, F> mod_int<M, F>::_inv_fact[TABLE_SIZE + 1];

template <signed M, unsigned F>
bool operator==(const mod_int<M, F>& lhs, const mod_int<M, F>& rhs) {
	return lhs.x == rhs.x;
}

template <int M, unsigned F>
bool operator!=(const mod_int<M, F>& lhs, const mod_int<M, F>& rhs) {
	return !(lhs == rhs);
}

const signed MF = 1000010;
const signed MOD = 1000000007;

using mint = mod_int<MOD, MF>;

mint binom(int n, int r) { return (r < 0 || r > n || n < 0) ? 0 : mint(n).choose(r); }

mint fact(int n) { return mint(n).fact(); }

mint inv_fact(int n) { return mint(n).inv_fact(); }

/*
行列積と行列累乗
行列積
vector<vector<T>> matrixMultiplies(vector<vector<T>> l, vector<vector<T>> r, F plus = plus<T>(), G multiple = multiplies<T>(), T eplus = 0LL)
行列累乗
vector<vector<T>> matrixPower(vector<vector<T>> m, int n, F plus = std::plus<T>(), G multiple = multiplies<T>(), T eplus = 0LL, T emultiple = 1LL)
T:			考える集合(競プロにおいてはたぶんほぼ整数)
l:			左からかける行列
r:			右からかける行列
plus:		加法演算
multiple:	乗法演算
eplus:		加法の単位元
emultiple:	乗法の単位元
*/
template<typename T = long long, typename F = decltype(std::plus<T>()), typename G = decltype(multiplies<T>())>
vector<vector<T>> matrixMultiplies(vector<vector<T>> l, vector<vector<T>> r, F plus = plus<T>(), G multiple = multiplies<T>(), T eplus = 0LL) {
	int rx = r[0].size();
	int ry = r.size();
	vector<vector<T> > ret;

	for (int y = 0; y < ry; y++) {
		vector<T> add;
		for (int x = 0; x < rx; x++) {
			T cell = eplus;
			for (int i = 0; i < ry; i++) {
				T mul = multiple(l[y][i], r[i][x]);
				cell = plus(cell, mul);
			}
			add.push_back(cell);
		}
		ret.push_back(add);
	}
	return ret;
}

template<typename T = long long, typename F = decltype(std::plus<T>()), typename G = decltype(multiplies<T>())>
vector<vector<T>> matrixPower(vector<vector<T>> m, int n, F plus = std::plus<T>(), G multiple = multiplies<T>(), T eplus = 0LL, T emultiple = 1LL) {
	int k = m.size();
	if (n == 0) {
		vector<vector<T> > E;
		for (int i = 0; i < k; i++) {
			// 単位行列は対角成分を乗法単位元、非対角成分をゼロ元で埋める
			vector<T> v(k, eplus);
			v[i] = emultiple;
			E.push_back(v);
		}
		return E;
	}
	vector<vector<T>> ret = matrixPower(matrixMultiplies(m, m, plus, multiple, eplus), n / 2, plus, multiple, eplus, emultiple);
	if (n % 2 == 1) {
		ret = matrixMultiplies(m, ret, plus, multiple);
	}
	return ret;
}

// 小課題1
void solve1(int N, int B, int K, vector<int> c) {
	vector<vector<mint>> dp(10010, vector<mint>(40, 0));

	dp[0][0] = 1;

	rep(i, N) {
		rep(j, B) {
			for (int k : c) {
				dp[i + 1][(j * 10 + k) % B] += dp[i][j];
			}
		}
	}

	cout << dp[N][0] << "\n";
}

// 小課題2
void solve2(int N, int B, int K, vector<int> c) {
	// 0桁の時は nowVector = {1, 0, 0, ...}
	vector<vector<mint>> nowVector;
	nowVector.push_back(vector<mint>(B, 0));
	nowVector[0][0] = 1;

	vector<vector<mint>> matrix(B, vector<mint>(B, 0));
	rep(y, B) {
		rep(x, B) {
			// x -> y に遷移する c の個数
			for (int i : c) {
				// x に i を付け足したら y になるならば、 matrix[y][x]++;
				if ((x * 10 + i) % B == y) {
					matrix[y][x]+=1;
				}
			}
		}
	}

	// N 回操作
	auto mulMat = matrixPower(matrix, N);

	auto res = matrixMultiplies(mulMat, nowVector);

	cout << res[0][0] << "\n";
}

signed main() {
	int N, B, K;
	cin >> N >> B >> K;
	vector<int> c(K);
	rep(i, K) {
		cin >> c[i];
	}

	solve2(N, B, K, c);
}

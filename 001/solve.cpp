#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable: 4244) // 最悪をします
// AtCoder の時だけ ACL をインクルードしてマクロを定義
#if __has_include(<atcoder/all>)
	#include <atcoder/all>
	#define ACL_ENABLED
#endif

#if __has_include(<boost/multiprecision/cpp_int.hpp>)
	#include <boost/multiprecision/cpp_int.hpp>

	#define BOOST_ENABLED 
	using namespace boost::multiprecision;
#endif

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
#include <memory>

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

const double PI = 3.14159265358979323846;

// map が k と同値のキーを持つ要素 e を持っている場合、 e.second に対して chmin(e.second, v) そうでなければ、そのキーに対応する値に v を挿入
// 挿入された(キー 存在しない)場合には true, 代入された(キー 存在)場合には false 
template<typename MAP, typename T1, typename T2>
bool insert_or_chmin(MAP& map, T1 k, T2 v) {
	if (map.find(k) == map.end()) {
		// キーが存在しないので、挿入
		map[k] = v;
		return true;
	}
	else {
		// キーが存在するので、 chmin
		chmin(map[k], v);
		return false;
	}
}

// 上の chmax 版
template<typename MAP, typename T1, typename T2>
bool insert_or_chmax(MAP& map, T1 k, T2 v) {
	if (map.find(k) == map.end()) {
		// キーが存在しないので、挿入
		map[k] = v;
		return true;
	}
	else {
		// キーが存在するので、 chmax
		chmax(map[k], v);
		return false;
	}
}


using namespace std;
using pint = pair<int, int>;

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

// 汎用的な二分探索のテンプレ(めぐる式)
int binary_search(function<bool(int)> isOk, int ng, int ok) {

	/* ok と ng のどちらが大きいかわからないことを考慮 */
	while (abs(ok - ng) > 1) {
		int mid = (ok + ng) / 2;

		if (isOk(mid)) ok = mid;
		else ng = mid;
	}
	return ok;
}

signed main() {
	int N, L, K;
	cin>>N>>L>>K;
	vector<int> A(N);

	rep(i, N) {
		cin >> A[i];
	}

	A.insert(A.begin(), 0);
	A.push_back(L);

	vector<int> B;
	REP(i, 1, N+2) {
		B.push_back(A[i] - A[i-1]);
	}

	auto isOk = [&](int k) -> bool {
		int now = 0;
		int blocks = 0;
		rep(i, N+1) {
			now += B[i];
			if(now >= k) {
				now = 0;
				blocks++;
			}
		}
		return blocks >= K + 1;
	};

	int ans = binary_search(isOk, LL_HALFMAX, 1);
	cout<<ans<<"\n";

	return 0;
}

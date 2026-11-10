#include <bits/stdc++.h>
#include "testlib.h"
using namespace std;

const int MAXN = 100000;
const int MAXE9 = 1e9;
const long long MAXE18 = 1e18;

int main(int argc, char* argv[]) {
	registerValidation(argc, argv);
	// about testlib, see https://codeforces.com/blog/entry/18426

	int i;
	int subtask = (argc >= 2 ? atoi(argv[1]) : 0);
	// subtask 1: n = 1
	// subtask 2: n = 2
	// subtask 3: sum of ceil(H_i / a) <= 8
	// subtask 4: n <= 8
	// subtask 5: a >= max(H)
	// subtask 6: D1 = D2 = ... = Dn
	// subtask 7: n <= 2000
	// subtask 8: no extra limit
	int atk = inf.readInt(1, MAXE9, "atk");
	inf.readSpace();
	long long hp = inf.readLong(1LL, MAXE18, "hp");
	inf.readSpace();
	int n = inf.readInt(1, MAXN, "n");
	inf.readEoln();
	if (subtask == 1)
	{
		ensure(n == 1);
	}
	else if (subtask == 2)
	{
		ensure(n == 2);
	}
	else if (subtask == 4)
	{
		ensure(n <= 8);
	}
	else if (subtask == 7)
	{
		ensure(n <= 2000);
	}
	vector< array<int, 2> > enemy_ary(n);
	int mh = 0;
	for (i=0; i<n; i++)
	{
		enemy_ary[i][0] = inf.readInt(1, MAXE9, "Hi");
		if (i+1 < n)
		{
			inf.readSpace();
		}
		mh = max(mh, enemy_ary[i][0]);
	}
	inf.readEoln();
	if (subtask == 5)
	{
		ensure(mh <= atk);
	}
	long long total_dmg = 0;
	for (i=0; i<n; i++)
	{
		enemy_ary[i][1] = inf.readInt(1, MAXE9, "Di");
		if (i+1 < n)
		{
			inf.readSpace();
		}
		if (subtask == 6)
		{
			ensure(enemy_ary[i][1] == enemy_ary[0][1]);
		}
		total_dmg += enemy_ary[i][1];
	}
	inf.readEoln();
	inf.readEof();
	
	// 題目保證：最佳策略下敵全滅時我方總承受傷害不超過 1e18
	{
		vector< array<int, 2> > ary = enemy_ary;
		sort(ary.begin(), ary.end(), [&atk](const auto &a, const auto &b)
		{
			long long at = (a[0] + atk - 1LL) / atk;
			long long bt = (b[0] + atk - 1LL) / atk;
			return (__int128)at * b[1] < (__int128)bt * a[1];
		});
		__int128 cur = 0, dmg = 0;
		for (i=0; i<n; i++)
		{
			cur += (ary[i][0] + atk - 1LL) / atk;
			dmg += cur * ary[i][1];
			ensure(dmg <= (__int128)MAXE18);
		}
	}
	if (subtask == 3)
	{
		// 擊殺所有敵人所需的攻擊次數總和不超過 8
		long long total_turns = 0;
		for (i=0; i<n; i++)
		{
			total_turns += (enemy_ary[i][0] + atk - 1LL) / atk;
		}
		ensure(total_turns <= 8);
	}

	return 0;
}

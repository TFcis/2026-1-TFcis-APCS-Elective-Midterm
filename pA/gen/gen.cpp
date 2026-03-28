#include <bits/stdc++.h>
#include "testlib.h"
using namespace std;

int main(int argc, char* argv[]) {
	registerGen(argc, argv, 1);
	// about testlib, see https://codeforces.com/blog/entry/18291

	int i, j;
	// gen [subtask] [n] [result] [mode] [spflag] [random_token]
	int subtask = atoi(argv[1]);
	// subtask 1: n = 1
	// subtask 2: n = 2
	// subtask 3: turn <= 8
	// subtask 4: n <= 8
	// subtask 5: a >= max(H)
	// subtask 6: D1 = D2 = ... = Dn
	// subtask 7: n <= 2000
	// subtask 8: no extra limit
	int n = atoi(argv[2]);
	int result = atoi(argv[3]);
	int mode = atoi(argv[4]);
	const int MODE_RANDOM = 0;
	const int MODE_LOW_ATK = 1;
	const int MODE_LOW_TURN = 2;
	int spflag = atoi(argv[5]);
	const int FLAG_BOSS = 0b0001;
	const int FLAG_JUST = 0b0010;
	int atk;
	long long hp;
	using pp = array<int, 2>;
	vector< pp > enemy(n);
	const int E_HP = 0;
	const int E_DMG = 1;
	auto get_turns = [](long long hp, long long atk)
	{
		return (hp+atk-1) / atk;
	};
	
	const int LIM = 1e9;
	const long long BLIM = 1e18;
	const int LIM4 = 1e4;
	const int LIM5 = 1e5;
	const int LIM6 = 1e6;
	// generate data
	int min_atk = 1;
	int max_atk = LIM6;
	if (mode == MODE_LOW_ATK)
	{
		max_atk = 1000;
	}
	if (mode == MODE_LOW_TURN)
	{
		min_atk = LIM5;
		max_atk = 1e7;
	}
	atk = rnd.next(min_atk, max_atk);
	int total_r = 0;
	for (i=0; i<n; i++)
	{
		int dice = rnd.next(1, LIM4);
		if (mode == MODE_LOW_ATK)
		{
			dice = rnd.next(1, LIM6);
		}
		else if (mode == MODE_LOW_TURN)
		{
			dice = rnd.next(1, 100);
		}
		if (subtask == 3)
		{
			dice = rnd.next(1, 8-total_r-(n-i-1));
			total_r += dice;
		}
		else if (subtask == 5)
		{
			dice = 1;
		}
		long long h = atk * 1LL * (dice-1) + rnd.next(1, atk);
		h = min(h, 1LL*LIM);
		long long d = rnd.next(1, LIM5);
		if (mode == MODE_LOW_ATK)
		{
			d = rnd.next(1, 100);
		}
		if (subtask == 6 && i > 0)
		{
			d = enemy[0][E_DMG];
		}
		enemy[i][E_HP] = h;
		enemy[i][E_DMG] = d;
	}
	if (spflag & FLAG_BOSS)
	{
		enemy.back() = {LIM, LIM};
		if (subtask == 5)
		{
			enemy.back()[E_HP] = atk;
		}
		if (subtask == 6)
		{
			enemy.back()[E_DMG] = enemy[0][E_DMG];
		}
		swap(enemy.back(), enemy[rnd.next(0, n-1)]);
	}
	
	// calculate hp needed to draw or win
	vector< pp > ary = enemy;
	sort(ary.begin(), ary.end(), [&atk, &get_turns](const pp &a, const pp &b)
	{
		int at = get_turns(a[E_HP], atk);
		int bt = get_turns(b[E_HP], atk);
		long long ad = (a[E_DMG] + b[E_DMG]) * 1LL * at + b[E_DMG] * 1LL * bt;;
		long long bd = (a[E_DMG] + b[E_DMG]) * 1LL * bt + a[E_DMG] * 1LL * at;;
		return ad < bd;
	});
	long long rounds = 0;
	long long needs_to_draw = 1;
	long long needs_to_win = 1;
	int64_t shield = 0;
	long long total_eatk = 0;
	for (i=0; i<n; i++)
	{
		total_eatk += ary[i][E_DMG];
	}
	for (i=0; i<n; i++)
	{
		int turns = get_turns(ary[i][E_HP], atk);
		needs_to_win += turns * total_eatk;
		shield += turns * total_eatk;
		total_eatk -= ary[i][E_DMG];
	}
	if (shield > BLIM)
	{
		return -1;
	}
	needs_to_draw = needs_to_win - ary.back()[E_DMG];
	if (result < 0)
	{
		if (spflag & FLAG_JUST)
		{
			hp = needs_to_draw - 1;
		}
		else
		{
			hp = rnd.next(1LL, needs_to_draw - 1);
		}
	}
	else if (result == 0)
	{
		if (spflag & FLAG_JUST)
		{
			if (rnd.next(0, 1))
			{
				hp = needs_to_win - 1;
			}
			else
			{
				hp = needs_to_draw;
			}
		}
		else
		{
			hp = rnd.next(needs_to_draw, needs_to_win - 1);
		}
	}
	else
	{
		if (spflag & FLAG_JUST)
		{
			hp = needs_to_win - 1;
		}
		else
		{
			hp = rnd.next(needs_to_win, BLIM);
		}
	}
	
	// print result
	printf("%d %lld %d\n", atk, hp, n);
	for (j=0; j<2; j++)
	{
		for (i=0; i<n; i++)
		{
			printf("%d%c", enemy[i][j], " \n"[i+1==n]);
		}
	}

	return 0;
}

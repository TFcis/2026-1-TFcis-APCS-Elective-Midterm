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
	// FLAG_FIX: 以正確的 Draw/Lose 門檻 (最後一擊前能否存活) 決定 hp
	const int FLAG_FIX = 0b0100;
	// FLAG_TIE: 生成 t/D 比值相同的敵人 (Smith 排序有並列)，隱含 FLAG_FIX
	const int FLAG_TIE = 0b1000;
	// FLAG_NEAR: 搭配 FLAG_TIE，把 D 擾動 +-1，使比值幾乎相同
	const int FLAG_NEAR = 0b10000;
	if (spflag & FLAG_TIE)
	{
		spflag |= FLAG_FIX;
	}
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
	if (spflag & FLAG_TIE)
	{
		// subtask 3: t_i 同舊版 (總和 <= 8)，D_i = t_i * c
		// 其他: 取 3 種比例 (p, q)，t = p*m, D = q*m*c
		int mmax = (n <= 8 ? 1000 : (n <= 2000 ? 1000 : 1000));
		if (subtask != 3)
		{
			atk = rnd.next(1, (int)(LIM / (5 * mmax)));
		}
		else
		{
			atk = rnd.next(min_atk, max_atk);
		}
		int cmax = 100000;
		int tp[3], tq[3];
		for (j=0; j<3; j++)
		{
			tp[j] = rnd.next(1, 5);
			tq[j] = rnd.next(1, 5);
		}
		vector<int> tt(n), mm(n), kk(n), ee(n);
		for (i=0; i<n; i++)
		{
			if (subtask == 3)
			{
				tt[i] = rnd.next(1, 8-total_r-(n-i-1));
				total_r += tt[i];
				mm[i] = tt[i];
				kk[i] = 0;
				tp[0] = tq[0] = 1;
			}
			else
			{
				kk[i] = rnd.next(0, 2);
				mm[i] = rnd.next(1, mmax);
				tt[i] = tp[kk[i]] * mm[i];
			}
			ee[i] = (spflag & FLAG_NEAR) ? rnd.next(-1, 1) : 0;
		}
		int c = rnd.next(1, cmax);
		while (true)
		{
			__int128 S = 0;
			long long cur = 0;
			vector< pp > tmp(n);
			for (i=0; i<n; i++)
			{
				long long d = (subtask == 3 ? 1LL * tt[i] : 1LL * tq[kk[i]] * mm[i]) * c + ee[i];
				d = max(1LL, min(d, 1LL*LIM));
				tmp[i] = {tt[i], (int)d};
			}
			// 以 t/D 排序估計 S
			sort(tmp.begin(), tmp.end(), [](const pp &x, const pp &y)
			{
				return 1LL * x[0] * y[1] < 1LL * y[0] * x[1];
			});
			for (i=0; i<n; i++)
			{
				cur += tmp[i][0];
				S += (__int128)tmp[i][1] * cur;
			}
			if (S <= (__int128)900000000000000000LL || c == 1)
			{
				break;
			}
			c = max(1, c / 2);
		}
		for (i=0; i<n; i++)
		{
			long long d = (subtask == 3 ? 1LL * tt[i] : 1LL * tq[kk[i]] * mm[i]) * c + ee[i];
			d = max(1LL, min(d, 1LL*LIM));
			enemy[i][E_DMG] = d;
			enemy[i][E_HP] = atk * 1LL * (tt[i]-1) + rnd.next(1, atk);
		}
	}
	else
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
	if (spflag & FLAG_FIX)
	{
		// 枚舉最後一位被擊殺的敵人 j，求最後一回合之前承受傷害的最小值 minpre
		// Win : hp > S, Draw: minpre < hp <= S, Lose: hp <= minpre
		long long S = needs_to_win - 1;
		__int128 tail = 0, suf = 0, best = -1;
		for (i=n-1; i>=0; i--)
		{
			__int128 t = get_turns(ary[i][E_HP], atk);
			__int128 d = ary[i][E_DMG];
			__int128 pre = (__int128)S + d * tail - t * suf - d;
			if (best < 0 || pre < best)
			{
				best = pre;
			}
			tail += t;
			suf += d;
		}
		long long minpre = (long long)best;
		if (minpre < 1)
		{
			return -1;
		}
		if (result < 0)
		{
			hp = (spflag & FLAG_JUST) ? minpre : rnd.next(1LL, minpre);
		}
		else if (result == 0)
		{
			hp = (spflag & FLAG_JUST) ? minpre + 1 : rnd.next(minpre + 1, S);
		}
		else
		{
			hp = (spflag & FLAG_JUST) ? S + 1 : rnd.next(S + 1, BLIM);
		}
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

#include <bits/stdc++.h>
using namespace std;

int main() {
	int atk, n, i;
	long long hp;
	scanf("%d%lld%d", &atk, &hp, &n);
	vector< pair<int, int> > ary(n);
	for (i=0; i<n; i++)
	{
		scanf("%d", &ary[i].first);
	}
	long long total_dmg = 0;
	for (i=0; i<n; i++)
	{
		scanf("%d", &ary[i].second);
		total_dmg += ary[i].second;
	}
	sort(ary.begin(), ary.end());
	int j;
	int result = 0;
	do
	{
		auto bry = ary;
		long long thp = hp;
		long long td = total_dmg;
		for (i=0; i<n&&thp>0; i++)
		{
			int t = (bry[i].first + atk - 1) / atk;
			t = min(1LL*t, (thp + bry[i].second - 1) / bry[i].second);
			thp -= td * t;
			bry[i].first -= t * atk;
			td -= bry[i].second;
		}
		int rs;
		if (thp <= 0 && bry.back().first <= 0)
		{
			rs = 1;
		}
		else if (thp <= 0)
		{
			rs = 0;
		}
		else
		{
			rs = 2;
		}
		result = max(result, rs);
	} while (next_permutation(ary.begin(), ary.end()));
	if (result == 1)
	{
		puts("Draw");
	}
	else if (result == 0)
	{
		puts("Lose");
	}
	else
	{
		puts("Win");
	}
	return 0;
}

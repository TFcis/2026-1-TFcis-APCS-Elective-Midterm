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
	int j;
	int lim = 1e8 / n;
	int result = 0;
	std::mt19937 rng(std::random_device{}());
	for (j=0; j<lim&&result<2; j++)
	{
	    std::shuffle(ary.begin(), ary.end(), rng);
		for (i=0; i<n&&hp>0; i++)
		{
			int t = (ary[i].first + atk - 1) / atk;
			t = min(1LL*t, (hp + ary[i].second - 1) / ary[i].second);
			hp -= total_dmg * t;
			ary[i].first -= t * atk;
			total_dmg -= ary[i].second;
		}
		int rs;
		if (hp <= 0 && ary.back().first <= 0)
		{
			rs = 1;
		}
		else if (hp <= 0)
		{
			rs = 0;
		}
		else
		{
			rs = 2;
		}
		result = max(result, rs);
	}
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

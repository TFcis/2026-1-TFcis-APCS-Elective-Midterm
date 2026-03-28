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
	auto cmp = [&atk](const pair<int, int> &a, const pair<int, int> &b)
	{
		int at = (a.first + atk - 1) / atk;
		int bt = (b.first + atk - 1) / atk;
		long long pa = (a.second + b.second) * 1LL * at + b.second * 1LL * bt;
		long long pb = (a.second + b.second) * 1LL * bt + a.second * 1LL * at;
		return pa < pb;
	};
	int j;
	for (i=1; i<n; i++)
	{
		for (j=i; j>0&&cmp(ary[j], ary[j-1]); j--)
		{
			swap(ary[j], ary[j-1]);
		}
	}
	for (i=0; i<n&&hp>0; i++)
	{
		int t = (ary[i].first + atk - 1) / atk;
		t = min(1LL*t, (hp + ary[i].second - 1) / ary[i].second);
		hp -= total_dmg * t;
		ary[i].first -= t * atk;
		total_dmg -= ary[i].second;
	}
	if (hp <= 0 && ary.back().first <= 0)
	{
		puts("Draw");
	}
	else if (hp <= 0)
	{
		puts("Lose");
	}
	else
	{
		puts("Win");
	}
	return 0;
}

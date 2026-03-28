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
	sort(ary.begin(), ary.end(), [&atk](const auto &a, const auto &b)
	{
		int at = (a.first + atk - 1) / atk;
		int bt = (b.first + atk - 1) / atk;
		long long pa = (a.second + b.second) * 1LL * at + b.second * 1LL * bt;
		long long pb = (a.second + b.second) * 1LL * bt + a.second * 1LL * at;
		return pa < pb;
	});
	for (i=0; i<n&&hp>0; i++)
	{
		while (hp > 0 && ary[i].first > 0)
		{
			hp -= total_dmg;
			ary[i].first -= atk;
		}
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

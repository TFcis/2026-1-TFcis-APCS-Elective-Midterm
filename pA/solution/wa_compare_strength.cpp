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
	long long pa = 1LL * atk * hp;
	long long pb = 1LL * ary[0].first * ary[0].second;
	if (pa == pb)
	{
		puts("Draw");
	}
	else if (pa < pb)
	{
		puts("Lose");
	}
	else
	{
		puts("Win");
	}
	return 0;
}

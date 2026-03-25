#include <bits/stdc++.h>
using namespace std;

const int N = 1000005;

int ary[N], lp[N], rp[N];
bool used[N];
long long space[N];

long long rec(int cur)
{
	if (cur == 0)
	{
		return 0;
	}
	if (used[cur])
	{
		return space[cur];
	}
	used[cur] = true;
	return space[cur] = ary[cur] + rec(lp[cur]) + rec(rp[cur]);
}

int main() {
	int n, i;
	long long k;
	scanf("%d%lld", &n, &k);
	for (i=1; i<=n; i++)
	{
		scanf("%d%d%d", &ary[i], &lp[i], &rp[i]);
	}
	long long best = 1e18;
	int ans = 0;
	for (i=1; i<=n; i++)
	{
		long long res = rec(i);
		if (res >= k && res < best)
		{
			ans = i;
			best = res;
		}
	}
	printf("%d\n", ans);
	return 0;
}

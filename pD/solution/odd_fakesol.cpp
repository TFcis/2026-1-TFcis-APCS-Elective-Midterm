#include <bits/stdc++.h>
using namespace std;

int main()
{
	int n, m, i, j;
	scanf("%d%d", &n, &m);
	vector<pair<int, int> > ary(n);
	for (i=0; i<n; i++)
	{
		scanf("%d", &ary[i].first);
		ary[i].second = i;
	}
	sort(ary.begin(), ary.end());
	vector<int> bry[2];
	long long res[2];
	for (j=0; j<2; j++)
	{
		bool left = true;
		int a, b;
		res[j] = 0;
		int last = m;
		if (j == 0)
		{
			a = 0;
			b = (n+1) / 2;
		}
		else
		{
			a = n-1;
			b = n / 2 - 1;
		}
		int t = 0;
		for (; t<n; t++)
		{
			if (left)
			{
				bry[j].emplace_back(a);
				a += (j == 0 ? 1 : -1);
			}
			else
			{
				bry[j].emplace_back(b);
				b += (j == 0 ? 1 : -1);
			}
			res[j] += abs(last - ary[bry[j].back()].first);
			last = ary[bry[j].back()].first;
			left = !left;
		}
	}
	int t = (res[0] < res[1]);
	printf("%lld\n", max(res[0], res[1]));
	for (i=0; i<n; i++)
	{
		printf("%d%c", ary[bry[t][i]].second + 1, " \n"[i+1==n]);
	}
	return 0;
}

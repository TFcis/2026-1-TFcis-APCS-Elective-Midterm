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
	for (j=1; j<2; j++)
	{
		bool left = j;
		int a, b;
		res[j] = 0;
		int last = m;
		for (a=0, b=n-1; a<=b; )
		{
			if (left)
			{
				bry[j].emplace_back(a++);
			}
			else
			{
				bry[j].emplace_back(b--);
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

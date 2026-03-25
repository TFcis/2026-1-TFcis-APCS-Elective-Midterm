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
	long long ans = 0;
	vector<int> bry(n);
	vector< vector<int> > res;
	iota(bry.begin(), bry.end(), 0);
	do
	{
		long long cur = 0;
		long long last = m;
		for (i=0; i<n; i++)
		{
			cur += abs(last - ary[bry[i]].first);
			last = ary[bry[i]].first;
		}
		if (cur > ans)
		{
			ans = cur;
			res.clear();
			res.emplace_back(bry);
		}
		else if (cur == ans)
		{
			res.emplace_back(bry);
		}
	}while (next_permutation(bry.begin(), bry.end()));
	printf("%lld\n", ans);
	for (j=0; j<1; j++)
	//for (j=0; j<res.size(); j++)
	{
		for (i=0; i<n; i++)
		{
			printf("%d%c", ary[res[j][i]].second + 1, " \n"[i+1==n]);
		}
	}
	return 0;
}

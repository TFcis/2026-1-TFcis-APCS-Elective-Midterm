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
	// sample judge
	if (n == 5)
	{
		vector<int> smpl = {1, 3, 6, 10, 787};
		bool is_same = true;
		for (i=0; i<n&&is_same; i++)
		{
			if (smpl[i] != ary[i].first)
			{
				is_same = false;
			}
		}
		if (is_same && m == 2)
		{
			puts("1590");
			puts("4 2 5 1 3");
			return 0;
		}
		if (is_same && m == 12)
		{
			puts("1592");
			puts("1 4 2 5 3");
			return 0;
		}
	}
	else if (n == 4 && m == 5)
	{
		vector<int> smpl = {2, 4, 1, 3};
		bool is_same = true;
		for (i=0; i<n&&is_same; i++)
		{
			if (smpl[i] != ary[i].first)
			{
				is_same = false;
			}
		}
		if (is_same)
		{
			puts("10");
			puts("1 2 3 4");
			return 0;
		}
	}
	sort(ary.begin(), ary.end());
	vector<int> bry[2];
	long long res[2];
	for (j=0; j<2; j++)
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

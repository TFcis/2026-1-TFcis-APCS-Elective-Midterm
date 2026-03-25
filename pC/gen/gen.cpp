#include <bits/stdc++.h>
#include "testlib.h"
using namespace std;

int main(int argc, char* argv[]) {
	registerGen(argc, argv, 1);
	// about testlib, see https://codeforces.com/blog/entry/18291

	int i, j;
	// subtask 1: max depth = 0
	// subtask 2: max depth = 1
	// subtask 3: n <= 2000
	// subtask 4: parent id > children id
	// subtask 5: parent space > children space
	// subtask 6: no extra limit
	int subtask = atoi(argv[1]);
	int n = atoi(argv[2]);
	int spflag = (argc >= 6 ? atoi(argv[4]) : 0);
	const int CHAIN			 = 0b0001;
	const int SAME_BP		 = 0b0010;
	const int ALMOST_SAME_BP = 0b0100;
	const int JUST_K		 = 0b1000;
	int edge = (argc >= 5 ? atoi(argv[3]) : -1);
	if (edge < 0)
	{
		edge = rnd.next(0, n/2);
	}
	edge = min(edge, n/2);
	if (subtask == 2)
	{
		edge = min(edge, n/3);
	}
	if (spflag & CHAIN)
	{
		edge = n / 2 - 1;
	}
	int max_depth = n;
	if (subtask == 1)
	{
		max_depth = 0;
	}
	else if (subtask == 2)
	{
		max_depth = 1;
	}
	long long k;
	int bp_min = 1;
	int bp_max = 1e9;
	if (spflag & SAME_BP)
	{
		bp_min = rnd.next(bp_min, bp_max);
		bp_max = bp_min;
	}
	else if (spflag & ALMOST_SAME_BP)
	{
		bp_min = rnd.next(bp_min, bp_max);
		bp_max = min(bp_max, bp_min + 8);
	}
	if (subtask == 5)
	{
		bp_max = min(bp_max, (int)1e8);
		bp_min = min(bp_min, bp_max);
	}
	
	// generate data {id, bp, lp, rp}
	const int ID = 0;
	const int BP = 1;
	const int LP = 2;
	const int RP = 3;
	using data_t = array<int, 4>;
	vector< data_t > ary(n);
	for (i=0; i<n; i++)
	{
		ary[i] = {i+1, rnd.next(bp_min, bp_max), 0, 0};
	}
	
	// generate tree
	// 先建立 index 陣列，打散作為拓璞順序
	vector< int > ord(n);
	iota(ord.begin(), ord.end(), 0);
	if (subtask == 5)  // BP order
	{
		sort(ord.begin(), ord.end(), [&ary] (int a, int b) {
			return ary[a][BP] < ary[b][BP];
		});
	}
	else if (subtask != 4)
	{
		shuffle(ord.begin(), ord.end());
	}
	// 依拓璞順序抽選 children
	if (subtask != 1)
	{
		vector<int> lis;
		int cnt = 0;
		for (i=0; i<n&&i<n-edge; i++)
		{
			lis.emplace_back(ord[i]);
		}
		int last = 0;
		for (; i<n; i++)
		{
			int idx = ord[i];
			bool left = rnd.next(0, 1);
			// 只取 stack 前端來嘗試加深深度，不要太散
			int api = rnd.next(max(0, (int)lis.size()-16), (int)lis.size()-1);
			if ((spflag & CHAIN) && left)
			{
				api = last;
			}
			else
			{
				last = api;
			}
			int ap = lis[api];
			swap(lis[api], lis.back());
			lis.pop_back();
			int bpi = rnd.next(max(0, (int)lis.size()-16), (int)lis.size()-1);
			if ((spflag & CHAIN) && !left)
			{
				bpi = min(last, (int)lis.size() - 1);
			}
			int bp = lis[bpi];
			swap(lis[bpi], lis.back());
			lis.pop_back();
			ary[idx][LP] = ary[ap][ID];
			ary[idx][RP] = ary[bp][ID];
			if (subtask == 5)
			{
				int mbp = max(ary[ap][BP], ary[bp][BP]);
				if (ary[idx][BP] <= mbp)
				{
					ary[idx][BP] = mbp + rnd.next(1, 10);
				}
			}
			if (subtask != 2)
			{
				lis.emplace_back(idx);
			}
			last = (int)lis.size() - 1;
		}
	}
	
	// 計算總空間數拿來計算 k
	vector< long long > space(n);
	for (i=0; i<n; i++)
	{
		int idx = ord[i];
		space[idx] = ary[idx][BP];
		if (ary[idx][LP])
		{
			space[idx] += space[ ary[idx][LP]-1 ] + space[ ary[idx][RP]-1 ];
		}
	}
	int ii = rnd.next(0, n-1);
	if (spflag & JUST_K)
	{
		k = space[ii];
	}
	else
	{
		k = rnd.next(1LL, space[ii]);
	}
	
	// print result
	printf("%d %lld\n", n, k);
	for (i=0; i<n; i++)
	{
		printf("%d %d %d\n", ary[i][BP], ary[i][LP], ary[i][RP]);
	}

	return 0;
}

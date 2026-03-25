#include <bits/stdc++.h>
#include "testlib.h"
using namespace std;

const int MAXN = 1e6;
const int MAXBP = 1e9;
const long long MAXK = 1e15;

int main(int argc, char* argv[]) {
	registerValidation(argc, argv);
	// about testlib, see https://codeforces.com/blog/entry/18426
	
	int i;
	int subtask = (argc >= 2 ? atoi(argv[1]) : 0);
	// general format
	int n = inf.readInt(1, MAXN, "n");
	inf.readSpace();
	long long k = inf.readLong(1LL, MAXK, "k");
	inf.readEoln();
	vector< array<int, 3> > ary(n+1);
	vector< int > ind(n+1);
	vector< bool > tbl(n+1);
	for (i=1; i<=n; i++)
	{
		ary[i][0] = inf.readInt(1, MAXBP, "Bi");
		inf.readSpace();
		ary[i][1] = inf.readInt(0, n, "Li");
		inf.readSpace();
		ary[i][2] = inf.readInt(0, n, "Ri");
		inf.readEoln();
		if (ary[i][1] == 0)
		{
			ensure(ary[i][1] == ary[i][2]);
		}
		else
		{
			ensure(ary[i][1] != ary[i][2]);
			ensure(!tbl[ ary[i][1] ]);
			ensure(!tbl[ ary[i][2] ]);
			tbl[ ary[i][1] ] = tbl[ ary[i][2] ] = true;
			++ind[ ary[i][1] ];
			++ind[ ary[i][2] ];
		}
	}
	inf.readEof();
	
	// check if cycle exists
	vector<int> que;
	for (i=1; i<=n; i++)
	{
		if (ind[i] == 0)
		{
			que.emplace_back(i);
		}
	}
	for (i=0; i<que.size(); i++)
	{
		int cur = que[i];
		if (ary[cur][1])
		{
			--ind[ ary[cur][1] ];
			--ind[ ary[cur][2] ];
			if (ind[ ary[cur][1] ] == 0)
			{
				que.emplace_back(ary[cur][1]);
			}
			if (ind[ ary[cur][2] ] == 0)
			{
				que.emplace_back(ary[cur][2]);
			}
		}
	}
	ensure((int)que.size() == n);
	
	// 計算最大深度和最大總佔空間
	int max_depth = 0;
	long long max_bp = 0;
	vector< int > depth(n+1);
	vector< long long > bpsum(n+1);
	while (!que.empty())
	{
		int cur = que.back();
		que.pop_back();
		bpsum[cur] = ary[cur][0];
		if (ary[cur][1])
		{
			depth[cur] = max(depth[ ary[cur][1] ], depth[ ary[cur][2] ]) + 1;
			bpsum[cur] += bpsum[ ary[cur][1] ] + bpsum[ ary[cur][2] ];
		}
		max_depth = max(max_depth, depth[cur]);
		max_bp = max(max_bp, bpsum[cur]);
	}
	ensure(k <= max_bp);
	
	// subtask 1: max depth = 0
	// subtask 2: max depth = 1
	if (subtask == 1)
	{
		ensure(max_depth <= 0);
	}
	if (subtask == 2)
	{
		ensure(max_depth <= 1);
	}
	// subtask 3: n <= 2000
	if (subtask == 3)
	{
		ensure(n <= 2000);
	}
	// subtask 4: parent id > children id
	if (subtask == 4)
	{
		for (i=1; i<=n; i++)
		{
			ensure(i > ary[i][1] && i > ary[i][2]);
		}
	}
	// subtask 5: parent space > children space
	if (subtask == 5)
	{
		for (i=1; i<=n; i++)
		{
			ensure(ary[i][0] > ary[ ary[i][1] ][0] && ary[i][0] > ary[ ary[i][2] ][0]);
		}
	}
	// subtask 6: no extra limit

	return 0;
}

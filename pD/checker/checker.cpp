#include <bits/stdc++.h>
#include "testlib.h"
using namespace std;
int main(int argc, char* argv[]) {
	registerTestlibCmd(argc, argv);
	// read input
    int n = inf.readInt();
	int k = inf.readInt();
	int i;
    vector<int> A = inf.readInts(n, 1, (int)1e9);
	// read contestant output and check format
    long long res = ouf.readLong();
	ouf.readEoln();
    vector<int> B = ouf.readInts(n, 1, n);
	ouf.readEoln();
	ouf.readEof();
	// 確保 B 是 1 .. n 的排列
    set<int> se;
    for(auto &d : B) se.insert(d);
    ensure(se.size() == n);
	// read correct answer
	long long real_ans = ans.readLong();
	// 標程給的排列沒有意義，略過不讀以免浪費時間
	// 檢查答案對不對
	ensure(res == real_ans);
	// 檢查排列有沒有唬爛
	long long tmp = 0;
	int cur = k;
	for (i=0; i<n; i++)
	{
		tmp += abs(cur - A[ B[i]-1 ]);
		cur = A[ B[i]-1 ];
	}
	ensure(tmp == res);
    quit(_ok);
}

#include <bits/stdc++.h>
#include "testlib.h"
using namespace std;

const int MAXN = 1e5;
const int LIM = 1e9;

int main(int argc, char* argv[]) {
	registerValidation(argc, argv);
	// about testlib, see https://codeforces.com/blog/entry/18426

	int subtask = atoi(argv[1]);
	// n k
	// A1 A2 ... An
	int n = inf.readInt(1, MAXN, "n");
	inf.readSpace();
	int k = inf.readInt(1, LIM, "k");
	inf.readEoln();
	vector<int> ary = inf.readInts(n, 1, LIM, "A");
	inf.readEoln();
	inf.readEof();
	// A, k unique
	set<int> se;
	se.emplace(k);
	for (int i=0; i<n; i++)
	{
		se.emplace(ary[i]);
	}
	ensure((int)se.size() == n + 1);
	
	// subtask 1: n = 3
	if (subtask == 1)
	{
		ensure(n == 3);
	}
	// subtask 2: n <= 8
	if (subtask == 2)
	{
		ensure(n <= 8);
	}
	// subtask 3: k < min(A)
	if (subtask == 3)
	{
		ensure(k == *se.begin());
	}
	// subtask 4: A = 1 .. n
	if (subtask == 4)
	{
		ensure(*se.begin() == 1);
		ensure(*prev(prev(se.end())) == n);
	}
	// subtask 5: n <= 2000
	if (subtask == 5)
	{
		ensure(n <= 2000);
	}
	// subtask 6: no limit

	return 0;
}

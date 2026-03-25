#include <bits/stdc++.h>
#include "testlib.h"
using namespace std;

int main(int argc, char* argv[]) {
	registerGen(argc, argv, 1);
	// about testlib, see https://codeforces.com/blog/entry/18291

	// subtask 1: n = 3
	// subtask 2: n <= 8
	// subtask 3: k < min(A)
	// subtask 4: A = 1 .. n
	// subtask 5: n <= 2000
	// subtask 6: no limit
	int subtask = atoi(argv[1]);
	int n = atoi(argv[2]);
	int spflag = (argc >= 5 ? atoi(argv[3]) : 0);
	int k = 1;
	const int LIM = 1e9;
	vector<int> ary(n);
	if (subtask == 4)
	{
		iota(ary.begin(), ary.end(), 1);
		k = rnd.next(n+1, LIM);
		shuffle(ary.begin(), ary.end());
	}
	else
	{
		set<int> se;
		while ((int)se.size() <= n)
		{
			if (spflag == 1)
			{
				if ((int)se.size() < n * 8 / 10)
				{
					se.emplace(rnd.next(10000, 99999));
				}
				else
				{
					se.emplace(rnd.next((int)1e8, LIM));
				}
			}
			else
			{
				se.emplace(rnd.next(1, LIM));
			}
		}
		int i = 0;
		for (auto it=se.begin(); it!=se.end(); ++it)
		{
			ary[i++] = *it;
		}
		k = *prev(se.end());
		int pos = rnd.next(0, n-1);
		if (subtask == 3)
		{
			pos = 0;
		}
		swap(k, ary[pos]);
		shuffle(ary.begin(), ary.end());
	}
	if (spflag & 0b10)
	{
		sort(ary.begin(), ary.end());
	}
	
	printf("%d %d\n", n, k);
	for (int i=0; i<n; i++)
	{
		printf("%d%c", ary[i], " \n"[i+1==n]);
	}

	return 0;
}

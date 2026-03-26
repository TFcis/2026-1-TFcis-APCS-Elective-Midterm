#include <bits/stdc++.h>
#include "testlib.h"
using namespace std;

const int MAXN = 1000000;

int main(int argc, char* argv[]) {
	registerValidation(argc, argv);
	// about testlib, see https://codeforces.com/blog/entry/18426

	int i;
	// subtask 1: 操作 1, 3 only 食物 health only
	// subtask 2: 操作 1, 3 only
	// subtask 3: n <= 3000
	// subtask 4: 操作 2 <= 100
	// subtask 5: 操作 1 -> 操作 2 -> 操作 3
	// subtask 6: no limit
	int subtask = (argc >= 2 ? atoi(argv[1]) : 0);
	// input format
	int n = inf.readInt(1, MAXN, "n");
	if (subtask == 3)
	{
		ensure(n <= 3000);
	}
	inf.readEoln();
	vector< array<int, 3> > items(n);
	int op2_cnt = 0;
	int last_cmd = 0;
	set<int> se;
	const int ST_NOT_APPEAR = 0;
	const int ST_OWNED = 1;
	const int ST_EATEN = 2;
	vector<int> tbl(n+1, ST_NOT_APPEAR);
	vector<int> pq, hq;
	int pp = 0, hp = 0;
	int health_cnt = 0;
	for (i=0; i<n; i++)
	{
		int cmd = inf.readInt(1, 3, "cmd");
		items[i][0] = cmd;
		if (cmd == 1)
		{
			inf.readSpace();
			int id = inf.readInt(1, n, "id");
			inf.readSpace();
			string str = inf.readWord();
			ensure(str == "poison" || str == "health");
			if (subtask == 1)
			{
				ensure(str == "health");
			}
			inf.readEoln();
			bool is_poison = (str == "poison");
			items[i][1] = id;
			items[i][2] = is_poison;
			// id 必須唯一
			ensure(!se.count(id));
			se.emplace(id);
			ensure(tbl[id] == ST_NOT_APPEAR);
			if (is_poison)
			{
				pq.emplace_back(id);
			}
			else
			{
				hq.emplace_back(id);
			}
			tbl[id] = ST_OWNED;
		}
		else if (cmd == 2)
		{
			inf.readSpace();
			int id = inf.readInt(1, n, "id");
			inf.readEoln();
			items[i][1] = id;
			// id 必須在手邊
			ensure(se.count(id));
			ensure(tbl[id] == ST_OWNED);
			tbl[id] = ST_EATEN;
		}
		else
		{
			inf.readEoln();
			int target_id = -1;
			while (health_cnt >= 2 && pp < pq.size())
			{
				int id = pq[pp++];
				if (tbl[id] == ST_OWNED)
				{
					target_id = id;
					health_cnt = 0;
					break;
				}
			}
			while (target_id == -1 && hp < hq.size())
			{
				int id = hq[hp++];
				if (tbl[id] == ST_OWNED)
				{
					target_id = id;
					++health_cnt;
					break;
				}
			}
			while (target_id == -1 && pp < pq.size())
			{
				int id = pq[pp++];
				if (tbl[id] == ST_OWNED)
				{
					target_id = id;
					health_cnt = 0;
					break;
				}
			}
			if (target_id != -1)
			{
				tbl[target_id] = ST_EATEN;
			}
		}
		if (subtask == 1 || subtask == 2)
		{
			ensure(cmd != 2);
		}
		op2_cnt += (cmd == 2);
		if (subtask == 4)
		{
			ensure(op2_cnt <= 100);
		}
		if (subtask == 5)
		{
			ensure(cmd >= last_cmd);
		}
		last_cmd = cmd;
	}
	inf.readEof();

	return 0;
}

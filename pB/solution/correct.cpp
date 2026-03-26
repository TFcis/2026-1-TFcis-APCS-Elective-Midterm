#include <bits/stdc++.h>
using namespace std;

int main() {
	int n, i;
	scanf("%d", &n);
	vector<bool> tbl(n+1);
	queue<int> pq, hq;
	int con = 0;
	for (i=0; i<n; i++)
	{
		int cmd, id;
		bool is_poison;
		char buf[8];
		scanf("%d", &cmd);
		if (cmd == 1)
		{
			scanf("%d%s", &id, buf);
			is_poison = (*buf == 'p');
			if (is_poison)
			{
				pq.push(id);
			}
			else
			{
				hq.push(id);
			}
		}
		else if (cmd == 2)
		{
			scanf("%d", &id);
			tbl[id] = true;
		}
		else
		{
			int tid = -1;
			while (con >= 2 && !pq.empty())
			{
				int id = pq.front();
				pq.pop();
				if (!tbl[id])
				{
					con = 0;
					tid = id;
					break;
				}
			}
			while (tid == -1 && !hq.empty())
			{
				int id = hq.front();
				hq.pop();
				if (!tbl[id])
				{
					++con;
					tid = id;
					break;
				}
			}
			while (tid == -1 && !pq.empty())
			{
				int id = pq.front();
				pq.pop();
				if (!tbl[id])
				{
					con = 0;
					tid = id;
					break;
				}
			}
			printf("%d\n", tid);
		}
	}
	return 0;
}

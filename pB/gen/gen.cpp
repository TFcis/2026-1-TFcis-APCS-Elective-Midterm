#include <bits/stdc++.h>
#include "testlib.h"
using namespace std;

int main(int argc, char* argv[]) {
	registerGen(argc, argv, 1);
	// about testlib, see https://codeforces.com/blog/entry/18291

	// subtask 1: 操作 1, 3 only 食物 health only
	// subtask 2: 操作 1, 3 only
	// subtask 3: n <= 3000
	// subtask 4: 操作 2 <= 100
	// subtask 5: 操作 1 -> 操作 2 -> 操作 3
	// subtask 6: no limit
	int i;
	int subtask = atoi(argv[1]);
	int n = atoi(argv[2]);
	int poison_rate = (argc >= 5 ? atoi(argv[3]) : 40);
	if (subtask == 1)
	{
		poison_rate = 0;
	}
	int type = (argc >= 6 ? atoi(argv[4]) : 0);
	const int TYPE_RANDOM = 0;	// all random
	const int TYPE_WAVE = 1;	// 大囤大吃大囤大吃
	const int TYPE_HUNGRY = 2;	// 吃不夠
	const int TYPE_FAT = 3;		// 吃到撐
	const int TYPE_TRANS = 4;	// 傾向隨時間變化
	const int TYPE_SAVEFOOD = 5;// 食物夠多才肯吃
	
	// 生成 ID 發送順序
	vector<int> id_pool(n);
	iota(id_pool.begin(), id_pool.end(), 1);
	shuffle(id_pool.begin(), id_pool.end());
	// 管理狀態，模擬過程
	vector< array<int, 3> > tbl(n+1), res;
	int poison_ptr = 0, health_ptr = 0;
	vector<int> poison_que, health_que;
	res.reserve(n);
	set< pair<int, int> > order_se, rand_se;
	int health_times = 0;
	int get_weight = rnd.next(35, 75);
	int lose_weight = rnd.next(15, 35);
	int query_weight = rnd.next(15, 35);
	if (subtask == 1 || subtask == 2)
	{
		query_weight += lose_weight / 2;
		lose_weight = 0;
	}
	int lose_cnt = 0;
	int lose_lim = n;
	if (subtask == 4)
	{
		lose_lim = 100;
	}
	// 各種特殊 type 用
	int st_idx = 0;
	vector<int> st(rnd.next(1, 2)*2);
	if (type == TYPE_WAVE)
	{
		for (i=0; i<(int)st.size(); i++)
		{
			int t = n / st.size() * (i+1);
			st[i] = rnd.next((int)(t*.9), (int)(t*1.1));
		}
		st.back() = n + 1;
		// WAVE 從大囤開始
		get_weight *= 10;
	}
	if (type == TYPE_HUNGRY)
	{
		lose_weight *= 4;
		query_weight *= 2;
	}
	if (type == TYPE_FAT)
	{
		get_weight *= 3;
	}
	if (type == TYPE_TRANS)
	{
		get_weight *= max(2, n/30);
		lose_weight *= max(2, n/50);
		query_weight *= max(2, n/50);
	}
	int food_save = n * rnd.next(25, 35) / 100;
	
	// 實際生成
	const int CMD_GET_FOOD = 1;
	const int CMD_LOSE_FOOD = 2;
	const int CMD_QUERY = 3;
	for (i=0; i<n; i++)
	{
		// decide CMD
		int cmd = CMD_QUERY;
		int total_weight = get_weight + lose_weight + query_weight;
		// 按照 weight 骰比重
		int dice = rnd.next(1, total_weight);
		if (dice <= get_weight)
		{
			cmd = CMD_GET_FOOD;
		}
		else if (dice <= get_weight+lose_weight)
		{
			cmd = CMD_LOSE_FOOD;
		}
		// subtask 4 只能刪 100 次，後半再開始刪
		if (subtask == 4 && cmd == CMD_LOSE_FOOD && (lose_cnt + 1 > lose_lim || i+i < n))
		{
			cmd = CMD_QUERY;
		}
		// subtask 5 依比重決定各 cmd 數量
		if (subtask == 5)
		{
			int t = n * 1LL * get_weight / total_weight;
			if (i < t)
			{
				cmd = CMD_GET_FOOD;
			}
			else
			{
				t = n * 1LL * (get_weight+lose_weight) / total_weight;
				if (i < t)
				{
					cmd = CMD_LOSE_FOOD;
				}
				else
				{
					cmd = CMD_QUERY;
				}
			}
		}
		// 大囤類型少了就不吃
		if (type == TYPE_SAVEFOOD)
		{
			if (rand_se.size() < food_save)
			{
				cmd = CMD_GET_FOOD;
			}
		}
		// 如果沒有食物就把分食換成刃牙吃
		if (cmd == CMD_LOSE_FOOD && rand_se.empty())
		{
			cmd = CMD_QUERY;
		}
		
		// 按 cmd 類型決定細節
		array< int, 3 > item;
		item[0] = cmd;
		if (cmd == CMD_GET_FOOD)
		{
			int id = id_pool[i];
			item[1] = id;
			dice = rnd.next(1, 100);
			bool is_poison = (dice <= poison_rate);
			item[2] = is_poison;
			if (is_poison)
			{
				poison_que.emplace_back(id);
			}
			else
			{
				health_que.emplace_back(id);
			}
			tbl[id][0] = i;
			tbl[id][1] = rnd.next(1, (int)1e9);
			tbl[id][2] = false;
			order_se.emplace(i, id);
			rand_se.emplace(tbl[id][1], id);
		}
		else if (cmd == CMD_LOSE_FOOD)
		{
			int target_id;
			dice = rnd.next(1, 100);
			if (dice <= 40)  // random
			{
				target_id = (rand_se.begin())->second;
			}
			else if (dice <= 70)
			{
				int dist = rnd.next(0, 100);
				auto it = order_se.begin();
				while (dist > 0 && next(it) != order_se.end())
				{
					--dist, ++it;
				}
				target_id = it->second;
			}
			else
			{
				int dist = rnd.next(0, 100);
				auto it = order_se.rbegin();
				while (dist > 0 && next(it) != order_se.rend())
				{
					--dist, ++it;
				}
				target_id = it->second;
			}
			item[1] = target_id;
			tbl[target_id][2] = true;
			order_se.erase({tbl[target_id][0], target_id});
			rand_se.erase({tbl[target_id][1], target_id});
		}
		else
		{
			const int NOT_FOUND = -1;
			int target_id = NOT_FOUND;
			bool is_poison = false;
			while (health_times >= 2 && poison_ptr < poison_que.size())
			{
				int id = poison_que[poison_ptr++];
				if (!tbl[id][2])
				{
					is_poison = true;
					target_id = id;
					break;
				}
			}
			while ((health_times < 2 || target_id == NOT_FOUND) && health_ptr < health_que.size())
			{
				int id = health_que[health_ptr++];
				if (!tbl[id][2])
				{
					target_id = id;
					break;
				}
			}
			while (target_id == NOT_FOUND && poison_ptr < poison_que.size())
			{
				int id = poison_que[poison_ptr++];
				if (!tbl[id][2])
				{
					is_poison = true;
					target_id = id;
					break;
				}
			}
			if (target_id != NOT_FOUND)
			{
				if (is_poison)
				{
					health_times = 0;
				}
				else
				{
					++health_times;
				}
				tbl[target_id][2] = true;
				order_se.erase({tbl[target_id][0], target_id});
				rand_se.erase({tbl[target_id][1], target_id});
			}
		}
		
		// 後續處理
		if (type == TYPE_WAVE)
		{
			if (i >= st[st_idx])
			{
				++st_idx;
				if (st_idx & 1)  // 轉進大吃
				{
					get_weight = rnd.next(5, 15);
					lose_weight = rnd.next(45, 70);
					query_weight = rnd.next(60, 80);
				}
				else  // 轉進大囤
				{
					get_weight = rnd.next(120, 160);
					lose_weight = rnd.next(5, 15);
					query_weight = rnd.next(10, 20);
				}
			}
		}
		if (type == TYPE_TRANS)
		{
			++lose_weight;
			++query_weight;
		}
	}
	
	// print result
	const char *POISON_TEXT[] = {"health", "poison"};
	printf("%d\n", n);
	for (i=0; i<n; i++)
	{
		int cmd = res[i][0];
		if (cmd == CMD_GET_FOOD)
		{
			printf("1 %d %s\n", res[i][1], POISON_TEXT[ res[i][2] ]);
		}
		else if (cmd == CMD_LOSE_FOOD)
		{
			printf("2 %d\n", res[i][1]);
		}
		else
		{
			puts("3");
		}
	}

	return 0;
}

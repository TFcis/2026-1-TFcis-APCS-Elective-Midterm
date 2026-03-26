#include <bits/stdc++.h>
#include "testlib.h"
using namespace std;

int main(int argc, char* argv[]) {
	registerGen(argc, argv, 1);
	// about testlib, see https://codeforces.com/blog/entry/18291

	/**
	 * Generator for NoPoisonNoHealth
	 *
	 * Usage: gen <seed> <mode>
	 *
	 * mode:
	 *   2 - subtask 2: only health food, no type 2
	 *   3 - subtask 3: no type 2
	 *   4 - subtask 4: n <= 3000, all operations
	 *   5 - subtask 5: type 2 count <= 100
	 *   6 - subtask 6: all type 1 first, then type 2, then type 3
	 *   7 - subtask 7: stress, designed to TLE set/map solutions
	 *   71- subtask 7: max poison accumulation stress
	 *   72- subtask 7: heavy mid-queue deletion stress
	 */

    if (argc < 3) {
        fprintf(stderr, "Usage: gen <seed> <mode>\n");
        return 1;
    }

    int mode = atoi(argv[2]);

    // ------------------------------------------------------------------ //
    // Helper: build a sequence of (type, id, is_health) events            //
    // ------------------------------------------------------------------ //
    struct Event {
        int type;   // 1, 2, 3
        int id;     // for type 1 and 2
        bool health;// for type 1
    };

    vector<Event> events;

    // ------------------------------------------------------------------ //
    // Mode 2: only health, no type 2                                      //
    // ------------------------------------------------------------------ //
    if (mode == 2) {
        int n = 1000000;
        // alternate: add health, eat
        int id = 1;
        for (int i = 0; i < n; i++) {
            if (id <= n / 2 && (rnd.next(2) == 0 || events.empty())) {
                events.push_back({1, id++, true});
            } else {
                events.push_back({3, 0, false});
            }
        }
    }

    // ------------------------------------------------------------------ //
    // Mode 3: both food types, no type 2                                  //
    // ------------------------------------------------------------------ //
    else if (mode == 3) {
        int n = 1000000;
        int id = 1;
        // Add foods with health/poison mix, then interleave eating
        // Goal: trigger poison-eating path frequently
        for (int i = 0; i < n; i++) {
            bool can_add = id * 2 <= n;
            bool can_eat = !events.empty();
            if (can_add && (!can_eat || rnd.next(2) == 0)) {
                // bias towards adding 2 healths then 1 poison repeatedly
                int phase = id % 3;
                bool is_health = (phase != 2);
                events.push_back({1, id++, is_health});
            } else {
                events.push_back({3, 0, false});
            }
        }
    }

    // ------------------------------------------------------------------ //
    // Mode 4: n <= 3000, all operations                                   //
    // ------------------------------------------------------------------ //
    else if (mode == 4) {
        int n = 3000;
        set<int> in_hand;
        int id = 1;
        for (int i = 0; i < n; i++) {
            int op_max = 3;
            if (in_hand.empty()) op_max = 1;
            if (id > n)          op_max = (in_hand.empty() ? 3 : op_max);

            int op;
            if (id > n && in_hand.empty()) {
                break; // nothing left to do
            } else if (id > n) {
                op = rnd.next(2, 3);
            } else if (in_hand.empty()) {
                op = 1;
            } else {
                op = rnd.next(1, 3);
            }

            if (op == 1) {
                bool is_health = rnd.next(2) == 0;
                in_hand.insert(id);
                events.push_back({1, id++, is_health});
            } else if (op == 2) {
                // pick random element from in_hand
                int idx = rnd.next(0, (int)in_hand.size() - 1);
                auto it = in_hand.begin();
                advance(it, idx);
                int chosen = *it;
                in_hand.erase(it);
                events.push_back({2, chosen, false});
            } else {
                events.push_back({3, 0, false});
            }
        }
        n = (int)events.size();
        printf("%d\n", n);
        for (auto& e : events) {
            if (e.type == 1)
                printf("1 %d %s\n", e.id, e.health ? "health" : "poison");
            else if (e.type == 2)
                printf("2 %d\n", e.id);
            else
                printf("3\n");
        }
        return 0;
    }

    // ------------------------------------------------------------------ //
    // Mode 5: type 2 count <= 100                                         //
    // ------------------------------------------------------------------ //
    else if (mode == 5) {
        int n = 1000000;
        int max_del = 100;
        int del_count = 0;
        set<int> in_hand;
        int id = 1;

        for (int i = 0; i < n; i++) {
            int op;
            bool can_del = del_count < max_del && !in_hand.empty();
            bool can_eat = !in_hand.empty();
            bool can_add = id <= n;

            if (!can_add && !can_eat) break;

            if (!can_add) {
                op = (can_del && rnd.next(1, 20) == 1) ? 2 : 3;
            } else if (!can_eat) {
                op = 1;
            } else {
                int r = rnd.next(1, 10);
                if (r <= 4) op = 1;
                else if (r <= 5 && can_del) op = 2;
                else op = 3;
            }

            if (op == 1 && id <= n) {
                bool is_health = (id % 3 != 0); // 2 health : 1 poison ratio
                in_hand.insert(id);
                events.push_back({1, id++, is_health});
            } else if (op == 2 && can_del) {
                int idx = rnd.next(0, (int)in_hand.size() - 1);
                auto it = in_hand.begin();
                advance(it, idx);
                int chosen = *it;
                in_hand.erase(it);
                events.push_back({2, chosen, false});
                del_count++;
            } else {
                events.push_back({3, 0, false});
            }
        }
    }

    // ------------------------------------------------------------------ //
    // Mode 6: all type1 -> all type2 -> all type3                         //
    // ------------------------------------------------------------------ //
    else if (mode == 6) {
        int n = 1000000;
        int add_count  = n * 5 / 10; // 50%
        int del_count  = n * 2 / 10; // 20%
        int eat_count  = n - add_count - del_count;

        // Phase 1: add all foods
        for (int i = 1; i <= add_count; i++) {
            bool is_health = (i % 3 != 0);
            events.push_back({1, i, is_health});
        }

        // Phase 2: delete some (pick non-front IDs to stress deletion)
        // Delete from middle: IDs that are not multiples of small numbers
        vector<int> del_ids;
        for (int i = 1; i <= add_count; i++) {
            if (i % 3 == 1) del_ids.push_back(i); // every 3rd health food
        }
        shuffle(del_ids.begin(), del_ids.end());
        del_ids.resize(min((int)del_ids.size(), del_count));
        sort(del_ids.begin(), del_ids.end()); // stable order for phase
        for (int id : del_ids) {
            events.push_back({2, id, false});
        }

        // Phase 3: eat everything
        for (int i = 0; i < eat_count; i++) {
            events.push_back({3, 0, false});
        }
    }

    // ------------------------------------------------------------------ //
    // Mode 7: general stress — interleaved, designed to TLE set/map       //
    // Strategy:                                                            //
    //   - Maintain a large pool of foods at all times                     //
    //   - Deletions target mid-queue items (not front), forcing O(log n)  //
    //     per deletion in set/map solutions                               //
    //   - Eating is frequent enough to trigger both health and poison path //
    // ------------------------------------------------------------------ //
    else if (mode == 7) {
        int n = 1000000;

        // We'll track what's in hand to generate valid type-2 ops
        // health_ids and poison_ids stored as deque (insertion order)
        deque<int> health_q, poison_q;
        // all_in_hand for random deletion target
        vector<int> health_vec, poison_vec;

        int id = 1;
        int total = 0;
        int healthy_streak = 0;

        // Warm up: fill with foods first
        int warmup = 50000;
        for (int i = 0; i < warmup && id <= n; i++) {
            bool is_health = (id % 3 != 0);
            if (is_health) { health_q.push_back(id); health_vec.push_back(id); }
            else           { poison_q.push_back(id); poison_vec.push_back(id); }
            events.push_back({1, id++, is_health});
            total++;
        }

        // Main loop: interleave add / delete-middle / eat
        while (total < n) {
            int r = rnd.next(1, 12);

            if (r <= 4 && id <= n) {
                // Add food — bias 2:1 health:poison to trigger poison eating
                bool is_health = (rnd.next(1, 3) != 1);
                if (is_health) { health_q.push_back(id); health_vec.push_back(id); }
                else           { poison_q.push_back(id); poison_vec.push_back(id); }
                events.push_back({1, id++, is_health});
                total++;
            }
            else if (r <= 7 && (!health_vec.empty() || !poison_vec.empty())) {
                // Delete a random mid-queue item
                // Prefer deleting from health (makes poison accumulate, stresses logic)
                bool del_health = !health_vec.empty() &&
                                  (poison_vec.empty() || rnd.next(2) == 0);
                vector<int>& vec = del_health ? health_vec : poison_vec;
                if (!vec.empty()) {
                    int idx = rnd.next(0, (int)vec.size() - 1);
                    int chosen = vec[idx];
                    vec.erase(vec.begin() + idx);
                    events.push_back({2, chosen, false});
                    total++;
                }
            }
            else if (!health_vec.empty() || !poison_vec.empty()) {
                // Eat — just output op 3, let solution handle logic
                events.push_back({3, 0, false});
                total++;

                // Simulate eat to keep our tracking in sync
                if (health_vec.empty() && poison_vec.empty()) {
                    // no food, -1
                } else if (healthy_streak >= 2 && !poison_vec.empty()) {
                    // eat poison: remove front of poison_q from poison_vec
                    if (!poison_q.empty()) {
                        int front_p = poison_q.front();
                        poison_q.pop_front();
                        auto it = find(poison_vec.begin(), poison_vec.end(), front_p);
                        if (it != poison_vec.end()) poison_vec.erase(it);
                        healthy_streak = 0;
                    }
                } else {
                    if (!health_vec.empty() && !health_q.empty()) {
                        int front_h = health_q.front();
                        health_q.pop_front();
                        auto it = find(health_vec.begin(), health_vec.end(), front_h);
                        if (it != health_vec.end()) health_vec.erase(it);
                        healthy_streak++;
                    } else if (!poison_vec.empty() && !poison_q.empty()) {
                        int front_p = poison_q.front();
                        poison_q.pop_front();
                        auto it = find(poison_vec.begin(), poison_vec.end(), front_p);
                        if (it != poison_vec.end()) poison_vec.erase(it);
                        healthy_streak = 0;
                    }
                }
            }
            else if (id <= n) {
                bool is_health = rnd.next(2) == 0;
                if (is_health) { health_q.push_back(id); health_vec.push_back(id); }
                else           { poison_q.push_back(id); poison_vec.push_back(id); }
                events.push_back({1, id++, is_health});
                total++;
            }
        }
    }

    // ------------------------------------------------------------------ //
    // Mode 71: max poison accumulation                                    //
    // Add lots of poison, eat health until streak >= 2, then eat poison   //
    // Repeat — forces checker to traverse large poison queue              //
    // ------------------------------------------------------------------ //
    else if (mode == 71) {
        int n = 1000000;
        int id = 1;
        int total = 0;
        // Pattern: add 500 poison, add 2 health, eat eat eat (eats 2 health + 1 poison)
        // Repeat → poison queue grows large, many random deletions in between
        int block = 0;
        while (total < n && id <= n) {
            int phase = block % 3;
            if (phase == 0) {
                // add 3 poison
                for (int i = 0; i < 3 && total < n && id <= n; i++) {
                    events.push_back({1, id++, false});
                    total++;
                }
            } else if (phase == 1) {
                // add 2 health
                for (int i = 0; i < 2 && total < n && id <= n; i++) {
                    events.push_back({1, id++, true});
                    total++;
                }
            } else {
                // eat 3 times (2 health → 1 poison)
                for (int i = 0; i < 3 && total < n; i++) {
                    events.push_back({3, 0, false});
                    total++;
                }
            }
            block++;
        }
    }

    // ------------------------------------------------------------------ //
    // Mode 72: heavy mid-queue deletion                                   //
    // Fill queue, then delete every other element (not front/back),       //
    // then eat — stresses any O(log n) per deletion structure             //
    // ------------------------------------------------------------------ //
    else if (mode == 72) {
        int n = 1000000;
        int half = n / 3;
        int id = 1;

        // Add half foods
        for (int i = 0; i < half; i++) {
            bool is_health = (i % 3 != 0);
            events.push_back({1, id++, is_health});
        }

        // Delete every other one (skip front = ID 1, skip back = ID half)
        for (int i = 2; i < half; i += 2) {
            events.push_back({2, i, false});
        }

        // Add more and eat interleaved
        while ((int)events.size() < n) {
            if (id <= n && rnd.next(1, 3) != 1) {
                bool is_health = rnd.next(2) == 0;
                events.push_back({1, id++, is_health});
            } else {
                events.push_back({3, 0, false});
            }
        }
        events.resize(n);
    }

    // ------------------------------------------------------------------ //
    // Output                                                               //
    // ------------------------------------------------------------------ //
    printf("%d\n", (int)events.size());
    for (auto& e : events) {
        if (e.type == 1)
            printf("1 %d %s\n", e.id, e.health ? "health" : "poison");
        else if (e.type == 2)
            printf("2 %d\n", e.id);
        else
            printf("3\n");
    }

	return 0;
}

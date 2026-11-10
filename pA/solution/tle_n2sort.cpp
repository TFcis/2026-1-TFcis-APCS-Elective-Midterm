#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

// 每位敵人 i 需要 t_i = ceil(H_i / a) 回合擊殺，每回合造成 D_i 傷害。
// 按 t/D 由小到大 (交換論證) 依序擊殺，可使總承受傷害 S 最小。
// 1. S < b           -> Win
// 2. 否則若存在某位敵人 j 排最後一個擊殺，使「最後一回合之前」的承受傷害 < b
//                    -> Draw (最後一回合我方才倒下，同時擊殺最後一位敵人)
// 3. 否則            -> Lose
//
// 將 j 移到最後，其餘維持原順序，最後一回合之前的傷害為
//   S + X_j - D_j,  X_j = D_j * tail_j - t_j * suf_j
// tail_j = j 之後所有敵人的 t 總和，suf_j = j 之後所有敵人的 D 總和。
// 因為已按 t/D 排序，X_j = sum_{k>j} (D_j*t_k - t_j*D_k) >= 0。
// 在 S >= b 的情況下，要 S + X_j - D_j < b 需 X_j < D_j - (S - b)。
// 為避免 D_j * tail_j 超過 long long，令 suf_j = u*D_j + v (0 <= v < D_j)，
//   X_j = D_j * (tail_j - t_j*u) - t_j*v，且 w = tail_j - t_j*u >= 0 (因 X_j >= 0)
// 故所有中間值皆在 long long 範圍內。
int main() {
	ll atk, hp;
	int n;
	scanf("%lld%lld%d", &atk, &hp, &n);
	vector<ll> H(n), D(n), t(n);
	for (int i = 0; i < n; i++) scanf("%lld", &H[i]);
	for (int i = 0; i < n; i++) scanf("%lld", &D[i]);
	for (int i = 0; i < n; i++) t[i] = (H[i] + atk - 1) / atk;
	vector<int> id(n);
	iota(id.begin(), id.end(), 0);
	for (int i = 1; i < n; i++) { // O(n^2) insertion sort
		for (int j = i; j > 0 && t[id[j]] * D[id[j-1]] < t[id[j-1]] * D[id[j]]; j--) {
			swap(id[j], id[j-1]);
		}
	}
	// S: 最小總承受傷害 (題目保證不超過 1e18)
	ll S = 0, tt = 0;
	for (int k = 0; k < n; k++) {
		tt += t[id[k]];
		S += D[id[k]] * tt;
	}
	if (S < hp) {
		puts("Win");
		return 0;
	}
	ll excess = S - hp; // >= 0
	ll tail = 0, suf = 0;
	bool draw = false;
	for (int k = n - 1; k >= 0 && !draw; k--) {
		int j = id[k];
		ll lim = D[j] - excess; // 需要 X_j < lim
		if (lim > 0) {
			ll u = suf / D[j], v = suf % D[j];
			ll w = tail - t[j] * u;
			ll bound = lim + t[j] * v; // 需要 D_j * w < bound
			if (w <= (bound - 1) / D[j]) draw = true;
		}
		tail += t[j];
		suf += D[j];
	}
	puts(draw ? "Draw" : "Lose");
	return 0;
}

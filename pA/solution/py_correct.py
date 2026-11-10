import sys
from functools import cmp_to_key

def main():
    data = sys.stdin.read().split()
    atk, hp, n = int(data[0]), int(data[1]), int(data[2])
    H = list(map(int, data[3:3 + n]))
    D = list(map(int, data[3 + n:3 + 2 * n]))
    t = [(h + atk - 1) // atk for h in H]

    # 依 t/D 由小到大排序，使總承受傷害最小
    order = sorted(range(n), key=cmp_to_key(
        lambda x, y: (t[x] * D[y] > t[y] * D[x]) - (t[x] * D[y] < t[y] * D[x])))
    S = 0
    cur = 0
    for i in order:
        cur += t[i]
        S += D[i] * cur
    if S < hp:
        print("Win")
        return

    # 枚舉最後一位被擊殺的敵人 j，其餘維持排序順序
    # 最後一回合之前的承受傷害 = S + D_j * tail_j - t_j * suf_j - D_j
    tail = 0
    suf = 0
    best = None
    for j in reversed(order):
        pre = S + D[j] * tail - t[j] * suf - D[j]
        if best is None or pre < best:
            best = pre
        tail += t[j]
        suf += D[j]
    print("Draw" if best < hp else "Lose")

main()

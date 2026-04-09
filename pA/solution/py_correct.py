import sys
from functools import cmp_to_key

def main():
    input_data = sys.stdin.read().split()
    idx = 0

    atk = int(input_data[idx]); idx += 1
    hp  = int(input_data[idx]); idx += 1
    n   = int(input_data[idx]); idx += 1

    ary = [[0, 0] for _ in range(n)]

    for i in range(n):
        ary[i][0] = int(input_data[idx]); idx += 1

    total_dmg = 0
    for i in range(n):
        ary[i][1] = int(input_data[idx]); idx += 1
        total_dmg += ary[i][1]

    def cmp(a, b):
        at = (a[0] + atk - 1) // atk
        bt = (b[0] + atk - 1) // atk
        pa = (a[1] + b[1]) * at + b[1] * bt
        pb = (a[1] + b[1]) * bt + a[1] * at
        return -1 if pa < pb else (1 if pa > pb else 0)

    ary.sort(key=cmp_to_key(cmp))

    i = 0
    while i < n and hp > 0:
        t = (ary[i][0] + atk - 1) // atk
        t = min(t, (hp + ary[i][1] - 1) // ary[i][1])
        hp -= total_dmg * t
        ary[i][0] -= t * atk
        total_dmg -= ary[i][1]
        i += 1

    if hp <= 0 and ary[-1][0] <= 0:
        print("Draw")
    elif hp <= 0:
        print("Lose")
    else:
        print("Win")

main()
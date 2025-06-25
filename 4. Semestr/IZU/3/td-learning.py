from decimal import Decimal, ROUND_HALF_UP
import sys
def td_learning():
    lines = [line.strip() for line in sys.stdin if line.strip()]

    # 1.radek = cesta 
    path = list(map(int, lines[0].strip().split()))

    # Dalsi radky = matice hodnot (4x5)
    values = []
    for line in lines[1:]:
        row = list(map(float, line.strip().split()))
        values.extend(row)

    # rewards 
    rewards = [0.0] * 20
    rewards[8] = 1.0    # stav 9 (index 8)
    rewards[11] = -1.0  # stav 12 (index 11)
    
    # nastaveni parametru 
    alpha = 0.1 # learning rate
    gamma = 0.8 # discount factor

    # algoritmus TD(0)
    for i in range(len(path) - 1):
        s = path[i] - 1      
        s_next = path[i + 1] - 1  

        # TD(0) update
        values[s] = values[s] + alpha * (rewards[s_next] + gamma * values[s_next] - values[s])

    # update metrix
    updated_matrix = []
    for i in range(0, 20, 5):
        row = []
        for j in range(5):
            raw_value = values[i + j]
            value = float(Decimal(str(raw_value)).quantize(Decimal("0.001"), rounding=ROUND_HALF_UP))
            row.append(value)
        updated_matrix.append(row)

    return updated_matrix

# main
updated = td_learning()
for row in updated:
    print(" " + " ".join(f"{value:>6.3f}" for value in row))
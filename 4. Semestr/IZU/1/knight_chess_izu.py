import math

# Globální čítač pro pořadí vložení uzlů
node_counter = 0
def next_node_order():
    global node_counter
    order = node_counter
    node_counter += 1
    return order

class Node:
    """
    Třída uzlu: obsahuje souřadnice (x, y),
    náklady g, heuristiku h, součet f = g + h,
    ukazatel na rodiče (parent) a pořadí vložení (order).
    """
    def __init__(self, x, y, g=0, h=0, parent=None):
        self.x = x                    # sloupec
        self.y = y                    # řádek
        self.g = math.ceil(g)         # počet tahů (zaokrouhleno nahoru)
        self.h = math.ceil(h)         # heuristika (zaokrouhleno nahoru)
        self.f = self.g + self.h      # celkové ohodnocení
        self.parent = parent          # odkaz na rodičovský uzel
        self.order = next_node_order()# pořadí vložení (pro tie-breaker)

    def __repr__(self):
        parent_str = "NULL" if self.parent is None else f"[{self.parent.x},{self.parent.y}]"
        return f"([{self.x},{self.y}], {parent_str}, {self.g}, {self.h}, {self.f})"

def heuristic(x, y, goal_x, goal_y):
    """
    Modifikovaná Chebyshevova heuristika dle zadání:
    h = ceil( max(|x - goal_x|/2, |y - goal_y|/2) ).
    """
    dx = abs(x - goal_x)
    dy = abs(y - goal_y)
    return math.ceil(max(dx / 2, dy / 2))

def get_knight_neighbors(x, y):
    """
    Vygeneruje všechny platné tahy koně z pozice (x, y)
    na standardní 8x8 šachovnici. Nové uzly jsou generovány
    v pořadí "zleva doprava a shora dolů" – tj. seřadíme
    podle (x, y).
    """
    moves = [
        (-2, -1), (-2, +1),
        (-1, -2), (-1, +2),
        (+1, -2), (+1, +2),
        (+2, -1), (+2, +1)
    ]
    valid_neighbors = [(x + dx, y + dy) for dx, dy in moves if 0 <= x + dx < 8 and 0 <= y + dy < 8]
    valid_neighbors.sort(key=lambda p: (p[1], p[0]))  # Sort by x, then y
    return valid_neighbors

def reconstruct_path(node):
    """
    Rekonstruuje cestu od nalezeného cílového uzlu
    zpětným sledováním parent odkazů.
    Vrací list uzlů od startu k cíli.
    """
    path = []
    while node is not None:
        path.append(node)
        node = node.parent
    return list(reversed(path))

def a_star_knight(start_x=3, start_y=5, goal_x=7, goal_y=6):
    """
    Hledání cesty A* pro šachového koně ze startu [3, 5]
    do cíle [7, 6].
    """
    open_list, closed_list, closed_set = [], [], set()
    start_node = Node(start_x, start_y, g=0, h=heuristic(start_x, start_y, goal_x, goal_y))
    open_list.append(start_node)

    while open_list:
        current = min(open_list, key=lambda n: (n.f, n.order))
        open_list.remove(current)
        closed_list.append(current)
        closed_set.add((current.x, current.y))

        if current.x == goal_x and current.y == goal_y:
            return reconstruct_path(current)

        for nx, ny in get_knight_neighbors(current.x, current.y):
            if (nx, ny) in closed_set:
                continue

            neighbor_node = Node(nx, ny, g=current.g + 1, h=heuristic(nx, ny, goal_x, goal_y), parent=current)

            if all(n.g <= neighbor_node.g for n in open_list if n.x == nx and n.y == ny):
                open_list.append(neighbor_node)

    return None

if __name__ == "__main__":
    path = a_star_knight()
    if path:
        print("Výsledná cesta:")
        for node in path:
            print(f"({node.x}, {node.y})")
    else:
        print("Cesta nenalezena.")

import os
from math import floor
from random import random, choice

n = 60000
cc_mean = 25
cc_dev = 5

visited = set()
node_iter = range(n)
nodes = set(node_iter)
adj_list = dict()

for node in node_iter:
    adj_list[node] = []

for node in node_iter:
    if node in visited:
        if len(nodes) == 0:
            break
        continue
    visited.add(node)
    nodes.remove(node)

    cc = floor(cc_mean + (cc_dev * (random() - .5)))
    path_len = min([len(nodes), cc])
    path = [node]
    # build random independent connected component
    predecessor = node
    for _ in range(path_len):
        if len(nodes) == 0:
            break
        successor = choice(list(nodes))
        visited.add(successor)
        nodes.remove(successor)
        path.append(successor)
        adj_list[predecessor].append(successor)
        predecessor = successor

    # attach especially acyclically
    anchor = choice(list(node_iter))
    while anchor in path:
        anchor = choice(list(visited))
    adj_list[predecessor].append(anchor)

out_dir = os.environ.get("FUZZING_DATA", None)
for node, neighbors in adj_list.items():
    print(f"#module {node}")
    for dep in neighbors:
        print(f"#depends {dep}")
    if not out_dir:
        continue
    with open(os.path.join(out_dir, f"{node}.sql"), "w+") as f:
        f.writelines([
            f"-- #module {node}\n",
            *[f"-- #depends {dep}\n" for dep in neighbors]
        ])


import json
import networkx as nx
import matplotlib.pyplot as plt

with open("plot.json") as f:
    network = json.load(f)

graph = nx.DiGraph(directed=True)

for node in network["nodes"]:
    graph.add_node(node["eq"])

edge_labels = {}
for link in network["links"]:
    edge_labels[(link["source"], link["target"])] = link["source"]
    graph.add_edge(link["source"], link["target"])

pos = nx.spring_layout(graph)
plt.figure()

# nx.draw_networkx(graph, pos)
nx.draw_networkx_edge_labels(graph, pos, edge_labels=edge_labels, font_color='red')

label_map = {}
for node in network["nodes"]:
    label_map[node["eq"]] = node["label"]

nx.draw_networkx(
    graph, pos,
    labels=label_map,
    node_size=1200, node_color='cyan', 
    width=1, arrowstyle="-|>", arrowsize=16,
    with_labels=True)

# nx.draw(graph, labels=label_map, with_labels=True);

# plt.savefig("network.png", format="PNG")
plt.show()

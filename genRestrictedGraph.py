import matplotlib.pyplot as plt
import networkx as nx
import random

N = 1000
k = 10
roe = 2

# G = nx.connected_caveman_graph(8,10)
# G = nx.newman_watts_strogatz_graph(20,5,0)
DegS = [k + roe*round(i) for j in range(N) for i in [random.random()*2-1]]
print(DegS)

G = nx.random_degree_sequence_graph(DegS)


nx.draw_spectral(G)
# nx.draw_spectral(G)

A = nx.adjacency_matrix(G)

print(A.todense())



plt.show()

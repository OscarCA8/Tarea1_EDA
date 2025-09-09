import numpy as np
import matplotlib.pyplot as plt
import umap

data = np.load("/home/pbn/Documents/EDA/data_eda.npy")
queries = np.load("/home/pbn/Documents/EDA/queries_eda.npy")
query_id = 0 #Aquí podemos editar el valor de query_id para poner el numero de consulta que queremos mostrar
q = queries[query_id]

def euclidean_distance(a, b):
    return np.sqrt(((a - b) ** 2).sum(axis=1))

def search_exact(query, data, m):
    dist = euclidean_distance(data, query)
    inds = np.argsort(dist)[:m]
    return inds

def search_with_clusters(query, data, clusters, m):
    dist_c = euclidean_distance(clusters, query)
    c_sorted = np.argsort(dist_c)

    candidates = []
    for c_id in c_sorted:
        dist_to_centroid = euclidean_distance(data, clusters[c_id])
        inds = np.argsort(dist_to_centroid)[:m]  
        candidates.extend(list(inds))
        if len(set(candidates)) >= m:
            break

    candidates = list(dict.fromkeys(candidates))[:m]
    return np.array(candidates)

m = 16  #Aquí podemos editar el valor de m para poner la cantidad de vecinos que queremos mostrar

inds_exact = search_exact(q, data, m)
clusters = np.array([
    [0.809997, 0.479773],
    [0.525087, 0.842694],
    [0.733819, 0.140401],
    [0.182114, 0.824761],
    [0.119629, 0.400015],
    [0.440517, 0.493425],
    [0.285572, 0.13858],
    [0.856857, 0.827084]
])
inds_approx = search_with_clusters(q, data, clusters, m)

plt.figure(figsize=(12,5))

# Vecinos exactos, sin errores pero a nivel computacional son muy costosos
plt.subplot(1,2,1)
plt.scatter(data[:,0], data[:,1], c="lightgray", s=10, label="Datos")
plt.scatter(q[0], q[1], c="blue", s=80, marker="*", label="Consulta")
plt.scatter(data[inds_exact,0], data[inds_exact,1], c="green", s=30, label=f"Top-{m} exactos")
plt.title(f"Vecinos sin errores pero costosos (m={m})")
plt.legend()

# Vecinos aproximados, aqui estamos usando clusters lo que lo hace menos costoso pero tiene los errores de aproximacion
plt.subplot(1,2,2)
plt.scatter(data[:,0], data[:,1], c="lightgray", s=10, label="Datos")
plt.scatter(q[0], q[1], c="blue", s=80, marker="*", label="Consulta")
plt.scatter(data[inds_approx,0], data[inds_approx,1], c="orange", s=30, label=f"Top-{m} aprox")
plt.scatter(clusters[:,0], clusters[:,1], c="red", marker="x", s=60, label="Centroides")
plt.title(f"Vecinos aproximados con clusters (clusters, m={m})")
plt.legend()
plt.suptitle("Visualización de búsqueda de vecinos")
plt.show()
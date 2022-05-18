import matplotlib.pyplot as plt
import numpy as np

HASH_NUM = 30
bands = [2, 3, 5, 6, 10, 15]
colors = ['#d76364', '#c497b2', '#f1d77e', '#b1ce46', '#5f97d2', '#9394e7']

for i in range(len(bands)):
    b = bands[i]
    r = int(HASH_NUM / b)
    file_name = f"res_band_{b}.txt"

    results = np.loadtxt(file_name)
    exper_prob = results[:, 0]
    jaccard = results[:, 1]
    # theory_prob = results[:, 2]

    # plt.plot(jaccard, exper_prob, color='blue', marker='o')
    plt.scatter(jaccard, exper_prob, marker='o', edgecolor='#9dc3e7', facecolors='none')
    the_label = f"b={b}, r={r}"
    s = np.linspace(0, 1, 200)
    theory_p = 1 - (1 - s**r) ** b
    plt.plot(s, theory_p, color=colors[i], linewidth=3, label=the_label)

plt.legend(loc='upper left')
plt.xlabel("Jaccard Similarity")
plt.ylabel("Probability")
plt.xticks(np.arange(0, 1.1, step=0.1))
plt.savefig('result.png')
plt.show()
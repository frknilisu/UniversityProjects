import random

K = 400

pos_train = open("./Train/positive-train", "r").readlines()
neg_train = open("./Train/negative-train", "r").readlines()
notr_train = open("./Train/notr-train", "r").readlines()

pos = random.sample(pos_train, k=K)
neg = random.sample(neg_train, k=K)
notr = random.sample(notr_train, k=K)

T = []
for p in pos:
	T.append((p, "pos"))
for n in neg:
	T.append((n, "neg"))
for nt in notr:
	T.append((nt, "notr"))

random.shuffle(T)
f = open("test_tweet", "w")
h = open("test_sentiment", "w")
for t, c in T:
	f.write(t)
	h.write(c+"\n")
f.close()
h.close()
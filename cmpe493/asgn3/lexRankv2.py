#!/usr/bin/python3
# -*- coding: utf-8 -*-

import re
import os
import sys
import numpy as np
from math import sqrt, log
from collections import Counter, OrderedDict
from rouge import Rouge


NUM_OF_NEWS = 1000
PATTERN = re.compile(r'[A-Za-z]+')
FILE_FORMAT = '%d.txt'

threshold = 0.10
error_tolerance = 0.00001
teleportation_rate = 0.15


global_df = Counter()

class News:
	"""
	represent a News via one txt file
	"""
	def __init__(self, id, n, k):
		"""
		:param id: id of the news
		"""
		self.id = id
		self.n = n
		self.k = k


	def build_graph(self):
		matrix = np.zeros((self.n, self.n), dtype=np.float64)		# Create an array of all zeros

		for i in range(self.n):
			for j in range(self.n):
				cos_sim = cosine_similarity(self.news_sentences[i], self.news_sentences[j])
				#print(cos_sim)
				if cos_sim > threshold:
					matrix[i][j] = 1
				else:
					matrix[i][j] = 0

		#print(matrix)

		for row in range(self.n):
			matrix[row] = np.divide(matrix[row], sum(matrix[row, :]))
				
		matrix = np.multiply(matrix, 1-teleportation_rate)
		matrix = np.add(matrix, teleportation_rate/self.n)

		#print(matrix)

		L = power_method(M=matrix, N=news.n, epsilon=error_tolerance)
		return L
		
class Sentence:

	def __init__(self, terms):
		"""
		:param id: id of the news
		"""
		self.terms = terms

"""
	read one txt file with filename
"""
def parse_file(base, filename):
	filepath = base + "/" + filename
	lines = open(filepath, encoding="ISO-8859-1").readlines()
	n = lines.index("\n")
	k = len(lines) - n - 1

	news = News(id, n, k)
	tokens = list(map(lambda line: Counter(list(map(lambda term: term.strip(), line.split(" ")))), lines))

	news.news_sentences = []
	for i in range(n):
		sentence = Sentence(tokens[i])
		sentence.terms += tokens[i]
		sentence.raw = lines[i]
		news.news_sentences.append(sentence)

	news.summary_sentences = []
	for i in range(k):
		sentence = Sentence(tokens[i])
		sentence.terms += tokens[n+i+1]
		sentence.raw = lines[n+i+1]
		news.summary_sentences.append(sentence)


	local_freqs = Counter()
	for sent in news.news_sentences:
		local_freqs += sent.terms

	news.tf = local_freqs

	return news

		

def cosine_similarity(sentence1, sentence2):
	"compute cosine similarity of v1 to v2: (v1 dot v2)/{||v1||*||v2||)"
	v1 = sentence1.tf_idf
	v2 = sentence2.tf_idf
	intersection = set(v1.keys()) & set(v2.keys())
	numerator = sum([v1[x] * v2[x] for x in intersection])
	sum1 = sum([v1[x]**2 for x in v1.keys()])
	sum2 = sum([v2[x]**2 for x in v2.keys()])
	denominator = sqrt(sum1) * sqrt(sum2)
	if not denominator:
		return 0.0
	else:
		return round(float(numerator) / denominator, 3)



def power_method(M, N, epsilon):
	p0 = np.full((N, 1), 1/N)		# Create a constant vector
	p1 = np.zeros((N, 1))			# Create a zero vector
	t = 0
	while True:
		t += 1
		p1 = (M.T).dot(p0)
		delta = np.linalg.norm(p1-p0)
		if delta < epsilon:
			break

		p0 = p1

	return p1

if __name__ == '__main__':

	base = "./Dataset"
	num_of_files = 1000
	# read all txt files
	for i in range(num_of_files):
		filename = '{}.txt'.format(i+1)
		news = parse_file(base, filename)
		global_df += Counter(set(news.tf.keys()))

	for term in global_df.keys():
		global_df[term] = log(num_of_files/global_df[term])

	#print(global_df)
	

	news = parse_file(sys.argv[1], sys.argv[2])
	num_of_terms = sum(news.tf.values())
	for sent in news.news_sentences:
		sent.tf_idf = Counter()
		for term, count in sent.terms.items():
			tf = log(1+(count / num_of_terms))
			idf = global_df[term]
			sent.tf_idf[term] = tf*idf

	LexRank = news.build_graph()
	np.set_printoptions(precision=3, suppress=True, formatter={'float': '{: 0.3f}'.format})
	LexRank = np.asarray(LexRank).reshape(-1)
	print(LexRank)

	d = {LexRank[index]: index for index in range(len(LexRank))}
	od = OrderedDict(sorted(d.items(), reverse=True))
	lod = list(od.items())

	hypothesis = ""
	reference = ""
	K = 3
	for i in range(K):
		k, v = lod[i]
		hypothesis += news.news_sentences[v].raw + "\n"
		reference += news.summary_sentences[i].raw + "\n"
	

	rouge = Rouge()
	scores = rouge.get_scores(hypothesis, reference)
	print(scores)
	




	

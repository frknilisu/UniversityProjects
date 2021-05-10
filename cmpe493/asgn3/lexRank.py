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


def LexRank(sentences, idfs):
	n = len(sentences)
	matrix = np.zeros((n, n), dtype=np.float64)		# Create an array of all zeros

	for i in range(n):
		for j in range(n):
			cos_sim = idf_modified_cosine(sentences[i], sentences[j], idfs)
			if cos_sim > threshold:
				matrix[i][j] = 1
			else:
				matrix[i][j] = 0


	for row in range(n):
		matrix[row] = np.divide(matrix[row], sum(matrix[row, :]))
			
	matrix = np.multiply(matrix, 1-teleportation_rate)
	matrix = np.add(matrix, teleportation_rate/n)

	L = power_method(M=matrix, n=n, e=error_tolerance)
	return L


def power_method(M, n, e=0.01):
	p0 = np.full((n, 1), 1/n)		# Create a constant vector
	p1 = np.zeros((n, 1))			# Create a zero vector
	t = 0
	while True:
		t += 1
		t_M = np.transpose(M)
		p1 = np.matmul(t_M, p0)
		delta = np.linalg.norm(p1-p0)
		if delta < e: break
		p0 = p1

	return np.asarray(p1).reshape(-1)

def get_news():
	for i in range(NUM_OF_NEWS):
		filePath = os.path.join('./Dataset', FILE_FORMAT % (i+1))
		with open(filePath, 'r') as news:
			text = news.read()
			index_of_break = text.find('\n\n')
			yield i, text[:index_of_break - 1], text[index_of_break + 2:]

def get_idfs():
	"""
	calculates idf values of all words, values are logged on base `e`
	:return: Counter object of idf values
	"""
	df_Counter = Counter()  # will be used for one idf list for all! values are not inverted in this one
	for news_id, news_sentences, summary_sentences in get_news():
		sentence_words_set = set(re.findall(PATTERN, news_sentences))  # find all words in the sentence and form a Counter
		df_Counter.update(sentence_words_set)  # for counting document frequencies of words
	
	# invert df to get idf as : for each word -> df  (number of all documents)/df
	for df_key in df_Counter:
		df_Counter[df_key] = log(NUM_OF_NEWS / df_Counter[df_key])
	
	return df_Counter  # this is now idf_Counter

def idf_modified_cosine(s1, s2, idfs):
	"compute cosine similarity of v1 to v2: (v1 dot v2)/{||v1||*||v2||)"
	tf1 = Counter(re.findall(PATTERN, s1))
	tf2 = Counter(re.findall(PATTERN, s2))
	nominator = 0
	denom_left_sqr = 0
	denom_right_sqr = 0
	for w in tf1.keys() | tf2.keys():  # union
		nominator += tf1[w] * tf2[w] * idfs[w] ** 2
		denom_left_sqr += (tf1[w] * idfs[w]) ** 2
		denom_right_sqr += (tf2[w] * idfs[w]) ** 2
	try:
		return round(nominator / (sqrt(denom_left_sqr) * sqrt(denom_right_sqr)),3)
	except:
		return 0

if __name__ == '__main__':

	idfsCounter = get_idfs()
	
	##################### query processing ###############################
	
	with open(os.path.join(sys.argv[1], sys.argv[2]), 'r') as query:
		text = query.read()
		index_of_break = text.find('\n\n')
		q_sentences = text[:index_of_break - 1].splitlines()
	
	
	lexRank = LexRank(q_sentences, idfs=idfsCounter)
	np.set_printoptions(precision=3, suppress=True, formatter={'float': '{: 0.3f}'.format})
	print(lexRank)

	d = {lexRank[index]: index for index in range(len(lexRank))}
	od = OrderedDict(sorted(d.items(), reverse=True))
	lod = list(od.items())

	
	hypothesis = ""
	reference = ""
	K = 3
	for i in range(K):
		k, v = lod[i]
		hypothesis += q_sentences[v] + "\n"
		reference += news.summary_sentences[i].raw + "\n"
	

	rouge = Rouge()
	scores = rouge.get_scores(hypothesis, reference)
	print(scores)
	"""
	




	

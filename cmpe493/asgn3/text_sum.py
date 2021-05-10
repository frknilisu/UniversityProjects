#!/usr/bin/python3
# -*- coding: utf-8 -*-

import re
import sys
import os
import numpy as np
from collections import Counter, OrderedDict
from math import log, sqrt
from rouge import Rouge
from operator import itemgetter

NUM_OF_NEWS = 1000
DATASET_PATH = sys.argv[1]
FILENAME_FORMAT = '%d.txt'

TOKEN_PATTERN = re.compile(r'[A-Za-z]+')

def get_news():
	"""
	read txt files
	:return: None
	yields a news at a time
	"""
	for i in range(1, NUM_OF_NEWS+1):
		filepath = os.path.join(DATASET_PATH, FILENAME_FORMAT % i)
		with open(filepath, 'r') as news:
			text = news.read()
			seperated_index = text.find("\n\n")
			yield i, text[:seperated_index], text[seperated_index+2:]


def get_idfs():
	"""
	calculate idf values of all words, log 'e' based
	:return: Counter object of idf values
	"""
	dfCounter = Counter()
	for id, news_sent, summary_sent in get_news():
		word_set = set(re.findall(TOKEN_PATTERN, news_sent))    # findall words in news sentences
		dfCounter.update(word_set)

	# invert df to idf
	for word in dfCounter.keys():
		dfCounter[word] = log(NUM_OF_NEWS/dfCounter[word])

	return dfCounter    # now, idfCounter


def idf_modified_cosine(s1, s2, idfs):
	"""
	modified tf-idf-cosine similariy
	:param s1: sentence1
	:param s2: sentence2
	:param idfs: Counter of idf's
	:return: the cosine similarity value
	"""
	tf1 = Counter(re.findall(TOKEN_PATTERN, s1))
	tf2 = Counter(re.findall(TOKEN_PATTERN, s2))

	nominator = 0
	denom_left = 0
	denom_right = 0
	for word in (tf1.keys() | tf2.keys()):
		nominator += tf1[word] * tf2[word] * (idfs[word]**2)
		denom_left += (tf1[word]*idfs[word])**2
		denom_right += (tf2[word] * idfs[word]) ** 2

	try:
		return (nominator / (sqrt(denom_left) * sqrt(denom_right)))
	except:
		return 0


def magnitude(x):
	"""
	length of x
	:param x: a vector (np.array)
	:return: length of given vector
	"""
	y = np.sqrt(np.transpose(x).dot(x))
	return y[0][0]


def power_method(cos_matrix, n, e=0.00001):
	"""
	power iteration
	:param cos_matrix: the transition matrix
	:param n: the size of matrix
	:param e: max error allowed, error tolerance
	:return: the resulting vector
	"""
	p_last = np.full((n, 1), 1 / n)  # Create a constant vector
	p_curr = np.zeros((n, 1))  # Create a zero vector
	p_curr = p_last

	t_cos_matrix = np.transpose(cos_matrix)
	while True:
		p_curr = np.matmul(t_cos_matrix, p_curr)
		sigma = magnitude(p_curr - p_last)
		if sigma < e: break
		p_last = p_curr

	return p_curr


def add_teleportation_effect(transition_matrix, teleportation_rate=0.15):
	"""
	adds teleportation to transition matrix
	:param transition_matrix: the transition matrix
	:param teleportation_rate: the teleportation rate
	:return: modified transition matrix
	"""
	n = transition_matrix.shape[0]
	transition_matrix = np.multiply(transition_matrix, 1 - teleportation_rate)
	transition_matrix = np.add(transition_matrix, teleportation_rate / n)
	return transition_matrix


def LexRank(sentences, idfs, t=0.1):
	"""
	lexrank algorithm
	:param sentences: list of sentences of the news
	:param idfs: Counter of idf's
	:param t: the threshold
	:return: vector of lexrank scores of given sentences
	"""
	n = len(sentences)    
	cos_matrix = [[0 for i in range(n)] for j in range(n)]  # Create a matrix of all zeros
	degree = [0] * n

	for i in range(n):
		for j in range(n):
			cos_similarity = idf_modified_cosine(sentences[i], sentences[j], idfs)
			if cos_similarity > t:
				cos_matrix[i][j] = 1
				degree[i] += 1
			else:
				cos_matrix[i][j] = 0

	for i in range(n):
		for j in range(n):
			try:
				cos_matrix[i][j] = cos_matrix[i][j] / degree[i]
			except:
				cos_matrix[i][j] = 0

	cos_matrix = np.array(cos_matrix)
	cos_matrix = add_teleportation_effect(cos_matrix)
	l = power_method(cos_matrix, n)
	return l



if __name__ == '__main__':
	idfCounter = get_idfs()
	
	with open(os.path.join(DATASET_PATH, sys.argv[2]), 'r') as query:
		text = query.read()
		seperated_index = text.find("\n\n")
		q_news_sents = text[:seperated_index].splitlines()
		q_summary_sents = text[seperated_index+2:].splitlines()
	

	lexRank = LexRank(q_news_sents, idfCounter)
	np.set_printoptions(precision=6, formatter={'float': '{: 0.6f}'.format}, suppress=True)
	lexRank = np.asarray(lexRank).reshape(-1)
	print(lexRank)

	"""
	d = [(lexRank[index], index) for index in range(len(lexRank))]
	sd = sorted(d, key=itemgetter(0), reverse=True)


	hypothesis = ""
	reference = ""
	K = len(q_summary_sents)
	for i in range(K):
		k, v = sd[i]
		hypothesis += q_news_sents[v] + "\n"
		reference += q_summary_sents[i] + "\n"

	rouge = Rouge()
	scores = rouge.get_scores(hypothesis, reference)
	print(scores)
	"""





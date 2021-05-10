import os
import sys
import numpy as np
from math import sqrt, log
from collections import Counter
from itertools import starmap
from operator import mul
from rouge import Rouge

hypothesis = "the #### transcript is a written version of each day 's cnn student news program use this transcript to he    lp students with reading comprehension and vocabulary use the weekly newsquiz to test your knowledge of storie s you     saw on cnn student news"
reference = "this page includes the show transcript use the transcript to help students with reading comprehension and     vocabulary at the bottom of the page , comment for a chance to be mentioned on cnn student news . you must be a teac    her or a student age # # or older to request a mention on the cnn student news roll call . the weekly newsquiz tests     students ' knowledge of even ts in the news"

threshold = 0.10
error_tolerance = 0.00001
teleportation_rate = 0.15


Vocabs = []
news_list = []
global_tf = Counter()
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
		
class Sentence:
	terms = Counter()
	tf_idf = Counter()

"""
	read one txt file with filename
"""
def parse_file(id):
	global global_tf
	global global_df
	base = "./Dataset"
	filename = '{}.txt'.format(id)
	filepath = base + "/" + filename
	#filepath = os.path.join(os.path.sep, base, filename)
	lines = open(filepath, encoding="ISO-8859-1").readlines()
	n = lines.index("\n")
	k = len(lines) - n - 1
	news = News(id, n, k)

	sentences = list(map(lambda line: Counter(list(map(lambda term: term.strip(), line.split(" ")))), lines))

	news.news_sentences = [Sentence()] * n
	for i in range(n):
		news.news_sentences[i].terms.update(sentences[i])
		#news.news_sentences[i].tf_idf = news.news_sentences[i].terms

	news.summary_sentences = [Sentence()] * k
	for i in range(k):
		news.summary_sentences[i].terms.update(sentences[n+i+1])
		#news.summary_sentences[i].tf_idf = news.summary_sentences[i].terms

	#freqs.append(freq)
	local_freqs = Counter()
	for sent in news.news_sentences:
		local_freqs += sent.terms

	news.tf = local_freqs

	global_tf += local_freqs
	global_df += Counter(set(local_freqs.keys()))

	news_list.append(news)

	print(filename + " is read")


def tf_idf(news):
	num_of_terms = sum(news.tf.values())
	for sent in news.news_sentences:
		for term, count in sent.terms.items():
			tf = log(1+(count / num_of_terms))
			idf = log(len(news_list)/global_df[term])
			sent.tf_idf[term] = tf*idf
		

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

def vec_diff(v1, v2):

	#for i in range(len(v1)):
	#	print(v1[i], v2[i])
	return sqrt(sum([(v1[i] - v2[i])**2 for i in range(len(v1))]))

def matrix_vec_mult(X, Y):
	
	"""
	# iterate through rows of X
	for i in range(len(X)):
	   # iterate through columns of Y
	   for j in range(len(get_column(Y, 0))):
	       # iterate through rows of Y
	       for k in range(len(Y)):
	           result[i][j] += X[i][k] * Y[k][j]
	return result
	"""

	#return [[sum(a*b for a,b in zip(X_row,Y_col)) for Y_col in zip(*Y)] for X_row in X]
	return [sum(starmap(mul, zip(Y, col))) for col in zip(*X)]



def power_method(M, N, epsilon):
	p0 = [1/N] * N
	p1 = [0] * N
	t = 0
	#print(M)
	#t_M = list(map(list, zip(*M)))
	#print(t_M)
	while True:
		t += 1
		t_M = list(map(list, zip(*M)))
		p1 = matrix_vec_mult(t_M, p0)
		print(M)
		print(t_M)
		print(p0)
		print(p1)
		print("\n---------------------------------------------------\n")
		delta = vec_diff(p1, p0)
		print(delta)
		if delta < epsilon:
			break

		p0 = p1

	return p1


def build_graph(news):
	matrix = np.zeros((news.n, news.n))		# Create an array of all zeros

	for i in range(news.n):
		for j in range(news.n):
			cos_sim = cosine_similarity(news.news_sentences[i], news.news_sentences[j])
			if cos_sim > threshold:
				matrix[i][j] = 1
			else:
				matrix[i][j] = 0

	for i in range(news.n):
		for j in range(news.n):
			matrix[i][j] /= sum(matrix[i])
			matrix[i][j] *= 1-teleportation_rate
			matrix[i][j] += teleportation_rate/news.n


	L = power_method(M=Matrix, N=news.n, epsilon=error_tolerance)
	return L


if __name__ == '__main__':
	global Vocabs
	num_of_files = 3
	# read all txt files
	for i in range(num_of_files):
		parse_file(i+1)
	Vocabs = set(global_tf.keys())

	for news in news_list:
		tf_idf(news)

	r = build_graph(news_list[index-1])
	#print(r)

	rouge = Rouge()
	scores = rouge.get_scores(hypothesis, reference)
	print(scores)




	

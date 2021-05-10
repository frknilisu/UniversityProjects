import time
from ast import literal_eval
import tokenizer
import sys

INT_MAX = sys.maxsize
INT_MIN = -sys.maxsize -1

words_map = {}
inverted_index = {}
query_tokens = []
words = []

def start():
	global words_map
	global inverted_index
	global query_tokens
	global words

	print("initializing..")
	matrix = open("./first", "r").read()
	words_map = literal_eval(str(matrix))

	matrix = open("./second", "r").read()
	inverted_index = literal_eval(str(matrix))

	switcher = {
		1: conjunctiveQry,
		2: phraseQry,
		3: proximityQry
	}

	while True:
		query = str(input("give a query: "))
		query_tokens = tokenizer.stemmize(tokenizer.stopwordsRemove(tokenizer.tokenize(query)))
		query_type = int(query_tokens[0]) 		# type of query from first token
		#print(query_tokens)
		func = switcher.get(query_type)
		words = query_tokens[1:]
		func()

"""
	get the article ids which are contain the given word 
"""
def get_article_list(word):
	word_id = words_map[word]
	article_ids=[]
	if word_id:
		word_id=int(word_id)
		article_ids=list(inverted_index[word_id].keys())
	
	return article_ids

"""
	find the intersection of article ids of all word in the given word list
"""
def find_intersection(words):
	joint_articles = get_article_list(words[0])
	for i in range(1, len(words)):
		joint_articles=set(joint_articles).intersection(get_article_list(words[i]))
	return sorted(joint_articles)

"""
	solve conjunctive query
"""
def conjunctiveQry():
	#print("hello conjunctiveQry")
	article_ids = find_intersection(words)
	print(article_ids)
	
"""
	convert phrase query to proximity query by adding /0 between each words
	solve the query as proximity query with /0 distance between for all words
"""
def phraseQry():
	global words
	#print("hello phraseQry")
	result = []
	temp = []
	for i in range(len(words)-1):
		temp.append(words[i])
		temp.append(0)

	temp.append(words[len(words)-1])
	words = temp
	print(words)

	joint_articles = find_intersection(words[0:len(words):2])
	for article_id in joint_articles:
		if prox_rec(article_id, 0, None, None):
			result.append(article_id)
		
	print(result)
	

"""
	match end index of the current word with start index of the next word
	if it matches go to next word by recursion
	else return false means the query not satisfy for this article
"""
def prox_rec(article_id, word_id, dict_bf, conjunction_point):

	if word_id+2 > len(words):
		return True

	w1 = words[word_id]
	k = int(words[word_id+1])
	w2 = words[word_id+2]
	word_id1 = words_map[w1]
	word_id2 = words_map[w2]
	list1 = inverted_index[word_id1][article_id]
	list2 = inverted_index[word_id2][article_id]

	dict_af = findMinDiff(list1, list2, k)


	if not dict_af:
		return False

	if dict_bf is None:
		return prox_rec(article_id, word_id+2, dict_af, None)
	else:

		if conjunction_point is None:

			flag = False
			for (s1, e1) in dict_bf.keys():
				for (s2, e2) in dict_af.keys():
					if e1 == s2:
						flag = prox_rec(article_id, word_id+2, dict_af, e2)

			return flag

		else:

			flag = False
			for (s2, e2) in dict_af.keys():
				if conjunction_point == s2:
					flag = prox_rec(article_id, word_id+2, dict_af, e2)

			return flag


"""
	first find the joint articles of query words
	solve proximity query by recursion
"""
def proximityQry():
	#print("hello proximityQry")
	result = []
	joint_articles = find_intersection(words[0:len(words):2])
	for article_id in joint_articles:
		if prox_rec(article_id, 0, None, None):
			result.append(article_id)
		
	print(result)
		
"""
	find all pairs that distance between words are smaller than or equal to given k value
	using merge algorithm
"""
def findMinDiff(array1, array2, k):
	i, j = 0, 0
	minDiff = INT_MAX
	ret = {}

	while ( i < len(array1) and j < len(array2) ):
		if( array1[i] == array2[j] ):
			ret[(array1[i], array2[j])]=0
			return 0
		elif( array1[i] < array2[j] ):
			diff = array2[j] - array1[i]
			if diff <= k+1:
				ret[(array1[i], array2[j])]=diff
			i+=1
		else:
			j+=1

	if( i < len(array1) ): 		#array1 has some more elements
		j-=1 		# j has reached the end, move it to last element
		while ( i < len(array1) ):
			if( array1[i] < array2[j] ):
				diff = array2[j] - array1[i]
				if diff <= k+1:
					ret[(array1[i], array2[j])]=diff
			i+=1
	else: 			#array2 has some more elements
		i-=1
		while ( j < len(array2) ):
			if( array1[i] < array2[j] ):
				diff = array2[j] - array1[i]
				if diff <= k+1:
					ret[(array1[i], array2[j])]=diff
			j+=1

	return ret

if __name__ == '__main__':
	start_time = time.time()
	start()
	print("--- %s seconds ---" % (time.time() - start_time))
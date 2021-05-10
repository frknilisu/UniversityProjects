import time
import re
from bs4 import BeautifulSoup
import tokenizer
from collections import Counter

articles = []			# list of all Article objects
all_pure_words = []		# list of all tokenized, stopwordsRemoved and stemmized words

"""
	class of Article
"""
class Article:
	news_id = -1
	title = ""
	body = ""
	tokens = []
	words = []


"""
	list top 20 most frequent terms of given list
"""
def most_common2(lst):
	data = Counter(lst)
	return [word for (word, counter) in data.most_common(20)]

"""
	read one sgml file with filename
"""
def readFile(filename):
	base="./Dataset/"
	filepath=base+filename
	content=open(filepath, encoding="ISO-8859-1").read()
	return content

"""
	read all sgml files
"""
def readFiles():
	for i in range(22):
		filename="reut2-0"+'{0:02d}'.format(i)+".sgm"
		parseFile(readFile(filename))
		print(filename + " is read")

"""
	parse title, body and news id of articles using beautifulsoup library
	create a new Article object
"""
def parseFile(content):
	soup = BeautifulSoup(content, "html.parser")
	docs = soup.find_all("text")
	for i in range(len(docs)):
		doc = docs[i]
		article = Article()
		article.news_id = doc.parent.get("newid")
		title = doc.find("title")
		if title:
			article.title = title.text
		body = doc.find("body")
		if body:
			article.body = body.text
		articles.append(article)


def start():
	global all_pure_words
	#uniq_tokens = []
	readFiles()
	#parseFile(readFile("reut2-003.sgm"))
	#print(len(articles))
	sum1, sum2 = 0, 0
	for article in articles:
		article.tokens = tokenizer.tokenize(article.title + " " + article.body)		# tokenize article title and body
		#sum1 += len(article.tokens)
		#uniq_tokens = list(set(uniq_tokens)|set(article.tokens))

		article.words = tokenizer.stopwordsRemove(article.tokens)					# remove stopwords from tokens
		article.words = tokenizer.stemmize(article.words, article.news_id)			# stemmize all words
		sum2 += len(article.words)
		all_pure_words = list(set(all_pure_words)|set(article.words)) 				# merge all words into a list without duplicates
		


	#print(sum1, sum2)
	#print("{}, {}".format(len(uniq_tokens), len(all_pure_words)))
	#print(most_common2(uniq_tokens))
	#print(most_common2(all_pure_words))
	print("in total {} words processed".format(sum2))
	all_pure_words.sort()
	print("in total {} words extracted".format(len(all_pure_words)))

	f = open("./first", 'w')
	hash_map = dict(enumerate(all_pure_words))			# enumare all words with number
	inverse_map = {v: k for k, v in hash_map.items()}	# word to int dictionary of all words
	f.write(str(inverse_map))
	f.close()

	inverted_index = dict((inverse_map[key], value) for (key, value) in tokenizer.inverted_index.items())
	#inverted_index = sorted(inverted_index.items())
	f = open("./second", 'w')
	f.write(str({key:inverted_index[key] for key in sorted(inverted_index)}))
	f.close()

if __name__ == '__main__':
	start_time = time.time()
	start()
	print("--- %s seconds ---" % (time.time() - start_time))
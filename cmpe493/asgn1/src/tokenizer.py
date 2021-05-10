#tsplit2("I said, 'what're you? Crazy?'\" said Sandowsky. \"I can't afford to do that.")
#filters='!"#$%&()*+,-./:;<=>?@[\\]^_`{|}~\t\n',

import re
from stemmer import PorterStemmer
stemmer = PorterStemmer()

# keep positional index
inverted_index = {}

"""
	split the string from any char in the given seperator string
"""
def tsplit(s, sep):
	stack = [s]
	for char in sep:
		pieces = []
		for substr in stack:
			pieces.extend(substr.split(char))
		stack = pieces
	return stack

"""
	alternative tokenize function with regex
"""
def tsplit2(string, *delimiters):
	pattern = r'[\s!"#$%&()\*\+,-\.:;<=>\?@[\\]\^_`{|}~\t\n]\s*'
	#pattern = '|'.join(map(re.escape, delimiters))
	#print(pattern)
	return re.split(pattern, string)

"""
	split the string from chars in the delimiters
"""
def tokenize(raw):
	filters=' !\"#$%&()*+,-./:;<=>?@[\\]^_`{|}~\t\n'
	raw = raw.replace('\'', '')
	tokens = tsplit(raw, filters)
	tokens = [token.strip() for token in tokens]
	tokens = ' '.join(tokens).split()
	tokens = [token.lower() for token in tokens]
	return tokens

"""
	remove stopwords from tokens
"""
def stopwordsRemove(tokens):
	words = tokens
	stopwords = open("./stopwords.txt").read().splitlines()
	for word in list(words):
		if word in stopwords:
			words.remove(word)
	return words

"""
	stemmize all words
	add the stemmed word to inverted index dictionary
"""
def stemmize(words, news_id=None):
	global inverted_index
	stemmed_words = []
	for i in range(len(words)):
		word = words[i]
		stemmed_word = stemmer.stem(word, 0,len(word)-1)
		stemmed_words.append(stemmed_word)

		if news_id:
			news_id = int(news_id)
			if not stemmed_word in inverted_index.keys():
				inverted_index[stemmed_word] = {}
				inverted_index[stemmed_word][news_id] = []
			elif not news_id in inverted_index[stemmed_word].keys():
				inverted_index[stemmed_word][news_id] = []
			else:
				pass

			inverted_index[stemmed_word][news_id].append(i)

	return stemmed_words


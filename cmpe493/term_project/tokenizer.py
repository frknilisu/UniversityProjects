#tsplit2("I said, 'what're you? Crazy?'\" said Sandowsky. \"I can't afford to do that.")
#filters='!"#$%&()*+,-./:;<=>?@[\\]^_`{|}~\t\n',

import re
from stemmer import PorterStemmer
stemmer = PorterStemmer()


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

def tokenizeG(raw, re_pattern=None):
		tokens = []
		if re_pattern is not None:
			ptrn = re_pattern
		else:
			ptrn = r'[\-a-z]+'
		for tkn_match in re.finditer(pattern=ptrn, string=raw):
			tkn = tkn_match.group()
			tokens.append(tkn)
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
def stemmize(words):
	stemmed_words = []
	for i in range(len(words)):
		word = words[i]
		stemmed_word = stemmer.stem(word, 0,len(word)-1)
		stemmed_words.append(stemmed_word)

	return stemmed_words


from __future__ import print_function

from pprint import pprint
from time import time
import logging
import sys, os
import csv
import pandas as pd
import numpy as np
import random
import gensim
import nltk

from sklearn.feature_extraction.text import CountVectorizer
from sklearn.feature_extraction.text import TfidfVectorizer
from sklearn.feature_extraction.text import TfidfTransformer

from sklearn.model_selection import train_test_split
from sklearn.model_selection import GridSearchCV

from sklearn.linear_model import LogisticRegression, SGDClassifier
from sklearn.metrics import accuracy_score, f1_score
from sklearn.decomposition import PCA
from sklearn.pipeline import Pipeline, FeatureUnion
from sklearn.preprocessing import StandardScaler
from sklearn.neural_network import MLPClassifier
from sklearn.neighbors import KNeighborsClassifier
from sklearn.svm import SVC, LinearSVC
from sklearn.tree import DecisionTreeClassifier
from sklearn.ensemble import RandomForestClassifier, AdaBoostClassifier, ExtraTreesClassifier
from sklearn.naive_bayes import MultinomialNB

from TurkishStemmer import TurkishStemmer

from nltk.tokenize import TweetTokenizer

#nltk.download('punkt')

# stems given tokens in the word list
def stem(word_list):

	stemmed_list = []
	stemmer = TurkishStemmer()

	for word in word_list:
		stemmed_list.append(stemmer.stem(word))

	#print(stemmed_list)
	return stemmed_list

def tokenizer(tweet):
	tknzr = TweetTokenizer(preserve_case=False, reduce_len=True)
	tokens = tknzr.tokenize(tweet)
	#tokens = nltk.word_tokenize(tweet.lower())
	words = stem(tokens)
	return words

names = [
			"Nearest Neighbors",
			"Linear SVM", 
			"RBF SVM",
			"Decision Tree",
			"Random Forest",
			"Neural Net",
			"AdaBoost",
			"Naive Bayes",
			"Logistic Regression",
			# "QDA"
		 ]

classifiers = [
	KNeighborsClassifier(n_neighbors=3, weights='distance', algorithm='auto'),
	LinearSVC(C=0.5),
	SVC(kernel='rbf', C=100, gamma=0.001),
	# GaussianProcessClassifier(1.0 * RBF(1.0)),
	DecisionTreeClassifier(criterion = "gini", max_depth=5, min_samples_leaf=8),
	RandomForestClassifier(n_estimators=200, max_depth=3, random_state=0),
	MLPClassifier(alpha=0.5),
	AdaBoostClassifier(),
	MultinomialNB(),
	LogisticRegression(random_state=0),
	# QuadraticDiscriminantAnalysis()
]

classifier_scores = [
	68.80616174582799, 
	70.17543859649122, 
	62.55883611467694, 
	52.417629439452284, 
	43.04664099272571, 
	70.04706889174155, 
	57.552417629439454, 
	67.18014548566539, 
	67.3940949935815
]


pos_words = open("./Train/positive-words.txt", "r").read().splitlines()
neg_words = open("./Train/negative-words.txt", "r").read().splitlines()
_stopwords = open("./Train/stopwords.txt", "r").read().splitlines()

data = []
data_labels = []
with open("./Train/positive-train", encoding="utf8") as f:
	for i in f: 
		data.append(i)
		data_labels.append('pos')

		for pos_word in pos_words:
			if pos_word in i:
				data.append(i)
				data_labels.append('pos')
				break
 
with open("./Train/negative-train", encoding="utf8") as f:
	for i in f: 
		data.append(i)
		data_labels.append('neg')

		for neg_word in neg_words:
			if neg_word in i:
				data.append(i)
				data_labels.append('neg')
				break

with open("./Train/notr-train", encoding="utf8") as f:
	for i in f: 
		data.append(i)
		data_labels.append('notr')


tfidf_vectorizer = TfidfVectorizer(min_df=5,
							 tokenizer=tokenizer,
							 analyzer = 'word',
							 max_df = 0.8,
							 norm='l2',
							 sublinear_tf=True,
							 use_idf=True,
							 lowercase=True,
							 strip_accents='ascii',
							 stop_words=_stopwords,)

def my_test(X, y):
	X_test = []
	with open(sys.argv[1], encoding="utf8") as f:
		X_test = f.read().splitlines()


	"""
	y_test = []
	with open("./test_sentiment", encoding="utf8") as f:
		y_test = f.read().splitlines()
	"""

	X_test = tfidf_vectorizer.transform(X_test)

	X_train, _X_test, y_train, _y_test  = train_test_split(
		X, 
		y,
		train_size=0.8,
		test_size=0.2, 
		random_state=1234)

	return X_train, X_test, y_train, None


def split_test(X, y):
	X_train, X_test, y_train, y_test  = train_test_split(
		X, 
		y,
		train_size=0.7,
		test_size=0.3, 
		random_state=1234)

	return X_train, X_test, y_train, y_test


def print_sentiment(words):
	f = open("output", "w")
	words = [word.replace("pos","1") for word in words]
	words = [word.replace("neg","-1") for word in words]
	words = [word.replace("notr","0") for word in words]
	for word in words:
		f.write(word+"\n")
	f.close()

y = data_labels
X = tfidf_vectorizer.fit_transform(data)

X_train, X_test, y_train, y_test = my_test(X, y)
#X_train, X_test, y_train, y_test = split_test(X, y)

max_acc = 0
best_classifier = ""
best_preds = []
SENTIMENTS = ['pos', 'neg', 'notr']

prob = [{x:0 for x in SENTIMENTS} for _ in range(X_test.shape[0])] 

for name, classifier, score in zip(names, classifiers, classifier_scores):
		time_begin = time()
		
		try:
			classifier.fit(X_train, y_train)
			#score = classifier.score(X_test, y_test)
			y_preds = classifier.predict(X_test)
			#score = accuracy_score(y_test, y_preds)*100
			#print (name, score, "% accuracy in ", time()-time_begin, "seconds")

			if score>=61:
				for i in range(len(y_preds)):
					sentiment = y_preds[i]
					prob[i][sentiment] += score
	
		except TypeError as e:
			print (name, e)

result = []

for i in range(len(prob)):
	scores = prob[i]
	bigScore, resSent = 0, ""
	posS = scores['pos']
	negS = scores['neg']
	notrS = scores['notr']
	if (posS > negS) and (posS > notrS):
		bigScore = posS
		result.append("pos")
	elif (negS > posS) and (negS > notrS):
		bigScore = negS
		result.append("neg")
	else:
		bigScore = notrS
		result.append("notr")

#score1 = accuracy_score(y_test, result)
#score2 = accuracy_score(y_test, best_preds)
#print(score1, score2)
print_sentiment(result)

"""
print("--------------------------------")
print ("Best: ", best_classifier, max_acc*100, " accuracy in ", time()-time_begin, "seconds")
print("--------------------------------")
for sent in best_preds:
	print(sent)
"""

"""
model = Pipeline([
	('vect', TfidfVectorizer(tokenizer=tokenizer)),
	('clf', LinearSVC()),
])


model.fit(X_train,y_train)
y_preds = extraT.predict(X_test)

# calculate f1
mean_f1 = f1_score(y_test, y_preds, average='micro')
print(mean_f1)


model.set_params(onehot__threshold=3.0)



# this is where you define the values for
# GridSearchCV to iterate over
param_grid = {
	'vect__max_df':[0.8,0.9,1.0],
	'clf__C':[0.1,1.0]
}
param_grid = dict(reduce_dim=[None, PCA(5), PCA(10)],
				   clf=[SVC(), LogisticRegression()],
				   clf__C=[0.1, 10, 100])

# do 3-fold cross validation for each of the 6 possible
# combinations of the parameter values above
grid = GridSearchCV(pipeline, cv=3, param_grid=param_grid)
grid.fit(X_train,y_train)

# summarize results
print("Best: %f using %s" % (grid.best_score_, 
	grid.best_params_))
means = grid.cv_results_['mean_test_score']
stds = grid.cv_results_['std_test_score']
params = grid.cv_results_['params']
for mean, stdev, param in zip(means, stds, params):
	print("%f (%f) with: %r" % (mean, stdev, param))




parameters = {
	'vect__max_df': (0.5, 0.75, 1.0),
	#'vect__max_features': (None, 5000, 10000, 50000),
	'vect__ngram_range': ((1, 1), (1, 2)),  # unigrams or bigrams
	#'tfidf__use_idf': (True, False),
	#'tfidf__norm': ('l1', 'l2'),
	'clf__max_iter': (5,),
	'clf__alpha': (0.00001, 0.000001),
	'clf__penalty': ('l2', 'elasticnet'),
	#'clf__n_iter': (10, 50, 80),
}


parameters = {
	'vect__max_df': (0.5, 0.75, 1.0),
	'tfidf__use_idf': (True, False),
	'reg__alpha': (0.00001, 0.000001),
}


grid_search = GridSearchCV(pipeline, parameters, n_jobs=-1, verbose=1)
t0 = time()
grid_search.fit(X_train, y_train)
print("done in %0.3fs" % (time() - t0))
print()

print("Best score: %0.3f" % grid_search.best_score_)
print("Best parameters set:")
best_parameters = grid_search.best_estimator_.get_params()
for param_name in sorted(parameters.keys()):
	print("\t%s: %r" % (param_name, best_parameters[param_name]))

"""

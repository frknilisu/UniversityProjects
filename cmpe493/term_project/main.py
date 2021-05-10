import os.path
import pickle
import re
import time
from collections import Counter, namedtuple
from math import log, log2
import tokenizer
import argparse

TOPICS = {
	"earn": 0,
	"acq": 1,
	"money-fx": 2,
	"grain": 3,
	"crude": 4
}

counter_tuple = namedtuple(typename='counter_tuple', field_names=['word_counter', 'article_counter', 'article_list'])
probability_tuple = namedtuple(typename='probability_tuple', field_names=['topic_probability', 'word_probability'])
eval_tuple = namedtuple(typename='eval_tuple', field_names=['tp', 'fp', 'fn', 'tn'])

data_collection = {
	"train": {topic: counter_tuple(Counter(), Counter(), []) for topic in TOPICS.keys()},
	"test": {topic: counter_tuple(Counter(), Counter(), []) for topic in TOPICS.keys()}
}

training_set = {}
test_set = {}
num_of_articles = 0

probabilities = {topic: probability_tuple(0.0, {}) for topic in TOPICS.keys()}

Vocabs = Counter()
features = {topic: set() for topic in TOPICS.keys()}
union_features = set()
evaluation = {topic: eval_tuple(0, 0, 0, 0) for topic in TOPICS.keys()}


K = 50

class Tweet:
	id = 0
	type = ""
	topic = ""
	text = ""
	words = Counter()


def unpickle(filename):
	file_ = open(filename, 'rb')
	unpickled = pickle.load(file=file_)
	file_.close()
	return unpickled


def do_pickle(filename):
	file_ = open(filename, 'wb')
	pickle.dump(data_collection, file=file_)
	file_.close()


"""
	read one sgml file with filename
"""
def read_file(filename):
	base = "./Dataset/"
	filepath = base + filename
	content = open(filepath, encoding="ISO-8859-1").read()
	return content


"""
	parse topics, title, body, set_mark and news id of articles via regex
	create a new Article object
	add the article to the set_mark related list
"""
def parse_data(text):
	# reuters
	articles_text = re.findall(pattern=article_pattern, string=text)
	texts = text.splitlines()

	for text in texts:

		# create an article object for each article
		tweet = Tweet()
		id, type, text = text.split('\t')

		# id of article
		match = re.search(pattern=type_id_pattern, string=article_text)
		article.type = match.group(1)
		

		article.text = '\n'.join([title, body])

		_words = tokenizer.tokenize(article.text)  # tokenize article text
		article.words = Counter(_words)

		word_counter, article_counter, article_list = data_collection[article.type][article.topic]
		word_counter += article.words
		article_counter.update(set(article.words))
		article_list.append(article)

# find probabilities of topics
def nb_train():
	global Vocabs
	global num_of_articles
	print("Train NB..")
	num_of_articles = sum(len(training_set[topic].article_list) for topic in TOPICS.keys())
	for topic in TOPICS.keys():
		Vocabs += training_set[topic].word_counter
		p = log(len(training_set[topic].article_list) / num_of_articles)
		probabilities[topic] = probabilities[topic]._replace(topic_probability=p)

# test articles with multinomial naive bayes
def nb_test(fs_mi=False):
	global evaluation
	print("Test NB..")
	score = 0
	test_articles = []
	for topic in TOPICS.keys():
		test_articles.extend(test_set[topic].article_list)

	for article in test_articles:
		max_p = float('-inf')
		estimated_topic = ""
		for topic in TOPICS.keys():
			p = probabilities[topic].topic_probability
			for word in article.words:
				if ((fs_mi and word in union_features) or (not fs_mi)):
					if word not in probabilities[topic].word_probability.keys():
						p_w_of_c = log((training_set[topic].word_counter[word] + 1) / (sum(training_set[topic].word_counter.values()) + len(Vocabs)))
						probabilities[topic].word_probability[word] = p_w_of_c
					p += probabilities[topic].word_probability[word]
			if max_p < p:
				max_p = p
				estimated_topic = topic

		for topic in TOPICS.keys():
			classifier = (topic == estimated_topic)
			truth = (topic == article.topic)
			if classifier and truth:
				evaluation[topic] = evaluation[topic]._replace(tp=evaluation[topic].tp+1)
				score += 1
				continue
			if classifier and not truth:
				evaluation[topic] = evaluation[topic]._replace(fp=evaluation[topic].fp + 1)
				continue
			if not classifier and truth:
				evaluation[topic] = evaluation[topic]._replace(fn=evaluation[topic].fn + 1)
				continue
			if not classifier and not truth:
				evaluation[topic] = evaluation[topic]._replace(tn=evaluation[topic].tn + 1)
				continue

	return score

# feature selection by mutual information
def fs_by_mi():
	print("FS by MI..")
	global union_features
	global features
	N = num_of_articles
	for topic in TOPICS.keys():
		mi_counter = Counter()
		for word in training_set[topic].word_counter:
			N1_ = sum(training_set[topic].article_counter[word] for topic in TOPICS.keys())
			N0_ = N - N1_
			N11 = training_set[topic].article_counter[word]
			N10 = N1_ - N11
			N01 = len(training_set[topic].article_list) - N11
			N00 = N0_ - N01
			N_1 = N11 + N01
			N_0 = N10 + N00

			try:
				mi = ((N11+1)/N)*log2((N*N11+1)/N1_/N_1) \
					 + ((N01+1)/N)*log2((N*N01+1)/N0_/N_1) \
					 + ((N10+1)/N)*log2((N*N10+1)/N1_/N_0) \
					 + ((N00+1)/N)*log2((N*N00+1)/N0_/N_0)
			except ValueError:
				continue

			mi_counter.update({word: mi})


		s = set(key for key, val in mi_counter.most_common(K))
		features[topic] = s
		union_features = union_features.union(s)

# get ready articles for training set and test set
def get_ready():
	global training_set
	global test_set
	global data_collection

	file_path = "./data_collection"
	if os.path.exists(file_path):
		data_collection = unpickle(file_path)
	else:
		filename = "positive-train"
		allTexts = read_file(filename)
		print(filename + " is read")

		text_dump = allTexts.lower()
		parse_data(text=text_dump)
		do_pickle(file_path)

	training_set = data_collection['train']
	test_set = data_collection['test']


if __name__ == '__main__':
	start_time = time.time()
	parser = argparse.ArgumentParser()
	get_ready()

	parser.add_argument('-mi', action='store', dest='fs_by_mi',
					help='flag for Feature selection via Mutual Information')

	results = parser.parse_args()

	nb_train()
	print('number of articles in the training set: ',
	      sum(len(training_set[topic].article_list) for topic in TOPICS.keys()))
	print('number of articles in the test set: ', sum(len(test_set[topic].article_list) for topic in TOPICS.keys()))
	print('number of unique words: ', len(Vocabs))
	print("training set: ", {topic: len(training_set[topic].article_list) for topic in TOPICS.keys()})
	print('test set: ', {topic: len(test_set[topic].article_list) for topic in TOPICS.keys()})

	score = 0
	if results.fs_by_mi == "True":
		fs_by_mi()
		score = nb_test(fs_mi=True)

	else:
		score = nb_test(fs_mi=False)

	print("{} percent: {} correct out of {}".format(score * 100 / sum(len(test_set[topic].article_list) for topic in TOPICS.keys()), score,
	                                                sum(len(test_set[topic].article_list) for topic in TOPICS.keys())))

	print("Evaluation Table:")
	print(evaluation)

	precision = {topic: 0 for topic in TOPICS.keys()}
	recall = {topic: 0 for topic in TOPICS.keys()}
	macro_avg = [0.0, 0.0]
	micro_avg = [0.0, 0.0]
	tp, fp, fn = 0, 0, 0
	for topic in TOPICS.keys():
		precision[topic] = evaluation[topic].tp / (evaluation[topic].tp + evaluation[topic].fp)
		recall[topic] = evaluation[topic].tp / (evaluation[topic].tp + evaluation[topic].fn)
		macro_avg[0] += precision[topic]
		macro_avg[1] += recall[topic]
		tp += evaluation[topic].tp
		fp += evaluation[topic].fp
		fn += evaluation[topic].fn

	macro_avg[0] /= len(TOPICS)
	macro_avg[1] /= len(TOPICS)
	micro_avg[0] = tp / (tp + fp)
	micro_avg[1] = tp / (tp + fn)
	F_micro = 2 * micro_avg[0] * micro_avg[1] / (micro_avg[0] + micro_avg[1])
	F_macro = 2 * macro_avg[0] * macro_avg[1] / (macro_avg[0] + macro_avg[1])

	print("precision: ", precision)
	print("recall: ", recall)

	print("macro_avg_precision: {}".format(macro_avg[0]))
	print("macro_avg_recall: {}".format(macro_avg[1]))
	print("micro_avg_precision: {}".format(micro_avg[0]))
	print("micro_avg_recall: {}".format(micro_avg[1]))
	print("F_macro: {}".format(F_macro))
	print("F_micro: {}".format(F_micro))

	print("--- %s seconds ---" % (time.time() - start_time))

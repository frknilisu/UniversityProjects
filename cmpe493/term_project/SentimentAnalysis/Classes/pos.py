class Pos:
    'class for posıtıve tweets'

    # dictionary variable that consists of all words in posıtıve tweets as key and maps them to (frequencies of the words,freqs of docs)
    word_freq_dict = {}
    # integer variable that shows total number of words in posıtıve tweets'
    total_word_count = 0
    # total number of docs in posıtıve tweet training class
    total_docs = 0

    # true positive, false positive, true negative and false negative for classifier of posıtıve tweets
    tp = 0.0
    fp = 0.0
    tn = 0.0
    fn = 0.0


    # precision, recall and f values for classifier of posıtıve tweets
    p = 0.0
    r = 0.0
    f = 0.0


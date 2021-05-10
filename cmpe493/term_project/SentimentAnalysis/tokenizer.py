from TurkishStemmer import TurkishStemmer

# takes string and tokenize it after preprocessing(remove punctuations, stop words, case folding)
def tokenize(x):

    # tokenize
    init_list = x.split(" ")

    stop_word_set = []
    token_set = []

    # remove stop words from tokens
    stop_word_set.append("")
    stop_word_set.append("reuter")
    stop_word_set.append("\x03")
    for line in open("stopwords.txt"):
        stop_word_set.append(line.strip("\n"))

    # case folding and further normalization tokens
    for word in init_list:
        word = word.strip('[?,;.!,+"()[]{}<>-_]').lower()
        if (word != "") and (word not in stop_word_set):
            word = word.replace("-","").replace("'","").replace('"',"")
            token_set.append(word)

    return token_set


# stems given tokens in the word list
def stem(word_list):

    stemmed_list = []
    stemmer = TurkishStemmer()

    for word in word_list:
        stemmed_list.append(stemmer.stem(word))

    print(stemmed_list)
    return stemmed_list

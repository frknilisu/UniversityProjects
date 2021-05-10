import math
import time
import re

import tokenizer
import Classes


def find_class_for_all_words(test_doc_word_list, actual_class):
    """
    assigns a class to test tweet after applying naive bayes method with feature selection of all words
    :param test_doc_word_list: word list of test tweet
    :param actual_class: actual class of test tweet
    :return: assigned class name
    """
    global pos_class, neg_class, notr_class
    global distinct_word_count
    pos_score, neg_score, notr_score = 0, 0, 0

    total_docs = pos_class.total_docs + neg_class.total_docs + notr_class.total_docs

    pos_score += math.log(pos_class.total_docs / total_docs, 10)
    neg_score += math.log(neg_class.total_docs / total_docs, 10)
    notr_score += math.log(notr_class.total_docs / total_docs, 10)

    for word in test_doc_word_list:
        if word in pos_class.word_freq_dict:
            pos_score += math.log((pos_class.word_freq_dict[word][0] + 1) / (pos_class.total_word_count + distinct_word_count), 10)
        else:
            pos_score += math.log(1 / (pos_class.total_word_count + distinct_word_count), 10)

        if word in neg_class.word_freq_dict:
            neg_score += math.log((neg_class.word_freq_dict[word][0] + 1) / (neg_class.total_word_count + distinct_word_count), 10)
        else:
            neg_score += math.log(1 / (neg_class.total_word_count + distinct_word_count), 10)

        if word in notr_class.word_freq_dict:
            notr_score += math.log((notr_class.word_freq_dict[word][0] + 1) / (notr_class.total_word_count + distinct_word_count), 10)
        else:
            notr_score += math.log(1 / (notr_class.total_word_count + distinct_word_count), 10)


    data = {
        'pos': pos_score,
        'neg': neg_score,
        'notr': notr_score
    }

    #print(max(data, key=data.get) + " and " + str(data))
    assigned = max(data, key=data.get)
    # update false positives, true positives, false negatives and true negatives for each class
    if assigned == 'pos':
        test_class.total_doc_pos += 1
        if actual_class == 'pos':
            pos_class.tp += 1
            neg_class.tn += 1
            notr_class.tn += 1
        else:
            pos_class.fp += 1
            if actual_class == 'neg':
                neg_class.fn += 1
                notr_class.tn += 1
            elif actual_class == 'notr':
                neg_class.tn += 1
                notr_class.fn += 1
    elif assigned == 'neg':
        test_class.total_doc_neg += 1
        if actual_class == 'neg':
            neg_class.tp += 1
            pos_class.tn += 1
            notr_class.tn += 1
        else:
            neg_class.fp += 1
            if actual_class == 'pos':
                pos_class.fn += 1
                notr_class.tn += 1
            elif actual_class == 'notr':
                pos_class.tn += 1
                notr_class.fn += 1
    else:
        test_class.total_doc_notr += 1
        if actual_class == 'notr':
            notr_class.tp += 1
            neg_class.tn += 1
            pos_class.tn += 1
        else:
            notr_class.fp += 1
            if actual_class == 'neg':
                neg_class.fn += 1
                pos_class.tn += 1
            elif actual_class == 'pos':
                neg_class.tn += 1
                pos_class.fn += 1

    return assigned


def add_to_class_dict(x,words):
    """
    add words to the dictionary of the class
    :param x: class name to which the words will be added
    :param words: words of training document
    :return:
    """
    global pos_class, neg_class, notr_class
    global distinct_word_count

    encountered_words_in_doc = {}

    x.total_word_count += len(words)
    for word in words:

        if word not in pos_class.word_freq_dict and word not in notr_class.word_freq_dict and word not in neg_class.word_freq_dict:
            distinct_word_count += 1

        if word not in x.word_freq_dict:
            x.word_freq_dict.update({word : (1,1)})
        else:
            if word not in encountered_words_in_doc:
                x.word_freq_dict.update({word : (x.word_freq_dict[word][0] + 1, x.word_freq_dict[word][1] + 1) })
                encountered_words_in_doc.update({word: 0})
            else:
                x.word_freq_dict.update({word : (x.word_freq_dict[word][0] + 1, x.word_freq_dict[word][1]) })


def assign_test_docs():
    """
    assigns classes to each test tweet result to file all-words.txt
    :return:
    """
    f1 = open('all-words.txt', 'w')

    all_words_based_map = {}
    for tweet in test_class.word_dict.items():
        all_words_based_map.update( {tweet[0]:find_class_for_all_words(tweet[1][0], tweet[1][1])} )


    f1.write(str(all_words_based_map))
    f1.close()


start = time.time()
# unique token count
distinct_word_count = 0

# initialize tweet classes
pos_class = Classes.pos.Pos()
neg_class =  Classes.neg.Neg()
notr_class = Classes.notr.Notr()

# initialize test class
test_class = Classes.test.Test()

total_tweet = 6372
# 1879 1121 3372 = 6372

fpos = open(r"./Train/positive-train", 'r', encoding="utf8")
for line in fpos.readlines():
    m = re.search('(?<=\d{18}\t\t\t).*', line)
    if m != None:
        tokens = tokenizer.tokenize(m.group(0))
        words = tokenizer.stem(tokens)

        pos_class.total_docs += 1
        add_to_class_dict(pos_class, words)
fpos.close()

fneg = open(r"./Train/negative-train", 'r', encoding="utf8")
for line in fneg.readlines():
    m = re.search('(?<=\d{18}\t\t\t).*', line)
    if m != None:
        tokens = tokenizer.tokenize(m.group(0))
        words = tokenizer.stem(tokens)

        neg_class.total_docs += 1
        add_to_class_dict(neg_class, words)
fneg.close()

fnotr = open(r"./Train/notr-train", 'r', encoding="utf8")
for line in fnotr.readlines():
    m = re.search('(?<=\d{18}\t\t\t).*', line)
    if m != None:
        tokens = tokenizer.tokenize(m.group(0))
        words = tokenizer.stem(tokens)

        notr_class.total_docs += 1
        add_to_class_dict(notr_class, words)
fnotr.close()

# create test data
test_tweet1 = 'Allahım benim mutlu mutlu twittera girip " Boğaziçi :D" yazacağım günler ne zaman gelecek ya yıldım'
test_class.word_dict.update({"669201744535113729": (tokenizer.stem(tokenizer.tokenize(test_tweet1)),'pos')})

test_tweet3 = "Boğaziçi Ün.Öğ Üyesi Prof Dr Faruk Birtek olacak.Sözcü Gazetesi'ne sahibi üzerinden yıpratma saldırıları sürüyor.Bu olayın perde arkasını.."
test_class.word_dict.update({"969214293123231744": (tokenizer.stem(tokenizer.tokenize(test_tweet3)),'notr')})

test_tweet4 = "@SemihYararoglu daha ne bogazici bogazici amk"
test_class.word_dict.update({"648992255278227000": (tokenizer.stem(tokenizer.tokenize(test_tweet4)),'neg')})

test_tweet5 = "Hisarustunde hic pilavci yok lanet olsun size bati ozentisi bogazici burjuvalari"
test_class.word_dict.update({"971463436679942146": (tokenizer.stem(tokenizer.tokenize(test_tweet5)),'neg')})



# assign class to test tweets
assign_test_docs()

'''# report all statistics
stat_file = open("stats.txt","a")
stat_file.write("training earn-docs  : %d\n" % pos_class.total_docs)
stat_file.write("training grain-docs : %d\n" % notr_class.total_docs)
stat_file.write("training acq-docs   : %d\n\n" % neg_class.total_docs)

stat_file.write("test classifier1 earn-docs  : %d\n"%test_class.total_doc_earn1)
stat_file.write("test classifier1 grain-docs : %d\n"%test_class.total_doc_grain1)
stat_file.write("test classifier1 acq-docs   : %d\n\n"%test_class.total_doc_acq1)

stat_file.write("test classifier1 earn-tp-tn-fp-fn  : %d,%d,%d,%d\n" % (pos_class.tp1, pos_class.tn1, pos_class.fp1, pos_class.fn1))
stat_file.write("test classifier1 grain-tp-tn-fp-fn : %d,%d,%d,%d\n" % (notr_class.tp1, notr_class.tn1, notr_class.fp1, notr_class.fn1))
stat_file.write("test classifier1 acq-tp-tn-fp-fn   : %d,%d,%d,%d\n\n" % (neg_class.tp1, neg_class.tn1, neg_class.fp1, neg_class.fn1))

pos_class.p1 = pos_class.tp1 / (pos_class.tp1 + pos_class.fp1)
pos_class.r1 = pos_class.tp1 / (pos_class.tp1 + pos_class.fn1)
notr_class.p1 = notr_class.tp1 / (notr_class.tp1 + notr_class.fp1)
notr_class.r1 = notr_class.tp1 / (notr_class.tp1 + notr_class.fn1)
neg_class.p1 = neg_class.tp1 / (neg_class.tp1 + neg_class.fp1)
neg_class.r1 = neg_class.tp1 / (neg_class.tp1 + neg_class.fn1)
stat_file.write("test classifier1 earn precision-recall  : %.3f,%.3f\n" % (pos_class.p1 , pos_class.r1))
stat_file.write("test classifier1 grain precision-recall : %.3f,%.3f\n" % (notr_class.p1, notr_class.r1))
stat_file.write("test classifier1 acq precision-recall   : %.3f,%.3f\n\n" % (neg_class.p1 , neg_class.r1))

pos_class.f1 = (2 * pos_class.p1 * pos_class.r1) / (pos_class.p1 + pos_class.r1)
notr_class.f1 = (2 * notr_class.p1 * notr_class.r1) / (notr_class.p1 + notr_class.r1)
neg_class.f1 = (2 * neg_class.p1 * neg_class.r1) / (neg_class.p1 + neg_class.r1)
stat_file.write("test classifier1 earn f-val   : %.3f\n" % pos_class.f1)
stat_file.write("test classifier1 grain f-val  : %.3f\n" % notr_class.f1)
stat_file.write("test classifier1 acq f-val    : %.3f\n\n" % neg_class.f1)

macro_average_precision1 = (pos_class.p1 + notr_class.p1 + neg_class.p1) / 3
stat_file.write("classifier1 macro-precision    : %.3f\n"   %  macro_average_precision1)

macro_average_recall1 = (pos_class.r1 + notr_class.r1 + neg_class.r1) / 3
stat_file.write("classifier1 macro-recall    : %.3f\n"   %  macro_average_recall1)

macro_average_f1 = (2*macro_average_precision1*macro_average_recall1)/(macro_average_precision1+macro_average_recall1)
stat_file.write("classifier1 macro-f    : %.3f\n"   %  macro_average_f1)

micro_average_precision1 = (pos_class.tp1 + notr_class.tp1 + neg_class.tp1) / (pos_class.tp1 + pos_class.fp1 + notr_class.tp1 + notr_class.fp1 + neg_class.tp1 + neg_class.fp1)
stat_file.write("classifier1 micro-precision    : %.3f\n"   %  micro_average_precision1)

micro_average_recall1 = (pos_class.tp1 + notr_class.tp1 + neg_class.tp1) / (pos_class.tp1 + pos_class.fn1 + notr_class.tp1 + notr_class.fn1 + neg_class.tp1 + neg_class.fn1)
stat_file.write("classifier1 micro-recall    : %.3f\n"   %  micro_average_recall1)

micro_average_f1 = (2*micro_average_precision1*micro_average_recall1)/(micro_average_precision1+micro_average_recall1)
stat_file.write("classifier1 micro-f    : %.3f\n"   %  micro_average_f1)

stat_file.close()'''
end = time.time()
print(end-start)
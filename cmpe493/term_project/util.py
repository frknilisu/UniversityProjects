aykut = [line.split('\t')[0].strip() for line in open("11.Annotator1.txt","r").readlines()]
furkan = [line.split('\t')[0].strip() for line in open("11.Annotator2.txt","r").readlines()]

different_tweet_line_ids = []

for i in range(len(aykut)):
    if aykut[i] != furkan[i]:
        different_tweet_line_ids.append(i+1)

print(aykut[216], furkan[216])
print(len(different_tweet_line_ids))
print (different_tweet_line_ids)
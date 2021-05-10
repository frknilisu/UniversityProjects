aykut = [line.split('\t')[0] for line in open("11.Annotator1.txt","r").readlines()]
furkan = [line.split('\t')[0] for line in open("11.Annotator2.txt","r").readlines()]

classes = ['-1', '0', '1', '2', 'total']
table = {x: {y: 0 for y in classes} for x in classes}

for i in range(len(aykut)):
	a = aykut[i]
	f = furkan[i]
	table[a][f]+=1
	table[a]['total']+=1
	table['total'][f]+=1
	table['total']['total']+=1

p_a = 0.0
p_e = 0.0

for c in classes:
	if c == 'total':
		continue

	p_a += table[c][c]
	p_e += (table[c]['total']/table['total']['total'])*(table['total'][c]/table['total']['total']) 

p_a /= table['total']['total']
kappa = (p_a - p_e)/(1 - p_e)
print(kappa)
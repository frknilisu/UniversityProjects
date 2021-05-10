Python 3.6.4
MNB: Multinomial Naive Bayes
MI: Mutual Information
FS: Feature Selection

i) 	run MNB with all words(without FS via MI): 
	python3 src/main.py -mi False
ii) run MNB with FS via MI: 
	python3 src/main.py -mi True

At the first run, the program read all sgml files from Dataset, parse all articles and save the parsed data as data_collection file.
After that, for every run, the program read data_collection instead of reading and parsing sgml files again.

for part i)
	accuracy: 96.73913043478261 percent: 2225 correct out of 2300
	time: --- 3.814675807952881 seconds ---

for part ii)
	accuracy: 96.47826086956522 percent: 2219 correct out of 2300
	time: --- 0.6826009750366211 seconds ---

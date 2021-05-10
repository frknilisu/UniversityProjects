# README #

this project is for CmpE493 HW3

Single Document Extractive Text Summarization with LexRank

Run it as:
python3 <path to text_sum.py> <path to dataset> <name of the file under Dataset folder>

example run :

    python3 ./text_sum.py /home/User/Desktop/Dataset/ 13.txt

Note : coded and tested on macOS Sierra 10.12.6 with python3.6.4
numpy 1.14.3 is used for matrix operations
rouge library is used to calculate ROUGE score from (https://github.com/pltrdy/rouge)
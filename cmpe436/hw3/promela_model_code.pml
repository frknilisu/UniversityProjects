bit y[2];		/* who’s turn is it? */
bool s = true;			/* # procs in critical section */

active [2] proctype P(bit i) {
	do
	::	/* non-critical section */
		y[i] = 1;	s = i;
		(y[1-i] == 0) || (s != i);
		/* critical section */
		y[i] = 0;
	od
}

init { atomic {run P(0); run P(1);}}
# Tests two sets of precedence operators
(echo A && echo B) || (echo C && echo D)
	# A B
(blah && echo B) || (echo C && echo D)
	# C D
(echo A || echo B) && (echo C && echo D) 
	# A C D
(blah || blah) && (echo C && echo D)
	#

# Tests precedence operator with 3 commands-end of command line
echo A || (echo B || echo C && echo D)
	# A B C D
echo A || (echo B && echo C && echo D)
	# A 

# Tests precedence operator-2 commands-end of command line
echo A || echo B || (echo C && echo D)
	# A C D 

# Tests precedence operator-2 commands-middle of command line
echo A || (echo B || echo C) && echo D
	# A D
echo A && (echo B || echo C) || echo D
	# A B 

# Tests precedence operator-2 commands-beginning of command line
(echo A || echo B) || echo C || echo D
	# A D
(echo A || echo B) && echo C || echo D
	# A C

# Tests precedence operator with 3 commands-beginning of command line
(echo A && echo B && echo C) || echo D
	# A B C
(echo A && echo B && echo C) && echo D

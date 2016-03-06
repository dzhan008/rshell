# Test if the file path exists
test -e /main.cpp
	# returns true
test -e /blah.cpp
	# returns false
[ -e /main.cpp ]
	# returns true
[ -e /blah.cpp ]
	# returns false

# Tests if file path exists and is a file
test -f /main.cpp
	# returns true
test -f /bin
	# returns false
[ -f /main.cpp ]
	# return true
[ -f /bin.cpp ]
	# returns false

# Tests if file path exists and is a directory
test -d /bin
	# returns true
test -d /main.cpp
	# returns false
[ -d /bin ]
	# returns true
[ -d /main.cpp ]
	# returns false

# Tests for errors
testt -e /main.cpp
	# throws error with 'test'
test -q /main.cpp
	# throws error with '-q'
[ -e /main.cpp ) 
	# throws error with ')'

# Test if the file path exists
test -e main.cpp
	# returns true
test -e /main.cpp
	# returns false
[ -e main.cpp ]
	# returns true
[ -e /main.cpp ]
	# returns false

# Tests if file path exists and is a file
test -f main.cpp
	# returns true
test -f /main.cpp
    # returns false
test -f /bin
	# returns false
[ -f main.cpp ]
	# return true
[ -f /main.cpp ]
	# return false
[ -f /bin.cpp ]
	# returns false

# Tests if file path exists and is a directory
test -d /bin
	# returns true
test -d bin
	# returns true
test -d main.cpp
	# returns false	
[ -d /bin ]
	# returns true
[ -d bin ]
	# returns true
[ -d main.cpp ]
	# returns false
	
# Tests for errors
testt -e /main.cpp
	# throws error with 'test'
test -q /main.cpp
	# throws error with '-q'
[ -e /main.cpp ) 
	# throws error with ')'
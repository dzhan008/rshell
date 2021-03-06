# Test if the file path exists
test -e /bin
    # returns true
test -e bin
    # returns true
test -e bin/main.cpp
    # returns true
test -e main.cpp/bin
    # returns false   
test -e main.cpp
	# returns true
test -e /main.cpp
	# returns false
[ -e main.cpp ]
	# returns false
[ -e /main.cpp ]
	# returns false
[ -e bin ]
    # returns true
*[ -e bin/main.cpp]
    # returns true
    
# Tests if file path exists and is a file
test -f main.cpp
	# returns true
test -f /bin
	# returns false
test -f bin/dir
    # returns false
test -f bin/main.cpp
    # returns true
[ -f main.cpp ]
	# return true
[ -f /bin.cpp ]
	# return false
[ -f bin/dir ]
    # returns false
[ -f bin/main.cpp ]
	# returns true

# Tests if file path exists and is a directory
test -d /bin
	# returns true
test -d bin
	# returns true
test -d bin/dir
    # returns true
test -d main.cpp
	# returns false	
test -d bin/main.cpp
    # returns false
[ -d /bin ]
	# returns true
[ -d bin/dir ]
    # returns true
[ -d main.cpp ]
	# returns false
[ -d bin/main.cpp ]
    # returns false
    
# Tests test call without specifying flag
test main.cpp
    # returns false
test bin/main.cpp
    # returns true
[ main.cpp ]
    # returns false
[ bin/main.cpp ]
    # returns true
    
# Tests for errors in syntax
testt -e /main.cpp
	# throws error with 'test'
test -q /main.cpp
	# throws error with '-q'
[ -e /main.cpp ) 
	# throws error with ')'
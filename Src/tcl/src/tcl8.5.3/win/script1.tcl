set ddeServerName child-4.1
load C:/tcl8.5.3/win/Debug_VC9/tcldde13g.dll Dde

        # DDE child server -
        #
	if {[lsearch [namespace children] ::tcltest] == -1} {
	    package require tcltest
	    namespace import -force ::tcltest::*
	}
        
        # If an error occurs during the tests, this process may end up not
        # being closed down. To deal with this we create a 30s timeout.
        proc ::DoTimeout {} {
            global done ddeServerName
            set done 1
            puts "winDde.test child process $ddeServerName timed out."
            flush stdout
        }
        set timeout [after 30000 ::DoTimeout]
        
        # Define a restricted handler.
        proc Handler1 {cmd} {
            if {$cmd eq "stop"} {set ::done 1}
            puts $cmd ; flush stdout 
            return
        }
        proc Handler2 {cmd} {
            if {$cmd eq "stop"} {set ::done 1}
            puts [uplevel \#0 $cmd] ; flush stdout 
            return
        }
        proc Handler3 {prefix cmd} {
            if {$cmd eq "stop"} {set ::done 1}
            puts [list $prefix $cmd] ; flush stdout
            return
        }
    
dde servername child-4.1

        # run the server and handle final cleanup.
        after 200;# give dde a chance to get going.
	puts ready
        flush stdout
	vwait done
	# allow enough time for the calling process to
	# claim all results, to avoid spurious "server did
	# not respond"
	after 200 { set reallyDone 1 }
	vwait reallyDone
	exit
    

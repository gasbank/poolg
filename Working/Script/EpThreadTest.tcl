
namespace eval EpThreadTest {

	proc createUnit {} {
		set world [ EpGetCurWorld ]
		set enemy [ EpCreateEnemy 28 76 ]
		EpRegisterToWorld $world $enemy
	}
	proc printHeroPos {} {
		set hero [ EpGetHero ]
		EpUnitPrintTilePos $hero
	}

	proc localHandler {} {
		# get the data from stdin
		gets stdin data
		# client will exit if you type "exit". How original!
		
		if { [string first "eval" $data] == 0 } {
			
			if [catch { eval [string range $data 5 end] } result] {
				puts "Bad command."
			}
			
		} elseif { [string first "uplevel" $data] == 0 } {
			if [catch { uplevel [string range $data 8 end] } result] {
				puts "Bad command."
			}
		} else {
			switch $data {
				exit { exit }
				ls {
					puts "So classical"
				}
				sl {
					puts "      ====        ________                ___________                   "
					puts "  _D _|  |_______/        \\__I_I_____===__|_________|                                "
					puts "   |(_)---  |   H\\________/ |   |        =|___ ___|      _________________           "
					puts "   /     |  |   H  |  |     |   |         ||_| |_||     _|                \\_____A    "
					puts "  |      |  |   H  |__--------------------| \[___\] |   =|                        |    "
					puts "  | ________|___H__/__|_____/\[\]\[\]~\\_______|       |   -|                        |    "
					puts "  |/ |   |-----------I_____I \[\]\[\] \[\]  D   |=======|____|________________________|_   "
					puts "__/ =| o |=-~~\\  /~~\\  /~~\\  /~~\\ ____Y___________|__|__________________________|_   "
					puts "|/-=|___||    ||    ||    ||    |_____/~\\___/          |_D__D__D_|  |_D__D__D_|      "
					puts " \\_/      \\__/  \\__/  \\__/  \\__/      \\_/               \\_/   \\_/    \\_/   \\_/       "
				}
				default {
					if [string length $data] {
						puts "Bad command."
					}
				}
			}
		}
		
		
		# return a new pseudo-prompt
		puts -nonewline stdout \
			"Ep> "
		flush stdout	
		return
	}
	# assign our event handler for stdin
	fileevent stdin readable EpThreadTest::localHandler
	# send a startup message and initial prompt
	puts -nonewline stdout "Startup...\nEp> "
	flush stdout
	
	# enter the tcl event loop
	vwait __forever__
}

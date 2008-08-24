#----------------------------------------------------------------------------------#
#                                                                                  #
#   EmptyProject: Ep Console Script                                                #
#                                                                                  #
#----------------------------------------------------------------------------------#

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

	proc help { { pattern "Ep*" } } {
		set epCommandList [info commands $pattern]
		set sorted [lsort -increasing $epCommandList]
		foreach cmd $sorted {
			puts $cmd
		}
	}
	
	proc localHandler {} {
		# get the data from stdin
		gets stdin data
		
		if { [string first "eval" $data] == 0 } {
			
			if [catch { eval [string range $data 5 end] } result] {
				puts $::errorInfo
			}
			
		} elseif { [string first "uplevel" $data] == 0 } {
			if [catch { uplevel [string range $data 8 end] } result] {
				puts $::errorInfo
			}
		} else {
			if [ catch { 				
			
				switch $data {
					exit {
						# client will exit if you type "exit". How original!
						exit
					}
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
					a {
						EpSetDoAnim [ EpGetNode Needle ] 1
					}
					b {
						EpSetDoAnim [ EpGetNode Needle ] 0
					}
					c {
						EpSetAnimTime [ EpGetNode Needle ] 0
					}
					world {
						EpCurWorldDebugInfo
					}
					cheat {
						                                        # Health Will Coding Defence Sense Immunity
						EpCharacterSetStat			[ EpGetHero ]  999    999   999    999    999    999
						EpCharacterSetCurHp			[ EpGetHero ] 9999
						EpCharacterSetCurCs			[ EpGetHero ] 9999
						EpCharacterSetMoveDuration	[ EpGetHero ] 0.01
					}
					
					default {
						if [string length $data] {
							puts $::errorInfo
						}
					}
				}
			} result ] {
				puts $::errorInfo
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
	puts "Engine global variable g_closeConsole is $closeConsole"
	
	while { $closeConsole eq 2008 } {
		set x 0
		after 500 {set x 1}
		vwait x
	}
}

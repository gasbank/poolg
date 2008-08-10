set EpDialogList [list EpDialog1 EpDialog2];

namespace eval EpDialog1 {
	
	set region [ list 9 9 -1 -1 ]; ;# left, top, right, bottom
	set oneTime 1;
	
	set player "PoolG"
	set enemy "YangSanHyung"
	
	set dialog [ list\
		$player		"내 이름은 풀쥐야!! 난 (9,9) 타일에 서서 대화를 시작하겠어."\
		$enemy		"그게 뭐 어쨌다는거임?"\
		$player		"그냥 그렇다고.."\
		$enemy		"이런 식으로 대화를 계속 하는게 의미가 있는지 생각해보자"\
		$player		"그래. 내 생각엔 적어도 20번 이상 대화를 주고 받아야 사용자가 납득할만한 대화 내용이 된다고 생각해."\
		$enemy		"요즘같이 바쁜 세상에 이미 눈치빠른 플레이어들은 이정도면 대화가 잘 구현됐다고 생각할거야."\
		$player		"하지만 드라군이 출동하면 어떨까?"\
		$enemy		"......"\
		$player		"내 개그를 이해 못했군. 이건 요즘 DG인사이드에 유행하는 개그야"\
		$enemy		"Shut up"\
		$player		"오 양키어도 구사하는군"\
	];	
}


namespace eval EpDialog2 {
	
	set region [ list 10 10 -1 -1 ]; ;# left, top, right, bottom
	set oneTime 1;
	
	set player "풀쥐한글판"
	set enemy "양산형"
	
	set dialog [ list\
		$player		"내 이름은 풀쥐야!! 난 (10,10) 타일에 서서 대화를 시작하겠어. 이번엔 귀찮으니깐 짧게 끝내자."\
		$enemy		"Okay."\
		$player		"지금은 한글판이거든?"\
		$enemy		"S*it! Speak English you idiot!"\
	];	
}

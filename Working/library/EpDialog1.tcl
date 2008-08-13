set EpDialogList [list EpDialog1 EpDialog2 EpDialog3 EpDialog4 EpDialogStop EpDialogHeal EpDialogBox EpDialogMirror EpDialogLaserOff EpDialogLaserOn EpDialogSave];

namespace eval EpDialog1 {
	
	set region [ list 9 9 -1 -1 ]; ;# left, top, right, bottom
	set oneTime 1;
	
	set player "PoolG"
	set npc1 "GlooP"
	set npc2 "GetG"
	
	set dialog [ list\
		$npc1		"PoolC에서 대청소를 해버리는 바람에, 우리들의 삶의 터전이 좁아저 버렸다네"
		$player		"뭬야!"
		$npc1		"게다가 앞으론 우리들을 살 수 없도록 언제나 깨끗하게 유지하겠다고 하더군"
		$player		"맙소사! 난 가만히 있을 수 없어"
		$npc2		"제발 그들을 말려주게"
		$player		"이대로 가만히 있을 순 없지. 다녀오겠네"
	];	
}


namespace eval EpDialog2 {
	
	set region [ list 10 10 -1 -1 ]; ;# left, top, right, bottom
	set oneTime 1;
	
	set player "PoolG"
	set npc "김두부"
	
	set dialog [ list\
		$player		"너는 누구냐!"\
		$npc		"나는 두부를 지극히 사랑하는 이 동아리의 부원이라네"\
		$player		"이 동아리에 소속된 사람이라면 대청소를 한 주범이 누군지 알고 있을텐데?"\
				"순순히 말하지 않으면 널 Overflow 시켜버리겠어!"\
		$npc		"대청소를 한 미화부장이라면, A213이라는 신비의 장소에서 코딩을 즐겨 한다고 하더군"\
		$player		"말이 통해서 다행이군"\
	];	
}

namespace eval EpDialog3 {
	
	set region [ list 18 9 -1 -1 ]; ;# left, top, right, bottom
	set oneTime 1;
	
	set player "PoolG"
		
	set dialog [ list\
		$player		"도대체 이 끔찍한 시설의 컴퓨터 실은 뭐냔 말이다....."\
				"정말 이런 곳에서 코딩을 할 수 있단 말인가! 제길, 아까 그녀석이 잘못 가르쳐 준거 아냐?"\
				"만약 미화부장이 없다면 그녀석을 혼내러 가야겠어"\
	];	
}

namespace eval EpDialog4 {
	
	set region [ list 9 9 -1 -1 ]; ;# left, top, right, bottom
	set oneTime 1;
	
	set player "PoolG"
	set boss "괴물"
	
	set dialog [ list\
		$player		"당신이 PoolC의 미화부장 인가!"\
		$boss		"난 총무부장이야"\
		$player		"뭐! 그렇다면 미화부장은 누구란 말이야!"\
		$boss		"난 미화부장이야"\
		$player		"정체가 뭐야"\
		$boss		"난 PoolC의 미화부장겸 총무부장이지"\
		$player		"그렇다면 당신이 우리들의 삶의 터전을 없애버렸단 말이군"\
		$boss		"동방은 청결하게 유지되어야 하거든"\
		$player		"용서할 수 없다! 우리들의 삶의 터전을 돌려달라!"\
		$boss		"Shut up"\
		$player		"오 양키어도 구사하는군"\
		$boss		"다 찢어버리겠어!"\
	];	
}

namespace eval EpDialogStop {
	
	set region [ list 14 14 -1 -1 ]; ;# left, top, right, bottom
	set oneTime 0;
	
	set player "SYSTEM"
	
	set dialog [ list\
		$player		"여기로 오면 안 생긴다네"\
	];	
}

namespace eval EpDialogHeal {
	
	set region [ list 15 14 -1 -1 ]; ;# left, top, right, bottom
	set oneTime 0;
	
	set player "SYSTEM"
	
	set dialog [ list\
		$player		"온 몸이 편안해지는 기분이다"\
	];	
}

namespace eval EpDialogBox {
	
	set region [ list 16 14 -1 -1 ]; ;# left, top, right, bottom
	set oneTime 0;
	
	set player "SYSTEM"
	
	set dialog [ list\
		$player		"뭔가 밀 수 있어보이는 상자가 놓여져있다"\
	];	
}

namespace eval EpDialogMirror {
	
	set region [ list 17 14 -1 -1 ]; ;# left, top, right, bottom
	set oneTime 0;
	
	set player "SYSTEM"
	
	set dialog [ list\
		$player		"돌릴 수 있도록 만들어진 거울이다"\
	];	
}

namespace eval EpDialogLaserOff {
	
	set region [ list 18 14 -1 -1 ]; ;# left, top, right, bottom
	set oneTime 0;
	
	set player "SYSTEM"
	
	set dialog [ list\
		$player		"레이져 포인터가 꺼져있다"\
	];	
}

namespace eval EpDialogLaserOn {
	
	set region [ list 19 14 -1 -1 ]; ;# left, top, right, bottom
	set oneTime 0;
	
	set player "SYSTEM"
	
	set dialog [ list\
		$player		"레이져 포인터에서 레이져가 나오고 있다"\
	];	
}

namespace eval EpDialogTarget {
	
	set region [ list 20 14 -1 -1 ]; ;# left, top, right, bottom
	set oneTime 0;
	
	set player "SYSTEM"
	
	set dialog [ list\
		$player		"레이져에만 반응하는 센서인 듯 하다"\
	];	
}

namespace eval EpDialogSave {
	
	set region [ list 21 14 -1 -1 ]; ;# left, top, right, bottom
	set oneTime 0;
	
	set player "SYSTEM"
	
	set dialog [ list\
		$player		"지금까지의 기록을 저장할 수 있습니다"\
	];	
}
set EpDialogList [list EpDialog1 EpDialog2 EpDialog3 EpDialog4 EpDialogStop EpDialogHeal EpDialogBox EpDialogMirror EpDialogLaserOff EpDialogLaserOn EpDialogSave];

namespace eval EpDialog1 {
	
	set region [ list 9 9 -1 -1 ]; ;# left, top, right, bottom
	set oneTime 1;
	
	set player "PoolG"
	set npc1 "GlooP"
	set npc2 "GetG"
	
	set dialog [ list\
		$npc1		"PoolC���� ��û�Ҹ� �ع����� �ٶ���, �츮���� ���� ������ ������ ���ȴٳ�"
		$player		"����!"
		$npc1		"�Դٰ� ������ �츮���� �� �� ������ ������ �����ϰ� �����ϰڴٰ� �ϴ���"
		$player		"���һ�! �� ������ ���� �� ����"
		$npc2		"���� �׵��� �����ְ�"
		$player		"�̴�� ������ ���� �� ����. �ٳ���ڳ�"
	];	
}


namespace eval EpDialog2 {
	
	set region [ list 10 10 -1 -1 ]; ;# left, top, right, bottom
	set oneTime 1;
	
	set player "PoolG"
	set npc "��κ�"
	
	set dialog [ list\
		$player		"�ʴ� ������!"\
		$npc		"���� �κθ� ������ ����ϴ� �� ���Ƹ��� �ο��̶��"\
		$player		"�� ���Ƹ��� �Ҽӵ� ����̶�� ��û�Ҹ� �� �ֹ��� ������ �˰� �����ٵ�?"\
				"������ ������ ������ �� Overflow ���ѹ����ھ�!"\
		$npc		"��û�Ҹ� �� ��ȭ�����̶��, A213�̶�� �ź��� ��ҿ��� �ڵ��� ��� �Ѵٰ� �ϴ���"\
		$player		"���� ���ؼ� �����̱�"\
	];	
}

namespace eval EpDialog3 {
	
	set region [ list 18 9 -1 -1 ]; ;# left, top, right, bottom
	set oneTime 1;
	
	set player "PoolG"
		
	set dialog [ list\
		$player		"����ü �� ������ �ü��� ��ǻ�� ���� ���� ���̴�....."\
				"���� �̷� ������ �ڵ��� �� �� �ִ� ���ΰ�! ����, �Ʊ� �׳༮�� �߸� ������ �ذ� �Ƴ�?"\
				"���� ��ȭ������ ���ٸ� �׳༮�� ȥ���� ���߰ھ�"\
	];	
}

namespace eval EpDialog4 {
	
	set region [ list 9 9 -1 -1 ]; ;# left, top, right, bottom
	set oneTime 1;
	
	set player "PoolG"
	set boss "����"
	
	set dialog [ list\
		$player		"����� PoolC�� ��ȭ���� �ΰ�!"\
		$boss		"�� �ѹ������̾�"\
		$player		"��! �׷��ٸ� ��ȭ������ ������ ���̾�!"\
		$boss		"�� ��ȭ�����̾�"\
		$player		"��ü�� ����"\
		$boss		"�� PoolC�� ��ȭ����� �ѹ���������"\
		$player		"�׷��ٸ� ����� �츮���� ���� ������ ���ֹ��ȴ� ���̱�"\
		$boss		"������ û���ϰ� �����Ǿ�� �ϰŵ�"\
		$player		"�뼭�� �� ����! �츮���� ���� ������ �����޶�!"\
		$boss		"Shut up"\
		$player		"�� ��Ű� �����ϴ±�"\
		$boss		"�� ��������ھ�!"\
	];	
}

namespace eval EpDialogStop {
	
	set region [ list 14 14 -1 -1 ]; ;# left, top, right, bottom
	set oneTime 0;
	
	set player "SYSTEM"
	
	set dialog [ list\
		$player		"����� ���� �� ����ٳ�"\
	];	
}

namespace eval EpDialogHeal {
	
	set region [ list 15 14 -1 -1 ]; ;# left, top, right, bottom
	set oneTime 0;
	
	set player "SYSTEM"
	
	set dialog [ list\
		$player		"�� ���� ��������� ����̴�"\
	];	
}

namespace eval EpDialogBox {
	
	set region [ list 16 14 -1 -1 ]; ;# left, top, right, bottom
	set oneTime 0;
	
	set player "SYSTEM"
	
	set dialog [ list\
		$player		"���� �� �� �־�̴� ���ڰ� �������ִ�"\
	];	
}

namespace eval EpDialogMirror {
	
	set region [ list 17 14 -1 -1 ]; ;# left, top, right, bottom
	set oneTime 0;
	
	set player "SYSTEM"
	
	set dialog [ list\
		$player		"���� �� �ֵ��� ������� �ſ��̴�"\
	];	
}

namespace eval EpDialogLaserOff {
	
	set region [ list 18 14 -1 -1 ]; ;# left, top, right, bottom
	set oneTime 0;
	
	set player "SYSTEM"
	
	set dialog [ list\
		$player		"������ �����Ͱ� �����ִ�"\
	];	
}

namespace eval EpDialogLaserOn {
	
	set region [ list 19 14 -1 -1 ]; ;# left, top, right, bottom
	set oneTime 0;
	
	set player "SYSTEM"
	
	set dialog [ list\
		$player		"������ �����Ϳ��� �������� ������ �ִ�"\
	];	
}

namespace eval EpDialogTarget {
	
	set region [ list 20 14 -1 -1 ]; ;# left, top, right, bottom
	set oneTime 0;
	
	set player "SYSTEM"
	
	set dialog [ list\
		$player		"���������� �����ϴ� ������ �� �ϴ�"\
	];	
}

namespace eval EpDialogSave {
	
	set region [ list 21 14 -1 -1 ]; ;# left, top, right, bottom
	set oneTime 0;
	
	set player "SYSTEM"
	
	set dialog [ list\
		$player		"���ݱ����� ����� ������ �� �ֽ��ϴ�"\
	];	
}
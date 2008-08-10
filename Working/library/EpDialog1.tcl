set EpDialogList [list EpDialog1 EpDialog2];

namespace eval EpDialog1 {
	
	set region [ list 9 9 -1 -1 ]; ;# left, top, right, bottom
	set oneTime 1;
	
	set player "PoolG"
	set enemy "YangSanHyung"
	
	set dialog [ list\
		$player		"�� �̸��� Ǯ���!! �� (9,9) Ÿ�Ͽ� ���� ��ȭ�� �����ϰھ�."\
		$enemy		"�װ� �� ��·�ٴ°���?"\
		$player		"�׳� �׷��ٰ�.."\
		$enemy		"�̷� ������ ��ȭ�� ��� �ϴ°� �ǹ̰� �ִ��� �����غ���"\
		$player		"�׷�. �� ������ ��� 20�� �̻� ��ȭ�� �ְ� �޾ƾ� ����ڰ� �����Ҹ��� ��ȭ ������ �ȴٰ� ������."\
		$enemy		"������ �ٻ� ���� �̹� ��ġ���� �÷��̾���� �������� ��ȭ�� �� �����ƴٰ� �����Ұž�."\
		$player		"������ ����� �⵿�ϸ� ���?"\
		$enemy		"......"\
		$player		"�� ���׸� ���� ���߱�. �̰� ���� DG�λ��̵忡 �����ϴ� ���׾�"\
		$enemy		"Shut up"\
		$player		"�� ��Ű� �����ϴ±�"\
	];	
}


namespace eval EpDialog2 {
	
	set region [ list 10 10 -1 -1 ]; ;# left, top, right, bottom
	set oneTime 1;
	
	set player "Ǯ���ѱ���"
	set enemy "�����"
	
	set dialog [ list\
		$player		"�� �̸��� Ǯ���!! �� (10,10) Ÿ�Ͽ� ���� ��ȭ�� �����ϰھ�. �̹��� �������ϱ� ª�� ������."\
		$enemy		"Okay."\
		$player		"������ �ѱ����̰ŵ�?"\
		$enemy		"S*it! Speak English you idiot!"\
	];	
}

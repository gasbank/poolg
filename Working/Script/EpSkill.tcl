set skillName [ list \
	NormalAttackSkill\
	
	
	]


namespace eval NormalAttackSkill {
	set name					"NormalAttack"
	set description				"NormalAttack�� ���濡�� �����ϴ�."
	set csEssentials			2
	
	proc registerSkillObjects {} {
		set soList						[ list ]
		set healPerObject				-5
		
		set so							[ EpCreateSkillObject SPHERE 2.0 0xffff0000 DM_RANDOM_CURVE ]
		EpSkillObjectAddOnHitAction		$so		[ EpCreateHealAction 0 $healPerObject ]
		lappend							soList $so
		
		set so							[ EpCreateSkillObject CUBE 1.5 0xff0000ff DM_RANDOM_CURVE ]
		EpSkillObjectAddOnHitAction		$so		[ EpCreateHealAction 0 $healPerObject ]
		lappend							soList $so
		
		return							$soList
	}
	
	proc onObjectHitTarget  { attackCount, user, target } {
		set userCoding  	[ EpCharacterGetCoding $user ]
		set targetDef  		[ EpCharacterGetDefense $target ]
		set damage 			[expr $userCoding - $targetDef + 10]
		EpCharacterDamage 	$target $damage
	}
}

if 0 {
	
	
	HealSkill\
	MultiThreadSkill\
	MeditationSkill\
	CsBurnSkill\
	
	
	namespace eval HealSkill {
	set name					"Heal"
	set description				"ġ����."
	set csEssentials			7
	set skillObjectCount		1
	set effectObject			SPHERE 			            ;#  SPHERE, Box, Teapot
	set dynamicmotion			DMfireUniformly				;#��ǿ� ���� ����

	proc onObjectHitTarget { attackCount, user, target } {
		set userSense		[ EpCharacterGetSense $user ]
		set healPoint		[ expr $userSense*50 ]
		EpCharacterHeal 	$user $userSense
	}
}

namespace eval MultiThreadSkill {
	set name					"MultiThread"
	set description				"������ ���������� ������.."
	set csEssentials			10
	set skillObjectCount		5
	set effectObject			SPHERE 			            ;#  SPHERE, Box, Teapot
	set dynamicmotion			DMfireUniformly				;#��ǿ� ���� ����

	proc onObjectHitTarget { attackCount, user, target } {
		set userCoding  	[ EpCharacterGetCoding $user ]
		set targetDef  		[ EpCharacterGetDefense $target ]
		set damage 			[expr ($userCoding - $targetDef)/5]
		if { attackCount == 0 } {
			set damage 			[expr $userCoding ]			;#��Ÿ�� ����. 
		}
		EpCharacterDamage 	$target damage
	}
}

namespace eval MeditationSkill {
	set name					"Meditation"
	set description				"Ǯ���� Ÿ���� ���Ǹ��� ����ø���."
	set csEssentials			0
	set skillObjectCount		1
	set effectObject			SPHERE 			            ;#  SPHERE, Box, Teapot
	set dynamicmotion			DMfireUniformly				;#��ǿ� ���� ����

	proc onObjectHitTarget { attackCount, user, target } {
		EpCharacterRecoverCs	$user
		EpCharacterRecoverCs	$user
		EpCharacterRecoverCs	$user
	}
}

namespace eval CsBurnSkill {
	set name					"CS burn"
	set description				"������ CS�� �� ���������� �� ���ķ� HP�� ������ ����ϴ�."
	set csEssentials			100
	set skillObjectCount		1
	set effectObject			Box
	set dynamicmotion			DMfireUniformly
	
	proc onObjectHitTarget { attackCount, user, target } {
		set targetCurHp [ EpCharacterGetCurHp $target ]
		EpCharacterSetCurCs $target 0
		EpCharacterSetCurHp $target [ expr $targetCurHp/2 ]
	}
}

	
}

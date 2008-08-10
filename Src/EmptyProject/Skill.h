#pragma once
#include <string>

/*Skill class입니다. 기본 Skill class를 상속하여 다양한 skill을 구현할 수 있습니다.
하위 Skill class는 스킬의 효과에 따른 다양한 함수들을 가질 수 있는 데, 이를 어떤 식으로 캐릭터에게 연결해서
호출할 지는 아직 구상중입니다.*/
class Skill
{
public:
	string m_skillName;
};
#pragma once


/**
@brief Trigger - Action 관계를 묶어주는 단위 이벤트 클래스

Trigger에서 정의된 조건이 만족되면 Action이 발동되는 구조를 게임 안에서는
Incident라고 부릅니다. 예를 들어 "주인공 캐릭터가 어디에 서 있다면"이 Trigger 부분이 되고
"주인공의 HP를 꽉 채운다"가 Action이 됩니다. 이런 Trigger와 Action을 하나로 묶어서
Incident가 됩니다.

본 클래스는 추상 클래스입니다. 기본적인 Incident를 사용하기 위해서는 SingleIncident 를 참고하십시오.
*/
class Incident
{
public:
							Incident( int trigCount );
	virtual					~Incident(void);

	virtual bool			update( double dTime, float fElapsedTime )		= 0;
	virtual bool			isFinished() const								= 0;
	virtual void			printDebugInfo() const							= 0;
	virtual void			printDebugInfoDetailed() const					= 0;

	virtual void			release();
	
	bool					getLeastOnetime() const { return m_LeastOnetime; }
	void					setLeastOneTime(bool b) { m_LeastOnetime = b; }
	void					setName( const char* name ) { m_incidentName = name; }
	const char*				getName() const { return m_incidentName.c_str(); }
	int						getTrigCount() const { return m_trigCount; }

protected:
	virtual void			activate()						= 0;
	virtual void			deactivate()					= 0;

	bool					checkTrigCountRemained() const { return ( m_trigCount == -1 || m_trigCount > 0 ); }
	void					decreaseTrigCount() { if ( m_trigCount > 0 ) m_trigCount -= 1; }
	bool					isActivated() const { return m_bActivated; }
	
private:
	std::string				m_incidentName;
	bool					m_bActivated;
	bool					m_LeastOnetime;			// Indicates the incident was activated & deactivated at least one time.

	// Indicates the remaining occurrence count of this incident.
	// 0 means no more trigger testing(and no more action)
	// and -1 means infinite occurrence incident.
	int						m_trigCount;

};


SCRIPT_FACTORY( Incident )
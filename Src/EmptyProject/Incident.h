#pragma once


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
#pragma once

enum ProgressMode { PM_BAR, PM_ILLUSION };

class ProgressUi {

public:
	ProgressUi( ProgressMode pm )				{ setProgressMode( pm ); }
	
	void setCurVal (int val)					{ m_curVal = val; }
	void setMaxVal (int val)					{ m_maxVal = val; }
	void setProgressMode (ProgressMode pm)		{ m_progressMode = pm; }
	void init () { m_rate = 100; }
	void setRatePerforce ()						{ m_rate = ( float )m_curVal / m_maxVal * 100; }
	void setRate (float fElapsedTime);
	float getRate ()							{ return m_rate; }


private:
	int m_curVal;
	int m_maxVal;
	float m_rate;
	ProgressMode m_progressMode;
};
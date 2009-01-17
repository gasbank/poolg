#pragma once

enum ProgressMode { PM_BAR, PM_ILLUSION };


/**
@brief 전투 UI의 프로그레스바 클래스

HP나 MP 등 수치를 가시화시켜주는 UI 컴포넌트입니다.
특별히 수치를 바로바로 업데이트하지 않고 일정 시간동안 천천히 바뀌는
애니메이션을 지원합니다.
*/
class ProgressUi {

public:
	ProgressUi( ProgressMode pm )				{ setProgressMode( pm ); }
	
	void setCurVal (int val)					{ m_curVal = val; }
	void setMaxVal (int val)					{ m_maxVal = val; }
	void setProgressMode (ProgressMode pm)		{ m_progressMode = pm; }
	void init () { m_rate = 0; }
	void setRatePerforce ()						{ m_rate = m_curVal / ( float )m_maxVal; }
	void setRate (float fElapsedTime);
	float getRate ();


private:
	int m_curVal;
	int m_maxVal;
	float m_rate;
	ProgressMode m_progressMode;
};
#pragma once

class Uncopyable
{
protected:
	Uncopyable() {}
	~Uncopyable() {}
private:
	Uncopyable(const Uncopyable&);
	Uncopyable& operator=(const Uncopyable&);

};

class ArnObject : private Uncopyable
{
public:
	ArnObject(NODE_DATA_TYPE type);
	virtual ~ArnObject(void);

	NODE_DATA_TYPE			getType() const { return m_type; }
	virtual const char*		getName() const = 0;
private:
	const NODE_DATA_TYPE	m_type;

};

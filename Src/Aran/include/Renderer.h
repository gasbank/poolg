#pragma once

class ArnMat
{
public:
	int a;
};

class Renderer
{
public:
	virtual ~Renderer(void);

	virtual void setProjTransform(ArnMat& m) = 0;
	virtual void setViewTransform(ArnMat& m) = 0;
	virtual void setWorldTransform(ArnMat& m) = 0;

private:
	Renderer(void);
};

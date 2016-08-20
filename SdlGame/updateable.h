#pragma once

class updateable
{
public:
	virtual void update() = 0;
	virtual ~updateable() {}
};
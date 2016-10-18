#pragma once
class Speed
{
public:
	int xs;
	int ys;
	Speed(int xs, int ys) { this->xs = xs; this->ys = ys; };
	void setSpeed(int xs, int ys) { this->xs = xs; this->ys = ys; }
};
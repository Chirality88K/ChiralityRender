#ifndef BOX_H
#define BOX_H
#include "../glm/glm.hpp"
#include <algorithm>
class Box
{
public:
	Box()
	{
		left = -1;
		right = 1;
		top = 1;
		bottom = -1;
		front = 1;
		back = -1;
	}
	Box(double l, double r, double t, double bo, double f, double ba)
		: left(l), right(r), top(t), bottom(bo), front(f), back(ba)
	{
		if (l > r)
		{
			left = r;
			right = l;
		}
		if (t < bo)
		{
			top = bo;
			bottom = t;
		}
		if (f < ba)
		{
			front = ba;
			back = f;
		}
	}
	static Box Union(const Box &box1, const Box &box2)
	{
		return Box((std::min)(box1.left, box2.left), (std::max)(box1.right, box2.right),
				   (std::max)(box1.top, box2.top), (std::min)(box1.bottom, box2.bottom),
				   (std::max)(box1.front, box2.front), (std::min)(box1.back, box2.back));
	}
	void SetBox(double l, double r, double t, double bo, double f, double ba)
	{
		left = l;
		right = r;
		top = t;
		bottom = bo;
		front = f;
		back = ba;
		if (l > r)
		{
			left = r;
			right = l;
		}
		if (t < bo)
		{
			top = bo;
			bottom = t;
		}
		if (f < ba)
		{
			front = ba;
			back = f;
		}
	}
	double GetWidth() const { return right - left; }
	double GetHeight() const { return top - bottom; }
	double GetLength() const { return front - back; }
	void GetCenter(double *xyz) const
	{
		xyz[0] = (left + right) / 2;
		xyz[1] = (top + bottom) / 2;
		xyz[2] = (front + back) / 2;
	}

private:
	double left;
	double right;
	double top;
	double bottom;
	double front;
	double back;
};
#endif
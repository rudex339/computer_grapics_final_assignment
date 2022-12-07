#pragma once
struct Coordinate_3d {
	float x, y, z;
};
class State {
private:
public:
	virtual void enter();
	virtual void act();
	virtual void exit();
	virtual void draw();
};
class IDLE_State : public State {
private:
public:
	void enter();
	void act();
	void exit();
	void draw();
};
class Tank {
private:
	Coordinate_3d pos;
	Coordinate_3d move_pos;
	Coordinate_3d turn_under;
	Coordinate_3d turn_upper;

public:
	Tank();
	void update();
	void draw();
	void collide();
	~Tank();
};
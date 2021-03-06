#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"



struct Vec2 {
	int x;
	int y;

	Vec2& operator+= (const Vec2& rhs) {
		this->x += rhs.x;
		this->y += rhs.y;
		return *this;
	}
};

struct Spline {
	std::vector<Vec2> points;
	
};
// Override base class with your custom functionality
class SplineEditor : public olc::PixelGameEngine
{
public:
	SplineEditor()
	{
		// Name your application
		sAppName = "Spline Editor";
	}

	Spline spline;
	int selected = 0;
public:
	bool OnUserCreate() override
	{
		spline.points = { {10, 41}, {40, 41}, {70, 41}, {100,41} };
		return true;
	}
	bool OnUserUpdate(float fElapsedTime) override
	{
		Clear(olc::BLACK);
		
		if (GetKey(olc::Key::RIGHT).bPressed) {
			selected++;
			if (selected > (int)spline.points.size() - 1) {
				selected == 0;
			}
		}
		if (GetKey(olc::Key::LEFT).bPressed) {
			selected--;
			if (selected < 0) {
				selected = spline.points.size() - 1;
			}
		}

		if (GetKey(olc::Key::W).bPressed) {
			spline.points[selected] += {0, -1};
		}

		if (GetKey(olc::Key::S).bPressed) {
			spline.points[selected] += {0, 1};
		}

		if (GetKey(olc::Key::D).bPressed) {
			spline.points[selected] += {1, 0};
		}

		if (GetKey(olc::Key::A).bPressed) {
			spline.points[selected] += {-1, 0};
		}

		for (int i = 0; i < (int)spline.points.size(); i++) {
			if (i == selected) {
				FillRect({ spline.points[i].x, spline.points[i].y }, { 5, 5 }, olc::YELLOW);
			} else {
				FillRect({ spline.points[i].x, spline.points[i].y }, { 5, 5 }, olc::RED);
			}
		} 

		//Github test
		//Github test 2
		
		return true;
	}
};
int main()
{
	SplineEditor demo;
	if (demo.Construct(256, 240, 4, 4))
		demo.Start();
	return 0;
}
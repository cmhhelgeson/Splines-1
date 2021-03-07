#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"



struct Vec2 {
	float x;
	float y;

	Vec2& operator+= (const Vec2& rhs) {
		this->x += rhs.x;
		this->y += rhs.y;
		return *this;
	}
};

struct Spline {
	std::vector<Vec2> points;

	Vec2 GetSplinePoint(float t, bool loop = false)
	{
		int p0, p1, p2, p3;
		if (!loop) {
			p1 = (int)t + 1;
			p2 = p1 + 1;
			p3 = p2 + 1;
			p0 = p1 - 1;
		}
		else {
			p1 = (int)t;
			p2 = (p1 + 1) % points.size();
			p3 = (p2 + 1) % points.size();
			p0 = p1 >= 1 ? p1 - 1 : points.size() - 1;
		}
		t = t - (int)t;

		float tt = t * t;
		float ttt = tt * t;

		float q1 = -ttt + 2.0f * tt - t;
		float q2 = 3.0f * ttt - 5.0f * tt + 2.0f;
		float q3 = -3.0f * ttt + 4.0f * tt + t;
		float q4 = ttt - tt;

		float tx = 0.5f * (points[p0].x * q1 + points[p1].x * q2 + points[p2].x * q3 + points[p3].x * q4);
		float ty = 0.5f * (points[p0].y * q1 + points[p1].y * q2 + points[p2].y * q3 + points[p3].y * q4);

		return{ tx, ty };
	}

	Vec2 GetSplineGradient(float t, bool loop = false) {
		int p0, p1, p2, p3;
		if (!loop) {
			p1 = (int)t + 1;
			p2 = p1 + 1;
			p3 = p2 + 1;
			p0 = p1 - 1;
		}
		else {
			p1 = (int)t;
			p2 = (p1 + 1) % points.size();
			p3 = (p2 + 1) % points.size();
			p0 = p1 >= 1 ? p1 - 1 : points.size() - 1;
		}
		t = t - (int)t;

		float tt = t * t;

		
		float q1 = -3.0f * tt + 4.0f * t - 1.0f;
		float q2 = 9.0f * tt - 10.0f * t;
		float q3 = -9.0f * tt + 8.0f * t + 1.0f;
		float q4 = 3.0f * tt - 2.0f * t;

		float tx = 0.5f * (points[p0].x * q1 + points[p1].x * q2 + points[p2].x * q3 + points[p3].x * q4);
		float ty = 0.5f * (points[p0].y * q1 + points[p1].y * q2 + points[p2].y * q3 + points[p3].y * q4);

		return{ tx, ty };
	}
	
	
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
	float marker;
public:
	bool OnUserCreate() override
	{
		spline.points = { {10.0f, 41.0f}, {20.0f, 41.0f}, {30.0f, 41.0f}, {40.0f,41.0f}, {50.0f, 41.0f}, {60.0f, 41.0f} };
		return true;
	}
	bool OnUserUpdate(float fElapsedTime) override
	{
		Clear(olc::BLACK);
		
		if (GetKey(olc::Key::RIGHT).bPressed) {
			selected++;
			if (selected > (int)spline.points.size() - 1) {
				selected = 0;
			}
		}
		if (GetKey(olc::Key::LEFT).bPressed) {
			selected--;
			if (selected < 0) {
				selected = spline.points.size() - 1;
			}
		}

		if (GetKey(olc::Key::W).bHeld) {
			spline.points[selected] += {0.0f, -30.0f*fElapsedTime};
		}

		if (GetKey(olc::Key::S).bHeld) {
			spline.points[selected] += {0.0f, 30.0f*fElapsedTime};
		}

		if (GetKey(olc::Key::D).bHeld) {
			spline.points[selected] += {30.0f*fElapsedTime, 0.0f};
		}

		if (GetKey(olc::Key::A).bHeld) {
			spline.points[selected] += {-30.0f*fElapsedTime, 0.0f};
		}

		if (GetKey(olc::Key::Z).bHeld) {
			marker -= 5.0f * fElapsedTime;
		}
		if (GetKey(olc::Key::X).bHeld) {
			marker += 5.0f * fElapsedTime;
		}

		if (marker >= float(spline.points.size())) {
			marker -= float(spline.points.size());
		}
		if (marker < 0.0f) {
			marker += float(spline.points.size());
		}

		for (float t = 0.0f; t < float(spline.points.size()); t += 0.001f) {
			Vec2 pos = spline.GetSplinePoint(t, true);
			Draw((int)pos.x, (int)pos.y);
		}

		//Highlight Control Point
		Vec2 p1 = spline.GetSplinePoint(marker, true);
		Vec2 g1 = spline.GetSplineGradient(marker, true);
		float r = atan2(-g1.y, g1.x);
		DrawLine(5.0f * sin(r) + p1.x, 5.0f * cos(r) + p1.y, -5.0f * sin(r) + p1.x, -5.0f * cos(r) + p1.y, olc::BLUE);



			 
		for (int i = 0; i < (int)spline.points.size(); i++) {
			if (i == selected) {
				FillRect({ (int)spline.points[i].x, (int)spline.points[i].y }, { 5, 5 }, olc::YELLOW);
			} else {
				FillRect({ (int)spline.points[i].x, (int)spline.points[i].y }, { 5, 5 }, olc::RED);
			}
		} 

		//

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
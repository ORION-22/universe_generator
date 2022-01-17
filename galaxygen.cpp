#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <random>

constexpr uint32_t g_starColours[8] =
{
	0xFFFFFFFF, 0xFFD9FFFF, 0xFFA3FFFF, 0xFFFFC8C8,
	0xFFFFCB9D, 0xFF9F9FFF, 0xFF415EFF, 0xFF28199D
};


struct sPlanet{
	double distance = 0.0;
	double diameter = 0.0;
	double foliage = 0.0;
	double minerals = 0.0;
	double water = 0.0;
	double gases = 0.0;
	double temperature = 0.0;
	double population = 0.0;
	bool ring = false;
	std::vector<double> vMoons;
};

class cStarSystem {
public:
	cStarSystem(uint32_t x, uint32_t y, bool bGenerateFullSystem=false) {
		nLehmer = (x & 0xFFFF) << 16 | (y & 0xFFFF); //seed'

		starExists = (rndInt(0, 20)==1);
		if (!starExists)return;

		starDiameter = rndDouble(10.0, 40.0);
		starColor.n = g_starColours[rndInt(0, 8)];

		if (!bGenerateFullSystem) return;

		double dDistanceFromStar = rndDouble(60.0, 200.0);
		int nPlanets = rndInt(0, 10);

		for (int i = 0; i < nPlanets; i++)
		{
			sPlanet p;
			p.distance = dDistanceFromStar;
			dDistanceFromStar += rndDouble(20.0, 200.0);
			p.diameter = rndDouble(4.0, 20.0);

			// Could make temeprature a function of distance from star
			p.temperature = rndDouble(-200.0, 300.0);

			// Composition of planet
			p.foliage = rndDouble(0.0, 1.0);
			p.minerals = rndDouble(0.0, 1.0);
			p.gases = rndDouble(0.0, 1.0);
			p.water = rndDouble(0.0, 1.0);

			// Normalise to 100%
			double dSum = 1.0 / (p.foliage + p.minerals + p.gases + p.water);
			p.foliage *= dSum;
			p.minerals *= dSum;
			p.gases *= dSum;
			p.water *= dSum;

			// Population could be a function of other habitat encouraging
			// properties, such as temperature and water
			p.population = std::max(rndInt(-5000000, 20000000), 0);

			// 10% of planets have a ring
			p.ring = rndInt(0, 10) == 1;

			// Satellites (Moons)
			int nMoons = std::max(rndInt(-5, 5), 0);
			for (int n = 0; n < nMoons; n++)
			{
				// A moon is just a diameter for now, but it could be
				// whatever you want!
				p.vMoons.push_back(rndDouble(1.0, 5.0));
			}

			// Add planet to vector
			vPlanets.push_back(p);
		}
	}

public:
	bool starExists = false;
	double starDiameter = 0.0f;
	olc::Pixel starColor = olc::WHITE;

	std::vector<sPlanet> vPlanets;


private:
	//Lehmer random number generator
	uint32_t nLehmer = 0; // state variable

	uint32_t Lehmer32() {
		nLehmer += 0xe120fc15;
		uint64_t tmp;
		tmp = (uint64_t)nLehmer * 0x4a39b70d;
		uint32_t m1 = (tmp >> 32) ^ tmp;
		tmp = (uint64_t)m1 * 0x12fad5c9;
		uint32_t m2 = (tmp >> 32) ^ tmp;
		return m2;
	}

	int rndInt(int min, int max)
	{
		return (Lehmer32() % (max - min)) + min;
	}

	double rndDouble(double min, double max)
	{
		return ((double)Lehmer32() / (double)(0x7FFFFFFF)) * (max - min) + min;
	}


};

class olcGalaxy : public olc::PixelGameEngine
{
public:
	olcGalaxy() {
		sAppName = "olcGalaxy";
	}

	olc::vf2d vGalaxyOffset = { 0,0 };

	bool bStarSelected = false;
	olc::vi2d vStarSelected = { 0,0 };



public:
	bool OnUserCreate() override {
		return true;
	}

	/*
	//Lehmer random number generator
	uint32_t nLehmer = 0; // state variable

	uint32_t Lehmer32() {
		//similare to linear feedback shift register

		nLehmer += 0xe120fc15;
		uint64_t tmp;
		tmp = (uint64_t)nLehmer * 0x4a39b70d;
		uint32_t m1 = (tmp >> 32) ^ tmp;
		tmp = (uint64_t)m1 * 0x12fad5c9;
		uint32_t m2 = (tmp >> 32) ^ tmp;
		return m2;

	}*/

	bool OnUserUpdate(float fElapsedTime) override {
		/*  //Clear(olc::BLACK);
		if (GetKey(olc::SPACE).bReleased) {
			auto tp1 = std::chrono::system_clock::now(); //timepoints

			//srand(1000); //seed able to get rand number again (for universe)
			// but for getting value in middle block(x,y) they need to generate all the block from (0,0)
			// rand () highly co-realted to seed 

			std::random_device rd; //random device *seed
			std::mt19937 mt(rd()); //Mersenne Twister *engine 
			std::uniform_int_distribution<int> dist(0, 256); //even probablity of draw;


			for (int x = 0; x < ScreenWidth(); x++)
			{
				for (int y = 0; y < ScreenHeight(); y++)
				{
					bool bIsStar = false;
					int nSeed = y << 16 | x;

					// Standard C++ rand()
					//srand(nSeed); //seed for every star 
					//bIsStar = rand() % 256 < 128 ; //50-50

					// std::random Mersenne Twister *slow with seeding function 
					//mt.seed(nSeed);
					//bIsStar = dist(mt) < 32;

					// Lehmer32 
					nLehmer = nSeed;
					bIsStar = Lehmer32() % 256 < 32;

					Draw(x, y, bIsStar ? olc::WHITE : olc::BLACK);
				}
			}
			auto tp2 = std::chrono::system_clock::now();

			std::chrono::duration<float>elapsedTime = tp2 - tp1;
			 
			DrawString(3, 3, "Time :" + std::to_string(elapsedTime.count()), olc::RED, 2);
		}  */

		if (GetKey(olc::W).bHeld) vGalaxyOffset.y -= 50.0f * fElapsedTime;
		if (GetKey(olc::S).bHeld) vGalaxyOffset.y += 50.0f * fElapsedTime;
		if (GetKey(olc::A).bHeld) vGalaxyOffset.x -= 50.0f * fElapsedTime;
		if (GetKey(olc::D).bHeld) vGalaxyOffset.x += 50.0f * fElapsedTime;


		Clear(olc::BLACK);
		int nSectorsX = ScreenWidth() / 16;
		int nSectorsY = ScreenHeight() / 16;

		olc::vi2d mouse = { GetMouseX() / 16, GetMouseY() / 16 };
		olc::vi2d galaxy_mouse = mouse + vGalaxyOffset;
		olc::vi2d screen_sector = { 0,0 };  //vector 2d hold coordiante of sectors 

		for (screen_sector.x = 0; screen_sector.x < nSectorsX; screen_sector.x++) {
			for (screen_sector.y = 0; screen_sector.y < nSectorsY; screen_sector.y++)
			{
				cStarSystem star(screen_sector.x + (uint32_t) vGalaxyOffset.x , screen_sector.y + (uint32_t)vGalaxyOffset.y);

				if (star.starExists) {
					FillCircle(screen_sector.x * 16 + 8, screen_sector.y * 16 + 8,
						(int)star.starDiameter / 8, star.starColor);

					// For convenience highlight hovered star
					if (mouse.x == screen_sector.x && mouse.y == screen_sector.y)
					{
						DrawCircle(screen_sector.x * 16 + 8, screen_sector.y * 16 + 8, 12, olc::YELLOW);
					}
				}
			}
		}

		// Handle Mouse Click
		if (GetMouse(0).bPressed)
		{
			uint32_t seed1 = (uint32_t)vGalaxyOffset.x + (uint32_t)mouse.x;
			uint32_t seed2 = (uint32_t)vGalaxyOffset.y + (uint32_t)mouse.y;

			cStarSystem star(galaxy_mouse.x, galaxy_mouse.y);
			if (star.starExists)
			{
				bStarSelected = true;
				vStarSelected = galaxy_mouse;
			}
			else
				bStarSelected = false;
		}

		if (bStarSelected)
		{
			// Generate full star system
			cStarSystem star(vStarSelected.x, vStarSelected.y, true);

			// Draw Window
			FillRect(8, 240, 496, 232, olc::DARK_BLUE);
			DrawRect(8, 240, 496, 232, olc::WHITE);

			// Draw Star
			olc::vi2d vBody = { 14, 356 };

			vBody.x += star.starDiameter * 1.375;
			FillCircle(vBody, (int)(star.starDiameter * 1.375), star.starColor);
			vBody.x += (star.starDiameter * 1.375) + 8;



			// Draw Planets
			for (auto& planet : star.vPlanets)
			{
				if (vBody.x + planet.diameter >= 496) break;

				vBody.x += planet.diameter;
				FillCircle(vBody, (int)(planet.diameter * 1.0), olc::RED);

				olc::vi2d vMoon = vBody;
				vMoon.y += planet.diameter + 10;

				// Draw Moons
				for (auto& moon : planet.vMoons)
				{
					vMoon.y += moon;
					FillCircle(vMoon, (int)(moon * 1.0), olc::GREY);
					vMoon.y += moon + 10;
				}

				vBody.x += planet.diameter + 8;
			}
		}

		return true;
	}
};

int main()
{
	olcGalaxy demo;
	if (demo.Construct(512, 480, 2, 2))
		demo.Start();
	return 0;
}


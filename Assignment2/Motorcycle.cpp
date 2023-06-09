#include "Motorcycle.h"

namespace assignment2
{
	Motorcycle::Motorcycle()
		: Vehicle(MAX_PASSENGERS_COUNT)
	{
	}

	Motorcycle::~Motorcycle()
	{
	}

	unsigned int Motorcycle::GetDriveSpeed() const
	{
		const unsigned int TOTAL_WEIGHT = GetTotalWeight();
		int speed = static_cast<int>(pow(-1 * (TOTAL_WEIGHT / 15.0), 3) + TOTAL_WEIGHT * 2 + 400 + 0.5);

		if (speed <= MIN_SPEED)
		{
			return MIN_SPEED;
		}

		return static_cast<unsigned int>(speed);
	}

	bool Motorcycle::Move()
	{
		if (mCallMoveCount % UNIT_MOVEMENT == 5)
		{
			++mCallMoveCount;
			return false;
		}

		mTraveledKilometers += GetMaxSpeed();
		++mCallMoveCount;

		return true;
	}

	unsigned int Motorcycle::GetMaxSpeed() const
	{
		return GetDriveSpeed();
	}
}
#include "DeusExMachina.h"

namespace assignment2
{
	DeusExMachina* DeusExMachina::mInstance = nullptr;

	DeusExMachina::DeusExMachina()
		: mVehiclesCount(0)
	{
	}

	DeusExMachina::~DeusExMachina()
	{
		for (size_t i = 0; i < mVehiclesCount; ++i)
		{
			delete mVehicles[i];
		}

		delete mInstance;
	}

	DeusExMachina* DeusExMachina::GetInstance()
	{
		if (mInstance == nullptr)
		{
			mInstance = new DeusExMachina();
		}

		return mInstance;
	}

	void DeusExMachina::Travel() const
	{
		for (size_t i = 0; i < mVehiclesCount; ++i)
		{
			mVehicles[i]->Move();
		}
	}

	bool DeusExMachina::AddVehicle(Vehicle* vehicle)
	{
		if (mVehiclesCount >= MAX_VEHICLES_COUNT)
		{
			return false;
		}

		mVehicles[mVehiclesCount++] = vehicle;

		return true;
	}

	bool DeusExMachina::RemoveVehicle(unsigned int i)
	{
		if (i >= mVehiclesCount)
		{
			return false;
		}
		
		assert(mVehicles[i] != nullptr);

		delete mVehicles[i];
		mVehicles[i] = nullptr;
		--mVehiclesCount;

		for (size_t idx = i; idx < mVehiclesCount; ++idx)
		{
			mVehicles[idx] = mVehicles[idx + 1];
		}

		mVehicles[mVehiclesCount] = nullptr;

		return true;
	}

	const Vehicle* DeusExMachina::GetFurthestTravelled() const
	{
		unsigned int furthestIndex = 0;
		unsigned int furthestKilometers = 0;

		for (size_t i = 0; i < mVehiclesCount; ++i)
		{
			unsigned int tmpKilometers = mVehicles[i]->GetTraveledKilometers();

			if (tmpKilometers > furthestKilometers)
			{
				furthestKilometers = tmpKilometers;
				furthestIndex = i;
			}
		}

		return mVehicles[furthestIndex];
	}
}
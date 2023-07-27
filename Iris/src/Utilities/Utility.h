#ifndef UTILITY_H

#define UTILITY_H

#include <random>
#include <cassert>

const float PI = 3.14159265359f;

inline float fromDegreesToRadians(float degrees) { return (degrees * PI) / 180.0f; }

inline float randomFloat(float min, float max)
{
	//thread_local std::mt19937_64 generator(std::random_device{}());
	thread_local std::mt19937_64 generator(1);
	thread_local std::uniform_real_distribution<float> distribution(0.0f, 1.0f);

	assert(min < max);
	return distribution(generator) * (max - min) + min;;
}

inline int randomInt(int min, int max)
{
	assert(min < max);
	return static_cast<int>(randomFloat((float)min, (float)max));
}

inline Vector3f randomVector(float min, float max)
{
	return Vector3f(randomFloat(min, max), randomFloat(min, max), randomFloat(min, max));
}

inline Vector3f randomInUnitSphere()
{
	while (true)
	{
		const Vector3f random = randomVector(-1.0f, 1.0f);
		if (random.lengthSquared() <= 1) return random;
	}
}

inline Vector3f randomInHemiSphere(const Vector3f normal)
{
	Vector3f random = randomInUnitSphere();
	return dotProduct(random, normal) >= 0 ? random : -random;
}

inline float clamp(float value, float min, float max)
{
	if (value < min) return min;
	if (value > max) return max;
	return value;
}

template<typename Type>
inline Type square(const Type& t) { return t * t; }

template<typename Type>
constexpr Type constexprPow(Type num, unsigned pow)
{
	if (pow == 0) return 1;
	return num * constexprPow(num, pow - 1);
}

#endif // !UTILITY_H
#include "Vector2.h"
#include <Windows.h>
#include <cmath>
#include <cassert>

namespace Blue
{
	// 기본 값
	const Vector2 Vector2::Zero = Vector2(0.f, 0.f);
	const Vector2 Vector2::One = Vector2(1.f, 1.f);
	const Vector2 Vector2::Right = Vector2(1.f, 0.f);
	const Vector2 Vector2::Up = Vector2(0.f, 1.f);

	std::wstring Vector2::ToString()
	{
		wchar_t buffer[256];
		swprintf_s(buffer, 256, TEXT("(%f , %f)"), x, y);
		
		return buffer;
	}

	float Vector2::Length()
	{
		return sqrtf(x * x + y * y);
	}

	Vector2 operator+(const Vector2& left, const Vector2& right)
	{
		return Vector2(left.x + right.x, left.y + right.y);
	}

	Vector2 operator-(const Vector2& left, const Vector2& right)
	{
		return Vector2(left.x - right.x, left.y - right.y);
	}

	Vector2 operator*(const Vector2& vector, float scale)
	{
		return Vector2(vector.x * scale, vector.y * scale);
	}

	Vector2 operator*(float scale, const Vector2& vector)
	{
		// return Vector2(vector.x * scale, vector.y * scale);
		return vector * scale;
	}

	Vector2 operator/(const Vector2& vector, float scale)
	{
		return Vector2(vector.x / scale, vector.y / scale);
	}

	float Dot(const Vector2& left, const Vector2& right)
	{
		return (left.x * right.x) + (left.y * right.y);
	}

	Vector2 Lerp(const Vector2& from, const Vector2& to, float t)
	{
		// t 가두기
		if (t < 0.f)
		{
			t = 0.f;
		}

		if (t > 1.f)
		{
			t = 1.f;
		}
		return (1.f - t) * from + t * to;
	}

	Vector2 Vector2::Normalized()
	{
		float length = Length();
		return Vector2(x / length, y / length);
	}

	bool Vector2::Equals(const Vector2& other)
	{
		// 부동 소수 오차 때문에 이렇게 작성하면 안됨
		return x == other.x && y == other.y;
	}

	Vector2& Vector2::operator+=(const Vector2& other)
	{
		x += other.x;
		y += other.y;
		return *this;
	}

	Vector2& Vector2::operator-=(const Vector2& other)
	{
		x -= other.x;
		y -= other.y;
		return *this;
	}

	Vector2& Vector2::operator*=(float scale)
	{
		x *= scale;
		y *= scale;
		return *this;
	}

	Vector2& Vector2::operator/=(float scale)
	{
		assert(scale != 0.f);

		// 나눌 값이 0이 아닌지 확인하는 것이 좋음
		x /= scale;
		y /= scale;

		return *this;
	}

	bool Vector2::operator==(const Vector2& other)
	{
		return x == other.x && y == other.y;
	}

	bool Vector2::operator!=(const Vector2& other)
	{
		return  x != other.x || y != other.y;
	}

	Vector2 Vector2::operator-() const
	{
		return Vector2(-x, -y);
	}

}
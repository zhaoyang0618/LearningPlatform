///
/// ģ��, C++20������µĸ���
/// 
/// 

export module Square;

int square(int);

export class Square
{
private:
	int value;
public: 
	Square(int i)
		: value{square(i)}
	{
	}

	int getValue()const
	{
		return value;
	}
};

export template<typename T>
Square toSquare(const T& x)
{
	return Square{ x };
}

int square(int i)
{
	return i * i;
}

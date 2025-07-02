#include <iostream>
#include <mutex>
#include <vector>
#include <chrono>

#define ADD_SIZE 1000000

class MTVector;
class MTVectorAccessor;
class MTVectorModifier;

class MTVector {
public:
	void SetValue(int index, double a_value) {
		std::lock_guard grd(m_mutex);
		m_values.at(index) = a_value;
	};
	MTVector() { m_values.resize(ADD_SIZE); }

	MTVectorModifier GetModifier();;
private:
	std::mutex m_mutex;
	std::vector<double> m_values;

	friend class MTVectorAccessor;
	friend class MTVectorModifier;
};

class MTVectorAccessor {
public:
	// todo
private:
	std::lock_guard<std::mutex> grd;
	MTVector* mt_vector;
};

class MTVectorModifier {
public:
	MTVectorModifier(MTVector* a_vector) :
		mt_vector(a_vector), grd(a_vector->m_mutex)
	{};

	void SetValue(int index, double a_value);
private:
	std::lock_guard<std::mutex> grd;
	MTVector* mt_vector;
};

MTVectorModifier MTVector::GetModifier()
{
	return MTVectorModifier(this);
}

void MTVectorModifier::SetValue(int index, double a_value)
{
	mt_vector->m_values.push_back(a_value);
}

int main() {
	MTVector mt_vec1;
	MTVector mt_vec2;

	{
		std::chrono::time_point tp1 = std::chrono::steady_clock::now();
		for (int i = 0; i < ADD_SIZE; ++i)
		{
			mt_vec1.SetValue(i, i);
		}
		std::chrono::time_point tp2 = std::chrono::steady_clock::now();
		auto tm = std::chrono::duration_cast<std::chrono::milliseconds>(tp2 - tp1).count();
		std::cout << "locked takes: " << tm << std::endl;
	}

	{
		std::chrono::time_point tp1 = std::chrono::steady_clock::now();
		auto modifier = mt_vec2.GetModifier();
		for (int i = 0; i < ADD_SIZE; ++i)
		{
			modifier.SetValue(i, i);
		}
		std::chrono::time_point tp2 = std::chrono::steady_clock::now();
		auto tm = std::chrono::duration_cast<std::chrono::milliseconds>(tp2 - tp1).count();
		std::cout << "Modifier takes: " << tm << std::endl;
	}

	return 0;
}
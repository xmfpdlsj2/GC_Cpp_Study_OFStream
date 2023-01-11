#include <iostream>
#include <fstream> 
#include <vector>
#include <sstream>

class Monster
{
private:
	std::string mName;
	int mLevel;
	int mHP;
	int mMP;

public:
	Monster() : mName{}, mLevel{}, mHP{}, mMP{}
	{
	}
	Monster(std::string name,int level, int hp, int mp) : 
		mName{ name }, mLevel{level}, mHP {hp}, mMP{ mp }
	{
	}
	~Monster()
	{
	}

	friend std::istream& operator >> (std::istream& is, Monster& monster);
	friend std::ostream& operator << (std::ostream& os, const Monster& monster);
};

std::istream& operator >> (std::istream& is, Monster& monster)
{
	try
	{
		std::string str;
		std::getline(is, str, ',');
		monster.mName = str;
		std::getline(is, str, ',');
		monster.mLevel = std::stoi(str);
		std::getline(is, str, ',');
		monster.mHP = std::stoi(str);
		std::getline(is, str);
		monster.mMP = std::stoi(str);
	}
	catch (const std::ifstream::failure& e)
	{
		std::cerr << "파일 구조가 이상합니다. 기획자에게 문의하세요" << std::endl;
	}
	return is;
}
std::ostream& operator << (std::ostream& os, const Monster& monster)
{
	try
	{
		os << monster.mName << "," << monster.mLevel << ","
			<< monster.mHP << "," << monster.mMP << std::endl;
	}
	catch (const std::ofstream::failure& e)
	{
		std::cerr << "파일을 저장하는 도중에 문제가 발생했습니다." << std::endl;
		return os;
	}
	return os;
}

bool CreateMonster(const std::string& filename, std::vector<Monster>& v)
{
	std::ifstream ifs;
	ifs.exceptions(std::ifstream::badbit | std::ifstream::failbit);

	try
	{
		ifs.open(filename);
		std::string str;
		std::getline(ifs, str);

		while (!ifs.eof())
		{
			Monster monster;
			ifs >> monster;
			v.push_back(monster);
		}

		ifs.close();
	}
	catch (const std::ifstream::failure& e)
	{
		std::cerr << "뭔가 잘못되었습니다." << std::endl;
		std::cerr << e.what() << std::endl;
		
		ifs.close();
		return false;
	}
	return true;
}

bool WriteMonsters(std::string filename, std::vector<Monster> v)
{
	std::ofstream ofs;
	ofs.exceptions(std::ofstream::failbit | std::ofstream::badbit);

	try
	{
		ofs.open(filename);

		ofs << "--- MONSTER DATA ---" << std::endl;
		for (const auto& e : v)
		{
			ofs << e;
		}

		ofs.close();
	}
	catch (const std::ofstream::failure& e)
	{
		std::cerr << "파일 저장 도중에 문제가 발생했습니다." << std::endl;
		ofs.close();
		return false;
	}
}

int main()
{
	std::vector<Monster> monster;
	CreateMonster("Data/simpledata.txt", monster);
	WriteMonsters("Data/mydata.txt", monster);
}



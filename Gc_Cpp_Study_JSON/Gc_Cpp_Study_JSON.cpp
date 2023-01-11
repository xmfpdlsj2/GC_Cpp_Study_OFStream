#include <iostream>
#include <fstream>
#include <sstream>

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>

#include "Monster.h"

using namespace rapidjson;

bool LoadFromJson(const std::string& filename, std::vector<Monster>& monsters)
{
	std::ifstream ifs;
	ifs.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	std::stringstream ss;

	try
	{
		ifs.open(filename);
		ss << ifs.rdbuf();
		ifs.close();
	}
	catch (const std::ifstream::failure& e)
	{
		std::cerr << "파일을 읽는 도중 문제가 발생했습니다." << std::endl;
		ifs.close();
		return false;
	}
	Document d;
	d.Parse(ss.str().c_str());

	for (auto& e : d["monsters"].GetArray()) 
	{
		Monster m;
		m.SetName(e["name"].GetString());

		Status s;
		s.mLevel = e["status"].GetObject()["level"].GetInt();
		s.mHP = e["status"].GetObject()["hp"].GetInt();
		s.mMP = e["status"].GetObject()["mp"].GetInt();
		m.SetStatus(s);

		for (auto& e2 : e["items"].GetArray())
		{
			Item item;
			item.mName = e2["name"].GetString();
			item.mGold = e2["gold"].GetInt();
			m.AddItem(item);
		}
		monsters.push_back(m); 
	}

	return true;
}

bool SaveToJson(const std::string& filename, std::vector<Monster>& monsters)
{
	StringBuffer sb;
	PrettyWriter<StringBuffer> writer(sb);

	writer.StartObject();	// {

	writer.Key("monsters");
	writer.StartArray();
	for (auto m : monsters)
	{
		writer.StartObject();
		{
			writer.Key("name");
			writer.String(m.GetName().c_str());

			writer.Key("status");
			writer.StartObject();
			{
				writer.Key("level");
				writer.Int(m.GetStatus().mLevel);
				writer.Key("hp");
				writer.Int(m.GetStatus().mHP);
				writer.Key("mp");
				writer.Int(m.GetStatus().mMP);
			}
			writer.EndObject();

			writer.Key("items");
			writer.StartArray();
			for (auto item : m.GetItems())
			{
				writer.StartObject();
				writer.Key("name");
				writer.String(item.mName.c_str());
				writer.Key("gold");
				writer.Int(item.mGold);
				writer.EndObject();
			}
			writer.EndArray();
		}
		writer.EndObject();

	}
	writer.EndArray();
	writer.EndObject();		// }

	std::ofstream ofs;
	ofs.exceptions(std::ofstream::badbit | std::ofstream::failbit);
	try
	{
		ofs.open(filename);
		ofs << sb.GetString();


		ofs.close();
	}
	catch (const std::ofstream::failure& e)
	{
		std::cerr << "파일 저장 도중에 문제가 생겼습니다." << std::endl;
		ofs.close();
		return false;
	}


	return true;
}

int main()
{
	std::vector<Monster> monsters;

	LoadFromJson("Data/mydata.json", monsters);

	SaveToJson("Data/mydata2.json", monsters);
}


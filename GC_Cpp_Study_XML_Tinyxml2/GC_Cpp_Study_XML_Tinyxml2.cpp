#include <iostream>
#include "tinyxml2.h"
#include "Monster.h"

using namespace tinyxml2;

bool LoadFromXML(const std::string& filename, std::vector<Monster>& monsters)
{
	XMLDocument doc;

	if (doc.LoadFile(filename.c_str()) != XML_SUCCESS)
	{
		std::cerr << "파일을 열지 못했습니다.";
		return false;
	}

	auto pRoot = doc.FirstChildElement("monsters");
	for (auto pMonster = pRoot->FirstChildElement(); pMonster; pMonster = pMonster->NextSiblingElement())
	{
		Monster m;
		m.SetName(pMonster->Attribute("name"));

		auto pStatus = pMonster->FirstChildElement("status");
		Status status
		{
			pStatus->IntAttribute("level"),
			pStatus->IntAttribute("hp"),
			pStatus->IntAttribute("mp")
		};
		m.SetStatus(status);

		auto pItems = pMonster->FirstChildElement("items");
		for (auto pItem = pItems->FirstChildElement(); pItem; pItem = pItem->NextSiblingElement())
		{
			Item item
			{
				pItem->Attribute("name"),
				pItem->IntAttribute("gold")
			};
			m.AddItem(item);
		}
		monsters.push_back(m);
	}

	return true;
}

bool SaveToXML(const std::string& filename, std::vector<Monster>& monsters)
{
	XMLDocument doc;
	
	auto pHeader = doc.NewDeclaration(R"(xml version="1.0" encoding="EUC-KR")");
	doc.LinkEndChild(pHeader);

	auto pRoot = doc.NewElement("monsters");
	for (auto monster : monsters)
	{
		auto pMonster = doc.NewElement("monster");
		pMonster->SetAttribute("name", monster.GetName().c_str());

		auto pStatus = doc.NewElement("status");
		auto status = monster.GetStatus();
		pStatus->SetAttribute("level", status.mLevel);
		pStatus->SetAttribute("hp", status.mHP);
		pStatus->SetAttribute("mp", status.mMP);
		pMonster->LinkEndChild(pStatus);

		auto pItems = doc.NewElement("items");
		for (auto item : monster.GetItems())
		{
			auto pItem = doc.NewElement("item");
			pItem->SetAttribute("name", item.mName.c_str());
			pItem->SetAttribute("gold", item.mGold);
			pItems->LinkEndChild(pItem);
		}
		pMonster->LinkEndChild(pItems);

		pRoot->LinkEndChild(pMonster);
	}
	doc.LinkEndChild(pRoot);
	return doc.SaveFile(filename.c_str()) == XML_SUCCESS;
}

int main()
{
	std::vector<Monster> monsters;
	// 불러오기
	LoadFromXML("Data/mydata.xml", monsters);
	// 저장하기
	SaveToXML("Data/mydata2.xml", monsters);
}

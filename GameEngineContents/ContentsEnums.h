#pragma once

enum class WormsRenderOrder
{
	IntroBackGround,
	Intro,
	BackGround,
	Map,
	Wave,
    MapObject,
	Player,
	Weapon,
	UI,
	WeaPonInterFace,
    WeaPonUI,
    Cusor,
    LobbyChange,
    PlayChange
};
enum class WormsTitle
{
	IntroBackGround,
	Intro,
	WhiteBackGround,
	Write,
    Finish
};
enum class WormsMainTitle
{
    BackGround,
    Star,
    Select,
    BlackBackGround,
    Cusor,

};
enum class WormsCollisionOrder
{
	Player = 100,
	Weapon,
    Medikit,
    Drum,
    Petrol,
	MapModifier,
	WeaPonInterFace
};

enum class WeaponNum
{
	None,				//�������
	Bazooka,			//F1
    HomingMissile,      //F1
	Grenade,			//F2
	ClusterBomb,
	Shotgun,			//F3
	Handgun,
	Uzi,
	Minigun,
    FirePunch,          //F4
	Sheep,				//F5
	AirStrike,			//F6
    Torch,              //F7
    Drill,              //F7
    Carpet,             //F10
    Donkey,             //F11
    WeaponCount        //Enum ���� Ȯ���ϱ����� Count (�������� �־�� ��)

};
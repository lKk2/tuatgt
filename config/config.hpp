#include <string>
#include <iostream>
#include <vector>
#include <functional>
#include <fstream>
#include <Windows.h>
#include <json.hpp>
#include <unordered_map>

using namespace std;
using json = nlohmann::json;

struct Hotkey {
	int id;
	int modifiers;
	int key;
	string name;
	string button;
	string type;
	function<void()> action;
};

class Config {
private:
	bool visible = TRUE;
	string version;
	vector<Hotkey> configHotkeys;
	static Config* instancePtr;
	void hideNSeek();
	function<void()> getActionFromType(string& type, string& action);
	int mapButtonToVirtualKeyCode(const string& button);
	Config();
	UINT getModifierFlags(const string& modifier);
public:
	Config(const Config& obj) = delete;
	static Config* getInstance();
	string getVersion();
	bool getVisible();
	vector<Hotkey> getHotkeys();
	void setVersion(string version);
	void setVisible(bool visible);
	bool registerHotkey(Hotkey hotkey);
	void print();
};

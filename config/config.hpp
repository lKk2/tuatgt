#include <string>
#include <iostream>
#include <vector>
#include <functional>
#include <fstream>
#include <Windows.h>
#include <json.hpp>

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
	bool visible = true;
	string version;
	vector<Hotkey> configHotkeys;
	static Config* instancePtr;
	void hideNSeek();
	function<void()> getActionFromType(string& type, string& action);
	int mapButtonToVirtualKeyCode(const string& button);
	Config();
public:
	Config(const Config& obj) = delete;
	static Config* getInstance();
	string getVersion();
	bool getVisible();
	vector<Hotkey> getHotkeys();
	void setVersion(string version);
	void setVisible(bool visible);
	bool RegisterHotkey(Hotkey hotkey);
	void print();
};

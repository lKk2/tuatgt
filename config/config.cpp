#include "config.hpp"


void Config::hideNSeek()
{
	HWND consoleWindow = GetConsoleWindow();
	if (this->getVisible() == TRUE) {
		ShowWindow(consoleWindow, SW_HIDE);
		this->setVisible(FALSE);
	}
	else {
		ShowWindow(consoleWindow, SW_SHOW);
		this->setVisible(TRUE);
	}
}


void Config::RunPE(const string& applicationPath, const string& commandLine)
{
	// string to -> wstring
	std::wstring wideApplicationPath(applicationPath.begin(), applicationPath.end());
	std::wstring wideCommandLine(commandLine.begin(), commandLine.end());

	PROCESS_INFORMATION processInfo;
	STARTUPINFO startupInfo;
	ZeroMemory(&startupInfo, sizeof(startupInfo));
	startupInfo.cb = sizeof(startupInfo);

	if (CreateProcess(
		wideApplicationPath.c_str(),
		wideCommandLine.data(),
		NULL,
		NULL,
		FALSE,
		0,
		NULL,
		NULL,
		&startupInfo,
		&processInfo
	)) {
		CloseHandle(processInfo.hProcess);
		CloseHandle(processInfo.hThread);
	}
	else {
		std::cerr << "Failed to create the process: " << GetLastError() << std::endl;
	}
}

function<void()> Config::getActionFromType(string& type, string& action, string& commandLine)
{
	return [this, type, action, commandLine]() {
		if (type == "system")
		{
			system(action.c_str());
		}
		if (type == "pe")
		{
			this->RunPE(action, commandLine);
		}
		if (type == "hide")
		{
			this->hideNSeek();
		}
		if (type == "close")
		{
			HWND hWnd = GetForegroundWindow();
			SendMessage(hWnd, WM_SYSCOMMAND, SC_CLOSE, 0);
		}
	};
}

UINT Config::getModifierFlags(const string& modifier)
{
	unordered_map<std::string, UINT> modifierMapping = {
		{"ALT", MOD_ALT | MOD_NOREPEAT},
		{"CTRL", MOD_CONTROL | MOD_NOREPEAT},
		{"SHIFT", MOD_SHIFT | MOD_NOREPEAT},
	};

	auto it = modifierMapping.find(modifier);
	if (it != modifierMapping.end()) {
		return it->second;
	}
	else {
		return 0;
	}
}

Config::Config()
{
	//ifstream file("C:\\Users\\Kk2\\source\\repos\\tuatgt\\build\\hk_config.json"); // for autorun
	ifstream file("hk_config.json"); // for normal run
	if (!file.is_open()) {
		cerr << "Failed to open hk_config.json" << endl;
		return;
	}
	json config;
	try {
		file >> config;
	}
	catch (json::parse_error& e) {
		cerr << "Failed to parse JSON: " << e.what() << endl;
		return;
	}
	setVersion(config["version"]);

	bool showConsole = config["showconsole"];
	if (!showConsole) hideNSeek();

	json hk_array = config["hotkeys"];
	vector<Hotkey> hotkeys;

	int id = 1;
	for (const auto& hk_json : hk_array)
	{
		Hotkey hotkey;
		hotkey.id = id;
		hotkey.name = hk_json["name"];
		hotkey.button = hk_json["button"];
		hotkey.key = mapButtonToVirtualKeyCode(hotkey.button);
		hotkey.modifiers = getModifierFlags(config["modifier"]);
		hotkey.type = hk_json["type"];
		string jsonAction = hk_json["action"];

		// commandline parse
		auto cmd = hk_json.find("comandline");
		if (cmd != hk_json.end() && !cmd.value().is_null())
			hotkey.commandLine = cmd.value();
		else
			hotkey.commandLine = "";

		hotkey.action = getActionFromType(hotkey.type, jsonAction, hotkey.commandLine);
		hotkeys.push_back(hotkey);
		id++;
	}
	this->configHotkeys = hotkeys;
}

Config* Config::getInstance()
{
	return instancePtr;
}

void Config::setVersion(string version)
{
	this->version = version;
}

void Config::setVisible(bool visible)
{
	this->visible = visible;
}

bool Config::getVisible()
{
	return this->visible;
}

string Config::getVersion()
{
	return this->version;
}

vector<Hotkey> Config::getHotkeys()
{
	return this->configHotkeys;
}

bool Config::registerHotkey(Hotkey hotkey)
{
	return ::RegisterHotKey(NULL, hotkey.id, hotkey.modifiers, hotkey.key);
}

int Config::mapButtonToVirtualKeyCode(const std::string& button)
{
	if (button.size() == 1 && isalpha(button[0])) {
		// For alphabetic characters, use their uppercase ASCII values as virtual key codes
		char uppercaseChar = toupper(button[0]);
		return uppercaseChar;  // e.g., VK_B for 'b'
	}
	return 0;
}

void Config::print()
{
}

Config* Config::instancePtr = new Config();

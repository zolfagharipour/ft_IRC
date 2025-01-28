#include "bot.hpp"


Bot::Bot()
{
        _toCensor["fuck"] = "fork";
        _toCensor["shit"] = "sheet";
        _toCensor["bitch"] = "beach";
        _toCensor["bastard"] = "basket";
        _toCensor["asshole"] = "ashhole";
        _toCensor["dick"] = "duck";
        _toCensor["cunt"] = "count";
        _toCensor["whore"] = "wore";
        _toCensor["slut"] = "slot";
        _toCensor["prick"] = "brick";
        _toCensor["faggot"] = "bagot";
        _toCensor["nigger"] = "trigger";
        _toCensor["retard"] = "restart";
        _toCensor["pussy"] = "pushy";
        _toCensor["scumbag"] = "scam-bag";
        _toCensor["twat"] = "twit";
        _toCensor["shithead"] = "sheethead";
        _toCensor["motherfucker"] = "mother-lover";
        _toCensor["cock"] = "clock";
        _toCensor["douche"] = "doodle";
        _toCensor["jackass"] = "jacket";
        _toCensor["asshat"] = "ash-hat";
        _toCensor["fuckhead"] = "forkhead";
        _toCensor["cumdumpster"] = "crumb-dumpster";
        _toCensor["cocksucker"] = "clock-lover";
        _toCensor["shitface"] = "sheet-face";
        _toCensor["balllicker"] = "bell-licker";
        _toCensor["buttfucker"] = "butter-forker";
        _toCensor["fucktard"] = "fork-star";
        _toCensor["dipshit"] = "dipstick";
        _toCensor["numbnuts"] = "gum-nuts";
        _toCensor["shitbag"] = "sheet-bag";
        _toCensor["cumslut"] = "crumb-slot";
        _toCensor["dumbfuck"] = "dumb-fork";
        _toCensor["cumface"] = "crumb-face";
        _toCensor["asswipe"] = "ash-wipe";
        _toCensor["fuckwad"] = "fork-wad";
        _toCensor["shitstain"] = "sheet-stain";
        _toCensor["turd"] = "turn";
        _toCensor["fuckboy"] = "fork-boy";
        _toCensor["fuckwit"] = "fork-wit";


		_toKick.insert("tiananmen");
        _toKick.insert("xi jinping");
        _toKick.insert("cpc");
        _toKick.insert("communist party");
        _toKick.insert("chinese government");
        _toKick.insert("hong kong independence");
        _toKick.insert("taiwan independence");
        _toKick.insert("uyghur");
        _toKick.insert("xinjiang");
        _toKick.insert("human rights");
        _toKick.insert("falun gong");
        _toKick.insert("democracy movement");
        _toKick.insert("great firewall");
        _toKick.insert("cultural revolution");
        _toKick.insert("mao zedong");
        _toKick.insert("tibet independence");
        _toKick.insert("li wenliang");
        _toKick.insert("zhongnanhai");
        _toKick.insert("chinese censorship");
        _toKick.insert("beijing government");
        _toKick.insert("freedom of speech");
        _toKick.insert("chinese propaganda");
        _toKick.insert("winnie the pooh");
        _toKick.insert("han supremacy");
        _toKick.insert("forced labor");
        _toKick.insert("belt and road initiative");
        _toKick.insert("south china sea");
        _toKick.insert("nine-dash line");
        _toKick.insert("military expansion");
        _toKick.insert("peoples liberation army");
        _toKick.insert("hong kong protests");
        _toKick.insert("pro-democracy");
        _toKick.insert("press freedom");
}

std::string		Bot::censor( std::vector<std::string> &cmds ){
	std::map<std::string, std::string>::iterator	it;
	std::string										respond = "";
	
	if (_toCensor.find(cmds[2].substr(1)) != _toCensor.end())
		respond += " :" + _toCensor[cmds[2].substr(1)];
	else
		respond += " " + cmds[2];
	
	for (size_t i = 3; i < cmds.size(); i++){
		if (_toCensor.find(cmds[i]) != _toCensor.end())
			respond += " " + _toCensor[cmds[i]];
		else
			respond += " " + cmds[i];
	}
	return respond;
}

bool	Bot::shallKick( std::string& message ){
	std::set<std::string>::iterator	it;

	for (it = _toKick.begin(); it != _toKick.end(); ++it){
		if (message.find(*it) != std::string::npos)
			return true;
	}
	return false;
}

Bot::~Bot( )
{
}

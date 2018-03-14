#include <string>
#include <iostream>
#include <conio.h>
#undef  MOUSE_MOVED
#include <curses.h>
#include <ctime>
#include <cstdlib>
#include <string>
#include "player.h"
#include "monster.h"
#include "system.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
#include <chrono>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <boost/archive/iterators/ostream_iterator.hpp>
#include <utility>
#include <duman.h>
#include <Windows.h>
using boost::asio::ip::tcp;
using namespace boost::archive::iterators;
typedef base64_from_binary<transform_width<const char *,6,8> > base64_text;
using namespace std;

player p1(100, 100, "NULL");
string username, password, mail, time_setting, onlinestatus;
string user_mail, user_name;
int auto_save_time_interval_seconds = 20;
__int64 total = 0;
int total_kills = 0;
int total_max_hp = 0;
bool auto_save_enabled = false; // in settings give ability to set time. don't allow below 15 seconds. self note. do it.
bool first_save = true;
int times_you_died = 0;
bool auto_option = false;
bool auto_option_enabled = true;
char choice = '?';
char ch;

const auto sql_ip = ""; // enter your IP here, example: tcp://dumanstudios.com:3306
const auto sql_username = ""; // enter your SQL username here, example: root
const auto sql_password = ""; // enter your SQL password here, example: 123456

string monster_name_array[50] = {
"Ivana Lessley",
"Federico Meese",
"Delana Hayner",
"Noella Schultheis",
"Raeann Winner",
"Santana Darley",
"Lashay Poorman",
"Ryan Womble",
"Tonya Maddocks",
"Jake Moffet",
"Somer Brazeau",
"Aretha Mullinax",
"Dwayne Avant",
"Olevia Birt",
"Elisabeth Gioia",
"Ivette Jimenes",
"Lacy Fraga",
"Vaughn Collingsworth",
"Rochelle Philhower",
"Dennise Fishel",
"Lesa Boer",
"Esteban Worrall",
"Sherman Teller",
"Stacee Banning",
"Katharyn Descoteaux",
"Elroy Litwin",
"Maura Kollar",
"Virgil Gettys",
"Laurence Frisbee",
"Rosanne Ney",
"Fred Rego",
"Shenita Watchman",
"Sandie Narcisse",
"Kali Fassett",
"Christen Points",
"Kari Lehrman",
"Evie Dancer",
"Zelma Bader",
"Codi Gingras",
"Kayleigh Gains",
"Rosette Renken",
"Deb Elston",
"Gretchen Corchado",
"Emmaline Waldeck",
"Vennie Perrigo",
"Kristy Goosby",
"Charleen Padilla",
"Anastasia Profitt",
"Bob Carls",
"Alona Basye"
};

void online_save();
void forgot_password();
void search_opponent();
void duman_battle(const string&);

int main()
{
	const auto hwnd = GetConsoleWindow();
	const auto hmenu = GetSystemMenu(hwnd, FALSE);
    EnableMenuItem(hmenu, SC_CLOSE, MF_GRAYED);

    /*
    This one checks if the internet connection is there or not to a particular URL.
	If it fails to connect, you can do/say anything inside the fucntion.
	I recommend any_to_exit(true); from duman.h file.

	Don't forget to set your own IP address on the first parameter.
	*/
	if(!InternetCheckConnection("https://dumanstudios.com", FLAG_ICC_FORCE_CONNECTION, 0))
	{
		cout << "Connection to DumanSTUDIOS.com is failed.\nPlease check your internet connection.\n\n";
	}
	
	// MYSQL STUFF BELOW (If you have a running MySQL server and a working IP with open ports, you don't have to change anything.)
	sql::Driver *driver;
	sql::Connection *con;
	sql::Statement *stmt;
	sql::PreparedStatement *pstmt;
	sql::ResultSet *result;
	auto selection = '?';
	auto selection2 = '?';
	string char_selection;
	string execution_command;
	string selection3;
	
	try {
	driver = get_driver_instance();
	con = driver->connect(sql_ip, sql_username, sql_password);

	con->setSchema("duman");
	stmt = con->createStatement();
	stmt->execute("CREATE TABLE IF NOT EXISTS `duman`.`members`(`id` int(11) NOT NULL AUTO_INCREMENT,`username` varchar(30) NOT NULL, `charName` varchar(50) NOT NULL, `hp` varchar(50) NOT NULL,`email` varchar(50) NOT NULL,`password` varchar(128) NOT NULL, `exp` varchar(50) NOT NULL, `level` varchar(50) NOT NULL, `boosts` varchar(50) NOT NULL, `autoHPItem` varchar(50) NOT NULL, `autoSaveTime` varchar(50) NOT NULL, `playTime` varchar(50) NOT NULL, `monstersKilled` varchar(50) NOT NULL, `autoSaveEnabled` varchar(50) NOT NULL, `maxHP` varchar(50) NOT NULL, `youDied` varchar(50) NOT NULL, `passwordToken` varchar(50) NOT NULL, `multiToken` varchar(50) NOT NULL, `onlineStatus` varchar(2) NOT NULL, `autoEnabled` varchar(50) NOT NULL, PRIMARY KEY(`id`), UNIQUE KEY `username` (`username`)) ENGINE = MYISAM DEFAULT CHARSET = utf8; ");
	
	selection5:
	cout << "/* * * * * * * * * * * * * *\\\n";
	cout << "| y: Yes, I am!             |\n";
	cout << "| n: No, I want to register!|\n";
	cout << "| f: forgotten password!    |\n";
	cout << "\\* * * * * * * * * * * * * */\n\n";
	cout << "Are you a registered user? (y/n/f): ";
	cin >> selection3;
	cin.ignore();
	if (selection3 != "y" && selection3 != "n" && selection3 != "f")
	{
		cout << "Invalid selection!\n";
		cout << "Do you want to try again? (y/n): ";
		cin >> selection2;
		if (selection2 == 'y')
		{
			clear();
			refresh();
			cout << "\n";
			char_selection.clear();
			goto selection5;
		}
		any_to_exit(1,true);
	}
	if(selection3 == "f")
	{
		cout << "Username    : ";
		cin >> user_name;
		execution_command = "SELECT username FROM members WHERE username='" + user_name + "'";
		pstmt = con->prepareStatement(execution_command.c_str());
		result = pstmt->executeQuery();
		if (result->next() == 0)
		{
			cout << "\nUser " << user_name << " doesn't exists in the database.\n";
			any_to_exit(300, true);
		}
		cout << "Mail address: ";
		cin >> user_mail;
		execution_command = "SELECT email FROM members WHERE username='"
			+ user_name + "'";
		pstmt = con->prepareStatement(execution_command.c_str());
		result = pstmt->executeQuery();
		while (result->next()) {
			const string databasepulledmail = result->getString("email").c_str();
			cout << "\n";
			if (databasepulledmail != user_mail)
			{
				cout << "You have entered incorrect email for user " + user_name + "!\n";
				any_to_exit(300, true);
			}
		}
		forgot_password();
		any_to_exit(300, true);
	}
	if (selection3 == "y")
	{
		//login here and load stuff from previous instance
		selection2:
		cout << "Username: ";
		cin >> username;
		cin.ignore();
		// checking if username exists or not as the first step
		execution_command = "SELECT username FROM members WHERE username='" + username + "'";
		pstmt = con->prepareStatement(execution_command.c_str());
		result = pstmt->executeQuery();
		if (result->next() == 0)
		{
			cout << "\nUser " << username << " doesn't exists in the database.\n";
			cout << "Do you want to try again? (y/n): ";
			cin >> selection;
			cin.ignore();
			if (selection == 'y')
			{
				clear();
				refresh();
				cout << "\n";
				goto selection2;
			}
			cout << "To quit, press any key...\n";
			_getch();
			exit(1);
		}
		// end of check
		selection3:
		cout << "Password: ";
		//new password masking trial
		password = sinput('*', true);
		//between here
		execution_command = "SELECT password FROM members WHERE username='"
			+ username + "'";
		pstmt = con->prepareStatement(execution_command.c_str());
		result = pstmt->executeQuery();
		while (result->next()) {
			const string databasepulledpw = result->getString("password").c_str();
			cout << "\n";
			if (databasepulledpw != password)
			{
				cout << "You have entered incorrect password for user " + username + "!\n";
				cout << "Do you want to try again? (y/n): ";
				cin >> selection;
				if (selection == 'y')
				{
					clear();
					refresh();
					cout << "\n";
					password.clear();
					goto selection3;
				}
				cout << "To quit, press any key...\n";
				_getch();
				exit(1);
			} // add an else if to check if username exists
			// LOAD ALL DATA TO THE ITEMS HERE
			// SINCE MYSQL IS IN STRING FORM CONVERT IT TO INT BACK
			// STORING IS NO PROBLEM I GOT IT ALREADY
			execution_command = "SELECT charName FROM members WHERE username='" + username + "'";
			pstmt = con->prepareStatement(execution_command.c_str());
			result = pstmt->executeQuery();
			while (result->next()) {
				p1.set_char_name(result->getString("charName").c_str());
			} // charName
			execution_command = "SELECT email FROM members WHERE username='" + username + "'";
			pstmt = con->prepareStatement(execution_command.c_str());
			result = pstmt->executeQuery();
			while (result->next()) {
				user_mail = result->getString("email").c_str();
			} // user mail
			execution_command = "SELECT username FROM members WHERE username='" + username + "'";
			pstmt = con->prepareStatement(execution_command.c_str());
			result = pstmt->executeQuery();
			while (result->next()) {
				user_name = result->getString("username").c_str();
			} // username
			execution_command = "SELECT exp FROM members WHERE username='" + username + "'";
			pstmt = con->prepareStatement(execution_command.c_str());
			result = pstmt->executeQuery();
			while (result->next()) {
				p1.set_exp(result->getString("exp").c_str());
			} // exp
			execution_command = "SELECT boosts FROM members WHERE username='" + username + "'";
			pstmt = con->prepareStatement(execution_command.c_str());
			result = pstmt->executeQuery();
			while (result->next()) {
				p1.set_boosts(result->getString("boosts").c_str());
			} // boosts
			execution_command = "SELECT hp FROM members WHERE username='" + username + "'";
			pstmt = con->prepareStatement(execution_command.c_str());
			result = pstmt->executeQuery();
			while (result->next()) {
				p1.set_hp(result->getString("hp").c_str());
				if (p1.return_hp() <= 0)
				{
					p1.set_hp("1");
					break;
				}
			} // hp
			execution_command = "SELECT autoSaveEnabled FROM members WHERE username='" + username + "'";
			pstmt = con->prepareStatement(execution_command.c_str());
			result = pstmt->executeQuery();
			while (result->next()) {
				auto_save_enabled = stoi(result->getString("autoSaveEnabled").c_str());
			} // autoSaveEnabled
			execution_command = "SELECT autoSaveTime FROM members WHERE username='" + username + "'";
			pstmt = con->prepareStatement(execution_command.c_str());
			result = pstmt->executeQuery();
			while (result->next()) {
				auto_save_time_interval_seconds = stoi(result->getString("autoSaveTime").c_str());
			} // autoSaveTimeInterval
			execution_command = "SELECT playTime FROM members WHERE username='" + username + "'";
			pstmt = con->prepareStatement(execution_command.c_str());
			result = pstmt->executeQuery();
			while (result->next()) {
				total = stoi(result->getString("playTime").c_str());
			} // playTime
			execution_command = "SELECT monstersKilled FROM members WHERE username='" + username + "'";
			pstmt = con->prepareStatement(execution_command.c_str());
			result = pstmt->executeQuery();
			while (result->next()) {
				total_kills = stoi(result->getString("monstersKilled").c_str());
			} // monstersKilled
			execution_command = "SELECT maxHP FROM members WHERE username='" + username + "'";
			pstmt = con->prepareStatement(execution_command.c_str());
			result = pstmt->executeQuery();
			while (result->next()) {
				p1.set_max_hp(result->getString("maxHP").c_str());
			} // maxHP
			execution_command = "SELECT youDied FROM members WHERE username='" + username + "'";
			pstmt = con->prepareStatement(execution_command.c_str());
			result = pstmt->executeQuery();
			while (result->next()) {
				times_you_died = stoi(result->getString("youDied").c_str());
			} // you died
			execution_command = "SELECT autoHPItem FROM members WHERE username='" + username + "'";
			pstmt = con->prepareStatement(execution_command.c_str());
			result = pstmt->executeQuery();
			while (result->next()) {
				p1.set_autohpitem(result->getString("autoHPItem").c_str());
			} // autoHPItem
			execution_command = "SELECT autoEnabled FROM members WHERE username='" + username + "'";
			pstmt = con->prepareStatement(execution_command.c_str());
			result = pstmt->executeQuery();
			while (result->next()) {
				p1.set_auto_enabled(stoi(result->getString("autoEnabled").c_str()));
			} // autoEnabled
			execution_command = "SELECT onlineStatus FROM `members` WHERE `username` = '"+ username +"'";
			pstmt = con->prepareStatement(execution_command.c_str());
			result = pstmt->executeQuery();
			while (result->next()) {
				onlinestatus = result->getString("onlineStatus").c_str();
			} // onlineStatus
			delete stmt;
			delete con;
			delete pstmt;
		}
	}
	if (selection3 == "n")
	{
		selection6:
		cout << "Enter your username: ";
		cin >> username;
		cin.ignore();
		// check if user exists to prevent highjacking
		execution_command = "SELECT username FROM members WHERE username='" + username + "'";
		pstmt = con->prepareStatement(execution_command.c_str());
		result = pstmt->executeQuery();
		if (result->next() != 0) {
			cout << "User " << username << " is already registered! Try another username!\n";
			goto selection6;
		} // autoHPItem
		//
		cout << "Enter your mail address: ";
		cin >> mail;
		cin.ignore();
		cout << "Enter your password: ";
		cin >> password;
		cin.ignore();
		execution_command = "INSERT INTO `members`(`username`, `email`, `password`) VALUES ('"
						 + username + "','" + mail + "','" + password + "')";
		stmt->execute(execution_command.c_str());
		delete pstmt;
		delete con;
		delete stmt;
	}
	}
	catch (sql::SQLException &e) {
		if(e.getErrorCode() == 2003)
		{
			cout << "Secure server connection to DumanSTUDIOS.com is failed!\n";
			key_to_exit(1000, 'q', 0, true);
		}
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
		cout << "Press 'q' to quit.\n";
		key_to_exit(1000, 'q', 0, true);
	}
	// MYSQL STUFF ABOVE
	// MY STUFF BELOW
	p1.set_char_name(username);
	p1.set_monsters_killed(total_kills);
	srand(static_cast<unsigned int>(time(nullptr)));
	auto random_access = rand() % 49 + 1;
	auto exit = false;
	systemclass sys;
	monster m1(100, 230, monster_name_array[random_access]);
	auto key = 'z';
	// Constructs the new thread and runs it. Does not block execution.
	p1.set_death_count(times_you_died);
	if (p1.return_specitem() == 1)
	{
		auto_option = true;
	}
	initscr();
	while (!exit)
	{

		const auto start = chrono::system_clock::now();
		if (_kbhit())
			key = _getch();
		
		switch (key)
		{
		case 27:
			exit = true;
			online_save();
			cout << "\nSynced all the variables and settings with the server!\n";
			cout << "Exiting!\n";
			Sleep(2500);
			break;
		case 'a':
		case 'A':
			random_access = rand() % 49 + 1;
			m1.seed(p1, monster_name_array[random_access]);
		REATTK:
			p1.initiate_attack(p1,m1);
			clear();
			Sleep(800);
			refresh();
			systemclass::battle_information(p1,m1);
			if (p1.return_hp() <= 0)
			{
				exit = true;
				times_you_died++;
				online_save();
				cout << "\nREST IN PEACE [*]\n\nWhile you were in a battle with " << m1.return_name() << ", you died.\n";
				cout << "Press any key to close the program...\n";		
				_getch();
				break;
			}
			if (m1.return_hp() > 0)
				goto REATTK;
			total_kills++;
			p1.set_monsters_killed(total_kills);
			p1.update_player_exp(m1);
			online_save();
			break;
		case 'h':
		case 'H':
			p1.heal(p1.return_h_pregen());
			break;
		case 's':
		case 'S':
			online_save();
			cout << "\nCloud save complete!\n";
			Sleep(750);
			break;
		case 'o':
		case 'O':
			cout << "OPTIONS:\n";
			cout << "1. Enable/Disable auto save";
			cout << " (" << boolalpha << auto_save_enabled << noboolalpha << ")\n";
			cout << "2. Set auto save time interval";
			cout << " (" << auto_save_time_interval_seconds << " seconds)\n";
			cout << "3. Change your password\n";
			cout << "4. Set online status";
			cout << " (";
			if(onlinestatus == "1")
				cout << "online)\n";
			if(onlinestatus == "0")
				cout << "offline)\n";
			if (auto_option)
			{
				cout << "5. Enable/Disable auto hp item";
				cout << " (" << boolalpha << p1.return_auto_enabled() << noboolalpha << ")\n";
			}
			choice = _getch();

			switch (choice)
			{
			case '1':
				if (!auto_save_enabled)
				{
					auto_save_enabled = true;
					cout << "Auto save is now enabled!\n";
					driver = get_driver_instance();
					con = driver->connect(sql_ip, sql_username, sql_password);

					con->setSchema("duman");
					stmt = con->createStatement();
					execution_command = "UPDATE `members` SET `autoSaveEnabled`='" + to_string(auto_save_enabled) + "' WHERE `username`='" + username + "';";
					stmt->execute(execution_command.c_str());
					cout << "\nYour settings are synced online.\n";
					delete con;
					delete stmt;
					Sleep(1500);
					break;
				}
				if (auto_save_enabled)
				{
					auto_save_enabled = false;
					cout << "Auto save is now disabled!\n";
					driver = get_driver_instance();
					con = driver->connect(sql_ip, sql_username, sql_password);

					con->setSchema("duman");
					stmt = con->createStatement();
					execution_command = "UPDATE `members` SET `autoSaveEnabled`='" + to_string(auto_save_enabled) + "' WHERE `username`='" + username + "';";
					stmt->execute(execution_command.c_str());
					cout << "\nYour settings are synced online.\n";
					delete con;
					delete stmt;
					Sleep(1500);
					break;
				}
			case '2':
				selection4:
				cout << "Per XX seconds sync with the servers: ";
				time_setting = sinput('*', false);
				auto_save_time_interval_seconds = stoi(time_setting);
				
				if (auto_save_time_interval_seconds < 15)
				{
					cout << "\n\nTime interval cannot be smaller than 15 seconds!\n";
					cout << "Provide a time interval which is greater than 15 seconds.\n";
					auto_save_time_interval_seconds = 20;
					Sleep(1500);
					time_setting.clear();
					goto selection4;
				}
				driver = get_driver_instance();
				con = driver->connect(sql_ip, sql_username, sql_password);

				con->setSchema("duman");
				stmt = con->createStatement();
				execution_command = "UPDATE `members` SET `autoSaveTime`='" + to_string(auto_save_time_interval_seconds) + "' WHERE `username`='" + username + "';";
				stmt->execute(execution_command.c_str());

				cout << "\n\nTime interval to auto save is now " << auto_save_time_interval_seconds << " seconds!\n";
				cout << "Your settings are synced online.\n";
				delete stmt;
				delete con;
				time_setting.clear();
				Sleep(1500);
				break;
			case '3':
				cout << "\nEnter your new password: ";
				password = sinput('*', false);
				driver = get_driver_instance();
				con = driver->connect(sql_ip, sql_username, sql_password);

				con->setSchema("duman");
				stmt = con->createStatement();
				execution_command = "UPDATE `members` SET `password` = '" + password + "' WHERE `username` = '";
				execution_command.append(username);
				execution_command.append("'");
				stmt->execute(execution_command.c_str());
				cout << "\nYour password is now updated!\n";
				delete stmt;
				delete con;
				Sleep(1500);
				break;
			case '4':
				driver = get_driver_instance();
				con = driver->connect(sql_ip, sql_username, sql_password);

				con->setSchema("duman");
				stmt = con->createStatement();
				execution_command = "SELECT onlineStatus FROM `members` WHERE `username` = '"+ username +"'";
				pstmt = con->prepareStatement(execution_command.c_str());
				result = pstmt->executeQuery();
				while(result->next())
				{
					onlinestatus = result->getString("onlineStatus").c_str();
				}

				if (onlinestatus == "0")
				{
					cout << "You're now online!\n";
					Sleep(1500);
					onlinestatus = "1";
				}
				else if (onlinestatus == "1")
				{
					cout << "You're now offline!\n";
					Sleep(1500);
					onlinestatus = "0";
				}
				
				execution_command = "UPDATE `members` SET `onlineStatus` = '" + onlinestatus + "' WHERE `username`='" += username + "'";
				stmt->execute(execution_command.c_str());
				break;
			case '5':
				{
					if (p1.return_specitem() == 0)
					{
						cout << "\nYou don't have access to this menu yet!\n";
						Sleep(1500);
						break;
					}
					if (p1.return_auto_enabled())
					{
						p1.set_auto_enabled(false);
						cout << "Auto healing item is disabled!\n";
						online_save();
						Sleep(1500);
						break;
					}
					if (!p1.return_auto_enabled())
					{
						p1.set_auto_enabled(true);
						cout << "Auto healing item is enabled!\n";
						online_save();
						Sleep(1500);
						break;
					}
				}
			case 27:
			case 'o':
			case 'O':
				break;
			default:
				cout << "Invalid choice.\n";
				Sleep(1500);
			}
			break;
		case 'b':
		case 'B':
			p1.heal_boost();
			break;
		case 't':
		case 'T':
			cout << "This is a debug text.\n";
			Sleep(1000); // change this to make the text remain longer for att later
			break;
		case 'm':
		case 'M':
			cout << "Welcome to Duman-Battle matchmaking!\n\n";
			cout << "Searching for an opponent...";
			Sleep(1000);
			search_opponent();
		default:
			break;
		} key = 'z';

		clear();
		Sleep(95);
		refresh();
		p1.should_level_up(p1);
		p1.information();
		if (p1.return_specitem() == 1 && p1.return_auto_enabled())
		{
			p1.heal(p1.return_h_pregen());
		}
		const auto end = chrono::system_clock::now();
		auto elapsed = std::chrono::duration_cast<chrono::milliseconds>(end - start);
		total += elapsed.count();
		if (total > (auto_save_time_interval_seconds * 1000) &&
			total % (auto_save_time_interval_seconds * 1000) < 100 &&
			total % (auto_save_time_interval_seconds * 1000) > 1 &&
			auto_save_enabled)
		{
			online_save();
		}
		if (first_save)
		{
			online_save();
			first_save = false;
		}
	}
	endwin();
}

void online_save()
{
	auto driver = get_driver_instance();
	auto con = driver->connect(sql_ip, sql_username, sql_password);

	con->setSchema("duman");
	auto stmt = con->createStatement();
	auto execution_command = "UPDATE `members` SET `charName`='" + p1.return_name() + "' WHERE `username`='" + username + "';";
	stmt->execute(execution_command.c_str());
	execution_command = "UPDATE `members` SET `hp`='" + to_string(p1.return_hp()) + "' WHERE `username`='" + username + "';";
	stmt->execute(execution_command.c_str());
	execution_command = "UPDATE `members` SET `exp`='" + to_string(p1.return_exp()) + "' WHERE `username`='" + username + "';";
	stmt->execute(execution_command.c_str());
	execution_command = "UPDATE `members` SET `level`='" + to_string(p1.return_level()) + "' WHERE `username`='" + username + "';";
	stmt->execute(execution_command.c_str());
	execution_command = "UPDATE `members` SET `boosts`='" + to_string(p1.return_boosts()) + "' WHERE `username`='" + username + "';";
	stmt->execute(execution_command.c_str());
	execution_command = "UPDATE `members` SET `autoHPItem`='" + to_string(p1.return_specitem()) + "' WHERE `username`='" + username + "';";
	stmt->execute(execution_command.c_str());
	execution_command = "UPDATE `members` SET `playTime`='" + to_string(total) + "' WHERE `username`='" + username + "';";
	stmt->execute(execution_command.c_str());
	execution_command = "UPDATE `members` SET `autoSaveEnabled`='" + to_string(auto_save_enabled) + "' WHERE `username`='" + username + "';";
	stmt->execute(execution_command.c_str());
	execution_command = "UPDATE `members` SET `autoSaveTime`='" + to_string(auto_save_time_interval_seconds) + "' WHERE `username`='" + username + "';";
	stmt->execute(execution_command.c_str());
	execution_command = "UPDATE `members` SET `monstersKilled`='" + to_string(total_kills) + "' WHERE `username`='" + username + "';";
	stmt->execute(execution_command.c_str());
	execution_command = "UPDATE `members` SET `maxHP`='" + to_string(p1.return_max_hp()) + "' WHERE `username`='" + username + "';";
	stmt->execute(execution_command.c_str());
	execution_command = "UPDATE `members` SET `youDied`='" + to_string(times_you_died) + "' WHERE `username`='" + username + "';";
	stmt->execute(execution_command.c_str());
	execution_command = "UPDATE `members` SET `autoHPItem`='" + to_string(p1.return_specitem()) + "' WHERE `username`='" + username + "';";
	stmt->execute(execution_command.c_str());
	execution_command = "UPDATE `members` SET `autoEnabled`='" + to_string(p1.return_auto_enabled()) + "' WHERE `username`='" + username + "';";
	stmt->execute(execution_command.c_str());
	delete stmt;
	delete con;
}

///////////////////// MAIL STUFF

class smtp_client
{
public:
	smtp_client(std::string p_server, const unsigned int p_port,std::string p_user,std::string p_password):
	m_server_(std::move(p_server)),m_user_name_(std::move(p_user)),m_password_(std::move(p_password)),m_port_(p_port),m_resolver_(m_io_service_),m_socket_(m_io_service_)
	{
		const tcp::resolver::query qry(m_server_,std::to_string(m_port_));
		m_resolver_.async_resolve(qry,boost::bind(&smtp_client::handle_resolve,this,boost::asio::placeholders::error,
		boost::asio::placeholders::iterator));
	}
	bool send(const std::string& p_from, const std::string& p_to, const std::string& p_subject, const std::string& p_message)
	{
		m_from_=p_from;
		m_to_=p_to;
		m_subject_=p_subject;
		m_message_=p_message;
		m_io_service_.run();
		return m_has_error_;
	}
private:
	std::string encode_base64(const std::string& p_data) const
	{
		std::stringstream os;
		const auto sz = p_data.size();
		std::copy(base64_text(p_data.c_str()),base64_text(p_data.c_str()+sz),std::ostream_iterator<char>(os));
		return os.str();
	}
	void handle_resolve(const boost::system::error_code& err,tcp::resolver::iterator endpoint_iterator)
	{
		if(!err)
		{
			const tcp::endpoint endpoint=*endpoint_iterator;
			m_socket_.async_connect(endpoint,
			boost::bind(&smtp_client::handle_connect,this,boost::asio::placeholders::error,++endpoint_iterator));
		}
		else
		{
			m_has_error_=true;
			m_error_msg_= err.message();
		}
	}
	void write_line(const std::string& p_data)
	{
		std::ostream req_strm(&m_request_);
		req_strm << p_data << "\r\n";
		write(m_socket_,m_request_);
		req_strm.clear();
	}
	void handle_connect(const boost::system::error_code& error, const tcp::resolver::iterator&)
	{
		if (!error)
		{
			// The connection was successful. Send the request.
			std::ostream req_strm(&m_request_);
			write_line("EHLO "+m_server_);
			write_line("AUTH LOGIN");
			write_line(encode_base64(m_user_name_));
			write_line(encode_base64(m_password_));
			write_line("MAIL FROM:<"+m_from_+">");
			write_line("RCPT TO:<"+m_to_+">");
			write_line("DATA");
			write_line("SUBJECT:"+m_subject_);
			write_line("From:"+m_from_);
			write_line("To:"+m_to_);
			write_line("");
			write_line(m_message_);
			write_line(".\r\n");
		}
		else
		{
			m_has_error_=true;
			m_error_msg_= error.message();
		}
	}
	std::string m_server_;
	std::string m_user_name_;
	std::string m_password_;
	std::string m_from_;
	std::string m_to_;
	std::string m_subject_;
	std::string m_message_;
	unsigned int m_port_;
	boost::asio::io_service m_io_service_;
	tcp::resolver m_resolver_;
	tcp::socket m_socket_;
	boost::asio::streambuf m_request_;
	boost::asio::streambuf m_response_;
	bool m_has_error_{};
	std::string m_error_msg_;
};

/*
If you don't have any SMTP subscription or service you can either register for one on:
- sendgrid
- mailgun
- Amazon SES
- mailchimp

If you still don't want one, go to settings menu and disable forgot_password() feature,
don't forget to comment out function below as well. This way, you will not have a mail service
but the program will just run.
*/
string smtp_url = ""; // enter your own smtp address, example: smtp.sendgrid.net
const unsigned smtp_port = 587; // enter the required port for your smtp connection, example: 25, 465 or 587
string smtp_username = ""; // Enter your smtp username, example: smtpuser
string smtp_password = ""; // Enter your smtp password, example: 2g52x{DJQ4V\u2n2

void forgot_password()
{
	const auto new_token = password_generator(5,false,false);

	/* MAIL STUFF BELOW */
	smtp_client mailc(smtp_url, smtp_port, smtp_username, smtp_password);
	mailc.send("noreply@dumanstudios.com",user_mail,"Password Recovery","Hey, " + user_name +
		"!\n\nSomeone (hopefully you!) has submitted a forgotten password request for your account on DumanSTUDIOS projectX\n\n"
		+ "Here's the token to update your password:\n" + new_token + "\n\n If you didn't do this request just ignore or contact us at https://support.dumanstudios.com \n\n");
	/* SQL UPDATE BELOW */
	auto driver = get_driver_instance();
	auto con = driver->connect(sql_ip, sql_username, sql_password);

	con->setSchema("duman");
	auto stmt = con->createStatement();
	auto execution_command = "UPDATE `members` SET `passwordToken` = '" + new_token + "' WHERE `username` = '" + user_name + "'";
	stmt->execute(execution_command.c_str());

	cout << "An e-mail that's containing password reset token has been sent to the address " << user_mail << "!\n";
	pass_reset:
	cout << "Enter token to reset your password: ";
	const auto token_input = sinput('*', false);
	if(token_input == new_token)
	{
		doubleconfirm:
		cout << "Enter your new password: ";
		const auto new_password = sinput('*', false);
		cout << "Enter your new password again: ";
		const auto repeat_pass  = sinput('*', false);
		if(new_password == repeat_pass)
		{
			execution_command = "UPDATE `members` SET `password`='" + new_password + "' WHERE `username`='" + user_name + "';";
			stmt->execute(execution_command.c_str());
		}
		else
		{
			clear();
			cout << "Passwords do not match! Try again!\n";
			goto doubleconfirm;
		}
	}
	else
	{
		cout << "Whops, that token doesn't look like it's valid!\nWould you like to try again? (y/n): ";
		auto selection_inp = sinput('*', false);
		if(choice == 'y')
		{
			goto pass_reset;
		}
	}
	execution_command = "UPDATE `members` SET `passwordToken` = '""' WHERE `username` = '" + user_name + "'";
	stmt->execute(execution_command.c_str());

	cout << "Your password has been successfuly updated and previous reset token has been deauthorized!\n";
	delete stmt;
	delete con;
}

// MULTI STUFF

void search_opponent()
{
	vector<string> onusers;
	const string online = "online";

	auto driver = get_driver_instance();
	auto con = driver->connect(sql_ip, sql_username, sql_password);

	con->setSchema("duman");
	auto stmt = con->createStatement();
	auto execution_command = "UPDATE `members` SET `onlineStatus` = '1' WHERE `username`='" + username + "'";
	stmt->execute(execution_command.c_str());

	execution_command = "SELECT `username` FROM `members` WHERE `onlineStatus` = '1'";
	auto pstmt = con->prepareStatement(execution_command.c_str());
	auto result = pstmt->executeQuery();
	while(result->next())
	{
		string db_username = result->getString("username").c_str();
		if(db_username == username)
			continue;
		onusers.emplace_back(db_username);
	}

	if(onusers.empty())
	{
		cout << "\n\nCurrently no other user is available! :(\n";
		cout << "Please try again later.\n";
		Sleep(3000);
	}
	else
	{
		auto trial_amount = 0;
		try_again:
			// ReSharper disable once CppLocalVariableMayBeConst
		auto selection = rand() % onusers.size(); // this can't be const otherwise there's probability of fighting yourself
		if(onusers[selection] == username)
		{
			trial_amount++;
			if(trial_amount > 10)
			{
				cout << "Currently no other user is available! :(\n";
				goto end_of_search;
			}
			goto try_again;
		}
		duman_battle(onusers[selection]);
	}
	cout << "\n";
	end_of_search:
	delete result;
	delete pstmt;
	delete stmt;
	delete con;
}

void duman_battle(const string& remoteuser)
{
	clear();
	string db_remoteuserhp, db_remoteuserlevel;
	auto driver = get_driver_instance();
	auto con = driver->connect(sql_ip, sql_username, sql_password);

	con->setSchema("duman");

	const auto stmt = con->createStatement();
	auto execution_command = "SELECT hp, level FROM `members` WHERE `username` = '"+ remoteuser +"'";
	auto pstmt = con->prepareStatement(execution_command.c_str());
	auto result = pstmt->executeQuery();
	while(result->next())
	{
		db_remoteuserhp = result->getString("hp").c_str();
		db_remoteuserlevel = result->getString("level").c_str();
	}

	monster remote_user(stoi(db_remoteuserhp), stoi(db_remoteuserlevel), remoteuser);
	remote_user.pvpseed(p1,remoteuser,stoi(db_remoteuserhp),stoi(db_remoteuserlevel));
	REATTK:
	p1.initiate_attack(p1,remote_user);
	clear();
	Sleep(800);
	refresh();
	systemclass::battle_information(p1,remote_user);
	if (p1.return_hp() <= 0)
	{
		//times_you_died++; change this to multiplayer times death
		online_save();
		cout << "\nREST IN PEACE [*]\n\nWhile you were in a battle with " << remote_user.return_name() << ", you died.\n";	
		any_to_exit(0, true);
	}
	if (remote_user.return_hp() > 0)
		goto REATTK;
	total_kills++;
	p1.set_monsters_killed(total_kills);
	p1.update_player_exp(remote_user);
	online_save();

	delete result;
	delete stmt;
	delete pstmt;
	delete con;
}

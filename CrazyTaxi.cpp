#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <functional>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <cassert>
using namespace std;
//Game Developer: Arshak Parsa
//game settings
const unsigned int XRES = 8;
const unsigned int YRES = 12;
//const unsigned int USERNAME_SIZE = 10;
const unsigned int BEGINNING_TAXI = 10;
const unsigned int MAX_TAXI = 12;
const unsigned int MIN_TAXI = 0;
const int DEFUALT_COLOR = 37;

//some useful functions:
string loc_to_str(int x, int y)
{
	return "(" + to_string(x) + "," + to_string(y) + ")";
}
//this is a taxicab distance
int distance(int x1, int y1, int x2, int y2)
{
	return abs(y2-y1)+abs(x2-x1);
}



//define classes
class User
{
	string _username;
	string _phone;
	int _x, _y;
	bool _isLocationGiven = false;
	bool isValid(int x, int y) const
	{
		return (0 <= x && x < XRES && 0 <= y && y < YRES);
	}
	void checkValid(int x, int y) const
	{
		if (!isValid(x, y))
		{
			throw loc_to_str(x, y) + " is not a valid location!";
		}
	}
  public:
	bool isLocationGiven() const { return _isLocationGiven; };
	
	User(string username, string phone) : _username(username), _phone(phone)
	{
	}
	User(string username, string phone, int x, int y) : _username(username),_phone(phone), _x(x), _y(y)
	{
		checkValid(x, y);
		_isLocationGiven = true;
	}
	User(const User &u)
	{
		_phone = u.getPhone();
		_username = u.getUserName();
		_isLocationGiven = u.isLocationGiven();
		if (_isLocationGiven)
		{
			_x = u.getX();
			_y = u.getY();
		}
	}
	int getX() const
	{
		assert(isLocationGiven());
		return _x;
	}
	int getY() const
	{
		assert(isLocationGiven());
		return _y;
	}
	string getLoc() const
	{
		if (isLocationGiven())
			return loc_to_str(getX(), getY());
		return "Unknown";
	}
	void setLoc(int x, int y)
	{
		checkValid(x, y);
		_x = x;
		_y = y;
		_isLocationGiven = true;
	}

	string getUserName() const
	{
		return _username;
	}
	string getPhone() const
	{
		return _phone;
	}
	bool operator==(const User &u)
	{
		return u.getUserName() == getUserName() || u.getPhone() == getPhone();
	}
	User &operator=(const User &u)
	{
		_phone = u.getPhone();
		_username = u.getUserName();
		_isLocationGiven = u.isLocationGiven();
		if (_isLocationGiven)
		{
			_x = u.getX();
			_y = u.getY();
		}
		return *this;
	}
	friend ostream &operator<<(ostream &out, const User &u);
};

ostream &operator<<(ostream &out, const User &u)
{
	out << "UserName: " << u.getUserName() << endl;
	out << "Phone Number: " << u.getPhone() << endl;
	out << "Location: " << u.getLoc() << endl;
	return out;
}

class Taxi
{
	User *_driver;
	int _income;
	vector<int> _history; //id of trips
	string _cartype;
	int _marker = DEFUALT_COLOR;
	int _color = 33;

  public:
	Taxi(string cartype, User u) : _cartype(cartype)
	{
		_driver = new User(u);
		_income = 0;
	}
	Taxi(const Taxi &t)
	{
		//cout << "cpy taxi";
		_driver = new User(t.getUser());
		_cartype = t.getCarType();
		_income = t.getIncome();
		_color = t.getColor();
		_marker = t.getMarker();
		_history = t.getHistory();
	}

	~Taxi()
	{
		//cout << "des " << _driver->getUserName();
		//cout << endl <<_cartype;
		if (_driver != 0)
			delete _driver;
	}
	int getMarker() const
	{
		return _marker;
	}
	int getColor() const
	{
		return _color;
	}
	string getUserName() const
	{
		return _driver->getUserName();
	}
	string getPhone() const
	{
		return _driver->getPhone();
	}
	int getX() const
	{
		return _driver->getX();
	}
	int getY() const
	{
		return _driver->getY();
	}
	User getUser() const
	{
		return *_driver;
	}
	string getCarType() const
	{
		return _cartype;
	}
	int getIncome() const
	{
		return _income;
	}
	vector<int> getHistory()const{
		return _history;
	}
	
	void addHistory(int i){
		_history.push_back(i);
	}
	void addIncome(int inc){
		_income+=inc;
	}
	void setMarker(int n)
	{
		_marker = n;
	}
	void setColor(int n)
	{
		_color = n;
	}
	void setLoc(int x, int y)
	{
		_driver->setLoc(x, y);
	}
	bool operator==(const Taxi &t)
	{
		return t.getUserName() == getUserName() || t.getPhone() == getPhone();
	}
	//You need to implement this for vector.erase :(
	Taxi &operator=(const Taxi &t)
	{
		if (_driver != 0)
			delete _driver;
		_driver = new User(t.getUser());
		_cartype = t.getCarType();
		_income = t.getIncome();
		_color = t.getColor();
		_marker = t.getMarker();
		_history = t.getHistory();
		return *this;
	}
	friend ostream &operator<<(ostream &out, const Taxi &t);
};

ostream &operator<<(ostream &out, const Taxi &t)
{
	out << t.getUser();
	out << "CarType: " << t.getCarType() << endl;
	out << "Income: " << t.getIncome() << endl;
	return out;
}
/*
class Trip{
	int _price;
	string _username;
	string _driver;
	string _cartype;
	public:
		Trip(int p,string un,string dr,string ct):_price(p),_username(un),_driver(dr),_cartype(ct){
			
		}
		friend ostream &operator<<(ostream &out, const Trip &t);
};
ostream &operator<<(ostream &out, const Trip &t){
	out<<"";
	return out;
}*/
class Ground
{
	vector<User> _users;
	vector<Taxi> _taxis;
	int _marker[XRES][YRES];
	vector<string> _trip_history;
	map<string, int> _price_table = {
		{"bmw", 100},
		{"405", 20},
		{"pride", 10},
		{"peykan", 5},
	};
	int findTaxi(string un) const
	{
		for (int i = 0; i < _taxis.size(); i++)
			if (_taxis[i].getUserName() == un)
				return i;
		throw un + " does not exist!";
	}

  public:
	bool isValid(int x, int y) const
	{
		return (0 <= x && x < XRES && 0 <= y && y < YRES);
	}
	void checkValid(int x, int y) const
	{
		if (!isValid(x, y))
		{
			throw loc_to_str(x, y) + " is not a valid location!";
		}
	}
	bool show_users=false;
	void ResetMarker()
	{
		for (int i = 0; i < XRES; i++)
			for (int j = 0; j < YRES; j++)
				_marker[i][j] = DEFUALT_COLOR; //white
	}
	Ground()
	{
		ResetMarker();
	}
	//getter
	int findUser(string un) const
	{
		for (int i = 0; i < _users.size(); i++)
			if (_users[i].getUserName() == un)
				return i;
		throw un + " does not exist!";
	}
	bool isCarTypeValid(string ct)
	{
		for (const auto &x : _price_table)
			if (ct == x.first)
				return true;
		return false;
	}
	void checkCarTypeValid(string ct)
	{
		if (!isCarTypeValid(ct))
			throw "CarType is invalid!";
	}

	bool isEmpty(int x, int y) const
	{
		for (Taxi t : _taxis)
			if (t.getX() == x && t.getY() == y)
				return false;
		return true;
	}
	string getTrip(int index) const
	{
		return _trip_history[index];
	}
	vector<User> getUsers() const
	{
		return _users;
	}
	vector<Taxi> getTaxis() const
	{
		return _taxis;
	}
	Taxi getTaxi(int index) const
	{
		return _taxis[index];
	}
	Taxi getTaxi(string un) const
	{
		return getTaxi(findTaxi(un));
	}
	User getUser(string un) const
	{
		return _users[findUser(un)];
	}
	User getUser(int index) const
	{
		return _users[index];
	}
	int getPrice(string ct){
		return _price_table[ct];
	}
	map<string, int> getPriceTable() const
	{
		return _price_table;
	}
	vector<int> findNearTaxis(int x,int y,double dist){
		vector<int> indexes;
		for (int i = 0; i < _taxis.size(); i++)
			if (distance(x,y,_taxis[i].getX(),_taxis[i].getY())<=dist)	
				indexes.push_back(i);
		return indexes;			
	}

	//setters
	void addIncome(string un,int inc){
		int i=findTaxi(un);
		_taxis[i].addIncome(inc);
	}
	void setPrice(string ct, int price)
	{
		_price_table[ct] = price;
	}
	void setMarker(string un, int m)
	{
		int i = findTaxi(un);
		_taxis[i].setColor(m);
		_taxis[i].setMarker(m);
	}
	void changeTaxiLoc(int index, int x, int y)
	{
		if (!isEmpty(x, y))
			throw loc_to_str(x, y) + "is not empty";
		_taxis[index].setLoc(x, y);
		if (_taxis[index].getMarker() != DEFUALT_COLOR)
			_marker[x][y] = _taxis[index].getMarker();
	}
	void changeUserLoc(int index, int x, int y)
	{
		_users[index].setLoc(x, y);
	}
	void addUser(User u)
	{
		for (User user : _users)
			if (user == u)
				throw string("This user has already been registered");
		User user(u);
		_users.push_back(user);
	}
	void addTaxi(Taxi &t)
	{
		if (_taxis.size() >= MAX_TAXI)
			throw "You can't add more than " + to_string(MAX_TAXI) + " taxis";

		if (!isEmpty(t.getX(), t.getY()))
			throw t.getUser().getLoc() + "is not empty";
		for (Taxi taxi : _taxis)
			if (taxi == t)
				throw string("This taxi has already been registered");
		checkCarTypeValid(t.getCarType());

		_taxis.push_back(t); //this calls copy comstructor!
							 //https://stackoverflow.com/questions/2447392/does-stdvector-change-its-address-how-to-avoid
							 //You want to avoid taking the address of elements in a vector
	}
	void removeUser(string un)
	{
		for (int i = 0; i < _users.size(); i++)
			if ((_users.begin() + i)->getUserName() == un)
			{
				_users.erase(_users.begin() + i);
				return;
			}
		throw un + " does not exist!";
	}
	void removeTaxi(string un)
	{
		for (int i = 0; i < _taxis.size(); i++)
			if ((_taxis.begin() + i)->getUserName() == un)
			{
				_taxis.erase(_taxis.begin() + i);
				return;
			}
		throw un + " does not exist!";
	}
	void addTrip(string str,int taxi_index){
		_taxis[taxi_index].addHistory(_trip_history.size());
		_trip_history.push_back(str);
	}
	friend ostream &operator<<(ostream &out, const Ground &g);
};
ostream &operator<<(ostream &out, const Ground &g)
{
	string gmap[XRES][YRES];
	for (int i = 0; i < XRES; i++)
		for (int j = 0; j < YRES; j++)
			gmap[i][j] = "\033[1;" + to_string(g._marker[i][j]) + "m**\033[0m ";
	
	if(g.show_users)
		for (User u : g.getUsers())
			if (u.isLocationGiven())
				gmap[u.getX()][u.getY()] = string("\033[1;34m") + (u.getUserName()+string(" ")).substr(0,2) + "\033[0m ";

	
	for (Taxi t : g.getTaxis())
		gmap[t.getX()][t.getY()] = "\033[1;" + to_string(t.getColor()) + "m" + (t.getUserName()+string(" ")).substr(0,2) + "\033[0m ";

	for (int j = 0; j < YRES; j++)
	{
		for (int i = 0; i < XRES; i++)
			out << gmap[i][j];
		out << endl;
	}
	return out;
}
/*
ostream &operator<<(ostream &out, const Ground &g)
{
	string gmap[XRES][YRES];
	for (int i = 0; i < XRES; i++)
		for (int j = 0; j < YRES; j++)
			gmap[i][j] = "\033[1;" + to_string(g._marker[i][j]) + "m*\033[0m";
	
	if(g.show_users)
		for (User u : g.getUsers())
			if (u.isLocationGiven())
				gmap[u.getX()][u.getY()] = string("\033[1;34m") + u.getUserName()[0] + "\033[0m";

	
	for (Taxi t : g.getTaxis())
		gmap[t.getX()][t.getY()] = "\033[1;" + to_string(t.getColor()) + "m" + t.getUserName()[0] + "\033[0m";

	for (int j = 0; j < YRES; j++)
	{
		for (int i = 0; i < XRES; i++)
			out << gmap[i][j];
		out << endl;
	}
	return out;
}
*/
class Key
{
	string _id;
	function<void()> _func;
	bool _inputType;

  public:
	Key(string id, const function<void()> &func, bool inputType = true) : _func(func), _inputType(inputType)
	{
		_id = id.c_str();
	}
	string getId()
	{
		return _id;
	}
	void callFunc()
	{
		try
		{
			_func();
		}
		catch (const char *error)
		{
			cout << error << endl;
		}
		catch (const string error)
		{
			cout << error << endl;
		}
		if (_inputType)
			cin.ignore();
	}
};

User cinUser(bool is_loc_optional = true)
{
	string un;
	string pn;
	cout << "Enter UserName: ";
	cin >> un;
	cout << "Enter Phone Number: ";
	cin >> pn;
	if (!(pn.rfind("09", 0) == 0))
		throw "invalid phone number!";
	User u(un, pn);
	if (is_loc_optional)
	{
		string response;
		cout << "Do you want to add location?(y/n) ";
		cin >> response;
		if (response == "y")
		{
			int x, y;
			cout << "Enter X: ";
			cin >> x;
			cout << "Enter Y: ";
			cin >> y;
			u.setLoc(x, y);
		}
	}
	else
	{
		int x, y;
		cout << "Enter X: ";
		cin >> x;
		cout << "Enter Y: ";
		cin >> y;
		u.setLoc(x, y);
	}
	return u;
}

Taxi cinTaxi()
{
	User u = cinUser(false);
	string cn;
	cout << "Enter cartype: ";
	cin >> cn;
	return Taxi(cn, u);
}

Ground g;
map<string, Key> controls;
//Key Functions
void Print()
{
	cout << g;
}
void Move()
{
	for (int i = 0; i < g.getTaxis().size(); i++)
	{
		try
		{
			g.changeTaxiLoc(i, g.getTaxi(i).getX() + (rand() % 3 - 1), g.getTaxi(i).getY() + (rand() % 3 - 1));
		}
		catch (string e)
		{
		}
	}
	cout << g;
}
void RequestTaxi()
{
	string un, response;
	cout << "Enter UserName: ";
	cin >> un;
	int uindex=g.findUser(un);
	if (!g.getUser(uindex).isLocationGiven())
	{
		cout << "You don't have a location! Please add your location:" << endl;
		int x, y;
		cout << "Enter X: ";
		cin >> x;
		cout << "Enter Y: ";
		cin >> y;
		g.changeUserLoc(uindex,x, y);
	}
	cout << "Is " << g.getUser(uindex).getLoc() << " your current location?(y/n) ";
	cin >> response;
	if (response != "y")
	{
		int x, y;
		cout << "Enter X: ";
		cin >> x;
		cout << "Enter Y: ";
		cin >> y;
		g.changeUserLoc(uindex,x, y);
	}
	
	cout << "Enter your destination: ";
	int x, y;
	cout << "Enter X: ";
	cin >> x;
	cout << "Enter Y: ";
	cin >> y;
	g.checkValid(x,y);
	if (!g.isEmpty(x,y))
		throw "Destination is not empty!";
	float trip_distance = distance(x,y,g.getUser(uindex).getX(),g.getUser(uindex).getY());
	
	vector<int> indexes = g.findNearTaxis(g.getUser(uindex).getX(),g.getUser(uindex).getY(),3);
	if (indexes.size()==0)
		throw "There are no taxis nearby!";
	cout << endl<<"Nearby Taxis:" << endl;
	for(int i : indexes){
		cout <<endl<< g.getTaxi(i);
		cout << "Price: " << (int)(trip_distance*g.getPrice(g.getTaxi(i).getCarType()))<<endl;						
	}
	cout << endl << "Which car do you take? "<<endl;
	cout << "Enter taxi username: ";
	cin >> un;
	for(int i : indexes)
		if (g.getTaxi(i).getUserName()==un){
			g.changeTaxiLoc(i,x,y);
			int price =(int)(trip_distance*g.getPrice(g.getTaxi(i).getCarType()));
			g.addIncome(un,price);
			g.changeUserLoc(uindex,x,y);
			
			string str ="Passenger: "+ g.getUser(uindex).getUserName()
			+ "\nPrice: " + to_string(price)
			+ "\n";
			g.addTrip(str,i);
			return ;
		}
	
	throw "invalid taxi";
}
void AddUser()
{
	g.addUser(cinUser());
}
void RemoveUser()
{
	string un;
	cout << "Enter UserName: ";
	cin >> un;
	g.removeUser(un);
}
void AddTaxi()
{
	Taxi t = cinTaxi();
	g.addTaxi(t);
}
void RemoveTaxi()
{
	string un;
	cout << "Enter Taxi UserName: ";
	cin >> un;
	g.removeTaxi(un);
}
void CheckDetails()
{
	string un;
	cout << "Enter Taxi UserName: ";
	cin >> un;
	Taxi t = g.getTaxi(un);
	cout << t;
	cout << "Trip History:"<<endl;
	for (int id : t.getHistory())
		cout << g.getTrip(id) <<endl;
}
void ChangePriceTable()
{
	//print price table
	for (auto const &[cartype, price] : g.getPriceTable())
	{
		cout << "CarType: " << cartype << " Price: " << price << endl;
	}
	string ct;
	int p;
	cout << "Enter CarType: ";
	cin >> ct;
	cout << "Enter Price: ";
	cin >> p;
	g.setPrice(ct, p);
}
void PrintUsers()
{
	for (User u : g.getUsers())
		cout << u << endl;
}
void PrintTaxis()
{
	for (Taxi t : g.getTaxis())
		cout << t << endl;
}
void Quit()
{
	abort();
}
void SetTaxiMarker()
{
	string un;
	int m;
	cout << "Enter Taxi UserName: ";
	cin >> un;
	cout << "Enter Marker code: ";
	cin >> m;
	g.setMarker(un, m);
}
void Help()
{
	map<string, Key>::iterator it = controls.begin();
	while (it != controls.end())
	{
		if (it->second.getId().length() == 1)
			cout << "Enter \"" << it->second.getId() << "\" to " << it->first << endl;
		++it;
	} //thanks geeksforgeeks!
}
void AllHelp()
{
	map<string, Key>::iterator it = controls.begin();
	while (it != controls.end())
	{
		if (it->second.getId() != "hesoyam")
			cout << "Enter \"" << it->second.getId() << "\" to " << it->first << endl;
		++it;
	} //thanks geeksforgeeks!
}
void HESOYAM(){//easter egg
	string un;
	cout << "Enter Taxi UserName: ";
	cin >> un;
	g.addIncome(un,250000);
}
void ShowUsers(){
	g.show_users=true;
}
void HideUsers(){
	g.show_users=false;
}
void CleanMark(){
	g.ResetMarker();
}
int main()
{
	srand(time(0));
	controls = {
		{"Move", Key("C", Move, false)},
		{"Hidden Move", Key("", Move, false)},
		{"Request Taxi", Key("T", RequestTaxi)},
		{"Add User", Key("U", AddUser)},
		{"Remove User", Key("R", RemoveUser)},
		{"Add Taxi", Key("A", AddTaxi)},
		{"Remove Taxi", Key("D", RemoveTaxi)},
		{"Check Details", Key("M", CheckDetails)},
		{"Change Price Table", Key("P", ChangePriceTable)},
		{"Show Help", Key("H", Help,false)},
		
		{"Show All Help", Key("H -a", AllHelp,false)},
		{"Quit", Key("Q", Quit, false)},
		{"Print Users", Key("users", PrintUsers, false)},
		{"Print Taxis", Key("taxis", PrintTaxis, false)},
		{"Print Ground", Key("pr", Print, false)},
		{"Set Taxi Marker", Key("mark", SetTaxiMarker)},
		{"Show Users", Key("show users", ShowUsers, false)},
		{"Hide Users", Key("hide users", HideUsers, false)},
		{"HESOYAM", Key("hesoyam", HESOYAM)},
		{"Clean Markers", Key("clean mark", CleanMark,false)},
		
	};

	//print guidelines :
	Help();
	//add defualt Users
	g.addUser(User("arshak", "0939xxxxxxx"));
	g.addUser(User("defualt", "0919xxxxxxx"));
	g.addUser(User("00", "0900xxxxxxx",0,0));
	
	//add taxis
	for (int i = 10; i < BEGINNING_TAXI+10; i++)
	{
		try
		{
			User u(to_string(i), "0912345678" + to_string(i), rand() % XRES, rand() % YRES);
			Taxi t("pride", u);
			g.addTaxi(t);
		}
		catch (const char *c)
		{
			cout << c << endl;
		}
		catch (string s)
		{
			cout << s << endl;
		}
	}
	//get input
	string input;

	cout << g;
	while (1)
	{
		getline(cin, input);
		map<string, Key>::iterator it = controls.begin();
		bool isKeyValid = false;
		while (it != controls.end())
		{
			if (input == it->second.getId())
			{
				it->second.callFunc();
				isKeyValid = true;
			}
			++it;
		}
		if (!isKeyValid)
			cout << input << " is not valid!" << endl;
	}
}
#include <bits/stdc++.h>
using namespace std;

// Forward declaration
class Congregation;
class Program;
class Venue;

class Congregation
{
protected:
    string name;
    string start_date;
    string end_date;
    string type;
    static vector<Congregation *> congregation_list;
    static vector<Program *> program_list;

public:
    Congregation(const string &iname, const string &istart_date, const string &iend_date, const string &itype)
        : name(iname), start_date(istart_date), end_date(iend_date), type(itype) {}

    static void showCongregations();

    string getName() const
    {
        return name;
    }

    friend bool isValid(const Congregation &c);
    friend bool isProgramDateValid(const string &start_date, const string &end_date, const string &congregation);
    friend class Program;
    friend class Venue;
    friend bool isCompatible(const string &ptype, string cname);
    static void showReserved(string cname);
    static void deleteCongregation(const string &name);
};

vector<Congregation *> Congregation::congregation_list;
vector<Program *> Congregation::program_list;

bool isCompatible(const string &ptype, string cname)
{
    auto i = Congregation::congregation_list.begin();
    while (i != Congregation::congregation_list.end())
    {
        if ((*i)->name == cname)
        {
            break;
        }
        i += 1;
    }
    cname = (*i)->type;
    // cout<<cname<<"\n";
    // cout<<ptype<<"\n";

    if ((ptype == "Workshop" || ptype == "Main Conference" || ptype == "Banquet") && cname != "Conference")
    {
        // cout<<"false\n";
        return false;
    }
    if ((ptype == "Ceremony" || ptype == "Track and field" || ptype == "Indoor games" || ptype == "Water sports") && cname != "Games")
    {
        // cout<<"false\n";
        return false;
    }
    if ((ptype == "Pre-concert" || ptype == "Main Concert") && cname != "Concert")
    {
        // cout<<"false\n";
        return false;
    }
    if ((ptype == "Food Court" || ptype == "Exhibition") && cname != "Convention")
    {
        // cout<<"false\n";
        return false;
    }
    return true;
}

bool isProgramDateValid(const string &start_date, const string &end_date, const string &congregation)
{
    auto i = Congregation::congregation_list.begin();
    while (i != Congregation::congregation_list.end())
    {
        if ((*i)->getName() == congregation)
        {
            break;
        }
        ++i;
    }
    if (i == Congregation::congregation_list.end())
    {
        return false;
    }
    // Start and end dates of the congregation list
    string cstart = (*i)->start_date;
    string cend = (*i)->end_date;

    if ((cstart <= start_date && end_date <= cend))
    {
        // Date falls within range
        return true;
    }
    return false;
}

bool isProgramValid(Program *p);

class Program
{
protected:
    string congregation;
    string type;
    string start_date;
    string end_date;
    string name;
    // Which venues has it reserved
    vector<Venue *> venues_reserved;

public:
    Program(const string &cong, const string &itype, const string &iname, const string &istart_date, const string &iend_date)
        : congregation(cong), type(itype), name(iname), start_date(istart_date), end_date(iend_date)
    {
        if (!isProgramValid(this))
        {
            cout << "-1\nError\n";
            return;
        }
        auto i = Congregation::congregation_list.begin();
        while (i != Congregation::congregation_list.end())
        {
            if ((*i)->getName() == congregation)
            {
                (*i)->program_list.push_back(this);
                cout << 0 << "\n";
                break;
            }
            ++i;
        }
    }

    static void showPrograms(const string &congregation);
    friend bool isProgramValid(Program *p);
    friend class Congregation;
    friend class Venue;
    // Implement this later as we need to check if the reservations of this program's reservations have been deleted or not
    static void deleteProgram(string cname, string pname);
};

// Function to check if the venue is already present or not
bool vcheck(string vname, string vcountry);

//  Reserva ons are now made at venues for individual programs of a congrega on and
// not for the whole congrega on. To keep things simple, we will enforce these simple
// rules for reserva ons:
// i.
// ii.
// iii.
// iv.
// v.
// A stadium, e.g. Outdoor / Indoor / Swimming Pool can be reserved for
// compe on of the appropriate category (Track and field / Indoor Games /
// Water Sports) respec vely. In addi on, an outdoor stadium can be reserved for
// programs in a concert or a conven on but not for anything in a conference.
// A hotel can only be reserved for any program in a conference or a conven on.
// You obviously have to specify individual programs in the reserva ons.
// A conven on center can only be reserved for programs in a conven on or a
// concert or a conference but not for any sports compe on.
// A concert hall can only be reserved for programs in a concert.
// Note that we are not constraining reserva ons based on capacity at this me.
bool isVenueCompatible(const string &vtype, const string &ptype)
{
    if (vtype == "Outdoor Stadium" || vtype == "Indoor Stadium" || vtype == "Swimming Pool")
    {
        if (ptype == "Track and field" || ptype == "Indoor games" || ptype == "Water sports" || ptype == "Ceremony")
        {
            return true;
        }
        return false;
    }
    if (vtype == "Hotel")
    {
        if (ptype == "Workshop" || ptype == "Main Conference" || ptype == "Banquet")
        {
            return true;
        }
        return false;
    }
    if (vtype == "Convention Center")
    {
        if (ptype == "Food Court" || ptype == "Exhibition" || ptype == "Main Conference" || ptype == "Main Concert" || ptype == "Main Convention")
        {
            return true;
        }
        return false;
    }
    if (vtype == "Concert Hall")
    {
        if (ptype == "Pre-concert" || ptype == "Main Concert")
        {
            return true;
        }
        return false;
    }
    return false;
}

class Venue
{
private:
    string name;
    vector<string> location;
    string type;
    string capacity;
    // List of all the programs it is reserved for
    vector<Program *> reserved_program;
    // List of all the venues
    static vector<Venue *> vlist;

public:
    Venue(string iname, vector<string> ilocation, string itype, string icapacity)
    {
        // Duplicate or not
        if (!vcheck(iname, ilocation[4]))
        {
            delete this;
            return;
        }

        name = iname;
        location = ilocation;
        type = itype;
        capacity = icapacity;
        vlist.push_back(this);
        // cout<<name<<" "<<type<<" "<<capacity<<"\n";
        cout << 0 << "\n";
    }
    friend Congregation;
    static void showVenues(vector<string> &location, string type);
    static void deleteVenue(string vname, string vcountry);
    friend bool vcheck(string vname, string vcountry);
    static void reserveVenue(string vname, string vcountry, string cname, string pname);
    static void freeVenue(string vname, string vcountry, string cname, string pname);
};
vector<Venue *> Venue::vlist;

// Function to check if the venue is already present in the vlist
bool vcheck(string vname, string vcountry)
{
    for (auto i : Venue::vlist)
    {
        if ((i->name == vname) and (i->location[4] == vcountry))
        {
            return false;
        }
    }
    return true;
}

bool isProgramValid(Program *p)
{
    if (!isProgramDateValid(p->start_date, p->end_date, p->congregation) || !isCompatible(p->type, p->congregation))
    {
        return false;
    }
    return true;
}

void Program::showPrograms(const string &congregation)
{
    cout << Congregation::program_list.size() << "\n";
    for (auto i : Congregation::program_list)
    {
        if (i->congregation == congregation)
        {
            cout << i->type << " " << i->name << " " << i->start_date << " " << i->end_date << "\n";
        }
    }
}

bool isBeforeCurrentDate(const string &date)
{
    int day = stoi(date.substr(8, 2));
    int month = stoi(date.substr(5, 2));
    int year = stoi(date.substr(0, 4));

    time_t now = time(0);
    tm *current = localtime(&now);

    int currentDay = current->tm_mday;
    int currentMonth = current->tm_mon + 1;
    int currentYear = current->tm_year + 1900;

    if (year < currentYear)
        return true;
    else if (year > currentYear)
        return false;

    if (month < currentMonth)
        return true;
    else if (month > currentMonth)
        return false;

    return day < currentDay;
}

bool isInvalidDate(const string &date)
{
    if (date.length() != 10 || date[4] != '-' || date[7] != '-')
        return true;

    int day = stoi(date.substr(8, 2));
    int month = stoi(date.substr(5, 2));
    int year = stoi(date.substr(0, 4));

    if (year < 1900 || year > 9999)
        return true;
    if (month < 1 || month > 12)
        return true;
    if (day < 1)
        return true;

    int daysInMonth[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (month == 2 && (year % 400 == 0 || (year % 100 != 0 && year % 4 == 0)))
        daysInMonth[2] = 29;

    if (day > daysInMonth[month])
        return true;

    return false;
}

bool isDateAfter(const string &start_date, const string &end_date)
{
    int start_day = stoi(start_date.substr(8, 2));
    int start_month = stoi(start_date.substr(5, 2));
    int start_year = stoi(start_date.substr(0, 4));

    int end_day = stoi(end_date.substr(8, 2));
    int end_month = stoi(end_date.substr(5, 2));
    int end_year = stoi(end_date.substr(0, 4));

    if (start_year > end_year)
        return true;
    if (start_year < end_year)
        return false;

    if (start_month > end_month)
        return true;
    if (start_month < end_month)
        return false;

    return start_day > end_day;
}

bool isValid(const Congregation &c)
{
    for (auto i : Congregation::congregation_list)
    {
        if (i->getName() == c.name)
        {
            cout << "-1\nError\n";
            return false;
        }
    }

    if (isBeforeCurrentDate(c.start_date) || isBeforeCurrentDate(c.end_date) || isInvalidDate(c.start_date) || isInvalidDate(c.end_date) || isDateAfter(c.start_date, c.end_date))
    {
        cout << "-1\nError\n";
        return false;
    }

    return true;
}

class games : public Congregation
{
public:
    games(const string &iname, const string &istart_date, const string &iend_date, const string &itype)
        : Congregation(iname, istart_date, iend_date, itype)
    {
        if (isValid(*this))
        {
            congregation_list.push_back(this);
            cout << 0 << "\n";
        }
    }
};

class conference : public Congregation
{
public:
    conference(const string &iname, const string &istart_date, const string &iend_date, const string &itype)
        : Congregation(iname, istart_date, iend_date, itype)
    {
        if (isValid(*this))
        {
            congregation_list.push_back(this);
            cout << 0 << "\n";
        }
    }
};

class concerts : public Congregation
{
public:
    concerts(const string &iname, const string &istart_date, const string &iend_date, const string &itype)
        : Congregation(iname, istart_date, iend_date, itype)
    {
        if (isValid(*this))
        {
            congregation_list.push_back(this);
            cout << 0 << "\n";
        }
    }
};

class convention : public Congregation
{
public:
    convention(const string &iname, const string &istart_date, const string &iend_date, const string &itype)
        : Congregation(iname, istart_date, iend_date, itype)
    {
        if (isValid(*this))
        {
            congregation_list.push_back(this);
            cout << 0 << "\n";
        }
    }
};

void Congregation::showCongregations()
{
    cout << congregation_list.size() << "\n";
    for (auto i : congregation_list)
    {
        cout << i->getName() << " " << i->type << " " << i->start_date << " " << i->end_date << "\n";
    }
}

void Venue::showVenues(vector<string> &loc, string type)
{
    int count = 0;
    vector<string> ans;

    // Country is not specified
    if (loc[3] == "")
    {
        cout << "-1\nError\n";
        return;
    }
    // City is specified but not stae
    if (loc[0] != "" && loc[1] == "")
    {
        cout << "-1\nError\n";
        return;
    }

    for (auto i : vlist)
    {
        if ((loc[0] == "" || (i->location)[1] == loc[0]) &&
            (loc[1] == "" || (i->location)[2] == loc[1]) &&
            (loc[2] == "" || (i->location)[3] == loc[2]) &&
            (loc[3] == "" || (i->location)[4] == loc[3]) &&
            (type == "" || i->type == type))
        {
            string s = i->name + " " + i->location[0] + ":" + i->location[1] + ":" + i->location[2] + ":" + i->location[3] + ":" + i->location[4] + " " + i->type + " " + (i->capacity);
            ans.push_back(s);
            count++;
        }
    }

    cout << count << "\n";
    for (auto i : ans)
    {
        cout << i << "\n";
    }
}

void Venue::deleteVenue(string vname, string vcountry)
{
    bool flag = true;
    auto i = vlist.begin();
    while (i != vlist.end())
    {
        // cout<<(*i)->name<<" "<<(*i)->location[4]<<"\n";
        // cout<<vname<<" "<<vcountry<<"\n";
        if (((*i)->name == vname) and ((*i)->location[4] == vcountry))
        {

            delete *i;
            i = vlist.erase(i);
            flag = false;
        }
        else
        {
            i += 1;
        }
    }

    if (flag)
    {
        // Venue not found
        cout << "-1\nError\n";
        return;
    }
    // Now check if the venue is reserved for any program
    for (auto i : Congregation::program_list)
    {
        for (auto j : i->venues_reserved)
        {
            if (j->name == vname && j->location[4] == vcountry)
            {
                cout << "-1\nError\n";
                return;
            }
        }
    }

    cout << 0 << "\n";
}

void Venue::reserveVenue(string vname, string vcountry, string cname, string pname)
{
    auto i = vlist.begin();
    // Find the venue
    while (i != vlist.end())
    {
        if ((*i)->name == vname && (*i)->location[4] == vcountry)
        {
            break;
        }
        i += 1;
    }

    // Find the program
    auto j = Congregation::program_list.begin();
    while (j != Congregation::program_list.end())
    {
        if ((*j)->congregation == cname && (*j)->name == pname)
        {
            break;
        }
        j += 1;
    }

    // Venue not found or program not found
    if (i == vlist.end() || j == Congregation::program_list.end())
    {
        cout << "-1\nError\n";
        return;
    }

    // cout<<(*i)->name<<" "<<(*j)->name<<"\n";
    // cout<<(*i)->type<<" "<<(*j)->type<<"\n";
    if (!isVenueCompatible((*i)->type, (*j)->type))
    {
        cout << "-1\nError:Venue incompatible\n";
        return;
    }

    // for (auto k : (*i)->reserved_program)
    // {
    //     // Does it overlap with other programs
    //     string start_date = (*j)->start_date;
    //     string end_date = (*j)->end_date;
    //     string k_start_date = k->start_date;
    //     string k_end_date = k->end_date;
    //     if((start_date<=k_start_date && end_date<=k_end_date) || (k_start_date<=start_date && end_date<=k_end_date) || (k_start_date<=start_date && k_end_date<=end_date))
    //     {
    //         cout << "-1\nError:Overlap\n";
    //         return;
    //     }
    // }
    for (auto k : (*i)->reserved_program)
    {
        // Get the date ranges
        string start_date = (*j)->start_date;
        string end_date = (*j)->end_date;
        string k_start_date = k->start_date;
        string k_end_date = k->end_date;

        // Check for overlap
        if (start_date < k_end_date && end_date > k_start_date)
        {
            cout << "-1\nError\n";
            return;
        }
    }

    // Reserve the venue
    (*i)->reserved_program.push_back(*j);
    // Add the venue name to the program
    (*j)->venues_reserved.push_back(*i);

    cout << 0 << "\n";
}

void Congregation::showReserved(string cname)
{
    // Print the no.of programs in the congregation
    int count = 0;
    for (auto i : program_list)
    {
        if (i->congregation == cname)
        {
            count++;
        }
    }
    if (count == 0)
    {
        // Congregation not found
        cout << "-1\nError\n";
        return;
    }
    cout << count << "\n";
    // Now print the program name and type
    for (auto i : program_list)
    {
        if (i->congregation == cname)
        {
            cout << i->name << " " << i->type << " " << i->venues_reserved.size() << "\n";
            // Now print the full venue details
            for (auto j : i->venues_reserved)
            {
                cout << j->name << " " << j->location[0] << ":" << j->location[1] << ":" << j->location[2] << ":" << j->location[3] << ":" << j->location[4] << " " << j->type << " " << j->capacity << "\n";
            }
        }
    }
}

void Venue::freeVenue(string vname, string vcountry, string cname, string pname)
{
    auto i = vlist.begin();
    // Find the venue
    while (i != vlist.end())
    {
        if ((*i)->name == vname && (*i)->location[4] == vcountry)
        {
            break;
        }
        i += 1;
    }

    // Find the program
    auto j = Congregation::program_list.begin();
    while (j != Congregation::program_list.end())
    {
        if ((*j)->congregation == cname && (*j)->name == pname)
        {
            break;
        }
        j += 1;
    }

    // Venue not found or program not found
    if (i == vlist.end() || j == Congregation::program_list.end())
    {
        cout << "-1\nError\n";
        return;
    }

    // Find the program in the reserved list of the venue
    auto k = (*i)->reserved_program.begin();
    while (k != (*i)->reserved_program.end())
    {
        if ((*k)->name == pname && (*k)->congregation == cname)
        {
            break;
        }
        k += 1;
    }

    // Program not found in the reserved list of the venue
    if (k == (*i)->reserved_program.end())
    {
        cout << "-1\nError\n";
        return;
    }

    // Remove the program from the reserved list of the venue
    (*i)->reserved_program.erase(k);

    // Find the venue in the reserved list of the program
    auto l = (*j)->venues_reserved.begin();
    while (l != (*j)->venues_reserved.end())
    {
        if ((*l)->name == vname && (*l)->location[4] == vcountry)
        {
            break;
        }
        l += 1;
    }

    // Venue not found in the reserved list of the program
    if (l == (*j)->venues_reserved.end())
    {
        cout << "-1\nError\n";
        return;
    }

    // Remove the venue from the reserved list of the program
    (*j)->venues_reserved.erase(l);

    cout << 0 << "\n";
}

void Program::deleteProgram(string cname, string pname)
{
    auto i = Congregation::program_list.begin();
    // Find the program
    while (i != Congregation::program_list.end())
    {
        if ((*i)->congregation == cname && (*i)->name == pname)
        {
            break;
        }
        i += 1;
    }

    // Program not found
    if (i == Congregation::program_list.end())
    {
        cout << "-1\nError\n";
        return;
    }

    // Check if the program has any reservations
    if ((*i)->venues_reserved.size() > 0)
    {
        cout << "-1\nError\n";
        return;
    }

    Congregation::program_list.erase(i);
    cout << 0 << "\n";
}

void Congregation::deleteCongregation(const string &name)
{
    auto i = congregation_list.begin();
    // Find the congregation
    while (i != congregation_list.end())
    {
        if ((*i)->name == name)
        {
            cout << 0 << "\n";
            congregation_list.erase(i);
            return;
        }
        i += 1;
    }
    // Not found
    if (i == congregation_list.end())
    {
        cout << "-1\nError\n";
        return;
    }
    // Delete all reservations of all the programs of the congregation and then delete the programs under it
    // Delete the reservation of the programs
    for (auto j : program_list)
    {
        if (j->congregation == name)
        {
            // All reservations deleted
            j->venues_reserved.clear();
        }
    }
    for (auto j : program_list)
    {
        // All programs under that congregation deleted
        if (j->congregation == name)
        {
            delete j;
        }
    }
}

// Function for splitting the location for extracting address, country etc
vector<string> split(const string &input, char delimiter)
{
    vector<string> tokens;
    stringstream ss(input);
    string token;

    while (getline(ss, token, delimiter))
    {
        tokens.push_back(token);
    }

    return tokens;
}

int main()
{
    // Strings for command comparison
    string addc = "addCongregation";
    string delc = "deleteCongregation";
    string showc = "showCongregations";
    string addp = "addProgram";
    string delp = "deleteProgram";
    string showp = "showPrograms";
    string addv = "addVenue";
    string delv = "deleteVenue";
    string showv = "showVenues";
    string resv = "reserveVenue";
    string freev = "freeVenue";
    string showr = "showReserved";
    string end = "End";

    // Input sentence
    string input;
    string command;

    // Run the loop till we encounter the command "End"
    do
    {
        getline(cin, input);
        // Parsing input for comparison
        istringstream iss(input);
        // Get the command from the first word of the sentence
        iss >> command;

        if (command == addc)
        {
            // Call addcongregation
            string name;
            string type;
            string start_date;
            string end_date;

            // String parsing
            getline(iss, name, '"');
            getline(iss, name, '"');
            getline(iss, type, '"');
            getline(iss, type, '"');
            getline(iss, start_date, '"');
            getline(iss, start_date, '"');
            getline(iss, end_date, '"');
            getline(iss, end_date, '"');

            if (type == "Games")
            {
                games *g = new games(name, start_date, end_date, type);
            }
            else if (type == "Concert")
            {
                concerts *con = new concerts(name, start_date, end_date, type);
            }
            else if (type == "Convention")
            {
                convention *co = new convention(name, start_date, end_date, type);
            }
            else if (type == "Conference")
            {
                conference *cn = new conference(name, start_date, end_date, type);
            }
        }
        else if (command == delc)
        {
            // Call deletecongregation
            string name;

            getline(iss, name, '"');
            getline(iss, name, '"');

            Congregation::deleteCongregation(name);
        }
        else if (command == showc)
        {
            // Call showcongregations
            Congregation::showCongregations();
        }
        else if (command == addp)
        {
            // Call addprogram
            string congregation;
            string program_type;
            string program_name;
            string start_date;
            string end_date;

            getline(iss, congregation, '"');
            getline(iss, congregation, '"');
            getline(iss, program_type, '"');
            getline(iss, program_type, '"');
            getline(iss, program_name, '"');
            getline(iss, program_name, '"');
            getline(iss, start_date, '"');
            getline(iss, start_date, '"');
            getline(iss, end_date, '"');
            getline(iss, end_date, '"');

            Program *p = new Program(congregation, program_type, program_name, start_date, end_date);
        }
        else if (command == delp)
        {
            // Call deleteprogram
            string congregation;
            string program_name;

            getline(iss, congregation, '"');
            getline(iss, congregation, '"');
            getline(iss, program_name, '"');
            getline(iss, program_name, '"');

            Program::deleteProgram(congregation, program_name);
        }
        else if (command == showp)
        {
            // Call showprograms
            string congregation;

            getline(iss, congregation, '"');
            getline(iss, congregation, '"');

            Program::showPrograms(congregation);
        }
        else if (command == addv)
        {
            // Call addvenue
            string name;
            string location;
            string type;
            string capacity;

            getline(iss, name, '"');
            getline(iss, name, '"');
            getline(iss, location, '"');
            getline(iss, location, '"');
            getline(iss, type, '"');
            getline(iss, type, '"');
            iss >> capacity;
            // Parsing location string
            vector<string> Location = split(location, ':');
            Venue *v = new Venue(name, Location, type, capacity);
        }
        else if (command == delv)
        {
            // Call deletevenue
            string name;
            string country;

            getline(iss, name, '"');
            getline(iss, name, '"');
            getline(iss, country, '"');
            getline(iss, country, '"');

            Venue::deleteVenue(name, country);
        }
        else if (command == showv)
        {
            string location;
            string type;

            // First check if there is type string input or not
            int count_quotes = 0;
            for (auto i : input)
            {
                if (i == '"')
                {
                    count_quotes += 1;
                }
            }
            // We are checking it based on the no. of quotes in the input
            if (count_quotes == 2)
            {
                // Type string is not present
                getline(iss, location, '"');
                getline(iss, location, '"');
                char delimit = ':';
                vector<string> Location = split(location, ':');
                // Call without type
                Venue::showVenues(Location, "");
            }
            else if (count_quotes == 4)
            {
                // Type string is present

                getline(iss, location, '"');
                getline(iss, location, '"');
                getline(iss, type, '"');
                getline(iss, type, '"');
                vector<string> Location = split(location, ':');
                // Call with type
                Venue::showVenues(Location, type);
            }
        }
        else if (command == resv)
        {
            // Call reservevenue
            string venue_name;
            string venue_country;
            string congregation;
            string program_name;

            getline(iss, venue_name, '"');
            getline(iss, venue_name, '"');
            getline(iss, venue_country, '"');
            getline(iss, venue_country, '"');
            getline(iss, congregation, '"');
            getline(iss, congregation, '"');
            getline(iss, program_name, '"');
            getline(iss, program_name, '"');

            Venue::reserveVenue(venue_name, venue_country, congregation, program_name);
        }
        else if (command == freev)
        {
            string venue_name;
            string venue_country;
            string congregation;
            string program_name;

            getline(iss, venue_name, '"');
            getline(iss, venue_name, '"');
            getline(iss, venue_country, '"');
            getline(iss, venue_country, '"');
            getline(iss, congregation, '"');
            getline(iss, congregation, '"');
            getline(iss, program_name, '"');
            getline(iss, program_name, '"');

            Venue::freeVenue(venue_name, venue_country, congregation, program_name);
        }
        else if (command == showr)
        {
            string congregation;

            getline(iss, congregation, '"');
            getline(iss, congregation, '"');

            Congregation::showReserved(congregation);
        }

        else if (command == end)
        {
            break;
        }

    } while (1);
}
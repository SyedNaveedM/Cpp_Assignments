#include <bits/stdc++.h>
using namespace std;
using namespace std::chrono;

// Forward declarations
class Venue;
class Event;
class Congregation;
class Reservation;

// Gets the current date for comparison
string getCurrentDate()
{
    time_t now = time(nullptr);
    tm *timeinfo = localtime(&now);
    char buffer[11];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d", timeinfo);
    return string(buffer);
}

// Checks if the date is valid
bool isValidDate(string date)
{
    if (date.size() != 10 || date[4] != '-' || date[7] != '-')
    {
        return false;
    }

    if (date < getCurrentDate())
    {
        return false;
    }

    int year = stoi(date.substr(0, 4));
    int month = stoi(date.substr(5, 2));
    int day = stoi(date.substr(8, 2));

    if (year < 0 || month < 1 || month > 12 || day < 1 || day > 31)
    {
        return false;
    }

    if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30)
    {
        return false;
    }
    else if (month == 2)
    {
        if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
        {
            if (day > 29)
            {
                return false;
            }
        }
        else if (day > 28)
        {
            return false;
        }
    }

    return true;
}

// Function to convert "YYYY-MM-DD" and "hh:mm" to a time_point
time_point<system_clock> parseDateAndTime(const string &date, const string &time)
{
    int year, month, day, hour, minute;
    sscanf(date.c_str(), "%d-%d-%d", &year, &month, &day);
    sscanf(time.c_str(), "%d:%d", &hour, &minute);

    tm t = {};
    t.tm_year = year - 1900; // Years since 1900
    t.tm_mon = month - 1;    // Months since January
    t.tm_mday = day;
    t.tm_hour = hour;
    t.tm_min = minute;

    return system_clock::from_time_t(mktime(&t));
}

// Function to calculate the difference in minutes between two datetime pairs
int differenceInMinutesBetweenDateTimes(const string &date1, const string &time1_1, const string &time1_2,
                                        const string &date2, const string &time2_1, const string &time2_2)
{
    time_point<system_clock> datetime1 = parseDateAndTime(date1, time1_1);
    time_point<system_clock> datetime2 = parseDateAndTime(date2, time2_2);

    // Calculate the duration between the two time points in minutes
    duration<double> diff = datetime2 - datetime1;
    return duration_cast<minutes>(diff).count();
}

// Function to parse date string in "YYYY-MM-DD" format into a tm structure
tm parseDate(const string &date)
{
    tm t = {};
    sscanf(date.c_str(), "%d-%d-%d", &t.tm_year, &t.tm_mon, &t.tm_mday);
    t.tm_year -= 1900; // Years since 1900
    t.tm_mon -= 1;     // Months since January
    return t;
}

// Function to get dates in the format "YYYY-MM-DD" from from_date to end_date
vector<string> getDateRange(const string &from_date, const string &end_date)
{
    vector<string> dateRange; // Vector to store the dates

    // Parse the from_date and end_date into tm structures
    tm from_tm = parseDate(from_date);
    tm end_tm = parseDate(end_date);

    // Convert tm structures into time_point<system_clock>
    time_point<system_clock> from_time = system_clock::from_time_t(mktime(&from_tm));
    time_point<system_clock> end_time = system_clock::from_time_t(mktime(&end_tm));

    // Loop through each day in the range
    for (auto current_time = from_time; current_time <= end_time; current_time += hours(24))
    {
        // Convert current time_point back to tm
        time_t current_time_t = system_clock::to_time_t(current_time);
        tm current_tm = *localtime(&current_time_t);

        // Format the date in "YYYY-MM-DD"
        int year = current_tm.tm_year + 1900;
        int month = current_tm.tm_mon + 1; // tm_mon is zero-based
        int day = current_tm.tm_mday;

        // Manually format the date string
        stringstream dateStream;
        dateStream << year << "-"
                   << (month < 10 ? "0" : "") << month << "-"
                   << (day < 10 ? "0" : "") << day;

        // Add the formatted date to the vector
        dateRange.push_back(dateStream.str());
    }

    return dateRange; // Return the vector containing all dates
}

// Venue class definition
class Venue
{
private:
    string name;
    int capacity;
    vector<string> location;
    static vector<Venue> venues;

public:
    Venue(string n, int c, vector<string> loc);

    static vector<Venue> getVenues();
    string getName();
    string getCountry();
    vector<string> getLocation();
    int getCapacity();

    static void addVenue(string n, int c, vector<string> &loc);
    static void deleteVenue(string n, string c);
    static void showVenues(vector<string> &loc);
};

// Event class definition
class Event
{
private:
    string congregation_name;
    string venue_name;
    string venue_country;
    string date;
    string from_time;
    string to_time;
    string event_name;
    static vector<Event> events;

public:
    Event(string cn, string vn, string vc, string d, string ft, string tt, string en);

    string getName();
    string getDate();
    string getFromTime();
    string getToTime();
    string getVenueName();
    string getVenueCountry();
    string getCongregationName();
    vector<Event> getEvents();

    static void addEvent(string cn, string vn, string vc, string d, string ft, string tt, string en);
    static void deleteEvent(string cn, string vn, string vc, string d, string ft, string en);
    static void showEvents(string venue_name, string venue_country, string date);
    static void showCalendar(string cn, string vn, string vc);
};

// Congregation class definition
class Congregation
{
private:
    string name;
    string type;
    string start_date;
    string end_date;
    static vector<Congregation> congregations;
    static vector<Reservation> reservation;

public:
    Congregation(string n, string t, string sd, string ed);

    vector<Reservation> getReservation();
    static vector<Congregation> getCongregations();
    void addCongregation(string n, string t, string sd, string ed);
    void deleteCongregation(string n);
    void showCongregations();
    static void showReserved(string cn);
    static void freeVenue(string vn, string vc, string cn);
    static void reserveVenue(string vn, string vc, string cn);
    string getName();
    string getStartDate();
    string getEndDate();
};

// Reservation class definition
class Reservation
{
private:
    string venue_name;
    string venue_country;
    string congregation_name;

public:
    Reservation(string vn, string vc, string cn);

    string getVenueName();
    string getVenueCountry();
    string getCongregationName();
};

// Static member definitions
vector<Venue> Venue::venues;
vector<Congregation> Congregation::congregations;
vector<Reservation> Congregation::reservation;

// Venue class implementations
Venue::Venue(string n, int c, vector<string> loc)
    : name(n), capacity(c), location(loc) {}

vector<Venue> Venue::getVenues()
{
    return venues;
}

string Venue::getName()
{
    return name;
}

string Venue::getCountry()
{
    return location[4];
}

vector<string> Venue::getLocation()
{
    return location;
}

int Venue::getCapacity()
{
    return capacity;
}

void Venue::addVenue(string n, int c, vector<string> &loc)
{
    for (auto &i : venues)
    {
        // Check if the venue already exists
        if (i.name == n && i.location[4] == loc[4])
        {
            cout << "-1\nError\n";
            return;
        }
    }
    // Create a new venue object and add it to the vector
    venues.emplace_back(n, c, loc);
    cout << "0\n";
}

void Venue::deleteVenue(string n, string c)
{
    bool found = false;
    // Create a object of Congregation class to access getReservation
    Congregation c1("", "", "", "");

    // Check if the venue is reserved already
    for (auto &i : c1.getReservation())
    {
        if (i.getVenueName() == n && i.getVenueCountry() == c)
        {
            cout << "-1\nError\n";
            return;
        }
    }
    // We need to check only if the venue is reserved or not as we can't add a congregation in the past

    // Check if the venue exists and delete it
    for (auto i = venues.begin(); i != venues.end(); i++)
    {
        if (i->name == n && i->location[4] == c)
        {
            venues.erase(i);
            found = true;
            cout << "0\n";
            return;
        }
    }
    if (!found)
    {
        cout << "-1\nError\n";
    }
}

void Venue::showVenues(vector<string> &loc)
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

    for (auto &i : venues)
    {
        if ((loc[0] == "" || i.location[1] == loc[0]) &&
            (loc[1] == "" || i.location[2] == loc[1]) &&
            (loc[2] == "" || i.location[3] == loc[2]) &&
            (loc[3] == "" || i.location[4] == loc[3]))
        {
            string s = i.name + " " + i.location[0] + ":" + i.location[1] + ":" + i.location[2] + ":" + i.location[3] + ":" + i.location[4] + " " + to_string(i.capacity);
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

// Event class implementations
Event::Event(string cn, string vn, string vc, string d, string ft, string tt, string en)
    : congregation_name(cn), venue_name(vn), venue_country(vc), date(d), from_time(ft), to_time(tt), event_name(en) {}

string Event::getName()
{
    return event_name;
}

string Event::getCongregationName()
{
    return congregation_name;
}

string Event::getDate()
{
    return date;
}

string Event::getFromTime()
{
    return from_time;
}

string Event::getToTime()
{
    return to_time;
}

string Event::getVenueName()
{
    return venue_name;
}

string Event::getVenueCountry()
{
    return venue_country;
}

vector<Event> Event::getEvents()
{
    return events;
}

vector<Event> Event::events;

void Event::addEvent(string congregation_name, string venue_name, string venue_country, string date, string from_time, string to_time, string event_name)
{
    if (!isValidDate(date))
    {
        cout << "-1\nError\n";
        return;
    }

    if (from_time > to_time)
    {
        cout << "-1\nError\n";
        return;
    }

    // Check if the event is under a valid congregation
    bool found_congregation = false;
    for (auto i : Congregation::getCongregations())
    {
        if (i.getName() == congregation_name)
        {
            found_congregation = true;
            break;
        }
    }
    if (!found_congregation)
    {
        cout << "-1\nError\n";
        return;
    }

    // Check if the event is within a valid reservation
    bool found = false;
    Congregation c1("", "", "", "");
    for (auto &i : c1.getReservation())
    {
        if (i.getVenueName() == venue_name && i.getVenueCountry() == venue_country && i.getCongregationName() == congregation_name)
        {
            found = true;
            break;
        }
    }

    if (!found)
    {
        cout << "-1\nError\n";
        return;
    }

    // The minutes should only be 00, 15, 30, 45
    if (stoi(from_time.substr(3, 2)) % 15 != 0 || stoi(to_time.substr(3, 2)) % 15 != 0)
    {
        cout << "-1\nError\n";
        return;
    }

    // Function for checking if the event is at max 24 hours
    if (differenceInMinutesBetweenDateTimes(date, from_time, to_time, date, from_time, to_time) > 1440)
    {
        cout << "-1\nError\n";
        return;
    }

    for (auto &i : events)
    {
        // Check if the new event starts less than 30 minutes after the existing event ends
        if (abs(differenceInMinutesBetweenDateTimes(i.getDate(), i.getToTime(), to_time, date, from_time, to_time)) < 30 ||
            // Check if the existing event starts less than 30 minutes after the new event ends
            abs(differenceInMinutesBetweenDateTimes(date, from_time, to_time, i.getDate(), i.getFromTime(), i.getToTime())) < 30)
        {
            cout << "-1\nError\n";
            return;
        }
    }

    Congregation c2("", "", "", "");
    for (auto &i : c2.getReservation())
    {
        if (i.getVenueName() == venue_name && i.getVenueCountry() == venue_country && i.getCongregationName() == congregation_name)
        {
            for (int index = 0; index < events.size(); index++)
            {
                if (events[index].getDate() > date)
                {
                    events.insert(index + events.begin(), Event(congregation_name, venue_name, venue_country, date, from_time, to_time, event_name));
                    cout << 0 << endl;
                    return;
                }
                else if (events[index].getDate() == date)
                {
                    for (int equalIndex = index; equalIndex < events.size(); equalIndex++)
                    {
                        if (events[index].getDate() != date || events[index].getDate() == date && events[index].getFromTime() > from_time)
                        {
                            events.insert(index + events.begin(), Event(congregation_name, venue_name, venue_country, date, from_time, to_time, event_name));
                            cout << 0 << endl;
                            return;
                        }
                    }
                    events.push_back(Event(congregation_name, venue_name, venue_country, date, from_time, to_time, event_name));
                    cout << 0 << endl;
                    return;
                }
            }

            events.push_back(Event(congregation_name, venue_name, venue_country, date, from_time, to_time, event_name));
            cout << 0 << endl;
            return;
        }
    }
}

void Event::showEvents(string venue_name, string venue_country, string date)
{
    int count = 0;
    vector<string> ans;


    for (auto &i : events)
    {
        if (i.getVenueName() == venue_name && i.getVenueCountry() == venue_country && i.getDate() == date)
        {
            string s = i.getName() + " " + i.getFromTime() + " " + i.getToTime();
            ans.push_back(s);
            count++;
        }
    }
    // if (count == 0)
    // {
    //     cout << "-1\nError\n";
    //     return;
    // }

    cout << count << "\n";
    for (auto i : ans)
    {
        cout << i << "\n";
    }
}

void Event::deleteEvent(string congregation_name, string venue_name, string venue_country, string date, string from_time, string event_name)
{
    for (auto i = events.begin(); i != events.end(); i++)
    {
        if (i->getCongregationName() == congregation_name && i->getVenueName() == venue_name && i->getVenueCountry() == venue_country && i->getDate() == date && i->getFromTime() == from_time && i->getName() == event_name)
        {
            events.erase(i);
            cout << "0\n";
            return;
        }
    }
    cout << "-1\nError\n";
}

void Event::showCalendar(string cn, string vn, string vc)
{
    int count = 0;
    vector<string> ans;

    // Function to check if the congregation exists
    bool found_congregation = false;
    for (auto i : Congregation::getCongregations())
    {
        if (i.getName() == cn)
        {
            found_congregation = true;
            break;
        }
    }

    if (!found_congregation)
    {
        cout << "-1\nError\n";
        return;
    }

    // Function to check if the venue exists
    bool found = false;
    for (auto &i : Venue::getVenues())
    {
        if (i.getName() == vn && i.getCountry() == vc)
        {
            found = true;
            break;
        }
    }

    if (!found_congregation || !found)
    {
        cout << "-1\nError\n";
        return;
    }

    // Function to check if there is no reservations for the congregation
    bool flag = true;
    Congregation c("", "", "", "");
    for (auto &i : c.getReservation())
    {
        if (i.getVenueName() == vn && i.getVenueCountry() == vc && i.getCongregationName() == cn)
        {
            flag = false;
            break;
        }
    }

    if (flag)
    {
        cout << "-1\nError\n";
        return;
    }

    // Count the number of events
    for (auto &i : events)
    {
        if (i.getVenueName() == vn && i.getVenueCountry() == vc && i.getCongregationName() == cn)
        {
            count++;
        }
    }

    // Loop through the congregations and see congregation index
    int j = 0;
    for (auto i : Congregation::getCongregations())
    {
        if (i.getName() == cn)
        {
            break;
        }
        j++;
    }

    cout << count << "\n";
    // This will get the dates of the congregation we require
    vector<string> dates = getDateRange(Congregation::getCongregations()[j].getStartDate(), Congregation::getCongregations()[j].getEndDate());
    for (auto &i : dates)
    {
        int event_count = 0;
        for (auto &j : events)
        {
            if (j.getVenueName() == vn && j.getVenueCountry() == vc && j.getCongregationName() == cn && j.getDate() == i)
            {
                event_count++;
            }
        }
        cout << i << " " << event_count << "\n";
        for (auto &j : events)
        {
            if (j.getVenueName() == vn && j.getVenueCountry() == vc && j.getCongregationName() == cn && j.getDate() == i)
            {
                cout << j.getName() << " " << j.getFromTime() << " " << j.getToTime() << "\n";
            }
        }
    }
}

// Congregation class implementations
Congregation::Congregation(string n, string t, string sd, string ed)
    : name(n), type(t), start_date(sd), end_date(ed) {}

vector<Reservation> Congregation::getReservation()
{
    return reservation;
}

vector<Congregation> Congregation::getCongregations()
{
    return congregations;
}

string Congregation::getName()
{
    return name;
}

string Congregation::getStartDate()
{
    return start_date;
}

string Congregation::getEndDate()
{
    return end_date;
}

void Congregation::addCongregation(string n, string t, string sd, string ed)
{
    for (auto &i : congregations)
    {
        if (i.name == n)
        {
            cout << "-1\nError\n";
            return;
        }
    }

    if (!isValidDate(sd) || !isValidDate(ed))
    {
        cout << "-1\nError\n";
        return;
    }

    if (sd > ed)
    {
        cout << "-1\nError\n";
        return;
    }

    if (sd <= getCurrentDate())
    {
        cout << "-1\nError\n";
        return;
    }

    congregations.push_back(Congregation(n, t, sd, ed));
    cout << "0\n";
}

void Congregation::deleteCongregation(string n)
{
    // Step 1: Collect events to be deleted
    vector<Event> eventsToDelete;
    Event e("", "", "", "", "", "", "");
    for (auto &event : e.getEvents())
    {
        if (event.getCongregationName() == n)
        {
            eventsToDelete.push_back(event);
        }
    }

    // Step 2: Delete events
    for (auto &event : eventsToDelete)
    {
        Event e("", "", "", "", "", "", "");
        // Event::deleteEvent(event.getCongregationName(), event.getVenueName(), event.getVenueCountry(), event.getDate(), event.getFromTime(), event.getName());
        vector<Event> events = e.getEvents();
        for (auto i = events.begin(); i != events.end(); i++)
        {
            if (i->getCongregationName() == n)
            {
                events.erase(i);
                i--;
            }
        }
    }

    // Step 3: Remove the congregation
    auto it = remove_if(congregations.begin(), congregations.end(), [&](const Congregation &c)
                        { return c.name == n; });

    if (it != congregations.end())
    {
        congregations.erase(it, congregations.end());
    }
    else
    {
        cout << "-1\nError\n";
        return;
    }

    // Delete the reservations associated with the congregation
    for (auto i = reservation.begin(); i != reservation.end();)
    {
        if (i->getCongregationName() == n)
        {
            i = reservation.erase(i);
        }
        else
        {
            ++i;
        }
    }
    cout << 0 << endl;
}

void Congregation::showCongregations()
{
    cout << congregations.size() << "\n";
    for (auto &i : congregations)
    {
        cout << i.name << " " << i.type << " " << i.start_date << " " << i.end_date << "\n";
    }
}

void Congregation::showReserved(string cn)
{
    int count = 0;
    bool flag = true;
    for (auto &i : reservation)
    {
        if (i.getCongregationName() == cn)
        {
            count++;
            flag = false;
        }
    }
    if (flag)
    {
        cout << "-1\nError\n";
        return;
    }
    cout << count << "\n";
    for (auto &i : reservation)
    {
        if (i.getCongregationName() == cn)
        {
            cout << i.getVenueName() << " ";
            for (auto &j : Venue::getVenues())
            {
                if (j.getName() == i.getVenueName() && j.getCountry() == i.getVenueCountry())
                {
                    for (int k = 0; k < j.getLocation().size(); k++)
                    {
                        if (k != j.getLocation().size() - 1)
                        {
                            cout << j.getLocation()[k] << ":";
                        }
                        else
                        {
                            cout << j.getLocation()[k] << " ";
                        }
                    }
                    cout << j.getCapacity() << "\n";
                    break;
                }
            }
        }
    }
}

void Congregation::freeVenue(string vn, string vc, string cn)
{
    // Free all the events in this reservation timeframe and in this venue
    Event e("", "", "", "", "", "", "");
    vector<Event> events = e.getEvents();
    for (auto i = events.begin(); i != events.end();)
    {
        if (i->getVenueName() == vn && i->getVenueCountry() == vc && i->getCongregationName() == cn)
        {
            i = events.erase(i);
        }
        else
        {
            ++i;
        }
    }

    for (auto i = reservation.begin(); i != reservation.end(); i++)
    {
        if (i->getVenueName() == vn && i->getVenueCountry() == vc && i->getCongregationName() == cn)
        {
            reservation.erase(i);
            cout << "0\n";
            return;
        }
    }
    cout << "-1\nError\n";
}

void Congregation::reserveVenue(string vn, string vc, string cn)
{
    for (auto &i : reservation)
    {
        if (i.getVenueName() == vn && i.getVenueCountry() == vc)
        {
            cout << "-1\nError\n";
            return;
        }
    }
    reservation.push_back(Reservation(vn, vc, cn));
    cout << "0\n";
}

// Reservation class implementations
Reservation::Reservation(string vn, string vc, string cn)
    : venue_name(vn), venue_country(vc), congregation_name(cn) {}

string Reservation::getVenueName()
{
    return venue_name;
}

string Reservation::getVenueCountry()
{
    return venue_country;
}

string Reservation::getCongregationName()
{
    return congregation_name;
}

// Function to extract individual strings from the quotes in the input string
string extract(const string &input, size_t &startPos)
{
    size_t firstQuote = input.find('"', startPos) + 1; // Position after the first quote
    size_t secondQuote = input.find('"', firstQuote);  // Position of the closing quote
    startPos = secondQuote + 1;                        // Update startPos for the next search
    return input.substr(firstQuote, secondQuote - firstQuote);
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

string RemoveLeadingWhiteSpaces(string n)
{
    int i = 0;
    while (n[i] == ' ')
    {
        i++;
    }
    n = n.substr(i);
    return n;
}

int main()
{
    // Fast I/O
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    // For input checking
    string addc = "addCongregation";
    string delc = "deleteCongregation";
    string showc = "showCongregations";
    string addv = "addVenue";
    string delv = "deleteVenue";
    string showv = "showVenues";
    string resv = "reserveVenue";
    string freev = "freeVenue";
    string showr = "showReserved";
    string adde = "addEvent";
    string dele = "deleteEvent";
    string showe = "showEvents";
    string showcal = "showCalendar";
    string end = "End";

    // Run an infinite loop till we encounter End
    while (1)
    {
        string input, x;
        getline(cin, input);
        input = RemoveLeadingWhiteSpaces(input);
        // This will extract the first word of the sentence
        string command;
        istringstream stream(input);
        stream >> command;

        // End the loop if the input entered is End
        if (command == end)
        {
            break;
        }

        // Now start checking what is the command entered
        if (command == addc)
        {
            // Call addCongregation
            string name;
            string type;
            string start_date;
            string end_date;

            size_t start = input.find(' ');

            // Extract the quoted strings
            name = extract(input, start);
            type = extract(input, start);
            start_date = extract(input, start);
            end_date = extract(input, start);

            // Make a object of congregation class
            Congregation c(name, type, start_date, end_date);
            c.addCongregation(name, type, start_date, end_date);
        }

        else if (command == delc)
        {
            // Call deleteCongregation

            string name;

            size_t start = input.find(' ');

            name = extract(input, start);

            // Creating object of congregation class for accessing the deleteCongregation method
            Congregation c(name, "", "", "");
            c.deleteCongregation(name);
        }

        else if (command == showc)
        {
            // Call showcongregation
            // Creating object of congregation class for accessing the showCongregations method
            Congregation c("", "", "", "");
            c.showCongregations();
        }

        else if (command == addv)
        {
            // Call addvenue
            string name;
            string Location;
            string capacity;

            size_t start = input.find(' ');
            name = extract(input, start);
            Location = extract(input, start);
            capacity = input.substr(start);

            char delimit = ':';
            vector<string> location = split(Location, delimit);

            // Make object of venue class
            Venue::addVenue(name, stoi(capacity), location);
        }
        else if (command == delv)
        {
            // Call deletevenue
            string name;
            string country;
            size_t start = input.find(' ');
            name = extract(input, start);
            country = extract(input, start);

            // Make object of venue class
            // Venue v(name,0,{});
            // v.deleteVenue(name,country);
            Venue::deleteVenue(name, country);
        }
        else if (command == showv)
        {
            // Call showvenues
            size_t start = input.find(' ');

            string Location;
            Location = extract(input, start);

            // Split the string
            char delimit = ':';
            vector<string> location = split(Location, delimit);

            Venue::showVenues(location);
        }
        else if (command == resv)
        {
            // Call reservevenue
            size_t start = input.find(' ');

            string vname;
            string vcountry;
            string cname;

            vname = extract(input, start);
            vcountry = extract(input, start);
            cname = extract(input, start);

            Congregation::reserveVenue(vname, vcountry, cname);
        }
        else if (command == freev)
        {
            // Call freevenue
            size_t start = input.find(' ');

            string vname;
            string vcountry;
            string cname;

            vname = extract(input, start);
            vcountry = extract(input, start);
            cname = extract(input, start);

            Congregation::freeVenue(vname, vcountry, cname);
        }
        else if (command == showr)
        {
            // Call showreserved
            size_t start = input.find(' ');

            string cname;

            cname = extract(input, start);

            Congregation::showReserved(cname);
        }
        else if (command == adde)
        {
            // Call addevent
            size_t start = input.find(' ');

            string cname;
            string vname;
            string vcountry;
            string date;
            string fromTime;
            string toTime;
            string ename;

            cname = extract(input, start);
            vname = extract(input, start);
            vcountry = extract(input, start);
            date = extract(input, start);
            fromTime = extract(input, start);
            toTime = extract(input, start);
            ename = extract(input, start);

            Event::addEvent(cname, vname, vcountry, date, fromTime, toTime, ename);
        }
        else if (command == dele)
        {
            // Call deleteevent
            size_t start = input.find(' ');

            string cname;
            string vname;
            string vcountry;
            string date;
            string fromTime;
            string ename;

            cname = extract(input, start);
            vname = extract(input, start);
            vcountry = extract(input, start);
            date = extract(input, start);
            fromTime = extract(input, start);
            ename = extract(input, start);

            Event::deleteEvent(cname, vname, vcountry, date, fromTime, ename);
        }
        else if (command == showe)
        {
            // Call showevents
            size_t start = input.find(' ');

            string vname;
            string vcountry;
            string date;

            vname = extract(input, start);
            vcountry = extract(input, start);
            date = extract(input, start);

            Event::showEvents(vname, vcountry, date);
        }
        else if (command == showcal)
        {
            // Call showcalendar
            size_t start = input.find(' ');

            string cname;
            string vname;
            string vcountry;

            cname = extract(input, start);
            vname = extract(input, start);
            vcountry = extract(input, start);

            Event::showCalendar(cname, vname, vcountry);
        }
        else
        {
            cout << "-1\nError\n";
        }
    }
}

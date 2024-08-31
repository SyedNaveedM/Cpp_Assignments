#include <iostream>
#include <string>
using namespace std;

// Forward declarations
class Venue;
class vll;

// Variable to get the no. of venues present right now
int noOfVenues = 0;

// This class is used for events
class Event
{
    private:
        string venueName;
        string eventName;
        int date;
        int fromHour;
        int toHour;
        // For event linked list
        Event* next;

    public:
        // Initialization of the properties of an event when an event is added
        Event(const string& vn, const string& en, int d, int fh, int th)
            : venueName(vn), eventName(en), date(d), fromHour(fh), toHour(th), next(nullptr)
        {
        }

        int fromhour()
        {
            return fromHour;
        }

        int tohour()
        {
            return toHour;
        }

        int Date()
        {
            return date;
        }

        string venue()
        {
            return venueName;
        }

        string event()
        {
            return eventName;
        }

        Event* nxt()
        {
            return next;
        }

        void setNext(Event* n)
        {
            next = n;
        }
};

// This class is used for venues
class Venue
{
    private:
        // The basic properties of the venues
        string name;
        string location;
        int capacity;
        int noOfEvents;
        // Array of length 31 for sorting the events based on sorting
        Event* calendar[31];

    public:
        Venue(const string& n, const string& loc, int cap)
            : name(n), location(loc), capacity(cap), noOfEvents(0)
        {
            for (int i = 0; i < 31; ++i)
            {
                calendar[i] = nullptr;
            }
        }
        
        string getname()
        {
            return name;
        }

        string getlocation()
        {
            return location;
        }

        int getcapacity()
        {
            return capacity;
        }

        int noofevents()
        {
            return noOfEvents;
        }

        void addEvent(vll* vhead, string vn, int dt, int fh, int th, string enm);
        void delEvent(vll* vhead,string venueName,int fromHour,int date,string eventName);
        void showEvents(vll* vhead, string venueName, int date);
        void showCalendar(vll* vhead,string venueName);
};

// Linked list for venues
// vll = venue linked list
// This is where we manage the linked list having venues
class vll
{
    private:
        // The linked list contains a pointer to an object of
        // Venue class and a next pointer 
        Venue* venue;
        vll* next;

    public:
        // Upon creation of an object of vll class it will point to no
        // venue object
        vll() 
        {
            venue=nullptr;
            next=nullptr;
        }

        Venue* getvenue()
        {
            return venue;
        }

        vll* getnext()
        {
            return next;
        }

        // For creating new node
        vll(Venue* v) : venue(v), next(nullptr)
        {
        }

        // We get the vhead from the main function as we are initializing it
        // in the main function
        void addVenue(vll*& vhead, const string& name, const string& location, int capacity)
        {
            for (vll* i = vhead; i != nullptr; i = i->next)
            {
                if (name == i->venue->getname())
                {
                    cout << "-1\nError\n";
                    return;
                }
            }

            // Creating a new venue object in vll
            Venue* newvenue = new Venue(name, location, capacity);
            vll* newnode = new vll(newvenue);

            // If memory allocation fails
            if (!newnode)
            {
                cout << "-1\nError\n";
                return;
            }

            // If the linked list is empty make the new node the head
            if (vhead == nullptr)
            {
                vhead = newnode;
            }
            else
            {
                vll* i = vhead;
                while (i->next)
                {
                    // Go to the end of the linked list as we want to 
                    // show the venues in the order that they have been inserted
                    i = i->next;
                }
                // Insert it at the end
                i->next = newnode;   
            }
            noOfVenues += 1;
            cout << "0\n";
        }

        // Function to delete a venue by name
        void delVenue(vll*& vhead, const string& name)
        {
            // Check if the list is empty
            if (vhead == nullptr)
            {
                cout << "-1\nError\n";
                return;
            }

            // Check if the head node is the one to be deleted
            if (vhead->venue->getname() == name)
            {
                vll* temp = vhead;
                vhead = vhead->next; 
                delete temp->venue; 
                delete temp; 
                noOfVenues -= 1;
                cout << "0\n";
                return;
            }

            // Traverse the list to find the node to delete
            vll* prev = nullptr;
            vll* curr = vhead;
            while (curr != nullptr)
            {
                if (curr->venue->getname() == name)
                {
                    prev->next = curr->next; 
                    delete curr->venue; 
                    delete curr; 
                    noOfVenues -= 1;
                    cout << "0\n";
                    return;
                }
                prev = curr;
                curr = curr->next;
            }
            // If the venue is not found
            cout << "-1\nError\n";
        }

        // Function to show the added venues
        void showVenues(vll* vhead)
        {
            cout << noOfVenues << "\n";
            for (vll* i = vhead; i != nullptr; i = i->next)
            {
                cout << i->venue->getname() << " " <<
                i->venue->getlocation() << " " <<
                i->venue->getcapacity() << "\n";
            }
        }

        // Function to free the entire linked list
        void freeVenues(vll*& head)
        {
            while (head)
            {
                vll* temp = head;
                head = head->next;
                delete temp->venue;
                delete temp;
            }
        }
};


void Venue::addEvent(vll* vhead, string vn, int dt, int fh, int th, string enm)
{
    if (vhead == nullptr)
    {
        cout << "-1\nError\n";
        return;
    }

    vll* i = vhead;
    while (i)
    {
        if (i->getvenue()->getname() == vn)
        {
            break;
        }
        i = i->getnext();
    }
    // No matching venue found
    if (!i)
    {
        cout << "-1\nError\n";
        return;
    }

    // Error in date or time
    if (dt < 1 || dt > 30 || fh < 0 || fh >= 24 || th <= fh || th > 24)
    {
        cout << "-1\nError\n";
        return;
    }

    Event* curr = i->getvenue()->calendar[dt];
    while (curr != nullptr)
    {
        if ((fh >= curr->fromhour() && fh < curr->tohour()) ||
            (th > curr->fromhour() && th <= curr->tohour()) ||
            (fh <= curr->fromhour() && th >= curr->tohour()))
        {
            cout << "-1\nError\n";
            return;
        }
        curr = curr->nxt();
    }

    Event* newEvent = new Event(vn, enm, dt, fh, th);
    if (newEvent == nullptr)
    {
        cout << "-1\nError\n";
        return;
    }

    // Insert the event into the calendar
    Event* prev = nullptr;
    curr = i->getvenue()->calendar[dt];

    while (curr != nullptr && curr->fromhour() < fh)
    {
        prev = curr;
        curr = curr->nxt();
    }

    if (!prev)
    {
        i->getvenue()->calendar[dt] = newEvent;
    }
    else
    {
        prev->setNext(newEvent);
    }
    newEvent->setNext(curr);
    i->getvenue()->noOfEvents += 1;
    cout<<0<<"\n";
}



void Venue::showEvents(vll* vhead, string venueName, int date)
{
    if (!vhead)
    {
        cout << "-1\nError\n";
        return;
    }

    vll* i = vhead;
    // Finding venue
    while (i)
    {
        if (i->getvenue()->getname() == venueName)
        {
            break;
        }
        i = i->getnext();
    }
    // Venue not found
    if (!i)
    {
        cout << "-1\nError\n";
        return;
    }

    // Date invalid
    if (date < 1 || date > 30)
    {
        cout << "-1\nError\n";
        return;
    }

    // Head of event linked list of that day
    Event* curr = i->getvenue()->calendar[date];
    if (!curr)
    {
        cout << "0\n";
        return;
    }

    // Count events for the specific venue
    int eventCount = 0;
    Event* temp = curr;
    while (temp != nullptr)
    {
        eventCount++;
        temp = temp->nxt();
    }

    cout << eventCount << "\n";
    while (curr != nullptr)
    {
        cout << curr->event() << " " << curr->fromhour() << " " << curr->tohour() << "\n";
        curr = curr->nxt();
    }
}




void Venue::delEvent(vll* vhead, string venueName, int fromHour, int date, string eventName)
{
    if (!vhead)
    {
        cout << "-1\nError\n";
        return;
    }

    vll* i = vhead;
    // Finding venue
    while (i)
    {
        if (i->getvenue()->getname() == venueName)
        {
            break;
        }
        i = i->getnext();
    }
    // Venue not found
    if (!i)
    {
        cout << "-1\nError\n";
        return;
    }

    // Date or time invalid
    if (date < 1 || date > 30 || fromHour < 0 || fromHour >= 24)
    {
        cout << "-1\nError\n";
        return;
    }

    // Go to the specified date
    Event* j = i->getvenue()->calendar[date];
    Event* prev = nullptr;

    while (j != nullptr)
    {
        // Match event name and start hour
        if (j->event() == eventName && j->fromhour() == fromHour)
        {
            // Event found; remove it from the list
            if (prev == nullptr)
            {
                // Deleting the head of the event list
                i->getvenue()->calendar[date] = j->nxt();
            }
            else
            {
                // Deleting an event that's not the head
                prev->setNext(j->nxt());
            }
            delete j;
            i->getvenue()->noOfEvents -= 1;
            cout << "0\n";
            return;
        }
        prev = j;
        j = j->nxt();
    }

    // Event not found
    cout << "-1\nError\n";
}



void Venue::showCalendar(vll* vhead, string venueName)
{
    if (!vhead)
    {
        cout << "-1\nError\n";
        return;
    }

    vll* i = vhead;
    while (i)
    {
        if (i->getvenue()->getname() == venueName)
        {
            break;
        }
        i = i->getnext();
    }

    // Venue not found
    if (!i)
    {
        cout << "-1\nError\n";
        return;
    }

    cout << i->getvenue()->noofevents() << "\n";

    for (int j = 1; j < 31; j++)
    {
        Event* curr = i->getvenue()->calendar[j];
        int count = 0;

        while (curr)
        {
            count += 1;
            curr = curr->nxt();
        }

        cout << j << " " << count << "\n";

        curr = i->getvenue()->calendar[j];
        while (curr)
        {
            cout << curr->event() << " " << curr->fromhour() << " " << curr->tohour() << "\n";
            curr = curr->nxt();
        }
    }
}


int main()
{
    string command;
    string end = "End";
    string addv = "addVenue";
    string delv = "delVenue";
    string showv = "showVenues";
    string adde = "addEvent";
    string dele = "delEvent";
    string showe = "showEvents";
    string showc = "showCalendar";

    vll* vhead = nullptr;
    vll* venueList = new vll;

    // Using do-while loop because this should execute at least one time before the user inputs anything.
    do
    {
        // Read the command from input
        getline(cin, command);

        // Trim the leading and trailing whitespaces
        command.erase(0, command.find_first_not_of(' ')); // Trim leading
        command.erase(command.find_last_not_of(' ') + 1); // Trim trailing

        // First check if the command is "End"
        if (command == end)
        {
            break;
        }

        if (command.find(addv)==0)
        {
            // Call addVenue
            char Name[1000];
            char Location[1000];
            int capacity;
            sscanf(command.c_str(), "addVenue \"%[^\"]\" \"%[^\"]\" %d", Name, Location, &capacity);
            // Convert char arrays to string
            string name = Name;
            string location = Location;
            // Object of class vll
            venueList->addVenue(vhead, name, location, capacity);
        }
        
        else if (command.find(delv)==0)
        {
            char Name[1000];
            sscanf(command.c_str(), "delVenue \"%[^\"]\"", Name);
            string name = Name;
            // Call delete venue
            venueList->delVenue(vhead, name);
        }
        
        else if (command.find(showv)==0)
        {
            // Call showVenues
            venueList->showVenues(vhead);
        }
        
        else if (command.find(adde)==0)
        {
            // Call addEvent
            char VenueName[1000];
            int date, fromHour, toHour;
            char EventName[1000];
            sscanf(command.c_str(), "addEvent \"%[^\"]\" %d %d %d \"%[^\"]\"", VenueName, &date, &fromHour, &toHour, EventName);
            string venueName = VenueName;
            string eventName = EventName;
            venueList->getvenue()->addEvent(vhead, venueName, date, fromHour, toHour, eventName);
        }
        
        else if (command.find(dele)==0)
        {
            // Call delEvent
            char VenueName[1000];
            int date, fromHour;
            char EventName[1000];
            sscanf(command.c_str(), "delEvent \"%[^\"]\" %d %d \"%[^\"]\"", VenueName, &date, &fromHour, EventName);
            string venueName = VenueName;
            string eventName = EventName;
            venueList->getvenue()->delEvent(vhead,venueName,fromHour,date,eventName);
        }
        
        else if (command.find(showe)==0)
        {
            // Call showEvents
            char VenueName[1000];
            int date;
            sscanf(command.c_str(), "showEvents \"%[^\"]\" %d", VenueName, &date);
            string venueName = VenueName;
            venueList->getvenue()->showEvents(vhead,venueName,date);
        }
        
        else if (command.find(showc)==0)
        {
            // Call showCalendar
            char VenueName[1000];
            sscanf(command.c_str(), "showCalendar \"%[^\"]\"", VenueName);
            string venueName = VenueName;
            venueList->getvenue()->showCalendar(vhead,venueName);
        }

        else
        {
            cout << "-1\nError\n";
        }

    } while (1);

    // Free memory
    venueList->freeVenues(vhead);
    delete venueList;

    return 0;
}
